#ifndef _SOYA_CALLBACKS_H
#define _SOYA_CALLBACKS_H

#include <stdio.h>

#include <soya/core/app.h>

#include <soya/glad/glad.h>

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
  (void)scancode;
  (void)mods;
  if (action == GLFW_PRESS) {
    syApp *app = (syApp *)glfwGetWindowUserPointer(window);
    if (app->onKeyPressed != NULL) {
      app->onKeyPressed(key);
    }
    if (key == GLFW_KEY_ESCAPE) {
      glfwSetWindowShouldClose(window, 1);
    }
  }
}

static inline void syOnMouseButton(GLFWwindow *window, int button, int action,
                                   int mods) {
  if (action == GLFW_PRESS) {
    syApp *app = (syApp *)glfwGetWindowUserPointer(window);
    if (app->onMousePressed != NULL) {
      double x, y;
      glfwGetCursorPos(window, &x, &y);
      app->onMousePressed(button, x, app->height - y);
    }
  }
}

#endif  // _SOYA_CALLBACKS_H
