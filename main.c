
#include <gtk/gtk.h>

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
      void on_btn_0_clicked(GtkButton *button, GtkTextView *text_label)
      {
         
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
        }
        
      
      
      }

      void
      callback_about (GtkMenuItem *menuitem, gpointer user_data)
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
