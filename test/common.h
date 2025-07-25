#pragma once

#include <stdio.h>

typedef enum TestResult { TEST_SUCCESS, TEST_FAIL } TestResult;

typedef struct TestStatus {
  char* message;
  TestResult result;
} TestStatus;

typedef TestStatus (*TestFn)();

typedef struct Test {
  char* title;
  TestFn fn;
} Test;

#define TEST(title) TestStatus test_##title()

#define REGISTER(title) (Test){#title, test_##title},

#define TESTS static const Test tests[] =

#define TEST_MESSAGE(exp) "Expression: " #exp " failed"

#define EXPECT(exp)                                                         \
  if (!(exp)) {                                                             \
    return (TestStatus){.message = TEST_MESSAGE(exp), .result = TEST_FAIL}; \
  }
