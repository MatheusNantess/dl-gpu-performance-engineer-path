// Mede latencia real de acesso a memoria global da GPU usando pointer
// chasing -- mesma tecnica do latency_benchmark.cpp (exemplo 01), portada
// pra CUDA.
//
// Por que UMA thread so, nao milhares:
// O objetivo aqui e medir a latencia de UM acesso isolado, sem deixar o
// hardware esconder isso atras de outras requisicoes em voo (ver arquivo de
// teoria 05, secao 3 -- "latency hiding"). Se lancassemos milhares de
// threads, o Streaming Multiprocessor trocaria de warp toda vez que um
// ficasse esperando memoria, e o numero medido seria throughput agregado,
// nao latencia de um acesso. Por isso <<<1, 1>>>: uma cadeia de dependencia
// real, sem nenhuma outra requisicao pra sobrepor.
//
// Cada elemento do buffer guarda o indice do PROXIMO elemento a visitar
// (permutacao de Sattolo, ciclo unico) -- exatamente como no exemplo 01,
// pra impedir qualquer prefetch de hardware de adivinhar o proximo
// endereco.
//
// Compilar (WSL2, CUDA 12.9, ver docs/hardware/rtx-3060.md):
//   nvcc -O3 -arch=sm_86 -o latency_gpu latency_gpu.cu
// Rodar:
//   ./latency_gpu

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <random>
#include <cuda_runtime.h>

#define CUDA_CHECK(call)                                                     \
    do {                                                                     \
        cudaError_t err = (call);                                            \
        if (err != cudaSuccess) {                                            \
            fprintf(stderr, "CUDA error %s:%d: %s\n", __FILE__, __LINE__,    \
                    cudaGetErrorString(err));                                \
            exit(1);                                                        \
        }                                                                    \
    } while (0)

// Mesmo algoritmo do exemplo 01: permutacao aleatoria de ciclo unico, seed
// fixa para resultados reproduziveis entre execucoes.
static std::vector<int> build_chase_buffer(size_t n) {
    std::vector<size_t> perm(n);
    for (size_t i = 0; i < n; i++) perm[i] = i;

    std::mt19937_64 rng(12345);
    for (size_t i = n - 1; i >= 1; i--) {
        std::uniform_int_distribution<size_t> dist(0, i - 1);
        size_t j = dist(rng);
        std::swap(perm[i], perm[j]);
    }

    std::vector<int> buf(n);
    for (size_t i = 0; i < n; i++) buf[i] = static_cast<int>(perm[i]);
    return buf;
}

// Uma thread so, percorrendo a cadeia `iters` vezes. `sink` acumula o
// indice final para impedir o compilador de eliminar o loop inteiro.
__global__ void chase_kernel(const int* buf, size_t iters, int* sink) {
    int idx = 0;
    for (size_t i = 0; i < iters; i++) {
        idx = buf[idx];
    }
    *sink = idx;
}

int main() {
    // Tamanhos em KB: do pequeno (deve caber em L1/L2 por SM) ate bem maior
    // que os 2.25 MB de L2 da RTX 3060 (docs/hardware/rtx-3060.md) -- o
    // numero que interessa pro Little's Law e o do MAIOR buffer, que forca
    // acesso real a VRAM.
    const size_t sizes_kb[] = {16, 128, 2048, 8192, 65536, 262144};

    int* d_sink;
    CUDA_CHECK(cudaMalloc(&d_sink, sizeof(int)));

    cudaEvent_t start, stop;
    CUDA_CHECK(cudaEventCreate(&start));
    CUDA_CHECK(cudaEventCreate(&stop));

    printf("Tamanho (KB)\tLatencia media (ns/acesso)\n");
    printf("------------\t--------------------------\n");

    for (size_t kb : sizes_kb) {
        size_t n = (kb * 1024) / sizeof(int);
        std::vector<int> h_buf = build_chase_buffer(n);

        int* d_buf;
        CUDA_CHECK(cudaMalloc(&d_buf, n * sizeof(int)));
        CUDA_CHECK(cudaMemcpy(d_buf, h_buf.data(), n * sizeof(int), cudaMemcpyHostToDevice));

        // Aquecimento: primeira passada paga custo de kernel launch a frio
        // e garante que o buffer ja foi tocado (paginas de VRAM alocadas).
        chase_kernel<<<1, 1>>>(d_buf, n, d_sink);
        CUDA_CHECK(cudaDeviceSynchronize());

        // Medicao real: cadeia inteira repetida varias vezes pra amostra
        // estatisticamente razoavel (mesma logica do exemplo 01).
        size_t iters = n * 20;

        CUDA_CHECK(cudaEventRecord(start));
        chase_kernel<<<1, 1>>>(d_buf, iters, d_sink);
        CUDA_CHECK(cudaEventRecord(stop));
        CUDA_CHECK(cudaEventSynchronize(stop));

        float elapsed_ms = 0.0f;
        CUDA_CHECK(cudaEventElapsedTime(&elapsed_ms, start, stop));

        double ns_per_access = (elapsed_ms * 1e6) / static_cast<double>(iters);

        printf("%10zu\t%.3f\n", kb, ns_per_access);

        CUDA_CHECK(cudaFree(d_buf));
    }

    CUDA_CHECK(cudaEventDestroy(start));
    CUDA_CHECK(cudaEventDestroy(stop));
    CUDA_CHECK(cudaFree(d_sink));

    return 0;
}
