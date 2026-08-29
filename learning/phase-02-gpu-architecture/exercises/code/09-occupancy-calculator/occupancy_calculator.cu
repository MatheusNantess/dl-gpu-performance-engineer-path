#include <cstdio>

// Capitulo 06. Usa a API real do CUDA runtime (nao uma conta de papel) para
// perguntar pro driver: "quantos blocks desse kernel cabem residentes por
// SM, dado blockSize e uso de recursos desse kernel especifico?"

__global__ void dummyKernel(float* data) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    data[i] = data[i] * 2.0f;
}

int main() {
    int blockSize = 256;
    int numBlocks;
    cudaOccupancyMaxActiveBlocksPerMultiprocessor(&numBlocks, dummyKernel, blockSize, 0);

    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);

    int activeWarps = numBlocks * blockSize / 32;
    int maxWarps = prop.maxThreadsPerMultiProcessor / 32;
    float occupancy = (float)activeWarps / maxWarps;

    printf("GPU: %s\n", prop.name);
    printf("blockSize = %d\n", blockSize);
    printf("blocks residentes por SM (API real) = %d\n", numBlocks);
    printf("warps ativos por SM = %d de %d possiveis\n", activeWarps, maxWarps);
    printf("occupancy teorica = %.1f%%\n", occupancy * 100);
    return 0;
}
