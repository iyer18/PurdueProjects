# include <gtk/gtk.h>

/* Our callback.
 *  * The data passed to this function is printed to stdout */
void callback( GtkWidget *widget,
		gpointer   data )
{
	g_print ("%s\n", (char *) data);
}

void add_user(GtkWidget *widget, gpointer data)
{
	//g_print("ADDED-USER\n");
}

void enter_room(GtkWidget *widget, gpointer data)
{
	//g_print("ENTERED-ROOM\n");
}

void create_room(GtkWidget *widget, gpointer data)
{
	//g_print("CREATED-ROOM\n");
}

void leave_room(GtkWidget *widget, gpointer data)
{
	//g_print("LEFT-ROOM\n");
}

void send_(GtkWidget *widget, GtkWidget *type)
{
	const gchar *type_text;
	type_text = gtk_entry_get_text (GTK_ENTRY (type));
	printf ("Message: %s\n", type_text);
}

void hover_button(GtkWidget *widget, gpointer data)
{
	GdkColor color;
	color.red = 27000;
	color.green = 30325;
	color.blue = 34181;
	gtk_widget_modify_bg(widget, GTK_STATE_PRELIGHT, &color);
}





/* This callback quits the program */
gint delete_event( GtkWidget *widget,
		GdkEvent  *event,
		gpointer   data )
{
	gtk_main_quit ();
	return(FALSE);
}

int main( int   argc,
		char *argv[] )
{
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *table;
	GtkWidget *adduser;
	GtkWidget *enterroom;
	GtkWidget *createroom;
	GtkWidget *leaveroom;
	GtkWidget *send;
	GtkWidget *users;
	GtkWidget *rooms;
	GtkWidget *members;
	GtkWidget *messages;
	GtkWidget *type;
	GtkWidget *title;

	gint tmp_pos;

	GdkColor color;
	color.red = 0xffff;
	color.green = 0xffff;
	color.blue = 0;

	gtk_init (&argc, &argv);

	/* Create a new window */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 1000, 600);
	gtk_window_set_title(GTK_WINDOW(window), "IRCClient");
	gtk_container_set_border_width(GTK_CONTAINER(window), 15);
	gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);


	gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (delete_event), NULL);

	/* Create a 2x2 table */
	table = gtk_table_new (8, 5, TRUE);
	gtk_table_set_col_spacings(GTK_TABLE(table), 20);
	/* Put the table in the main window */
	gtk_container_add (GTK_CONTAINER (window), table);

	/* Create first button */
	adduser = gtk_button_new_with_label ("ADD-USER");
	gtk_widget_set_size_request(adduser, 100, 30);

	g_signal_connect(G_OBJECT(adduser), "enter",
			G_CALLBACK(hover_button), NULL);

	gtk_signal_connect (GTK_OBJECT (adduser), "clicked",
			GTK_SIGNAL_FUNC (callback), (gpointer) "ADDED USER");

	gtk_table_attach (GTK_TABLE(table), adduser, 0, 1, 7, 8, GTK_FILL, GTK_FILL, 50, 20);

	gtk_widget_show (adduser);

	/* Create second button */

	createroom = gtk_button_new_with_label("Create Room");
	gtk_widget_set_size_request(createroom, 100, 30);

	g_signal_connect(G_OBJECT(createroom), "enter",
			G_CALLBACK(hover_button), NULL);

	gtk_signal_connect (GTK_OBJECT (createroom), "clicked",
			GTK_SIGNAL_FUNC (callback), (gpointer) "ROOM CREATED");

	gtk_table_attach (GTK_TABLE(table), createroom, 3, 4, 0, 1, GTK_FILL, GTK_FILL, 50, 20);
	gtk_widget_show (createroom);

	/* Create third button */

	enterroom = gtk_button_new_with_label("Enter Room");
	gtk_widget_set_size_request(enterroom, 100, 30);

	g_signal_connect(G_OBJECT(enterroom), "enter",
			G_CALLBACK(hover_button), NULL);

	gtk_signal_connect (GTK_OBJECT (enterroom), "clicked",
			GTK_SIGNAL_FUNC (callback), (gpointer) "ENTERED ROOM");

	gtk_table_attach (GTK_TABLE(table), enterroom, 1, 2, 7, 8, GTK_FILL, GTK_FILL, 50, 20);

	gtk_widget_show (enterroom);

	/* Create fourth button */

	leaveroom = gtk_button_new_with_label("Leave Room");
	gtk_widget_set_size_request(leaveroom, 100, 30);

	g_signal_connect(G_OBJECT(leaveroom), "enter",
			G_CALLBACK(hover_button), NULL);

	gtk_signal_connect (GTK_OBJECT (leaveroom), "clicked",
			GTK_SIGNAL_FUNC (callback), (gpointer) "LEFT ROOM");

	gtk_table_attach (GTK_TABLE(table), leaveroom, 4, 5, 0, 1, GTK_FILL, GTK_FILL, 50, 20);

	gtk_widget_show (leaveroom);

	/* room user title */

	title = gtk_label_new("Available Users");
	gtk_table_attach(GTK_TABLE(table), title, 0, 1, 0, 1,
			GTK_FILL, GTK_FILL, 0, 0);

	gtk_widget_show(title);
	/* Create user list */

	users = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(users), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(users), TRUE);

	gtk_table_attach(GTK_TABLE(table), users, 0, 1, 1, 7,  GTK_FILL, GTK_FILL, 1, 1);

	gtk_widget_show(users);

	/* room list title */

	title = gtk_label_new("Available Rooms");
	gtk_table_attach(GTK_TABLE(table), title, 1, 2, 0, 1, 
			GTK_FILL, GTK_FILL, 0, 0);

	gtk_widget_show(title);

	/* Create room list */

	rooms = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(rooms), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(rooms), TRUE);

	gtk_table_attach(GTK_TABLE(table), rooms, 1, 2, 1, 7,  GTK_FILL, GTK_FILL, 1, 1);

	gtk_widget_show(rooms);

	/* room users in current room title */

	title = gtk_label_new("Users in this Room");
	gtk_table_attach(GTK_TABLE(table), title, 2, 3, 0, 1,
			GTK_FILL, GTK_FILL, 0, 0);

	gtk_widget_show(title);
	/* Create members in room list */
	members = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(members), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(members), TRUE);

	gtk_table_attach(GTK_TABLE(table), members, 2, 3, 1, 7,  GTK_FILL, GTK_FILL, 1, 1);

	gtk_widget_show(members);

	/* room Chat history title */

	title = gtk_label_new("Chat History");
	gtk_table_attach(GTK_TABLE(table), title, 3, 4, 1, 2,
			GTK_FILL, GTK_FILL, 0, 0);

	gtk_widget_show(title);
	/* Create messages list */
	messages = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(messages), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(messages), TRUE);

	gtk_table_attach(GTK_TABLE(table), messages, 3, 5, 2,5,  GTK_FILL, GTK_FILL, 1, 1);

	gtk_widget_show(messages);

	/* room enter message title */

	title = gtk_label_new("Enter Message");
	gtk_table_attach(GTK_TABLE(table), title, 3, 4, 5, 6,
			GTK_FILL, GTK_FILL, 0, 0);

	gtk_widget_show(title);

	/* Create enter message editable box */
	type = gtk_entry_new();

	gtk_entry_set_max_length (GTK_ENTRY (type), 1000);
	gtk_table_attach(GTK_TABLE(table), type, 3, 5, 6,7,  GTK_FILL, GTK_FILL, 1, 1);

	gtk_entry_set_text (GTK_ENTRY (type), "Hello");


	tmp_pos = GTK_ENTRY (type)->text_length;
	gtk_editable_insert_text (GTK_EDITABLE (type), " World", -1, &tmp_pos);
	gtk_editable_select_region (GTK_EDITABLE (type), 0, GTK_ENTRY (type)->text_length);
	g_signal_connect (send, "clicked", G_CALLBACK (send_),  type);

	gtk_widget_show(type);

	/* Create fifth button */

	send = gtk_button_new_with_label("Send");
	gtk_widget_set_size_request(send, 100, 30);

	g_signal_connect(G_OBJECT(send), "enter",
			G_CALLBACK(hover_button), NULL);

	g_signal_connect (send, "clicked", G_CALLBACK (send_),  type);
///	gtk_signal_connect (GTK_OBJECT (send), "clicked",
//			GTK_SIGNAL_FUNC (callback), (gpointer) "MESSAGE SENT");

	gtk_table_attach (GTK_TABLE(table), send, 4, 5, 7, 8,  GTK_FILL, GTK_FILL, 50, 20);

	gtk_widget_show (send);

	//gtk_widget_show (button);
	gtk_widget_show (table);
	gtk_widget_show (window);

	gtk_main ();

	return 0;
}
