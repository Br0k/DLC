#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

mpz_t a,b,c,d,e,f,g,h;
mpz_t valeurH[8];
mpz_t blockData[64]; //512 = 64 * 8 (un caractère) ==> sha 256

mpz_t k_values[64]; // 

char * chaineAtraiter;


void initialiserHashStructure(mpz_t valeurH[8]);
void initialiserK_valeurConstante(mpz_t k_values[64]);
void initialiserBlockData(char* message);

void recuppererBlockPartie(char *block, int position, mpz_t data);
void moidifierBlockPartie(char * block,int position, mpz_t data);

void fonction_sommeZero(mpz_t rop, mpz_t x);
void fonction_sommeUn(mpz_t rop, mpz_t x);
void fonction_Maj(mpz_t rop,mpz_t x,mpz_t y,mpz_t z);
void fonction_ch(mpz_t rop,mpz_t x,mpz_t y,mpz_t z);

char * rotationR(char * chaine,int nombreRotation );
char * shR(char * chaine,int nombreRotation );
char * paddingChaine(char * chaine, int nombreDeBitsArespecter);

void sigmaZero(mpz_t rop, mpz_t x);
void sigmaUn(mpz_t rop,mpz_t x);

void copieConstante();
void nouvelleValeur();