#define SOYA_NO_MAIN

#include <assert.h>

#include <soya/extras/preprocessor.h>
#include <soya/soya.h>

static const char *RESOURCE_DIR = SOYA_TEST_RESOURCES;

int main() {
  {  // Pass Through
    char *buf = NULL;
    char path[1024] = {0};
    snprintf(path, 1024, "%s%s", RESOURCE_DIR, "/basic.frag");
    const char *expected = syReadFile(path);
    int res = syPreprocess(path, &buf);
    assert(res == 0);
    assert(strcmp(buf, expected) == 0);
    free(buf);
    free((void *)expected);
  }
  {  // Comment
    char *buf = NULL;
    char path[1024] = {0};
    snprintf(path, 1024, "%s%s", RESOURCE_DIR, "/basic.frag");
    const char *expected = syReadFile(path);
    snprintf(path, 1024, "%s%s", RESOURCE_DIR, "/comment.frag");
    int res = syPreprocess(path, &buf);
    assert(res == 0);
    assert(strcmp(buf, expected) == 0);
    free(buf);
    free((void *)expected);
  }
  {  // Single Include
    char *buf = NULL;
    char path[1024] = {0};
    snprintf(path, 1024, "%s%s", RESOURCE_DIR, "/hash.glsl");
    const char *expected = syReadFile(path);
    snprintf(path, 1024, "%s%s", RESOURCE_DIR, "/singleinclude.frag");
    int res = syPreprocess(path, &buf);
    assert(res == 0);
    assert(strcmp(buf, expected) == 0);
    free(buf);
    free((void *)expected);
  }
  {  // Commented include
    char *buf = NULL;
    char path[1024] = {0};
    snprintf(path, 1024, "%s%s", RESOURCE_DIR, "/commentedInclude.glsl");
    int res = syPreprocess(path, &buf);
    char expected[] = "void main(){}\n";
    assert(strcmp(buf, expected) == 0);
    assert(res == 0);
    free(buf);
  }
  {
    // 2 includes
    char *buf = NULL;
    char path[1024] = {0};
    snprintf(path, 1024, "%s%s", RESOURCE_DIR, "/doubleInclude.glsl");
    int res = syPreprocess(path, &buf);
    snprintf(path, 1024, "%s%s", RESOURCE_DIR, "/doubleHash.glsl");
    const char *expected = syReadFile(path);
    assert(res == 0);
    assert(strcmp(buf, expected) == 0);
    free(buf);
    free((void *)expected);
  }

  return 0;
}
