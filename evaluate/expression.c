
#include <debug.h>

#include <parser/zebu.h>

#include "12.conditional.h"
#include "expression.h"

struct value* evaluate_expression(
	struct zebu_expression* expression)
{
	ENTER;
	
	struct value* result = evaluate_conditional_expression(expression->inner);
	
	EXIT;
	return result;
}

