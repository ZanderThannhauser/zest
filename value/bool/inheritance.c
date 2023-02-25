
#include "../inheritance.h"

#include "inheritance.h"
#include "print.h"
#include "free.h"

struct value_inheritance bool_value_inheritance = {
	.print = bool_value_print,
	.free = free_bool_value,
};


