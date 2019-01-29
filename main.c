#include "sha2/sha2.c"
#include "sha2/fonctionSha2.c"
#include "sha2/traitementSha2.c"
#include "historique/main_historique.c"
#include <gmp.h>

#include <gtk/gtk.h>
#include "AES/AES_encrypt.c"
#include "AES/AES_decrypt.c"
#include "hashlib.c"


gchar * distro ;
gchar * algo;
gchar * AES;
gchar * Combobox1;
gchar * Combobox2;
gchar * algo_value;
gchar * TypeAES;
gchar *hmacKey;
char* filename;
int lenAES;

unsigned char* key;
unsigned char * IV=NULL;

GtkWidget *window;
GtkWidget *dialog_AES;
GtkWidget *dialog_AES_CBC;
GtkWidget *dialogHash;

GtkEntry *Entry_AES_IV;
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

void Historique(char* op1,char* op2,char* type,char* type2){

  int taille =SizeFile(maFile);
  printf("%d\n", taille);
  char tmp[1024];
  if(type2!=NULL){
    sprintf(tmp,"\t%s(%s)  =  %s(%s)\n",op1,type,op2,type2);
  }
  else{
    sprintf(tmp,"\t%s(%s)  =  %s\n",op1,type,op2);
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
  gtk_builder_add_from_file (builder, "test.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));

  dialog_AES = GTK_WIDGET(gtk_builder_get_object(builder, "DialogAES"));
  dialog_AES_CBC = GTK_WIDGET(gtk_builder_get_object(builder, "DialogAES_CBC"));
  dialogHash = GTK_WIDGET(gtk_builder_get_object(builder, "DialogHash"));
  
  label = GTK_WIDGET(gtk_builder_get_object(builder,"old_calcul"));
  
  gtk_window_set_transient_for(GTK_WINDOW(dialog_AES),GTK_WINDOW(window));
  gtk_window_set_attached_to(GTK_WINDOW(window),dialog_AES);

  gtk_window_set_transient_for(GTK_WINDOW(dialog_AES_CBC),GTK_WINDOW(window));
  gtk_window_set_attached_to(GTK_WINDOW(window),dialog_AES_CBC);

  gtk_window_set_transient_for(GTK_WINDOW(dialogHash),GTK_WINDOW(window));
  gtk_window_set_attached_to(GTK_WINDOW(window),dialogHash);

  gtk_builder_connect_signals(builder, NULL);

  maFile = initialiser();
  maFile = Lecture();
  afficherFile(maFile,label); 
  g_signal_connect (G_OBJECT(window), "destroy",G_CALLBACK(destroy), NULL);  

  gtk_widget_show(window);                
  gtk_main();


  return 0;}

void Changed_combo2(GtkComboBox *widget, GtkTextView *text_label){
  GtkComboBox *combo_box = widget;
  GtkTextBuffer *buffer;
  GtkTextIter iter;
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_end_iter(buffer,&iter);

  gint index = -1;
  index = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_box));

  if (index!= -1) {
    distro= gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo_box));    
    int i = gtk_combo_box_get_active(combo_box);
    Combobox2=distro;
    g_print ("You chose %s ; %d\n", distro, i);
  }
}

void ChangeCombo1(GtkComboBox *widget, GtkTextView *text_label){
  GtkComboBox *combo_box = widget;
  GtkTextBuffer *buffer;
  GtkTextIter iter;
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_end_iter(buffer,&iter);

  gint index = -1;
  index = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_box));

  if (index != -1) {
    distro= gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo_box));
    int i = gtk_combo_box_get_active(combo_box);
    Combobox1 = distro;
   g_print ("You chose %s ; %d\n", distro, i);

  } 
}

void import_path(GtkFileChooserButton *btn){

  filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(btn));
  printf("%s\n",filename );
  gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(btn));
}


void hashList(GtkComboBox *widget){
  GtkComboBox *combo_box = widget;

  gint index = -1;
  index = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_box));
  
  if (index != -1) {
    algo= gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo_box));
    //int i = gtk_combo_box_get_active(combo_box);
    algo_value = algo;
   g_print ("You chose %s\n", algo);
   
  }  
}

void printErreur(char *msg){ 
  GtkWidget*  dialog;
  dialog = gtk_message_dialog_new (GTK_WINDOW(window) ,
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_CLOSE,
                                  msg);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);}
 
void Dialog_click_Annuler(GtkButton *button,GtkEntry *entry){
  gtk_entry_set_text(entry,"");
  gtk_entry_set_text(entry,"Entrer votre clé AES ...");
  gtk_widget_hide((GtkWidget*)dialog_AES); 
 }

void Dialog_AES_CBC_click_Annuler(GtkButton *button,GtkEntry *entry)
{
  const gchar *entry_text_IV;
  entry_text_IV = gtk_entry_get_text (GTK_ENTRY (Entry_AES_IV));
  gtk_entry_set_text(entry,"");
  gtk_entry_set_text(entry,"Entrer votre clé AES ...");
  gtk_entry_set_text(GTK_ENTRY(entry_text_IV),"");
  gtk_entry_set_text(GTK_ENTRY(entry_text_IV),"Entrer votre IV...");
  gtk_widget_hide((GtkWidget*)dialog_AES_CBC);
}

void DialogHash_cancel(GtkButton *button,GtkEntry *entry){

  gtk_entry_set_text(entry,"");
  gtk_entry_set_text(entry,"Entrer une clé : ");
  gtk_widget_destroy(dialogHash);}

void DialogHash_send(GtkButton *button,GtkEntry *entry){
  const gchar *entry_text;

  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  hmacKey=malloc(strlen(entry_text));
  strcpy(hmacKey,entry_text);
  printf("%s\n",hmacKey );
  gtk_entry_set_text(GTK_ENTRY (entry),"");
  gtk_entry_set_text(GTK_ENTRY (entry),"Entrer une clé : ");
  gtk_widget_hide((GtkWidget*)dialogHash);
}

void Dialog_click_Ok(GtkButton *button,GtkEntry *entry){
  
  const gchar *entry_text;
  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  if(strlen(entry_text)!=(lenAES*2)+lenAES-1)
  {
    printErreur("Taille de clé incorrect !");
  }
  else{
    key = malloc(lenAES);
    char* cs;
    for (int i = 0; (cs = strtok ((char*)entry_text, " ")); i++)
    {
      int tmp = hexadecimalToDecimal(cs); 
      
      key[i]=tmp;
      printf("%02X ", (unsigned char) *(key+i));       
      entry_text = NULL;
      
    }
    gtk_entry_set_text(GTK_ENTRY (entry),"");
    gtk_entry_set_text(GTK_ENTRY (entry),"Entrer votre clés AES ...");
    gtk_widget_hide((GtkWidget*)dialog_AES);
  }
}  

void DialogAES_CBC_click_Ok(GtkButton *button,GtkEntry *Key_Entry){
  const gchar *entry_text_key;
  GtkWidget *Entry_AES_IV = GTK_WIDGET(gtk_builder_get_object(builder,"IV"));

  entry_text_key = gtk_entry_get_text (GTK_ENTRY (Key_Entry));
  printf("entry len: %zu et len: %d\n",strlen((char*)entry_text_key),(lenAES*2)+lenAES-1);
  if(strlen((char*)entry_text_key)!=(lenAES*2)+lenAES-1)
  {
    printErreur("Taille de clé incorrect !");
  }
  else{
    key = malloc(lenAES);
    char* cs;
    for (int i = 0; (cs = strtok ((char*)entry_text_key, " ")); i++)
    {
      int tmp = hexadecimalToDecimal(cs); 
      key[i]=tmp;
      printf("%02X ", (unsigned char) *(key+i));       
      entry_text_key = NULL;
      
    }
  }
  const gchar* entry_text_IV;
  entry_text_IV = gtk_entry_get_text(GTK_ENTRY (Entry_AES_IV));
  printf("%s\n",(char*)entry_text_IV );
  IV = malloc(sizeof(char*)*16);
  char* cs;
  for (int i = 0; (cs = strtok ((char*)entry_text_IV, " ")); i++)
    {
      int tmp = hexadecimalToDecimal(cs); 
      
      IV[i]=tmp;
      printf("%02X ", (unsigned char) *(IV+i));       
      entry_text_IV = NULL;
      
    }

    gtk_entry_set_text(Key_Entry,"");
    gtk_entry_set_text(Key_Entry,"Entrer votre clés AES ...");
    gtk_entry_set_text(GTK_ENTRY(entry_text_IV),"");
    gtk_entry_set_text(GTK_ENTRY(entry_text_IV),"Entrer votre IV...");
    gtk_widget_hide((GtkWidget*)dialog_AES_CBC);}


char ** InitTableau(char ** tab,int size)
{
  tab = malloc (sizeof(uint8_t*) * size);
  for (int i = 0; i < size; i++)
  {
    tab[i]=malloc(sizeof(uint8_t*)*16);
  }
  return tab;
}  

unsigned char **  TraitementFile(char* mode,unsigned char** chiffre,int lenght,int * lenPaddedMsg){
  int len, lenPaddedMessage;
  unsigned char **buf;
  
  printf("%s\n",filename );
  FILE* file;
  if(strcmp(mode,"rb")==0)
  {
   file = fopen(filename, mode);

    if (file){
      fseek (file, 0, SEEK_END);
      len = ftell (file);
      if(len % 16 !=0)
        lenPaddedMessage = (len/16 +1); 

      *lenPaddedMsg=lenPaddedMessage;
      fseek (file, 0, SEEK_SET);
      buf = InitTableau(buf,lenPaddedMessage);
      int lenPadde = len/16+1;
      for (int i = 0; i <= lenPadde; i++)
      {
        fgets((char*)buf[i], 17, file);
      }
      int reste = len%16;
      int last =lenPadde-1;

      for (int i = reste; i < 16; i++)
      {
        //strcpy(buf[1][i],(uint8_t)0x00);
        buf[last][i]=(uint8_t)0x00;
 
      }

    }
  } 
  if(strcmp(mode,"w")==0)
  {
    file = fopen("Chiffre.enc", "w");
    if (file != NULL)
    { 
        for (int y = 0; y < lenght; y++)
          for (int i = 0; i < 16; i++)
            fprintf(file,"%02X",(unsigned char) *(chiffre[y]+i) );
        //buf="";      

    }
  }
  fclose (file);
  return buf;
}

GtkWidget * ReturnDialogAES(char * TypeAES){
  GtkWidget * AESDialog;

  if(strcmp("AES 128",TypeAES)==0 || strcmp("AES 192",TypeAES)==0 || strcmp("AES 256",TypeAES)==0)
  {
     AESDialog = GTK_WIDGET(dialog_AES) ;
  }
  else{
     AESDialog = GTK_WIDGET(dialog_AES_CBC);
  }
  return AESDialog;
}  


char * Afficher_AES_Label(GtkTextBuffer *buffer,GtkTextIter end,uint8_t** msg,GtkTextView *text_label,int size){
  int decimal;
  char * result_history;
  result_history=(char *)calloc(size,sizeof(char*));
  char tmp[16],msgEncrypt[16];
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_end_iter(buffer,&end);

  for (int i = 0; i < 16; ++i) 
  {
    decimal=msg[0][i];
    sprintf(tmp,"%02X",decimal);
    g_print ("You chose %d\n ",msg[0][i]);
    char * msgEncrypt= strcat(tmp," ");
    strcat(result_history,tmp);

    gtk_text_buffer_insert(buffer,&end,msgEncrypt,-1); 
  }
  return result_history;

}


void Encrypt_AES(GtkButton *button,GtkTextView *text_label)
{

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
    if(filename==NULL && (strlen(message)==0))
      printErreur("Aucun données a chiffrer !");
    else{
      if(filename!=NULL && (strlen(message)==0))
      {
        int lenPaddedMessage;
        unsigned char ** clair= TraitementFile("rb",NULL,0,&lenPaddedMessage);
        printf("%d\n",lenPaddedMessage );
        if(key==NULL)
        {
          GtkWidget * AESDialog = ReturnDialogAES(TypeAES);

          gtk_dialog_run(AESDialog); 
        }        
          uint8_t **enc_msg;

          GtkDialog * AESDialog = ReturnDialogAES(TypeAES);
          gtk_dialog_run(GTK_DIALOG(AESDialog)); 
        }

        if(key!=NULL)
        {
          enc_msg = Main_AES(TypeAES,clair,key,lenPaddedMessage,IV);

          //char** bu = TraitementFile("w",enc_msg,lenPaddedMessage,NULL);
          TraitementFile("w",enc_msg,lenPaddedMessage,NULL);
        }      
      }else{
        if ((strlen(message)!=47))
          printErreur("La longueur du bloc doit etre de 16 !");
        else{
          //Fonction AES
          unsigned char **MsgToAES; 
          char * cs;
          MsgToAES=malloc(sizeof(uint8_t*)*1);
          MsgToAES[0]=malloc(sizeof(uint8_t*)*16);
     

          for (int i = 0; (cs = strtok ((char*)message," ")); i++)
          {
            int tmp = hexadecimalToDecimal(cs); 
            MsgToAES[0][i]=(uint8_t)tmp;
            message = NULL; 
          }
          if(key==NULL)   
          {
            GtkWidget * AESDialog = ReturnDialogAES(TypeAES);
            //GtkDialog * AESDialog = ReturnDialogAES(TypeAES);
            gtk_dialog_run(GTK_DIALOG(AESDialog)); 
          }
          
          for (int i = 0; i < 16; ++i)
          {
            printf("%02X ",(unsigned char) (MsgToAES[0][i]) );
          }
          if (key != NULL)
          {
            uint8_t ** msg = Main_AES(TypeAES,MsgToAES,key,1,IV);
            char tmp[16];
            int decimal;
            //Pour supprimer
            buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
            gtk_text_buffer_get_end_iter(buffer,&end);
            gtk_text_buffer_delete(buffer,&start,&end);
            //Pour ajouter

            char *result_history=Afficher_AES_Label(buffer,end,msg,text_label,sizeof(message));
            Historique(history,result_history,TypeAES,NULL);

          }         
        }
      }
        key=NULL;
        IV=NULL;
        free(IV);
        free(key);
    } 
  }
}

void DECRYPT_AES(GtkButton *button, GtkTextView *text_label){

  GtkTextBuffer *buffer;
  GtkTextIter start,end;
  char* message;
  char * history;
  char * result_history;
  //ON recupere le message dans la textBox si il est pas trop grand
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  message=gtk_text_buffer_get_text(buffer,&start,&end,-1);
  history=malloc(sizeof(char*)*sizeof(message));
  result_history=(char *)calloc(sizeof(message),sizeof(char*));

  strcpy(history,message);

  if(filename==NULL && (strlen(message)==0))
    printErreur("Aucun données a chiffrer !");

  else{
    if(filename!=NULL && (strlen(message)==0))
    {
      int lenPaddedMessage;
      unsigned char ** chiffre= TraitementFile("rb",NULL,0,&lenPaddedMessage);
      printf("%d\n",lenPaddedMessage );
      if(key==NULL)
      {
        GtkWidget * AESDialog = ReturnDialogAES(TypeAES);
        //GtkDialog * AESDialog = ReturnDialogAES(TypeAES);
        gtk_dialog_run(GTK_DIALOG(AESDialog)); 
      }
      
      unsigned char* enc_msg;
      for (int i = 0; i < lenPaddedMessage; i++)
      {
        for (int y = 0; y < 16; y++)
        {
          printf("%02X",(unsigned char) *(chiffre[i]+y) );
        }
        printf("\n");
      }


      if(key!=NULL)
      {
        int lenKey = sizeof(key);
        enc_msg = AES_decrypt((unsigned char*)key, (unsigned char *)chiffre, lenKey, lenPaddedMessage);

        //char** bu = TraitementFile("w",enc_msg,lenPaddedMessage,NULL);
        TraitementFile("w",(unsigned char**)enc_msg,lenPaddedMessage,NULL);
      }      
    }else{
      if ((strlen(message)!=47))
        printErreur("La longueur du bloc doit etre de 16 !");
      else{
        //Fonction AES
        unsigned char **MsgToAES; 
        char* cs;
        MsgToAES=malloc(sizeof(uint8_t*)*1);
        MsgToAES[0]=malloc(sizeof(uint8_t*)*16);
   

        for (int i = 0; (cs = strtok ((char*)message," ")); i++)
        {
          int tmp = hexadecimalToDecimal(cs); 
          MsgToAES[0][i]=(uint8_t)tmp;
          message = NULL; 
        }
        if(key==NULL)   
        {
          GtkWidget * AESDialog = ReturnDialogAES(TypeAES);
          //GtkDialog * AESDialog = ReturnDialogAES(TypeAES);
          gtk_dialog_run(GTK_DIALOG(AESDialog));

        }
        
        for (int i = 0; i < 16; ++i)
        {
          printf("%02X ",(unsigned char) (MsgToAES[0][i]) );
        }
        if (key != NULL)
        {
          uint8_t ** msg = Main_AES(TypeAES,MsgToAES,key,1,IV);
          char tmp[16];
          int decimal;
          //Pour supprimer
          buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
          gtk_text_buffer_get_end_iter(buffer,&end);
          gtk_text_buffer_delete(buffer,&start,&end);
          //Pour ajouter
          buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
          gtk_text_buffer_get_end_iter(buffer,&end);
          
          for (int i = 0; i < 16; ++i) 
          {
            decimal=msg[0][i];
            sprintf(tmp,"%02X",decimal);
            g_print ("You chose %d\n ",msg[0][i]);
            char * msgEncrypt= strcat(tmp," ");
            strcat(result_history,tmp);

            gtk_text_buffer_insert(buffer,&end,msgEncrypt,-1); 
          }

          Historique(history,result_history,TypeAES,NULL);

        }         
      }
    }
    key=NULL;
    IV=NULL;
    free(IV);
    free(key);
}
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
    

    printf("%s et len %d\n",TypeAES,lenAES );
   
  }}  



void on_click_convert(GtkButton *button,GtkTextView *text_label){
  GtkTextBuffer *buffer;
  GtkTextIter start,end;
  char* msg,*hist;
  char msgconvert[100]={""};
 
  char *token;
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  msg=gtk_text_buffer_get_text(buffer,&start,&end,-1);
  hist=malloc(sizeof(char*)*sizeof(msg));
  strcpy(hist,msg);
  printf("%s\n",msg );

  
  if(strcmp(Combobox1,Combobox2)!=0 && (strcmp(Combobox1,"DEC")==0 || strcmp(Combobox1,"HEX")==0))
  {
    //Pour convertir decimal to hexa
    if (strcmp(Combobox1,"DEC")==0)
    {
     
      token=strtok(msg," ");
      while( token != NULL )
      {
        char tmp[10];
        sprintf(tmp,"%02X",atoi(token));
        strcat(tmp," ");
        strcat(msgconvert,tmp);
        printf("%s\n",msgconvert );
        token = strtok(NULL," ");

      }
      //sprintf(msgconvert,"%02X",atoi(msg));
      gtk_text_buffer_delete(buffer,&start,&end);
      buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
      gtk_text_buffer_insert(buffer,&end,msgconvert,-1);
    }else{

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
  }
  Historique(hist,msgconvert,Combobox1,Combobox2);}

void generateAESKey(GtkButton *button,GtkTextView *text_label){
  if(TypeAES==NULL)
    printErreur("Vous devez choisir votre AES !");
  else{
    
    key = (unsigned char *) malloc(lenAES);
    printf("%d\n",lenAES );
    char AESKey[lenAES];
    char tmp[lenAES];
    unsigned int generate[lenAES];
    mpz_t key;
    gmp_randstate_t prng;
    gmp_randinit_default(prng);
    unsigned long int seed;
    seed=time(NULL);
    gmp_randseed_ui(prng,seed);
    mpz_inits(key,NULL);

    for (int i = 0; i < lenAES; i++)
    {
      mpz_urandomb(key,prng,8);
      generate[i]=mpz_get_ui(key);
      sprintf(tmp,"%02X",generate[i]);
      strcat(AESKey," ");
      strcat(AESKey,tmp);

    }
    printf("%s\n",AESKey );

  }}

void on_btn_0_clicked(GtkButton *button, GtkTextView *text_label){
   
  GtkTextBuffer *buffer;
  GtkTextIter iter;
  int i;
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  
  gtk_text_buffer_get_end_iter(buffer,&iter);
  sscanf(gtk_button_get_label(button),"%d",&i);
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
  }}      
        
void on_click_hash(GtkButton *button, GtkTextView *text_label){

  GtkTextBuffer * buffer;
  GtkTextIter start,end; 
  char *input;
  char* hash;
  printf("cocuc\n");

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);

  if (strcmp(algo_value, "MD5") == 0){
    input = gtk_text_buffer_get_text(buffer,&start,&end,-1);
    hash = md5digest(input);

    gtk_text_buffer_delete(buffer,&start,&end);
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
    gtk_text_buffer_insert(buffer,&end,hash,-1);
    }

  if (strcmp(algo_value, "SHA1") == 0){
    input = gtk_text_buffer_get_text(buffer,&start,&end,-1);
    hash = sha1digest(input);

    gtk_text_buffer_delete(buffer,&start,&end);
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
    gtk_text_buffer_insert(buffer,&end,hash,-1);
    }
 
    if (strcmp(algo_value, "SHA2") == 0){ 

    input = gtk_text_buffer_get_text(buffer,&start,&end,-1);
    hash = sha2_appel(input);  

    gtk_text_buffer_delete(buffer,&start,&end); 
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
    gtk_text_buffer_insert(buffer,&end,hash,-1);
    }

  if (strcmp(algo_value, "HMAC-SHA1") == 0){


    gtk_dialog_run(GTK_DIALOG (dialogHash));

    input = gtk_text_buffer_get_text(buffer,&start,&end,-1);
    hash = hmac_sha1(hmacKey,input);

    gtk_text_buffer_delete(buffer,&start,&end);
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
    gtk_text_buffer_get_start_iter(buffer, &start);     
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer,&end,hash,-1);
    
  }
  
  //char tmp[1024];
  Historique(input,hash,algo_value,NULL);
}

