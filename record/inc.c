
#include <debug.h>

#include "struct.h"
#include "inc.h"

struct record* inc_record(
	struct record* this)
{
	if (this) this->refcount++;
	return this;
}

