
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>

struct zebu_token
{
	unsigned char* data;
	unsigned len, refcount, line;
};

struct zebu_$start
{
	struct {
		struct zebu_test** data;
		unsigned n, cap;
	} tests;
	unsigned refcount, startline, endline;
};

struct zebu_command
{
	struct {
		struct zebu_token** data;
		unsigned n, cap;
	} args;
	unsigned refcount, startline, endline;
};

struct zebu_commands
{
	struct {
		struct zebu_command** data;
		unsigned n, cap;
	} commands;
	unsigned refcount, startline, endline;
};

struct zebu_environment
{
	struct {
		struct zebu_variable** data;
		unsigned n, cap;
	} variables;
	unsigned refcount, startline, endline;
};

struct zebu_file
{
	struct zebu_text* content;
	struct zebu_token* path;
	struct zebu_token* srcpath;
	unsigned refcount, startline, endline;
};

struct zebu_test
{
	unsigned int code;
	struct zebu_commands* commands;
	struct zebu_environment* environment;
	struct {
		struct zebu_file** data;
		unsigned n, cap;
	} files;
	struct zebu_text* input;
	struct zebu_text* output;
	unsigned refcount, startline, endline;
};

struct zebu_text
{
	struct {
		struct zebu_token** data;
		unsigned n, cap;
	} lines;
	unsigned refcount, startline, endline;
};

struct zebu_variable
{
	struct zebu_token* name;
	struct zebu_token* value;
	unsigned refcount, startline, endline;
};



extern struct zebu_token* inc_zebu_token(struct zebu_token* token);
extern struct zebu_$start* inc_zebu_$start(struct zebu_$start* ptree);
extern struct zebu_command* inc_zebu_command(struct zebu_command* ptree);
extern struct zebu_commands* inc_zebu_commands(struct zebu_commands* ptree);
extern struct zebu_environment* inc_zebu_environment(struct zebu_environment* ptree);
extern struct zebu_file* inc_zebu_file(struct zebu_file* ptree);
extern struct zebu_test* inc_zebu_test(struct zebu_test* ptree);
extern struct zebu_text* inc_zebu_text(struct zebu_text* ptree);
extern struct zebu_variable* inc_zebu_variable(struct zebu_variable* ptree);


extern void free_zebu_token(struct zebu_token* token);
extern void free_zebu_$start(struct zebu_$start* ptree);

extern void free_zebu_command(struct zebu_command* ptree);

extern void free_zebu_commands(struct zebu_commands* ptree);

extern void free_zebu_environment(struct zebu_environment* ptree);

extern void free_zebu_file(struct zebu_file* ptree);

extern void free_zebu_test(struct zebu_test* ptree);

extern void free_zebu_text(struct zebu_text* ptree);

extern void free_zebu_variable(struct zebu_variable* ptree);



struct zebu_$start* zebu_parse(FILE* stream);
