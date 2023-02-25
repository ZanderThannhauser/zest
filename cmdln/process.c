
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <debug.h>

#include "test_paths.h"
#include "verbose.h"
#include "process.h"

static const char* usage_message = "usage: zest [-vh] <test directories or files...>\n";

void cmdln_process(int argc, char* const* argv)
{
	ENTER;
	
	int opt;
	while ((opt = getopt(argc, argv, "vh")) != -1)
	{
		switch (opt)
		{
			case 'v':
				verbose = true;
				break;
			
			case 'h':
				fputs(usage_message, stdout), exit(0);
				break;
			
			default:
				fputs(usage_message, stdout), exit(1);
				break;
		}
	}
	
	test_paths = &argv[optind];
	
	if (!*test_paths)
		fputs(usage_message, stderr), exit(1);
	
	EXIT;
}











