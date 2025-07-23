#ifndef _SOYA_RENDERER_H
#define _SOYA_RENDERER_H

#include <soya/core/shader.h>

#include <cglm/struct.h>
#include <soya/glad/glad.h>

typedef struct syRenderer {
  GLuint vao, vbo, cbo, ibo;
  syShader shader;
  syShader defaultShader;
  mat4s modelMatrix, viewMatrix, projectionMatrix;
  float color[4];
} syRenderer;

static inline void syRendererInit(syRenderer *r, int width, int height) {
  glGenVertexArrays(1, &r->vao);
  glGenBuffers(1, &r->vbo);
  glGenBuffers(1, &r->cbo);
  glGenBuffers(1, &r->ibo);
  r->color[0] = 1;
  r->color[1] = 1;
  r->color[2] = 1;
  r->color[3] = 1;
  r->projectionMatrix =
      glms_ortho(0, (float)width, 0, (float)height, 0.1f, 100.0);
  r->viewMatrix = glms_translate_make((vec3s){{0, 0, -1}});
  r->modelMatrix = glms_mat4_identity();
  printf("%s(): Loading default shader\n", __func__);
  r->shader = syShaderProgramLoadDefault();
  r->defaultShader = r->shader;
  glUseProgram(r->shader);
}

static inline void syRendererSetShaderUniforms(syRenderer *r, syShader s) {
  mat4s mat =
      glms_mul(r->projectionMatrix, (glms_mul(r->viewMatrix, r->modelMatrix)));
  syShaderUniformMat4fv(s, "modelViewProjectionMatrix", (float *)&mat);
}

#endif  // _SOYA_RENDERER_H
