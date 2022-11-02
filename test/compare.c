
#include <debug.h>

#include "struct.h"
#include "compare.h"

int compare_tests(const void* a, const void* b)
{
	const struct test *A = a, *B = b;
	return strcmp(A->path, B->path) ?: A->index - B->index;
}

