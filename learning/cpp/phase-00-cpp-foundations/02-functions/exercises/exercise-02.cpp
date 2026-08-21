/*
Exercise 02 — Value transformations
Difficulty: EASY

Goal:
Functions that compute rather than mutate.

Requirements:
- Write celsius_to_fahrenheit(float) and its inverse.
- Write clamp(float value, float lo, float hi).
- Write lerp(float a, float b, float t) -- linear interpolation.
- Verify each round-trips or behaves correctly at the boundaries.

Questions before coding:
1. Should any of these take a pointer? Why not?
2. What does clamp do when lo > hi? Decide, then document your choice.
3. Is lerp(a,b,0) exactly a, in floating point? Predict, then check.

Why this matters for CUDA:
These are elementwise operations -- exactly the shape of the simplest GPU
kernels. Later, each of these becomes one thread's worth of work.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}