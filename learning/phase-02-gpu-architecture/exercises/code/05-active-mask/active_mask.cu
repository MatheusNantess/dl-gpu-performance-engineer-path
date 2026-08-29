#include <cstdio>

// Capitulos 03/09. __activemask() mostra, em binario, quais lanes do warp
// estao participando da instrucao atual. Dentro de um "if" que so 20 lanes
// entram, a mascara deve ter exatamente os 20 bits mais baixos ligados.

__global__ void activeMaskDemo() {
    int lane = threadIdx.x;
    if (lane < 20) {
        unsigned mask = __activemask();
        if (lane == 0) {
            printf("Dentro do 'if (lane < 20)': active mask = 0x%08x\n", mask);
            printf("(0x000fffff = 20 bits baixos ligados = lanes 0..19 ativas)\n");
        }
    }
    __syncwarp();
    if (lane == 0) {
        unsigned fullMask = __activemask();
        printf("Fora do if, warp reconvergiu: active mask = 0x%08x (deve ser 0xffffffff = 32 lanes)\n", fullMask);
    }
}

int main() {
    activeMaskDemo<<<1, 32>>>();
    cudaDeviceSynchronize();
    return 0;
}
