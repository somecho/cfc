//
// Example: extras-pipeencoder.c
// Description:
// Use the pipe encoder to record applpication for 5 seconds with ffmpeg.

//
// This header must be included at the very top because it declares POSIX
// functions, which need to be declared before any stdlib functions are
// imported.
//

#define SY_NO_CONFIGURE

#include <soya/extras/pipeencoder.h>
#include <soya/soyalib.h>

syPipeEncoder encoder;

void setup(syApp *app)
{
  syPipeEncoderOptions opts = {0}; // First initialize `opts` to 0.
  //
  // All options should be explicitly provided except for `extraInputArgs` and
  // `extraOutputArgs`.
  //
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
  else if (app->frameNum >= 120 * 5) // 5 seconds with 120 fps
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
