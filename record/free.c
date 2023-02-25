
#include <stdlib.h>

#include <debug.h>

#include <string/free.h>

#include "struct.h"
#include "free.h"

void free_record(void* ptr)
{
	ENTER;
	
	struct record* this = ptr;
	
	if (!--this->refcount)
	{
		free_string(this->path);
		
		free(this);
	}
	
	EXIT;
}

