# 12 — CUDA Cores, Tensor Cores e Diagnóstico Mecânico

> Pré-requisito: todos os capítulos anteriores e Roofline/Amdahl da Phase 1.

## 1. Unidades diferentes para trabalhos diferentes

**CUDA Cores** é o nome comercial usado para lanes aritméticas escalares FP/INT dentro dos
SMs. Elas executam operações comuns produzidas pelo kernel.

**Tensor Cores** executam operações matrix multiply-accumulate sobre pequenos tiles com
tipos e formatos suportados, acumulando muitas operações por instrução. Não aceleram
qualquer código automaticamente só porque ele “usa tensores”. A implementação/biblioteca
precisa gerar instruções adequadas, com shapes, alinhamento, dtype e layout compatíveis.

Tensor Core utilization baixa pode significar ausência dessas instruções, alimentação
insuficiente, shapes ruins ou outro gargalo. Não é sinônimo de occupancy baixa.

## 2. Pipeline de diagnóstico da Phase 2

```text
1. A operação está correta?
2. Qual é a unidade de análise: thread, warp, block, SM ou GPU?
3. Quantos FLOPs e bytes? AI e regime esperado?
4. Os endereços por warp coalescem?
5. Há branches divergentes, barriers ou atomics contended?
6. Registers/shared/block size limitam residência?
7. Há warps ready/eligible suficientes para esconder latência?
8. O grid/tamanho amortiza launch overhead e alimenta todos os SMs?
9. Qual medição poderia REFUTAR a hipótese?
```

## 3. Sintoma não é causa

| Sintoma | Hipóteses — não conclusão |
| --- | --- |
| bandwidth baixa | coalescing ruim, grid pequeno, pouca MLP, cache, overhead |
| occupancy baixa | registers, shared, block size; talvez suficiente mesmo assim |
| SM utilization baixa | stalls, pouco trabalho, dependências, sincronização |
| kernel curto | pouco trabalho ou excelente kernel; launch overhead pode dominar |
| atomics lentos | contenção no endereço, não atomicidade abstrata |

Na Phase 4, Nsight Systems localiza o que importa no timeline e Nsight Compute mede
transações, stalls e utilização. A ferramenta não substitui esta cadeia; ela fornece
evidência para escolher entre hipóteses.

## 4. Amdahl fecha a decisão

Mesmo encontrando uma causa real, meça `P`: fração do workload total ocupada pelo kernel.
AI/Roofline dizem **como** atacar; arquitetura diz **qual mecanismo**; Amdahl diz **se o
retorno máximo justifica o esforço**.

## Pergunta rápida

Um kernel memory-bound alcança só 20% da bandwidth medida. Qual é a conclusão correta?

<details><summary>Resposta</summary>

Apenas que ele está longe do teto de bandwidth empírico para aquele experimento. Ainda não
sabemos por quê. Conte transações esperadas, confira tamanho/grid, dependências, cache e
overhead; depois use métricas para separar hipóteses.

</details>

## O que guardar

- Unidades especializadas só ajudam instruções/trabalhos compatíveis.
- Toda conclusão de performance deve nomear mecanismo e evidência.
- Otimização profissional é hipótese falsificável, uma mudança por vez e nova medição.

**Próxima fase:** implementar kernels fundamentais e percorrer o ciclo completo de
correção → benchmark → profile → hipótese → otimização → nova medição.

Fontes primárias:

- [CUDA Programming Guide](https://docs.nvidia.com/cuda/cuda-programming-guide/)
- [NVIDIA Ampere Tuning Guide](https://docs.nvidia.com/cuda/ampere-tuning-guide/)

