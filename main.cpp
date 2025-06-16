#include <windows.h>
#include "tests/test.hpp"
using namespace std;


int main(void) {
    SetConsoleOutputCP(65001);
    run_tests();
    return 0;
}
