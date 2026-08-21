# Exemplo 03 — Arithmetic Intensity, na Mão e em Código

## Cálculo na mão (resumo — derivação completa está no arquivo de teoria 07)

### `vector_add`: `out[i] = a[i] + b[i]`

```text
FLOPs por elemento:  1 (uma soma)
Bytes por elemento:  4 (le a) + 4 (le b) + 4 (escreve out) = 12
AI = 1 / 12 ≈ 0.083 FLOPs/byte
```

### Matmul ingênuo N×N: `C[i][j] = soma_k( A[i][k] * B[k][j] )`

```text
FLOPs totais:   2 * N^3   (N multiplicacoes + N somas, por elemento de C, N^2 elementos)
Bytes (pior caso, sem reuso de cache): proporcional a N^3 tambem
AI ≈ 0.25 FLOPs/byte (constante, independente de N, na versao ingenua)
```

## O que este exemplo faz DE VERDADE (e o que ele NÃO prova)

`ai_measure.cpp` roda as duas operações em arrays grandes o suficiente pra não caber
inteiro em cache, mede o tempo real, e calcula:

1. **GFLOP/s alcançado** = FLOPs contados na mão / tempo medido — **isso é uma medição
   real**, tempo de parede de verdade.
2. **GB/s alcançado** = bytes contados na mão / tempo medido — **também é uma medição
   real**.
3. A **razão entre os dois dois números acima**.

**Atenção a uma armadilha real que o próprio autor deste material cometeu na primeira
versão**: o item 3 (a razão GFLOP/s ÷ GB/s) **não é uma confirmação empírica independente**
da arithmetic intensity — é uma **tautologia matemática**. Como os FLOPs e os bytes usados
nas contas 1 e 2 vêm da **mesma fórmula contada na mão** (não de um hardware performance
counter medindo tráfego real), e os dois são divididos pelo **mesmo tempo medido**, o tempo
se cancela na razão:

```text
GFLOP/s     (FLOPs_mão / tempo)     FLOPs_mão
───────  =  ─────────────────  =   ─────────    <- exatamente a AI que você já
GB/s        (bytes_mão / tempo)    bytes_mão        tinha calculado na mão antes!
```

Ou seja: **o resultado "bater exatamente" com o cálculo manual é garantido pela álgebra,
não uma prova de que o hardware moveu exatamente aquela quantidade de bytes.** O que este
exemplo prova de verdade é mais modesto, mas ainda útil: (a) o **throughput real** (GFLOP/s
e GB/s) que a operação atinge nesse hardware, medido de verdade; (b) que a contagem de
FLOPs/bytes no código não tem erro de digitação óbvio (se a razão não bater com a conta
manual, tem bug na contagem). Isso é chamado de **AI algorítmica** (arquivo de teoria 07,
seção 6) — uma estimativa a partir do código, não uma medição de tráfego real de hardware.

**O que provaria de verdade que aquele é o tráfego real**: usar contadores de performance
de hardware (`perf stat` no Linux, com eventos de cache-misses/memory-bytes; ou, em GPU,
o Nsight Compute medindo tráfego real de DRAM/L2) — isso mede o que o **hardware**
efetivamente moveu, que pode ser diferente do que o código "pede" no papel (cache line
inteira buscada pra aproveitar 4 bytes, reuso de cache não contado, etc — arquivo 07,
seção 6, explica por que essa diferença existe).

## Como rodar

```bash
g++ -O2 -o ai_measure ai_measure.cpp
./ai_measure
```

## O segundo programa: `peak_compute_benchmark.cpp`

`vector_add` e matmul ingênuo são **memory-bound** (arquivo de teoria 08) — rodá-los nunca
revela o pico real de cálculo do hardware, porque a ULA passa boa parte do tempo esperando
dado, não calculando. Pra medir o **teto de cálculo** de verdade (necessário pro roofline,
arquivo 09), é preciso um workload desenhado especificamente pra saturar a ULA: dados
mantidos em variáveis locais (sem tocar memória no loop quente), com **múltiplos
acumuladores independentes** (lembra do arquivo de teoria 02, ILP — um acumulador só criaria
uma cadeia de dependência que impediria o hardware de atingir o pico real).

```bash
g++ -O2 -o peak_compute_benchmark peak_compute_benchmark.cpp
./peak_compute_benchmark
```

O número impresso é o `PEAK_GFLOPS` de verdade pra usar no `examples/04-roofline-plot/`.

## Conectando com o roofline (arquivo 09)

Depois de rodar `01-latency-benchmark`, `02-bandwidth-benchmark` e
`peak_compute_benchmark.cpp` acima, você tem os dois picos reais do seu hardware. Compare o
GFLOP/s e GB/s alcançados por `vector_add`/matmul contra esses picos — essa comparação é
exatamente o que vira um ponto no gráfico roofline do exemplo 04.
