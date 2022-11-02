
#include <debug.h>

#include "free.h"

void free_cmdln(
	struct cmdln_flags* this)
{
	free(this);
}

