/*
Exercise 06 — Run-time errors
Difficulty: MEDIUM

Goal:
Distinguish failures that the build cannot catch.

Requirements:
- This file compiles cleanly, even with -Wall -Wextra. It still has bugs.
- Predict which lines fail at run time, and how: crash, wrong value, or
  silently wrong. Write the predictions down.
- Run it. Then run under -fsanitize=address and -fsanitize=undefined.
- Record what each tool caught that the compiler did not.
- Fix everything and confirm both sanitizers are clean.

Questions before coding:
1. Which failures crash and which are silent? Which is more dangerous?
2. Why can the compiler not catch these?
3. What does each sanitizer catch that the other does not?

Why this matters for CUDA:
Device-side equivalents are worse: errors are asynchronous, surfacing at the
next CUDA call, and no sanitizer runs by default.
*/

#include <iostream>

int main() {
    int  data[5] = {1, 2, 3, 4, 5};

    // A: reads past the end
    std::cout << "A: " << data[7] << "\n";

    // B: dereferences null
    int* p = nullptr;
    // std::cout << "B: " << *p << "\n";     // uncomment when ready

    // C: use after free
    int* h = new int[3]{1, 2, 3};
    delete[] h;
    std::cout << "C: " << h[0] << "\n";

    // D: division by zero
    int n = 0;
    // std::cout << "D: " << 10 / n << "\n"; // uncomment when ready

    // E: leak
    int* leaked = new int[100];
    (void)leaked;

    return 0;
}

/*
MY PREDICTIONS (crash / wrong value / silent):
A:   B:   C:   D:   E:

ACTUAL, and what each sanitizer reported:

*/