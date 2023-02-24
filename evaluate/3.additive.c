
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
			case vk_integer:
				TODO;
				break;
			
			case vk_string:
			{
				struct string_value* spef_left = (void*) left;
				struct string_value* spef_right = (void*) right;
				
				unsigned len = spef_left->len + spef_right->len;
				uint8_t *data = malloc(len + 1);
				
				if (!data)
					fprintf(stderr, "%s: malloc(%u): %m\n", argv0, len),
					result = NULL;
				else
				{
					memcpy(data, spef_left->data, spef_left->len);
					memcpy(data + spef_left->len, spef_right->data, spef_right->len);
					data[len] = 0;
					
					result = new_string_value(data, len);
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



















