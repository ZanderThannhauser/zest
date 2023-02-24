
#include <assert.h>

#include <debug.h>

#include "struct.h"
#include "popn.h"

void* quack_popn(struct quack* this)
{
	ENTER;
	
	assert(this->n);
	
	void* ele = this->data[(this->i + --this->n) % this->cap];
	
	dpv(this->n);
	dpv(this->i);
	
	EXIT;
	return ele;
}

