#ifndef _SOYA_POLYLINE_H
#define _SOYA_POLYLINE_H

#include <soya/lib/vec.h>
#include <soya/lib/math.h>
#include <cglm/struct.h>

#ifdef _DEBUG
#include <stdio.h>
#endif

/**
 * Polyline
 * */
typedef struct syPl {
  size_t len, cap;
  vec3s *data;
  /** Cumulative lengths. */
  syVec(float) lengths;
} syPl;

static inline void syPlUpdate(syPl *pl) {
  float length = 0;
  pl->lengths.data[0] = 0;
  for (size_t i = 1; i < pl->len; i++) {
    length += glms_vec3_distance(pl->data[i - 1], pl->data[i]);
    pl->lengths.data[i] = length;
  }
}
static inline void syPlAddVertex(syPl *pl, vec3s v) {
  syVecPush(*pl, v);
  syVecPush(pl->lengths, 0);
  syPlUpdate(pl);
}

static inline void syPlInit(syPl *pl) {
  pl->len = 0;
  pl->cap = 16;
  pl->data = calloc(pl->cap, sizeof(vec3s));
  syVecInit(pl->lengths, float);
}

static inline void syPlDestroy(syPl *pl) {
  syVecDestroy(pl->lengths);
  free((void *)pl->data);
  pl->data = NULL;
}

/**
 * Shallow copy.
 * */
static inline syPl syPlCopy(const syPl *const pl) {
  syPl copied;
  syPlInit(&copied);
  for (size_t i = 0; i < pl->len; i++) {
    syPlAddVertex(&copied, pl->data[i]);
  }
  return copied;
}

static inline float syPlGetLength(const syPl *const pl) {
  if (pl->len < 2) {
    return 0;
  }
  return pl->lengths.data[pl->len - 1];
}

static inline float syPlGetIndexAtLength(const syPl *const pl, float length) {
  if (pl->len < 2) {
    return 0.0;
  }

  float totalLength = syPlGetLength(pl);
  float queryLength = glm_clamp(length, 0, totalLength);

  size_t lastPointIndex = pl->len;
  size_t i1 = (size_t)glm_clamp(
      floorf(queryLength / totalLength * (float)lastPointIndex), 0,
      (float)(pl->len - 2));
  size_t leftLimit = 0, rightLimit = lastPointIndex;

  float distAt1 = 0.f, distAt2 = 0.f;

  for (size_t iter = 0; iter < 32; iter++) {
    i1 = (size_t)glm_clamp((float)i1, 0, (float)(pl->len - 1));
    distAt1 = pl->lengths.data[i1];
    if (distAt1 <= queryLength) {
      distAt2 = pl->lengths.data[i1 + 1];
      if (distAt2 >= queryLength) {
        float t = syMapRange(queryLength, distAt1, distAt2, 0, 1);
        return (float)i1 + t;
      } else {
        leftLimit = i1;
      }
    } else {
      rightLimit = i1;
    }
    i1 = (leftLimit + rightLimit) / 2;
  }
  return 0.0;
}

/**
 * @param findex Interpolated index
 * */
static inline vec3s syPlGetVertexAtIndex(const syPl *const pl, float findex) {
  size_t il = (size_t)glm_clamp(floorf(findex), 0, (float)(pl->len - 1));
  size_t ir = (size_t)glm_clamp(ceilf(findex), 0, (float)(pl->len - 1));
  vec3s left = pl->data[il];
  vec3s right = pl->data[ir];
  float t = findex - floorf(findex);
  return glms_vec3_lerp(left, right, t);
}

static inline vec3s syPlGetVertexAtLength(const syPl *const pl, float length) {
  return syPlGetVertexAtIndex(pl, syPlGetIndexAtLength(pl, length));
}

static inline syPl syPlGetResampledBySpacing(const syPl *const pl,
                                             float spacing) {
  if (spacing == 0.0 || pl->len == 0) {
    return syPlCopy(pl);
  }
  syPl new;
  syPlInit(&new);
  float totalLength = syPlGetLength(pl);
  float f = 0;
  for (f = 0; f <= totalLength; f += spacing) {
    syPlAddVertex(&new, syPlGetVertexAtLength(pl, f));
  }
  syPlAddVertex(&new, pl->data[pl->len - 1]);
  return new;
}

static inline syPl syPlGetResampledByCount(const syPl *const pl, size_t count) {
  float totalLength = syPlGetLength(pl);
  size_t c = count;
  if (count < 2) {
#ifdef _DEBUG
    puts(
        "Warning: syPlGetResampledByCount() given count < 2. Using minimum "
        "count of 2.");
#endif
    c = 2;
  }
  return syPlGetResampledBySpacing(pl, totalLength / (float)(c - 1));
}

static inline vec3s syPlLerp(const syPl *const pl, float t) {
  float findex = syMapRange(t, 0, 1, 0, (float)(pl->len - 1));
  return syPlGetVertexAtIndex(pl, findex);
}

#endif  // _SOYA_POLYLINE_H
