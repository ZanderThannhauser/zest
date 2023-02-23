
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

#include "parse_test.h"
#include "find_tests.h"

void find_tests(
	struct avl_tree_t* tests,
	const char* path)
{
	ENTER;
	
	dpvs(path);
	
	struct stat statbuf;
	
	if (stat(path, &statbuf) < 0)
	{
		TODO;
		exit(1);
	}
	else switch (statbuf.st_mode & S_IFMT)
	{
		case S_IFDIR:
		{
			DIR* dir = opendir(path);
			
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
					
					stpcpy(stpcpy(stpcpy(subpath, path), "/"), dirent->d_name);
					
					dpvs(subpath);
					
					find_tests(tests, subpath);
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
			unsigned len = strlen(path);
			
			if (len > 5 && strequals(path + len - 5, ".zest"))
				parse_test(tests, path);
			
			break;
		}
		
		case S_IFSOCK:
			TODO;
			break;
		
		case S_IFLNK:
			TODO;
			break;
		
		case S_IFBLK:
			TODO;
			break;
		
		case S_IFCHR:
			TODO;
			break;
		
		case S_IFIFO:
			TODO;
			break;
		
		default:
			TODO;
			break;
	}
	
	EXIT;
}








