#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <functional>
#include <sstream>
#include <iostream>
#include "sequences/ArraySequence.hpp"
#include "sequences/ListSequence.hpp"
using namespace std;


template <typename T>
class Node {
    public:
        T data;
        int height;
        Node *left;
        Node *right;
        Node(T value) {
            data = value;
            height = 1;
            left = nullptr;
            right = nullptr;
        }
};

template <typename T>
class BinaryTree {
    private:
        friend class Node<T>;
        Node<T> *root;

        // Функции для конструкторов
        void Clear(Node<T> *node);

        // Функции для обхода
        void RootLeftRight(Node<T> *node, Sequence<T> *sequence);
        void RootRightLeft(Node<T> *node, Sequence<T> *sequence);
        void LeftRightRoot(Node<T> *node, Sequence<T> *sequence);
        void LeftRootRight(Node<T> *node, Sequence<T> *sequence);
        void RightLeftRoot(Node<T> *node, Sequence<T> *sequence);
        void RightRootLeft(Node<T> *node, Sequence<T> *sequence);

        // Функции для операций
        Node<T>* Add(Node<T> *node, T value);
        Node<T>* Remove(Node<T> *node, T value);
        Node<T>* Min(Node<T> *node);
        Node<T>* FindElement(Node<T> *node, T value);
        bool FindSubTree(Node<T> *tree, Node<T> *subtree);
        bool Equivalent(Node<T> *tree_1, Node<T> *tree_2);
        Node<T>* GetSubTree(Node<T> *node);

        // Функции для балансировки
        int GetHeight(Node<T> *node);
        int GetBalance(Node<T> *node);
        void SetHeight(Node<T> *node);
        Node<T>* RotateLeft(Node<T> *node);
        Node<T>* RotateRight(Node<T> *node);
        Node<T>* Balance(Node<T> *node);

        // Работа со строками
        string SaveString(Node<T> *node, string format, size_t index);
        Node<T>* ReadString(string line, size_t &lineIndex, string format, size_t formatIndex);
        void search(string line, size_t &index, string &token);
        T parse(string token);
    public:
        // Конструкторы
        BinaryTree();
        ~BinaryTree();
        BinaryTree(BinaryTree<T> &&other);

        // Обход
        Sequence<T>* Traversal(string type);

        // Перегрузка операторов
        BinaryTree<T>& operator=(BinaryTree<T> &&other);

        // Операции
        void Add(T value);
        void Remove(T value);
        bool FindElement(T value);
        bool FindSubTree(BinaryTree<T> &subtree);
        BinaryTree<T> GetSubTree(T value);
        BinaryTree<T>* Concat(BinaryTree<T> &other);
        Node<T>* GetRoot();

        // Map-Where-Reduce
        template<typename U>
        BinaryTree<U> Map(function<U(T)> func);
        BinaryTree<T> Where(function<bool(T)> func);
        T Reduce(function<T(T,T)> func, T start);

        // Работа со строками
        string SaveString(string format);
        void ReadString(string format, string line);
};

// Функции для конструкторов
template <typename T>
void BinaryTree<T>::Clear(Node<T> *node) {
    if (node) {
        Clear(node->left);
        Clear(node->right);
        delete node;
    }
}

// Функции для обхода
template <typename T>
void BinaryTree<T>::RootLeftRight(Node<T> *node, Sequence<T> *sequence) {
    if (node) {
        sequence->Append(node->data);
        RootLeftRight(node->left, sequence);
        RootLeftRight(node->right, sequence);
    }
}

template <typename T>
void BinaryTree<T>::RootRightLeft(Node<T> *node, Sequence<T> *sequence) {
    if (node) {
        sequence->Append(node->data);
        RootRightLeft(node->right, sequence);
        RootRightLeft(node->left, sequence);
    }
}

template <typename T>
void BinaryTree<T>::LeftRightRoot(Node<T> *node, Sequence<T> *sequence) {
    if (node) {
        LeftRightRoot(node->left, sequence);
        LeftRightRoot(node->right, sequence);
        sequence->Append(node->data);
    }
}

template <typename T>
void BinaryTree<T>::LeftRootRight(Node<T> *node, Sequence<T> *sequence) {
    if (node) {
        LeftRootRight(node->left, sequence);
        sequence->Append(node->data);
        LeftRootRight(node->right, sequence);
    }
}

template <typename T>
void BinaryTree<T>::RightLeftRoot(Node<T> *node, Sequence<T> *sequence) {
    if (node) {
        RightLeftRoot(node->right, sequence);
        RightLeftRoot(node->left, sequence);
        sequence->Append(node->data);
    }
}

template <typename T>
void BinaryTree<T>::RightRootLeft(Node<T> *node, Sequence<T> *sequence) {
    if (node) {
        RightRootLeft(node->right, sequence);
        sequence->Append(node->data);
        RightRootLeft(node->left, sequence);
    }
}

// Функции для операций
template <typename T>
Node<T>* BinaryTree<T>::Add(Node<T> *node, T value) {
    if (!node) return new Node<T>(value);
    else if (value < node->data) node->left = Add(node->left, value);
    else if (value > node->data) node->right = Add(node->right, value);
    else throw invalid_argument("Такой элемент уже есть ("+to_string(value)+")!");
    return Balance(node);
}

template <typename T>
Node<T>* BinaryTree<T>::Remove(Node<T> *node, T value) {
    if (!node) throw out_of_range("Такого элемента нет ("+to_string(value)+")!");
    else if (value < node->data) node->left = Remove(node->left, value);
    else if (value > node->data) node->right = Remove(node->right, value);
    else {
        if (!node->left || !node->right) {
            Node<T> *newNode = node->left ? node->left : node->right;
            if (!newNode) return nullptr;
            else *node = *newNode;
            delete newNode;
        } else {
            Node<T> *newNode = Min(node->right);
            node->data = newNode->data;
            node->right = Remove(node->right, newNode->data);
        }
    }
    return Balance(node);
}

template <typename T>
Node<T>* BinaryTree<T>::Min(Node<T> *node) {
    while (node && node->left) node = node->left;
    return node;
}

template <typename T>
Node<T>* BinaryTree<T>::FindElement(Node<T> *node, T value) {
    if (!node) return nullptr;
    if (value < node->data) return FindElement(node->left, value);
    if (value > node->data) return FindElement(node->right, value);
    return node;
}

template <typename T>
bool BinaryTree<T>::FindSubTree(Node<T> *tree, Node<T> *subtree) {
    if (!subtree) return true;
    if (!tree) return false;
    if (Equivalent(tree, subtree)) return true;
    return FindSubTree(tree->left, subtree) || FindSubTree(tree->right, subtree);
}

template <typename T>
bool BinaryTree<T>::Equivalent(Node<T> *tree_1, Node<T> *tree_2) {
    if (!tree_1 && !tree_2) return true;
    if (!tree_1 || !tree_2) return false;
    return tree_1->data == tree_2->data && Equivalent(tree_1->left, tree_2->left) && Equivalent(tree_1->right, tree_2->right);
}

template <typename T>
Node<T>* BinaryTree<T>::GetSubTree(Node<T> *node) {
    if (node) {
        Node<T> *newNode = new Node<T>(node->data);
        newNode->left = GetSubTree(node->left);
        newNode->right = GetSubTree(node->right);
        return newNode;
    }
    return nullptr;
}

// Функции для балансировки
template <typename T>
int BinaryTree<T>::GetHeight(Node<T> *node) {
    if (node) return node->height;
    return 0;
}

template <typename T>
int BinaryTree<T>::GetBalance(Node<T> *node) {
    if (node) return GetHeight(node->left)-GetHeight(node->right);
    return 0;
}

template <typename T>
void BinaryTree<T>::SetHeight(Node<T> *node) {
    if (node) node->height = 1+max(GetHeight(node->left), GetHeight(node->right));
}

template <typename T>
Node<T>* BinaryTree<T>::RotateLeft(Node<T> *node) {
    Node<T> *right = node->right;
    Node<T> *left = right->left;
    right->left = node;
    node->right = left;
    SetHeight(node);
    SetHeight(right);
    return right;
}

template <typename T>
Node<T>* BinaryTree<T>::RotateRight(Node<T> *node) {
    Node<T> *left = node->left;
    Node<T> *right = left->right;
    left->right = node;
    node->left = right;
    SetHeight(node);
    SetHeight(left);
    return left;
}

template <typename T>
Node<T>* BinaryTree<T>::Balance(Node<T> *node) {
    if (!node) return nullptr;
    SetHeight(node);
    int current = GetBalance(node);
    if (current > 1) {
        if (GetBalance(node->left) < 0) {
            node->left = RotateLeft(node->left);
        }
        return RotateRight(node);
    } else if (current < -1) {
        if (GetBalance(node->right) > 0) {
            node->right = RotateRight(node->right);
        }
        return RotateLeft(node);
    }
    return node;
}

// Работа со строками
template <typename T>
string BinaryTree<T>::SaveString(Node<T> *node, string format, size_t index) {
    if (!node) return " null ";
    string result;
    while (index < format.size()) {
        char s = format[index++];
        if (s == '{' || s == '[' || s == '(' || s == '}' || s == ']' || s == ')') {
            result += s;
        } else if (s == 'K' || s == 'L' || s == 'P') {
            if (s == 'K') {
                result += node ? " "+to_string(node->data)+" " : " null ";
            } else if (s == 'L') {
                if (node) result += SaveString(node->left, format, 0);
            } else if (s == 'P') {
                if (node) result += SaveString(node->right, format, 0);
            }
        }
    }
    return result;
}

template <typename T>
Node<T>* BinaryTree<T>::ReadString(string line, size_t &lineIndex, string format, size_t formatIndex) {
    Node<T> *node = nullptr;
    string token;
    while (lineIndex < line.size() && formatIndex < format.size()) {
        char s = format[formatIndex++];
        if (s == '{' || s == '[' || s == '(' || s == '}' || s == ']' || s == ')') {
            while (lineIndex < line.size() && isspace(line[lineIndex])) lineIndex++;
            if (lineIndex >= line.size() || s != line[lineIndex]) {
                throw invalid_argument("Ожидалась скобка '"+string(1, s)+
                                       "', но найдено '"+(lineIndex < line.size() ? string(1, line[lineIndex]) : "конец строки") + "'");
            }
            lineIndex++;
        } else if (s == 'K' || s == 'L' || s == 'P') {
            if (s == 'K') {
                search(line, lineIndex, token);
                if (token != "null") {
                    T value;
                    try {value = parse(token);}
                    catch (...) {throw invalid_argument("Неверный тип значения: "+token+"!");}
                    if (!node) node = new Node<T>(value);
                    else node->data = value;
                }
                else return nullptr;
            } else if (s == 'L') {
                size_t checkIndex = lineIndex;
                search(line, checkIndex, token);
                if (!node) node = new Node<T>(parse("0"));
                if (token == "null") {
                    node->left = nullptr;
                    lineIndex = checkIndex;
                } else node->left = ReadString(line, lineIndex, format, 0);
            } else if (s == 'P') {
                size_t checkIndex = lineIndex;
                search(line, checkIndex, token);
                if (!node) node = new Node<T>(parse("0"));
                if (token == "null") {
                    node->right = nullptr;
                    lineIndex = checkIndex;
                } else node->right = ReadString(line, lineIndex, format, 0);
            }
        }
    }
    return node;
}

template <typename T>
void BinaryTree<T>::search(string line, size_t &index, string &token) {
    while (index < line.size() && isspace(line[index])) index++;
    token.clear();
    while (index < line.size() && !isspace(line[index]) && 
        line[index] != '{' && line[index] != '}' &&
        line[index] != '[' && line[index] != ']' &&
        line[index] != '(' && line[index] != ')') {
        token += line[index++];
    }
}

template <typename T>
T BinaryTree<T>::parse(string token) {
    istringstream iss(token);
    T value;
    iss >> value;
    return value;
}

// Конструкторы
template <typename T>
BinaryTree<T>::BinaryTree() : root(nullptr) {}

template <typename T>
BinaryTree<T>::~BinaryTree() {
    Clear(root);
}

template <typename T>
BinaryTree<T>::BinaryTree(BinaryTree<T> &&other) {
    root = other.root;
    other.root = nullptr;
}

// Обход
template <typename T>
Sequence<T>* BinaryTree<T>::Traversal(string type) {
    Sequence<T> *sequence = new ArraySequence<T>();
    if (type == "КЛП") RootLeftRight(root, sequence);
    else if (type == "КПЛ") RootRightLeft(root, sequence);
    else if (type == "ЛПК") LeftRightRoot(root, sequence);
    else if (type == "ЛКП") LeftRootRight(root, sequence);
    else if (type == "ПЛК") RightLeftRoot(root, sequence);
    else if (type == "ПКЛ") RightRootLeft(root, sequence);
    else throw invalid_argument("Неизвестный обход ("+type+")!");
    return sequence;
}

// Перегрузка операторов
template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(BinaryTree<T> &&other) {
    if (this != &other) {
        Clear(root);
        root = other.root;
        other.root = nullptr;
    }
    return *this;
}

// Операции
template <typename T>
void BinaryTree<T>::Add(T value) {
    root = Add(root, value);
}

template <typename T>
void BinaryTree<T>::Remove(T value) {
    root = Remove(root, value);
}

template <typename T>
bool BinaryTree<T>::FindElement(T value) {
    if (FindElement(root, value)) return true;
    return false;
}

template <typename T>
bool BinaryTree<T>::FindSubTree(BinaryTree<T> &subtree) {
    return FindSubTree(root, subtree.root);
}

template <typename T>
BinaryTree<T> BinaryTree<T>::GetSubTree(T value) {
    BinaryTree<T> subTree;
    Node<T> *subRoot = FindElement(root, value);
    if (subRoot) subTree.root = GetSubTree(subRoot);
    else throw invalid_argument("Такого элемента нет ("+to_string(value)+")!");
    return subTree;
}

template <typename T>
BinaryTree<T>* BinaryTree<T>::Concat(BinaryTree<T> &other) {
    Sequence<T> *sequence = other.Traversal("КЛП");
    for (int i = 0; i < sequence->GetLength(); i++) {
        if (!FindElement(sequence->Get(i))) Add(sequence->Get(i));
    }
    return this;
}

template <typename T>
Node<T>* BinaryTree<T>::GetRoot() {
    return root;
}

// Map-Where-Reduce
template <typename T>
template <typename U>
BinaryTree<U> BinaryTree<T>::Map(function<U(T)> func) {
    BinaryTree<U> newTree;
    Sequence<T> *sequence = Traversal("КЛП");
    for (int i = 0; i < sequence->GetLength(); i++) {
        newTree.Add(func(sequence->Get(i)));
    }
    return newTree;
}

template <typename T>
BinaryTree<T> BinaryTree<T>::Where(function<bool(T)> func) {
    BinaryTree<T> newTree;
    Sequence<T> *sequence = Traversal("КЛП");
    for (int i = 0; i < sequence->GetLength(); i++) {
        if (func(sequence->Get(i))) newTree.Add(sequence->Get(i));
    }
    return newTree;
}

template <typename T>
T BinaryTree<T>::Reduce(function<T(T, T)> func, T start) {
    Sequence<T> *sequence = Traversal("КЛП");
    for (int i = 0; i < sequence->GetLength(); i++) {
        start = func(start, sequence->Get(i));
    }
    return start;
}

// Работа со строками
template <typename T>
string BinaryTree<T>::SaveString(string format) {
    if (format.size() != 7 && format.size() != 9) throw invalid_argument("Неверный формат!");
    bool k = false, l = false, p = false;
    string type;
    int flag = 0;
    for (char s : format) {
        if (s == 'K' || s == 'L' || s == 'P') {
            type += s;
            if (s == 'K') k = true;
            if (s == 'L') l = true;
            if (s == 'P') p = true;
        }
        else if (s == '{' || s == '[' || s == '(') flag++;
        else if (s == '}' || s == ']' || s == ')') {
            if (--flag != 0) throw invalid_argument("Несбалансированные скобки в формате!");
        } else throw invalid_argument("Недопустимый символ в формате!");
    }
    if (flag) throw invalid_argument("Несбалансированные скобки в формате!");
    if (type.size() != 3 || !k || !l || !p) throw invalid_argument("Формат должен содержать K, L и P ровно по одному разу!");
    return SaveString(root, format, 0);
}

template <typename T>
void BinaryTree<T>::ReadString(string format, string line) {
    if (format.size() != 7 && format.size() != 9) throw invalid_argument("Неверный формат!");
    bool k = false, l = false, p = false;
    string type;
    int flag = 0;
    for (char s : format) {
        if (s == 'K' || s == 'L' || s == 'P') {
            type += s;
            if (s == 'K') k = true;
            if (s == 'L') l = true;
            if (s == 'P') p = true;
        }
        else if (s == '{' || s == '[' || s == '(') flag++;
        else if (s == '}' || s == ']' || s == ')') {
            if (--flag != 0) throw invalid_argument("Несбалансированные скобки в формате!");
        } else throw invalid_argument("Недопустимый символ в формате!");
    }
    if (flag) throw invalid_argument("Несбалансированные скобки в формате!");
    if (type.size() != 3 || !k || !l || !p) throw invalid_argument("Формат должен содержать K, L и P ровно по одному разу!");
    Clear(root);
    size_t lineIndex = 0;
    try {
        root = ReadString(line, lineIndex, format, 0);
    } catch (const exception& e) {
        Clear(root);
        root = nullptr;
        throw;
    }
}

#endif // BINARYTREE_HPP
