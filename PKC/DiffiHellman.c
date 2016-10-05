/**
 * Diffie - Hellman Key Exchange using Public Key Cryptography
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

    mpz_t x1,y1,x2,y2, key1, key2;
    mpz_inits(x1, y1, x2, y2, key1, key2, (mpz_ptr)NULL);
    /* User1 private key x1*/
    mpz_urandomb(x1, rstate, 10);
    /* User1 public key y1 */
    mpz_powm(y1, per, x1, prime);

    gmp_printf("\nUser1 Private Key (x,a,q): %Zd, %Zd, %Zd", x1, per, prime);
    gmp_printf("\nUser1 Private Key (y,a,q): %Zd, %Zd, %Zd", y1, per, prime);

    /* User2 private key x2 */
    mpz_urandomb(x2, rstate, 10);
    /* User2 public key y2 */
    mpz_powm(y2, per, x2, prime);

    gmp_printf("\nUser2 Private Key (x,a,q): %Zd, %Zd, %Zd", x2, per, prime);
    gmp_printf("\nUser2 Private Key (y,a,q): %Zd, %Zd, %Zd", y2, per, prime);

    /* calculate key using each other public, private key */
    mpz_powm(key1, y2, x1, prime);
    mpz_powm(key2, y1, x2, prime);


    gmp_printf("\nUser1 Key: %Zd \nUser2 Key: %Zd\n", key1, key2);

    gmp_randclear(rstate);
    mpz_clear(rand);

    return 0;
}
