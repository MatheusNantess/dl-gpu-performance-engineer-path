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

__global__ void stride_copy(const float* __restrict__ in,
                            float* __restrict__ out,
                            size_t n, int stride) {
    size_t i = static_cast<size_t>(blockIdx.x) * blockDim.x + threadIdx.x;
    if (i < n) out[i] = in[i * static_cast<size_t>(stride)];
}

static float median(std::vector<float> v) {
    std::sort(v.begin(), v.end());
    return v[v.size() / 2];
}

int main() {
    constexpr size_t n = 8ull * 1024 * 1024;
    constexpr int max_stride = 32;
    constexpr int warmup = 5;
    constexpr int repeats = 20;
    const int strides[] = {1, 2, 4, 8, 16, 32};

    float *in = nullptr, *out = nullptr;
    CUDA_CHECK(cudaMalloc(&in, n * max_stride * sizeof(float)));
    CUDA_CHECK(cudaMalloc(&out, n * sizeof(float)));
    CUDA_CHECK(cudaMemset(in, 0, n * max_stride * sizeof(float)));

    int block = 256;
    int grid = static_cast<int>((n + block - 1) / block);
    cudaEvent_t start, stop;
    CUDA_CHECK(cudaEventCreate(&start));
    CUDA_CHECK(cudaEventCreate(&stop));

    std::printf("stride\tmedian_ms\tuseful_GB/s\n");
    for (int stride : strides) {
        for (int i = 0; i < warmup; ++i)
            stride_copy<<<grid, block>>>(in, out, n, stride);
        CUDA_CHECK(cudaGetLastError());
        CUDA_CHECK(cudaDeviceSynchronize());

        std::vector<float> times;
        for (int r = 0; r < repeats; ++r) {
            CUDA_CHECK(cudaEventRecord(start));
            stride_copy<<<grid, block>>>(in, out, n, stride);
            CUDA_CHECK(cudaEventRecord(stop));
            CUDA_CHECK(cudaEventSynchronize(stop));
            float ms = 0.0f;
            CUDA_CHECK(cudaEventElapsedTime(&ms, start, stop));
            times.push_back(ms);
        }
        float ms = median(times);
        double useful_gb_s = (n * 2.0 * sizeof(float)) / (ms * 1e-3) / 1e9;
        std::printf("%d\t%.4f\t\t%.2f\n", stride, ms, useful_gb_s);
    }

    float sink = 0.0f;
    CUDA_CHECK(cudaMemcpy(&sink, out + n / 2, sizeof(float), cudaMemcpyDeviceToHost));
    std::printf("sink: %.1f\n", sink);
    CUDA_CHECK(cudaEventDestroy(start));
    CUDA_CHECK(cudaEventDestroy(stop));
    CUDA_CHECK(cudaFree(in));
    CUDA_CHECK(cudaFree(out));
    return 0;
}

