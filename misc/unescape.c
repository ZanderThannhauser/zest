
#include <debug.h>

#include <parser/zebu.h>

#include "unescape.h"

struct unescaped unescape(
	struct zebu_token* token)
{
	ENTER;
	
	struct {
		uint8_t* data;
		unsigned n, cap;
	} buffer = {};
	
	void append(uint8_t c)
	{
		if (buffer.n == buffer.cap)
		{
			buffer.cap = buffer.cap << 1 ?: 1;
			buffer.data = srealloc(buffer.data, sizeof(*buffer.data) * buffer.cap);
		}
		
		buffer.data[buffer.n++] = c;
	}
	
	uint8_t* start = token->data + 1, *end = start + token->len - 2, c;
	
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


















