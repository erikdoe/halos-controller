#include <Arduino.h>

#define D6 1175
#define Fs6 1480
#define A6 1760

void playTone(uint8_t pin, int freq, int dur, int wait) {
  int n = (uint32_t)2*freq*dur/1000;
  int t = 1000000/(2*freq);
  for (int i = 0; i < n; i++) {
    digitalWrite(pin, 1);
    delayMicroseconds(t);
    digitalWrite(pin, 0);
    delayMicroseconds(t);
  }
  delay(wait);
}

void startup(uint8_t pin) {
  for (int i = 0; i < 2; i++) {
    playTone(pin, A6, 30, 20);
    playTone(pin, D6, 30, 20);
  }
  playTone(pin, A6, 30, 0);
}

void beep(uint8_t pin) {
  playTone(pin, A6, 20, 0);
}

void beepbeep(uint8_t pin) {
  playTone(pin, A6, 50, 10000); // this shouldn't have to be so long
  playTone(pin, A6, 50, 0);
}

