
#include <assert.h>

#include <debug.h>

#include "inheritance.h"
#include "struct.h"
#include "print.h"

struct stringtree* value_print(
	const struct value* this)
{
	assert(this);
	
	assert(this->inheritance);
	
	dpv(this->kind);
	
	assert(this->inheritance->print);
	
	return (this->inheritance->print)(this);
}

