
#include <debug.h>

#include "struct.h"
#include "compare.h"

int compare_strings(const void* a, const void* b)
{
	const struct string* A = a, *B = b;
	
	return strcmp(A->chars, B->chars);
}

