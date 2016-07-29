
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "LinkedList.h"

//
// Initialize a linked list
//
void llist_init(LinkedList * list)
{
	list->head = NULL;
}

//
// It prints the elements in the list in the form:
// 4, 6, 2, 3, 8,7
//
char * llist_print(LinkedList * list) {

	ListNode * e;
	//char*  msg[llist_number_elements(list)];
	char * msg = (char*) malloc (1000000*sizeof(char));
	const char * line = "\r\n";
	if (list->head == NULL) {
		//	printf("{EMPTY}\n");
		const char * null = "NO-NEW-MESSAGES";
		strcat(msg, null);
		strcat(msg, line);
		//	msg[0] = "EMPTY\n";
		return msg;
	}

	//printf("{");

	e = list->head;
	int i = 0;
	while (e != NULL) {

		//printf("%s\n", e->user);
		//msg[i] = e->user
		strcat(msg, strdup(e->user));
		strcat(msg, line);
		e = e->next;
		if (e!=NULL) {
			//		printf("\n ");
		}
	}
	strcat(msg, line);
	return msg;
	//printf("\n");
}

//
// Appends a new node with this value at the beginning of the list
//
void llist_add(LinkedList * list, char *user, char * pword) {
	// Create new node
	ListNode * n = (ListNode *) malloc(sizeof(ListNode));
	n->user = user;
	n->pword = pword;

	// Add at the beginning of the list
	n->next = list->head;
	list->head = n;
}

//
// Returns true if the value exists in the list.
//
int llist_exists(LinkedList * list, char * user, char *pword) {
	ListNode * i;
	i = list->head;
	while (i != NULL) 
	{
		char * j = i->user;
		char * k = i->pword;
		if( (strcmp(j, user) == 0) && (strcmp(k, pword) ==0)) 
			return 1;
		else i = i->next;
	}
	return 0;
}

int llist_user_exists(LinkedList * list, char * user) {
	ListNode * i;
	i = list->head;
	while (i != NULL)
	{
		char * j = i->user;
		//char * k = i->pword;
		if( (strcmp(j, user) == 0) )
			return 1;
		else i = i->next;
	}
	return 0;
}

//
// It removes the entry with that value in the list.
//
int llist_remove(LinkedList * list, char * user, char * pword) {
	ListNode *i;
	ListNode *temp;
	i =  list->head;
	while (i->next != NULL)
	{
		char * j = i->user;
		if( strcmp(j, user) == 0) 
		{ 
			//i->next->value = 0;
			temp = i->next;
			i->user = i->next->user;
			i->pword = i->next->pword;
			i->next= i->next->next;
			return 1;

		}
		else i = i->next;
	}
	return 0;
}

//
// It stores in *value the value that correspond to the ith entry.
// It returns 1 if success or 0 if there is no ith entry.
//
int llist_get_ith(LinkedList * list, int ith, char ** user, char ** pword) {
	ListNode *i;
	ListNode *temp;
	i = list->head;
	int k = 0;
	while (i != NULL) 
	{
		if( k != ith)
		{
			i = i->next;
			k++;
		}
		else 
		{
			*user = i->user;
			*pword = i->pword;
			return 1;
		}
	}

	return 0;
}//
// It removes the ith entry from the list.
// It returns 1 if success or 0 if there is no ith entry.
//
int llist_remove_ith(LinkedList * list, int ith) {
	ListNode *i;
	ListNode *temp;
	i = list->head;
	int k  = 0;
	while(i->next != NULL)
	{
		if(k !=ith)
		{
			i = i->next;
			k++;
		}
		else 
		{
			//int val = i->user;
			temp = i->next;
			i->user = i->next->user;
			i->pword = i->next->pword;
			i->next= i->next->next;
			return 1;
		}

	}
	return 0;
}

//
// It returns the number of elements in the list.
//
int llist_number_elements(LinkedList * list) {
	ListNode *i;
	i = list->head;
	int k = 1;
	while (i->next != NULL) {
		i= i->next;
		k++;
	}
	return k;
}


//
// It saves the list in a file called file_name. The format of the
// file is as follows:
//
// value1\n
// value2\n
// ...
//
/*int llist_save(LinkedList * list, char * file_name) {
  ListNode *i;
//ListNode *value;
i = list->head;
FILE *fd = fopen(file_name, "w");
if( fd != NULL) {

while (i != NULL) 
{
int k = i->value;
fprintf(fd, "%d\n", k);
k++;
i = i->next;
}
fclose(fd);
return 0;
}
else return 0;

}*/

//
// It reads the list from the file_name indicated. If the list already has entries, 
// it will clear the entries.
//
//
/*int llist_read(LinkedList * list, char * file_name) {
  ListNode *i;
//ListNode *value;
i = list->head;
int k,c;
FILE *fd = fopen(file_name, "r");
char word[200];
int wordLength=0;
if( fd == NULL) {
return 0;
}
else {
while((c = fgetc(fd)) != EOF) {
if ((c != '\n') &&(c != '\t') && (c != ' ') && (c != '\b') &&(c != EOF))
{
word[wordLength] = c;
wordLength++;
}
if (((c == '\n') ||(c == '\t') || (c == ' ') || (c == '\b') ||(c == -1)) && (wordLength != 0 ))
{
word[wordLength] = 0;
wordLength = 0;
k = atoi(word);
llist_add(list, k);

}
//llist_add(list, k);

}
fclose(fd);
return 1;
}
return 0;
}*/


//
// It sorts the list. The parameter ascending determines if the
// order si ascending (1) or descending(0).
//
void llist_sort(LinkedList * list, int ascending) {

	//ListNode *i;
	int swapped = 1;
	ListNode *i;
	ListNode *temp=NULL;
	if (ascending == 1 ) {
		while(swapped)
		{
			swapped = 0;
			i = list->head;
			while (i->next != temp)
			{
				if (strcmp(i->user, i->next->user) > 0 )
				{ 
					char* a = i->user;
					char* b = i->pword;
					i->user  = i->next->user;
					i->pword = i->next->pword;
					i->next->user = a;
					i->next->pword = b;
					swapped = 1;
				}
				i = i->next;
			}
			temp = i;
			//llist_print(list);
		}
	}
	if (ascending == 0 ) {
		while(swapped)
		{
			swapped = 0;
			i = list->head;
			while (i->next != temp)
			{
				if (strcmp(i->user,i->next->user) < 0)
				{
					char * a = i->user;
					char * b = i->pword;
					i->user = i->next->user;
					i->pword = i->next->pword;
					i->next->user = a;
					i->next->pword = b;
					swapped = 1;
				}
				i = i->next;
			}
			temp = i;
			//llist_print(list);
		}

	}

}


//
// It removes the first entry in the list and puts value in *value.
// It also frees memory allocated for the node
//
int llist_remove_first(LinkedList * list, char ** user, char ** pword) {
	ListNode *i;
	ListNode *temp;
	i = list->head;

	while (i != NULL) {
		//*user = i->user;

		//int val = i->value;
		temp = i->next;
		i->user = i->next->user;
		i->pword = i->next->pword;
		i->next= i->next->next;
		return 1;
	}
	return 0;
}

//
// It removes the last entry in the list and puts value in *value/
// It also frees memory allocated for node.
//
int llist_remove_last(LinkedList * list, char ** user, char ** pword) {
	ListNode *i;
	ListNode *temp;
	i = list->head;
	int k;
	while (i != NULL)
	{
		int j = llist_number_elements(list);
		j=j-2;
		for( k = 0; k < j; k++) {
			i = i->next;
		}
		//*user = i->next->user;
		//char * u = i->user;
		temp = i->next;
		i->next= i->next->next;
		return 1;
	}
	return 0;

}

//
// Insert a value at the beginning of the list.
// There is no check if the value exists. The entry is added
// at the beginning of the list.
//
void llist_insert_first(LinkedList * list, char * user, char * pword) {
	ListNode *n = (ListNode *) malloc(sizeof(ListNode));
	//i = list->head;
	n->user = user;
	n->pword = pword;

	// Add at the beginning of the list
	n->next = list->head;
	list->head = n;

}

//
// Insert a value at the end of the list.
// There is no check if the name already exists. The entry is added
// at the end of the list.
//
void llist_insert_last(LinkedList * list, char * user, char * pword) {
	ListNode *n = (ListNode *) malloc(sizeof(ListNode));
	ListNode *i;
	i = list->head;
	while( i != NULL){
		//i = i->next;
		llist_insert_first(list, user, pword);
		return;
	}
	n->user = user;
	n->pword = pword;
	n->next = NULL;

	while(i->next) {
		i= i->next;
	}
	i->next = n;
}

//
// Clear all elements in the list and free the nodes
//
void llist_clear(LinkedList *list)
{

	list->head = NULL;

}
