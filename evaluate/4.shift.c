
#include <debug.h>

#include <parser/zebu.h>

#include "3.additive.h"
#include "4.shift.h"

struct value* evaluate_shift_expression(
	struct zebu_shift_expression* expression)
{
	struct value* result;
	ENTER;
	
	if (expression->inner)
	{
		result = evaluate_additive_expression(expression->inner);
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

