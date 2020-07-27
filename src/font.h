#ifndef FONT_H
#define FONT_H
#include <stdlib.h>
#include <avr/pgmspace.h>
typedef unsigned char uint8_t;

namespace hpasteur {
  class Font
  {
  public:

	static const size_t size_A = 22;
	static const size_t size_B = 28;
	static const size_t size_C = 19;
	static const size_t size_D = 24;
	static const size_t size_E = 22;
	static const size_t size_F = 18;
	static const size_t size_G = 26;
	static const size_t size_H = 25;
	static const size_t size_I = 19;
	static const size_t size_J = 20;
	static const size_t size_K = 23;
	static const size_t size_L = 15;
	static const size_t size_M = 30;
	static const size_t size_N = 28;
	static const size_t size_O = 28;
	static const size_t size_P = 21;
	static const size_t size_Q = 31;
	static const size_t size_R = 26;
	static const size_t size_S = 28;
	static const size_t size_T = 20;
	static const size_t size_U = 22;
	static const size_t size_V = 18;
	static const size_t size_W = 26;
	static const size_t size_X = 25;
	static const size_t size_Y = 20;
	static const size_t size_Z = 25;
	static const size_t size_0 = 37;
	static const size_t size_1 = 19;
	static const size_t size_2 = 25;
	static const size_t size_3 = 30;
	static const size_t size_4 = 19;
	static const size_t size_5 = 22;
	static const size_t size_6 = 28;
	static const size_t size_7 = 19;
	static const size_t size_8 = 37;
	static const size_t size_9 = 32;

	static const uint8_t font_A[22];
	static const uint8_t font_B[28];
	static const uint8_t font_C[19];
	static const uint8_t font_D[24];
	static const uint8_t font_E[22];
	static const uint8_t font_F[18];
	static const uint8_t font_G[26];
	static const uint8_t font_H[25];
	static const uint8_t font_I[19];
	static const uint8_t font_J[20];
	static const uint8_t font_K[23];
	static const uint8_t font_L[15];
	static const uint8_t font_M[30];
	static const uint8_t font_N[28];
	static const uint8_t font_O[28];
	static const uint8_t font_P[21];
	static const uint8_t font_Q[31];
	static const uint8_t font_R[26];
	static const uint8_t font_S[28];
	static const uint8_t font_T[20];
	static const uint8_t font_U[22];
	static const uint8_t font_V[18];
	static const uint8_t font_W[26];
	static const uint8_t font_X[25];
	static const uint8_t font_Y[20];
	static const uint8_t font_Z[25];
	static const uint8_t font_0[37];
	static const uint8_t font_1[19];
	static const uint8_t font_2[25];
	static const uint8_t font_3[30];
	static const uint8_t font_4[19];
	static const uint8_t font_5[22];
	static const uint8_t font_6[28];
	static const uint8_t font_7[19];
	static const uint8_t font_8[37];
	static const uint8_t font_9[32];

	static const uint8_t * const characters[36];
	static const size_t    characters_size[36];

	uint8_t maxIndex = 0;
    
  public:

    /* Constructor */
    Font(/* args */) {
      maxIndex = ('Z' - 'A' + 1) + ('9' - '0' + 1) + 1; // All the capital letters + all the digits
    }

    ~Font() {}

    bool getCharacterByIndex(size_t index, const uint8_t * &char_ptr, size_t & char_size) {
      if (index > maxIndex) return false;
      char_ptr = characters[index];
      char_size = characters_size[index];
      return true;
    }

    bool getCharacterByChar(char chr, const uint8_t * &char_ptr, size_t & char_size) {

      if (chr >= 'A' && chr <= 'Z')  {
        getCharacterByIndex(chr - 'A', char_ptr, char_size);
      }
      else if (chr >= '0' && chr <= '9')  {
        //getCharacterByIndex(chr - '0' + 'Z' + 1, char_ptr, char_size); // NON ? exemple pour chr = '0' => 123
        getCharacterByIndex(chr - '0' + ('Z' - 'A' + 1), char_ptr, char_size); // les 26 lettres en premier puis les chiffres
      }
	  else if(chr == ' ')
	  {
		  getCharacterByIndex(36, char_ptr, char_size);
	  }
	  
      else return false; // char is not supported by this font

      return true;
    }
  };
}

#endif