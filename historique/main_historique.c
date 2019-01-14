#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct data
{
	char operande1 [200] ;
	char operande2 [200] ;
	char operateur [100];
};
typedef struct data data;
// valeur operande  = zero ==> pas de valeur à cette opérande
void PremierEntrerPremierSortie(data history[10],data nouveauElement){
	for (int i = 0 ; i < 9; i++){
		strcpy(history[i].operande1 ,history[i+1].operande1);
		strcpy(history[i].operande2 ,history[i+1].operande2);
		strcpy(history[i].operateur ,history[i+1].operateur);
	}
	strcpy(history[9].operande1 ,nouveauElement.operande1);
	strcpy(history[9].operande2 ,nouveauElement.operande2);
	strcpy(history[9].operateur ,nouveauElement.operateur);

}
void stockageDansLefichier(data history[10]){
	FILE * fichier;
	fichier = fopen("fileHistorique.txt","w");
	char *ligne = malloc(512*sizeof(char));
	for (int i = 0; i < 10; i++)
		fprintf(fichier,"{ operandeUn : %s , operandeDeux : %s , operateur : %s },\n",history[i].operande1,history[i].operande2,history[i].operateur);
	fclose(fichier);
		
	printf("\n" );

}

int main (int argc, char **argv){
	data history[10];
	char  t [200];
	FILE * fichier;
	fichier = fopen("fileHistorique.txt","r");

	if (fichier == NULL){
		printf("Erreur d'accès au fichier \n");
		exit (0);
	}
	
	for (int i = 0; i < 10; i++){
		fscanf(fichier,"{ operandeUn : %s , operandeDeux : %s , operateur : %s },",history[i].operande1,history[i].operande2,history[i].operateur);
		fseek(fichier,+1,SEEK_CUR);
	}
	fclose(fichier);
		
	/*data nouveauElement ;
	strcpy(nouveauElement.operande1,"7894545");
	strcpy(nouveauElement.operande2,"4555");
	strcpy(nouveauElement.operateur,"-");
	PremierEntrerPremierSortie(history,nouveauElement);
	for (int i = 0; i < 10 ;i++)
		printf("%s  %s %s\n",history[i].operande1,history[i].operande2,history[i].operateur );
	strcpy(history[9].operande2 ,"kerfala");
	strcpy(history[9].operateur,"sha3");
	stockageDansLefichier(history);*/


	return 0;
}
