/**
 * @file color.h
 * */

#ifndef _SOYA_COLOR_H
#define _SOYA_COLOR_H

#include <math.h>
#include <string.h>

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

static inline syColor syRgbToHsv(syColor hsv) {
  float h = hsv.h * 360.f;
  float c = hsv.v * hsv.s;
  float x = c * (1.f - fabsf(fmodf(h / 60.f, 2.0) - 1.f));
  float m = hsv.v - c;
  float rgb[3] = {0};
  if (0.f <= h && h < 60.f) {
    memcpy(rgb, (float[3]){c, x, 0}, sizeof(float) * 3);
  } else if (60.f <= h && h < 120.f) {
    memcpy(rgb, (float[3]){x, c, 0}, sizeof(float) * 3);
  } else if (120.f <= h && h < 180.f) {
    memcpy(rgb, (float[3]){0, c, x}, sizeof(float) * 3);
  } else if (180.f <= h && h < 240.f) {
    memcpy(rgb, (float[3]){0, x, c}, sizeof(float) * 3);
  } else if (240.f <= h && h < 300.f) {
    memcpy(rgb, (float[3]){x, 0, c}, sizeof(float) * 3);
  } else if (300.f <= h && h < 360.f) {
    memcpy(rgb, (float[3]){c, 0, x}, sizeof(float) * 3);
  };
  return (syColor){{rgb[0] + m}, {rgb[1] + m}, {rgb[2] + m}, hsv.a};
}

/**
 * @c color in HSV or RGB
 * */
static inline syColor syWithAlpha(const syColor *const c, float a) {
  return syRgb(c->r, c->g, c->b, a);
}

#endif  // _SOYA_COLOR_H
