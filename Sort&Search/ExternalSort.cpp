#include "ExternalSort.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>





/*
	External Sorting algorithm.
	Step 1: Reads the input file one chunk at a time
	Step 2: Sorts each chunk
	Step 3: Writes each chunk to a temporary file
	Step 4: Performs a k-wise merge on the temporary files

*/
void extern_sort(string in_fname, string out_fname, string chunk_location, int chunk_size) {
	ifstream myReadFile;
	myReadFile.open(in_fname);
	string line;
	string * chunk = new string[chunk_size];
	int elementNum = 0;
	int num_chunks = 00000;
	int a = 0;
	while ( getline(myReadFile, line)) {
		// Step 1: Reads the input file one chunk at a time
		if (elementNum < chunk_size-1 ) 
			chunk[elementNum++] = line;
		else {
			chunk[elementNum] = line;
			// Step 2: Sorts each chunk 
			sort(chunk, 0, elementNum); 
			// Setup chunk id 
			ostringstream convert;
			convert << setfill('0') << setw(5)<< ++num_chunks;
			string fileName = chunk_location + "chunk" + convert.str() + ".txt";
			// Step 3: Writes each chunk to a temporary file
			write_chunk(fileName, elementNum+1, chunk);
			elementNum = 0;
		}
	}
	if ( elementNum != 0) {
		// Step 2: Sorts last posibly smaller chunk 
		sort(chunk, 0, elementNum-1); 
		// Sepup chunk id
		ostringstream convert;
		convert << setfill('0') << setw(5)<< ++num_chunks;
		string fileName = chunk_location + "chunk" + convert.str() + ".txt";
		// Step 3: Writes each chunk to a temporary file
		write_chunk(fileName, elementNum, chunk);
		elementNum = 0;
	}

	myReadFile.close();
	// Step 4: Performs a k-wise merge on the temporary files
	k_merge(chunk_location, out_fname,  num_chunks, chunk_size);
	return;
} 


void write_chunk(string fileName, int numItems, string * data) {
	ofstream chunkFile;
	chunkFile.open(fileName);
	// Write words on to file
	for ( int i = 0; i < numItems; i++) 
		chunkFile << data[i] << endl;
	chunkFile.close();
	return;
}


/*
	Your favorite n*log(n) sorting algorithm.
	Be sure that you use an in-place version of the algorithm.
	i.e. It does not generate a new array to return, but rather
	works by swapping elements in the input array.
	
	Possible options:
		-Quicksort
		-Mergesort
		-Heapsort
		
*/

void sort(string* chunk, int start, int finish) {
	// Merge Sort
	int left = start;
	int right = finish;
	string pivot = chunk[(start+finish)/2];
    while (left < right) {
    	while (chunk[left] < pivot) 
    		left++; 
    	while (chunk[right] > pivot) 
    		right--; 
    	if (left <= right) {
      		string temp = chunk[left];
      		chunk[left] = chunk[right];
      		chunk[right] = temp;
	  		left++;
  	  		right--;
    	} 
    } 
    if (start < right) sort(chunk,start,right);
    if (left < finish) sort(chunk,left,finish);
	return;
}

// Merges the k temporary files
void k_merge(string chunk_location, string out_fname, int num_chunks, int chunk_size) {
	ofstream myOutputFile;
	// Allocate memory
	ifstream * chunkfile = new ifstream[num_chunks];
	string ** chunk = new string*[num_chunks];
	for (int i = 0; i < num_chunks; i++)
    	chunk[i] = new string[chunk_size];
	int cPoint[num_chunks];
	// Read chunks and store values in matrix
	for (int i = 0; i < num_chunks; i++) {
		ostringstream os;
		os << setfill('0') << setw(5) << i+1;
		string id = os.str();
		chunkfile[i].open(chunk_location + "chunk" + id + ".txt");
		int j;
		for ( j = 0; j < chunk_size; j++) 
			chunkfile[i] >> chunk[i][j];
	}
	// Array to keep track of pointers
	fill_n(cPoint, num_chunks, 0);
	myOutputFile.open(out_fname);
	// Merge
	while ( true) {
		string * list = new string[num_chunks];
		for ( int i = 0; i < num_chunks; i++) {
			if( cPoint[i] < chunk_size && chunk[i][cPoint[i]] != "") {
				list[i] = chunk[i][cPoint[i]];
			}
			else
				list[i] = "-1";
		}
		int incPoint = smallest(list , num_chunks);
		cPoint[incPoint]++;
		int flag = 0;
		for ( int i = 0; i < num_chunks; i++) 
			if ( list[i] != "-1")
				flag = 1;
		if ( flag == 0)
			break;
		myOutputFile << list[incPoint] << "\n";		
	}

	
	return;
}

int smallest( string* chunk, int num_chunks) {
	int smallest = 0;
	string temp;
	for (int i = 0; i < num_chunks; i++) {
		if ( chunk[i] != "-1") {
			 temp = chunk[i];
			 smallest = i;
			 break;
		}
	}
	for (int i = 0; i < num_chunks; i++) {
    	if ( chunk[i] != "-1" && chunk[i] < temp ) {
        	temp = chunk[i];
        	smallest = i;
    	}
	}
	return smallest;
}













