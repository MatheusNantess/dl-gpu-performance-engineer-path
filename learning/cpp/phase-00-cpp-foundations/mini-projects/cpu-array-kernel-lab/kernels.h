#ifndef KERNELS_H
#define KERNELS_H

// Declarations for the six operations.
//
// Note what every signature has in common with a CUDA kernel:
//   - arrays passed as pointers, because they cannot be passed any other way
//   - an explicit element count, because a pointer does not carry a length
//   - const on inputs, non-const on outputs
//
// The reduction operations (sum, max_element, dot_product) return a value
// here. Their CUDA equivalents cannot -- a kernel returns void -- so they
// will write to an output pointer instead. Worth noticing now.

void  vector_add(const float* a, const float* b, float* out, int n);
void  scale(float* data, int n, float factor);
float sum(const float* data, int n);
float max_element(const float* data, int n);
float dot_product(const float* a, const float* b, int n);
void  normalize(float* data, int n);

#endif  // KERNELS_H