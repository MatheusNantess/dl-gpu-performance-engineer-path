# Exemplo 07 — Barreiras e Contenção Atômica

## Hipóteses antes de rodar

1. O custo por `__syncthreads()` muda com blocks de 32, 128, 256 e 512 threads?
2. Atomics no mesmo contador serão mais lentos que atomics distribuídos por warp?
3. O número de atomics é igual nos dois casos; qual variável muda?

## Compilar e rodar

```bash
nvcc -O3 -arch=sm_86 -o sync_atomics sync_atomics.cu
./sync_atomics
```

O teste de barreira executa 10.000 barreiras por block e reporta tempo médio aproximado
por barreira no kernel inteiro. Isso não é “latência universal de uma barreira”: blocks
executam concorrentemente.

O teste atômico mantém o mesmo total de `atomicAdd`; muda o número de destinos, de um
hotspot global para um contador por warp.

| Experimento | Configuração | Resultado | Interpretação |
| --- | --- | --- | --- |
| barrier | 32 threads/block | | |
| barrier | 128 threads/block | | |
| barrier | 256 threads/block | | |
| barrier | 512 threads/block | | |
| atomic | um contador | | |
| atomic | um contador/warp | | |

O programa valida a soma final dos contadores. Um resultado rápido, mas incorreto, aborta.

