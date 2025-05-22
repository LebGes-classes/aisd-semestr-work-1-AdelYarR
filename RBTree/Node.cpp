#include "Node.h"

Node::Node(int _key)
{
	this->key = _key;
	this->color = Color::Red;
	this->left = nullptr;
	this->right = nullptr;
	this->parent = nullptr;
}

int Node::getKey()
{
	return this->key;
}

void Node::setKey(int _key)
{
	this->key = _key;
}

Color Node::getColor()
{
	return color;
}

void Node::setColor(Color _color)
{
	this->color = _color;
}

Node* Node::getLeft()
{
	return this->left;
}

void Node::setLeft(Node* _left)
{
	this->left = _left;
}

Node* Node::getRight()
{
	return this->right;
}

void Node::setRight(Node* _right)
{
	this->right = _right;
}

Node* Node::getParent()
{
	return this->parent;
}

void Node::setParent(Node* _parent)
{
	this->parent = _parent;
}
