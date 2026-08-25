#include <cstdio>
#include <cstdlib>
#include <cuda_runtime.h>

#define CUDA_CHECK(call) do {                                                \
    cudaError_t err = (call);                                                \
    if (err != cudaSuccess) {                                                \
        std::fprintf(stderr, "CUDA error %s:%d: %s\n", __FILE__, __LINE__,  \
                     cudaGetErrorString(err));                               \
        std::exit(EXIT_FAILURE);                                             \
    }                                                                        \
} while (0)

int main() {
    int device = 0;
    CUDA_CHECK(cudaGetDevice(&device));

    cudaDeviceProp p{};
    CUDA_CHECK(cudaGetDeviceProperties(&p, device));

    const double theoretical_gb_s =
        2.0 * static_cast<double>(p.memoryClockRate) * 1000.0 *
        static_cast<double>(p.memoryBusWidth) / 8.0 / 1e9;

    std::printf("GPU: %s\n", p.name);
    std::printf("Compute capability: %d.%d (compile target: sm_%d%d)\n",
                p.major, p.minor, p.major, p.minor);
    std::printf("Global memory: %.2f GiB\n", p.totalGlobalMem / 1073741824.0);
    std::printf("SMs: %d\n", p.multiProcessorCount);
    std::printf("Warp size: %d\n", p.warpSize);
    std::printf("Max threads/block: %d\n", p.maxThreadsPerBlock);
    std::printf("Max threads/SM: %d (%d warps)\n",
                p.maxThreadsPerMultiProcessor,
                p.maxThreadsPerMultiProcessor / p.warpSize);
    std::printf("Max blocks/SM: %d\n", p.maxBlocksPerMultiProcessor);
    std::printf("Registers/SM (32-bit): %d\n", p.regsPerMultiprocessor);
    std::printf("Registers/block: %d\n", p.regsPerBlock);
    std::printf("Shared memory/SM: %zu B\n", p.sharedMemPerMultiprocessor);
    std::printf("Shared memory/block default: %zu B\n", p.sharedMemPerBlock);
    std::printf("Shared memory/block opt-in: %zu B\n", p.sharedMemPerBlockOptin);
    std::printf("L2 cache: %d B\n", p.l2CacheSize);
    std::printf("Memory bus: %d bits\n", p.memoryBusWidth);
    std::printf("Memory clock reported: %d kHz\n", p.memoryClockRate);
    std::printf("Bandwidth derived from reported clock: %.2f GB/s (theoretical, not measured)\n",
                theoretical_gb_s);
    return 0;
}

