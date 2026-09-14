#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cuda_runtime.h>

#define N 100000

// Example 3x2 @ 2x4 = 3x4 -> (M x K) @ (K x N) = (M x N)
// A = [[1, 2], 
//      [3, 4], 
//      [5, 6]]

// B = [[7, 8, 9, 10],
//      [11, 12, 13, 14]]

// C = A * B = [[1*7 + 2*11, 1*8 + 2*12, 1*9 + 2*13, 1*10 + 2*14],
//              [3*7 + 4*11, 3*8 + 4*12, 3*9 + 4*13, 3*10 + 4*14],
//              [5*7 + 6*11, 5*8 + 6*12, 5*9 + 6*13, 5*10 + 6*14]]

// C = [[29, 32, 35, 38],
//      [65, 72, 79, 86],
//      [101, 112, 123, 134]]


__global__ void vector_add(const float*A, const float*B, float* C, int n){
    int thread_idx = threadIdx.x + blockDim.x * blockIdx.x;
    if(thread_idx < n){
        C[thread_idx] = A[thread_idx] + B[thread_idx];

    }
}

void init_vector(float *vect, int n) {
    for (int i = 0; i < n; i++) {
        vect[i] = (float)rand() / RAND_MAX;
    }
}

int main(){
    float * h_a, *h_b, *h_c;
    float * d_a, *d_b, *d_c;

    size_t size_a = N * sizeof(float);
    size_t size_b = N * sizeof(float);
    size_t size_c = N * sizeof(float);

    h_a = (float*)malloc(size_a);
    h_b = (float*)malloc(size_b);
    h_c = (float*)malloc(size_c);

    init_vector(h_a, N);
    init_vector(h_b, N);

    printf("h_A = %p", (void*)h_a);
    printf("h_b = %p", (void*)h_b);

    cudaMalloc(&d_a, size_a);
    cudaMalloc(&d_b, size_b);
    cudaMalloc(&d_c, size_c);


    printf("d_A = %p", (void*)d_a);
    printf("d_b = %p", (void*)d_b);

    cudaMemcpy(d_a, h_a, size_a, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size_b, cudaMemcpyHostToDevice);

    printf("h_a[0] = %f\n", h_a[0]);
    printf("h_b[0] = %f\n", h_b[0]);

    //float check;
    //cudaMemcpy(&check, d_a, sizeof(float), cudaMemcpyDeviceToHost);
    //printf("d_a[0] = %f\n", check);

    int block_size = 256;
    int grid_size = (N +block_size -1)/block_size;
    vector_add<<<grid_size, block_size>>>(d_a,d_b,d_c, N);
    cudaDeviceSynchronize();

     float check;
    cudaMemcpy(&check, d_c, sizeof(float), cudaMemcpyDeviceToHost);
    printf("d_c[0] = %f\n", check);

    cudaMemcpy(h_c, d_c, size_c, cudaMemcpyDeviceToHost);

    free(h_a);
    free(h_b);
    free(h_c);

    cudaFree(d_c);
    cudaFree(d_b);
    cudaFree(d_c);


    
    
    



  





}