
#include <assert.h>

#include <debug.h>

#include <parser/zebu.h>

#include "8.exclusive_or.h"
#include "9.inclusive_or.h"

struct value* evaluate_inclusive_or_expression(
	struct zebu_inclusive_or_expression* expression)
{
	ENTER;
	
	struct value* result;
	
	if (expression->inner)
	{
		result = evaluate_exclusive_or_expression(expression->inner);
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

