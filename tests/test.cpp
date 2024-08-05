#include <iostream>

using namespace std;

// Function to test
bool function1(int a) {
    return a > 5;   
}

// If parameter is not true, test fails
#define IS_TRUE(x) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; }

// Test for function1()
void test_function1()
{
    IS_TRUE(!function1(0));
    IS_TRUE(!function1(5));
    IS_TRUE(function1(10));
}

int main(void) {
    test_function1();
}