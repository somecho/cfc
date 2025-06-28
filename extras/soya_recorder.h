#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>

#ifndef SOYA_RECORDER_H_
#define SOYA_RECORDER_H_

typedef struct Node Node;

typedef struct Node
{
  void *data;
  Node *head;
  Node *tail;
} Node;

// Lock Free Queue
typedef struct LFQ
{
  Node *head;
  Node *tail;
  atomic_int count;
} LFQ;

void LFQ_Init(LFQ *q)
{
  q->head = NULL;
  q->tail = NULL;
  q->count = 0;
}

bool LFQ_IsEmpty(LFQ *q)
{
  return q->head == NULL && q->tail == NULL;
}

void LFQ_Produce(LFQ *q, void *data)
{
  Node *n = (Node *)calloc(1, sizeof(Node));
  n->data = data;
  bool isLFQEmpty = q->head == NULL;
  if (isLFQEmpty)
  {
    n->head = NULL;
    n->tail = NULL;
    q->head = n;
    q->count++;
    return;
  }
  bool LFQHasOne = q->head != NULL && q->tail == NULL;
  if (LFQHasOne)
  {
    n->head = q->head;
    n->tail = NULL;
    q->head->tail = n;
    q->tail = n;
    q->count++;
    return;
  }

  n->head = q->tail;
  n->tail = NULL;
  q->tail->tail = n;
  q->tail = n;
  q->count++;
}

void *LFQ_Consume(LFQ *q)
{
  if (LFQ_IsEmpty(q))
  {
    return NULL;
  }
  if (q->head != NULL)
  {
    Node *n = q->head;
    void *data = n->data;
    Node *second = q->head->tail;
    if (second != NULL)
    {
      if (second == q->tail)
      {
        q->tail = NULL;
      }
      second->head = NULL;
      q->head = second;
    }
    else
    {
      q->head = NULL;
    }

    free(n);
    n = NULL;
    q->count--;
    return data;
  }
  return NULL;
}

#endif // SOYA_RECORDER_H_
