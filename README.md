# Overhead Addressable RGB LEDs at CCHS

This repository holds reference material and demos for the overhead RGB leds at CCHS.

The LEDs are WS2812b.

For more specific detail on the hardware setup, please see the [hardware](hardware/) directory.

For demo and test software, please see the [software](software/) directory, subdirectories are prefixed by the model of dev board used.


## Control/Development Board Notes
The system requires a development board in the Arduino Uno form factor, with 3.3v or 5v logic levels.

Please bring your own, and be mindful that defective boards may damage the PC they are attached to if a fault occurs.

The system power supply has 0v tied to mains ground. To minimise the possibility of damage due to ground potential:

**Please ensure your laptop is not charging or attached to the wall or other peripherals.**

**Consider using a sacrificial USB hub.**

## Tested Boards

| Board Model     | Processor       | SRAM Capacity | Additional Notes |
| --------------- | ----            | ---:          | ---              |
| Arduino Uno R3  | ATmega328P      | 2KB           |                  |
| WeMos D1 R1     | ESP8266 12B/12E | 520KB         |                  |
