//gcc -o RSA_CRT RSA_CRT.c -lgmp -lcrypto && ./RSA_CRT

#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include<string.h>
#include "gmp.h"
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>
#include"Base64Encode.c"

void formatRsaPrivateKey(mpz_t d, mpz_t p, mpz_t q, mpz_t dp, mpz_t dq, mpz_t ip) {

	printf("DEDANS!");
	FILE *f = fopen("privateKey.txt", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	fprintf(f, "Private key: \n");

	fprintf(f, "d: ");
	mpz_out_str(f, 10, d );
	fprintf(f, "\n");
	fprintf(f, "dp ");
	mpz_out_str(f, 10, p);
	fprintf(f, "\n");
	fprintf(f, "q: ");
	mpz_out_str(f, 10, q);
	fprintf(f, "\n");
	fprintf(f, "dp: ");
	mpz_out_str(f, 10, dp);
	fprintf(f, "\n");
	fprintf(f, "dq: ");
	mpz_out_str(f, 10, dq);
	fprintf(f, "\n");
	fprintf(f, "ip: ");
	mpz_out_str(f, 10, ip);

	fclose(f);
}

void formatRsaPublicKey(mpz_t n, mpz_t e) {
	mpz_t hex;
	mpz_t mpzHeader;
	mpz_t mpzFooter;
	mpz_t header;
	mpz_inits(mpzHeader, mpzFooter, hex, header, NULL);

	unsigned char head[9] = {48, 130, 1, 10, 2, 130, 1, 1, 0};
	int footer[2];
	footer[0] = 2;
	footer[1] = 3;
	char *strN = mpz_get_str(NULL, 10, n);
	//gmp_printf("%Zd \n", n);
	/*for (int i = 0; i < strlen(strN); i++) {
		printf("%c", strN[i]);
	}*/

	char *strE = mpz_get_str(NULL, 10, e);

	int size = 9 + strlen(strN) + 2 + strlen(strE);
	unsigned char *key;

	key = malloc (sizeof(int) * size);
	for (int i = 0 ; i < size; i++) {
		key[i] = '\0';
	}

	/*
	strcat(key, "481301102130110");
	strcat(key, strN);
	strcat(key, "0203");
	strcat(key, strE);
	printf("%s ", key);*/

	for (int i = 0; i < size; i++) {
		if (i <= strlen(head)) {
			key[i] = head[i];
		}
		else {
			key[i] = '\0';
		}
	}
	
	int headSize = strlen(head) +1;

	for (int i = 0; i < strlen(strN); i++) {
		unsigned char a = strN[i];
		key[headSize + i] = a;
	}

	int nSize = headSize + strlen(strN);


	for (int i = 0; i < 2; i++){
		unsigned char a = footer[i];
		key[nSize + i] =  a;
	}

	int  footSize = nSize + 2;

	for (int i =0; i < strlen(strE); i++) {
		unsigned char a = strE[i];
		key[footSize + i] = a;
		//printf("%d ", footSize + i);
	}
	
	key[size] = '\0';

	size_t *output;
	//A MODIFIER
	char *b64text = base64_encode(key, 632, &output);
	
	//Base64Encode(key, size, &b64text);
	//printf("%ld ", strlen(key));


	FILE *f = fopen("publicKey.txt", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	fprintf(f, "Public key: \n");
	fprintf(f, "-----BEGIN RSA PUBLIC KEY-----\n");
	for (int i = 0; i < strlen(b64text); i++) {
		int tmp = strlen(b64text) / 4;
		int tmp2 = strlen(b64text) / 2;
		int tmp3 = (strlen(b64text) / 4 * 3);
		if (i == tmp || i == tmp2 || i == tmp3) {
			fprintf(f, "\n");
		}
		fprintf(f, "%c", b64text[i]);
	}
	fprintf(f, "\n-----END RSA PUBLIC KEY-----\n");

	fclose(f);
}

void RSA_CRT_Gen_Key(mpz_t p, mpz_t q, mpz_t n, mpz_t dp, mpz_t dq, mpz_t ip, mpz_t k, mpz_t e, mpz_t d) {

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
	mpz_t phi;

	mpz_init(rand);
	mpz_init(kSurDEux);
	mpz_init(powMin);
	mpz_init(powMax);
	mpz_init(pMoinsUn);
	mpz_init(qMoinsUn);
	mpz_init(rop);
	mpz_init(phi);
	
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

	mpz_sub_ui(pMoinsUn, p, 1);
	mpz_sub_ui(qMoinsUn, q, 1);
	mpz_mul(phi, pMoinsUn, qMoinsUn);
	mpz_invert(d, e, phi);

	mpz_mul(n, p, q);
	mpz_sub_ui(pMoinsUn, p, 1);
	mpz_invert(dp, e, pMoinsUn);
	mpz_sub_ui(qMoinsUn, q, 1);
	mpz_invert(dq, e, qMoinsUn);
	mpz_invert(ip, p, q);

	formatRsaPublicKey(n, e);
	formatRsaPrivateKey(d, p, q, dp, dq, ip);
}

void RSA_Encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
	mpz_powm(c, m, e, n);
}

void RSA_Decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
	mpz_powm(m, c, d, n);
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
	mpz_set_ui(k, 2048);
	mpz_set_ui(e, 3);

	RSA_CRT_Gen_Key(p, q, n, dp, dq, ip, k, e, d);
	/*RSA_Encrypt(c, m, e, n);
	RSA_Decrypt(m, c, d, n);
	RSA_CRT_Decrypt(m, c, p, q, dp, dq, ip);
	RSA_Signature(sign, m, d, n);
	RSA_Verif(m, sign, e, n);
	return 0;*/
}
