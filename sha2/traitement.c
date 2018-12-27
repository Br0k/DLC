#include "tete.h"


void initialiserK_valeurConstante(mpz_t k_values[65]){
	int i ;
	FILE * fichier;
	char * ligne = malloc (10 * sizeof(char));
	fichier = fopen("k_64_constantes.txt","r");
	if (fichier == NULL){
		printf("Erreur d'accès au fichier \n");
		exit (0);
	}
	for (int j = 0 ; j < 65; j++)
		mpz_init(k_values[j]);
	i=1; // ??
	while (fgets(ligne,10,fichier) != NULL){
		mpz_set_str(k_values[i],ligne,16);
		i++;
	}
	//fclose(fichier);	  waou??
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
}
void recuppererBlockPartie(char *block, int position, mpz_t data){
	mpz_init(data);
	char * chaine = malloc (32*sizeof(char));
	int debut = position * 32;
	for (int i = debut; i < debut+32; i++ )
		sprintf(chaine,"%s%c",chaine,block[i]);
		
	mpz_set_str(data,chaine,2);
}


void initialiserHashStructure(mpz_t valeurH[8]){
	//printf("1");
	char * chaine = malloc(32 * sizeof(char));
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

// reste à modifier blockData
void initialiserBlockData(char* message){  // sha2
	mpz_t donnee; 
	int k = 1;
	int taille = strlen(message);
	int valeur_message_bit = taille *8;
	char * elt = malloc(sizeof(char)) ;
	char * chaine; 
	int taille_bloc = 64; // taille d'un bloc 64 * 8 = 512
	chaineAtraiter ="";
	mpz_init(donnee);
	if ( taille_bloc - 9 >= taille)
		chaineAtraiter = malloc(512 * sizeof(char));
	else {
		do{
			k++;
			taille_bloc  =k * 64;     // i * 1 block
		}while( taille_bloc - 9 < taille );	
		chaineAtraiter = malloc(k *512 *sizeof(char));
	}
	// mets tous les caractères  recupe hexa puis mpz_t puis binary avec pading8 puis chaine
	for (int i = 0 ; i < taille;i++){
		sprintf(elt,"%02x",message[i]);
		mpz_set_str(donnee,elt,16);
		sprintf(chaineAtraiter,"%s%s",chaineAtraiter,paddingChaine( mpz_get_str(chaine,2,donnee) , 8 ));
	}
	mpz_set_str(donnee,"80",16);// désigne fin message
	sprintf(chaineAtraiter,"%s%s",chaineAtraiter,paddingChaine( mpz_get_str(chaine,2,donnee) , 8 ));

	for (int i = taille +1; i < taille_bloc - 8; i++){  //i < 56 (formule) si la valeur du message varie
		mpz_set_str(donnee,"0",16);
		sprintf(chaineAtraiter,"%s%s",chaineAtraiter,paddingChaine( mpz_get_str(chaine,2,donnee) , 8 ));
	}
	//
	mpz_set_ui(donnee,valeur_message_bit);  // review pading 64
	sprintf(chaineAtraiter,"%s%s",chaineAtraiter,paddingChaine( mpz_get_str(chaine,2,donnee) , 64 ));


	//printf("%s ici\n",chaineAtraiter);
	for (int i = 0 ; i < 64; i++) 
		mpz_init(blockData[i]);    //globale
	/*for (int i = 0 ; i < 16; i++)
		recuppererBlockPartie(chaineAtraiter, i,blockData[i]);	*/

}