#include <stdio.h>
#include <cuda_runtime.h>
#define TILE_SIZE 16

__global__ void tilledMatmul(const float * A, const float *B, float * C, int M, int K, int N){
    //DECLARAÇÃO DOS TILES EM SHARED MEMORY. nesse caso usarei 16x16
    __shared__ float A_tile[TILE_SIZE][TILE_SIZE];
    __shared__ float B_tile[TILE_SIZE][TILE_SIZE];




    //row no eixo y pois o y nos warps muda muito menos. A execução de warps acontece inteira pelas threads do eixo x e dps avançam
    //pro próximo "andar" digamos
    //declaração do índice global das rows de A e do index global das cols de B 
    int row = threadIdx.y + blockDim.y * blockIdx.y;
    int col = threadIdx.x + blockDim.x * blockIdx.x; 
    int tx = threadIdx.x;
    int ty = threadIdx.y;


    //pra saber quanto tiles vao caber dentro de A e B 
    int number_tiles = (K + TILE_SIZE -1 )/TILE_SIZE;
    float soma = 0;

    for(int t = 0; t < number_tiles; t++){
        //para saber em qual index determinada row ou column do tile começará
        //usamos essa formual multiplicando o index do tile pelo tamanho dele 
        //logo, qnd t for 0, saberemos que o index de rows de A_tile começara em 0
        //quando t for 1, a coluna do proximo tile começara em 16
        int tile_start = t * TILE_SIZE;

        int a_tile_index_cols = threadIdx.x + tile_start;
        int b_tile_index_rows = threadIdx.y + tile_start;
    

        if(row < M   &&  a_tile_index_cols < K ){
            A_tile[ty][tx] = A[row * K + a_tile_index_cols];

        } else {
            //caso o index passe de um dos dois casos, ele adiciona meio q um padding no tile
            A_tile[ty][tx] = 0.0f;
        } 

        if(b_tile_index_rows < K   && col < N){
            B_tile[ty][tx] = B[b_tile_index_rows * N + col];

        } else {
            //caso o index passe de um dos dois casos, ele adiciona meio q um padding no tile
            B_tile[ty][tx] = 0.0f;

        }
        //evita race condition, por exemplo pode ser que n tenha sido carregad um elemento de A_tile ou B_tile
        //na hr da multiplicação como ele n existe será multiplicado um valor que não queremos.
        //syncthreads evita isso justamente isso, ele espera todas as threads terminarem de caregar os elementos
        //de A_tile e B_tile pra poder continuar.
        __syncthreads();


        for(int i = 0; i < TILE_SIZE; i++){
            //aqui estamos iterando sobre o numero de multiplicações para resultar em um elemento de C
            //logo, como em matmul multiplicamos linha de A por coluna de B
            //podemos perceber que em A a mesma thread que cuida das rows será a msm variando a coluna em I
            //Em B como usamos as colunas, o que vai variar são as linhas, logo o index i vai variar em linhas, enquanto as threads de X permanecem
    
            soma = soma + A_tile[ty][i] * B_tile[i][tx];
        }
        //evita race condition tbm. Mas nesse caso imagina que ainda estamos multiplicando o tile 0
        //poderia acontecer de algma thread avançando para o tile 1 e sobrescrever 
        // um valor do tile 0 com um valor do tile 1 antes de todas as threads terminarem de usar o tile 0.
        __syncthreads();


    }
    if(row < M && col < N){
        C[row * N + col] = soma;
    }


}

int main(){
    float * h_A, *h_B, *h_C; 
    float * d_A, *d_B, *d_C;
    int M = 50
    int K = 40
    int N = 60

    size_t size_a = M*K * sizeof(h_a);
    size_t size_b = K*N * sizeof(h_b);
    size_t size_c = M*N * sizeof(h_c);


    h_a = (float*)malloc(size_a);
    h_b = (float*)malloc(size_b);
    h_c = (float*)malloc(size_c);

    thread

}
