// math_utils.cpp -- DEFINITIONS.
//
// Compiled separately from main.cpp. The linker joins them.

#include "math_utils.h"   // quotes: search relative to this file first

int square(int x) {
    return x * x;
}

int cube(int x) {
    return x * x * x;
}

float average(const float* data, int n) {
    if (n <= 0) return 0.0f;
    float sum = 0.0f;
    for (int i = 0; i < n; ++i) {
        sum += data[i];
    }
    return sum / static_cast<float>(n);
}

void scale(float* data, int n, float factor) {
    for (int i = 0; i < n; ++i) {
        data[i] *= factor;
    }
}

// scale() is worth a second look. Replace the loop with a thread index and
// a bounds check, add __global__, and it is a CUDA kernel:
//
//     __global__ void scale(float* data, int n, float factor) {
//         int i = blockIdx.x * blockDim.x + threadIdx.x;
//         if (i < n) data[i] *= factor;
//     }
//
// The signature does not change at all. That is the point of Phase 0.