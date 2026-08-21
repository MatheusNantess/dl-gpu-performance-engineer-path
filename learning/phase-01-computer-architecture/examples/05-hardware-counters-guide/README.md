# Exemplo 05 (opcional) — Guia: Medindo Tráfego Real de Memória com Hardware Counters

> **Status verificado em 2026-08-20**: `perf` **não está instalado** neste ambiente WSL2
> (`which perf` retorna vazio, `perf --version` falha com "command not found"). Este
> arquivo é, portanto, um **guia de procedimento** — documenta como o experimento seria
> feito, sem afirmar que já foi executado ou inventar números de resultado. Se você rodar
> isso no futuro, preencha os resultados reais na seção 5.

## Por que este experimento existe

O `03-arithmetic-intensity-calc/` deste módulo deixa claro que a "AI algorítmica" (contada
na mão, a partir do código) **não é** uma medição de tráfego real de memória — é uma
tautologia matemática quando calculada como GFLOP/s÷GB/s a partir da mesma fórmula que já
tinha sido usada nos dois numeradores/denominadores (arquivo de teoria 07, seção 6). A
única forma de saber quanto tráfego **real** o hardware moveu é medir com **contadores de
performance de hardware** — circuitos dedicados na CPU que contam eventos reais (cache
misses, bytes transferidos, etc.), não inferidos do código-fonte.

Este experimento existe pra, no futuro, demonstrar empiricamente que **bytes algorítmicos
≠ tráfego real de memória necessariamente** — às vezes o hardware move mais (cache line
inteira buscada pra aproveitar 4 bytes), às vezes move menos (reuso de cache elimina uma
leitura que o código "pede" no papel).

## Ferramentas possíveis

| Ferramenta | Onde roda | Nota |
| --- | --- | --- |
| `perf stat` | Linux nativo, ou WSL2 com suporte | **Não confirmado funcionando neste ambiente** — WSL2 é uma VM, e acesso a contadores de hardware do host nem sempre é exposto ao kernel guest por padrão, mesmo depois de instalado |
| Intel VTune | Windows nativo ou Linux | Ferramenta profissional da Intel, mais completa que `perf`, mas exige instalação separada e licença (tem versão gratuita) |
| Nsight Compute | GPU (CUDA) | O equivalente real que você vai usar depois, na Phase 4 — mede tráfego real de DRAM/L2/L1 por kernel, com muito mais confiabilidade que ferramentas de CPU nesse ambiente específico |

**Nota honesta**: dado que este é um ambiente WSL2 (não Linux nativo), mesmo que `perf`
seja instalado, contadores de hardware podem não funcionar corretamente ou exigir
configuração adicional no lado do Windows (habilitar acesso a performance counters pra
VMs, que nem sempre é trivial ou possível dependendo da configuração do sistema). **Não
assuma que funciona sem testar.**

## Procedimento (pra executar no futuro, em ambiente confirmado)

```bash
# instalar (Ubuntu/Debian):
sudo apt install linux-tools-common linux-tools-generic

# verificar que funciona de verdade (não só que instalou):
perf stat -e cache-misses,cache-references,instructions ./algum_programa

# se os contadores retornarem "not supported" ou valores claramente errados
# (ex: 0 pra tudo), o ambiente não suporta acesso a hardware counters --
# nesse caso, considerar rodar em Linux nativo (dual boot) em vez de WSL2.
```

Depois de confirmar que `perf` funciona de verdade:

```bash
g++ -O2 -o ai_measure ../03-arithmetic-intensity-calc/ai_measure.cpp
perf stat -e cache-misses,cache-references,LLC-loads,LLC-load-misses ./ai_measure
```

## O que comparar

1. **Bytes algorítmicos** (calculados na mão, arquivo de teoria 07): `12n` bytes pra
   `vector_add`, por exemplo.
2. **Tráfego real inferido dos contadores**: `cache-misses × tamanho da cache line (64
   bytes)` dá uma estimativa aproximada de bytes que realmente vieram de fora da cache
   (não é exato — cache misses contam eventos, não bytes diretamente, e o mapeamento entre
   os dois tem nuances que a documentação do `perf` explica).
3. **A razão entre os dois** — essa sim seria uma comparação empírica real entre AI
   algorítmica e comportamento de hardware, ao contrário da tautologia do
   `ai_measure.cpp` original.

## Cuidado ao interpretar (mesmo com dados reais)

- `cache-misses` sozinho **não é** uma medição perfeita de bytes de DRAM — um cache miss
  gera uma transação de uma cache line inteira (64 bytes tipicamente), mas o número exato
  de bytes que efetivamente chegaram de DRAM pode variar com prefetch, coalescing de
  requisições, e outros fatores que o contador simples não separa.
- Comparar números de `perf` entre execuções diferentes exige controlar variáveis (mesma
  frequência de clock, mesma carga do sistema, várias repetições) — a mesma disciplina de
  benchmarking rigoroso que o `docs/benchmark-methodology/` do seu lab já exige pra GPU.

## Seção 5 — Resultados (preencher quando executado de verdade)

```text
Data:
Ambiente (WSL2 / Linux nativo / outro):
Ferramenta usada:
Comando exato:

Bytes algorítmicos calculados (vector_add): 12n bytes
Cache-misses medidos:
Estimativa de bytes reais (cache-misses × 64):
Razão (bytes reais / bytes algorítmicos):

Conclusão:
```

**Este experimento é opcional e não bloqueia a conclusão da Phase 1** — o objetivo dele é
aprofundar intuição sobre a diferença entre AI algorítmica e tráfego real, não é um
requisito de gate pra Phase 2.
