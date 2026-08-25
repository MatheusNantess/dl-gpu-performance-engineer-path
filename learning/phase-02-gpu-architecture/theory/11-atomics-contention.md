# 11 — Atomics, Contenção e Serialização

> Pré-requisito: data races da Phase 0 e [10 — sincronização](10-synchronization.md).

## 1. O problema de read-modify-write

```cuda
counter = counter + 1;
```

Isso é load → add → store. Threads concorrentes podem ler o mesmo valor e sobrescrever
resultados. `atomicAdd(&counter, 1)` torna a atualização indivisível para o escopo suportado.

Atomicidade não significa:

- ordem previsível entre threads;
- sincronização de todos os outros endereços;
- execução paralela ilimitada no mesmo endereço;
- barreira para o block.

## 2. Contenção

Se muitas lanes atualizam o mesmo endereço, todas as atualizações precisam acontecer sem se
perder. O hardware pode combinar/otimizar partes, mas existe dependência lógica naquele
local: o throughput é limitado e a ordem permanece indefinida.

```text
sharded:   lane 0→c[0], lane 1→c[1], ...   pouca contenção
contended: lane 0→c[0], lane 1→c[0], ...   hotspot
```

Atomics distribuídos por endereços podem escalar muito melhor que um único contador.

## 3. Reduzir antes de atomizar

Uma estratégia comum é agregar dentro de uma thread, warp ou block e emitir menos atomics
globais. Isso troca contenção global por:

- cálculo/redução local;
- shared memory ou warp primitives;
- sincronização;
- código mais complexo.

É a mesma lógica de bytes e Amdahl: reduzir o gargalo dominante, não remover atomics por
princípio.

## Pergunta rápida

Mil threads fazem `atomicAdd(&counter, 1)`. O valor final pode estar correto e o kernel
ainda ser lento?

<details><summary>Resposta</summary>

Sim. Atomic garante que as mil atualizações sejam preservadas, mas todas disputam o mesmo
endereço. Correção e throughput são dimensões separadas.

</details>

## O que guardar

- Atomic resolve indivisibilidade; barreira resolve encontro/ordenação de um grupo.
- Contenção é propriedade do padrão de endereços, não da palavra `atomic` isoladamente.
- Privatização/agregação reduz operações contended ao custo de recursos e sincronização.

**Conexão:** o último capítulo junta unidades de execução e um fluxo de diagnóstico para
escolher qual desses mecanismos investigar.

