//
// Example: 06-framebuffer.c
// Description:
// A framebuffer object (fbo) allows rendering offscreen. This example shows how
// to use the `syFbo` struct.
//

#define SOYA_NO_CONFIGURE

#include <soya/soya.h>

syFbo fbo;

void setup(syApp *app) {
  // This function is called to initialize and allocate resources for the FBO.
  syFboOptions opts = {0};
  opts.width = app->width;
  opts.height = app->height;
  opts.internalFormat = GL_RGB;
  opts.type = GL_UNSIGNED_INT;

  fbo = syFboCreate(&opts);
}

void loop(syApp *app) {
  syFboBegin(&fbo);  // Begin drawing to the FBO

  syClear(SY_CYAN);
  sySetColor(app, SY_MAGENTA);

  int numPts = 80;
  syVec(float) pts;
  syVecInit(pts, float);
  for (int i = 0; i < numPts; i++) {
    float x = (i / (float)numPts) * app->width;
    float y =
        (app->height * 0.5) + sinf((float)i * 0.5 + app->time * 3.0) * 200;
    syVecPush3(pts, x, y, 0);
  }
  syDrawLines(app, pts.data, pts.len / 3);
  syVecDestroy(pts);

  syFboEnd();  // End drawing to the FBO

  syDrawFbo(app, &fbo);  // Draw the FBO to the screen
}
