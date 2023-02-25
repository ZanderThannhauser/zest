
#include <debug.h>

#include <parser/zebu.h>

#include <value/compare.h>
#include <value/free.h>

#include "5.relational.h"
#include "6.equality.h"

struct value* evaluate_equality_expression(
	struct zebu_equality_expression* expression)
{
	struct value* result;
	ENTER;
	
	if (expression->inner)
	{
		result = evaluate_relational_expression(expression->inner);
	}
	else if (expression->left)
	{
		struct value* left = evaluate_equality_expression(expression->left);
		
		if (left)
		{
			struct value* right = evaluate_relational_expression(expression->right);
			
			if (right)
			{
				int cmp = compare_values(left, right);
				
				TODO;
			}
			
			free_value(right);
		}
		
		free_value(left);
	}
	else
	{
		TODO;
	}
	
	EXIT;
	return result;
}

