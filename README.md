# Halos Controller

This program turns an Arduino into a controller for Phanteks' [Halos Digital RGB Fan Frames](http://www.phanteks.com/HalosDigital.html). It comes with ten predefined patterns, and further patterns can be created in code with the help of a number of versatile primitives. 

Provided there is a suitable source to indicate that the computer is sleeping (see below), the program renders a breathing pattern when the computer is sleeping.


## Wiring

**No warranties. The following diagram might be incorrect. I take no responsibility for any damanges caused by using the diagram and information below.**

![Wiring diagram](doc/Wiring_bb.png)

### Power

* internal USB2 header: pin 8 -> GND, pin 2 -> Vin
* alternatively connect the Arduino with a USB cable

An Arduino draws less than 50mA and a single Halos frame can draw up to about 380mA (my measurements, use at your own risk). A USB port provides at least 500mA. So, running the controller and one frame on a single USB port should be fine. 

Depending on the BIOS/UEFI settings the mainboard may  provide power to its USB headers and ports even when it's off. On my mainboard this happens when the ErP power saving mode is disabled. In that case the breathing light is also displayed even when the computer is off.

### Halos Digital frame

* grey wire from frame -> 5V
* black center wire from frame -> pin 6
* black outer wire from frame -> GND

### Controls

* push button (open/GND) -> pin 2
* sleep indicator (HIGH/LOW) -> pin 3

The reset button can be a good choice for the push button.

The PWR or MSG LED pins on the front-panel header can be used to detect whether the computer is awake/asleep. (This is what I use.) The program assumes `LOW` = asleep, `HIGH` = awake. Another option could be the 5V pin of an internal HDD power cable. I haven't tested this, though.


## Required libraries

The following libraries must be installed in the Arduino library directory. They are not included in this git repository.

- Adafruit NeoPixel: https://github.com/adafruit/Adafruit_NeoPixel
- Timer One: https://code.google.com/archive/p/arduino-timerone


## Usage

* Upload sketch to Arduino.
* Press push button to cycle through patterns.
* Press push button and hold for more than 2s to turn LEDs on and off.
