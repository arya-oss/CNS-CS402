#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gmp.h>

/**
 * Title - Chinese Remainder Theorem
 *
 * Author: Rajmani Arya
 * Date: 21st Sep 2016
 */

/**
 * Calculates GCD of two number a,b and set gcd in r
 * a's coefficient = x
 * b's coefficient = y
 * x,y,r must be initialized
 * @param a     First Number
 * @param b     Second Number
 * @param x     First number coefficient
 * @param y     Second Number coefficient
 * @param r     Resultant GCD
 */
void ext_gcd(mpz_t a, mpz_t b, mpz_t r, mpz_t x, mpz_t y) {
    if (mpz_cmp_si(a, 0) == 0) {
        mpz_set_si(x, 0);
        mpz_set_si(y, 1);
        mpz_set(r, b);
        return;
    }
    mpz_t t, x1, y1;
    mpz_inits(t, x1, y1, (mpz_ptr)NULL);
    mpz_mod(t, b, a);
    ext_gcd(t, a, r, x1, y1);
    mpz_fdiv_q(x, b, a);
    mpz_mul(x, x, x1);
    mpz_sub(x, x, y1);
    mpz_mul_si(x, x, -1);
    mpz_set(y, x1);
}

/**
 * @see https://en.wikipedia.org/Chinese_Remainder_Theorem
 *
 */

int main () {
	mpz_t a[10], m[10], mt, final, c, t1, t2, x, y;
	mpz_inits(t1, t2, x, y, mt, c, final, (mpz_ptr)NULL);
	printf("Enter no. of pairs \n");
	int i, pairs;
	scanf("%d", &pairs);
	printf("Enter (A,M) Pairs \n");
	mpz_set_si(mt, 1);

	for (i=0; i<pairs; i++) {
		mpz_inits(a[i], m[i], (mpz_ptr)NULL);
		gmp_scanf("%Zd %Zd", a[i], m[i]);
		mpz_mul(mt, mt, m[i]);
	}
	mpz_set_si(final, 0);
	for (i=0; i<pairs; i++) {
		mpz_fdiv_q(t1, mt, m[i]);
		mpz_set(t2, m[i]);
		ext_gcd(t1, t2, x, c, y);
		if (mpz_cmp_si(c, 0) < 0) {
			mpz_add(c, m[i], c);
		}
		mpz_mul(c, c, a[i]);
		mpz_fdiv_q(t1, mt, m[i]);
		mpz_mul(c, c, t1);
		mpz_add(final, final, c);
	}

	mpz_mod(final, final, mt);

	gmp_printf("Number = %Zd\n", final);

	return 0;
}
