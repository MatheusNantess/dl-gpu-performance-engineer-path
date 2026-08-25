# Exemplo 03 — Kernel Launch Overhead

## Hipótese antes de rodar

Estime o custo de um launch vazio em μs. Depois diga qual medida espera que seja maior:
launches enfileirados em lote ou launch + synchronize a cada iteração.

## Compilar e rodar

```bash
nvcc -O3 -arch=sm_86 -o launch_overhead launch_overhead.cu
./launch_overhead
```

O programa aquece o contexto, executa 9 trials e reporta mediana de:

- **batched device timeline:** milhares de kernels vazios entre CUDA Events;
- **host batch:** tempo do host para enfileirar e drenar o lote;
- **synchronized round trip:** launch seguido de `cudaDeviceSynchronize()`.

O segundo inclui custo de espera host/device e não deve ser chamado apenas de “launch”.

| Métrica | Mediana (μs/kernel) | Ambiente/data |
| --- | --- | --- |
| Batched device timeline | | |
| Host enqueue + drain | | |
| Synchronized round trip | | |

Limitações: WSL2, compositor usando a GPU, clocks e kernels vazios podem aumentar variância.
Não compare diretamente com um launch medido em Linux nativo sem registrar o ambiente.

