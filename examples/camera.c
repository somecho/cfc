#define SOYA_NO_CONFIGURE
#include <soya/soya.h>

syCamera cam = {0};

void onMouseMove(double x, double y) { syCameraOnMouseMove(&cam, x, y); }

void onKey(bool pressed, int key) { syCameraOnKey(&cam, pressed, key); }

void setup(syApp *app) {
  syCameraInit(&cam, (vec3s){{5, 5, -5}}, GLMS_VEC3_ZERO, (vec3s){{0, 1, 0}});
  app->renderer.projectionMatrix = syGetDefaultPerspective(app);
  app->onMouseMove = onMouseMove;
  app->onKey = onKey;
  glfwSetInputMode(app->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetInputMode(app->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void loop(syApp *app) {
  syCameraUpdate(app, &cam);
  syClear(SY_BLACK);
  sySetColor(app, SY_MAGENTA);
  syDrawCube(app, &(syCube){.size = 1});
}
