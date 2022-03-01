#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include<time.h>
#include <math.h>
#include <ctype.h>
#include"bus-header.h"
// Main container widgets
GtkWidget *window;
GtkButton *Sb,*Bb;
GtkBuilder *builder;
GtkStack *stack1;
GtkWidget *fixed2,*fixed1;

//Busline page
//searchbox
GtkSearchEntry *entry1;
GtkWidget *box1;
GtkButton *subbutton1[10];
int pos1=-1;
GtkSearchEntry *entry2;
GtkWidget *box2;
GtkButton *subbutton2[10];
int pos2=-1;
GtkButton *re;
//Distance bar
GtkLabel *lb3,*lb4,*lb5;
GtkButton *button1;
//Map
GtkWidget *view3;
GtkWidget *grid1;
GtkWidget *fixed[MAXN];
GtkWidget *sublabel[MAXN],*blabel;
GtkWidget *label[20];
GtkWidget *button3[MAXN];
GtkWidget *image;
//Bus page
//searchbox
GtkSearchEntry *entry3;
GtkButton *subbutton3[10];
int pos3=-1;
GtkWidget *box4;
GtkLabel *lb11;
//busline box
GtkLabel *lb9,*lb10;
GtkWidget *view6;
GtkWidget *view7;
GtkWidget *grid6, *grid7;
GtkWidget *button6[MAXN],*button7[MAXN];
//Station page
//box
GtkLabel *lb7;
GtkWidget *grid5;
GtkWidget *glabel[30];
GtkButton *button[30];
//Autocomplete
GtkListStore *ls1,*ls2;
GtkEntryCompletion *completion1,*completion2,*completion3;
GtkTreeIter iter;
//Other variables
Graph g;
Station st;
Station item;
Bus bus[150];
int length_bus;
int i,j;
int a,b;
double wei;
char* markup;
const char* format = "<span weight=\"bold\" foreground='#ffffff'>\%s</span>";
const char* format1 = "<span weight=\"bold\" foreground='#006699'>\%s</span>";
const char* format2 = "<span weight=\"medium\" foreground='#77767b'>\%s</span>";
char direction[10];
int status=0;
int status_bus=0;
//Declare functions
void ReadfileStation(Graph ,Station ,GtkListStore *);
void on_Sb_clicked();
void on_Bb_clicked();
void make_history_box(GtkButton **,GtkWidget *);
void on_entry1_activate();
void on_entry2_activate();
void on_entry1_delete();
void on_entry2_delete();
void on_convert_clicked();
void on_entry3_activate();
void on_entry3_delete();
void on_subbutton1_clicked(GtkButton *);
void on_subbutton2_clicked(GtkButton *);
void on_subbutton3_clicked(GtkButton *);
void on_destroy();
void set_grid();
void init_busline_page();
void on_clicked(GtkButton *);
void on_button1_clicked();
double show_information(int, int);
void show_businformation(PathBus*, int,GtkWidget*,GtkWidget*,GtkWidget **);
void on_row(GtkButton *);
void init_bus_page();
void set_completion(GtkEntryCompletion **completion,GtkListStore *ls){
    *completion=gtk_entry_completion_new();
    gtk_entry_completion_set_model(*completion,GTK_TREE_MODEL(ls));
    gtk_entry_completion_set_text_column(*completion,0);
}
// main function
int main(int argc, char *argv[]) {
    gtk_init (&argc, &argv); // init Gtk
    ls1=gtk_list_store_new(1,G_TYPE_STRING);
    ls2=gtk_list_store_new(1,G_TYPE_STRING);
    g=createGraph();
    st=(Station)malloc(MAXN*sizeof(station));
    ReadfileStation(g,st,ls1);
    ReadfileBusLine(g);
    ReadfileBus(bus,&length_bus);
    for(i=0;i<length_bus;i++){
        gtk_list_store_append(ls2,&iter);
        gtk_list_store_set(ls2,&iter,0,bus[i].bus,-1);
    }
    set_completion(&completion1,ls1);
    set_completion(&completion2,ls1);
    set_completion(&completion3,ls2);
    //color of entries
    GdkColor color;
    color.red=0x6e6e;
    color.green=0x9696;
    color.blue=0xb0b0;
    GdkColor colour;
    colour.red=0xffff;
    colour.green=0xd1d1;
    colour.blue=0x6969;
     // Main container widgets
    builder = gtk_builder_new_from_file ("project2.glade");
    window = GTK_WIDGET (gtk_builder_get_object(builder, "window"));
    g_signal_connect (window, "destroy", G_CALLBACK (on_destroy), NULL);
    gtk_builder_connect_signals (builder, NULL);
    stack1=GTK_STACK(gtk_builder_get_object(builder,"stack1"));
    fixed1=GTK_WIDGET(gtk_builder_get_object(builder,"fixed1"));
    fixed2=GTK_WIDGET(gtk_builder_get_object(builder,"fixed2"));
    Sb=GTK_BUTTON(gtk_builder_get_object(builder,"Sb"));
    g_signal_connect(Sb,"clicked",G_CALLBACK(on_Sb_clicked),NULL);
    gtk_widget_modify_bg(GTK_WIDGET(Sb),GTK_STATE_NORMAL,&colour);
    Bb=GTK_BUTTON(gtk_builder_get_object(builder,"Bb"));
    g_signal_connect(Bb,"clicked",G_CALLBACK(on_Bb_clicked),NULL);
    gtk_widget_modify_bg(GTK_WIDGET(Bb),GTK_STATE_NORMAL,&color);
    //Busline page
    entry1=GTK_SEARCH_ENTRY(gtk_builder_get_object(builder,"entry1"));
    gtk_widget_modify_bg(GTK_WIDGET(entry1),GTK_STATE_NORMAL,&color);
    gtk_entry_set_completion(GTK_ENTRY(entry1),completion1);
    entry2=GTK_SEARCH_ENTRY(gtk_builder_get_object(builder,"entry2"));
    gtk_widget_modify_bg(GTK_WIDGET(entry2),GTK_STATE_NORMAL,&color);
    re=GTK_BUTTON(gtk_builder_get_object(builder,"convert"));
    gtk_entry_set_completion(GTK_ENTRY(entry2),completion2);
    box1=GTK_WIDGET(gtk_builder_get_object(builder,"box1"));
    box2=GTK_WIDGET(gtk_builder_get_object(builder,"box2"));
    g_signal_connect (entry1, "activate", G_CALLBACK (on_entry1_activate), NULL);
    g_signal_connect (entry1, "icon-press", G_CALLBACK (on_entry1_delete), NULL);
    g_signal_connect (entry1, "backspace", G_CALLBACK (on_entry1_delete), NULL);
    g_signal_connect (entry2, "activate", G_CALLBACK (on_entry2_activate), NULL);
    g_signal_connect (entry2, "icon-press", G_CALLBACK (on_entry2_delete), NULL);
    g_signal_connect (entry2, "backspace", G_CALLBACK (on_entry2_delete), NULL);
    g_signal_connect (re, "clicked", G_CALLBACK (on_convert_clicked), NULL);
    make_history_box(subbutton1,box1);
    for(i=0;i<10;i++)
    g_signal_connect(subbutton1[i],"clicked",G_CALLBACK(on_subbutton1_clicked),NULL);
    make_history_box(subbutton2,box2);
    for(i=0;i<10;i++)
    g_signal_connect(subbutton2[i],"clicked",G_CALLBACK(on_subbutton2_clicked),NULL);
    lb3=GTK_LABEL(gtk_builder_get_object(builder,"lb3"));
    lb4=GTK_LABEL(gtk_builder_get_object(builder,"lb4"));
    lb5=GTK_LABEL(gtk_builder_get_object(builder,"lb5"));
    button1=GTK_BUTTON(gtk_builder_get_object(builder,"button1"));
    g_signal_connect(button1,"clicked",G_CALLBACK(on_button1_clicked),NULL);
    view3 = GTK_WIDGET (gtk_builder_get_object (builder, "view3"));
    //Station page
    lb7=GTK_LABEL(gtk_builder_get_object(builder,"lb7"));
    grid5=GTK_WIDGET(gtk_builder_get_object(builder,"grid5"));
    set_grid(GTK_GRID(grid5));
    //Bus page
    entry3=GTK_SEARCH_ENTRY(gtk_builder_get_object(builder,"entry3"));
    gtk_widget_modify_bg(GTK_WIDGET(entry3),GTK_STATE_NORMAL,&color);
    gtk_entry_set_completion(GTK_ENTRY(entry3),completion3);
    box4=GTK_WIDGET(gtk_builder_get_object(builder,"box4"));
    g_signal_connect (entry3, "activate", G_CALLBACK (on_entry3_activate), NULL);
    g_signal_connect (entry3, "icon-press", G_CALLBACK (on_entry3_delete), NULL);
    g_signal_connect (entry3, "backspace", G_CALLBACK (on_entry3_delete), NULL);
    make_history_box(subbutton3,box4);
    for(i=0;i<10;i++)
    g_signal_connect(subbutton3[i],"clicked",G_CALLBACK(on_subbutton3_clicked),NULL);
    lb9=GTK_LABEL(gtk_builder_get_object(builder,"lb9"));
    lb10=GTK_LABEL(gtk_builder_get_object(builder,"lb10"));
    lb11=GTK_LABEL(gtk_builder_get_object(builder,"lb11"));
    view6 = GTK_WIDGET (gtk_builder_get_object (builder, "view6"));
    view7 = GTK_WIDGET (gtk_builder_get_object (builder, "view7"));
    gtk_widget_show_all(window);
    gtk_widget_hide(grid5);
    gtk_main();
    return EXIT_SUCCESS;
}
void on_Sb_clicked(){
    GdkColor color;
    color.red=0x6e6e;
    color.green=0x9696;
    color.blue=0xb0b0;
    GdkColor colour;
    colour.red=0xffff;
    colour.green=0xd1d1;
    colour.blue=0x6969;
    gtk_stack_set_visible_child(stack1,fixed1);
    gtk_widget_modify_bg(GTK_WIDGET(Sb),GTK_STATE_NORMAL,&colour);
    gtk_widget_modify_bg(GTK_WIDGET(Bb),GTK_STATE_NORMAL,&color);
}
void on_Bb_clicked(){
    GdkColor color;
    color.red=0x6e6e;
    color.green=0x9696;
    color.blue=0xb0b0;
    GdkColor colour;
    colour.red=0xffff;
    colour.green=0xd1d1;
    colour.blue=0x6969;
    gtk_stack_set_visible_child(stack1,fixed2);
    gtk_widget_modify_bg(GTK_WIDGET(Bb),GTK_STATE_NORMAL,&colour);
    gtk_widget_modify_bg(GTK_WIDGET(Sb),GTK_STATE_NORMAL,&color);
}
void ReadfileStation(Graph g,Station valu,GtkListStore *ls){
int i,j;
  char c;
  int s=0;
  StringArray string[20];
  char st[256];
  FILE *fpt=fopen("station.txt","r");
  while(1){
    if(feof(fpt)) break;

    fgets(st,256,fpt);
    st[strlen(st)-1]='\0';
    gtk_list_store_append(GTK_LIST_STORE(ls),&iter);
    gtk_list_store_set(ls,&iter,0,st,-1);
    i=0;j=0;
    while(1){
      c=fgetc(fpt);
      if(c=='\n' || feof(fpt)){
        string[i].str[j]='\0';
        break;
      }
      if(c=='/'){
        string[i].str[j]='\0';
        i++;
        j=0;
        continue;
      }
      string[i].str[j]=c;
      j++;
    }
    s++;
    valu[s].size=i+1;
    for(j=0;j<=i;j++){
      valu[s].arr[j]=string[j];
      //printf("%s\n",valu[s].arr[j].str);
    }
    strcpy(valu[s].n,st);
    addVertex(g,s,&valu[s]);

  }
  fclose(fpt);
}
void on_destroy(){
    free(st);
    gtk_main_quit();
}
void make_history_box(GtkButton **subbutton1,GtkWidget *box1){
    for(i=0;i<10;i++){
        subbutton1[i]=GTK_BUTTON(gtk_button_new_with_label(""));
        gtk_button_set_alignment (subbutton1[i], 0.0, 0.5); // hor left, ver center
        gtk_button_set_relief(subbutton1[i],GTK_RELIEF_NONE);
        gtk_widget_set_size_request(GTK_WIDGET(subbutton1[i]),1,50);
        gtk_grid_attach(GTK_GRID(box1),GTK_WIDGET(subbutton1[i]),i,1,1,1);
        
    }
}
void set_grid(GtkGrid *gri){
    GdkColor color;
    color.red=0xffff;
    color.green=0xffff;
    color.blue=0xffff;
    for(i=0;i<30;i++){
    glabel[i]=gtk_label_new(NULL);
    button[i] = GTK_BUTTON(gtk_button_new());
    gtk_container_add (GTK_CONTAINER (button[i]), glabel[i]);
    gtk_widget_set_size_request(GTK_WIDGET(button[i]),1,40);
    gtk_button_set_alignment (GTK_BUTTON (button[i]), 0.5, 0.5); // hor left, ver center
    gtk_button_set_relief(GTK_BUTTON(button[i]),GTK_RELIEF_NONE);
    gtk_widget_modify_bg(GTK_WIDGET(button[i]),GTK_STATE_NORMAL,&color);
    gtk_grid_attach(GTK_GRID(gri), GTK_WIDGET(button[i]), i%2, i/2, 1, 1);
    g_signal_connect(button[i], "clicked", G_CALLBACK (on_clicked), NULL);
    }
}
void change_label(int index){
    item=getVertex(g,index);
    for(i=0;i<item->size;i++){
        markup = g_markup_printf_escaped (format2, item->arr[i].str);
        gtk_label_set_markup (GTK_LABEL (glabel[i]),markup);
    }
    gtk_widget_show_all(grid5);
    for(i=item->size;i<30;i++){
        gtk_widget_hide(GTK_WIDGET(button[i]));
    }
}
void init_busline_page(){
    gtk_label_set_text(lb7,"");
    gtk_widget_hide(grid5);
    if(status!=0){
    gtk_container_remove(GTK_CONTAINER(view3),grid1);}
    status=0;
    gtk_label_set_text(lb5,"");
}
void on_entry1_activate(){
    init_busline_page();
    char start[256];
    strcpy(start,gtk_entry_get_text(GTK_ENTRY(entry1)));
    a=getVertexID(g,start);
    if(a==INFINITY_VALUE) gtk_label_set_text(lb3,"Can't find this station.");
    else{
        gtk_label_set_text(lb3,start);
        gtk_label_set_text(lb7,start);
         change_label(a);
        if(strcmp(gtk_label_get_text(lb4),"")!=0 && strcmp(gtk_label_get_text(lb4),"Can't find this station.")!=0){
        wei=show_information(a,b);
        if(wei==INFINITY_VALUE){strcpy(direction,"No path!");}
        else{sprintf(direction,"%g",wei);
        status=1;}
        gtk_label_set_text(lb5,direction);
    }
    }
    //
    int check=0;
    for(i=0;i<=pos1;i++){
        if(strcmp(start,gtk_button_get_label(subbutton1[i]))==0){
            for(int j=i;j>0;j--){
                gtk_button_set_label(subbutton1[j],gtk_button_get_label(subbutton1[j-1]));
            }
            check=1;
            break;
        }
    }
    if(check==0){
    pos1++;
    if(pos1>9) pos1=9;
    {
        for(i=pos1;i>0;i--){
            gtk_button_set_label(subbutton1[i],gtk_button_get_label(subbutton1[i-1]));
        }
    }
    }
    gtk_button_set_label(subbutton1[0],start);
    //
}
void on_entry1_delete(){
    gtk_label_set_text(lb3,"");
    init_busline_page();
}
void on_subbutton1_clicked(GtkButton *b){
    init_busline_page();
    gtk_label_set_text(lb3,"");
    const char *tmp;
    tmp=gtk_button_get_label(b);
    gtk_entry_set_text(GTK_ENTRY(entry1),tmp);
}
void on_entry2_activate(){
    init_busline_page();
    char end[256];
    strcpy(end,gtk_entry_get_text(GTK_ENTRY(entry2)));
    b=getVertexID(g,end);
    if(b==INFINITY_VALUE) gtk_label_set_text(lb4,"Can't find this station.");
    else{
        gtk_label_set_text(lb4,end);
        gtk_label_set_text(lb7,end);
         change_label(b);
        if(strcmp(gtk_label_get_text(lb3),"")!=0 && strcmp(gtk_label_get_text(lb3),"Can't find this station.")!=0){
        wei=show_information(a,b);
        if(wei==INFINITY_VALUE){strcpy(direction,"No path!");}
        else{sprintf(direction,"%g",wei);
        status=1;}
        gtk_label_set_text(lb5,direction);
        }
    }
    //
    int check=0;
    for(i=0;i<=pos2;i++){
        if(strcmp(end,gtk_button_get_label(subbutton2[i]))==0){
            for(int j=i;j>0;j--){
                gtk_button_set_label(subbutton2[j],gtk_button_get_label(subbutton2[j-1]));
            }
            check=1;
            break;
        }
    }
    if(check==0){
    pos2++;
    if(pos2>9) pos2=9;
    {
        for(i=pos2;i>0;i--){
            gtk_button_set_label(subbutton2[i],gtk_button_get_label(subbutton2[i-1]));
        }
    }
    }
    gtk_button_set_label(subbutton2[0],end);
    //
}
void on_entry2_delete(){
    gtk_label_set_text(lb4,"");
    init_busline_page();
}
void on_subbutton2_clicked(GtkButton *b){
    init_busline_page();
    gtk_label_set_text(lb4,"");
    const char *tmp;
    tmp=gtk_button_get_label(b);
    gtk_entry_set_text(GTK_ENTRY(entry2),tmp);
}
double show_information(int s1, int s2){
    Path path[MAXN];
    int length;
    double w;
    w=shortestPath(g,s1,s2,path,&length);
    if(w==INFINITY_VALUE) return INFINITY_VALUE;
    if(w==0) return 0.0;
    grid1=gtk_grid_new();
    gtk_grid_insert_column(GTK_GRID(grid1), 0);
    gtk_grid_insert_column(GTK_GRID(grid1), 1);
    gtk_grid_insert_column(GTK_GRID(grid1), 2);
    gtk_container_add(GTK_CONTAINER(view3),grid1);
    GdkColor color;
    color.red=0x55d3;
    color.green=0x91d3;
    color.blue=0xb6d3;
    GdkColor bcolor;
    bcolor.red=0xffff;
    bcolor.green=0xffff;
    bcolor.blue=0xffff;
    int row = 0;
    Station tmp1,tmp2;
    int k,pos;
    char str[10];
    tmp1=getVertex(g,path[0].point);
    gtk_grid_insert_row (GTK_GRID(grid1), row);
    image = gtk_image_new_from_file ("Images/destination.png");
    gtk_grid_attach (GTK_GRID(grid1), image, 1, row, 1, 1);
    blabel = gtk_label_new (NULL);
    markup = g_markup_printf_escaped (format1, tmp1->n);
    gtk_label_set_markup (GTK_LABEL (blabel),markup);
    gtk_widget_set_size_request(blabel,370,40);
    gtk_label_set_line_wrap(GTK_LABEL(blabel),TRUE);
    button3[row]=gtk_button_new();
    gtk_container_add (GTK_CONTAINER (button3[row]), blabel);
    gtk_button_set_alignment (GTK_BUTTON (button3[ row]), 0.0, 0.5); // hor left, ver center
    gtk_widget_modify_bg(button3[row],GTK_STATE_NORMAL,&bcolor);
    gtk_grid_attach(GTK_GRID(grid1), button3[row], 2, row, 1, 1);
    g_signal_connect(button3[row], "clicked", G_CALLBACK (on_row), NULL);
    for(i=1;i<=length;i++){
        row++;
        tmp2=getVertex(g,path[i].point);
        gtk_grid_insert_row (GTK_GRID(grid1), row);
        sprintf(str,"%g",path[i].weight);
        sublabel[row] = gtk_label_new (NULL);
        markup = g_markup_printf_escaped (format1, str);
        gtk_label_set_markup (GTK_LABEL (sublabel[row]), markup);
        gtk_widget_set_size_request(sublabel[row],52,1);
        gtk_label_set_justify (GTK_LABEL (sublabel[row]), GTK_JUSTIFY_RIGHT);
        gtk_label_set_xalign (GTK_LABEL (sublabel[row]), 1.0);
        gtk_grid_attach (GTK_GRID(grid1), sublabel[row], 0, row, 1, 1);
        image = gtk_image_new_from_file ("Images/line.png");
        gtk_grid_attach (GTK_GRID(grid1), image, 1, row, 1, 1);
        fixed[row] = gtk_fixed_new ();
        pos=5;
        for(j=0;j<tmp1->size;j++){
            for(k=0;k<tmp2->size;k++){
                if(strcmp(tmp1->arr[j].str,tmp2->arr[k].str)==0){
                    label[j]=gtk_label_new(NULL);
                    gtk_widget_set_size_request(label[j],60,40);
                    markup = g_markup_printf_escaped (format, tmp2->arr[k].str);
                    gtk_label_set_markup (GTK_LABEL (label[j]), markup);
                    gtk_label_set_justify (GTK_LABEL (label[j]), GTK_JUSTIFY_LEFT);
                    gtk_label_set_xalign (GTK_LABEL (label[j]), 0.5);
                    gtk_widget_modify_bg(GTK_WIDGET(label[j]),GTK_STATE_NORMAL,&color);
                    gtk_fixed_put(GTK_FIXED(fixed[row]),label[j],pos,15);
                    pos+=65;
                    break;
                }
            }
        }
         gtk_grid_attach (GTK_GRID(grid1), fixed[row], 2, row, 1, 1);
        row++;
        if(i!=length){
            image = gtk_image_new_from_file ("Images/point.png");
        }else{
            image = gtk_image_new_from_file ("Images/destination.png");
        }
        gtk_grid_attach (GTK_GRID(grid1), image, 1, row, 1, 1);
        blabel = gtk_label_new (NULL);
        markup = g_markup_printf_escaped (format1, tmp2->n);
        gtk_label_set_markup (GTK_LABEL (blabel),markup);
        gtk_widget_set_size_request(blabel,370,40);
        gtk_label_set_line_wrap(GTK_LABEL(blabel),TRUE);
        button3[row]=gtk_button_new();
        gtk_container_add (GTK_CONTAINER (button3[row]), blabel);
        gtk_button_set_alignment (GTK_BUTTON (button3[ row]), 0.0, 0.5); // hor left, ver center
        gtk_widget_modify_bg(button3[row],GTK_STATE_NORMAL,&bcolor);
        gtk_grid_attach(GTK_GRID(grid1), button3[row], 2, row, 1, 1);
        g_signal_connect(button3[row], "clicked", G_CALLBACK (on_row), NULL);
        tmp1=tmp2;
    }
    return w;
}
void on_button1_clicked(){
    gtk_widget_show_all(view3);
}
void on_convert_clicked(){
    char start[256];
    char end[256];
    strcpy(start,gtk_entry_get_text(GTK_ENTRY(entry1)));
    strcpy(end,gtk_entry_get_text(GTK_ENTRY(entry2)));
    gtk_entry_set_text(GTK_ENTRY(entry1),end);
    gtk_entry_set_text(GTK_ENTRY(entry2),start);
    gtk_label_set_text(lb4,"");
    on_entry1_activate();
}
void on_row(GtkButton* b){
    int e;
    char *tmp;
    strcpy(tmp,gtk_label_get_text (GTK_LABEL(gtk_bin_get_child(GTK_BIN(b)))));
    gtk_label_set_text(lb7,tmp);
    e=getVertexID(g,tmp);
    change_label(e);
}
void on_clicked(GtkButton *b){
    init_bus_page();
    on_Bb_clicked();
    const char *tmp=gtk_label_get_text (GTK_LABEL(gtk_bin_get_child(GTK_BIN(b))));
    gtk_entry_set_text(GTK_ENTRY(entry3),tmp);
    on_entry3_activate();
}
void on_entry3_activate(){
    init_bus_page();
    int position,c,d;
    double w;
    char name[256];
    PathBus pa[MAXN];
    int length_line;
    strcpy(name,gtk_entry_get_text(GTK_ENTRY(entry3)));
    int check=0;
    for(i=0;i<=pos3;i++){
        if(strcmp(name,gtk_button_get_label(subbutton3[i]))==0){
            for(int j=i;j>0;j--){
                gtk_button_set_label(subbutton3[j],gtk_button_get_label(subbutton3[j-1]));
            }
            check=1;
            break;
        }
    }
    if(check==0){
    pos3++;
    if(pos3>9) pos3=9;
    {
        for(i=pos3;i>0;i--){
            gtk_button_set_label(subbutton3[i],gtk_button_get_label(subbutton3[i-1]));
        }
    }
    }
    gtk_button_set_label(subbutton3[0],name);
    //
    position=-1;
    for(i=0;i<length_bus;i++){
        if(strcmp(bus[i].bus,name)==0){
          position=i;
          break;
        }
    }
    if(position==-1){
        gtk_label_set_text(lb11,"Can't find this bus!");
        return;
    }
    c=getVertexID(g,bus[position].start);
    d=getVertexID(g,bus[position].end);
    w=PathOfItem(g,name,c,d,pa,&length_line);
    if(w==INFINITY_VALUE) gtk_label_set_text(lb9,"No path!");
    else{
        sprintf(direction,"%g",w);
        status_bus=1;
        gtk_label_set_text(lb9,direction);
        grid6=gtk_grid_new();
        show_businformation(pa,length_line,view6,grid6,button6);
    }
    w=PathOfItem(g,name,d,c,pa,&length_line);
    if(w==INFINITY_VALUE) gtk_label_set_text(lb10,"No path!");
    else{
        sprintf(direction,"%g",w);
        status_bus=1;
        gtk_label_set_text(lb10,direction);
        show_businformation(pa,length_line,view7,grid7,button7);
    }
    //
}
void show_businformation(PathBus pa[],int length,GtkWidget *view,GtkWidget *grid,GtkWidget **button){
    gtk_grid_insert_column(GTK_GRID(grid), 0);
    gtk_grid_insert_column(GTK_GRID(grid), 1);
    gtk_container_add(GTK_CONTAINER(view),grid);
    GdkColor bcolor;
    bcolor.red=0xffff;
    bcolor.green=0xffff;
    bcolor.blue=0xffff;
    int row = 0;
    for(i=0;i<length;i++){
        gtk_grid_insert_row (GTK_GRID(grid), row);
        if(i==0 || i==length-1){
            image=gtk_image_new_from_file ("Images/des1.png");
        } else image=gtk_image_new_from_file ("Images/point1.png");
        gtk_grid_attach (GTK_GRID(grid), image, 0, row, 1, 1);
        blabel = gtk_label_new (NULL);
        markup = g_markup_printf_escaped (format1, pa[i].sta);
        gtk_label_set_markup (GTK_LABEL (blabel),markup);
        gtk_widget_set_size_request(blabel,250,40);
        gtk_label_set_line_wrap(GTK_LABEL(blabel),TRUE);
        button[row]=gtk_button_new();
        gtk_container_add (GTK_CONTAINER (button[row]), blabel);
           gtk_button_set_alignment (GTK_BUTTON (button[ row]), 0.0, 0.5); // hor left, ver center
           gtk_button_set_relief(GTK_BUTTON(button[row]),GTK_RELIEF_NONE);
          gtk_widget_modify_bg(button[row],GTK_STATE_NORMAL,&bcolor);
           gtk_grid_attach(GTK_GRID(grid), button[row], 1, row, 1, 1);
           g_signal_connect(button[row], "clicked", G_CALLBACK (on_row), NULL);
           row++;
    }
    gtk_widget_show_all(view);
}
void init_bus_page(){
    gtk_label_set_text(lb9,"");
    gtk_label_set_text(lb10,"");
    gtk_label_set_text(lb11,"");
    if(status_bus!=0){
        gtk_container_remove(GTK_CONTAINER(view6),grid6);
        //gtk_container_remove(GTK_CONTAINER(view7),grid7);
    }
    status_bus=0;
}
void on_entry3_delete(){
    init_bus_page();
    gtk_label_set_text(lb7,"");
    gtk_widget_hide(grid5);
}
void on_subbutton3_clicked(GtkButton *b){
    init_bus_page();
    const char *tmp;
    tmp=gtk_button_get_label(b);
    gtk_entry_set_text(GTK_ENTRY(entry3),tmp);
}
