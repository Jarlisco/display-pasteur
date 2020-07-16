#include "./font.h"

namespace hpasteur {

	const uint8_t PROGMEM Font::font_A[22] = {29, 26, 25, 2, 108, 65, 67, 92, 95, 94, 13, 7, 89, 53, 50, 74, 41, 35, 30, 86, 87, 51};
	const uint8_t PROGMEM Font::font_B[28] = {29, 28, 27, 6, 5, 4, 87, 77, 39, 56, 62, 58, 32, 34, 35, 51, 74, 43, 50, 49, 55, 12, 11, 10, 81, 19, 15, 36};
	const uint8_t PROGMEM Font::font_C[19] = {29, 62, 63, 69, 68, 67, 73, 97, 98, 102, 105, 17, 16, 23, 24, 32, 58, 12, 11};
	const uint8_t PROGMEM Font::font_D[24] = {29, 28, 27, 6, 5, 4, 87, 77, 39, 56, 62, 58, 32, 34, 35, 42, 46, 47, 14, 19, 81, 10, 11, 12};
	const uint8_t PROGMEM Font::font_E[22] = {29, 28, 27, 6, 5, 4, 87, 77, 39, 56, 62, 58, 33, 31, 30, 12, 85, 84, 83, 51, 50, 48};
	const uint8_t PROGMEM Font::font_F[18] = {62, 56, 39, 77, 87, 4, 51, 50, 48, 5, 6, 27, 28, 29, 12, 85, 84, 83};
	const uint8_t PROGMEM Font::font_G[26] = {29, 62, 98, 97, 73, 67, 68, 69, 63, 102, 105, 17, 16, 23, 24, 86, 87, 51, 58, 32, 34, 74, 43, 36, 35, 48};
	const uint8_t PROGMEM Font::font_H[25] = {20, 17, 104, 101, 99, 72, 67, 65, 86, 87, 51, 31, 35, 36, 43, 48, 55, 50, 94, 18, 81, 84, 49, 100, 96};
	const uint8_t PROGMEM Font::font_I[19] = {64, 63, 62, 58, 33, 22, 24, 29, 12, 85, 28, 27, 6, 5, 4, 87, 77, 39, 56};
	const uint8_t PROGMEM Font::font_J[20] = {20, 22, 24, 29, 12, 85, 84, 28, 27, 6, 5, 4, 87, 77, 39, 56, 62, 61, 60, 59};
	const uint8_t PROGMEM Font::font_K[23] = {62, 56, 39, 87, 77, 4, 5, 6, 27, 28, 29, 52, 88, 89, 8, 9, 10, 84, 76, 40, 37, 34, 31};
	const uint8_t PROGMEM Font::font_L[15] = {29, 28, 27, 6, 5, 4, 87, 77, 39, 56, 62, 58, 33, 31, 30};
	const uint8_t PROGMEM Font::font_M[30] = {87, 0, 1, 2, 90, 3, 16, 20, 52, 88, 89, 8, 9, 10, 84, 17, 104, 101, 99, 96, 72, 67, 65, 31, 35, 36, 48, 55, 18, 81};
	const uint8_t PROGMEM Font::font_N[28] = {65, 67, 72, 20, 17, 104, 101, 99, 16, 3, 90, 2, 1, 0, 87, 76, 40, 37, 34, 31, 36, 48, 55, 18, 81, 84, 43, 35};
	const uint8_t PROGMEM Font::font_O[28] = {29, 62, 63, 69, 68, 67, 73, 97, 98, 102, 105, 17, 16, 23, 24, 12, 11, 10, 19, 81, 14, 47, 46, 42, 35, 34, 32, 58};
	const uint8_t PROGMEM Font::font_P[21] = {62, 56, 39, 77, 87, 4, 5, 6, 27, 28, 29, 12, 11, 10, 81, 19, 15, 55, 49, 50, 51};
	const uint8_t PROGMEM Font::font_Q[31] = {22, 16, 17, 105, 102, 98, 97, 73, 67, 68, 69, 63, 62, 58, 32, 34, 35, 42, 46, 47, 14, 19, 81, 10, 11, 12, 24, 29, 37, 31, 40};
	const uint8_t PROGMEM Font::font_R[26] = {62, 56, 39, 77, 87, 4, 5, 6, 27, 28, 29, 12, 11, 10, 81, 19, 15, 55, 49, 50, 51, 76, 40, 37, 34, 31};
	const uint8_t PROGMEM Font::font_S[28] = {10, 11, 12, 29, 24, 23, 16, 17, 105, 103, 101, 100, 94, 86, 87, 51, 74, 43, 36, 35, 34, 32, 58, 63, 69, 67, 68, 62};
	const uint8_t PROGMEM Font::font_T[20] = {21, 20, 22, 24, 29, 12, 85, 83, 84, 28, 27, 6, 5, 4, 87, 77, 39, 56, 62, 61};
	const uint8_t PROGMEM Font::font_U[22] = {67, 68, 69, 63, 58, 62, 32, 34, 35, 36, 48, 55, 18, 81, 84, 72, 99, 20, 17, 104, 101, 43};
	const uint8_t PROGMEM Font::font_V[18] = {62, 70, 93, 94, 107, 106, 17, 21, 83, 82, 81, 80, 54, 57, 38, 40, 75, 50};
	const uint8_t PROGMEM Font::font_W[26] = {20, 17, 104, 65, 67, 72, 99, 101, 68, 71, 93, 91, 87, 76, 40, 37, 34, 31, 36, 43, 48, 55, 18, 81, 84, 35};
	const uint8_t PROGMEM Font::font_X[25] = {20, 16, 3, 90, 2, 1, 0, 87, 76, 40, 37, 34, 31, 65, 68, 71, 93, 91, 52, 88, 89, 8, 9, 10, 84};
	const uint8_t PROGMEM Font::font_Y[20] = {16, 20, 3, 90, 2, 1, 0, 87, 52, 88, 89, 8, 9, 10, 84, 77, 39, 56, 62, 61};
	const uint8_t PROGMEM Font::font_Z[25] = {22, 20, 24, 29, 12, 85, 84, 10, 9, 8, 89, 88, 52, 87, 91, 93, 71, 68, 65, 64, 63, 62, 58, 33, 31};
	const uint8_t PROGMEM Font::font_0[37] = {29, 62, 58, 32, 34, 35, 42, 46, 47, 14, 19, 81, 10, 11, 12, 24, 23, 16, 17, 105, 102, 98, 97, 73, 67, 68, 69, 63, 3, 90, 2, 1, 0, 87, 76, 40, 37};
	const uint8_t PROGMEM Font::font_1[19] = {29, 28, 27, 6, 5, 4, 87, 77, 39, 56, 62, 64, 58, 63, 33, 26, 25, 90, 106};
	const uint8_t PROGMEM Font::font_2[25] = {64, 63, 62, 58, 33, 31, 68, 71, 93, 91, 87, 103, 105, 17, 16, 23, 24, 29, 12, 11, 52, 88, 89, 8, 9};
	const uint8_t PROGMEM Font::font_3[30] = {87, 86, 51, 50, 49, 55, 15, 19, 81, 10, 11, 12, 29, 24, 23, 16, 105, 17, 74, 43, 42, 35, 34, 32, 58, 62, 63, 69, 68, 67};
	const uint8_t PROGMEM Font::font_4[19] = {87, 51, 50, 86, 94, 48, 20, 17, 104, 101, 84, 81, 18, 55, 43, 31, 36, 35, 100};
	const uint8_t PROGMEM Font::font_5[22] = {29, 12, 85, 84, 83, 28, 27, 6, 5, 4, 87, 51, 36, 35, 34, 32, 62, 42, 46, 48, 50, 58};
	const uint8_t PROGMEM Font::font_6[28] = {29, 24, 23, 16, 17, 105, 102, 98, 97, 73, 67, 68, 69, 63, 62, 58, 32, 34, 35, 36, 43, 74, 51, 87, 86, 95, 96, 12};
	const uint8_t PROGMEM Font::font_7[19] = {21, 20, 22, 24, 29, 12, 85, 84, 83, 82, 81, 80, 54, 50, 75, 40, 38, 57, 62};
	const uint8_t PROGMEM Font::font_8[37] = {62, 63, 69, 68, 67, 72, 96, 95, 94, 86, 87, 51, 74, 43, 36, 35, 34, 32, 58, 100, 101, 103, 105, 17, 16, 23, 24, 29, 12, 11, 10, 81, 19, 15, 55, 49, 50};
	const uint8_t PROGMEM Font::font_9[32] = {67, 68, 69, 63, 62, 58, 32, 34, 35, 36, 43, 48, 55, 18, 81, 10, 11, 12, 29, 24, 23, 16, 17, 103, 101, 100, 94, 86, 87, 51, 50, 105};

	const uint8_t * const Font::characters[36] = {Font::font_A, Font::font_B, Font::font_C, Font::font_D, Font::font_E, Font::font_F, Font::font_G, Font::font_H, Font::font_I, Font::font_J, Font::font_K, Font::font_L, Font::font_M, Font::font_N, Font::font_O, Font::font_P, Font::font_Q, Font::font_R, Font::font_S, Font::font_T, Font::font_U, Font::font_V, Font::font_W, Font::font_X, Font::font_Y, Font::font_Z, Font::font_0, Font::font_1, Font::font_2, Font::font_3, Font::font_4, Font::font_5, Font::font_6, Font::font_7, Font::font_8, Font::font_9};
	const size_t    Font::characters_size[36] = {Font::size_A, Font::size_B, Font::size_C, Font::size_D, Font::size_E, Font::size_F, Font::size_G, Font::size_H, Font::size_I, Font::size_J, Font::size_K, Font::size_L, Font::size_M, Font::size_N, Font::size_O, Font::size_P, Font::size_Q, Font::size_R, Font::size_S, Font::size_T, Font::size_U, Font::size_V, Font::size_W, Font::size_X, Font::size_Y, Font::size_Z, Font::size_0, Font::size_1, Font::size_2, Font::size_3, Font::size_4, Font::size_5, Font::size_6, Font::size_7, Font::size_8, Font::size_9};
}
