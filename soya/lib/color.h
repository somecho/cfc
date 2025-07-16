#ifndef _SOYA_COLOR_H
#define _SOYA_COLOR_H

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

#endif  // _SOYA_COLOR_H
