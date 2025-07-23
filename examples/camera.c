#define SOYA_NO_CONFIGURE
#include <soya/soya.h>

syCamera cam = {0};

void onMouseMove(double x, double y) { syCameraOnMouseMove(&cam, x, y); }

void onKey(bool pressed, int key) { syCameraOnKey(&cam, pressed, key); }

void setup(syApp *app) {
  syCameraInit(&cam, (vec3s){{5, 5, -5}}, GLMS_VEC3_ZERO, (vec3s){{0, 1, 0}});
  app->onMouseMove = onMouseMove;
  app->onKey = onKey;
  syAppDisableCursor(app);
}

void loop(syApp *app) {
  syCameraUpdate(app, &cam);
  syClear(SY_BLACK);
  sySetColor(app, SY_MAGENTA);
  syDrawCube(app, &(syCube){.size = 1, .center = (vec3s){{0, 0.5, 0}}});
  syDrawSphere(app, &(sySphere){.radius = 1, .center = (vec3s){{4, 1, 0}}});
}
