
#include "../struct.h"

struct string_value
{
	struct value super;
	
	uint8_t* data;
	unsigned len;
};

