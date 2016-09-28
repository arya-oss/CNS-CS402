#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>

int main () {
    mpz_t prime, alpha, x, y;
    mpz_inits(prime, alpha, x, y, (mpz_ptr)NULL);
    
    mpz_set_ui(prime, 8431);
    mpz_set_ui(alpha, 2065);

    mpz_set_ui(x, 203);
    mpz_powm (y, alpha, x, prime);
    gmp_printf("%Zd\n", y);
    return 0;
}
