#include <stdio.h>
#include <gmp.h>

/**
 * Title: Find factorial of large number
 * 
 * @author Rajmani Arya
 * Date - 10th Aug 2016
 */

/**
 * Computes factorial of num and result stored in r
 * 
 * @param num   Number whose factorial is to be calculated
 * @param r     Result to be stored
 */
void factorial (mpz_t num, mpz_t r) {
    if(mpz_cmp_si(num,1) == 0) {
        return;
    }
    mpz_mul(r, r, num);
    mpz_sub_ui(num, num, 1);
    factorial(num, r);
}

int main(int argc, char * argv[]) {
    mpz_t num,n,f;
    mpz_inits(num, n, f, (mpz_ptr)NULL);
    printf("Enter Number: ");
    gmp_scanf("%Zd", num);
    mpz_set(n, num);
    mpz_set_ui(f, 1);
    factorial(n, f);

    gmp_printf("%Zd factorial => %Zd\n", num, f);
    return 0;
}