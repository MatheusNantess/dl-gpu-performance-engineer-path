/*
Exercise 02 — Find the padding
Difficulty: EASY

Goal:
Locate padding precisely rather than inferring it.

Requirements:
- Define struct A { char x; int y; };
- Print sizeof(A), alignof(A), offsetof(A,x), offsetof(A,y).
- Compute the padding bytes from the offsets and confirm against sizeof.
- Repeat for three more structs with different field type mixes.
- Build a table in a comment: field, offset, size, padding after.

Questions before coding:
1. Where exactly are the padding bytes -- before y, or after it?
2. Is there trailing padding? How do the offsets prove it?
3. What would sizeof be if padding were removed entirely?

Why this matters for CUDA:
Padding bytes get moved across the memory bus and carry no information. For a memory-bound kernel that is pure waste.
*/

#include <iostream>
#include <cstddef>

int main() {
    // TODO: implement
    return 0;
}