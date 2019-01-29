#include "sha3.h"

//sha3 256  r=1088 c=512   mbit = 01  puis 1 suivi de 0 terminé par 1
//padding
//faire des blocs de 25 elts * 64 ==>1600bits
char * paddingChaineSha3(char * chaine, int nombreDeBitsArespecter){
	int decallage = (nombreDeBitsArespecter - strlen (chaine)); 
	char * resultat = malloc (nombreDeBitsArespecter * sizeof(char));
	if (strlen(chaine) < nombreDeBitsArespecter){
		for (int i = 0 ; i < decallage ; i++)
			sprintf(resultat+i,"%c",'0'); 
	}
	for (int i = 0 ; i < strlen(chaine); i++ )
		sprintf( resultat + i + decallage,"%c",chaine[i]);
	return resultat;
}
// xor r and block
void formaterChaineSha3(char *message){
	 // penser à gérer quand le message est plus grand que r ( après)
	// le nombre state depend de la taille message ( variable ==> keccakData )
	mpz_t donnee; 
	int taille = strlen(message);
	char * elt = malloc(sizeof(char)) ;
	char * chaine = malloc (8*sizeof(char)); 
	int taille_bit = 1600; 
	int r = 1088;
	chaineAtraiterSha3 ="";
	chaineAtraiterSha3= malloc(1600 * sizeof(char));
	
	mpz_init(donnee);
	for (int i = 0 ; i < taille;i++){
		sprintf(elt,"%02x",message[i]);
		mpz_set_str(donnee,elt,16);
		sprintf(chaineAtraiterSha3,"%s%s",chaineAtraiterSha3,paddingChaineSha3( mpz_get_str(chaine,2,donnee) , 8 ));
	}
	printf("ok\n");
	//sha3 256 d= 01 suivi 1
	sprintf(chaineAtraiterSha3,"%s%s",chaineAtraiterSha3,"011");
	
	//for (int i = 8*taille +3; i < taille_bit -1; i++){
	for (int i = 8*taille +3; i < r -1; i++){
		sprintf(chaineAtraiterSha3,"%s%c",chaineAtraiterSha3,'0');
	}
	sprintf(chaineAtraiterSha3,"%s%c",chaineAtraiterSha3,'1');
	for (int i = r; i < 1600; i++)
		sprintf(chaineAtraiterSha3,"%s%c",chaineAtraiterSha3,'0');
	
	//printf("%s  ok\n",chaineAtraiterSha3 );
	
	// a verifier et les valeurs
	int a = 65;
	int pad;
	for (int k = 0; k < 64 ; k++){
		pad = 0;
		for (int i = 0 ; i < 5; i++){ // colonne 
			for (int j = 0 ; j < 5; j++){  // ligne   ===>fait --> // A voir si tableau a deux dimensions sur element [x][y]
				keccakData[k].element[i][j] = chaineAtraiterSha3[pad + (k*25)];
				pad++;
			}
		}
	}
	for (int k = 0; k < 64 ; k++){
		printf("%d -->",k+1);
		for (int i = 0; i < 5 ; i++){
			for (int j = 0 ; j < 5; j++){
				printf("%c ", keccakData[k].element[i][j]);
			}
		}
		printf("\n");
	}

	printf("ok \n%s\n",chaineAtraiterSha3);
	
}
// question de modulo
char colonneY(etat plan, int x){
	char resultat='0';
	//char *tab = malloc (5* sizeof(char*));
	for (int i = 0 ; i < 5 ; i++){
		resultat = (resultat ^ plan.element[x][i]) + '0';
		//tab[i] = plan.element[k];
	
	}
	return resultat;
}
void initialiserState(etat finalData[64]){
	for (int k = 0; k < 64 ; k++){
		for (int i = 0; i < 5 ; i++){
			for (int j = 0 ; j < 5; j++){
				finalData[k].element[i][j] = '0';
			}
		}
	}
}
void copieState(etat source[64],etat destination[64]){
	for (int k = 0; k < 64 ; k++){
		for (int i = 0; i < 5 ; i++){
			for (int j = 0 ; j < 5; j++){
				destination[k].element[i][j] = source[k].element[i][j];
			}
		}
	}
}
void absorptionPhase(etat finalData[64], etat etatCourant[64]){  // etatCourant represente le block state en courant de traitement
	
	for (int k = 0; k < 64 ; k++){
		for(int i = 0; i < 5; i++){
			for (int j = 0 ; j < 5; j++){
				finalData[k].element[i][j] = ( finalData[k].element[i][j] ^ etatCourant[k].element[i][j] ) +'0'; // +'0' ==> remettre en caractère
			}
		}
	}
	/*printf("après *****\n");
	for (int i = 0 ; i < 5; i++){
		for(int j = 0; j < 5; j++)
			printf("%c  ",etat.element[i] );
	}*/

}
// lié à a fonction teta
int mod5(int a){ //  (nb % 5 ) + ou - (1)
	if (a == -1)
		return a +5;  // 4
	else if (a == 5)
		return 0;  //a -5
	else 
		return a;
}
// fonction rho and pi
//*******************************************
char [64] creationDeL_axe_z_ApartirState(etat temp[64],int x,int y){

}
void rotationSurl_axe_z(char lane[64],int nombreRotation){
	char dernierElement;
	for (int i = 0; i < nombreRotation; i++){
		dernierElement = lane[63];
		for (int j = 63; j >0; j--){ // rotation circulaire
			lane[j] = lane[j-1];
		}
		lane[0] = dernierElement;
	}
}
void piAndRho(etat temp[64], etat stateB[64],int constanteOffsetRotation [5][5]){
	printf("je suis!!");
	// rho sur temp
	for (int k = 0; k < 64; k++){
		for (int i = 0 ; i < 5; i++){
			for(int j = 0; j < 5; j++){
				 /// ??? autre petit blocus  (concept comprehension) 
			}
		}
	}
	initialiserState(temp);

}
//***********************************************
void fonctionTours(etat finalData[64], mpz_t constante,int constanteOffsetRotation [5][5]){ // ajout constanteoffsetR
	// chaque point pour teta col gauche xor col droite et point lui même
	// teta
	//*****
	// faire une copie d'etat ?? !! 
	char colg ;
	char cold ;
	for (int k = 0; k < 64; k++){
		for (int i = 0 ; i < 5; i++){
			for(int j = 0; j < 5; j++){
				// je suis là !!
				colg = colonneY(finalData[k],mod5((j%5)-1 )); // col gauche  attention gestion négative number à faire 
				cold = colonneY(finalData[k],mod5((j%5)+1 ));
				finalData[k].element[i][j] = (finalData[k].element[i][j] ^ colg ^ cold ) + '0';// xor copie ??
			}
			
		} 
	}
	// rho and pi
	// B[x,y] ==> tranche dont chaque position est modifié de : B[y,2*x+3*y]= rot(A[x,y], r[x,y]) 
	etat stateB[64];
	etat temp[64];
	initialiserState(stateB);
	copieState(finalData,temp);
	piAndRho(temp,stateB,constanteOffsetRotation);



}
void keccak_f(etat finalData[64]){ //n = 24
	for (int i = 0; i < 24; i++){
		//fonctionTours(etat,tourConstante[i]);
		printf("ee");
	}
}
void keccak(){
	//initialiserStateFinal(etat finalData[64])
	// la phase elle est par le nombre de block state que nous avons
	//absorptionPhase(keccakData,etat);
	//keccak_f(etat);
	//for (int i = 0; i < )
	//etat = keccakData[i].element[j]
}

//void keccak elle même
// a chaque point teta = bit origine xor 5 bit  de la colonne à gauche xor  5 bit de col de droite et position 1 bit
void initialiserTourConstante(){
	for (int i = 0; i < 24; i++)
		mpz_init(tourConstante[i]);

	mpz_set_str(tourConstante[0],"0000000000000001",16);	mpz_set_str(tourConstante[12],"000000008000808B",16);
	mpz_set_str(tourConstante[1],"0000000000008082",16);	mpz_set_str(tourConstante[13],"800000000000008B",16);
	mpz_set_str(tourConstante[2],"800000000000808A",16);	mpz_set_str(tourConstante[14],"8000000000008089",16);
	mpz_set_str(tourConstante[3],"8000000080008000",16);	mpz_set_str(tourConstante[15],"8000000000008003",16);
	mpz_set_str(tourConstante[4],"000000000000808B",16);	mpz_set_str(tourConstante[16],"8000000000008002",16);
	mpz_set_str(tourConstante[5],"0000000080000001",16);	mpz_set_str(tourConstante[17],"8000000000000080",16);
	mpz_set_str(tourConstante[6],"8000000080008081",16);	mpz_set_str(tourConstante[18],"000000000000800A",16);
	mpz_set_str(tourConstante[7],"8000000000008009",16);	mpz_set_str(tourConstante[19],"800000008000000A",16);
	mpz_set_str(tourConstante[8],"000000000000008A",16);	mpz_set_str(tourConstante[20],"8000000080008081",16);
	mpz_set_str(tourConstante[9],"0000000000000088",16);	mpz_set_str(tourConstante[21],"8000000000008080",16);
	mpz_set_str(tourConstante[10],"0000000080008009",16);	mpz_set_str(tourConstante[22],"0000000080000001",16);
	mpz_set_str(tourConstante[11],"000000008000000A",16);	mpz_set_str(tourConstante[23],"8000000080008008",16);
}

// absorption phase
// xorage de tous elements de la feuille et en profondeur
// appel de keccak fonction

//int a=8;  
//char c=a+'0'