#include <iostream>
#include <string>

using namespace std;

// Main function for sorting the file
void extern_sort(string in_fname, string out_fname,
		string chunk_location, int chunk_size);

// Pick your favorite n*log(n) sorting algorithm here!		
void sort(string* chunk, int start, int finish);

// Write a temporary chunk file
void write_chunk(string fileName, int numItems, string * data);

// Merges the k temporary files
void k_merge(string chunk_location, string out_fname, int num_chunks, int chunk_size);
int smallest( string * chunk, int num_chunks);