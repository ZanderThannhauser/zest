
#include <debug.h>

#include "../new.h"

#include "struct.h"
#include "inheritance.h"
#include "new.h"

struct value* new_int_value(
	int value)
{
	ENTER;
	
	struct int_value* this = (void*) new_value(
		vk_int,
		&int_value_inheritance,
		sizeof(*this));
	
	this->value = value;
	
	EXIT;
	return (void*) this;
}

