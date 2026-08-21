/*
Exercise 03 — Three signatures
Difficulty: MEDIUM

Goal:
The core comparison of this module.

Requirements:
- Write three functions that each try to set their argument to 100:
    void a(int x);  void b(int* x);  void c(int& x);
- Predict in writing which change the caller.
- Call all three on the same variable, resetting between calls.
- Inside each, print the address being operated on.
- Compare the call sites and note which announce mutation.

Questions before coding:
1. Which addresses match the caller's variable?
2. Which call sites tell a reader that mutation may happen?
3. How many bytes are copied into each function's frame?

Why this matters for CUDA:
This is the copy-or-original question that scales all the way up to whether a tensor crosses the PCIe bus.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}