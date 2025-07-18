#include <soya/lib/vec.h>

/*
This example prints:
Length: 10
Capacity: 16
1.500000 2.500000 2.600000 2.700000 0.000000 0.000000
0.000000 1.000000 2.000000 3.000000
*/
int main() {
  syVec(float) center;
  syVecInit(center, float);
  syVecPush3(center, 0., 0., 0.);

  syVec(float) nums;
  syVecInit(nums, float);

  float normal[3] = {1, 2, 3};

  syVecPush(nums, 1.5);
  syVecPush3(nums, 2.5, 2.6, 2.7);
  syVecPushVec(nums, center);
  syVecPushArr(nums, normal, 3);
  syVecDBG("%f ", nums);
  syVecDestroy(center);
  syVecDestroy(nums);
}
