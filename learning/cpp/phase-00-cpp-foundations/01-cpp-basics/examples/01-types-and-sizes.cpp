/*
Example 01 — Types and sizes

A complete, working program. Read it, predict every line of output,
THEN compile and run.

    g++ -std=c++17 -Wall -Wextra -g -O0 -o 01-types-and-sizes 01-types-and-sizes.cpp
    ./01-types-and-sizes

The numbers this prints are the byte counts every stride calculation in
every kernel you ever write will be based on.
*/

#include <iostream>
#include <climits>
#include <cfloat>

int main() {
    // sizeof is evaluated at COMPILE TIME. No work happens at run time.
    std::cout << "--- sizes in bytes ---\n";
    std::cout << "bool        " << sizeof(bool) << "\n";
    std::cout << "char        " << sizeof(char) << "\n";
    std::cout << "int         " << sizeof(int) << "\n";
    std::cout << "unsigned    " << sizeof(unsigned int) << "\n";
    std::cout << "long long   " << sizeof(long long) << "\n";
    std::cout << "float       " << sizeof(float) << "\n";
    std::cout << "double      " << sizeof(double) << "\n";
    std::cout << "size_t      " << sizeof(size_t) << "\n";
    std::cout << "int*        " << sizeof(int*) << "\n";
    std::cout << "double*     " << sizeof(double*) << "\n";

    // Note the last two: a pointer's size does not depend on what it
    // points to. A pointer is an address, and addresses are all the
    // same width on a given machine.

    std::cout << "\n--- limits ---\n";
    std::cout << "INT_MAX     " << INT_MAX << "\n";
    std::cout << "INT_MIN     " << INT_MIN << "\n";
    std::cout << "FLT_MAX     " << FLT_MAX << "\n";
    std::cout << "FLT_EPSILON " << FLT_EPSILON << "\n";

    // FLT_EPSILON is the smallest value that, added to 1.0f, produces
    // something different from 1.0f. It is the practical meaning of
    // "float has about 7 decimal digits of precision", and it is why
    // summing millions of floats loses accuracy -- a fact that comes
    // back as a real problem in reduction kernels.

    std::cout << "\n--- bandwidth consequence ---\n";
    const long long n = 10000000;   // 10 million elements
    std::cout << "10M floats  " << (n * sizeof(float))  / (1024 * 1024) << " MB\n";
    std::cout << "10M doubles " << (n * sizeof(double)) / (1024 * 1024) << " MB\n";

    // For a memory-bound kernel -- which is most deep learning kernels --
    // twice the bytes means roughly half the speed. The choice of float
    // over double is a performance decision, not a style one.

    return 0;
}