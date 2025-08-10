#pragma once

#include <cglm/struct.h>

typedef struct syTriangle {
  vec3s a, b, c;
} syTriangle;

/**
 * Projects vec3 `a` onto vec3 `b`.
 * */
vec3s syProjectVec3(vec3s a, vec3s b) {
  vec3s bnorm = glms_normalize(b);
  float dot = glms_vec3_dot(a, bnorm);
  vec3s scaled = glms_vec3_scale(bnorm, dot);
  return scaled;
}

float syTriangleArea(syTriangle t) {
  vec3s ptd = syProjectVec3(glms_vec3_sub(t.a, t.b), glms_vec3_sub(t.c, t.b));
  float base = glms_vec3_distance(t.b, t.c);
  float height = glms_vec3_distance(ptd, GLMS_VEC3_ZERO);
  return base * height * 0.5;
}
