# Dissecando a convolução 2D — exercício de trace manual

Objetivo: entender exatamente o que cada loop faz antes de calcular FLOPs/bytes/AI.
Não é pra "resolver rápido" — é pra rastrear iteração por iteração até o código
parar de parecer confuso.

## O código

```c
// Convolução 2D, imagem N x N, kernel K x K, stride 1, sem padding
// saída: (N-K+1) x (N-K+1)
for (int i = 0; i < N-K+1; i++)
  for (int j = 0; j < N-K+1; j++) {
    float sum = 0;
    for (int ki = 0; ki < K; ki++)
      for (int kj = 0; kj < K; kj++)
        sum += img[i+ki][j+kj] * kernel[ki][kj];
    out[i][j] = sum;
  }
```

## Passo 1 — escolha valores pequenos pra traçar na mão

Use **N = 5** e **K = 3**. Isso dá uma imagem 5x5 e uma saída (5-3+1) x (5-3+1) = **3x3**.

Antes de continuar, responda (sem código, só olhando o loop):
- Quantos pixels de saída existem? Quantos pares `(i, j)` isso significa?
- Para cada pixel de saída, quantos pares `(ki, kj)` o loop interno percorre?

## Passo 2 — trace o primeiro pixel de saída (i=0, j=0)

Preencha a tabela abaixo à mão, linha por linha, seguindo a ordem real de execução
do loop (ki é o mais externo dos dois internos, kj é o mais interno de todos):

| ki | kj | img[i+ki][j+kj] → índice real | kernel[ki][kj] | sum acumulado (simbólico) |
|----|----|-------------------------------|----------------|----------------------------|
| 0  | 0  | img[0][0]                     | kernel[0][0]   | + img[0][0]*kernel[0][0]   |
| 0  | 1  |                               |                |                             |
| 0  | 2  |                               |                |                             |
| 1  | 0  |                               |                |                             |
| 1  | 1  |                               |                |                             |
| 1  | 2  |                               |                |                             |
| 2  | 0  |                               |                |                             |
| 2  | 1  |                               |                |                             |
| 2  | 2  |                               |                |                             |

Pergunta: quantas multiplicações e quantas somas aconteceram só para calcular
`out[0][0]`?

## Passo 3 — trace o pixel vizinho (i=0, j=1)

Repita a mesma tabela para `out[0][1]` (mesma estrutura, só troque `j+kj` pelos
novos valores).

Pergunta chave: **quais índices de `img[...]` apareceram nas DUAS tabelas** (a de
`out[0][0]` e a de `out[0][1]`)? Circule ou marque eles.

Isso é o coração do exercício: essa sobreposição é o "reuso" que o cálculo
ingênuo de AI ignora (porque assume que cada pixel de saída relê a janela do
zero, sem cache).

## Passo 4 — generalize

Depois de rastrear os dois pixels na mão, responda em função de N e K (sem
número fixo agora):

1. **FLOPs totais**: quantos pixels de saída existem × quantos flops por pixel?
2. **Bytes movidos (caso ingênuo — sem reuso de cache)**: cada pixel de saída
   relê a janela K×K inteira da imagem + o kernel K×K inteiro, do zero.
3. **AI da operação** = FLOPs / Bytes
4. **Comparação com hardware**: GPU fictícia com pico de 20 TFLOP/s (FP32) e
   banda de 1,5 TB/s.
   - Ridge point desse hardware = ?
   - Essa convolução ingênua é compute-bound ou memory-bound?

## Passo 5 (opcional, pra depois)

Depois de fechar o caso ingênuo: quantos bytes você moveria se pudesse manter a
janela K×K em cache/registrador entre pixels vizinhos, aproveitando a
sobreposição que você marcou no Passo 3? Não precisa resolver agora — só anota
a pergunta pra quando chegarmos em tiling/shared memory.
