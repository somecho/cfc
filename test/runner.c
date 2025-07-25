#include "tests.h"
#include <stdio.h>


int main() {
    size_t numTests = sizeof(tests) / sizeof(tests[0]);
    printf("Running %zu tests\n", numTests);
    for(size_t i = 0;i < numTests; i++){
        int result = tests[i].fn();
        char * status = result == TEST_SUCCESS ? "PASS" : "FAIL";
        printf("  %s %s\n",status, tests[i].title);
    }
    return 0;
}
