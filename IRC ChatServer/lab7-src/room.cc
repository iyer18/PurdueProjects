
#include <time.h>
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

void update_list_rooms();
void update_users_in_room();
void update_message_in_room();
char * host;
char * user;
char * password;
char * args;
char * room;
char * message;
char * sport;
GtkListStore * list_rooms;
GtkListStore * list_users_in_room;
GtkListStore * list_messages;
GtkTextBuffer *tbuffer;
int port;

static int nSeconds = 0;                 
static gint timer = 0;
static int bTimerRunning = FALSE;

//void UpdateSeconds (int);

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

	//	printf("response:%s\n", response);
	response[len] = '\0';
	close(sock);
}

void printUsage()
{
	printf("Usage: talk-client host port user password\n");
	exit(1);
}

void add_user( GtkWindow *parent) {
	char response[ MAX_RESPONSE ];
	if( user != NULL && password !=NULL) {
		sendCommand(host, port, "ADD-USER", user, password, "", response);
		printf("Response %s\n", response);
		if (!strcmp(response,"OK\r\n")) {
			printf("User %s added\n", user);
		}
	}
	else {
		GtkWidget  *window, *label, *image, *hbox;



		window = gtk_dialog_new_with_buttons ("ERROR", parent,
				GTK_DIALOG_MODAL,
				GTK_STOCK_OK, GTK_RESPONSE_OK,
				NULL);

		gtk_dialog_set_has_separator (GTK_DIALOG (window), FALSE);
		label = gtk_label_new ("Error: incorrect submission");
		image = gtk_image_new_from_stock (GTK_STOCK_DIALOG_ERROR,
				GTK_ICON_SIZE_DIALOG);
		hbox = gtk_hbox_new (FALSE, 5);
		gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
		gtk_box_pack_start_defaults (GTK_BOX (hbox), image);
		gtk_box_pack_start_defaults (GTK_BOX (hbox), label);
		/* Pack the dialog content into the dialog's GtkVBox. */
		gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (window)->vbox), hbox);
		gtk_widget_show_all (window);
		/* Create the dialog as modal and destroy it when a button is clicked. */
		gtk_dialog_run (GTK_DIALOG (window));
		gtk_widget_destroy (window);

	}

}

void create_room( GtkWindow *parent) {
	//GtkWidget *window;

	char response[ MAX_RESPONSE ];
	if( user != NULL && password !=NULL && room != NULL) {
		sendCommand(host, port, "CREATE-ROOM", user, password, room, response);
		printf("Response %s\n", response);
		if (!strcmp(response,"OK\r\n")) {
			printf("User %s created room %s\n", user, room);
			GtkWidget  *window, *label, *image, *hbox;
			window = gtk_dialog_new_with_buttons ("Information", parent,
					GTK_DIALOG_MODAL,
					GTK_STOCK_OK, GTK_RESPONSE_OK,
					NULL);

			gtk_dialog_set_has_separator (GTK_DIALOG (window), FALSE);
			label = gtk_label_new ("You have created a room.");
			image = gtk_image_new_from_stock (GTK_STOCK_DIALOG_INFO,
					GTK_ICON_SIZE_DIALOG);
			hbox = gtk_hbox_new (FALSE, 5);
			gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
			gtk_box_pack_start_defaults (GTK_BOX (hbox), image);
			gtk_box_pack_start_defaults (GTK_BOX (hbox), label);
			/* Pack the dialog content into the dialog's GtkVBox. */
			gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (window)->vbox), hbox);
			gtk_widget_show_all (window);
			/* Create the dialog as modal and destroy it when a button is clicked. */
			gtk_dialog_run (GTK_DIALOG (window));
			gtk_widget_destroy (window);
		}


		else {
			GtkWidget  *window, *label, *image, *hbox;



			window = gtk_dialog_new_with_buttons ("ERROR", parent,
					GTK_DIALOG_MODAL,
					GTK_STOCK_OK, GTK_RESPONSE_OK,
					NULL);

			gtk_dialog_set_has_separator (GTK_DIALOG (window), FALSE);
			label = gtk_label_new ("WARNING: Room already exists");
			image = gtk_image_new_from_stock (GTK_STOCK_DIALOG_INFO,
					GTK_ICON_SIZE_DIALOG);
			hbox = gtk_hbox_new (FALSE, 5);
			gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
			gtk_box_pack_start_defaults (GTK_BOX (hbox), image);
			gtk_box_pack_start_defaults (GTK_BOX (hbox), label);
			/* Pack the dialog content into the dialog's GtkVBox. */
			gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (window)->vbox), hbox);
			gtk_widget_show_all (window);
			/* Create the dialog as modal and destroy it when a button is clicked. */
			gtk_dialog_run (GTK_DIALOG (window));
			gtk_widget_destroy (window);
		}
	}
}


void enter_room(GtkWindow *parent) {
	char response[ MAX_RESPONSE ];
	//char response1 [MAX_RESPONSE];
	if( user != NULL && password !=NULL && room != NULL) {
		sendCommand(host, port, "ENTER-ROOM", user, password, room, response);
		//char * r = strdup (room);
		 //               strcat( r, " has left the room");
		//sendCommand(host, port, "SEND-MESSAGE", user, password, r, response);
		//sendCommand(host, port, "SEND-MESSAGE", user, password, strcat(room, " has entered the room"), response1);
		 //sendCommand(host, port, "SEND-MESSAGE", user, password, strcat(room, " has entered the room"), response1);
		printf("Response %s\n", response);
		update_users_in_room();
		if (!strcmp(response,"OK\r\n")) {
			printf("User %s added to room %s\n", user, room);
			GtkWidget  *window, *label, *image, *hbox;
			window = gtk_dialog_new_with_buttons ("Information", parent,
					GTK_DIALOG_MODAL,
					GTK_STOCK_OK, GTK_RESPONSE_OK,
					NULL);

			gtk_dialog_set_has_separator (GTK_DIALOG (window), FALSE);
			label = gtk_label_new ("You have entered a room.");
			image = gtk_image_new_from_stock (GTK_STOCK_DIALOG_INFO,
					GTK_ICON_SIZE_DIALOG);
			hbox = gtk_hbox_new (FALSE, 5);
			gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
			gtk_box_pack_start_defaults (GTK_BOX (hbox), image);
			gtk_box_pack_start_defaults (GTK_BOX (hbox), label);
			/* Pack the dialog content into the dialog's GtkVBox. */
			gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (window)->vbox), hbox);
			gtk_widget_show_all (window);
			/* Create the dialog as modal and destroy it when a button is clicked. */
			gtk_dialog_run (GTK_DIALOG (window));
			gtk_widget_destroy (window);
		}
		else {
			GtkWidget  *window, *label, *image, *hbox;



			window = gtk_dialog_new_with_buttons ("ERROR", parent,
					GTK_DIALOG_MODAL,
					GTK_STOCK_OK, GTK_RESPONSE_OK,
					NULL);

			gtk_dialog_set_has_separator (GTK_DIALOG (window), FALSE);
			label = gtk_label_new ("WARNING: Already in room");
			image = gtk_image_new_from_stock (GTK_STOCK_DIALOG_INFO,
					GTK_ICON_SIZE_DIALOG);
			hbox = gtk_hbox_new (FALSE, 5);
			gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
			gtk_box_pack_start_defaults (GTK_BOX (hbox), image);
			gtk_box_pack_start_defaults (GTK_BOX (hbox), label);
			/* Pack the dialog content into the dialog's GtkVBox. */
			gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (window)->vbox), hbox);
			gtk_widget_show_all (window);
			/* Create the dialog as modal and destroy it when a button is clicked. */
			gtk_dialog_run (GTK_DIALOG (window));
			gtk_widget_destroy (window);
		}
	}else {
		GtkWidget  *window, *label, *image, *hbox;



		window = gtk_dialog_new_with_buttons ("ERROR", parent,
				GTK_DIALOG_MODAL,
				GTK_STOCK_OK, GTK_RESPONSE_OK,
				NULL);

		gtk_dialog_set_has_separator (GTK_DIALOG (window), FALSE);
		label = gtk_label_new ("ERROR: Sign in");
		image = gtk_image_new_from_stock (GTK_STOCK_DIALOG_ERROR,
				GTK_ICON_SIZE_DIALOG);
		hbox = gtk_hbox_new (FALSE, 5);
		gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
		gtk_box_pack_start_defaults (GTK_BOX (hbox), image);
		gtk_box_pack_start_defaults (GTK_BOX (hbox), label);
		/* Pack the dialog content into the dialog's GtkVBox. */
		gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (window)->vbox), hbox);
		gtk_widget_show_all (window);
		/* Create the dialog as modal and destroy it when a button is clicked. */
		gtk_dialog_run (GTK_DIALOG (window));
		gtk_widget_destroy (window);
	}
}

void leave_room() {
	char response[ MAX_RESPONSE ];
	if( user != NULL && password !=NULL && room != NULL) {
		sendCommand(host, port, "LEAVE-ROOM", user, password, room, response);
		//char * r = strdup (room);
		//strcat( r, " has left the room");
		//sendCommand(host, port, "SEND-MESSAGE", user, password, r, response);
		                //update_users_in_room();	
		printf("Response %s\n", response);
	
	}

}
void get_users_in_room () {
	char response[ MAX_RESPONSE ];
	if( user != NULL && password !=NULL && room != NULL) {
		sendCommand(host, port, "GET-USERS-IN-ROOM", user, password, room, response);
		printf("Response %s\n", response);
	}
}

void get_messages() {
	char response[ MAX_RESPONSE ];
	if( user != NULL && password !=NULL && room != NULL) {
		const gchar * r = strdup(args);
		strcat(args, "0 ");
		strcat(args, room);
		sendCommand(host, port, "GET-MESSAGES", user, password, args, response);
		args = strdup(r);
	}
}

	static gboolean
time_handler(GtkWidget *widget)
{
	if (widget->window == NULL) return FALSE;

	//gtk_widget_queue_draw(widget);
	//
	fprintf(stderr, "Hi\n");
	if( user != NULL && password !=NULL) {
		update_list_rooms();
		if( room != NULL) {
			//	printf("room != NULL\n");
			update_message_in_room();
			//	printf("after update message\n");
		}

	}
	return TRUE;

}

void send_message() {
	char response[ MAX_RESPONSE ];
	//args = (gchar *) malloc (1000*sizeof(gchar));
	//strcat(args, strdup(room));
	//strcat(args, " ");
	//strcat(args, strdup(message));
	if( user != NULL && password !=NULL && room != NULL) { 
		const gchar * r = strdup(room);
		strcat(room, " ");
		strcat(room, message);
		sendCommand(host, port, "SEND-MESSAGE", user, password, room, response);
		room = strdup(r);
		printf("Response %s\n", response);
		if (!strcmp(response,"OK\r\n")) {
			printf("User %s sent message %s to room %s\n", user, message,  room);
		}
	}

}


void print_messages() {
	GtkTextIter ei;
	const gchar * text = message;

	gtk_text_buffer_set_text(tbuffer, "", 0);
	gtk_text_buffer_get_end_iter(tbuffer, &ei);
	gtk_text_buffer_insert(tbuffer, &ei, text, -1);

}

void print_users_in_room() {
}

void print_users() {
}

void printPrompt() {
	printf("talk> ");
	fflush(stdout);
}

void printHelp() {
	printf("Commands:\n");
	printf(" -who   - Gets users in room\n");
	printf(" -users - Prints all registered users\n");
	printf(" -help  - Prints this help\n");
	printf(" -quit  - Leaves the room\n");
	printf("Anything that does not start with \"-\" will be a message to the chat room\n");
}

void * getMessagesThread(void * arg) {

	while (1) {
		usleep(2*1000*1000);
	}
}

void startGetMessageThread()
{
	pthread_create(NULL, NULL, getMessagesThread, NULL);
}

static void insert_text( GtkTextBuffer *buffer, const char * initialText )
{
	GtkTextIter iter;

	gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);
	gtk_text_buffer_insert (buffer, &iter, initialText,-1);
}

void update_message_in_room() {
	GtkTreeIter iter;
	int i = 0;
	char response[ MAX_RESPONSE ];
	response[0] = '\0';
	//sendCommand(host, port, "GET-MESSAGES", user, password, args, response);
	//	send_message();
	//printf("%s\n", response);

	if( user != NULL && password !=NULL && room != NULL) {
		const gchar * r = "0 ";
		args = strdup(r);
		//strcat(args, r);
		strcat(args, room);
		//printf("args, %s\n", args);
		sendCommand(host, port, "GET-MESSAGES", user, password, args, response);
		//printf("after get messages msg\n");
		//args = strdup(r);
	}

	gtk_list_store_clear(GTK_LIST_STORE ( list_messages));

	char * r =  strtok(response, "\r\n");
	gchar * msg;

	while (*r != '\0') {
		//	printf("Response:%s\n", r);

		msg = g_strdup_printf (r);
		gtk_list_store_append (GTK_LIST_STORE (list_messages), &iter);
		gtk_list_store_set (GTK_LIST_STORE (list_messages),
				&iter,
				0, msg,
				-1);


		r = strtok(NULL, "\r\n");
		if ( r == NULL) {
			break;

		}
	}
}

void update_users_in_room() {
	GtkTreeIter iter;
	int i = 0;

	//user = "yo";
	//password = "yo";
	char response[ MAX_RESPONSE ];
	response[0] = '\0';
	//	printf("1\n");
	//sendCommand(host, port, "LIST-ROOMS", user, password, " ", response);
	//get_users_in_room ();

	if( user != NULL && password !=NULL && room != NULL) {
		sendCommand(host, port, "GET-USERS-IN-ROOM", user, password, room, response);

	}

	//printf("%s\n", response);
	gtk_list_store_clear(GTK_LIST_STORE (list_users_in_room));



	char * r =  strtok(response, "\r\n");
	gchar * msg;

	while (*r != '\0') {
		//		printf("Response:%s\n", r);

		msg = g_strdup_printf (r);
		gtk_list_store_append (GTK_LIST_STORE (list_users_in_room), &iter);
		gtk_list_store_set (GTK_LIST_STORE (list_users_in_room),
				&iter,
				0, msg,
				-1);


		r = strtok(NULL, "\r\n");
		if ( r == NULL) {
			break;
		}
	}

}

void update_list_rooms() {
	GtkTreeIter iter;
	int i =0;
	//user = "yo";
	//password = "yo";
	char response[ MAX_RESPONSE ];
	response[0] = '\0';
	//printf("1\n");
	//sendCommand(host, port, "LIST-ROOMS", user, password, " ", response);
	//list_Rooms();

	if( user != NULL && password !=NULL) {
		sendCommand(host, port, "LIST-ROOMS", user, password, "", response);

	}

	//	printf("%s\n", response);
	gtk_list_store_clear(GTK_LIST_STORE (list_rooms));



	char * r =  strtok(response, "\r\n");
	gchar * msg;

	while (*r != '\0') {
		//		printf("Response:%s\n", r);

		msg = g_strdup_printf (r);
		gtk_list_store_append (GTK_LIST_STORE (list_rooms), &iter);
		gtk_list_store_set (GTK_LIST_STORE (list_rooms),
				&iter,
				0, msg,
				-1);


		r = strtok(NULL, "\r\n");
		if ( r == NULL) {
			break;
		}
	}
}


/*gint TimerCallback (gpointer data)
  {
  nSeconds++;
  user = "yo";
  password = "yo";
  update_message_in_room();
  update_users_in_room();
  update_list_rooms();
  return 1;
  }
  void StartTimer ()
  {
  if( !bTimerRunning) {
  nSeconds = 0;
  timer = gtk_timeout_add(6000, TimerCallback, NULL);
  bTimerRunning = TRUE;
  }
  }

  void StopTimer()
  {
  if(bTimerRunning) {

  gtk_timeout_remove(timer);
  bTimerRunning = FALSE;
  }
  }*/


static GtkWidget *create_list( const char * titleColumn, GtkListStore *model )
{
	GtkWidget *scrolled_window;
	GtkWidget *tree_view;
	//GtkListStore *model;
	GtkCellRenderer *cell;
	GtkTreeViewColumn *column;

	GdkColor color;
	color.red = 0xa6a6a6;
	color.green = 0xf9f9f9;
	color.blue = 0x959595;

	int i;

	/* Create a new scrolled window, with scrollbars only if needed */
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	//	model = gtk_list_store_new (1, G_TYPE_STRING);
	tree_view = gtk_tree_view_new ();
	gtk_container_add (GTK_CONTAINER (scrolled_window), tree_view);
	gtk_tree_view_set_model (GTK_TREE_VIEW (tree_view), GTK_TREE_MODEL (model));
	//gtk_widget_modify_bg(tree_view, GTK_STATE_NORMAL, &color);
	gtk_widget_show (tree_view);

	cell = gtk_cell_renderer_text_new ();

	column = gtk_tree_view_column_new_with_attributes (titleColumn,cell,"text",  0,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view),GTK_TREE_VIEW_COLUMN (column));
	return scrolled_window;
}

int leaveRoom (GtkButton * button, GtkWindow * parent)
{
	GtkWidget *dialog, *window, *table, *username, *roomname, *frame;
	GtkWidget *lbl1, *lbl3, *label, *image, *hbox;
	gint result;

	dialog = gtk_dialog_new_with_buttons ("Enter User Information", parent,
			GTK_DIALOG_MODAL,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_OK);
	lbl3 = gtk_label_new ("Room Name:");
	roomname = gtk_entry_new ();
	gtk_entry_set_text (GTK_ENTRY (roomname), "Room");

	table = gtk_table_new (4, 2, FALSE);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl3, 0, 1, 2, 3);
	gtk_table_attach_defaults (GTK_TABLE (table), roomname, 1, 2, 2, 3);
	gtk_table_set_row_spacings (GTK_TABLE (table), 5);
	gtk_table_set_col_spacings (GTK_TABLE (table), 5);
	gtk_container_set_border_width (GTK_CONTAINER (table), 5);
	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), table);
	gtk_widget_show_all (dialog);
	result = gtk_dialog_run (GTK_DIALOG (dialog));
	if (result == GTK_RESPONSE_OK)
	{
		const gchar * r = gtk_entry_get_text (GTK_ENTRY (roomname));
		g_print ("Room Name:\t %s\n", gtk_entry_get_text (GTK_ENTRY (roomname)));
		room = strdup(r);
		leave_room ();
	}
	else if( result != GTK_RESPONSE_OK) {
		gtk_widget_destroy (window);
	}
	window = gtk_dialog_new_with_buttons ("Information", parent,
			GTK_DIALOG_MODAL,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			NULL);

	gtk_dialog_set_has_separator (GTK_DIALOG (window), FALSE);
	label = gtk_label_new ("You have left the room.");
	image = gtk_image_new_from_stock (GTK_STOCK_DIALOG_INFO,
			GTK_ICON_SIZE_DIALOG);
	hbox = gtk_hbox_new (FALSE, 5);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
	gtk_box_pack_start_defaults (GTK_BOX (hbox), image);
	gtk_box_pack_start_defaults (GTK_BOX (hbox), label);
	/* Pack the dialog content into the dialog's GtkVBox. */
	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (window)->vbox), hbox);
	gtk_widget_show_all (window);
	/* Create the dialog as modal and destroy it when a button is clicked. */
	gtk_dialog_run (GTK_DIALOG (window));
	gtk_widget_destroy (window);


	gtk_widget_destroy (dialog);
	return 0;

}



/* this callback is for popup box */
int enterRoom (GtkButton * button, GtkWindow * parent)
{
	GtkWidget *dialog, *window, *table, *username, *roomname, *frame;
	GtkWidget *lbl1, *lbl3, *label, *image, *hbox;
	gint result;

	dialog = gtk_dialog_new_with_buttons ("Enter User Information", parent,
			GTK_DIALOG_MODAL,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_OK);
	lbl3 = gtk_label_new ("Room Name:");
	roomname = gtk_entry_new ();
	gtk_entry_set_text (GTK_ENTRY (roomname), "Room");

	table = gtk_table_new (4, 2, FALSE);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl3, 0, 1, 2, 3);
	gtk_table_attach_defaults (GTK_TABLE (table), roomname, 1, 2, 2, 3);
	gtk_table_set_row_spacings (GTK_TABLE (table), 5);
	gtk_table_set_col_spacings (GTK_TABLE (table), 5);
	gtk_container_set_border_width (GTK_CONTAINER (table), 5);
	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), table);
	gtk_widget_show_all (dialog);
	result = gtk_dialog_run (GTK_DIALOG (dialog));
	if (result == GTK_RESPONSE_OK)
	{
		const gchar * r = gtk_entry_get_text (GTK_ENTRY (roomname));
		g_print ("Room Name:\t %s\n", gtk_entry_get_text (GTK_ENTRY (roomname)));
		room = strdup(r);
		enter_room (parent);
	}

	//update_users_in_room();

	gtk_widget_destroy (dialog);
	return 0;



}


/* this callback is for popup box */
int createRoom (GtkButton * button, GtkWindow *parent)
{
	GtkWidget *dialog, *window, *table, *username, *roomname;
	GtkWidget *lbl1, *lbl3, *label, *image, *hbox;
	gint result;

	dialog = gtk_dialog_new_with_buttons ("Enter User Information", parent,
			GTK_DIALOG_MODAL,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_OK);

	lbl3 = gtk_label_new ("Room Name:");
	roomname = gtk_entry_new ();
	gtk_entry_set_text (GTK_ENTRY (roomname), "Room");
	table = gtk_table_new (4, 2, FALSE);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl3, 0, 1, 2, 3);
	gtk_table_attach_defaults (GTK_TABLE (table), roomname, 1, 2, 2, 3);
	gtk_table_set_row_spacings (GTK_TABLE (table), 5);
	gtk_table_set_col_spacings (GTK_TABLE (table), 5);
	gtk_container_set_border_width (GTK_CONTAINER (table), 5);
	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), table);
	gtk_widget_show_all (dialog);
	result = gtk_dialog_run (GTK_DIALOG (dialog));
	if (result == GTK_RESPONSE_OK)
	{
		const gchar * r = gtk_entry_get_text (GTK_ENTRY (roomname));
		g_print ("Room Name:\t %s\n", gtk_entry_get_text (GTK_ENTRY (roomname)));
		room = strdup(r);
		create_room (parent);

	}
	gtk_widget_destroy (dialog);
	return 0;

}

/* this callback is for popup box */ 
int createUser (GtkButton * button, GtkWindow *parent)
{
	GtkWidget *dialog, *window, *table, *username, *pass, *roomname;
	GtkWidget *lbl1, *lbl2, *lbl3, *label, *image, *hbox;
	gint result;

	dialog = gtk_dialog_new_with_buttons ("Enter User Information", parent,
			GTK_DIALOG_MODAL,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
	gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_OK);

	lbl1 = gtk_label_new ("User Name:");
	lbl2 = gtk_label_new ("Password:");
	username = gtk_entry_new ();
	pass = gtk_entry_new ();
	gtk_entry_set_visibility (GTK_ENTRY (pass), FALSE);
	gtk_entry_set_invisible_char (GTK_ENTRY (pass), '*');

	/* Retrieve the user's information for the default values. */
	gtk_entry_set_text (GTK_ENTRY (username), g_get_user_name());
	gtk_entry_set_text (GTK_ENTRY (pass), "Password");
	table = gtk_table_new (3, 2, FALSE);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl1, 0, 1, 0, 1);
	gtk_table_attach_defaults (GTK_TABLE (table), lbl2, 0, 1, 2, 3);

	gtk_table_attach_defaults (GTK_TABLE (table), username, 1, 2, 0, 1);
	gtk_table_attach_defaults (GTK_TABLE (table), pass, 1, 2, 2, 3);
	gtk_table_set_row_spacings (GTK_TABLE (table), 5);
	gtk_table_set_col_spacings (GTK_TABLE (table), 5);
	gtk_container_set_border_width (GTK_CONTAINER (table), 5);
	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), table);
	gtk_widget_show_all (dialog);

	result = gtk_dialog_run (GTK_DIALOG (dialog));

	if (result == GTK_RESPONSE_OK)
	{
		const gchar * u = gtk_entry_get_text (GTK_ENTRY (username));
		const gchar * p = gtk_entry_get_text (GTK_ENTRY (pass));
		g_print ("User Name:\t %s\n", gtk_entry_get_text (GTK_ENTRY (username)));
		g_print ("Password:\t %s\n", gtk_entry_get_text (GTK_ENTRY (pass)));
		user = strdup(u);
		password = strdup(p);
		add_user (parent);
	}

	window = gtk_dialog_new_with_buttons ("Information", parent,
			GTK_DIALOG_MODAL,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			NULL);

	gtk_dialog_set_has_separator (GTK_DIALOG (window), FALSE);
	label = gtk_label_new ("You have created a new user.");
	image = gtk_image_new_from_stock (GTK_STOCK_DIALOG_INFO,
			GTK_ICON_SIZE_DIALOG);
	hbox = gtk_hbox_new (FALSE, 5);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
	gtk_box_pack_start_defaults (GTK_BOX (hbox), image);
	gtk_box_pack_start_defaults (GTK_BOX (hbox), label);
	/* Pack the dialog content into the dialog's GtkVBox. */
	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (window)->vbox), hbox);
	gtk_widget_show_all (window);
	/* Create the dialog as modal and destroy it when a button is clicked. */
	gtk_dialog_run (GTK_DIALOG (window));
	gtk_widget_destroy (window);

	gtk_widget_destroy (dialog);
	return 0;

}


/* this callback lets the server know when a button is clicked*/
void callback( GtkWidget *widget, gpointer   data )
{
	g_print ("%s\n", (char *) data);
}

/* this callback reads the message in the entry box*/
void send_(GtkWidget *widget, GtkWidget *type)
{
	const gchar *m;
	m = gtk_entry_get_text (GTK_ENTRY (type));
	message = strdup(m);
	send_message();
	printf ("Message:\t %s\n", m);
}
/* This callback changes the color of buttons when you hover over them*/
void hover_button(GtkWidget *widget, gpointer data)
{
	GdkColor color;
	color.red = 27000;
	color.green = 30325;
	color.blue = 34181;
	gtk_widget_modify_bg(widget, GTK_STATE_PRELIGHT, &color);
}
/* This callback quits the program */
gint delete_event( GtkWidget *widget,GdkEvent  *event, gpointer data )
{
	gtk_main_quit ();
	return(FALSE);
}


int main( int   argc, char *argv[] )
{
	GtkWidget *window;
	GtkWidget *createroom;
	GtkWidget *createaccount;
	GtkWidget *table;
	GtkWidget *notebook;
	GtkWidget *frame;
	GtkWidget *type;
	GtkWidget *label;
	GtkWidget *leaveroom;
	GtkWidget *send;
	GtkWidget *title;
	GtkWidget *image;
	GtkWidget *avirooms;
	GtkWidget *usersinroom;
	GtkWidget *enterroom;
	GtkWidget *tree_view;
	GtkWidget *list;
	//	GtkListStore *model;

	gint tmp_pos;

	char line[MAX_MESSAGE_LEN+1];

	if (argc < 3) {
		printUsage();
	}

	host = argv[1];
	sport = argv[2];
	//user = argv[3];
	//password = argv[4];

	printf("\nStarting talk-client %s %s \n", host, sport);//, user, password);
	sscanf(sport, "%d", &port);

	//	add_user();



	char lName[32];
	char fName[32];

	GdkColor color;
	color.red = 0xa6a6a6;
	color.green = 0xf9f9f9;
	color.blue = 0x959595;

	gtk_init (&argc, &argv);
	//user = "yo";
	//password = "yo";
	//room = "yo";
	//add_user();
	//create_room();

	/* Setting up the GUI */

	/* Create a window and set it up */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 1400, 1200);
	//	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
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
	sprintf(lName, "ROOM");// should be inputted by user
	frame = gtk_scrolled_window_new(NULL,NULL);
	//gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (frame),
	//		GTK_POLICY_AUTOMATIC,
	//		GTK_POLICY_AUTOMATIC);
	list_messages = gtk_list_store_new (1, G_TYPE_STRING);
	//update_message_in_room();
	frame = create_list("Messages in this room", list_messages);
	//gtk_table_attach_defaults (GTK_T (table), frame, 0, 8, 0, 8);
	//gtk_text_view_set_editable(GTK_TEXT_VIEW(frame), FALSE);
	//gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(frame), TRUE);
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
	//const gchar * m = gtk_entry_get_text (GTK_ENTRY (type));
	//message = strdup(m);

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
	g_signal_connect (G_OBJECT (enterroom), "clicked", G_CALLBACK (enterRoom), (gpointer)window );
	//const gchar * r = strdup(room);
	//gtk_notebook_insert_page (GTK_NOTEBOOK (notebook), enterroom, 2);
	gtk_table_attach (GTK_TABLE(table), enterroom, 6, 8, 9, 10, GTK_FILL, GTK_FILL, 1, 25);
	gtk_widget_show (enterroom);

	/* add a leaveroom  button */
	leaveroom = gtk_button_new_with_label("LEAVE ROOM");
	gtk_signal_connect (GTK_OBJECT (leaveroom), "clicked", GTK_SIGNAL_FUNC (callback), (gpointer) "Button clicked:\t Leave Room");
	g_signal_connect (G_OBJECT (leaveroom), "clicked", G_CALLBACK (leaveRoom), (gpointer) window);

	gtk_table_attach (GTK_TABLE(table), leaveroom, 4, 6, 9, 10, GTK_FILL, GTK_FILL, 1, 25);
	gtk_widget_show (leaveroom);

	/* create a listview of users in room*/

	//title = gtk_label_new("Users in Rooms");
	//gtk_table_attach(GTK_TABLE(table), title, 8, 10, 0, 1,
	//		GTK_FILL, GTK_FILL, 0, 0);
	//gtk_widget_show(title);
	//
	list_users_in_room = gtk_list_store_new (1, G_TYPE_STRING);
	//update_users_in_room();
	usersinroom = create_list("Users in this room", list_users_in_room);
	gtk_table_attach_defaults (GTK_TABLE (table), usersinroom, 8, 10, 0, 5);
	//gtk_widget_modify_bg(usersinroom, GTK_STATE_NORMAL, &color);
	//	gtk_text_view_new();
	//gtk_text_view_set_editable(GTK_TEXT_VIEW(usersinroom), FALSE);
	//gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(usersinroom), TRUE);
	//gtk_table_attach(GTK_TABLE(table), usersinroom, 8, 10, 1, 5,  GTK_FILL, GTK_FILL, 1, 1);
	gtk_widget_show(usersinroom);

	/* create a listview of available rooms*/

	//title = gtk_label_new("Available Rooms");
	//gtk_table_attach(GTK_TABLE(table), title, 8, 10, 5, 6,
	//		GTK_FILL, GTK_FILL, 0, 0);
	//gtk_widget_show(title);

	//avirooms = gtk_text_view_new();
	//gtk_text_view_set_editable(GTK_TEXT_VIEW(avirooms), FALSE);
	//gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(avirooms), TRUE);
	//gtk_table_attach(GTK_TABLE(table), avirooms, 8, 10, 6, 10,  GTK_FILL, GTK_FILL, 1, 1);
	//gtk_widget_show(avirooms);

	list_rooms = gtk_list_store_new (1, G_TYPE_STRING);
	//update_list_rooms();
	list = create_list("Available Rooms", list_rooms);
	gtk_table_attach_defaults (GTK_TABLE (table), list, 8, 10, 5, 10);
	//        gtk_container_add (GTK_CONTAINER (avirooms), list);
	gtk_widget_show (list);


	/*show the table and window*/
	gtk_widget_show (table);
	gtk_widget_show (window);

	/*make the window closeable*/
	gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (delete_event), NULL);

	g_timeout_add(5000, (GSourceFunc) time_handler, (gpointer) window);
	gtk_widget_show_all(window);
	//time_handler(window);
	gtk_main ();


	/* setting up the talk -client */
}
