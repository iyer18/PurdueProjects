
#ifndef Linked_List
#define Linked_List
// Data structs for a list of ints

struct ListNode {
	char * user;
	char * pword;
	struct ListNode * next;
};

typedef struct ListNode ListNode;

struct LinkedList {
	ListNode * head;
};

typedef struct LinkedList LinkedList;

void llist_init(LinkedList * list);
char *  llist_print(LinkedList * list);
void llist_add(LinkedList * list, char * user, char * pword);
int llist_exists(LinkedList * list, char * user, char * pword);
int llist_user_exists(LinkedList * list, char * user);
int llist_remove(LinkedList * list, char * user, char * pword);
int llist_get_ith(LinkedList * list, int ith, char ** user, char ** pword);
int llist_remove_ith(LinkedList * list, int ith);
int llist_number_elements(LinkedList * list);
//int llist_save(LinkedList * list, char * file_name);
//int llist_read(LinkedList * list, char * file_name);
void llist_sort(LinkedList * list, int ascending);
void llist_clear(LinkedList *list);

int llist_remove_first(LinkedList * list, char ** user, char ** pword);
int llist_remove_last(LinkedList * list, char ** user, char ** pword);
void llist_insert_first(LinkedList * list, char * user, char * pword);
void llist_insert_last(LinkedList * list, char * user, char *pword);
#endif
