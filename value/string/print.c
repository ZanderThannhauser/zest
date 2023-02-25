
#include <debug.h>

#include <misc/escape.h>

#include <string/new.h>
#include <string/free.h>

#include <stringtree/new.h>
#include <stringtree/append_string.h>
#include <stringtree/free.h>

#include "struct.h"
#include "print.h"

struct stringtree* string_value_print(
	const struct value* super)
{
	HERE;
	
	const struct string_value* spef = (const void*) super;
	
	dpv(spef->len);
	
	char* chars = escape(spef->chars, spef->len);
	
	struct string* str = new_string(chars);
	
	struct stringtree* tree = new_stringtree();
	
	stringtree_append_string(tree, str);
	
	free_string(str);
	
	return tree;
}












