
#include <stdlib.h>
#include <stdio.h>

#include <debug.h>

#include <record/free.h>

#include "struct.h"
#include "free.h"

void free_flattened_records(
	struct flattened_records* this)
{
	ENTER;
	
	for (unsigned i = 0, n = this->n; i < n; i++)
		free_record(this->data[i]);
		
	free(this->data);
	
	free(this);
	
	EXIT;
}

