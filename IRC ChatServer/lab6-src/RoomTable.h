#ifndef Room_Table
#define Room_Table

#include "LinkedList.h"
#include "MessageList.h"
struct RoomInfo {
	char * room;
	LinkedList userpass;
	MessageList messages;
};

typedef struct RoomInfo RoomInfo;

struct RoomTable {
	// Number of words in the array
	int nRooms;

	// Maximum number of words in array before resiing
	int maxRooms;
	RoomInfo * roomArray;
};

typedef struct RoomTable RoomTable;

// Initializes a word table
void rtable_init(RoomTable * rtable);

// Print contents of the table.
char * rtable_print(RoomTable * rtable);

// Add word to the tableand position. Position is added to the corresponding linked list.
char * rtable_send_message( RoomTable *rtable, char * room, char *user, char  *pword, char * message);
char * rtable_get_message(RoomTable * rtable, char * room, char * user, char * pword, long num);
char * rtable_remove_user( RoomTable *rtable, char * room, char *user, char  *pword);
char *  rtable_add_user(RoomTable * rtable, char * room, char * user, char * pword);
char * rtable_get_user(RoomTable * rtable, char * room, char * user, char * pword);
char *  rtable_add(RoomTable * rtable, char * room, char * user, char * pword);
bool rtable_exists(RoomTable *rtable, char * room);

// Read a file and obtain words and positions of the words and save them in table.
// 
//int rtable_createFromFile(RoomTable * rtable, char * fileName, int verbose);

// Get pointer to list of positions where the word occurs
LinkedList * rtable_getPositions(RoomTable * rtable, char * room);

// Sort table in alphabetical order.
void rtable_sort(RoomTable * rtable);

// Print all segments of text in fileName that contain word.
// from pos-100 to pos+100. Use fseek to position file pointer.
// Type "man fseek" for more info. 
//int rtable_textSegments(RoomTable * rtable, char * room, char * fileName);
#endif
