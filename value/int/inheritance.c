
#include "../inheritance.h"

#include "inheritance.h"
#include "compare.h"
#include "print.h"
#include "free.h"

struct value_inheritance int_value_inheritance = {
	.print = int_value_print,
	.compare = compare_int_values,
	.free = free_int_value,
};


