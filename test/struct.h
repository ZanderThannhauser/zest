
struct test
{
	char path[PATH_MAX];
	
	unsigned index;
	
	struct {
		struct file {
			char path[PATH_MAX];
			uint8_t* data;
			unsigned size;
		}** data;
		unsigned n, cap;
	} files;
	
	struct {
		struct command {
			char** arg;
			unsigned n, cap;
		}** data;
		unsigned n, cap;
	} commands;
	
	struct {
		struct keyval {
			char *key, *val;
		}** data;
		unsigned n, cap;
	} environment;
	
	struct {
		uint8_t* data;
		unsigned size;
	} input, output;
	
	int exit_code;
};













