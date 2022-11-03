
#include <stdio.h>
#include <stdlib.h>

#include <debug.h>

#include <defines/argv0.h>

#include <enums/error.h>

#include <record/struct.h>

#include "store_database.h"

void store_database(
	struct record** records, unsigned n,
	const char* path)
{
	ENTER;
	
	FILE* stream = fopen(path, "w");
	
	if (!stream)
	{
		fprintf(stderr, "%s: fopen(\"%s\"): %m\n", argv0, path);
		exit(e_syscall_failed);
	}
	
	for (unsigned i = 0; i < n; i++)
	{
		struct record* record = records[i];
		
		dpvs(record->path);
		
		fprintf(stream, "%s\t%u\t%La\n", record->path, record->index, record->score);
	}
	
	fclose(stream);
	
	EXIT;
}

