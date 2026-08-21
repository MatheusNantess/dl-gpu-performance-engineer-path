/*
Exercise 02 — Type errors
Difficulty: EASY

Goal:
See what static typing rejects, and what it silently allows.

Requirements:
- Several lines below are type errors; some are only warnings.
- Predict, for EACH marked line, whether it is an error, a warning, or accepted
  silently. Write the predictions down first.
- Compile with -Wall -Wextra and compare.
- Fix the errors. For each warning, decide whether it is a real bug.

Questions before coding:
1. Which of these would Python accept and fail on only at run time?
2. Which lines lose data without complaint?
3. Why is a narrowing conversion an error with braces and not with =?

Why this matters for CUDA:
Mixing float, double and half precision in kernels produces exactly these
conversions, and silent ones change numerical results.
*/

#include <iostream>
#include <string>

int main() {
    int         a = "hello";        // A
    std::string s = 42;             // B
    int         b = 3.99;           // C
    int         c{3.99};            // D
    unsigned    d = -1;             // E
    float       e = 1e50;           // F

    int  n = 5;
    bool flag = n;                  // G

    std::cout << a << b << c << d << e << flag << s << "\n";
    return 0;
}

/*
MY PREDICTIONS (error / warning / silent):
A:   B:   C:   D:   E:   F:   G:

ACTUAL:

*/