#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <functional>
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

        // Функции для работы со строками
        string SaveString1(Node<T> *node);
        string SaveString2(Node<T> *node);
        string SaveString3(Node<T> *node);
        string SaveString4(Node<T> *node);
        string SaveString5(Node<T> *node);
        string SaveString6(Node<T> *node);

        Node<T>* ReadString1(string format, int index);
        Node<T>* ReadString2(string format, int index);
        Node<T>* ReadString3(string format, int index);
        Node<T>* ReadString4(string format, int index);
        Node<T>* ReadString5(string format, int index);
        Node<T>* ReadString6(string format, int index);
    public:
        // Конструкторы
        BinaryTree();
        ~BinaryTree();

        // Обход
        Sequence<T>* Traversal(string type);

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

// Функции для работы со строками
template <typename T>
string BinaryTree<T>::SaveString1(Node<T> *node) {
    if (node) {
        string result = to_string(node->data) + " ";
        result += SaveString1(node->left);
        result += SaveString1(node->right);
        return result;
    }
    return "null ";
}

template <typename T>
string BinaryTree<T>::SaveString2(Node<T> *node) {
    if (node) {
        string result = to_string(node->data) + " ";
        result += SaveString2(node->right);
        result += SaveString2(node->left);
        return result;
    }
    return "null ";
}

template <typename T>
string BinaryTree<T>::SaveString3(Node<T> *node) {
    if (node) {
        string result = SaveString3(node->left);
        result += SaveString3(node->right);
        result += to_string(node->data) + " ";
        return result;
    }
    return "null ";
}

template <typename T>
string BinaryTree<T>::SaveString4(Node<T> *node) {
    if (node) {
        string result = SaveString4(node->left);
        result += to_string(node->data) + " ";
        result += SaveString4(node->right);
        return result;
    }
    return "null ";
}

template <typename T>
string BinaryTree<T>::SaveString5(Node<T> *node) {
    if (node) {
        string result = SaveString5(node->right);
        result += SaveString5(node->left);
        result += to_string(node->data) + " ";
        return result;
    }
    return "null ";
}

template <typename T>
string BinaryTree<T>::SaveString6(Node<T> *node) {
    if (node) {
        string result = SaveString6(node->right);
        result += to_string(node->data) + " ";
        result += SaveString6(node->left);
        return result;
    }
    return "null ";
}

// Конструкторы
template <typename T>
BinaryTree<T>::BinaryTree() : root(nullptr) {}

template <typename T>
BinaryTree<T>::~BinaryTree() {
    Clear(root);
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
    if (format.size() > 9) throw invalid_argument("Неверный формат!");
    string type;
    int i, flag = 0;
    char s;
    for (i = 0; i < format.size(); i++) {
        s = format[i];
        if (s == L'К' || s == L'Л' || s == L'П') type += s;
        else if (s != '{' && s != '[' && s != '(' &&
                 s != '}' && s != ']' && s != ')' || flag < 0) throw invalid_argument("Неверный формат!");
        else if (s == '{' || s == '[' || s == '(') flag++;
        else if (s == '}' || s == ']' || s == ')') flag--;
    }
    if (flag || type.size() != 3) throw invalid_argument("Неверный формат!");

    string result, temp;
    for (i = 0; i < format.size(); i++) {
        s = format[i];
        if (s == '{' || s == '[' || s == '(' || s == '}' || s == ']' || s == ')') result += s;
        else if (s == L'К') result += root ? " "+to_string(root->data)+" " : " null ";
        else if (s == L'Л' || s == L'П') {
            if (root) {
                Node<T> *node = s == L'Л' ? root->left : root->right;
                if (type == "КЛП") temp = SaveString1(node);
                else if (type == "КПЛ") temp = SaveString2(node);
                else if (type == "ЛПК") temp = SaveString3(node);
                else if (type == "ЛКП") temp = SaveString4(node);
                else if (type == "ПЛК") temp = SaveString5(node);
                else if (type == "ПКЛ") temp = SaveString6(node);
                else throw invalid_argument("Неверный формат!");
                result += " "+temp;
            } else result += " null ";
        }
    }
    return result;
}

template <typename T>
void BinaryTree<T>::ReadString(string format, string line) {
    // if (format.size() > 9) throw "Неправильный формат заданной строки!";
    // string type;
    // int flag = 0;
    // for (const char &s : format) {
    //     if (s == "К" || s == "Л" || s == "П") type += s;
    //     else if (s != "{" && s != "[" && s != "(" &&
    //              s != "}" && s != "]" && s != ")" || flag < 0) throw "Неправильный формат заданной строки!";
    //     else if (s == "{" || s == "[" || s == "(") flag++;
    //     else if (s == "}" || s == "]" || s == ")") flag--;
    // }
    // if (flag || type.size() != 3) throw "Неправильный формат заданной строки!";

    // Clear(root);
    // root = nullptr;
    // istringstream stream(line);
    // string token;
    // Sequence<T> *tokens;
    // while (stream >> token) {
    //     tokens->Append(token);
    // }
    // for (const char &s : format) {
    //     if (s == "К") {
    //         root = new Node<T>();
    //     }
    // }
}

#endif // BINARYTREE_HPP
