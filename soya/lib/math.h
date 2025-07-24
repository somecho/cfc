/**
 * @file math.h
 * @brief Mathematical utility functions
 * */
#pragma once

#include <stdlib.h>

/**
 * Maps `i` which ranges from `a`-`b` to a range from `c`-`d`
 *
 * Example:
 * ```
 * float v = syMapRand(sinf(0.5), -1., 1., 0., 1.);
 * assert((v >= 0.0 && v <= 1.0))
 * ```
 *
 * @param i Input number to be mapped
 * @param a Lower limit of input range
 * @param b Upper limit of input range
 * @param c Lower limit of output range
 * @param d Upper limit of output range
 * @since 0.1.0
 * */
static inline float syMapRange(float i, float a, float b, float c, float d) {
  return (i - a) / (b - a) * (d - c) + c;
}

/**
 * @returns a random int between 0 and `RAND_MAX`
 * @since 0.1.0
 * */
static inline int syRandi(void) { return rand(); }

/**
 * @returns a random float between 0.0 and 1.0.
 * @since 0.1.0
 * */
static inline float syRandf(void) { return (float)syRandi() / (float)RAND_MAX; }

/**
 * @returns a random int from `a` upto and including `b`.
 * @since 0.1.0
 * */
static inline int syRandiRange(int a, int b) {
  return (int)syMapRange(syRandf(), 0.f, 1.f, (float)a, (float)b);
}

/**
 * @returns a random float from `a` upto and including `b`.
 * @since 0.1.0
 * */
static inline float syRandfRange(float a, float b) {
  return syMapRange(syRandf(), 0.f, 1.f, a, b);
}
