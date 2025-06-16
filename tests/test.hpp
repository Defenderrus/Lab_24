#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include "../BinaryTree.hpp"
#include "unity.h"
using namespace std;


void setUp(void) {}

void tearDown(void) {}

// Добавление-удаление-поиск элементов, балансировка и прошивка
void test_base(void) {
    BinaryTree<int> tree;
    Sequence<int> *sequence;
    tree.Add(5);
    tree.Add(10);
    tree.Add(15);
    TEST_ASSERT_TRUE(tree.FindElement(10));
    TEST_ASSERT_TRUE(tree.FindElement(5));
    TEST_ASSERT_TRUE(tree.FindElement(15));
    try {tree.Add(10);}
    catch (const exception &e) {cout << e.what() << endl;}

    sequence = tree.Traversal("ЛКП");
    TEST_ASSERT_EQUAL(3, sequence->GetLength());
    TEST_ASSERT_EQUAL(5, sequence->GetFirst());
    TEST_ASSERT_EQUAL(10, sequence->Get(1));
    TEST_ASSERT_EQUAL(15, sequence->GetLast());
    try {sequence = tree.Traversal("Абракадабра");}
    catch (const exception &e) {cout << e.what() << endl;}
    delete sequence;

    tree.Remove(10);
    sequence = tree.Traversal("КЛП");
    TEST_ASSERT_FALSE(tree.FindElement(10));
    TEST_ASSERT_EQUAL(2, sequence->GetLength());
    TEST_ASSERT_EQUAL(15, sequence->GetFirst());
    TEST_ASSERT_EQUAL(5, sequence->GetLast());
    try {tree.Remove(1);}
    catch (const exception &e) {cout << e.what() << endl;}
    delete sequence;
}

// Поиск и извлечение поддерева
void test_subtree(void) {
    BinaryTree<int> tree;
    tree.Add(5);
    tree.Add(10);
    tree.Add(12);
    tree.Add(15);
    tree.Add(17);

    BinaryTree<int> subtree = tree.GetSubTree(15);
    Sequence<int> *sequence = subtree.Traversal("ЛКП");
    TEST_ASSERT_EQUAL(3, sequence->GetLength());
    TEST_ASSERT_TRUE(subtree.FindElement(12));
    TEST_ASSERT_TRUE(subtree.FindElement(15));
    TEST_ASSERT_TRUE(subtree.FindElement(17));
    TEST_ASSERT_TRUE(tree.FindSubTree(subtree));
    delete sequence;

    subtree.Remove(12);
    TEST_ASSERT_FALSE(tree.FindSubTree(subtree));
    try {subtree = tree.GetSubTree(1);}
    catch (const exception &e) {cout << e.what() << endl;}
}

// Слияние деревьев
void test_concat(void) {
    BinaryTree<int> tree1, tree2;
    Sequence<int> *sequence;
    tree1.Add(5);
    tree1.Add(10);
    tree1.Add(15);
    tree2.Add(12);
    tree2.Add(15);
    tree2.Add(17);

    tree1.Concat(tree2);
    sequence = tree1.Traversal("ЛКП");
    TEST_ASSERT_EQUAL(5, sequence->GetLength());
    TEST_ASSERT_EQUAL(5, sequence->GetFirst());
    TEST_ASSERT_EQUAL(10, sequence->Get(1));
    TEST_ASSERT_EQUAL(12, sequence->Get(2));
    TEST_ASSERT_EQUAL(15, sequence->Get(3));
    TEST_ASSERT_EQUAL(17, sequence->GetLast());
    delete sequence;
}

void test_map_reduce_where(void) {
    BinaryTree<int> tree;
    Sequence<int> *sequence;
    tree.Add(5);
    tree.Add(10);
    tree.Add(15);

    auto map = tree.Map<int>([](int x){return x + 1;});
    sequence = map.Traversal("ЛКП");
    TEST_ASSERT_EQUAL(6, sequence->GetFirst());
    TEST_ASSERT_EQUAL(11, sequence->Get(1));
    TEST_ASSERT_EQUAL(16, sequence->GetLast());
    delete sequence;

    auto where = tree.Where([](int x){return x % 2 == 1;});
    sequence = where.Traversal("ЛКП");
    TEST_ASSERT_EQUAL(2, sequence->GetLength());
    TEST_ASSERT_EQUAL(5, sequence->GetFirst());
    TEST_ASSERT_EQUAL(15, sequence->GetLast());
    delete sequence;

    int reduce = tree.Reduce([](int x, int y){return x + y;}, 0);
    TEST_ASSERT_EQUAL(30, reduce);
}

int run_tests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_base);
    RUN_TEST(test_subtree);
    RUN_TEST(test_concat);
    RUN_TEST(test_map_reduce_where);
    return UNITY_END();
}

#endif // TEST_HPP
