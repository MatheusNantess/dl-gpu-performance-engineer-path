/*
Example 01 — Copy versus original

Complete, working program. Predict every printed value before running.

    g++ -std=c++17 -Wall -Wextra -g -O0 -o 01-copy-vs-original 01-copy-vs-original.cpp
    ./01-copy-vs-original

The whole of Module 02 is contained in the difference between the first
two functions.
*/

#include <iostream>

// Takes a COPY. Anything done here is invisible to the caller.
void by_value(int x) {
    std::cout << "  inside by_value, received: " << x << "\n";
    x = 999;
    std::cout << "  inside by_value, changed to: " << x << "\n";
}

// Takes an ADDRESS. Writing through it reaches the caller's variable.
void by_pointer(int* x) {
    std::cout << "  inside by_pointer, pointing at value: " << *x << "\n";
    *x = 999;
    std::cout << "  inside by_pointer, changed to: " << *x << "\n";
}

// Reads through the pointer but cannot write. Try uncommenting the
// assignment below and read the compiler error -- it is worth seeing.
void by_const_pointer(const int* x) {
    std::cout << "  inside by_const_pointer, value: " << *x << "\n";
    // *x = 999;   // error: assignment of read-only location
}

// Demonstrates that the POINTER ITSELF is still a copy. Reassigning it
// inside the function does not change the caller's pointer.
void reassign_pointer(int* x) {
    int local = 7;
    x = &local;         // only the local copy of the pointer changes
    std::cout << "  inside reassign_pointer, now pointing at: " << *x << "\n";
}

int main() {
    int a = 5;

    std::cout << "a = " << a << "\n";
    std::cout << "calling by_value(a)\n";
    by_value(a);
    std::cout << "after: a = " << a << "\n\n";

    std::cout << "calling by_pointer(&a)\n";
    by_pointer(&a);
    std::cout << "after: a = " << a << "\n\n";

    int b = 42;
    std::cout << "calling by_const_pointer(&b)\n";
    by_const_pointer(&b);
    std::cout << "after: b = " << b << "\n\n";

    int  c = 1;
    int* p = &c;
    std::cout << "p points at value " << *p << "\n";
    std::cout << "calling reassign_pointer(p)\n";
    reassign_pointer(p);
    std::cout << "after: p still points at value " << *p << "\n";

    // The last case is the subtle one. by_pointer changed what the pointer
    // POINTS TO. reassign_pointer changed the pointer itself -- and since
    // the pointer was passed by value, that change was local. To change
    // the caller's pointer you need a pointer to a pointer, or a reference.

    return 0;
}