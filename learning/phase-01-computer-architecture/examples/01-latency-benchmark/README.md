# Exemplo 01 — Benchmark de Latência de Memória

Mede a latência real de acesso à memória em buffers de tamanhos crescentes, usando a
técnica de **pointer chasing**: cada elemento do array guarda o índice do próximo elemento
a visitar, formando uma cadeia onde cada acesso **depende** do resultado do anterior. Isso
impede o hardware de prever/pré-buscar o próximo endereço (lembra do arquivo 05 — sem
dependência, o hardware sobrepõe requisições e esconde latência; com dependência forçada,
você mede a latência real, sem trapaça).

Conforme o buffer cresce e deixa de caber em L1, depois L2, depois L3, você deve ver a
latência média por acesso **saltar** em degraus — essa é a evidência visual e numérica da
hierarquia de memória do arquivo 04, medida na sua própria máquina, não num diagrama.

## Como rodar

```bash
g++ -O2 -o latency_benchmark latency_benchmark.cpp
./latency_benchmark
```

**Importante (Módulo 07 do Phase 0)**: `-O2`, nunca `-O0`, para medição de performance.

## O que observar

- Anote em qual tamanho de buffer a latência dá o primeiro salto grande (isso é o tamanho
  aproximado da sua L1).
- O segundo salto (L1 → L2) e o terceiro (L2 → L3) devem aparecer também, embora menos
  nítidos dependendo da CPU e do quanto outros processos estão competindo por cache.
- Compare os números medidos com os valores típicos citados no arquivo 04
  (~1ns L1, ~4ns L2, ~15ns L3, ~100ns RAM) — não vão bater exatamente (isso é normal, cada
  CPU é diferente), mas a **ordem de grandeza** e o **padrão de degraus** devem aparecer.

## Onde registrar

Depois de rodar, registre os números medidos no `checkpoint.md` deste módulo, na tabela de
picos medidos — eles alimentam o roofline final.
