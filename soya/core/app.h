#pragma once

#include <stdint.h>

#include <soya/core/defaults.h>
#include <soya/core/renderer.h>

#include <GLFW/glfw3.h>

typedef struct syApp {
  // Window width. Default: 1280
  int width;

  // Window height. Default: 720
  int height;

  // Pointer to app's window
  GLFWwindow *window;

  // Window title. Default: empty string
  const char *title;

  // Major OpenGL version number. Default: 4
  int glVersionMajor;

  // Major OpenGL version number. Default: 3
  int glVersionMinor;

  // Number of samples used for multisampling. Default: 8
  int samples;

  // Current frame number.
  uint64_t frameNum;

  // Current frame rate.
  float fps;

  // Time in seconds since initialization.
  double time;

  syRenderer renderer;

  void (*onKey)(bool pressed, int key);

  void (*onMouseMove)(double x, double y);
  void (*onMousePress)(int button, double x, double y);
  void (*onMouseRelease)(int button, double x, double y);
  void (*onScroll)(double x, double y);
  void (*onExit)(void);
} syApp;

static inline void syAppPreConfigure(syApp *app) {
  app->width = SY_DEFAULT_WINDOW_WIDTH;
  app->height = SY_DEFAULT_WINDOW_HEIGHT;
  app->samples = SY_DEFAULT_WINDOW_SAMPLES;
  app->title = "";
  app->glVersionMajor = SY_DEFAULT_GL_VERSION_MAJOR;
  app->glVersionMinor = SY_DEFAULT_GL_VERSION_MINOR;
}

static inline void syAppDisableCursor(const syApp *const app) {
  glfwSetInputMode(app->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetInputMode(app->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}
