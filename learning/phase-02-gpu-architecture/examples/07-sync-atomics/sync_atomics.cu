#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cuda_runtime.h>

#define CUDA_CHECK(call) do {                                                \
    cudaError_t err = (call);                                                \
    if (err != cudaSuccess) {                                                \
        std::fprintf(stderr, "CUDA error %s:%d: %s\n", __FILE__, __LINE__,  \
                     cudaGetErrorString(err));                               \
        std::exit(EXIT_FAILURE);                                             \
    }                                                                        \
} while (0)

__global__ void barrier_kernel(unsigned long long* sink, int iterations) {
    unsigned long long x = threadIdx.x + 1;
    for (int i = 0; i < iterations; ++i) {
        __syncthreads();
        x += static_cast<unsigned long long>(i & 1);
    }
    if (threadIdx.x == 0) atomicAdd(sink, x);
}

__global__ void atomic_kernel(unsigned long long* counters, int iterations,
                              bool contended) {
    size_t global = static_cast<size_t>(blockIdx.x) * blockDim.x + threadIdx.x;
    size_t warp_global = global / warpSize;
    size_t target = contended ? 0 : warp_global;
    for (int i = 0; i < iterations; ++i) atomicAdd(&counters[target], 1ull);
}

static float time_barrier(int threads, int blocks, int iterations,
                          unsigned long long* counters,
                          cudaEvent_t start, cudaEvent_t stop) {
    CUDA_CHECK(cudaMemset(counters, 0, sizeof(unsigned long long)));
    CUDA_CHECK(cudaEventRecord(start));
    barrier_kernel<<<blocks, threads>>>(counters, iterations);
    CUDA_CHECK(cudaEventRecord(stop));
    CUDA_CHECK(cudaEventSynchronize(stop));
    CUDA_CHECK(cudaGetLastError());
    float ms = 0.0f;
    CUDA_CHECK(cudaEventElapsedTime(&ms, start, stop));
    return ms;
}

static float time_atomic(bool contended, int blocks, int threads, int iterations,
                         unsigned long long* counters, size_t counter_count,
                         cudaEvent_t start, cudaEvent_t stop) {
    CUDA_CHECK(cudaMemset(counters, 0, counter_count * sizeof(unsigned long long)));
    CUDA_CHECK(cudaEventRecord(start));
    atomic_kernel<<<blocks, threads>>>(counters, iterations, contended);
    CUDA_CHECK(cudaEventRecord(stop));
    CUDA_CHECK(cudaEventSynchronize(stop));
    CUDA_CHECK(cudaGetLastError());
    float ms = 0.0f;
    CUDA_CHECK(cudaEventElapsedTime(&ms, start, stop));
    return ms;
}

int main() {
    constexpr int blocks = 112;
    constexpr int barrier_iterations = 10000;
    constexpr int atomic_iterations = 256;
    constexpr int atomic_threads = 256;
    constexpr size_t max_counters = blocks * atomic_threads / 32;

    unsigned long long* counters = nullptr;
    CUDA_CHECK(cudaMalloc(&counters, max_counters * sizeof(unsigned long long)));
    cudaEvent_t start, stop;
    CUDA_CHECK(cudaEventCreate(&start));
    CUDA_CHECK(cudaEventCreate(&stop));

    std::printf("barriers (single run; %d blocks, %d barriers/block)\n",
                blocks, barrier_iterations);
    for (int threads : {32, 128, 256, 512}) {
        float ms = time_barrier(threads, blocks, barrier_iterations,
                                counters, start, stop);
        std::printf("threads=%d\ttotal_ms=%.4f\tapprox_ns/barrier=%.2f\n",
                    threads, ms, ms * 1e6 / barrier_iterations);
    }

    float contended_ms = time_atomic(true, blocks, atomic_threads,
                                     atomic_iterations, counters, max_counters,
                                     start, stop);
    unsigned long long result = 0;
    CUDA_CHECK(cudaMemcpy(&result, counters, sizeof(result), cudaMemcpyDeviceToHost));
    unsigned long long expected = static_cast<unsigned long long>(blocks) *
                                  atomic_threads * atomic_iterations;
    if (result != expected) {
        std::fprintf(stderr, "contended validation failed: %llu != %llu\n",
                     result, expected);
        return EXIT_FAILURE;
    }

    float sharded_ms = time_atomic(false, blocks, atomic_threads,
                                   atomic_iterations, counters, max_counters,
                                   start, stop);
    std::vector<unsigned long long> host(max_counters);
    CUDA_CHECK(cudaMemcpy(host.data(), counters,
                          max_counters * sizeof(unsigned long long),
                          cudaMemcpyDeviceToHost));
    unsigned long long sum = 0;
    for (auto value : host) sum += value;
    if (sum != expected) {
        std::fprintf(stderr, "sharded validation failed: %llu != %llu\n", sum, expected);
        return EXIT_FAILURE;
    }

    double operations = static_cast<double>(expected);
    std::printf("atomics (%llu total)\n", expected);
    std::printf("one_counter_ms: %.4f (%.2f Gatomic/s)\n",
                contended_ms, operations / (contended_ms * 1e-3) / 1e9);
    std::printf("one_counter_per_warp_ms: %.4f (%.2f Gatomic/s)\n",
                sharded_ms, operations / (sharded_ms * 1e-3) / 1e9);
    std::printf("contended/sharded: %.3fx\n", contended_ms / sharded_ms);

    CUDA_CHECK(cudaEventDestroy(start));
    CUDA_CHECK(cudaEventDestroy(stop));
    CUDA_CHECK(cudaFree(counters));
    return 0;
}

