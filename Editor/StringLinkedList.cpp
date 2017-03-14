#include "StringLinkedList.h"

/**
	Virtual destructor.
*/
StringLinkedList::~StringLinkedList() {
	Node *node = first;

	while (node != NULL) {
		Node *temp = node;
		node = node->next;
		delete temp;
	}
}

/**
	Appends a new Node with the specified data at the end of the list.
	@param data The data that will be appended with the new Node.
*/
void StringLinkedList::add(string data) {
	Node *node = new Node();
	node->data = data;

	if (first == NULL) {
		first = node;
		listSize++;
	}
	else {
		Node *currNode = first;
		Node *prevNode = NULL;

		while (currNode != NULL) {
			prevNode = currNode;
			currNode = currNode->next;
		}

		if (prevNode != NULL) {
			prevNode->next = node;
			listSize++;
		}
	}
}

/**
	Inserts a new node at the position specified by the index parameter
	@param index The position to insert the new Node at.
	@param data The data to insert into the new node.
*/
void StringLinkedList::insertAt(int index, string data) {
	Node *node = new Node();
	node->data = data;
	int i = 0;

	Node* currNode = first;
	Node* prevNode = NULL;

	if (index == 0) {
		if (first == NULL) {
			first = node;
		}
		else {
			node->next = first;
			first = node;
		}
		listSize++;
		return;
	}

	while (currNode != NULL) {
		if (i == index) {
			break;
		}

		prevNode = currNode;
		currNode = currNode->next;

		i++;
	}

	if (prevNode != NULL) {
		prevNode->next = node;
		node->next = currNode;
		listSize++;
	}
}

/**
	Updates a Node's data value.
	@param index The position of the node to be updated.
	@param value The new value of the node's data.
*/
void StringLinkedList::updateValue(int index, string value) {
	Node *currNode = first;
	int i = 0;

	while (currNode != NULL) {
		if (i == index) {
			currNode->data = value;
			break;
		}
		
		currNode = currNode->next;
		i++;
	}
}

/**
	Deletes the first node that contains the specified value.
	@param value The value of the node to be deleted.
*/
void StringLinkedList::deleteValue(string value) {
	Node *currNode = first;
	Node *prevNode = NULL;

	while (currNode != NULL) {
		if (currNode->data == value) {
			break;
		}

		prevNode = currNode;
		currNode = currNode->next;
	}

	if (currNode != NULL) {
		if (currNode == first) {
			first = currNode->next;
		}
		else {
			prevNode->next = currNode->next;
		}

		listSize--;
		delete currNode;
	}
}

/**
	Deletes a Node at the location specified by the index parameter.
	@param index The position of the Node to be deleted.
*/
void StringLinkedList::deleteNode(int index) {
	int i = 0;

	Node *currNode = first;
	Node *prevNode = NULL;

	while (currNode != NULL) {
		if (i == index) {
			break;
		}

		prevNode = currNode;
		currNode = currNode->next;

		i++;
	}

	if (i >= 0) {
		if (currNode == first) {
			first = currNode->next;
		}
		else {
			prevNode->next = currNode->next;
		}

		listSize--;
		delete currNode;
	}
}

/**
	Deletes a range of Nodes, starting at the position specified by the start parameter,
	and the size specified by the numItems parameter.
	@param start The position at which to start deleting Nodes.
	@param numItems The amount of Nodes to be deleted.
*/
void StringLinkedList::deleteRange(int start, int numItems) {
	Node *currNode = first;
	Node *prevNode = NULL;
	int i = 0;

	while (currNode != NULL) {
		if (i == start) {
			Node *temp = NULL;

			for (int x = 0; x < numItems; x++) {
				if (currNode != NULL) {
					temp = currNode;

					if (currNode->next != NULL) {
						currNode = currNode->next;
					}
					else {
						currNode = NULL;
					}

					delete temp;
					listSize--;
					temp = NULL;
				}
				else {
					break;
				}
			}

			if (prevNode != NULL) {
				prevNode->next = currNode;
			}
			else {
				first = currNode;
			}
			break;
		}

		prevNode = currNode;
		currNode = currNode->next;

		i++;
	}
}

/**
	Inserts a new Node after a Node with the specified value has been found. If no Nodes with
	the specified value can be found, the Node will be apended at the end of the list.
	@param value The value of the Node after which the new Node will be insterted.
	@param data The data of the new Node to be insterted.
*/
void StringLinkedList::insertAfterValue(string value, string data) {
	Node *node = new Node();
	node->data = data;

	// search for node to insert after
	Node *prev = first;

	while (prev != NULL) {
		if (prev->data == value) {
			break;
		}
		prev = prev->next;
	}

	// insert node into list
	if (first == NULL) {
		first = node;
		listSize++;
	}
	else {
		if (prev != NULL) {
			node->next = prev->next;
			prev->next = node;
			listSize++;
		}
		else {
			// could not find the node to insert after
			// so defaulting to Add function
			add(data);
		}
	}
}

/**
	Gets the value of Node at the position specified by the index parameter.
	@returns The value of the node.
*/
string StringLinkedList::get(int index) {
	string value;
	Node *currNode = first;
	int i = 0;

	while (currNode != NULL)
	{
		if (i == index) {
			value = currNode->data;
		}

		currNode = currNode->next;
		i++;
	}

	return value;
}

/**
	Returns the number of Nodes contained by this LinkedList.
	@returns The number of Nodes in the list.
*/
int StringLinkedList::size() {
	return listSize;
}

/**
	Overridden output (<<) operator.
*/
ostream& operator<<(ostream& output, StringLinkedList& list)
{
	Node *currNode = list.first;

	while (currNode != NULL)
	{
		output << currNode->data;

		currNode = currNode->next;

		if (currNode != NULL) {
			output << endl;
		}
	}

	return output;
}