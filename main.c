
/*#include <defines/argv0.h>*/
/*#include <defines/DATABASE_PATH.h>*/
/*#include <defines/AGILITY.h>*/

/*#include <enums/error.h>*/

/*#include <avl/alloc_tree.h>*/
/*#include <avl/search.h>*/
/*#include <avl/free_tree.h>*/

/*#include <parser/parse_files.h>*/

/*#include <test/struct.h>*/

/*#include <record/struct.h>*/
/*#include <record/compare.h>*/
/*#include <record/free.h>*/

/*#include <parse_database.h>*/

/*#include <cross_reference.h>*/

/*#include <store_database.h>*/

/*#include <clear_dirfd.h>*/
/*#include <run_test.h>*/

/*#include <string.h>*/
/*#include <unistd.h>*/
/*#include <assert.h>*/
/*#include <fcntl.h>*/
/*#include <stdlib.h>*/
/*#include <stdio.h>*/
/*#include <sys/stat.h>*/
/*#include <sys/types.h>*/

#include <debug.h>

#include <cmdln/process.h>
#include <cmdln/test_paths.h>

#include <test/compare.h>
#include <test/free.h>

#include <parser/find_tests.h>

int main(int argc, char* const* argv)
{
	ENTER;
	
	cmdln_process(argc, argv);
	
	struct avl_tree_t* tests = avl_alloc_tree(compare_tests, free_test);
	
	for (char* test_path; (test_path = *test_paths++); )
	{
		find_tests(tests, test_path);
	}
	
	// cross reference
	
	// flatten
	
	// open test directory
	
	// expressions:
		// (byte) strings, (mpz) integers, lists
		// special forms:
			// shell(<complex-command>):
				// for each complex command:
					// for each simple command:
						// evaluate expressions, save strings
					// if redirect_in:
						// open
					// for each simple command:
						// fork
							// execvp
						// if last:
							// if redirect_out:
								// open
						// else:
							// pipe
					// wait, return exit code
			// file(<string>):
				// reads all of the content of the path,
				// returns content as string
	
	// for each test:
		// delete all files in test directory
		
		// for each file:
			// evaluate expression, write string
		
		// fork:
			// chroot
			// evaluate each expression:
				// if not true:
					// print error message
		// else
			// waitpid
			// check exit code
	
	TODO;
	#if 0
	struct avl_tree_t* records = avl_alloc_tree(compare_records, free_record);
	
	parse_database(records, DATABASE_PATH);
	
	cross_reference(tests, records);
	
	struct {
		struct record** data;
		unsigned n, cap;
	} flattened_records = {};
	
	for (struct avl_node_t* node = records->head; node; node = node->next)
	{
		if (flattened_records.n == flattened_records.cap)
		{
			flattened_records.cap = flattened_records.cap << 1 ?: 1;
			flattened_records.data = realloc(flattened_records.data,
				sizeof(*flattened_records.data) * flattened_records.cap);
		}
		
		flattened_records.data[flattened_records.n++] = node->item;
	}
	
	qsort(flattened_records.data, flattened_records.n,
		sizeof(*flattened_records.data), ({
			int compare(const void* a, const void* b)
			{
				const struct record *const *A = a, *const *B = b;
				
				if ((*A)->score > (*B)->score)
					return +1;
				else if ((*A)->score < (*B)->score)
					return -1;
				else
					return strcmp((*A)->path, (*B)->path);
			}
			compare;
		}));
	
	int tmp_dirfd = -1, zest_dirfd = -1;
	
	if ((tmp_dirfd = open("/tmp/", O_PATH)) < 0)
	{
		fprintf(stderr, "%s: open(\"/tmp/\"): %m\n", argv0);
		exit(e_syscall_failed);
	}
	else if (mkdirat(tmp_dirfd, "zest", 0774) < 0 && errno != EEXIST)
	{
		fprintf(stderr, "%s: mkdir(\"zest\"): %m\n", argv0);
		exit(e_syscall_failed);
	}
	else if ((zest_dirfd = openat(tmp_dirfd, "zest", O_PATH)) < 0)
	{
		fprintf(stderr, "%s: open(\"zest\"): %m\n", argv0);
		exit(e_syscall_failed);
	}
	
	bool is_passing = true;
	
	for (unsigned i = 0, n = flattened_records.n; is_passing && i < n; i++)
	{
		struct record* record = flattened_records.data[i];
		
		printf(
			"\e[33m" "[%u/%u]: running test %u of '%s'..." "\e[m" "\n",
			i + 1, n, record->index + 1, record->path);
		
		fflush(stdout);
		
		struct avl_node_t* node = avl_search(tests, &(struct test) {record->path, record->index});
		assert(node);
		
		clear_dirfd(zest_dirfd);
		
		// write files
		
		// fork:
			// chroot
			// for each command:
				// parse and exec
		// else: waitpid
		
		TODO;
		#if 0
		is_passing = run_test(zest_dirfd, node->item);
		
		record->score = (is_passing + record->score * (AGILITY - 1)) / AGILITY;
		#endif
	}
	
	if (is_passing)
	{
		printf("\e[K" "\e[32m" "all tests pass!" "\e[m" "\n");
	}
	
	store_database(flattened_records.data, flattened_records.n, DATABASE_PATH);
	
	free(flattened_records.data);
	
	avl_free_tree(records);
	
	avl_free_tree(tests);
	
	free_cmdln(flags);
	
	close(zest_dirfd);
	
	close(tmp_dirfd);
	
	EXIT;
	return is_passing ? 0 : e_failed_test;
	#endif
}

















