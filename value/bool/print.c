
#include <debug.h>

#include <stringtree/new.h>
#include <stringtree/append_printf.h>

#include "struct.h"
#include "print.h"

struct stringtree* bool_value_print(
	const struct value* super)
{
	const struct bool_value* spef = (const void*) super;
	
	struct stringtree* tree = new_stringtree();
	
	stringtree_append_printf(tree, spef->value ? "true" : "false");
	
	return tree;
}

