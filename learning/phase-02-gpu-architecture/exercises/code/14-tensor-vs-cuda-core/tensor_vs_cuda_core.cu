#include <cstdio>

// Capitulo 12. Conceitual (implementacao WMMA esta fora de escopo nesta
// fase). So imprime as specs reais da sua GPU e explica a diferenca de
// granularidade entre CUDA core e Tensor core.

int main() {
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);

    int coresPerSM = 128; // Ampere (sm_86); varia por arquitetura
    int totalCores = prop.multiProcessorCount * coresPerSM;

    printf("GPU: %s\n", prop.name);
    printf("SMs: %d\n", prop.multiProcessorCount);
    printf("CUDA cores (128/SM na Ampere): %d SMs x %d = %d\n",
           prop.multiProcessorCount, coresPerSM, totalCores);
    printf("\n");
    printf("Diferenca conceitual:\n");
    printf("- CUDA core: 1 operacao ESCALAR por ciclo (ex: 1 FMA = a*b+c em floats individuais)\n");
    printf("- Tensor core: 1 instrucao processa uma multiplicacao de MATRIZES pequenas inteira\n");
    printf("  (ex: blocos 4x4 ou maiores, dependendo da geracao), usado por baixo dos panos em\n");
    printf("  cuBLAS/cuDNN para GEMM, convolucao e attention.\n");
    printf("Implementar WMMA fica pra fase seguinte -- aqui e so o modelo mental.\n");
    return 0;
}
