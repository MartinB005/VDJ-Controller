# 🎛️ VDJ-Controller

A custom DIY hardware DJ controller project designed to interface seamlessly with **VirtualDJ** via MIDI protocols. 

---

##  Features

* **Custom Hardware Integration:** Built using microcontrollers (Arduino) and low-cost electronic components (potentiometers, buttons, rotary encoders, etc.).
* **MIDI Communication:** Translates physical inputs (knobs, sliders, button presses) into standard MIDI messages.
* **VirtualDJ Mapping:** Includes custom mapper configuration files to easily link physical hardware controls to VirtualDJ software functions (decks, effects, EQ, volume faders, loop controls, etc.).
* **Cost-Effective & Open-Source:** An affordable alternative to commercial DJ controllers, allowing complete customization of layout and functionality.

---
## Usage
- Move driver file (driver.xml) to /Devices in VirtualDJ data folder.
- Move mapper file (vdj-controller.xml to /Mappers in VirtualDJ data folder.
- Connect MCU and your PC should seen it as MIDI device
- In VDJ, go to Settings -> Controllers, you should see the controller and adjust mapping if you want
- Enjoy MIXING
