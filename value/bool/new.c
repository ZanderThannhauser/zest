
#include <debug.h>

#include "../new.h"

#include "struct.h"
#include "inheritance.h"
#include "new.h"

struct value* new_bool_value(
	bool value)
{
	ENTER;
	
	struct bool_value* this = (void*) new_value(
		vk_bool,
		&bool_value_inheritance,
		sizeof(*this));
	
	this->value = value;
	
	EXIT;
	return (void*) this;
}

