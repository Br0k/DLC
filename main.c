#include "sha2/sha2.c"
#include "sha2/fonctionSha2.c"
#include "sha2/traitementSha2.c"

#include <gtk/gtk.h>
#include "AES.c"

gchar * distro ;
gchar * AES;
gchar * Combobox1;
gchar * Combobox2;    

typedef struct
{
  GtkBuilder *builder;
  gpointer user_data;
} SGlobalData;

  

int main(int argc, char *argv [])
{
  GtkBuilder      *builder; 
  GtkWidget       *window;

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "test.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  gtk_widget_show(window);                
  gtk_main();

  return 0;
}
 void Changed_combo2(GtkComboBox *widget, GtkTextView *text_label){
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

void Encrypt_AES(GtkButton *button,GtkTextView *text_label)
{
  unsigned char * msg = Main_AES();
  int len = strlen(msg);              
  printf("%d\n", len);
  int * test;
  char lol[len];
  int tmp;
  GtkTextBuffer *buffer;
  GtkTextIter iter;
  int i;
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_label));
  gtk_text_buffer_get_end_iter(buffer,&iter);
  for (int i = 0; i < len-1; ++i)
  {
    tmp=msg[i];
    sprintf(lol,"%02X",tmp);
    g_print ("You chose %d\n ",msg[i]);
    char * test=strcat(lol," ");
    gtk_text_buffer_insert(buffer,&iter,test,-1);
  }
}
  void Changed_AES(GtkComboBox *comboBox){
  if (gtk_combo_box_get_active (comboBox) != NULL) {
    AES= gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(comboBox));
    g_print ("You chose %s\n", AES);

   
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

void test()
{
  printf("tet\n");
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
     
          
        
	// sha2
	void on_change_comboBox_Hash(GtkComboBox *combo){
		printf("ddd");
	}    
	void on_click_hash(GtkButton *button, GtkTextView *text_label){
		printf("ddd");
		printf("--> %s\n",sha2_appel());
	}   

        
  

