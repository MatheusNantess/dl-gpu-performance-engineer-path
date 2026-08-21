/*
Exercise 04 — Will this change outside?
Difficulty: MEDIUM

Goal:
Test the copy-versus-original model before the compiler settles it.

Requirements:
- Write FOUR functions, each attempting to set its argument to 100:
    void a(int x);
    void b(int* x);
    void c(const int* x);      // will not compile if it writes -- that is the point
    void d(int* x);            // this one reassigns the POINTER, not *x
- For each, write your prediction in the comment block below BEFORE compiling.
- Then run and compare.

Questions before coding:
1. Which of these changes the caller's variable?
2. For (d), what exactly gets modified?
3. How many bytes are copied into each function's frame?

Why this matters for CUDA:
Reading a kernel signature and knowing instantly what it can modify is the
readiness bar for this whole phase.
*/

#include <iostream>

int main() {
    /*
    MY PREDICTIONS (fill in before compiling):
    a: value after call =
    b: value after call =
    c: compiles? =
    d: value after call =
    */

    // TODO: implement
    return 0;
}