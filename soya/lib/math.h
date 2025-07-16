#ifndef SOYA_MATH_H
#define SOYA_MATH_H

#include <stdlib.h>

// Maps `i` which ranges from `a` - `b` to a range from `c` - `d`
//
// Example:
// ```
// float v = syMapRand(sinf(0.5), -1., 1., 0., 1.);
// assert((v >= 0.0 && v <= 1.0))
// ```
static inline float syMapRange(float i, float a, float b, float c, float d) {
  return (i - a) / (b - a) * (d - c) + c;
}

// @returns 0 to RAND_MAX
static inline int syRandi() { return rand(); }

// @returns 0 to 1
static inline float syRandf() { return (float)syRandi() / (float)RAND_MAX; }

static inline int syRandiRange(int lo, int hi) {
  return (int)syMapRange((float)syRandi(), (float)0, (float)RAND_MAX, (float)lo,
                         (float)hi);
}

static inline float syRandfRange(float lo, float hi) {
  return syMapRange(syRandf(), 0, 1, lo, hi);
}

#endif  // SOYAMATH_H_
