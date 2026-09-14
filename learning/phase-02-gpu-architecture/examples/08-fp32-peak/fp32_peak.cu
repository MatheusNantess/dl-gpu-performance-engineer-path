#include <cstdio>
#include <cstdlib>
#include <cuda_runtime.h>

// Exercicio: medir o pico REAL de FP32 (FMA) da sua GPU.
// Objetivo: manter as unidades de FMA de cada SM ocupadas o tempo todo,
// sem gargalo de memoria, e calcular GFLOP/s = FLOPs totais / tempo.
//
// Preencha os TODOs na ordem -- cada um e pequeno de proposito. Se travar
// em algum, vai testando (nvcc reclama alto e claro). Va compilando a cada
// 2-3 TODOs, nao espere o arquivo inteiro pra tentar compilar pela primeira
// vez.

#define CUDA_CHECK(call) do {                                               \
    cudaError_t err = (call);                                               \
    if (err != cudaSuccess) {                                               \
        fprintf(stderr, "CUDA error %s:%d: %s\n", __FILE__, __LINE__,       \
                cudaGetErrorString(err));                                   \
        exit(EXIT_FAILURE);                                                 \
    }                                                                       \
} while (0)

#define NUM_ACC 8            // acumuladores independentes por thread (ILP)
#define ITERS 100000         // FMAs por acumulador, por thread
#define THREADS_PER_BLOCK 256
#define NUM_BLOCKS (28 * 32) // 28 SMs (RTX 3060) * fator de sobra -- nao
                              // precisa ser exato, so nao deixar SM ocioso

// TODO 1: escreva o kernel.
//
// Assinatura sugerida:
//   __global__ void fp32_peak(const float* seed, float* out, int iters)
//
// Passos dentro do kernel:
//   a) indice global da thread (voce ja sabe fazer isso, e' o de sempre)
//   b) declare NUM_ACC variaveis float, ex: `float acc[NUM_ACC];`
//      inicialize CADA UMA com um valor DIFERENTE derivado de
//      `seed[idx]` (ex: acc[j] = seed[idx] + j). Nao inicialize tudo com
//      literais fixos tipo 1.0f -- se o compilador ve so constantes, pode
//      simplificar/eliminar o calculo em tempo de compilacao.
//   c) `for (int i = 0; i < iters; i++) { ... }` -- ponha
//      `#pragma unroll` na linha de cima desse for, ajuda o compilador a
//      eliminar overhead de branch entre as FMAs.
//   d) dentro do loop, atualize CADA acumulador de forma INDEPENDENTE:
//        acc[j] = acc[j] * <multiplicador> + <somando>;
//      Use `#pragma unroll` num for interno sobre `j` tambem, ou escreva
//      as NUM_ACC linhas na mao. O ponto critico: acc[j] NUNCA pode
//      depender de acc[outro indice] -- cada cadeia tem que ser
//      independente das outras, senao volta a virar uma cadeia so
//      (latency-bound, nao throughput-bound).
//   e) depois do loop, some os NUM_ACC acumuladores e escreva o resultado
//      em `out[idx]`. Isso impede o compilador de descartar o trabalho
//      todo por "resultado nunca usado".


// TODO 2: escreva get_time() -- e a mesma funcao que voce ja tem no seu
// fundamentals/01-vector-add/vector_add.cu (clock_gettime CLOCK_MONOTONIC).
// Copie de la.


int main() {
    // TODO 3: aloque um array `h_seed` (host, tamanho NUM_BLOCKS *
    // THREADS_PER_BLOCK) e preencha com um valor float diferente por
    // posicao -- pode ser so o indice convertido: h_seed[i] = (float)i.
    // Isso e o que evita a constant-folding do TODO 1b.

    // TODO 4: aloque `d_seed` e `d_out` no device (cudaMalloc), do mesmo
    // tamanho (NUM_BLOCKS * THREADS_PER_BLOCK floats). Copie h_seed pra
    // d_seed (cudaMemcpy HostToDevice).

    // TODO 5: warm-up -- rode o kernel 2-3x com
    // fp32_peak<<<NUM_BLOCKS, THREADS_PER_BLOCK>>>(d_seed, d_out, ITERS);
    // seguido de cudaDeviceSynchronize(), SEM cronometrar. Voce ja sabe
    // por que faz warm-up.

    // TODO 6: cronometre N repeticoes (ex: 10) do kernel. Pra cada uma:
    // get_time() antes, lanca o kernel, cudaDeviceSynchronize() (senao
    // voce mede so o tempo de ENFILEIRAR o kernel, nao de executar),
    // get_time() depois. Acumule o tempo total, divida pelo N pra media.

    // TODO 7: calcule os FLOPs totais de UM lancamento do kernel:
    //   FLOPs = (long long)NUM_BLOCKS * THREADS_PER_BLOCK * ITERS * NUM_ACC * 2;
    // (2 porque cada FMA = 1 multiplicacao + 1 soma)
    // Calcule GFLOP/s = FLOPs / tempo_medio_segundos / 1e9 e imprima.

    // TODO 8: sanity check -- copie d_out pra h_out (cudaMemcpy
    // DeviceToHost) e imprima uns 2-3 valores. Confirme que nao e NaN/inf
    // (se os multiplicadores/somandos que voce escolheu no TODO 1d forem
    // ruins, os acumuladores podem divergir pra infinito rapido -- se
    // isso acontecer, ajuste os valores, ex: multiplicador proximo de 1.0
    // tipo 1.00001f).

    // TODO 9: libere memoria -- free() do host, cudaFree() do device.

    return 0;
}
