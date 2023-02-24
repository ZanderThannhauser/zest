
#include <debug.h>

#include "opendirat.h"

DIR* opendirat(
	int dirfd,
	const char* path)
{
	return fdopendir(openat(dirfd, path, O_RDONLY));
}

