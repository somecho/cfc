#ifndef _SOYA_IO_H
#define _SOYA_IO_H

#include <stdio.h>
#include <stdlib.h>

static inline const char *syReadFile(char *filePath) {
  FILE *file = fopen(filePath, "rb");
  if (!file) {
    perror("Failed to open file");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);
  if (fileSize < 0) {
    perror("Failed to determine file size");
    return NULL;
  }

  char *buffer = (char *)calloc((size_t)fileSize + 1, sizeof(char));
  size_t bytesRead = fread(buffer, 1, (size_t)fileSize, file);
  buffer[bytesRead] = '\0';  // Null-terminate
  fclose(file);
  return buffer;
}

#endif  // _SOYA_IO_H
