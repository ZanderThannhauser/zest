#include "parser.h"

#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

const unsigned zebu_shifts[103][17] = {
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
	[25][2] = 34,
	[27][2] = 34,
	[29][15] = 40,
	[33][6] = 42,
	[33][16] = 41,
	[34][3] = 46,
	[34][16] = 45,
	[35][6] = 49,
	[35][7] = 50,
	[35][8] = 51,
	[35][9] = 52,
	[35][10] = 53,
	[35][11] = 54,
	[35][12] = 55,
	[35][13] = 48,
	[36][6] = 58,
	[36][16] = 57,
	[37][6] = 49,
	[37][7] = 50,
	[37][8] = 51,
	[37][9] = 52,
	[37][10] = 53,
	[37][11] = 54,
	[37][12] = 55,
	[37][13] = 48,
	[38][6] = 49,
	[38][7] = 50,
	[38][8] = 51,
	[38][9] = 52,
	[38][10] = 53,
	[38][11] = 54,
	[38][12] = 55,
	[38][13] = 48,
	[39][6] = 49,
	[39][7] = 50,
	[39][8] = 51,
	[39][9] = 52,
	[39][10] = 53,
	[39][11] = 54,
	[39][12] = 55,
	[39][13] = 48,
	[40][6] = 49,
	[40][7] = 50,
	[40][8] = 51,
	[40][9] = 52,
	[40][10] = 53,
	[40][11] = 54,
	[40][12] = 55,
	[40][13] = 48,
	[41][14] = 65,
	[42][6] = 49,
	[42][7] = 50,
	[42][8] = 51,
	[42][9] = 52,
	[42][10] = 53,
	[42][11] = 54,
	[42][12] = 55,
	[42][13] = 48,
	[44][6] = 68,
	[44][13] = 67,
	[44][16] = 41,
	[45][3] = 73,
	[45][13] = 72,
	[45][16] = 71,
	[48][6] = 7,
	[48][7] = 8,
	[48][8] = 9,
	[48][9] = 10,
	[48][10] = 11,
	[48][11] = 12,
	[48][12] = 13,
	[50][14] = 19,
	[51][14] = 21,
	[52][14] = 23,
	[53][14] = 25,
	[54][14] = 27,
	[55][14] = 29,
	[57][14] = 82,
	[60][6] = 84,
	[60][13] = 83,
	[60][16] = 57,
	[65][2] = 87,
	[65][16] = 88,
	[67][6] = 42,
	[67][16] = 41,
	[68][6] = 49,
	[68][7] = 50,
	[68][8] = 51,
	[68][9] = 52,
	[68][10] = 53,
	[68][11] = 54,
	[68][12] = 55,
	[68][13] = 48,
	[70][6] = 68,
	[70][13] = 67,
	[70][16] = 41,
	[71][3] = 73,
	[71][13] = 72,
	[71][16] = 71,
	[72][3] = 46,
	[72][16] = 45,
	[82][16] = 95,
	[83][6] = 58,
	[83][16] = 57,
	[86][6] = 84,
	[86][13] = 83,
	[86][16] = 57,
	[87][3] = 99,
	[87][16] = 98,
	[98][3] = 103,
	[98][13] = 102,
	[98][16] = 101,
	[101][3] = 103,
	[101][13] = 102,
	[101][16] = 101,
	[102][3] = 99,
	[102][16] = 98,
};


const unsigned zebu_reduces[107][17] = {
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
	[43][3] = 15,
	[43][4] = 15,
	[43][13] = 15,
	[46][6] = 16,
	[46][7] = 16,
	[46][8] = 16,
	[46][9] = 16,
	[46][10] = 16,
	[46][11] = 16,
	[46][12] = 16,
	[46][13] = 16,
	[47][6] = 17,
	[47][7] = 17,
	[47][8] = 17,
	[47][9] = 17,
	[47][10] = 17,
	[47][11] = 17,
	[47][12] = 17,
	[47][13] = 17,
	[49][3] = 18,
	[49][4] = 18,
	[49][13] = 18,
	[56][3] = 19,
	[56][4] = 19,
	[56][13] = 19,
	[58][6] = 20,
	[58][7] = 20,
	[58][8] = 20,
	[58][9] = 20,
	[58][10] = 20,
	[58][11] = 20,
	[58][12] = 20,
	[58][13] = 20,
	[59][6] = 21,
	[59][7] = 21,
	[59][8] = 21,
	[59][9] = 21,
	[59][10] = 21,
	[59][11] = 21,
	[59][12] = 21,
	[59][13] = 21,
	[61][3] = 22,
	[61][4] = 22,
	[61][13] = 22,
	[62][3] = 23,
	[62][4] = 23,
	[62][13] = 23,
	[63][3] = 24,
	[63][4] = 24,
	[63][13] = 24,
	[64][3] = 25,
	[64][4] = 25,
	[64][13] = 25,
	[66][3] = 26,
	[66][4] = 26,
	[66][13] = 26,
	[69][3] = 27,
	[69][4] = 27,
	[69][13] = 27,
	[73][6] = 28,
	[73][7] = 28,
	[73][8] = 28,
	[73][9] = 28,
	[73][10] = 28,
	[73][11] = 28,
	[73][12] = 28,
	[73][13] = 28,
	[74][6] = 29,
	[74][7] = 29,
	[74][8] = 29,
	[74][9] = 29,
	[74][10] = 29,
	[74][11] = 29,
	[74][12] = 29,
	[74][13] = 29,
	[75][3] = 30,
	[75][4] = 30,
	[75][13] = 30,
	[76][3] = 31,
	[76][4] = 31,
	[76][13] = 31,
	[77][3] = 32,
	[77][4] = 32,
	[77][13] = 32,
	[78][3] = 33,
	[78][4] = 33,
	[78][13] = 33,
	[79][3] = 34,
	[79][4] = 34,
	[79][13] = 34,
	[80][3] = 35,
	[80][4] = 35,
	[80][13] = 35,
	[81][3] = 36,
	[81][4] = 36,
	[81][13] = 36,
	[84][6] = 37,
	[84][7] = 37,
	[84][8] = 37,
	[84][9] = 37,
	[84][10] = 37,
	[84][11] = 37,
	[84][12] = 37,
	[84][13] = 37,
	[85][6] = 38,
	[85][7] = 38,
	[85][8] = 38,
	[85][9] = 38,
	[85][10] = 38,
	[85][11] = 38,
	[85][12] = 38,
	[85][13] = 38,
	[88][6] = 39,
	[88][13] = 39,
	[88][16] = 39,
	[89][6] = 40,
	[89][13] = 40,
	[89][16] = 40,
	[90][3] = 41,
	[90][4] = 41,
	[90][13] = 41,
	[91][3] = 42,
	[91][4] = 42,
	[91][13] = 42,
	[92][3] = 43,
	[92][4] = 43,
	[92][13] = 43,
	[93][6] = 44,
	[93][7] = 44,
	[93][8] = 44,
	[93][9] = 44,
	[93][10] = 44,
	[93][11] = 44,
	[93][12] = 44,
	[93][13] = 44,
	[94][6] = 45,
	[94][7] = 45,
	[94][8] = 45,
	[94][9] = 45,
	[94][10] = 45,
	[94][11] = 45,
	[94][12] = 45,
	[94][13] = 45,
	[95][6] = 46,
	[95][13] = 46,
	[95][16] = 46,
	[96][6] = 47,
	[96][7] = 47,
	[96][8] = 47,
	[96][9] = 47,
	[96][10] = 47,
	[96][11] = 47,
	[96][12] = 47,
	[96][13] = 47,
	[97][6] = 48,
	[97][7] = 48,
	[97][8] = 48,
	[97][9] = 48,
	[97][10] = 48,
	[97][11] = 48,
	[97][12] = 48,
	[97][13] = 48,
	[99][6] = 16,
	[99][13] = 16,
	[99][16] = 16,
	[100][6] = 17,
	[100][13] = 17,
	[100][16] = 17,
	[103][6] = 28,
	[103][13] = 28,
	[103][16] = 28,
	[104][6] = 29,
	[104][13] = 29,
	[104][16] = 29,
	[105][6] = 44,
	[105][13] = 44,
	[105][16] = 44,
	[106][6] = 45,
	[106][13] = 45,
	[106][16] = 45,
};


const unsigned zebu_gotos[103][22] = {
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
	[25][11] = 38,
	[27][11] = 39,
	[33][13] = 43,
	[33][14] = 44,
	[34][15] = 47,
	[35][16] = 56,
	[36][17] = 59,
	[36][18] = 60,
	[37][16] = 61,
	[38][16] = 62,
	[39][16] = 63,
	[40][16] = 64,
	[42][16] = 66,
	[44][14] = 70,
	[44][19] = 69,
	[45][20] = 74,
	[48][3] = 75,
	[50][5] = 76,
	[51][6] = 77,
	[52][7] = 78,
	[53][8] = 79,
	[54][9] = 80,
	[55][10] = 81,
	[60][18] = 86,
	[60][21] = 85,
	[65][11] = 89,
	[67][13] = 90,
	[67][14] = 44,
	[68][16] = 91,
	[70][14] = 70,
	[70][19] = 92,
	[71][20] = 93,
	[72][15] = 94,
	[83][17] = 96,
	[83][18] = 60,
	[86][18] = 86,
	[86][21] = 97,
	[87][15] = 100,
	[98][20] = 104,
	[101][20] = 105,
	[102][15] = 106,
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
	[5][123] = 19,
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
	[26][9] = 26,
	[26][32] = 26,
	[26][33] = 26,
	[26][34] = 32,
	[26][35] = 26,
	[26][36] = 26,
	[26][37] = 26,
	[26][38] = 26,
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
	[26][63] = 26,
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
	[26][94] = 26,
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


const unsigned zebu_lexer_starts[107] = {
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
	[38] = 11,
	[39] = 11,
	[40] = 11,
	[41] = 6,
	[42] = 11,
	[43] = 5,
	[44] = 12,
	[45] = 13,
	[46] = 11,
	[47] = 11,
	[48] = 4,
	[49] = 5,
	[50] = 6,
	[51] = 6,
	[52] = 6,
	[53] = 6,
	[54] = 6,
	[55] = 6,
	[56] = 5,
	[57] = 6,
	[58] = 11,
	[59] = 11,
	[60] = 12,
	[61] = 5,
	[62] = 5,
	[63] = 5,
	[64] = 5,
	[65] = 14,
	[66] = 5,
	[67] = 9,
	[68] = 11,
	[69] = 5,
	[70] = 12,
	[71] = 13,
	[72] = 10,
	[73] = 11,
	[74] = 11,
	[75] = 5,
	[76] = 5,
	[77] = 5,
	[78] = 5,
	[79] = 5,
	[80] = 5,
	[81] = 5,
	[82] = 15,
	[83] = 9,
	[84] = 11,
	[85] = 11,
	[86] = 12,
	[87] = 10,
	[88] = 12,
	[89] = 12,
	[90] = 5,
	[91] = 5,
	[92] = 5,
	[93] = 11,
	[94] = 11,
	[95] = 12,
	[96] = 11,
	[97] = 11,
	[98] = 13,
	[99] = 12,
	[100] = 12,
	[101] = 13,
	[102] = 10,
	[103] = 12,
	[104] = 12,
	[105] = 12,
	[106] = 12,
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
					
					l = original_l, t = 0, f = 0;
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
			else if (t)
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
	case 2:
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
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 22;
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
		d = value, g = 1;
		break;
	}
	case 6:
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
		d = value, g = 1;
		break;
	}
	case 14:
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
		d = value, g = 4;
		break;
	}
	case 13:
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
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 4;
		break;
	}
	case 5:
	{
		struct zebu_$start* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 4;
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
		struct zebu_text* subgrammar = data.data[--yacc.n, --data.n];
		free_zebu_text(value->output), value->output = inc_zebu_text(subgrammar);
		free_zebu_text(subgrammar);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 9;
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
		d = value, g = 10;
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
		d = value, g = 13;
		break;
	}
	case 43:
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
		d = value, g = 19;
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
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 19;
		break;
	}
	case 42:
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
		d = value, g = 19;
		break;
	}
	case 44:
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
		d = value, g = 20;
		break;
	}
	case 45:
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
		d = value, g = 20;
		break;
	}
	case 28:
	{
		struct zebu_text* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 20;
		break;
	}
	case 29:
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
		d = value, g = 15;
		break;
	}
	case 16:
	{
		struct zebu_text* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 15;
		break;
	}
	case 38:
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
		d = value, g = 17;
		break;
	}
	case 20:
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
	case 47:
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
		d = value, g = 21;
		break;
	}
	case 37:
	{
		struct zebu_environment* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 21;
		break;
	}
	case 48:
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
		d = value, g = 21;
		break;
	}
	case 18:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 16;
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
		d = value, g = 16;
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
		d = value, g = 16;
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
		d = value, g = 16;
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
		d = value, g = 16;
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
		d = value, g = 16;
		break;
	}
	case 35:
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
		d = value, g = 16;
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
		d = value, g = 16;
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
		d = value, g = 3;
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
		d = value, g = 3;
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
		d = value, g = 3;
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
		d = value, g = 3;
		break;
	}
	case 12:
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
		d = value, g = 3;
		break;
	}
	case 3:
	{
		struct zebu_test* value = memset(malloc(sizeof(*value)), 0, sizeof(*value));
		value->refcount = 1;
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 3;
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
		d = value, g = 3;
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
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 5;
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
		free_zebu_text(value->commands), value->commands = inc_zebu_text(subgrammar);
		free_zebu_text(subgrammar);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 6;
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
		struct zebu_environment* subgrammar = data.data[--yacc.n, --data.n];
		free_zebu_environment(value->environment), value->environment = inc_zebu_environment(subgrammar);
		free_zebu_environment(subgrammar);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 7;
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
		struct zebu_text* subgrammar = data.data[--yacc.n, --data.n];
		free_zebu_text(value->input), value->input = inc_zebu_text(subgrammar);
		free_zebu_text(subgrammar);
		}
		{
		struct zebu_token* token = data.data[--yacc.n, --data.n];
		free_zebu_token(token);
		}
		d = value, g = 8;
		break;
	}
	case 21:
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
		d = value, g = 12;
		break;
	}
	case 39:
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
		d = value, g = 14;
		break;
	}
	case 40:
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
		d = value, g = 14;
		break;
	}
	case 4:
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
	case 17:
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
		d = value, g = 11;
		break;
	}
	case 46:
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
		d = value, g = 18;
		break;
	}
}
			
			if (g == 22)
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

















