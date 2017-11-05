# Notagamecube

## Hardware Proof of Concept # 1
A breadboard with these components
* Teensy 3.2 with Audio Shield
* 4x4 NeoPixel Matrix on pin: 12
* Buttons on pins: 0, 1, 2
* Analog Knobs: A1 (on Audio Shield), A2, A3

## Some Libraries

[fastled library](https://github.com/FastLED/FastLED/wiki/Overview)

[Bounce button library](https://www.pjrc.com/teensy/td_libs_Bounce.html)

[notagamecube wiki](https://github.com/kuwala/notagamecube/wiki)

## Daniels Alpha Button Array Notes

### Port Expander Chip for getting multiple buttons in
MCP23017

|   | MCP23017 PINS | Teensy 3.2 PINS |
|---|---------------|-----------------|
|VDD| 9             |                 |
|VSS| 10            |                 |
|   |               |                 |
|SCL| 12            |                 |
|SDA| 13            |                 |
