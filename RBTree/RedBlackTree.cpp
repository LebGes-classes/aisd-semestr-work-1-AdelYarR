#include "RedBlackTree.h"
#include <iostream>

RedBlackTree::RedBlackTree() : root(nullptr) {}

RedBlackTree::RedBlackTree(Node* _root) : root(_root) {}

RedBlackTree::~RedBlackTree()
{
    deleteTree(root);
}

void RedBlackTree::insert(int key)
{
    Node* t = new Node(key);

    if (root == nullptr)
    {
        root = t;
    }
    else
    {
        Node* p = root;
        Node* q = nullptr;

        // Проходим с корня вниз для вставки элемента
        while (p != nullptr)
        {
            q = p;
            if (p->getKey() < t->getKey())
            {
                p = p->getRight();
            }
            else
            {
                p = p->getLeft();

            }
        }

        t->setParent(q);

        if (q->getKey() < t->getKey())
        {
            q->setRight(t);
        }
        else
        {
            q->setLeft(t);
        }
    }

    // Исправляем ситуацию, если два красных узла идут подряд
    fixInsertion(t);
}

void RedBlackTree::fixInsertion(Node* t)
{
    if (t == nullptr) return;

    // Если текущий узел - корень, то по свойству он чёрный
    if (t == root)
    {
        t->setColor(Color::Black);
        return;
    }

    Node* parent = t->getParent();
    if (parent == nullptr) return;

    // Поиск нарушения свойства, чтобы красный родитель не имел красных сыновей
    while (t != root && t->getColor() == Color::Red && parent->getColor() == Color::Red)
    {
        Node* grandparent = parent->getParent();
        if (grandparent == nullptr) break;

        // Если отец - левый узел
        if (grandparent->getLeft() == parent)
        {
            Node* uncle = grandparent->getRight();
            // Если дядя красный, то красим отца и дядю в чёрный, а дедушку в красный
            if (uncle != nullptr && uncle->getColor() == Color::Red)
            {
                parent->setColor(Color::Black);
                uncle->setColor(Color::Black);
                grandparent->setColor(Color::Red);
                t = grandparent;
                parent = t->getParent();
            }
            else
            {
                // Если дядя чёрный, то делаем поворот
                if (parent->getRight() == t)
                {
                    leftRotate(parent);
                    t = parent;
                    parent = t->getParent();
                }
                rightRotate(grandparent);
                parent->setColor(Color::Black);
                grandparent->setColor(Color::Red);
                t = parent;
            }
        }
        else  // Отец - правый узел
        {
            Node* uncle = grandparent->getLeft();
            if (uncle != nullptr && uncle->getColor() == Color::Red)
            {
                parent->setColor(Color::Black);
                uncle->setColor(Color::Black);
                grandparent->setColor(Color::Red);
                t = grandparent;
                parent = t->getParent();
            }
            else
            {
                if (parent->getLeft() == t)
                {
                    rightRotate(parent);
                    t = parent;
                    parent = t->getParent();
                }
                leftRotate(grandparent);
                parent->setColor(Color::Black);
                grandparent->setColor(Color::Red);
                t = parent;
            }
        }
    }

    root->setColor(Color::Black);
}

void RedBlackTree::remove(int key)
{
    Node* node = root;
    // Поиск узла по ключу
    Node* q = nullptr;
    while (node != nullptr)
    {
        if (node->getKey() == key)
        {
            q = node;
            break;
        }
        else if (node->getKey() < key)
        {
            node = node->getRight();
        }
        else {
            node = node->getLeft();
        }
    }

    if (q == nullptr)
    {
        std::cout << "Failed to remove: node hasn't been found" << std::endl;
        return;
    }

    Color originalColor = q->getColor();
    // Узел, с которого начнется восстановления возможных нарушений баланса
    Node* p = nullptr;
    // Если у узла есть только один ребёнок или вообще нет детей
    if (((q->getLeft() != nullptr) ^ (q->getRight() != nullptr)) ||
        (q->getLeft() == nullptr && q->getRight() == nullptr))
    {
        p = (q->getLeft() != nullptr) ? q->getLeft() : q->getRight();

        if (q->getParent() == nullptr)
        {
            root = p;
        }
        else
        {
            if (p != nullptr)
            {
                p->setParent(q->getParent());
            }

            if (q == q->getParent()->getLeft())
            {
                q->getParent()->setLeft(p);
            }
            else {
                q->getParent()->setRight(p);
            }
        }
    } 
    // Если у узла есть оба ребёнка
    else
    {
        // Идём к правому сыну, а затем проходим по левым сыновьям
        Node* r = q->getRight();
        while (r->getLeft() != nullptr)
        {
            r = r->getLeft();
        }

        p = r->getRight();
        originalColor = r->getColor();

        if (r->getParent() != q)
        {
            r->getParent()->setLeft(p);
            if (p != nullptr)
            {
                p->setParent(r->getParent());
            }

            r->setRight(q->getRight());
            q->getRight()->setParent(r);
        }
        else
        {
            if (p != nullptr) {
                p->setParent(r);
            }
        }

        if (q->getParent() == nullptr)
        {
            root = r;
        }
        else {
            if (q->getParent()->getLeft() == q) {
                q->getParent()->setLeft(r);
            }
            else 
            {
                q->getParent()->setRight(r);
            }

            r->setParent(q->getParent());
        }

        r->setLeft(q->getLeft());
        q->getLeft()->setParent(r);
        r->setColor(q->getColor());
    }

    delete q;
    // Если мы удалили чёрный узел, то чёрная высота поддерева уменьшилась на единицу - нужно сбалансировать
    if (originalColor == Color::Black && p != nullptr)
    {
        fixDeleting(p);
    }
}

void RedBlackTree::fixDeleting(Node* node)
{
    while (node != root && node->getColor() == Color::Black)
    {
        // Обрабатываем зеркальные случаи (относительного того, является ли узел левым сыном или правым)
        if (node == node->getParent()->getLeft())
        {
            Node* sibling = node->getParent()->getRight();
            // Случай, если брат текущего узла - красный, значит отец и племянники чёрные - делаем поворот и меняем цвета
            if (sibling->getColor() == Color::Red) 
            {
                sibling->setColor(Color::Black);
                node->getParent()->setColor(Color::Red);
                leftRotate(node->getParent());
                sibling = node->getParent()->getRight();
            }
            // После вращения у текущего узла чёрная высота на единицу меньше, проверяем является ли брат чёрным
            // Случай, когда оба племянника чёрные - красим брата в красный
            if ((sibling->getLeft() == nullptr
                || sibling->getLeft()->getColor() == Color::Black)
                && (sibling->getRight() == nullptr
                    || sibling->getRight()->getColor() == Color::Black))
            {
                sibling->setColor(Color::Red);
                node = node->getParent();
            }
            else {
                // Случай, если у красного брата правый ребёнок чёрный - перекрашиваем брата и его левого сына, делаем вращение
                if (sibling->getRight() == nullptr
                    || sibling->getRight()->getColor() == Color::Black) 
                {
                    if (sibling->getLeft() != nullptr)
                    {
                        sibling->getLeft()->setColor(Color::Black);
                    }
                    sibling->setColor(Color::Red);
                    rightRotate(sibling);
                    sibling = node->getParent()->getRight();
                }
                // Если у брата правый ребёнок красный - перекрашиваем брата в цвет отца, его ребёнка и отца — в чёрный, делаем вращение.
                sibling->setColor(node->getParent()->getColor());
                node->getParent()->setColor(Color::Black);
                if (sibling->getRight() != nullptr)
                {
                    sibling->getRight()->setColor(Color::Black);
                }
                leftRotate(node->getParent());
                node = root;
            }
        }
        // Зеркальный случай - меняем все вращения в другую сторону
        else {
            Node* sibling = node->getParent()->getLeft();
            if (sibling->getColor() == Color::Red) 
            {
                sibling->setColor(Color::Black);
                node->getParent()->setColor(Color::Red);
                rightRotate(node->getParent());
                sibling = node->getParent()->getLeft();
            }
            if ((sibling->getLeft() == nullptr
                || sibling->getLeft()->getColor() == Color::Black)
                && (sibling->getRight() == nullptr
                    || sibling->getRight()->getColor() == Color::Black))
            {
                sibling->setColor(Color::Red);
                node = node->getParent();
            }
            else {
                if (sibling->getLeft() == nullptr
                    || sibling->getLeft()->getColor() == Color::Black)
                {
                    if (sibling->getRight() != nullptr)
                    {
                        sibling->getRight()->setColor(Color::Black);
                    }
                    sibling->setColor(Color::Red);
                    leftRotate(sibling);
                    sibling = node->getParent()->getLeft();
                }
                sibling->setColor(node->getParent()->getColor());
                node->getParent()->setColor(Color::Black);
                if (sibling->getLeft() != nullptr)
                {
                    sibling->getLeft()->setColor(Color::Black);
                }
                rightRotate(node->getParent());
                node = root;
            }
        }
    }
    node->setColor(Color::Black);
}

void RedBlackTree::leftRotate(Node* t)
{
    if (t == nullptr) return;

    Node* child = t->getRight();
    if (child == nullptr) return; 

    t->setRight(child->getLeft());
    if (t->getRight() != nullptr)
    {
        t->getRight()->setParent(t);
    }

    child->setParent(t->getParent());

    if (t->getParent() == nullptr)
    {
        root = child;
    }
    else if (t == t->getParent()->getLeft())
    {
        t->getParent()->setLeft(child);
    }
    else
    {
        t->getParent()->setRight(child);
    }

    child->setLeft(t);
    t->setParent(child);
}

void RedBlackTree::rightRotate(Node* t)
{
    if (t == nullptr) return;

    Node* child = t->getLeft();
    if (child == nullptr) return; 

    t->setLeft(child->getRight());
    if (t->getLeft() != nullptr)
    {
        t->getLeft()->setParent(t);
    }

    child->setParent(t->getParent());

    if (t->getParent() == nullptr)
    {
        root = child;
    }
    else if (t == t->getParent()->getLeft())
    {
        t->getParent()->setLeft(child);
    }
    else
    {
        t->getParent()->setRight(child);
    }

    child->setRight(t);
    t->setParent(child);
}

void printNode(Node* node, bool isRight, std::string otstyp)
{
    if (node != nullptr)
    {
        std::cout << otstyp;
        otstyp += "   ";
        if (node->getParent() == nullptr)
        {
            std::cout << "Root: ";
        }
        else
        {
            if (isRight) {
                std::cout << "Right: ";
            }
            else {
                std::cout << "Left: ";
            }
        }
        std::string currColor = (node->getColor() == Color::Black) ? "Black" : "Red";
        std::cout << node->getKey() << "(" << currColor << ")" << std::endl;
        printNode(node->getLeft(), false, otstyp);
        printNode(node->getRight(), true, otstyp);
    }
}

void RedBlackTree::printTree()
{
    if (root == nullptr)
    {
        std::cout << "Tree doesn't have any nodes" << std::endl;
    }
    else
    {
        std::cout << "RedBlackTree:" << std::endl;
        printNode(root, true, "");
    }
}

bool RedBlackTree::search(int key)
{
    Node* p = root;
    
    // Если узел с значением key найден - выводим true, иначе дальше итерируемся вниз по узлам
    while (p != nullptr)
    {
        if (p->getKey() == key)
        {
            return true;
        }
        else if (p->getKey() < key)
        {
            p = p->getRight();
        }
        else
        {
            p = p->getLeft();
        }
    }

    return false;
}

void RedBlackTree::deleteTree(Node* node)
{
    if (node != nullptr)
    {
        deleteTree(node->getLeft());
        deleteTree(node->getRight());
        delete node;
    }
    root = nullptr;
}