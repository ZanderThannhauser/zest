
#include <debug.h>

#include <value/string/struct.h>
#include <value/string/new.h>
#include <value/free.h>

#include "../expression.h"

#include "file.h"

struct value* evaluate_file_form(
	struct zebu_expression* expression)
{
	ENTER;
	
	struct value* path = evaluate_expression(expression);
	
	if (path->kind != vk_string)
	{
		TODO;
		exit(1);
	}
	
	struct string_value* text = (void*) path;
	
	dpvsn(text->chars, text->len);
	
	char chunk[4096], *data = NULL;
	unsigned n = 0, cap = 0;
	ssize_t res;
	
	int fd = open(text->chars, O_RDONLY);
	
	if (fd < 0)
	{
		fprintf(stderr, "%s: open(\"%s\"): %m\n", argv0, text->chars);
		exit(e_failed_test);
	}
	else while (errno = 0, (res = read(fd, chunk, sizeof(chunk))) > 0)
	{
		while (n + res >= cap)
		{
			cap = cap << 1 ?: 1;
			data = srealloc(data, sizeof(*data) * cap);
		}
		
		memcpy(data + n, chunk, res), n += res;
	}
	
	if (res < 0)
	{
		TODO;
		exit(1);
	}
	
	data[n] = 0;
	
	dpv(n);
	dpvsn(data, n);
	
	close(fd);
	
	struct value* content = new_string_value(data, n);
	
	free_value(path);
	
	EXIT;
	return content;
}














