# MP14 Firmware Setup Guide

[See on Github](https://github.com/kalabashlabs/Midi-Planter/tree/abc15cf49541974a32a134d85bc21599ec5fb134/firmware)

## 1. Install Arduino IDE

### Download and Install:

- Go to the [Arduino IDE download page](https://www.arduino.cc/en/software) and download the latest version for your operating system.
- Install the Arduino IDE following the on-screen instructions.

## 2. Install Adafruit Board Support Package

### Open Arduino IDE:

- Launch the Arduino IDE on your computer.

### Add Board Manager URL:

- Go to **Arduino > Preferences** on macOS or **File > Preferences** on Windows.
- In the **Additional Board Manager URLs** field, add the following URL: `https://adafruit.github.io/arduino-board-index/package_adafruit_index.json`
- Click **OK**.

### Open Boards Manager:

- Go to **Tools > Board > Boards Manager**.

### Install Adafruit SAMD Boards:

- In the Boards Manager, search for **Adafruit SAMD**.
- Click **Install** on **Adafruit SAMD Boards** by Adafruit.

## 3. Connect Adafruit Trinket M0

### Connect the Trinket M0:

- Use a USB cable to connect the Trinket M0 to your computer.

### Select the Board:

- Go to **Tools > Board**.
- Select **Adafruit Trinket M0**.

### Select the Port:

- Go to **Tools > Port**.
- Select the port that corresponds to your Trinket M0. It should look something like **COMx** on Windows or **/dev/cu.usbmodemxxxx** on macOS.

## 4. Uploading Source Code

### Open or Create a Sketch:

- Create a new one.
- Copy and paste the MP14.14 code to the Arduino sketch.

### Compile the Sketch:

- Click the checkmark icon to verify (compile) your sketch.

### Upload the Sketch:

- Click the right arrow icon to upload the sketch to the Trinket M0.
- If the upload is successful, the on-board LED should start blinking.

## 5. Troubleshooting

### Driver Issues (Windows):

- If your Trinket M0 is not recognized, you might need to install the USB driver. Follow the instructions on the Adafruit website to install the necessary drivers.

### Resetting the Trinket M0:

- If the upload fails, double-click the reset button on the Trinket M0 to enter bootloader mode (the red LED should pulse). Try uploading the sketch again.
