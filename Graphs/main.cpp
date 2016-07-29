#include <iostream>
#include <string>
#include "graph.h"

using namespace std;

// Part 1: Find tickets for an airline passenger
void P5_part1()
{
	int numAir, numRoutes;
	// Read n and m values
	cin >> numAir >> numRoutes;
	// Create adjacency matrix
	Graph  g(numAir, numRoutes);
	string src, dest;
	// Read src
	cin >> src;
	// Read all ticket options
	while(src != "END") // Find tickets until END appears
	{
		// Read destinations
		cin >> dest;
		// Call method from graph class
		g.find_ticket(src, dest);
		// Read next src
		cin >> src;
	}
	delete[] g.row;
	for( int i = numAir; i > 0; i--)
		delete[] g.matrix[i- 1];
	delete[] g.matrix;
}

// Part 2: Perform an eulerian tour of the graph.
void P5_part2()
{
	string cmd1;
	int numAir, numRoutes;
	// Read m and n values
	cin >> numAir >> numRoutes;
	// Create adjacency matrix
	Graph g(numAir, numRoutes);
	// initiate the check boolean array to false
	g.check = new bool[numAir];
	for ( int i = 0; i < numRoutes; i++) 
		g.check[i] = false;
	// Sort adjacency matrix alphabetically
	//cout << "b prims" << endl;
	double ** mst = g.prims();

	if ( mst != NULL) {

		/*	cout << "print matrix" << endl;
			for(int x = 0; x < numAir; x++) {
			for(int y = 0 ; y < numAir; y++) 
			cout << mst[x][y] << " ";  
			cout << endl; 
			}*/
		//	 cout << "a prims" << endl;
		g.sort(mst);
		// Read root node
		cin >> cmd1;
		int node = -1;
		// Identify position of root node
		for ( int i = 0; i < numAir; i++) {
			if ( g.row[i].compare(cmd1) == 0) {
				node = i;
				break;
			}
		}
		// Call method from graph class
		string * output = new string[numAir];
		g.counter = 0;
		g.eulerian_tour( node, output, mst);
		if (g.counter == numAir) {
			for ( int i = 0 ; i < numAir; i++) {
				cout << output[i] << endl;
			}
		} 
		delete[] output;
	}
	else cout << "not possible" << endl;
	delete[] g.check;
	delete[] g.row;
	for( int i = numAir; i > 0; i--) {
		delete[] g.matrix[i- 1];
		delete[] mst[i-1];
	}
	delete[] g.matrix;
	delete[] mst;
}

int main()
{
	int cmd;
	cin >> cmd;
	if(cmd == 1) P5_part1();
	else if(cmd == 2) P5_part2();
	return 0;
}
