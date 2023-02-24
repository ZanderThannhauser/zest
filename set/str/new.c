
#include <debug.h>

#include <avl/alloc_tree.h>

#include <string/compare.h>
#include <string/free.h>

#include "struct.h"
#include "new.h"

struct strset* new_strset()
{
	ENTER;
	
	struct strset* this = smalloc(sizeof(*this));
	
	this->tree = avl_alloc_tree(compare_strings, free_string);
	
	EXIT;
	return this;
}

