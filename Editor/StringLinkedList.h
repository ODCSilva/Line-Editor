#ifndef STRINGLINKEDLIST_H
#define STRINGLINKEDLIST_H
#include "Node.h"
#include <string>

using namespace std;

class StringLinkedList
{
private:
	Node *first;
	int listSize;

public:
	friend ostream& operator<<(ostream& output, StringLinkedList& list);
	int size();
	string get(int index);
	StringLinkedList() : first(NULL), listSize(0) {}
	virtual ~StringLinkedList();
	void add(string data);
	void deleteNode(int index);
	void deleteRange(int start, int numItems);
	void deleteValue(string value);
	void insertAfterValue(string value, string data);
	void insertAt(int index, string data);
	void updateValue(int index, string value);
};

#endif
