#include <iostream>
#include <cstdlib>
#include <cstring>

#include "heap.h"
#include "hash.h"

using namespace std;


void CuckooHashing() {
    // TODO : add your logic here.
    int r; // Table length
    int m; // Max Looping
    cin >> r >> m;
    Hash h(r, m);
    char command;
    while (cin >> command) {
        int n;
        switch(command){
            case 'i': // insert
                cin >> n;
                h.insert(n);
                break;
            case 'r': // remove
                cin >> n;
                h.remove(n);
                break;
            case 'l': // search
                cin >> n;
                pair<int, int> item = h.lookup(n);
                pair<int, int> null = make_pair(0,0);
                if( item != null) 
                    cout << n << " at T" << item.first << "[" << item.second << "]" << endl;
                else 
                    cout << "no " << n << endl;
                break;
        }
    }
}

void BHeap() {
    // TODO : add your logic here.
    int n;
    cin >> n; 
    char command1;
    int minmax;
    cin >> command1 >> minmax;
    bool max;
    if( minmax == 1) max = true;
    if( minmax == 2) max = false;
    BinaryHeap b(max);
    n--;
    while (n--) {
        
        char command;
        cin >> command;
        switch (command) {
            case 'i': //insert
                int key;
                int value;
                cin >> key >> value;
                b.insert(new TreeNode(key, value));
                break;
            case 's': //size
                cout << b.size() << endl;
                break;
            case 'e': //delete
                b.extract();
                break;
            case 'p': //peek
                b.peek();
                break;
            case 'h': //heapify
                int size;
                cin >> size;
                while ( size--) {
                    int key;
                    cin >> key;
                    b.insert(new TreeNode(key, 0));
                }
                break;
            default:
                cout << "Wrong option" << endl;
        }
    }
    cout  << endl;
}

void HeapSort() {
    // TODO : add your logic here.
    int minmax;
    cin >> minmax;
    bool max;
    if( minmax == 2) max = true;
    if( minmax == 1) max = false;
    BinaryHeap b(max);
    int numElements;
    cin >> numElements;
    int n = numElements;
    while ( n--) {
        int key;
        int value;
        cin >> key >> value;
        b.insert(new TreeNode(key, value));

    }
    char command;
    cin >> command;
     n = numElements;
    while ( n-- >0) {
        b.extract();
    }
    cout  << endl;

}


/* Main function */
int main(int argc, char** argv) {

    /* Read the option */
    int option;
    cin >> option;

    /* Call the respective function */
    switch(option) {

        /* Cuckoo Hashing */
        case 1:
            CuckooHashing();
        break;

        /* Binary Heap */
        case 2:
            BHeap();
        break;
        /* Heap Sort */
        case 3:
            HeapSort();
        break;

        /* Wrong option */
        default:
            cout << "Wrong option" << endl;

    }

    /* Close the program */
	return 0;
}

