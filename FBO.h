#ifndef FBO_H_
#define FBO_H_

#include <epoxy/gl.h>

typedef struct FBO
{
  GLuint framebuffer;
  GLuint texture;
  GLenum format;
} FBO;

bool FBO_Allocate(FBO *fbo, int width, int height, GLenum format)
{
  // Generate framebuffer
  glGenFramebuffers(1, &fbo->framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->framebuffer);

  // Generate texture
  glGenTextures(1, &fbo->texture);

  // Initialize and configure texture
  glBindTexture(GL_TEXTURE_2D, fbo->texture);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // Attach texture to Framebuffer's Color Attachment 0
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbo->texture, 0);

  // Enable drawing to color attachment 0
  GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, drawBuffers);

  // Set viewport of framebuffer
  glViewport(0, 0, width, height);

  // Bind default framebuffer and texture
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Return success
  return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void FBO_Bind(FBO *fbo)
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->framebuffer);
}

void FBO_Unbind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#endif // FBO_H_
