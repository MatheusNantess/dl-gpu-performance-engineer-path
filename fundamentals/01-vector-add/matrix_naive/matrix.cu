#include <cstdio>
#include <cuda_runtime.h>

#define M 4096
#define N 4096
#define O 4096
#define BLOCK_SIZE 16

// Cada thread calcula um único elemento C[row][col], lendo direto da global memory
// (sem shared memory/tiling): percorre a linha inteira de A e a coluna inteira de B,
// somando os produtos — daí "naive", cada elemento de A/B é relido por várias threads.
__global__ void GpuMatrixMultiplication(float *A, float *B, float *C, int m, int n, int o){
    // Índice global da linha de A/C que essa thread vai processar.
    int row = threadIdx.y + blockDim.y * blockIdx.y;
    // Índice global da coluna de B/C que essa thread vai processar.
    int col = threadIdx.x + blockDim.x * blockIdx.x;

    // Só calcula se estiver dentro dos limites reais da matriz de saída (m x o).
    if(row < m && col < o){
        // Acumulador do produto escalar linha(A) . coluna(B) para C[row][col].
        float sum = 0;
        // Percorre toda a dimensão compartilhada n, lendo A e B direto da global memory
        // (sem cache em shared memory) — cada iteração é uma multiplicação-e-soma.
        for(int i = 0; i < n; i++){
            sum += A[row * n + i] * B[i * o + col];
        }
        // Grava o resultado final desse elemento em C, na posição linearizada row*o + col.
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

    

    GpuMatrixMultiplication<<<gridDim, blockDim>>>(d_A,d_B,d_C, M, N, O);

    cudaMemcpy(h_C, d_C, size_c, cudaMemcpyDeviceToHost);
    printf("C[0] = %f\n", h_C[0]);

    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
    free(h_A); free(h_B); free(h_C);

    return 0;
}