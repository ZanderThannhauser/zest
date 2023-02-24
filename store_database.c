
#include <stdio.h>
#include <stdlib.h>

#include <debug.h>

#include <defines/argv0.h>

#include <enums/error.h>

#include <record/struct.h>

#include <flattened_records/struct.h>

#include <string/struct.h>

#include "store_database.h"

void store_database(
	struct flattened_records* flat,
	const char* path)
{
	ENTER;
	
	FILE* stream = fopen(path, "w");
	
	if (!stream)
	{
		fprintf(stderr, "%s: fopen(\"%s\"): %m\n", argv0, path);
		exit(e_syscall_failed);
	}
	
	for (unsigned i = 0, n = flat->n; i < n; i++)
	{
		struct record* record = flat->data[i];
		
		dpvs(record->path->chars);
		
		fprintf(stream, "%s\t%u\t%La\n",
			record->path->chars, record->index, record->score);
	}
	
	fclose(stream);
	
	EXIT;
}














