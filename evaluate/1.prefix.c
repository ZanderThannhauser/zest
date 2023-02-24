
#include <debug.h>

#include <parser/zebu.h>

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
		TODO;
	}
	else
	{
		TODO;
	}
	
	EXIT;
	return result;
}

