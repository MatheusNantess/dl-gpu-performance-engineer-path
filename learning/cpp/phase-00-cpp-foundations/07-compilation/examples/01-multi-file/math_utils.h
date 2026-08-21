// math_utils.h -- DECLARATIONS only.
//
// A header says what exists. The definitions live in math_utils.cpp.
// This split is why main.cpp can be compiled without ever seeing the
// bodies of these functions: the compiler needs the signature, and the
// linker supplies the body.

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// The include guard above prevents this file's contents being pasted twice
// into the same translation unit. #pragma once does the same thing in one
// line and is universally supported in practice.

int   square(int x);
int   cube(int x);
float average(const float* data, int n);
void  scale(float* data, int n, float factor);

// Note the last two signatures: a const pointer for reading, a non-const
// pointer for writing, and an explicit element count in both cases, because
// the array does not carry its own length.
//
// These are, almost exactly, CUDA kernel signatures minus __global__.

#endif  // MATH_UTILS_H