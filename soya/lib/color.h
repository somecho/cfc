/**
 * @file color.h
 *
 * Color-related data structures and functions. Also contains macros for
 * predefined RGBA colors.
 *
 * */

#pragma once

#include <stdio.h>
#include <math.h>

/**
 * A 4-component color struct. The first 3 components can be used to represent
 * RGB or HSV. The last component represents alpha. The color values are encoded
 * as 32-bit floats.
 *
 * Example usage:
 * ```
 * syColor white = {{1},{1},{1},{1}};
 * syColor red = {{1},{0},{0},{1}};
 * assert(white.r == red.r);
 * ```
 *
 * @sa syRgb, syHsv
 * @since 0.1.0
 **/
typedef struct syColor {
  union {
    /** Red */
    float r;
    /** Hue */
    float h;
  };
  union {
    /** Green */
    float g;
    /** Saturation */
    float s;
  };
  union {
    /** Blue */
    float b;
    /** Value */
    float v;
  };
  /** Alpha */
  float a;
} syColor;

#define SY_BLACK (syColor){{0}, {0}, {0}, 1}
#define SY_BLUE (syColor){{0}, {0}, {1}, 1}
#define SY_CYAN (syColor){{0}, {1}, {1}, 1}
#define SY_GREEN (syColor){{0}, {1}, {0}, 1}
#define SY_MAGENTA (syColor){{1}, {0}, {1}, 1}
#define SY_RED (syColor){{1}, {0}, {0}, 1}
#define SY_WHITE (syColor){{1}, {1}, {1}, 1}
#define SY_YELLOW (syColor){{1}, {1}, {0}, 1}
#define SY_CRIMSON (syColor){{0.8633}, {0.082}, {0.2383}, 1}
#define SY_MAROON (syColor){{0.502}, {0}, {0}, 1}
#define SY_DARKGREEN (syColor){{0}, {0.3921}, {0}, 1}
#define SY_DEEPPINK (syColor){{1}, {0.082}, {0.5781}, 1}

/**
 * Creates a @ref syColor to hold RGBA values.
 * @param r Red. [0-1]
 * @param g Green. [0-1]
 * @param b Blue. [0-1]
 * @param a Alpha. [0-1]
 * @returns syColor
 * @since 0.1.0
 * */
static inline syColor syRgb(float r, float g, float b, float a) {
  return (syColor){{r}, {g}, {b}, a};
}

/**
 * Creates a @ref syColor to hold HSVA values.
 * @param h Hue. [0-1]
 * @param s Saturation. [0-1]
 * @param v Value. [0-1]
 * @param a Alpha. [0-1]
 * @returns syColor
 * @since 0.1.0
 * */
static inline syColor syHsv(float h, float s, float v, float a) {
  return (syColor){{h}, {s}, {v}, a};
}

/**
 * @param rgb A `syColor` containg RGBA values.
 * @returns A new HSV `syColor` converted from `rgb`
 * @sa syHsvToRgb
 * @since 0.1.0
 * */
static inline syColor syRgbToHsv(syColor rgb) {
  float fCMax = fmaxf(fmaxf(rgb.r, rgb.g), rgb.b);
  float fCMin = fminf(fminf(rgb.r, rgb.g), rgb.b);
  float fDelta = fCMax - fCMin;
  syColor out;
  out.a = rgb.a;
  if (fDelta > 0) {
    if (fCMax == rgb.r) {
      out.h = 60 * (fmod(((rgb.g - rgb.b) / fDelta), 6));
    } else if (fCMax == rgb.g) {
      out.h = 60 * (((rgb.b - rgb.r) / fDelta) + 2);
    } else if (fCMax == rgb.b) {
      out.h = 60 * (((rgb.r - rgb.g) / fDelta) + 4);
    }
    out.s = fCMax > 0 ? fDelta / fCMax : 0;
    out.v = fCMax;
  } else {
    out = syHsv(0, 0, fCMax, rgb.a);
  }
  if (out.h < 0) {
    out.h = 360 + out.h;
  }
  out.h /= 360.f;
  return out;
}

/**
 * @param rgb A `syColor` containg HSVA values.
 * @returns A new RGB `syColor` converted from `hsv`
 * @sa syRgbToHsv
 * @since 0.2.0
 * */
static inline syColor syHsvToRgb(syColor hsv) {
  float fV = hsv.v;
  float fS = hsv.s;
  float fC = fV * fS;  // Chroma
  float fHPrime = fmodf(hsv.h * 6.f, 6);
  float fX = fC * (1 - fabsf(fmodf(fHPrime, 2) - 1));
  float fM = fV - fC;
  float fR, fG, fB;
  if (0 <= fHPrime && fHPrime < 1) {
    fR = fC, fG = fX, fB = 0;
  } else if (1 <= fHPrime && fHPrime < 2) {
    fR = fX, fG = fC, fB = 0;
  } else if (2 <= fHPrime && fHPrime < 3) {
    fR = 0, fG = fC, fB = fX;
  } else if (3 <= fHPrime && fHPrime < 4) {
    fR = 0, fG = fX, fB = fC;
  } else if (4 <= fHPrime && fHPrime < 5) {
    fR = fX, fG = 0, fB = fC;
  } else if (5 <= fHPrime && fHPrime < 6) {
    fR = fC, fG = 0, fB = fX;
  } else {
    fR = 0, fG = 0, fB = 0;
  }
  fR += fM;
  fG += fM;
  fB += fM;
  return syRgb(fR, fG, fB, hsv.a);
}

/**
 * @param c A color containing either HSV or RGB values.
 * @returns A copy of `c` with new alpha value of `a`
 * @since 0.1.0
 * */
static inline syColor syWithAlpha(syColor c, float a) {
  return syRgb(c.r, c.g, c.b, a);
}

/**
 * Prints the values of the color's 4 components.
 * @param c The color to be printed.
 * @since 0.1.0
 * */
static inline void syPrintColor(syColor c) {
  printf("syColor %f %f %f %f\n", c.r, c.g, c.b, c.a);
}
