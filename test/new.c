
#include <string.h>

#include <debug.h>

#include <memory/smalloc.h>

#include <string/inc.h>

#include <parser/zebu.h>

#include "struct.h"
#include "new.h"

struct test* new_test(
	struct string* path,
	unsigned index,
	struct zebu_test* ztest)
{
	ENTER;
	
	struct test* this = smalloc(sizeof(*this));
	
	this->path = inc_string(path);
	
	this->index = index;
	
	this->ztest = inc_zebu_test(ztest);
	
	EXIT;
	return this;
}

