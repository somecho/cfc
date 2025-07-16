#ifndef SOYA_VEC_H
#define SOYA_VEC_H

/**
 *
 * Declares a vector-type struct.
 *
 * @param type The type of the elements this vector will contain
 *
 * Example:
 * ```
 * syVec(float) positions;
 * ```
 *
 * */
#define syVec(type)  \
  struct {           \
    size_t len, cap; \
    type *data;      \
  }

/**
 *
 * Initializes the vector to have space for 4 elements. Must be called before
 * the vector can be used.
 *
 * */
#define syVecInit(v, t)                         \
  do {                                          \
    (v).len = 0;                                \
    (v).cap = 4;                                \
    (v).data = (t *)calloc((v).cap, sizeof(t)); \
  } while (0)

/**
 *
 * Prints length, capacity of the vector. Each element is also printed using the
 * format string provided.
 *
 * @param fmt The format string to print each element with
 * @param v The vector whose information is to be printed
 *
 * Example:
 * ```
 * syVecDBG("%f ", vecOfFloats);
 * ```
 *
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
 *
 * Frees the heap-allocated array the vector contains. If the vector is a vector
 * of pointers, these aren't freed! Pointers contained within vectors must be
 * individually freed or they will be left dangling when this macro is called.
 *
 * */
#define syVecDestroy(v) \
  do {                  \
    free((v).data);     \
    (v).data = NULL;    \
  } while (0)

/**
 *
 * Pushes an element in to the vector. The vector and the item but be of the
 * same type.
 *
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
 *
 * Pushes 2 elements in to the vector sequentially. The vector and all elements
 * must be of the same type.
 *
 * */
#define syVecPush2(v, v1, v2) \
  do {                        \
    syVecPush((v), (v1));     \
    syVecPush((v), (v2));     \
  } while (0)

/**
 *
 * Pushes 3 elements in to the vector sequentially. The vector and all elements
 * must be of the same type.
 *
 * */
#define syVecPush3(v, v1, v2, v3) \
  syVecPush2((v), (v1), (v2));    \
  syVecPush((v), (v3));

/**
 *
 * Pushes 4 elements in to the vector sequentially. The vector and all elements
 * must be of the same type.
 *
 * */
#define syVecPush4(v, v1, v2, v3, v4) \
  syVecPush3((v), (v1), (v2), (v3));  \
  syVecPush((v), (v4));

/**
 *
 * @returns total size bytes of all elements in vec
 *
 * */
#define syVecSizeb(v) (v).len == 0 ? 0 : sizeof(v.data[0]) * (v).len

#endif  // SOYA_VEC_H
