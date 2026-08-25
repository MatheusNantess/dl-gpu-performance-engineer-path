# Exemplo 02 — Bandwidth e Latência (reutilizado)

Este experimento **não duplica código**. Ele aponta para a medição que já foi criada e
executada durante a ponte da Phase 1:

- [README e procedimento](../../../phase-01-computer-architecture/examples/06-gpu-bandwidth-latency/README.md)
- [bandwidth_gpu.py](../../../phase-01-computer-architecture/examples/06-gpu-bandwidth-latency/bandwidth_gpu.py)
- [latency_gpu.cu](../../../phase-01-computer-architecture/examples/06-gpu-bandwidth-latency/latency_gpu.cu)
- [little_law_gpu.py](../../../phase-01-computer-architecture/examples/06-gpu-bandwidth-latency/little_law_gpu.py)

## Resultados existentes

| Métrica | Valor medido | Método |
| --- | --- | --- |
| Bandwidth sustentável | 305,21 GB/s | STREAM Triad, CUDA Events, 50 repetições |
| Latência global isolada | 289,7 ns/acesso | pointer chasing Sattolo, `<<<1,1>>>`, 256 MB |
| Concorrência por Little's Law | ~2764 requests | setores de 32 B; aproximação |

Esses números foram registrados em 2026-08-23 no
[`docs/hardware/rtx-3060.md`](../../../../docs/hardware/rtx-3060.md). Não execute novamente
apenas para marcar uma caixa. Reexecute quando ambiente, driver, clocks ou metodologia
mudar, e registre a nova data como outra medição.

## Pergunta de interpretação

Por que o benchmark de latência usa uma thread e o de bandwidth precisa de paralelismo
massivo? Responda em termos de dependent loads, MLP e latency hiding.

