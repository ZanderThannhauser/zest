
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

static const char* usage_message = ""
	"usage: hexdump <input-files>..." "\n"
"";

static const char* printable = ""
	"abcdefghijklmnopqrstuvwxyz"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"0123456789"
	"``!@#$%^&*()_+"
	"{}|"
	"[]\\"
	";\'"
	":\""
	",./"
	"<>?"
"";

static const char* argv0;

static void hexdump(int fd)
{
	ssize_t res = 0;
	unsigned i, n;
	off_t pos = 0;
	uint8_t row[16];
	
	errno = 0;
	
	do {
		printf("%08lX ", pos);
		
		for (n = 0; n < 16; )
			if ((res = read(fd, row + n, 16 - n)) > 0)
				n += res;
			else
				break;
		
		for (i = 0; i < n; i++)
			printf(" %02hhX", row[i]);
		for (; i < 16; i++)
			printf("   ");
		
		printf("  |");
		for (i = 0; i < n; i++)
			printf("%c", row[i] && index(printable, row[i]) ? row[i] : '.');
		puts("|");
		
		pos += n;
	} while (!errno && n);
	
	if (errno)
	{
		fprintf(stderr, "%s: error when reading input: %m\n", argv0);
		exit(1);
	}
}

int main(int argc, char* const* argv)
{
	argv0 = argv[0];
	
	int opt;
	while ((opt = getopt(argc, argv, "h")) > 0)
	{
		switch (opt)
		{
			case 'h':
			{
				fputs(usage_message, stdout);
				exit(0);
				break;
			}
			
			default:
			{
				fputs(usage_message, stderr);
				exit(1);
			}
		}
	}
	
	const char* arg;
	
	if (!(arg = argv[optind++]))
		hexdump(0);
	else do {
		int fd = open(arg, O_RDONLY);
		
		if (fd < 0)
			fprintf(stderr, "%s: open(\"%s\"): %m\n", argv0, arg),
			exit(1);
		else
			hexdump(fd);
		
		close(fd);
	} while ((arg = argv[optind++]));
	
	return 0;
}
















