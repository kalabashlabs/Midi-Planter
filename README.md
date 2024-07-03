# Midi-Planter 1.4
Midi Planter is a midi instrument that senses bio-signals in the roots and the soil of a house plant. The ranges of the bio-signals are mapped to midi notes ands sent to a computer, smartphone or analog synthesizer. 

![Sketch of Midi-Planter](assets/3D_drawing.png)

## Introduction
This repository contains the source code and documentation for a MIDI device that measures bio signals from plant roots and soil, converting them into MIDI messages using a custom PCB and an Adafruit Trinket M0.

## Features & Compatibility
- **Musical Scales:** Bio signals that are measured can be mapped to Chromatic, Major, Minor, Indian and Dorian.
- **MIDI OFF:** Pause midi messages sent to external device.
- **Open Source:** Designed specifically community collaboration.
- **Adafruit Trinket M0:** Utilizes the Adafruit Trinket M0 for processing and communication.
- **Connectability:** Connect to any instrument that accepts midi-In.  

## Getting Started

### Prerequisites
- PCB Setup Guide
- USB cable
- 3.5mm TRS Cable
- Arduino IDE

### Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/kalabashlabs/Midi-Planter.git
   cd Midi-Planter

### Setup Firmware
Follow the [MP14 Firmware Setup Guide w/Arduino Trinket M0](https://github.com/kalabashlabs/Midi-Planter/tree/9c78c0cd12679d66a3d97c33ffdac4fd24a90cb2/firmware) Guide
