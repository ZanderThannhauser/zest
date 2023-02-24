
#include <debug.h>

#include <parser/zebu.h>

#include "1.prefix.h"
#include "2.multiplicative.h"

struct value* evaluate_multiplicative_expression(
	struct zebu_multiplicative_expression* expression)
{
	struct value* result;
	ENTER;
	
	if (expression->inner)
	{
		result = evaluate_prefix_expression(expression->inner);
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

