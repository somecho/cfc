//
// Example: data-structures.c
// Description:
// Soya has some simple data structures to help with some tasks you might need
// to do often. Data structures shown here:
// - Generic Array
//

#define SY_NO_CONFIGURE

#include <soya/soya.h>

syVec(float) nums;

void setup(syApp *app) { syVecInit(nums, float); }

void loop(syApp *app) {
  syVecPush(nums, app->frameNum);
  if (app->frameNum == 12) {
    syVecDBG("%f ", nums);
    syVecDestroy(nums);
    glfwSetWindowShouldClose(app->window, true);
  }
}
