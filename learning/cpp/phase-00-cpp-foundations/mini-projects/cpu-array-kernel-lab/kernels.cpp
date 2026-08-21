#include "kernels.h"

// TODO: implement all six.
//
// For EACH function, before writing the body, answer in a comment:
//   - How many bytes does it read, for n elements?
//   - How many bytes does it write?
//   - Is it memory bound or compute bound? How do you know?
//   - What should it do when n == 0, or when a pointer is null?

// out[i] = a[i] + b[i]
// TODO: bytes read =        bytes written =
void vector_add(const float* a, const float* b, float* out, int n) {
    // TODO: implement
    (void)a; (void)b; (void)out; (void)n;
}

// data[i] *= factor, in place
// TODO: bytes read =        bytes written =
void scale(float* data, int n, float factor) {
    // TODO: implement
    (void)data; (void)n; (void)factor;
}

// returns the sum of all elements
// TODO: bytes read =        bytes written =
float sum(const float* data, int n) {
    // TODO: implement
    (void)data; (void)n;
    return 0.0f;
}

// returns the largest element
// TODO: bytes read =        bytes written =
float max_element(const float* data, int n) {
    // TODO: implement
    (void)data; (void)n;
    return 0.0f;
}

// returns the sum of a[i] * b[i]
// TODO: bytes read =        bytes written =
float dot_product(const float* a, const float* b, int n) {
    // TODO: implement
    (void)a; (void)b; (void)n;
    return 0.0f;
}

// rescales data to zero mean and unit standard deviation, in place
// TODO: how many PASSES over the data does your version need?
// TODO: bytes read =        bytes written =
void normalize(float* data, int n) {
    // TODO: implement
    (void)data; (void)n;
}