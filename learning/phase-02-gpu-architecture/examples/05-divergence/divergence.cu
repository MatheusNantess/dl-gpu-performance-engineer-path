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

__device__ __noinline__ float path_a(float x) {
    #pragma unroll 1
    for (int i = 0; i < 128; ++i) x = x * 1.000001f + 0.000003f;
    return x;
}

__device__ __noinline__ float path_b(float x) {
    #pragma unroll 1
    for (int i = 0; i < 128; ++i) x = x * 0.999999f - 0.000002f;
    return x;
}

__global__ void branch_kernel(const float* in, float* out, size_t n,
                              bool divergent) {
    size_t i = static_cast<size_t>(blockIdx.x) * blockDim.x + threadIdx.x;
    if (i >= n) return;
    unsigned selector = divergent ? (threadIdx.x & 1u)
                                  : ((threadIdx.x / warpSize) & 1u);
    out[i] = selector ? path_a(in[i]) : path_b(in[i]);
}

static float median(std::vector<float> v) {
    std::sort(v.begin(), v.end());
    return v[v.size() / 2];
}

static float measure(bool divergent, const float* in, float* out, size_t n,
                     cudaEvent_t start, cudaEvent_t stop) {
    int block = 256;
    int grid = static_cast<int>((n + block - 1) / block);
    for (int i = 0; i < 5; ++i)
        branch_kernel<<<grid, block>>>(in, out, n, divergent);
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    std::vector<float> times;
    for (int i = 0; i < 21; ++i) {
        CUDA_CHECK(cudaEventRecord(start));
        branch_kernel<<<grid, block>>>(in, out, n, divergent);
        CUDA_CHECK(cudaEventRecord(stop));
        CUDA_CHECK(cudaEventSynchronize(stop));
        float ms = 0.0f;
        CUDA_CHECK(cudaEventElapsedTime(&ms, start, stop));
        times.push_back(ms);
    }
    return median(times);
}

int main() {
    constexpr size_t n = 64ull * 1024 * 1024;
    float *in = nullptr, *out = nullptr;
    CUDA_CHECK(cudaMalloc(&in, n * sizeof(float)));
    CUDA_CHECK(cudaMalloc(&out, n * sizeof(float)));
    CUDA_CHECK(cudaMemset(in, 0, n * sizeof(float)));

    cudaEvent_t start, stop;
    CUDA_CHECK(cudaEventCreate(&start));
    CUDA_CHECK(cudaEventCreate(&stop));
    float uniform_ms = measure(false, in, out, n, start, stop);
    float divergent_ms = measure(true, in, out, n, start, stop);

    std::printf("uniform_by_warp_ms: %.4f\n", uniform_ms);
    std::printf("divergent_by_lane_ms: %.4f\n", divergent_ms);
    std::printf("divergent/uniform: %.3fx\n", divergent_ms / uniform_ms);

    float sink = 0.0f;
    CUDA_CHECK(cudaMemcpy(&sink, out + n / 2, sizeof(float), cudaMemcpyDeviceToHost));
    std::printf("sink: %.7f\n", sink);
    CUDA_CHECK(cudaEventDestroy(start));
    CUDA_CHECK(cudaEventDestroy(stop));
    CUDA_CHECK(cudaFree(in));
    CUDA_CHECK(cudaFree(out));
    return 0;
}

