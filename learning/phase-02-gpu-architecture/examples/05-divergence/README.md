# Exemplo 05 — Divergência Dentro vs Entre Warps

## Hipótese antes de rodar

Compare:

- **uniform:** warps pares executam A, warps ímpares executam B;
- **divergent:** lanes pares executam A, lanes ímpares executam B.

Todos os threads produzem um resultado e os dois caminhos têm trabalho semelhante. Preveja
a razão de tempo, sem prometer “exatos 2x”.

## Compilar e rodar

```bash
nvcc -O3 -arch=sm_86 -o divergence divergence.cu
./divergence
```

O programa usa 5 warmups, 21 repetições e CUDA Events. O array (~256 MiB) reduz a chance
de o kernel ser pequeno demais, mas o cálculo interno é deliberadamente pesado para que a
branch seja observável.

| Variante | Mediana (ms) | Razão vs uniform | Interpretação |
| --- | --- | --- | --- |
| Uniform por warp | | 1,00 | |
| Divergent por lane | | | |

Limitação: o compilador pode usar predication ou transformar controle. O tempo demonstra
o custo deste binário; confirmar instruções/active lanes exige SASS/Nsight na Phase 4.

