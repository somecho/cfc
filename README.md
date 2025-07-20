# Soya - Creative Coding with C 

> ⚠️ Under active development, API can change at any moment!

Soya is a header-only library for creating 2D/3D graphics. 

### Lightweight

Soya only has 2 hard dependencies:
- [GLFW][glfw] for window management
- [Cglm][cglm] for vector and matrix math

With CMake, you won't need to install them.

### Easy API

It's somewhere in between Processing and OpenFrameworks in terms of ease of use. Go to [getting started](https://github.com/somecho/soya/wiki/Getting-Started) for more details.

```c
#include <soya/soya.h>

void configure(syApp *app){
  app->width = 1000;
  app->height = 1000;
}

void setup(syApp *app){}

void loop(syApp *app){
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
```

[glfw]:https://github.com/glfw/glfw
[cglm]:https://github.com/recp/cglm
