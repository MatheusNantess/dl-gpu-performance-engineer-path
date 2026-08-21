/*
Exercise 03 — Integer division traps
Difficulty: EASY

Goal:
Get the cast in the right place.

Requirements:
- Write a program that reads two integers a and b.
- Print: integer quotient, remainder, and the true decimal quotient.
- Produce the decimal quotient in TWO different ways (two different cast
  placements), and confirm both give the same answer.
- Handle b == 0 without crashing.

Questions before coding:
1. Why does `double r = a / b;` give the wrong answer?
2. What does integer division do with negative numbers -- round down, or
   toward zero? Predict, then verify.
3. What happens on integer division by zero, versus float division by zero?

Why this matters for CUDA:
Index arithmetic is integer arithmetic. Off-by-one from truncation is a
whole class of kernel bug.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}