// Example: clear.c
//
// Description:
// Clears the screen with a color;

#include "../cfc.h"
#include <math.h>

void setup() {
  // The default window dimensions are 1200x800. This is defined in
  // CC_DEFAULT_WINDOW_WIDTH and CC_DEFAULT_WINDOW_HEIGHT. Use ccSetWindowSize
  // to change the window's size.
  ccSetWindowSize(800, 600);
}

void loop() {
  float r = sinf(glfwGetTime()) * 0.5 + 0.5;
  float g = cosf(glfwGetTime() * 1.02) * 0.5 + 0.5;
  float b = sinf(glfwGetTime() * 0.9) * 0.5 + 0.5;
  ccClearWindow(r, g, b, 1);
}
