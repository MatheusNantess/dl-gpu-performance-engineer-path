/*
Exercise 04 — Leaks and double frees
Difficulty: MEDIUM

Goal:
Create the classic heap bugs and detect them with tools.

Requirements:
- Write a function that allocates and never frees. Call it in a loop.
- Observe memory growth (watch with 	op or read /proc/self/status).
- Run under valgrind or -fsanitize=address and read the leak report.
- Then deliberately delete[] the same pointer twice. Observe.
- Then use memory after delete[]. Observe.
- Fix all three and re-run the tools to confirm clean.

Questions before coding:
1. Does the leaking program crash? Should it?
2. What does the sanitizer report for the double free, and how precise is it?
3. Why does setting the pointer to nullptr after delete help?

Why this matters for CUDA:
cudaMalloc without cudaFree leaks device memory, and the GPU has far less of it than the host. There is no garbage collector on either side.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}