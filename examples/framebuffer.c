#include "../cfc.h"
#include "../FBO.h"

FBO myFbo;
GLuint shader;

void setup()
{
  FBO_Allocate(&myFbo, ccGetWidth(), ccGetHeight(), GL_RGB);
  auto fs = ccReadFile("../resources/framebuffer.frag");
  shader = ccCreateShaderProgramFromSource(fs, CC_DEFAULT_VERTEX_SHADER);
}

void loop()
{
  FBO_Bind(&myFbo);
  ccClearWindow(CC_RED);
  ccSetColor(CC_YELLOW);
  ccDrawQuad(ccGetWidth() * 0.25, ccGetHeight() * 0.25, ccGetWidth() * 0.5,
             ccGetHeight() * 0.5);
  FBO_Unbind();

  ccUseShader(shader);
  ccSetUniformTexture(shader, "tex0", myFbo.texture);
  ccSetUniform2f(shader, "res", ccGetWidth(), ccGetHeight());
  ccDrawQuad(0, 0, ccGetWidth(), ccGetHeight());
  ccResetShader();
}
