# Exemplo 04 — Coalescing por Stride

## Hipótese antes de rodar

Ordene strides `1, 2, 4, 8, 16, 32` por bandwidth útil e justifique contando segmentos de
32 B tocados por uma instrução de um warp.

## Compilar e rodar

```bash
nvcc -O3 -arch=sm_86 -o coalescing coalescing.cu
./coalescing
```

O número de threads e bytes **úteis** é constante; apenas a distância entre loads muda. O
input máximo ocupa ~1 GiB e o output ~32 MiB. Há 5 warmups, 20 repetições por stride e
timing com CUDA Events.

| Stride | Segmentos previstos/warp | Mediana ms | GB/s útil | % do stride 1 |
| --- | --- | --- | --- | --- |
| 1 | | | | |
| 2 | | | | |
| 4 | | | | |
| 8 | | | | |
| 16 | | | | |
| 32 | | | | |

“GB/s útil” conta 4 B lidos + 4 B escritos por thread, não os bytes físicos inferidos.
Para confirmar setores/transações reais, use Nsight Compute na Phase 4. Tempo menor ou
maior sozinho não prova quantos bytes a DRAM moveu.

