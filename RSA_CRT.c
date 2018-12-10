//gcc -o RSA_CRT RSA_CRT.c -lgmp && ./RSA_CRT 8 11


#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include "gmp.h"
#include <time.h>
#include <stdlib.h>

int *RSA_CRT_Gen_Key()
{

	int booleen = FALSE;
	gmp_randstate_t state;
	unsigned long seed = time(NULL);
	mpz_t k;
	mpz_t e;
	mpz_t n;
	mpz_t rand;
	mpz_t p;
	mpz_t q;
	mpz_t powMin;
	mpz_t powMax;
	mpz_t kSurDEux;
	mpz_t pFinal;
	mpz_t qFinal;

	mpz_init(k);
	mpz_init(e);
	mpz_init(n);
	mpz_init(rand);
	mpz_init(p);
	mpz_init(q);
	mpz_init(kSurDEux);
	mpz_init(powMin);
	mpz_init(powMax);
	mpz_init(pFinal);
	mpz_init(qFinal);

	gmp_randinit_default(state);
	gmp_randseed_ui(state, seed);
	while (booleen != TRUE)
	{
		mpz_set_ui(k, 99);
		mpz_fdiv_q_ui(kSurDEux, k, 2);
		gmp_printf(kSurDEux);

		mpz_ui_pow_ui(powMin, 2, (mpz_get_ui(kSurDEux)-1));
		mpz_ui_pow_ui(powMax, 2, (mpz_get_ui(kSurDEux)));
		booleen = TRUE;

	}
}

int main(int argc, char *argv[])
{

	RSA_CRT_Gen_Key();

	return 0;
}
