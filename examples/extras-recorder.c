#include "../extras/soya_recorder.h"

#include <stdio.h>
#include <assert.h>
#include <pthread.h>

LFQ q = {0};

bool consuming = false;

void *consumer()
{
  while (consuming || q.count > 0)
  {
    float *f = LFQ_Consume(&q);
    printf("CONSUMED: %f\n", *f);
    free(f);
  }
  return NULL;
}

int main()
{
  LFQ_Init(&q);

  consuming = true;
  pthread_t t;
  pthread_create(&t, NULL, consumer, NULL);

  for (int i = 0; i < 100; i++)
  {
    float *f = calloc(1, sizeof(float));
    *f = (float)i;
    printf("PRODUCING: %f\n", *f);
    LFQ_Produce(&q, f);
  }

  consuming = false;
  pthread_join(t, NULL);

  return 0;
}
