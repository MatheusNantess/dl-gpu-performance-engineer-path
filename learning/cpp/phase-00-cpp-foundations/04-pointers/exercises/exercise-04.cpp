/*
Exercise 04 — Pointer to array
Difficulty: EASY

Goal:
Confirm that an array name is an address.

Requirements:
- Declare an int array and set a pointer to it without using &.
- Print the array name and the pointer. Compare.
- Print &arr[0]. Compare with both.
- Access elements through the pointer using both p[i] and *(p+i).
- Print sizeof(arr) and sizeof(p) and explain the difference.

Questions before coding:
1. Why is no & needed when assigning an array to a pointer?
2. Are arr, &arr[0] and p the same value?
3. Why does sizeof differ, if the values are the same?

Why this matters for CUDA:
Kernels receive arrays as bare pointers. This is the conversion that makes that possible.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}