
/*#include <stdlib.h>*/
/*#include <string.h>*/
/*#include <stdbool.h>*/
/*#include <ftw.h>*/

/*#include <defines/argv0.h>*/

/*#include <enums/error.h>*/

/*#include <macros/strequals.h>*/

/*#include <avl/tree_t.h>*/
/*#include <avl/insert.h>*/

/*#include <test/new.h>*/

#include <dirent.h>

#include <debug.h>

#include <set/str/new.h>
#include <set/str/add.h>
#include <set/str/free.h>

#include <quack/new.h>
#include <quack/append.h>
#include <quack/is_nonempty.h>
#include <quack/pop.h>
#include <quack/free.h>

#include <string/struct.h>
#include <string/new.h>
#include <string/free.h>

#include <cmdln/verbose.h>

#include "parse_test.h"
#include "find_tests.h"

void find_tests(
	struct avl_tree_t* tests,
	char* const* paths)
{
	ENTER;
	
	struct strset* queued = new_strset();
	
	struct quack* todo = new_quack();
	
	for (char* arg; (arg = *paths++); )
	{
		struct string* string = new_string(arg);
		
		if (strset_add(queued, string))
		{
			if (verbose)
				printf("%s: queued '%s'...\n", argv0, string->chars);
			
			quack_append(todo, string);
		}
	}
	
	while (quack_is_nonempty(todo))
	{
		struct string* path = quack_pop(todo);
		
		dpvs(path->chars);
		
		struct stat statbuf;
		
		if (stat(path->chars, &statbuf) < 0)
		{
			TODO;
			exit(1);
		}
		else switch (statbuf.st_mode & S_IFMT)
		{
			case S_IFDIR:
			{
				DIR* dir = opendir(path->chars);
				
				struct dirent* dirent;
				
				if (!dir)
				{
					TODO;
					exit(1);
				}
				else while (errno = 0, (dirent = readdir(dir)))
				{
					if (dirent->d_name[0] != '.')
					{
						char subpath[PATH_MAX];
						
						stpcpy(stpcpy(stpcpy(subpath, path->chars), "/"), dirent->d_name);
						
						dpvs(subpath);
						
						struct string* dup = new_string(subpath);
						
						if (strset_add(queued, dup))
						{
							if (verbose)
								printf("%s: queued '%s'...\n", argv0, subpath);
							
							quack_append(todo, dup);
						}
					}
				}
				
				if (errno)
				{
					TODO;
					exit(1);
				}
				
				closedir(dir);
				break;
			}
			
			case S_IFREG:
			{
				if (path->len > 5 && strequals(path->chars + path->len - 5, ".zest"))
				{
					parse_test(tests, path);
				}
				
				break;
			}
		}
		
		free_string(path);
	}
	
	free_quack(todo);
	
	free_strset(queued);
	
	EXIT;
}








