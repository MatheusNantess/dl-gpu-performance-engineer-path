/*
Example 01 — The four meanings

Complete, working program. Before running, draw the memory picture on
paper and write down which lines print the SAME value as each other.

    g++ -std=c++17 -Wall -Wextra -g -O0 -o 01-four-meanings 01-four-meanings.cpp
    ./01-four-meanings

Run it twice. The addresses change; the relationships do not.
*/

#include <iostream>

int main() {
    int  x = 10;
    int* p = &x;

    std::cout << "--- the four meanings ---\n";
    std::cout << "x   (value of x)              = " << x  << "\n";
    std::cout << "&x  (address of x)            = " << &x << "\n";
    std::cout << "p   (value of p: an address)  = " << p  << "\n";
    std::cout << "*p  (value at that address)   = " << *p << "\n";
    std::cout << "&p  (address of p itself)     = " << &p << "\n";

    // Two pairs are equal:  x == *p   and   &x == p
    // &p is a different address entirely: the pointer is its own variable,
    // stored somewhere, occupying its own bytes.

    std::cout << "\n--- checks ---\n";
    std::cout << "x  == *p ?  " << (x  == *p ? "yes" : "no") << "\n";
    std::cout << "&x == p  ?  " << (&x == p  ? "yes" : "no") << "\n";

    std::cout << "\n--- writing through the pointer ---\n";
    *p = 20;
    std::cout << "after *p = 20, x is now " << x << "\n";
    // x was never on the left-hand side. The write reached it via the address.

    std::cout << "\n--- the pointer is a variable too ---\n";
    std::cout << "sizeof(x) = " << sizeof(x) << "\n";
    std::cout << "sizeof(p) = " << sizeof(p) << "\n";
    // 8 on a 64-bit machine, regardless of what it points to.

    int y = 99;
    p = &y;                  // repoint: change p, not *p
    std::cout << "\nafter p = &y:\n";
    std::cout << "  *p = " << *p << "   (now reading y)\n";
    std::cout << "  x  = " << x  << "   (unchanged)\n";

    std::cout << "\n--- pointer to pointer ---\n";
    int** pp = &p;
    std::cout << "pp   (address of p)   = " << pp   << "\n";
    std::cout << "*pp  (value of p)     = " << *pp  << "\n";
    std::cout << "**pp (value of y)     = " << **pp << "\n";

    // This is the shape of cudaMalloc(&d_data, bytes). To let a function
    // set the caller's POINTER, you pass the pointer's address.

    std::cout << "\n--- nullptr ---\n";
    int* n = nullptr;
    std::cout << "n = " << n << "\n";
    std::cout << "is n null? " << (n == nullptr ? "yes" : "no") << "\n";
    // Dereferencing n here would segfault. That is the GOOD failure mode:
    // an uninitialized pointer might instead write somewhere valid and
    // corrupt data silently.

    return 0;
}