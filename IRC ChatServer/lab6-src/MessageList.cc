
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MessageList.h"

//
// Initialize a linked list
//
void mlist_init(MessageList * list)
{
	list->head = NULL;
}

//
// It prints the elements in the list in the form:
// 4, 6, 2, 3, 8,7
//
char *  mlist_print_last(MessageList * list, long num) {

	MListNode * e;

	char * msg = (char*) malloc (1000000*sizeof(char));
	const char * line = "\r\n";
	if (list->head == NULL) {
		const char * null = "NO-NEW-MESSAGES";
		strcat(msg, null);
		strcat(msg, line);
		return msg;
	}

	int i = 0;//(mlist_number_elements(list) - 1);
	char * str= (char*) malloc(100*sizeof(char));

	//	e = list->head;
	if((num) == (mlist_number_elements(list) )) {
		strcat(msg, "NO-NEW-MESSAGES");
		strcat(msg, line);
		return msg;
	}
	e = list->head;
	while (e != NULL) {
		while( i <= num) {
			i++;
			e = e->next;
		}
		sprintf(str, "%d", i);
		strcat(msg, str);
		strcat(msg, " ");
		//mlist_get_ith(list, i, message);
		strcat(msg, strdup(e->user));
		strcat(msg, " ");
		strcat(msg, strdup(e->message));
		strcat(msg, line);
		e = e->next;
		i++;
		if(i == (mlist_number_elements(list) )) {
			strcat(msg, line);
			return msg;
		}
		/*	if((num+1) == (mlist_number_elements(list) )) {
			strcat(msg, "NO-NEW-MESSAGES");
			strcat(msg, line);
			return msg;
			}*/
	}

	strcat(msg, line);
	return msg;
}

char *  mlist_print(MessageList * list) {

		MListNode * e;
		char * msg = (char*) malloc (1000000*sizeof(char));
		const char * line = "\r\n";
		if (list->head == NULL) {
			const char * null = "NO-NEW-MESSAGES";
			strcat(msg, null);
			strcat(msg, line);
			return msg;
		}


		e = list->head;
		int i = 0;
		while (e != NULL) {

			strcat(msg, strdup(e->user));
			strcat(msg, line);
			e = e->next;
			if (e!=NULL) {
			}
		}
		strcat(msg, line);
		return msg;
}

//
// Appends a new node with this value at the beginning of the list
//
void mlist_add(MessageList * list, char * message, char * user) {
	// Create new node
	MListNode * n = (MListNode *) malloc(sizeof(MListNode));
	n->user = user;
	n->message = message;

	n->next = list->head;
	list->head = n;
}

//
// Returns true if the value exists in the list.
//
int mlist_exists(MessageList * list, char * user) {
	MListNode * i;
	i = list->head;
	while (i != NULL) 
	{
		char * j = i->user;

		if( strcmp(j, user) == 0 )
			return 1;
		else i = i->next;
	}
	return 0;
}

//
// It removes the entry with that value in the list.
//
int mlist_remove(MessageList * list, char * message, char * user) {
	MListNode *i;
	MListNode *temp;
	i =  list->head;
	while (i->next != NULL)
	{
		char * j = i->message;
		char * k = i->user;
		if( strcmp(j, message) == 0) 
		{ 
			//i->next->value = 0;
			temp = i->next;
			i->message = i->next->message;
			i->user = i->next->user;
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
int mlist_get_ith(MessageList * list, int ith, char ** message) {
	MListNode *i;
	MListNode *temp;
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
			*message = i->message;
			return 1;
		}
	}

	return 0;
}//
// It removes the ith entry from the list.
// It returns 1 if success or 0 if there is no ith entry.
//
int mlist_remove_ith(MessageList * list, int ith) {
	MListNode *i;
	MListNode *temp;
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
			temp = i->next;
			i->message = i->next->message;
			i->next= i->next->next;
			return 1;
		}

	}
	return 0;
}

//
// It returns the number of elements in the list.
//
int mlist_number_elements(MessageList * list) {
	MListNode *i;
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
/*int mlist_save(LinkedList * list, char * file_name) {
  MListNode *i;
  MListNode *value;
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
/*int mlist_read(LinkedList * list, char * file_name) {
  MListNode *i;
  MListNode *value;
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
  mlist_add(list, k);

  }
//mlist_add(list, k);

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
void mlist_sort(MessageList * list, int ascending) {

	//MListNode *i;
	int swapped = 1;
	MListNode *i;
	MListNode *temp=NULL;
	if (ascending == 1 ) {
		while(swapped)
		{
			swapped = 0;
			i = list->head;
			while (i->next != temp)
			{
				if (strcmp(i->user, i->next->user) > 0)
				{ 
					char *  a = i->user;
					i->user = i->next->user;
					i->next->user = a;
					swapped = 1;
				}
				i = i->next;
			}
			temp = i;
			//mlist_print(list);
		}
	}
	if (ascending == 0 ) {
		while(swapped)
		{
			swapped = 0;
			i = list->head;
			while (i->next != temp)
			{
				if (strcmp(i->user ,i->next->user)< 0)
				{
					char * a = i->user;
					i->user = i->next->user;
					i->next->user = a;
					swapped = 1;
				}
				i = i->next;
			}
			temp = i;
			//mlist_print(list);
		}

	}

}


//
// It removes the first entry in the list and puts value in *value.
// It also frees memory allocated for the node
//
int mlist_remove_first(MessageList * list, char ** message) {
	MListNode *i;
	MListNode *temp;
	i = list->head;

	while (i != NULL) {
		//*value = i->value;

		//int val = i->value;
		temp = i->next;
		i->message = i->next->message;
		i->next= i->next->next;
		return 1;
	}
	return 0;
}

//
// It removes the last entry in the list and puts value in *value/
// It also frees memory allocated for node.
//
int mlist_remove_last(MessageList * list, char ** message) {
	MListNode *i;
	MListNode *temp;
	i = list->head;
	int k;
	while (i != NULL)
	{
		int j = mlist_number_elements(list);
		j=j-2;
		for( k = 0; k < j; k++) {
			i = i->next;
		}
		//*value = i->next->value;
		//int val = i->value;
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
void mlist_insert_first(MessageList * list, char * message, char * user) {
	MListNode *n = (MListNode *) malloc(sizeof(MListNode));
	//i = list->head;
	n->message = message;
	n->user = user;

	// Add at the beginning of the list
	n->next = list->head;
	list->head = n;

}

//
// Insert a value at the end of the list.
// There is no check if the name already exists. The entry is added
// at the end of the list.
//
void mlist_insert_last(MessageList * list, char * message, char * user) {


	MListNode *n = (MListNode *) malloc(sizeof(MListNode));
	MListNode *i;
	i = list->head;
	if( i == NULL) {
		mlist_insert_first(list, message, user);
		return ;
	}
	n->message = message;
	n->user = user;
	n->next = NULL;

	//MListNode *last = NULL;

	while( i->next){
		i = i->next;
	}
	i->next = n;
	/*n->message = message;
	  n->user = user;
	  n->next = NULL;
	  if( last == NULL) {
	  list->head = n;
	  } else {
	  last->next = n;
	  }*/
}

//
// Clear all elements in the list and free the nodes
//
void mlist_clear(MessageList *list)
{

	list->head = NULL;

}
