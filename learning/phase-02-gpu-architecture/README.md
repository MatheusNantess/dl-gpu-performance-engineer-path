# Phase 2 — GPU Architecture

Esta fase responde a uma pergunta: **quando um kernel se comporta de certo modo, que
parte do hardware explica esse comportamento?** O objetivo não é decorar o diagrama de
uma GPU. É prever coalescing, divergência, pressão de registradores, occupancy e stalls
antes de abrir um profiler — e depois usar medições para corrigir a previsão.

## Pré-requisitos

- Phase 0: pointers, arrays, layout e compilação pela linha de comando.
- Phase 1: latência vs bandwidth, ILP, SIMD, MLP, Little's Law, arithmetic intensity,
  Roofline e Amdahl.
- Ambiente CUDA funcional no WSL2, conforme
  [`docs/hardware/rtx-3060.md`](../../docs/hardware/rtx-3060.md).

Isto **não é ainda um curso completo de CUDA**. O código desta fase existe como aparelho
de laboratório: muda uma variável de arquitetura por vez e mede o efeito. A progressão
`vector add → coalescing → transpose → shared memory → reductions`, com implementação e
otimização completas, pertence à [Phase 3](../../fundamentals/README.md).

## Ordem de estudo

### Núcleo obrigatório

| # | Arquivo | Pergunta central |
| --- | --- | --- |
| 01 | [Modelo de execução e GA106](theory/01-gpu-execution-model-and-ga106.md) | O que existe fisicamente na RTX 3060 e qual trabalho cada parte recebe? |
| 02 | [Threads, blocks e grids](theory/02-threads-blocks-grids.md) | Como a hierarquia lógica é mapeada em SMs? |
| 03 | [Warps, SIMT e scheduling](theory/03-warps-simt-scheduling.md) | O que realmente é emitido quando 32 threads executam? |
| 04 | [Hierarquia de memória](theory/04-gpu-memory-hierarchy.md) | Onde o dado pode morar e quem consegue vê-lo? |
| 05 | [Memory coalescing](theory/05-memory-coalescing.md) | Quantas transações um warp gera para um padrão de endereços? |
| 06 | [Registradores e occupancy](theory/06-registers-occupancy.md) | Como recursos por thread/block limitam warps residentes? |
| 07 | [Shared memory e banks](theory/07-shared-memory-banks.md) | Como cooperação explícita pode reduzir tráfego — ou serializar acessos? |
| 08 | [Latency hiding](theory/08-latency-hiding-mlp.md) | Como warps prontos transformam latência alta em throughput? |
| 09 | [Warp divergence](theory/09-warp-divergence.md) | O que acontece quando lanes do mesmo warp seguem caminhos diferentes? |
| 10 | [Sincronização](theory/10-synchronization.md) | O que `__syncthreads()` garante, quem espera e onde pode dar deadlock? |
| 11 | [Atomics](theory/11-atomics-contention.md) | Quando correção exige serialização e como contenção aparece? |
| 12 | [CUDA/Tensor Cores e diagnóstico](theory/12-execution-units-and-diagnosis.md) | Como ligar um sintoma medido a uma causa de hardware? |

Os capítulos 07, 11 e a parte de Tensor Cores do 12 são mais conceituais nesta fase. A
implementação aprofundada volta nas fases 3, 8 e 9.

## A ponte direta com a Phase 1

| Phase 1 | Nesta fase |
| --- | --- |
| SIMD | SIMT: threads com estado próprio agrupadas em warps de 32 lanes |
| Cadeia de dependência / ILP | ILP dentro de cada thread e warps independentes entre si |
| Cache line e localidade espacial | Transações de 32 B e coalescing por warp |
| MLP disponível | Loads independentes expostos por threads/warps |
| Little's Law | Quantos pedidos precisam permanecer em voo para sustentar bandwidth |
| Hierarquia CPU | registrador → shared/L1 → L2 → VRAM |
| Memory-/compute-bound | Qual subsistema precisa ser atacado primeiro |
| Amdahl | Se vale a pena atacar esse kernel no workload completo |

## Microbenchmark suite

| # | Experimento | Estado esperado |
| --- | --- | --- |
| 01 | [Device inventory](examples/01-device-inventory/) | Confirma propriedades reais via runtime |
| 02 | [Bandwidth + latência](examples/02-bandwidth-latency/) | Reutiliza as medições existentes da Phase 1 |
| 03 | [Launch overhead](examples/03-launch-overhead/) | Mede custo fixo de kernel vazio |
| 04 | [Coalescing](examples/04-coalescing/) | Varia stride mantendo o trabalho lógico |
| 05 | [Divergência](examples/05-divergence/) | Separa divergência dentro e entre warps |
| 06 | [Register pressure / occupancy](examples/06-register-pressure/) | Varia recursos e observa o custo |
| 07 | [Sincronização e atomics](examples/07-sync-atomics/) | Compara barreira e contenção atômica |

Cada README exige uma hipótese escrita **antes** da execução, warmup, repetições, método
de timing e uma tabela vazia. Não confunda os números já medidos em 2026-08-23 com os
experimentos ainda pendentes.

### Compilar

No WSL2, a partir deste diretório:

```bash
chmod +x run_suite.sh
./run_suite.sh --build
```

Isso compila com `nvcc -O3 -arch=sm_86 -lineinfo`. Para também executar os experimentos
curtos:

```bash
./run_suite.sh --run
```

O benchmark de bandwidth usa até ~1,5 GB de VRAM e fica fora da execução automática. Rode
separadamente conforme o README do exemplo 02.

## Como estudar cada experimento

```text
PREVER → justificar pelo hardware → medir → comparar → explicar a diferença
```

1. Identifique a unidade de análise: thread, warp, block, SM ou GPU inteira.
2. Identifique o recurso: lanes, transações, registradores, shared memory ou filas.
3. Escreva a previsão no [checkpoint](checkpoint.md).
4. Rode várias repetições; registre mediana e dispersão quando o programa reportar.
5. Explique mecanismo, não apenas “A foi mais rápido que B”.

## Deliverable

Uma suíte de microbenchmarks medida na RTX 3060 contendo:

- bandwidth alcançável e latência de memória global (já medidas; reutilizadas);
- kernel launch overhead (pendente até executar o exemplo 03);
- pelo menos um experimento medido de coalescing;
- registro atualizado em [`docs/hardware/rtx-3060.md`](../../docs/hardware/rtx-3060.md).

O deliverable só está pronto quando os números foram realmente produzidos nesta máquina,
com data, ambiente e comando. Código compilado sem execução não é medição.

## Critérios de conclusão

O gate detalhado está em [checkpoint.md](checkpoint.md). Em resumo, você precisa conseguir:

- explicar o que ocorre num branch divergente e em `__syncthreads()`;
- prever coalescing olhando os endereços de um warp;
- calcular occupancy limitada por threads, blocks, registradores e shared memory;
- conectar occupancy/warps elegíveis a latency hiding sem dizer que occupancy alta garante
  performance alta;
- medir bandwidth, latência e launch overhead e registrar a metodologia.

Tempo sugerido: **12–18 horas**, incluindo leitura, previsões, compilação, medições e
explicações. Medir com cuidado costuma levar mais tempo que ler.

## Fora de escopo

- otimização completa de transpose, redução ou GEMM;
- programação profunda de warp shuffles, streams e async copies;
- leitura sistemática de PTX/SASS;
- Nsight Compute em profundidade;
- implementação WMMA/Tensor Core;
- sincronização entre blocks, clusters ou múltiplas GPUs.

Esses tópicos pertencem às fases seguintes. Aqui basta construir o modelo mecânico que
permite entendê-los quando aparecerem.

## Fontes primárias

- [CUDA Programming Guide](https://docs.nvidia.com/cuda/cuda-programming-guide/)
- [NVIDIA Ampere Tuning Guide](https://docs.nvidia.com/cuda/ampere-tuning-guide/)
- [CUDA C++ Best Practices Guide](https://docs.nvidia.com/cuda/cuda-c-best-practices-guide/)

