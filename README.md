# Halos Controller

Arduino controller for Phanteks [Halos RGB Fan Frames](http://www.phanteks.com/Halos.html)


## Setup

### Wiring

#### Frame

* grey wire from frame -> 5V
* black center wire from frame -> pin 6
* black outer wire from frame -> GND

#### Controls

* pattern push button (open / connect to GND) -> pin 2
* sleep indicator (open / connect to GND) -> pin 3

#### Power

* connect Arduino with USB cable
* alternatively use internal USB2 cable: black -> GND, red -> Vin

### Required libraries

The following libraries must be installed in the Arduino library directory. The are not included in this git repository.

- Adafruit NeoPixel: https://github.com/adafruit/Adafruit_NeoPixel/
- Timer One: https://code.google.com/archive/p/arduino-timerone


## Usage

* Upload sketch to Arduino.
* Press pattern push button to cycle through selected patterns.
* Press pattern push button and hold for more than 2s to turn LEDs on and off.
