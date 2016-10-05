#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

/**
 * Title - Encryption Decryption using RSA Public Key Cryptography
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

void gcd(mpz_t a, mpz_t b, mpz_t r) {
    if (mpz_cmp_si(b, 0) == 0) {
        mpz_set(r, a);
        return;
    }
    mpz_mod(a, a, b);
    gcd(b, a, r);
}

int main () {
	mpz_t p,q,e,n,phi,d,r,x,y,t1,t2,random;
	mpz_inits(p,q,e,n,phi,r,d,x,y,t1,t2,random,(mpz_ptr)NULL);

  gmp_randstate_t rstate;
  gmp_randinit_mt(rstate);
  gmp_randseed_ui(rstate, time(NULL));

  mpz_urandomb(random, rstate, 20);
  mpz_nextprime(p, random);
  mpz_urandomb(random, rstate, 20);
  mpz_nextprime(q, random);

  while (mpz_cmp(p, q) == 0) {
    mpz_urandomb(random, rstate, 20);
    mpz_nextprime(q, random);
  }

	mpz_mul(n, p, q);
	mpz_sub_ui(t1, p, 1);
	mpz_sub_ui(t2, q, 1);
	mpz_mul(phi, t1, t2);
	unsigned long int i;
	unsigned long int en = mpz_get_ui(phi);
	for(i=2; i < en; i++) {
		mpz_set_ui(t1, i);
		mpz_set(t2, phi);
		gcd(t1, t2, r);
		if(mpz_cmp_ui(r, 1) == 0) {
			// printf("Gotcha !\n");
			mpz_set_ui(e, i);

			mpz_set(t1, e);
			mpz_set(t2, phi);
			ext_gcd(t1, t2, r, d, y);
			if (mpz_cmp_si(d, 0) < 0){
				mpz_add(d, d, phi);
			}
			break;
		}
	}
	gmp_printf("e=%Zd d=%Zd\n", e, d);
	unsigned char text[128], decipher[128];
  unsigned long int cipher[128];
  printf("Enter Text: ");

	fgets(text, 128, stdin);
	for (i=0; i < strlen(text); i++) {
		mpz_set_ui(p, text[i]);
		mpz_powm(q, p, e, n);
		en = mpz_get_ui(q);
		cipher[i] = en;
	}
	printf("\nCipher Text: ");
	for(i = 0; i < strlen(text); ++i) {
		printf("%lx ", cipher[i]);
	}

	for (i=0; i < strlen(text); i++) {
		mpz_set_ui(p, cipher[i]);
		mpz_powm(q, p, d, n);
		en = mpz_get_ui(q);
		decipher[i] = (unsigned char)en;
	}
	printf("\nDecipher Text: %s\n", decipher);
	return 0;
}
