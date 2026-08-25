#include <algorithm>
#include <chrono>
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

__global__ void empty_kernel() {}

static double median(std::vector<double> v) {
    std::sort(v.begin(), v.end());
    return v[v.size() / 2];
}

int main() {
    constexpr int warmup = 100;
    constexpr int launches = 10000;
    constexpr int trials = 9;

    for (int i = 0; i < warmup; ++i) empty_kernel<<<1, 1>>>();
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    cudaEvent_t start, stop;
    CUDA_CHECK(cudaEventCreate(&start));
    CUDA_CHECK(cudaEventCreate(&stop));

    std::vector<double> device_us, host_batch_us, roundtrip_us;
    for (int t = 0; t < trials; ++t) {
        CUDA_CHECK(cudaEventRecord(start));
        auto host_start = std::chrono::steady_clock::now();
        for (int i = 0; i < launches; ++i) empty_kernel<<<1, 1>>>();
        CUDA_CHECK(cudaGetLastError());
        CUDA_CHECK(cudaEventRecord(stop));
        CUDA_CHECK(cudaEventSynchronize(stop));
        auto host_stop = std::chrono::steady_clock::now();

        float elapsed_ms = 0.0f;
        CUDA_CHECK(cudaEventElapsedTime(&elapsed_ms, start, stop));
        device_us.push_back(elapsed_ms * 1000.0 / launches);
        host_batch_us.push_back(
            std::chrono::duration<double, std::micro>(host_stop - host_start).count() /
            launches);

        auto sync_start = std::chrono::steady_clock::now();
        for (int i = 0; i < 1000; ++i) {
            empty_kernel<<<1, 1>>>();
            CUDA_CHECK(cudaDeviceSynchronize());
        }
        auto sync_stop = std::chrono::steady_clock::now();
        roundtrip_us.push_back(
            std::chrono::duration<double, std::micro>(sync_stop - sync_start).count() /
            1000.0);
    }

    std::printf("Trials: %d; batched launches/trial: %d\n", trials, launches);
    std::printf("Batched device timeline:      %.3f us/kernel (median)\n", median(device_us));
    std::printf("Host enqueue + batch drain:   %.3f us/kernel (median)\n", median(host_batch_us));
    std::printf("Launch + synchronize roundtrip: %.3f us/kernel (median)\n", median(roundtrip_us));

    CUDA_CHECK(cudaEventDestroy(start));
    CUDA_CHECK(cudaEventDestroy(stop));
    return 0;
}

