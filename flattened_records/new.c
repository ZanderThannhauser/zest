
#include <debug.h>

#include <memory/smalloc.h>

#include "struct.h"
#include "new.h"

struct flattened_records* new_flattened_records()
{
	ENTER;
	
	struct flattened_records* this = smalloc(sizeof(*this));
	
	this->data = NULL;
	this->n = 0;
	this->cap = 0;
	
	EXIT;
	return this;
}
