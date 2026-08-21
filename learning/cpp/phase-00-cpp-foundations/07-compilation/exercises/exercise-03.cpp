/*
Exercise 03 — Warnings that are bugs
Difficulty: MEDIUM

Goal:
Learn that in C++, a warning is usually a defect report.

Requirements:
- This program COMPILES and produces wrong results.
- Predict its output before compiling. Write it down.
- Compile WITHOUT -Wall first. Note that it is silent.
- Compile with -Wall -Wextra. Read every warning.
- For each warning, explain the bug it is pointing at, then fix it.
- Confirm the fixed version produces the correct output.

Questions before coding:
1. Which loop never terminates, and why?
2. Which comparison is always true or always false?
3. Why is compiling without -Wall a bad default?

Why this matters for CUDA:
Sign-comparison and uninitialized-value warnings in index arithmetic are
exactly the bugs that produce out-of-bounds device writes.
*/

#include <iostream>

int main() {
    int data[5] = {10, 20, 30, 40, 50};
    int n = 5;

    // A
    for (unsigned int i = n - 1; i >= 0; --i) {
        std::cout << data[i] << " ";
        if (i > 100) break;   // safety valve so this terminates at all
    }
    std::cout << "\n";

    // B
    int sum;
    for (int i = 0; i < n; ++i) sum += data[i];
    std::cout << "sum = " << sum << "\n";

    // C
    unsigned int count = 3;
    int          offset = -5;
    if (offset < count) std::cout << "offset is smaller\n";
    else                std::cout << "offset is NOT smaller\n";

    // D
    int x = 5;
    if (x = 10) std::cout << "x is ten?\n";
    std::cout << "x = " << x << "\n";

    return 0;
}

/*
MY PREDICTIONS:
Output:
Warnings expected:

ACTUAL:

*/