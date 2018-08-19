#include <Arduino.h>

// Array that holds the patterns to use
extern void (*patternFunctions[])();

// Number of patterns in the above array
extern uint8_t numPatterns;

// Function to turn off all LEDs
void clear();

// Function to create a breathe effect (hard-coded to blue)
void breathe();
