
#include <debug.h>

#include <test/struct.h>

#include <record/new.h>

#include "cross_reference.h"

void cross_reference(
	struct avl_tree_t* tests,
	struct avl_tree_t* records)
{
	ENTER;
	
	struct avl_node_t* tnode = tests->head;
	struct avl_node_t* rnode = records->head;
	
	while (tnode && rnode)
	{
		TODO;
	}
	
	while (tnode)
	{
		struct test* test = tnode->item;
		
		dpvs(test->path);
		dpv(test->index);
		
		struct record* record = new_record(test->path, test->index, 0.0);
		
		if (!avl_insert(records, record))
		{
			fprintf(stderr, "%s: avl_insert(): %m\n", argv0);
			exit(e_syscall_failed);
		}
		
		tnode = tnode->next;
	}
	
	while (rnode)
	{
		TODO;
	}
	
	EXIT;
}









