
#include <debug.h>

#include <memory/srealloc.h>

#include <record/inc.h>

#include "struct.h"
#include "append.h"

void flattened_records_append(
	struct flattened_records* this,
	struct record* element)
{
	ENTER;
	
	if (this->n == this->cap)
	{
		this->cap = this->cap << 1 ?: 1;
		this->data = srealloc(this->data, sizeof(*this->data) * this->cap);
	}
	
	this->data[this->n++] = inc_record(element);
	
	EXIT;
}

