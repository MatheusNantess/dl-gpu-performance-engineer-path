#include <cstdio>
#include <cuda_runtime.h>
#define N 1000000 //número de elementos nas arrays
#define BLOCK_SIZE 32
#define BLOCK_DIM 256
// One block, 64 threads -> exactly 2 warps. Small on purpose: the printf
// output is meant to be read line by line, not benchmarked.

void vector_add_cpu(float *a, float *b, float *c, int n) {
    for (int i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

__global__ void vectorAdd_gpu(const float* a, const float* b, float* c, int n) {
    int index_thread = threadIdx.x + blockDim.x * blockIdx.x;
    if(index_thread < n){
        c[index_thread] = a[index_thread] + b[index_thread];
    }

}
void init_vector(float * array,  int n){
    for(int i = 0; i < n; i++){
        array[i] = (float)rand() / RAND_MAX;
    }
}

// Function to measure execution time
double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

// 1: declara h_a, h_b, h_c, h_c_ref (host) e d_a, d_b, d_c (device)
// 2: malloc dos 4 do host
// 3: cudaMalloc dos 3 do device — cudaMalloc((void**)&d_a, bytes)
// 4: init_vector em h_a e h_b — roda na CPU, C puro
// 5: cudaMemcpy HostToDevice (h_a->d_a, h_b->d_b)
// 6: threads=256, blocks=(n+threads-1)/threads
// 7: lanca o kernel + cudaGetLastError() + cudaDeviceSynchronize()
// 8: cudaMemcpy DeviceToHost (d_c->h_c)
// 9: h_c_ref[i]=h_a[i]+h_b[i] na CPU, compara com fabs(...) > 1e-5f
// 10: free x4 (host), cudaFree x3 (device)
int main() {
    float * h_a, *h_b, *h_c_cpu, *h_c_gpu;
    float * d_a, *d_b, *d_c;

    size_t size = N * sizeof(float);

    h_a = (float*)malloc(size);
    h_b = (float*)malloc(size);
    h_c_cpu = (float*)malloc(size); 
    h_c_gpu = (float*)malloc(size);

    srand(time(NULL));
    init_vector(h_a, N);
    init_vector(h_b, N);

    cudaMalloc(&d_a, size);
    cudaMalloc(&d_b, size); 
    cudaMalloc(&d_c, size);

    cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

    //blockDim = numero blocos no dim
    int num_blocks = (N * BLOCK_SIZE -1 /BLOCK_SIZE);
    
    // Warm-up runs
    printf("Performing warm-up runs...\n");
    for (int i = 0; i < 3; i++) {
        vector_add_cpu(h_a, h_b, h_c_cpu, N);
        vectorAdd_gpu<<<num_blocks, BLOCK_SIZE>>>(d_a, d_b, d_c, N);
        cudaDeviceSynchronize();
    }

    // Benchmark CPU implementation
    printf("Benchmarking CPU implementation...\n");
    double cpu_total_time = 0.0;
    for (int i = 0; i < 20; i++) {
        double start_time = get_time();
        vector_add_cpu(h_a, h_b, h_c_cpu, N);
        double end_time = get_time();
        cpu_total_time += end_time - start_time;
    }
    double cpu_avg_time = cpu_total_time / 20.0;
    
    // Benchmark GPU implementation
    printf("Benchmarking GPU implementation...\n");
    double gpu_total_time = 0.0;
    for (int i = 0; i < 20; i++) {
        double start_time = get_time();
        vectorAdd_gpu<<<num_blocks, BLOCK_SIZE>>>(d_a, d_b, d_c, N);
        cudaDeviceSynchronize();
        double end_time = get_time();
        gpu_total_time += end_time - start_time;
    }
    double gpu_avg_time = gpu_total_time / 20.0;

    // Print results
    printf("CPU average time: %f milliseconds\n", cpu_avg_time*1000);
    printf("GPU average time: %f milliseconds\n", gpu_avg_time*1000);
    printf("Speedup: %fx\n", cpu_avg_time / gpu_avg_time);

    // Verify results (optional)
    cudaMemcpy(h_c_gpu, d_c, size, cudaMemcpyDeviceToHost);
    bool correct = true;
    for (int i = 0; i < N; i++) {
        if (fabs(h_c_cpu[i] - h_c_gpu[i]) > 1e-5) {
            correct = false;
            break;
        }
    }
    printf("Results are %s\n", correct ? "correct" : "incorrect");

    // Free memory
    free(h_a);
    free(h_b);
    free(h_c_cpu);
    free(h_c_gpu);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

}
