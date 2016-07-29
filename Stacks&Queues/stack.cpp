#include "stack.h"
#include <utility>


Stack::Stack(){
	// initialize the top most element to NULL
	top = NULL;
}
void Stack::push(pair<int, int> item)
{	
	//TODO: Write the push method
	// Create and Initialize a new node
	struct node *n = new node;
	// Store the item to push in this node
	n->data = item;
	// Set this node's next element to NULL
	n->next=NULL;
	// If the Stack is not empty attach this node to the top most element of the Stack
	if( top!=NULL)
		n->next=top;
	// Set n to be the top most element in the Stack
	top=n;
	// Increment number of elements in the Stack
	numElements++;

}

pair<int,int> Stack::pop()
{
	//TODO: Write the pop method
	// Create a new node n
	struct node *n;
	// If the top most node in the Stack is null, the Stack is empty so return an empty pair
	if(top==NULL) {
		pair<int,int> t = make_pair(-1,-1);
		return t;
	}
	// Set the top most node as the new node created
	n=top;
	// set the next top most node to the top node
	top=top->next;
	// Store the date in the n node into a temp pair object
	pair<int,int> temp =  n->data;
	// Delete the previous top most node
	delete n;
	// Decrement the number of elements in the Stack
	numElements--;
	// Return the data in the temp pair variable;
	return temp;
}
