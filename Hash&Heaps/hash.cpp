#include <iostream>
#include <utility>
#include "hash.h"
using namespace std;


Hash::Hash(int n, int max)
{
	//TODO: initialize Hash table 1 and 2
	size = n;
	maxLoop = max;
    // Create T1
	T1 = new int[size];
    // Initiazize T1 elements to -1
	fill_n(T1, size, -1);
    // Create T2
	T2 = new int[size];
    // Initiazize T2 elements to -1
	fill_n(T2, size, -1);

}

int Hash::h1(int n) 
{
    // h1 = n mod k
    return n % size;
}
int Hash::h2(int n) 
{
    // h2 = n mod (k-1)
	return n % (size-1);
}
void Hash::rehash() 
{
    // store T1 and T2 temporarily in T3 and T4
	int * T3 = T1;
	int * T4 = T2;
    // double size
	size *= 2;
    // Resize T1 and T2
	T1 = new int[size];
	T2 = new int[size];
    // Re initilize T1 and T2 to -1
	fill_n(T1, size, -1);
	fill_n(T2, size, -1);
    // iterate through both T3 and T4 and insert elements (not -1) into T1 and T2
	for ( int i = 0; i < size/2; i++) {
		if( T3[i] != -1)
			insert(T3[i]);
		if( T4[i] != -1)
			insert(T4[i]);
	}
}
bool Hash::insert(int n) 
{
	 // TODO:  insert int n
	 pair<int, int> found = lookup(n);
	 pair<int, int> null = make_pair(0,0);
	 int m = maxLoop;
    // if lookup(x) then return
	 if( found != null) {
	 	cout << n << " already in T" << found.first << "[" << found.second << "]" << endl;
	 	return false;
	 }
	 int x = n;
     // loop MaxLoop times
	 while ( m-- > 0) {
        //   if T1[h1(x)] = empty then {T1[h1(x)] = x; return }
	 	if( T1[h1(x)] == -1) {
	 		T1[h1(x)] = x;
	 		cout << x << " in T1[" << h1(x) << "]!" << endl;
	 		return true;
	 	}
        //   x ↔ T1[h1(x)]
	 	else {
	 		int temp = T1[h1(x)];
	 		T1[h1(x)] = x;
	 		cout << x << " in T1[" << h1(x) << "]" << endl;
	 		x = temp;
	 	}
        //   ifT2[h2(x)] = empty then {T2[h2(x)] = x; return }
	 	if( T2[h2(x)] == -1) {
	 		T2[h2(x)] = x;
	 		cout << x << " in T2[" << h2(x) << "]!" << endl;
	 		return true;
	 	}
        //   x ↔ T2[h2(x)]
	 	else {
	 		int temp = T2[h2(x)];
	 		T2[h2(x)] = x;
	 		cout << x << " in T2[" << h2(x) << "]" << endl;
	 		x = temp;
	 	}
	 // end loop
     }
	 cout << "maxloop reached" << endl;
     // rehash()
     rehash();
     //insert(x)
	 insert(x);
	 return true;

}
bool Hash::remove(int n)
{
	// TODO:	remove int n 49 out T1[9]
    // if n exists in T1 remove
	if( T1[h1(n)] == n) {
		T1[h1(n)] = -1;
		cout << n << " out T1[" << h1(n) << "]" << endl;
		return true;
	}
    // if n exists in T2 remove
	else if( T2[h2(n)] == n) {
		T2[h2(n)] = -1;
		cout << n << " out T2[" << h2(n) << "]" << endl;
		return true;
	}
    // n doesnt exist
	else {
		cout << "no out " << n << endl;
		return false;
	}


}
pair<int,int> Hash::lookup(int n) 
{
	// TODO:	look for int n
    // if n exists in T1 return ( T?, key?)
	if ( T1[h1(n)] == n)
		return make_pair(1,h1(n));
    // if n exists in T2 return ( T?, key?)
	else if ( T2[h2(n)] == n)
		return make_pair(2,h2(n));
    // n doesnt exist
	else
		return make_pair(0,0);
}
