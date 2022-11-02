
#include <debug.h>

#include "struct.h"
#include "compare.h"

int compare_records(const void* a, const void* b)
{
	const struct record *A = a, *B = b;
	return strcmp(A->path, B->path) ?: A->index - B->index;
}

