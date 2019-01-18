#include "sha2/sha2.c"
#include "sha2/fonctionSha2.c"
#include "sha2/traitementSha2.c"
#include <gmp.h>

#include <gtk/gtk.h>
#include "AES.c"
#include "hashlib.c"


gchar * distro ;
gchar * algo;
gchar * AES;
gchar * Combobox1;
gchar * Combobox2;
gchar * algo_value;
gchar * TypeAES;
gchar *hmacKey;
int lenAES;
GtkWidget *window;
char *key;
GtkWidget *dialog;
GtkWidget *dialogHash;


typedef struct
{
  GtkBuilder *builder;
  gpointer user_data;
} SGlobalData;

  

int main(int argc, char *argv [])
{
  GtkBuilder *builder; 
  

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "test.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
  dialog = GTK_WIDGET(gtk_builder_get_object(builder, "Dialog"));
  dialogHash = GTK_WIDGET(gtk_builder_get_object(builder, "DialogHash"));
  gtk_window_set_transient_for(dialog,window);
  gtk_window_set_attached_to(window,dialog);
  gtk_window_set_transient_for(dialogHash,window);
  gtk_window_set_attached_to(window,dialogHash);
  gtk_builder_connect_signals(builder, NULL);



  g_object_unref(builder);

  gtk_widget_show(window);                
  gtk_main();

  return 0;
}
void Changed_combo2(GtkComboBox *widget, GtkTextView *text_label)
{
  GtkComboBox *combo_box = widget;
  GtkTextBuffer *buffer;
  GtkTextIter iter;
  int i;
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_end_iter(buffer,&iter);

  if (gtk_combo_box_get_active (combo_box) != NULL) {
    distro= gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo_box));    
    int i = gtk_combo_box_get_active(combo_box);
    Combobox2=distro;
    g_print ("You chose %d\n", i);
  }
}

void ChangeCombo1(GtkComboBox *widget, GtkTextView *text_label)
{
  GtkComboBox *combo_box = widget;
  GtkTextBuffer *buffer;
   GtkTextIter iter;
   int i;
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_end_iter(buffer,&iter);

  if (gtk_combo_box_get_active (combo_box) != NULL) {
    distro= gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo_box));
    int i = gtk_combo_box_get_active(combo_box);
    Combobox1 = distro;
   g_print ("You chose %s\n", distro);

   
  }  
}

void import_path(GtkFileChooserButton *btn)
{
  char *filename = gtk_file_chooser_get_filename(btn);
  long len;
  char * buf ;
  printf("%s\n",filename );
  
  FILE * file = fopen(filename, "rb");

  if (file)
  {
    fseek (file, 0, SEEK_END);
    len = ftell (file);
    fseek (file, 0, SEEK_SET);
    buf = malloc (sizeof(char) * len);
    if (buf)
    {
      fread (buf, 1, len, file);
    }
    
  }
  unsigned char *test[len];
  for (int i = 0; i < len; i++) 
  {
   
    test[i]=(uint8_t) *(buf+i);
    printf("%02X\n",test[i] );
  }
  fclose (file);
  unsigned char * chiffre = Main_AES(TypeAES,test,key,len);
}

void hashList(GtkComboBox *widget, GtkTextView *text_label)
{
  GtkComboBox *combo_box = widget;
  GtkTextBuffer *buffer;
   GtkTextIter iter;
   int i;
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_end_iter(buffer,&iter);

  if (gtk_combo_box_get_active (combo_box) != NULL) {
    algo= gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo_box));
    int i = gtk_combo_box_get_active(combo_box);
    algo_value = algo;
   g_print ("You chose %s\n", algo);
   
  }  
}

void printErreur(char *msg)
{ 
  GtkWidget*  dialog;
  dialog = gtk_message_dialog_new (window,
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_CLOSE,
                                  msg);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}
 
void Dialog_click_Annuler(GtkButton *button,GtkEntry *entry)
{
  gtk_entry_set_text(entry,"");
      gtk_entry_set_text(entry,"Entrer votre clé AES ...");
   gtk_widget_hide(dialog); 
}

void DialogHash_cancel(GtkButton *button){

	gtk_widget_destroy(dialogHash);
}

void DialogHash_send(GtkButton *button,GtkEntry *entry){

	hmacKey = gtk_entry_get_text (GTK_ENTRY (entry));
	gtk_widget_hide(dialogHash);
}

void Dialog_click_Ok(GtkButton *button,GtkEntry *entry)
{
  
  gchar *entry_text;
  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  printf ("Entry contents: %s et longueur: %d\n", entry_text,(lenAES*2)+23);
  if(strlen(entry_text)!=(lenAES*2)+lenAES-1)
  {
    printErreur("Taille de clé incorrect !");
  }
  else{
    key = malloc(lenAES);
    char* cs;
    char test[3];
    for (int i = 0; (cs = strtok (entry_text, " ")); i++)
    {
      printf("%s\n",cs );
      int tmp = hexadecimalToDecimal(cs); 
      
      key[i]=tmp;
      printf("%02X ", (unsigned char) *(key+i));       
      entry_text = NULL;
      
    }
    gtk_entry_set_text(entry,"");
    gtk_entry_set_text(entry,"Entrer votre clés AES ...");
    gtk_widget_hide(dialog);
  }
 

                  
}

void Encrypt_AES(GtkButton *button,GtkTextView *text_label)
{

  GtkTextBuffer *buffer;
  GtkTextIter iter,start,end;
  unsigned char* message;
  //ON recupere le message dans la textBox si il est pas trop grand
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  message=gtk_text_buffer_get_text(buffer,&start,&end,-1);
  
  if(TypeAES==NULL)
  {
    printErreur("Vous devez choisir votre AES !");

  }else{
    if (strlen(message)!=47)
    {
      printErreur("La longueur du bloc doit etre de 16 !");
    }else{
      //Fonction AES
       unsigned char *MsgToAES[16]; 
       unsigned char * cs;
      
      for (int i = 0; (cs = strtok (message," ")); i++)
      {
        int tmp = hexadecimalToDecimal(cs); 
        MsgToAES[i]=(uint8_t)tmp;
        
        message = NULL; 
      }
      printf("Avant AES\n");
      for (int i = 0; i < 16; i++)
      { 
        printf("%02X\n",MsgToAES[i] );
      }
       
        /*01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
6B C1 BE E2 2E 40 9F 96 E9 3D 7E 11 73 93 17 2A
8E 73 B0 F7 DA 0E 64 52 C8 10 F3 2B 80 90 79 E5 62 F8 EA D2 52 2C 6B 7B

AES128

2B 7E 15 16 28 AE D2 A6 AB F7 15 88 09 CF 4F 3C

AES256

60 3D EB 10 15 CA 71 BE 2B 73 AE F0 85 7D 77 81 1F 35 2C 07 3B 61 08 D7 2D 98 10 A3 09 14 DF F4
    */    
        gtk_dialog_run(dialog);

        if(key!=NULL)
        {
          unsigned char * msg = Main_AES(TypeAES,MsgToAES,key,16);
          for (int i = 0; i < strlen(msg); i++)
          {
            printf("%02X\n", msg[i]);
          }
          int len = strlen(msg); 
          
          int * msgEncrypt;
          char tmp[len];
          int decimal;
          int i;
          //Pour supprimer
          buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
          gtk_text_buffer_get_end_iter(buffer,&iter);
          gtk_text_buffer_delete(buffer,&start,&end);
          //Pour ajouter
          buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
          gtk_text_buffer_get_end_iter(buffer,&iter);
          for (int i = 0; i < len-1; ++i) 
          {
            decimal=msg[i];
            sprintf(tmp,"%02X",decimal);
            g_print ("You chose %d\n ",msg[i]);
            char * msgEncrypt=strcat(tmp," ");
            gtk_text_buffer_insert(buffer,&iter,msgEncrypt,-1);
          }
         
          key=NULL;
          free(key);
      }
    }
  }
}
void Changed_AES(GtkComboBox *comboBox)
{
  if (gtk_combo_box_get_active (comboBox) != NULL) {
    TypeAES= gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(comboBox));
   

    if(strcmp(TypeAES,"AES 128")==0)
      lenAES=16;
    else if (strcmp(TypeAES,"AES 192")==0)
      lenAES=24;
    else
      lenAES=32;
    

    printf("%s et len %d\n",TypeAES,lenAES );
   
  }
}  
int hexadecimalToDecimal(char hexVal[]) 
{    
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

void on_click_convert(GtkButton *button,GtkTextView *text_label)
{
  GtkTextBuffer *buffer;
  GtkTextIter iter,start,end;
  char* msg;
  char msgconvert[100]={""};
 
  int i;
  char *token;
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);

  
  if(strcmp(Combobox1,Combobox2)!=0 && (strcmp(Combobox1,"DEC")==0 || strcmp(Combobox1,"HEX")==0))
  {
    //Pour convertir decimal to hexa
    if (strcmp(Combobox1,"DEC")==0)
    {
      msg=gtk_text_buffer_get_text(buffer,&start,&end,-1);
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
      msg=gtk_text_buffer_get_text(buffer,&start,&end,-1);

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
}

void generateAESKey(GtkButton *button,GtkTextView *text_label)
{
  if(TypeAES==NULL)
    printErreur("Vous devez choisir votre AES !");
  else{
    
    key = (char *) malloc(lenAES);
    printf("%d\n",lenAES );
    long tmp[lenAES];
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
      tmp[i]=mpz_get_ui(key);
      printf("%d\n",tmp[i]);
    }

  }
  
  
}

void on_btn_0_clicked(GtkButton *button, GtkTextView *text_label)
{
   
   GtkTextBuffer *buffer;
   gchar* msg;
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
  }

}

void  callback_about (GtkMenuItem *menuitem, gpointer user_data)
{
  /* Transtypage du pointeur user_data pour récupérer nos données. */
  SGlobalData *data = (SGlobalData*) user_data;
  GtkWidget *dialog = NULL;          

  /* Récupération de la fenêtre "AboutWindow". */
  dialog =  GTK_WIDGET (gtk_builder_get_object (data->builder, "AboutWindow"));
  gtk_dialog_run (GTK_DIALOG (dialog));

  /* On cache la fenêtre de dialogue. Si on la détruisait le prochain appel */
  /* à ce callback provoquerait un segdefault! */
  gtk_widget_hide (dialog);
}
     
          
        
void on_click_hash(GtkButton *button, GtkTextView *text_label){

	GtkTextBuffer *buffer;
	GtkTextIter iter,start,end; 
	char *input;

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);

	if (strcmp(algo_value, "MD5") == 0){
		input = gtk_text_buffer_get_text(buffer,&start,&end,-1);
		char* md5 = md5digest(input);

		gtk_text_buffer_delete(buffer,&start,&end);
		buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
		gtk_text_buffer_insert(buffer,&end,md5,-1);
    }

	if (strcmp(algo_value, "SHA1") == 0){
		input = gtk_text_buffer_get_text(buffer,&start,&end,-1);
		char* sha1 = sha1digest(input);

		gtk_text_buffer_delete(buffer,&start,&end);
		buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
		gtk_text_buffer_insert(buffer,&end,sha1,-1);
    }

    if (strcmp(algo_value, "SHA2") == 0){

		input = gtk_text_buffer_get_text(buffer,&start,&end,-1);
		char* sha2 = sha2_appel(input);

		gtk_text_buffer_delete(buffer,&start,&end);
		buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
		gtk_text_buffer_insert(buffer,&end,sha2,-1);
    }

	if (strcmp(algo_value, "HMAC-SHA1") == 0){

		gtk_dialog_run(dialogHash);

		input = gtk_text_buffer_get_text(buffer,&start,&end,-1);
		char* hmac = hmac_sha1(hmacKey,input);

		gtk_text_buffer_delete(buffer,&start,&end);
		buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
		gtk_text_buffer_insert(buffer,&end,hmac,-1);

    }
}