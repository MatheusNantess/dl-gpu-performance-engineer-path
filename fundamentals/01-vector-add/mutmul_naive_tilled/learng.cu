#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#define TILE_SIZE 16

#define CUDA_CHECK(call) do { \
    cudaError_t err = (call); \
    if (err != cudaSuccess) { \
        fprintf(stderr, "CUDA error at %s:%d: %s\n", __FILE__, __LINE__, cudaGetErrorString(err)); \
        exit(1); \
    } \
} while (0)



__global__ void tilledMatmul(const float * A, const float *B, float * C, int M, int K, int N){
    // Buffers em shared memory (memória rápida, compartilhada por todas as threads do bloco).
    // Cada bloco carrega um "pedaço" (tile) 16x16 de A e um tile 16x16 de B por vez,
    // em vez de cada thread ler direto da global memory a cada multiplicação.
    __shared__ float A_tile[TILE_SIZE][TILE_SIZE];
    __shared__ float B_tile[TILE_SIZE][TILE_SIZE];

    // row no eixo y pois o y nos warps muda muito menos. A execução de warps acontece inteira pelas
    // threads do eixo x e dps avançam pro próximo "andar" digamos.
    // Índice global da linha de C/A que essa thread vai calcular/carregar.
    int row = threadIdx.y + blockDim.y * blockIdx.y;
    // Índice global da coluna de C/B que essa thread vai calcular/carregar.
    int col = threadIdx.x + blockDim.x * blockIdx.x;
    // Coordenadas locais da thread dentro do bloco (0..TILE_SIZE-1) — usadas pra indexar os tiles.
    int tx = threadIdx.x;
    int ty = threadIdx.y;

    // Quantos tiles de tamanho TILE_SIZE cabem na dimensão compartilhada K (colunas de A / linhas de B).
    // Arredonda pra cima (ceil) pra garantir que sobre um tile parcial no final se K não for múltiplo de TILE_SIZE.
    int number_tiles = (K + TILE_SIZE -1 )/TILE_SIZE;
    // Acumulador do produto escalar (linha de A) . (coluna de B) que vira C[row][col].
    float soma = 0;

    // Percorre a dimensão K em fatias de TILE_SIZE: em cada iteração processa um tile de A e um de B.
    for(int t = 0; t < number_tiles; t++){
        // Índice, dentro de K, onde o tile atual começa.
        // pra saber em qual index determinada row ou column do tile começará
        // usamos essa formula multiplicando o index do tile pelo tamanho dele
        // logo, qnd t for 0, saberemos que o index de rows de A_tile começara em 0
        // quando t for 1, a coluna do proximo tile começara em 16
        int tile_start = t * TILE_SIZE;

        // Coluna (em K) que essa thread vai ler de A dentro do tile atual.
        int a_tile_index_cols = threadIdx.x + tile_start;
        // Linha (em K) que essa thread vai ler de B dentro do tile atual.
        int b_tile_index_rows = threadIdx.y + tile_start;

        // Carrega um elemento de A pro shared memory: A[row][tile_start + tx].
        // Cada thread (ty, tx) do bloco carrega exatamente 1 elemento — juntas, preenchem o tile 16x16 inteiro.
        if(row < M   &&  a_tile_index_cols < K ){
            A_tile[ty][tx] = A[row * K + a_tile_index_cols];

        } else {
            // Fora dos limites de A (acontece quando M ou K não são múltiplos de TILE_SIZE):
            // caso o index passe de um dos dois casos, ele adiciona meio que um padding no tile
            // com zero, que não afeta a soma do produto escalar.
            A_tile[ty][tx] = 0.0f;
        }

        // Carrega um elemento de B pro shared memory: B[tile_start + ty][col].
        if(b_tile_index_rows < K   && col < N){
            B_tile[ty][tx] = B[b_tile_index_rows * N + col];

        } else {
            // caso o index passe de um dos dois casos, ele adiciona meio que um padding no tile
            B_tile[ty][tx] = 0.0f;

        }
        // evita race condition, por exemplo pode ser que n tenha sido carregado um elemento de A_tile ou B_tile
        // na hr da multiplicação como ele n existe será multiplicado um valor que não queremos.
        // syncthreads evita isso justamente isso, ele espera todas as threads terminarem de carregar os elementos
        // de A_tile e B_tile pra poder continuar.
        __syncthreads();

        // Com o tile inteiro já carregado em shared memory, cada thread calcula a contribuição
        // desse tile pro seu C[row][col]: soma_i A_tile[ty][i] * B_tile[i][tx].
        for(int i = 0; i < TILE_SIZE; i++){
            // aqui estamos iterando sobre o numero de multiplicações para resultar em um elemento de C
            // logo, como em matmul multiplicamos linha de A por coluna de B
            // podemos perceber que em A a mesma thread que cuida das rows será a msm variando a coluna em I
            // Em B como usamos as colunas, o que vai variar são as linhas, logo o index i vai variar em linhas,
            // enquanto as threads de X permanecem
            soma = soma + A_tile[ty][i] * B_tile[i][tx];
        }
        // evita race condition tbm. Mas nesse caso imagina que ainda estamos multiplicando o tile 0
        // poderia acontecer de alguma thread avançando para o tile 1 e sobrescrever
        // um valor do tile 0 com um valor do tile 1 antes de todas as threads terminarem de usar o tile 0.
        __syncthreads();

        // Próxima iteração do for: avança pro próximo tile (t+1), repetindo carga + multiplicação
        // até cobrir toda a dimensão K.
    }
    // Só escreve em C se essa thread cair dentro dos limites reais da matriz de saída (M x N) —
    // threads "extras" nas bordas (quando M/N não são múltiplos de TILE_SIZE) não escrevem nada.
    if(row < M && col < N){
        C[row * N + col] = soma;
    }

}


void init_matrix(float *mat, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        mat[i] = (float)rand() / RAND_MAX;
    }
}

int main(){

    float * h_a, *h_b, *h_c;
    float * d_a, *d_b, *d_c;
    int M = 4096;
    int K = 4096;
    int N = 4096;

    size_t size_a = M*K * sizeof(float);
    size_t size_b = K*N * sizeof(float);
    size_t size_c = M*N * sizeof(float);

    h_a = (float*)malloc(size_a);
    h_b = (float*)malloc(size_b);
    h_c = (float*)malloc(size_c);

    init_matrix(h_a, M, K);
    init_matrix(h_b, K, N);

    CUDA_CHECK(cudaMalloc(&d_a, size_a));
    CUDA_CHECK(cudaMalloc(&d_b, size_b));
    CUDA_CHECK(cudaMalloc(&d_c, size_c));

    CUDA_CHECK(cudaMemcpy(d_a, h_a, size_a, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_b, h_b, size_b, cudaMemcpyHostToDevice));

    dim3 block_dim(TILE_SIZE, TILE_SIZE);
    dim3 grid_size((N + TILE_SIZE - 1) / TILE_SIZE,
                   (M + TILE_SIZE - 1) / TILE_SIZE);

    tilledMatmul<<<grid_size, block_dim>>>(d_a, d_b, d_c, M, K, N);
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    CUDA_CHECK(cudaMemcpy(h_c, d_c, size_c, cudaMemcpyDeviceToHost));
    printf("C[0] = %f\n", h_c[0]);

    free(h_a);
    free(h_b);
    free(h_c);
    CUDA_CHECK(cudaFree(d_a));
    CUDA_CHECK(cudaFree(d_b));
    CUDA_CHECK(cudaFree(d_c));

    return 0;
}