# MP14 Firmware Setup Guide w/Arduino Trinket M0

## Introduction
This guide will walk you through the steps to connect an Adafruit Trinket M0 to the Arduino IDE on both macOS and Windows. You'll learn how to configure the board and upload the firmware to it.

## Requirements
- Assembled PCB w/Adafruit Trinket M0 [LINK]
- Micro USB cable
- Computer (macOS or Windows)
- Arduino IDE (latest version)

## Step-by-Step Guide

### 1. Install Arduino IDE
1. **Download and Install:**
   - Go to the [Arduino IDE download page](https://www.arduino.cc/en/software) and download the latest version for your operating system.
   - Install the Arduino IDE following the on-screen instructions.

### 2. Install Adafruit Board Support Package
1. **Open Arduino IDE:**
   - Launch the Arduino IDE on your computer.
   
2. **Add Board Manager URL:**
   - Go to `Arduino` > `Preferences` on macOS or `File` > `Preferences` on Windows.
   - In the `Additional Board Manager URLs` field, add the following URL: 
     ```
     https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
     ```
   - Click `OK`.

3. **Open Boards Manager:**
   - Go to `Tools` > `Board` > `Boards Manager`.

4. **Install Adafruit SAMD Boards:**
   - In the Boards Manager, search for `Adafruit SAMD`.
   - Click `Install` on `Adafruit SAMD Boards` by Adafruit.

### 3. Connect Adafruit Trinket M0
1. **Connect the Trinket M0:**
   - Use a USB cable to connect the Trinket M0 to your computer.

2. **Select the Board:**
   - Go to `Tools` > `Board`.
   - Select `Adafruit Trinket M0`.

3. **Select the Port:**
   - Go to `Tools` > `Port`.
   - Select the port that corresponds to your Trinket M0. It should look something like `COMx` on Windows or `/dev/cu.usbmodemxxxx` on macOS.

### 4. Uploading Source Code
1. **Open or Create a Sketch:**
   - Create a new one.
   - Copy and Paste the MP14.14 code to the Arduino sketch

2. **Compile the Sketch:**
   - Click the checkmark icon to verify (compile) your sketch.

3. **Upload the Sketch:**
   - Click the right arrow icon to upload the sketch to the Trinket M0.
   - If the upload is successful, the on-board LED should start blinking.

### 5. Troubleshooting
1. **Driver Issues (Windows):**
   - If your Trinket M0 is not recognized, you might need to install the USB driver. Follow the instructions on the Adafruit website to install the necessary drivers.

2. **Resetting the Trinket M0:**
   - If the upload fails, double-click the reset button on the Trinket M0 to enter bootloader mode (the red LED should pulse). Try uploading the sketch again.

### Conclusion
You have successfully set up your Adafruit Trinket M0 with the Arduino IDE and uploaded your first sketch. 

For more information and troubleshooting, refer to the [Adafruit Learning System](https://learn.adafruit.com/adafruit-trinket-m0-circuitpython-arduino) or the [Arduino Forums](https://forum.arduino.cc/).

