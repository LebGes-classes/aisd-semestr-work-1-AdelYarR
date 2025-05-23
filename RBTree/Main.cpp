#include "RedBlackTree.h"
#include "iostream";

int main()
{
	RedBlackTree rbtree = RedBlackTree();

    // ��������� �������� � ������
    rbtree.insert(7);
    rbtree.insert(3);
    rbtree.insert(18);
    rbtree.insert(10);
    rbtree.insert(22);
    rbtree.insert(8);
    rbtree.insert(11);
    rbtree.insert(26);
    rbtree.insert(2);
    rbtree.insert(6);

    // ����� ������
    rbtree.printTree();

    // ������� �������� �� ������
    std::cout << "Deleted 18:" << std::endl;
    rbtree.remove(18);
    rbtree.printTree();

    std::cout << "Deleted 11:" << std::endl;
    rbtree.remove(11);
    rbtree.printTree();

    std::cout << "Deleted 3:" << std::endl;
    rbtree.remove(3);
    rbtree.printTree();
}