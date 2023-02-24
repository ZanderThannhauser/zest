
struct zebu_token;

struct unescaped
{
	uint8_t* data;
	unsigned n;
};

struct unescaped unescape(
	struct zebu_token* token);
