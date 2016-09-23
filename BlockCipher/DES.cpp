/**
 * Title: Data Encryption Standard Algorithm
 * 
 * Author: Rajmani Arya
 * Date: 21st Sep 2016
 *
 * Modified version of @see https://github.com/tarequeh/DES
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define ISSET(x, p) (x & (0x80 >> p))
#define SET(x, p) (x = x | (0x80 >> p))
#define RESET(x, p) ( x = x & ~(0x80 >> p))
#define TOGGLE(x, p) (x = x ^ (0x80 >> p))

#define ROUNDS 16
#define ENCRYPTION 0
#define DECRYPTION 1

// #define DEBUG

typedef unsigned char uchar_t;

int init_perm_key[] = {
	57,49,41,33,25,17,9,
	1,58,50,42,34,26,18,
	10,2,59,51,43,35,27,
	19,11,3,60,52,44,36,
	63,55,47,39,31,23,15,
	7,62,54,46,38,30,22,
	14,6,61,53,45,37,29,
	21,13,5,28,20,12,4
};

int sub_perm_key[] = {
	14,17,11,24,1,5,
	3,28,15,6,21,10,
	23,19,12,4,26,8,
	16,7,27,20,13,2,
	41,52,31,37,47,55,
	30,40,51,45,33,48,
	44,49,39,56,34,53,
	46,42,50,36,29,32
};

int key_shift[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

int init_perm[] = {
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17, 9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7
};

int text_expand [] = {
	32, 1, 2, 3, 4, 5,
	4, 5, 6, 7, 8, 9,
	8, 9,10,11,12,13,
	12,13,14,15,16,17,
	16,17,18,19,20,21,
	20,21,22,23,24,25,
	24,25,26,27,28,29,
	28,29,30,31,32, 1
};

int sBox[8][4][16] = {
  {
	{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
	{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
	{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
	{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
  },
  {
	{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
	{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
	{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
 	{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
  },
  {
	{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
	{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
	{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
	{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
  },
  {
	{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
	{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
	{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
	{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
  },
  {
	{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
	{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
	{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
	{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
  },
  {
	{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
	{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
	{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
	{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
  },
  {
	{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
	{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
	{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
	{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
  },
  {
	{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
	{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
	{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
	{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
  }
};

int r_perm [] = {
	16,7,20,21,
	29,12,28,17,
	1,15,23,26,
	5,18,31,10,
	2,8,24,14,
	32,27,3,9,
	19,13,30,6,
	22,11,4,25
};

int inv_perm[] = {
	40,8,48,16,56,24,64,32,
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33, 1,41,9,49,17,57,25
};

uchar_t key_sets[17][8];

void print_array_hex(uchar_t * arr, unsigned int size) {
	for (unsigned i=0; i<size; i++) {
		if (arr[i] < 16u) {
			printf("0%x", arr[i]);
		} else {
			printf("%x", arr[i]);
		}
	}
	printf("\n");
}

void genKeys (uchar_t key[])  {
	unsigned int x,y;
	memset (key_sets, 0, sizeof(key_sets));
	uchar_t c[4], d[4];

	for (unsigned int i = 0; i < 56; ++i) {
		x = (init_perm_key[i]-1)/8;
		y = (init_perm_key[i]-1)%8;
		if (ISSET(key[x], y)) {
			x = i/8;
			y = i%8;
			SET(key_sets[0][x], y);
		}
	}

	/* copy left 28 bits to c0 */
	for (unsigned int i=0; i<3; i++) {
		c[i] = key_sets[0][i]; 
	}
	c[3] = key_sets[0][3] & 0xF0;
	/* copy right 28 bits to d0 */
	for (unsigned int i=0; i<3; i++) {
		d[i] = (key_sets[0][i+3] & 0x0F) << 4;
		d[i] |= (key_sets[0][i+4] & 0xF0) >> 4;
	}
	d[3] = (key_sets[0][6] & 0x0F) << 4;

	int shift; uchar_t shift_byte, first, second, third, fourth;

	for (int i=1; i <= ROUNDS; i++) {
		shift = key_shift[i-1];
		if (shift == 1) {
			shift_byte = 0x80; // 10000000
		} else {
			shift_byte = 0xC0; // 11000000
		}
		
		/* Shift operation on left half of key */
		first = shift_byte & c[0];
		second = shift_byte & c[1];
		third = shift_byte & c[2];
		fourth = shift_byte & c[3];

		c[0] <<= shift;
		c[0] |= (second >> (8-shift));

		c[1] <<= shift;
		c[1] |= (third >> (8-shift));

		c[2] <<= shift;
		c[2] |= (fourth >> (8-shift));

		c[3] <<= shift;
		c[3] |= (first >> (4-shift));

		/* Shift operation on right half of key */
		first = shift_byte & d[0];
		second = shift_byte & d[1];
		third = shift_byte & d[2];
		fourth = shift_byte & d[3];

		d[0] <<= shift;
		d[0] |= (second >> (8-shift));

		d[1] <<= shift;
		d[1] |= (third >> (8-shift));

		d[2] <<= shift;
		d[2] |= (fourth >> (8-shift));

		d[3] <<= shift;
		d[3] |= (first >> (4-shift));

#ifdef DEBUG
		printf("C%d: ", i);
		print_array_hex(c, 4);
		printf("D%d: ", i);
		print_array_hex(d, 4);
#endif

		for (unsigned int j=0; j < 48; j++) {
			if (sub_perm_key[j] <= 28) // Take from left side
			{
				x = (sub_perm_key[j]-1)/8;
				y = (sub_perm_key[j]-1)%8;
				
				if (ISSET(c[x], y)) {
					SET(key_sets[i][j/8], j%8);
				}
			} else // take it from right side
			{
				x = (sub_perm_key[j]-29)/8;
				y = (sub_perm_key[j]-29)%8;
				
				if (ISSET(d[x], y)) {
					SET(key_sets[i][j/8], j%8);
				}
			}
		}
	}
}

void DesCipher (uchar_t msg[8], uchar_t cipher[8], bool MODE) {
	uchar_t L[4], R[4]; int x, y, k; uchar_t row, col;
	uchar_t X[4], res[4], exp[6];
	uchar_t T[8], _tmp[8];
	memset(T, 0, sizeof(T));

	/* Apply initial permutation on text */
	for (unsigned int i=0; i < 64; i++) {
		x = (init_perm[i]-1)/8;
		y = (init_perm[i]-1)%8;
		if (ISSET(msg[x], y)) {
			x = i/8;
			y = i%8;
			SET(T[x], y);
		}
	}

	for (unsigned int i=0; i<4; i++) {
		L[i] = T[i];
		R[i] = T[4+i];
	}

	for (int i = 0; i < 16; i++) {
		if (MODE == DECRYPTION) {
			k = 16-i;
		} else {
			k = i+1;
		}
		memset(exp, 0, sizeof(exp));
		for (int j=0; j < 48; j++) {
			x = (text_expand[j]-1)/8;
			y = (text_expand[j]-1)%8;
			if (ISSET(R[x], y)) {
				x = j/8;
				y = j%8;
				SET(exp[x], y);
			}
		}

		for (int j=0; j<6; j++) {
			exp[j] ^= key_sets[k][j];
		}
		memset(res, 0, sizeof(res));

		// Byte 1
		row = 0;
		row |= ((exp[0] & 0x80) >> 6);
		row |= ((exp[0] & 0x04) >> 2);

		col = 0;
		col |= ((exp[0] & 0x78) >> 3);

		res[0] |= ((uchar_t)sBox[0][row][col] << 4);

		row = 0;
		row |= (exp[0] & 0x02);
		row |= ((exp[1] & 0x10) >> 4);

		col = 0;
		col |= ((exp[0] & 0x01) << 3);
		col |= ((exp[1] & 0xE0) >> 5);

		res[0] |= (uchar_t)sBox[1][row][col];

		// Byte 2
		row = 0;
		row |= ((exp[1] & 0x08) >> 2);
		row |= ((exp[2] & 0x40) >> 6);

		col = 0;
		col |= ((exp[1] & 0x07) << 1);
		col |= ((exp[2] & 0x80) >> 7);

		res[1] |= ((uchar_t)sBox[2][row][col] << 4);

		row = 0;
		row |= ((exp[2] & 0x20) >> 4);
		row |= (exp[2] & 0x01);

		col = 0;
		col |= ((exp[2] & 0x1E) >> 1);

		res[1] |= (uchar_t)sBox[3][row][col];

		// Byte 3
		row = 0;
		row |= ((exp[3] & 0x80) >> 6);
		row |= ((exp[3] & 0x04) >> 2);

		col = 0;
		col |= ((exp[3] & 0x78) >> 3);

		res[2] |= ((uchar_t)sBox[4][row][col] << 4);

		row = 0;
		row |= (exp[3] & 0x02);
		row |= ((exp[4] & 0x10) >> 4);

		col = 0;
		col |= ((exp[3] & 0x01) << 3);
		col |= ((exp[4] & 0xE0) >> 5);

		res[2] |= (uchar_t)sBox[5][row][col];

		// Byte 4
		row = 0;
		row |= ((exp[4] & 0x08) >> 2);
		row |= ((exp[5] & 0x40) >> 6);

		col = 0;
		col |= ((exp[4] & 0x07) << 1);
		col |= ((exp[5] & 0x80) >> 7);

		res[3] |= ((uchar_t)sBox[6][row][col] << 4);

		row = 0;
		row |= ((exp[5] & 0x20) >> 4);
		row |= (exp[5] & 0x01);

		col = 0;
		col |= ((exp[5] & 0x1E) >> 1);

		res[3] |= (uchar_t)sBox[7][row][col];
		memset(X, 0, sizeof(X));
		
		for (int j=0; j < 32; j++) {
			x = (r_perm[j]-1)/8;
			y = (r_perm[j]-1)%8;
			if (ISSET(res[x], y)) {
				x = j/8;
				y = j%8;
				SET(X[x], y);
			}
		}

		for (int j=0; j<4; j++) {
			X[j] ^= L[j];
			L[j] = R[j];
		}

		for (int j=0; j<4; j++) {
			R[j] = X[j];
		}

#ifdef DEBUG
		printf("L%d ", i);
		print_array_hex(L, 4);
		printf("R%d ", i);
		print_array_hex(R, 4);
#endif

	}

	for (int i=0; i<4; i++) {
		_tmp[i] = R[i];
		_tmp[4+i] = L[i];
	}
	
	/* Apply inverse permutation on R16L16 */
	for (int i=0; i < 64; i++) {
		x = (inv_perm[i]-1)/8;
		y = (inv_perm[i]-1)%8;
		if (ISSET(_tmp[x], y)) {
			x = i/8;
			y = i%8;
			SET(cipher[x], y);
		}
	}
}

int main () {
    uchar_t key[8]= { 0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59};
    genKeys(key);

#ifdef DEBUG
    for (int i=0; i<17; i++) {
    	printf("K%d: ", i);
		print_array_hex(key_sets[i], 6);
    }
#endif
    
    uchar_t message[8] = {0x02, 0x46, 0x8a, 0xce, 0xec, 0xa8, 0x64, 0x20};
    uchar_t cipher[8];

    memset(cipher, 0, sizeof(cipher));
    DesCipher(message, cipher, ENCRYPTION);
    cout << "cipher: ";
    print_array_hex(cipher, 8);

    uchar_t decipher[8];
    memset(decipher, 0, sizeof(decipher));
    DesCipher(cipher, decipher, DECRYPTION);
    cout << "decipher: ";
    print_array_hex(decipher, 8);

    return 0;
}

// Plaintext: 	02468aceeca86420
// Key:			0f1571c947d9e859
// Ciphertext:	da02ce3a89ecac3b