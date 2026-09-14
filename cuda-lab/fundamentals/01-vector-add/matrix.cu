#include <cstdio>
#include <time.h>
#define M 256  // Number of rows in A and C
#define N 512   // Number of columns in A and rows in B
#define O 256 // Number of columns in B and C
#define BLOCK_SIZE 32  



//A: 3x2 e B: 2x4 C:3x4
//   mxn      nxo   mxo
                            
/*                             linha * largura + coluna        
Matriz A (3x2): A[0][0] ----> index = 0 x 2 + 0 = 0
[ 2  1 ]        A[0][1] ----> index = 0x2 +1 = 1
[ 5  4 ]        A[1][0] ----> index =  1x2 + 0= 2
[ 4  3 ]        A[1][1] ----> index =  1x2 + 1= 3

Matriz B (2x4):
[ 2  9  2  7 ]
[ 1  1  2  4 ]

multiplicação: A[0][0] * B[0][0] + A[0][1] * B[1][0]

*/


void matrixMultiplication(float *A, float* B, float* C, int m, int n, int o){
    //primeiro loop iteramos o numero de vezes do numero de linhas da primeira matriz
    for(int i = 0; i <  m; i++){
        //segundo loop iteramos o numero de vezes do numero de colunas da segunda (ambos vão gerar o numero de elementos totais da matriz)
        //se A for dimensão 3x2 e B for dimensão 2x4 logo C terá 3x4 = 12 elementos
        for(int j = 0; j < o; j++){
            //a variavel soma serve pra literalmente guardar o resultado da soma das multiplicações dos numeros
            float soma = 0;
            //novamente pensando em A e B com as dimensões de exemplo ja ditas, podemos perceber que os resultados de cada elemento fica: 
            //ex: primeiro elemento vai ser A[0][0] * B[0][0] + A[0][1] * B[1][0] 
            // ou seja o terceiro loop tem q iterar o mesmo numero de multiplicações que resultam em um elemento, nesse caso 2 vezes 

            for(int k = 0; k < n; k++){
                //como tensores n conseguer ser escritos nas dimensões que possuem, em C++ achatamos eles para formarem um array linear na memória
                //a formula necessária para isso é: indice da linha * largura (numero de colunas) + indice da coluna
                //na matriz A o índice de linhas é i (estabelecido no primeiro loop) n é o número de colunas que A possui e k o índice de colunas
                //na matriz B o índice de linhas é k, o número de colunas é "o", e o índice de colunas é j (estabelecido no segundo loop)

                soma = soma + A[i * n + k] * B[k * o + j] ;
            }
        //dimensões resultantes de C no nosso exemplo é 3x4. Ou seja, o índice de linhas de C é o mesmo de A que é "i"
        //O número de colunas de C é "o" e o índice de colunas de C é j (refente ao segundo loop)
        C[i* o + j] = soma;
        }

    }
    
}
__global__ void GpuMatrixMultiplication(float *A, float *B, float *C, int m, int n, int o){
    //por algum motivo parece que deixa a coluna nas threads do eixo x parece que escala melhor a coalescencia da memorai
    //col varia dentro de uma linha ou array, 
    //indice global das threads que vão processar as linhas no eixo y
    int row = threadIdx.y + blockDim.y * blockIdx.y;
     //indice global das threads que vão processar as colunas no eixo x
    int col = threadIdx.x + blockDim.x * blockIdx.x;

    //esse if faz o trabalho dos 2 loops iniciais dentro do c++ padrão no algoritmo de GEMM
    if(row < m && col <o){
        float sum = 0;
        //lembrando que pra matriz A, o "i" é o índice das colunas e para a B, o "i" é o índice das linhas
        for(int i = 0; i < n; i++){
            sum = sum + A[row* n + i] * B[i* o + col];
        }
        C[row*o + col] = sum ;
    };
}

void init_matrix(float *mat, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        mat[i] = (float)rand() / RAND_MAX;
    }
}

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}


int main(){

    float * h_A, *h_B, *h_C;
    float * d_A, *d_B, *d_C;



    size_t size_a = M*N * sizeof(float);
    size_t size_b = N*O * sizeof(float);
    size_t size_c = M*O * sizeof(float);


    h_A = (float*)malloc(size_a);
    h_B = (float*)malloc(size_b);
    h_C = (float*)malloc(size_c);

    //inicia os vetores nas variaveis da cpu usando a função init_vector

    //seed aleatoria para a randomização
    srand(time(NULL));
    init_matrix(h_A, M, N);
    init_matrix(h_B, N, O);
    init_matrix(h_C, M, O);

    //aloca as variaveis na memoria (por enquanto sem o conteudo)
    cudaMalloc(&d_A, size_a);
    cudaMalloc(&d_B, size_b);
    cudaMalloc(&d_C, size_c);

    //copia o conteudo das variaveis de host para as variaveis da gpu
    cudaMemcpy(d_A, h_A, size_a, cudaMemcpyHostToDevice);   // CERTO -- repara "size_a" minusculo tambem
    cudaMemcpy(d_B, h_B, size_b, cudaMemcpyHostToDevice);

    // number of threads in x and y axis
    dim3 blockDim(BLOCK_SIZE, BLOCK_SIZE);
    // Number of blocks in x and y axis (it has to be the same as inside the function)
    dim3 gridDim((O + BLOCK_SIZE - 1) / BLOCK_SIZE, (M + BLOCK_SIZE - 1) / BLOCK_SIZE);

    // Warm-up
    printf("Performing warm-up runs...\n");
    for (int i = 0; i < 3; i++) {
        matrixMultiplication(h_A, h_B, h_C, M, N, O);
        GpuMatrixMultiplication<<<gridDim, blockDim>>>(d_A, d_B, d_C, M, N, O);
        cudaDeviceSynchronize();
    }

    // Benchmark CPU
    double cpu_total_time = 0.0;
    for (int i = 0; i < 20; i++) {
        double start_time = get_time();
        matrixMultiplication(h_A, h_B, h_C, M, N, O);
        double end_time = get_time();
        cpu_total_time += end_time - start_time;
    }
    double cpu_avg_time = cpu_total_time / 20.0;

    // Benchmark GPU
    double gpu_total_time = 0.0;
    for (int i = 0; i < 20; i++) {
        double start_time = get_time();
        GpuMatrixMultiplication<<<gridDim, blockDim>>>(d_A, d_B, d_C, M, N, O);
        cudaDeviceSynchronize();   // ESSENCIAL aqui -- sem isso voce mede so o tempo de LANCAR, nao de EXECUTAR
        double end_time = get_time();
        gpu_total_time += end_time - start_time;
    }
    double gpu_avg_time = gpu_total_time / 20.0;

    printf("CPU average: %f us\n", cpu_avg_time * 1e6);
    printf("GPU average: %f us\n", gpu_avg_time * 1e6);
    printf("Speedup: %fx\n", cpu_avg_time / gpu_avg_time);
    

    // Free memory
    free(h_A);
    free(h_B);
    free(h_C);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

  



}

