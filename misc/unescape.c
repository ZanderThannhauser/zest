
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <debug.h>

#include <parser/zebu.h>

#include <defines/argv0.h>

#include <memory/srealloc.h>

#include <enums/error.h>

#include "hexchars.h"
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
			
			case 't': append('\t'); break;
			
			case '0': append('\0'); break;
			
			case '\\': append('\\'); break;
			
			case '\"': append('\"'); break;
			
			case '\'': append('\''); break;
			
			case 'x':
			{
				char a = *start++, b = *start++;
				
				dpvc(a);
				dpvc(b);
				
				unsigned code = (index(hexchars, a) - hexchars) << 4 | (index(hexchars, b) - hexchars);
				
				dpv(code);
				
				append(code);
				
				break;
			}
			
			default:
			{
				fprintf(stderr, "%s: unknown escape sequence '\\%c'!\n", argv0, c);
				exit(e_syntax_error);
				break;
			}
		}
	}
	
	append(0);
	
	EXIT;
	return (struct unescaped) {buffer.data, buffer.n - 1};
}


















