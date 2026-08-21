/*
Exercise 05 — Header problems
Difficulty: MEDIUM

Goal:
See what a missing include guard and a definition-in-a-header actually do.

Requirements:
1. Compile this file as written. Predict the result first.
2. Add a second #include "bad_header.h" and compile again. Predict first.
3. Create a second .cpp file that also includes bad_header.h, and link both.
   Predict the error and its category.
4. Fix bad_header.h with an include guard. Does step 2 work now? Does step 3?
5. Fix it properly: declaration in the header, definition in a .cpp file.
6. Then try `inline` on the definition in the header instead. Does that work?
   Explain why.

Questions before coding:
1. What does the preprocessor literally do with #include?
2. Why does an include guard fix step 2 but not step 3?
3. Why does inline change the answer?

Why this matters for CUDA:
CUDA projects are header-heavy, and __device__ functions defined in headages
included by several translation units hit exactly this rule.
*/

#include <iostream>
#include "bad_header.h"

// Step 2: uncomment this.
// #include "bad_header.h"

int main() {
    std::cout << "triple(5) = " << triple(5) << "\n";
    return 0;
}

/*
MY PREDICTIONS:
Step 1:
Step 2:
Step 3:
Step 6:

ACTUAL:

*/