
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <debug.h>

#include <defines/argv0.h>
#include <defines/DATABASE_PATH.h>
#include <defines/AGILITY.h>

#include <enums/error.h>

#include <avl/alloc_tree.h>
#include <avl/search.h>
#include <avl/free_tree.h>

#include <cmdln/flags.h>
#include <cmdln/process.h>
#include <cmdln/free.h>

#include <parser/parse_files.h>

#include <test/struct.h>
#include <test/compare.h>
#include <test/free.h>

#include <record/struct.h>
#include <record/compare.h>
#include <record/free.h>

#include <parse_database.h>

#include <cross_reference.h>

#include <store_database.h>

#include <run_test.h>

int main(int argc, char* const* argv)
{
	ENTER;
	
	struct cmdln_flags* flags = cmdln_process(argc, argv);
	
	struct avl_tree_t* tests = avl_alloc_tree(compare_tests, free_test);
	
	parse_files(tests, flags->input_directory);
	
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
			"\r" "\e[K" "\e[33m" "[%u/%u]: running test %u of '%s'..." "\e[m",
			i + 1, n, record->index, record->path);
		
		fflush(stdout);
		
		struct avl_node_t* node = avl_search(tests, &(struct test) {record->path, record->index});
		
		assert(node);
		
		is_passing = run_test(zest_dirfd, node->item);
		
		if (is_passing)
			record->score = (1 + record->score * (AGILITY - 1)) / AGILITY;
		else
			record->score = (0 + record->score * (AGILITY - 1)) / AGILITY;
	}
	
	if (is_passing)
	{
		puts("\r" "\e[K" "\e[32m" "all tests pass!" "\e[m");
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
}

















