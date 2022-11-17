
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

#include <defines/argv0.h>

#include "clear_dirfd.h"

void clear_dirfd(
	int dirfd)
{
	ENTER;
	
	int fd;
	
	if ((fd = openat(dirfd, ".", O_DIRECTORY)) < 0)
	{
		TODO;
		exit(1);
	}
	
	DIR* dir = fdopendir(fd);
	
	if (!dir)
	{
		TODO;
		exit(1);
	}
	
	for (struct dirent *dirent; errno = 0, (dirent = readdir(dir)); )
	{
		if (strcmp(".", dirent->d_name) && strcmp("..", dirent->d_name))
		{
			if (unlinkat(dirfd, dirent->d_name, 0) < 0)
			{
				fprintf(stderr, "%s: unlinkat(): %m\n", argv0);
				exit(1);
			}
		}
	}
	
	if (errno)
	{
		fprintf(stderr, "%s: readdir(): %m\n", argv0);
		exit(1);
	}
	
	closedir(dir);
	
	EXIT;
}












