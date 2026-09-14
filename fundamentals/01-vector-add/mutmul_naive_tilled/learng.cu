#include <stdio.h>
#include <cuda_runtime.h>
#define TILE_SIZE 16



__global__ void tilledMatmul(const float * A, const float *B, float * C, int M, int K, int N){
    //DECLARAÇÃO DOS TILES EM SHARED MEMORY. nesse caso usarei 16x16
    __shared__ float A_tile[TILE_SIZE][TILE_SIZE];
    __shared__ float B_tile[TILE_SIZE][TILE_SIZE];

    int row = threadIdx.y + blockDim.y * blockIdx.y;
    int col = threadIdx.x + blockDim.x * blockIdx.x; 
    int tx = threadIdx.x;
    int ty = threadIdx.y;

    int number_tiles = (K + TILE_SIZE -1 )/TILE_SIZE;
    float soma = 0;

    for(int t = 0; t < number_tiles; t++){
        int tile_start = t * TILE_SIZE;

        int a_tile_index_cols = threadIdx.x + tile_start;
        int b_tile_index_rows = threadIdx.y + tile_start;

        if(row < M   &&  a_tile_index_cols < K ){
            A_tile[ty][tx] = A[row * K + a_tile_index_cols];
        } else {
            A_tile[ty][tx] = 0.0f;
        } 

        if(b_tile_index_rows < K   && col < N){
            B_tile[ty][tx] = B[b_tile_index_rows * N + col];
        } else {
            B_tile[ty][tx] = 0.0f;
        }
        __syncthreads();

        for(int i = 0; i < TILE_SIZE; i++){
            soma = soma + A_tile[ty][i] * B_tile[i][tx];
        }
        __syncthreads();
    }
    if(row < M && col < N){
        C[row * N + col] = soma;
    }
}


void init_matrix(float *mat, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        mat[i] = (float)rand() / RAND_MAX;
    }
}

int main(){

    float * h_a, *h_b, *h_c;
    float * d_a, *d_b, *d_c;
    int M = 4096;
    int K = 4096;
    int N = 4096;

    size_t size_a = M*K * sizeof(float);
    size_t size_b = K*N * sizeof(float);
    size_t size_c = M*N * sizeof(float);

    h_a = (float*)malloc(size_a);
    h_b = (float*)malloc(size_b);
    h_c = (float*)malloc(size_c);

    init_matrix(h_a, M, K);
    init_matrix(h_b, K, N);

    cudaMalloc(&d_a, size_a);
    cudaMalloc(&d_b, size_b);
    cudaMalloc(&d_c, size_c);

    cudaMemcpy(d_a, h_a, size_a, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size_b, cudaMemcpyHostToDevice);

    dim3 block_dim(TILE_SIZE, TILE_SIZE);
    dim3 grid_size((N + TILE_SIZE - 1) / TILE_SIZE,
                   (M + TILE_SIZE - 1) / TILE_SIZE);

    // warmup
    for (int i = 0; i < 3; i++) {
        tilledMatmul<<<grid_size, block_dim>>>(d_a, d_b, d_c, M, K, N);
    }
    cudaDeviceSynchronize();

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    int iters = 20;
    cudaEventRecord(start);
    for (int i = 0; i < iters; i++) {
        tilledMatmul<<<grid_size, block_dim>>>(d_a, d_b, d_c, M, K, N);
    }
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float ms;
    cudaEventElapsedTime(&ms, start, stop);
    ms /= iters;

    double flops = 2.0 * M * K * N;
    double gflops = flops / (ms * 1e-3) / 1e9;

    printf("Tiled (com bounds check): %.4f ms | %.2f GFLOPs/s\n", ms, gflops);

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) printf("Erro no kernel: %s\n", cudaGetErrorString(err));

    cudaMemcpy(h_c, d_c, size_c, cudaMemcpyDeviceToHost);
    printf("C[0] = %f\n", h_c[0]);

    free(h_a);
    free(h_b);
    free(h_c);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;
}