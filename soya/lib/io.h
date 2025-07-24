/**
 * @file io.h
 * @brief IO related utilities
 * */

#pragma once

#include <stdio.h>
#include <stdlib.h>

/**
 * @param filePath The path of the file to be read. This is relative to the
 * current working directory of the calling process.
 *
 * @returns A `char *` buffer that the caller of this function now owns and is
 * responsible of freeing. `NULL` if the function fails opening the file or
 * determining the filesize.
 *
 * @since 0.1.0
 * */
static inline char *syReadFile(const char *const filePath) {
  FILE *file = fopen(filePath, "rb");
  if (!file) {
    perror("syReadFile(): Failed to open file");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);
  if (fileSize < 0) {
    perror("syReadFile(): Failed to determine file size");
    return NULL;
  }

  char *buffer = (char *)calloc((size_t)fileSize + 1, sizeof(char));
  size_t bytesRead = fread(buffer, 1, (size_t)fileSize, file);
  buffer[bytesRead] = '\0';
  fclose(file);
  return buffer;
}
