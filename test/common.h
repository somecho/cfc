#pragma once

#include <stdio.h>

typedef int (*TestFn)();

typedef struct Test {
    char * title;
    TestFn fn;
} Test;


#define TEST(title) int test_ ##title ()

#define REGISTER(title) (Test){ #title, test_ ##title},

#define TESTS static const Test tests[] =

#define TEST_SUCCESS 0
#define TEST_FAIL 1

#define EXPECT(exp) if(!(exp)) return TEST_FAIL;
