
#include <debug.h>

#include "inheritance.h"
#include "struct.h"
#include "free.h"

void free_value(
	struct value* this)
{
	ENTER;
	
	if (this && !--this->refcount)
	{
		assert(this->inheritance);
		assert(this->inheritance->free);
		
		(this->inheritance->free)(this);
		
		free(this);
	}
	
	EXIT;
}
