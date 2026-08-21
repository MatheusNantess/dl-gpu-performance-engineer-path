/*
Exercise 05 — Predict the output
Difficulty: MEDIUM

Goal:
Test the type mental model before the compiler corrects it.

Requirements:
- Do NOT compile this yet.
- Write your prediction for every marked line in the comment block below.
- Then compile, run, and record which predictions were wrong and why.

    g++ -std=c++17 -Wall -Wextra -O0 -o exercise-05 exercise-05.cpp

Questions before coding:
1. Which lines trigger a compiler warning? Predict that too.
2. Which of these are undefined behaviour rather than merely surprising?

Why this matters for CUDA:
Predict-then-verify is the same discipline used later for profiler metrics.
Being wrong here is cheap; being wrong there costs a week.
*/

#include <iostream>

int main() {
    /*
    MY PREDICTIONS (fill in before compiling):
    A:
    B:
    C:
    D:
    E:
    F:
    */

    int   a = 5;
    int   b = 2;
    float c = 5.0f;

    std::cout << "A: " << a / b       << "\n";
    std::cout << "B: " << c / b       << "\n";
    std::cout << "C: " << a / 2.0     << "\n";
    std::cout << "D: " << (a > b)     << "\n";

    char ch = 'A';
    std::cout << "E: " << ch          << "\n";
    std::cout << "F: " << ch + 1      << "\n";

    // TODO: after running, write below what you got wrong and WHY.

    return 0;
}