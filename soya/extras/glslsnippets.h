#ifndef _SOYA_GLSLSNIPPETS_H
#define _SOYA_GLSLSNIPPETS_H

#include <soya/lib/sl.h>

#define SY_GLSL_SNIPPET_HSV2RGB                               \
  SYSL_FN(vec3 hsv2rgb(vec3 c) {                              \
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);            \
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);         \
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y); \
  })

#endif  // _SOYA_GLSLSNIPPETS_H
