
#include <string.h>

#include <debug.h>

#include <memory/smalloc.h>

#include "struct.h"
#include "new.h"

struct string* new_string(const char* chars)
{
	ENTER;
	
	struct string* this = smalloc(sizeof(*this));
	
	unsigned len = strlen(chars);
	
	this->chars = memcpy(smalloc(len + 1), chars, len + 1);
	this->len = len;
	
	this->refcount = 1;
	
	EXIT;
	return this;
}

