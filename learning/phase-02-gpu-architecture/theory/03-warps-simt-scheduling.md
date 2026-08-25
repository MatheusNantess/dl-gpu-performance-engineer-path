# 03 — Warps, SIMT e Warp Scheduling

> Pré-requisito: [02 — threads, blocks e grids](02-threads-blocks-grids.md) e SIMD da Phase 1.

## 1. Warp: a unidade operacional

Threads consecutivas de um block são agrupadas em **warps de 32 lanes**. O modelo CUDA
apresenta threads com registradores, índices e fluxo de controle próprios, mas o hardware
agrupa lanes ativas de um warp para executar uma instrução comum. Isso é **SIMT — Single
Instruction, Multiple Threads**.

```text
warp: lane 0  lane 1  ... lane 31
      thread  thread       thread
         \       |          /
            instrução emitida
```

## 2. SIMD não é SIMT com outro nome

Em SIMD, uma instrução vetorial opera sobre posições de um registrador vetorial e há um
fluxo de controle explícito. Em SIMT, o programador escreve threads escalares, cada uma com
estado lógico próprio; o hardware as agrupa. Essa abstração permite branches por thread,
mas diferenças dentro do warp criam máscaras e potencial divergência.

Desde Volta, **Independent Thread Scheduling** mantém estado por thread. Portanto, código
que pressupõe sincronização implícita entre lanes é frágil; use primitivas como
`__syncwarp()` quando a comunicação intra-warp exigir ordem.

## 3. Resident, ready, eligible, issued

- **Resident:** estado do warp está alocado no SM.
- **Ready:** dependências da próxima instrução foram satisfeitas.
- **Eligible:** ready e apto a ser escolhido pelo scheduler naquele ciclo.
- **Issued:** o scheduler efetivamente enviou a instrução a uma unidade.

Occupancy conta warps **residentes**, não garante que estejam ready/eligible. Quarenta e
oito warps esperando a mesma dependência não fornecem trabalho útil ao scheduler.

## 4. Trocar de warp é barato

O estado dos warps residentes já está no SM, especialmente em registradores. Quando um
warp espera um load, o scheduler pode emitir instrução de outro warp pronto sem troca de
contexto de sistema operacional. Essa é a base de latency hiding desenvolvida no arquivo 08.

## Pergunta rápida

Um SM tem 24 warps residentes, mas apenas 1 está eligible. A occupancy pode parecer razoável;
o throughput de emissão precisa ser alto?

<details><summary>Resposta</summary>

Não. Occupancy descreve residência. Se 23 warps esperam dados ou dependências, o scheduler
tem só um candidato e pode deixar unidades ociosas. Warps eligible por ciclo é mais próximo
da pergunta de utilização, embora ainda não explique tudo.

</details>

## O que guardar

- Warp tem 32 lanes e é a unidade de emissão relevante para coalescing/divergência.
- SIMT oferece estado lógico por thread, não um vetor rígido exposto pelo programa.
- Resident não significa ready; ready não significa issued.

**Conexão:** para saber por que um warp deixa de ficar ready, precisamos mapear onde seus
dados vivem e quanto custa alcançá-los.

Fonte primária: [CUDA Programming Guide — Programming Model](https://docs.nvidia.com/cuda/cuda-programming-guide/01-introduction/programming-model.html).

