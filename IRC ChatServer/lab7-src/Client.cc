#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

void add_user(GtkWidget *widget, gpointer data)
{
	g_print("ADDED-USER\n");
}

void enter_room(GtkWidget *widget, gpointer data)
{
	g_print("ENTERED-ROOM\n");
}

void create_room(GtkWidget *widget, gpointer data)
{
	g_print("CREATED-ROOM\n");
}

void leave_room(GtkWidget *widget, gpointer data)
{
	g_print("LEFT-ROOM\n");
}

void send_(GtkWidget *widget, gpointer data)
{
	g_print("MESSAGE-SENT\n");
}

void hover_button(GtkWidget *widget, gpointer data) 
{ 
  GdkColor color;
    color.red = 27000;
      color.green = 30325;
        color.blue = 34181;
	  gtk_widget_modify_bg(widget, GTK_STATE_PRELIGHT, &color);
	  	}

int main( int argc, char *argv[])
{

	GtkWidget *window;
	GtkWidget *adduser;
	GtkWidget *enterroom;
	GtkWidget *createroom;
	GtkWidget *leaveroom;
	GtkWidget *send;

	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *halign;
	GtkWidget *valign;

	GdkColor color;
	color.red = 0xffff;
	color.green = 0xffff;
	color.blue = 0;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 1200, 600);
	gtk_window_set_title(GTK_WINDOW(window), "IRCClient");
	gtk_container_set_border_width(GTK_CONTAINER(window), 15);
	gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);

	vbox = gtk_vbox_new(FALSE, 5);

	valign = gtk_alignment_new(0, 1, 0, 0);
	gtk_container_add(GTK_CONTAINER(vbox), valign);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	hbox = gtk_hbox_new(TRUE, 150);

	adduser = gtk_button_new_with_label("ADD-USER");
	gtk_widget_set_size_request(adduser, 100, 30);
	gtk_container_add(GTK_CONTAINER(hbox), adduser);
	enterroom = gtk_button_new_with_label("Enter Room");
	gtk_widget_set_size_request(enterroom, 100, 30);
	gtk_container_add(GTK_CONTAINER(hbox), enterroom);
	createroom = gtk_button_new_with_label("Create Room");
	gtk_widget_set_size_request(createroom, 100, 30);
	gtk_container_add(GTK_CONTAINER(hbox), createroom);
	leaveroom = gtk_button_new_with_label("Leave Room");
	gtk_widget_set_size_request(leaveroom, 100, 30);
	gtk_container_add(GTK_CONTAINER(hbox), leaveroom);
	send = gtk_button_new_with_label("Send");
	gtk_widget_set_size_request(send, 100, 30);
	gtk_container_add(GTK_CONTAINER(hbox), send);

	halign = gtk_alignment_new(1, 0, 0, 0);
	gtk_container_add(GTK_CONTAINER(halign), hbox);

	gtk_box_pack_start(GTK_BOX(vbox), halign, FALSE, FALSE, 0);

	//when clicked
	g_signal_connect(G_OBJECT(adduser), "clicked", 
			G_CALLBACK(add_user), NULL);

	g_signal_connect(G_OBJECT(enterroom), "clicked", 
			G_CALLBACK(enter_room), NULL);

	g_signal_connect(G_OBJECT(createroom), "clicked",
			G_CALLBACK(create_room), NULL);

	g_signal_connect(G_OBJECT(leaveroom), "clicked",
			G_CALLBACK(leave_room), NULL);

	g_signal_connect(G_OBJECT(send), "clicked",
			G_CALLBACK(send_), NULL);
	//when hover
	g_signal_connect(G_OBJECT(adduser), "enter", 
	      G_CALLBACK(hover_button), NULL);
	
	g_signal_connect(G_OBJECT(enterroom), "enter", 
	      G_CALLBACK(hover_button), NULL);

	g_signal_connect(G_OBJECT(leaveroom), "enter", 
	      G_CALLBACK(hover_button), NULL);

	g_signal_connect(G_OBJECT(createroom), "enter", 
	      G_CALLBACK(hover_button), NULL);
	
	g_signal_connect(G_OBJECT(send), "enter", 
	      G_CALLBACK(hover_button), NULL);
	
	
	g_signal_connect_swapped(G_OBJECT(window), "destroy",
			G_CALLBACK(gtk_main_quit), G_OBJECT(window));

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}


