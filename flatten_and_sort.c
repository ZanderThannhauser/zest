
#include <stdlib.h>

#include <debug.h>

#include <record/struct.h>

#include <flattened_records/struct.h>
#include <flattened_records/new.h>
#include <flattened_records/append.h>

#include <string/compare.h>

#include <avl/tree_t.h>

#include "flatten_and_sort.h"

struct flattened_records* flatten_and_sort(
	struct avl_tree_t* records)
{
	ENTER;
	
	struct flattened_records* flat = new_flattened_records();
	
	for (struct avl_node_t* node = records->head; node; node = node->next)
	{
		flattened_records_append(flat, node->item);
	}
	
	qsort(flat->data, flat->n, sizeof(*flat->data), ({
			int compare(const void* a, const void* b)
			{
				const struct record *const *A = a, *const *B = b;
				
				if ((*A)->score > (*B)->score)
					return +1;
				else if ((*A)->score < (*B)->score)
					return -1;
				else
					return compare_strings((*A)->path, (*B)->path);
			}
			compare;
		}));
	
	EXIT;
	return flat;
}

