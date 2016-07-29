#include <stdio.h>
#include <gtk/gtk.h>
#include <time.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


void update_list_users();
char * host;
char * user;
char * password;
char * args;
char * message;
char * sport;
int port;

#define MAX_MESSAGES 100
#define MAX_MESSAGE_LEN 300
#define MAX_RESPONSE (20 * 1024)

int lastMessage = 0;

GtkListStore * list_rooms;
GtkListStore *list_users;
GtkListStore *list_messages;

int open_client_socket(char * host, int port) {
	//	Initialize socket address structure
	struct  sockaddr_in socketAddress;

	// Clear sockaddr structure
	memset((char *)&socketAddress,0,sizeof(socketAddress));

	// Set family to Internet
	socketAddress.sin_family = AF_INET;

	// Set port
	socketAddress.sin_port = htons((u_short)port);

	// Get host table entry for this host
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

	response [len] = '\0';
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
	char response[MAX_RESPONSE];

	if( user!=NULL && password!=NULL)
	{
		sendCommand(host, port, "ENTER-ROOM", user, password, args, response);
		char * temp = strdup(args);
		strcat(temp, " has entered room");
		sendCommand(host, port, "SEND-MESSAGE", user, password, temp, response);
		sendCommand(host, port, "SEND-MESSAGE", user, password, temp, response);
		update_list_users();
		/*	if (!strcmp(response,"ERROR (Wrong password)\r\n")) {
			printf("Wrong password\n");
			exit(1);
			}*/
	}
}

void leave_room() {
	char response[MAX_RESPONSE];
	if( user!=NULL && password!=NULL)
	{
		sendCommand(host, port, "LEAVE-ROOM", user, password, args, response);
		char * temp = strdup(args);
		strcat(temp, " has left room");
		sendCommand(host,port,"SEND-MESSAGE", user, password, temp, response);
		sendCommand(host,port,"SEND-MESSAGE", user, password, temp, response);
		update_list_users();
	}
}

void get_messages() {

}

void send_message() {
	char response[ MAX_RESPONSE ];
	if( user!=NULL && password!=NULL && args != NULL)
	{
		const gchar * temp = strdup(args);
		strcat(args, " ");
		strcat(args, message);

		sendCommand(host, port, "SEND-MESSAGE", user, password, args, response);
		args = strdup(temp);
	}
}

void print_users_in_room() {
	char response[MAX_RESPONSE];
	if( user!=NULL && password!=NULL)
	{
		sendCommand(host,port,"GET-USERS-IN-ROOM", user, password, "", response);
		char *s;
		s = strtok (response, "\r\n");
		while (s != NULL) {
			if (strlen(s) > 1) {
				printf("%s", s);
				printf("\n");
			}
			else
				;
			s = strtok(NULL, "\r\n");
		}
	}
}

void print_users() {
	char response[MAX_RESPONSE];
	if( user!=NULL && password!=NULL)
	{
		sendCommand(host,port,"GET-ALL-USERS", user, password, "", response);
		char *s;
		s = strtok(response, "\r\n");
		while (s != NULL) {
			printf("%s", s);
			s = strtok(NULL, "\r\n");
		}
	}
}
void create_room() {
	if( user!=NULL && password!=NULL && args != NULL)
	{
		char response[ MAX_RESPONSE ];
		sendCommand(host, port, "CREATE-ROOM", user, password, args, response);
	}
}


void update_list_rooms() {
	GtkTreeIter iter;
	int i=0;
	char response[MAX_RESPONSE];
	response[0] = '\0';
	//printf( "1\n");
	if( user!=NULL && password!=NULL )
	{
		//printf( "2\n");
		sendCommand(host,port,"LIST-ROOMS", user, password, "", response);
		//printf( "3\n");
	}



	/* Clear rooms list*/
	gtk_list_store_clear(GTK_LIST_STORE (list_rooms));

	char *l_rooms = strtok(response,"\r\n");

	/*if(l_rooms[i]==NULL) {
	  exit(0);
	  }*/

	/* Add differetn rooms to list_rooms */
	while (l_rooms != '\0') {
		//printf( "4\n");
		gchar *msg = g_strdup_printf (l_rooms);
		gtk_list_store_append (GTK_LIST_STORE (list_rooms), &iter);
		gtk_list_store_set (GTK_LIST_STORE (list_rooms),
				&iter,
				0, msg,
				-1);
		g_free (msg);
		//i++;
		//printf( "5\n");
		l_rooms = strtok(NULL,"\r\n");
		//printf( "6\n");
		if(l_rooms==NULL) {
			//	printf( "7\n");
			break;
		}
	}


}
void update_list_users() {
	GtkTreeIter iter;
	int i=0;
	char response[MAX_RESPONSE];
	response[0] = '\0';
	//printf( "1\n");
	if( user!=NULL && password!=NULL && args!=NULL)
	{
		//printf( "2\n");
		sendCommand(host,port,"GET-USERS-IN-ROOM", user, password, args, response);
	}



	/* Clear rooms list*/
	gtk_list_store_clear(GTK_LIST_STORE (list_users));

	char *users_in_rooms = strtok(response,"\r\n");

	/*if(users_in_rooms[i]==NULL) {
	  exit(0);
	  }*/

	/* Add differetn rooms to list_rooms */
	while (users_in_rooms != '\0') {
		//printf( "3\n");
		gchar *msg = g_strdup_printf (users_in_rooms);
		//printf( "4\n");
		gtk_list_store_append (GTK_LIST_STORE (list_users), &iter);
		//printf( "5\n");
		gtk_list_store_set (GTK_LIST_STORE (list_users),
				&iter,
				0, msg,
				-1);
		g_free (msg);
		//i++;
		//printf( "6\n");
		users_in_rooms = strtok(NULL,"\r\n");
		//printf( "7\n");
		if(users_in_rooms==NULL) {
			//	printf( "8\n");
			break;
		}
	}

}
void update_list_messages() {
	GtkTreeIter iter;
	int i=0;
	char response[MAX_RESPONSE];
	response[0] = '\0';
	printf( "1\n");

	if( user!=NULL && password!=NULL && args!=NULL)
	{
		printf("2\n");
		const gchar *temp_room = strdup(args);
		const gchar *temp = "0 ";
		printf("3\n");
		args = strdup(temp);
		strcat(args,temp_room);
		sendCommand(host,port,"GET-MESSAGES", user, password,args, response);
		printf("4\n");
		args = strdup(temp_room);
		printf("5\n");
	}
	printf("6\n");



	/* Clear rooms list*/
	gtk_list_store_clear(GTK_LIST_STORE (list_messages));

	char *messages_in_room = strtok(response,"\r\n");

	/*if(users_in_rooms[i]==NULL) {
	  exit(0);
	  }*/

	/* Add differetn rooms to list_rooms */
	while (messages_in_room != '\0') {
		printf( "7\n");
		gchar *msg = g_strdup_printf (messages_in_room);
		printf( "8\n");
		gtk_list_store_append (GTK_LIST_STORE (list_messages), &iter);
		printf( "9\n");
		gtk_list_store_set (GTK_LIST_STORE (list_messages),
				&iter,
				0, msg,
				-1);
		g_free (msg);
		//i++;
		printf( "10\n");
		messages_in_room = strtok(NULL,"\r\n");
		printf( "11\n");
		if(messages_in_room == NULL) {
			printf( "12\n");
			break;
		}
	}

}

void accountButton(GtkButton * button, GtkWindow *window) {
	GtkWidget *dialog,*table,*uName,*pWord;
	GtkWidget *lbl1, *lbl2;
	gint result;

	dialog = gtk_dialog_new_with_buttons ("Information", window, GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
	lbl1 = gtk_label_new("User Name:");
	lbl2 = gtk_label_new("Password:");

	uName = gtk_entry_new();
	pWord  = gtk_entry_new();

	table = gtk_table_new(4,2,FALSE);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl1, 0, 1, 0, 1);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl2, 0, 1, 1, 2);

	gtk_table_attach_defaults (GTK_TABLE (table), uName, 1, 2, 0, 1);
	gtk_table_attach_defaults (GTK_TABLE (table), pWord, 1, 2, 1, 2);


	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), table);
	gtk_widget_show_all(dialog);

	result = gtk_dialog_run(GTK_DIALOG (dialog));
	if(result == GTK_RESPONSE_OK) {
		const gchar* dup_user = gtk_entry_get_text(GTK_ENTRY(uName));
		const gchar* dup_password = gtk_entry_get_text(GTK_ENTRY(pWord));
		user = strdup(dup_user);
		password = strdup(dup_password);
		add_user();
	}
	gtk_widget_destroy(dialog);

}
void enterRoomButton(GtkButton * button, GtkWindow *window) {
	GtkWidget *dialog,*table,*eName;
	GtkWidget *lbl1;
	gint result;

	dialog = gtk_dialog_new_with_buttons ("Information", window, GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
	lbl1 = gtk_label_new("EnterRoom:");

	eName = gtk_entry_new();

	table = gtk_table_new(4,2,FALSE);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl1, 0, 1, 0, 1);

	gtk_table_attach_defaults (GTK_TABLE (table), eName, 1, 2, 0, 1);


	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), table);
	gtk_widget_show_all(dialog);

	result = gtk_dialog_run(GTK_DIALOG (dialog));
	if(result == GTK_RESPONSE_OK) {
		const gchar* dup_r = gtk_entry_get_text(GTK_ENTRY(eName));
		args = strdup(dup_r);
		enter_room();
	}
	gtk_widget_destroy(dialog);

}
void leaveRoomButton(GtkButton * button, GtkWindow *window) {
	GtkWidget *dialog,*table,*eName;
	GtkWidget *lbl1;
	gint result;

	dialog = gtk_dialog_new_with_buttons ("Information", window, GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
	lbl1 = gtk_label_new("LeaveRoom:");

	eName = gtk_entry_new();

	table = gtk_table_new(4,2,FALSE);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl1, 0, 1, 0, 1);

	gtk_table_attach_defaults (GTK_TABLE (table), eName, 1, 2, 0, 1);


	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), table);
	gtk_widget_show_all(dialog);

	result = gtk_dialog_run(GTK_DIALOG (dialog));
	if(result == GTK_RESPONSE_OK) {
		const gchar* dup_r = gtk_entry_get_text(GTK_ENTRY(eName));
		args = strdup(dup_r);
		leave_room();
	}
	gtk_widget_destroy(dialog);

}



void roomButton(GtkButton * button, GtkWindow *window) {
	GtkWidget *dialog,*table,*rName;
	GtkWidget *lbl1;
	gint result;

	dialog = gtk_dialog_new_with_buttons ("Information", window, GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
	lbl1 = gtk_label_new("Room:");

	rName = gtk_entry_new();

	table = gtk_table_new(4,2,FALSE);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl1, 0, 1, 0, 1);

	gtk_table_attach_defaults (GTK_TABLE (table), rName, 1, 2, 0, 1);


	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), table);
	gtk_widget_show_all(dialog);

	result = gtk_dialog_run(GTK_DIALOG (dialog));
	if(result == GTK_RESPONSE_OK) {
		const gchar* dup_r = gtk_entry_get_text(GTK_ENTRY(rName));
		args = strdup(dup_r);
		create_room();
	}
	gtk_widget_destroy(dialog);

}



/* Create the list of "messages" */
static GtkWidget *create_list( const char * titleColumn, GtkListStore *model )
{
	GtkWidget *scrolled_window;
	GtkWidget *tree_view;
	/*GtkListStore *model;*/
	GtkCellRenderer *cell;
	GtkTreeViewColumn *column;

	int i;

	/* Create a new scrolled window, with scrollbars only if needed */
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
			GTK_POLICY_AUTOMATIC,
			GTK_POLICY_AUTOMATIC);

	/*model = gtk_list_store_new (1, G_TYPE_STRING);*/
	tree_view = gtk_tree_view_new ();
	gtk_container_add (GTK_CONTAINER (scrolled_window), tree_view);
	gtk_tree_view_set_model (GTK_TREE_VIEW (tree_view), GTK_TREE_MODEL (model));
	gtk_widget_show (tree_view);

	cell = gtk_cell_renderer_text_new ();

	column = gtk_tree_view_column_new_with_attributes (titleColumn,
			cell,
			"text", 0,
			NULL);

	gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view),
			GTK_TREE_VIEW_COLUMN (column));

	return scrolled_window;
}

/* Add some text to our text widget - this is a callback that is invoked
 *  when our window is realized. We could also force our window to be
 *   realized with gtk_widget_realize, but it would have to be part of
 *    a hierarchy first */

static void insert_text( GtkTextBuffer *buffer, const char * initialText )
{
	GtkTextIter iter;

	gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);
	gtk_text_buffer_insert (buffer, &iter, initialText,-1);
}

void send_msg (GtkWidget * window, GtkWidget *msg) {
	const gchar *temp;
	temp = gtk_entry_get_text (GTK_ENTRY (msg));
	message = strdup(temp);
	send_message();
}

/* Create a scrolled text area that displays a "message" */
static GtkWidget *create_text( const char * initialText )
{
	GtkWidget *scrolled_window;
	GtkWidget *view;
	GtkTextBuffer *buffer;

	view = gtk_text_view_new ();
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
			GTK_POLICY_AUTOMATIC,
			GTK_POLICY_AUTOMATIC);

	gtk_container_add (GTK_CONTAINER (scrolled_window), view);
	insert_text (buffer, initialText);

	gtk_widget_show_all (scrolled_window);

	return scrolled_window;
}

	static gboolean
time_handler(GtkWidget *widget)
{
	if (widget->window == NULL) return FALSE;

	/*gtk_widget_queue_draw(widget);*/

	fprintf(stderr, "Hi\n");
	//printf ("update list rooms\n");
	if(user != NULL && password != NULL) {
		printf ("update list rooms\n");
		update_list_rooms();
		printf ("update list users\n");
		update_list_messages();
	}
	return TRUE;
}


int main( int   argc,
		char *argv[] )
{
	GtkWidget *window;
	GtkWidget *list;
	GtkWidget *listUsers;
	GtkWidget *messages;
	GtkWidget *myMessage;
	char line[MAX_MESSAGE_LEN+1];


	host = argv[1];
	sport = argv[2];

	printf("\nStarting talk-client %s %s\n", host, sport);

	// Convert port to number
	sscanf(sport, "%d", &port);
	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "Paned Windows");
	g_signal_connect (window, "destroy",
			G_CALLBACK (gtk_main_quit), NULL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_widget_set_size_request (GTK_WIDGET (window), 450, 400);

	/* Create a table to place the widgets. Use a 7x4 Grid (7 rows x 4 columns)*/
	GtkWidget *table = gtk_table_new (7, 4, TRUE);
	gtk_container_add (GTK_CONTAINER (window), table);
	gtk_table_set_row_spacings(GTK_TABLE (table), 5);
	gtk_table_set_col_spacings(GTK_TABLE (table), 5);
	gtk_widget_show (table);

	/* Add list of rooms. Use columns 0 to 4 (exclusive) and rows 0 to 4 (exclusive)*/
	list_rooms = gtk_list_store_new (1, G_TYPE_STRING);
	//update_list_rooms();
	list = create_list ("Rooms", list_rooms);
	gtk_table_attach_defaults (GTK_TABLE (table), list, 2, 4, 0, 2);
	gtk_widget_show (list);

	list_users = gtk_list_store_new (1, G_TYPE_STRING);
	//update_list_users();
	listUsers = create_list ("Users", list_users);
	gtk_table_attach_defaults (GTK_TABLE (table), listUsers, 0, 2, 0, 2);
	gtk_widget_show (listUsers);

	/* Add messages text. Use columns 0 to 4 (exclusive) and rows 4 to 7 (exclusive)*/
	list_messages = gtk_list_store_new (1, G_TYPE_STRING);
	messages = create_list ("Messages", list_messages);
	gtk_table_attach_defaults (GTK_TABLE (table), messages, 0, 4, 2, 6);
	gtk_widget_show (messages);
	/* Add messages text. Use columns 0 to 4 (exclusive) and rows 4 to 7 (exclusive)*/

	myMessage =  gtk_entry_new(); //create_text ("I am fine, thanks and you?\n");
	gtk_table_attach_defaults (GTK_TABLE (table), myMessage, 0, 3, 6, 7);
	gtk_widget_show (myMessage);

	/* Add create account button. Use columns 0 to 1 (exclusive) and rows 4 to 7 (exclusive)*/
	GtkWidget *account_button = gtk_button_new_with_label ("Create Account");
	gtk_table_attach_defaults(GTK_TABLE (table), account_button, 3, 4, 7, 8);
	gtk_widget_show (account_button);

	g_signal_connect (G_OBJECT (account_button), "clicked", G_CALLBACK (accountButton), (gpointer) window); 

	/* Add create room button. Use columns 0 to 1 (exclusive) and rows 4 to 7 (exclusive)*/
	GtkWidget *room_button = gtk_button_new_with_label ("Create Room");
	gtk_table_attach_defaults(GTK_TABLE (table), room_button, 2, 3, 7, 8);
	gtk_widget_show (room_button);

	g_signal_connect (G_OBJECT (room_button), "clicked", G_CALLBACK (roomButton), (gpointer) window);

	/* Add enter room button. Use columns 0 to 1 (exclusive) and rows 4 to 7 (exclusive)*/
	GtkWidget *enterRoom_button = gtk_button_new_with_label ("Enter Room");
	gtk_table_attach_defaults(GTK_TABLE (table), enterRoom_button, 1, 2, 7, 8);
	gtk_widget_show (enterRoom_button);

	g_signal_connect (G_OBJECT (enterRoom_button), "clicked", G_CALLBACK (enterRoomButton), (gpointer) window);

	GtkWidget *leaveRoom_button = gtk_button_new_with_label ("Leave Room"); 
	gtk_table_attach_defaults(GTK_TABLE (table), leaveRoom_button, 0, 1, 7, 8);
	gtk_widget_show (leaveRoom_button);

	g_signal_connect (G_OBJECT (leaveRoom_button), "clicked", G_CALLBACK (leaveRoomButton), (gpointer) window);

	/* Add send button. Use columns 0 to 1 (exclusive) and rows 4 to 7 (exclusive)*/
	GtkWidget *send_button = gtk_button_new_with_label ("Send");
	gtk_table_attach_defaults(GTK_TABLE (table), send_button, 3, 4, 6, 7);
	//g_signal_connect (G_OBJECT (send_button), "clicked", G_CALLBACK (send_message), (gpointer) window);

	g_signal_connect (send_button, "clicked", G_CALLBACK(send_msg),  myMessage);
	gtk_widget_show (send_button);

	gtk_widget_show (table);
	gtk_widget_show (window);

	g_timeout_add(5000, (GSourceFunc) time_handler, (gpointer) window);

	gtk_main ();

	return 0;
}

