
#include <debug.h>

#include "../new.h"

#include "struct.h"
#include "inheritance.h"
#include "new.h"

struct value* new_int_value(
	int value)
{
	ENTER;
	
	struct int_value* this = (void*) new_value(
		vk_int,
		&int_value_inheritance,
		sizeof(*this));
	
	mpz_init(this->mpz);
	mpz_set_si(this->mpz, value);
	
	EXIT;
	return (void*) this;
}

struct value* new_int_value_from_str(
	const char* str)
{
	ENTER;
	
	dpvs(str);
	
	struct int_value* this = (void*) new_value(
		vk_int,
		&int_value_inheritance,
		sizeof(*this));
	
	mpz_init(this->mpz);
	mpz_set_str(this->mpz, str, 10);
	
	EXIT;
	return (void*) this;
}

struct value* new_int_value_from_mpz(
	mpz_t mpz)
{
	ENTER;
	
	struct int_value* this = (void*) new_value(
		vk_int,
		&int_value_inheritance,
		sizeof(*this));
	
	mpz_init(this->mpz);
	mpz_set(this->mpz, mpz);
	
	EXIT;
	return (void*) this;
}
















