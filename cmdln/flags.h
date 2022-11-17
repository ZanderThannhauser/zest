
#include <stdbool.h>

struct cmdln_flags
{
	bool verbose;
	
	bool clear_dirfd;
	
	const char* input_directory;
};
