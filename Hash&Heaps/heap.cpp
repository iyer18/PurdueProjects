#include <iostream>
#include <cstdlib>
#include <string>

#include "heap.h"

using namespace std;

bool TreeNode::operator < (const TreeNode& anotherNode) {
	// TODO : add your logic here.
	return (this->key < anotherNode.key);
}

bool TreeNode::operator > (const TreeNode& anotherNode) {
	// TODO : add your logic here.
	return (this->key > anotherNode.key);
}

/*
 * If isMaxHeap == true, initialize as a MaxHeap.
 * Else, initialize as a MinHeap.
 */
BinaryHeap::BinaryHeap(bool isMaxHeap) {
	// TODO : add your logic here.
	this->isMaxHeap = isMaxHeap;
	root = NULL;
	count = 0;


}
/*
 * Given an array of TreeNode elements, create the heap.
 * Assume the heap is empty, when this is called.
 */
void BinaryHeap::heapify(int size, TreeNode * nodes) {

	// TODO : add your logic here.
	// This has been replaced by downheap and upheap
}
/*
 * Given the root node, downheap, starts at the top and sorts the heap by swaping
 * Should only be called if root is not null
 */
void BinaryHeap::downheap(TreeNode * nodes) {
	// Temporary TreeNode to store the root node
	TreeNode * current = nodes;
	// if Max Heap
	if ( isMaxHeap == true ) {
		// if both children exist
		if (current->leftChild != NULL && current->rightChild != NULL) {
			// if c < l && c < r
			if (current->key < current->leftChild->key && current->key < current->rightChild->key) {
				// if l < r
				if (current->leftChild->key > current->rightChild->key) {
					// swap
					int tempKey = current->key;
					int tempValue = current->value;
					current->key = current->leftChild->key;
					current->value = current->leftChild->value;
					current->leftChild->key = tempKey;
					current->leftChild->value = tempValue;
					// call downheap again with old root that is now the left child of the new root
					downheap(current->leftChild);

				} else { // else if r > l
					// swap
					int tempKey = current->key;
					int tempValue = current->value;
					current->key = current->rightChild->key;
					current->value = current->rightChild->value;
					current->rightChild->key = tempKey;
					current->rightChild->value = tempValue;
					// call downheap again with old root that is now the right child of the new root
					downheap(current->rightChild);
				}
			// if c < l && c > r	
			} else if (current->key < current->leftChild->key && current->key > current->rightChild->key) {
				// swap
				int tempKey = current->key;
				int tempValue = current->value;
				current->key = current->leftChild->key;
				current->value = current->leftChild->value;
				current->leftChild->key = tempKey;
				current->leftChild->value = tempValue;
				// call downheap again with old root that is now the left child of the new root
				downheap(current->leftChild);
			// if c > l && c < r
			} else if (current->key > current->leftChild->key && current->key < current->rightChild->key) {
				// swap
				int tempKey = current->key;
				int tempValue = current->value;
				current->key = current->rightChild->key;
				current->value = current->rightChild->value;
				current->rightChild->key = tempKey;
				current->rightChild->value = tempValue;
				// call downheap again with old root that is now the right child of the new root
				downheap(current->rightChild);

			}
		// r == NULL && l != NULL	
		} else if (current->rightChild == NULL && current->leftChild != NULL) {
			// c < l
			if ( current->key < current->leftChild->key ) {
				// swap
				int tempKey = current->key;
				int tempValue = current->value;
				current->key = current->leftChild->key;
				current->value = current->leftChild->value;
				current->leftChild->key = tempKey;
				current->leftChild->value = tempValue;
				// reached Last node so no need to call downHeap() again;
			}
		}
	// Min Heap	
	} else {
		// l != NULL && r != NULL		
		if (current->leftChild != NULL && current->rightChild != NULL) {
			// c > l && c > r
			if (current->key > current->leftChild->key && current->key > current->rightChild->key) {
				// l < r
				if (current->leftChild->key < current->rightChild->key) {
					// lswap
					int tempKey = current->key;
					int tempValue = current->value;
					current->key = current->leftChild->key;
					current->value = current->leftChild->value;
					current->leftChild->key = tempKey;
					current->leftChild->value = tempValue;
					// call downheap again with old root that is now the left child of the new root
					downheap(current->leftChild);
				// l > r
				} else {
					// swap
					int tempKey = current->key;
					int tempValue = current->value;
					current->key = current->rightChild->key;
					current->value = current->rightChild->value;
					current->rightChild->key = tempKey;
					current->rightChild->value = tempValue;
					// call downheap again with old root that is now the right child of the new root
					downheap(current->rightChild);
				}
			// c > l && c < r
			} else if (current->key > current->leftChild->key && current->key < current->rightChild->key) {
				// swap	
				int tempKey = current->key;
				int tempValue = current->value;
				current->key = current->leftChild->key;
				current->value = current->leftChild->value;
				current->leftChild->key = tempKey;
				current->leftChild->value = tempValue;
				// call downheap again with old root that is now the left child of the new root
				downheap(current->leftChild);
			// c < l && c > r
			} else if (current->key < current->leftChild->key && current->key > current->rightChild->key) {
				// swap
				int tempKey = current->key;
				int tempValue = current->value;
				current->key = current->rightChild->key;
				current->value = current->rightChild->value;
				current->rightChild->key = tempKey;
				current->rightChild->value = tempValue;
				// call downheap again with old root that is now the right child of the new root
				downheap(current->rightChild);
			}
		// r == NULL && l 1= NULL	
		} else if (current->rightChild == NULL && current->leftChild != NULL) {
			// c > l
			if (current->key > current->leftChild->key ) {
				// swap
				int tempKey = current->key;
				int tempValue = current->value;
				current->key = current->leftChild->key;
				current->value = current->leftChild->value;
				current->leftChild->key = tempKey;
				current->leftChild->value = tempValue;
				// reached Last node so no need to call downHeap() again;
			}
		}
	}
}
/*
 * Given the last most node, upheap, starts at the bottom and sorts the heap by swaping upward
 * 
 */
void BinaryHeap::upheap( TreeNode * nodes) {

	TreeNode * current = nodes;
	// Max heap
	if ( isMaxHeap == true) {
		// if root is reached
		if( current->parent == NULL) return;
		// c > p
		if ( current->key > current->parent->key) {
			// swap
			int tempKey = current->parent->key;
			int tempValue = current->parent->value;
			current->parent->key = current->key;
			current->parent->value = current->value;
			current->key = tempKey;
			current->value = tempValue;
			// call upheap again with the parent of the current node
			upheap(current->parent);
		}
	}
	// Min Heap
	else {
		// if root is reached
		if( current->parent == NULL) return;
		// c < p
		if ( current->key < current->parent->key) {
			// swap
			int tempKey = current->key;
			int tempValue = current->value;
			current->key = current->parent->key;
			current->value = current->parent->value;
			current->parent->key = tempKey;
			current->parent->value = tempValue;
			// call upheap again with the parent of the current node
			upheap(current->parent);

		}
	}


}

/*
 * insert the node into the heap.
 * return false, in case of failure.
 * return true for success.
 */
bool BinaryHeap::insert(TreeNode * node) {
	// TODO : add your logic here.
	// if root in null, set up heap
	if ( root == NULL) {
		root = node;
		count++;
		return true;
	}
	// else add node to heap
	else {
		// TreeNode variable to keep track of current node
		TreeNode * current = root;
		// Increment counter of elements in heap
		count++;
		// get binary value of count to help insert element in BinaryHeapTree;
		int value = count;
		int remainder;
		int * binary = (int*)malloc((100) * sizeof(int));
		int j = 0;
		while(value != 0) {
			remainder = value % 2;  
			binary[j] = remainder;     
			j++;                            
			value /= 2;
		}
		// Use binary numbers to iterate through heap to get to the last new node slot in the heap
		int i = 1;
		for(  i = j - 2; i > 0; i--) {
			if( binary[i] == 0) 
				current = current->leftChild;
			if( binary[i] == 1) 
				current = current->rightChild;
		}
		// Last parent reached, if last binary num is 0, the new node is to be inserted as the left child
		if( binary[i] == 0) {
			current->leftChild = node;
			current->leftChild->parent = current;
			// call upheap to sort the array
			upheap(current->leftChild);
		}
		// Last parent reached, if last binary num is 1, the new node is to be inserted as the right child
		if( binary[i] == 1) {
			//   cout << "bin = 1" << endl;
			current->rightChild = node;
			current->rightChild->parent = current;
			// call upheap to sort the array
			upheap(current->rightChild);
		}
	}
	return true;
}



/*
 * Return the minimum element of the min-heap [max element of max-heap]
 */
TreeNode * BinaryHeap::extract() {
	// TODO : add your logic here.
	// if root in null, Heap is empty
	if ( root == NULL) {
		cout << "empty" << endl;
		return NULL;
	}
	// This node is deleted and replaced
	TreeNode * current = root;
	// Last node that replaces the root
	TreeNode * replace  = root;
	// Use binary to get to the last node (same as in insert)
	int value = count;
	int remainder;
	int * binary = (int*)malloc((100) * sizeof(int));
	int j = 0;
	while(value != 0) {
		remainder = value % 2;  
		binary[j] = remainder;     
		j++;                            
		value /= 2;
	}
	int i;
	for(  i = j - 2; i >= 0; i--) {
		if( binary[i] == 0)
			replace = replace->leftChild;
		if( binary[i] == 1)
			replace = replace->rightChild;
	}
	// Prints the deleted node
	cout << current->key << " " << current->value << endl;
	// Root node is replaced by the last node
	root->key = replace->key;
	root->value = replace->value;
	// The last most node is set to null
	if( replace->parent != NULL) {
		TreeNode * p = replace->parent;
		if ( p->leftChild != NULL && p->leftChild->key == replace->key)
			p->leftChild = NULL;
		if ( p->rightChild != NULL && p->rightChild->key == replace->key)
			p->rightChild = NULL;
	}
	else 
		root = NULL;
	// count is decremented as size of the heap has decreased
	count--;
	// downheap is called to sort the heap
	if( count > 0) 
		downheap(root);
	return current;
}

/*
 * Print the minimum element of the min-heap [max element of max-heap]
 */
void BinaryHeap::peek() {
	// TODO : add your logic here.
	// If count is 0, heap is empty
	if( count == 0) {
		cout << "empty" << endl;
		return;
	}
	// Print max or min most element
	cout << root->key << " " << root->value << endl;
}
/*
 * Return the current size of the Heap.
 */
int BinaryHeap::size() {
	// TODO : add your logic here.
	return count;
}

void heapSort(TreeNode * elements, bool isReverseOrder) {
	// TODO : add your logic here.
}
