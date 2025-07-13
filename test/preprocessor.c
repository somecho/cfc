#define SOYA_NO_MAIN

#include "../extras/preprocessor.h"
#include "../soyalib.h"
#include <assert.h>

int main()
{
  { // Pass Through
    char *buf = NULL;
    const char *expected = syReadFile("resources/basic.frag");
    int res = syPreprocess("resources/basic.frag", &buf);
    assert(res == 0);
    assert(strcmp(buf, expected) == 0);
    free(buf);
    free((void *)expected);
  }
  { // Comment
    char *buf = NULL;
    const char *expected = syReadFile("resources/basic.frag");
    int res = syPreprocess("resources/comment.frag", &buf);
    assert(res == 0);
    assert(strcmp(buf, expected) == 0);
    free(buf);
    free((void *)expected);
  }
  { // Single Include
    char *buf = NULL;
    const char *expected = syReadFile("resources/hash.glsl");
    int res = syPreprocess("resources/singleinclude.frag", &buf);
    assert(res == 0);
    assert(strcmp(buf, expected) == 0);
    free(buf);
    free((void *)expected);
  }
  { // Commented include
    char *buf = NULL;
    int res = syPreprocess("resources/commentedInclude.glsl", &buf);
    char expected[] = "void main(){}\n";
    assert(strcmp(buf, expected) == 0);
    assert(res == 0);
    free(buf);
  }
  {
    // 2 includes
    char *buf = NULL;
    int res = syPreprocess("resources/doubleInclude.glsl", &buf);
    const char *expected = syReadFile("resources/doubleHash.glsl");
    assert(res == 0);
    assert(strcmp(buf, expected) == 0);
    free(buf);
    free((void *)expected);
  }

  return 0;
}
