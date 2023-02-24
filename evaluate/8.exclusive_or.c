
#include <debug.h>

#include <parser/zebu.h>

#include "7.and.h"
#include "8.exclusive_or.h"

struct value* evaluate_exclusive_or_expression(
	struct zebu_exclusive_or_expression* expression)
{
	struct value* result;
	ENTER;
	
	if (expression->inner)
	{
		result = evaluate_and_expression(expression->inner);
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

