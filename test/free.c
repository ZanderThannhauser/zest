
#include <debug.h>

#include <parser/parser.h>

#include "struct.h"
#include "free.h"

void free_test(void* ptr)
{
	ENTER;
	
	struct test* this = ptr;
	
	free(this->path);
	free_zebu_test(this->ztest);
	free(this);
	
	EXIT;
}

