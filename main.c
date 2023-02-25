
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <debug.h>

#include <cmdln/verbose.h>

#include <defines/argv0.h>
#include <defines/DATABASE_PATH.h>
#include <defines/AGILITY.h>

#include <enums/error.h>

#include <cmdln/process.h>
#include <cmdln/test_paths.h>

#include <test/compare.h>
#include <test/free.h>

#include <avl/search.h>
#include <avl/alloc_tree.h>
#include <avl/free_tree.h>

#include <parser/find_tests.h>

#include <record/struct.h>
#include <record/compare.h>
#include <record/free.h>

#include <parse_database.h>

#include <cross_reference.h>

#include <flatten_and_sort.h>

#include <misc/mkdiropenat.h>

#include <flattened_records/struct.h>

#include <flattened_records/free.h>

#include <string/struct.h>

#include <test/struct.h>

#include <store_database.h>

#include <run_test.h>

int main(int argc, char* const* argv)
{
	ENTER;
	
	#ifdef DEBUGGING
	setvbuf(stdout, NULL, _IONBF, 0);
	#endif
	
	cmdln_process(argc, argv);
	
	struct avl_tree_t* tests = avl_alloc_tree(compare_tests, free_test);
	
	find_tests(tests, test_paths);
	
	struct avl_tree_t* records = avl_alloc_tree(compare_records, free_record);
	
	parse_database(records, DATABASE_PATH);
	
	cross_reference(tests, records);
	
	struct flattened_records* flat = flatten_and_sort(records);
	
	// open test directory
	int test_dirfd = mkdiropenat("/tmp/", "zest", NULL);
	
	dpv(test_dirfd);
	
	bool is_passing = true;
	
	for (unsigned i = 0, n = flat->n; is_passing && i < n; i++)
	{
		struct record* record = flat->data[i];
		
		printf(
			"\e[33m" "[%u/%u]: running test %u of '%s'..." "\e[m" "\n",
			i + 1, n, record->index + 1, record->path->chars);
		
		fflush(stdout);
		
		struct avl_node_t* node = avl_search(tests,
			&(struct test) {record->path, record->index});
		assert(node);
		
		struct test* test = node->item;
		
		is_passing = run_test(test_dirfd, test->ztest);
		
		record->score = (is_passing + record->score * (AGILITY - 1)) / AGILITY;
	}
	
	if (is_passing)
	{
		printf("\e[K" "\e[32m" "all tests pass!" "\e[m" "\n");
	}
	
	store_database(flat, DATABASE_PATH);
	
	free_flattened_records(flat);
	
	avl_free_tree(records);
	
	avl_free_tree(tests);
	
	close(test_dirfd);
	
	EXIT;
	return is_passing ? 0 : e_failed_test;
}

















