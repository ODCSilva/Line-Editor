#ifndef NODE_H
#define NODE_H

#include <string>

using namespace std;

struct Node
{
public:
	Node() : next(NULL), data("") {}

	string data;
	Node *next;
};

#endif // !NODE_H