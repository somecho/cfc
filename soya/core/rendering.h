/**
 * @file rendering.h
 * */

#ifndef _SOYA_RENDERING_H
#define _SOYA_RENDERING_H

#include <soya/lib/color.h>
#include <soya/lib/gl.h>
#include <soya/core/fbo.h>
#include <soya/core/app.h>
#include <soya/glad/glad.h>

/**@{*/
static inline void syDrawUnindexed(syApp *app, float *vertices, float *colors,
                                   size_t n, GLenum mode) {
  glBindVertexArray(app->renderer.vao);
  syWriteArrayBuffer(app->renderer.vbo, sizeof(float) * n * 3, vertices);
  syVertexAttribute3f(0);

  if (colors == 0) {
    float *c = calloc(n * 4, sizeof(float));
    for (size_t i = 0; i < n; i++) {
      memcpy(&c[i * 4], app->renderer.color, sizeof(app->renderer.color));
    }
    syWriteArrayBuffer(app->renderer.cbo, sizeof(float) * n * 4, c);
    free(c);
  } else {
    syWriteArrayBuffer(app->renderer.cbo, sizeof(float) * n * 4, colors);
  }
  syVertexAttribute4f(1);

  syRendererSetShaderUniforms(&app->renderer, app->renderer.shader);
  glDrawArrays(mode, 0, n);
}

static inline void syDrawIndexed(syApp *app, float *vertices, float *colors,
                                 uint32_t *indices, size_t numVertices,
                                 size_t numIndices, GLenum mode) {
  glBindVertexArray(app->renderer.vao);

  syWriteArrayBuffer(app->renderer.vbo, sizeof(float) * numVertices * 3,
                     vertices);
  syVertexAttribute3f(0);

  if (colors == 0) {
    float *c = calloc(numVertices * 4, sizeof(float));
    for (size_t i = 0; i < numVertices; i++) {
      memcpy(&c[i * 4], app->renderer.color, sizeof(app->renderer.color));
    }
    syWriteArrayBuffer(app->renderer.cbo, sizeof(float) * numVertices * 4, c);
    free(c);
  } else {
    syWriteArrayBuffer(app->renderer.cbo, sizeof(float) * numVertices * 4,
                       colors);
  }
  syVertexAttribute4f(1);
}

/**
 * Draws a triangle
 */
static inline void syDrawTriangle(syApp *app, float x1, float y1, float z1,
                                  float x2, float y2, float z2, float x3,
                                  float y3, float z3) {
  float vertices[] = {x1, y1, z1, x2, y2, z2, x3, y3, z3};
  syDrawUnindexed(app, vertices, NULL, 3, GL_TRIANGLES);
}

/**
 * Draws a quad
 * */
static inline void syDrawQuad(syApp *app, float x, float y, float width,
                              float height) {
  float vertices[] = {x,         y,          0, x + width, y,          0,
                      x + width, y + height, 0, x,         y + height, 0};
  syDrawUnindexed(app, vertices, NULL, 4, GL_TRIANGLE_FAN);
}

/**
 * Draws a line
 * */
static inline void syDrawLine(syApp *app, float x1, float y1, float z1,
                              float x2, float y2, float z2) {
  float vertices[] = {x1, y1, z1, x2, y2, z2};
  syDrawUnindexed(app, vertices, NULL, 2, GL_LINES);
}

static inline void syDrawLines(syApp *app, float *vertices, int n) {
  syDrawUnindexed(app, vertices, NULL, n, GL_LINE_STRIP);
}

static inline void syDrawPolygon(syApp *app, float x, float y, float z,
                                 float radius, size_t numSides) {
  if (numSides < 3) {
    perror("numSides must be greater than 3");
  }
  size_t numItems = numSides + 2;  // 1 extra for the center and 1 for the end
  float *vertices = (float *)calloc(numItems * 3, sizeof(float));

  vertices[0] = x;
  vertices[1] = y;
  vertices[2] = z;

  float t = GLM_PI * 2.f / (float)numSides;
  for (size_t i = 0; i < numSides; i++) {
    vertices[(i + 1) * 3] = cosf(t * i) * radius + x;
    vertices[(i + 1) * 3 + 1] = sinf(t * i) * radius + y;
    vertices[(i + 1) * 3 + 2] = 0;
  }

  vertices[numItems * 3 - 3] = vertices[3];
  vertices[numItems * 3 - 2] = vertices[4];
  vertices[numItems * 3 - 1] = vertices[5];

  syDrawUnindexed(app, vertices, NULL, numSides + 2, GL_TRIANGLE_FAN);
  free((void *)vertices);
}

/**@}*/

/**@{ */
static inline void syClear(syColor col) {
  glClearColor(col.r, col.g, col.b, col.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static inline void sySetColor(syApp *app, syColor col) {
  memcpy(app->renderer.color, (float *)&col, sizeof(float) * 4);
}

static inline void syTranslate(syApp *app, float x, float y, float z) {
  app->renderer.modelMatrix =
      glms_translate(app->renderer.modelMatrix, (vec3s){{x, y, z}});
}

static inline void syRotate(syApp *app, float angle, float x, float y,
                            float z) {
  app->renderer.modelMatrix =
      glms_rotate(app->renderer.modelMatrix, angle, (vec3s){{x, y, z}});
}

static inline void syResetTransformations(syApp *app) {
  app->renderer.modelMatrix = glms_mat4_identity();
}
/**@}*/

/**@{ */
static inline void syBeginShader(syApp *app, syShader shader) {
  glUseProgram(shader);
  app->renderer.shader = shader;
}

static inline void syEndShader(syApp *app) {
  glUseProgram(app->renderer.defaultShader);
  app->renderer.shader = app->renderer.defaultShader;
}
/**@}*/

/**@{*/
static inline void syFboBegin(syFbo *fbo) {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->framebuffer);
}

static inline void syFboEnd() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

static inline void syFboDraw(syApp *app, syFbo *fbo) {
  syBeginShader(app, fbo->shader);
  syShaderUniformTexture(fbo->shader, "tex0", fbo->texture);
  syShaderUniform2f(fbo->shader, "res", app->width, app->height);
  syDrawQuad(app, 0, 0, app->width, app->height);
  syEndShader(app);
}
/**@}*/

#endif  // _SOYA_RENDERING_H
