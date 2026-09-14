#include <cstdio>
#include <cstdlib>
#include <cuda_runtime.h>

// Exercicio 01 (fundamentals/README.md): vector add.
// Objetivo: c[i] = a[i] + b[i] para n elementos, na GPU.
//
// Preencha os TODOs. Consulte cuda-lab/fundamentals/00-cuda-syntax-primer.md
// se travar em sintaxe. Nao existe "resposta errada" aqui alem de nao
// compilar/nao dar o resultado certo -- va tentando.

#define CUDA_CHECK(call) do {                                               \
    cudaError_t err = (call);                                               \
    if (err != cudaSuccess) {                                               \
        fprintf(stderr, "CUDA error %s:%d: %s\n", __FILE__, __LINE__,       \
                cudaGetErrorString(err));                                   \
        exit(EXIT_FAILURE);                                                 \
    }                                                                       \
} while (0)

// TODO 1: escreva o kernel.
// Assinatura sugerida: __global__ void vectorAdd(const float* a, const float* b, float* c, int n)
// Dentro: calcule o indice global da thread, cheque limite (i < n), some.


// TODO 2 (referencia de correcao, roda na CPU): mesma soma, sequencial.
// void vectorAddCPU(const float* a, const float* b, float* c, int n) { ... }


__global__ void vectorAdd(const float*a, const float*b, float* c, int n){
        for(int i = 0; i < n; i++){
            c[i] = a[i] + b[i];

        }

}


int main() {
    const int n = 1 << 20; // ~1M elementos
    size_t bytes = n * sizeof(float);
    float * h_a = new float[n];
    float* h_b = new float[n]; 
    float *h_c = new float[n];
    float * d_a ;
    float* d_b ; 
    float *d_c;
    //segundo argumento é quantos bytes alocar no ponteiro 
    CUDA_CHECK(cudaMalloc(&d_a, bytes));
    CUDA_CHECK(cudaMalloc(&d_b, bytes));
    CUDA_CHECK(cudaMalloc(&d_c, bytes));

    // TODO 3: aloque h_a, h_b, h_c (host, com `new float[n]`) e preencha
    // h_a[i] e h_b[i] com valores quaisquer (ex: i e i*2).

    // TODO 4: aloque d_a, d_b, d_c na GPU com cudaMalloc (use CUDA_CHECK).

    // TODO 5: copie h_a -> d_a e h_b -> d_b com cudaMemcpy (HostToDevice).

    // TODO 6: lance o kernel. Escolha um blockSize (ex: 256) e calcule
    // gridSize = (n + blockSize - 1) / blockSize.

    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    // TODO 7: copie d_c -> h_c (DeviceToHost).

    // TODO 8: valide contra uma referencia CPU (TODO 2) para uns 10
    // elementos aleatorios. Imprima "OK" ou "MISMATCH em i=...".

    // TODO 9: libere tudo (cudaFree para device, delete[] para host).

    return 0;
}
