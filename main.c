//fonctions de hachage
#include "hash/hashlib.c"
#include "hash/SHA2/sha2.c"
#include "hash/SHA2/fonctionSha2.c"
#include "hash/SHA2/traitementSha2.c"
#include "hash/SHA3/sha3.c"

//historique
#include "historique/main_historique.c"

//fonction AES
#include "AES/AES_encrypt.c"
#include "AES/AES_decrypt.c"

//RSA
#include "RSA/RSA_CRT.c"
#include <stdio.h>
#include <stdlib.h>

//includes de base
#include <ctype.h>
#include <gmp.h>
#include <gtk/gtk.h>

gchar * distro;
gchar * hashAlgo;
gchar * AES;
gchar * TypeAES;
gchar *hmacKey;
int bool_retour_dialog_annuler;
int bool_retour_dialog_annuler_RSA;

unsigned char* key;

gchar *RSAPriv;
gchar *RSASign;
gchar *rsa;
gchar *rsaValue;

gchar * get_p;
gchar * get_q;
gchar * get_dp;
gchar * get_dq;
gchar * get_ip;

char* file_path;
int lenAES;
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
gchar *RSAKeyLenChoice;
int RSAKeyLen;

unsigned char *keyHex;
unsigned char IV[16];

GtkWidget *window;
GtkWidget *dialog_AES;
GtkWidget *dialog_AES_CBC;
GtkWidget *dialogRSAK;
GtkWidget *dialogHash;
GtkWidget *dialogRSAP;
GtkWidget *dialogRSAS;
GtkWidget *dialogCRT;
GtkWidget *hide_image;
GtkWidget *eye_image;

GtkEntry *Entry_AES_IV;
GtkEntry *Entry_AES;
GtkEntry *Entry_AES_KEY;
GtkEntry *Entry_AES_KEY_CBC;
GtkEntry *Entry_CRT_P;
GtkEntry *Entry_CRT_Q;
GtkEntry *Entry_CRT_DP;
GtkEntry *Entry_CRT_DQ;
GtkEntry *Entry_CRT_IP;
GtkBuilder *builder;  


GtkWidget* label;
File *maFile;

typedef struct
{
  GtkBuilder *builder;
  gpointer user_data;
} SGlobalData;

static void destroy( GtkWidget *widget){
  stockageDansLefichier(maFile);  
  gtk_main_quit ();
  g_object_unref(builder);
} 



// Gestion de l'historique avec le fichier fileHistorique.txt
void Historique(char* op1,char* op2,char* type,char* type2){

  int taille =SizeFile(maFile);
  printf("%d\n", taille);
  char tmp[1024];
  if(strlen(type2)!=0){
    sprintf(tmp,"\t%s(%s)  =  %s(%s)\n",type,op1,type2,op2);
  }
  else{
    sprintf(tmp,"\t%s(%s)  =  %s\n",type,op1,op2);
  }
  if(taille>=7){
    defiler(maFile);
    enfiler(maFile,tmp);
  }
  else {
    enfiler(maFile,tmp);
  }

  afficherFile(maFile,label);
}          

// Fonction de conversion HEX<->DEC
int hexadecimalToDecimal(char hexVal[]) {
    int len = strlen(hexVal); 
      
    // Initializing base value to 1, i.e 16^0    
    int base = 1; 
    int dec_val = 0; 
      
    // Extracting characters as digits from last character 
    for (int i=len-1; i>=0; i--) 
    {      

        if (hexVal[i]>='0' && hexVal[i]<='9') 
        { 
            dec_val += (hexVal[i] - 48)*base; 
                  
            // incrementing base by power 
            base = base * 16; 
        }  

        else if (hexVal[i]>='A' && hexVal[i]<='F') 
        {   
            dec_val += (hexVal[i] - 55)*base; 
             
            // incrementing base by power 
            base = base*16; 
        } 
    } 
      
    return dec_val; 
} 

int main(int argc, char *argv []){
   

  gtk_init(&argc, &argv);
  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "IHM.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));

  dialog_AES = GTK_WIDGET(gtk_builder_get_object(builder, "DialogAES"));
  dialog_AES_CBC = GTK_WIDGET(gtk_builder_get_object(builder, "DialogAES_CBC"));
  dialogHash = GTK_WIDGET(gtk_builder_get_object(builder, "DialogHash"));
  dialogRSAK = GTK_WIDGET(gtk_builder_get_object(builder, "DialogRSAKey"));
  dialogRSAP = GTK_WIDGET(gtk_builder_get_object(builder, "DialogRSAP"));
  dialogRSAS = GTK_WIDGET(gtk_builder_get_object(builder, "DialogRSAS"));
  dialogCRT = GTK_WIDGET(gtk_builder_get_object(builder, "DialogCRT"));

  label = GTK_WIDGET(gtk_builder_get_object(builder,"old_calcul"));
  hide_image = GTK_WIDGET(gtk_builder_get_object(builder,"hide"));
  eye_image = GTK_WIDGET(gtk_builder_get_object(builder,"eye"));
  Entry_AES_KEY = GTK_WIDGET(gtk_builder_get_object(builder,"keyAES"));

  key = (unsigned char *) malloc(sizeof(unsigned char*)*16);
  keyHex = (unsigned char *) calloc(16*3,sizeof(unsigned char*)*16*3);

  Entry_CRT_P = GTK_WIDGET(gtk_builder_get_object(builder,"CRT_P"));
  Entry_CRT_Q = GTK_WIDGET(gtk_builder_get_object(builder,"CRT_Q"));
  Entry_CRT_DP = GTK_WIDGET(gtk_builder_get_object(builder,"CRT_DP"));
  Entry_CRT_DQ = GTK_WIDGET(gtk_builder_get_object(builder,"CRT_DQ"));
  Entry_CRT_IP = GTK_WIDGET(gtk_builder_get_object(builder,"CRT_IP"));
  
  gtk_window_set_transient_for(GTK_WINDOW(dialog_AES),GTK_WINDOW(window));
  gtk_window_set_attached_to(GTK_WINDOW(window),dialog_AES);

  gtk_window_set_transient_for(GTK_WINDOW(dialog_AES_CBC),GTK_WINDOW(window));
  gtk_window_set_attached_to(GTK_WINDOW(window),dialog_AES_CBC);

  gtk_window_set_transient_for(GTK_WINDOW(dialogHash),GTK_WINDOW(window));
  gtk_window_set_attached_to(GTK_WINDOW(window),dialogHash);

  gtk_window_set_transient_for(GTK_WINDOW(dialogRSAK), GTK_WINDOW(window));
  gtk_window_set_attached_to(GTK_WINDOW(window), dialogRSAK);

  gtk_window_set_transient_for(GTK_WINDOW(dialogRSAP),GTK_WINDOW(window));
  gtk_window_set_attached_to(GTK_WINDOW(window),dialogRSAP);

  gtk_window_set_transient_for(GTK_WINDOW(dialogRSAS),GTK_WINDOW(window));
  gtk_window_set_attached_to(GTK_WINDOW(window),dialogRSAS);

  gtk_window_set_transient_for(GTK_WINDOW(dialogCRT),GTK_WINDOW(window));
  gtk_window_set_attached_to(GTK_WINDOW(window),dialogCRT);

  gtk_builder_connect_signals(builder, NULL);

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

  maFile = initialiser(); 
  maFile = Lecture();
  afficherFile(maFile,label); 
  g_signal_connect (G_OBJECT(window), "destroy",G_CALLBACK(destroy), NULL);  

  gtk_widget_show(window);                
  gtk_main();
  free(key);
  free(keyHex);


  return 0;}

// Récupération du chemin complet du fichier importé
void import_path(GtkFileChooserButton *btn){
  file_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(btn));
  //printf("%s\n",file_path );
  //gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(btn));
}

// Récupération de l'algorithme de hachage choisi
void hashList(GtkComboBox *widget){
  GtkComboBox *combo_box = widget;

  gint index = -1;
  index = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_box));
  
  if (index != -1) {
    hashAlgo= gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo_box));
   //g_print ("Choix : %s\n", hashAlgo);
   
  }  
}

void rsaList(GtkComboBox *widget) {
  GtkComboBox *combo_box = widget;

  gint index = -1;
  index = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));

  if (index != -1)
  {
    rsa = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_box));
  }
}

void rsaLenList(GtkComboBox *widget) {
  GtkComboBox *combo_box = widget;

  gint index = -1;
  index = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));

  if (index != -1)
  {
    RSAKeyLenChoice = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_box));
  }
}

char* readFile(char* filename){

  char *buffer = NULL;
  int string_size, read_size;
  FILE *handler = fopen(filename, "r");	

  if (handler)
  {
    fseek(handler, 0, SEEK_END);
    string_size = ftell(handler);
    rewind(handler);
    buffer = (char*) malloc(sizeof(char) * (string_size + 1) );
    read_size = fread(buffer, sizeof(char), string_size, handler);
    buffer[string_size] = '\0';

    if (string_size != read_size)
    {
       free(buffer);
       buffer = NULL;
    }
    fclose(handler);
  }
  return buffer;
}

// Affichage d'une pop-up en cas d'erreur
void printErreur(char *msg){ 
  GtkWidget*  dialog;
  dialog = gtk_message_dialog_new (GTK_WINDOW(window) ,
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_CLOSE,
                                  msg);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}
 
void Dialog_click_Annuler(GtkButton *button,GtkEntry *entry){
  gtk_entry_set_text(entry,"");
  gtk_entry_set_text(entry,"Entrer votre clé AES ...");
  gtk_widget_hide((GtkWidget*)dialog_AES); 
  bool_retour_dialog_annuler=1;

 }



void Dialog_AES_CBC_click_Annuler(GtkButton *button,GtkEntry *entry){
  const gchar *entry_text_IV;
  entry_text_IV = gtk_entry_get_text (GTK_ENTRY (Entry_AES_IV));
  gtk_entry_set_text(entry,"");
  gtk_entry_set_text(entry,"Entrer votre clé AES ...");
  gtk_entry_set_text(GTK_ENTRY(entry_text_IV),"");
  gtk_entry_set_text(GTK_ENTRY(entry_text_IV),"Entrer votre IV...");
  gtk_widget_hide((GtkWidget*)dialog_AES_CBC);
  bool_retour_dialog_annuler=1;
}

void DialogHash_cancel(GtkButton *button,GtkEntry *entry){

  gtk_entry_set_text(entry,"");
  gtk_entry_set_text(entry,"Entrer une clé : ");
  gtk_widget_destroy(dialogHash);
}

// Récupération de la clé à utiliser pour HMAC-SHA1
void DialogHash_send(GtkButton *button,GtkEntry *entry){
  const gchar *entry_text;

  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  hmacKey=malloc(sizeof(char*)*strlen(entry_text));
  strcpy(hmacKey,entry_text);
  printf("%s\n",hmacKey );
  gtk_entry_set_text(GTK_ENTRY (entry),"");
  gtk_entry_set_text(GTK_ENTRY (entry),"Entrer une clé : ");
  gtk_widget_hide((GtkWidget*)dialogHash);
}

void DialogRSAP_cancel(GtkButton *button,GtkEntry *entry){

  gtk_entry_set_text(entry,"");
  gtk_entry_set_text(entry,"Entrer une clé RSA : ");
  gtk_widget_destroy(dialogRSAP);

  bool_retour_dialog_annuler_RSA=1;
}


void DialogRSAP_send(GtkButton *button,GtkEntry *entry){
  const gchar *entry_text;

  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  RSAPriv=malloc(sizeof(char *) * strlen(entry_text));
  strcpy(RSAPriv,entry_text);
  gtk_entry_set_text(GTK_ENTRY (entry),"");
  gtk_entry_set_text(GTK_ENTRY (entry),"Entrer une clé RSA : ");
  gtk_widget_hide((GtkWidget*)dialogRSAP);
}

void DialogRSAS_cancel(GtkButton *button,GtkEntry *entry){

  gtk_entry_set_text(entry,"");
  gtk_entry_set_text(entry,"Entrer une clé RSA : ");
  gtk_widget_destroy(dialogRSAS);
  bool_retour_dialog_annuler_RSA=1;

  }

void DialogRSAS_send(GtkButton *button,GtkEntry *entry){
  const gchar *entry_text;

  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  RSASign=malloc(sizeof(char *) * strlen(entry_text));
  strcpy(RSASign,entry_text);
  gtk_entry_set_text(GTK_ENTRY (entry),"");
  gtk_entry_set_text(GTK_ENTRY (entry),"Entrer une clé RSA : ");
  gtk_widget_hide((GtkWidget*)dialogRSAS);
}

void DialogCRT_cancel(GtkButton *button,GtkEntry *entry){

  gtk_entry_set_text(entry,"");
  gtk_entry_set_text(entry,"Entrer une clé RSA : ");
  gtk_widget_destroy(dialogCRT);
  bool_retour_dialog_annuler_RSA=1;

}

void DialogCRT_send(GtkButton *button){
  const gchar *entry_p;
  const gchar *entry_q;
  const gchar *entry_dp;
  const gchar *entry_dq;
  const gchar *entry_ip;

  entry_p = gtk_entry_get_text (GTK_ENTRY (Entry_CRT_P));
  entry_q = gtk_entry_get_text (GTK_ENTRY (Entry_CRT_Q));
  entry_dp = gtk_entry_get_text (GTK_ENTRY (Entry_CRT_DP));
  entry_dq = gtk_entry_get_text (GTK_ENTRY (Entry_CRT_DQ));
  entry_ip = gtk_entry_get_text (GTK_ENTRY (Entry_CRT_IP));

  get_p = malloc(sizeof(char *) * strlen(entry_p));
  get_q = malloc(sizeof(char *) * strlen(entry_p));
  get_dp = malloc(sizeof(char *) * strlen(entry_p));
  get_dq = malloc(sizeof(char *) * strlen(entry_p));
  get_ip = malloc(sizeof(char *) * strlen(entry_p));

  strcpy(get_p, entry_p);
  strcpy(get_q, entry_q);
  strcpy(get_dp, entry_dp);
  strcpy(get_dq, entry_dq);
  strcpy(get_ip, entry_ip);

  //gtk_entry_set_text(GTK_ENTRY (entry),"");
  //gtk_entry_set_text(GTK_ENTRY (entry),"Entrer une clé RSA : ");
  gtk_widget_hide((GtkWidget*)dialogCRT);
}

void DialogRSAK_cancel(GtkButton *button, GtkEntry *entry){
	gtk_entry_set_text(entry, "");
	gtk_entry_set_text(entry, "Entrer une clé : ");
	gtk_widget_destroy(dialogRSAK);
  bool_retour_dialog_annuler_RSA=1;

}


void DialogRSAK_send(GtkButton *button, GtkComboBox *widget){
	GtkComboBox *combo_box = widget;

	if (strcmp(RSAKeyLenChoice, "512 bits") == 0) {
		RSAKeyLen = 512;
	}

	if (strcmp(RSAKeyLenChoice, "1024 bits") == 0) {
		RSAKeyLen = 1024;
	}

	if (strcmp(RSAKeyLenChoice, "2048 bits") == 0) {
		RSAKeyLen = 2048;
	}

	gtk_widget_hide((GtkWidget *)dialogRSAK);
}

void Dialog_click_Ok(GtkButton *button,GtkEntry *entry){
  
  const gchar *entry_text;
  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  if(strlen(entry_text)!=(lenAES*2)+lenAES-1)
  {
    printErreur("Taille de clé incorrecte !");
  }
  else{
    char* cs;
    for (int i = 0; (cs = strtok ((char*)entry_text, " ")); i++)
    {
      int tmp = hexadecimalToDecimal(cs); 
      
      key[i]=tmp;
      //printf("%02X ", (unsigned char) *(key+i));       
      entry_text = NULL;
    
    }

    printf("Fin Dialo\n");
    gtk_entry_set_text(GTK_ENTRY (entry),"");
    gtk_entry_set_text(GTK_ENTRY (entry),"Entrer votre clé AES ...");
    gtk_widget_hide((GtkWidget*)dialog_AES);
    bool_retour_dialog_annuler=0;
  }
}  

// Récupération de la clé pour AES-CBC
void DialogAES_CBC_click_Ok(GtkButton *button,GtkEntry *Key_Entry){
  const gchar *entry_text_key;
  GtkWidget *Entry_AES_IV = GTK_WIDGET(gtk_builder_get_object(builder,"IV"));

  entry_text_key = gtk_entry_get_text (GTK_ENTRY (Key_Entry));
  printf("entry len: %zu et len: %d\n",strlen((char*)entry_text_key),(lenAES*2)+lenAES-1);
  if(strlen((char*)entry_text_key)!=(lenAES*2)+lenAES-1)
  {
    printErreur("Taille de clé incorrecte !");
  }
  else
  {
    char* cs;
    for (int i = 0; (cs = strtok ((char*)entry_text_key, " ")); i++){
      int tmp = hexadecimalToDecimal(cs); 
      key[i]=tmp;
      printf("%02X ", (unsigned char) *(key+i));       
      entry_text_key = NULL;
    }

    

    const gchar* entry_text_IV;
    entry_text_IV = gtk_entry_get_text(GTK_ENTRY (Entry_AES_IV));
    if(strlen(entry_text_IV)== 32+15)
    {
      printf("%s\n",(char*)entry_text_IV );
      
      char* character;
      for (int i = 0; (character = strtok ((char*)entry_text_IV, " ")); i++)
        {
          int tmp = hexadecimalToDecimal(character); 
          
          IV[i]=tmp;
          printf("%02X ", (unsigned char) *(IV+i));       
          entry_text_IV = NULL;
          
        }

      bool_retour_dialog_annuler=0;
      free(entry_text_key);
    free(entry_text_IV);
    }else{
      printErreur("Taille de l'IV incorrecte !");
    }
    
  }
}

    



uint8_t ** InitTableau(int size){
  uint8_t **tab = malloc (sizeof(uint8_t*) * size);
  for (int i = 0; i < size; i++)
  {
    tab[i]=malloc(sizeof(uint8_t*)*16);
  }
  return tab;
}  

unsigned char **  TraitementFile(char* mode,unsigned char** chiffre,int lenght,int * lenPaddedMsg,char * filename,GtkFileChooserButton *btn, int ChooseMode){
  int len, lenPaddedMessage;
  uint8_t **buf;

  FILE* file;
  if(strcmp(mode,"rb")==0 && ChooseMode==0)
  {
    file = fopen(filename, mode);

    if (file){
      fseek (file, 0, SEEK_END);
      len = ftell (file);
      printf("%d\n",len );
      if(len % 16 !=0)
        lenPaddedMessage = (len/16 +1); 

      *lenPaddedMsg=lenPaddedMessage;
      fseek (file, 0, SEEK_SET);
      buf = InitTableau(lenPaddedMessage);
      int lenPadde = len/16+1;
      for (int i = 0; i < lenPaddedMessage-1; i++)
      {
        for (int j = 0; j < 16; j++)
        {
          char tmp = fgetc(file);
          //printf("le tmp : %c\n",tmp);

          if(tmp!='\n')
          {

            buf[i][j]=tmp;
          }else
          {
            buf[i][j]='\n';
          }
        }
      }
      int reste = len%16;
      int last =lenPadde-1;
      if(reste!=0)
      {
        fgets((uint8_t*)buf[last], reste, file);
        printf("je plante ici ?\n");
        for (int i = reste; i < 16; i++)
        {
        //strcpy(buf[1][i],(uint8_t)0x00);
        buf[last][i]=(uint8_t)0x00; 
        }
      }
      

    }
          
          printf("je plante  ?\n");
  } 

  if(strcmp(mode,"rb")==0 && ChooseMode==1)
  {
   file = fopen(filename, mode);

    if (file){
      fseek (file, 0, SEEK_END);
      len = ftell (file);

      *lenPaddedMsg=len;
      printf("Longueur test : %d\n", len);

      fseek (file, 0, SEEK_SET);
      buf = malloc (sizeof(char*) * len);

      fgets(buf,len+1,file);
    }
  } 

  if(strcmp(mode,"w")==0 && ChooseMode==0)
  {
    char * path = strtok ((char*)filename,".");
    strcat(path,".enc");
    printf("%s\n",path );
    file = fopen(path, "w");
    if (file != NULL)
    { 
      for (int y = 0; y < lenght; y++)
        for (int i = 0; i < 16; i++)
          fprintf(file,"%02X ",(unsigned char) *(chiffre[y]+i) );
    }
    gtk_file_chooser_set_filename (btn,path);
  }


  if(strcmp(mode,"w")==0 && ChooseMode==1)
  {
    char * path = strtok ((char*)filename,".");
    strcat(path,".clair");
    printf("%s\n",path);
    file = fopen(path, "w");

    if (file != NULL)
    { 
      for (int i = 0; i < lenght/16; i++)
      {
        for (int j = 0; j < 16; j++)
        {
          char tmp = chiffre[i][j];
          fputc(tmp,file);
        }
      }
    }
    gtk_file_chooser_set_filename (btn,path);
  }

  fclose (file);


  return buf;
}

GtkWidget * ReturnDialogAES(char * TypeAES){
  GtkWidget * AESDialog;

  if(strcmp("AES 128",TypeAES)==0 || strcmp("AES 192",TypeAES)==0 || strcmp("AES 256",TypeAES)==0)
  {
     AESDialog = GTK_WIDGET(dialog_AES) ;
     Entry_AES = GTK_ENTRY(Entry_AES_KEY);
  }
  else{
     AESDialog = GTK_WIDGET(dialog_AES_CBC);
     Entry_AES = GTK_ENTRY(Entry_AES_KEY_CBC);
  }
  return AESDialog;
}  

// Chiffrement de fichier avec AES
void Chiffrement_Fichier(GtkButton *button,GtkFileChooserButton *btn){
  char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(btn));
  printf("%s et type %s\n",filename ,TypeAES);

  if(TypeAES==NULL)
    printErreur("Vous devez choisir votre AES !");
  else{
    if(filename==NULL)
      printErreur("Selectionner un fichier !");
    else{
      int lenPaddedMessage;
      
      uint8_t ** clair= TraitementFile("rb",NULL,0,&lenPaddedMessage,filename,btn, 0);
      GtkWidget * AESDialog = ReturnDialogAES(TypeAES);
      //GtkDialog * AESDialog = ReturnDialogAES(TypeAES);
      gtk_entry_set_text(Entry_AES,keyHex);          
      gtk_dialog_run(GTK_DIALOG(AESDialog));     

      uint8_t **enc_msg;
      if(bool_retour_dialog_annuler==0)
      {
        enc_msg = Main_AES(TypeAES,clair,key,lenPaddedMessage,IV);
        printf("je passe ici\n");
        char** bu =TraitementFile("w",enc_msg,lenPaddedMessage,NULL,filename,btn, 0);
        bool_retour_dialog_annuler=0;
      }
    }
  }
 
}

// Déchiffrement de fichier avec AES
void Dechiffrement_Fichier(GtkButton *button,GtkFileChooserButton *btn){
  char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(btn));
  printf("%s et type %s\n",filename ,TypeAES);


  if(TypeAES==NULL)
    printErreur("Vous devez choisir votre AES !");
  else{
    if(filename==NULL)
      printErreur("Selectionner un fichier !");
    else{
      int lenPaddedMessage;
      uint8_t ** chiffre=TraitementFile("rb",NULL,0,&lenPaddedMessage,filename,btn, 1);
      int lenChiffre = strlen(chiffre);

        gtk_entry_set_text(GTK_ENTRY(Entry_AES),keyHex);
        GtkWidget * AESDialog = ReturnDialogAES(TypeAES);
        gtk_dialog_run(GTK_DIALOG(AESDialog));

      uint8_t **plaintext;
      if(bool_retour_dialog_annuler==0)
      {
        plaintext = InvMain_AES(TypeAES,chiffre,key,&lenChiffre,IV,1);

        char** bu =TraitementFile("w",plaintext,lenChiffre,NULL,filename,btn, 1);
        //TraitementFile("w",plaintext,lenPaddedMessage,NULL,NULL);
        bool_retour_dialog_annuler=1;
      }
    } 
  }


}

char * Afficher_AES_Label(GtkTextBuffer *buffer,GtkTextIter end,uint8_t** msg,GtkTextView *text_label,int size){
  int decimal;
  char * result_history;
  result_history=(char *)calloc(size,sizeof(char*));
  char tmp[16];
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_end_iter(buffer,&end);
  char * msgEncrypt;

  for (int i = 0; i < 16; ++i) 
  {
    decimal=msg[0][i];
    sprintf(tmp,"%02X",decimal);
    if (i==15)
      msgEncrypt= strcat(tmp,"\0");
    else
      msgEncrypt= strcat(tmp," ");
    strcat(result_history,tmp);

    gtk_text_buffer_insert(buffer,&end,msgEncrypt,-1); 
  }
  return result_history;
}

// Fonction de chiffrement AES
void Encrypt_AES(GtkButton *button,GtkTextView *text_label){

  GtkTextBuffer *buffer;
  GtkTextIter start,end;
  char* message;
  char * history;
  //ON recupere le message dans la textBox si il est pas trop grand
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  message=gtk_text_buffer_get_text(buffer,&start,&end,-1);
  history=malloc(sizeof(char*)*sizeof(message));

  strcpy(history,message);

  if(TypeAES==NULL)
    printErreur("Vous devez choisir votre AES !");
  else{
    if(strlen(message)==0)
      printErreur("Aucun données a chiffrer !");
    else{
       if ((strlen(message)!=47))
        printErreur("La longueur du bloc doit etre de 16 !");
      else{
        //Fonction AES
        uint8_t **MsgToAES; 
        char * cs;
        MsgToAES=malloc(sizeof(uint8_t*)*1);
        MsgToAES[0]=malloc(sizeof(uint8_t*)*16);
   
        for (int i = 0; (cs = strtok ((char*)message," ")); i++)
        {
          int tmp = hexadecimalToDecimal(cs); 
          MsgToAES[0][i]=(uint8_t)tmp;
          message = NULL; 
        }

          GtkWidget * AESDialog = ReturnDialogAES(TypeAES);
          //GtkDialog * AESDialog = ReturnDialogAES(TypeAES);
          gtk_entry_set_text(Entry_AES,keyHex);          
          gtk_dialog_run(GTK_DIALOG(AESDialog));

        if (bool_retour_dialog_annuler==0)
        {
          uint8_t ** msg = Main_AES(TypeAES,MsgToAES,key,1,IV); 
          //char tmp[16];
          //int decimal;
          //Pour supprimer
          buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
          gtk_text_buffer_get_end_iter(buffer,&end);
          gtk_text_buffer_delete(buffer,&start,&end);
          //Pour ajouter
                    printf("sizeof %d et strlen: %d\n",sizeof(msg),strlen(msg) );

          char *result_history=Afficher_AES_Label(buffer,end,msg,text_label,16);
          Historique(history,result_history,TypeAES,"");
          bool_retour_dialog_annuler=0;

        }   

 
    free(MsgToAES);  
      }
    }
    gtk_entry_set_text(Entry_AES_KEY," ");
    free(history);
   
  } 
}

// Fonction de déchiffrement AES
void Decrypt_AES(GtkButton *button,GtkTextView *text_label){
  GtkTextBuffer *buffer;
  GtkTextIter start,end;
  char* message;
  char * history;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  message=gtk_text_buffer_get_text(buffer,&start,&end,-1);
  history=malloc(sizeof(char*)*sizeof(message));
  strcpy(history,message);
  

  if(TypeAES==NULL)
    printErreur("Vous devez choisir votre AES !");
  else{
  if(file_path==NULL && (strlen(message)==0))
      printErreur("Aucun données a chiffrer !");           
  else{
      if ((strlen(message)!=47))

        printErreur("La longueur du bloc doit etre de 16 !");
      else{
          GtkWidget * AESDialog = ReturnDialogAES(TypeAES);
          //GtkDialog * AESDialog = ReturnDialogAES(TypeAES);
           gtk_entry_set_text(Entry_AES,keyHex);          
          gtk_dialog_run(GTK_DIALOG(AESDialog));

          uint8_t **MsgToAES; 
          char * cs;
          MsgToAES=malloc(sizeof(uint8_t*)*1);
          MsgToAES[0]=malloc(sizeof(uint8_t*)*16);

          for (int i = 0; (cs = strtok ((char*)message, " ")); ++i)
          {
            int tmp = hexadecimalToDecimal(cs); 
            MsgToAES[0][i]=tmp;      
            message = NULL;
            
          }

        if (bool_retour_dialog_annuler==0)
        {
          int lenChiffre = 1;

          uint8_t ** msg = InvMain_AES(TypeAES,MsgToAES,key,&lenChiffre,IV,0);
          buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
          gtk_text_buffer_get_end_iter(buffer,&end);
          gtk_text_buffer_delete(buffer,&start,&end);
                    printf("ICI\n");

          
          char *result_history=Afficher_AES_Label(buffer,end,msg,text_label,16);
          Historique(history,result_history,TypeAES,"");
          printf("ICI\n");
          bool_retour_dialog_annuler=0;

        }
        free(MsgToAES);         
      }
      
    }

  } 
  free(history);
  
}



void Changed_AES(GtkComboBox *comboBox){
  gint index = -1;
  index = gtk_combo_box_get_active (GTK_COMBO_BOX(comboBox));
  if (index != -1) {
    TypeAES= gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(comboBox));
   

    if(strcmp(TypeAES,"AES 128")==0 || strcmp(TypeAES,"AES 128 CBC")==0)
      lenAES=16;
    else if (strcmp(TypeAES,"AES 192")==0 || strcmp(TypeAES,"AES 192 CBC")==0)
      lenAES=24;
    else
      lenAES=32;
  
    key=(unsigned char*) realloc(key,sizeof(unsigned char *)*lenAES);
    keyHex=(unsigned char*) realloc(keyHex,sizeof(unsigned char *)*lenAES*3);
       
  }
}

void convertisseur_hexa_dec(const gchar * Label_RadioButton,GtkTextView *text_label){
  GtkTextBuffer *buffer;
  GtkTextIter start,end;
  int bool_conve=0;
  char *hist;
  char msgconvert[100]={""};
  int BoolSaveHistorique=0;
  char * msg;
 
  char *token,*op1_historique,*op2_historique;
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  int size_buffer=gtk_text_buffer_get_char_count(buffer);
  if(size_buffer!=0)
  {
    msg = malloc(sizeof(char *)*size_buffer);
    msg=gtk_text_buffer_get_text(buffer,&start,&end,-1);
    printf("is digit: %d\n", isalpha(msg[0]));
    hist=malloc(sizeof(char*)*sizeof(msg));
    strcpy(hist,msg);


      if (strcmp(Label_RadioButton,"Héxadécimal")==0)
      {
        token=strtok(msg," ");
        for (int i = 0; i < size_buffer; ++i)
        {
          printf("%d\n",isalpha(msg[i]) );
          if(isalpha(msg[i]))
          {
            bool_conve=1;
            break;
          }
        }

        if(bool_conve!=1)
        {
          while( token != NULL )
          {
            char tmp[10];
            printf("%d\n",atoi(token) );
            
            sprintf(tmp,"%02X",atoi(token));
            strcat(tmp," ");
            strcat(msgconvert,tmp);        
            token = strtok(NULL," ");
          }
          op1_historique="DEC";
          op2_historique="HEX";
        } 
        else{
          strcat(msgconvert,hist);
          BoolSaveHistorique=1;
        }
        //sprintf(msgconvert,"%02X",atoi(msg));
        gtk_text_buffer_delete(buffer,&start,&end);
        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
        gtk_text_buffer_insert(buffer,&end,msgconvert,-1);
      }else{
        op1_historique="HEX";
        op2_historique="DEC";
        token=strtok(msg," ");
        while( token != NULL )
        {
          char tmp[10];
          int decimal=hexadecimalToDecimal(token);
          sprintf(tmp,"%d",decimal);
          strcat(tmp," ");
          strcat(msgconvert,tmp);
          token = strtok(NULL," ");
         
        } 
        gtk_text_buffer_delete(buffer,&start,&end);
        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
        gtk_text_buffer_insert(buffer,&end,msgconvert,-1);
        
      }

    if(BoolSaveHistorique!=1)
      Historique(hist,msgconvert,op1_historique,op2_historique);

  }
}

void Convertisseur(GtkToggleButton *button,GtkTextView *text_label){
  GSList * group = gtk_radio_button_get_group (GTK_RADIO_BUTTON(button));
  for (; group != NULL; group = group->next) {
    GtkRadioButton *radio = group->data;
     
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (radio))) {
      printf("%s\n",gtk_button_get_label(GTK_BUTTON(radio)) );
      convertisseur_hexa_dec(gtk_button_get_label (GTK_BUTTON (radio)),text_label);      
      break;
    }
  }
}

// Fonction de génération d'une clé AES
void generateAESKey(GtkButton *button,GtkTextView *text_label){
  if(TypeAES==NULL)
    printErreur("Vous devez choisir votre AES !");
  else{
    free(keyHex);
    keyHex = (unsigned char *) calloc(lenAES*3,sizeof(unsigned char*)*lenAES*3);

    printf("%d\n",lenAES );
    //char AESKey[lenAES];
    //char tmp[lenAES];
    mpz_t number_generate;
    gmp_randstate_t prng;
    gmp_randinit_default(prng);
    unsigned long int seed;
    seed=time(NULL);
    gmp_randseed_ui(prng,seed);
    mpz_inits(number_generate,NULL);

    for (int i = 0; i < lenAES; i++)
    {
      char tmp[3];
      mpz_urandomb(number_generate,prng,8);
      key[i]=mpz_get_ui(number_generate);
      sprintf(tmp,"%02X",key[i]);
      if(i==lenAES-1)
        strcat(tmp,"\0");
      else
        strcat(tmp," ");
      strcat(keyHex,tmp);
    }
    printf("Clé AES générée : \n%s\n",keyHex);

  }
}

// Gestion des entrées/sorties du pavé numérique
void on_btn_0_clicked(GtkButton *button, GtkTextView *text_label){
   
  GtkTextBuffer *buffer;
  GtkTextIter iter;
  
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  
  gtk_text_buffer_get_end_iter(buffer,&iter);
  //sscanf(,"%s",&i);
  int i= hexadecimalToDecimal(gtk_button_get_label(button));
  switch(i)
  {
    case 0:
    {
      gtk_text_buffer_insert(buffer,&iter,"0",-1);
      break;
    }

    case 1:
    {
      gtk_text_buffer_insert(buffer,&iter,"1",-1);
      break;
    }
     case 2:
    {
      gtk_text_buffer_insert(buffer,&iter,"2",-1);
      break;
    }
     case 3:
    {
      gtk_text_buffer_insert(buffer,&iter,"3",-1);
      break;
    }
     case 4:
    {
      gtk_text_buffer_insert(buffer,&iter,"4",-1);
      break;
    }
     case 5:
    {
      gtk_text_buffer_insert(buffer,&iter,"5",-1);
      break;
    }
     case 6:
    {
      gtk_text_buffer_insert(buffer,&iter,"6",-1);
      break;
    }
     case 7:
    {
      gtk_text_buffer_insert(buffer,&iter,"7",-1);
      break;
    }
     case 8:
    {
      gtk_text_buffer_insert(buffer,&iter,"8",-1);
      break;
    }
     case 9:
    {
      gtk_text_buffer_insert(buffer,&iter,"9",-1);
      break;
    }
    case 10:
    {
      gtk_text_buffer_insert(buffer,&iter,"A",-1);
      break;
    }
    case 11:
    {
      gtk_text_buffer_insert(buffer,&iter,"B",-1);
      break;
    }
    case 12:
    {
      gtk_text_buffer_insert(buffer,&iter,"C",-1);
      break;
    }
    case 13:
    {
      gtk_text_buffer_insert(buffer,&iter,"D",-1);
      break;
    }
    case 14:
    {
      gtk_text_buffer_insert(buffer,&iter,"E",-1);
      break;
    }
    case 15:
    {
      gtk_text_buffer_insert(buffer,&iter,"F",-1);
      break;
    }

  }
}

void on_click_rsa(GtkButton *button, GtkTextView *text_label) {
	GtkTextBuffer *buffer;
	GtkTextIter start, end;
	char *input;
  char *chiffre;
  char *dechiffre;
  int boolChiffr = 0;

	mpz_set_ui(e, 11);

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);
    if (strcmp(rsa, "Génération de clés RSA") == 0) {
      gtk_dialog_run(GTK_DIALOG(dialogRSAK));
      mpz_set_ui(k, RSAKeyLen);
      RSA_CRT_Gen_Key(p, q, n, dp, dq, ip, k, e, d);

      gtk_text_buffer_delete(buffer, &start, &end);
      buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
      char *key = mpz_get_str(NULL, 16, n);
      gtk_text_buffer_insert(buffer, &end, key, -1);
      
      /*char head[30];
      char footer[28];
      strcpy(head, "-----BEGIN RSA PUBLIC KEY-----\n");
      strcpy(footer, "-----END RSA PUBLIC KEY-----");
      strcpy(key, mpz_get_str(NULL, 10, n));
      strcat(head, key);
      strcat(head, footer);
      printf("%s ", head);*/

    }
    else {
    
    if (strcmp(rsa, "Chiffrement RSA") == 0 ) {
      boolChiffr = 1;
      if (mpz_get_ui(k) == 0)
      {
        printErreur("Veuillez générer une clé");
    }
    else {
        input = gtk_text_buffer_get_text(buffer, &start, &end, -1);
        /*int valeur = 0;
        char buf[strlen(input)];
        char *finalInput = malloc(sizeof(char) * strlen(buf));
        for (int i = 0; i < strlen(input); i++)
        {
          finalInput[i] = '\0';
        }
        for (int i = 0; i < strlen(input); i++) {
          int j = (int) input[i];
          valeur = sprintf(buf, "%d", j);
          //printf("%d", j);
          if(j < 100) {
            if (j < 10) {
              strcat(finalInput, "9");
            }
            strcat(finalInput, "9");
          }
          strcat(finalInput, buf);
          
          }
          printf("%s ", finalInput);
          mpz_set_str(m, finalInput, 10);*/
        mpz_set_str(m, input, 10);
        RSA_Encrypt(c, m, e, n);
        chiffre = mpz_get_str(NULL, 10, c);

        gtk_text_buffer_delete(buffer, &start, &end);
        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
        gtk_text_buffer_insert(buffer, &end, chiffre, -1);
    }
    }

    if (strcmp(rsa, "Déchiffrement RSA") == 0 ) {
     if (mpz_get_ui(k) == 0) {
      printErreur("Veuillez générer une clé");
    }
    else {
      gtk_dialog_run(GTK_DIALOG (dialogRSAP));
      input = gtk_text_buffer_get_text(buffer, &start, &end, -1);
      mpz_set_str(c, input, 10);
      RSA_Decrypt(m, c, d, n);
      dechiffre = mpz_get_str(NULL, 10, m);
      //printf("%s ", dechiffre);
      /*
      char *final;
      for (int i = 0; i < strlen(dechiffre); i++) {
        if (dechiffre[i] == '9' ) {
          if (i < strlen(dechiffre)) {
            if (dechiffre[i+1] == '9'){
              strcat(final, dechiffre[i+2]);
              exit(0);
          } 
          else {
            strcat(final, dechiffre[i + 1]);
            strcat(final, dechiffre[i + 2]);
          }
        }
        else {
          strcat(final, dechiffre[i]);
        }
      }
      else {
        strcat(final, dechiffre[i]);
        strcat(final, dechiffre[i + 1]);
        strcat(final, dechiffre[i + 2]);
      }
  }
      exit(0);*/

      gtk_text_buffer_delete(buffer, &start, &end);
      buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
      gtk_text_buffer_insert(buffer, &end, dechiffre, -1);
    }
    }
    if (strcmp(rsa, "Déchiffrement RSA CRT") == 0 ) {
     if (mpz_get_ui(k) == 0) {
      printErreur("Veuillez générer une clé");
    }
    else {

    gtk_dialog_run(GTK_DIALOG (dialogCRT));

    mpz_set_str(p,get_p,10);
    mpz_set_str(q,get_q,10);
    mpz_set_str(dp,get_dp,10);
    mpz_set_str(dq,get_dq,10);
    mpz_set_str(ip,get_ip,10);

      input = gtk_text_buffer_get_text(buffer, &start, &end, -1);
      mpz_set_str(c, input, 10);
      RSA_CRT_Decrypt(m, c, p, q, dp, dq, ip);

     dechiffre = mpz_get_str(NULL, 10, m);

      gtk_text_buffer_delete(buffer, &start, &end);
      buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
      gtk_text_buffer_insert(buffer, &end, dechiffre, -1);
    }
    }
    if (strcmp(rsa, "Signature RSA") == 0 ) {
     if (mpz_get_ui(k) == 0) {
      printErreur("Veuillez générer une clé");
    }
    else {
      gtk_dialog_run(GTK_DIALOG (dialogRSAS));
      printf("%s\n", RSASign);

        input = gtk_text_buffer_get_text(buffer, &start, &end, -1);
        mpz_set_str(m, input, 10);
        RSA_Signature(sign, m, d, n);

        dechiffre = mpz_get_str(NULL, 10, sign);

        gtk_text_buffer_delete(buffer, &start, &end);
        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
        gtk_text_buffer_insert(buffer, &end, dechiffre, -1);
    }
    }
    if (strcmp(rsa, "Vérification") == 0 ) {
      
     if (mpz_get_ui(k) == 0) {
      printErreur("Veuillez générer une clé");
    }
    else {
      input = gtk_text_buffer_get_text(buffer, &start, &end, -1);
      mpz_set_str(sign, input, 10); 
      char *myM = mpz_get_str(NULL, 10, m);
      RSA_Verif(m, sign, e, n);
      gtk_text_buffer_delete(buffer, &start, &end);
      buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
      char *mySign = mpz_get_str(NULL, 10, m);
      printf("My m :%s \n ", myM);
      printf("MySIgn %s \n", mySign);
      if (strcmp(myM, mySign) == 0) {
        boolChiffr = 2;
        gtk_text_buffer_insert(buffer, &end, "Verif ok", -1);
      }
      else {
        boolChiffr = 3;
        gtk_text_buffer_insert(buffer, &end, "Verif pas valide", -1);
      }

      //char *verif = mpz_get_str(NULL, 10, m);
    }

  }

    char tmp[15] = "RSA ";
    strcat(tmp, RSAKeyLenChoice);
    printf("%s ", tmp);
    if (boolChiffr == 0)
    {
      Historique(input, dechiffre, tmp, "");
    }
    if (boolChiffr == 1) {
      Historique(input, chiffre, tmp, "");
    }
    if (boolChiffr == 2) {
      Historique(input, "Verif OK", tmp, "");
    }
    if (boolChiffr == 3)
    {
      Historique(input, "Verif pas valide", tmp, "");
    }
}
}

void on_click_hash(GtkButton *button, GtkTextView *text_label){
	GtkTextBuffer *buffer;
	GtkTextIter start, end;
	char *input;
	char *hash;

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);

  if (hashAlgo==NULL){
  	printErreur("Choisir un algo de hachage");
  }

  else{ 	

	  if (strcmp(hashAlgo, "MD5") == 0){
	    input = gtk_text_buffer_get_text(buffer,&start,&end,-1);
	    hash = md5digest(input);

	    gtk_text_buffer_delete(buffer,&start,&end);
	    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
	    gtk_text_buffer_insert(buffer,&end,hash,-1);
	    }

	  if (strcmp(hashAlgo, "SHA1") == 0){
	    input = gtk_text_buffer_get_text(buffer,&start,&end,-1);
	    hash = sha1digest(input);

	    gtk_text_buffer_delete(buffer,&start,&end);
	    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
	    gtk_text_buffer_insert(buffer,&end,hash,-1);
	    }
	 
	    if (strcmp(hashAlgo, "SHA2") == 0){ 


	    input = gtk_text_buffer_get_text(buffer,&start,&end,-1);
	    hash = sha2_appel(input);  

	    gtk_text_buffer_delete(buffer,&start,&end); 
	    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
	    gtk_text_buffer_insert(buffer,&end,hash,-1);
	    }

	    if (strcmp(hashAlgo, "SHA3") == 0){ 

	    input = gtk_text_buffer_get_text(buffer,&start,&end,-1);
	    hash = sha3_keccak(input);  

	    gtk_text_buffer_delete(buffer,&start,&end); 
	    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
	    gtk_text_buffer_insert(buffer,&end,hash,-1);
	    }

	  if (strcmp(hashAlgo, "HMAC-SHA1") == 0){


	    gtk_dialog_run(GTK_DIALOG (dialogHash));

	    input = gtk_text_buffer_get_text(buffer,&start,&end,-1);
	    hash = hmac_sha1(hmacKey,input);

	    gtk_text_buffer_delete(buffer,&start,&end);
	    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
	    gtk_text_buffer_get_start_iter(buffer, &start);     
	    gtk_text_buffer_get_end_iter(buffer, &end);
	    gtk_text_buffer_insert(buffer,&end,hash,-1);
	    
	  }
	    Historique(input,hash,hashAlgo,"");
	}
}

// Fonctions de hachage d'un fichier en fonction de l'algo choisi 
void on_hash_file(GtkButton *button, GtkTextView *text_label){

  GtkTextBuffer * buffer;
  GtkTextIter start,end;
  char *input;
  char *hash;

  if (file_path==NULL)
  	printErreur("Veuillez importer un fichier");

  else{  
	  input =readFile(file_path);

	  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
	  gtk_text_buffer_get_start_iter(buffer, &start);
	  gtk_text_buffer_get_end_iter(buffer, &end);

	  if (hashAlgo==NULL)
	  	printErreur("Choisir un algo de hachage");

	  else{

		  if (strcmp(hashAlgo, "MD5") == 0){

		    hash = md5digest(input);

		    gtk_text_buffer_delete(buffer,&start,&end);
		      gtk_text_buffer_get_start_iter(buffer, &start);     
		      gtk_text_buffer_get_end_iter(buffer, &end);
		      gtk_text_buffer_insert(buffer,&end,hash,-1);
		  }

		  if (strcmp(hashAlgo, "SHA1") == 0){

		    hash = sha1digest(input);

		    gtk_text_buffer_delete(buffer,&start,&end);
		      gtk_text_buffer_get_start_iter(buffer, &start);     
		      gtk_text_buffer_get_end_iter(buffer, &end);
		      gtk_text_buffer_insert(buffer,&end,hash,-1);
		  }

		  if (strcmp(hashAlgo, "SHA2") == 0){

		    hash = sha2_appel(input);

		    gtk_text_buffer_delete(buffer,&start,&end);
		      gtk_text_buffer_get_start_iter(buffer, &start);     
		      gtk_text_buffer_get_end_iter(buffer, &end);
		      gtk_text_buffer_insert(buffer,&end,hash,-1);
		  }

		  if (strcmp(hashAlgo, "SHA3") == 0){

		    hash = sha3_keccak(input);


		    gtk_text_buffer_delete(buffer,&start,&end);
		      gtk_text_buffer_get_start_iter(buffer, &start);     
		      gtk_text_buffer_get_end_iter(buffer, &end);
		      gtk_text_buffer_insert(buffer,&end,hash,-1);
		  }

		  if (strcmp(hashAlgo, "HMAC-SHA1") == 0){

		    gtk_dialog_run(GTK_DIALOG (dialogHash));

		    hash = hmac_sha1(hmacKey,input);

		    gtk_text_buffer_delete(buffer,&start,&end);
		      gtk_text_buffer_get_start_iter(buffer, &start);     
		      gtk_text_buffer_get_end_iter(buffer, &end);
		      gtk_text_buffer_insert(buffer,&end,hash,-1);

		    }
		}
	} 
}

void on_change_key(GtkButton *button, GtkEntry *entry){
  GtkWidget *Image_Widget=gtk_button_get_image (button);
  if(strcmp(gtk_widget_get_name (Image_Widget),"hide")==0)
  {
    gtk_button_set_image(button,eye_image);
    gtk_entry_set_visibility (entry,FALSE);
  }
  else
  {
    gtk_button_set_image(button,hide_image);
    gtk_entry_set_visibility (entry,TRUE);
  }
}