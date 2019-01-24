#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

struct data
{
    char operande1 [200] ;
    char operande2 [200] ;
    char operateur [100];
};
typedef struct data data;


typedef struct Element Element;
struct Element
{
    char cmd[1024];
    Element *suivant;
};

typedef struct File File;
struct File
{
    Element *premier;
};



void enfiler(File *file, char * command)
{
    Element *nouveau = malloc(sizeof(*nouveau));
    if (file == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }



    strcpy(nouveau->cmd,command);
    

    nouveau->suivant = NULL;

    if (file->premier != NULL) /* La file n'est pas vide */
    {
        /* On se positionne à la fin de la file */
        Element *elementActuel = file->premier;
        while (elementActuel->suivant != NULL)
        {
            elementActuel = elementActuel->suivant;
        }
        elementActuel->suivant = nouveau;
    }
    else /* La file est vide, notre élément est le premier */
    {
        file->premier = nouveau;
    }
}

void afficherFile(File *file,GtkWidget *label)
{
  char tmp[9999]="";
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }
    Element *actuel = file->premier;

    while (actuel != NULL)
    {
        strcat(tmp,actuel->cmd );
        strcat(tmp,"\n"); 

        actuel = actuel->suivant; 
    }
    gtk_label_set_text(GTK_LABEL(label),tmp);
}

int SizeFile(File *file)
{
    int size=0;
    if (file == NULL)
    {
        return size;
    }
    Element *actuel = file->premier;

    while (actuel != NULL)
    {
       size++;
       actuel = actuel->suivant;
    }
    return size;
}



char * defiler(File *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    char * commande=malloc(sizeof(char *)*1024);

    /* On vérifie s'il y a quelque chose à défiler */
    if (file->premier != NULL)
    {
        Element *elementDefile = file->premier;

        strcpy(commande,elementDefile->cmd);
        //commande = elementDefile->cmd;
        file->premier = elementDefile->suivant;
        free(elementDefile);
    }

    return commande;
}

File *initialiser()
{
    File *file = malloc(sizeof(*file));
    file->premier = NULL;

    return file;
}

File* Lecture(){
    char nbelement[2];
    int size;
    File *maFile = initialiser();
    FILE * fichier = fopen( "fileHistorique.txt","r");
    fgets( nbelement, 1024, fichier );
    size = atoi(nbelement);
    printf("%d\n",size );
    for (int i = 0; i <size ; i++)
    {
        char tmp[1024];
        fgets(tmp,1024, fichier );
        enfiler(maFile,tmp);    
        
    }

    return maFile;

}
        
void stockageDansLefichier(File * maFile){
    FILE * fichier;
    fichier = fopen("fileHistorique.txt","w+");
    int size = SizeFile(maFile);
    fprintf(fichier,"%d\n",size);
    for (int i = 0; i < size; i++)
    {
        char * operation = defiler(maFile);
        fprintf(fichier,"%s",operation);
    }
    
    
    fclose(fichier);


}
/*
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
    fclose(fichier);*/
        
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


