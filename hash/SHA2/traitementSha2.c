#include "header.h"


void initialiserK_valeurConstante(mpz_t k_values[64]){
	
	for (int i = 0; i < 64; i++)
		mpz_init(k_values[i]);

	mpz_set_str(k_values[0],"428a2f98",16); mpz_set_str(k_values[1],"71374491",16); mpz_set_str(k_values[2],"b5c0fbcf",16); mpz_set_str(k_values[3],"e9b5dba5",16);
	mpz_set_str(k_values[4],"3956c25b",16); mpz_set_str(k_values[5],"59f111f1",16); mpz_set_str(k_values[6],"923f82a4",16); mpz_set_str(k_values[7],"ab1c5ed5",16);
	
	mpz_set_str(k_values[8],"d807aa98",16); mpz_set_str(k_values[9],"12835b01",16); mpz_set_str(k_values[10],"243185be",16); mpz_set_str(k_values[11],"550c7dc3",16);
	mpz_set_str(k_values[12],"72be5d74",16); mpz_set_str(k_values[13],"80deb1fe",16); mpz_set_str(k_values[14],"9bdc06a7",16); mpz_set_str(k_values[15],"c19bf174",16);

	mpz_set_str(k_values[16],"e49b69c1",16); mpz_set_str(k_values[17],"efbe4786",16); mpz_set_str(k_values[18],"0fc19dc6",16); mpz_set_str(k_values[19],"240ca1cc",16);
	mpz_set_str(k_values[20],"2de92c6f",16); mpz_set_str(k_values[21],"4a7484aa",16); mpz_set_str(k_values[22],"5cb0a9dc",16); mpz_set_str(k_values[23],"76f988da",16);

	mpz_set_str(k_values[24],"983e5152",16); mpz_set_str(k_values[25],"a831c66d",16); mpz_set_str(k_values[26],"b00327c8",16); mpz_set_str(k_values[27],"bf597fc7",16);
	mpz_set_str(k_values[28],"c6e00bf3",16); mpz_set_str(k_values[29],"d5a79147",16); mpz_set_str(k_values[30],"06ca6351",16); mpz_set_str(k_values[31],"14292967",16);

    mpz_set_str(k_values[32],"27b70a85",16); mpz_set_str(k_values[33],"2e1b2138",16); mpz_set_str(k_values[34],"4d2c6dfc",16); mpz_set_str(k_values[35],"53380d13",16);
	mpz_set_str(k_values[36],"650a7354",16); mpz_set_str(k_values[37],"766a0abb",16); mpz_set_str(k_values[38],"81c2c92e",16); mpz_set_str(k_values[39],"92722c85",16);
	
	mpz_set_str(k_values[40],"a2bfe8a1",16); mpz_set_str(k_values[41],"a81a664b",16); mpz_set_str(k_values[42],"c24b8b70",16); mpz_set_str(k_values[43],"c76c51a3",16);
	mpz_set_str(k_values[44],"d192e819",16); mpz_set_str(k_values[45],"d6990624",16); mpz_set_str(k_values[46],"f40e3585",16); mpz_set_str(k_values[47],"106aa070",16);

	mpz_set_str(k_values[48],"19a4c116",16); mpz_set_str(k_values[49],"1e376c08",16); mpz_set_str(k_values[50],"2748774c",16); mpz_set_str(k_values[51],"34b0bcb5",16);
	mpz_set_str(k_values[52],"391c0cb3",16); mpz_set_str(k_values[53],"4ed8aa4a",16); mpz_set_str(k_values[54],"5b9cca4f",16); mpz_set_str(k_values[55],"682e6ff3",16);

	mpz_set_str(k_values[56],"748f82ee",16); mpz_set_str(k_values[57],"78a5636f",16); mpz_set_str(k_values[58],"84c87814",16); mpz_set_str(k_values[59],"8cc70208",16);
	mpz_set_str(k_values[60],"90befffa",16); mpz_set_str(k_values[61],"a4506ceb",16); mpz_set_str(k_values[62],"bef9a3f7",16); mpz_set_str(k_values[63],"c67178f2",16);

}
char * paddingChaine(char * chaine, int nombreDeBitsArespecter){
	int decallage = (nombreDeBitsArespecter - strlen (chaine)); 
	char * resultat = malloc (nombreDeBitsArespecter * sizeof(char));
	if (strlen(chaine) < nombreDeBitsArespecter){
		for (int i = 0 ; i < decallage ; i++)
			sprintf(resultat+i,"%c",'0'); 
	}
	for (int i = 0 ; i < strlen(chaine); i++ )
		sprintf( resultat + i + decallage,"%c",chaine[i]);
	return resultat;
	free(resultat);
}
void nettoyer(char * chaine,int taille){
	for (int i = 0; i < taille; i++)
		chaine[i] = '\0';
}
void recuppererBlockPartie(char *block, int position, mpz_t data){
	mpz_init(data);
	char * chaine = malloc (32*sizeof(char));
	for ( int i = 0 ; i < 32; i++)  // nettoyer !! avant de commencer les opérations
		chaine[i] = '\0';
	int debut = position * 32;
	for (int i = debut; i < debut+32; i++ )
		sprintf(chaine,"%s%c",chaine,block[i]);
	mpz_set_str(data,chaine,2);
	free(chaine);
}


void initialiserHashStructure(mpz_t valeurH[8]){
	//printf("1");
	//char * chaine = malloc(32 * sizeof(char));
	for (int i = 0 ; i < 8; i++)
		mpz_init(valeurH[i]);
	mpz_set_str(valeurH[0],"6a09e667",16);
	mpz_set_str(valeurH[1],"bb67ae85",16);
	mpz_set_str(valeurH[2],"3c6ef372",16);
	mpz_set_str(valeurH[3],"a54ff53a",16);
	mpz_set_str(valeurH[4],"510e527f",16);
	mpz_set_str(valeurH[5],"9b05688c",16);
	mpz_set_str(valeurH[6],"1f83d9ab",16);
	mpz_set_str(valeurH[7],"5be0cd19",16);

}

// reste à modifier blockData;
int tailleAallouer(int tailleMessage){
	int k =1;
	int taille_bloc = 64;
	
	if ( taille_bloc - 9 >= tailleMessage)
		return 512;
	else {
		do{
			k++;
			taille_bloc  =k * 64;     // i * 1 block
		}while( taille_bloc - 9 < tailleMessage );
	return 512*k;
	}
}
char * initialiserBlockData(char* message,mpz_t blockData[64]){  // sha2
	mpz_t donnee; 
	int taille = strlen(message);
	int valeur_message_bit = taille *8;
	char * elt = malloc(sizeof(char)) ;
	char * chaine = malloc(64 *sizeof(char));
	char * chaineAtraiter;

	//int taille_bloc = 64; // taille d'un bloc 64 * 8 = 512
	//chaineAtraiter ="";
	mpz_init(donnee);
	//printf("%d\n", tailleAallouer(taille));
	//exit(0);
	chaineAtraiter = malloc(tailleAallouer(taille) * sizeof(char)); 
	nettoyer(chaineAtraiter,tailleAallouer(taille));
	int t = tailleAallouer(taille);
	char y [8] ;
	for (int i = 0; i <8;i++)
		y [i] ='\0';
	for (int i = 0; i < t ; i++)
		chaineAtraiter[i] = '\0';
	// mets tous les caractères  recupe hexa puis mpz_t puis binary avec pading8 puis chaine
	for (int i = 0 ; i < taille;i++){
		sprintf(elt,"%02x",message[i]);
		mpz_set_str(donnee,elt,16);
		strcpy(y,paddingChaine( mpz_get_str(chaine,2,donnee) , 8 ));		
		for (int j = 0; j < 8; j++){
			chaineAtraiter[j+(i*8)]= y[j];
		}
		
	}
	chaineAtraiter[valeur_message_bit]='1';
	
	
	int taille_bloc=t-valeur_message_bit-1-64;

	for (int i = valeur_message_bit +1; i < valeur_message_bit + taille_bloc +1; i++){  //i < 56 (formule) si la valeur du message varie
		chaineAtraiter[i] = '0';
		
	}
	

	//
	mpz_set_ui(donnee,valeur_message_bit);  // review pading 64

	sprintf(chaineAtraiter,"%s%s",chaineAtraiter,paddingChaine( mpz_get_str(chaine,2,donnee) , 64 ));



	for (int i = 0 ; i < 64; i++) 
		mpz_init(blockData[i]);    //globale

	
	return chaineAtraiter;

	free(chaineAtraiter);
	free(elt);
}
