
#include <debug.h>

#include <parser/parser.h>

#include "struct.h"
#include "new.h"

struct test* new_test(
	const char* ro_path,
	unsigned index,
	struct zebu_test* ztest)
{
	ENTER;
	
	char* path = strdup(ro_path);
	
	struct test* this = smalloc(sizeof(*this));
	
	this->path = path;
	
	this->index = index;
	
	this->ztest = inc_zebu_test(ztest);
	
	EXIT;
	return this;
}

