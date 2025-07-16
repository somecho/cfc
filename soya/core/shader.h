#ifndef _SOYA_SHADER_H
#define _SOYA_SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <soya/lib/preprocessor.h>
#include <soya/core/defaultshaders.h>

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

// @returns a `GLuint` representing the shader compiled from `shaderPath` and
// given the `shaderType`. This operation may fail.
static inline GLuint syShaderLoadFromFile(char *shaderPath, GLenum shaderType) {
  char *src = NULL;
  syPreprocess(shaderPath, &src);
  GLuint shader = syShaderLoadFromSource(src, shaderType);
  free((void *)src);
  return shader;
}

static inline void syShaderUniform1f(GLuint shader, const char *uniformName,
                                     float f) {
  GLint u = glGetUniformLocation(shader, uniformName);
  glUniform1f(u, f);
}

static inline void syShaderUniform2f(GLuint shader, const char *uniformName,
                                     float f1, float f2) {
  GLint u = glGetUniformLocation(shader, uniformName);
  glUniform2f(u, f1, f2);
}

static inline void syShaderUniformMat4fv(GLuint shader, const char *uniformName,
                                         const float *value) {
  GLint u = glGetUniformLocation(shader, uniformName);
  glUniformMatrix4fv(u, 1, GL_FALSE, value);
}

static inline void syShaderUniformTexture(GLuint shader, const char *name,
                                          GLuint texture) {
  glActiveTexture(GL_TEXTURE0 + texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  GLuint uTex = glGetUniformLocation(shader, name);
  glUniform1i(uTex, texture);
}

#endif  // _SOYA_SHADER_H
