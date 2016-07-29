#include <iostream>
#include <string>
#include "ExternalSort.h"
#include "ExternalSearch.h"

using namespace std;

int main(int argc, const char* argv[])
{	
	
	if (argc < 4) {
		cout << "Usage: \n./program 1 Input.txt Output.txt ChunkSize ChunkLocation \n./program 2 Input.txt StringToFind" << endl;
		exit(1);
	}
	if (argv[1] == string("1")) // Run P4 Part 1
		extern_sort(argv[2], argv[3], argv[5], atoi(argv[4]));
	else if (argv[1] == string("2")) {
		string filename = argv[2];
		for ( int i = 3; i < argc; i++) {
			if (extern_search(filename, argv[i])) 
				cout << "found" << endl;
			else
				cout << "missing" << endl;

		}
	}
	return 0;
}