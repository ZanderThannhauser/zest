
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>

#include <defines/argv0.h>

#include <debug.h>

#include "mkdiropenat.h"

int mkdiropenat(const char* initial_dir, ...)
{
	ENTER;
	
	int dirfd = AT_FDCWD;
	
	const char* dir = initial_dir;
	
	va_list ap;
	
	va_start(ap, initial_dir);
	
	do {
		int old_dirfd = dirfd;
		
		dpvs(dir);
		
		if ((dirfd = openat(dirfd, dir, O_PATH)) < 0)
		{
			fprintf(stderr, "%s: openat(): %m\n", argv0);
			exit(1);
		}
		
		if (old_dirfd > 0)
			close(old_dirfd);
	} while ((dir = va_arg(ap, char*)));
	
	va_end(ap);
	
	EXIT;
	return dirfd;
}

