
#include <assert.h>

#include <debug.h>

#include <parser/zebu.h>

#include "11.logical_or.h"
#include "expression.h"

struct value* evaluate_conditional_expression(
	struct zebu_conditional_expression* expression)
{
	struct value* result;
	ENTER;
	
	if (expression->truecase)
	{
		TODO;
	}
	else
	{
		result = evaluate_logical_or_expression(expression->inner);
	}
	
	EXIT;
	return result;
}

