#include "zebu.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

const unsigned zebu_shifts[123][17] = {
	[1][2] = 2,
	[2][3] = 3,
	[2][4] = 4,
	[4][6] = 7,
	[4][7] = 8,
	[4][8] = 9,
	[4][9] = 10,
	[4][10] = 11,
	[4][11] = 12,
	[4][12] = 13,
	[6][3] = 16,
	[6][4] = 4,
	[6][13] = 15,
	[8][14] = 19,
	[9][14] = 21,
	[10][14] = 23,
	[11][14] = 25,
	[12][14] = 27,
	[13][14] = 29,
	[15][3] = 3,
	[15][4] = 4,
	[18][3] = 16,
	[18][4] = 4,
	[18][13] = 15,
	[19][4] = 33,
	[21][2] = 34,
	[23][4] = 36,
	[25][2] = 38,
	[27][2] = 38,
	[29][15] = 41,
	[33][6] = 43,
	[33][16] = 42,
	[34][2] = 46,
	[34][3] = 47,
	[35][6] = 51,
	[35][7] = 52,
	[35][8] = 53,
	[35][9] = 54,
	[35][10] = 55,
	[35][11] = 56,
	[35][12] = 57,
	[35][13] = 50,
	[36][6] = 60,
	[36][16] = 59,
	[37][6] = 51,
	[37][7] = 52,
	[37][8] = 53,
	[37][9] = 54,
	[37][10] = 55,
	[37][11] = 56,
	[37][12] = 57,
	[37][13] = 50,
	[38][3] = 65,
	[38][16] = 64,
	[39][6] = 51,
	[39][7] = 52,
	[39][8] = 53,
	[39][9] = 54,
	[39][10] = 55,
	[39][11] = 56,
	[39][12] = 57,
	[39][13] = 50,
	[40][6] = 51,
	[40][7] = 52,
	[40][8] = 53,
	[40][9] = 54,
	[40][10] = 55,
	[40][11] = 56,
	[40][12] = 57,
	[40][13] = 50,
	[41][6] = 51,
	[41][7] = 52,
	[41][8] = 53,
	[41][9] = 54,
	[41][10] = 55,
	[41][11] = 56,
	[41][12] = 57,
	[41][13] = 50,
	[42][14] = 70,
	[43][6] = 51,
	[43][7] = 52,
	[43][8] = 53,
	[43][9] = 54,
	[43][10] = 55,
	[43][11] = 56,
	[43][12] = 57,
	[43][13] = 50,
	[45][6] = 73,
	[45][13] = 72,
	[45][16] = 42,
	[46][3] = 77,
	[46][16] = 76,
	[49][2] = 46,
	[49][3] = 80,
	[49][13] = 79,
	[50][6] = 7,
	[50][7] = 8,
	[50][8] = 9,
	[50][9] = 10,
	[50][10] = 11,
	[50][11] = 12,
	[50][12] = 13,
	[52][14] = 19,
	[53][14] = 21,
	[54][14] = 23,
	[55][14] = 25,
	[56][14] = 27,
	[57][14] = 29,
	[59][14] = 90,
	[62][6] = 92,
	[62][13] = 91,
	[62][16] = 59,
	[64][3] = 97,
	[64][13] = 96,
	[64][16] = 95,
	[70][2] = 99,
	[70][16] = 100,
	[72][6] = 43,
	[72][16] = 42,
	[73][6] = 51,
	[73][7] = 52,
	[73][8] = 53,
	[73][9] = 54,
	[73][10] = 55,
	[73][11] = 56,
	[73][12] = 57,
	[73][13] = 50,
	[75][6] = 73,
	[75][13] = 72,
	[75][16] = 42,
	[76][3] = 107,
	[76][13] = 106,
	[76][16] = 105,
	[79][2] = 46,
	[79][3] = 47,
	[82][2] = 46,
	[82][3] = 80,
	[82][13] = 79,
	[90][16] = 111,
	[91][6] = 60,
	[91][16] = 59,
	[94][6] = 92,
	[94][13] = 91,
	[94][16] = 59,
	[95][3] = 97,
	[95][13] = 96,
	[95][16] = 95,
	[96][3] = 65,
	[96][16] = 64,
	[99][3] = 117,
	[99][16] = 116,
	[105][3] = 107,
	[105][13] = 106,
	[105][16] = 105,
	[106][3] = 77,
	[106][16] = 76,
	[116][3] = 123,
	[116][13] = 122,
	[116][16] = 121,
	[121][3] = 123,
	[121][13] = 122,
	[121][16] = 121,
	[122][3] = 117,
	[122][16] = 116,
};


const unsigned zebu_reduces[127][17] = {
	[3][5] = 1,
	[5][5] = 2,
	[7][3] = 3,
	[7][4] = 3,
	[7][13] = 3,
	[14][3] = 4,
	[14][4] = 4,
	[14][13] = 4,
	[16][5] = 5,
	[17][5] = 6,
	[20][3] = 7,
	[20][4] = 7,
	[20][13] = 7,
	[22][3] = 8,
	[22][4] = 8,
	[22][13] = 8,
	[24][3] = 9,
	[24][4] = 9,
	[24][13] = 9,
	[26][3] = 10,
	[26][4] = 10,
	[26][13] = 10,
	[28][3] = 11,
	[28][4] = 11,
	[28][13] = 11,
	[30][3] = 12,
	[30][4] = 12,
	[30][13] = 12,
	[31][5] = 13,
	[32][5] = 14,
	[44][3] = 15,
	[44][4] = 15,
	[44][13] = 15,
	[47][6] = 16,
	[47][7] = 16,
	[47][8] = 16,
	[47][9] = 16,
	[47][10] = 16,
	[47][11] = 16,
	[47][12] = 16,
	[47][13] = 16,
	[48][6] = 17,
	[48][7] = 17,
	[48][8] = 17,
	[48][9] = 17,
	[48][10] = 17,
	[48][11] = 17,
	[48][12] = 17,
	[48][13] = 17,
	[51][3] = 18,
	[51][4] = 18,
	[51][13] = 18,
	[58][3] = 19,
	[58][4] = 19,
	[58][13] = 19,
	[60][6] = 20,
	[60][7] = 20,
	[60][8] = 20,
	[60][9] = 20,
	[60][10] = 20,
	[60][11] = 20,
	[60][12] = 20,
	[60][13] = 20,
	[61][6] = 21,
	[61][7] = 21,
	[61][8] = 21,
	[61][9] = 21,
	[61][10] = 21,
	[61][11] = 21,
	[61][12] = 21,
	[61][13] = 21,
	[63][3] = 22,
	[63][4] = 22,
	[63][13] = 22,
	[65][6] = 23,
	[65][7] = 23,
	[65][8] = 23,
	[65][9] = 23,
	[65][10] = 23,
	[65][11] = 23,
	[65][12] = 23,
	[65][13] = 23,
	[66][6] = 24,
	[66][7] = 24,
	[66][8] = 24,
	[66][9] = 24,
	[66][10] = 24,
	[66][11] = 24,
	[66][12] = 24,
	[66][13] = 24,
	[67][3] = 25,
	[67][4] = 25,
	[67][13] = 25,
	[68][3] = 26,
	[68][4] = 26,
	[68][13] = 26,
	[69][3] = 27,
	[69][4] = 27,
	[69][13] = 27,
	[71][3] = 28,
	[71][4] = 28,
	[71][13] = 28,
	[74][3] = 29,
	[74][4] = 29,
	[74][13] = 29,
	[77][2] = 30,
	[77][3] = 30,
	[77][13] = 30,
	[78][2] = 31,
	[78][3] = 31,
	[78][13] = 31,
	[80][6] = 32,
	[80][7] = 32,
	[80][8] = 32,
	[80][9] = 32,
	[80][10] = 32,
	[80][11] = 32,
	[80][12] = 32,
	[80][13] = 32,
	[81][6] = 33,
	[81][7] = 33,
	[81][8] = 33,
	[81][9] = 33,
	[81][10] = 33,
	[81][11] = 33,
	[81][12] = 33,
	[81][13] = 33,
	[83][3] = 34,
	[83][4] = 34,
	[83][13] = 34,
	[84][3] = 35,
	[84][4] = 35,
	[84][13] = 35,
	[85][3] = 36,
	[85][4] = 36,
	[85][13] = 36,
	[86][3] = 37,
	[86][4] = 37,
	[86][13] = 37,
	[87][3] = 38,
	[87][4] = 38,
	[87][13] = 38,
	[88][3] = 39,
	[88][4] = 39,
	[88][13] = 39,
	[89][3] = 40,
	[89][4] = 40,
	[89][13] = 40,
	[92][6] = 41,
	[92][7] = 41,
	[92][8] = 41,
	[92][9] = 41,
	[92][10] = 41,
	[92][11] = 41,
	[92][12] = 41,
	[92][13] = 41,
	[93][6] = 42,
	[93][7] = 42,
	[93][8] = 42,
	[93][9] = 42,
	[93][10] = 42,
	[93][11] = 42,
	[93][12] = 42,
	[93][13] = 42,
	[97][6] = 43,
	[97][7] = 43,
	[97][8] = 43,
	[97][9] = 43,
	[97][10] = 43,
	[97][11] = 43,
	[97][12] = 43,
	[97][13] = 43,
	[98][6] = 44,
	[98][7] = 44,
	[98][8] = 44,
	[98][9] = 44,
	[98][10] = 44,
	[98][11] = 44,
	[98][12] = 44,
	[98][13] = 44,
	[100][6] = 45,
	[100][13] = 45,
	[100][16] = 45,
	[101][6] = 46,
	[101][13] = 46,
	[101][16] = 46,
	[102][3] = 47,
	[102][4] = 47,
	[102][13] = 47,
	[103][3] = 48,
	[103][4] = 48,
	[103][13] = 48,
	[104][3] = 49,
	[104][4] = 49,
	[104][13] = 49,
	[107][2] = 50,
	[107][3] = 50,
	[107][13] = 50,
	[108][2] = 51,
	[108][3] = 51,
	[108][13] = 51,
	[109][6] = 52,
	[109][7] = 52,
	[109][8] = 52,
	[109][9] = 52,
	[109][10] = 52,
	[109][11] = 52,
	[109][12] = 52,
	[109][13] = 52,
	[110][6] = 53,
	[110][7] = 53,
	[110][8] = 53,
	[110][9] = 53,
	[110][10] = 53,
	[110][11] = 53,
	[110][12] = 53,
	[110][13] = 53,
	[111][6] = 54,
	[111][13] = 54,
	[111][16] = 54,
	[112][6] = 55,
	[112][7] = 55,
	[112][8] = 55,
	[112][9] = 55,
	[112][10] = 55,
	[112][11] = 55,
	[112][12] = 55,
	[112][13] = 55,
	[113][6] = 56,
	[113][7] = 56,
	[113][8] = 56,
	[113][9] = 56,
	[113][10] = 56,
	[113][11] = 56,
	[113][12] = 56,
	[113][13] = 56,
	[114][6] = 57,
	[114][7] = 57,
	[114][8] = 57,
	[114][9] = 57,
	[114][10] = 57,
	[114][11] = 57,
	[114][12] = 57,
	[114][13] = 57,
	[115][6] = 58,
	[115][7] = 58,
	[115][8] = 58,
	[115][9] = 58,
	[115][10] = 58,
	[115][11] = 58,
	[115][12] = 58,
	[115][13] = 58,
	[117][6] = 23,
	[117][13] = 23,
	[117][16] = 23,
	[118][6] = 24,
	[118][13] = 24,
	[118][16] = 24,
	[119][2] = 59,
	[119][3] = 59,
	[119][13] = 59,
	[120][2] = 60,
	[120][3] = 60,
	[120][13] = 60,
	[123][6] = 43,
	[123][13] = 43,
	[123][16] = 43,
	[124][6] = 44,
	[124][13] = 44,
	[124][16] = 44,
	[125][6] = 57,
	[125][13] = 57,
	[125][16] = 57,
	[126][6] = 58,
	[126][13] = 58,
	[126][16] = 58,
};


const unsigned zebu_gotos[123][28] = {
	[2][1] = 5,
	[2][2] = 6,
	[4][3] = 14,
	[6][2] = 18,
	[6][4] = 17,
	[8][5] = 20,
	[9][6] = 22,
	[10][7] = 24,
	[11][8] = 26,
	[12][9] = 28,
	[13][10] = 30,
	[15][1] = 31,
	[15][2] = 6,
	[18][2] = 18,
	[18][4] = 32,
	[21][11] = 35,
	[23][12] = 37,
	[25][13] = 39,
	[27][13] = 40,
	[33][14] = 44,
	[33][15] = 45,
	[34][16] = 48,
	[34][17] = 49,
	[35][18] = 58,
	[36][19] = 61,
	[36][20] = 62,
	[37][18] = 63,
	[38][21] = 66,
	[39][18] = 67,
	[40][18] = 68,
	[41][18] = 69,
	[43][18] = 71,
	[45][15] = 75,
	[45][22] = 74,
	[46][23] = 78,
	[49][17] = 82,
	[49][24] = 81,
	[50][3] = 83,
	[52][5] = 84,
	[53][6] = 85,
	[54][7] = 86,
	[55][8] = 87,
	[56][9] = 88,
	[57][10] = 89,
	[62][20] = 94,
	[62][25] = 93,
	[64][26] = 98,
	[70][13] = 101,
	[72][14] = 102,
	[72][15] = 45,
	[73][18] = 103,
	[75][15] = 75,
	[75][22] = 104,
	[76][27] = 108,
	[79][16] = 109,
	[79][17] = 49,
	[82][17] = 82,
	[82][24] = 110,
	[91][19] = 112,
	[91][20] = 62,
	[94][20] = 94,
	[94][25] = 113,
	[95][26] = 114,
	[96][21] = 115,
	[99][21] = 118,
	[105][27] = 119,
	[106][23] = 120,
	[116][26] = 124,
	[121][26] = 125,
	[122][21] = 126,
};


const unsigned zebu_lexer[79][256] = {
	[1][9] = 18,
	[1][10] = 18,
	[1][32] = 18,
	[1][91] = 19,
	[2][9] = 18,
	[2][10] = 18,
	[2][32] = 18,
	[2][93] = 20,
	[2][123] = 21,
	[3][9] = 18,
	[3][10] = 18,
	[3][32] = 18,
	[4][9] = 18,
	[4][10] = 18,
	[4][32] = 18,
	[4][34] = 23,
	[4][125] = 24,
	[5][9] = 18,
	[5][10] = 18,
	[5][32] = 18,
	[5][44] = 25,
	[5][93] = 20,
	[5][123] = 21,
	[6][9] = 18,
	[6][10] = 18,
	[6][32] = 18,
	[6][58] = 26,
	[7][9] = 18,
	[7][10] = 18,
	[7][32] = 18,
	[7][123] = 21,
	[8][9] = 18,
	[8][10] = 18,
	[8][32] = 18,
	[8][48] = 27,
	[8][49] = 27,
	[8][50] = 27,
	[8][51] = 27,
	[8][52] = 27,
	[8][53] = 27,
	[8][54] = 27,
	[8][55] = 27,
	[8][56] = 27,
	[8][57] = 27,
	[9][9] = 18,
	[9][10] = 18,
	[9][32] = 18,
	[9][34] = 28,
	[9][125] = 24,
	[10][9] = 18,
	[10][10] = 18,
	[10][32] = 18,
	[10][91] = 19,
	[10][93] = 20,
	[11][9] = 18,
	[11][10] = 18,
	[11][32] = 18,
	[11][34] = 23,
	[11][44] = 25,
	[11][125] = 24,
	[12][9] = 18,
	[12][10] = 18,
	[12][32] = 18,
	[12][34] = 28,
	[12][93] = 20,
	[13][9] = 18,
	[13][10] = 18,
	[13][32] = 18,
	[13][34] = 28,
	[13][44] = 25,
	[13][125] = 24,
	[14][9] = 18,
	[14][10] = 18,
	[14][32] = 18,
	[14][44] = 25,
	[14][91] = 19,
	[14][93] = 20,
	[15][9] = 18,
	[15][10] = 18,
	[15][32] = 18,
	[15][34] = 28,
	[15][44] = 25,
	[15][93] = 20,
	[16][9] = 18,
	[16][10] = 18,
	[16][32] = 18,
	[16][34] = 28,
	[16][91] = 19,
	[17][9] = 18,
	[17][10] = 18,
	[17][32] = 18,
	[17][34] = 28,
	[18][9] = 18,
	[18][10] = 18,
	[18][32] = 18,
	[23][99] = 29,
	[23][101] = 30,
	[23][102] = 31,
	[23][105] = 32,
	[23][111] = 33,
	[27][48] = 27,
	[27][49] = 27,
	[27][50] = 27,
	[27][51] = 27,
	[27][52] = 27,
	[27][53] = 27,
	[27][54] = 27,
	[27][55] = 27,
	[27][56] = 27,
	[27][57] = 27,
	[28][0] = 28,
	[28][1] = 28,
	[28][2] = 28,
	[28][3] = 28,
	[28][4] = 28,
	[28][5] = 28,
	[28][6] = 28,
	[28][7] = 28,
	[28][8] = 28,
	[28][9] = 28,
	[28][10] = 28,
	[28][11] = 28,
	[28][12] = 28,
	[28][13] = 28,
	[28][14] = 28,
	[28][15] = 28,
	[28][16] = 28,
	[28][17] = 28,
	[28][18] = 28,
	[28][19] = 28,
	[28][20] = 28,
	[28][21] = 28,
	[28][22] = 28,
	[28][23] = 28,
	[28][24] = 28,
	[28][25] = 28,
	[28][26] = 28,
	[28][27] = 28,
	[28][28] = 28,
	[28][29] = 28,
	[28][30] = 28,
	[28][31] = 28,
	[28][32] = 28,
	[28][33] = 28,
	[28][34] = 34,
	[28][35] = 28,
	[28][36] = 28,
	[28][37] = 28,
	[28][38] = 28,
	[28][39] = 28,
	[28][40] = 28,
	[28][41] = 28,
	[28][42] = 28,
	[28][43] = 28,
	[28][44] = 28,
	[28][45] = 28,
	[28][46] = 28,
	[28][47] = 28,
	[28][48] = 28,
	[28][49] = 28,
	[28][50] = 28,
	[28][51] = 28,
	[28][52] = 28,
	[28][53] = 28,
	[28][54] = 28,
	[28][55] = 28,
	[28][56] = 28,
	[28][57] = 28,
	[28][58] = 28,
	[28][59] = 28,
	[28][60] = 28,
	[28][61] = 28,
	[28][62] = 28,
	[28][63] = 28,
	[28][64] = 28,
	[28][65] = 28,
	[28][66] = 28,
	[28][67] = 28,
	[28][68] = 28,
	[28][69] = 28,
	[28][70] = 28,
	[28][71] = 28,
	[28][72] = 28,
	[28][73] = 28,
	[28][74] = 28,
	[28][75] = 28,
	[28][76] = 28,
	[28][77] = 28,
	[28][78] = 28,
	[28][79] = 28,
	[28][80] = 28,
	[28][81] = 28,
	[28][82] = 28,
	[28][83] = 28,
	[28][84] = 28,
	[28][85] = 28,
	[28][86] = 28,
	[28][87] = 28,
	[28][88] = 28,
	[28][89] = 28,
	[28][90] = 28,
	[28][91] = 28,
	[28][92] = 35,
	[28][93] = 28,
	[28][94] = 28,
	[28][95] = 28,
	[28][96] = 28,
	[28][97] = 28,
	[28][98] = 28,
	[28][99] = 28,
	[28][100] = 28,
	[28][101] = 28,
	[28][102] = 28,
	[28][103] = 28,
	[28][104] = 28,
	[28][105] = 28,
	[28][106] = 28,
	[28][107] = 28,
	[28][108] = 28,
	[28][109] = 28,
	[28][110] = 28,
	[28][111] = 28,
	[28][112] = 28,
	[28][113] = 28,
	[28][114] = 28,
	[28][115] = 28,
	[28][116] = 28,
	[28][117] = 28,
	[28][118] = 28,
	[28][119] = 28,
	[28][120] = 28,
	[28][121] = 28,
	[28][122] = 28,
	[28][123] = 28,
	[28][124] = 28,
	[28][125] = 28,
	[28][126] = 28,
	[28][127] = 28,
	[28][128] = 28,
	[28][129] = 28,
	[28][130] = 28,
	[28][131] = 28,
	[28][132] = 28,
	[28][133] = 28,
	[28][134] = 28,
	[28][135] = 28,
	[28][136] = 28,
	[28][137] = 28,
	[28][138] = 28,
	[28][139] = 28,
	[28][140] = 28,
	[28][141] = 28,
	[28][142] = 28,
	[28][143] = 28,
	[28][144] = 28,
	[28][145] = 28,
	[28][146] = 28,
	[28][147] = 28,
	[28][148] = 28,
	[28][149] = 28,
	[28][150] = 28,
	[28][151] = 28,
	[28][152] = 28,
	[28][153] = 28,
	[28][154] = 28,
	[28][155] = 28,
	[28][156] = 28,
	[28][157] = 28,
	[28][158] = 28,
	[28][159] = 28,
	[28][160] = 28,
	[28][161] = 28,
	[28][162] = 28,
	[28][163] = 28,
	[28][164] = 28,
	[28][165] = 28,
	[28][166] = 28,
	[28][167] = 28,
	[28][168] = 28,
	[28][169] = 28,
	[28][170] = 28,
	[28][171] = 28,
	[28][172] = 28,
	[28][173] = 28,
	[28][174] = 28,
	[28][175] = 28,
	[28][176] = 28,
	[28][177] = 28,
	[28][178] = 28,
	[28][179] = 28,
	[28][180] = 28,
	[28][181] = 28,
	[28][182] = 28,
	[28][183] = 28,
	[28][184] = 28,
	[28][185] = 28,
	[28][186] = 28,
	[28][187] = 28,
	[28][188] = 28,
	[28][189] = 28,
	[28][190] = 28,
	[28][191] = 28,
	[28][192] = 28,
	[28][193] = 28,
	[28][194] = 28,
	[28][195] = 28,
	[28][196] = 28,
	[28][197] = 28,
	[28][198] = 28,
	[28][199] = 28,
	[28][200] = 28,
	[28][201] = 28,
	[28][202] = 28,
	[28][203] = 28,
	[28][204] = 28,
	[28][205] = 28,
	[28][206] = 28,
	[28][207] = 28,
	[28][208] = 28,
	[28][209] = 28,
	[28][210] = 28,
	[28][211] = 28,
	[28][212] = 28,
	[28][213] = 28,
	[28][214] = 28,
	[28][215] = 28,
	[28][216] = 28,
	[28][217] = 28,
	[28][218] = 28,
	[28][219] = 28,
	[28][220] = 28,
	[28][221] = 28,
	[28][222] = 28,
	[28][223] = 28,
	[28][224] = 28,
	[28][225] = 28,
	[28][226] = 28,
	[28][227] = 28,
	[28][228] = 28,
	[28][229] = 28,
	[28][230] = 28,
	[28][231] = 28,
	[28][232] = 28,
	[28][233] = 28,
	[28][234] = 28,
	[28][235] = 28,
	[28][236] = 28,
	[28][237] = 28,
	[28][238] = 28,
	[28][239] = 28,
	[28][240] = 28,
	[28][241] = 28,
	[28][242] = 28,
	[28][243] = 28,
	[28][244] = 28,
	[28][245] = 28,
	[28][246] = 28,
	[28][247] = 28,
	[28][248] = 28,
	[28][249] = 28,
	[28][250] = 28,
	[28][251] = 28,
	[28][252] = 28,
	[28][253] = 28,
	[28][254] = 28,
	[28][255] = 28,
	[29][111] = 36,
	[30][110] = 37,
	[30][120] = 38,
	[31][105] = 39,
	[32][110] = 40,
	[33][117] = 41,
	[35][34] = 28,
	[35][92] = 28,
	[35][110] = 28,
	[35][116] = 28,
	[36][109] = 42,
	[37][118] = 43,
	[38][105] = 44,
	[39][108] = 45,
	[40][112] = 46,
	[41][116] = 47,
	[42][109] = 48,
	[43][105] = 49,
	[44][116] = 50,
	[45][101] = 51,
	[46][117] = 52,
	[47][112] = 53,
	[48][97] = 54,
	[49][114] = 55,
	[50][45] = 56,
	[51][115] = 57,
	[52][116] = 58,
	[53][117] = 59,
	[54][110] = 60,
	[55][111] = 61,
	[56][99] = 62,
	[57][34] = 63,
	[58][34] = 64,
	[59][116] = 65,
	[60][100] = 66,
	[61][110] = 67,
	[62][111] = 68,
	[65][34] = 69,
	[66][115] = 70,
	[67][109] = 71,
	[68][100] = 72,
	[70][34] = 73,
	[71][101] = 74,
	[72][101] = 75,
	[74][110] = 76,
	[75][34] = 77,
	[76][116] = 78,
	[78][34] = 79,
};


const unsigned zebu_lexer_starts[127] = {
	[1] = 1,
	[2] = 2,
	[3] = 3,
	[4] = 4,
	[5] = 3,
	[6] = 5,
	[7] = 5,
	[8] = 6,
	[9] = 6,
	[10] = 6,
	[11] = 6,
	[12] = 6,
	[13] = 6,
	[14] = 5,
	[15] = 2,
	[16] = 3,
	[17] = 3,
	[18] = 5,
	[19] = 7,
	[20] = 5,
	[21] = 1,
	[22] = 5,
	[23] = 7,
	[24] = 5,
	[25] = 1,
	[26] = 5,
	[27] = 1,
	[28] = 5,
	[29] = 8,
	[30] = 5,
	[31] = 3,
	[32] = 3,
	[33] = 9,
	[34] = 10,
	[35] = 11,
	[36] = 9,
	[37] = 11,
	[38] = 12,
	[39] = 11,
	[40] = 11,
	[41] = 11,
	[42] = 6,
	[43] = 11,
	[44] = 5,
	[45] = 13,
	[46] = 12,
	[47] = 11,
	[48] = 11,
	[49] = 14,
	[50] = 4,
	[51] = 5,
	[52] = 6,
	[53] = 6,
	[54] = 6,
	[55] = 6,
	[56] = 6,
	[57] = 6,
	[58] = 5,
	[59] = 6,
	[60] = 11,
	[61] = 11,
	[62] = 13,
	[63] = 5,
	[64] = 15,
	[65] = 11,
	[66] = 11,
	[67] = 5,
	[68] = 5,
	[69] = 5,
	[70] = 16,
	[71] = 5,
	[72] = 9,
	[73] = 11,
	[74] = 5,
	[75] = 13,
	[76] = 15,
	[77] = 14,
	[78] = 14,
	[79] = 10,
	[80] = 11,
	[81] = 11,
	[82] = 14,
	[83] = 5,
	[84] = 5,
	[85] = 5,
	[86] = 5,
	[87] = 5,
	[88] = 5,
	[89] = 5,
	[90] = 17,
	[91] = 9,
	[92] = 11,
	[93] = 11,
	[94] = 13,
	[95] = 15,
	[96] = 12,
	[97] = 11,
	[98] = 11,
	[99] = 12,
	[100] = 13,
	[101] = 13,
	[102] = 5,
	[103] = 5,
	[104] = 5,
	[105] = 15,
	[106] = 12,
	[107] = 14,
	[108] = 14,
	[109] = 11,
	[110] = 11,
	[111] = 13,
	[112] = 11,
	[113] = 11,
	[114] = 11,
	[115] = 11,
	[116] = 15,
	[117] = 13,
	[118] = 13,
	[119] = 14,
	[120] = 14,
	[121] = 15,
	[122] = 12,
	[123] = 13,
	[124] = 13,
	[125] = 13,
	[126] = 13,
};


const unsigned zebu_lexer_accepts[80] = {
	[18] = 1,
	[19] = 2,
	[20] = 3,
	[21] = 4,
	[22] = 5,
	[24] = 6,
	[25] = 13,
	[26] = 14,
	[27] = 15,
	[34] = 16,
	[63] = 7,
	[64] = 10,
	[69] = 11,
	[73] = 8,
	[77] = 12,
	[79] = 9,
};


const unsigned zebu_lexer_EOFs[4] = {
	[3] = 22,
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
	printf("\"\e[0m) on line %u\n", token->line);
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

void print_zebu_command(struct link* links, enum prefix p, const char* name, struct zebu_command* ptree);

void print_zebu_commands(struct link* links, enum prefix p, const char* name, struct zebu_commands* ptree);

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
	printf("\e[34m%s\e[m (\e[36m$start\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
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
void print_zebu_command(struct link* links, enum prefix p, const char* name, struct zebu_command* ptree)
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
	printf("\e[34m%s\e[m (\e[36mcommand\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
	if (ptree->args.n)
	{
		for (unsigned i = 0, n = ptree->args.n; i < n; i++)
		{
			char label[4 + 30];
			snprintf(label, sizeof(label), "args[%u]", i);
			print_token_leaf(new ?: links, i + 1 < n ? p_not_last_child : p_last_child, label, ptree->args.data[i]);
		}
	}
	else
	{
		print_empty_leaf(new ?: links, p_last_child, "token[]", "args");
	}
	free(new);
}
void print_zebu_commands(struct link* links, enum prefix p, const char* name, struct zebu_commands* ptree)
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
	printf("\e[34m%s\e[m (\e[36mcommands\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
	if (ptree->commands.n)
	{
		for (unsigned i = 0, n = ptree->commands.n; i < n; i++)
		{
			char label[8 + 30];
			snprintf(label, sizeof(label), "commands[%u]", i);
			print_zebu_command(new ?: links, i + 1 < n ? p_not_last_child : p_last_child, label, ptree->commands.data[i]);
		}
	}
	else
	{
		print_empty_leaf(new ?: links, p_last_child, "command[]", "commands");
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
	printf("\e[34m%s\e[m (\e[36menvironment\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
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
	printf("\e[34m%s\e[m (\e[36mfile\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
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
	printf("\e[34m%s\e[m (\e[36mtest\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
	print_scanf_leaf(new ?: links, p_not_last_child, "code", "%u", ptree->code);
	if (ptree->commands)
		print_zebu_commands(new ?: links, p_not_last_child, "commands", ptree->commands);
	else
		print_empty_leaf(new ?: links, p_not_last_child, "commands", "commands");
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
	printf("\e[34m%s\e[m (\e[36mtext\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
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
	printf("\e[34m%s\e[m (\e[36mvariable\e[m) on line %u through %u\n", name, ptree->startline, ptree->endline);
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

struct zebu_command* inc_zebu_command(struct zebu_command* ptree)
{
	if (ptree) ptree->refcount++;
	return ptree;
}

struct zebu_commands* inc_zebu_commands(struct zebu_commands* ptree)
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

extern void free_zebu_command(struct zebu_command* ptree);

extern void free_zebu_commands(struct zebu_commands* ptree);

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

void free_zebu_command(struct zebu_command* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		for (unsigned i = 0, n = ptree->args.n; i < n; i++)
			free_zebu_token(ptree->args.data[i]);
		free(ptree->args.data);
		free(ptree);
	}
}

void free_zebu_commands(struct zebu_commands* ptree)
{
	if (ptree && !--ptree->refcount)
	{
		for (unsigned i = 0, n = ptree->commands.n; i < n; i++)
			free_zebu_command(ptree->commands.data[i]);
		free(ptree->commands.data);
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
		free_zebu_commands(ptree->commands);
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



#define argv0 (program_invocation_name)

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
		case '/':
		case '<': case '>':
		case '(': case ')':
		case '{': case '}':
		case '[': case ']':
		case ':': case ';':
		case ',': case '.':
		case '_':
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
			sprintf(out, "\\x%02hhX", in);
			break;
	}
}
#endif

struct zebu_$start* zebu_parse(FILE* stream)
{
	void* root;
	struct { unsigned* data, n, cap; } yacc = {};
	struct { void** data; unsigned n, cap; } data = {};
	struct { unsigned char* data; unsigned n, cap, line; } lexer = {
		.line = 1,
	};
	
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
	
	void push_char(unsigned char c)
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
	
	unsigned y, t, s, r;
	void* td;
	
	void read_token(unsigned l)
	{
		unsigned original_l = l, i = 0, a, b, c, f = 0;
		
		unsigned line = lexer.line;
		
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
				push_char(c);
				
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
					l = a, t = b, f = i++, lexer.line = line;
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
				
				if (c == '\n')
				{
					line++;
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: line: %u\n", line);
					#endif
				}
			}
			else if (b)
			{
				#ifdef ZEBU_DEBUG
				ddprintf("lexer: token: \"%.*s\", line: %u\n", i, lexer.data, line);
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
					
					l = original_l, t = 0, lexer.line = line;
					memmove(lexer.data, lexer.data + i, lexer.n - i), lexer.n -= i, i = 0;
				}
				else
				{
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: i = %u\n", i);
					#endif
					
					struct zebu_token* token = malloc(sizeof(*token));
					token->refcount = 1;
					token->line = line;
					token->data = memcpy(malloc(i + 1), lexer.data, i);
					token->data[i] = 0;
					token->len = i;
					t = b, td = token;
					
					lexer.line = line;
					memmove(lexer.data, lexer.data + i, lexer.n - i), lexer.n -= i;
					break;
				}
			}
			else if (t)
			{
				if (t == 1)
				{
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: falling back to whitespace: \"%.*s\"\n", f, lexer.data);
					#endif
					
					l = original_l, t = 0, line = lexer.line;
					memmove(lexer.data, lexer.data + f, lexer.n - f), lexer.n -= f, f = 0, i = 0;
				}
				else
				{
					#ifdef ZEBU_DEBUG
					ddprintf("lexer: falling back to token: \"%.*s\"\n", f, lexer.data);
					#endif
					
					struct zebu_token* token = malloc(sizeof(*token));
					token->refcount = 1;
					token->line = lexer.line;
					token->data = memcpy(malloc(f + 1), lexer.data, f);
					token->data[f] = 0;
					token->len = f;
					td = token;
					
					memmove(lexer.data, lexer.data + f, lexer.n - f), lexer.n -= f, f = 0;
					break;
				}
			}
			else
			{
				if (i != 0)
				{
					if (c == (unsigned) EOF)
						fprintf(stderr, "%s: unexpected '<EOF>' when reading '%.*s' on line %u!\n", argv0, i, lexer.data, line);
					else
						fprintf(stderr, "%s: unexpected '%c' when reading '%.*s' on line %u!\n", argv0, c, i, lexer.data, line);
				}
				else
				{
					if (c == (unsigned) EOF)
						fprintf(stderr, "%s: unexpected '<EOF>' on line %u!\n", argv0, line);
					else
						fprintf(stderr, "%s: unexpected '%c' on line %u!\n", argv0, c, line);
				}
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
	case 2:
	{
		struct zebu_$start* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_$start* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
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
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 28;
		break;
	}
	case 1:
	{
		struct zebu_$start* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 1;
		break;
	}
	case 6:
	{
		struct zebu_$start* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_$start* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
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
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
if (value->tests.n == value->tests.cap)
{
value->tests.cap = value->tests.cap << 1 ?: 1;
value->tests.data = realloc(value->tests.data, sizeof(*value->tests.data) * value->tests.cap);
}
memmove(value->tests.data + 1, value->tests.data, sizeof(*value->tests.data) * value->tests.n);
value->tests.data[0] = inc_zebu_test(subgrammar), value->tests.n++;
free_zebu_test(subgrammar);
}
		d = value, g = 1;
		break;
	}
	case 5:
	{
		struct zebu_$start* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 4;
		break;
	}
	case 14:
	{
		struct zebu_$start* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_$start* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
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
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
if (value->tests.n == value->tests.cap)
{
value->tests.cap = value->tests.cap << 1 ?: 1;
value->tests.data = realloc(value->tests.data, sizeof(*value->tests.data) * value->tests.cap);
}
memmove(value->tests.data + 1, value->tests.data, sizeof(*value->tests.data) * value->tests.n);
value->tests.data[0] = inc_zebu_test(subgrammar), value->tests.n++;
free_zebu_test(subgrammar);
}
		d = value, g = 4;
		break;
	}
	case 13:
	{
		struct zebu_$start* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_$start* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
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
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 4;
		break;
	}
	case 15:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 5;
		break;
	}
	case 19:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
struct zebu_commands* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_commands(value->commands), value->commands = inc_zebu_commands(subgrammar);
free_zebu_commands(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 6;
		break;
	}
	case 22:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_environment(value->environment), value->environment = inc_zebu_environment(subgrammar);
free_zebu_environment(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 7;
		break;
	}
	case 25:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_text(value->input), value->input = inc_zebu_text(subgrammar);
free_zebu_text(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 8;
		break;
	}
	case 26:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_text(value->output), value->output = inc_zebu_text(subgrammar);
free_zebu_text(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 9;
		break;
	}
	case 27:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 10;
		break;
	}
	case 28:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 14;
		break;
	}
	case 29:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
if (value->files.n == value->files.cap)
{
value->files.cap = value->files.cap << 1 ?: 1;
value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
}
memmove(value->files.data + 1, value->files.data, sizeof(*value->files.data) * value->files.n);
value->files.data[0] = inc_zebu_file(subgrammar), value->files.n++;
free_zebu_file(subgrammar);
}
		d = value, g = 14;
		break;
	}
	case 47:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 22;
		break;
	}
	case 48:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 22;
		break;
	}
	case 49:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
if (value->files.n == value->files.cap)
{
value->files.cap = value->files.cap << 1 ?: 1;
value->files.data = realloc(value->files.data, sizeof(*value->files.data) * value->files.cap);
}
memmove(value->files.data + 1, value->files.data, sizeof(*value->files.data) * value->files.n);
value->files.data[0] = inc_zebu_file(subgrammar), value->files.n++;
free_zebu_file(subgrammar);
}
		d = value, g = 22;
		break;
	}
	case 43:
	{
		struct zebu_text* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 26;
		break;
	}
	case 57:
	{
		struct zebu_text* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_text* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->lines.n){
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
if (value->lines.n == value->lines.cap)
{
value->lines.cap = value->lines.cap << 1 ?: 1;
value->lines.data = realloc(value->lines.data, sizeof(*value->lines.data) * value->lines.cap);
}
memmove(value->lines.data + 1, value->lines.data, sizeof(*value->lines.data) * value->lines.n);
value->lines.data[0] = inc_zebu_token(token), value->lines.n++;
free_zebu_token(token);
}
		d = value, g = 26;
		break;
	}
	case 58:
	{
		struct zebu_text* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_text* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->lines.n){
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 26;
		break;
	}
	case 44:
	{
		struct zebu_text* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_text* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->lines.n){
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
if (value->lines.n == value->lines.cap)
{
value->lines.cap = value->lines.cap << 1 ?: 1;
value->lines.data = realloc(value->lines.data, sizeof(*value->lines.data) * value->lines.cap);
}
memmove(value->lines.data + 1, value->lines.data, sizeof(*value->lines.data) * value->lines.n);
value->lines.data[0] = inc_zebu_token(token), value->lines.n++;
free_zebu_token(token);
}
		d = value, g = 21;
		break;
	}
	case 23:
	{
		struct zebu_text* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 21;
		break;
	}
	case 60:
	{
		struct zebu_command* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_command* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->args.n){
while (value->args.n + trie->args.n > value->args.cap)
{
value->args.cap = value->args.cap << 1 ?: 1;
value->args.data = realloc(value->args.data, sizeof(*value->args.data) * value->args.cap);
}
memmove(value->args.data + trie->args.n, value->args.data, sizeof(*value->args.data) * value->args.n);
for (unsigned i = 0, n = trie->args.n; i < n; i++)
value->args.data[i] = inc_zebu_token(trie->args.data[i]);
value->args.n += trie->args.n;
}
free_zebu_command(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 27;
		break;
	}
	case 59:
	{
		struct zebu_command* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_command* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->args.n){
while (value->args.n + trie->args.n > value->args.cap)
{
value->args.cap = value->args.cap << 1 ?: 1;
value->args.data = realloc(value->args.data, sizeof(*value->args.data) * value->args.cap);
}
memmove(value->args.data + trie->args.n, value->args.data, sizeof(*value->args.data) * value->args.n);
for (unsigned i = 0, n = trie->args.n; i < n; i++)
value->args.data[i] = inc_zebu_token(trie->args.data[i]);
value->args.n += trie->args.n;
}
free_zebu_command(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
if (value->args.n == value->args.cap)
{
value->args.cap = value->args.cap << 1 ?: 1;
value->args.data = realloc(value->args.data, sizeof(*value->args.data) * value->args.cap);
}
memmove(value->args.data + 1, value->args.data, sizeof(*value->args.data) * value->args.n);
value->args.data[0] = inc_zebu_token(token), value->args.n++;
free_zebu_token(token);
}
		d = value, g = 27;
		break;
	}
	case 50:
	{
		struct zebu_command* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 27;
		break;
	}
	case 51:
	{
		struct zebu_command* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_command* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->args.n){
while (value->args.n + trie->args.n > value->args.cap)
{
value->args.cap = value->args.cap << 1 ?: 1;
value->args.data = realloc(value->args.data, sizeof(*value->args.data) * value->args.cap);
}
memmove(value->args.data + trie->args.n, value->args.data, sizeof(*value->args.data) * value->args.n);
for (unsigned i = 0, n = trie->args.n; i < n; i++)
value->args.data[i] = inc_zebu_token(trie->args.data[i]);
value->args.n += trie->args.n;
}
free_zebu_command(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
if (value->args.n == value->args.cap)
{
value->args.cap = value->args.cap << 1 ?: 1;
value->args.data = realloc(value->args.data, sizeof(*value->args.data) * value->args.cap);
}
memmove(value->args.data + 1, value->args.data, sizeof(*value->args.data) * value->args.n);
value->args.data[0] = inc_zebu_token(token), value->args.n++;
free_zebu_token(token);
}
		d = value, g = 23;
		break;
	}
	case 30:
	{
		struct zebu_command* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 23;
		break;
	}
	case 33:
	{
		struct zebu_commands* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_commands* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->commands.n)
{
while (value->commands.n + trie->commands.n > value->commands.cap)
{
value->commands.cap = value->commands.cap << 1 ?: 1;
value->commands.data = realloc(value->commands.data, sizeof(*value->commands.data) * value->commands.cap);
}
memmove(value->commands.data + trie->commands.n, value->commands.data, sizeof(*value->commands.data) * value->commands.n);
for (unsigned i = 0, n = trie->commands.n; i < n; i++)
value->commands.data[i] = inc_zebu_command(trie->commands.data[i]);
value->commands.n += trie->commands.n;
}
free_zebu_commands(trie);
}
{
struct zebu_command* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
if (value->commands.n == value->commands.cap)
{
value->commands.cap = value->commands.cap << 1 ?: 1;
value->commands.data = realloc(value->commands.data, sizeof(*value->commands.data) * value->commands.cap);
}
memmove(value->commands.data + 1, value->commands.data, sizeof(*value->commands.data) * value->commands.n);
value->commands.data[0] = inc_zebu_command(subgrammar), value->commands.n++;
free_zebu_command(subgrammar);
}
		d = value, g = 16;
		break;
	}
	case 16:
	{
		struct zebu_commands* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 16;
		break;
	}
	case 52:
	{
		struct zebu_commands* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_commands* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->commands.n)
{
while (value->commands.n + trie->commands.n > value->commands.cap)
{
value->commands.cap = value->commands.cap << 1 ?: 1;
value->commands.data = realloc(value->commands.data, sizeof(*value->commands.data) * value->commands.cap);
}
memmove(value->commands.data + trie->commands.n, value->commands.data, sizeof(*value->commands.data) * value->commands.n);
for (unsigned i = 0, n = trie->commands.n; i < n; i++)
value->commands.data[i] = inc_zebu_command(trie->commands.data[i]);
value->commands.n += trie->commands.n;
}
free_zebu_commands(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 24;
		break;
	}
	case 53:
	{
		struct zebu_commands* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_commands* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->commands.n)
{
while (value->commands.n + trie->commands.n > value->commands.cap)
{
value->commands.cap = value->commands.cap << 1 ?: 1;
value->commands.data = realloc(value->commands.data, sizeof(*value->commands.data) * value->commands.cap);
}
memmove(value->commands.data + trie->commands.n, value->commands.data, sizeof(*value->commands.data) * value->commands.n);
for (unsigned i = 0, n = trie->commands.n; i < n; i++)
value->commands.data[i] = inc_zebu_command(trie->commands.data[i]);
value->commands.n += trie->commands.n;
}
free_zebu_commands(trie);
}
{
struct zebu_command* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
if (value->commands.n == value->commands.cap)
{
value->commands.cap = value->commands.cap << 1 ?: 1;
value->commands.data = realloc(value->commands.data, sizeof(*value->commands.data) * value->commands.cap);
}
memmove(value->commands.data + 1, value->commands.data, sizeof(*value->commands.data) * value->commands.n);
value->commands.data[0] = inc_zebu_command(subgrammar), value->commands.n++;
free_zebu_command(subgrammar);
}
		d = value, g = 24;
		break;
	}
	case 32:
	{
		struct zebu_commands* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 24;
		break;
	}
	case 42:
	{
		struct zebu_environment* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_environment* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
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
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
if (value->variables.n == value->variables.cap)
{
value->variables.cap = value->variables.cap << 1 ?: 1;
value->variables.data = realloc(value->variables.data, sizeof(*value->variables.data) * value->variables.cap);
}
memmove(value->variables.data + 1, value->variables.data, sizeof(*value->variables.data) * value->variables.n);
value->variables.data[0] = inc_zebu_variable(subgrammar), value->variables.n++;
free_zebu_variable(subgrammar);
}
		d = value, g = 19;
		break;
	}
	case 20:
	{
		struct zebu_environment* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 19;
		break;
	}
	case 56:
	{
		struct zebu_environment* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_environment* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
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
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
if (value->variables.n == value->variables.cap)
{
value->variables.cap = value->variables.cap << 1 ?: 1;
value->variables.data = realloc(value->variables.data, sizeof(*value->variables.data) * value->variables.cap);
}
memmove(value->variables.data + 1, value->variables.data, sizeof(*value->variables.data) * value->variables.n);
value->variables.data[0] = inc_zebu_variable(subgrammar), value->variables.n++;
free_zebu_variable(subgrammar);
}
		d = value, g = 25;
		break;
	}
	case 55:
	{
		struct zebu_environment* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_environment* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 25;
		break;
	}
	case 41:
	{
		struct zebu_environment* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 25;
		break;
	}
	case 38:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 18;
		break;
	}
	case 39:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 18;
		break;
	}
	case 35:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 18;
		break;
	}
	case 36:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 18;
		break;
	}
	case 40:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 18;
		break;
	}
	case 18:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 18;
		break;
	}
	case 34:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 18;
		break;
	}
	case 37:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 18;
		break;
	}
	case 3:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 3;
		break;
	}
	case 9:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 3;
		break;
	}
	case 10:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 3;
		break;
	}
	case 11:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 3;
		break;
	}
	case 12:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 3;
		break;
	}
	case 7:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 3;
		break;
	}
	case 8:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 3;
		break;
	}
	case 31:
	{
		struct zebu_command* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_command* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->args.n){
while (value->args.n + trie->args.n > value->args.cap)
{
value->args.cap = value->args.cap << 1 ?: 1;
value->args.data = realloc(value->args.data, sizeof(*value->args.data) * value->args.cap);
}
memmove(value->args.data + trie->args.n, value->args.data, sizeof(*value->args.data) * value->args.n);
for (unsigned i = 0, n = trie->args.n; i < n; i++)
value->args.data[i] = inc_zebu_token(trie->args.data[i]);
value->args.n += trie->args.n;
}
free_zebu_command(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 17;
		break;
	}
	case 17:
	{
		struct zebu_commands* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_commands* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->commands.n)
{
while (value->commands.n + trie->commands.n > value->commands.cap)
{
value->commands.cap = value->commands.cap << 1 ?: 1;
value->commands.data = realloc(value->commands.data, sizeof(*value->commands.data) * value->commands.cap);
}
memmove(value->commands.data + trie->commands.n, value->commands.data, sizeof(*value->commands.data) * value->commands.n);
for (unsigned i = 0, n = trie->commands.n; i < n; i++)
value->commands.data[i] = inc_zebu_command(trie->commands.data[i]);
value->commands.n += trie->commands.n;
}
free_zebu_commands(trie);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 11;
		break;
	}
	case 21:
	{
		struct zebu_environment* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_environment* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 12;
		break;
	}
	case 45:
	{
		struct zebu_file* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(value->srcpath), value->srcpath = inc_zebu_token(token);
free_zebu_token(token);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(value->path), value->path = inc_zebu_token(token);
free_zebu_token(token);
}
		d = value, g = 15;
		break;
	}
	case 46:
	{
		struct zebu_file* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_text* subgrammar = data.data[--yacc.n, --data.n];
if (subgrammar->startline < value->startline) value->startline = subgrammar->startline;
if (value->endline < subgrammar->endline) value->endline = subgrammar->endline;
free_zebu_text(value->content), value->content = inc_zebu_text(subgrammar);
free_zebu_text(subgrammar);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(value->path), value->path = inc_zebu_token(token);
free_zebu_token(token);
}
		d = value, g = 15;
		break;
	}
	case 4:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_test* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->code) { value->code = trie->code; }
if (trie->commands) { free_zebu_commands(value->commands); value->commands = inc_zebu_commands(trie->commands); }
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 2;
		break;
	}
	case 24:
	{
		struct zebu_text* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_text* trie = data.data[--yacc.n, --data.n];
if (trie->startline < value->startline) value->startline = trie->startline;
if (value->endline < trie->endline) value->endline = trie->endline;
if (trie->lines.n){
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
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
		d = value, g = 13;
		break;
	}
	case 54:
	{
		struct zebu_variable* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->startline = -1;
		value->endline = 0;
		value->refcount = 1;
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(value->value), value->value = inc_zebu_token(token);
free_zebu_token(token);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(token);
}
{
struct zebu_token* token = data.data[--yacc.n, --data.n];
if (token->line < value->startline) value->startline = token->line;
if (value->endline < token->line) value->endline = token->line;
free_zebu_token(value->name), value->name = inc_zebu_token(token);
free_zebu_token(token);
}
		d = value, g = 20;
		break;
	}
}
			
			if (g == 28)
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

















