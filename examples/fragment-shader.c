//
// Example: 05-fragment-shader.c
// Description:
// This example shows how to use your own fragment shader and pass uniforms to
// it.
//

#include <soya/soya.h>

syShader shader;

void configure(syApp *app) { (void)app; }

void setup(syApp *app) {
  (void)app;
  char shaderPath[1024];
  sprintf(shaderPath, "%s%s", RESOURCE_DIR, "/example05.frag");
  const char *fsSource = syReadFile(shaderPath);
  shader = syShaderProgramLoadFromSource(fsSource, NULL);
  free((void *)fsSource);
}

void loop(syApp *app) {
  syClear(SY_BLACK);
  syBeginShader(app, shader);
  syShaderUniform1f(shader, "time", glfwGetTime());
  syShaderUniform2f(shader, "res", app->width, app->height);
  syDrawQuad(app, 0, 0, app->width, app->height);
  syEndShader(app);
}
