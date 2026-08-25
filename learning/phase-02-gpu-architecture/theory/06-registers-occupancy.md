# 06 — Registradores, Register Pressure e Occupancy

> Pré-requisito: [02 — mapeamento](02-threads-blocks-grids.md) e [04 — memória](04-gpu-memory-hierarchy.md).

## 1. Registradores são rápidos e finitos

Variáveis locais escalares normalmente vivem em registradores por thread. Mais
registradores podem reduzir loads e aumentar ILP, mas o arquivo do SM é compartilhado por
todas as threads residentes.

```text
registers/block ≈ registers/thread × threads/block
```

Na RTX 3060 há 65536 registradores de 32 bits por SM. Um kernel de 256 threads usando 80
registradores/thread pede cerca de 20480 por block antes de considerar granularidades de
alocação. Isso permite no máximo 3 blocks pelo limite bruto de registers.

## 2. Occupancy

```text
occupancy = warps residentes / máximo de warps residentes do SM
```

Para `sm_86`, o teto é 48 warps/SM. Blocks residentes são limitados simultaneamente por:

- threads/warps por SM;
- blocks por SM;
- registers por SM e por block;
- shared memory por SM e por block;
- limites arquiteturais adicionais/granularidades.

O menor limite vence.

## 3. Register pressure e spilling

Se o compilador precisa de mais valores vivos do que consegue alocar de modo útil, pode
ocorrer **spill** para local memory. Isso troca um recurso rápido por loads/stores na
hierarquia de memória. `nvcc -Xptxas -v` reporta registers e spill stores/loads.

Forçar poucos registradores também pode piorar performance: aumenta spilling ou destrói
ILP. Portanto, “reduzir registers” não é objetivo isolado.

## 4. Por que occupancy máxima não é a meta

Occupancy fornece warps candidatos para esconder latência. Depois que há warps suficientes,
mais occupancy pode não ajudar. Um kernel com occupancy menor pode ganhar por:

- mais ILP por thread;
- mais reuso em registers/shared;
- menos instruções;
- melhor coalescing;
- menos spills.

## Pergunta rápida

Se um kernel passa de 32 para 64 registers/thread e occupancy cai, podemos concluir que ele
ficará mais lento?

<details><summary>Resposta</summary>

Não. Os registradores extras podem eliminar loads, permitir mais reuso ou ILP. A queda só
vira problema se reduzir warps elegíveis abaixo do necessário para esconder stalls. Meça
tempo e confirme o limitador; não use occupancy como placar.

</details>

## O que guardar

- Occupancy é residência, não utilização.
- Registers têm benefício local e custo global de capacidade.
- O primeiro limitador é o mínimo entre vários recursos.

**Conexão:** shared memory também é um recurso por block e adiciona outro fenômeno: banks.

Fonte primária: [Ampere Tuning Guide — Occupancy](https://docs.nvidia.com/cuda/ampere-tuning-guide/#occupancy).

