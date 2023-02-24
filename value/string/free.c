
#include <debug.h>

#include "struct.h"
#include "free.h"

void free_string_value(
	struct value* super)
{
	struct string_value* this = (void*) super;
	ENTER;
	
	free(this->data);
	
	EXIT;
}

