#include <stdio.h>
#include <gmp.h>
#include <string.h>

/**
 * Title - Find GCD of two large integers and their coefficient
 *         Extended Euclidean algorithm
 *
 * Author: Rajmani Arya
 * Date: 3rd Aug 2016
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

int main() {
    mpz_t a,b,r,t1,t2,x,y;
    mpz_inits(a, b, r, t1, t2, x, y, (mpz_ptr)0);
    printf("Enter two large integers \n");
    gmp_scanf("%Zd %Zd", a, b);
    mpz_set(t1, a);
    mpz_set(t2, b);

    ext_gcd(t1, t2, r, x, y);
    gmp_printf("%Zd * %Zd + %Zd * %Zd = %Zd\n", a, x, b, y, r);
    return 0;
}
