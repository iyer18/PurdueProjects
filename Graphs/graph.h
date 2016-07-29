#include <iostream>
#include <string>

using namespace std;

class Graph
{
	public:
		// Constructor
		Graph(int n, int m);
		// Part 1: Find a ticket using Dijkstra
		void find_ticket(const string &source,const string &destination); 
		void dijkstra(int src, int dest);
		// Part 2: Try to do a tour of all cities
		void eulerian_tour(int node, string * output, double ** mst); 
		double** prims();
		void sort(double** mst);
		int minKey(double * key, bool * mstSet);


		// Instance variables
		string * row; // array to store position of all airports
		double ** matrix; // adjacency matrix 
		int rSize; // number of airports
		bool * check; // check array 
		int counter;
	
	// Add any needed private methods here
};