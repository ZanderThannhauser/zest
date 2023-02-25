
#include <string.h>
#include <stdio.h>

#include <debug.h>

#include <memory/smalloc.h>
#include <memory/srealloc.h>

#include "struct.h"
#include "prepend.h"

void quack_prepend(
	struct quack *this,
	void *element)
{
	ENTER;
	
	if (this->n == this->cap)
	{
		unsigned old_cap = this->cap, new_cap = old_cap << 1 ?: 1;
		
		this->data = srealloc(this->data, sizeof(*this->data) * new_cap);
		
		if (this->i)
		{
			unsigned new_i = new_cap - old_cap + this->i;
			
			memmove(
				/* dst: */ &this->data[new_i],
				/* src: */ &this->data[this->i],
				/* len: */ sizeof(*this->data) * (old_cap - this->i));
			
			this->i = new_i;
			dpv(this->i);
		}
		
		this->cap = new_cap;
		dpv(this->cap);
	}
	
	this->data[(this->i + this->n++) % this->cap] = element;
	
	if (this->i)
		this->i--;
	else
		this->i = this->n - 1;
	
	dpv(this->n);
	
	EXIT;
}
