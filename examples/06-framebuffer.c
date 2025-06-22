//
// Example: 06-framebuffer.c
// Description:
// A framebuffer object (fbo) allows rendering offscreen. This example shows how
// to use the `syFbo` struct.
//

#include "../soyalib.h"

syFbo fbo;

void configure(syApp *app)
{
}

void setup(syApp *app)
{
  // This function is called to initialize and allocate resources for the FBO.
  syFboAllocate(&fbo, app->width, app->height, GL_RGB);
}

void loop(syApp *app)
{
  syFboBegin(&fbo); // Begin drawing to the FBO

  syClear(SY_CYAN);
  sySetColor(app, SY_MAGENTA);

  i32 numPts = 80;
  syArrayF32 pts;
  syArrayF32Init(&pts);
  for (i32 i = 0; i < numPts; i++)
  {
    float x = (i / (float)numPts) * app->width;
    float y =
        (app->height * 0.5) + sinf((float)i * 0.5 + app->time * 3.0) * 200;
    syArrayPush(pts, x);
    syArrayPush(pts, y);
    syArrayPush(pts, 0);
  }
  syDrawLines(app, pts.data, pts.len / 3);
  syArrayDestroy(pts);

  syFboEnd(); // End drawing to the FBO

  syFboDraw(app, &fbo); // Draw the FBO to the screen
}
