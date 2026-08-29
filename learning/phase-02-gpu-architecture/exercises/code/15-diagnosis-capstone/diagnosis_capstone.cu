#include <cstdio>

// Capitulo 12, capstone. Um kernel com 3 defeitos de performance ligados
// por flag de compilacao. Compile as 8 combinacoes (veja README /
// run_all.sh) e meca o tempo de cada uma pra descobrir qual fator pesa
// mais NO SEU CASO -- antes de ler a resposta, escreva sua hipotese.
//
// -DUSE_UNCOALESCED : acesso com stride ruim em vez de contiguo
// -DUSE_DIVERGENT    : branch que separa lanes pares/impares
// -DUSE_HEAVY_REGS   : trabalho extra que aumenta pressao de registrador

#define N (1 << 20)
#define THREADS 256

__global__ void mixedKernel(float* data, int n) {
#ifdef USE_UNCOALESCED
    int i = ((blockIdx.x * blockDim.x + threadIdx.x) * 32) % n; // stride ruim
#else
    int i = blockIdx.x * blockDim.x + threadIdx.x; // contiguo
#endif
    if (i >= n) return;

    float v = data[i];

#ifdef USE_DIVERGENT
    if (threadIdx.x % 2 == 0) {
        v = v * 2.0f + 1.0f;
    } else {
        v = v * 3.0f - 1.0f;
    }
#else
    v = v * 2.0f + 1.0f;
#endif

#ifdef USE_HEAVY_REGS
    float r[32];
    r[0] = v;
    #pragma unroll
    for (int k = 1; k < 32; k++) r[k] = r[k - 1] * 1.0001f + k;
    #pragma unroll
    for (int k = 0; k < 32; k++) v += r[k];
#endif

    data[i] = v;
}

int main() {
    float* d_data;
    cudaMalloc(&d_data, N * sizeof(float));
    cudaMemset(d_data, 0, N * sizeof(float));

    int blocks = (N + THREADS - 1) / THREADS;
    cudaEvent_t start, stop;
    cudaEventCreate(&start); cudaEventCreate(&stop);

    mixedKernel<<<blocks, THREADS>>>(d_data, N); cudaDeviceSynchronize();
    cudaEventRecord(start);
    for (int r = 0; r < 20; r++) mixedKernel<<<blocks, THREADS>>>(d_data, N);
    cudaEventRecord(stop); cudaEventSynchronize(stop);
    float ms;
    cudaEventElapsedTime(&ms, start, stop);

    printf("Config: "
#ifdef USE_UNCOALESCED
        "UNCOALESCED "
#endif
#ifdef USE_DIVERGENT
        "DIVERGENT "
#endif
#ifdef USE_HEAVY_REGS
        "HEAVY_REGS "
#endif
        "-> %.4f ms (20 launches)\n", ms);

    cudaFree(d_data);
    return 0;
}
