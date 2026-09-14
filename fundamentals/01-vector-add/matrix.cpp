#include <cstdio>

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
        C[i* o + j] = soma ;
        }
    }
}


int main(){
    float A[6] = {
    2, 1,
    5, 4,
    4, 3
};

    float B[8] = {
    2, 9, 2, 7,
    1, 1, 2, 4
};

    float C[12];   // resultado, sem inicializar (a funcao preenche)

    matrixMultiplication(A, B, C, 3, 2, 4);

}