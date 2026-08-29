#include <cstdio>

// Capitulo 06. Compile com -Xptxas -v (veja README) e compare a saida do
// ptxas para os dois kernels: numero de registradores usados e "bytes
// spill stores/loads". manyRegs le 64 valores reais da memoria (o
// compilador nao pode "dobrar" isso em constantes) e depois combina cada
// par (r[i], r[i+1]) -- isso forca os 64 valores a ficarem vivos ao mesmo
// tempo, criando pressao de registrador de verdade.

__global__ void fewRegs(float* out, float a) {
    float r0 = a + 1.0f;
    float r1 = r0 * 2.0f;
    float r2 = r1 - 3.0f;
    out[threadIdx.x] = r0 + r1 + r2;
}

__global__ void manyRegs(float* in, float* out) {
    float r[64];
    #pragma unroll
    for (int i = 0; i < 64; i++) {
        r[i] = in[threadIdx.x * 64 + i] * 1.0001f + i; // dado real, nao constante
    }
    float sum = 0.0f;
    #pragma unroll
    for (int i = 0; i < 64; i++) {
        sum += r[i] * r[(i + 1) % 64]; // termo cruzado: todos os 64 precisam estar vivos juntos
    }
    out[threadIdx.x] = sum;
}

int main() {
    float* d_out;
    float* d_in;
    cudaMalloc(&d_out, 256 * sizeof(float));
    cudaMalloc(&d_in, 256 * 64 * sizeof(float));
    cudaMemset(d_in, 0, 256 * 64 * sizeof(float));

    fewRegs<<<1, 256>>>(d_out, 1.0f);
    manyRegs<<<1, 256>>>(d_in, d_out);
    cudaDeviceSynchronize();

    cudaFree(d_out);
    cudaFree(d_in);
    printf("Rode a COMPILACAO com -Xptxas -v (nao a execucao) e compare fewRegs vs manyRegs:\n");
    printf("procure a linha 'Used N registers' (na RTX 3060: 10 vs 39).\n");
    printf("Experimente tambem --maxrregcount=16 (ou menor) para tentar forcar spill visivel\n");
    printf("em 'bytes spill stores/loads' -- o compilador as vezes resiste ate cortar bem fundo.\n");
    return 0;
}
