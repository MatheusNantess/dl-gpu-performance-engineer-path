# 05 — Memory Coalescing

> Pré-requisito: [03 — warps](03-warps-simt-scheduling.md) e [04 — memória](04-gpu-memory-hierarchy.md).

## 1. A pergunta correta

Coalescing não pergunta “cada thread acessa memória contígua ao longo do tempo?”. Pergunta:

> **Para uma única instrução de memória de um warp, em quais segmentos caem os endereços
> pedidos pelas lanes ativas?**

Em dispositivos modernos, acessos globais são atendidos pelo número de transações de 32 B
necessário para cobrir os endereços. Um warp lendo 32 `float` consecutivos usa 128 B úteis,
tipicamente quatro segmentos de 32 B quando alinhado.

```text
lane:      0  1  2  3 ... 31
endereço:  0  4  8 12 ...124 B
segmentos: [   32   ][   32   ][   32   ][   32   ]
```

## 2. Stride aumenta overfetch

Para `x[stride * lane]`:

- stride 1 agrupa palavras vizinhas;
- stride 2 usa metade dos bytes de cada faixa tocada;
- strides maiores espalham lanes por mais segmentos;
- stride 32 pode fazer cada lane tocar uma região distante.

O kernel ainda está correto. O custo aparece como transações extras e bytes transferidos
que nenhuma lane usa.

```text
eficiência aproximada = bytes úteis pedidos / bytes físicos transferidos
```

Cache pode suavizar o tempo observado, mas não torna o padrão magicamente coalesced.

## 3. Alinhamento e lanes ativas

Um acesso consecutivo desalinhado pode cruzar um segmento adicional. Um warp parcial ainda
pode coalescer perfeitamente entre as lanes ativas, embora desperdice capacidade de
execução por lanes ausentes. Broadcast (muitas lanes lendo o mesmo endereço) é um caso
especial e não equivale a 32 transações independentes.

## Pergunta rápida

Cada lane lê um `float`: `x[8 * lane]`. Quantos bytes separam endereços consecutivos? O
warp usa todos os bytes trazidos?

<details><summary>Resposta</summary>

32 B. Cada lane cai no começo de outro segmento de 32 B e aproveita apenas 4 B dele, no
caso frio simples: eficiência útil de aproximadamente 4/32 = 12,5%. Cache e detalhes da
hierarquia podem alterar o tempo, mas a geometria de endereços continua ruim.

</details>

## Procedimento mental

1. Fixe **uma instrução** e **um warp**.
2. Liste lanes ativas.
3. Converta índices em endereços em bytes.
4. Marque segmentos alinhados de 32 B.
5. Conte segmentos e bytes úteis.

## O que guardar

- Coalescing é propriedade transversal às lanes de um warp num acesso.
- Correção e coalescing são independentes.
- Contar endereços antes de medir é obrigatório; bandwidth baixa sozinha não prova causa.

**Conexão:** mesmo com acessos perfeitos, o SM precisa manter warps suficientes residentes;
registers e shared memory determinam quantos cabem.

Fonte primária: [CUDA Programming Guide — Coalesced Global Memory Access](https://docs.nvidia.com/cuda/cuda-programming-guide/02-basics/writing-cuda-kernels.html#coalesced-global-memory-access).

