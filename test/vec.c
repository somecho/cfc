#include <soya/lib/vec.h>

#include <assert.h>

int main() {
  syVec(int) nums;
  syVecInit(nums, int);
  syVecPush3(nums, 1, 2, 3);
  assert(nums.data[0] == 1);
}
