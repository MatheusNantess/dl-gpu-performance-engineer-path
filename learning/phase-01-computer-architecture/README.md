# Phase 1 — Computer Architecture

Ainda não é GPU. É entender, de verdade, por que hardware se comporta do jeito que se
comporta — o vocabulário e o modelo mental que a Phase 2 (GPU Architecture) inteira
assume que você já tem.

A pergunta que este módulo constrói a capacidade de responder, sempre: **"onde está o
gargalo real desta operação — no cálculo, ou em mover dado?"** Toda otimização de kernel,
pra sempre, volta a essa pergunta.

## Por que isso vem antes de CUDA

CPU e GPU são hardwares diferentes, mas os princípios que fazem eles rápidos ou lentos são
os mesmos: pipelining, hierarquia de memória, paralelismo, o trade-off entre latência e
throughput. Aprender esses princípios numa CPU primeiro (que você já tem, sem precisar de
driver nenhum) significa que quando a Phase 2 apresentar "warp", "SM", "shared memory",
você já vai reconhecer a ideia por trás — só o nome muda.

## Teoria — núcleo (obrigatório, bloqueia a passagem pra Phase 2)

| # | Arquivo | Tópico |
| --- | --- | --- |
| 01 | [01-cpu-execution-pipeline.md](theory/01-cpu-execution-pipeline.md) | O ciclo fetch-decode-execute-writeback; por que pipelining existe |
| 02 | [02-ilp-and-superscalar.md](theory/02-ilp-and-superscalar.md) | Instruction-level parallelism, execução superescalar e fora de ordem |
| 03 | [03-simd.md](theory/03-simd.md) | Instruções vetorizadas, aliasing/`restrict`, alignment — a base de hardware por trás de código vetorizado ser rápido |
| 04 | [04-memory-hierarchy.md](theory/04-memory-hierarchy.md) | Registrador → L1 → L2 → L3 → RAM, e por que cada nível existe |
| 05 | [05-latency-vs-bandwidth.md](theory/05-latency-vs-bandwidth.md) | Tempo de uma requisição vs throughput de muitas, **e Little's Law** (`L = λW`) — quanta concorrência é necessária pra saturar bandwidth |
| 06 | [06-memory-as-cost-model.md](theory/06-memory-as-cost-model.md) | Tratar acesso à memória como o custo real de um cálculo |
| 07 | [07-arithmetic-intensity.md](theory/07-arithmetic-intensity.md) | A fórmula que prevê se uma operação é limitada por cálculo ou por memória, e a distinção AI algorítmica vs AI de hardware |
| 08 | [08-compute-bound-vs-memory-bound.md](theory/08-compute-bound-vs-memory-bound.md) | Os dois regimes, e como reconhecer qual você está enfrentando |
| 09 | [09-roofline-model.md](theory/09-roofline-model.md) | Juntando tudo num gráfico só — o modelo Roofline (teórico e empírico) |
| 11 | [11-prefetching-and-memory-level-parallelism.md](theory/11-prefetching-and-memory-level-parallelism.md) | Prefetching, Memory-Level Parallelism, e a conexão com Little's Law (concorrência necessária vs disponível) |
| 14 | [14-amdahls-law.md](theory/14-amdahls-law.md) | Vale a pena otimizar isso? Profile first, optimize what matters |
| 15 | [15-normalization-case-study.md](theory/15-normalization-case-study.md) | Case study fechando o módulo: FLOPs/bytes/passadas de RMSNorm e LayerNorm, ligado direto ao North Star do lab |

*(Numeração pulando o 10/12/13 é proposital — ver seção seguinte.)*

Cada arquivo tem perguntas teóricas e exercícios de "prever antes de rodar" embutidos ao
longo do texto, não só no final — a ideia é testar entendimento no momento em que o
conceito é apresentado, enquanto ainda está fresco.

## Teoria — leitura complementar (opcional, NÃO bloqueia Phase 2)

| # | Arquivo | Tópico |
| --- | --- | --- |
| 10 | [10-cache-associativity-and-working-sets.md](theory/10-cache-associativity-and-working-sets.md) | Sets, ways, tipos de cache miss, working set — por que "caber na cache" não garante ausência de miss |
| 12 | [12-branch-prediction-and-control-flow.md](theory/12-branch-prediction-and-control-flow.md) | Branch misprediction, pipeline flush — com aviso explícito de que isso **não** é a mesma coisa que divergência de warp |
| 13 | [13-tlb-pages-and-address-translation.md](theory/13-tlb-pages-and-address-translation.md) | Tradução de endereço e TLB miss — por que nem toda lentidão de memória é culpa da RAM |

Esses três arquivos existem e são bons material de referência, mas **não são gate** pra
Phase 2 — não é preciso dominá-los agora. Voltam a ser relevantes conforme aparecerem
sintomas específicos (ex: um benchmark com comportamento estranho de miss que não se
explica só por capacidade de cache) mais adiante no roadmap.

## Exemplos

- [`examples/01-latency-benchmark/`](examples/01-latency-benchmark/) — mede latência real de acesso à memória em strides diferentes (pointer chasing com permutação aleatória — Sattolo — depois de uma primeira tentativa com stride fixo ter sido distorcida pelo hardware prefetcher; ver arquivo 11)
- [`examples/02-bandwidth-benchmark/`](examples/02-bandwidth-benchmark/) — mede bandwidth sustentável real (estilo STREAM)
- [`examples/03-arithmetic-intensity-calc/`](examples/03-arithmetic-intensity-calc/) — AI algorítmica resolvida na mão + throughput real confirmado em código, pra `vector_add` e matmul, **com a ressalva explícita de que a razão GFLOP/s÷GB/s não é validação empírica de tráfego real** (ver README do exemplo e arquivo de teoria 07, seção 6); inclui também `peak_compute_benchmark.cpp`, um benchmark de pico de cálculo puro de verdade
- [`examples/04-roofline-plot/`](examples/04-roofline-plot/) — script que desenha o roofline a partir de picos medidos (com fallback em texto se `matplotlib` não estiver instalado)
- [`examples/05-hardware-counters-guide/`](examples/05-hardware-counters-guide/) — **opcional, não bloqueia nada**: guia documentado (não resultado inventado) de como medir tráfego real de memória com `perf`/VTune no futuro; `perf` foi verificado como indisponível neste ambiente WSL2

## Exercícios

Ver [`exercises/README.md`](exercises/README.md). Mistura de cálculo na mão, classificação
compute/memory-bound, "predict the output", cálculo de concorrência com Little's Law,
decisão de otimização com Amdahl, análise de normalização, e as tarefas de medição real que
alimentam o roofline final.

## Deliverable

Um roofline plot da sua própria CPU, com pelo menos um benchmark seu posicionado nele.
Prefira picos **empíricos** (medidos nos exemplos 01/02/03) — são o que importa pra decisão
prática de otimização no seu hardware. Picos **teóricos** (de datasheet) são válidos como
limite superior de referência, mas identifique explicitamente qual tipo você está usando
(arquivo de teoria 09, seção 4) — não misture os dois sem rotular.

## Checkpoint

[checkpoint.md](checkpoint.md)

## Fora de escopo aqui

Arquitetura de GPU específica (SM, warp, Tensor Core) — isso é Phase 2. Este módulo fica
inteiramente em CPU, deliberadamente, porque os princípios são os mesmos e você já tem o
hardware. Também fora de escopo, deliberadamente (podem aparecer depois se forem
necessários, mas não bloqueiam nada agora): cache coherence/MESI, virtual memory
aprofundada, internals de branch predictor, ISA/assembly profundo, NUMA, sincronização
multicore, circuitos digitais.

## Ready for GPU Architecture

Quando a Phase 2 chegar, cada conceito deste módulo reaparece com um nome novo, aplicado a
um hardware mais paralelo. Não é conteúdo novo — é o mesmo raciocínio, numa escala maior:

| Nesta fase (CPU) | Vira, na Phase 2 (GPU) |
| --- | --- |
| Hierarquia de memória (registrador → L1 → L2 → L3 → RAM) | Hierarquia de memória de GPU (registrador → shared memory/L1 → L2 → memória global/VRAM) |
| SIMD (uma instrução, vários dados numa lane) | SIMT / warps (uma instrução, 32 threads) |
| Latency hiding via sobreposição de requisições | Latency hiding via troca de warp |
| Little's Law (concorrência necessária) | Requisitos de occupancy pra saturar bandwidth |
| Memory-Level Parallelism | Requisições de memória pendentes por warp/SM |
| Bandwidth de RAM | Bandwidth de memória global da GPU |
| Localidade de cache / cache line | Memory coalescing / reuso de dado entre threads de um warp |
| Arithmetic Intensity / Roofline de CPU | Roofline de GPU (com teto de compute muito mais alto, ridge point diferente) |
| Lei de Amdahl (otimizar o que importa) | Mesma lógica, aplicada a qual kernel otimizar num pipeline de treino inteiro |
| Save vs recompute (arquivo 06) | Ativação checkpointing / decisões de memória em backward |
| Case study de normalização (arquivo 15) | Kernels reais de RMSNorm/LayerNorm, medidos com Nsight Compute |

Nenhuma dessas linhas é coincidência — é o mesmo modelo mental de performance
(compute + dado + paralelismo + dependência + limites de hardware), só que a unidade de
paralelismo cresce de "alguns acumuladores numa thread" pra "milhares de threads numa GPU
inteira".
