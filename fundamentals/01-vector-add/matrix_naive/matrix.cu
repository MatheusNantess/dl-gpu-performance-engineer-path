#include <cstdio>
#include <cuda_runtime.h>

#define M 4096
#define N 4096
#define O 4096
#define BLOCK_SIZE 16

__global__ void GpuMatrixMultiplication(float *A, float *B, float *C, int m, int n, int o){
    int row = threadIdx.y + blockDim.y * blockIdx.y;
    int col = threadIdx.x + blockDim.x * blockIdx.x;

    if(row < m && col < o){
        float sum = 0;
        for(int i = 0; i < n; i++){
            sum += A[row * n + i] * B[i * o + col];
        }
        C[row*o + col] = sum;
    }
}

void init_matrix(float *mat, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        mat[i] = (float)rand() / RAND_MAX;
    }
}

int main(){
    float *h_A, *h_B, *h_C;
    float *d_A, *d_B, *d_C;

    size_t size_a = M*N * sizeof(float);
    size_t size_b = N*O * sizeof(float);
    size_t size_c = M*O * sizeof(float);

    h_A = (float*)malloc(size_a);
    h_B = (float*)malloc(size_b);
    h_C = (float*)malloc(size_c);

    init_matrix(h_A, M, N);
    init_matrix(h_B, N, O);

    cudaMalloc(&d_A, size_a);
    cudaMalloc(&d_B, size_b);
    cudaMalloc(&d_C, size_c);

    cudaMemcpy(d_A, h_A, size_a, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size_b, cudaMemcpyHostToDevice);

    dim3 blockDim(BLOCK_SIZE, BLOCK_SIZE);
    dim3 gridDim((O + BLOCK_SIZE - 1) / BLOCK_SIZE, (M + BLOCK_SIZE - 1) / BLOCK_SIZE);

    // warmup
    for (int i = 0; i < 3; i++) {
        GpuMatrixMultiplication<<<gridDim, blockDim>>>(d_A, d_B, d_C, M, N, O);
    }
    cudaDeviceSynchronize();

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    int iters = 20;
    cudaEventRecord(start);
    for (int i = 0; i < iters; i++) {
        GpuMatrixMultiplication<<<gridDim, blockDim>>>(d_A, d_B, d_C, M, N, O);
    }
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float ms;
    cudaEventElapsedTime(&ms, start, stop);
    ms /= iters;

    double flops = 2.0 * M * N * O;
    double gflops = flops / (ms * 1e-3) / 1e9;

    printf("Naive GEMM: %.4f ms | %.2f GFLOPs/s\n", ms, gflops);

    cudaMemcpy(h_C, d_C, size_c, cudaMemcpyDeviceToHost);
    printf("C[0] = %f\n", h_C[0]);

    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
    free(h_A); free(h_B); free(h_C);

    return 0;
}