#define SOYA_NO_CONFIGURE
#include <soya/soya.h>

void setup(syApp *app) {
  // An orthographic projection is used by default. We change it to a
  // perspective projection here.
  app->renderer.projectionMatrix =
      glms_perspective_default((float)app->width / (float)app->height);
}

void loop(syApp *app) {
  syClear(SY_BLACK);
  sySetColor(app, SY_RED);
  syRotate(app, app->frameNum / 120.f, 1, 1, 1);
  syDrawCube(app, 0.25);
  syResetTransformations(app);
}
