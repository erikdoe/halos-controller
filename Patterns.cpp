#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

extern Adafruit_NeoPixel frame;


// HELPER FUNCTIONS

#define CW 0
#define CCW 1

#define R(c) ((c) >> 16 & 0xFF)
#define G(c) ((c) >> 8 & 0xFF)
#define B(c) ((c) & 0xFF)

uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return frame.Color(r, g, b);
}

uint32_t rgb_g(uint8_t r, uint8_t g, uint8_t b) {
  return frame.Color(frame.gamma8(r), frame.gamma8(g), frame.gamma8(b));
}

uint32_t mixChannel(uint32_t c1, uint32_t c2, uint16_t r) {
  return (c1 > c2) ? (c1 - (c1-c2)*r/1000) 
                   : (c2 - (c2-c1)*(1000-r)/1000);
}

uint32_t mix(uint32_t c1, uint32_t c2, uint16_t r) {
  return rgb(mixChannel(R(c1), R(c2), r), 
             mixChannel(G(c1), G(c2), r), 
             mixChannel(B(c1), B(c2), r));
}

uint32_t mix_g(uint32_t c1, uint32_t c2, uint16_t r) {
  return rgb_g(mixChannel(R(c1), R(c2), r), 
               mixChannel(G(c1), G(c2), r), 
               mixChannel(B(c1), B(c2), r));
}

uint8_t shift(int idx, uint8_t val, uint8_t dir) {
  uint16_t n = frame.numPixels();
  uint8_t result = (idx+val) % n;
  return (dir == CW) ? result : (n-1) - result;
}

uint8_t incOffset() {
  static uint8_t offset = 0;
  offset = (offset+1) % frame.numPixels();
  return offset;
}


// PRIMITIVES

// Fills LEDs with the given colour
// c: colour
void fill(uint32_t c, uint16_t wait) {
  uint16_t n = frame.numPixels();
  for (int i = 0; i < n; i++) {
    frame.setPixelColor(i, c);
  }
  frame.show();
  delay(wait);
}

// Shows a rotor pattern
// c1: blade colour
// c2: background colour
// s: spacing between blades (should be a divisor of 30)
// w: width of the blades (should be less than s)
// dir: direction
void rotor(uint32_t c1, uint32_t c2, uint8_t s, uint8_t w, int8_t dir, uint16_t wait) {
  uint16_t n = frame.numPixels();
  uint8_t offset = incOffset();

  for (int i = 0; i < n; i ++) {
    frame.setPixelColor(shift(i, offset, dir), (i%s < w) ? c1 : c2);
  }

  frame.show();
  delay(wait);
}

// Shows a rotating repeated gradient
// c1: first colour
// c2: second colour
// s: spacing between gradient starts (should be a divisor of 30)
// dir: direction
void gradient(uint32_t c1, uint32_t c2, uint8_t s, uint8_t dir, uint16_t wait) {
  uint16_t n = frame.numPixels();
  uint8_t offset = incOffset();

  for (int i = 0; i < n; i++) {
    uint16_t r = 1000 - 1000*(i%s)/s;
    frame.setPixelColor(shift(i, offset, dir), mix_g(c1, c2, r));
  }
  
  frame.show();
  delay(wait);
}

// Shows a rotating double gradient
// c1: first colour
// c2: second colour
void doubleGradient(uint32_t c1, uint32_t c2, uint16_t wait) {
  uint16_t n = frame.numPixels();
  uint8_t offset = incOffset();
  
  for (int i = 0; i < n; i++) {
    if (i < n/2) {
      frame.setPixelColor(shift(i, offset, CW), mix(c1, c2, 1000*2*i/n));
    } else {
      frame.setPixelColor(shift(i, offset, CW), mix(c2, c1, 1000*(2*i-n)/n));
    }
  }
  
  frame.show();
  delay(wait);
}

// Shows a rotating triple gradient
// c1: first colour
// c2: second colour
// c3: third colour
void tripleGradient(uint32_t c1, uint32_t c2, uint32_t c3, uint16_t wait) {
  uint16_t n = frame.numPixels();
  uint8_t offset = incOffset();
  
  for (int i = 0; i < n; i++) {
    if (i < n/3) {
      frame.setPixelColor(shift(i, offset, CW), mix(c1, c2, 1000*3*i/n));
    } else if (i < n*2/3) {
      frame.setPixelColor(shift(i, offset, CW), mix(c2, c3, 1000*(3*i-n)/n));
    } else {
      frame.setPixelColor(shift(i, offset, CW), mix(c3, c1, 1000*(3*i-2*n)/n));
    }
  }
  
  frame.show();
  delay(wait);
}

// Shows a lightning
// c1: lightning colour
// c2: background colour
// w: width of lightning
void lightning(uint32_t c1, uint32_t c2, uint8_t w, uint16_t wait) {
  uint16_t n = frame.numPixels();
  int i = random(n);

  for (int j = 0; j < w; j++) {
    frame.setPixelColor((i+j)%n, c1);
  }
  frame.show();
  delay(50);

  for (int j = 0; j < w; j++) {
    frame.setPixelColor((i+j)%n, c2);
  }
  frame.show();
  delay(wait);
}


// BREATHE PATTERN

void breathe() 
{
  static uint8_t counter = 0;
  counter = (counter+1) % 256;

  uint8_t x = frame.sine8(counter);
  uint8_t l = frame.gamma8(x/2+8); // shifting by 8 to get roughly 57 zeroes in a cycle
  // breatheDiagnostics(l);
  fill(rgb(0, 0, l), 14);
}

void breatheDiagnostics(uint8_t l) {
  static uint8_t lmax = 0;
  static uint8_t prev = 0;
  static uint8_t zeroes = 0;

  if (l > lmax) {
    lmax = l;
    Serial.print("new max: ");
    Serial.println(lmax);
  }

  if ((l == 0) && (prev != 0)) {
    zeroes = 0;
  } else if ((l == 0) && (prev == 0)) {
    zeroes += 1;
  } else if ((l != 0) && (prev == 0)) {
    Serial.print("zero count: ");
    Serial.println(zeroes);
  }
  prev = l;
}


// OTHER PATTERNS

void clear() {
  fill(rgb(0, 0, 0), 500);
}

void fillDarkBlue() {
  fill(rgb(0, 0, 49), 500);
}

void blueFiveBladeRotor() {
  rotor(rgb_g(0, 0, 255), rgb_g(0, 0, 96), 6, 1, CCW, 60);
}

void magentaSpinner() {
  rotor(rgb(255, 0, 255), rgb(0, 0, 150), 30, 4, CW, 30);
}

void redPropeller() {
  rotor(rgb(150, 0, 0), rgb(0, 0, 100), 15, 3, CW, 40);
}

void clockHand() {
  rotor(rgb(255, 0, 0), rgb(0, 0, 150), 30, 2, CW, 1000);
}

void darkOrangeSweep() {
  gradient(rgb(160, 80, 0), rgb(160-2*30, 64-30, 0), 30, CW, 30);
}

void darkRedSweep() {
  gradient(rgb(160, 0, 0), rgb(160-2*30, 0, 0), 30, CW, 30);
}

void blueMultiSweep() {
  gradient(rgb(0, 0, 255), rgb(0, 0, 96), 6, CCW, 60);
}

void blueRedDoubleGradient() {
  doubleGradient(rgb(0, 0, 255), rgb(192, 0, 0), 50);
}

void rainbow() {
  tripleGradient(rgb(255, 0, 0), rgb(0, 0, 255), rgb(0, 255, 0), 50);
}

void randomLightning() {
  lightning(rgb(255, 255, 0), rgb(0, 0, 40), random(1, 4), random(100, 800));
}


// PATTERN SELECTION

#define NUM_PATTERNS 10

uint8_t numPatterns = NUM_PATTERNS;

void (*patternFunctions[NUM_PATTERNS])() = {
  rainbow,
  blueRedDoubleGradient,
  magentaSpinner,
  redPropeller,
  blueFiveBladeRotor,
  blueMultiSweep,
  darkRedSweep,
  darkOrangeSweep,
  fillDarkBlue,
  randomLightning
};
