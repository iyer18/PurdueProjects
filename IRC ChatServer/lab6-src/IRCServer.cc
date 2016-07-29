
const char * usage =
"                                                               \n"
"IRCServer:                                                   \n"
"                                                               \n"
"Simple server program used to communicate multiple users       \n"
"                                                               \n"
"To use it in one window type:                                  \n"
"                                                               \n"
"   IRCServer <port>                                          \n"
"                                                               \n"
"Where 1024 < port < 65536.                                     \n"
"                                                               \n"
"In another window type:                                        \n"
"                                                               \n"
"   telnet <host> <port>                                        \n"
"                                                               \n"
"where <host> is the name of the machine where talk-server      \n"
"is running. <port> is the port number you used when you run    \n"
"daytime-server.                                                \n"
"                                                               \n";

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "IRCServer.h"
#include "RoomTable.cc"
#include "LinkedList.cc"
#include "MessageList.cc"

int QueueLength = 5;
LinkedList userpass;
MessageList messages;
RoomTable chatrooms;
int
IRCServer::open_server_socket(int port) {

	// Set the IP address and port for this server
	struct sockaddr_in serverIPAddress; 
	memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
	serverIPAddress.sin_family = AF_INET;
	serverIPAddress.sin_addr.s_addr = INADDR_ANY;
	serverIPAddress.sin_port = htons((u_short) port);

	// Allocate a socket
	int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
	if ( masterSocket < 0) {
		perror("socket");
		exit( -1 );
	}

	// Set socket options to reuse port. Otherwise we will
	// have to wait about 2 minutes before reusing the sae port number
	int optval = 1; 
	int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, 
			(char *) &optval, sizeof( int ) );

	// Bind the socket to the IP address and port
	int error = bind( masterSocket,
			(struct sockaddr *)&serverIPAddress,
			sizeof(serverIPAddress) );
	if ( error ) {
		perror("bind");
		exit( -1 );
	}

	// Put socket in listening mode and set the 
	// size of the queue of unprocessed connections
	error = listen( masterSocket, QueueLength);
	if ( error ) {
		perror("listen");
		exit( -1 );
	}

	return masterSocket;
}

	void
IRCServer::runServer(int port)
{
	int masterSocket = open_server_socket(port);

	initialize();

	while ( 1 ) {

		// Accept incoming connections
		struct sockaddr_in clientIPAddress;
		int alen = sizeof( clientIPAddress );
		int slaveSocket = accept( masterSocket,
				(struct sockaddr *)&clientIPAddress,
				(socklen_t*)&alen);

		if ( slaveSocket < 0 ) {
			perror( "accept" );
			exit( -1 );
		}

		// Process request.
		processRequest( slaveSocket );		
	}
}

	int
main( int argc, char ** argv )
{
	// Print usage if not enough arguments
	if ( argc < 2 ) {
		fprintf( stderr, "%s", usage );
		exit( -1 );
	}

	// Get the port from the arguments
	int port = atoi( argv[1] );

	IRCServer ircServer;

	// It will never return
	ircServer.runServer(port);

}

//
// Commands:
//   Commands are started y the client.
//
//   Request: ADD-USER <USER> <PASSWD>\r\n
//   Answer: OK\r\n or DENIED\r\n
//
//   REQUEST: GET-ALL-USERS <USER> <PASSWD>\r\n
//   Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//
//   REQUEST: CREATE-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LIST-ROOMS <USER> <PASSWD>\r\n
//   Answer: room1\r\n
//           room2\r\n
//           ...
//           \r\n
//
//   Request: ENTER-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LEAVE-ROOM <USER> <PASSWD>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: SEND-MESSAGE <USER> <PASSWD> <MESSAGE> <ROOM>\n
//   Answer: OK\n or DENIED\n
//
//   Request: GET-MESSAGES <USER> <PASSWD> <LAST-MESSAGE-NUM> <ROOM>\r\n
//   Answer: MSGNUM1 USER1 MESSAGE1\r\n
//           MSGNUM2 USER2 MESSAGE2\r\n
//           MSGNUM3 USER2 MESSAGE2\r\n
//           ...\r\n
//           \r\n
//
//    REQUEST: GET-USERS-IN-ROOM <USER> <PASSWD> <ROOM>\r\n
//    Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//

	void
IRCServer::processRequest( int fd )
{
	// Buffer used to store the comand received from the client
	const int MaxCommandLine = 1024;
	char commandLine[ MaxCommandLine + 1 ];
	int commandLineLength = 0;
	int n;

	// Currently character read
	unsigned char prevChar = 0;
	unsigned char newChar = 0;

	//
	// The client should send COMMAND-LINE\n
	// Read the name of the client character by character until a
	// \n is found.
	//

	// Read character by character until a \n is found or the command string is full.
	while ( commandLineLength < MaxCommandLine &&
			read( fd, &newChar, 1) > 0 ) {

		if (newChar == '\n' && prevChar == '\r') {
			break;
		}

		commandLine[ commandLineLength ] = newChar;
		commandLineLength++;

		prevChar = newChar;
	}

	// Add null character at the end of the string
	// Eliminate last \r
	commandLineLength--;
	commandLine[ commandLineLength ] = 0;

	printf("RECEIVED: %s\n", commandLine);

	printf("The commandLine has the following format:\n");
	printf("COMMAND <user> <password> <arguments>. See below.\n");
	printf("You need to separate the commandLine into those components\n");
	printf("For now, command, user, and password are hardwired.\n");

	char * command = (char *) malloc(20*sizeof(char));
	char * user = (char *) malloc(100*sizeof(char));
	char * password = (char*)malloc(100*sizeof(char));
	char * args = (char*)malloc(800*sizeof(char));
	//char * message = (char *) malloc (800*sizeof(char));
	//char * word4 = (char*) malloc(800*sizeof(char));
	//char * message = (char*) malloc (800*sizeof(char));
	sscanf(commandLine, "%s %s %s %[^\t\n]", command, user, password,args);
	//memcpy(message, &commandLine, 1000);
	//strcpy(strstr(commandLine, word4), args);

	printf("command=%s\n", command);
	printf("user=%s\n", user);
	printf( "password=%s\n", password );
	printf("args=%s\n", args);
	//printf("message=%s\n", message);

	if (!strcmp(command, "ADD-USER")) {
		addUser(fd, user, password, args);
	}
	else if (!strcmp(command, "CREATE-ROOM")) {
		createRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "ENTER-ROOM")) {
		enterRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "LEAVE-ROOM")) {
		leaveRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "LIST-ROOMS")) {
		listRooms(fd, user, password, args);
	}
	else if (!strcmp(command, "SEND-MESSAGE")) {
		sendMessage(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-MESSAGES")) {
		getMessages(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-USERS-IN-ROOM")) {
		getUsersInRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-ALL-USERS")) {
		getAllUsers(fd, user, password, args);
	}
	else {
		const char * msg =  "UNKNOWN COMMAND\r\n";
		write(fd, msg, strlen(msg));
	}

	// Send OK answer
	//const char * msg =  "OK\n";
	//write(fd, msg, strlen(msg));

	close(fd);	
}

	void
IRCServer::initialize()
{
	// Open password file
	FILE * file = fopen("password.txt", "r");
	char * password = (char *) malloc (100*sizeof(char));
	char * username = (char *) malloc (100*sizeof(char));
	if( file ==NULL) {
		//create a file
		file = fopen("password.txt", "w");
	}
	// Initialize users in room
	llist_init(&userpass);
	//printf("init\n");
	while( fscanf(file, "%s %s", username, password) == 2) {
		//printf("ADD\n");
		llist_add(&userpass, strdup(username), strdup(password));
		//printf("%s %s\n", username, password);
	}
	fclose(file);
	rtable_init(&chatrooms);
	//printf("after while\n");
	//llist_print(&userpass);
	// Initalize message list

}

bool
IRCServer::checkPassword(int fd, const char * user, const char * password) {
	// Here check the password
	if( llist_exists(&userpass, strdup(user), strdup(password)))
		return true;
	else return false;
}

	void
IRCServer::addUser(int fd, const char * user, const char * password, const char * args)
{
	// Here add a new user. For now always return OK.
	char * username = (char*) malloc(strlen(user)*sizeof(char));
	username = strdup(user);
	char * pass = (char*) malloc(strlen(password)*sizeof(char));
	pass = strdup(password);

	if(! (llist_user_exists(&userpass, strdup(user)))) {
		llist_add(&userpass,username,pass);

		const char * msg =  "OK\r\n";
		write(fd, msg, strlen(msg));
	}
	else if( llist_exists(&userpass, strdup(user), strdup(password))) {
		const char * msg =  "OK\r\n";
		write(fd, msg, strlen(msg));
	}
	else {
		const char * msg =  "ERROR (user with this name already exists)\r\n";
		write(fd, msg, strlen(msg));
	}

	return;		
}

	void
IRCServer::createRoom(int fd, const char *user, const char * password, const char * args)
{
	const char * msg = "OK\r\n";
	char * room  = (char*) malloc(100*sizeof(char));
	sscanf(args, "%s ",room) ;
	//write(fd, msg, strlen(msg));
	char * username = (char*) malloc(strlen(user)*sizeof(char));
	username = strdup(user);
	//write(fd, msg, strlen(msg));
	char * pass = (char*) malloc(strlen(password)*sizeof(char));
	pass = strdup(password);
	//write(fd, msg, strlen(msg));
	const char * add = rtable_add(&chatrooms, room, username, pass);
	write(fd, add, strlen(add));

	return;
}

	void 
IRCServer::listRooms(int fd, const char * user, const char * password, const char * args) 
{
	const char * msg  = rtable_print(&chatrooms);
	write(fd, msg, strlen(msg));

}

	void
IRCServer::enterRoom(int fd, const char * user, const char * password, const char * args)
{
	//const char * msg = "ERROR: Room doesn't exist\r\n";
	char * room  = (char*) malloc(100*sizeof(char));
	sscanf(args, "%s ",room);
	//room = strdup(args);
	char * username = (char*) malloc(strlen(user)*sizeof(char));
	username = strdup(user);
	char * pass = (char*) malloc(strlen(password)*sizeof(char));
	pass = strdup(password);
	if( checkPassword( fd, user, password) == true) {
		const char * add = rtable_add_user(&chatrooms, room, username, pass);
		write(fd, add, strlen(add));
	}
	else {
		const char * msg = "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}

}

	void
IRCServer::leaveRoom(int fd, const char * user, const char * password, const char * args)
{

	const char * msg = "ERROR (Wrong password)\r\n";
	char * room  = (char*) malloc(100*sizeof(char));
	sscanf(args, "%s ",room) ;
	//room = strdup(args);
	char * username = (char*) malloc(strlen(user)*sizeof(char));
	username = strdup(user);
	char * pass = (char*) malloc(strlen(password)*sizeof(char));
	pass = strdup(password);
	if( checkPassword( fd, user, password) == true) {
		const char * add = rtable_remove_user(&chatrooms, room, username, pass);
		write(fd, add, strlen(add));
	}
	else{
		write(fd, msg, strlen(msg));
	}

}

	void
IRCServer::sendMessage(int fd, const char * user, const char * password, const char * args)
{
	if( checkPassword( fd, user, password) == true) {
		char * username = (char*) malloc(strlen(user)*sizeof(char));
		username = strdup(user);
		char * pass = (char*) malloc(strlen(password)*sizeof(char));
		pass = strdup(password);
		char * room = (char*) malloc(100*sizeof(char));
		//room  = strdup(args);
		char * mess = (char*) malloc(strlen(args)*sizeof(char));
		sscanf(args, "%s %[^\t\n]",room, mess) ;
		const char * msg = rtable_send_message(&chatrooms,room,  username, pass, mess);
		write(fd, msg, strlen(msg));
	}

	else{
		const char * msg = "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}

}

	void
IRCServer::getMessages(int fd, const char * user, const char * password, const char * args)
{
	if( checkPassword( fd, user, password) == true) {
		char * username = (char*) malloc(strlen(user)*sizeof(char));
		username = strdup(user);
		char * pass = (char*) malloc(strlen(password)*sizeof(char));
		pass = strdup(password);
		char * num = (char *) malloc(20*sizeof(char));;
		char * room = (char*) malloc(100*sizeof(char));
		sscanf(args, "%s %s",num,room);
		long number = strtol(num, NULL, 10);
		//room  = strdup(args);
		//char * mess = (char*) malloc(strlen(args)*sizeof(char));
		//message = strdup(message);
		//const char * msg = rtable_get_message(&chatrooms,room,  username, pass, number);
		const char * msg = rtable_get_message(&chatrooms,room,  username, pass, number);
		write(fd, msg, strlen(msg));
	}

	else{
		const char * msg = "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}

}

	void
IRCServer::getUsersInRoom(int fd, const char * user, const char * password, const char * args)
{
	char * room  = (char*) malloc(strlen(args)*sizeof(char));
	room = strdup(args);
	char * username = (char*) malloc(strlen(user)*sizeof(char));
	username = strdup(user);
	char * pass = (char*) malloc(strlen(password)*sizeof(char));
	pass = strdup(password);
	if( checkPassword( fd, user, password) == true) {
		const char * msg = rtable_get_user(&chatrooms, room,  username, pass);
		write(fd, msg, strlen(msg));
	}
	else {
		const char * msg = "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}
}

	void
IRCServer::getAllUsers(int fd, const char * user, const char * password,const  char * args)
{
	char * msg  = (char*) malloc(10000*sizeof(char));
	if( checkPassword( fd, user, password) == true) {
		llist_sort(&userpass, 1);	
		const char * msg = llist_print(&userpass);
		write(fd, msg, strlen(msg));
	}
	else {
		const char * msg = "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}

}

