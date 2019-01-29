//gcc -o RSA_CRT RSA_CRT.c -lgmp && ./RSA_CRT


#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include<string.h>
#include "gmp.h"
#include <time.h>
#include <stdlib.h>
#include <memory.h>

void formatRsaPublicKey(mpz_t n, mpz_t e) {

	char* header = "3082010A0282010100";
	char* footer = "0203";
	char *strNdec = mpz_get_str(NULL, 10, n);
	char* strN = mpz_get_str(NULL, 16, n);
	char *strE = mpz_get_str(NULL, 16, e);

	printf("%ld \n", strlen(strNdec));
	printf("%ld \n", strlen(strN));
	

	printf("%s \n", strNdec);
	printf("%s \n", strN);
	printf("\n");

	//printf("%s", strN);

	//char *rsaPubKey = malloc(sizeof(char) * 19);
	//rsaPubKey = "aaaaa";
	//strcat(rsaPubKey, "3082010A0282010100");
	//Cat n



	//strcat(rsaPubKey, "0203");

//Cat e
	//strcat(rsaPubKey, mpz_get_str(rsaPubKey, 10, e));

	//Malloc
	//
	//strcat(rsaPubKey, mpz_get_str(rsaPubKey, 10, n));

//Encode b64

	//printf("-----BEGIN RSA PUBLIC KEY-----\n");
	//printf("%s", header);
	//printf("%s \n", strN);
	//printf("%s", footer);
	//printf("%s \n", strE);
	//printf("-----END RSA PUBLIC KEY-----\n");

	//free(rsaPubKey);
}

void RSA_CRT_Gen_Key(mpz_t p, mpz_t q, mpz_t n, mpz_t dp, mpz_t dq, mpz_t ip, mpz_t k, mpz_t e) {

	int booleen = FALSE;
	gmp_randstate_t state;
	unsigned long seed = time(NULL);

	mpz_t rand;
	mpz_t rop;
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
	mpz_init(rop);
	
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
		if (mpz_mod_ui(rop, p, 2) != 0) {
			if (mpz_probab_prime_p(p, 15) == 1) {
				mpz_t gcd;
				mpz_init(gcd);
				mpz_gcd_ui(gcd, e, mpz_get_ui(p)-1);
				if (mpz_get_ui(gcd) == 1) {
					primeP = TRUE;
				}
			}
		}
	}

	while (primeQ == FALSE) {
		mpz_urandomb(q, state, mpz_get_ui(kSurDEux) - 1);
		mpz_add(q, q, powMin);
		if (mpz_mod_ui(rop, q, 2) != 0) {
			if (mpz_probab_prime_p(q, 15) == 1) {
				mpz_t gcd;
				mpz_init(gcd);
				mpz_gcd_ui(gcd, e, mpz_get_ui(q) - 1);
				if (mpz_get_ui(gcd) == 1) {
					primeQ = TRUE;
				}
			}
		}
	}

	mpz_mul(n, p, q);
	mpz_sub_ui(pMoinsUn, p, 1);
	mpz_invert(dp, e, pMoinsUn);
	mpz_sub_ui(qMoinsUn, q, 1);
	mpz_invert(dq, e, qMoinsUn);
	mpz_invert(ip, p, q);

	formatRsaPublicKey(n, e);
	
	
}


void RSA_Encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
	mpz_set_ui(m, 123456);
	mpz_powm(c, m, e, n);
	gmp_printf("Chiffré = %Zd \n", c);
}

void RSA_Decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n, mpz_t e, mpz_t p, mpz_t q) {
	mpz_t phi;
	mpz_t pMoinsUn;
	mpz_t qMoinsUn;
	mpz_t moinsUn;
	mpz_init(phi);
	mpz_init(pMoinsUn);
	mpz_init(qMoinsUn);
	mpz_init(moinsUn);
	mpz_set_si(moinsUn, -1);

	mpz_sub_ui(pMoinsUn, p, 1);
	mpz_sub_ui(qMoinsUn, q, 1);
	mpz_mul(phi, pMoinsUn, qMoinsUn);
	gmp_printf("phi = %Zd\n", phi);
	mpz_invert(d, e, phi);
	gmp_printf("d = %Zd\n", d);

	mpz_powm(m, c, d, n);
	gmp_printf("Déchiffré no Crt : %Zd \n", m);
}

void RSA_CRT_Decrypt(mpz_t m, mpz_t c, mpz_t p, mpz_t q, mpz_t dp, mpz_t dq, mpz_t ip) {
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

void RSA_Signature(mpz_t sign, mpz_t m, mpz_t d, mpz_t n) {
	mpz_powm(sign, m, d, n);
	gmp_printf("Sign = %Zd \n", sign);
}

void RSA_Verif(mpz_t m, mpz_t sign, mpz_t e, mpz_t n) {
	mpz_t verif;
	mpz_init(verif);
	mpz_powm(verif, sign, e, n);
	if (mpz_cmp(verif, m) == 0) {
		gmp_printf("Verifié \n");
	}
	else {
		gmp_printf("Message pas vérifié \n");
	}
}

int main(int argc, char *argv[]) {

	mpz_t p;
	mpz_t q;
	mpz_t n;
	mpz_t dp;
	mpz_t dq;
	mpz_t d;
	mpz_t ip;
	mpz_t k;
	mpz_t e;
	mpz_t c;
	mpz_t m;
	mpz_t sign;
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(dp);
	mpz_init(dq);
	mpz_init(d);
	mpz_init(ip);
	mpz_init(k);
	mpz_init(e);
	mpz_init(c);
	mpz_init(m);
	mpz_init(sign);
	//A supprimer	
	mpz_set_ui(k, 256);
	mpz_set_ui(e, 11);

	RSA_CRT_Gen_Key(p, q, n, dp, dq, ip, k, e);
	//RSA_Encrypt(c, m, e, n);
	//RSA_Decrypt(m, c, d, n, e, p, q);
	//RSA_CRT_Decrypt(m, c, p, q, dp, dq, ip);
	//RSA_Signature(sign, m, d, n);
	//RSA_Verif(m, sign, e, n);
	return 0;
}
