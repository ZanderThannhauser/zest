
#include <sys/types.h>
#include <dirent.h>

DIR* opendirat(
	int dirfd,
	const char* path);
