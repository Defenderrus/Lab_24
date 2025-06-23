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

void test_string(void) {
    BinaryTree<int> tree;
    tree.Add(5);
    tree.Add(10);
    tree.Add(15);
    string result1 = tree.SaveString("{K}(L)[P]");
    string result2 = tree.SaveString("(L)K[P]");
    string result3 = tree.SaveString("(L)[P]K");
    string correct1 = "{ 10 }({ 5 }( null )[ null ])[{ 15 }( null )[ null ]]";
    string correct2 = "(( null ) 5 [ null ]) 10 [( null ) 15 [ null ]]";
    string correct3 = "(( null )[ null ] 5 )[( null )[ null ] 15 ] 10 ";
    TEST_ASSERT_EQUAL_STRING(correct1.c_str(), result1.c_str());
    TEST_ASSERT_EQUAL_STRING(correct2.c_str(), result2.c_str());
    TEST_ASSERT_EQUAL_STRING(correct3.c_str(), result3.c_str());

    BinaryTree<int> tree1;
    BinaryTree<int> tree2;
    BinaryTree<int> tree3;
    tree1.ReadString("{K}(L)[P]", result1);
    tree2.ReadString("(L)K[P]", result2);
    tree3.ReadString("(L)[P]K", result3);
    string result11 = tree1.SaveString("{K}(L)[P]");
    string result22 = tree2.SaveString("(L)K[P]");
    string result33 = tree3.SaveString("(L)[P]K");
    TEST_ASSERT_EQUAL_STRING(correct1.c_str(), result11.c_str());
    TEST_ASSERT_EQUAL_STRING(correct2.c_str(), result22.c_str());
    TEST_ASSERT_EQUAL_STRING(correct3.c_str(), result33.c_str());

    try {
        BinaryTree<int> treeError;
        string error = treeError.SaveString("(L[K)P]");
    } catch (const exception &e) {cout << e.what() << endl;}
    try {
        BinaryTree<int> treeError;
        string error = treeError.SaveString("K(L)[K]");
    } catch (const exception &e) {cout << e.what() << endl;}
    try {
        BinaryTree<int> treeError;
        string error = treeError.SaveString("K(L)[P]gg");
    } catch (const exception &e) {cout << e.what() << endl;}
}

void test_classes(void) {
    BinaryTree<Complex> complexTree;
    complexTree.Add(Complex(1.0, 2.0));
    complexTree.Add(Complex(3.5, -1.2));
    complexTree.Add(Complex(0.0, 4.2));

    Sequence<Complex> *sequence1 = complexTree.Traversal("ЛКП");
    cout << "Первое комплексное число: " << sequence1->GetFirst() << endl;
    cout << "Второе комплексное число: " << sequence1->Get(1) << endl;
    cout << "Третье комплексное число: " << sequence1->GetLast() << endl;

    BinaryTree<Student> studentTree;
    time_t studentBirth = time(nullptr);
    studentTree.Add(Student(PersonID(1111, 111111), "Иван", "Петрович", "Сидоров", studentBirth, 0001, "Б24-521", 4.5));
    studentTree.Add(Student(PersonID(2222, 222222), "Николай", "Олегович", "Кузнецов", studentBirth, 0002, "Б24-521", 4.8));
    studentTree.Add(Student(PersonID(3333, 333333), "Александр", "Сергеевич", "Смирнов", studentBirth, 0003, "Б24-521", 4.2));

    Sequence<Student> *sequence2 = studentTree.Traversal("ЛКП");
    cout << sequence2->GetFirst() << endl;
    cout << sequence2->Get(1) << endl;
    cout << sequence2->GetLast() << endl;
    
    BinaryTree<Teacher> teacherTree;
    time_t teacherBirth = time(nullptr);
    teacherTree.Add(Teacher(PersonID(4444, 444444), "Иван", "Петрович", "Сидоров", teacherBirth, "Математика", "Заведующий кафедры", 15));
    teacherTree.Add(Teacher(PersonID(5555, 555555), "Николай", "Олегович", "Кузнецов", teacherBirth, "Информатика", "Преподаватель", 7));
    teacherTree.Add(Teacher(PersonID(6666, 666666), "Александр", "Сергеевич", "Смирнов", teacherBirth, "Физика", "Преподаватель", 8));

    Sequence<Teacher> *sequence3 = teacherTree.Traversal("ЛКП");
    cout << sequence3->GetFirst() << endl;
    cout << sequence3->Get(1) << endl;
    cout << sequence3->GetLast() << endl;
}

int run_tests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_base);
    RUN_TEST(test_subtree);
    RUN_TEST(test_concat);
    RUN_TEST(test_map_reduce_where);
    RUN_TEST(test_string);
    RUN_TEST(test_classes);
    return UNITY_END();
}

#endif // TEST_HPP
