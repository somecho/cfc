#pragma once

#include "../../vendor/stb_image_write.h"

#include <stdbool.h>

#include <soya/glad/glad.h>

void sySaveFrame(int w, int h, char *filename) {
  stbi_flip_vertically_on_write(1);
  void *pixels = calloc(w * h * 4, sizeof(char));
  glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  int comp = 4;  // RGBA
  int stride = w * 4 * sizeof(char);
  stbi_write_png(filename, w, h, comp, pixels, stride);
}
