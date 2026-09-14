#include <cstdio>

// Capitulos 02/03. blockDim.x = 100 NAO e multiplo de 32. O hardware aloca
// warps inteiros mesmo assim: 100 threads precisam de 4 warps (128
// lane-slots), entao o ultimo warp tem 28 lanes que existem fisicamente
// mas nao rodam nenhuma thread real do seu grid.

#define N 100

__global__ void partialWarp() {
    int tid = threadIdx.x;
    int warpId = tid / 32;
    int lane = tid % 32;
    if (warpId == 3) {
        printf("thread %3d | warp %d | lane %2d | thread REAL, faz trabalho\n", tid, warpId, lane);
    }
}

int main() {
    partialWarp<<<1, N>>>();
    cudaDeviceSynchronize();
    printf("\nLancei %d threads -> ceil(%d/32) = 4 warps alocados (128 lane-slots).\n", N, N);
    printf("So aparecem threads 96-99 no warp 3 (lanes 0-3); as lanes 4-31 desse warp\n");
    printf("nao correspondem a nenhuma thread sua -- ficam mascaradas, sem custo de trabalho\n");
    printf("extra, mas o warp inteiro ainda ocupa um slot de escalonamento.\n");
    return 0;
}
