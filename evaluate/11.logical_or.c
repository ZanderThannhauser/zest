
#include <debug.h>

#include <parser/zebu.h>

#include "10.logical_and.h"
#include "11.logical_or.h"

struct value* evaluate_logical_or_expression(
	struct zebu_logical_or_expression* expression)
{
	struct value* result;
	ENTER;
	
	if (expression->inner)
	{
		result = evaluate_logical_and_expression(expression->inner);
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

