# Exemplo 02 — Benchmark de Bandwidth Sustentável

Mede a bandwidth real de memória (não a de datasheet) usando um kernel estilo **STREAM
Triad** — o benchmark padrão da indústria pra medir bandwidth sustentável de RAM.

```text
c[i] = a[i] + scalar * b[i]
```

Essa operação lê dois arrays grandes, escreve um terceiro, e faz pouquíssimo cálculo por
byte movido (AI baixa, de propósito — o objetivo é saturar a memória, não a ULA, pra medir
o teto real de bandwidth, não ficar limitado por cálculo).

## Por que arrays GRANDES (maiores que a cache)

O benchmark usa arrays deliberadamente maiores que a L3 da sua CPU. Isso garante que os
acessos batem em RAM de verdade — se os arrays coubessem em cache, você estaria medindo
bandwidth de cache, não de RAM, e teria um número inflado que não representa o teto real
usado no roofline (arquivo 09).

## Como rodar

```bash
g++ -O2 -o bandwidth_benchmark bandwidth_benchmark.cpp
./bandwidth_benchmark
```

## O que observar

O programa imprime a bandwidth sustentável medida em GB/s. Esse é o número que vai pro
`checkpoint.md` deste módulo e alimenta diretamente o cálculo do ridge point (arquivo 08) e
o eixo do roofline (arquivo 09) — **não** use o número de bandwidth do datasheet da sua RAM
para isso.
