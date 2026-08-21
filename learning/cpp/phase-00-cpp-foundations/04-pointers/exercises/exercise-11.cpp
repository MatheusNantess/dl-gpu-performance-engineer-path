/*
Exercise 11 — Predict: pointers and functions
Difficulty: MEDIUM

Goal:
Combine Module 02 with pointers.

Requirements:
- Predict, in writing, what each of the following does to the caller's data:
    void f1(int x)         { x = 99; }
    void f2(int* x)        { *x = 99; }
    void f3(int* x)        { x = nullptr; }
    void f4(int** x)       { *x = nullptr; }
    void f5(int* a, int n) { for (int i=0;i<n;++i) a[i] = 99; }
- Then implement and verify each.

Questions before coding:
1. Which ones change the caller's value? Which change the caller's pointer?
2. Why does f3 fail to do what it looks like it does?
3. What does f4 demonstrate that f3 does not?

Why this matters for CUDA:
f4 is cudaMalloc. If f3 versus f4 is clear, that API stops being mysterious.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}