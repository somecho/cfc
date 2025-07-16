#ifndef _SOYA_PREPROCESSOR_H
#define _SOYA_PREPROCESSOR_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * A simple preprocessor similar to what C/C++ compilers do. It replaces
 * `#include` directives with the contents of the path specified in the
 * directive. `#include` directives are parsed with the following rules:
 *
 * - There can only be one `#include` per line
 * - Paths must be enclosed in double quote. E.g. `#include "file"`
 * - Paths are relative to `filepath`. E.g. if `filepath` is
 *   `resources/frag.glsl` and it contains `#include "hash.glsl"`, then the
 *   resolved path is `resources/hash.glsl`.
 *
 * @param filepath Path to file to preprocess
 * @param buf A pointer to a char buffer that should be `NULL`. If
 * `syPreprocess` returned `PREPROCESSOR_OK`, then the results will be stored in
 * `buf`.
 **/
static inline int syPreprocess(char *filepath, char **buf);

static const char INCLUDE_LITERAL[] = "include";

#define PREPROCESSOR_OK 0x00000000
#define PREPROCESSOR_NULL_FILE_POINTER 0x00010000
#define PREPROCESSOR_CANNOT_READ_FILESIZE 0x00010001
#define PREPROCESSOR_CANNOT_OPEN_FILE 0x00010002
#define PREPROCESSOR_CANNOT_OPEN_INCLUDE_FILE 0x00010003

typedef struct syPreprocessingState {
  // File to preprocess
  FILE *file;
  int error;
  long filesize;
  // The directory that contains the file to preprocess
  char *workingDir;
  char *outputBuf;
  size_t outputLen;
  size_t outputIndex;
  int justEmbeddedInclude;
} syPreprocessingState;

static inline void syPreprocessorCleanup(syPreprocessingState *state) {
  if (state->file) {
    fclose(state->file);
  }
  free((void *)state->workingDir);
  free((void *)state->outputBuf);
}

static inline void syPreprocessorOpenFile(char *filepath,
                                          syPreprocessingState *state) {
  state->file = fopen(filepath, "rb");
  if (state->file == NULL) {
    state->error = PREPROCESSOR_CANNOT_OPEN_FILE;
    return;
  }
}
static inline void syPreprocessorSetFilesize(syPreprocessingState *state) {
  if (state->file == NULL) {
    state->error = PREPROCESSOR_NULL_FILE_POINTER;
    return;
  }
  fseek(state->file, 0, SEEK_END);
  state->filesize = ftell(state->file);
  rewind(state->file);
  if (state->filesize < 0) {
    state->error = PREPROCESSOR_CANNOT_READ_FILESIZE;
    return;
  }
}

static inline void syPreprocessorInitOutput(syPreprocessingState *state) {
  size_t initSize = 1024;
  state->outputBuf = (char *)calloc(initSize, sizeof(char));
  state->outputLen = initSize;
  state->outputIndex = 0;
}

static inline void syPreprocessorSetWorkingDir(char *filepath,
                                               syPreprocessingState *state) {
  size_t pos = 0, i = 0;
  char c = filepath[i];
  while (c != '\0') {
    if (c == '/' || c == '\\') {
      pos = i;
    }
    i++;
    c = filepath[i];
  }
  state->workingDir = (char *)calloc(pos + 2, sizeof(char));
  memcpy(state->workingDir, filepath, pos + 1);
}

static inline void syPreprocessInsertToken(char c,
                                           syPreprocessingState *state) {
  state->outputBuf[state->outputIndex] = c;
  state->outputIndex++;
  if (state->outputIndex + 1 >= state->outputLen) {
    state->outputBuf = realloc(state->outputBuf, state->outputLen * 2);
    state->outputLen *= 2;
  }
}

static inline void syPreprocessDeleteToken(syPreprocessingState *state) {
  state->outputBuf[state->outputIndex - 1] = '\0';
  state->outputIndex--;
}

static inline void syPreprocessParseComment(syPreprocessingState *state) {
  char cur = getc(state->file);
  while (cur != '\n') {
    cur = getc(state->file);
  }
  syPreprocessDeleteToken(state);
}

static inline void syPreprocessEmbedInclude(char *includePath,
                                            syPreprocessingState *state) {
  char resolvedPath[2048] = {0};
  snprintf(resolvedPath, 2048, "%s%s", state->workingDir, includePath);
  FILE *fp = fopen(resolvedPath, "rb");
  if (!fp) {
    state->error = PREPROCESSOR_CANNOT_OPEN_INCLUDE_FILE;
    return;
  }
  char c = getc(fp);
  while (c != EOF) {
    syPreprocessInsertToken(c, state);
    c = getc(fp);
  }
  state->justEmbeddedInclude = 1;
}

static inline void syPreprocessParseInclude(syPreprocessingState *state) {
  char cur = getc(state->file);

  while (cur != '"')  // scan to first quote
  {
    cur = getc(state->file);
  }
  cur = getc(state->file);  // consume quote
  size_t includePathLen = 0;
  while (cur != '"')  // scan to close quote
  {
    includePathLen++;  // track length between quotes
    cur = getc(state->file);
  }

  char includePath[includePathLen + 1];
  memset(includePath, 0, includePathLen + 1);
  fseek(state->file, -includePathLen - 1, SEEK_CUR);
  fread(includePath, sizeof(char), includePathLen, state->file);

  while (cur != '\n')  // scan to end of line
  {
    cur = getc(state->file);
  }
  syPreprocessEmbedInclude(includePath, state);
  if (state->error != PREPROCESSOR_OK) {
    return;
  }
}

static inline void syPreprocessParseDirective(syPreprocessingState *state) {
  char tmp[8] = {0};
  fread(tmp, sizeof(char), 7, state->file);
  if (strcmp(tmp, INCLUDE_LITERAL) == 0) {
    syPreprocessParseInclude(state);
    if (state->error != PREPROCESSOR_OK) {
      return;
    }
  } else {
    fseek(state->file, -7L, SEEK_CUR);
  }
}

static inline void syPreprocessParse(syPreprocessingState *state) {
  char prev = '\0', cur = getc(state->file);
  while (cur != EOF) {
    if (prev == '/' && cur == '/') {
      syPreprocessParseComment(state);
      cur = getc(state->file);
    } else if (cur == '#') {
      syPreprocessParseDirective(state);
      if (state->error != PREPROCESSOR_OK) {
        return;
      }
    }
    if (state->justEmbeddedInclude) {
      state->justEmbeddedInclude = 0;
      prev = cur;
      cur = getc(state->file);
      continue;
    }
    syPreprocessInsertToken(cur, state);
    prev = cur;
    cur = getc(state->file);
  }
}

static inline int syPreprocess(char *filepath, char **buf) {
  syPreprocessingState state = {0};

  syPreprocessorOpenFile(filepath, &state);
  if (state.error != PREPROCESSOR_OK) {
    syPreprocessorCleanup(&state);
    return state.error;
  }

  syPreprocessorSetFilesize(&state);
  if (state.error != PREPROCESSOR_OK) {
    syPreprocessorCleanup(&state);
    return state.error;
  }

  syPreprocessorSetWorkingDir(filepath, &state);

  syPreprocessorInitOutput(&state);
  syPreprocessParse(&state);
  if (state.error != PREPROCESSOR_OK) {
    syPreprocessorCleanup(&state);
    return state.error;
  }

  *buf = state.outputBuf;
  free(state.workingDir);
  return state.error;
}

#endif  // _SOYA_PREPROCESSOR_H
