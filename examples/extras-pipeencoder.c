//
//
//

#include "../extras/pipeencoder.h"

#define SY_NO_CONFIGURE
#include "../soyalib.h"

syPipeEncoder encoder;

void setup(syApp *app)
{
  syPipeEncoderOptions opts = {0};
  opts.width = app->width;
  opts.height = app->height;
  opts.codec = "h264_nvenc";
  opts.inputFps = 120;
  opts.outputFps = 60;
  opts.outputPath = "../../pipeencoder-example.mp4";
  opts.inputPixelFormat = "rgb24";
  opts.outputPixelFormat = "yuv420p";
  syPipeEncoderInit(&encoder, &opts);
}

void loop(syApp *app)
{
  syClear(SY_RED);
  syTranslate(app, app->width / 2., app->height / 2., 0);
  float r = (sinf(app->frameNum / 120.0) * 0.5 + 0.5) * 250 + 100;
  syDrawPolygon(app, 0, 0, 0, r, 72);
  syResetTransformations(app);

  if (app->frameNum == 0)
  {
    syPipeEncoderStart(&encoder);
  }
  else if (app->frameNum >= 120 * 10)
  {
    syPipeEncoderStop(&encoder);
    glfwSetWindowShouldClose(app->window, true);
  }
  else
  {
    void *pixels = calloc(app->width * app->height * 3, sizeof(int));
    glReadPixels(0, 0, app->width, app->height, GL_RGB, GL_UNSIGNED_BYTE,
                 pixels);
    syPipeEncoderEncode(&encoder, pixels);
  }
}
