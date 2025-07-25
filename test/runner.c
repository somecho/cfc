#include "tests.h"
#include <stdio.h>

int main() {
  size_t numTests = sizeof(tests) / sizeof(tests[0]);
  printf("Running %zu tests\n", numTests);
  for (size_t i = 0; i < numTests; i++) {
    TestStatus status = tests[i].fn();
    char* result = status.result == TEST_SUCCESS ? "PASS" : "FAIL";
    printf("  %s %s\n", result, tests[i].title);
    if (status.message != NULL) {
      printf("    %s\n", status.message);
    }
  }
  return 0;
}
