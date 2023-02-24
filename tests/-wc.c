
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

static const char* const usage_message = ""
	"usage: wc [-lwc] <files>..." "\n"
"";

int main(int argc, char* const* argv)
{
	const char* argv0 = argv[0];
	int display = 0;
	int opt;
	
	while ((opt = getopt(argc, argv, "lwc")) > 0)
	{
		switch (opt)
		{
			case 'l': display |= 4; break;
			case 'w': display |= 2; break;
			case 'c': display |= 1; break;
			default: fputs(usage_message, stderr), exit(1);
		}
	}
	
	if (!display)
		display = -1;
	
	unsigned lines = 0, words = 0, chars = 0;
	
	void count(int fd)
	{
		char buffer[4096];
		
		ssize_t res;
		
		bool in_word = false;
		
		while (errno = 0, (res = read(fd, buffer, sizeof(buffer))) > 0)
		{
			for (unsigned i = 0; i < res; i++)
			{
				switch (buffer[i])
				{
					case 'a' ... 'z':
					case 'A' ... 'Z':
						chars++, in_word = true;
						break;
					
					case '\n': lines++;
					
					default: if (in_word)
						words++, in_word = false;
				}
			}
		}
		
		if (res < 0)
		{
			fprintf(stderr, "%s: read(): %m\n", argv0);
			exit(1);
		}
		
		if (in_word)
			words++;
	}
	
	char* arg;
	
	if (!(arg = argv[optind++]))
		count(0);
	else do
	{
		int fd = open(arg, O_RDONLY);
		
		if (fd < 0)
			fprintf(stderr, "%s: open(\"%s\"): %m\n", argv0, arg),
			exit(1);
		else
			count(fd);
		
		close(fd);
	}
	while ((arg = argv[optind++]));
	
	if (display & 4) printf("%u ", lines);
	if (display & 2) printf("%u ", words);
	if (display & 1) printf("%u ", chars);
	puts("");
	
	return 0;
}






