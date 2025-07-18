#define SOYA_NO_CONFIGURE
#include <soya/soya.h>

// clang-format off
static const char * fs = SYSL(
    SYSL_VERSION(430)
    SYSL_UNIFORM_VEC2(res)
    SYSL_UNIFORM_FLOAT(time)
    SYSL_OUT_VEC4(FragColor)
    SYSL_MAIN(
        vec2 uv = gl_FragCoord.xy / res;
        float c = sin(time) * 0.5 + 0.5;
        FragColor = vec4(uv, sin(time + uv.x * 3.14) * 0.5 + 0.5, 1.);
    )
);
// clang-format on

syShader shader;

void setup(syApp *app) { shader = syShaderProgramLoadFromSource(fs, NULL); }

void loop(syApp *app) {
  syBeginShader(app, shader);
  syShaderUniform1f(shader, "time", app->frameNum / 120.f);
  syShaderUniform2f(shader, "res", app->width, app->height);
  syDrawQuad(app, 0, 0, app->width, app->height);
  syEndShader(app);
}
