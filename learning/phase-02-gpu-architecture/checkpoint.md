# Checkpoint — Phase 2: GPU Architecture

Marque somente quando conseguir demonstrar ou explicar **sem reler a teoria**. “Li o
arquivo” não é critério de conclusão.

## Teoria — núcleo obrigatório

- [ ] Consigo distinguir GPU, GPC (quando relevante), SM, warp, thread e lane sem tratá-los como sinônimos
- [ ] Consigo explicar o papel de CUDA Cores e Tensor Cores sem dizer que uma CUDA Core é equivalente a um núcleo completo de CPU
- [ ] Consigo mapear `threadIdx`, `blockIdx`, `blockDim` e `gridDim` para um índice global
- [ ] Consigo calcular quantos warps um block usa, incluindo o warp parcialmente vazio
- [ ] Consigo explicar por que blocks são a unidade de distribuição entre SMs e não migram entre SMs durante sua execução
- [ ] Consigo explicar SIMT e sua diferença para SIMD
- [ ] Consigo explicar o papel do warp scheduler e o significado de um warp ready/eligible
- [ ] Consigo desenhar registradores → shared/L1 → L2 → VRAM, indicando escopo e ordem de capacidade/latência
- [ ] Consigo explicar por que shared memory é software-managed e cache não é
- [ ] Consigo explicar register pressure, spilling e por que spill normalmente vira tráfego na hierarquia de memória
- [ ] Consigo calcular occupancy teórica limitada por threads, blocks, registradores e shared memory
- [ ] Consigo explicar por que occupancy alta não garante performance alta e por que occupancy baixa também não prova performance ruim
- [ ] Consigo conectar warps residentes, warps elegíveis, MLP e latency hiding
- [ ] Consigo usar Little's Law como ordem de grandeza da concorrência necessária, sem igualar “threads residentes” a “requisições em voo”
- [ ] Consigo explicar bank conflict e o caso especial de broadcast
- [ ] Consigo explicar por que atomics garantem indivisibilidade, mas não uma ordem útil entre threads concorrentes
- [ ] Consigo explicar quando contenção em atomics serializa progresso
- [ ] Consigo diferenciar CUDA Cores e Tensor Cores pelo tipo/granularidade de operação

## Gates explícitos do ROADMAP

- [ ] **Consigo explicar o que acontece com um warp e com o block inteiro em `__syncthreads()`**
- [ ] **Consigo explicar o que acontece numa branch divergente, incluindo active mask e reconvergência**
- [ ] **Consigo prever se um padrão de acesso coalesces antes de medi-lo**
- [ ] **Medi bandwidth alcançável e comparei com o teto de especificação, mantendo os dois rótulos separados**
- [ ] **Medi kernel launch overhead**
- [ ] **O registro `docs/hardware/rtx-3060.md` contém valores, datas, comandos e metodologia reais**

## Predict before running

Registre respostas antes de executar os exemplos.

```text
Coalescing — ordem prevista (melhor → pior) para strides 1, 2, 4, 8, 16, 32:

Por quê (conte segmentos de 32 B tocados por um warp):


Divergência — qual deve ser mais caro?
  A) branch uniforme por warp
  B) lanes pares/ímpares seguem caminhos diferentes

Por quê:


Register pressure — o que espero que aconteça ao aumentar registradores por thread?


Atomics — qual padrão deve escalar pior?
  A) cada thread atualiza um contador diferente
  B) todas atualizam o mesmo contador

Por quê:
```

## Resultados medidos

Não preencha com datasheet nem estimativa.

| Métrica | Resultado | Data | Comando/método |
| --- | --- | --- | --- |
| Bandwidth global sustentável | 305,21 GB/s | 2026-08-23 | Phase 1 exemplo 06, STREAM Triad PyTorch, CUDA Events, 50 repetições |
| Bandwidth de especificação | 360,0 GB/s (teórico) | consultado no registro | barramento × data rate; não medido |
| Eficiência medida/especificação | 84,8% | 2026-08-23 | `305,21 / 360,0` |
| Latência global isolada | 289,7 ns/acesso | 2026-08-23 | pointer chasing Sattolo, `<<<1,1>>>`, buffer 256 MB |
| Launch overhead — CUDA runtime | | | `examples/03-launch-overhead` |
| Launch overhead — PyTorch (opcional) | | | |
| Melhor bandwidth no experimento de stride | | | `examples/04-coalescing` |
| Pior bandwidth no experimento de stride | | | `examples/04-coalescing` |
| Razão divergent/uniform | | | `examples/05-divergence` |
| Limite observado de register pressure | | | `examples/06-register-pressure` |
| Custo de `__syncthreads()` | | | `examples/07-sync-atomics` |
| Razão atomic contended/sharded | | | `examples/07-sync-atomics` |

## Explicação pós-medição

```text
1. Onde minha previsão de coalescing acertou? Onde cache/overhead alterou o resultado?


2. Divergência foi o único fator diferente entre as variantes? Que trabalho útil cada
   variante realmente executou?


3. Qual recurso limitou occupancy no experimento 06? Threads, blocks, registers ou shared?


4. O launch overhead medido é grande ou pequeno comparado a um kernel típico deste lab?


5. Que medição exigiria Nsight Compute para confirmar, em vez de ser inferida por tempo?
```

## Exercícios

- [ ] Completei a seção A (mapeamento e contagem)
- [ ] Completei a seção B (coalescing no papel)
- [ ] Completei a seção C (occupancy e Little's Law)
- [ ] Completei a seção D (controle e sincronização)
- [ ] Escrevi as hipóteses da seção E antes de medir
- [ ] Completei pelo menos dois desafios aplicados da seção F

## Mastery

- [ ] Dado um kernel curto, identifico a unidade correta de análise para cada problema: thread, warp, block, SM ou GPU
- [ ] Dado um acesso `base[f(threadIdx.x)]`, enumero os endereços das 32 lanes e estimo as transações
- [ ] Dado uso de registers/shared/block size e limites do SM, encontro o primeiro limitador de occupancy
- [ ] Dado um stall de memória, separo três hipóteses: acesso não coalesced, pouca concorrência/warps elegíveis e latência inevitável
- [ ] Formulo uma hipótese falsificável e escolho uma métrica/experimento que poderia refutá-la
- [ ] Não concluo “compute-bound” ou “memory-bound” olhando apenas occupancy ou duração

## Definition of Done

Phase 2 está pronta quando você consegue percorrer esta cadeia diante de um kernel novo:

> mapear threads → agrupar em warps → enumerar endereços por warp → contar transações →
> identificar recursos por thread/block → estimar blocks/warps residentes → verificar se
> existem warps elegíveis para esconder latência → localizar divergência/barreiras/atomics
> → formular uma causa de hardware → medir para tentar refutá-la.

## Pronto para Phase 3 quando

- [ ] Todos os itens do núcleo e os gates do ROADMAP estão honestamente completos
- [ ] Bandwidth, latência e launch overhead estão registrados com metodologia reproduzível
- [ ] Consigo prever coalescing e divergência sem executar o código
- [ ] Consigo explicar por que a configuração com occupancy máxima pode perder para outra
- [ ] Estou pronto para implementar `fundamentals/01-vector-add` sem otimizar às cegas

