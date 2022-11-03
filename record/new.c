
#include <string.h>

#include <debug.h>

#include <memory/smalloc.h>

#include "struct.h"
#include "new.h"

struct record* new_record(
	const char* ro_path,
	unsigned index,
	long double score)
{
	ENTER;
	
	char* path = strdup(ro_path);
	
	struct record* this = smalloc(sizeof(*this));
	
	this->path = path;
	this->index = index;
	this->score = score;
	
	dpvs(this->path);
	
	EXIT;
	return this;
}

