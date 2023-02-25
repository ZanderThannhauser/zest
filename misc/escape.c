
#include <assert.h>

#include <debug.h>

#include <memory/srealloc.h>

#include "hexchars.h"
#include "escape.h"

char* escape(
	char* chars,
	unsigned len)
{
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
	
	append('\"');
	
	for (unsigned i = 0; i < len; i++)
	{
		switch (chars[i])
		{
			case '\t': append('\\'), append('t'); break;
			
			case '\n': append('\\'), append('n'); break;
			
			case '\'': append('\\'), append('\''); break;
			
			case '\\': append('\\'), append('\\'); break;
			
			case '\"': append('\\'), append('\"'); break;
			
			case ' ':
			case '.':
			case '/':
			case ':':
			case '-':
			case '|':
			case '!':
			case '(': case ')':
			case '[': case ']':
			case '{': case '}':
			case '<': case '>':
			case 'a' ... 'z':
			case 'A' ... 'Z':
			case '0' ... '9':
				append(chars[i]); break;
			
			default:
			{
				append('\\');
				append('x');
				append(hexchars[0xF & (chars[i] >> 4)]);
				append(hexchars[0xF & (chars[i] >> 0)]);
				break;
			}
		}
	}
	
	append('\"');
	
	append(0);
	
	assert(buffer.data);
	
	return buffer.data;
}















