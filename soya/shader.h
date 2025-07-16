#ifndef _SOYA_SHADER_H
#define _SOYA_SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <soya/defaultshaders.h>

#include <soya/glad/glad.h>

typedef GLuint syShader;

// @returns the info log to `shader` as a `const char *` owned by the caller.
static inline const char *syShaderInfoLog(const GLuint shader) {
  char *buffer = (char *)malloc(512);
  glGetShaderInfoLog(shader, 512, NULL, buffer);
  return buffer;
}

// @returns `true` if `shader` has compiled susyessfully. Otherwise `false`.
static inline bool syShaderCompileSuccess(const GLuint shader) {
  int susyess;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &susyess);
  return susyess == 1;
}

static inline GLuint syShaderLoadFromSource(const char *shaderSource,
                                            GLenum shaderType) {
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);
  if (!syShaderCompileSuccess(shader)) {
    const char *log = syShaderInfoLog(shader);
    perror(log);
    free((void *)log);
  }
  return shader;
}

static inline GLuint syShaderProgramLoadFromSource(
    const char *fragmentShaderSource, const char *vertexShaderSource) {
  const char *fsSrc = fragmentShaderSource == NULL ? SY_DEFAULT_FRAGMENT_SHADER
                                                   : fragmentShaderSource;
  const char *vsSrc = vertexShaderSource == NULL ? SY_DEFAULT_VERTEX_SHADER
                                                 : vertexShaderSource;

  const GLuint fs = syShaderLoadFromSource(fsSrc, GL_FRAGMENT_SHADER);
  const GLuint vs = syShaderLoadFromSource(vsSrc, GL_VERTEX_SHADER);

  const GLuint shader = glCreateProgram();

  glAttachShader(shader, vs);
  glAttachShader(shader, fs);
  glLinkProgram(shader);
  glDeleteShader(vs);
  glDeleteShader(fs);
  return shader;
}

static inline GLuint syShaderProgramLoadDefault() {
  return syShaderProgramLoadFromSource(SY_DEFAULT_FRAGMENT_SHADER,
                                       SY_DEFAULT_VERTEX_SHADER);
}

#endif  // _SOYA_SHADER_H
