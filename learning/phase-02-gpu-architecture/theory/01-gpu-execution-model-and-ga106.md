# 01 — Modelo de Execução da GPU e a GA106

> Pré-requisito: Phase 1 completa, especialmente SIMD, latency vs bandwidth e MLP.

## 1. A mudança de estratégia

Uma CPU gasta muito silício tentando terminar poucas threads rapidamente: caches grandes,
execução fora de ordem e branch prediction sofisticado. Uma GPU dedica uma fração maior do
chip a executar o mesmo tipo de trabalho sobre muitos dados e tolera latência mantendo
muito mais trabalho disponível.

```text
CPU: poucos workers sofisticados → baixa latência por thread
GPU: muitos workers mais simples  → throughput agregado alto
```

Isso não significa que “GPU sempre é mais rápida”. Um problema precisa expor paralelismo,
ter trabalho suficiente e amortizar transferências/launch overhead.

## 2. Hierarquia física mínima

```text
GPU
└── vários Streaming Multiprocessors (SMs)
    ├── warp schedulers / dispatch
    ├── unidades FP32/INT (frequentemente chamadas CUDA Cores)
    ├── Tensor Cores
    ├── registradores
    └── shared memory + L1 c      

Todos os SMs compartilham L2 e controladores que chegam à VRAM.
```

O **SM** é a unidade física central desta fase. Blocks são enviados a SMs; warps desses
blocks são escalonados dentro do SM; instruções usam unidades de execução e dados próximos
ao SM ou via L2/VRAM.

Uma “CUDA Core” não é um núcleo de CPU completo. É uma lane/unidade aritmética dentro de
um SM, sem o aparato autônomo de uma CPU inteira.

## 3. O dispositivo real deste lab

Valores consultados pelo runtime e registrados em
[`docs/hardware/rtx-3060.md`](../../../docs/hardware/rtx-3060.md):

| Propriedade | RTX 3060 do lab |
| --- | --- |
| Arquitetura / chip | Ampere GA106 |
| Compute capability | 8.6 (`sm_86`) |
| SMs | 28 |
| Warp | 32 threads |
| Threads/SM | 1536 (48 warps) |
| Registers/SM | 65536 registradores de 32 bits |
| Shared memory/SM | 100 KB |
| L2 | 2,25 MB |
| VRAM | 12 GB |

Há diferença entre **limite arquitetural**, **propriedade reportada pelo runtime** e
**resultado medido**. Por exemplo, 360 GB/s é teto derivado da especificação; 305,21 GB/s
é o valor sustentável medido. Nunca troque um pelo outro.

## Pergunta rápida

Um grid tem apenas 10 blocks grandes. Quantos dos 28 SMs podem receber ao menos um block
desse grid ao mesmo tempo, no melhor caso?

<details><summary>Resposta</summary>

No máximo 10. Threads dentro de um block não são espalhadas por SMs diferentes. Ter muitas
threads totais dentro desses 10 blocks não cria blocks adicionais para alimentar os outros
18 SMs.

</details>

## Confusões comuns

- Somar “CUDA Cores” e concluir throughput sem considerar instrução, clock, dual issue,
  dependências e utilização.
- Chamar qualquer nível de memória de “VRAM”. Registradores e shared/L1 estão no SM; L2 é
  compartilhado; VRAM é device/global memory fora do chip lógico de execução.
- Confundir compute capability 8.6 com versão do CUDA Toolkit.

## O que guardar

- SM é a unidade física que recebe blocks e mantém warps residentes.
- A GPU busca throughput com paralelismo massivo e troca de warps, não baixa latência por
  thread.
- O número real do dispositivo vem do runtime/medição; a arquitetura só dá limites.

**Conexão:** o próximo arquivo mapeia a hierarquia lógica CUDA (thread/block/grid) nessa
hierarquia física.

Fonte primária: [NVIDIA Ampere Tuning Guide](https://docs.nvidia.com/cuda/ampere-tuning-guide/).

