# Hardware

This section describes specifics of the platform hardware.

## LEDs

There are 6 individually driven strips/lines, each as follows:

Strips contain WS2812b addressable RGB LEDs with the follow specs:

* LED Spacing: 30mm centre-centre
* Length:	5m
* N LEDs: 170

## Power

Power is supplied as 5v from an ATX power supply, where the main system supply has a current shunt and voltage monitor.

Each strip is fed by a cable, each cable is individually fused.

### Observations from stress testing the power system

Using the following sketch:
* esp32_stress_test.ino

Observations of current at maximum brightness of each colour channel in isolation:
* Red:	Each strip of 170 LEDs draws: (2lines-1line) = (6.1A-3.7A) = 2.4A, System maximum red total = 2.4A x 6 = 14.4A
* Grn:	Each strip of 170 LEDs draws: (2lines-1line) = (4.4A-2.7A) = 1.7A, System maximum grn total = 1.7A x 6 = 10.2A
* Blu:	Each strip of 170 LEDs draws: (2lines-1line) = (5.0A-3.0A) = 2.0A, System maximum blu total = 2.0A x 6 = 12.0A

Derived WS2812b characteristics based on the tests in this program:
* Ired(max) = 14mA
* Igrn(max) = 10mA
* Iblu(max) = 12mA

Theoretical max current consumption per WD2812b, based on ideal supply/conductors presenting no voltage drop at brightness=255, colour=white
* Iwht(max) = SUM(r,g,b) = 36mA
  
Theoretical max system current:
* 0.036 x 170 x 6 = 36.7A
  
This does not occur, reasons:
* Voltage drop in supply cables
* Voltage drop along length of WS2812B strip (causing colour fade towards orange at far end)
* Voltage sag of power supply

## Data

Data is carried via coax from the controlling dev board to the LEDs.

Buffered line drivers with 3.3v inputs, 5v tolerant, are used to drive the coax cables.

The line drivers and LEDs are both protected against faults that may occur due to the presence of a signal with absent power (as may occur when main power is off, yet the development board is running off a USB port).


## Control/Development Board

A modified Arduino Uno compatible shield is used to supply 12v power to the development board, and to carry CMOS signals from the development board to the line drivers.

The system will work with 5v and 3.3v logic levels.

Do not supply additional external power or 5v to the dev board, as this may conflict with the existing supply. 

USB is an exception due to this supply handover being safely handled by a MOSFET, cheap dev boards may not adequate protection (see the approved table below). 

If in doubt, use an external USB hub that you wouldn't mind sacrificing to save your computer.

When connecting to the dev board using a USB cable, ensure the laptop is not connected to mains power or other wired peripherals to minimise the risk of damage that may occur from ground potentials.

Available SRAM in the development board is a limiting factor, Arduino Uno is unable to concurrently hold the RGB colour space for all pixels. However one strip length can be programmed and each strip can be loaded from that memory object, so all strips behave identically.

| OverheadRGB | Arduino |
| ----------- | ------- |
| 12v 1A      | VIN     |
| 0v          |	GND     |
| Line 1 data | D2      |
| Line 2 data | D3      |
| Line 3 data | D4      |
| Line 4 data | D5      |
| Line 5 data | D6      |
| Line 6 data | D7      |
