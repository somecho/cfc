/**
 * @file vec.h
 *
 * @brief `vec.h` contains a collection of macros to make working with
 * dynamically-sized generically-typed containers in C easier.
 *
 * Basic usage involves declaring and initializing a vector with @ref syVec and
 * @ref syVecInit respectively. Multiple macros are provided for pushing
 * elements into vectors, differing in the number of elements to be pushed at
 * any time, e.g. @ref syVecPush2, @ref syVecPush3. Elements of other vectors or
 * arrays can also be copied into the vector with @ref syVecPushVec and @ref
 * syVecPushArr. Once the vector is no longer needed, it can be cleaned up with
 * @ref syVecDestroy.
 *
 * Since vectors are allocated on the heap, they must be explicitly initialized
 * and destroyed exactly once in their lifetimes.
 *
 * ### Example
 * @include vectors.c
 * */

#ifndef _SOYA_VEC_H
#define _SOYA_VEC_H

#include <stdlib.h>
#include <stdio.h>

/**
 * Declares a vector-type struct.
 * @param type The type of the elements this vector will contain
 * */
#define syVec(type)  \
  struct {           \
    size_t len, cap; \
    type *data;      \
  }

/**
 * Initializes the vector to have space by-default for 4 elements. Must be
 * called before the vector can be used.
 * @param v Uninitialized vector defined with @ref syVec
 * @param type The type of the elements this vector contains
 * @sa syVecDestroy
 * */
#define syVecInit(v, t)                         \
  do {                                          \
    (v).len = 0;                                \
    (v).cap = 4;                                \
    (v).data = (t *)calloc((v).cap, sizeof(t)); \
  } while (0)

/**
 * Frees the pointer that the vector holds pointing to raw data. Becareful when
 * storing pointers in vectors. They must be freed before this macro is called
 * or they will dangle and leak memory.
 * */
#define syVecDestroy(v) \
  do {                  \
    free((v).data);     \
    (v).data = NULL;    \
  } while (0)

/**
 * Pushes an element into the vector. The vector and the item must have the
 * same type.
 * @param v The vector into which elements will be pushed
 * @param val The element to be pushed into the vector
 * */
#define syVecPush(v, val)                                              \
  do {                                                                 \
    if ((v).len + 1 >= (v).cap) {                                      \
      (v).data = realloc((v).data, (v).cap * 2 * sizeof((v).data[0])); \
      (v).cap *= 2;                                                    \
    }                                                                  \
    (v).data[(v).len] = (val);                                         \
    (v).len++;                                                         \
  } while (0)

/**
 * Pushes 2 elements into the vector. The vector and all elements
 * must have the same type.
 * @param v The vector into which elements will be pushed
 * @param v1 The first element to be pushed into the vector
 * @param v2 The second element to be pushed into the vector
 * */
#define syVecPush2(v, v1, v2) \
  do {                        \
    syVecPush((v), (v1));     \
    syVecPush((v), (v2));     \
  } while (0)

/**
 * Pushes 3 elements into the vector. The vector and all elements
 * have the same type.
 * @param v The vector into which elements will be pushed
 * @param v1 The first element to be pushed into the vector
 * @param v2 The second element to be pushed into the vector
 * @param v3 The third element to be pushed into the vector
 * */
#define syVecPush3(v, v1, v2, v3) \
  syVecPush2((v), (v1), (v2));    \
  syVecPush((v), (v3));

/**
 * Pushes 4 elements into the vector. The vector and all elements
 * have the same type.
 * @param v The vector into which elements will be pushed
 * @param v1 The first element to be pushed into the vector
 * @param v2 The second element to be pushed into the vector
 * @param v3 The third element to be pushed into the vector
 * @param v4 The fourth element to be pushed into the vector
 * */
#define syVecPush4(v, v1, v2, v3, v4) \
  syVecPush3((v), (v1), (v2), (v3));  \
  syVecPush((v), (v4));

/**
 * Pushes all elements from another vector defined with @ref syVec into vector.
 * Both vectors must have the same type.
 * @param v The vector into which elements will be pushed
 * @param w The other vector from which elements are pushed
 * */
#define syVecPushVec(v, w)                 \
  do {                                     \
    for (size_t i = 0; i < (w).len; i++) { \
      syVecPush((v), (w).data[i]);         \
    }                                      \
  } while (0)

/**
 * Pushes all elements from an array into the vector. Both vector and array must
 * have the same type.
 * @param v The vector into which elements will be pushed
 * @param arr The array from which elements are pushed
 * @param len The number of elements in `arr`
 * */
#define syVecPushArr(v, arr, len)        \
  do {                                   \
    for (size_t i = 0; i < (len); i++) { \
      syVecPush((v), (arr)[i]);          \
    }                                    \
  } while (0)

/**
 * Prints length and capacity of the vector. Each element is also printed using
 * the format string provided.
 * @param fmt The format string to print each element with
 * @param v Vector defined with @ref syVec
 * */
#define syVecDBG(fmt, v)                   \
  do {                                     \
    printf("Length: %zu\n", (v).len);      \
    printf("Capacity: %zu\n", (v).cap);    \
    for (size_t i = 0; i < (v).len; i++) { \
      printf(fmt, (v).data[i]);            \
    }                                      \
    printf("\n");                          \
  } while (0)

/**
 * @param v Vector defined with @ref syVec
 * @returns total size bytes of all elements in vec
 * */
#define syVecSizeb(v) (v).len == 0 ? 0 : sizeof(v.data[0]) * (v).len

#endif  // _SOYA_VEC_H
