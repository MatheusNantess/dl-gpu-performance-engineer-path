#include <cuda_runtime.h>
#include <nvtx3/nvToolsExt.h>
#include <iostream>
#include <cstdio>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define M 2
#define N 2
#define K 2
#define BLOCK_SIZE 16
__global__ void matmul(const float* A, const float*B,  float*C, int m, int n, int k){
    int col = threadIdx.x + blockDim.x *blockIdx.x;
    int row = threadIdx.y + blockDim.y * blockIdx.y;

    if(col < k && row < m){
        float soma = 0;
        for(int i = 0; i < n; i++){
            soma = soma + A[row * n + i] * B[i*k + col];
            
        }
        C[row * k + col] = soma;

    }

}
void init_matrix(float *mat, int rows, int cols){
    for(int i = 0; i< rows*cols; i++){
        mat[i] = (float)rand() / RAND_MAX;
    }
}



int main(){
    float * h_a, *h_b, *h_c;
    float * d_a, *d_b, *d_c;

    //numero de elementos em A * o tamanho dos floats
    size_t size_a = M*N * sizeof(float);
    size_t size_b = N*K * sizeof(float);
    size_t size_c = M*K * sizeof(float);

    h_a = (float*)malloc(size_a);
    h_b = (float*)malloc(size_b);
    h_c = (float*)malloc(size_c);

    printf("h_a: %p", (void*)h_a);
    printf("h_b: %p", (void*)h_b);
    printf("h_c: %p", (void*)h_c);

    init_matrix(h_a, M, N);
    init_matrix(h_b, N, K);
    


    printf("h_a value: %f", *h_a);

    // cudaMalloc recebe um void** porque ele precisa alterar o valor da variável
    // que guarda o ponteiro para o buffer no device. Aqui, d_a já é um ponteiro,
    // mas o que a API precisa receber é o endereço da variável d_a (ou seja,
    // &d_a), para que ela possa escrever dentro dela o endereço real do bloco
    // alocado no GPU. Em outras palavras:
    //   d_a  -> valor armazenado na variável (endereço no device)
    //   &d_a -> endereço da variável, onde cudaMalloc vai guardar esse valor
    // Por isso usamos &d_a em cudaMalloc, mas usamos d_a em cudaMemcpy, porque
    // cudaMemcpy precisa do ponteiro já preenchido para copiar os dados.
    cudaMalloc(&d_a, size_a);
    cudaMalloc(&d_b, size_b);
    cudaMalloc(&d_c, size_c);

    printf("d_a endereço: %p", d_a);

    // d_a já contém o endereço do buffer no dispositivo; não use &d_a em cudaMemcpy.
    cudaMemcpy(d_a, h_a, size_a, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size_b, cudaMemcpyHostToDevice);

    float check; 
    cudaMemcpy(&check, d_a, sizeof(float), cudaMemcpyDeviceToHost);
    printf("d_a value: %f", check);

    dim3 block_size(BLOCK_SIZE,BLOCK_SIZE);
    //a ordem x,y tem que ser a mesma construida no kernel, ou seja col = x e row = y
    //   col = x → gridDim.x precisa cobrir o range de col (que é K)
    //   row = y → gridDim.y precisa cobrir o range de row (que é M)
    dim3 grid_dim((K + BLOCK_SIZE - 1) / BLOCK_SIZE, (M + BLOCK_SIZE - 1) / BLOCK_SIZE);

    matmul<<<grid_dim, block_size>>>(d_a, d_b, d_c, M, N, K);
    cudaDeviceSynchronize();


    cudaMemcpy(h_c, d_c, size_c, cudaMemcpyDeviceToHost);

    free(h_a);
    free(h_b);
    free(h_c);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) printf("Erro: %s\n", cudaGetErrorString(err));

    

}
    





















    


