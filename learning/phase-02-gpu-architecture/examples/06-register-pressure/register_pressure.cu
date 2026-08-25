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

template <int NACC>
__global__ void accumulator_kernel(const float* in, float* out, size_t n) {
    size_t tid = static_cast<size_t>(blockIdx.x) * blockDim.x + threadIdx.x;
    if (tid >= n) return;

    float acc[NACC];
    #pragma unroll
    for (int j = 0; j < NACC; ++j)
        acc[j] = in[(tid + static_cast<size_t>(j) * 32) % n] + j * 0.0001f;

    #pragma unroll 1
    for (int iter = 0; iter < 32; ++iter) {
        #pragma unroll
        for (int j = 0; j < NACC; ++j)
            acc[j] = acc[j] * 1.000001f + (j + 1) * 0.000001f;
    }

    float sum = 0.0f;
    #pragma unroll
    for (int j = 0; j < NACC; ++j) sum += acc[j];
    out[tid] = sum;
}

static float median(std::vector<float> v) {
    std::sort(v.begin(), v.end());
    return v[v.size() / 2];
}

template <int NACC>
static void run_case(const float* in, float* out, size_t n,
                     cudaEvent_t start, cudaEvent_t stop, int max_warps) {
    constexpr int block = 256;
    int grid = static_cast<int>((n + block - 1) / block);
    int active_blocks = 0;
    CUDA_CHECK(cudaOccupancyMaxActiveBlocksPerMultiprocessor(
        &active_blocks, accumulator_kernel<NACC>, block, 0));
    double occupancy = (active_blocks * block / 32.0) / max_warps;

    for (int i = 0; i < 5; ++i)
        accumulator_kernel<NACC><<<grid, block>>>(in, out, n);
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    std::vector<float> times;
    for (int i = 0; i < 21; ++i) {
        CUDA_CHECK(cudaEventRecord(start));
        accumulator_kernel<NACC><<<grid, block>>>(in, out, n);
        CUDA_CHECK(cudaEventRecord(stop));
        CUDA_CHECK(cudaEventSynchronize(stop));
        float ms = 0.0f;
        CUDA_CHECK(cudaEventElapsedTime(&ms, start, stop));
        times.push_back(ms);
    }
    std::printf("%d\t%d\t\t%.1f%%\t\t%.4f\n",
                NACC, active_blocks, occupancy * 100.0, median(times));
}

int main() {
    constexpr size_t n = 16ull * 1024 * 1024;
    float *in = nullptr, *out = nullptr;
    CUDA_CHECK(cudaMalloc(&in, n * sizeof(float)));
    CUDA_CHECK(cudaMalloc(&out, n * sizeof(float)));
    CUDA_CHECK(cudaMemset(in, 0, n * sizeof(float)));

    cudaDeviceProp p{};
    CUDA_CHECK(cudaGetDeviceProperties(&p, 0));
    int max_warps = p.maxThreadsPerMultiProcessor / p.warpSize;
    cudaEvent_t start, stop;
    CUDA_CHECK(cudaEventCreate(&start));
    CUDA_CHECK(cudaEventCreate(&stop));

    std::printf("accumulators\tblocks/SM\toccupancy\tmedian_ms\n");
    run_case<8>(in, out, n, start, stop, max_warps);
    run_case<16>(in, out, n, start, stop, max_warps);
    run_case<32>(in, out, n, start, stop, max_warps);
    run_case<64>(in, out, n, start, stop, max_warps);

    float sink = 0.0f;
    CUDA_CHECK(cudaMemcpy(&sink, out + n / 2, sizeof(float), cudaMemcpyDeviceToHost));
    std::printf("sink: %.7f\n", sink);
    CUDA_CHECK(cudaEventDestroy(start));
    CUDA_CHECK(cudaEventDestroy(stop));
    CUDA_CHECK(cudaFree(in));
    CUDA_CHECK(cudaFree(out));
    return 0;
}

