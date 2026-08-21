/*
Exercise 01 — Syntax errors
Difficulty: EASY

Goal:
Read compiler messages and notice that the reported line is where the compiler
NOTICED the problem, not always where it is.

Requirements:
- There are several syntax errors below.
- Predict how many errors the compiler reports. Write it down.
- Compile. Fix ONLY the first error, then recompile. Repeat.
- Record each error message and whether the reported line was the real cause.

Questions before coding:
1. Does the number of reported errors match the number of mistakes? Why not?
2. Which error message points at the wrong line, and why does it?
3. Is any of this detectable without compiling?

Why this matters for CUDA:
nvcc error messages are longer and sometimes reference generated code. Reading
them starts with the habit of fixing one error at a time.
*/

#include <iostream>

int main() {
    int x = 5
    int y = 10;

    if (x < y) {
        std::cout << "x is smaller\n"
    }

    for (int i = 0; i < 3; ++i {
        std::cout << i << "\n";
    }

    return 0;
}

/*
MY PREDICTIONS:
Number of errors reported:
Category:

ACTUAL MESSAGES (paste verbatim):

*/