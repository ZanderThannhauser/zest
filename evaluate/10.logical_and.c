
#include <assert.h>

#include <debug.h>

#include <parser/zebu.h>

#include "9.inclusive_or.h"
#include "10.logical_and.h"

struct value* evaluate_logical_and_expression(
	struct zebu_logical_and_expression* expression)
{
	struct value* result;
	ENTER;
	
	if (expression->inner)
	{
		result = evaluate_inclusive_or_expression(expression->inner);
	}
	else if (expression->left)
	{
		TODO;
	}
	else
	{
		TODO;
	}
	
	EXIT;
	return result;
}

