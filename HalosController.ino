#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include <TimerOne.h>
#include "Patterns.h"


// Address in EEPROM where to store the selected pattern index
#define PATTERN_IDX_EEPROM_ADDR 67

// Variable that holds the index of the currently selected pattern
volatile uint8_t patternIdx = 0;

// Variable to turn frame on and off (off = paused)
volatile uint8_t isPaused = 0;

// Variable to remember last button interrupt time
volatile unsigned long lastButtonIntr = 0;

// Object for the fan frame, which is compatible with NeoPixel
Adafruit_NeoPixel frame;


// Arduino setup and loop functions

void setup() {
  Serial.begin(115200); 

  // Parameter 1 = number of pixels in strip
  // Parameter 2 = Arduino pin number (most are valid)
  // Parameter 3 = pixel type flags, add together as needed:
  frame = Adafruit_NeoPixel(30, 6, NEO_GRB + NEO_KHZ800);
  frame.begin();
  frame.show();
  
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT);

  attachInterrupt(digitalPinToInterrupt(2), buttonIntr, CHANGE);
  Timer1.initialize(2000000L);

  patternIdx = EEPROM.read(PATTERN_IDX_EEPROM_ADDR);
  if (patternIdx >= numPatterns) {
    patternIdx = 0;
  }
}


void loop() {
  if (!isPaused) {
    if (isSleeping()) {
      breathe();
    } else {
      patternFunctions[patternIdx]();
    }
  }
}


// Interrupt service routines for button and timer

void buttonIntr() {
  delay(2);
  unsigned long t = millis() - lastButtonIntr;
  if (isButtonDown()) {
    if (t > 2) {
      Timer1.start();
      Timer1.attachInterrupt(timerIntr);
   }
 } else {
    if ((t > 2) && (t < 2000)) {
      buttonPress();
    }
  }
  lastButtonIntr = millis();
}

void timerIntr() {
  unsigned long t = millis() - lastButtonIntr;
  if (t > 2) {
    Timer1.stop();
    Timer1.detachInterrupt();
    if (isButtonDown()) {
      buttonLongPress();
    }
   }
}

void buttonPress() {
  patternIdx = (patternIdx+1) % numPatterns;
  EEPROM.update(PATTERN_IDX_EEPROM_ADDR, patternIdx);
}

void buttonLongPress() {
  isPaused ^= 1;
  if (isPaused) {
    clear();
  }
}


// Helper functions to read pins
uint8_t isButtonDown() {
  return digitalRead(2) == 0;
}

uint8_t isSleeping() {
  return digitalRead(3) == 0;
}

