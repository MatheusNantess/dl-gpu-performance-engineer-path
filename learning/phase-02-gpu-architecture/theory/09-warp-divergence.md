# 09 — Warp Divergence e Fluxo de Controle

> Pré-requisito: [03 — SIMT](03-warps-simt-scheduling.md) e branch prediction da Phase 1 (opcional).

## 1. Divergência é local ao warp

Se lanes do mesmo warp tomam caminhos diferentes, o hardware executa os caminhos com
**active masks** diferentes. Lanes fora do caminho atual ficam inativas.

```cuda
if (threadIdx.x % 2 == 0) caminho_A();
else                       caminho_B();
```

```text
A: lanes pares ativas; lanes ímpares mascaradas
B: lanes ímpares ativas; lanes pares mascaradas
reconvergência: todas as lanes ainda existentes voltam ao fluxo comum
```

O custo não é necessariamente “exatos 2x”: depende do tamanho dos caminhos, predication de
branches curtos, loads feitos por cada caminho, reconvergência e outras limitações.

## 2. Branch diferente não implica divergência

Se um warp inteiro toma A e outro warp inteiro toma B, cada warp tem controle uniforme.
Por exemplo, `if ((threadIdx.x / 32) % 2 == 0)` separa warps, mas não lanes do mesmo warp.

Loops também divergem quando lanes fazem números diferentes de iterações. Threads que
terminam cedo ficam inativas enquanto outras continuam.

## 3. Não é branch prediction de CPU

CPU geralmente prediz um caminho e paga flush ao errar. SIMT permite fluxo lógico por
thread, agrupando lanes compatíveis e mascarando outras. Não há “taxa de acerto do branch
predictor” como explicação principal da divergência.

Em Ampere existe Independent Thread Scheduling, mas warp continua sendo a unidade crucial
para eficiência. Isso não torna divergência gratuita nem valida sincronização implícita.

## Pergunta rápida

`if (blockIdx.x % 2 == 0)` faz metade dos blocks executar A e metade B. Há divergência de
warp por causa dessa condição?

<details><summary>Resposta</summary>

Não: `blockIdx.x` é igual para todas as threads do block, logo também para todas as lanes
de cada warp. Pode haver desbalanceamento de duração entre blocks, mas é outro problema.

</details>

## O que guardar

- Sempre pergunte se a condição varia **dentro de um warp**.
- Active lanes, não threads lançadas, determinam utilização de cada instrução.
- Branch uniforme pode ser barato; branchless pode fazer trabalho extra e piorar.

**Conexão:** quando threads cooperam, máscaras e caminhos também determinam se todas chegam
corretamente a uma barreira.

