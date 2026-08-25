# 04 — Hierarquia de Memória da GPU

> Pré-requisito: hierarquia de memória da Phase 1 e [03 — warps](03-warps-simt-scheduling.md).

## 1. O mapa mínimo

```text
por thread:      registers (muito rápidos, privados, escassos)
por block/SM:    shared memory (explícita) + L1 (cache)
por GPU:         L2 cache
fora dos SMs:    global memory / VRAM (grande, alta latência)
```

Há ainda constant/texture caches e local memory, mas não são o eixo principal desta fase.
**Local memory** é um nome perigoso: é privada logicamente por thread, porém fica na
hierarquia de device memory e pode surgir por spilling; não significa “perto”.

## 2. Cache e shared memory não são a mesma coisa

As duas usam recursos próximos ao SM e, em Ampere, compartilham capacidade física/configuração,
mas têm modelos diferentes:

- cache é preenchida/evictada automaticamente pelo hardware;
- shared memory é alocada pelo block e indexada explicitamente pelo programa;
- shared permite comunicação entre threads do mesmo block, com sincronização;
- reservar mais shared por block pode reduzir blocks residentes.

## 3. Escopo e tempo de vida

| Espaço | Visível por | Tempo de vida aproximado |
| --- | --- | --- |
| register | uma thread | thread/kernel |
| shared | threads do block | block |
| L1 | SM (cache) | gerenciado pelo hardware |
| L2 | todos os SMs | gerenciado pelo hardware |
| global/VRAM | grid e host via APIs | alocação |

Visibilidade não é sincronização: duas threads poderem acessar o mesmo endereço não
garante ordem nem elimina data races.

## 4. O custo é dado em camadas

Um load global pode acertar L1, L2 ou ir à VRAM. “Latência da memória global” é, portanto,
dependente do caminho real, padrão de acesso, cache e concorrência. O pointer chasing de
256 MB da Phase 1 foi desenhado para reduzir reuso e medir o platô próximo à VRAM.

## Pergunta rápida

Guardar um tile em shared memory sempre melhora performance?

<details><summary>Resposta</summary>

Não. Só ajuda se o reuso evitar tráfego suficiente para pagar cópia, endereçamento e
barreiras. Shared excessiva também pode reduzir occupancy; acesso por banks pode conflitar.
É uma hipótese a medir, não um ritual.

</details>

## O que guardar

- “Perto” é rápido e pequeno; “longe” é grande e exige concorrência.
- Register e shared são recursos de residência, não apenas locais de armazenamento.
- Local memory pode ser tráfego caro apesar do nome.

**Conexão:** o próximo arquivo analisa como as 32 lanes transformam endereços globais em
transações físicas.

