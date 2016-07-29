
//
// Implementation of a HashTable that stores void *
//
#include "HashTableVoid.h"

// Obtain the hash code of a key
int HashTableVoid::hash(const char * key)
{
	// Add implementation here
	int sum = 0;
	const char * p = key;
	while (*p ) {
		sum = sum + *p;
		p++;
	}
	return sum % 2039;
}

// Constructor for hash table. Initializes hash table
HashTableVoid::HashTableVoid()
{
	// Add implementation here
	_buckets = (HashTableVoidEntry **) malloc (2039 * sizeof(HashTableVoidEntry*));

	int i;
	for(i = 0; i < 2039; i++) {
		_buckets[i] = NULL;
	}

}

// Add a record to the hash table. Returns true if key already exists.
// Substitute content if key already exists.
bool HashTableVoid::insertItem( const char * key, void * data)
{
	// Add implementation here
	int sum = hash(key);
	HashTableVoidEntry * elm = _buckets[sum];
	while( elm != NULL) {
		if ( strcmp(elm->_key, key) == 0) {
			elm->_data = data;
			return true;
		}
		elm = elm->_next;
	}
	elm = _buckets[sum];
	while (elm != NULL) {
		elm = elm->_next;
	}

	HashTableVoidEntry * nElm = new HashTableVoidEntry();
	nElm->_key = strdup(key);
	nElm->_data = data;
	nElm->_next = NULL;
	_buckets[sum] = elm;
	if( elm == NULL) {
		_buckets[sum] = nElm;
	}
	else {
		elm->_next = nElm;
	}
	return false;
}

// Find a key in the dictionary and place in "data" the corresponding record
// Returns false if key is does not exist
bool HashTableVoid::find( const char * key, void ** data)
{
	// Add implementation here
	int sum = hash(key);
	HashTableVoidEntry * elm = _buckets[sum];
	while( elm != NULL) {
		if( strcmp(elm->_key, key) == 0) {
			*data = elm->_data;
			return true;
		}
		elm = elm->_next;
	}
	return false;
}

// Removes an element in the hash table. Return false if key does not exist.
bool HashTableVoid::removeElement(const char * key)
{
	// Add implementation here
	int sum = hash(key);
	HashTableVoidEntry * elm = _buckets[sum];
	HashTableVoidEntry * temp = NULL;

	while( elm != NULL ) {
		if (strcmp(elm->_key, key) == 0) {
			if(temp != NULL) {
				temp->_next = elm->_next;
			}
			else {
				_buckets[sum] = elm->_next;
			}
			//free(elm->_key);
			delete elm;
			return true;
		}
		temp = elm;
		elm = elm->_next;
	}
	return false;
}

// Creates an iterator object for this hash table
HashTableVoidIterator::HashTableVoidIterator(HashTableVoid * hashTable)
{
	// Add implementation here
	_hashTable = hashTable;
	_currentBucket = 0;
	_currentEntry = NULL;

}

// Returns true if there is a next element. Stores data value in data.
bool HashTableVoidIterator::next(const char * & key, void * & data)
{
	// Add implementation here

	if (_currentEntry != NULL) // first run
	{
		if (_currentEntry->_next != NULL)
		{
			key = _currentEntry->_next->_key;
			data = _currentEntry->_next->_data;
			_currentEntry = _currentEntry->_next;
			return true;
		}
	}
	int i;
	i = _currentBucket + 1;
	while (i < 2039 &&
			_hashTable->_buckets[i] == NULL)
	{
		i++;
	}
	if (i < 2039 &&
			_hashTable->_buckets[i] != NULL)
	{
		key = _hashTable->_buckets[i]->_key;
		data = _hashTable->_buckets[i]->_data;
		_currentBucket = i;
		_currentEntry = _hashTable->_buckets[i];
		return true;
	}
	return false;
}

