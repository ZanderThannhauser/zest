
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <debug.h>

#include <avl/tree_t.h>
#include <avl/insert.h>
#include <avl/delete_node.h>

#include <defines/argv0.h>

#include <enums/error.h>

#include <test/struct.h>

#include <record/struct.h>
#include <record/new.h>
#include <record/compare.h>

#include <string/struct.h>
#include <string/compare.h>

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
		struct test *at = tnode->item;
		struct record *rt = rnode->item;
		
		int cmp = compare_strings(at->path, rt->path) ?: at->index - rt->index;
		
		if (cmp < 0)
		{
			struct record* record = new_record(at->path, at->index, 0.0);
			
			if (!avl_insert(records, record))
			{
				fprintf(stderr, "%s: avl_insert(): %m\n", argv0);
				exit(e_syscall_failed);
			}
			
			tnode = tnode->next;
		}
		else if (cmp > 0)
		{
			struct avl_node_t* new = rnode->next;
			avl_delete_node(records, rnode);
			rnode = new;
		}
		else
		{
			tnode = tnode->next;
			rnode = rnode->next;
		}
	}
	
	while (tnode)
	{
		struct test* test = tnode->item;
		
		dpvs(test->path->chars);
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
		struct avl_node_t* new = rnode->next;
		avl_delete_node(records, rnode);
		rnode = new;
	}
	
	EXIT;
}









