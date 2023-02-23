
#include <debug.h>

#include <cmdln/verbose.h>

#include "zebu.h"
#include "parse_test.h"

void parse_test(
	struct avl_tree_t* tests,
	const char* path)
{
	ENTER;
	
	FILE* stream = fopen(path, "r");
	
	if (!stream)
	{
		fprintf(stderr, "%s: fopen(\"%s\"): %m\n", argv0, path);
		exit(e_syscall_failed);
	}
	
	if (verbose) printf("parsing '%s'...\n", path);
	
	struct zebu_$start* start = zebu_parse(stream);
	
	for (unsigned i = 0; i < start->tests.n; i++)
	{
		TODO;
		#if 0
		struct zebu_test* ztest = start->tests.data[i];
		
		struct test* test = smalloc(sizeof(*test));
		
		strcpy(test->path, path), test->index = i;
		
		for (unsigned j = 0, m = ztest->files.n; j < m; j++)
		{
			TODO;
		}
		
		struct {
			struct file {
				char path[PATH_MAX];
				uint8_t* data;
				unsigned size;
			}** data;
			unsigned n, cap;
		} files;
		
		struct {
			struct command {
				char** arg;
				unsigned n, cap;
			}** data;
			unsigned n, cap;
		} commands;
		
		struct {
			struct keyval {
				char *key, *val;
			}** data;
			unsigned n, cap;
		} environment;
		
		struct {
			uint8_t* data;
			unsigned size;
		} input, output;
		
		int exit_code;
		
		TODO;
		#if 0
		if (!avl_insert(tests, test))
		{
			fprintf(stderr, "%s: avl_insert(): %m\n", argv0);
			exit(e_syscall_failed);
		}
		#endif
		#endif
	}
	
	free_zebu_$start(start);
	
	fclose(stream);
	EXIT;
}



















