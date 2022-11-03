
#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <debug.h>

#include <defines/argv0.h>

#include <test/struct.h>

#include <parser/parser.h>

#include "run_test.h"

bool run_test(int zest_dirfd, const struct test* test)
{
	bool result = true;
	ENTER;
	
	dpv(zest_dirfd);
	dpvs(test->path);
	
	struct {
		char* data;
		unsigned n, cap;
	} buffer = {};
	
	void append(unsigned char c)
	{
		if (buffer.n == buffer.cap)
		{
			buffer.cap = buffer.cap << 1 ?: 1;
			buffer.data = realloc(buffer.data, buffer.cap);
		}
		
		buffer.data[buffer.n++] = c;
	}
	
	bool convert(struct zebu_token* token)
	{
		buffer.n = 0;
		
		unsigned char *start = token->data + 1, *end = token->data + token->len - 1;
		
		while (start < end)
		{
			if (*start == '\\')
			{
				switch (*++start)
				{
					case '\"': append('\"'), start++; break;
					
					case '\\': append('\\'), start++; break;
					
					default:
						fprintf(stderr, "\n" "%s: unknown escape sequence '\\%c'!\n", argv0, *start);
						return false;
				}
			}
			else
			{
				append(*start++);
			}
		}
		
		append(0);
		return true;
	}
	
	struct zebu_test* ztest = test->ztest;
	
	for (unsigned i = 0, n = ztest->files.n; result && i < n; i++)
	{
		struct zebu_file* file = ztest->files.data[i];
		
		result = convert(file->path);
		
		if (result)
		{
			dpvs(buffer.data);
			
			int dstfd = openat(zest_dirfd, buffer.data, O_WRONLY | O_TRUNC | O_CREAT, 0664);
			
			if (dstfd < 0)
				fprintf(stderr, "\n" "%s: open(): %m\n", argv0), result = false;
			else if (file->srcpath)
			{
				convert(file->srcpath);
				
				if (result)
				{
					dpvs(buffer.data);
					
					int srcfd = open(buffer.data, O_RDONLY);
					
					struct stat statbuf;
					
					if (srcfd < 0)
						fprintf(stderr, "\n" "%s: open(\"%s\"): %m\n", argv0, buffer.data), result = false;
					else if (fstat(srcfd, &statbuf) < 0)
						fprintf(stderr, "\n" "%s: fstat(): %m\n", argv0), result = false;
					else if (fchmod(dstfd, statbuf.st_mode) < 0)
						fprintf(stderr, "\n" "%s: fchmod(): %m\n", argv0), result = false;
					else
					{
						char buffer[4096];
						ssize_t rrettval;
						
						while (result && (rrettval = read(srcfd, buffer, sizeof(buffer))) > 0)
						{
							if (write(dstfd, buffer, rrettval) != rrettval)
							{
								fprintf(stderr, "\n" "%s: write(): %m\n", argv0), result = false;
							}
						}
						
						if (rrettval < 0)
						{
							fprintf(stderr, "\n" "%s: read(): %m\n", argv0), result = false;
						}
					}
					
					close(srcfd);
				}
			}
			else if (file->content)
			{
				for (unsigned i = 0, n = file->content->lines.n; result && i < n; i++)
				{
					convert(file->content->lines.data[i]);
					
					if (result)
					{
						dpvs(buffer.data);
						
						buffer.n--, append('\n');
						
						if (write(dstfd, buffer.data, buffer.n) != buffer.n)
						{
							fprintf(stderr, "\n" "%s: write(): %m\n", argv0), result = false;
						}
					}
				}
			}
			else
			{
				TODO;
			}
			
			close(dstfd);
		}
	}
	
	if (!ztest->commands)
	{
		fprintf(stderr, "\n" "%s: test missing commands!\n", argv0), result = false;
	}
	
	if (result)
	{
		for (unsigned i = 0, n = ztest->commands->lines.n; result && i < n; i++)
		{
			int wstatus;
			
			convert(ztest->commands->lines.data[i]);
			
			dpvs(buffer.data);
			
			char* cmd[] = {"sh", "-c", buffer.data, NULL};
			
			pid_t child = -1;
			
			if ((child = fork()) < 0)
				fprintf(stderr, "\n" "%s: fork(): %m\n", argv0), result = false;
			else if (child)
			{
				if (waitpid(child, &wstatus, 0) < 0)
					fprintf(stderr, "\n" "%s: waitpid(): %m\n", argv0), result = false;
				else if (!WIFEXITED(wstatus))
					fprintf(stderr, "\n" "%s: subtask '%s' did not exit normally!\n", argv0, buffer.data), result = false;
				else if (ztest->code != WEXITSTATUS(wstatus))
				{
					fprintf(stderr, "\n" "%s: subtask '%s' did not return expected "
						"exit-code! (expected %u, actual: %u)\n",
						argv0, buffer.data, ztest->code, WEXITSTATUS(wstatus));
					
					printf("%s: look at the output of the test at "
						"'/tmp/zest/output.txt'.\n", argv0);
					
					result = false;
				}
			}
			else if (fchdir(zest_dirfd) < 0)
				fprintf(stderr, "\n" "%s: fchdir(): %m\n", argv0), result = false;
			else
			{
				int output_fd = open("./output.txt", O_WRONLY | O_TRUNC | O_CREAT, 0664);
				
				if (output_fd < 0)
					fprintf(stderr, "\n" "%s: open(): %m\n", argv0), result = false;
				else if (close(0) < 0)
					fprintf(stderr, "\n" "%s: close(): %m\n", argv0), result = false;
				else if (dup2(output_fd, 1) < 0)
					fprintf(stderr, "\n" "%s: dup2(): %m\n", argv0), result = false;
				else if (dup2(output_fd, 2) < 0)
					fprintf(stderr, "\n" "%s: dup2(): %m\n", argv0), result = false;
				else if (execvp(cmd[0], cmd) < 0)
					fprintf(stderr, "\n" "%s: execvp(): %m\n", argv0), result = false;
				
				close(output_fd);
			}
		}
	}
	
	free(buffer.data);
	
	EXIT;
	return result;
}












