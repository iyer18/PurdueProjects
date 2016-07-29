#include <iostream>
#include "deque.h"


using namespace std;


Deque::Deque()
{
	//TODO: Write the Deque constructor
	//NOTE: Start with an array of size 10!
	// Initialize new array of size 10
	myArray = new int[10];
	// Set pointer variables front and back to -1
	front = -1;
	rear = -1;
	// Set counter variable to 0
	count = 0;
	// Set size variable to 10
	size = 10;
}

void Deque::enqueue_front(int n)
{
	//TODO: Front enqueue method
	again: // Arrives here after reallocation has happened
	// If front is -1 ( no elements in the queue)
	if( front == -1)
	{
		// Increment Front and rear pointer 
		front++;
		rear++;
		// Add element n at the 0th index
		myArray[rear] = n;
		// Increment counter
		count++;
	}
	// If queue is full, reallocate and then retry implementing
	else if ( is_full())
	{
		reallocate();
		goto again;
	}
	else {	
		// Loop through elements in queue and move all the exixting elements back
		int i;
		for ( i = count; i > 0; i--)
		{
			myArray[i] = myArray[i-1];
		}
		// Add the new element n at the 0th index
		myArray[i] = n;
		// Increment counter
		count++;
		// Increment the rear pointer so it points to the last element in the queue
		rear++;
	}
}

void Deque::enqueue_back(int n)
{
	//TODO: Back enqueue
	again: // Arrives here after reallocation
	// If front is -1 ( no elements in the queue)
	if ( front == -1)
	{
		// Increment Front and rear pointer 
		front++;
		rear++;
		// Add element n at the 0th index
		myArray[rear] = n;
		// Increment counter
		count++;
	}
	// If queue is full, reallocate and then retry implementing
	else if ( is_full())
	{
		reallocate();
		goto again;
	}
	else 
	{
		// Add new element n after the rear pointer
		myArray[++rear] = n;
		// Increment counter
		count++;	
	}
}

int Deque::dequeue_front()
{
	//TODO: front dequeue
	// If front is -1 the queue is empty
	if ( front == -1) 
	{
		cout << "empty ";
	}
	else 
	{
		// if front == rear, there is only one element in the queues, 
		// so set front and rear to -1 to indicate an empty queue
		if ( front == rear) 
		{
			int ans = myArray[front];
			front = rear = -1;
			return ans;
		}
		// Set answer variable to a[front] 
		int ans = myArray[front];
		count--;
		int i;
                for ( i = 0; i <  count; i++)
                {
                        myArray[i] = myArray[i+1];
                }
		// Increment front and decrement count
		rear--;
		return ans;	
	}
	return -1;
}

int Deque::dequeue_back()
{
	//TODO: back dequeue
	// If front is -1 the queue is empty
	if ( front == -1) 
	{
		cout << "empty ";
	}
	else 
	{
		// if front == rear, there is only one element in the queues, 
		// so set front and rear to -1 to indicate an empty queue
		if( front == rear) 
		{
			int ans = myArray[rear];
			front = rear = -1;
			return ans;
		}
		
		// Set answer variable to a[front] 
		int ans = myArray[rear];
		// Decrement rear and count and return ans
		rear--;
		count--;
		return ans;
	}
	return -1;
}

bool Deque::is_full()
{
	//TODO: Determine if you need to double the array
	if ( rear >= size - 1 )
	{
		return true;
	}
	return false;
}

void Deque::reallocate()
{
	//TODO: Correctly reallocate memory for the deque. Use the doubling strategy.
	int nsize = 2 * size;
	int* nmyArray = new int[nsize];
	for ( int i = 0; i < size; i++)
		nmyArray[i] = myArray[i];
	myArray = nmyArray;
	size = nsize;
	delete [] nmyArray;

}
