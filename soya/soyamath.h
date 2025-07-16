#ifndef SOYA_MATH_H
#define SOYA_MATH_H

#include "soyatypedefs.h"

// Maps `i` which ranges from `a` - `b` to a range from `c` - `d`
//
// Example:
// ```
// float v = syMapRand(sinf(0.5), -1., 1., 0., 1.);
// assert((v >= 0.0 && v <= 1.0))
// ```
static inline f32 syMapRange(f32 i, f32 a, f32 b, f32 c, f32 d) {
  return (i - a) / (b - a) * (d - c) + c;
}

// @returns 0 to RAND_MAX
static inline i32 syRandi() { return rand(); }

// @returns 0 to 1
static inline f32 syRandf() { return (f32)syRandi() / (f32)RAND_MAX; }

static inline i32 syRandiRange(i32 lo, i32 hi) {
  return (i32)syMapRange((f32)syRandi(), (f32)0, (f32)RAND_MAX, (f32)lo,
                         (f32)hi);
}

static inline f32 syRandfRange(f32 lo, f32 hi) {
  return syMapRange(syRandf(), 0, 1, lo, hi);
}

#endif  // SOYAMATH_H_
