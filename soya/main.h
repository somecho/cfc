#ifndef _SOYA_MAIN_H
#define _SOYA_MAIN_H

#ifndef SOYA_NO_MAIN

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <soya/app.h>
#include <soya/defaults.h>
#include <soya/callbacks.h>
#include <soya/renderer.h>

#include <soya/glad/glad.h>

#include <GLFW/glfw3.h>  // GLFW must always be imported last

void configure(syApp *app);
void setup(syApp *app);
void loop(syApp *app);

#ifdef SOYA_NO_SETUP
static inline const void setup(syApp *app) { (void)app; }
#endif

#ifdef SY_NO_CONFIGURE
static inline const void configure(syApp *app) { (void)app; }
#endif

static inline bool syMainPostConfigure(syApp *app) {
  printf("GL Version: %i.%i\n", app->glVersionMajor, app->glVersionMinor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, app->glVersionMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, app->glVersionMinor);
  glfwWindowHint(GLFW_SAMPLES, 8);

  puts("Creating window");
  app->window =

      glfwCreateWindow(app->width, app->height, app->title, NULL, NULL);

  if (!app->window) {
    glfwTerminate();
    return false;
  }

  puts("Obtaining context");
  glfwMakeContextCurrent(app->window);
  gladLoadGL();
  glViewport(0, 0, app->width, app->height);
  return true;
}

int main() {
  srand(time(NULL));
  int success = -1;

  glfwSetErrorCallback(syOnError);
  puts("Initializing GLFW");
  if (!glfwInit()) {
    return false;
  }

  syApp app = {0};
  syAppPreConfigure(&app);
  configure(&app);
  if (!syMainPostConfigure(&app)) {
    return success;
  };
  syRendererInit(&app.renderer, app.width, app.height);
  glPixelStorei(GL_PACK_ALIGNMENT, 2);
  setup(&app);

  //
  // Callbacks
  //
  glfwSetWindowUserPointer(app.window, (void *)&app);
  glfwSetFramebufferSizeCallback(app.window, syOnFrameBufferSize);
  glfwSetKeyCallback(app.window, syOnKey);
  glfwSetMouseButtonCallback(app.window, syOnMouseButton);

  double prevTime = glfwGetTime();
  while (!glfwWindowShouldClose(app.window)) {
    loop(&app);
    glfwSwapBuffers(app.window);
    glfwPollEvents();
    app.frameNum++;
    app.time = glfwGetTime();
    app.fps = 1.0 / (app.time - prevTime);
    prevTime = app.time;
  };
  // CLEAN UP
  glDeleteBuffers(1, &app.renderer.vbo);
  glDeleteBuffers(1, &app.renderer.cbo);
  glDeleteBuffers(1, &app.renderer.ibo);
  glfwTerminate();
  return 0;
}

#endif  // SOYA_NO_MAIN
#endif  // _SOYA_MAIN_H
