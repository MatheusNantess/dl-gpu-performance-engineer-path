/*
Exercise 04b — Linker errors (file 2 of 2)

The definition that exercise-04a.cpp needs. See that file for the tasks.

Build both:
    g++ -std=c++17 -Wall -Wextra -c exercise-04a.cpp -o 04a.o
    g++ -std=c++17 -Wall -Wextra -c exercise-04b.cpp -o 04b.o
    g++ 04a.o 04b.o -o prog
*/

int helper(int x) {
    return x * 2;
}

// TODO (step 4): change the parameter type to double, leaving the
// declaration in exercise-04a.cpp as int. Predict the failing stage first.