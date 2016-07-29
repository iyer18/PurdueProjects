 #include <iostream>
 #include <utility>
 
 using namespace std;
 


struct node
{
    //int data;
    pair < int, int > data;
    struct node *next;
    
};

class Stack
 {
    struct node *top;
    public:
        Stack();
		void push(pair<int, int> data);
		pair<int, int> pop();
        int numElements = 0;
     
 };
