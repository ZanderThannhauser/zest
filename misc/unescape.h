
struct zebu_token;

struct unescaped
{
	char* chars;
	unsigned n;
};

struct unescaped unescape(
	struct zebu_token* token);
