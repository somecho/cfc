/**
 *
 * Example: vectors.c
 *
 * This example shows how to use syVec, a simple dynamic array structure.
 *
 **/

#define SY_NO_CONFIGURE
#include <soya/soya.h>

syVec(float) nums;  // 1. Declaring

void setup(syApp *app) {
  syVecInit(nums, float);  // 2. Initializing
}

void loop(syApp *app) {
  syVecPush(nums, app->frameNum);  // 3. Inserting elements
  if (app->frameNum == 12) {
    syVecDBG("%f ", nums);
    syVecDestroy(nums);  // 4. Cleaning up
    glfwSetWindowShouldClose(app->window, true);
  }
}
