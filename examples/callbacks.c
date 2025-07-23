#define SOYA_NO_SETUP
#include <soya/soya.h>

void onMousePress(int button, double x, double y) {
  printf("Clicked: %lf %lf\n", x, y);
}
void onMouseRelease(int button, double x, double y) {
  printf("Released: %lf %lf\n", x, y);
}

void onKey(bool pressed, int key) {
  if (pressed) {
    printf("Key pressed: %c\n", key);
  } else {
    printf("Key released: %c\n", key);
  }
}

void onMouseMove(double x, double y) { printf("Moved: %lf %lf\n", x, y); }

void onScroll(double x, double y) { printf("Scrolled: %lf %lf\n", x, y); }

void configure(syApp *app) {
  app->onMousePress = onMousePress;
  app->onMouseRelease = onMouseRelease;
  app->onMouseMove = onMouseMove;
  app->onKey = onKey;
  app->onScroll = onScroll;
}

void loop(syApp *app) {}
