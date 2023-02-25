
#include <string.h>
#include <gmp.h>
#include <assert.h>

#include <debug.h>

#include <cmdln/verbose.h>

#include <stringtree/new.h>
#include <stringtree/append_printf.h>
#include <stringtree/append_tree.h>
#include <stringtree/stream.h>
#include <stringtree/free.h>

#include <value/print.h>

#include <memory/smalloc.h>

#include <parser/zebu.h>

#include <value/struct.h>
#include <value/free.h>
#include <value/string/struct.h>
#include <value/string/new.h>
#include <value/int/struct.h>
#include <value/int/new.h>

#include <defines/argv0.h>

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
		struct value* left = evaluate_multiplicative_expression(expression->left);
		
		struct value* right = evaluate_prefix_expression(expression->right);
		
		if (!left || !right)
			result = NULL;
		else if (left->kind != right->kind)
			fprintf(stderr, "%s: incompatiable types for + operator!\n", argv0),
			result = NULL;
		else switch (left->kind)
		{
			case vk_int:
			{
				struct int_value* spef_left = (void*) left;
				struct int_value* spef_right = (void*) right;
				
				mpz_t mul;
				mpz_init(mul);
				if (expression->mul)
					mpz_mul(mul, spef_left->mpz, spef_right->mpz);
				else if (expression->div)
					mpz_div(mul, spef_left->mpz, spef_right->mpz);
				else
					mpz_fdiv_q(mul, spef_left->mpz, spef_right->mpz);
				
				result = new_int_value_from_mpz(mul);
				
				mpz_clear(mul);
				break;
			}
			
			case vk_string:
			{
				struct string_value* spef_left = (void*) left;
				struct string_value* spef_right = (void*) right;
				
				unsigned len = spef_left->len + spef_right->len;
				
				char *chars = smalloc(len + 1);
				
				memcpy(chars, spef_left->chars, spef_left->len);
				memcpy(chars + spef_left->len, spef_right->chars, spef_right->len);
				chars[len] = 0;
				
				result = new_string_value(chars, len);
				break;
			}
			
			default:
				TODO;
				break;
		}
		
		if (verbose)
		{
			struct stringtree* s = new_stringtree();
			
			struct stringtree* l = value_print(left);
			struct stringtree* r = value_print(right);
			struct stringtree* e = value_print(result);
			
			stringtree_append_printf(s, "%s: ", argv0);
			stringtree_append_tree(s, l);
			if (expression->mul)
				stringtree_append_printf(s, " * ");
			else if (expression->div)
				stringtree_append_printf(s, " / ");
			else
				stringtree_append_printf(s, " %% ");
			stringtree_append_tree(s, r);
			stringtree_append_printf(s, " == ");
			stringtree_append_tree(s, e);
			stringtree_append_printf(s, "\n");
			
			stringtree_stream(s, stdout);
			
			free_stringtree(l), free_stringtree(r), free_stringtree(e);
			free_stringtree(s);
		}
		
		free_value(left), free_value(right);
	}
	else
	{
		TODO;
	}
	
	EXIT;
	return result;
}

