
#include <debug.h>

#include <parser/zebu.h>

#include "4.shift.h"
#include "5.relational.h"

struct value* evaluate_relational_expression(
	struct zebu_relational_expression* expression)
{
	struct value* result;
	ENTER;
	
	if (expression->inner)
	{
		result = evaluate_shift_expression(expression->inner);
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

