
#include <debug.h>

#include <avl/free_tree.h>

#include "struct.h"
#include "free.h"

void free_strset(
	struct strset* this)
{
	ENTER;
	
	avl_free_tree(this->tree);
	
	free(this);
	
	EXIT;
}

