
#include <unistd.h>

#include <debug.h>

#include <memory/smalloc.h>

#include "flags.h"
#include "usage.h"
#include "process.h"

struct cmdln_flags* cmdln_process(int argc, char* const* argv)
{
	ENTER;
	
	for (unsigned i = 0; i < argc; i++)
	{
		ddprintf("argv[%u] == \"%s\"\n", i, argv[i]);
	}
	
	bool verbose = false;
	bool clear_dirfd = true;
	
	int opt;
	while ((opt = getopt(argc, argv, "hvD")) != -1)
	{
		switch (opt)
		{
			case 'v':
				verbose = true;
				dpvb(verbose);
				break;
			
			case 'D':
				clear_dirfd = false;
				break;
			
			case 'h':
				usage(0);
				break;
			
			default:
				usage(1);
				break;
		}
	}
	
	const char* input_directory = argv[optind++];
	
	dpvs(input_directory);
	
	if (!input_directory)
	{
		usage(1);
	}
	
	struct cmdln_flags* flags = smalloc(sizeof(*flags));
	
	flags->input_directory = input_directory;
	flags->clear_dirfd = clear_dirfd;
	flags->verbose = verbose;
	
	dpvs(flags->input_directory);
	dpvb(flags->clear_dirfd);
	dpvb(flags->verbose);
	
	EXIT;
	return flags;
}











