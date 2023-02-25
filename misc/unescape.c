
#include <debug.h>

#include <parser/zebu.h>

#include "unescape.h"

struct unescaped unescape(
	struct zebu_token* token)
{
	ENTER;
	
	struct {
		char* data;
		unsigned n, cap;
	} buffer = {};
	
	void append(char c)
	{
		if (buffer.n == buffer.cap)
		{
			buffer.cap = buffer.cap << 1 ?: 1;
			buffer.data = srealloc(buffer.data, sizeof(*buffer.data) * buffer.cap);
		}
		
		buffer.data[buffer.n++] = c;
	}
	
	char* start = (char*) token->data + 1, *end = start + token->len - 2, c;
	
	while (start < end)
	{
		if ((c = *start++) != '\\')
			append(c);
		else switch (c = *start++)
		{
			case 'n': append('\n'); break;
			
			case 't':
				TODO;
				break;
			
			case '0':
				TODO;
				break;
			
			case '\"':
				TODO;
				break;
			
			case '\'':
				TODO;
				break;
			
			default:
				TODO;
				break;
		}
	}
	
	append(0);
	
	EXIT;
	return (struct unescaped) {buffer.data, buffer.n - 1};
}


















