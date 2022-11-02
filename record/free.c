
#include <debug.h>

#include "struct.h"
#include "free.h"

void free_record(void* ptr)
{
	ENTER;
	
	struct record* this = ptr;
	
	dpvs(this->path);
	
	free(this->path);
	free(this);
	
	EXIT;
}

