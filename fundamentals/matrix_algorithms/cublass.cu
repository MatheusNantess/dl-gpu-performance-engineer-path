#include <cstdio>
#include <cstdlib>
#include <cublas_v2.h>
#include <cuda_runtime.h>

void init_matrix(float *mat, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        mat[i] = (float)rand() / RAND_MAX;
    }
}

int main(){
    int M = 256;
    int K = 512;
    int N = 256;

    size_t size_a = M*K * sizeof(float);
    size_t size_b = K*N * sizeof(float);
    size_t size_c = M*N * sizeof(float);

    float *h_a = (float*)malloc(size_a);
    float *h_b = (float*)malloc(size_b);
    float *h_c = (float*)malloc(size_c);

    init_matrix(h_a, M, K);
    init_matrix(h_b, K, N);

    float *d_a, *d_b, *d_c;
    cudaMalloc(&d_a, size_a);
    cudaMalloc(&d_b, size_b);
    cudaMalloc(&d_c, size_c);

    cudaMemcpy(d_a, h_a, size_a, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size_b, cudaMemcpyHostToDevice);

    cublasHandle_t handle;
    cublasCreate(&handle);

    float alpha = 1.0f, beta = 0.0f;

    // Truque row-major: pedimos ao cuBLAS C^T = B^T * A^T (column-major)
    // que é EXATAMENTE C = A*B em row-major, sem transpor nada de verdade.
    // Por isso N e M trocam de posição na chamada.
    for (int i = 0; i < 3; i++) {  // warmup
        cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N,
                    N, M, K,
                    &alpha,
                    d_b, N,
                    d_a, K,
                    &beta,
                    d_c, N);
    }
    cudaDeviceSynchronize();

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    int iters = 100;
    cudaEventRecord(start);
    for (int i = 0; i < iters; i++) {
        cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N,
                    N, M, K,
                    &alpha,
                    d_b, N,
                    d_a, K,
                    &beta,
                    d_c, N);
    }
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float ms;
    cudaEventElapsedTime(&ms, start, stop);
    ms /= iters;

    double flops = 2.0 * M * K * N;
    double gflops = flops / (ms * 1e-3) / 1e9;

    printf("cuBLAS SGEMM: %.4f ms | %.2f GFLOPs/s\n", ms, gflops);

    cudaMemcpy(h_c, d_c, size_c, cudaMemcpyDeviceToHost);
    printf("C[0] = %f\n", h_c[0]);

    cublasDestroy(handle);
    cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
    free(h_a); free(h_b); free(h_c);

    return 0;
}