#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>


void nettoyer(char * chaine,int taille);
int tailleAallouer(int tailleMessage);
void initialiserHashStructure(mpz_t valeurH[8]);
void initialiserK_valeurConstante(mpz_t k_values[64]);
char * initialiserBlockData(char* message,mpz_t blockData[64]);

void recuppererBlockPartie(char *block, int position, mpz_t data);

void fonction_sommeZero(mpz_t rop, mpz_t x);
void fonction_sommeUn(mpz_t rop, mpz_t x);
void fonction_Maj(mpz_t rop,mpz_t x,mpz_t y,mpz_t z);
void fonction_ch(mpz_t rop,mpz_t x,mpz_t y,mpz_t z);

char * rotationR(char * chaine,int nombreRotation );
char * shR(char * chaine,int nombreRotation );
char * paddingChaine(char * chaine, int nombreDeBitsArespecter);

void sigmaZero(mpz_t rop, mpz_t x);
void sigmaUn(mpz_t rop,mpz_t x);

void copieConstante(mpz_t valeurH[8],mpz_t a,mpz_t b,mpz_t c,mpz_t d,mpz_t e,mpz_t f,mpz_t g,mpz_t h);
void nouvelleValeur(mpz_t valeurH[8],mpz_t a,mpz_t b,mpz_t c,mpz_t d,mpz_t e,mpz_t f,mpz_t g,mpz_t h);
