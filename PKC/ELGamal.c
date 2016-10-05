/**
 * ELGAmal Public Key Cryptography
 *
 * Author: Rajmani Arya
 * Date: 2nd Oct 2016
 */

#include <stdio.h>
#include <gmp.h>
#include <time.h>

int main() {
    mpz_t prime, p2, h, rand, per, x, rem;
    mpz_inits(prime, p2, h, rand, per, x, rem, (mpz_ptr)NULL);

    unsigned long seed;
    seed = time(NULL);

    gmp_randstate_t rstate;
    gmp_randinit_mt(rstate);
    gmp_randseed_ui(rstate, seed);
    /* Find Safe Prime
     * safe_prime = 2 * prime + 1
     */
    while (1) {
      mpz_urandomb(rand, rstate, 20);
      /* generate a random prime number of range 20 bits */
      mpz_nextprime (p2, rand);
      mpz_mul_ui(prime, p2, 2);
      mpz_add_ui(prime, prime, 1);
      if (mpz_probab_prime_p(prime, 2) == 2) {
        break;
      }
    }

    mpz_sub_ui(h, prime, 1);
    mpz_set(rem, h);
    mpz_fdiv_q_ui(h, h, 2);

    /* Find a primitive root g^((p-1)/2) = -1 mod p */
    while(1) {
        mpz_urandomm(per, rstate, prime);
        mpz_powm(x, per, h, prime);
        if (mpz_cmp(x, rem) == 0) {
            break;
        }
    }

    /* Now generate private and public key */

    mpz_t x1,y1, k, c1, c2, K;
    mpz_inits(x1, y1, c1, c2, k, K, (mpz_ptr)NULL);
    /* User private key x1*/
    mpz_urandomb(x1, rstate, 10);
    /* User public key y1 */
    mpz_powm(y1, per, x1, prime);

    gmp_printf("\nUser Private Key (x,a, q): %Zd, %Zd, %Zd", x1, per, prime);
    gmp_printf("\nUser Private Key (y,a, q): %Zd, %Zd, %Zd", y1, per, prime);

    mpz_t message;
    mpz_init(message);
    mpz_urandomb(message, rstate, 12); // max 12bit message

    gmp_printf("\n\nMesage: %Zd\n", message);

    /* Encryption */
    /* Generate k */
    mpz_urandomb(k, rstate, 12);
    mpz_powm(K, y1, k, prime);
    mpz_powm(c1, per, k, prime);
    mpz_mul(c2, K, message);
    mpz_mod(c2, c2, prime);

    gmp_printf("\nCipher: %Zd, %Zd\n", c1, c2);

    /* Decryption */

    mpz_powm(K, c1, x1, prime);
    mpz_t r,y;
    mpz_inits(r, y, (mpz_ptr)NULL);

    mpz_gcdext(r, x, y, K, prime);
    if (mpz_cmp_si(x, 0) < 0) {
    	mpz_add(x, x, prime);
    }

    mpz_mul(K, x, c2);

    mpz_mod(K, K, prime);

    gmp_printf("\nDecipher: %Zd\n", K);

    gmp_randclear(rstate);
    mpz_clear(rand);

    return 0;
}
