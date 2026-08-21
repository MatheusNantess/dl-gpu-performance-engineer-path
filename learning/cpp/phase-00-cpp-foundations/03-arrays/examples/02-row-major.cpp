/*
Example 02 — Row-major layout

Complete, working program. Predict the printed offsets before running.

    g++ -std=c++17 -Wall -Wextra -g -O0 -o 02-row-major 02-row-major.cpp
    ./02-row-major
*/

#include <iostream>

int main() {
    const int rows = 3;
    const int cols = 4;

    // A language-level 2D array...
    int m[rows][cols] = {
        { 0,  1,  2,  3},
        { 4,  5,  6,  7},
        { 8,  9, 10, 11}
    };

    std::cout << "--- logical view ---\n";
    for (int r = 0; r < rows; ++r) {
        std::cout << "  ";
        for (int c = 0; c < cols; ++c) std::cout << m[r][c] << "\t";
        std::cout << "\n";
    }

    std::cout << "\n--- memory view: element offset from the start ---\n";
    const int* base = &m[0][0];
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            std::cout << "  m[" << r << "][" << c << "]  offset "
                      << (&m[r][c] - base)
                      << "   formula r*cols+c = " << (r * cols + c) << "\n";
        }
    }

    // The offset always equals r * cols + c. That formula IS the layout.
    // Note it uses cols, not rows. Swapping them is a classic bug that
    // produces plausible but wrong results.

    std::cout << "\n--- the same data as a flat array ---\n";
    // This is how GPU code almost always does it: one flat allocation,
    // indexed by hand. cudaMalloc returns a flat pointer, so there is no
    // alternative anyway.
    int flat[rows * cols] = {0,1,2,3,4,5,6,7,8,9,10,11};

    for (int r = 0; r < rows; ++r) {
        std::cout << "  ";
        for (int c = 0; c < cols; ++c) std::cout << flat[r * cols + c] << "\t";
        std::cout << "\n";
    }

    std::cout << "\n--- traversal order ---\n";

    std::cout << "  row-major order (walks memory forward):    ";
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            std::cout << flat[r * cols + c] << " ";
    std::cout << "\n";

    std::cout << "  column-major order (jumps by cols each step): ";
    for (int c = 0; c < cols; ++c)
        for (int r = 0; r < rows; ++r)
            std::cout << flat[r * cols + c] << " ";
    std::cout << "\n";

    // Same values, same instruction count, different address sequence.
    // On a 3x4 array the difference is invisible. On a large matrix the
    // second one costs a cache miss per access on the CPU -- and on a GPU
    // it costs most of the available bandwidth, because the warp's 32
    // reads no longer land in one contiguous block.
    //
    // Exercise 14 asks you to measure this rather than believe it.

    return 0;
}