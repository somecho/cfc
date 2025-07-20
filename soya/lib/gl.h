/**
 * @file gl.h
 *
 * OpenGL helper functions.
 * */
#ifndef _SOYA_GL_H_
#define _SOYA_GL_H_

#include <stddef.h>
#include <soya/glad/glad.h>

static inline void syWriteBuffer(GLenum target, GLuint buffer, GLsizeiptr size,
                                 const void *data, GLenum usage) {
  glBindBuffer(target, buffer);
  glBufferData(target, size, data, usage);
}

static inline void syWriteArrayBuffer(GLuint buffer, size_t size, void *data) {
  syWriteBuffer(GL_ARRAY_BUFFER, buffer, (GLsizeiptr)size, data,
                GL_DYNAMIC_DRAW);
}

static inline void syVertexAttribute(GLuint index, GLint size, GLenum type,
                                     GLboolean normalized, GLsizei stride,
                                     const void *pointer) {
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
  glEnableVertexAttribArray(index);
}

static inline void syVertexAttribute2f(GLuint index) {
  syVertexAttribute(index, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
}

static inline void syVertexAttribute3f(GLuint index) {
  syVertexAttribute(index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
}

static inline void syVertexAttribute4f(GLuint index) {
  syVertexAttribute(index, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
}

#endif  // _SOYA_GL_H_
