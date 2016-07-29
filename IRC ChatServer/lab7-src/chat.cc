nclude <time.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CAMERA_WIDTH 128
#define CAMERA_HEIGHT 128


char * host;
char * user;
char * password;
char * args;
char * room;
char * sport;
int port;

#define MAX_MESSAGES 100
#define MAX_MESSAGE_LEN 300
#define MAX_RESPONSE (20 * 1024)

int lastMessage = 0;




/* connecting to socket */
int open_client_socket(char * host, int port) {
	struct  sockaddr_in socketAddress;

	memset((char *)&socketAddress,0,sizeof(socketAddress));

	socketAddress.sin_family = AF_INET;

	socketAddress.sin_port = htons((u_short)port);

	struct  hostent  *ptrh = gethostbyname(host);
	if ( ptrh == NULL ) {
		perror("gethostbyname");
		exit(1);
	}

	memcpy(&socketAddress.sin_addr, ptrh->h_addr, ptrh->h_length);

	struct  protoent *ptrp = getprotobyname("tcp");
	if ( ptrp == NULL ) {
		perror("getprotobyname");
		exit(1);
	}

	int sock = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
	if (sock < 0) {
		perror("socket");
		exit(1);
	}

	if (connect(sock, (struct sockaddr *)&socketAddress,
				sizeof(socketAddress)) < 0) {
		perror("connect");
		exit(1);
	}

	return sock;
}

int sendCommand(char * host, int port, char * command, char * user,
		char * password, char * args, char * response) {
	int sock = open_client_socket( host, port);

	write(sock, command, strlen(command));
	write(sock, " ", 1);
	write(sock, user, strlen(user));
	write(sock, " ", 1);
	write(sock, password, strlen(password));
	write(sock, " ", 1);
	write(sock, args, strlen(args));
	write(sock, "\r\n",2);

	int n = 0;
	int len = 0;
	while ((n=read(sock, response+len, MAX_RESPONSE - len))>0) {
		len += n;
	}

	printf("response:%s\n", response);

	close(sock);
}

void printUsage()
{
	printf("Usage: talk-client host port user password\n");
	exit(1);
}

void add_user() {
	char response[ MAX_RESPONSE ];
	sendCommand(host, port, "ADD-USER", user, password, "", response);

	if (!strcmp(response,"OK\r\n")) {
		printf("User %s added\n", user);
	}
}
void enter_room() {
	char response[ MAX_RESPONSE ];
	sendCommand(host, port, "ENTER-ROOM", user, password, room, response);

	if (!strcmp(response,"OK\r\n")) {
		printf("User %s added to room %s\n", user, room);
	}
}
/* callback from the entry button */
void send_(GtkWidget *widget, GtkWidget *type)
{
	const gchar *type_text;
	type_text = gtk_entry_get_text (GTK_ENTRY (type));
	printf ("MESSAGE SENT:\t %s\n", type_text);
}

/* callback when a button is pressed */
void callback(GtkWidget *button, gpointer data)
{
	printf("Button Presses:\t %s\n", data);
}

/* close the window */
gint delete_event( GtkWidget *widget, GdkEvent  *event, gpointer data )
{
	gtk_main_quit ();
	return(FALSE);
}

/* createUser  popup window */
gint createUser (GtkButton *button, GtkWindow *parent)
{
	GtkWidget *dialog, *table, *username, *pass;
	GtkWidget *lbl1, *lbl2;
	gint result;

	dialog = gtk_dialog_new_with_buttons ("Edit User Information", NULL
			GTK_DIALOG_MODAL,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_OK);


	/* Create four entries that will tell the user what data to enter. */
	lbl1 = gtk_label_new ("UserName:");
	lbl2 = gtk_label_new ("PassWord:");
	user = gtk_entry_new ();
	home = gtk_entry_new ();
	/* Retrieve the user's information for the default values. */
	gtk_entry_set_text (GTK_ENTRY (username), "User Name");
	gtk_entry_set_text (GTK_ENTRY (roomname), "Password");

	table = gtk_table_new (4, 2, FALSE);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl1, 0, 1, 0, 1);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl2, 0, 1, 1, 2);

	gtk_table_attach_defaults (GTK_TABLE (table), username, 1, 2, 0, 1);
	gtk_table_attach_defaults (GTK_TABLE (table), pass, 1, 2, 2, 3);

	gtk_table_set_row_spacings (GTK_TABLE (table), 5);
	gtk_table_set_col_spacings (GTK_TABLE (table), 5);
	gtk_container_set_border_width (GTK_CONTAINER (table), 5);
	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), table);

	gtk_widget_show_all (dialog);
	/* Run the dialog and output the data if the user clicks the OK button. */
	result = gtk_dialog_run (GTK_DIALOG (dialog));
	if (result == GTK_RESPONSE_OK)
	{
		const gchar * n = gtk_entry_get_text (GTK_ENTRY (username));
		const gchar * p = gtk_entry_get_text (GTK_ENTRY (pass));
		g_print ("User Name: %s\n", gtk_entry_get_text (GTK_ENTRY (username)));
		g_print ("Host Name: %s\n", gtk_entry_get_text (GTK_ENTRY (pass)));
		user = strdup(n);
		password = strdup(p);
	}

	gtk_widget_destroy (dialog);
	return 0;

}

/* createRoom popup window */
gint createRoom (GtkButton *button, GtkWindow *parent)
{
	GtkWidget *dialog, *table, *username, *roomname;
	GtkWidget *lbl1, *lbl2;
	gint result;

	dialog = gtk_dialog_new_with_buttons ("Edit User Information", NULL
			GTK_DIALOG_MODAL,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_OK);


	/* Create four entries that will tell the user what data to enter. */
	lbl1 = gtk_label_new ("UserName:");
	lbl2 = gtk_label_new ("RoomName:");
	user = gtk_entry_new ();
	home = gtk_entry_new ();
	/* Retrieve the user's information for the default values. */
	gtk_entry_set_text (GTK_ENTRY (username), "User Name");
	gtk_entry_set_text (GTK_ENTRY (roomname), "Room Name");

	table = gtk_table_new (4, 2, FALSE);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl1, 0, 1, 0, 1);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl2, 0, 1, 1, 2);

	gtk_table_attach_defaults (GTK_TABLE (table), username, 1, 2, 0, 1);
	gtk_table_attach_defaults (GTK_TABLE (table), roomname, 1, 2, 2, 3);

	gtk_table_set_row_spacings (GTK_TABLE (table), 5);
	gtk_table_set_col_spacings (GTK_TABLE (table), 5);
	gtk_container_set_border_width (GTK_CONTAINER (table), 5);
	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), table);

	gtk_widget_show_all (dialog);
	/* Run the dialog and output the data if the user clicks the OK button. */
	result = gtk_dialog_run (GTK_DIALOG (dialog));
	if (result == GTK_RESPONSE_OK)
	{
		const gchar * n = gtk_entry_get_text (GTK_ENTRY (username));
		const gchar * r = gtk_entry_get_text (GTK_ENTRY (roomname));
		g_print ("User Name: %s\n", gtk_entry_get_text (GTK_ENTRY (username)));
		g_print ("Host Name: %s\n", gtk_entry_get_text (GTK_ENTRY (roomname)));
		user = strdup(n);
		room = strdup(r);
	}

	gtk_widget_destroy (dialog);
	return 0;

}

/* enterRoom  popup window */
gint enterRoom (GtkButton *button, GtkWindow *parent)
{
	GtkWidget *dialog, *table, *roomname;
	GtkWidget *lbl1, *lbl2;
	gint result;

	dialog = gtk_dialog_new_with_buttons ("Edit User Information", NULL
			GTK_DIALOG_MODAL,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_OK);


	/* Create four entries that will tell the user what data to enter. */
	lbl1 = gtk_label_new ("RoomName:");
	roomname = gtk_entry_new ();
	/* Retrieve the user's information for the default values. */
	gtk_entry_set_text (GTK_ENTRY (roomname), "Room Name");

	table = gtk_table_new (4, 2, FALSE);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl1, 0, 1, 0, 1);

	gtk_table_attach_defaults (GTK_TABLE (table), roomname, 1, 2, 0, 1);

	gtk_table_set_row_spacings (GTK_TABLE (table), 5);
	gtk_table_set_col_spacings (GTK_TABLE (table), 5);
	gtk_container_set_border_width (GTK_CONTAINER (table), 5);
	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), table);

	gtk_widget_show_all (dialog);
	/* Run the dialog and output the data if the user clicks the OK button. */
	result = gtk_dialog_run (GTK_DIALOG (dialog));
	if (result == GTK_RESPONSE_OK)
	{
		const gchar * r = gtk_entry_get_text (GTK_ENTRY (roomname));
		g_print ("Room Name: %s\n", gtk_entry_get_text (GTK_ENTRY (roomname)));
		room = strdup(r);
	}

	gtk_widget_destroy (dialog);
	return 0;

}

/* enterRoom  popup window */
gint enterRoom (GtkButton *button, GtkWindow *parent)
{
	GtkWidget *dialog, *table, *roomname;
	GtkWidget *lbl1, *lbl2;
	gint result;

	dialog = gtk_dialog_new_with_buttons ("Edit User Information", NULL
			GTK_DIALOG_MODAL,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_OK);


	/* Create four entries that will tell the user what data to enter. */
	lbl1 = gtk_label_new ("RoomName:");
	roomname = gtk_entry_new ();
	/* Retrieve the user's information for the default values. */
	gtk_entry_set_text (GTK_ENTRY (roomname), "Room Name");

	table = gtk_table_new (4, 2, FALSE);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl1, 0, 1, 0, 1);

	gtk_table_attach_defaults (GTK_TABLE (table), roomname, 1, 2, 0, 1);

	gtk_table_set_row_spacings (GTK_TABLE (table), 5);
	gtk_table_set_col_spacings (GTK_TABLE (table), 5);
	gtk_container_set_border_width (GTK_CONTAINER (table), 5);
	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), table);

	gtk_widget_show_all (dialog);
	/* Run the dialog and output the data if the user clicks the OK button. */
	result = gtk_dialog_run (GTK_DIALOG (dialog));
	if (result == GTK_RESPONSE_OK)
	{
		const gchar * r = gtk_entry_get_text (GTK_ENTRY (roomname));
		g_print ("Room Name: %s\n", gtk_entry_get_text (GTK_ENTRY (roomname)));
		room = strdup(r);
	}       

	gtk_widget_destroy (dialog);
	return 0;

}


int main ( int argc, char *argv[]) 
{
	GtkWidget *window;
	GtkWidget *table;
	GtkWidget *notebook;
	GtkWidget *frame;
	GtkWidget *label;
	GtkWidget *type;
	GtkWidget *createroom;
	GtkWidget *createaccount;
	GtkWidget *leaveroom;
	GtkWidget *send;
	GtkWidget *title;
	GtkWidget *image;
	GtkWidget *avirooms;
	GtkWidget *usersinroom;
	GtkWidget *enterroom;

	gint tmp_pos;

	char line[MAX_MESSAGE_LEN+1];

	if (argc < 5) {
		printUsage();
	}

	host = argv[1];
	sport = argv[2];
	user = argv[3];
	password = argv[4];

	printf("\nStarting talk-client %s %s %s %s\n", host, sport, user, password);
	sscanf(sport, "%d", &port);

	char lName[32];
	char fName[32];

	GdkColor color;
	color.red = 0xffff;
	color.green = 0xffff;
	color.blue = 0;

	gtk_init (&argc, &argv);

	/* Setting up the GUI */

	/* Create a window and set it up */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 1400, 1200);
	gtk_window_set_title(GTK_WINDOW(window), "IRCClient");
	gtk_container_set_border_width(GTK_CONTAINER(window), 15);
	gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);

	/*create a table and set it up*/
	table = gtk_table_new(10, 10, TRUE);
	gtk_table_set_col_spacings(GTK_TABLE(table), 20);
	gtk_container_add (GTK_CONTAINER (window), table);

	/*create a notebook and set it up*/
	notebook = gtk_notebook_new();
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
	gtk_table_attach_defaults(GTK_TABLE(table), notebook, 0, 8, 0, 8);
	gtk_widget_show(notebook);

	/*append a single page to the notebook*/
	sprintf(lName, "Room Name");// should be inputted by user
	frame = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(frame), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(frame), TRUE);
	gtk_widget_show (frame);
	/* label the first page in the notebook*/
	label = gtk_label_new(lName);
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);

	/* set tabs to first page*/
	gtk_notebook_set_page (GTK_NOTEBOOK(notebook), 0);

	/*create a send message button*/
	send = gtk_button_new_with_label("Send");
	gtk_table_attach(GTK_TABLE(table), send, 7,8,8,9, GTK_FILL, GTK_FILL, 1, 27);
	gtk_signal_connect (GTK_OBJECT (send), "clicked", GTK_SIGNAL_FUNC (callback), (gpointer) "Button clicked:\t Send");
	gtk_widget_show(send);

	/*create an editable message box */
	type = gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY (type), 1000);
	gtk_table_attach(GTK_TABLE(table), type, 0, 7, 8, 9, GTK_FILL,GTK_FILL,1 , 1);
	gtk_entry_set_text (GTK_ENTRY (type), "Hello");

	tmp_pos = GTK_ENTRY (type)->text_length;
	gtk_editable_insert_text (GTK_EDITABLE (type), " World", -1, &tmp_pos);
	gtk_editable_select_region (GTK_EDITABLE (type), 0, GTK_ENTRY (type)->text_length);
	g_signal_connect (send, "clicked", G_CALLBACK (send_),  type);
	gtk_widget_show(type);

	/* add a createroom button */
	createroom = gtk_button_new_with_label("CREATE ROOM");
	gtk_signal_connect (GTK_OBJECT (createroom), "clicked", GTK_SIGNAL_FUNC (callback), (gpointer) "Button clicked:\t Create Room");
	g_signal_connect (G_OBJECT (createroom), "clicked", G_CALLBACK (createRoom), (gpointer) window);

	gtk_table_attach (GTK_TABLE(table), createroom, 2, 4, 9, 10, GTK_FILL, GTK_FILL, 1, 25);
	gtk_widget_show (createroom);

	/* add a CreateAccount  button */
	createaccount = gtk_button_new_with_label("NEW ACCOUNT");
	gtk_signal_connect (GTK_OBJECT (createaccount), "clicked", GTK_SIGNAL_FUNC (callback), (gpointer) "Button clicked:\t Create Account");
	g_signal_connect (G_OBJECT (createaccount), "clicked", G_CALLBACK (createUser), (gpointer) window);

	gtk_table_attach (GTK_TABLE(table), createaccount, 0, 2, 9, 10, GTK_FILL, GTK_FILL, 1, 25);
	gtk_widget_show (createaccount);

	/* add a enterroom  button */
	enterroom = gtk_button_new_with_label("ENTER ROOM");
	gtk_signal_connect (GTK_OBJECT (enterroom), "clicked", GTK_SIGNAL_FUNC (callback), (gpointer) "Button clicked:\t Enter Room");
	g_signal_connect (G_OBJECT (enterroom), "clicked", G_CALLBACK (enterRoom), (gpointer) window);
	gtk_table_attach (GTK_TABLE(table), enterroom, 6, 8, 9, 10, GTK_FILL, GTK_FILL, 1, 25);
	gtk_widget_show (enterroom);

	/* add a leaveroom  button */
	leaveroom = gtk_button_new_with_label("LEAVE ROOM");
	gtk_signal_connect (GTK_OBJECT (leaveroom), "clicked", GTK_SIGNAL_FUNC (callback), (gpointer) "Button clicked:\t Leave Room");
	g_signal_connect (G_OBJECT (leaveroom), "clicked", G_CALLBACK (enterRoom), (gpointer) window);

	gtk_table_attach (GTK_TABLE(table), leaveroom, 4, 6, 9, 10, GTK_FILL, GTK_FILL, 1, 25);
	gtk_widget_show (leaveroom);

	/* create a listview of users in room*/

	title = gtk_label_new("Users in Rooms");
	gtk_table_attach(GTK_TABLE(table), title, 8, 10, 0, 1,
			GTK_FILL, GTK_FILL, 0, 0);
	gtk_widget_show(title);

	usersinroom = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(usersinroom), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(usersinroom), TRUE);
	gtk_table_attach(GTK_TABLE(table), usersinroom, 8, 10, 1, 5,  GTK_FILL, GTK_FILL, 1, 1);
	gtk_widget_show(usersinroom);

	/* create a listview of available rooms*/

	title = gtk_label_new("Available Rooms");
	gtk_table_attach(GTK_TABLE(table), title, 8, 10, 5, 6,
			GTK_FILL, GTK_FILL, 0, 0);
	gtk_widget_show(title);

	avirooms = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(avirooms), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(avirooms), TRUE);
	gtk_table_attach(GTK_TABLE(table), avirooms, 8, 10, 6, 10,  GTK_FILL, GTK_FILL, 1, 1);
	gtk_widget_show(avirooms);

	/*show the table and window*/
	gtk_widget_show (table);
	gtk_widget_show (window);

	/*make the window closeable*/
	gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (delete_event), NULL);

	gtk_main ();

	/* setting up the talk -client */
}


