#ifndef _SOYA_GLSLSNIPPETS_H
#define _SOYA_GLSLSNIPPETS_H

#include <soya/lib/sl.h>

#define SY_GLSL_HSV2RGB                                       \
  SYSL_FN(vec3 hsv2rgb(vec3 c) {                              \
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);            \
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);         \
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y); \
  })

#define SY_GLSL_RGB2HSV                                                      \
  SYSL_FN(vec3 rgb2hsv(vec3 c) {                                             \
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);                         \
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));        \
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));        \
    float d = q.x - min(q.w, q.y);                                           \
    float e = 1.0e-10;                                                       \
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x); \
  })

#define SY_GLSL_MAP                                                            \
  SYSL_FN(                                                                     \
      float map(float value, float min1, float max1, float min2, float max2) { \
        return min2 + (value - min1) * (max2 - min2) / (max1 - min1);          \
      })

#endif  // _SOYA_GLSLSNIPPETS_H
