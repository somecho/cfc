//
// Example: 06-framebuffer.c
// Description:
// A framebuffer object (fbo) allows rendering offscreen. This example shows how
// to use the `syFbo` struct.
//

#define SY_NO_CONFIGURE

#ifdef USE_CMAKE_SOYA
#include <soya/soyalib.h>
#else
#include "../soya/soyalib.h"
#endif

syFbo fbo;

void setup(syApp *app)
{
  // This function is called to initialize and allocate resources for the FBO.
  syFboOptions opts = {0};
  opts.width = app->width;
  opts.height = app->height;
  opts.internalFormat = GL_RGB;
  opts.type = GL_UNSIGNED_INT;

  fbo = syFboCreate(&opts);
}

void loop(syApp *app)
{
  syFboBegin(&fbo); // Begin drawing to the FBO

  syClear(SY_CYAN);
  sySetColor(app, SY_MAGENTA);

  i32 numPts = 80;
  SY_DEFARRAY(pts, f32);
  syArrayInit(pts, f32);
  for (i32 i = 0; i < numPts; i++)
  {
    float x = (i / (float)numPts) * app->width;
    float y =
        (app->height * 0.5) + sinf((float)i * 0.5 + app->time * 3.0) * 200;
    syArrayPush3(pts, x, y, 0);
  }
  syDrawLines(app, pts.data, pts.len / 3);
  syArrayDestroy(pts);

  syFboEnd(); // End drawing to the FBO

  syFboDraw(app, &fbo); // Draw the FBO to the screen
}
