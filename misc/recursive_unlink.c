#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <assert.h>

#include <debug.h>

#include <enums/error.h>

#include <defines/argv0.h>

#include <misc/opendirat.h>

#include <cmdln/verbose.h>

#include "recursive_unlink.h"

void recursive_unlink(
	int dirfd, const char* path)
{
	ENTER;
	
	dpv(dirfd);
	dpvs(path);
	
	int fd = -1;
	DIR* dir = NULL;
	
	if ((fd = openat(dirfd, path, O_RDONLY)) < 0)
	{
		TODO;
	}
	else if (!(dir = fdopendir(fd)))
	{
		TODO;
	}
	else for (struct dirent* ent; errno = 0, (ent = readdir(dir)); )
	{
		if (ent->d_name[0] != '.')
		{
			dpvs(ent->d_name);
			
			struct stat statbuf;
			
			if (fstatat(fd, ent->d_name, &statbuf, AT_SYMLINK_NOFOLLOW) < 0)
			{
				fprintf(stderr, "%s: fstatat: %m\n", argv0);
				exit(e_syscall_failed);
			}
			else switch (statbuf.st_mode & S_IFMT)
			{
				case S_IFDIR:
				{
					recursive_unlink(fd, ent->d_name);
					
					if (verbose) printf("%s: rmdir '%s'\n", argv0, ent->d_name);
					
					if (unlinkat(fd, ent->d_name, AT_REMOVEDIR) < 0)
					{
						fprintf(stderr, "%s: unlinkat(): %m\n", argv0);
						exit(1);
					}
					break;
				}
				
				case S_IFREG:
				{
					if (verbose) printf("%s: unlink '%s'\n", argv0, ent->d_name);
					
					if (unlinkat(fd, ent->d_name, 0) < 0)
					{
						fprintf(stderr, "%s: unlinkat(): %m\n", argv0);
						exit(1);
					}
					break;
				}
				
				default:
					TODO;
					break;
			}
		}
	}
	
	if (errno)
	{
		fprintf(stderr, "%s: readdir(): %m\n", argv0);
		exit(e_syscall_failed);
	}
	
	if (dir)
		closedir(dir);
	else if (fd > 0)
		close(fd);
	
	EXIT;
}












