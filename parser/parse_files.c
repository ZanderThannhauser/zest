
#include <debug.h>

#include <test/new.h>

#include "parser.h"
#include "parse_files.h"

void parse_files(
	struct avl_tree_t* tests,
	const char* directory)
{
	ENTER;
	
	dpvs(directory);
	
	if (nftw(directory, ({
		int fn(
			const char *fpath,
			const struct stat *sb,
			int typeflag,
			struct FTW *ftwbuf)
		{
			unsigned len;
			
			if (true
				&& typeflag == FTW_F
				&& (len = strlen(fpath)) > 5
				&& strequals(fpath + len - 5, ".json"))
			{
				dpvs(fpath);
				
				FILE* stream = fopen(fpath, "r");
				
				if (!stream)
				{
					fprintf(stderr, "%s: fopen(\"%s\"): %m\n", argv0, fpath);
					exit(e_syscall_failed);
				}
				
				struct zebu_$start* start = zebu_parse(stream);
				
				for (unsigned i = 0; i < start->tests.n; i++)
				{
					struct zebu_test* ztest = start->tests.data[i];
					
					struct test* test = new_test(fpath, i, ztest);
					
					if (!avl_insert(tests, test))
					{
						fprintf(stderr, "%s: avl_insert(): %m\n", argv0);
						exit(e_syscall_failed);
					}
				}
				
				free_zebu_$start(start);
				
				fclose(stream);
			}
			
			return FTW_CONTINUE;
		}
		fn;
	}), 100, 0) < 0)
	{
		fprintf(stderr, "%s: nftw(\"%s\"): %m\n", argv0, directory);
		exit(e_syscall_failed);
	}
	
	EXIT;
}








