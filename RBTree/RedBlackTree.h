#pragma once
#include "Node.h"

class RedBlackTree
{
	Node* root = nullptr;

public:
	RedBlackTree();
	RedBlackTree(Node* _root);

	void insert(int key);
	void remove(int key);
	bool search(int key);
	void fixInsertion(Node* t);
	void fixDeleting(Node* t);
	void leftRotate(Node* t);
	void rightRotate(Node* t);
	void printTree();
};
