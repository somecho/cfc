#include "common.h"
#include <soya/lib/vec.h>

TEST(vec_push3) {
  syVec(int) nums;
  syVecInit(nums, int);
  syVecPush3(nums, 1, 2, 3);
  EXPECT(nums.data[0] == 1);
  EXPECT(nums.data[1] == 2);
  EXPECT(nums.data[2] == 3);
  EXPECT(nums.len == 3);
  syVecDestroy(nums);
  return (TestStatus){.result = TEST_SUCCESS};
}
