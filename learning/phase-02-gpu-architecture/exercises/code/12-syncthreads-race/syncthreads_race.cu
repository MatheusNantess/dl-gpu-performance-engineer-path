#include <cstdio>

// Capitulo 10. 64 threads = 2 warps. warp 0 e artificialmente atrasado
// (com __nanosleep, um intrinsic de hardware -- um loop vazio comum
// costuma ser otimizado pelo compilador e nao atrasa nada de verdade)
// antes de escrever na shared memory; warp 1 escreve rapido e tenta ler o
// dado do warp 0 sem esperar. SEM __syncthreads() isso e uma corrida real
// entre warps (nao dentro de um warp -- dentro de um warp a execucao ja e
// lockstep). COM __syncthreads() o resultado e sempre correto.

#define N 64

__global__ void withoutSync(int* out) {
    __shared__ int s[N];
    int tid = threadIdx.x;

    if (tid < 32) {
        for (int r = 0; r < 20; r++) __nanosleep(100000); // ~2ms de atraso real
        s[tid] = tid;
    } else {
        s[tid] = tid; // warp 1 escreve rapido
    }
    // SEM barreira: warp 1 pode ler o slot do warp 0 antes dele escrever
    int neighbor = s[(tid + 32) % N];
    out[tid] = neighbor;
}

__global__ void withSync(int* out) {
    __shared__ int s[N];
    int tid = threadIdx.x;

    if (tid < 32) {
        for (int r = 0; r < 20; r++) __nanosleep(100000);
        s[tid] = tid;
    } else {
        s[tid] = tid;
    }
    __syncthreads(); // TODAS as threads do block esperam aqui
    int neighbor = s[(tid + 32) % N];
    out[tid] = neighbor;
}

int main() {
    int* d_out;
    cudaMalloc(&d_out, N * sizeof(int));
    int h_out[N];

    withoutSync<<<1, N>>>(d_out);
    cudaMemcpy(h_out, d_out, N * sizeof(int), cudaMemcpyDeviceToHost);
    printf("SEM __syncthreads() -- threads 32-63 leem o vizinho do warp 0 antes dele escrever:\n");
    printf("esperado se fosse seguro: 0 1 2 3 4 5 6 7 ... | obtido: ");
    for (int i = 32; i < 40; i++) printf("%d ", h_out[i]);
    printf("...\n\n");

    withSync<<<1, N>>>(d_out);
    cudaMemcpy(h_out, d_out, N * sizeof(int), cudaMemcpyDeviceToHost);
    printf("COM __syncthreads() -- sempre correto:\n");
    printf("obtido: ");
    for (int i = 32; i < 40; i++) printf("%d ", h_out[i]);
    printf("...\n");

    cudaFree(d_out);
    return 0;
}
