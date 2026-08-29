#include <cstdio>

// Capitulo 03. 128 threads = 4 warps. So imprime a lane 0 de cada warp
// para deixar claro que warps sao SEMPRE 32 threads CONSECUTIVAS por
// threadIdx -- voce nunca escreve isso no codigo, e o hardware quem fatia.

#define N 128 // 4 warps

__global__ void warpFormation() {
    int tid = threadIdx.x;
    int warpId = tid / 32;
    int lane = tid % 32;
    if (lane == 0) {
        printf("warp %d comeca na thread %3d e termina na thread %3d (32 threads consecutivas)\n",
               warpId, tid, tid + 31);
    }
}

int main() {
    warpFormation<<<1, N>>>();
    cudaDeviceSynchronize();
    return 0;
}
