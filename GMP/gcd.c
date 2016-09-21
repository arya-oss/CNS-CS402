#include <stdio.h>
#include <gmp.h>
#include <string.h>

/**
 * Title - Find GCD of two large integers
 *
 * Author: Rajmani Arya
 * Date: 3rd Aug 2016
 */

/**
 * Calculates GCD of two number a,b and set gcd in r
 * r must be initialized
 * @param a     First Number
 * @param b     Second Number
 * @param r     Resultant GCD
 */
void gcd(mpz_t a, mpz_t b, mpz_t r) {
    if(mpz_cmp_si(b,0) == 0) {
        mpz_set(r, a);
        return;
    }
    mpz_mod(a, a, b);
    gcd(b, a, r);
}

int main() {
    mpz_t a,b,r, t1, t2;
    mpz_inits(a, b, r, t1, t2, (mpz_ptr)0);
    printf("Enter two large integers \n");
    gmp_scanf("%Zd %Zd", a, b);
    mpz_set(t1, a);
    mpz_set(t2, b);

    gcd(t1, t2, r);
    gmp_printf("%Zd mod %Zd = %Zd\n", a, b, r);
    return 0;
}
