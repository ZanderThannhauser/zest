
#include "kind.h"

struct value* new_value(
	enum value_kind kind,
	struct value_inheritance* inheritance,
	unsigned alloc_size);
