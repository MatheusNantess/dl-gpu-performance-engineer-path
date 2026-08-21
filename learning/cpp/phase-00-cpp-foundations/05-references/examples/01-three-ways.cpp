/*
Example 01 — The three ways, side by side

Complete, working program. Predict all output before running.

    g++ -std=c++17 -Wall -Wextra -g -O0 -o 01-three-ways 01-three-ways.cpp
    ./01-three-ways
*/

#include <iostream>

void by_value(int x) {
    x = 99;
    std::cout << "    inside by_value:     x = " << x << " at " << &x << "\n";
}

void by_pointer(int* x) {
    *x = 99;
    std::cout << "    inside by_pointer:  *x = " << *x << " at " << x << "\n";
}

void by_reference(int& x) {
    x = 99;
    std::cout << "    inside by_reference: x = " << x << " at " << &x << "\n";
}

struct Big {
    double data[64];    // 512 bytes
};

void big_by_value(Big b)         { (void)b; }   // copies 512 bytes
void big_by_cref(const Big& b)   { (void)b; }   // copies nothing

int main() {
    std::cout << "--- three ways, one variable ---\n";

    int a = 1;
    std::cout << "a = " << a << " at " << &a << "\n";

    std::cout << "  by_value(a):\n";      by_value(a);
    std::cout << "  after: a = " << a << "\n";

    a = 1;
    std::cout << "  by_pointer(&a):\n";   by_pointer(&a);
    std::cout << "  after: a = " << a << "\n";

    a = 1;
    std::cout << "  by_reference(a):\n";  by_reference(a);
    std::cout << "  after: a = " << a << "\n";

    // Note the addresses printed inside each function. by_value operates on a
    // DIFFERENT address -- its own copy. The other two operate on a's address.
    //
    // Note also the call sites: by_pointer(&a) announces that something is
    // being passed by address. by_reference(a) looks exactly like by_value(a)
    // and behaves completely differently. That invisibility is the main
    // argument against references in code where mutation matters.

    std::cout << "\n--- a reference cannot be reseated ---\n";
    int x = 1, y = 2;
    int& r = x;
    std::cout << "before: x = " << x << ", y = " << y << "\n";
    r = y;              // assigns y's VALUE to x. Does NOT bind r to y.
    std::cout << "after r = y: x = " << x << ", y = " << y << "\n";
    std::cout << "&r == &x ? " << (&r == &x ? "yes" : "no") << "\n";
    std::cout << "&r == &y ? " << (&r == &y ? "yes" : "no") << "\n";

    // A pointer would have been repointed. A reference never is.

    std::cout << "\n--- copy cost ---\n";
    std::cout << "sizeof(Big) = " << sizeof(Big) << " bytes\n";
    Big b{};
    big_by_value(b);    // 512 bytes copied into the frame
    big_by_cref(b);     // nothing copied
    std::cout << "big_by_value copies " << sizeof(Big) << " bytes per call\n";
    std::cout << "big_by_cref  copies 0 bytes\n";

    // On a CPU this is 512 bytes. The same mistake on a GPU is a tensor
    // crossing the PCIe bus that did not need to, or an intermediate written
    // to global memory that could have stayed in registers. Same question,
    // six orders of magnitude more expensive.

    return 0;
}