#define SOYA_NO_SETUP
#include <soya/soya.h>

void configure(syApp *app) {
  app->width = 1000;
  app->height = 1000;
}

void loop(syApp *app) {
  syClear(SY_WHITE);

  sySetColor(app, SY_BLACK);

  syTranslate(app, app->width * 0.25, app->height * 0.75, 0);
  syRotate(app, app->time * 0.05, 0, 0, 1);
  syDrawQuad(app, -100, -100, 200, 200);
  syResetTransformations(app);

  sySetColor(app, SY_MAGENTA);
  syTranslate(app, app->width * 0.75, app->height * 0.75, 0);
  syRotate(app, app->time * -0.15, 0, 0, 1);
  syDrawPolygon(app, 0, 0, 0, 200, 7);
  syResetTransformations(app);

  sySetColor(app, SY_GREEN);
  float linePts[] = {250, 250, 0,   250, 500, 0,   500, 500,
                     0,   500, 250, 0,   250, 250, 0};
  syDrawLines(app, linePts, 5);
}
