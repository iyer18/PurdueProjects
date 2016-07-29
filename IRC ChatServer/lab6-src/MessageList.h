#ifndef Message_List
#define Message_List
// Data structs for a list of ints

struct MListNode {
	char * message;
	char * user;
	int value;
	struct MListNode * next;
};

typedef struct MListNode MListNode;

struct MessageList {
	MListNode * head;
};

typedef struct MessageList MessageList;

void mlist_init(MessageList * list);
char *  mlist_print(MessageList * list);
void mlist_add(MessageList * list, char * message, char * user);
char *  mlist_print_last(MessageList * list, long num);
int mlist_exists(MessageList * list,  char * user);
int mlist_remove(MessageList * list, char * message, char * user);
int mlist_get_ith(MessageList * list, int ith, char ** message, char ** user);
int mlist_remove_ith(MessageList * list, int ith);
int mlist_number_elements(MessageList * list);
//int mlist_save(MessageList * list, char * file_name);
//int mlist_read(MessageList * list, char * file_name);
void mlist_sort(MessageList * list, int ascending);
void mlist_clear(MessageList *list);

int mlist_remove_first(MessageList * list, char ** message, char ** user);
int mlist_remove_last(MessageList * list, char ** message, char ** user);
void mlist_insert_first(MessageList * list, char * message, char * user);
void mlist_insert_last(MessageList * list, char * message, char * user);
#endif
