#include "ExternalSearch.h"
#include <iostream>
#include <fstream>


bool extern_search(string fileName, string toFind) {
	// Open file
	fstream file;
	file.open(fileName);
	string temp;
	int first = 0;
	int middle = 0;
	int last = 0;
	// SetUp pointers
	file.seekg( 0, file.end );
	int numChars = file.tellg();
	last = numChars;
	file.seekg( 0, file.beg );
	first = file.tellg();
	// Binary Sreach
	while(first <= last) {
		middle = ( first + last ) / 2;
		file.seekg( middle );
		while ( file.tellg() != 0 && file.peek() != 10 ) 
			file.seekg(-1, file.cur);
		file >> temp;
		if( temp > toFind )  
			last = middle - 1;
		else if( temp < toFind )  
			first = middle + 1;
		else 
			return true;
		//cout << "first: " << first << " last: " << last << " middle: " << middle << endl;  //**
		if ( first == last && last == numChars ) 
			break;
	}
	return false;
}
