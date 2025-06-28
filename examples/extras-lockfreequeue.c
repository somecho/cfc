//
// Example: extras-lockfreequeue.c
// Description:
// Example usage of the threadsafe lock-free queue syLFQ.
//

#include "../extras/lockfreequeue.h"

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <threads.h>
#include <unistd.h>

bool consuming = false;

void *consumer(void *arg)
{
  syLFQ *q = arg;
  while (consuming || atomic_load(&q->count) > 0)
  {
    float *f = syLFQConsume(q);
    if (f)
    {
      printf("CONSUMED: %f\n", *f);
      free(f);
    }
  }
  return NULL;
}

int main()
{
  syLFQ q;
  syLFQInit(&q);

  consuming = true;

  pthread_t t;
  pthread_create(&t, NULL, consumer, &q);

  for (int i = 0; i < 10; i++)
  {
    float *f = calloc(1, sizeof(float));
    *f = (float)i;
    printf("PRODUCING: %f\n", *f);
    syLFQProduce(&q, f);
    sleep(1);
  }

  consuming = false;
  pthread_join(t, NULL);

  return 0;
}
