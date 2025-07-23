#pragma once

#ifndef SOYA_NO_MAIN

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <soya/core/core.h>
#include <soya/glad/glad.h>

#include <GLFW/glfw3.h>

void setup(syApp *app);
#ifdef SOYA_NO_SETUP
void setup(syApp *app) { (void)app; }
#endif

void configure(syApp *app);
#ifdef SOYA_NO_CONFIGURE
void configure(syApp *app) { (void)app; }
#endif

void loop(syApp *app);

static inline bool syMainPostConfigure(syApp *app) {
  printf("%s(): Using GL Version %i.%i\n", __func__, app->glVersionMajor,
         app->glVersionMinor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, app->glVersionMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, app->glVersionMinor);
  glfwWindowHint(GLFW_SAMPLES, 8);

  printf("%s(): Creating %ix%i window\n", __func__, app->width, app->height);
  app->window =
      glfwCreateWindow(app->width, app->height, app->title, NULL, NULL);

  if (!app->window) {
    glfwTerminate();
    return false;
  }

  printf("%s(): calling glfwMakeContextCurrent()\n", __func__);
  glfwMakeContextCurrent(app->window);
  glfwSwapInterval(1);
  gladLoadGL();
  glViewport(0, 0, app->width, app->height);
  glEnable(GL_DEPTH_TEST);
  return true;
}

int main(void) {
#ifdef _DEBUG
  printf("%s(): Configuration: DEBUG\n", __func__);
#endif
  srand((unsigned)time(NULL));
  int success = -1;

  glfwSetErrorCallback(syOnError);
  printf("%s(): calling glfwInit()\n", __func__);
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
  printf("%s(): Setting GL_PACK_ALIGNMENT to 2\n", __func__);
  glPixelStorei(GL_PACK_ALIGNMENT, 2);
  setup(&app);

  glfwSetWindowUserPointer(app.window, (void *)&app);
  glfwSetFramebufferSizeCallback(app.window, syOnFrameBufferSize);
  glfwSetKeyCallback(app.window, syOnKey);
  glfwSetMouseButtonCallback(app.window, syOnMouseButton);
  glfwSetCursorPosCallback(app.window, syOnMouseMoved);
  glfwSetScrollCallback(app.window, syOnScroll);

  printf("%s(): Beginning main loop...\n", __func__);
  double prevTime = glfwGetTime();
  while (!glfwWindowShouldClose(app.window)) {
    loop(&app);
    glfwSwapBuffers(app.window);
    glfwPollEvents();
    app.frameNum++;
    app.time = glfwGetTime();
    app.fps = 1.f / (float)(app.time - prevTime);
    prevTime = app.time;
  }

  printf("%s(): Main loop exited\n", __func__);
  if (app.onExit != NULL) {
    printf("%s(): Calling onExit()\n", __func__);
    app.onExit();
  }
  printf("%s(): Cleaning up resources\n", __func__);
  glDeleteBuffers(1, &app.renderer.vbo);
  glDeleteBuffers(1, &app.renderer.cbo);
  glDeleteBuffers(1, &app.renderer.ibo);
  glfwTerminate();
  return 0;
}

#endif  // SOYA_NO_MAIN
