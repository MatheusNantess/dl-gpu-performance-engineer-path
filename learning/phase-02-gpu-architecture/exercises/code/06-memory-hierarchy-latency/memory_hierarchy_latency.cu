#include <cstdio>

// Capitulo 04. Compara, em ciclos de clock (clock64()), o custo de repetir
// uma operacao em registrador vs shared memory vs memoria global. Compile
// com -O0 (veja README) para o compilador nao eliminar os loops.

#define ITERS 1000

__global__ void latencyCompare(int* globalData) {
    __shared__ int sharedData[32];
    int tid = threadIdx.x;
    sharedData[tid] = tid;
    __syncthreads();

    long long t0 = clock64();
    int regVal = tid;
    for (int i = 0; i < ITERS; i++) regVal += 1;
    long long t1 = clock64();

    for (int i = 0; i < ITERS; i++) sharedData[tid] = sharedData[tid] + 1;
    long long t2 = clock64();

    for (int i = 0; i < ITERS; i++) globalData[tid] = globalData[tid] + 1;
    long long t3 = clock64();

    if (tid == 0) {
        printf("Ciclos para %d iteracoes de leitura+escrita repetida:\n", ITERS);
        printf("  registrador:   %6lld ciclos\n", t1 - t0);
        printf("  shared memory: %6lld ciclos\n", t2 - t1);
        printf("  global memory: %6lld ciclos\n", t3 - t2);
        printf("(valores de controle p/ o compilador nao eliminar os loops: regVal=%d shared=%d)\n",
               regVal, sharedData[tid]);
    }
}

int main() {
    int* d_data;
    cudaMalloc(&d_data, 32 * sizeof(int));
    cudaMemset(d_data, 0, 32 * sizeof(int));
    latencyCompare<<<1, 32>>>(d_data);
    cudaDeviceSynchronize();
    cudaFree(d_data);
    return 0;
}
