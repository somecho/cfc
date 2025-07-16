#ifndef SOYA_COLOR_H
#define SOYA_COLOR_H

#include <soya/types.h>
#include <soya/math.h>

#define SY_BLACK (syRgba){0, 0, 0, 1}
#define SY_BLUE (syRgba){0, 0, 1, 1}
#define SY_CYAN (syRgba){0, 1, 1, 1}
#define SY_GREEN (syRgba){0, 1, 0, 1}
#define SY_MAGENTA (syRgba){1, 0, 1, 1}
#define SY_RED (syRgba){1, 0, 0, 1}
#define SY_WHITE (syRgba){1, 1, 1, 1}
#define SY_YELLOW (syRgba){1, 1, 0, 1}

static inline const syRgba syRgbWithAlpha(const syRgb c, f32 a) {
  return (syRgba){c.r, c.g, c.b, a};
}

static inline const syHsva syHsvWithAlpha(const syHsv c, f32 a) {
  return (syHsva){c.h, c.s, c.v, a};
}

static inline syRgb syRandRgb() {
  return (syRgb){syRandf(), syRandf(), syRandf()};
}

static inline syRgba syRandOpaqueRgb() {
  return (syRgba){syRandf(), syRandf(), syRandf(), 1.};
}

static inline syHsv syRandHsv() {
  return (syHsv){syRandf(), syRandf(), syRandf()};
}

static inline void syApplyRgba(void (*fn)(f32, f32, f32, f32), syRgba c) {
  fn(c.r, c.g, c.b, c.a);
}

#define syWithAlpha(c, a) \
  _Generic((c), syRgb: syRgbWithAlpha, syHsv: syHsvWithAlpha)((c), a)

#define syOpaque(c) syWithAlpha((c), 1.)

#endif
