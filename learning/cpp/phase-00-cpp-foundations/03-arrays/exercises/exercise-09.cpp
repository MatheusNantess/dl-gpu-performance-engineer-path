/*
Exercise 09 — Out-of-bounds
Difficulty: MEDIUM

Goal:
Trigger the bug deliberately and see how quiet it is.

Requirements:
- Declare a small int array with known values.
- Declare another variable immediately after it.
- Deliberately write past the end of the array.
- Print the other variable. Report what happened.
- Then read far past the end -- try index 1000 -- and see what occurs.
- Compile with -fsanitize=address and run again. Record the difference.

Questions before coding:
1. Did the program crash? Should it have?
2. Did the neighbouring variable change? Why would it?
3. Why is a program that appears to work not evidence of correctness here?

Why this matters for CUDA:
An out-of-bounds write in a kernel corrupts another tensor and produces wrong training results with no crash and no message. This is why bounds checks appear in kernels.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}