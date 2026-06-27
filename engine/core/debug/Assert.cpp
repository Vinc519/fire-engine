#include "Assert.h"

#include <cstdio>
#include <cstring>

void FE_AssertReport(const char *expr, const char *msg, const char *file, i32 line)
{
	fprintf(stderr, "[ASSERT] %s\n  Expression : %s\n  File : %s:%d\n", 
			msg,
			expr,
	        strstr(file, "fire-engine") + 12,
			line
	);
}
