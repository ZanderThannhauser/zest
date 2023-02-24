#include <sys/types.h>
#include <sys/wait.h>

#include <debug.h>

#include <cmdln/verbose.h>

#include <defines/argv0.h>
#include <defines/DATABASE_PATH.h>
#include <defines/AGILITY.h>

#include <cmdln/process.h>
#include <cmdln/test_paths.h>

#include <test/compare.h>
#include <test/free.h>

#include <parser/find_tests.h>

#include <record/struct.h>
#include <record/compare.h>
#include <record/free.h>

#include <parse_database.h>

#include <cross_reference.h>

#include <flatten_and_sort.h>

#include <misc/mkdiropenat.h>

#include <flattened_records/struct.h>

#include <string/struct.h>

#include <test/struct.h>

#include <misc/recursive_unlink.h>

#include <parser/zebu.h>

#include <misc/unescape.h>

#include <evaluate/expression.h>

#include <value/free.h>

#include <value/struct.h>

#include <value/string/struct.h>

#include <store_database.h>

#include <flattened_records/free.h>

#include <value/bool/struct.h>

int main(int argc, char* const* argv)
{
	ENTER;
	
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
		
		dpvs(record->path->chars);
		
		printf(
			"\e[33m" "[%u/%u]: running test %u of '%s'..." "\e[m" "\n",
			i + 1, n, record->index + 1, record->path->chars);
		
		fflush(stdout);
		
		struct avl_node_t* node = avl_search(tests,
			&(struct test) {record->path, record->index});
		assert(node);
		
		recursive_unlink(test_dirfd, ".");
		
		struct test* test = node->item;
		
		for (unsigned i = 0, n = test->ztest->files.n; is_passing && i < n; i++)
		{
			struct zebu_file* zfile = test->ztest->files.data[i];
			
			struct unescaped path = unescape(zfile->path);
			
			dpvsn(path.data, path.n);
			
			struct value* content = evaluate_expression(zfile->expression);
			
			if (!content)
				is_passing = false;
			else if (content->kind != vk_string)
				fprintf(stderr, "%s: file content must be a string!\n", argv0),
				is_passing = false;
			else
			{
				struct string_value* spef = (void*) content;
				
				int fd = openat(test_dirfd, (void*) path.data, O_WRONLY | O_TRUNC | O_CREAT, 0664);
				uint8_t* moving = spef->data;
				size_t left = spef->len;
				ssize_t res;
				
				if (fd < 0)
					fprintf(stderr, "%s: open(\"%s\"): %m\n", argv0, path.data),
					is_passing = false;
				else while (left && (res = write(fd, moving, left)) > 0)
					moving += res, left -= res;
				
				if (res < 0)
					fprintf(stderr, "%s: write(): %m\n", argv0),
					is_passing = false;
				
				if (verbose)
					printf("%s: wrote '%s'\n", argv0, (char*) path.data);
				
				close(fd);
			}
			
			free_value(content);
			
			free(path.data);
		}
		
		if (is_passing)
		{
			pid_t child = -1;
			
			int wstatus;
			
			if ((child = fork()) < 0)
			{
				TODO;
				exit(1);
			}
			else if (!child)
			{
				*environ = NULL;
				
				if (fchdir(test_dirfd) < 0)
					fprintf(stderr, "%s: chdir(): %m\n", argv0),
					exit(1);
				else if (chroot(".") < 0)
					fprintf(stderr, "%s: chroot(): %m\n", argv0),
					exit(1);
				else for (unsigned i = 0, n = test->ztest->assertions.n;
					i < n; i++)
				{
					struct zebu_expression* assertion =
						test->ztest->assertions.data[i]->conditional;
					
					struct value* res = evaluate_expression(assertion);
					
					if (!res)
						exit(1);
					else if (res->kind != vk_bool)
						fprintf(stderr, "%s: assertions must return a bool value!\n", argv0),
						exit(1);
					else if (!((struct bool_value*) res)->value)
						fprintf(stderr, "%s: assertion failed!\n", argv0),
						exit(1);
					else
						free_value(res);
				}
				
				exit(0);
			}
			else if (waitpid(child, &wstatus, 0) < 0)
			{
				TODO;
				exit(1);
			}
			else if (!WIFEXITED(wstatus) || WEXITSTATUS(wstatus))
				fprintf(stderr, "%s: subcommands failed!\n", argv0),
				is_passing = false;
		}
		
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

















