#include <iostream>
#include <utility>
using namespace std;

class Hash
{
	public:
		Hash(int, int); 
		int h1(int);
		int h2(int);
		void rehash();
		bool insert(int);
		bool remove(int);
		pair<int,int> lookup(int);
		int size;
		int maxLoop;
		int * T1;
		int * T2;
};
