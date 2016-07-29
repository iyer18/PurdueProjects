#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include "graph.h"

using namespace std;

/*
	Graph class for use in Project 5.
	
	This provided code does not include any specific data structure
	for storing a graph, only a list of methods that will be called 
	from main. Based on the project handout and the lecture slides,
	select a method for storing the graph and implement it here.

*/

// Constructor for Graph. Read in data from the input
// and set up your data structures here.
Graph::Graph(int n, int m)
{
	string src,dest;
	double price;
	rSize = n;
	matrix = new double * [n];
	for ( int i = 0; i < n; i++) {
		matrix[i] = new double[n];
	}
	for ( int i = 0 ; i < n ; i++) {
		for ( int j = 0 ; j < n; j++) {
			if ( i == j)
				matrix[i][j] = 0;
			else 
				matrix[i][j] = INFINITY;
		}
	} 
	row = new string [n];
	int r = 0;
	for ( int i = 0; i < m; i ++) {
	 	cin >> src >> dest >> price;
	 	bool sflag = true;
	 	bool dflag = true;
	 	int x;
	 	int y;
	 	for ( int j = 0; j < n; j++) {
	 		if ( row[j].compare(src) == 0) {
	 			x = j;
	 			sflag = false;
	 		}
	 		if ( row[j].compare(dest) == 0) {
	 			y = j;
	 			dflag = false;
	 		}
	 		if ( dflag == false && sflag == false)
	 			break;
	 	}
	 	if ( sflag == true) {
	 		x = r;
	 		row[r++] = src;
	 	}
	 	if ( dflag == true) {
	 		y = r;
	 		row[r++] = dest;
	 	}
	 	matrix[x][y] = price;
	 	matrix[y][x] = price;

	}
	/*cout << "print matrix" << endl;
	for(int x = 0; x < n; x++) {
        for(int y = 0 ; y < n; y++) 
            cout << matrix[x][y] << " ";  
    	cout << endl; 
    }*/

}

// Code for part 1. Print out the sequence of airports and price
// of the trip.
void Graph::find_ticket(const string &source,const string &destination)
{
	int s = -1;
	int d = -1;
	for ( int i = 0; i < rSize; i++) {
		if ( row[i].compare(source) == 0) 
			s = i;
		if ( row[i].compare(destination) == 0)
			d = i;
		if( s != -1 && d != -1)
			break;
	}
		dijkstra(s, d);	
}
void Graph::dijkstra(int src, int dest) {
	double dist[rSize];
	bool done[rSize];
	int path[rSize];
	int paths[rSize][rSize];
	for ( int i = 0 ; i < rSize ; i++) {
		for ( int j = 0 ; j < rSize; j++) { 
				paths[i][j] = 0;
		}
	}
	for ( int i = 0; i < rSize; i++) {
		dist[i] = INFINITY;
		done[i] = false;
		path[i] = 0;
	}
		paths[src][src] = ++path[src];

	//path[src] = 0;
	dist[src] = 0;
	while ( true) {
		int v = -1;
		double smallest = INFINITY;
		for ( int i = 0; i < rSize; i++) {
			if( !done[i] && dist[i] < smallest) {
				v = i;
				smallest = dist[i];
				//cout << "r " << row[i] << endl;
					//paths[i][j] = 0;
			}
		}
				//cout << "v: " << v<< endl;

		if ( smallest == INFINITY) break;
		for ( int i = 0; i < rSize; i++) {
			if( !done[i] && matrix[v][i] != INFINITY) {
				if ( dist[i] > dist[v] + matrix[v][i]) {
					dist[i] = dist[v] + matrix[v][i];
					//cout << " " << row[v] << " i " << i << " matrix[v][i] " << matrix[v][i] << " dist[v] " << dist[v] << " dist[i] " << dist[i] << endl;
					//if (  path[v] == -1)
					//	path[v] = k++;
					//cout << row[v] << " " << i << endl;
 					
					if ( paths[v][src] != 0) {
						//path[i] = 0;
						for ( int j = 0; j < rSize; j++) {
								paths[i][j] = 0;
						}
						path[i] = 0;
						int k = 1;
						for ( int j = 0; j < rSize; j++) {
							if(paths[v][j] != 0 && paths[i][j] == 0 && paths[v][j] == k) {
								paths[i][j] = ++path[i];
								k++;
								j = -1;
							} 
						}
					}
					if( paths[i][v] == 0)
						paths[i][v] = ++path[i];
					/*for ( int i = 0; i < rSize; i++) {
						for ( int j = 0; j < rSize; j++)
							cout << paths[i][j]  << " ";
						cout << endl;
					}
					cout << endl;*/
					
				}
			}
		}
		//path[v] = k;
		if( v != src) 
			paths[v][v] = ++path[v];
		done[v] = true;

	}
	//if ( path[dest] == -1)
	//		path[dest] = k++;
	int j = 0;
	if( dist[dest] != INFINITY) {
		cout << fixed << setprecision(2);
		/*for ( int i = 0; i < rSize; i++) {
			for ( int j = 0; j < rSize; j++)
				cout << paths[i][j]  << " ";
			cout << endl;
		}*/
		int k = 1;
		for ( int i = 0; i < rSize; i++) {
			if( paths[dest][i] == k) {
				cout << row[i] << " ";
				i = -1;
				k++;
			}
		}
		cout << dist[dest] << endl;
	}
	else 
		cout << "not possible" << endl;
}

int Graph::minKey(double * key, bool * mstSet)
{
   // Initialize min value
   double min = INFINITY;
   int minI;
 
   for (int i = 0; i < rSize; i++) {
     if (mstSet[i] == false && key[i] < min) {
         min = key[i];
         minI = i;
           // cout << "minI: " << minI << endl;

     }
   }
   //cout << "minI: " << minI << endl;
   return minI;

}
double** Graph::prims() {
	int mst[rSize];
	double key[rSize];
	bool mstCheck[rSize];
	for ( int i = 0; i < rSize; i++) {
		key[i] = INFINITY;
		mstCheck[i] = false;
	}

	key[0] = 0 ;
	mst[0] = -1;
	for ( int i = 0; i < rSize - 1; i++) {
		int u = minKey( key, mstCheck);
		//cout << "minkey: " << u<< endl;
		mstCheck[u] = true;
		for ( int j = 0; j < rSize; j++) {
			if( matrix[u][j] != 0 && matrix[u][j] != INFINITY && !mstCheck[j] && matrix[u][j] < key[j]) {
				mst[j] = u;
				//cout << "mst: " << j << ", " << mst[j] << endl;
				key[j] = matrix[u][j];
			}
		}

	}
	//cout << "d prims" << endl;

	double ** msg = new double * [rSize];
	for ( int i = 0; i < rSize; i++) {
		msg[i] = new double[rSize];
	}
	for ( int i = 0 ; i < rSize ; i++) {
		for ( int j = 0 ; j < rSize; j++) {
			if ( i == j)
				msg[i][j] = 0;
			else 
				msg[i][j] = INFINITY;
		}
	}
	//cout << "a matrix" << endl;

	for ( int i = 1; i < rSize; i++) {
		//cout << mst[i] << " - " << i << " " << matrix[i][mst[i]]<< endl;
		if(mst[i] >= 0 && mst[i] < rSize ) {
		msg[mst[i]][i] = matrix[i][mst[i]];
		msg[i][mst[i]] = matrix[i][mst[i]];
		}
		else return NULL;

	}
	return msg;
}
void Graph::sort(double ** mst) {
	for ( int i = 0; i <rSize; i++) {
		for ( int j = i ; j < rSize; j++) {
			if ( row[i].compare(row[j]) > 0) {
				string temp = row[i];
				row[i] = row[j];
				row[j] = temp;
				double * mtemp  = mst[i];
				mst[i] = mst[j];
				mst[j] = mtemp;
				for ( int k = 0 ; k < rSize; k++) {
					double tmp = mst[k][i];
					mst[k][i] = mst[k][j];
					mst[k][j] = tmp;
				}
			}
		} 
	}
}

// Code for part 2. Print out the tour sequence or "not possible"
// if not all cities can be visited.
void Graph::eulerian_tour(int node, string * output, double ** mst)
{
	//cout << row[node] << endl;
	output[counter++] = row[node];
	check[node] = true;
	string temp = " ";
	for ( int i = 0; i < rSize; i++) {
		if( mst[i][node] != INFINITY  &&  mst[i][node] != 0 && !check[i]) {
			eulerian_tour(i, output, mst);
		}
	}
	
}













