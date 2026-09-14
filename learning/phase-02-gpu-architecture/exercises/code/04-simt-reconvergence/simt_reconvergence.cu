#include <cstdio>

// Capitulo 03/09. Um warp so (32 threads). Lanes pares seguem o caminho A,
// lanes impares seguem o caminho B. O hardware executa um caminho inteiro
// (para as lanes ativas dele), depois o outro -- nunca os dois ao mesmo
// tempo -- e so entao reconverge. Repare que os prints de PATH A tendem a
// sair agrupados, e os de PATH B tambem, em vez de intercalados.

__global__ void divergentBranch() {
    int lane = threadIdx.x;
    if (lane % 2 == 0) {
        printf("[PATH A] lane %2d executando caminho par\n", lane);
    } else {
        printf("[PATH B] lane %2d executando caminho impar\n", lane);
    }
    __syncwarp(); // ponto de reconvergencia explicito
    if (lane == 0) {
        printf("--- reconvergencia: o warp inteiro volta a executar junto daqui pra frente ---\n");
    }
}

int main() {
    divergentBranch<<<1, 32>>>();
    cudaDeviceSynchronize();
    printf("\nNota: a ordem exata de saida do printf nao e uma garantia arquitetural formal,\n");
    printf("mas na pratica reflete a serializacao PATH A / PATH B do warp divergente.\n");
    return 0;
}
