
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
	struct zebu_root* root;
	unsigned refcount, startline, endline;
};

struct zebu_additive_expression
{
	struct zebu_multiplicative_expression* inner;
	struct zebu_additive_expression* left;
	struct zebu_multiplicative_expression* right;
	unsigned refcount, startline, endline;
};

struct zebu_and_expression
{
	struct zebu_equality_expression* inner;
	struct zebu_and_expression* left;
	struct zebu_equality_expression* right;
	unsigned refcount, startline, endline;
};

struct zebu_assertion
{
	struct zebu_expression* conditional;
	unsigned refcount, startline, endline;
};

struct zebu_complex_command
{
	struct zebu_primary_expression* redirect_in;
	struct zebu_primary_expression* redirect_out;
	struct {
		struct zebu_simple_command** data;
		unsigned n, cap;
	} subcommands;
	unsigned refcount, startline, endline;
};

struct zebu_conditional_expression
{
	struct zebu_logical_or_expression* conditional;
	struct zebu_conditional_expression* falsecase;
	struct zebu_logical_or_expression* inner;
	struct zebu_expression* truecase;
	unsigned refcount, startline, endline;
};

struct zebu_equality_expression
{
	struct zebu_relational_expression* inner;
	struct zebu_equality_expression* left;
	struct zebu_relational_expression* right;
	unsigned refcount, startline, endline;
};

struct zebu_exclusive_or_expression
{
	struct zebu_and_expression* inner;
	struct zebu_exclusive_or_expression* left;
	struct zebu_and_expression* right;
	unsigned refcount, startline, endline;
};

struct zebu_expression
{
	struct zebu_conditional_expression* inner;
	unsigned refcount, startline, endline;
};

struct zebu_file
{
	struct {
		struct zebu_token** data;
		unsigned n, cap;
	} chmods;
	struct zebu_expression* expression;
	struct zebu_token* octal;
	struct zebu_token* path;
	unsigned refcount, startline, endline;
};

struct zebu_inclusive_or_expression
{
	struct zebu_exclusive_or_expression* inner;
	struct zebu_inclusive_or_expression* left;
	struct zebu_exclusive_or_expression* right;
	unsigned refcount, startline, endline;
};

struct zebu_logical_and_expression
{
	struct zebu_inclusive_or_expression* inner;
	struct zebu_logical_and_expression* left;
	struct zebu_inclusive_or_expression* right;
	unsigned refcount, startline, endline;
};

struct zebu_logical_or_expression
{
	struct zebu_logical_and_expression* inner;
	struct zebu_logical_or_expression* left;
	struct zebu_logical_and_expression* right;
	unsigned refcount, startline, endline;
};

struct zebu_multiplicative_expression
{
	struct zebu_prefix_expression* inner;
	struct zebu_multiplicative_expression* left;
	struct zebu_prefix_expression* right;
	unsigned refcount, startline, endline;
};

struct zebu_prefix_expression
{
	struct zebu_expression* file;
	struct zebu_primary_expression* inner;
	struct zebu_complex_command* shell;
	struct zebu_prefix_expression* sub;
	unsigned refcount, startline, endline;
};

struct zebu_primary_expression
{
	struct zebu_token* integer;
	struct zebu_token* string;
	struct zebu_expression* sub;
	unsigned refcount, startline, endline;
};

struct zebu_relational_expression
{
	struct zebu_shift_expression* inner;
	struct zebu_relational_expression* left;
	struct zebu_shift_expression* right;
	unsigned refcount, startline, endline;
};

struct zebu_root
{
	struct {
		struct zebu_test** data;
		unsigned n, cap;
	} tests;
	unsigned refcount, startline, endline;
};

struct zebu_shift_expression
{
	struct zebu_additive_expression* inner;
	struct zebu_shift_expression* left;
	struct zebu_additive_expression* right;
	unsigned refcount, startline, endline;
};

struct zebu_simple_command
{
	struct {
		struct zebu_primary_expression** data;
		unsigned n, cap;
	} args;
	unsigned refcount, startline, endline;
};

struct zebu_test
{
	struct {
		struct zebu_assertion** data;
		unsigned n, cap;
	} assertions;
	struct {
		struct zebu_file** data;
		unsigned n, cap;
	} files;
	unsigned refcount, startline, endline;
};



extern struct zebu_token* inc_zebu_token(struct zebu_token* token);
extern struct zebu_$start* inc_zebu_$start(struct zebu_$start* ptree);
extern struct zebu_additive_expression* inc_zebu_additive_expression(struct zebu_additive_expression* ptree);
extern struct zebu_and_expression* inc_zebu_and_expression(struct zebu_and_expression* ptree);
extern struct zebu_assertion* inc_zebu_assertion(struct zebu_assertion* ptree);
extern struct zebu_complex_command* inc_zebu_complex_command(struct zebu_complex_command* ptree);
extern struct zebu_conditional_expression* inc_zebu_conditional_expression(struct zebu_conditional_expression* ptree);
extern struct zebu_equality_expression* inc_zebu_equality_expression(struct zebu_equality_expression* ptree);
extern struct zebu_exclusive_or_expression* inc_zebu_exclusive_or_expression(struct zebu_exclusive_or_expression* ptree);
extern struct zebu_expression* inc_zebu_expression(struct zebu_expression* ptree);
extern struct zebu_file* inc_zebu_file(struct zebu_file* ptree);
extern struct zebu_inclusive_or_expression* inc_zebu_inclusive_or_expression(struct zebu_inclusive_or_expression* ptree);
extern struct zebu_logical_and_expression* inc_zebu_logical_and_expression(struct zebu_logical_and_expression* ptree);
extern struct zebu_logical_or_expression* inc_zebu_logical_or_expression(struct zebu_logical_or_expression* ptree);
extern struct zebu_multiplicative_expression* inc_zebu_multiplicative_expression(struct zebu_multiplicative_expression* ptree);
extern struct zebu_prefix_expression* inc_zebu_prefix_expression(struct zebu_prefix_expression* ptree);
extern struct zebu_primary_expression* inc_zebu_primary_expression(struct zebu_primary_expression* ptree);
extern struct zebu_relational_expression* inc_zebu_relational_expression(struct zebu_relational_expression* ptree);
extern struct zebu_root* inc_zebu_root(struct zebu_root* ptree);
extern struct zebu_shift_expression* inc_zebu_shift_expression(struct zebu_shift_expression* ptree);
extern struct zebu_simple_command* inc_zebu_simple_command(struct zebu_simple_command* ptree);
extern struct zebu_test* inc_zebu_test(struct zebu_test* ptree);


extern void free_zebu_token(struct zebu_token* token);
extern void free_zebu_$start(struct zebu_$start* ptree);

extern void free_zebu_additive_expression(struct zebu_additive_expression* ptree);

extern void free_zebu_and_expression(struct zebu_and_expression* ptree);

extern void free_zebu_assertion(struct zebu_assertion* ptree);

extern void free_zebu_complex_command(struct zebu_complex_command* ptree);

extern void free_zebu_conditional_expression(struct zebu_conditional_expression* ptree);

extern void free_zebu_equality_expression(struct zebu_equality_expression* ptree);

extern void free_zebu_exclusive_or_expression(struct zebu_exclusive_or_expression* ptree);

extern void free_zebu_expression(struct zebu_expression* ptree);

extern void free_zebu_file(struct zebu_file* ptree);

extern void free_zebu_inclusive_or_expression(struct zebu_inclusive_or_expression* ptree);

extern void free_zebu_logical_and_expression(struct zebu_logical_and_expression* ptree);

extern void free_zebu_logical_or_expression(struct zebu_logical_or_expression* ptree);

extern void free_zebu_multiplicative_expression(struct zebu_multiplicative_expression* ptree);

extern void free_zebu_prefix_expression(struct zebu_prefix_expression* ptree);

extern void free_zebu_primary_expression(struct zebu_primary_expression* ptree);

extern void free_zebu_relational_expression(struct zebu_relational_expression* ptree);

extern void free_zebu_root(struct zebu_root* ptree);

extern void free_zebu_shift_expression(struct zebu_shift_expression* ptree);

extern void free_zebu_simple_command(struct zebu_simple_command* ptree);

extern void free_zebu_test(struct zebu_test* ptree);



struct zebu_$start* zebu_parse(FILE* stream);
