
#include <debug.h>

#include <parser/zebu.h>

#include <value/struct.h>
#include <value/string/struct.h>
#include <value/string/new.h>
#include <value/free.h>

#include "2.multiplicative.h"
#include "3.additive.h"

struct value* evaluate_additive_expression(
	struct zebu_additive_expression* expression)
{
	struct value* result;
	ENTER;
	
	if (expression->inner)
	{
		result = evaluate_multiplicative_expression(expression->inner);
	}
	else if (expression->left)
	{
		struct value* left = evaluate_additive_expression(expression->left);
		
		struct value* right = evaluate_multiplicative_expression(expression->right);
		
		if (!left || !right)
			result = NULL;
		else if (left->kind != right->kind)
			fprintf(stderr, "%s: incompatiable types for + operator!\n", argv0),
			result = NULL;
		else switch (left->kind)
		{
			case vk_int:
				TODO;
				break;
			
			case vk_string:
			{
				struct string_value* spef_left = (void*) left;
				struct string_value* spef_right = (void*) right;
				
				unsigned len = spef_left->len + spef_right->len;
				char *chars = malloc(len + 1);
				
				if (!chars)
					fprintf(stderr, "%s: malloc(%u): %m\n", argv0, len),
					result = NULL;
				else
				{
					memcpy(chars, spef_left->chars, spef_left->len);
					memcpy(chars + spef_left->len, spef_right->chars, spef_right->len);
					chars[len] = 0;
					
					result = new_string_value(chars, len);
				}
				break;
			}
			
			default:
				TODO;
				break;
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



















