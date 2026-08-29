#include <cstdio>
#include <cuda_runtime.h>

// One block, 64 threads -> exactly 2 warps. Small on purpose: the printf
// output is meant to be read line by line, not benchmarked.
#define N 64

__global__ void vectorAdd(const float* a, const float* b, float* c) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int warpId = threadIdx.x / 32;
    int lane = threadIdx.x % 32;

    c[i] = a[i] + b[i];

    // Only print a handful of threads so the output stays readable.
    if (threadIdx.x < 4 || (threadIdx.x >= 32 && threadIdx.x < 36)) {
        printf("thread %2d | block %d | warp %d | lane %2d | core computes c[%2d] = %.1f + %.1f = %.1f\n",
               threadIdx.x, blockIdx.x, warpId, lane, i, a[i], b[i], c[i]);
    }
}

int main() {
    float h_a[N], h_b[N], h_c[N];
    for (int i = 0; i < N; i++) {
        h_a[i] = (float)i;
        h_b[i] = (float)(i * 10);
    }

    float *d_a, *d_b, *d_c;
    cudaMalloc(&d_a, N * sizeof(float));
    cudaMalloc(&d_b, N * sizeof(float));
    cudaMalloc(&d_c, N * sizeof(float));

    cudaMemcpy(d_a, h_a, N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, N * sizeof(float), cudaMemcpyHostToDevice);

    // 1 block of 64 threads = 2 warps, all resident on the same SM.
    vectorAdd<<<1, 64>>>(d_a, d_b, d_c);
    cudaDeviceSynchronize();

    cudaMemcpy(h_c, d_c, N * sizeof(float), cudaMemcpyDeviceToHost);

    printf("\nc[0]=%.1f c[31]=%.1f c[32]=%.1f c[63]=%.1f\n", h_c[0], h_c[31], h_c[32], h_c[63]);

    cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
    return 0;
}
