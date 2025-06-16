#include "../cfc.h"
#include <math.h>
#include <time.h>

#define NUM_PARTICLES 50000
#define PRINT_FPS false
#define USE_PERLIN_NOISE true

// Data structure to house all the data pertaining to the particle system.
typedef struct
{
  vec3 pos[NUM_PARTICLES];
  float heading[NUM_PARTICLES];
  float speed[NUM_PARTICLES];
  uint8_t age[NUM_PARTICLES];
  uint32_t maxAge[NUM_PARTICLES];
  uint32_t numParticles;
} Particles;

// Use white to render particles
static const float particleColor[4] = {1.0, 1.0, 1.0, 1.0};

static Particles ps;

// This method converts our particle system in to a ccGeometry, which can be
// rendered with the default renderer.
ccGeometry createGeometryFromParticles(Particles *p)
{
  // Initialize the geometry and dynamically allocate the arrays
  ccGeometry g;
  g.vertices = (float *)malloc(p->numParticles * sizeof(float) * 3 /*xyz*/);
  g.colors = (float *)malloc(p->numParticles * sizeof(float) * 4 /*rgba*/);
  g.indices = (uint32_t *)malloc(p->numParticles * sizeof(uint32_t));

  // Write the data from the particle system to the ccGeometry
  for (size_t i = 0; i < p->numParticles; i++)
  {
    memcpy(&g.vertices[i * 3], p->pos[i], sizeof(float) * 3);
    memcpy(&g.colors[i * 4], particleColor, sizeof(particleColor));
    // The default renderer requires indices, so each vertex gets its own index
    g.indices[i] = (uint32_t)i;
  }

  g.numVertices = p->numParticles;
  g.numIndices = p->numParticles;
  return g;
}

// We convert the heading and speed to a velocity and add it to the particle's
// position. Then we nudge the heading abit and increment the age. If the
// particle is older than its max age, we generate new data.
void updateParticles(Particles *p)
{
  float noisef = 0.005f;
  float t = (float)glfwGetTime() * 0.5f;
  for (size_t i = 0; i < p->numParticles; i++)
  {
    if (p->age[i] < p->maxAge[i])
    {
      vec3 velocity = {cosf(p->heading[i]) * p->speed[i],
                       sinf(p->heading[i]) * p->speed[i], 0.0};
      glm_vec3_add(velocity, p->pos[i], p->pos[i]);
#if USE_PERLIN_NOISE
      p->heading[i] = glm_perlin_vec3((vec3){p->pos[i][0] * noisef,
                                             p->pos[i][1] * noisef, t}) *
                      (float)M_PI * 2.f;
#else
      p->heading[i] += p->speed[i] * 0.01;
#endif
      p->age[i]++;
    }
    else
    {
      p->pos[i][0] = ((float)rand() / (float)RAND_MAX) * (float)ccGetWidth();
      p->pos[i][1] = ((float)rand() / (float)RAND_MAX) * (float)ccGetWidth();
      p->heading[i] = ((float)rand() / (float)RAND_MAX) * (float)M_PI * 2.f;
      p->speed[i] = ((float)rand() / (float)RAND_MAX) * 2.f + 1.f;
      p->age[i] = 0;
      p->maxAge[i] = (uint32_t)rand() % 200;
    }
  }
}

void setup()
{
  srand((uint32_t)time(NULL));
  ccSetRenderMode(GL_POINTS);
  // Initialize the particle system
  ps.numParticles = NUM_PARTICLES;
  for (size_t i = 0; i < NUM_PARTICLES; i++)
  {
    ps.pos[i][0] = ((float)rand() / (float)RAND_MAX) * (float)ccGetWidth();
    ps.pos[i][1] = ((float)rand() / (float)RAND_MAX) * (float)ccGetWidth();
    ps.pos[i][2] = 0;
    ps.heading[i] = ((float)rand() / (float)RAND_MAX) * (float)M_PI * 2.f;
    ps.speed[i] = ((float)rand() / (float)RAND_MAX) * 2.f + 1.f;
    ps.age[i] = 0;
    ps.maxAge[i] = (uint32_t)rand() % 500;
  }
}

void loop()
{
  ccClearWindow(0, 0, 0, 1.0);
  ccGeometry g = createGeometryFromParticles(&ps);
  ccDrawGeometry(&g);
  if (PRINT_FPS)
  {
    printf("%f\n", ccGetFps());
  }
  free(g.vertices);
  free(g.indices);
  free(g.colors);
  updateParticles(&ps);
}
