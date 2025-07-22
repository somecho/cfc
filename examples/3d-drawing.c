#define SOYA_NO_CONFIGURE
#include <soya/soya.h>

void setup(syApp *app) {
  app->renderer.projectionMatrix =
      glms_perspective(45, (float)app->width / (float)app->height, 0.1, 100.);
}

void loop(syApp *app) {
  syClear(SY_BLACK);
  sySetColor(app, SY_RED);
  syTranslate(app, 0, 0, -2);
  syRotate(app, app->frameNum / 120.f, 1, 1, 1);
  syDrawCube(app, 1);
  syResetTransformations(app);
}
