---
title: "MP14 MIDI Planter 1.4 Assembly Manual"
date: 2024-06-01
menu: main
---

## MP14 MIDI PLANTER 1.4 ASSEMBLY MANUAL

![MP14 MIDI Planter](images/3D_drawing.png)

## <a id="safety"></a>Safety

### Soldering Safety

- **Work in a Well-Ventilated Area:**
  Always solder in a well-ventilated space to avoid inhaling fumes. Use a fume extractor if possible.
- **Wear Protective Gear:**
  - Safety Glasses: Protect your eyes from solder splashes.
  - Heat-Resistant Gloves: Prevent burns from the soldering iron and hot solder.
- **Use a Proper Soldering Iron Stand:**
  Always place the soldering iron in a stand when not in use to prevent accidental burns and fire hazards.
- **Avoid Flammable Materials:**
  Keep your work area clear of flammable materials. Ensure the soldering iron does not come into contact with paper, plastic, or other flammable objects.
- **Handle Soldering Iron Carefully:**
  Never touch the tip of the soldering iron. Be mindful of where you place your hands to avoid accidental burns.
- **Clean Soldering Tip Regularly:**
  Use a damp sponge or brass wire cleaner to keep the soldering tip clean, ensuring efficient and safe soldering.
- **Avoid Inhaling Solder Fumes:**
  Use lead-free solder to minimize health risks. Ensure good ventilation or use a fume extractor.
- **Properly Dispose of Waste:**
  Dispose of used solder and electronic components according to local regulations. Do not throw them in regular trash.

### Sanding Copper Wire Safety

- **Wear Protective Gear:**
  - Safety Glasses: Protect your eyes from copper dust and particles.
  - Dust Mask: Prevent inhalation of copper dust.
  - Gloves: Protect your hands from the sanding skin with sandpaper.
- **Secure the Wire (Optional):**
  Use a vise or clamp to securely hold the copper wire in place while sanding to prevent slipping and accidental injury.
- **Use Appropriate Sanding Tools:**
  Use fine-grit sandpaper to remove the insulator layer. Avoid using power tools, as they can remove too much material and generate excessive dust.
- **Work in a Well-Ventilated Area:**
  Sand in a well-ventilated space to ensure you are not breathing in copper dust. An outdoor workspace is ideal.
- **Clean Up Properly:**
  After sanding, clean the work area thoroughly to remove copper dust. Wash your hands and any exposed skin to avoid prolonged contact with copper particles.
- **Avoid Excessive Force:**
  Apply gentle, even pressure while sanding to avoid damaging the wire and to maintain control of the sanding process.
- **Store Tools Safely:**
  After use, store sandpaper and other tools in a safe place to prevent accidental contact.

### General Safety Tips

- **Read Instructions Carefully:**
  Follow all steps as outlined in the manual. Do not skip steps or take shortcuts.
- **Keep a First Aid Kit Handy:**
  In case of minor injuries, have a first aid kit available for immediate treatment.
- **Seek Help If Needed:**

## <a id="materials-list"></a>Materials List

### Electronics

- PCB Board
- Adafruit Trinket M0
- Headers for Adafruit Trinket M0
- 555 timer
- 555 timer connector (Optional)
- 220 ohm resistor (x3)
- 1 uF Ceramic Capacitor
- 47 uF Ceramic Capacitor
- 1 nF Ceramic Capacitor
- TRS (PJ320A)
- Tactile Switch [PART NUMBER HERE]
- Potentiometer [PART NUMBER HERE]
- 0.9 mm copper coil
- M4 Screws [LENGTH HERE] (x6)
- M4 Brass Nut Inserts (x6)
- 3.5mm Auxiliary Jack Male
- USB to Micro USB

### 3D Model

- Planter
- PCB Bracket
- Hood
- Shell
- Pool
- Button
- Knob

### Tools Required

- 3D Printer
- Soldering Iron
- Solder
- Multi-meter

### Recommended Software

- Arduino IDE
- Visual Studio Code

## <a id="printed-circuit-board"></a>Printed Circuit Board

1. Snap off 2 sets of 5 header pins that come with the trinket M0. Solder the headers onto the board where the trinket M0 is outlined. The longer sides of the pins should face up.
   ![pin_solo](images/2_Pins_solo.png)
   ![pin_bottom](images/2_Pin_bottom.png)
   ![pin_top](images/2_Pin_top.png)

2. Solder the trinket M0 onto the headers.
   ![trink1](images/2_trinket1.png)
   ![trink2](images/2_trinket2.png)

3. Bend all the pins on the tactile switch so that they face outward instead of down. Using a wire cutter or scissors, cut the pins so they fit onto the U5 area on the PCB.
   ![tactile](images/3_tactile.png)

4. Solder the four pins onto the four pin areas of U5, then solder the tactile switch onto U5.
   ![tac1](images/4_tactile1.png)
   ![tac2](images/4_tactile2.png)

5. Place the potentiometer into the R1 placement on the PCB. It should fit snug. Solder the Potentiometer onto R1.
   ![pot1](images/5_pot.png)
   ![pot2](images/5_pot2.png)

6. Solder the 555 timer onto U6.
   ![timer](images/6_timer.png)
7. Solder the resistors and capacitors:
   - Resistor R5 = 220 ohm
   - Resistor R6 = 220 ohm
   - Resistor R4 = 220 ohm
   - Capacitor C2 = 1uF
   - Capacitor C1 = 47uF
   - Capacitor C3 = 4.7nF
8. Solder the TRS connector to U5.

### Copper Probes

1. Cut two pieces of the 0.9mm copper coil at around 72 millimeters.
   ![probes](images/probes.png)

2. Sand off the insulating layer of the coils with sandpaper. Wear gloves to protect hands from sandpaper.
3. Set your digital multimeter to continuity mode. To test, touch the multimeter probes to the ends of the copper wire one at a time. If the multimeter beeps or buzzes, it indicates successful end-to-end continuity. If there is no sound, it means the insulating layer on the wire needs to be sanded more thoroughly. Test multiple areas of the probe to make sure the insulating layer is evenly sanded off.
4. Solder the two copper coil strips to U7 and U8 on the PCB.
   ![sand](images/sanded_probes.png)

### Power and Ground (Optional)

The GND and PWR connectors are optional for a battery pack. The recommended voltage for the circuit is 3.5V. Two CR2032 cell batteries were tested and worked well.

## <a id="mp14-firmware-setup-guide"></a>MP14 Firmware Setup Guide

### [See on Github](https://github.com/kalabashlabs/Midi-Planter/tree/92a48ba50bc03f4589a67e164f653b5d439beccf/firmware)

1. **Install Arduino IDE**
   - **Download and Install:**
     Go to the [Arduino IDE download page](https://www.arduino.cc/en/software) and download the latest version for your operating system.
   - **Install the Arduino IDE** following the on-screen instructions.
2. **Install Adafruit Board Support Package**

   - **Open Arduino IDE:**
     Launch the Arduino IDE on your computer.
   - **Add Board Manager URL:**
     Go to Arduino > Preferences on macOS or File > Preferences on Windows. In the Additional Board Manager URLs field, add the following URL: `https://adafruit.github.io/arduino-board-index/package_adafruit_index.json`
     Click OK.
   - **Open Boards Manager:**
     Go to Tools > Board > Boards Manager.
   - **Install Adafruit SAMD Boards:**
     In the Boards Manager, search for Adafruit SAMD. Click Install on Adafruit SAMD Boards by Adafruit.

3. **Connect Adafruit Trinket M0**

   - **Connect the Trinket M0:**
     Use a USB cable to connect the Trinket M0 to your computer.
   - **Select the Board:**
     Go to Tools > Board. Select Adafruit Trinket M0.
   - **Select the Port:**
     Go to Tools > Port. Select the port that corresponds to your Trinket M0. It should look something like COMx on Windows or /dev/cu.usbmodemxxxx on macOS.

4. **Uploading Source Code**

   - **Open or Create a Sketch:**
     Create a new one.
   - **Copy and Paste the [MP14.14 firmware](https://github.com/kalabashlabs/Midi-Planter/blob/92a48ba50bc03f4589a67e164f653b5d439beccf/firmware/mp14.14_base/mp14.14_base.ino) to the Arduino sketch.**
   - **Compile the Sketch:**
     Click the checkmark icon to verify (compile) your sketch.
   - **Upload the Sketch:**
     Click the right arrow icon to upload the sketch to the Trinket M0. If the upload is successful, the on-board LED should start blinking.

5. **Troubleshooting**
   - **Driver Issues (Windows):**
     If your Trinket M0 is not recognized, you might need to install the USB driver. Follow the instructions on the Adafruit website to install the necessary drivers.
   - **Resetting the Trinket M0:**
     If the upload fails, double-click the reset button on the Trinket M0 to enter bootloader mode (the red LED should pulse). Try uploading the sketch again.

## <a id="3d-printed-components"></a>3D Printed Components

- PCB Bracket
- Planter
- Shell
- Hood
- Button & Knob
- Drip Tray

### Print Settings

![brack](images/bracket.png)

1. **PCB BRACKET**

   - Material: PLA
   - Layer Height: 0.3 mm
   - Sparse Infill Density: 30%
   - Support: No

![planter](images/planter.png) 2. **PLANTER**

- Material: PLA
- Layer Height: 0.3 mm
- Sparse Infill Density: 30%
- Support: No

![shell](images/shell.png) 3. **SHELL**

- Material: PLA
- Layer Height: 0.3 mm
- Sparse Infill Density: 30%
- Support: No

![hood](images/hood.png) 4. **HOOD**

- Material: PLA
- Layer Height: 0.3 mm
- Sparse Infill Density: 30%
- Support: No

![knob](images/knobs.png) 5. **KNOB & BUTTON**

- Material: PLA
- Layer Height: 0.3 mm
- Sparse Infill Density: 30%
- Support: No

![drip](images/pool.png) 6. **DRIP TRAY**

- Material: PLA
- Layer Height: 0.3 mm
- Sparse Infill Density: 30%
- Support: No

## <a id="component-assembly"></a>Component Assembly

1. Place the PCB board inside the bracket.
   ![brak_fro](images/bracket_pcb_front.png)
   ![brak_bac](images/bracket_pcb_back.png)

2. Place four of the M4 brass inserts in the four holes on the front side of the planter. The smaller diameter side of the brass inserts should go inside the hole first.
   ![plan_soc](images/planter_sockets.png)
   ![plan_soc2](images/planter_sockets_2.png)

3. With a soldering iron, hold the tip of the iron onto one brass insert and gently wait for the insert to slide into place. The heat of the iron contacting the insert will expand the PLA material so the insert will fit snug into the hole. Do this for all four holes.
   ![plan_soc_s](images/planter_socket_screw.png)

4. Use the same technique from step 2 to place the inserts into the two holes at the bottom of the planter.
   ![plan_soc_b](images/planter_socket_bottom.png)

5. Line the bracket up with the front of the planter and place four M4 screws through the four holes of the PCB Bracket. The copper wires go through the two small holes on the front of the planter. Then, screw the four M4 screws to the planter with a screwdriver.
   ![brac_pla](images/bracket_planter.png)
   ![screw_brac](images/screw_bracket.png)

6. Place the planter inside of the shell. The back of the planter should slide against the inside back of the shell so the potentiometer can fit. Once the bottom of the planter makes contact with the shell, slide the planter forward to fit into the groove while the potentiometer sticks out of the front hole.
   ![slide_shell](images/slide_planter_shell.png)

7. Using pliers or scissors, remove excess PLA that is blocking the two holes at the bottom of the shell.
8. Place the planter and shell upside down and screw two M4 screws into the two holes with a screwdriver.
   ![screw_shell](images/screw_shell.png)

9. Slide the knob onto the area of the potentiometer sticking out of the shell. Snap the button onto the tactile switch from the front of the shell. Make sure the square on the button is aligned with the square on the tactile switch.
   ![button_planter](images/button_planter.png)

10. Place the Shell onto the drip tray.
    ![pool_bottom](images/pool_bottom.png)

11. Flip the planter over and place the hood onto the top.
    ![finish](images/finish.png)
