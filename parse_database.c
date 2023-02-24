
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include <debug.h>

#include <defines/argv0.h>

#include <enums/error.h>

#include <avl/tree_t.h>
#include <avl/insert.h>

#include <string/new.h>
#include <string/free.h>

#include <record/new.h>

#include "parse_database.h"

void parse_database(
	struct avl_tree_t* records,
	const char* path)
{
	ENTER;
	
	dpvs(path);
	
	FILE* stream = fopen(path, "r");
	
	if (stream)
	{
		char path[PATH_MAX];
		unsigned index;
		long double score;
		
		char *line = NULL;
		size_t len = 0;
		ssize_t nread;
		
		while ((nread = getline(&line, &len, stream)) != -1)
		{
			line[--nread] = 0;
			
			dpvs(line);
			
			if (sscanf(line, "%s %u %La ", path, &index, &score) == 3)
			{
				struct string* str = new_string(path);
				
				struct record* record = new_record(str, index, score);
				
				avl_insert(records, record);
				
				free_string(str);
			}
			else
			{
				TODO;
			}
		}
		
		free(line);
		fclose(stream);
	}
	else if (errno != ENOENT)
	{
		fprintf(stderr, "%s: fopen(\"%s\"): %m\n", argv0, path);
		exit(e_syscall_failed);
	}
	
	EXIT;
}










