/*
Exercise 09 — Predict the output II
Difficulty: MEDIUM

Goal:
Precedence and pointer modification.

Requirements:
- Predict every result before compiling, in writing.
- Then verify.

Given: int a[4] = {1,2,3,4}; int* p = a;

  A: *p++          then print p - a
  B: (*p)++        then print a[0] and a[1]
  C: ++*p
  D: *++p

- Reset the array between cases so each starts clean.

Questions before coding:
1. Which of these modify the array, and which modify the pointer?
2. What is the precedence of ++ versus * in each case?
3. Which would you actually write in real code, and which would you parenthesize?

Why this matters for CUDA:
This notation appears in real CUDA code. Misreading it produces off-by-one bugs in index arithmetic.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}