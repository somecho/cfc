#include "../cfc.h"

GLuint shader;

void setup()
{
  const char *fsSource = ccReadFile("../resources/fragmentShader.glsl");
  shader = ccCreateShaderProgramFromSource(fsSource, NULL);
  free((void *)fsSource);
}

void loop()
{
  ccClearWindow(CC_BLACK);
  ccUseShader(shader);
  ccSetUniform1f(shader, "time", glfwGetTime());
  ccSetUniform2f(shader, "res", ccGetWidth(), ccGetHeight());
  ccDrawQuad(0, 0, ccGetWidth(), ccGetHeight());
  ccResetShader();
}
