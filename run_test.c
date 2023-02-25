
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <debug.h>

#include <misc/recursive_unlink.h>

#include <parser/zebu.h>

#include <defines/argv0.h>

#include <misc/unescape.h>

#include <evaluate/expression.h>

#include <value/struct.h>
#include <value/bool/struct.h>
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
				errno = 0;
				
				char* m;
				unsigned long num = strtoul((char*) zfile->octal, &m, 8);
				
				if (errno || *m)
					fprintf(stderr, "%s: invalid octal "
						"mode '%s'!\n", argv0, (char*) zfile->octal),
					is_passing = false;
				else
					mode = 0777 & num;
			}
			else for (unsigned i = 0, n = zfile->chmods.n; i < n; i++)
			{
				struct zebu_token* code = zfile->chmods.data[i];
				
				dpvsn(code->data, code->len);
				
				char* moving = (char*) code->data;
				
				mode_t mask = 0;
				
				while (index("ugoa", *moving)) switch (*moving++)
				{
					case 'u': mask |= 05700; break;
					case 'g': mask |= 03070; break;
					case 'o': mask |= 01007; break;
					case 'a': mask |= 07777; break;
				}
				
				if (!mask) mask = -1;
				
				while (*moving && index("+-=", *moving))
				{
					mode_t bits = 0;
					
					char action = *moving++;
					
					dpvc(action);
					
					if (index("ugo", *moving))
					{
						switch (*moving++)
						{
							case 'u': bits = 7 & (mode >> 6); break;
							case 'g': bits = 7 & (mode >> 3); break;
							case 'o': bits = 7 & (mode >> 0); break;
						}
						
						bits |= bits << 6 | bits << 3;
					}
					else while (*moving && index("rwxXst", *moving))
					{
						switch (*moving++)
						{
							case 'r': bits |= 00444; break;
							case 'w': bits |= 00222; break;
							case 'x': bits |= 00111; break;
							case 'X': bits |= mode & 00111 ? 00111 : 0; break;
							case 's': bits |= 06000; break;
							case 't': bits |= 01000; break;
						}
					}
					
					switch (action)
					{
						case '+': mode |=  mask &  bits; break;
						
						case '-': mode &= ~mask | ~bits; break;
						
						case '=': mode = (mask & bits) | (~mask & mode); break;
					}
				}
				
				assert(!*moving);
			}
			
			dpvo(mode);
			
			struct string_value* spef = (void*) content;
			
			int fd = openat(test_dirfd, path.chars, O_WRONLY | O_TRUNC | O_CREAT, mode);
			char* moving = spef->chars;
			size_t left = spef->len;
			ssize_t res;
			
			if (fd < 0)
				fprintf(stderr, "%s: open(\"%s\"): %m\n", argv0, path.chars),
				is_passing = false;
			else if (fchmod(fd, mode) < 0)
				fprintf(stderr, "%s: fchmod(): %m\n", argv0),
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
	
	if (is_passing)
	{
		pid_t child = -1;
		
		int wstatus;
		
		if ((child = fork()) < 0)
			fprintf(stderr, "%s: fork(): %m\n", argv0),
			is_passing = false;
		else if (!child)
		{
			*environ = NULL;
			
			if (fchdir(test_dirfd) < 0)
				fprintf(stderr, "%s: chdir(): %m\n", argv0),
				exit(1);
			else if (chroot(".") < 0)
				fprintf(stderr, "%s: chroot(): %m\n", argv0),
				exit(1);
			else if (chdir("/") < 0)
				fprintf(stderr, "%s: chdir(): %m\n", argv0),
				exit(1);
			else for (unsigned i = 0, n = test->assertions.n; i < n; i++)
			{
				struct zebu_expression* assertion =
					test->assertions.data[i]->conditional;
				
				struct value* res = evaluate_expression(assertion);
				
				if (!res)
					exit(1);
				else if (res->kind != vk_bool)
					fprintf(stderr, "%s: assertions must return a bool value!\n", argv0),
					exit(1);
				else if (!((struct bool_value*) res)->value)
					fprintf(stderr, "%s: assertion failed!\n", argv0),
					exit(1);
				
				free_value(res);
			}
			
			fflush(stdout);
			fflush(stderr);
			exit(0);
		}
		else if (waitpid(child, &wstatus, 0) < 0)
			fprintf(stderr, "%s: waitpid(): %m\n", argv0),
			is_passing = false;
		else if (!WIFEXITED(wstatus) || WEXITSTATUS(wstatus))
			fprintf(stderr, "%s: subcommands failed!\n", argv0),
			is_passing = false;
	}
	
	EXIT;
	return is_passing;
}












