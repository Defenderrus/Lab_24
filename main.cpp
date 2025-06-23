// #include <chrono>
// #include <random>
// #include <functional>
#include <windows.h>
#include "tests/test.hpp"

// using namespace std;
// using namespace std::chrono;

// // Генератор случайных чисел
// mt19937 gen(random_device{}());
// uniform_int_distribution<int> dist(1, 1000000000);

// // Функция для измерения времени выполнения
// template<typename Func>
// long long measureTime(Func func) {
//     auto start = high_resolution_clock::now();
//     func();
//     auto stop = high_resolution_clock::now();
//     return duration_cast<milliseconds>(stop - start).count();
// }

// // Тестирование вставки элементов
// void testAdd(int size) {
//     BinaryTree<int> tree;
//     long long time = measureTime([&]() {
//         for (int i = 0; i < size; i++) tree.Add(dist(gen));
//     });
//     cout << "Add " << size << " elements: " << time << " ms" << endl;
// }

// // Тестирование поиска элементов
// void testFind(int size) {
//     BinaryTree<int> tree;
//     for (int i = 0; i < size; i++) tree.Add(dist(gen));
//     long long time = measureTime([&]() {
//         for (int i = 0; i < size; i++) {
//             tree.FindElement(dist(gen));
//         }
//     });
//     cout << "Find in " << size << " elements: " << time << " ms" << endl;
// }

// // Тестирование удаления элементов
// void testRemove(int size) {
//     BinaryTree<int> tree;
//     vector<int> elements;
//     for (int i = 0; i < size; i++) {
//         int val = dist(gen);
//         tree.Add(val);
//         elements.push_back(val);
//     }
//     long long time = measureTime([&]() {
//         for (int i = 0; i < size; i++) {
//             tree.Remove(elements[i]);
//         }
//     });
//     cout << "Remove from " << size << " elements: " << time << " ms" << endl;
// }

// // Тестирование обхода дерева
// void testTraversal(int size) {
//     if (size < 100000) {
//         BinaryTree<int> tree;
//         for (int i = 0; i < size; i++) tree.Add(dist(gen));
//         long long time = measureTime([&]() {
//             auto seq = tree.Traversal("ЛКП");
//             delete seq;
//         });
//         cout << "Traversal of " << size << " elements: " << time << " ms" << endl;
//     }
// }

// // Тестирование Map/Where/Reduce
// void testHigherOrder(int size) {
//     if (size < 100000) {
//         BinaryTree<int> tree;
//         for (int i = 0; i < size; i++) tree.Add(dist(gen));
        
//         // Map
//         long long mapTime = measureTime([&]() {
//             auto mapped = tree.Map<int>([](int x) { return x * 2; });
//         });
//         cout << "Map for " << size << " elements: " << mapTime << " ms" << endl;
        
//         // Where
//         long long whereTime = measureTime([&]() {
//             auto filtered = tree.Where([](int x) { return x % 2 == 0; });
//         });
//         cout << "Where for " << size << " elements: " << whereTime << " ms" << endl;
        
//         // Reduce
//         long long reduceTime = measureTime([&]() {
//             int sum = tree.Reduce([](int a, int b) { return a + b; }, 0);
//         });
//         cout << "Reduce for " << size << " elements: " << reduceTime << " ms" << endl;
//     }
// }

int main() {
    SetConsoleOutputCP(65001);
    run_tests();
    // vector<int> sizes = {1000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000};
    // for (int size : sizes) {
    //     cout << "Testing with size: " << size << endl;
    //     testAdd(size);
    //     testFind(size);
    //     testRemove(size);
    //     testTraversal(size);
    //     testHigherOrder(size);
    //     cout << "------------------------" << endl;
    // }
    return 0;
}
