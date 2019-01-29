#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>



// chaque ligne contient 5 bits qui 
struct etat{  // ?? 25 element char 
	char element[5][5];
};
typedef struct etat etat;
etat keccakData[64];

etat tranche;

mpz_t tourConstante[24];
char *chaineAtraiterSha3;




void formaterChaineSha3(char *message);
char * paddingChaineSha3(char * chaine, int nombreDeBitsArespecter);

void keccak();