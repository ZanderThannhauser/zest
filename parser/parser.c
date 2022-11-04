#include "parser.h"

#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

const unsigned zebu_shifts[101][17] = {
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
	[31][3] = 44,
	[31][16] = 43,
	[32][6] = 47,
	[32][7] = 48,
	[32][8] = 49,
	[32][9] = 50,
	[32][10] = 51,
	[32][11] = 52,
	[32][12] = 53,
	[32][13] = 46,
	[33][6] = 56,
	[33][16] = 55,
	[34][6] = 47,
	[34][7] = 48,
	[34][8] = 49,
	[34][9] = 50,
	[34][10] = 51,
	[34][11] = 52,
	[34][12] = 53,
	[34][13] = 46,
	[35][6] = 47,
	[35][7] = 48,
	[35][8] = 49,
	[35][9] = 50,
	[35][10] = 51,
	[35][11] = 52,
	[35][12] = 53,
	[35][13] = 46,
	[36][6] = 47,
	[36][7] = 48,
	[36][8] = 49,
	[36][9] = 50,
	[36][10] = 51,
	[36][11] = 52,
	[36][12] = 53,
	[36][13] = 46,
	[37][6] = 47,
	[37][7] = 48,
	[37][8] = 49,
	[37][9] = 50,
	[37][10] = 51,
	[37][11] = 52,
	[37][12] = 53,
	[37][13] = 46,
	[39][14] = 63,
	[40][6] = 47,
	[40][7] = 48,
	[40][8] = 49,
	[40][9] = 50,
	[40][10] = 51,
	[40][11] = 52,
	[40][12] = 53,
	[40][13] = 46,
	[42][6] = 66,
	[42][13] = 65,
	[42][16] = 39,
	[43][3] = 71,
	[43][13] = 70,
	[43][16] = 69,
	[46][6] = 6,
	[46][7] = 7,
	[46][8] = 8,
	[46][9] = 9,
	[46][10] = 10,
	[46][11] = 11,
	[46][12] = 12,
	[48][14] = 17,
	[49][14] = 19,
	[50][14] = 21,
	[51][14] = 23,
	[52][14] = 25,
	[53][14] = 27,
	[55][14] = 80,
	[58][6] = 82,
	[58][13] = 81,
	[58][16] = 55,
	[63][2] = 85,
	[63][16] = 86,
	[65][6] = 40,
	[65][16] = 39,
	[66][6] = 47,
	[66][7] = 48,
	[66][8] = 49,
	[66][9] = 50,
	[66][10] = 51,
	[66][11] = 52,
	[66][12] = 53,
	[66][13] = 46,
	[68][6] = 66,
	[68][13] = 65,
	[68][16] = 39,
	[69][3] = 71,
	[69][13] = 70,
	[69][16] = 69,
	[70][3] = 44,
	[70][16] = 43,
	[80][16] = 93,
	[81][6] = 56,
	[81][16] = 55,
	[84][6] = 82,
	[84][13] = 81,
	[84][16] = 55,
	[85][3] = 97,
	[85][16] = 96,
	[96][3] = 101,
	[96][13] = 100,
	[96][16] = 99,
	[99][3] = 101,
	[99][13] = 100,
	[99][16] = 99,
	[100][3] = 97,
	[100][16] = 96,
};


const unsigned zebu_reduces[105][17] = {
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
	[41][3] = 13,
	[41][13] = 13,
	[44][6] = 14,
	[44][7] = 14,
	[44][8] = 14,
	[44][9] = 14,
	[44][10] = 14,
	[44][11] = 14,
	[44][12] = 14,
	[44][13] = 14,
	[45][6] = 15,
	[45][7] = 15,
	[45][8] = 15,
	[45][9] = 15,
	[45][10] = 15,
	[45][11] = 15,
	[45][12] = 15,
	[45][13] = 15,
	[47][3] = 16,
	[47][13] = 16,
	[54][3] = 17,
	[54][13] = 17,
	[56][6] = 18,
	[56][7] = 18,
	[56][8] = 18,
	[56][9] = 18,
	[56][10] = 18,
	[56][11] = 18,
	[56][12] = 18,
	[56][13] = 18,
	[57][6] = 19,
	[57][7] = 19,
	[57][8] = 19,
	[57][9] = 19,
	[57][10] = 19,
	[57][11] = 19,
	[57][12] = 19,
	[57][13] = 19,
	[59][3] = 20,
	[59][13] = 20,
	[60][3] = 21,
	[60][13] = 21,
	[61][3] = 22,
	[61][13] = 22,
	[62][3] = 23,
	[62][13] = 23,
	[64][3] = 24,
	[64][13] = 24,
	[67][3] = 25,
	[67][13] = 25,
	[71][6] = 26,
	[71][7] = 26,
	[71][8] = 26,
	[71][9] = 26,
	[71][10] = 26,
	[71][11] = 26,
	[71][12] = 26,
	[71][13] = 26,
	[72][6] = 27,
	[72][7] = 27,
	[72][8] = 27,
	[72][9] = 27,
	[72][10] = 27,
	[72][11] = 27,
	[72][12] = 27,
	[72][13] = 27,
	[73][3] = 28,
	[73][13] = 28,
	[74][3] = 29,
	[74][13] = 29,
	[75][3] = 30,
	[75][13] = 30,
	[76][3] = 31,
	[76][13] = 31,
	[77][3] = 32,
	[77][13] = 32,
	[78][3] = 33,
	[78][13] = 33,
	[79][3] = 34,
	[79][13] = 34,
	[82][6] = 35,
	[82][7] = 35,
	[82][8] = 35,
	[82][9] = 35,
	[82][10] = 35,
	[82][11] = 35,
	[82][12] = 35,
	[82][13] = 35,
	[83][6] = 36,
	[83][7] = 36,
	[83][8] = 36,
	[83][9] = 36,
	[83][10] = 36,
	[83][11] = 36,
	[83][12] = 36,
	[83][13] = 36,
	[86][6] = 37,
	[86][13] = 37,
	[86][16] = 37,
	[87][6] = 38,
	[87][13] = 38,
	[87][16] = 38,
	[88][3] = 39,
	[88][13] = 39,
	[89][3] = 40,
	[89][13] = 40,
	[90][3] = 41,
	[90][13] = 41,
	[91][6] = 42,
	[91][7] = 42,
	[91][8] = 42,
	[91][9] = 42,
	[91][10] = 42,
	[91][11] = 42,
	[91][12] = 42,
	[91][13] = 42,
	[92][6] = 43,
	[92][7] = 43,
	[92][8] = 43,
	[92][9] = 43,
	[92][10] = 43,
	[92][11] = 43,
	[92][12] = 43,
	[92][13] = 43,
	[93][6] = 44,
	[93][13] = 44,
	[93][16] = 44,
	[94][6] = 45,
	[94][7] = 45,
	[94][8] = 45,
	[94][9] = 45,
	[94][10] = 45,
	[94][11] = 45,
	[94][12] = 45,
	[94][13] = 45,
	[95][6] = 46,
	[95][7] = 46,
	[95][8] = 46,
	[95][9] = 46,
	[95][10] = 46,
	[95][11] = 46,
	[95][12] = 46,
	[95][13] = 46,
	[97][6] = 14,
	[97][13] = 14,
	[97][16] = 14,
	[98][6] = 15,
	[98][13] = 15,
	[98][16] = 15,
	[101][6] = 26,
	[101][13] = 26,
	[101][16] = 26,
	[102][6] = 27,
	[102][13] = 27,
	[102][16] = 27,
	[103][6] = 42,
	[103][13] = 42,
	[103][16] = 42,
	[104][6] = 43,
	[104][13] = 43,
	[104][16] = 43,
};


const unsigned zebu_gotos[101][21] = {
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
	[30][13] = 42,
	[31][14] = 45,
	[32][15] = 54,
	[33][16] = 57,
	[33][17] = 58,
	[34][15] = 59,
	[35][15] = 60,
	[36][15] = 61,
	[37][15] = 62,
	[40][15] = 64,
	[42][13] = 68,
	[42][18] = 67,
	[43][19] = 72,
	[46][2] = 73,
	[48][4] = 74,
	[49][5] = 75,
	[50][6] = 76,
	[51][7] = 77,
	[52][8] = 78,
	[53][9] = 79,
	[58][17] = 84,
	[58][20] = 83,
	[63][10] = 87,
	[65][12] = 88,
	[65][13] = 42,
	[66][15] = 89,
	[68][13] = 68,
	[68][18] = 90,
	[69][19] = 91,
	[70][14] = 92,
	[81][16] = 94,
	[81][17] = 58,
	[84][17] = 84,
	[84][20] = 95,
	[85][14] = 98,
	[96][19] = 102,
	[99][19] = 103,
	[100][14] = 104,
};


const unsigned zebu_lexer[77][127] = {
	[1][9] = 16,
	[1][10] = 16,
	[1][32] = 16,
	[1][91] = 17,
	[2][9] = 16,
	[2][10] = 16,
	[2][32] = 16,
	[2][93] = 18,
	[2][123] = 19,
	[3][9] = 16,
	[3][10] = 16,
	[3][32] = 16,
	[4][9] = 16,
	[4][10] = 16,
	[4][32] = 16,
	[4][34] = 21,
	[4][125] = 22,
	[5][9] = 16,
	[5][10] = 16,
	[5][32] = 16,
	[5][44] = 23,
	[5][93] = 18,
	[6][9] = 16,
	[6][10] = 16,
	[6][32] = 16,
	[6][58] = 24,
	[7][9] = 16,
	[7][10] = 16,
	[7][32] = 16,
	[7][123] = 19,
	[8][9] = 16,
	[8][10] = 16,
	[8][32] = 16,
	[8][48] = 25,
	[8][49] = 25,
	[8][50] = 25,
	[8][51] = 25,
	[8][52] = 25,
	[8][53] = 25,
	[8][54] = 25,
	[8][55] = 25,
	[8][56] = 25,
	[8][57] = 25,
	[9][9] = 16,
	[9][10] = 16,
	[9][32] = 16,
	[9][34] = 26,
	[9][125] = 22,
	[10][9] = 16,
	[10][10] = 16,
	[10][32] = 16,
	[10][34] = 26,
	[10][93] = 18,
	[11][9] = 16,
	[11][10] = 16,
	[11][32] = 16,
	[11][34] = 21,
	[11][44] = 23,
	[11][125] = 22,
	[12][9] = 16,
	[12][10] = 16,
	[12][32] = 16,
	[12][34] = 26,
	[12][44] = 23,
	[12][125] = 22,
	[13][9] = 16,
	[13][10] = 16,
	[13][32] = 16,
	[13][34] = 26,
	[13][44] = 23,
	[13][93] = 18,
	[14][9] = 16,
	[14][10] = 16,
	[14][32] = 16,
	[14][34] = 26,
	[14][91] = 17,
	[15][9] = 16,
	[15][10] = 16,
	[15][32] = 16,
	[15][34] = 26,
	[16][9] = 16,
	[16][10] = 16,
	[16][32] = 16,
	[21][99] = 27,
	[21][101] = 28,
	[21][102] = 29,
	[21][105] = 30,
	[21][111] = 31,
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
	[26][32] = 26,
	[26][33] = 26,
	[26][34] = 32,
	[26][35] = 26,
	[26][36] = 26,
	[26][37] = 26,
	[26][39] = 26,
	[26][40] = 26,
	[26][41] = 26,
	[26][42] = 26,
	[26][43] = 26,
	[26][44] = 26,
	[26][45] = 26,
	[26][46] = 26,
	[26][47] = 26,
	[26][48] = 26,
	[26][49] = 26,
	[26][50] = 26,
	[26][51] = 26,
	[26][52] = 26,
	[26][53] = 26,
	[26][54] = 26,
	[26][55] = 26,
	[26][56] = 26,
	[26][57] = 26,
	[26][58] = 26,
	[26][59] = 26,
	[26][60] = 26,
	[26][61] = 26,
	[26][62] = 26,
	[26][65] = 26,
	[26][66] = 26,
	[26][67] = 26,
	[26][68] = 26,
	[26][69] = 26,
	[26][70] = 26,
	[26][71] = 26,
	[26][72] = 26,
	[26][73] = 26,
	[26][74] = 26,
	[26][75] = 26,
	[26][76] = 26,
	[26][77] = 26,
	[26][78] = 26,
	[26][79] = 26,
	[26][80] = 26,
	[26][81] = 26,
	[26][82] = 26,
	[26][83] = 26,
	[26][84] = 26,
	[26][85] = 26,
	[26][86] = 26,
	[26][87] = 26,
	[26][88] = 26,
	[26][89] = 26,
	[26][90] = 26,
	[26][91] = 26,
	[26][92] = 33,
	[26][93] = 26,
	[26][95] = 26,
	[26][96] = 26,
	[26][97] = 26,
	[26][98] = 26,
	[26][99] = 26,
	[26][100] = 26,
	[26][101] = 26,
	[26][102] = 26,
	[26][103] = 26,
	[26][104] = 26,
	[26][105] = 26,
	[26][106] = 26,
	[26][107] = 26,
	[26][108] = 26,
	[26][109] = 26,
	[26][110] = 26,
	[26][111] = 26,
	[26][112] = 26,
	[26][113] = 26,
	[26][114] = 26,
	[26][115] = 26,
	[26][116] = 26,
	[26][117] = 26,
	[26][118] = 26,
	[26][119] = 26,
	[26][120] = 26,
	[26][121] = 26,
	[26][122] = 26,
	[26][123] = 26,
	[26][124] = 26,
	[26][125] = 26,
	[26][126] = 26,
	[27][111] = 34,
	[28][110] = 35,
	[28][120] = 36,
	[29][105] = 37,
	[30][110] = 38,
	[31][117] = 39,
	[33][34] = 26,
	[33][92] = 26,
	[33][110] = 26,
	[33][116] = 26,
	[34][109] = 40,
	[35][118] = 41,
	[36][105] = 42,
	[37][108] = 43,
	[38][112] = 44,
	[39][116] = 45,
	[40][109] = 46,
	[41][105] = 47,
	[42][116] = 48,
	[43][101] = 49,
	[44][117] = 50,
	[45][112] = 51,
	[46][97] = 52,
	[47][114] = 53,
	[48][45] = 54,
	[49][115] = 55,
	[50][116] = 56,
	[51][117] = 57,
	[52][110] = 58,
	[53][111] = 59,
	[54][99] = 60,
	[55][34] = 61,
	[56][34] = 62,
	[57][116] = 63,
	[58][100] = 64,
	[59][110] = 65,
	[60][111] = 66,
	[63][34] = 67,
	[64][115] = 68,
	[65][109] = 69,
	[66][100] = 70,
	[68][34] = 71,
	[69][101] = 72,
	[70][101] = 73,
	[72][110] = 74,
	[73][34] = 75,
	[74][116] = 76,
	[76][34] = 77,
};


const unsigned zebu_lexer_starts[105] = {
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
	[41] = 5,
	[42] = 12,
	[43] = 13,
	[44] = 11,
	[45] = 11,
	[46] = 4,
	[47] = 5,
	[48] = 6,
	[49] = 6,
	[50] = 6,
	[51] = 6,
	[52] = 6,
	[53] = 6,
	[54] = 5,
	[55] = 6,
	[56] = 11,
	[57] = 11,
	[58] = 12,
	[59] = 5,
	[60] = 5,
	[61] = 5,
	[62] = 5,
	[63] = 14,
	[64] = 5,
	[65] = 9,
	[66] = 11,
	[67] = 5,
	[68] = 12,
	[69] = 13,
	[70] = 10,
	[71] = 11,
	[72] = 11,
	[73] = 5,
	[74] = 5,
	[75] = 5,
	[76] = 5,
	[77] = 5,
	[78] = 5,
	[79] = 5,
	[80] = 15,
	[81] = 9,
	[82] = 11,
	[83] = 11,
	[84] = 12,
	[85] = 10,
	[86] = 12,
	[87] = 12,
	[88] = 5,
	[89] = 5,
	[90] = 5,
	[91] = 11,
	[92] = 11,
	[93] = 12,
	[94] = 11,
	[95] = 11,
	[96] = 13,
	[97] = 12,
	[98] = 12,
	[99] = 13,
	[100] = 10,
	[101] = 12,
	[102] = 12,
	[103] = 12,
	[104] = 12,
};


const unsigned zebu_lexer_accepts[78] = {
	[16] = 1,
	[17] = 2,
	[18] = 3,
	[19] = 4,
	[20] = 5,
	[22] = 6,
	[23] = 13,
	[24] = 14,
	[25] = 15,
	[32] = 16,
	[61] = 7,
	[62] = 10,
	[67] = 11,
	[71] = 8,
	[75] = 12,
	[77] = 9,
};


const unsigned zebu_lexer_EOFs[4] = {
	[3] = 20,
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
				
				exit(1);
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
		d = value, g = 21;
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
		d = value, g = 21;
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
	case 36:
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
		d = value, g = 16;
		break;
	}
	case 18:
	{
		struct zebu_environment* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 16;
		break;
	}
	case 23:
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
	case 24:
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
		d = value, g = 12;
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
		d = value, g = 12;
		break;
	}
	case 39:
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
		d = value, g = 18;
		break;
	}
	case 40:
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
		d = value, g = 18;
		break;
	}
	case 41:
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
		d = value, g = 18;
		break;
	}
	case 42:
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
		d = value, g = 19;
		break;
	}
	case 43:
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
		free_zebu_token(token);
		}
		d = value, g = 19;
		break;
	}
	case 26:
	{
		struct zebu_text* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 19;
		break;
	}
	case 27:
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
		d = value, g = 14;
		break;
	}
	case 14:
	{
		struct zebu_text* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 14;
		break;
	}
	case 35:
	{
		struct zebu_environment* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 20;
		break;
	}
	case 45:
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
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 20;
		break;
	}
	case 46:
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
		d = value, g = 20;
		break;
	}
	case 33:
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
		d = value, g = 15;
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
		d = value, g = 15;
		break;
	}
	case 32:
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
	case 34:
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
		d = value, g = 15;
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
		d = value, g = 15;
		break;
	}
	case 16:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 15;
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
	case 13:
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
		d = value, g = 4;
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
	case 19:
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
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 11;
		break;
	}
	case 37:
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
		d = value, g = 13;
		break;
	}
	case 38:
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
		d = value, g = 13;
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
	case 15:
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
		free_zebu_token(token);
		}
		d = value, g = 10;
		break;
	}
	case 44:
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
		d = value, g = 17;
		break;
	}
}
			
			if (g == 21)
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
			struct zebu_token* token = td;
			
			fprintf(stderr, "zebu: unexpected token '%.*s'!\n", token->len, token->data);
			
			exit(1);
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

















