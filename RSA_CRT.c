//gcc -o RSA_CRT RSA_CRT.c -lgmp && ./RSA_CRT


#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include "gmp.h"
#include <time.h>
#include <stdlib.h>

int *RSA_CRT_Gen_Key(mpz_t p, mpz_t q, mpz_t n, mpz_t dp, mpz_t dq, mpz_t ip, mpz_t k, mpz_t e)
{

	int booleen = FALSE;
	gmp_randstate_t state;
	unsigned long seed = time(NULL);

	mpz_t rand;
	mpz_t powMin;
	mpz_t powMax;
	mpz_t kSurDEux;
	mpz_t pMoinsUn;
	mpz_t qMoinsUn;

	mpz_init(rand);
	mpz_init(kSurDEux);
	mpz_init(powMin);
	mpz_init(powMax);
	mpz_init(pMoinsUn);
	mpz_init(qMoinsUn);
	
	gmp_randinit_default(state);
	gmp_randseed_ui(state, seed);

	mpz_fdiv_q_ui(kSurDEux, k, 2);

	mpz_ui_pow_ui(powMin, 2, (mpz_get_ui(kSurDEux)-1));
// Génération de p et q premiers random de taille k/2-bit
	int primeP = FALSE;
	int primeQ = FALSE;
	while (primeP == FALSE) {
		mpz_urandomb(p, state, mpz_get_ui(kSurDEux)-1);
		mpz_add(p, p, powMin);
		if (mpz_probab_prime_p(p, 5) == 2) {
			mpz_t gcd;
			mpz_init(gcd);
			mpz_gcd_ui(gcd, e, mpz_get_ui(p)-1);
			if (mpz_get_ui(gcd) == 1) {
				primeP = TRUE;
			}
		}
	}

	while (primeQ == FALSE)
	{
		mpz_urandomb(q, state, mpz_get_ui(kSurDEux) - 1);
		mpz_add(q, q, powMin);
		if (mpz_probab_prime_p(q, 5) == 2)
		{
			mpz_t gcd;
			mpz_init(gcd);
			mpz_gcd_ui(gcd, e, mpz_get_ui(q) - 1);
			if (mpz_get_ui(gcd) == 1)
			{
				primeQ = TRUE;
			}
		}
	}
//A supprimer
	//mpz_set_ui(p, 547);
	//mpz_set_ui(q, 797);
//

	mpz_mul(n, p, q);
	mpz_sub_ui(pMoinsUn, p, 1);
	mpz_invert(dp, e, pMoinsUn);
	mpz_sub_ui(qMoinsUn, q, 1);
	mpz_invert(dq, e, qMoinsUn);
	mpz_invert(ip, p, q);

	gmp_printf("p = %Zd \n", p);
	gmp_printf("q = %Zd \n", q);
	gmp_printf("n = %Zd \n", n);
	gmp_printf("dp = %Zd \n", dp);
	gmp_printf("dq = %Zd \n", dq);
	gmp_printf("ip = %Zd \n", ip);
}

void RSA_CRT_Encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
	mpz_set_ui(m, 123456);
	mpz_powm(c, m, e, n);
	gmp_printf("Chiffré = %Zd \n", c);
}

void RSA_CRT_Decrypt(mpz_t m, mpz_t c, mpz_t p, mpz_t q, mpz_t dp, mpz_t dq, mpz_t ip)
{
	mpz_t mp;
	mpz_t mq;
	mpz_t rop;
	mpz_t tmp;
	mpz_init(mp);
	mpz_init(mq);
	mpz_init(rop);
	mpz_init(tmp);

	mpz_powm(mp, c, dp, p);
	mpz_powm(mq, c, dq, q);
	gmp_printf("mp = %Zd \n", mp);
	gmp_printf("mq = %Zd \n", mq);
	mpz_sub(rop, mq, mp);
	mpz_mul(rop, rop, ip);
	mpz_mod(rop, rop, q);
	mpz_mul(rop, rop, p);
	mpz_add(m, rop, mp);
	gmp_printf("Déchiffré = %Zd \n", m);
}

int main(int argc, char *argv[]) {

	mpz_t p;
	mpz_t q;
	mpz_t n;
	mpz_t dp;
	mpz_t dq;
	mpz_t ip;
	mpz_t k;
	mpz_t e;
	mpz_t c;
	mpz_t m;
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(dp);
	mpz_init(dq);
	mpz_init(ip);
	mpz_init(k);
	mpz_init(e);
	mpz_init(c);
	mpz_init(m);
//A supprimer	
	mpz_set_ui(k, 20);
	mpz_set_ui(e, 11);

	RSA_CRT_Gen_Key(p, q, n, dp, dq, ip, k, e);
	RSA_CRT_Encrypt(c, m, e, n);
	RSA_CRT_Decrypt(m, c, p, q, dp, dq, ip);
	return 0;
}
