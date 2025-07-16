#ifndef SOYA_TYPEDEFS_H
#define SOYA_TYPEDEFS_H

#include <stdint.h>
#include <stdlib.h>

// Numbers

typedef float f32;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int i32;
typedef size_t usize;

// Color

typedef struct syRgb {
  f32 r, g, b;
} syRgb;

typedef struct syRgba {
  f32 r, g, b, a;
} syRgba;

typedef struct syHsv {
  f32 h, s, v;
} syHsv;

typedef struct syHsva {
  f32 h, s, v, a;
} syHsva;

#endif  // SOYATYPEDEFS_H
