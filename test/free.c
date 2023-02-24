
#include <stdlib.h>

#include <debug.h>

#include <parser/zebu.h>

#include <string/free.h>

#include "struct.h"
#include "free.h"

void free_test(void* ptr)
{
	ENTER;
	
	struct test* this = ptr;
	
	free_string(this->path);
	
	free_zebu_test(this->ztest);
	
	free(this);
	
	EXIT;
}

