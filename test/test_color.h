#pragma once

#include "common.h"
#include <soya/lib/color.h>
#include <soya/lib/math.h>

TEST(color_syRgb) {
  syColor res = syRgb(1, 1, 1, 1);
  EXPECT(res.r == 1.);
  EXPECT(res.g == 1.);
  EXPECT(res.b == 1.);
  EXPECT(res.b == 1.);

  return (TestStatus){.result = TEST_SUCCESS};
}

TEST(color_conversions) {
  syColor cols[] = {SY_RED,     SY_BLACK, SY_BLUE, SY_YELLOW,
                    SY_MAGENTA, SY_CYAN,  SY_GREEN};
  for (size_t i = 0; i < sizeof(cols) / sizeof(cols[i]); i++) {
    syColor o = syHsvToRgb(syRgbToHsv(cols[i]));
    EXPECT(o.r == cols[i].r);
    EXPECT(o.g == cols[i].g);
    EXPECT(o.b == cols[i].b);
    EXPECT(o.a == cols[i].a);
  }
  return (TestStatus){.result = TEST_SUCCESS};
}

TEST(color_with_alpha) {
  int numRuns = 20;
  for (int i = 0; i < numRuns; i++) {
    float r = syRandf();
    float g = syRandf();
    float b = syRandf();
    float a = syRandf();
    float a2 = syRandf();
    syColor c1 = syRgb(r, g, b, a);
    syColor c2 = syWithAlpha(c1, a2);
    EXPECT(c1.r == c2.r);
    EXPECT(c1.g == c2.g);
    EXPECT(c1.b == c2.b);
    EXPECT(c1.a != c2.a);
    EXPECT(c2.a == a2);
  }
  return (TestStatus){.result = TEST_SUCCESS};
}
