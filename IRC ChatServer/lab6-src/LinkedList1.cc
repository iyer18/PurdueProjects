
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
void llist_print(LinkedList * list) {

	ListNode * e;

	if (list->head == NULL) {
		printf("{EMPTY}\n");
		return;
	}

	printf("{");

	e = list->head;
	while (e != NULL) {
		printf("%d", e->value);
		e = e->next;
		if (e!=NULL) {
			printf(", ");
		}
	}
	printf("}\n");
}

//
// Appends a new node with this value at the beginning of the list
//
void llist_add(LinkedList * list, int value) {
	// Create new node
	ListNode * n = (ListNode *) malloc(sizeof(ListNode));
	n->value = value;

	// Add at the beginning of the list
	n->next = list->head;
	list->head = n;
}

//
// Returns true if the value exists in the list.
//
int llist_exists(LinkedList * list, int value) {
	ListNode * i;
	i = list->head;
	while (i != NULL) 
	{
		int j = i->value;
		if( j == value) return 1;
		else i = i->next;
	}
	return 0;
}

//
// It removes the entry with that value in the list.
//
int llist_remove(LinkedList * list, int value) {
	ListNode *i;
	ListNode *temp;
	i =  list->head;
	while (i->next != NULL)
	{
		int j = i->value;
		if( j == value) 
		{ 
			//i->next->value = 0;
			temp = i->next;
			i->value = i->next->value;
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
int llist_get_ith(LinkedList * list, int ith, int * value) {
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
			*value = i->value;
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
			int val = i->value;
			temp = i->next;
			i->value = i->next->value;
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
int llist_save(LinkedList * list, char * file_name) {
	ListNode *i;
	ListNode *value;
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

}

//
// It reads the list from the file_name indicated. If the list already has entries, 
// it will clear the entries.
//
int llist_read(LinkedList * list, char * file_name) {
	ListNode *i;
	ListNode *value;
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
}


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
				if (i->value > i->next->value)
				{ 
					int a = i->value;
					i->value = i->next->value;
					i->next->value = a;
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
				if (i->value < i->next->value)
				{
					int a = i->value;
					i->value = i->next->value;
					i->next->value = a;
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
int llist_remove_first(LinkedList * list, int * value) {
	ListNode *i;
	ListNode *temp;
	i = list->head;

	while (i != NULL) {
		*value = i->value;

		int val = i->value;
		temp = i->next;
		i->value = i->next->value;
		i->next= i->next->next;
		return 1;
	}
	return 0;
}

//
// It removes the last entry in the list and puts value in *value/
// It also frees memory allocated for node.
//
int llist_remove_last(LinkedList * list, int *value) {
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
		*value = i->next->value;
		int val = i->value;
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
void llist_insert_first(LinkedList * list, int value) {
	ListNode *n = (ListNode *) malloc(sizeof(ListNode));
	//i = list->head;
	n->value = value;

	        // Add at the beginning of the list
         n->next = list->head;
         list->head = n;
	
}

//
// Insert a value at the end of the list.
// There is no check if the name already exists. The entry is added
// at the end of the list.
//
void llist_insert_last(LinkedList * list, int value) {
	ListNode *n = (ListNode *) malloc(sizeof(ListNode));
	ListNode *i;
	i = list->head;
	while( i->next != NULL){
		i = i->next;
	}
	i->next = n;
	n->value = value;

}

//
// Clear all elements in the list and free the nodes
//
void llist_clear(LinkedList *list)
{

	list->head = NULL;

}
