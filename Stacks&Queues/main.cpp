#include <iostream>
#include "stack.h"
#include "deque.h"
#include <string>
//TODO: Include your stack and deque files here

using namespace std;



void read_part_1()
{
	//Read the number of commands to execute
	int n;	
	cin >> n;
	// Initialize your stack here...
	Stack s;
	// Variable to store first command
	char command1;
	// Variables to store values 1 and 2
	int value1,  value2;
	// While loop to iterate through all the commads given for the stack execution
	while (n--) 
	{	
		// Read in the first command
		cin >> command1;
		// If statement to check if command1 is an insersion command
		if (command1 == 'i')
		{
			// TODO: read and push the value in your stack here.
			cin >> value1;
			cin >> value2;
			pair<int, int> item = make_pair(value1, value2);
			s.push(item);
		}
		// If statement to check if command1 is a deletion command
		if (command1 == 'p')
		{
			// TODO: Call Pop here, and print the value.
			pair <int, int> item = s.pop();
			pair<int, int> t= make_pair(-1,-1);
			if ( item == t)
				cout << "empty" << endl;
			else
				cout << item.first << " " << item.second << endl;
		}
		// If statement to check if command1 is a check size command
		if (command1 == 's')
		{
			// TODO: print the size of the stack here.
			cout << s.numElements << endl;
		}
	}
}


void read_part_2()
{

	//Read in and store the number of rows and cols in the puzzle
	int num_rows,num_cols;
	cin >> num_rows >> num_cols;
	//Declare and initialize the word search array //cout << "num_rows: " << num_rows << " num_cols: " << num_cols << endl; 
	char** word_search;
	word_search = new char*[num_rows];
	for(int i = 0; i < num_rows; ++i)
		word_search[i] = new char[num_cols];

	//Fill in the array with inputs from the console
	for(int i = 0; i < num_rows; i++) {
		for(int j = 0; j < num_cols; j++) {
			cin >> word_search[i][j];
		}
	}
	//Read in the word we're looking for
	string to_find;
	cin >> to_find;

	//TODO: Part 1b: Run the word search
	// Create an object of the Stack class
	Stack s;
	// Create a pair object 
	pair <int, int> item;
	// Initialize coodinate variables 
	int x = 0;
	int y = 0;

	// While loop to iterate through all the letter of the word to search
	int k = 0;
	while  ( k < to_find.length()) {
		// Initialize variables to be used in nested for loops
		int i, j;
		int flag = 0;

		// Find the first letter in matrix
		if ( k == 0) {
			for(i = 0; i < num_rows; i++) {
				for(j = 0; j < num_cols; j++) {
					oncemore: // Arrives here if the first letter does not lead to the complete word
					if (word_search[i][j] == to_find[0]) {
						item = make_pair(i, j);
						// Push first element into the Stack
						s.push (item) ;
						//cout << "1st item added: " << i << " " << j << endl;
						// Set i and j as new x and y coordinates
						x = i;
						y = j;
						// Increment k  and skip out of all loops
						k++;
						goto finish;
					}
				}
			}
		}
		// Check to see if the first letter has been found, if not the word does not exist
		if ( k == 0) {
			cout << "not found" << endl;
			return;
		}
		finish: // Arrives here after breaking out of all previous loops
		// Initialize new vairable to check for surrounding letter in the word search
		int dx, dy;
		// Special nested for loop that iterates through all surrounding letters
		for ( dx = (x > 0 ? -1 : 0); dx <= (x < num_rows ? 1 : 0); ++dx) {
			for ( dy = (y > 0 ? -1 : 0); dy <= (y < num_cols ? 1 : 0); ++dy) {
				again: // Arrives here after poping dead ends
				if ( word_search[x + dx][y + dy] == to_find[k]) {
					// Update coordinates
					x += dx;
					y += dy;
					item = make_pair(x, y);
					// Push next letter into the Stack
					s.push (item) ;
					//cout << "item added: " << x << " " << y << endl;
					// As next letter has been found successfully, update flag to 1
					flag = 1;
					// Skip out of all loops
					goto skip;	
				}
			}
		}
		// If at any point all but the very first letter in this track reach a dead end
		if ( k == 1)
		{	
			// Update coordinates
			if( (j + 1) <= num_cols)
			{
				j++;
			}
			else {
				i++;
				j = 0;
			}
			// Delete the first letter from the stack and look for another occurance for the first letter
			pair<int, int> del = s.pop();
			//cout << "item deleted: " << del.first << " " << del.second << endl;
			k--;
			goto oncemore;
		} 
		// Check to see if the latest letter was found
		if ( flag != 1) 
		{
			// Temporarily store x and y in tx and ty
			int tx = x;
			int ty = y;
			// Pop and discard the last letter as it is a dead end
			pair <int, int> del = s.pop();
			//cout << "item deleted: " << del.first << " " << del.second << endl;
			// Pop the second last letter to get its coordinates
			pair <int, int> del2 = s.pop();
			//cout << "item deleted: " << del2.first << " " << del2.second << endl;
			// Push it back in as it may not be a dead end as it may have another path
			s.push(del2);
			//cout << "item readded: " << del2.first << " " << del2.second << endl;
			// Update x and y and dx and dy values
			x = del2.first;
			y = del2.second;
			if( (ty - y + 1) <= 1) {
				dx = tx - x;
				dy = ty - y + 1;
			} else {
				dx = tx - x + 1;
				dy = ty - y;
			}
			// Decrement k as we now have to look for the previous letter
			k--;
			// Repeat the seach for last letter 
			goto again;	

		}
		skip: // Arrives here after skiping through all loops and checks
		// Increment k as the last letter has been found
		k++;
	}	
	// Initialize new stack as the order of old stack is reverse
	Stack s2;
	// Loop thrigh elements in old stack and push into new stack
	while (s.numElements != 0){
		pair <int, int> itemP = s.pop();
		s2.push(itemP);
	}
	// Loop through new stack and print outputs in correct order
	while (s2.numElements != 0) {
		pair <int, int> itemP2 = s2.pop();
		cout << itemP2.first << " " << itemP2.second << endl;
	}


}





void read_part_3()
{
	//Read in the number of commands to execute
	int n;
	cin >> n;
	// Create variable for the commands
	char command1,command2;
	// Create and object for the Deque Class
	Deque d;
	// While loop to iterate over all the commands
	while(n--)
	{	
		// Read first command
		cin >> command1;
		// Switch case of commands
		switch(command1)
		{
			// Enque case
			case 'e':
				// Read command2
				cin >> command2;
				// Switch case of all enque command2s
				switch(command2)
				{	
					// Front case
					case 'f':
						//TODO: Read in an int and front-enqueue
						int in1;
						cin >> in1;
						d.enqueue_front(in1);
						break;
					// Back case
					case 'b':
						//TODO: Read in an int and back-enqueue
						int in2;
						cin >> in2;
						d.enqueue_back(in2);
						break;
				}
				break;
			// Deque case
			case 'd':
				// Read command2
				cin >> command2;
				// Switch case of all deque command2s
				switch(command2)
				{	
					// Front case
					case 'f':
						//TODO: dequeue from the front and print the result
						int out1;
						out1 = d.dequeue_front();
						cout << out1 << endl;
						break;
					// Back case
					case 'b':
						//TODO:dequeue from the back and print the result
						int out2;
						out2 = d.dequeue_back();
						cout << out2 << endl;
						break;
				}
				break;
			// Size case
			case 's':
				//TODO: Print the size of the deque's array
				cout << d.size << endl;
				break;
		}
	}
}

void read_input()
{
	int project_part;
	//Read the first line of the test case
	cin >> project_part;

	switch(project_part)
	{
		case 1:
			read_part_1();
			break;
		case 2:
			read_part_2();
			break;
		case 3:
			read_part_3();
			break;
	}
}

int main()
{
	// Read in the test case
	read_input();
	return 0;
}
