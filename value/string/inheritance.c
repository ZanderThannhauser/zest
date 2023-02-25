
#include "../inheritance.h"

#include "inheritance.h"
#include "compare.h"
#include "print.h"
#include "free.h"

struct value_inheritance string_value_inheritance = {
	.print = string_value_print,
	.compare = compare_string_value,
	.free = free_string_value,
};


