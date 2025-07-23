#pragma once

#include <cglm/struct.h>
#include <soya/core/app.h>

// todo : orbital
typedef enum syCameraMode { SY_CAMERA_FIRST_PERSON } syCameraMode;

typedef struct syCamera {
  vec3s pos;
  vec3s dir;
  vec3s up;
  mat4s view;
  struct {
    double prevX, prevY;
  } mouse;
  bool wdown, adown, sdown, ddown, spacedown, shiftdown;
  syCameraMode mode;
} syCamera;

static inline void syCameraInit(syCamera *cam, vec3s pos, vec3s target,
                                vec3s up) {
  cam->pos = pos;
  cam->dir = glms_vec3_sub(target, pos);
  cam->up = up;
  cam->view = glms_lookat(pos, target, up);
  cam->mouse.prevX = 0, cam->mouse.prevY = 0;
  cam->mode = SY_CAMERA_FIRST_PERSON;
#ifdef _DEBUG
  puts(
      "syCameraInit(): call syCameraOnMouseMove() and syCameraOnKey() in your "
      "callbacks to enable first-person movement.");
#endif
}
static inline void syCameraOnMouseMove(syCamera *cam, double x, double y) {
  if (cam->mouse.prevX == 0) {
    cam->mouse.prevX = x;
  }
  if (cam->mouse.prevY == 0) {
    cam->mouse.prevY = y;
  }
  double dx = x - cam->mouse.prevX, dy = y - cam->mouse.prevY;
  vec3s right = glms_vec3_normalize(glms_vec3_cross(cam->dir, cam->up));
  cam->dir = glms_vec3_rotate(cam->dir, dy * -0.001, right);
  cam->dir = glms_vec3_rotate(cam->dir, dx * 0.001, (vec3s){{0, -1, 0}});
  cam->view = glms_lookat(cam->pos, glms_vec3_add(cam->pos, cam->dir), cam->up);
  cam->mouse.prevX = x, cam->mouse.prevY = y;
}

static inline void syCameraOnKey(syCamera *cam, bool pressed, int key) {
  switch (key) {
    case 'W':
      cam->wdown = pressed;
      break;
    case 'A':
      cam->adown = pressed;
      break;
    case 'S':
      cam->sdown = pressed;
      break;
    case 'D':
      cam->ddown = pressed;
      break;
    case ' ':
      cam->spacedown = pressed;
      break;
    case 340:  // left shift
      cam->shiftdown = pressed;
      break;
  }
}

static inline float syGetAspect(const syApp *const app) {
  return (float)app->width / (float)app->height;
}

static inline mat4s syGetDefaultPerspective(const syApp *const app) {
  return glms_perspective_default(syGetAspect(app));
}

static inline void syCameraUpdate(syApp *app, syCamera *cam) {
  app->renderer.viewMatrix = cam->view;
  app->renderer.projectionMatrix = syGetDefaultPerspective(app);
  vec3s right = glms_vec3_normalize(glms_vec3_cross(cam->dir, cam->up));
  float moveSpeed = 0.1;
  if (cam->wdown) {
    cam->pos =
        glms_vec3_add(cam->pos, glms_vec3_scale(cam->dir, moveSpeed * 0.25));
  }
  if (cam->sdown) {
    cam->pos =
        glms_vec3_add(cam->pos, glms_vec3_scale(cam->dir, -moveSpeed * 0.25));
  }
  if (cam->adown) {
    cam->pos = glms_vec3_add(cam->pos, glms_vec3_scale(right, -moveSpeed));
  }
  if (cam->ddown) {
    cam->pos = glms_vec3_add(cam->pos, glms_vec3_scale(right, moveSpeed));
  }
  if (cam->spacedown) {
    cam->pos = glms_vec3_add(cam->pos, glms_vec3_scale(cam->up, moveSpeed));
  }
  if (cam->shiftdown) {
    cam->pos = glms_vec3_add(cam->pos, glms_vec3_scale(cam->up, -moveSpeed));
  }
  cam->view = glms_lookat(cam->pos, glms_vec3_add(cam->pos, cam->dir), cam->up);
}
