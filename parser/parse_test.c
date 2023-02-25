
#include <stdlib.h>

#include <debug.h>

#include <avl/insert.h>

#include <enums/error.h>

#include <cmdln/verbose.h>

#include <test/new.h>

#include <string/struct.h>

#include <defines/argv0.h>

#include "zebu.h"
#include "parse_test.h"

void parse_test(
	struct avl_tree_t* tests,
	struct string* path)
{
	ENTER;
	
	FILE* stream = fopen(path->chars, "r");
	
	if (!stream)
	{
		fprintf(stderr, "%s: fopen(\"%s\"): %m\n", argv0, path->chars);
		exit(e_syscall_failed);
	}
	
	if (verbose)
	{
		printf("%s: parsing '%s'...\n", argv0, path->chars);
	}
	
	struct zebu_$start* start = zebu_parse(stream);
	
	struct zebu_root* root = start->root;
	
	for (unsigned i = 0; i < root->tests.n; i++)
	{
		struct zebu_test* ztest = root->tests.data[i];
		
		struct test* test = new_test(path, i, ztest);
		
		if (!avl_insert(tests, test))
		{
			fprintf(stderr, "%s: avl_insert(): %m\n", argv0);
			exit(e_syscall_failed);
		}
	}
	
	free_zebu_$start(start);
	
	fclose(stream);
	
	EXIT;
}





























