#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cuda_runtime.h>

#define M 256  // Number of rows in A and C
#define K 512  // Number of columns in A and rows in B
#define N 256  // Number of columns in B and C
#define BLOCK_SIZE 32

// Kernel isolado para abrir no Nsight Compute -- sem benchmark de CPU, sem
// timing manual. O proprio `ncu` mede o kernel quando voce roda:
//
//   ncu -o matmul_naive_report ./matmul_naive
//
// Correcao numerica ja foi validada nos exercicios anteriores (matrix.cu);
// o unico objetivo deste arquivo e ter um kernel simples e representativo
// pra olhar no profiler -- naive, sem tiling, mesmo kernel que voce ja
// escreveu, so sem o ruido do benchmark CPU x GPU em volta.

__global__ void matmul_gpu(float *A, float *B, float *C, int m, int k, int n) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < m && col < n) {
        float sum = 0.0f;
        for (int l = 0; l < k; l++) {
            sum += A[row * k + l] * B[l * n + col];
        }
        C[row * n + col] = sum;
    }
}

void init_matrix(float *mat, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        mat[i] = (float)rand() / RAND_MAX;
    }
}

int main() {
    float *h_A, *h_B;
    float *d_A, *d_B, *d_C;
    size_t size_A = (size_t)M * K * sizeof(float);
    size_t size_B = (size_t)K * N * sizeof(float);
    size_t size_C = (size_t)M * N * sizeof(float);

    h_A = (float*)malloc(size_A);
    h_B = (float*)malloc(size_B);

    srand(time(NULL));
    init_matrix(h_A, M, K);
    init_matrix(h_B, K, N);

    cudaMalloc(&d_A, size_A);
    cudaMalloc(&d_B, size_B);
    cudaMalloc(&d_C, size_C);

    cudaMemcpy(d_A, h_A, size_A, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size_B, cudaMemcpyHostToDevice);

    dim3 blockDim(BLOCK_SIZE, BLOCK_SIZE);
    dim3 gridDim((N + BLOCK_SIZE - 1) / BLOCK_SIZE, (M + BLOCK_SIZE - 1) / BLOCK_SIZE);

    // warm-up: absorve o custo de JIT/driver init, fora do que interessa perfilar
    matmul_gpu<<<gridDim, blockDim>>>(d_A, d_B, d_C, M, K, N);
    cudaDeviceSynchronize();

    // este e' o launch que o Nsight Compute vai interceptar e perfilar
    matmul_gpu<<<gridDim, blockDim>>>(d_A, d_B, d_C, M, K, N);
    cudaDeviceSynchronize();

    printf("done\n");

    free(h_A);
    free(h_B);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}
