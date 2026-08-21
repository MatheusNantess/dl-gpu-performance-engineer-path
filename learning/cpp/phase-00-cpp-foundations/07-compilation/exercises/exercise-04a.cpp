/*
Exercise 04a — Linker errors (file 1 of 2)
Difficulty: MEDIUM

Goal:
Produce and recognize the two classic linker failures.

Requirements:
Work through these in order, predicting the category each time:

  1. Compile this file alone:
         g++ -std=c++17 -Wall -Wextra -c exercise-04a.cpp -o 04a.o
     Does it succeed? It calls a function it has never seen defined.

  2. Try to link it alone:
         g++ 04a.o -o prog
     Predict the error and its category first.

  3. Compile exercise-04b.cpp and link both. Does it work now?

  4. In exercise-04b.cpp, change helper's parameter from int to double,
     leaving the declaration here unchanged. Predict what fails, and at
     which stage.

  5. Uncomment the duplicate definition at the bottom of this file and
     link both again. Predict the error.

Questions before coding:
1. Why does step 1 compile at all?
2. In step 4, both files compile. Why does linking fail?
3. What does a mangled name in a linker message tell you?

Why this matters for CUDA:
A __device__ function declared and never defined fails exactly like step 2,
with a message that does not look like the C++ you wrote.
*/

#include <iostream>

int helper(int x);          // declared here, defined in exercise-04b.cpp

int main() {
    std::cout << "helper(7) = " << helper(7) << "\n";
    return 0;
}

// Step 5: uncomment this to create a multiple definition.
// int helper(int x) { return x * 100; }

/*
MY PREDICTIONS:
Step 1 category:
Step 2 category:
Step 4 category:
Step 5 category:

ACTUAL:

*/