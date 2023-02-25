
#include <assert.h>

#include <debug.h>

#include <parser/zebu.h>

#include "6.equality.h"
#include "7.and.h"

struct value* evaluate_and_expression(
	struct zebu_and_expression* expression)
{
	struct value* result;
	ENTER;
	
	if (expression->inner)
	{
		result = evaluate_equality_expression(expression->inner);
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

