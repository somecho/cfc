#include "../cfc.h"
#include <math.h>
#include <time.h>

#define NUM_PARTICLES 50000
#define PRINT_FPS false
#define USE_PERLIN_NOISE true

static float pos[NUM_PARTICLES * 3];
static float heading[NUM_PARTICLES];
static float speed[NUM_PARTICLES];
static uint8_t age[NUM_PARTICLES];
static uint32_t maxAge[NUM_PARTICLES];
static float colors[NUM_PARTICLES * 4];

static const float particleColor[4] = {CC_WHITE};

void randomizeParticle(size_t i)
{
  pos[i * 3] = ((float)rand() / (float)RAND_MAX) * ccGetWidth();
  pos[i * 3 + 1] = ((float)rand() / (float)RAND_MAX) * ccGetHeight();
  heading[i] = ((float)rand() / (float)RAND_MAX) * M_PI * 2.f;
  speed[i] = ((float)rand() / (float)RAND_MAX) * 2.f + 1.f;
  age[i] = 0;
  maxAge[i] = rand() % 200;
}

void updateParticles()
{
  float noisef = 0.005f;
  float t = glfwGetTime() * 0.5f;
  for (size_t i = 0; i < NUM_PARTICLES; i++)
  {
    if (age[i] < maxAge[i])
    {
      pos[i * 3] += cosf(heading[i]) * speed[i];
      pos[i * 3 + 1] += sinf(heading[i]) * speed[i];
#if USE_PERLIN_NOISE
      vec3 n = {pos[i * 3] * noisef, pos[i * 3 + 1] * noisef, t};
      heading[i] = glm_perlin_vec3(n) * M_PI * 2;
#else
      heading[i] += speed[i] * 0.01;
#endif
      age[i]++;
    }
    else
    {
      randomizeParticle(i);
    }
  }
}

void setup()
{
  srand(time(NULL));
  for (size_t i = 0; i < NUM_PARTICLES; i++)
  {
    memcpy(&colors[i * 4], particleColor, sizeof(particleColor));
    randomizeParticle(i);
    pos[i * 3 + 2] = 0;
  }
}

void loop()
{
  ccClearWindow(0, 0, 0, 1.0);
  ccGeometry g = {
      .vertices = pos, .colors = colors, .numVertices = NUM_PARTICLES};
  ccDrawGeometryUnindexed(&g, GL_POINTS);
  if (PRINT_FPS)
  {
    printf("%f\n", ccGetFps());
  }
  updateParticles();
}
