#pragma once

enum class Color
{
	Red,
	Black
};

class Node
{
	int key;
	Color color = Color::Red;
	Node* left = nullptr;
	Node* right = nullptr;
	Node* parent = nullptr;

public:
	Node(int _key);

	int getKey();
	void setKey(int _key);
	Color getColor();
	void setColor(Color _color);
	Node* getLeft();
	void setLeft(Node* _left);
	Node* getRight();
	void setRight(Node* _right);
	Node* getParent();
	void setParent(Node* _parent);
};