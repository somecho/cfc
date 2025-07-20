/**
 * @file color.h
 * */

#ifndef _SOYA_COLOR_H
#define _SOYA_COLOR_H

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

typedef struct syColor {
  union {
    float r, h;
  };
  union {
    float g, s;
  };
  union {
    float b, v;
  };
  float a;
} syColor;

#define SY_BLACK (syColor){0, 0, 0, 1}
#define SY_BLUE (syColor){0, 0, 1, 1}
#define SY_CYAN (syColor){0, 1, 1, 1}
#define SY_GREEN (syColor){0, 1, 0, 1}
#define SY_MAGENTA (syColor){1, 0, 1, 1}
#define SY_RED (syColor){1, 0, 0, 1}
#define SY_WHITE (syColor){1, 1, 1, 1}
#define SY_YELLOW (syColor){1, 1, 0, 1}

static inline syColor syRgb(float r, float g, float b, float a) {
  return (syColor){{r}, {g}, {b}, a};
}

static inline syColor syHsv(float h, float s, float v, float a) {
  return (syColor){{h}, {s}, {v}, a};
}

/** @since 0.1.0 */
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

/** @since 0.2.0 */
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
 * @c color in HSV or RGB
 * */
static inline syColor syWithAlpha(syColor c, float a) {
  return syRgb(c.r, c.g, c.b, a);
}

static inline void syPrintColor(syColor c) {
  printf("syColor %f %f %f %f\n", c.r, c.g, c.b, c.a);
}

#endif  // _SOYA_COLOR_H
