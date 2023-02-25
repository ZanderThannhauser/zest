
#include <gmp.h>

#include <debug.h>

#include "struct.h"
#include "free.h"

void free_int_value(
	struct value* super)
{
	struct int_value* this = (void*) super;
	ENTER;
	
	mpz_clear(this->mpz);
	
	EXIT;
}

