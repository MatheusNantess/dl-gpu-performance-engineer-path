# Checkpoint — Phase 1: Computer Architecture

## 📘 Teoria — núcleo (obrigatório)

- [ ] Consigo explicar as 4 etapas do pipeline de CPU (fetch, decode, execute, writeback) e por que pipelining aumenta throughput sem reduzir latência de uma instrução
- [ ] Consigo explicar o que é um data hazard e por que ele causa uma "bolha" no pipeline
- [ ] Consigo explicar ILP e dar um exemplo de código com ILP alto e ILP baixo
- [ ] Consigo explicar execução fora de ordem, em uma frase
- [ ] Consigo explicar o que SIMD faz fisicamente, e por que é diferente de múltiplos núcleos
- [ ] Consigo explicar a ligação entre SIMD e por que NumPy/loop vetorizado é mais rápido que Python puro
- [ ] Consigo explicar por que `const` sozinho não prova ausência de aliasing, e por que `restrict` resolve isso
- [ ] Consigo explicar por que alignment importa pra loads vetorizados
- [ ] Consigo desenhar a hierarquia de memória (registrador → L1 → L2 → L3 → RAM) com latências aproximadas de cada nível
- [ ] Consigo explicar localidade temporal e localidade espacial, com um exemplo de cada
- [ ] Consigo explicar a diferença entre latência e bandwidth usando a analogia da autoestrada (ou outra própria)
- [ ] Consigo explicar por que uma lista encadeada é latency-bound e um array percorrido em sequência é bandwidth-bound
- [ ] **Consigo enunciar Little's Law (`L = λW`) e usá-la pra calcular concorrência necessária, dado throughput-alvo e latência**
- [ ] **Consigo explicar por que "alta occupancy" não garante "alta performance" sozinha (Little's Law diz quanta concorrência é necessária, não resolve outros gargalos)**
- [ ] Consigo explicar o que um hardware prefetcher faz, e por que um padrão de acesso previsível (stride fixo) pode "esconder" latência que eu queria medir
- [ ] Consigo explicar a diferença entre dependent load (pointer chasing) e independent load (streaming), e por que só o segundo se beneficia de sobreposição de requisições
- [ ] Consigo explicar o que é Memory-Level Parallelism e conectar isso com occupancy/latency hiding em GPU
- [ ] **Consigo explicar a diferença entre "concorrência necessária" (Little's Law) e "concorrência disponível" (MLP), e por que o resultado real depende do menor dos dois**
- [ ] Consigo explicar por que o custo real de um algoritmo, em hardware moderno, é frequentemente "bytes movidos" e não "operações feitas"
- [ ] Consigo calcular arithmetic intensity de uma operação simples na mão, mostrando FLOPs e bytes separadamente
- [ ] Consigo explicar por que a mesma operação matemática (matmul) pode ter AI diferente dependendo de como é implementada
- [ ] **Consigo explicar a diferença entre AI algorítmica (calculada na mão) e AI de hardware (medida com performance counters), e por que a razão GFLOP/s÷GB/s de um benchmark simples não é uma confirmação independente da AI — é uma tautologia matemática (o tempo cancela)**
- [ ] Consigo explicar o que é o ridge point e como ele se calcula
- [ ] Consigo classificar uma operação como memory-bound ou compute-bound, dado a AI dela e o ridge point do hardware
- [ ] Consigo explicar por que o teto de memória no roofline é uma linha diagonal e o teto de cálculo é uma linha horizontal
- [ ] Consigo explicar a diferença entre roofline teórico e empírico, e quando cada um é apropriado (nenhum dos dois é "errado")
- [ ] **Consigo enunciar a Lei de Amdahl e calcular o speedup total, dado `P` (fração afetada) e `S` (speedup local)**
- [ ] **Consigo explicar por que otimizar uma parte pequena do tempo total (mesmo com `S` gigante) rende pouco speedup total — e por que isso justifica "profile first, optimize what matters"**
- [ ] **Consigo contar FLOPs, bytes e passadas de uma normalização simples (RMSNorm) na mão, e comparar uma versão multi-pass com uma versão fundida em termos de AI**
- [ ] **Consigo explicar, conceitualmente, por que LayerNorm precisa de mais reduções/passadas que RMSNorm, e como o truque `mean(x²) - mean(x)²` reduz isso**
- [ ] **Consigo explicar, em termos gerais (sem derivar), por que o backward de uma normalização tende a ser mais complexo que o forward (gradiente de parâmetro exige redução ao longo do batch, não só da dimensão hidden)**

## 📘 Teoria — leitura complementar (opcional, não bloqueia Phase 2)

- [ ] Consigo explicar o que é um set e uma way numa cache, e por que "caber na cache" não garante ausência de cache miss (`10-cache-associativity-and-working-sets.md`)
- [ ] Consigo distinguir compulsory, capacity e conflict miss, com um exemplo de cada
- [ ] Consigo explicar por que tiling/blocking reduz cache miss, em termos de working set
- [ ] Consigo explicar o que é branch misprediction e pipeline flush (`12-branch-prediction-and-control-flow.md`)
- [ ] Consigo explicar por que branch prediction de CPU e divergência de warp em GPU são modelos DIFERENTES, não a mesma coisa com nome diferente
- [ ] Consigo explicar o que é uma página, uma page table e um TLB, e por que TLB miss é uma causa possível de lentidão distinta de cache miss de dado (`13-tlb-pages-and-address-translation.md`)

## 💻 Exercícios

O `exercises/README.md` foi reescrito pra seguir uma filosofia investigativa (pergunta →
confusão → pesquisa → hipótese → código → medição → explicação), organizada por categoria
(A-K) e nível (1-6), não mais números sequenciais fixos. Marque por categoria:

- [ ] A) Recall — as 3 perguntas de checagem rápida
- [ ] B) Investigation (🔎) — as 4 perguntas, incluindo pesquisa real quando travei
- [ ] C) Predict Before Running — previsão escrita ANTES de rodar, pros 4 pares de código
- [ ] D) Explain the Result — as 2 perguntas, depois de já ter os resultados de C
- [ ] E) Debug the Mental Model — as 4 afirmações, com o que cada uma esconde identificado
- [ ] F) Performance Mysteries (🔎) — as 3, implementadas e medidas antes de explicar
- [ ] G) Calculation + Interpretation — os 3 cálculos gerais + os 2 de normalização (24-25)
- [ ] H) Code Experiments — checklist completo rodado
- [ ] K) CUDA Bridge — hipóteses **escritas** (registradas abaixo), antes de estudar Phase 2
- [ ] Rodei os exemplos 01 a 04 (e opcionalmente 05) e registrei os números medidos abaixo
- [ ] Medi a diferença real entre soma com 1 acumulador vs 4 acumuladores independentes (seção C/F)
- [ ] Rodei `peak_compute_benchmark.cpp` e obtive um pico de GFLOP/s real (não placeholder)

## 🔬 Mastery

- [ ] Dado o código de uma operação nova (nunca vista), consigo calcular a AI dela sem ajuda e identificar se é algorítmica ou de hardware
- [ ] Dado a AI de uma operação e os dois picos do meu hardware, consigo dizer o regime e o teto teórico de performance, sem rodar nada
- [ ] Consigo explicar para alguém, do zero, por que "fazer menos contas" às vezes não ajuda em nada
- [ ] Consigo ler um número de "% do peak alcançado" e dizer se isso é bom, médio ou ruim para aquela AI específica
- [ ] Aplico isso a algo novo: dado a descrição de uma operação (não o código), estimo se ela provavelmente é memory-bound ou compute-bound antes de calcular
- [ ] Dado throughput-alvo e latência de um sistema qualquer (não só memória — I/O, rede), calculo a concorrência necessária com Little's Law
- [ ] Dado a fração de tempo que uma otimização afetaria e o speedup local esperado, decido se vale a pena investir esforço nela (Amdahl)
- [ ] Consigo pegar uma operação de deep learning nova (não normalização) e fazer a mesma análise de FLOPs/bytes/passadas/AI que fiz pra RMSNorm/LayerNorm

## Picos medidos (a base de tudo — sem isso, nenhum número de "% de peak" significa nada)

Preencher a partir de `examples/01`, `02` e `03/peak_compute_benchmark.cpp`. Estes são
picos **empíricos** — se quiser comparar contra picos **teóricos** de datasheet, adicione
uma coluna separada e identifique claramente qual é qual (arquivo de teoria 09, seção 4).

| Métrica | Valor medido | Como medido |
| --- | --- | --- |
| Latência L1 (aprox.) | | `examples/01-latency-benchmark` |
| Latência L2 (aprox.) | | `examples/01-latency-benchmark` |
| Latência L3 (aprox.) | | `examples/01-latency-benchmark` |
| Latência RAM (aprox.) | | `examples/01-latency-benchmark` |
| Peak bandwidth sustentável (empírico) | | `examples/02-bandwidth-benchmark` |
| Peak GFLOP/s (empírico) | | `examples/03-arithmetic-intensity-calc/peak_compute_benchmark.cpp` |
| Ridge point | | calculado: peak GFLOP/s ÷ peak bandwidth |
| Concorrência necessária p/ saturar bandwidth (Little's Law) | | calculado: throughput × latência (arquivo 05, seção 7) |

## Benchmarks posicionados no roofline

| Operação | AI algorítmica calculada | GFLOP/s alcançado (real) | % do teto empírico |
| --- | --- | --- | --- |
| `vector_add` | | | |
| matmul ingênuo | | | |
| (sua operação nova, exercício 26) | | | |

## Hipóteses CUDA Bridge (seção K dos exercícios) — escrever ANTES de estudar Phase 2

Registre aqui suas próprias hipóteses, por escrito, antes de ler qualquer teoria de GPU.
Depois de terminar a Phase 2, volte aqui e compare — o objetivo não é acertar, é notar
onde sua intuição bateu e onde ela precisou ser corrigida.

```text
27. Como uma GPU esconderia latência de memória sem OoO/prefetch de CPU?

Minha hipótese:


28. Como uma GPU "escala" SIMD pra milhares de valores?

Minha hipótese:


29. Qual "unidade" de GPU gera a concorrência que Little's Law pede?

Minha hipótese:

```

## Deliverable

- [ ] Roofline plot gerado (`examples/04-roofline-plot`) com pelo menos 2 benchmarks reais posicionados, usando picos empíricos
- [ ] Gráfico (ou output de texto) salvo e referenciado aqui
- [ ] Análise escrita (algumas frases, não precisa ser longa) do case study de normalização — RMSNorm estratégia A vs B, em termos de AI

## Definition of Done — o que "Phase 1 completa" realmente significa

Não é ter marcado todas as caixinhas acima mecanicamente. É conseguir raciocinar assim,
diante de qualquer operação nova (não decorado, aplicado):

> Performance não é só "quantas operações existem?". É: quantos FLOPs, quantos bytes, em
> qual nível da memória, qual latência, qual bandwidth, quanta concorrência é necessária
> (Little's Law), quanta o padrão de acesso realmente expõe (MLP), qual arithmetic
> intensity, memory-bound ou compute-bound, qual teto (teórico ou empírico), qual fração do
> runtime total essa parte realmente ocupa (Amdahl) antes de investir esforço nela, quais
> intermediários são realmente necessários, e vale salvar ou recomputar.

Se você consegue percorrer essa lista de perguntas sozinho, sem consultar nada, dado uma
operação que nunca viu antes (inclusive uma de deep learning, como no case study de
normalização) — Phase 1 está feita, no sentido que importa.

## Pronto para Phase 2 quando

- [ ] Todas as seções **núcleo** (teoria obrigatória, exercícios fáceis/médios/difícil,
      mastery, deliverable) honestamente marcadas — a seção de leitura complementar
      (cache associativity, branch prediction, TLB) **não** precisa estar completa
- [ ] Tenho os picos medidos do meu próprio hardware, registrados em `docs/hardware/`
      (criar um registro de CPU seguindo o mesmo espírito do `docs/hardware/rtx-3060.md`,
      adaptado — identidade da CPU, não de GPU)
- [ ] Consigo, sem consultar nada, pegar uma operação nova e responder: "isso é
      memory-bound ou compute-bound, e por quê?"
- [ ] Consigo, sem consultar nada, decidir se vale a pena otimizar algo dado a fração de
      tempo que ocupa (Amdahl)
- [ ] Consigo fazer a análise de FLOPs/bytes/AI de uma operação de normalização nova
      (não só repetir RMSNorm/LayerNorm de cor)
