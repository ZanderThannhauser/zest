
#include <assert.h>

#include <debug.h>

#include "inheritance.h"
#include "struct.h"
#include "compare.h"

int compare_values(
	const struct value* A,
	const struct value* B)
{
	HERE;
	
	if (A->kind < B->kind)
		return -1;
	else if (A->kind > B->kind)
		return +1;
	else
	{
		dpv(A->kind);
		
		HERE;
		
		dpv(A->inheritance);
		
		assert(A->inheritance);
		
		HERE;
		
		dpv(A->inheritance->compare);
		
		assert(A->inheritance->compare);
		
		HERE;
		return (A->inheritance->compare)(A, B);
	}
}

