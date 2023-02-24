
#include <string.h>

#include <debug.h>

#include <memory/smalloc.h>

#include <string/inc.h>

#include "struct.h"
#include "new.h"

struct record* new_record(
	struct string* path,
	unsigned index,
	long double score)
{
	ENTER;
	
	struct record* this = smalloc(sizeof(*this));
	
	this->path = inc_string(path);
	this->index = index;
	this->score = score;
	
	this->refcount = 1;
	
	EXIT;
	return this;
}

