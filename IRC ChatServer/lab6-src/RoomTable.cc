
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "MessageList.h"
#include "RoomTable.h"

// Initializes a word table
void rtable_init(RoomTable * rtable)
{
	// Allocate and initialize space for the table
	rtable->nRooms = 0;
	rtable->maxRooms = 10;
	rtable->roomArray = (RoomInfo *) malloc(1000 * sizeof(RoomInfo));
	for (int i = 0; i < rtable->maxRooms; i++) {
		llist_init(&rtable->roomArray[i].userpass);
		mlist_init(&rtable->roomArray[i].messages);	
	}
}



bool rtable_exists(RoomTable *rtable, char * room) {
	for (int i = 0; i < rtable->nRooms; i++) {
		if ( strcmp(rtable->roomArray[i].room, room)== 0 ) {
			//        const char * error = "ROOM Already exists\r\n";
			return true;
		}
	}
	return false;

}
// Add room to the tableand position. Position is added to the corresponding linked list.
char * rtable_add_user( RoomTable *rtable, char * room, char *user, char  *pword) {

	char * msg = (char *)  malloc (1000000* sizeof(char));
	const char * line = "\r\n";
	const char * yes = "OK\r\n";
	const char * no = "ERROR (No room)\r\n";
	for (int i = 0; i < rtable->nRooms; i++) {
		if(strcmp(rtable->roomArray[i].room, room) == 0 ) {
			if( !(llist_exists(&rtable->roomArray[i].userpass, user,  pword))) {
				llist_add(&rtable->roomArray[i].userpass, user, pword);
				//mlist_add(&rtable->roomArray[i].messages, user,);
				strcat(msg, yes);
				return msg;
			}
			else {
				strcat(msg, yes);
				return msg;
			}
		}
	}
	strcat(msg, no);
	return msg;


}

char * rtable_send_message( RoomTable *rtable,char * room,  char *user, char  *pword, char * message) {

	char * msg = (char *)  malloc (1000000* sizeof(char));
	const char * line = "\r\n";
	const char * yes = "OK\r\n";
	const char * no = "ERROR (user not in room)\r\n";
	for (int i = 0; i < rtable->nRooms; i++) {
		if(strcmp(rtable->roomArray[i].room, room) == 0 ) {
			if( (llist_exists(&rtable->roomArray[i].userpass, user,  pword))) {
				mlist_insert_last(&rtable->roomArray[i].messages, message, user);
				strcat(msg, yes);
				return msg;
			}
		}

	}
	strcat(msg, no);
	return msg;


}

char * rtable_remove_user( RoomTable *rtable, char * room, char *user, char  *pword) {

	char * msg = (char *)  malloc (1000000* sizeof(char));
	const char * line = "\r\n";
	const char * yes = "OK\r\n";
	const char * no = "ERROR (No user in room)\r\n";
	for (int i = 0; i < rtable->nRooms; i++) {
		if(strcmp(rtable->roomArray[i].room, room) == 0 ) {
			if( (llist_exists(&rtable->roomArray[i].userpass, user,  pword))) {
				llist_remove(&rtable->roomArray[i].userpass, user, pword);
				strcat(msg, yes);
				return msg;
			}
		}
	}
	strcat(msg, no);
	return msg;


}

char * rtable_get_user(RoomTable * rtable, char * room, char * user, char * pword) {

	char * msg = (char *)  malloc (1000000* sizeof(char));
	const char * line = "\r\n";
	const char * yes = "OK\r\n";
	const char * no = "ERROR (user not in room)\r\n";
	for (int i = 0; i < rtable->nRooms; i++) {
		if(strcmp(rtable->roomArray[i].room, room) == 0 ) {
			/*		if(llist_number_elements(&rtable->roomArray[i].userpass) == 0) {
					strcat(msg, line);
					return msg;
					}*/
			//if(! (llist_exists(&rtable->roomArray[i].userpass, user, pword))){
			//if(!(mlist_exists(&rtable->roomArray[i].messages, user))){
			if(llist_number_elements(&rtable->roomArray[i].userpass) < 2){
					strcat(msg, line);
				return msg;
			}
			else {
				llist_sort(&rtable->roomArray[i].userpass, 1);
				//	llist_remove_first(&rtable->roomArray[i].userpass,room, *us, *ps);
				const char * u = " ";
				const char * p = " ";
				char * us = (char *)  malloc (10* sizeof(char));
				char * ps = (char *)  malloc (10* sizeof(char));
				strcat(us, u);
				strcat(ps, p);
				rtable_remove_user(rtable, room, us, ps);
				strcat( msg, llist_print(&rtable->roomArray[i].userpass));
				return msg;
			}
		}

		}
		strcat(msg, no);
		return msg;
		}

		char * rtable_get_message(RoomTable * rtable, char * room, char * user, char * pword, long num) {

			char * msg = (char *)  malloc (1000000* sizeof(char));
			const char * line = "\r\n";
			const char * yes = "OK\r\n";
			const char * no = "ERROR (User not in room)\r\n";
			for (int i = 0; i < rtable->nRooms; i++) {
				if(strcmp(rtable->roomArray[i].room, room) == 0 ) {
					//	mlist_sort(&rtable->roomArray[i].messages, 1);
					if( (llist_exists(&rtable->roomArray[i].userpass, user,  pword))) {
						strcat( msg, mlist_print_last(&rtable->roomArray[i].messages, num));
						//strcat( msg, mlist_print(&rtable->roomArray[i].messages));
						return msg;
					}
				}
			}
			strcat(msg, no);
			return msg;
		}

		char *  rtable_add(RoomTable * rtable, char * room, char * user, char * pword)
		{

			//	printf("1\n");	
			char * msg = (char *)  malloc (1000000* sizeof(char));
			const char * line = "\r\n";
			for (int i = 0; i < rtable->nRooms; i++) {
				if ( strcmp(rtable->roomArray[i].room, room)== 0 ) {
					// Found word. Add position in the list of positions
					//llist_insert_last(&rtable->roomArray[i].userpass, user, pword );
					//	printf("ROOM exists\r\n");
					const char * error = "ROOM Already exists\r\n";
					strcat(msg, error);
					//		strcat(msg, line);
					return msg;
				} 
			}

			// Word not found.
			//printf("1\n");	

			// Make sure that the array has space.
			// Expand the wordArray here.
			if( rtable->nRooms+2 == rtable->maxRooms) {
				// if the number of words is more than the maxwords alowed, then max words has to be increased
				rtable->maxRooms = rtable->maxRooms + 20000;
				// accordingling each has to have memory allocated
				rtable->roomArray = (RoomInfo *) realloc(rtable->roomArray,rtable->maxRooms *sizeof(RoomInfo));
			}
			// Add new word and position
			rtable->roomArray[rtable->nRooms].room = strdup(room);
			//rtable->roomArray[rtable->nRooms].userpass = NULL;
			//llist_insert_last(&rtable->roomArray[rtable->nRooms].userpass, user, pword);
			const char * u = " ";
			const char * p = " ";
			char * us = (char *)  malloc (10* sizeof(char));
			char * ps = (char *)  malloc (10* sizeof(char));
			strcat(us, u);
			strcat(ps, p);
			llist_add(&rtable->roomArray[rtable->nRooms].userpass, us, ps);
			//printf("add\r\n");
			rtable->nRooms++;
			const char * yes = "OK\r\n";
			strcat(msg, yes);
			//	strcat(msg, line);
			return msg;

		}



		// Print contents of the table.
		char * rtable_print(RoomTable * rtable)
		{
			//fprintf(fd, "------- ROOM TABLE -------\n");

			// Print words
			char * msg = (char *)  malloc (1000000* sizeof(char));
			const char * line = "\r\n";
			if (rtable->nRooms == 0) {
				const char * null = "NO-ROOMS\r\n";
				strcat(msg, null);
				return msg;
			}
			rtable_sort(rtable);
			for (int i = 0; i < rtable->nRooms; i++) {
				//fprintf(fd, "%d: %s: ", i, rtable->roomArray[i].room);
				strcat(msg, strdup(rtable->roomArray[i].room));
				strcat(msg, line);
				//llist_print( &rtable->roomArray[i].userpass);
			}
			strcat(msg, line);
			return msg;
		}

		// Get positions where the word occurs
		LinkedList * rtable_getPositions(RoomTable * rtable, char * room)
		{
			// Write your code here
			for(int i = 0; i < rtable->nRooms;i++) {
				if( strcmp(rtable->roomArray[i].room, room) == 0){
					LinkedList * k = &rtable->roomArray[i].userpass;
					return k;
				}
			}
			return NULL;
		}

		//
		// Separates the string into words
		//

#define MAXROOM 200
		char room[MAXROOM];
		int roomLength;
		int roomCount;
		int charCount;
		int roomPos = 0;
		int pos = 0;

#define isaletter(ch) ((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z'))



		// It returns the next word from stdin.
		// If there are no more more words it returns NULL.
		// A word is a sequence of alphabetical characters.
		static char * nextword(FILE * fd) {
			// Write your code here, very similar to nextword from lab3
			int c;
			while ((c= fgetc(fd))  != -1)
			{
				roomPos++;
				//	if ((c != '\n') &&(c != '\t') && (c != ' ') && (c != EOF))
				// instead of this must use isaletter
				if(isaletter(c))
				{
					room[roomLength] = c;
					roomLength++;
				}
				//if (((c == '\n') ||(c == '\t') || (c == ' ') || (c == -1)) && (wordLength != 0 ))
				if( !(isaletter(c)) && roomLength != 0)
				{
					room[roomLength] = 0;
					pos = roomPos - roomLength - 1;
					roomLength = 0; 
					return room;
				} 
			}

			if ( roomLength >= 1)
			{
				room[roomLength] = '\0';
				roomLength = 0;
				return room;
			}
			return 0;
		}

		// Conver string to lower case
		void toLower(char *s) {
			// Write your code here
			// to convert one char to lower case. You have to do it for all chars in string.
			int len = strlen(s);
			//len stores the number of characters in the word, thereby allowing me to use a for loop
			for (int i = 0 ; i < len; i++) {
				char ch = s[i];
				if (ch>='A'&&ch<='Z') {
					ch = (ch-'A')+'a';
					s[i]=ch;
				}    

			}
		}


		// Read a file and obtain words and positions of the words and save them in table.
		/*int wtable_createFromFile(WordTable * wtable, char * fileName, int verbose)
		  {

		// Write your code here
		LinkedList * k;
		FILE *fd = fopen(fileName, "r");
		//open the file to read from
		int i = 0;
		if(fd != NULL) {// use if instead of while, as this is not a loop
		char * word;
		while ((word = nextword(fd)) != NULL) {
		toLower(word);
		wtable_add(wtable, word, pos);
		if(verbose ==1) {
		printf("%d: word=%s, pos=%d\n", i ,word, pos);
		}
		i++;
		}
		}
		return 0;
		}*/

		// Sort table in alphabetical order.
		void rtable_sort(RoomTable * rtable)
		{
			// Write your code here
			RoomInfo temp;

			for ( int i = 0 ; i < rtable->nRooms; i++) {
				for ( int j = 0; j < rtable->nRooms; j++) {
					if( strcmp(rtable->roomArray[i].room,rtable->roomArray[j].room) < 0){
						temp  =rtable->roomArray[i];
						rtable->roomArray[i] =rtable->roomArray[j];
						rtable->roomArray[j] = temp;
					}
				}
			}
		}

		// Print all segments of text in fileName that contain word.
		// at most 200 character. Use fseek to position file pointer.
		// Type "man fseek" for more info. 
		/*int rtable_textSegments(RoomTable * rtable, char * room, char * fileName)
		  {

		// Write your code here
		FILE * fd = fopen(fileName, "r");
		char k;
		if (fd == NULL) return 0;
		printf("===== Segments for word \"%s\" in book \"%s\" =====\n", room, fileName);
		//get al the positions of the word, and store in a LinkedList
		LinkedList * list = wtable_getPositions(wtable,word );
		if(list->head ==NULL) return 0;
		// cycle through the text untill u get to the position using linkedlists 
		ListNode *i = list->head;
		while (i != NULL) {
		printf("---------- pos=%d-----\n", i->value);
		// use fseek(FILE *stream, long offset, int whence),
		// stream is fd, offset is the position of the word, whence is a preset option SEEK_SET
		fseek( fd, i->value, SEEK_SET);
		//print the segment of 200 characters
		printf("......");
		for (int j =0 ; j < 200; j++){
		//get character and store in linkedlist
		k = fgetc(fd);
		//print character
		printf("%c", k);
		}
		printf("......\n");
		//move on to the next instance of the word
		i = i->next;
		}
		fclose(fd);
		}*/

