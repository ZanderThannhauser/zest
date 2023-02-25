
#include <stdlib.h>

#include <debug.h>

#include "struct.h"
#include "free.h"

void free_string(void* ptr)
{
	ENTER;
	
	struct string* this = ptr;
	
	if (!--this->refcount)
	{
		free(this->chars);
		free(this);
	}
	
	EXIT;
}

