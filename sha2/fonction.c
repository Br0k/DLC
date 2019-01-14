#include "tete.h"


// modification à faire taille chaine pour convertir
//512 bits ( 448  --> message proprement dit + 64 --> valeur de l en binaire 
// m+l + 1 +k


char * rotationR(char * chaine,int nombreRotation ){
	//printf("%s  %ld+++\n",chaine,strlen(chaine));printf("2");
	//printf("2");
	char * inter = malloc(32 * sizeof(char));
	strcpy(inter,chaine);
	char   *elt = malloc(32*sizeof(char)) ;
	//printf("%s  --\n",inter );
	for ( int i = 0; i < nombreRotation ; i++ ){
		elt[0] = inter[31];
		for (int j = 0 ; j < 31; j++)
			elt [j+1] = inter[j];
		strcpy(inter,elt);
		
	}
	//printf("%s \n",elt);
	return elt;

}
char * shR(char * chaine,int nombreRotation ){
	//printf("3");
	char * inter = malloc(32 * sizeof(char));
	strcpy(inter,chaine);
	char   *elt = malloc(32*sizeof(char)) ;
	for ( int i = 0; i < nombreRotation ; i++ ){
		elt[0] = '0';
		for (int j = 0 ; j < 31; j++)
			elt [j+1] = inter[j];
		strcpy(inter,elt);
		
	}
	return elt;
}

// function du Hash
// fonction rot et shr
void complementBinaire(mpz_t rop,mpz_t x){
	//printf("5");
	char *resultat = malloc(32* sizeof(char));
	char * chaine = malloc (32* sizeof(char));
	
	chaine = paddingChaine(mpz_get_str(chaine,2,x),32);
	for (int i = 0; i < 32 ; i++){
		if (chaine[i] == '1')
			sprintf(resultat,"%s%c",resultat,'0');
		else
			sprintf(resultat,"%s%c",resultat,'1');

	}
	mpz_set_str(rop,resultat,2);
}

void fonction_ch(mpz_t rop,mpz_t x,mpz_t y,mpz_t z){
	//printf("6");
	char *chaine= malloc(32*sizeof(char));
	mpz_t temp1,temp2;
	mpz_inits(temp1,temp2,NULL);
	mpz_and(temp1,x,y);
	complementBinaire(temp2,x); 
	//gmp_printf("%s temp2 --x \n", paddingChaine(mpz_get_str(chaine,2,temp2), 32));
	mpz_and(temp2,temp2,z);
	mpz_xor(rop,temp1,temp2);

	/*
	//gmp_printf("%s x\n", paddingChaine(mpz_get_str(chaine,2,x), 32));
	//gmp_printf("%s y\n",paddingChaine(mpz_get_str(chaine,2,y), 32));
	//gmp_printf("%s temp1\n",paddingChaine(mpz_get_str(chaine,2,temp1), 32));

	
	gmp_printf("%s z\n", paddingChaine(mpz_get_str(chaine,2,z), 32));
	gmp_printf("%s temp2\n", paddingChaine(mpz_get_str(chaine,2,temp2), 32));
	gmp_printf("%s temp1\n", paddingChaine(mpz_get_str(chaine,2,temp1), 32));
	gmp_printf("%s rop\n", paddingChaine(mpz_get_str(chaine,2,rop), 32));*/


}
void fonction_Maj(mpz_t rop,mpz_t x,mpz_t y,mpz_t z){
	//printf("7");
	char * chaine = malloc(32*sizeof(char));
	mpz_t temp1,temp2,temp3;
	mpz_inits(temp1,temp2,temp3,NULL);
	mpz_and(temp1,x,y);
	mpz_and(temp2,x,z);
	mpz_and(temp3,y,z);
	mpz_xor(rop,temp1,temp2);
	mpz_xor(rop,rop,temp3);
	
	
}
void fonction_sommeZero(mpz_t rop, mpz_t x){  // rot2 rot13 rot22
	//printf("8 ");
	
	char * chaine = malloc(32*sizeof(char));
	mpz_t r1,r2,r3,cp;
	mpz_inits(r1,r2,r3,cp,NULL);

	mpz_set_str(r1,rotationR( paddingChaine( mpz_get_str(chaine,2,x),32) , 2 ), 2); 
	mpz_set_str(r2,rotationR( paddingChaine( mpz_get_str(chaine,2,x),32) , 13), 2);
	mpz_set_str(r3,rotationR( paddingChaine( mpz_get_str(chaine,2,x),32) , 22), 2); 
	mpz_xor(rop,r1,r2);
	mpz_xor(rop,rop,r3);
	
	/*gmp_printf("%s base\n", paddingChaine(mpz_get_str(chaine,2,x), 32));
	gmp_printf("%s rot2\n", paddingChaine(mpz_get_str(chaine,2,r1), 32));
	gmp_printf("%s rot13\n",paddingChaine(mpz_get_str(chaine,2,r2), 32));
	gmp_printf("%s rot22\n",paddingChaine(mpz_get_str(chaine,2,r3), 32));
	gmp_printf("%s finale\n", paddingChaine(mpz_get_str(chaine,2,rop), 32));*/
}
void fonction_sommeUn(mpz_t rop, mpz_t x){ // rot6 rot11 rot25
	//printf("9");
	char * chaine = malloc(32 *sizeof(char));
	mpz_t r1,r2,r3;
	mpz_inits(r1,r2,r3,NULL);
	//mpz_set(cp,x);
	mpz_set_str(r1,rotationR( paddingChaine( mpz_get_str(chaine,2,x),32) , 6 ), 2); 
	mpz_set_str(r2,rotationR( paddingChaine( mpz_get_str(chaine,2,x),32) , 11), 2);
	mpz_set_str(r3,rotationR( paddingChaine( mpz_get_str(chaine,2,x),32) , 25), 2);  /**/
	mpz_xor(rop,r1,r2);
	mpz_xor(rop,rop,r3);

}
void sigmaZero(mpz_t rop, mpz_t x){ // rot7 rot18 shr3
	//printf("sig0\n");
	char * chaine = malloc(32 *sizeof(char));
	mpz_t r1,r2,shr;
	mpz_inits(r1,r2,shr,NULL);
	mpz_set_str(r1,rotationR( paddingChaine( mpz_get_str(chaine,2,x),32) , 7 ), 2); 
	mpz_set_str(r2,rotationR( paddingChaine( mpz_get_str(chaine,2,x),32) , 18), 2);
	mpz_set_str(shr,      shR( paddingChaine( mpz_get_str(chaine,2,x),32) , 3), 2);  /**/
	mpz_xor(rop,r1,r2);
	mpz_xor(rop,rop,shr);

	/*gmp_printf("%s base\n", paddingChaine(mpz_get_str(chaine,2,x), 32));
	gmp_printf("%s rot7\n", paddingChaine(mpz_get_str(chaine,2,r1), 32));
	gmp_printf("%s rot18\n",paddingChaine(mpz_get_str(chaine,2,r2), 32));
	gmp_printf("%s shr3\n",paddingChaine(mpz_get_str(chaine,2,shr), 32));
	gmp_printf("%s finale\n", paddingChaine(mpz_get_str(chaine,2,rop), 32));*/
}
void sigmaUn(mpz_t rop,mpz_t x){ // rot17 rot19 shr10
	//printf("sig1\n");
	char * chaine = malloc(32 *sizeof(char));
	mpz_t r1,r2,shr;
	mpz_inits(r1,r2,shr,NULL);
	mpz_set_str(r1,rotationR( paddingChaine( mpz_get_str(chaine,2,x),32) , 17), 2); 
	mpz_set_str(r2,rotationR( paddingChaine( mpz_get_str(chaine,2,x),32) , 19), 2);
	mpz_set_str(shr,      shR( paddingChaine( mpz_get_str(chaine,2,x),32) , 10), 2);  /**/
	mpz_xor(rop,r1,r2);
	mpz_xor(rop,rop,shr);

	/*gmp_printf("%s base\n", paddingChaine(mpz_get_str(chaine,2,x), 32));
	gmp_printf("%s rot17\n", paddingChaine(mpz_get_str(chaine,2,r1), 32));
	gmp_printf("%s rot19\n",paddingChaine(mpz_get_str(chaine,2,r2), 32));
	gmp_printf("%s shr10\n",paddingChaine(mpz_get_str(chaine,2,shr), 32));
	gmp_printf("%s finale\n", paddingChaine(mpz_get_str(chaine,2,rop), 32));*/
}

//
void copieConstante(){
	mpz_set(a,valeurH[0]);
	mpz_set(b,valeurH[1]);
	mpz_set(c,valeurH[2]);
	mpz_set(d,valeurH[3]);
	mpz_set(e,valeurH[4]);
	mpz_set(f,valeurH[5]);
	mpz_set(g,valeurH[6]);
	mpz_set(h,valeurH[7]);
}

void nouvelleValeur(){
	char * chaine = malloc (32*sizeof(char));
	mpz_add(valeurH[0],valeurH[0],a);
	mpz_set_str(valeurH[0],paddingChaine(mpz_get_str(chaine,16,valeurH[0]),8),16);
	mpz_add(valeurH[1],valeurH[1],b);
	mpz_set_str(valeurH[1],paddingChaine(mpz_get_str(chaine,16,valeurH[1]),8),16);

	mpz_add(valeurH[2],valeurH[2],c);
	mpz_set_str(valeurH[2],paddingChaine(mpz_get_str(chaine,16,valeurH[2]),8),16);

	mpz_add(valeurH[3],valeurH[3],d);
	mpz_set_str(valeurH[3],paddingChaine(mpz_get_str(chaine,16,valeurH[3]),8),16);

	mpz_add(valeurH[4],valeurH[4],e);
	mpz_set_str(valeurH[4],paddingChaine(mpz_get_str(chaine,16,valeurH[4]),8),16);

	mpz_add(valeurH[5],valeurH[5],f);
	mpz_set_str(valeurH[5],paddingChaine(mpz_get_str(chaine,16,valeurH[5]),8),16);

	mpz_add(valeurH[6],valeurH[6],g);
	mpz_set_str(valeurH[6],paddingChaine(mpz_get_str(chaine,16,valeurH[6]),8),16);

	mpz_add(valeurH[7],valeurH[7],h);
	mpz_set_str(valeurH[7],paddingChaine(mpz_get_str(chaine,16,valeurH[7]),8),16);
}