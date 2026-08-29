#include <cstdio>

// Capitulo 02 (threads/blocks/grids). Grid 2D de 2x2 blocks, cada block 4x4
// threads = 64 threads totais. Mostra como blockIdx + threadIdx + blockDim
// + gridDim se combinam num unico indice global -- a formula que voce
// escreve na mao em quase todo kernel.

__global__ void globalIndexMap() {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int width = gridDim.x * blockDim.x;
    int globalIdx = row * width + col;

    if (threadIdx.x < 2 && threadIdx.y < 2) {
        printf("block(%d,%d) thread(%d,%d) -> row=%d col=%d globalIdx=%d\n",
               blockIdx.x, blockIdx.y, threadIdx.x, threadIdx.y, row, col, globalIdx);
    }
}

int main() {
    dim3 block(4, 4);
    dim3 grid(2, 2);
    globalIndexMap<<<grid, block>>>();
    cudaDeviceSynchronize();
    printf("\nObserve: block(0,0) e block(1,1) nunca geram o mesmo globalIdx --\n");
    printf("a formula garante que cada uma das 64 threads tem um indice unico.\n");
    return 0;
}
