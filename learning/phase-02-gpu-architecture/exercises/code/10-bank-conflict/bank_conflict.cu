#include <cstdio>

// Capitulo 07. noConflict: cada lane acessa um bank de shared memory
// diferente (stride 1) -- acesso paralelo de verdade. bankConflict: todas
// as 32 lanes acessam o MESMO bank (stride 32) -- o hardware serializa em
// 32 acessos sequenciais. Comparamos o tempo repetindo o acesso muitas vezes.

#ifndef ITERS
#define ITERS 200000
#endif
#define THREADS 32

// volatile e necessario: sem ele, o compilador percebe que o valor lido
// nunca muda dentro do loop (loop-invariant) e substitui as 10000+ leituras
// repetidas por UMA leitura + multiplicacao, escondendo o custo real do
// acesso a shared memory que queremos medir aqui.

__global__ void noConflict(float* out) {
    __shared__ volatile float s[THREADS];
    int tid = threadIdx.x;
    s[tid] = tid;
    __syncthreads();
    float acc = 0.0f;
    for (int i = 0; i < ITERS; i++) {
        acc += s[tid]; // cada lane num bank diferente
    }
    out[tid] = acc;
}

__global__ void bankConflict(float* out) {
    __shared__ volatile float s[THREADS * 32];
    int tid = threadIdx.x;
    s[tid * 32] = tid; // stride 32 -> todas as lanes caem no MESMO bank
    __syncthreads();
    float acc = 0.0f;
    for (int i = 0; i < ITERS; i++) {
        acc += s[tid * 32]; // 32-way bank conflict, acesso serializado
    }
    out[tid] = acc;
}

int main() {
    float* d_out;
    cudaMalloc(&d_out, THREADS * sizeof(float));
    cudaEvent_t start, stop;
    cudaEventCreate(&start); cudaEventCreate(&stop);
    float ms;

    noConflict<<<1, THREADS>>>(d_out); cudaDeviceSynchronize();
    cudaEventRecord(start);
    noConflict<<<1, THREADS>>>(d_out);
    cudaEventRecord(stop); cudaEventSynchronize(stop);
    cudaEventElapsedTime(&ms, start, stop);
    printf("sem conflito (stride 1, bancos diferentes):  %.4f ms\n", ms);

    bankConflict<<<1, THREADS>>>(d_out); cudaDeviceSynchronize();
    cudaEventRecord(start);
    bankConflict<<<1, THREADS>>>(d_out);
    cudaEventRecord(stop); cudaEventSynchronize(stop);
    cudaEventElapsedTime(&ms, start, stop);
    printf("com conflito (stride 32, mesmo banco):       %.4f ms\n", ms);

    cudaFree(d_out);
    return 0;
}
