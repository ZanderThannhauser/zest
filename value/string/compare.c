
#include <string.h>

#include <debug.h>

#include "struct.h"
#include "compare.h"

int compare_string_value(
	const struct value* a,
	const struct value* b)
{
	HERE;
	const struct string_value *A = (void*) a, *B = (void*) b;
	HERE;
	dpv(A->chars);
	dpv(B->chars);
	return strverscmp(A->chars, B->chars);
}

