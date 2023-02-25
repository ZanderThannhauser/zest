
#include <assert.h>
#include <debug.h>

#include <parser/zebu.h>

#include <misc/unescape.h>

#include <value/string/new.h>

#include <value/int/new.h>

#include "0.primary.h"

struct value* evaluate_primary_expression(
	struct zebu_primary_expression* expression)
{
	struct value* result;
	ENTER;
	
	if (expression->sub)
	{
		TODO;
	}
	else if (expression->string)
	{
		struct unescaped content = unescape(expression->string);
		
		dpvsn(content.chars, content.n);
		
		result = new_string_value(content.chars, content.n);
	}
	else if (expression->integer)
	{
		result = new_int_value_from_str((char*) expression->integer->data);
	}
	else
	{
		TODO;
	}
	
	EXIT;
	return result;
}
















