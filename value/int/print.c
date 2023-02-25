
#include <gmp.h>

#include <debug.h>

#include <string/new.h>
#include <string/free.h>

#include <stringtree/new.h>
#include <stringtree/append_string.h>

#include "struct.h"
#include "print.h"

struct stringtree* int_value_print(
	const struct value* super)
{
	const struct int_value* this = (const void*) super;
	
	HERE;
	
	char* out = NULL;
	
	gmp_asprintf(&out, "%Zi", this->mpz);
	
	dpvs(out);
	
	HERE;
	
	struct string* str = new_string(out);
	
	struct stringtree* tree = new_stringtree();
	
	stringtree_append_string(tree, str);
	
	free_string(str);
	
	return tree;
}
