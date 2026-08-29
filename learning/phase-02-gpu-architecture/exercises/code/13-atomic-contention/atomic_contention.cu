#include <cstdio>

// Capitulo 11. contendedAtomic: 1 milhao de threads disputam o MESMO
// endereco -- serializacao total. shardedAtomic: a disputa e espalhada
// entre 32 contadores (shards), reduzindo contencao; soma-se os shards no
// host no final. Ambos dao o resultado numerico certo -- a diferenca e so
// o tempo.

#define N (1 << 20)
#define THREADS 256

__global__ void contendedAtomic(int* counter) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < N) atomicAdd(counter, 1);
}

__global__ void shardedAtomic(int* counters, int numShards) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < N) {
        int shard = threadIdx.x % numShards;
        atomicAdd(&counters[shard], 1);
    }
}

int main() {
    int* d_counter;
    int* d_shards;
    int numShards = 32;
    cudaMalloc(&d_counter, sizeof(int));
    cudaMalloc(&d_shards, numShards * sizeof(int));

    int blocks = (N + THREADS - 1) / THREADS;
    cudaEvent_t start, stop;
    cudaEventCreate(&start); cudaEventCreate(&stop);
    float ms;

    cudaMemset(d_counter, 0, sizeof(int));
    cudaEventRecord(start);
    contendedAtomic<<<blocks, THREADS>>>(d_counter);
    cudaEventRecord(stop); cudaEventSynchronize(stop);
    cudaEventElapsedTime(&ms, start, stop);
    int result;
    cudaMemcpy(&result, d_counter, sizeof(int), cudaMemcpyDeviceToHost);
    printf("atomic contendido (1 contador p/ %d threads): %.4f ms, resultado=%d\n", N, ms, result);

    cudaMemset(d_shards, 0, numShards * sizeof(int));
    cudaEventRecord(start);
    shardedAtomic<<<blocks, THREADS>>>(d_shards, numShards);
    cudaEventRecord(stop); cudaEventSynchronize(stop);
    cudaEventElapsedTime(&ms, start, stop);
    int shardResults[32], total = 0;
    cudaMemcpy(shardResults, d_shards, numShards * sizeof(int), cudaMemcpyDeviceToHost);
    for (int i = 0; i < numShards; i++) total += shardResults[i];
    printf("atomic em %d shards:                    %.4f ms, resultado total=%d\n", numShards, ms, total);

    cudaFree(d_counter);
    cudaFree(d_shards);
    return 0;
}
