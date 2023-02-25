
#include <gmp.h>
#include <assert.h>

#include <debug.h>

#include <parser/zebu.h>

#include <value/free.h>
#include <value/struct.h>
#include <value/int/struct.h>

#include "forms/file.h"
#include "forms/shell.h"

#include "0.primary.h"
#include "1.prefix.h"

struct value* evaluate_prefix_expression(
	struct zebu_prefix_expression* expression)
{
	struct value* result;
	ENTER;
	
	if (expression->inner)
	{
		result = evaluate_primary_expression(expression->inner);
	}
	else if (expression->file)
	{
		result = evaluate_file_form(expression->file);
	}
	else if (expression->shell)
	{
		result = evaluate_shell_form(expression->shell);
	}
	else if (expression->sub)
	{
		if (expression->logneg)
		{
			TODO;
		}
		else if (expression->numneg)
		{
			result = evaluate_prefix_expression(expression->sub);
			
			if (result->kind == vk_int)
			{
				struct int_value* spef = (void*) result;
				
				mpz_neg(spef->mpz, spef->mpz);
			}
			else
			{
				free_value(result);
				
				result = NULL;
			}
		}
		else if (expression->bitneg)
		{
			TODO;
		}
		else
		{
			result = evaluate_prefix_expression(expression->sub);
		}
	}
	else
	{
		TODO;
	}
	
	EXIT;
	return result;
}

















