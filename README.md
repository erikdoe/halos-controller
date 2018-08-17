# Halos Controller

Arduino controller for Phanteks [Halos RGB Fan Frames](http://www.phanteks.com/Halos.html)


## Wiring

![Wiring diagram](doc/Wiring_bb.png)

### Halos frame

* grey wire from frame -> 5V
* black center wire from frame -> pin 6
* black outer wire from frame -> GND

### Controls

* push button (open / connect to GND) -> pin 2
* sleep indicator (open / connect to GND) -> pin 3

### Power

* internal USB2 header: pin 8 -> GND, pin 2 -> Vin
* alternatively connect Arduino with USB cable

## Required libraries

The following libraries must be installed in the Arduino library directory. They are not included in this git repository.

- Adafruit NeoPixel: https://github.com/adafruit/Adafruit_NeoPixel
- Timer One: https://code.google.com/archive/p/arduino-timerone


## Usage

* Upload sketch to Arduino.
* Press push button to cycle through patterns.
* Press push button and hold for more than 2s to turn LEDs on and off.
