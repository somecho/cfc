// Example: triangle.c
//
// Description:
// Draws a white triangle on the screen.

#include "../cfc.h"

void setup() {
  float x1 = ccGetWidth() * 0.5;
  float y1 = ccGetHeight() * 0.75;
  float x2 = ccGetWidth() * 0.27;
  float y2 = ccGetHeight() * 0.25;
  float x3 = ccGetWidth() * 0.75;
  float y3 = ccGetHeight() * 0.25;
  ccDrawTriangle(x1, y1, 0, x2, y2, 0, x3, y3, 0);
}

void loop() {}
