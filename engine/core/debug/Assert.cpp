#include "Assert.h"
#include <cstdio>

void FE_AssertReport(const char *expr, const char *msg, const char *file, i32 line) {
  fprintf(stderr, "[ASSERT] %s\n  Expression : %s\n  File : %s:%d\n", msg, expr, file, line);
}
