#include <cstdio>

// Capitulo 08. Compara o mesmo trabalho por-elemento rodando com poucos
// warps residentes (1 block) vs muitos warps espalhados pela GPU inteira
// (grid cheio). O throughput por warp deve ser muito maior no segundo
// caso porque ha warps suficientes prontos pra esconder a latencia de
// memoria uns dos outros.

#define N (1 << 22)

__global__ void touchMemory(float* data, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        data[i] = data[i] * 2.0f + 1.0f;
    }
}

int main() {
    float* d_data;
    cudaMalloc(&d_data, N * sizeof(float));
    cudaMemset(d_data, 0, N * sizeof(float));

    cudaEvent_t start, stop;
    cudaEventCreate(&start); cudaEventCreate(&stop);
    float ms;

    touchMemory<<<1, 32>>>(d_data, N); cudaDeviceSynchronize();
    cudaEventRecord(start);
    for (int r = 0; r < 100; r++) touchMemory<<<1, 32>>>(d_data, N);
    cudaEventRecord(stop); cudaEventSynchronize(stop);
    cudaEventElapsedTime(&ms, start, stop);
    printf("1 block (1 warp) x100 launches: %.3f ms\n", ms);

    int blockSize = 256;
    int gridSize = (N + blockSize - 1) / blockSize;
    touchMemory<<<gridSize, blockSize>>>(d_data, N); cudaDeviceSynchronize();
    cudaEventRecord(start);
    for (int r = 0; r < 100; r++) touchMemory<<<gridSize, blockSize>>>(d_data, N);
    cudaEventRecord(stop); cudaEventSynchronize(stop);
    cudaEventElapsedTime(&ms, start, stop);
    printf("grid cheio (%d blocks) x100 launches: %.3f ms (processa %dx mais dados por launch)\n",
           gridSize, ms, gridSize);

    cudaFree(d_data);
    return 0;
}
