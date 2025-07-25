#pragma once

#include "common.h"
#include <soya/lib/color.h>

TEST(color_syRgb){
    syColor res = syRgb(1,1,1,1);
    EXPECT(res.r == 1.);
    EXPECT(res.g == 1.);
    EXPECT(res.b == 1.);
    EXPECT(res.b == 1.);

    return TEST_SUCCESS;
}

TEST(color_conversions){
    syColor red = syHsvToRgb(syRgbToHsv(SY_RED));
    EXPECT(red.r == SY_RED.r);
    EXPECT(red.g == SY_RED.g);
    EXPECT(red.b == SY_RED.b);
    EXPECT(red.a == SY_RED.a);
    return TEST_SUCCESS;
}
