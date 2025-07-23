#pragma once

#include <soya/core/app.h>
#include <soya/glad/glad.h>

#include <stdio.h>
#include <GLFW/glfw3.h>

static inline void syOnError(int error_code, const char *description) {
  (void)error_code;
  perror(description);
}

static inline void syOnFrameBufferSize(GLFWwindow *window, int width,
                                       int height) {
  (void)window;
  glViewport(0, 0, width, height);
}
static inline void syOnKey(GLFWwindow *window, int key, int scancode,
                           int action, int mods) {
  syApp *app = (syApp *)glfwGetWindowUserPointer(window);
  (void)scancode;
  (void)mods;
  if (action == GLFW_PRESS) {
    if (app->onKey != NULL) {
      app->onKey(true, key);
    }
    if (key == GLFW_KEY_ESCAPE) {
      glfwSetWindowShouldClose(window, 1);
    }
  } else if (action == GLFW_RELEASE && app->onKey != NULL) {
    app->onKey(false, key);
  }
}

static inline void syOnMouseButton(GLFWwindow *window, int button, int action,
                                   int mods) {
  syApp *app = (syApp *)glfwGetWindowUserPointer(window);
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  if (action == GLFW_PRESS && app->onMousePress != NULL) {
    app->onMousePress(button, x, app->height - y);
  } else if (action == GLFW_RELEASE && app->onMouseRelease != NULL) {
    app->onMouseRelease(button, x, app->height - y);
  }
}

static inline void syOnMouseMoved(GLFWwindow *window, double xpos,
                                  double ypos) {
  syApp *app = (syApp *)glfwGetWindowUserPointer(window);
  if (app->onMouseMove != NULL) {
    app->onMouseMove(xpos, ypos);
  }
}

static inline void syOnScroll(GLFWwindow *window, double xoffset,
                              double yoffset) {
  syApp *app = (syApp *)glfwGetWindowUserPointer(window);
  if (app->onScroll != NULL) {
    app->onScroll(xoffset, yoffset);
  }
}
