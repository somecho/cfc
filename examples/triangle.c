// Example: triangle.c
//
// Description:
// Draws a red triangle to the screen.

#include "../cfc.h"

void setup()
{
  printf("Example: triangle.c\n");
}

void loop()
{
  ccClearWindow(CC_BLACK);
  float x1 = ccGetWidth() * 0.5;
  float y1 = ccGetHeight() * 0.75;
  float x2 = ccGetWidth() * 0.27;
  float y2 = ccGetHeight() * 0.25;
  float x3 = ccGetWidth() * 0.75;
  float y3 = ccGetHeight() * 0.25;
  ccSetColor(CC_RED);
  ccDrawTriangle(x1, y1, 0, x2, y2, 0, x3, y3, 0);
}
