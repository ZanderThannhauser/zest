
/*#include <assert.h>*/
/*#include <stdbool.h>*/
/*#include <sys/types.h>*/
/*#include <sys/wait.h>*/
/*#include <stdlib.h>*/
/*#include <sys/types.h>*/
/*#include <sys/stat.h>*/
/*#include <fcntl.h>*/
/*#include <unistd.h>*/

/*#include <defines/argv0.h>*/

/*#include <test/struct.h>*/

/*#include <parser/parser.h>*/

#include <debug.h>

#include <misc/recursive_unlink.h>

#include <parser/zebu.h>

#include <misc/unescape.h>

#include <evaluate/expression.h>

#include <value/struct.h>
#include <value/string/struct.h>
#include <value/free.h>

#include <cmdln/verbose.h>

#include "run_test.h"

bool run_test(
	int test_dirfd,
	struct zebu_test* test)
{
	bool is_passing = true;
	ENTER;
	
	recursive_unlink(test_dirfd, ".");
	
	for (unsigned i = 0, n = test->files.n; is_passing && i < n; i++)
	{
		struct zebu_file* zfile = test->files.data[i];
		
		struct unescaped path = unescape(zfile->path);
		
		dpvsn(path.chars, path.n);
		
		struct value* content = evaluate_expression(zfile->expression);
		
		if (!content)
			is_passing = false;
		else if (content->kind != vk_string)
			fprintf(stderr, "%s: file content must be a string!\n", argv0),
			is_passing = false;
		else
		{
			mode_t mode = 0664;
			
			if (zfile->octal)
			{
				TODO;
			}
			else for (unsigned i = 0, n = zfile->chmods.n; i < n; i++)
			{
				struct zebu_token* code = zfile->chmods.data[i];
				
				dpvsn(code->data, code->len);
				
				char* moving = code->data;
				
				// this needs to be better.
				TODO;
				#if 0
				unsigned ugo = 0;
				
				ugo_loop: switch (*moving)
				{
					case 'u': ugo |= 4, moving++; goto ugo_loop;
					case 'g': ugo |= 2, moving++; goto ugo_loop;
					case 'o': ugo |= 1, moving++; goto ugo_loop;
				}
				
				if (ugo) ugo = 7;
				
				while (index("+=-", c))
				{
					mode_t bits = 0;
					
					switch (*moving)
					{
						case 'u': bits = mode >> 6, moving++; break;
						case 'g': bits = mode >> 3, moving++; break;
						case 'o': bits = mode >> 0, moving++; break;
						
						rwxXst_loop:
						case 'r': case 'w':
						case 'x': case 'X':
						case 's': case 't':
							switch (*moving)
							{
								case 'r': bits |= 4; break;
								case 'w': bits |= 2; break;
								case 'x': bits |= 1; break;
								case 'X': bits |= !!(mode & 0111); break;
								case 's': bits |= 04000; break;
								case 't': bits |= 01000; break;
							}
					}
					
					switch (action)
					{
						case '+':
						{
							if (action & 4) mode |= bits << 6;
							if (action & 2) mode |= bits << 3;
							if (action & 1) mode |= bits << 0;
							break;
						}
						
						case '=':
						{
							if (action & 4) mode |= bits << 6;
							if (action & 2) mode |= bits << 3;
							if (action & 1) mode |= bits << 0;
							break;
						}
						
						case '-':
						{
							if (action & 4) mode &= ~(bits << 6);
							if (action & 2) mode &= ~(bits << 3);
							if (action & 1) mode &= ~(bits << 0);
							break;
						}
					}
				}
				#endif
				
				assert(!*ugo);
				
				TODO;
			}
			
			TODO;
			
			struct string_value* spef = (void*) content;
			
			int fd = openat(test_dirfd, path.chars, O_WRONLY | O_TRUNC | O_CREAT, mode);
			char* moving = spef->chars;
			size_t left = spef->len;
			ssize_t res;
			
			if (fd < 0)
				fprintf(stderr, "%s: open(\"%s\"): %m\n", argv0, path.chars),
				is_passing = false;
			else while (left && (res = write(fd, moving, left)) > 0)
				moving += res, left -= res;
			
			if (res < 0)
				fprintf(stderr, "%s: write(): %m\n", argv0),
				is_passing = false;
			
			if (verbose)
				printf("%s: wrote '%s'\n", argv0, (char*) path.chars);
			
			close(fd);
		}
		
		free_value(content);
		
		free(path.chars);
	}
	
	TODO;
	#if 0
	if (is_passing)
	{
		dpv(getpid());
		
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
			
			dpv(getpid());
			
			if (fchdir(test_dirfd) < 0)
				fprintf(stderr, "%s: chdir(): %m\n", argv0),
				exit(1);
			else if (chroot(".") < 0)
				fprintf(stderr, "%s: chroot(): %m\n", argv0),
				exit(1);
			else if (chdir("/") < 0)
				fprintf(stderr, "%s: chdir(): %m\n", argv0),
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
	
	#endif
	EXIT;
}












