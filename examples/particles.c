//
// Example: 03-particles.c
// Description:
// Simple particle system in a perlin noise flow field.
//

#include <soya/soya.h>

#define NUM_PARTICLES 50000  // you may need -O3 to run at high fps

static float pos[NUM_PARTICLES * 3];
static float heading[NUM_PARTICLES];
static float speed[NUM_PARTICLES];
static uint8_t age[NUM_PARTICLES];
static uint32_t maxAge[NUM_PARTICLES];

void randomizeParticle(size_t i, int w, int h) {
  pos[i * 3] = ((float)rand() / (float)RAND_MAX) * w;
  pos[i * 3 + 1] = ((float)rand() / (float)RAND_MAX) * h;
  heading[i] = ((float)rand() / (float)RAND_MAX) * GLM_PI * 2.f;
  speed[i] = ((float)rand() / (float)RAND_MAX) * 2.f + 1.f;
  age[i] = 0;
  maxAge[i] = rand() % 200;
}

void updateParticles(int w, int h) {
  float noisef = 0.005f;
  float t = glfwGetTime() * 0.5f;
  for (size_t i = 0; i < NUM_PARTICLES; i++) {
    if (age[i] > maxAge[i]) {
      randomizeParticle(i, w, h);
      continue;
    }
    pos[i * 3] += cosf(heading[i]) * speed[i];
    pos[i * 3 + 1] += sinf(heading[i]) * speed[i];
    vec3 n = {pos[i * 3] * noisef, pos[i * 3 + 1] * noisef, t};
    heading[i] = glm_perlin_vec3(n) * GLM_PI * 2;
    age[i]++;
  }
}

void configure(syApp *app) {
  app->width = 1200;
  app->height = 800;
}

void setup(syApp *app) {
  srand(time(NULL));
  for (size_t i = 0; i < NUM_PARTICLES; i++) {
    randomizeParticle(i, app->width, app->height);
    pos[i * 3 + 2] = 0;
  }
}

void loop(syApp *app) {
  syClear(SY_BLACK);
  syDrawUnindexed(app, pos, NULL, NUM_PARTICLES, GL_POINTS);
  updateParticles(app->width, app->height);

#ifdef PRINT_FPS  // compile with -DPRINT_FPS to print the fps
  printf("%f\n", app->fps);
#endif
}
