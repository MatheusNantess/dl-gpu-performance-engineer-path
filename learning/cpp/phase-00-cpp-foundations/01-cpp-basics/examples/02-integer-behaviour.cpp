/*
Example 02 — Integer behaviour that surprises Python programmers

A complete, working program. Predict each line before running.

    g++ -std=c++17 -Wall -Wextra -g -O0 -o 02-integer-behaviour 02-integer-behaviour.cpp
    ./02-integer-behaviour

Compile it a second time WITHOUT -Wall -Wextra and notice how much
the compiler stops telling you.
*/

#include <iostream>
#include <climits>

int main() {
    std::cout << "--- integer division ---\n";
    int a = 7, b = 2;
    std::cout << "7 / 2            = " << a / b << "\n";
    std::cout << "7 % 2            = " << a % b << "\n";
    std::cout << "(double)7 / 2    = " << static_cast<double>(a) / b << "\n";
    std::cout << "7 / (double)2    = " << a / static_cast<double>(b) << "\n";

    // The cast must happen BEFORE the division. Writing
    //     double wrong = a / b;
    // performs integer division first and then converts 3 to 3.0.

    std::cout << "\n--- overflow ---\n";
    int big = INT_MAX;
    std::cout << "INT_MAX          = " << big << "\n";
    std::cout << "INT_MAX + 1      = " << big + 1 << "\n";

    // In Python this would grow to a larger integer. Here it wraps.
    // Signed overflow is undefined behaviour: the compiler is allowed
    // to assume it never happens, so at -O2 this may behave differently.

    std::cout << "\n--- unsigned wraparound ---\n";
    unsigned int u = 0;
    std::cout << "0u - 1           = " << u - 1 << "\n";

    // Unsigned arithmetic wraps in a DEFINED way, which makes it
    // predictable and no less dangerous. A loop written
    //     for (unsigned i = n - 1; i >= 0; --i)
    // never terminates, because an unsigned value is always >= 0.

    std::cout << "\n--- mixing signed and unsigned ---\n";
    int          s = -1;
    unsigned int t = 1;
    std::cout << "(-1 < 1u) is     " << (s < t ? "true" : "false") << "\n";

    // The signed value is converted to unsigned before comparison,
    // so -1 becomes a very large number. -Wall warns about this.
    // Index arithmetic mixing int and size_t hits it constantly.

    std::cout << "\n--- float precision ---\n";
    float f = 0.1f;
    float sum = 0.0f;
    for (int i = 0; i < 10; ++i) { sum += f; }
    std::cout << "0.1f added 10x   = " << sum << "\n";
    std::cout << "equals 1.0f?     " << (sum == 1.0f ? "yes" : "no") << "\n";

    // 0.1 is not representable exactly in binary floating point.
    // Error accumulates across the additions. Ten iterations is
    // harmless; a reduction over ten million elements is not, and
    // that is why summation order and accumulation precision are
    // real topics in kernel engineering.

    return 0;
}