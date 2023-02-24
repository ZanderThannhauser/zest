
#include <debug.h>

#include <avl/insert.h>

#include <string/inc.h>

#include "struct.h"
#include "add.h"

bool strset_add(
	struct strset* this,
	struct string* element)
{
	bool new;
	ENTER;
	
	if (avl_insert(this->tree, element))
		new = true, inc_string(element);
	else if (errno == EEXIST)
		new = false;
	else
	{
		TODO;
		exit(1);
	}
	
	EXIT;
	return new;
}

