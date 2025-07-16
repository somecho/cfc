#ifndef _SOYA_FBO_H
#define _SOYA_FBO_H

#include <soya/core/shader.h>
#include <soya/glad/glad.h>

typedef struct syFbo {
  GLuint framebuffer;
  GLuint texture;
  GLenum format;
  syShader shader;
} syFbo;

typedef struct syFboOptions {
  int width, height;
  GLenum internalFormat, format, type;
  GLint magFilter, minFilter;
} syFboOptions;

static const char *SY_RGB_FBO_FRAGMENT_SHADER =
    "#version 430 core\n"
    "out vec4 color;\n"
    "uniform sampler2D tex0;\n"
    "uniform vec2 res;"
    "void main()\n"
    "{\n"
    "  vec2 UV = gl_FragCoord.xy / res;"
    "  color = vec4(texture(tex0, UV).xyz,1.0);"
    "}\n\0";

static inline syFbo syFboCreate(syFboOptions *options) {
  syFbo fbo;
  // Generate framebuffer
  glGenFramebuffers(1, &fbo.framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo.framebuffer);

  // Generate texture
  glGenTextures(1, &fbo.texture);

  // Initialize and configure texture
  glBindTexture(GL_TEXTURE_2D, fbo.texture);

  GLenum format =
      options->format == 0 ? options->internalFormat : options->format;
  glTexImage2D(GL_TEXTURE_2D, 0, options->internalFormat, options->width,
               options->height, 0, format, options->type, 0);

  GLint magFilter = options->magFilter == 0 ? GL_NEAREST : options->minFilter;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

  GLint minFilter = options->magFilter == 0 ? GL_NEAREST : options->minFilter;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

  // Attach texture to Framebuffer's Color Attachment 0
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbo.texture, 0);

  // Enable drawing to color attachment 0
  GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, drawBuffers);

  // Set viewport of framebuffer
  glViewport(0, 0, options->width, options->height);

  // Bind default framebuffer and texture
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  // TODO: Handle other FBO texture formats
  fbo.shader = syShaderProgramLoadFromSource(SY_RGB_FBO_FRAGMENT_SHADER,
                                             SY_DEFAULT_VERTEX_SHADER);
  /* assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) ==
   * GL_FRAMEBUFFER_COMPLETE); */
  return fbo;
}

#endif
