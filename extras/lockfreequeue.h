#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>

#ifndef SOYA_LFQ_H_
#define SOYA_LFQ_H_

typedef struct _Node _Node;
typedef struct syLFQ syLFQ;

static void syLFQInit(syLFQ *q);
static void syLFQProduce(syLFQ *q, void *data);
static void *syLFQConsume(syLFQ *q);

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

    // Check if tail is still the same (avoid ABA problem)
    if (tail == atomic_load_explicit(&q->tail, memory_order_acquire))
    {
      if (next == NULL)
      {
        // Try to link node at the end of the list
        if (atomic_compare_exchange_weak_explicit(&tail->next, &next, node,
                                                  memory_order_release,
                                                  memory_order_relaxed))
        {
          // Successfully linked node, now try to swing tail to new node
          atomic_compare_exchange_weak_explicit(&q->tail, &tail, node,
                                                memory_order_release,
                                                memory_order_relaxed);
          break;
        }
      }
      else
      {
        // Tail is lagging behind, try to advance it
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

    // Check if head is still the same (avoid ABA problem)
    if (head == atomic_load_explicit(&q->head, memory_order_acquire))
    {
      if (head == tail)
      {
        if (next == NULL)
        {
          // Queue is empty
          return NULL;
        }

        // Tail is lagging behind, try to advance it
        atomic_compare_exchange_weak_explicit(
            &q->tail, &tail, next, memory_order_release, memory_order_relaxed);
      }
      else
      {
        if (next == NULL)
        {
          // This shouldn't happen in a well-formed queue
          continue;
        }

        // Read data before CAS, as another thread may free the node
        data = next->data;

        // Try to swing head to next node
        if (atomic_compare_exchange_weak_explicit(&q->head, &head, next,
                                                  memory_order_release,
                                                  memory_order_relaxed))
        {
          old_head = head; // Save the old head to free it
          break;
        }
      }
    }
  }

  // Free the old dummy node
  free(old_head);
  atomic_fetch_sub_explicit(&q->count, 1, memory_order_relaxed);

  return data;
}

#endif // SOYA_LFQ_H_
