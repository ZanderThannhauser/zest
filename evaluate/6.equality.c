
#include <assert.h>

#include <debug.h>

#include <defines/argv0.h>

#include <parser/zebu.h>

#include <cmdln/verbose.h>

#include <value/bool/new.h>
#include <value/compare.h>
#include <value/print.h>
#include <value/free.h>

#include <stringtree/new.h>
#include <stringtree/append_printf.h>
#include <stringtree/append_tree.h>
#include <stringtree/stream.h>
#include <stringtree/free.h>

#include "5.relational.h"
#include "6.equality.h"

struct value* evaluate_equality_expression(
	struct zebu_equality_expression* expression)
{
	struct value* result = NULL;
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
				
				result = new_bool_value(expression->eq ? cmp == 0 : cmp != 0);
				
				if (verbose)
				{
					struct stringtree* s = new_stringtree();
					
					struct stringtree* l = value_print(left);
					struct stringtree* r = value_print(right);
					struct stringtree* e = value_print(result);
					
					stringtree_append_printf(s, "%s: ", argv0);
					stringtree_append_tree(s, l);
					if (expression->eq)
						stringtree_append_printf(s, " == ");
					else
						stringtree_append_printf(s, " != ");
					stringtree_append_tree(s, r);
					stringtree_append_printf(s, " == ");
					stringtree_append_tree(s, e);
					stringtree_append_printf(s, "\n");
					
					stringtree_stream(s, stdout);
					
					free_stringtree(l), free_stringtree(r), free_stringtree(e);
					free_stringtree(s);
				}
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













