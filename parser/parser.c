#include "parser.h"

#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

const unsigned zebu_shifts[96][17] = {
	[1][2] = 2,
	[2][3] = 3,
	[2][4] = 4,
	[4][6] = 6,
	[4][7] = 7,
	[4][8] = 8,
	[4][9] = 9,
	[4][10] = 10,
	[4][11] = 11,
	[4][12] = 12,
	[5][3] = 15,
	[5][13] = 14,
	[7][14] = 17,
	[8][14] = 19,
	[9][14] = 21,
	[10][14] = 23,
	[11][14] = 25,
	[12][14] = 27,
	[14][4] = 4,
	[17][4] = 30,
	[19][2] = 31,
	[21][4] = 33,
	[23][2] = 31,
	[25][2] = 31,
	[27][15] = 37,
	[29][3] = 15,
	[29][13] = 14,
	[30][6] = 40,
	[30][16] = 39,
	[31][3] = 43,
	[31][16] = 42,
	[32][6] = 45,
	[32][7] = 46,
	[32][8] = 47,
	[32][9] = 48,
	[32][10] = 49,
	[32][11] = 50,
	[32][12] = 51,
	[32][13] = 44,
	[33][6] = 54,
	[33][16] = 53,
	[34][6] = 45,
	[34][7] = 46,
	[34][8] = 47,
	[34][9] = 48,
	[34][10] = 49,
	[34][11] = 50,
	[34][12] = 51,
	[34][13] = 44,
	[35][6] = 45,
	[35][7] = 46,
	[35][8] = 47,
	[35][9] = 48,
	[35][10] = 49,
	[35][11] = 50,
	[35][12] = 51,
	[35][13] = 44,
	[36][6] = 45,
	[36][7] = 46,
	[36][8] = 47,
	[36][9] = 48,
	[36][10] = 49,
	[36][11] = 50,
	[36][12] = 51,
	[36][13] = 44,
	[37][6] = 45,
	[37][7] = 46,
	[37][8] = 47,
	[37][9] = 48,
	[37][10] = 49,
	[37][11] = 50,
	[37][12] = 51,
	[37][13] = 44,
	[39][14] = 60,
	[40][6] = 45,
	[40][7] = 46,
	[40][8] = 47,
	[40][9] = 48,
	[40][10] = 49,
	[40][11] = 50,
	[40][12] = 51,
	[40][13] = 44,
	[41][6] = 63,
	[41][13] = 62,
	[42][3] = 66,
	[42][13] = 65,
	[44][6] = 6,
	[44][7] = 7,
	[44][8] = 8,
	[44][9] = 9,
	[44][10] = 10,
	[44][11] = 11,
	[44][12] = 12,
	[46][14] = 17,
	[47][14] = 19,
	[48][14] = 21,
	[49][14] = 23,
	[50][14] = 25,
	[51][14] = 27,
	[53][14] = 75,
	[55][6] = 77,
	[55][13] = 76,
	[60][2] = 79,
	[60][16] = 80,
	[62][16] = 39,
	[63][6] = 45,
	[63][7] = 46,
	[63][8] = 47,
	[63][9] = 48,
	[63][10] = 49,
	[63][11] = 50,
	[63][12] = 51,
	[63][13] = 44,
	[65][16] = 84,
	[75][16] = 85,
	[76][16] = 53,
	[79][3] = 88,
	[79][16] = 87,
	[82][6] = 63,
	[82][13] = 62,
	[84][3] = 66,
	[84][13] = 65,
	[86][6] = 77,
	[86][13] = 76,
	[87][3] = 93,
	[87][13] = 92,
	[92][16] = 95,
	[95][3] = 93,
	[95][13] = 92,
};


const unsigned zebu_reduces[97][14] = {
	[3][5] = 1,
	[6][3] = 2,
	[6][13] = 2,
	[13][3] = 3,
	[13][13] = 3,
	[15][5] = 4,
	[16][5] = 5,
	[18][3] = 6,
	[18][13] = 6,
	[20][3] = 7,
	[20][13] = 7,
	[22][3] = 8,
	[22][13] = 8,
	[24][3] = 9,
	[24][13] = 9,
	[26][3] = 10,
	[26][13] = 10,
	[28][3] = 11,
	[28][13] = 11,
	[38][5] = 12,
	[43][6] = 13,
	[43][7] = 13,
	[43][8] = 13,
	[43][9] = 13,
	[43][10] = 13,
	[43][11] = 13,
	[43][12] = 13,
	[43][13] = 13,
	[45][3] = 14,
	[45][13] = 14,
	[52][3] = 15,
	[52][13] = 15,
	[54][6] = 16,
	[54][7] = 16,
	[54][8] = 16,
	[54][9] = 16,
	[54][10] = 16,
	[54][11] = 16,
	[54][12] = 16,
	[54][13] = 16,
	[56][3] = 17,
	[56][13] = 17,
	[57][3] = 18,
	[57][13] = 18,
	[58][3] = 19,
	[58][13] = 19,
	[59][3] = 20,
	[59][13] = 20,
	[61][3] = 21,
	[61][13] = 21,
	[64][3] = 22,
	[64][13] = 22,
	[66][6] = 23,
	[66][7] = 23,
	[66][8] = 23,
	[66][9] = 23,
	[66][10] = 23,
	[66][11] = 23,
	[66][12] = 23,
	[66][13] = 23,
	[67][6] = 24,
	[67][7] = 24,
	[67][8] = 24,
	[67][9] = 24,
	[67][10] = 24,
	[67][11] = 24,
	[67][12] = 24,
	[67][13] = 24,
	[68][3] = 25,
	[68][13] = 25,
	[69][3] = 26,
	[69][13] = 26,
	[70][3] = 27,
	[70][13] = 27,
	[71][3] = 28,
	[71][13] = 28,
	[72][3] = 29,
	[72][13] = 29,
	[73][3] = 30,
	[73][13] = 30,
	[74][3] = 31,
	[74][13] = 31,
	[77][6] = 32,
	[77][7] = 32,
	[77][8] = 32,
	[77][9] = 32,
	[77][10] = 32,
	[77][11] = 32,
	[77][12] = 32,
	[77][13] = 32,
	[78][6] = 33,
	[78][7] = 33,
	[78][8] = 33,
	[78][9] = 33,
	[78][10] = 33,
	[78][11] = 33,
	[78][12] = 33,
	[78][13] = 33,
	[80][6] = 34,
	[80][13] = 34,
	[81][6] = 35,
	[81][13] = 35,
	[83][3] = 36,
	[83][13] = 36,
	[85][6] = 37,
	[85][13] = 37,
	[88][6] = 13,
	[88][13] = 13,
	[89][3] = 38,
	[89][13] = 38,
	[90][6] = 39,
	[90][7] = 39,
	[90][8] = 39,
	[90][9] = 39,
	[90][10] = 39,
	[90][11] = 39,
	[90][12] = 39,
	[90][13] = 39,
	[91][6] = 40,
	[91][7] = 40,
	[91][8] = 40,
	[91][9] = 40,
	[91][10] = 40,
	[91][11] = 40,
	[91][12] = 40,
	[91][13] = 40,
	[93][6] = 23,
	[93][13] = 23,
	[94][6] = 24,
	[94][13] = 24,
	[96][6] = 39,
	[96][13] = 39,
};


const unsigned zebu_gotos[96][18] = {
	[2][1] = 5,
	[4][2] = 13,
	[5][3] = 16,
	[7][4] = 18,
	[8][5] = 20,
	[9][6] = 22,
	[10][7] = 24,
	[11][8] = 26,
	[12][9] = 28,
	[14][1] = 29,
	[19][10] = 32,
	[21][11] = 34,
	[23][10] = 35,
	[25][10] = 36,
	[29][3] = 38,
	[30][12] = 41,
	[32][13] = 52,
	[33][14] = 55,
	[34][13] = 56,
	[35][13] = 57,
	[36][13] = 58,
	[37][13] = 59,
	[40][13] = 61,
	[41][15] = 64,
	[42][16] = 67,
	[44][2] = 68,
	[46][4] = 69,
	[47][5] = 70,
	[48][6] = 71,
	[49][7] = 72,
	[50][8] = 73,
	[51][9] = 74,
	[55][17] = 78,
	[60][10] = 81,
	[62][12] = 82,
	[63][13] = 83,
	[76][14] = 86,
	[82][15] = 89,
	[84][16] = 90,
	[86][17] = 91,
	[87][16] = 94,
	[95][16] = 96,
};


const unsigned zebu_lexer[76][127] = {
	[1][9] = 15,
	[1][10] = 15,
	[1][32] = 15,
	[1][91] = 16,
	[2][9] = 15,
	[2][10] = 15,
	[2][32] = 15,
	[2][93] = 17,
	[2][123] = 18,
	[3][9] = 15,
	[3][10] = 15,
	[3][32] = 15,
	[4][9] = 15,
	[4][10] = 15,
	[4][32] = 15,
	[4][34] = 20,
	[4][125] = 21,
	[5][9] = 15,
	[5][10] = 15,
	[5][32] = 15,
	[5][44] = 22,
	[5][93] = 17,
	[6][9] = 15,
	[6][10] = 15,
	[6][32] = 15,
	[6][58] = 23,
	[7][9] = 15,
	[7][10] = 15,
	[7][32] = 15,
	[7][123] = 18,
	[8][9] = 15,
	[8][10] = 15,
	[8][32] = 15,
	[8][48] = 24,
	[8][49] = 24,
	[8][50] = 24,
	[8][51] = 24,
	[8][52] = 24,
	[8][53] = 24,
	[8][54] = 24,
	[8][55] = 24,
	[8][56] = 24,
	[8][57] = 24,
	[9][9] = 15,
	[9][10] = 15,
	[9][32] = 15,
	[9][34] = 25,
	[9][125] = 21,
	[10][9] = 15,
	[10][10] = 15,
	[10][32] = 15,
	[10][34] = 25,
	[10][93] = 17,
	[11][9] = 15,
	[11][10] = 15,
	[11][32] = 15,
	[11][34] = 20,
	[11][44] = 22,
	[11][125] = 21,
	[12][9] = 15,
	[12][10] = 15,
	[12][32] = 15,
	[12][44] = 22,
	[12][125] = 21,
	[13][9] = 15,
	[13][10] = 15,
	[13][32] = 15,
	[13][34] = 25,
	[13][91] = 16,
	[14][9] = 15,
	[14][10] = 15,
	[14][32] = 15,
	[14][34] = 25,
	[15][9] = 15,
	[15][10] = 15,
	[15][32] = 15,
	[20][99] = 26,
	[20][101] = 27,
	[20][102] = 28,
	[20][105] = 29,
	[20][111] = 30,
	[24][48] = 24,
	[24][49] = 24,
	[24][50] = 24,
	[24][51] = 24,
	[24][52] = 24,
	[24][53] = 24,
	[24][54] = 24,
	[24][55] = 24,
	[24][56] = 24,
	[24][57] = 24,
	[25][32] = 25,
	[25][33] = 25,
	[25][34] = 31,
	[25][35] = 25,
	[25][36] = 25,
	[25][37] = 25,
	[25][39] = 25,
	[25][40] = 25,
	[25][41] = 25,
	[25][42] = 25,
	[25][43] = 25,
	[25][45] = 25,
	[25][46] = 25,
	[25][47] = 25,
	[25][48] = 25,
	[25][49] = 25,
	[25][50] = 25,
	[25][51] = 25,
	[25][52] = 25,
	[25][53] = 25,
	[25][54] = 25,
	[25][55] = 25,
	[25][56] = 25,
	[25][57] = 25,
	[25][58] = 25,
	[25][59] = 25,
	[25][61] = 25,
	[25][65] = 25,
	[25][66] = 25,
	[25][67] = 25,
	[25][68] = 25,
	[25][69] = 25,
	[25][70] = 25,
	[25][71] = 25,
	[25][72] = 25,
	[25][73] = 25,
	[25][74] = 25,
	[25][75] = 25,
	[25][76] = 25,
	[25][77] = 25,
	[25][78] = 25,
	[25][79] = 25,
	[25][80] = 25,
	[25][81] = 25,
	[25][82] = 25,
	[25][83] = 25,
	[25][84] = 25,
	[25][85] = 25,
	[25][86] = 25,
	[25][87] = 25,
	[25][88] = 25,
	[25][89] = 25,
	[25][90] = 25,
	[25][91] = 25,
	[25][92] = 32,
	[25][93] = 25,
	[25][95] = 25,
	[25][96] = 25,
	[25][97] = 25,
	[25][98] = 25,
	[25][99] = 25,
	[25][100] = 25,
	[25][101] = 25,
	[25][102] = 25,
	[25][103] = 25,
	[25][104] = 25,
	[25][105] = 25,
	[25][106] = 25,
	[25][107] = 25,
	[25][108] = 25,
	[25][109] = 25,
	[25][110] = 25,
	[25][111] = 25,
	[25][112] = 25,
	[25][113] = 25,
	[25][114] = 25,
	[25][115] = 25,
	[25][116] = 25,
	[25][117] = 25,
	[25][118] = 25,
	[25][119] = 25,
	[25][120] = 25,
	[25][121] = 25,
	[25][122] = 25,
	[25][126] = 25,
	[26][111] = 33,
	[27][110] = 34,
	[27][120] = 35,
	[28][105] = 36,
	[29][110] = 37,
	[30][117] = 38,
	[32][34] = 25,
	[32][110] = 25,
	[33][109] = 39,
	[34][118] = 40,
	[35][105] = 41,
	[36][108] = 42,
	[37][112] = 43,
	[38][116] = 44,
	[39][109] = 45,
	[40][105] = 46,
	[41][116] = 47,
	[42][101] = 48,
	[43][117] = 49,
	[44][112] = 50,
	[45][97] = 51,
	[46][114] = 52,
	[47][45] = 53,
	[48][115] = 54,
	[49][116] = 55,
	[50][117] = 56,
	[51][110] = 57,
	[52][111] = 58,
	[53][99] = 59,
	[54][34] = 60,
	[55][34] = 61,
	[56][116] = 62,
	[57][100] = 63,
	[58][110] = 64,
	[59][111] = 65,
	[62][34] = 66,
	[63][115] = 67,
	[64][109] = 68,
	[65][100] = 69,
	[67][34] = 70,
	[68][101] = 71,
	[69][101] = 72,
	[71][110] = 73,
	[72][34] = 74,
	[73][116] = 75,
	[75][34] = 76,
};


const unsigned zebu_lexer_starts[97] = {
	[1] = 1,
	[2] = 2,
	[3] = 3,
	[4] = 4,
	[5] = 5,
	[6] = 5,
	[7] = 6,
	[8] = 6,
	[9] = 6,
	[10] = 6,
	[11] = 6,
	[12] = 6,
	[13] = 5,
	[14] = 7,
	[15] = 3,
	[16] = 3,
	[17] = 7,
	[18] = 5,
	[19] = 1,
	[20] = 5,
	[21] = 7,
	[22] = 5,
	[23] = 1,
	[24] = 5,
	[25] = 1,
	[26] = 5,
	[27] = 8,
	[28] = 5,
	[29] = 5,
	[30] = 9,
	[31] = 10,
	[32] = 11,
	[33] = 9,
	[34] = 11,
	[35] = 11,
	[36] = 11,
	[37] = 11,
	[38] = 3,
	[39] = 6,
	[40] = 11,
	[41] = 12,
	[42] = 5,
	[43] = 11,
	[44] = 4,
	[45] = 5,
	[46] = 6,
	[47] = 6,
	[48] = 6,
	[49] = 6,
	[50] = 6,
	[51] = 6,
	[52] = 5,
	[53] = 6,
	[54] = 11,
	[55] = 12,
	[56] = 5,
	[57] = 5,
	[58] = 5,
	[59] = 5,
	[60] = 13,
	[61] = 5,
	[62] = 14,
	[63] = 11,
	[64] = 5,
	[65] = 14,
	[66] = 11,
	[67] = 11,
	[68] = 5,
	[69] = 5,
	[70] = 5,
	[71] = 5,
	[72] = 5,
	[73] = 5,
	[74] = 5,
	[75] = 14,
	[76] = 14,
	[77] = 11,
	[78] = 11,
	[79] = 10,
	[80] = 12,
	[81] = 12,
	[82] = 12,
	[83] = 5,
	[84] = 5,
	[85] = 12,
	[86] = 12,
	[87] = 5,
	[88] = 12,
	[89] = 5,
	[90] = 11,
	[91] = 11,
	[92] = 14,
	[93] = 12,
	[94] = 12,
	[95] = 5,
	[96] = 12,
};


const unsigned zebu_lexer_accepts[77] = {
	[15] = 1,
	[16] = 2,
	[17] = 3,
	[18] = 4,
	[19] = 5,
	[21] = 6,
	[22] = 13,
	[23] = 14,
	[24] = 15,
	[31] = 16,
	[60] = 7,
	[61] = 10,
	[66] = 11,
	[70] = 8,
	[74] = 12,
	[76] = 9,
};


const unsigned zebu_lexer_EOFs[4] = {
	[3] = 19,
};


#ifdef ZEBU_DEBUG
#include <stdbool.h>

struct link
{
	bool is_last;
	struct link* prev;
};

static void print_links(struct link* link)
{
	if (!link) return;
	
	print_links(link->prev);
	
	if (link->is_last)
		fputs("    ", stdout);
	else
		fputs("│   ", stdout);
}

static void print_tree_escape(char *out, char in)
{
	switch (in)
	{
		case ' ':
		case '~':
		case '!':
		case '@':
		case '#':
		case '$':
		case '%':
		case '^':
		case '&':
		case '*':
		case '-':
		case '+':
		case '=':
		case '|':
		case '<': case '>':
		case '(': case ')':
		case '{': case '}':
		case '[': case ']':
		case ':': case ';':
		case ',': case '.':
		case '_':
		case '/':
		case '`':
		case '0' ... '9':
		case 'a' ... 'z':
		case 'A' ... 'Z':
			*out++ = in;
			*out = 0;
			break;
		
		case '\\': *out++ = '\\', *out++ = '\\', *out = 0; break;
		
		case '\'': *out++ = '\\', *out++ = '\'', *out = 0; break;
		
		case '\"': *out++ = '\\', *out++ = '\"', *out = 0; break;
		
		case '\t': *out++ = '\\', *out++ = 't', *out = 0; break;
		
		case '\n': *out++ = '\\', *out++ = 'n', *out = 0; break;
		
		default:
			sprintf(out, "\\x%02X", in);
			break;
	}
}

enum prefix
{
	p_root,
	p_not_last_child,
	p_last_child,
};

void print_token_leaf(struct link* links, enum prefix p, const char* name, struct zebu_token* token)
{
	print_links(links);
	switch (p)
	{
		case p_root: break;
		case p_not_last_child: fputs("├── ", stdout); break;
		case p_last_child: fputs("└── ", stdout); break;
	}
	printf("\e[32m%s\e[0m (\e[35m\"", name);
	char escape[10];
	for (unsigned i = 0, n = token->len; i < n; i++)
		print_tree_escape(escape, token->data[i]), fputs(escape, stdout);
	printf("\"\e[0m)\n");
}

void print_scanf_leaf(struct link* links, enum prefix p, const char* name, const char* format, ...)
{
	va_list ap;
	print_links(links);
	switch (p)
	{
		case p_root: break;
		case p_not_last_child: fputs("├── ", stdout); break;
		case p_last_child: fputs("└── ", stdout); break;
	}
	printf("\e[32m%s\e[0m (\e[35m%s\e[m: \e[35m", name, format);
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	printf("\e[0m)\n");
}

void print_empty_leaf(struct link* links, enum prefix p, const char* type, const char* name)
{
	print_links(links);
	switch (p)
	{
		case p_root: break;
		case p_not_last_child: fputs("├── ", stdout); break;
		case p_last_child: fputs("└── ", stdout); break;
	}
	printf("\e[31m%s\e[0m (\e[36m%s\e[0m)\n", name, type);
}
void print_zebu_$start(struct link* links, enum prefix p, const char* name, struct zebu_$start* ptree);

void print_zebu_environment(struct link* links, enum prefix p, const char* name, struct zebu_environment* ptree);

void print_zebu_file(struct link* links, enum prefix p, const char* name, struct zebu_file* ptree);

void print_zebu_test(struct link* links, enum prefix p, const char* name, struct zebu_test* ptree);

void print_zebu_text(struct link* links, enum prefix p, const char* name, struct zebu_text* ptree);

void print_zebu_variable(struct link* links, enum prefix p, const char* name, struct zebu_variable* ptree);

void print_zebu_$start(struct link* links, enum prefix p, const char* name, struct zebu_$start* ptree)
{
	print_links(links);
	
	struct link* new = NULL;
	
	switch (p)
	{
		case p_root:
			break;
		
		case p_not_last_child:
			fputs("├── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = false;
			new->prev = links;
			break;
		
		case p_last_child:
			fputs("└── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = true;
			new->prev = links;
		break;
	}
	printf("\e[34m%s\e[m (\e[36m$start\e[m)\n", name);
	if (ptree->tests.n)
	{
		for (unsigned i = 0, n = ptree->tests.n; i < n; i++)
		{
			char label[5 + 30];
			snprintf(label, sizeof(label), "tests[%u]", i);
			print_zebu_test(new ?: links, i + 1 < n ? p_not_last_child : p_last_child, label, ptree->tests.data[i]);
		}
	}
	else
	{
		print_empty_leaf(new ?: links, p_last_child, "test[]", "tests");
	}
	free(new);
}
void print_zebu_environment(struct link* links, enum prefix p, const char* name, struct zebu_environment* ptree)
{
	print_links(links);
	
	struct link* new = NULL;
	
	switch (p)
	{
		case p_root:
			break;
		
		case p_not_last_child:
			fputs("├── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = false;
			new->prev = links;
			break;
		
		case p_last_child:
			fputs("└── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = true;
			new->prev = links;
		break;
	}
	printf("\e[34m%s\e[m (\e[36menvironment\e[m)\n", name);
	if (ptree->variables.n)
	{
		for (unsigned i = 0, n = ptree->variables.n; i < n; i++)
		{
			char label[9 + 30];
			snprintf(label, sizeof(label), "variables[%u]", i);
			print_zebu_variable(new ?: links, i + 1 < n ? p_not_last_child : p_last_child, label, ptree->variables.data[i]);
		}
	}
	else
	{
		print_empty_leaf(new ?: links, p_last_child, "variable[]", "variables");
	}
	free(new);
}
void print_zebu_file(struct link* links, enum prefix p, const char* name, struct zebu_file* ptree)
{
	print_links(links);
	
	struct link* new = NULL;
	
	switch (p)
	{
		case p_root:
			break;
		
		case p_not_last_child:
			fputs("├── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = false;
			new->prev = links;
			break;
		
		case p_last_child:
			fputs("└── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = true;
			new->prev = links;
		break;
	}
	printf("\e[34m%s\e[m (\e[36mfile\e[m)\n", name);
	if (ptree->content)
		print_zebu_text(new ?: links, p_not_last_child, "content", ptree->content);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "text", "content");
	if (ptree->path)
		print_token_leaf(new ?: links, p_not_last_child, "path", ptree->path);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "token", "path");
	if (ptree->srcpath)
		print_token_leaf(new ?: links, p_last_child, "srcpath", ptree->srcpath);
	else
		print_empty_leaf(new ?: links, p_last_child, "token", "srcpath");
	free(new);
}
void print_zebu_test(struct link* links, enum prefix p, const char* name, struct zebu_test* ptree)
{
	print_links(links);
	
	struct link* new = NULL;
	
	switch (p)
	{
		case p_root:
			break;
		
		case p_not_last_child:
			fputs("├── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = false;
			new->prev = links;
			break;
		
		case p_last_child:
			fputs("└── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = true;
			new->prev = links;
		break;
	}
	printf("\e[34m%s\e[m (\e[36mtest\e[m)\n", name);
	print_scanf_leaf(new ?: links, p_not_last_child, "code", "%u", ptree->code);
	if (ptree->commands)
		print_zebu_text(new ?: links, p_not_last_child, "commands", ptree->commands);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "text", "commands");
	if (ptree->environment)
		print_zebu_environment(new ?: links, p_not_last_child, "environment", ptree->environment);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "environment", "environment");
	if (ptree->files.n)
	{
		for (unsigned i = 0, n = ptree->files.n; i < n; i++)
		{
			char label[5 + 30];
			snprintf(label, sizeof(label), "files[%u]", i);
			print_zebu_file(new ?: links, i + 1 < n ? p_not_last_child : p_not_last_child, label, ptree->files.data[i]);
		}
	}
	else
	{
		print_empty_leaf(new ?: links, p_not_last_child, "file[]", "files");
	}
	if (ptree->input)
		print_zebu_text(new ?: links, p_not_last_child, "input", ptree->input);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "text", "input");
	if (ptree->output)
		print_zebu_text(new ?: links, p_last_child, "output", ptree->output);
	else
		print_empty_leaf(new ?: links, p_last_child, "text", "output");
	free(new);
}
void print_zebu_text(struct link* links, enum prefix p, const char* name, struct zebu_text* ptree)
{
	print_links(links);
	
	struct link* new = NULL;
	
	switch (p)
	{
		case p_root:
			break;
		
		case p_not_last_child:
			fputs("├── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = false;
			new->prev = links;
			break;
		
		case p_last_child:
			fputs("└── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = true;
			new->prev = links;
		break;
	}
	printf("\e[34m%s\e[m (\e[36mtext\e[m)\n", name);
	if (ptree->lines.n)
	{
		for (unsigned i = 0, n = ptree->lines.n; i < n; i++)
		{
			char label[5 + 30];
			snprintf(label, sizeof(label), "lines[%u]", i);
			print_token_leaf(new ?: links, i + 1 < n ? p_not_last_child : p_last_child, label, ptree->lines.data[i]);
		}
	}
	else
	{
		print_empty_leaf(new ?: links, p_last_child, "token[]", "lines");
	}
	free(new);
}
void print_zebu_variable(struct link* links, enum prefix p, const char* name, struct zebu_variable* ptree)
{
	print_links(links);
	
	struct link* new = NULL;
	
	switch (p)
	{
		case p_root:
			break;
		
		case p_not_last_child:
			fputs("├── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = false;
			new->prev = links;
			break;
		
		case p_last_child:
			fputs("└── ", stdout);
			new = malloc(sizeof(*new));
			new->is_last = true;
			new->prev = links;
		break;
	}
	printf("\e[34m%s\e[m (\e[36mvariable\e[m)\n", name);
	if (ptree->name)
		print_token_leaf(new ?: links, p_not_last_child, "name", ptree->name);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "token", "name");
	if (ptree->value)
		print_token_leaf(new ?: links, p_last_child, "value", ptree->value);
	else
		print_empty_leaf(new ?: links, p_last_child, "token", "value");
	free(new);
}

#endif

struct zebu_token* inc_zebu_token(struct zebu_token* token)
{
	if (token) token->refcount++;
	return token;
}
struct zebu_$start* inc_zebu_$start(struct zebu_$start* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}

struct zebu_environment* inc_zebu_environment(struct zebu_environment* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}

struct zebu_file* inc_zebu_file(struct zebu_file* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}

struct zebu_test* inc_zebu_test(struct zebu_test* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}

struct zebu_text* inc_zebu_text(struct zebu_text* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}

struct zebu_variable* inc_zebu_variable(struct zebu_variable* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}



extern void free_zebu_token(struct zebu_token* token);
extern void free_zebu_$start(struct zebu_$start* ptree);

extern void free_zebu_environment(struct zebu_environment* ptree);

extern void free_zebu_file(struct zebu_file* ptree);

extern void free_zebu_test(struct zebu_test* ptree);

extern void free_zebu_text(struct zebu_text* ptree);

extern void free_zebu_variable(struct zebu_variable* ptree);

void free_zebu_token(struct zebu_token* token)
{
	if (token && !--token->refcount)
	{
		free(token->data);
		free(token);
	}
}
void free_zebu_$start(struct zebu_$start* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		for (unsigned i = 0, n = ptree->tests.n; i < n; i++)
			free_zebu_test(ptree->tests.data[i]);
		free(ptree->tests.data);
		free(ptree);
	}
}

void free_zebu_environment(struct zebu_environment* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		for (unsigned i = 0, n = ptree->variables.n; i < n; i++)
			free_zebu_variable(ptree->variables.data[i]);
		free(ptree->variables.data);
		free(ptree);
	}
}

void free_zebu_file(struct zebu_file* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		free_zebu_text(ptree->content);
		free_zebu_token(ptree->path);
		free_zebu_token(ptree->srcpath);
		free(ptree);
	}
}

void free_zebu_test(struct zebu_test* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		free_zebu_text(ptree->commands);
		free_zebu_environment(ptree->environment);
		for (unsigned i = 0, n = ptree->files.n; i < n; i++)
			free_zebu_file(ptree->files.data[i]);
		free(ptree->files.data);
		free_zebu_text(ptree->input);
		free_zebu_text(ptree->output);
		free(ptree);
	}
}

void free_zebu_text(struct zebu_text* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		for (unsigned i = 0, n = ptree->lines.n; i < n; i++)
			free_zebu_token(ptree->lines.data[i]);
		free(ptree->lines.data);
		free(ptree);
	}
}

void free_zebu_variable(struct zebu_variable* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		free_zebu_token(ptree->name);
		free_zebu_token(ptree->value);
		free(ptree);
	}
}



#define N(array) (sizeof(array) / sizeof(*array))

#ifdef ZEBU_DEBUG
static void escape(char *out, unsigned char in)
{
	switch (in)
	{
		case ' ':
		case '~':
		case '!':
		case '@':
		case '#':
		case '$':
		case '%':
		case '^':
		case '&':
		case '*':
		case '-':
		case '+':
		case '=':
		case '|':
		case '<': case '>':
		case '(': case ')':
		case '{': case '}':
		case '[': case ']':
		case ':': case ';':
		case ',': case '.':
		case '_':
		case '/':
		case '0' ... '9':
		case 'a' ... 'z':
		case 'A' ... 'Z':
			*out++ = in;
			*out = 0;
			break;
		
		case '\\': *out++ = '\\', *out++ = '\\', *out = 0; break;
		
		case '\"': *out++ = '\\', *out++ = '\"', *out = 0; break;
		
		case '\t': *out++ = '\\', *out++ = 't', *out = 0; break;
		
		case '\n': *out++ = '\\', *out++ = 'n', *out = 0; break;
		
		default:
			sprintf(out, "\\x%02X", in);
			break;
	}
}
#endif

struct zebu_$start* zebu_parse(FILE* stream)
{
	void* root;
	struct { unsigned* data, n, cap; } yacc = {};
	struct { void** data; unsigned n, cap; } data = {};
	struct { unsigned char* data; unsigned n, cap; } lexer = {};
	
	void push_state(unsigned y)
	{
		if (yacc.n + 1 >= yacc.cap)
		{
			yacc.cap = yacc.cap << 1 ?: 1;
			yacc.data = realloc(yacc.data, sizeof(*yacc.data) * yacc.cap);
		}
		yacc.data[yacc.n++] = y;
	}
	
	void push_data(void* d)
	{
		if (data.n + 1 >= data.cap)
		{
			data.cap = data.cap << 1 ?: 1;
			data.data = realloc(data.data, sizeof(*data.data) * data.cap);
		}
		data.data[data.n++] = d;
	}
	
	#ifdef ZEBU_DEBUG
	void ddprintf(const char* fmt, ...)
	{
		for (unsigned i = 0, n = yacc.n; i < n; i++)
			printf("%u ", yacc.data[i]);
		
		printf("| ");
		
		va_list va;
		va_start(va, fmt);
		vprintf(fmt, va);
		va_end(va);
	}
	#endif
	
	unsigned y, t, s, r;
	void* td;
	
	void read_token(unsigned l)
	{
		void append(unsigned char c)
		{
			while (lexer.n + 1 >= lexer.cap)
			{
				lexer.cap = lexer.cap << 1 ?: 1;
				#ifdef ZEBU_DEBUG
				ddprintf("lexer.cap == %u\n", lexer.cap);
				#endif
				lexer.data = realloc(lexer.data, lexer.cap);
			}
			
			lexer.data[lexer.n++] = c;
		}
		
		unsigned original_l = l, i = 0, a, b, c, f = 0;
		
		t = 0;
		
		#ifdef ZEBU_DEBUG
		ddprintf("lexer: \"%.*s\": l = %u\n", lexer.n, lexer.data, l);
		#endif
		
		while (1)
		{
			if (i < lexer.n)
			{
				c = lexer.data[i];
				
				#ifdef ZEBU_DEBUG
				char escaped[10];
				
				escape(escaped, c);
				
				ddprintf("lexer: c = '%s' (0x%X) (from cache)\n", escaped, c);
				#endif
				
				a = l < N(zebu_lexer) && c < N(*zebu_lexer) ? zebu_lexer[l][c] : 0;
			}
			else if ((c = getc(stream)) != EOF)
			{
				append(c);
				
				#ifdef ZEBU_DEBUG
				char escaped[10];
				
				escape(escaped, c);
				
				ddprintf("lexer: c = '%s' (0x%X)\n", escaped, c);
				#endif
				
				a = l < N(zebu_lexer) && c < N(*zebu_lexer) ? zebu_lexer[l][c] : 0;
			}
			else
			{
				c = EOF;
				
				#ifdef ZEBU_DEBUG
				ddprintf("lexer: c = <EOF>\n");
				#endif
				
				a = l < N(zebu_lexer_EOFs) ? zebu_lexer_EOFs[l] : 0;
			}
			
			b = l < N(zebu_lexer_accepts) ? zebu_lexer_accepts[l] : 0;
			
			#ifdef ZEBU_DEBUG
			ddprintf("lexer: \"%.*s\" (%u): a = %u, b = %u\n", lexer.n, lexer.data, i, a, b);
			#endif
			
			if (a)
			{
				if (b)
				{
					l = a, t = b, f = i++;
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: l = %u\n", l);
					#endif
				}
				else
				{
					l = a, i++;
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: l = %u\n", l);
					#endif
				}
			}
			else if (b)
			{
				#ifdef ZEBU_DEBUG
				ddprintf("lexer: token: \"%.*s\"\n", i, lexer.data);
				#endif
				
				if (!lexer.n)
				{
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: EOF.\n");
					#endif
					t = b, td = NULL;
					break;
				}
				else if (b == 1)
				{
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: whitespace: \"%.*s\"\n", i, lexer.data);
					#endif
					
					l = original_l, t = 0;
					memmove(lexer.data, lexer.data + i, lexer.n - i), lexer.n -= i, i = 0;
				}
				else
				{
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: i = %u\n", i);
					#endif
					
					struct zebu_token* token = malloc(sizeof(*token));
					token->refcount = 1;
					token->data = memcpy(malloc(i + 1), lexer.data, i);
					token->len = i;
					token->data[i] = 0;
					t = b, td = token;
					
					memmove(lexer.data, lexer.data + i, lexer.n - i), lexer.n -= i;
					break;
				}
			}
			else if (f)
			{
				if (t == 1)
				{
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: falling back to whitespace: \"%.*s\"\n", f, lexer.data);
					#endif
					
					l = original_l, t = 0;
					memmove(lexer.data, lexer.data + f, lexer.n - f), lexer.n -= f, f = 0, i = 0;
				}
				else
				{
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: falling back to token: \"%.*s\"\n", f, lexer.data);
					#endif
					
					struct zebu_token* token = malloc(sizeof(*token));
					token->refcount = 1;
					token->data = memcpy(malloc(f + 1), lexer.data, f);
					token->len = f;
					token->data[f] = 0;
					td = token;
					
					memmove(lexer.data, lexer.data + f, lexer.n - f), lexer.n -= f, f = 0;
					break;
				}
			}
			else
			{
				if (c == (unsigned) EOF)
					fprintf(stderr, "zebu: unexpected '<EOF>' when reading '%.*s'!\n", i, lexer.data);
				else
					fprintf(stderr, "zebu: unexpected '%c' when reading '%.*s'!\n", c, i, lexer.data);
				
				abort();
			}
		}
	}
	
	push_state(1), y = 1, read_token(1);
	
	#ifdef ZEBU_DEBUG
	ddprintf("y = %u, t = %u\n", y, t);
	#endif
	
	while (yacc.n)
	{
		if (y < N(zebu_shifts) && t < N(*zebu_shifts) && (s = zebu_shifts[y][t]))
		{
			#ifdef ZEBU_DEBUG
			ddprintf("s == %u\n", s);
			#endif
			
			y = s, push_state(y), push_data(td);
			
			read_token(zebu_lexer_starts[y]);
			
			#ifdef ZEBU_DEBUG
			ddprintf("t = %u\n", t);
			#endif
		}
		else if (y < N( zebu_reduces) && t < N(*zebu_reduces) && (r = zebu_reduces[y][t]))
		{
			#ifdef ZEBU_DEBUG
			ddprintf("r == %u\n", r);
			#endif
			
			unsigned g;
			void* d;
			
			switch (r)
{
	case 5:
	{
		struct zebu_$start* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_$start* trie = data.data[--yacc.n, --data.n];
			if (trie->tests.n)
			{
				while (value->tests.n + trie->tests.n > value->tests.cap)
				{
					value->tests.cap = value->tests.cap << 1 ?: 1;
					value->tests.data = realloc(value->tests.data, sizeof(*value->tests.data) * value->tests.cap);
				}
				memmove(value->tests.data + trie->tests.n, value->tests.data, sizeof(*value->tests.data) * value->tests.n);
				for (unsigned i = 0, n = trie->tests.n; i < n; i++)
					value->tests.data[i] = inc_zebu_test(trie->tests.data[i]);
				value->tests.n += trie->tests.n;
			}
			free_zebu_$start(trie);
		}
		{
		struct zebu_test* subgrammar = data.data[--yacc.n, --data.n];
		if (value->tests.n == value->tests.cap)
		{
			value->tests.cap = value->tests.cap << 1 ?: 1;
			value->tests.data = realloc(value->tests.data, sizeof(*value->tests.data) * value->tests.cap);
		}
		memmove(value->tests.data + 1, value->tests.data, sizeof(*value->tests.data) * value->tests.n);
		value->tests.data[0] = inc_zebu_test(subgrammar), value->tests.n++;
		free_zebu_test(subgrammar);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 18;
		break;
	}
	case 1:
	{
		struct zebu_$start* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 18;
		break;
	}
	case 12:
	{
		struct zebu_$start* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_$start* trie = data.data[--yacc.n, --data.n];
			if (trie->tests.n)
			{
				while (value->tests.n + trie->tests.n > value->tests.cap)
				{
					value->tests.cap = value->tests.cap << 1 ?: 1;
					value->tests.data = realloc(value->tests.data, sizeof(*value->tests.data) * value->tests.cap);
				}
				memmove(value->tests.data + trie->tests.n, value->tests.data, sizeof(*value->tests.data) * value->tests.n);
				for (unsigned i = 0, n = trie->tests.n; i < n; i++)
					value->tests.data[i] = inc_zebu_test(trie->tests.data[i]);
				value->tests.n += trie->tests.n;
			}
			free_zebu_$start(trie);
		}
		{
		struct zebu_test* subgrammar = data.data[--yacc.n, --data.n];
		if (value->tests.n == value->tests.cap)
		{
			value->tests.cap = value->tests.cap << 1 ?: 1;
			value->tests.data = realloc(value->tests.data, sizeof(*value->tests.data) * value->tests.cap);
		}
		memmove(value->tests.data + 1, value->tests.data, sizeof(*value->tests.data) * value->tests.n);
		value->tests.data[0] = inc_zebu_test(subgrammar), value->tests.n++;
		free_zebu_test(subgrammar);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 3;
		break;
	}
	case 4:
	{
		struct zebu_$start* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 3;
		break;
	}
	case 40:
	{
		struct zebu_environment* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_environment* trie = data.data[--yacc.n, --data.n];
			if (trie->variables.n)
			{
				while (value->variables.n + trie->variables.n > value->variables.cap)
				{
					value->variables.cap = value->variables.cap << 1 ?: 1;
					value->variables.data = realloc(value->variables.data, sizeof(*value->variables.data) * value->variables.cap);
				}
				memmove(value->variables.data + trie->variables.n, value->variables.data, sizeof(*value->variables.data) * value->variables.n);
				for (unsigned i = 0, n = trie->variables.n; i < n; i++)
					value->variables.data[i] = inc_zebu_variable(trie->variables.data[i]);
				value->variables.n += trie->variables.n;
			}
			free_zebu_environment(trie);
		}
		{
		struct zebu_variable* subgrammar = data.data[--yacc.n, --data.n];
		if (value->variables.n == value->variables.cap)
		{
			value->variables.cap = value->variables.cap << 1 ?: 1;
			value->variables.data = realloc(value->variables.data, sizeof(*value->variables.data) * value->variables.cap);
		}
		memmove(value->variables.data + 1, value->variables.data, sizeof(*value->variables.data) * value->variables.n);
		value->variables.data[0] = inc_zebu_variable(subgrammar), value->variables.n++;
		free_zebu_variable(subgrammar);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 17;
		break;
	}
	case 32:
	{
		struct zebu_environment* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 17;
		break;
	}
	case 38:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_file* subgrammar = data.data[--yacc.n, --data.n];
		if (value->files.n == value->files.cap)
		{
			value->files.cap = value->files.cap << 1 ?: 1;
			value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
		}
		memmove(value->files.data + 1, value->files.data, sizeof(*value->files.data) * value->files.n);
		value->files.data[0] = inc_zebu_file(subgrammar), value->files.n++;
		free_zebu_file(subgrammar);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 15;
		break;
	}
	case 36:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 15;
		break;
	}
	case 39:
	{
		struct zebu_text* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_text* trie = data.data[--yacc.n, --data.n];
			if (trie->lines.n)			{
				while (value->lines.n + trie->lines.n > value->lines.cap)
				{
					value->lines.cap = value->lines.cap << 1 ?: 1;
					value->lines.data = realloc(value->lines.data, sizeof(*value->lines.data) * value->lines.cap);
				}
				memmove(value->lines.data + trie->lines.n, value->lines.data, sizeof(*value->lines.data) * value->lines.n);
				for (unsigned i = 0, n = trie->lines.n; i < n; i++)
					value->lines.data[i] = inc_zebu_token(trie->lines.data[i]);
				value->lines.n += trie->lines.n;
			}
			free_zebu_text(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		if (value->lines.n == value->lines.cap)
		{
			value->lines.cap = value->lines.cap << 1 ?: 1;
			value->lines.data = realloc(value->lines.data, sizeof(*value->lines.data) * value->lines.cap);
		}
		memmove(value->lines.data + 1, value->lines.data, sizeof(*value->lines.data) * value->lines.n);
		value->lines.data[0] = inc_zebu_token(token), value->lines.n++;
		free_zebu_token(token);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 16;
		break;
	}
	case 23:
	{
		struct zebu_text* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 16;
		break;
	}
	case 28:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 13;
		break;
	}
	case 30:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 13;
		break;
	}
	case 31:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 13;
		break;
	}
	case 29:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 13;
		break;
	}
	case 25:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 13;
		break;
	}
	case 14:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 13;
		break;
	}
	case 27:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 13;
		break;
	}
	case 26:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 13;
		break;
	}
	case 8:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 2;
		break;
	}
	case 7:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 2;
		break;
	}
	case 6:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 2;
		break;
	}
	case 2:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 2;
		break;
	}
	case 11:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 2;
		break;
	}
	case 9:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 2;
		break;
	}
	case 10:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 2;
		break;
	}
	case 21:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 4;
		break;
	}
	case 22:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_file* subgrammar = data.data[--yacc.n, --data.n];
		if (value->files.n == value->files.cap)
		{
			value->files.cap = value->files.cap << 1 ?: 1;
			value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
		}
		memmove(value->files.data + 1, value->files.data, sizeof(*value->files.data) * value->files.n);
		value->files.data[0] = inc_zebu_file(subgrammar), value->files.n++;
		free_zebu_file(subgrammar);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 4;
		break;
	}
	case 15:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_text* subgrammar = data.data[--yacc.n, --data.n];
		free_zebu_text(value->commands), value->commands = inc_zebu_text(subgrammar);
		free_zebu_text(subgrammar);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 5;
		break;
	}
	case 17:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_environment* subgrammar = data.data[--yacc.n, --data.n];
		free_zebu_environment(value->environment), value->environment = inc_zebu_environment(subgrammar);
		free_zebu_environment(subgrammar);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 6;
		break;
	}
	case 18:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_text* subgrammar = data.data[--yacc.n, --data.n];
		free_zebu_text(value->input), value->input = inc_zebu_text(subgrammar);
		free_zebu_text(subgrammar);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 7;
		break;
	}
	case 19:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_text* subgrammar = data.data[--yacc.n, --data.n];
		free_zebu_text(value->output), value->output = inc_zebu_text(subgrammar);
		free_zebu_text(subgrammar);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 8;
		break;
	}
	case 20:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		{
			char *m;
			errno = 0;
			unsigned long raw = strtoul((char*) token->data, &m, 10);
			if (*m) {
				fprintf(stderr, "%s: strtoul('%s'): invalid character '%c'!\n", program_invocation_name, token->data, *m);
				exit(1);
			} else if (errno) {
				fprintf(stderr, "%s: strtoul('%s'): %m!\n", program_invocation_name, token->data);
				exit(1);
			} else if (raw >= UINT_MAX) {
				fprintf(stderr, "%s: %%o scanf-token given a value too high for an unsigned int!\n", program_invocation_name);
				exit(1);
			}
			value->code = raw;
		}
		free_zebu_token(token);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 9;
		break;
	}
	case 33:
	{
		struct zebu_environment* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_environment* trie = data.data[--yacc.n, --data.n];
			if (trie->variables.n)
			{
				while (value->variables.n + trie->variables.n > value->variables.cap)
				{
					value->variables.cap = value->variables.cap << 1 ?: 1;
					value->variables.data = realloc(value->variables.data, sizeof(*value->variables.data) * value->variables.cap);
				}
				memmove(value->variables.data + trie->variables.n, value->variables.data, sizeof(*value->variables.data) * value->variables.n);
				for (unsigned i = 0, n = trie->variables.n; i < n; i++)
					value->variables.data[i] = inc_zebu_variable(trie->variables.data[i]);
				value->variables.n += trie->variables.n;
			}
			free_zebu_environment(trie);
		}
		{
		struct zebu_variable* subgrammar = data.data[--yacc.n, --data.n];
		if (value->variables.n == value->variables.cap)
		{
			value->variables.cap = value->variables.cap << 1 ?: 1;
			value->variables.data = realloc(value->variables.data, sizeof(*value->variables.data) * value->variables.cap);
		}
		memmove(value->variables.data + 1, value->variables.data, sizeof(*value->variables.data) * value->variables.n);
		value->variables.data[0] = inc_zebu_variable(subgrammar), value->variables.n++;
		free_zebu_variable(subgrammar);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 11;
		break;
	}
	case 16:
	{
		struct zebu_environment* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 11;
		break;
	}
	case 35:
	{
		struct zebu_file* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_text* subgrammar = data.data[--yacc.n, --data.n];
		free_zebu_text(value->content), value->content = inc_zebu_text(subgrammar);
		free_zebu_text(subgrammar);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(value->path), value->path = inc_zebu_token(token);
		free_zebu_token(token);
		}
		d = value, g = 12;
		break;
	}
	case 34:
	{
		struct zebu_file* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(value->srcpath), value->srcpath = inc_zebu_token(token);
		free_zebu_token(token);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(value->path), value->path = inc_zebu_token(token);
		free_zebu_token(token);
		}
		d = value, g = 12;
		break;
	}
	case 3:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_test* trie = data.data[--yacc.n, --data.n];
			if (trie->code) { value->code = trie->code; }
			if (trie->commands) { free_zebu_text(value->commands); value->commands = inc_zebu_text(trie->commands); }
			if (trie->environment) { free_zebu_environment(value->environment); value->environment = inc_zebu_environment(trie->environment); }
			if (trie->files.n)
			{
				while (value->files.n + trie->files.n > value->files.cap)
				{
					value->files.cap = value->files.cap << 1 ?: 1;
					value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
				}
				memmove(value->files.data + trie->files.n, value->files.data, sizeof(*value->files.data) * value->files.n);
				for (unsigned i = 0, n = trie->files.n; i < n; i++)
					value->files.data[i] = inc_zebu_file(trie->files.data[i]);
				value->files.n += trie->files.n;
			}
			if (trie->input) { free_zebu_text(value->input); value->input = inc_zebu_text(trie->input); }
			if (trie->output) { free_zebu_text(value->output); value->output = inc_zebu_text(trie->output); }
			free_zebu_test(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 1;
		break;
	}
	case 24:
	{
		struct zebu_text* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
			struct zebu_text* trie = data.data[--yacc.n, --data.n];
			if (trie->lines.n)			{
				while (value->lines.n + trie->lines.n > value->lines.cap)
				{
					value->lines.cap = value->lines.cap << 1 ?: 1;
					value->lines.data = realloc(value->lines.data, sizeof(*value->lines.data) * value->lines.cap);
				}
				memmove(value->lines.data + trie->lines.n, value->lines.data, sizeof(*value->lines.data) * value->lines.n);
				for (unsigned i = 0, n = trie->lines.n; i < n; i++)
					value->lines.data[i] = inc_zebu_token(trie->lines.data[i]);
				value->lines.n += trie->lines.n;
			}
			free_zebu_text(trie);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		if (value->lines.n == value->lines.cap)
		{
			value->lines.cap = value->lines.cap << 1 ?: 1;
			value->lines.data = realloc(value->lines.data, sizeof(*value->lines.data) * value->lines.cap);
		}
		memmove(value->lines.data + 1, value->lines.data, sizeof(*value->lines.data) * value->lines.n);
		value->lines.data[0] = inc_zebu_token(token), value->lines.n++;
		free_zebu_token(token);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 10;
		break;
	}
	case 13:
	{
		struct zebu_text* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 10;
		break;
	}
	case 37:
	{
		struct zebu_variable* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(value->value), value->value = inc_zebu_token(token);
		free_zebu_token(token);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(value->name), value->name = inc_zebu_token(token);
		free_zebu_token(token);
		}
		d = value, g = 14;
		break;
	}
}
			
			if (g == 18)
			{
				free_zebu_token(td);
				yacc.n = 0, root = d;
			}
			else
			{
				y = yacc.data[yacc.n - 1];
				
				#ifdef ZEBU_DEBUG
				ddprintf("y = %u\n", y);
				#endif
				
				assert(y < N(zebu_gotos) && g < N(*zebu_gotos));
				
				s = zebu_gotos[y][g];
				
				#ifdef ZEBU_DEBUG
				ddprintf("s = %u\n", s);
				#endif
				
				y = s, push_state(y), push_data(d);
			}
		}
		else
		{
			assert(!"266");
		}
	}
	
	#ifdef ZEBU_DEBUG
	print_zebu_$start(NULL, p_root, "start", root);
	#endif
	
	free(yacc.data);
	free(data.data);
	free(lexer.data);
	
	return root;
}

















