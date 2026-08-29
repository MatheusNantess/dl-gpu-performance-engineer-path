#include <cstdio>

// Capitulo 05. Mude STRIDE (1, 2, 4, 8, 32) e recompile. O programa imprime
// o endereco relativo real que cada lane do warp toca, e em qual segmento
// de 32 bytes ele cai. Conte quantos segmentos DISTINTOS aparecem -- esse
// e o numero de transacoes de memoria que o warp gera para esse padrao.

#ifndef STRIDE
#define STRIDE 1
#endif

__global__ void coalescingAddresses(float* x) {
    int lane = threadIdx.x; // um warp so, 32 threads
    float* addr = &x[lane * STRIDE];
    long long offset = (char*)addr - (char*)x;
    int segment = (int)(offset / 32); // segmentos de 32 bytes

    printf("lane %2d -> x[%3d] offset=%4lld bytes -> segmento de 32B #%d\n",
           lane, lane * STRIDE, offset, segment);
}

int main() {
    float* d_x;
    cudaMalloc(&d_x, 4096 * sizeof(float));
    printf("STRIDE = %d\n\n", STRIDE);
    coalescingAddresses<<<1, 32>>>(d_x);
    cudaDeviceSynchronize();
    cudaFree(d_x);
    printf("\nConte os segmentos #distintos acima -- isso e o numero de transacoes\n");
    printf("de 32B que esse warp gera para STRIDE=%d.\n", STRIDE);
    return 0;
}
