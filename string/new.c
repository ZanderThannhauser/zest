
#include <debug.h>

#include "struct.h"
#include "new.h"

struct string* new_string(const char* chars)
{
	ENTER;
	
	struct string* this = smalloc(sizeof(*this));
	
	unsigned len = strlen(chars);
	
	this->chars = memcpy(malloc(len + 1), chars, len + 1);
	this->len = len;
	
	this->refcount = 1;
	
	EXIT;
	return this;
}
