
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <debug.h>

#include <defines/argv0.h>

#include <memory/smalloc.h>

#include <cmdln/verbose.h>

#include <parser/zebu.h>

#include <evaluate/0.primary.h>

#include <value/struct.h>
#include <value/string/struct.h>
#include <value/int/new.h>
#include <value/free.h>

#include "shell.h"

struct value* evaluate_shell_form(
	struct zebu_complex_command* complex_command)
{
	bool happy = true;
	
	struct {
		int src, dst;
	} redirect_in = {-1, 0}, redirect_out = {-1, 1};
	
	unsigned n = complex_command->subcommands.n;
	
	int (*pipes)[n - 1][2] = smalloc(sizeof(*pipes));
	
	pid_t (*pids)[n] = smalloc(sizeof(*pids));
	
	memset(pipes, -1, sizeof(*pipes));
	
	memset(pids, 0, sizeof(*pids));
	
	if (happy && complex_command->redirect_in)
	{
		struct value* res = evaluate_primary_expression(complex_command->redirect_in);
		
		if (res->kind != vk_string)
		{
			fprintf(stderr, "%s: error in shell redirect-in must be a string!\n", argv0);
			happy = false;
		}
		
		if (happy)
		{
			struct string_value* spef = (struct string_value*) res;
			
			if ((redirect_in.src = open(spef->chars, O_RDONLY | O_CLOEXEC)) < 0)
			{
				fprintf(stderr, "%s: error in shell redirect-in: open(\"%s\"): %m\n", argv0, spef->chars);
				happy = false;
			}
		}
		
		char* c = (char*) complex_command->rinfd->data + 1;
		
		if (*c)
		{
			redirect_in.dst = 0;
			do redirect_in.dst = redirect_in.dst * 10 + *c++ - '0';
			while (*c && index("0123456789", *c));
			
			dpv(redirect_in.dst);
		}
		
		free_value(res);
	}
	
	if (happy && complex_command->redirect_out)
	{
		struct value* res = evaluate_primary_expression(complex_command->redirect_out);
		
		if (res->kind != vk_string)
		{
			fprintf(stderr, "%s: error in shell redirect-out must be a string!\n", argv0);
			happy = false;
		}
		
		if (happy)
		{
			struct string_value* spef = (struct string_value*) res;
			
			if ((redirect_out.src = open(spef->chars, O_WRONLY | O_CLOEXEC | O_CREAT | O_TRUNC, 0664)) < 0)
			{
				fprintf(stderr, "%s: error in shell redirect-out: open(\"%s\"): %m\n", argv0, spef->chars);
				happy = false;
			}
		}
		
		char* c = (char*) complex_command->routfd->data + 1;
		
		if (*c)
		{
			redirect_out.dst = 0;
			do redirect_out.dst = redirect_out.dst * 10 + *c++ - '0';
			while (*c && index("0123456789", *c));
			
			dpv(redirect_out.dst);
		}
		
		free_value(res);
	}
	
	for (unsigned i = 0; happy && i < n - 1; i++)
	{
		if (pipe2(pipes[0][i], O_CLOEXEC) < 0)
		{
			fprintf(stderr, "%s: pipe(): %m\n", argv0);
			happy = false;
		}
	}
	
	for (unsigned i = 0; happy && i < n; i++)
	{
		struct zebu_simple_command* simple = complex_command->subcommands.data[i];
		
		unsigned j, m = simple->args.n;
		
		char* (*args)[m + 1] = smalloc(sizeof(*args));
		
		memset(args, 0, sizeof(*args));
		
		for (j = 0; j < m; j++)
		{
			struct value* arg = evaluate_primary_expression(simple->args.data[j]);
			
			if (arg->kind == vk_string)
			{
				struct string_value* spef = (void*) arg;
				
				args[0][j] = strdup(spef->chars);
			}
			else
			{
				TODO;
				happy = false;
			}
			
			free_value(arg);
		}
		
		if (happy)
		{
			pid_t child;
			
			if ((child = fork()) < 0)
			{
				TODO;
				happy = false;
			}
			else if (!child)
			{
				args[0][m] = NULL;
				
				if (i)
				{
					if (dup2(pipes[0][i-1][0], 0) < 0)
					{
						TODO;
						exit(errno);
					}
				}
				else if (redirect_in.src > 0)
				{
					if (dup2(redirect_in.src, redirect_in.dst) < 0)
					{
						TODO;
						exit(errno);
					}
				}
				else
				{
					close(0);
				}
				
				if (i + 1 < n)
				{
					if (dup2(pipes[0][i][1], 1) < 0)
					{
						TODO;
						exit(errno);
					}
				}
				else if (redirect_out.src > 0)
				{
					if (dup2(redirect_out.src, redirect_out.dst) < 0)
					{
						TODO;
					exit(errno);
					}
				}
				else
				{
					close(1);
				}
				
				if (execvp(**args, *args) < 0)
				{
					fprintf(stderr, "%s: error in shell exec('%s'): %m\n", argv0, **args);
					happy = false;
					exit(errno);
				}
				
				abort();
			}
			else
			{
				pids[0][i] = child;
			}
		}
		
		for (j = 0; j < m; j++)
			free(args[0][j]);
		
		free(args);
	}
	
	for (unsigned i = 0; i < n - 1; i++)
	{
		if (pipes[0][i][0] > 0)
		{
			close(pipes[0][i][0]);
			close(pipes[0][i][1]);
		}
	}
	
	int code = 0;
	
	for (unsigned i = 0; i < n; i++)
	{
		if (pids[0][i])
		{
			int wstatus;
			
			if (waitpid(pids[0][i], &wstatus, 0) < 0)
			{
				TODO;
				happy = false;
			}
			else if (wstatus)
			{
				code = wstatus;
			}
		}
	}
	
	if (redirect_in.src > 0)
		close(redirect_in.src);
	
	if (redirect_out.src > 0)
		close(redirect_out.src);
	
	free(pipes);
	
	dpv(code);
	
	if (happy)
	{
		if (verbose)
			printf("%s: shell() returned %u\n", argv0, code);
		
		return new_int_value(code);
	}
	else
	{
		return NULL;
	}
}



































