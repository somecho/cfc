#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>

#ifndef SOYA_LFQ_H_
#define SOYA_LFQ_H_

// Internal structure for singly linked-list.
typedef struct _Node _Node;

// Lock-Free Queue for generic data implemented with as a singly linked-list.
typedef struct syLFQ syLFQ;

// Initialized the LFQ. This function is threadsafe.
static void syLFQInit(syLFQ *q);

// Creates an element to hold `data` and inserts it into the LFQ. Threadsafe.
static void syLFQProduce(syLFQ *q, void *data);

// Consumes a node and removes it from the LFQ.
// @returns the data the node is pointing to.
static void *syLFQConsume(syLFQ *q);

// Consumes all remaining elements in the LFQ, frees them and the data they
// point to.
static void syLFQDestroy(syLFQ *q);

typedef struct _Node
{
  void *data;
  _Atomic(_Node *) next;
} _Node;

typedef struct syLFQ
{
  _Atomic(_Node *) head;
  _Atomic(_Node *) tail;
  atomic_int count;
} syLFQ;

static void syLFQInit(syLFQ *q)
{
  _Node *dummy = (_Node *)calloc(1, sizeof(_Node));
  dummy->data = NULL;
  atomic_store(&dummy->next, NULL);
  atomic_store(&q->head, dummy);
  atomic_store(&q->tail, dummy);
  atomic_store(&q->count, 0);
}

static void syLFQProduce(syLFQ *q, void *data)
{
  // Allocate new node
  _Node *node = (_Node *)calloc(1, sizeof(_Node));
  node->data = data;
  atomic_store(&node->next, NULL);

  while (true)
  {
    _Node *tail = atomic_load_explicit(&q->tail, memory_order_acquire);
    _Node *next = atomic_load_explicit(&tail->next, memory_order_acquire);

    if (tail == atomic_load_explicit(&q->tail, memory_order_acquire))
    {
      if (next == NULL)
      {
        if (atomic_compare_exchange_weak_explicit(&tail->next, &next, node,
                                                  memory_order_release,
                                                  memory_order_relaxed))
        {
          atomic_compare_exchange_weak_explicit(&q->tail, &tail, node,
                                                memory_order_release,
                                                memory_order_relaxed);
          break;
        }
      }
      else
      {
        atomic_compare_exchange_weak_explicit(
            &q->tail, &tail, next, memory_order_release, memory_order_relaxed);
      }
    }
  }

  atomic_fetch_add_explicit(&q->count, 1, memory_order_relaxed);
}

static void *syLFQConsume(syLFQ *q)
{
  void *data = NULL;
  _Node *old_head = NULL;

  while (true)
  {
    _Node *head = atomic_load_explicit(&q->head, memory_order_acquire);
    _Node *tail = atomic_load_explicit(&q->tail, memory_order_acquire);
    _Node *next = atomic_load_explicit(&head->next, memory_order_acquire);

    if (head == atomic_load_explicit(&q->head, memory_order_acquire))
    {
      if (head == tail)
      {
        if (next == NULL)
        {
          return NULL;
        }

        atomic_compare_exchange_weak_explicit(
            &q->tail, &tail, next, memory_order_release, memory_order_relaxed);
      }
      else
      {
        if (next == NULL)
        {
          continue;
        }

        data = next->data;

        if (atomic_compare_exchange_weak_explicit(&q->head, &head, next,
                                                  memory_order_release,
                                                  memory_order_relaxed))
        {
          old_head = head;
          break;
        }
      }
    }
  }

  free(old_head);
  atomic_fetch_sub_explicit(&q->count, 1, memory_order_relaxed);
  return data;
}

void syLFQDestroy(syLFQ *q)
{
  void *data = syLFQConsume(q);
  while (data != NULL)
  {
    free(data);
    data = syLFQConsume(q);
  }
  _Node *head = atomic_load(&q->head);
  if (head != NULL)
  {
    free(head);
    head = NULL;
  }
}

#endif // SOYA_LFQ_H_
