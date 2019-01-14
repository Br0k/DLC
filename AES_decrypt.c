//gcc -o AES_decrypt AES_decrypt.c -lgmp && ./AES_decrypt 8 11

#include <stdio.h>
#include "gmp.h"
#include <time.h>
#include <stdlib.h>

int* AES_decrypt() {

    int booleen = FALSE;
    gmp_randstate_t state;
    unsigned long seed = time(NULL);

    mpz_t k;
    mpz_t e;
    mpz_t n;

    mpz_init(k);
    mpz_init(e);
    mpz_init(n);
    //mpz_set_str(k, argv[1], 10);
    //mpz_set_str(e, argv[2], 10);

    gmp_randinit_default (state);
    gmp_randseed_ui(state, seed);
    /*
    while (booleen != TRUE) {
        mpz_ui_pow_ui(powMin, 2, (mpz_get_ui(k)-1));
        mpz_ui_pow_ui(powMax, 2, (mpz_get_ui(k)));

        //Changer le nextPrime
        mpz_urandomb(p, state, (mpz_get_ui(k)-1));
        mpz_nextprime(pFinal, p);
        mpz_urandomb(q, state, (mpz_get_ui(k)-1));
        mpz_nextprime(qFinal, q);
        mpz_mul(n, pFinal, qFinal);
    }
    */

}

int main(int argc, char *argv[]) {



    return 0;
}
