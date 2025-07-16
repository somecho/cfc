//
// Example: 02-basic-drawing.c
// Description:
// This example shows some of the basic shape drawing and transformation
// functions in Soya.
//

#include <soya/soya.h>

void configure(syApp *app) {
  app->width = 1000;
  app->height = 1000;
}

void setup(syApp *app) {
  (void)app;  // Just to prevent the unused-parameter warning
}

void loop(syApp *app) {
  syClear(1., 1., 1., 1.);

  sySetColor(app, SY_BLACK.r, SY_BLACK.g, SY_BLACK.b, SY_BLACK.a);

  syTranslate(app, app->width * 0.25, app->height * 0.75, 0);
  syRotate(app, app->time * 0.05, 0, 0, 1);
  syDrawQuad(app, -100, -100, 200, 200);
  syResetTransformations(app);

  sySetColor(app, SY_MAGENTA.r, SY_MAGENTA.g, SY_MAGENTA.b, SY_MAGENTA.a);
  syTranslate(app, app->width * 0.75, app->height * 0.75, 0);
  syRotate(app, app->time * -0.15, 0, 0, 1);
  syDrawPolygon(app, 0, 0, 0, 200, 7);
  syResetTransformations(app);

  sySetColor(app, SY_GREEN.r, SY_GREEN.g, SY_GREEN.b, SY_GREEN.a);
  float linePts[] = {250, 250, 0,   250, 500, 0,   500, 500,
                     0,   500, 250, 0,   250, 250, 0};
  syDrawLines(app, linePts, 5);
}
