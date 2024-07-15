//Uncomment for debug mode. Use the serial monitor to see the debug values printed
#define DEBUG_MODE (1)

// Import libraries
#include "MIDIUSB.h"
#include <Arduino.h>
#include "MIDI.h"
#include <Adafruit_DotStar.h>

// Setup midi instance
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

// Setup main led on trinket M0
#define NUMPIXELS 1  // Number of LEDs in strip
#define DATAPIN 7
#define CLOCKPIN 8
Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

// Setup color mapping for different musical scales
#define COLOR_MAJOR strip.gamma32(strip.ColorHSV(0, 190, 190))         // Red
#define COLOR_MINOR strip.gamma32(strip.ColorHSV(43690, 190, 190))     // Blue
#define COLOR_DIAMINOR strip.gamma32(strip.ColorHSV(60000, 190, 190))  // Magenta
#define COLOR_INDIAN strip.gamma32(strip.ColorHSV(5500, 180, 180))     // Orange
#define COLOR_CHROMATIC 0                                              // Special case for Rainbow

// Setup color mapping for diffent modes
#define COLOR_WHITE strip.gamma32(strip.ColorHSV(0, 0, 100))
#define COLOR_GREEN strip.gamma32(strip.ColorHSV(20000, 190, 100))
#define COLOR_YELLOW strip.gamma32(strip.ColorHSV(11000, 190, 190))

// Setup blicking mode for LED
unsigned long lastUpdate = 0;            // Time of the last LED update
const unsigned long pulseInterval = 30;  // Time between brightness changes during the pulse
float brightness = 0.0;                  // Current brightness level
bool increasing = true;                  // Whether the brightness is increasing or decreasing
const float maxBrightness = 0.05;        // Maximum brightness level
const float easeOutSpeed = 0.5;          // Speed of the ease-out effect
bool isNoteOn = false;

// Setup midi pause feature
unsigned long buttonPressStartTime = 0;
bool longPressTriggered = false;
bool shortPressTriggered = false;
bool isPaused = false;
const unsigned long longPressDuration = 2000;  // 2000 milliseconds = 3 seconds

// Setup pinouts of trinket M0
const byte interruptPin = A0;  //galvanometer input
const byte knobPin = A1;       //potentiometer (knob) input
const byte buttonPin = A2;
const byte ledPin = 13;

// Setup LED state
enum LedState { LED_HOME,
                LED_SCALE,
                };
LedState currentLedState = LED_HOME;

// Setup Knob state
enum KnobMode { KNOB_HOME,
                KNOB_SCALE,
                };
KnobMode currKnobMode = KNOB_HOME;
bool modeChanged = false;

const byte samplesize = 10;              //set sample array size
const byte analysize = samplesize - 1;  //trim for analysis array

byte timeout = 0;
int value = 0;
int prevValue = 0;

volatile unsigned long sample_time;  //sampling timer
volatile byte sample_idx = 0;
volatile unsigned long samples[samplesize];

unsigned long previousMillis = 0;
unsigned long currentMillis = 1;

float threshold = 1.0;   //2.3;  //change threshold multiplier
float threshMin = 1.0;  //scaling threshold min
float threshMax = 1.0;  //scaling threshold max
float knobMin = 1;
float knobMax = 1024;

//MIDI note config
const byte polyphony = 5;  //above 8 notes may run out of ram
int channel = 1;           //setting channel to 11 or 12 often helps simply computer midi routing setups
int noteMin = 48;          //C3  - keyboard note minimum
int noteMax = 96;          //C7  - keyboard note maximum

//set scaled values, sorted array, first element scale length
const int scaleCount = 5;
const int scaleLen = 13;  //maximum scale length plus 1 for 'used length'
int currScale = 0;        //current scale, default Chrom
int note_scales[scaleCount][scaleLen] = {
  { 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 },  //Chromatic
  { 7, 1, 3, 5, 6, 8, 10, 12 },                   //Major
  { 7, 1, 3, 4, 6, 8, 9, 11 },                    //DiaMinor
  { 7, 1, 2, 2, 5, 6, 9, 11 },                    //Indian
  { 7, 1, 3, 4, 6, 8, 9, 11 }                     //Minor
};
int root = 0;  //initialize for root, pitch shifting

int noteIndex = 0;
typedef struct _MIDImessage {  //build structure for Note and Control MIDImessages
  unsigned int type;
  int value;
  int velocity;
  long duration;
  long period;
  int channel;
} MIDImessage;
MIDImessage noteArray[polyphony];
MIDImessage controlMessage;  //manage MIDImessage data for Control Message (CV out)
byte controlNumber = 80;     //set to mappable control, low values may interfere with other soft synth controls.

//Helper function to map one range onto another
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  strip.begin();  // Initialize pins for output
  strip.show();   // Turn all LEDs off ASAP

  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial1.begin(31250);

#ifdef DEBUG_MODE
  Serial.begin(31250);
#endif
  controlMessage.value = 0;  //begin CV at 0

  pinMode(knobPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(buttonPin), button_isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPin), sensor_isr, RISING);
}

volatile bool buttonPressed = false;
volatile bool buttonReleased = false;

void button_isr() {
  if (digitalRead(buttonPin) == LOW) {  // Button is pressed
    if (buttonPressStartTime == 0) {
      buttonPressStartTime = millis();
      longPressTriggered = false;
    }
  } else {  // Button is released
    if (millis() - buttonPressStartTime < longPressDuration) {
      shortPressTriggered = true;
    }
    buttonPressStartTime = 0;
  }
}

void changeMode() {
  switch (currKnobMode) {
    case KNOB_SCALE:
      currKnobMode = KNOB_HOME;
      currentLedState = LED_HOME;
      break;
    case KNOB_HOME:
      currKnobMode = KNOB_SCALE;
      currentLedState = LED_SCALE;
      break;
  }
  modeChanged = true;
#ifdef DEBUG_MODE
  Serial.print("Mode: ");
  Serial.println(currKnobMode);
#endif
}

void loop() {
  // Set time to milliseconds
  currentMillis = millis();

  // Check if the button is currently pressed and held for 3 seconds
  if (buttonPressStartTime > 0 && (currentMillis - buttonPressStartTime >= longPressDuration) && !longPressTriggered) {
    longPressTriggered = true;  // Prevent multiple triggers

    // long press action
    strip.setPixelColor(0, strip.Color(255, 0, 0));  // Set LED to red
    strip.show();
    delay(500);                                    // Red LED indication for 500 ms
    strip.setPixelColor(0, strip.Color(0, 0, 0));  // Turn off LED
    strip.show();

    isPaused = !isPaused;
    //Serial.println("long press");
  }
  // Short press action
  if (shortPressTriggered) {
    shortPressTriggered = false;
    changeMode();
    Serial.println("short press");
  }

  if (!isPaused) {

    if (sample_idx >= samplesize) {
      analyzeSample();
    }

    // Switch colors based on mode
    switch (currentLedState) {
      case LED_HOME:
        blinkLed(COLOR_WHITE);
        break;
      case LED_SCALE:
        displayScaleColor();
        break;
    }

    // Pulse MIDI notes on HOME
    if (currentLedState == LED_HOME) {
      if (isNoteOn) {
        blinkLed(currentLedState == LED_HOME ? COLOR_WHITE : COLOR_GREEN);
      } else {
        // Keep the LED off or at a steady state when no note is playing
        strip.setPixelColor(0, currentLedState == LED_HOME ? COLOR_WHITE : COLOR_GREEN);
        strip.show();
      }
    } else if (currentLedState == LED_SCALE) {
      displayScaleColor();  // As implemented previously
    }

    if (modeChanged) {
      modeChanged = false;
    }

    checkKnob();
    checkNotes();
    checkControl();
  }
}

//interrupt timing sample array
void sensor_isr() {
  if (sample_idx < samplesize) {
    samples[sample_idx] = micros() - sample_time;
    sample_time = samples[sample_idx] + sample_time;  //rebuild micros() value w/o recalling
    sample_idx += 1;
  }
}

void displayScaleColor() {
  switch (currScale) {
    case 0:  // Chromatic
      animateRainbow(300);
      break;
    case 1:  // Major
      strip.setPixelColor(0, COLOR_MAJOR);
      strip.show();
      break;
    case 2:  // Minor
      strip.setPixelColor(0, COLOR_MINOR);
      strip.show();
      break;
    case 3:  // Diaminor
      strip.setPixelColor(0, COLOR_DIAMINOR);
      strip.show();
      break;
    case 4:  // Indian
      strip.setPixelColor(0, COLOR_INDIAN);
      strip.show();
      break;
  }
}

void analyzeSample() {
  unsigned long averg = 0;
  unsigned long maxim = 0;
  unsigned long minim = 100000;
  float stdevi = 0;
  unsigned long delta = 0;
  byte change = 0;

  if (sample_idx == samplesize) {  //array is full
    unsigned long sampanalysis[analysize];
    for (byte i = 0; i < analysize; i++) {
      //skip first element in the array
      sampanalysis[i] = samples[i + 1];  //load analysis table (due to volatile)

      if (sampanalysis[i] > maxim) { maxim = sampanalysis[i]; }
      if (sampanalysis[i] < minim) { minim = sampanalysis[i]; }
      averg += sampanalysis[i];
      stdevi += sampanalysis[i] * sampanalysis[i];  //prep stdevi
    }

    averg = averg / analysize;
    stdevi = sqrt(stdevi / analysize - averg * averg);  //calculate stdevu
    if (stdevi < 1) { stdevi = 1.0; }                   //min stdevi of 1
    delta = maxim - minim;

#ifdef DEBUG_MODE
    Serial.println(delta);
#endif

    //**********perform change detection
    if (delta > (stdevi * threshold)) {
      change = 1;
    }
    //*********

    if (change) {                                             // set note and control vector
      int dur = 150 + (map(delta % 127, 1, 127, 100, 2500));  //length of note
      int ramp = 3 + (dur % 100);                             //control slide rate, min 25 (or 3 ;)

      //set scaling, root key, note
      int note = map(averg % 127, 1, 127, noteMin, noteMax);  //derive note, min and max note
      note = scaleNote(note, root);                           //scale the note

      setNote(note, 100, dur, channel);

      //derive control parameters and set
      setControl(controlNumber, controlMessage.value, delta % 127, ramp);  //set the ramp rate for the control
    }

    //reset array for next sample
    sample_idx = 0;
  }
}

void blinkLed(uint32_t color) {
  unsigned long currentMillis = millis();

  if (isNoteOn) {
    if (currentMillis - lastUpdate > pulseInterval) {
      lastUpdate = currentMillis;

      if (increasing) {
        brightness += 0.1;
        if (brightness >= maxBrightness) {
          increasing = false;
        }
      } else {
        brightness -= 0.1;
        if (brightness <= 0) {
          increasing = true;
        }
      }
    }
  } else {
    if (brightness > 0) {
      brightness -= easeOutSpeed;
    }
  }

  // Apply the calculated brightness to the color
  int red = (int)(((color >> 16) & 0xFF) * brightness);
  int green = (int)(((color >> 8) & 0xFF) * brightness);
  int blue = (int)((color & 0xFF) * brightness);

  strip.setPixelColor(0, strip.Color(red, green, blue));
  strip.show();
}


int scaleSearch(int note) {
  int scalesize = (note_scales[currScale][0]);
  for (byte i = 1; i < scalesize; i++) {
    if (note == note_scales[currScale][i]) {
      return note;
    } else {
      if (note < note_scales[currScale][i]) { return note_scales[currScale][i]; }
    }  //highest scale value less than or equal to note
    //otherwise continue search
  }
  //didn't find note and didn't pass note value, uh oh!
  return 6;  //give arbitrary value rather than fail
}

int scaleNote(int note, int root) {
  //input note mod 12 for scaling, note/12 octave
  //search array for nearest note, return scaled*octave
  int scaled = note % 12;
  int octave = note / 12;
  //search entire array and return closest scaled note
  scaled = scaleSearch(scaled);
  scaled = (scaled + (12 * octave)) + root;  //apply octave and root
  return scaled;
}

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void MIDINoteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = { 0x09, 0x90 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOn);
  MIDI.sendNoteOn(pitch, velocity, channel);
  isNoteOn = true;
}

void MIDINoteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = { 0x08, 0x80 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOff);
  MIDI.sendNoteOff(pitch, velocity, channel);
  isNoteOn = false;
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void MIDIControlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
}

void setNote(int value, int velocity, long duration, int notechannel) {
  //find available note in array (velocity = 0);
  for (int i = 0; i < polyphony; i++) {
    if (!noteArray[i].velocity) {
      //if velocity is 0, replace note in array
      noteArray[i].type = 0;
      noteArray[i].value = value;
      noteArray[i].velocity = velocity;
      noteArray[i].duration = currentMillis + duration;
      noteArray[i].channel = notechannel;

      MIDINoteOn(channel, value, velocity);
      break;
    }
  }
}

void checkKnob() {
  int val = analogRead(knobPin);
  if (currKnobMode == KNOB_SCALE) {
    currScale = map(val, knobMin, knobMax, 0, scaleCount);
    setScaleColor(currScale);
  }
}

void setScaleColor(int scale) {
  uint32_t color;
  switch (scale) {
    case 0:
      color = COLOR_CHROMATIC;
      break;
    case 1:
      color = COLOR_MAJOR;
      break;
    case 2:
      color = COLOR_MINOR;
      break;
    case 3:
      color = COLOR_DIAMINOR;
      break;
    case 4:
      color = COLOR_INDIAN;
      break;
  }

  if (color == COLOR_CHROMATIC) {
    // Logic for rainbow color
    animateRainbow(300);
  } else {
    strip.setPixelColor(0, color);
    strip.show();
  }
}

void animateRainbow(unsigned long interval) {
  static unsigned long lastChangeTime = 0;
  static int colorIndex = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - lastChangeTime >= interval) {
    lastChangeTime = currentMillis;
    uint32_t colors[] = { COLOR_MAJOR, COLOR_MINOR, COLOR_DIAMINOR, COLOR_INDIAN };
    strip.setPixelColor(0, colors[colorIndex]);
    colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));
    strip.show();
  }
}

void checkNotes() {
  //send noteOff for all notes with non-zero velocity and expired duration
  for (int i = 0; i < polyphony; i++) {
    if ((noteArray[i].velocity) && (noteArray[i].duration <= currentMillis)) {
      MIDINoteOff(channel, noteArray[i].value, 0);
      noteArray[i].velocity = 0;
    }
  }
}

void setControl(int type, int value, int velocity, long duration) {
  controlMessage.type = type;
  controlMessage.value = value;
  controlMessage.velocity = velocity;
  controlMessage.period = duration;
  controlMessage.duration = currentMillis + duration;  //schedule for update cycle
}

void checkControl() {
  //need to make this a smooth slide transition, using high precision
  //distance is current minus goal
  signed int distance = controlMessage.velocity - controlMessage.value;
  //if still sliding
  if ((distance != 0) && (currentMillis > controlMessage.duration)) {
    controlMessage.duration = currentMillis + controlMessage.period;  //extend duration
    if (distance > 0) {
      controlMessage.value += 1;
    } else {
      controlMessage.value -= 1;
    }

    //send MIDI control message after ramp duration expires, on each increment
    MIDIControlChange(channel, controlMessage.type, controlMessage.value);
  }
}
