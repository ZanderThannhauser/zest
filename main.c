
#include <stdio.h>

#include <debug.h>

#include <defines/DATABASE_PATH.h>

#include <cmdln/flags.h>
#include <cmdln/process.h>
#include <cmdln/free.h>

#include <parser/parse_files.h>

#include <test/compare.h>
#include <test/free.h>

#include <record/struct.h>
#include <record/compare.h>
#include <record/free.h>

#include <parse_database.h>

#include <cross_reference.h>

#include <store_database.h>

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
					return +0;
			}
			compare;
		}));
	
	for (unsigned i = 0, n = flattened_records.n; i < n; i++)
	{
		struct record* record = flattened_records.data[i];
		
		printf("running test in '%s'[%u] (score %Lg%%):\n",
			record->path, record->index, record->score * 100);
		
		// run test
	}
	
	store_database(flattened_records.data, flattened_records.n, DATABASE_PATH);
	
	free(flattened_records.data);
	
	avl_free_tree(records);
	
	avl_free_tree(tests);
	
	free_cmdln(flags);
	
	EXIT;
	return 0;
}

















