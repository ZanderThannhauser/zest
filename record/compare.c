
#include <string.h>

#include <debug.h>

#include <string/compare.h>

#include "struct.h"
#include "compare.h"

int compare_records(const void* a, const void* b)
{
	const struct record *A = a, *B = b;
	
	return compare_strings(A->path, B->path) ?: A->index - B->index;
}

