
#include <debug.h>

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
		
		bool reached_EOF = false;
		
		while (reached_EOF)
		{
			switch (fscanf(stream, "%s %u %La", path, &index, &score))
			{
				case EOF:
					reached_EOF = true;
					break;
				
				case 3:
					TODO;
					break;
				
				default:
					TODO;
					break;
			}
		}
		
		fclose(stream);
	}
	else if (errno != ENOENT)
	{
		fprintf(stderr, "%s: fopen(\"%s\"): %m\n", argv0, path);
		exit(e_syscall_failed);
	}
	
	EXIT;
}









