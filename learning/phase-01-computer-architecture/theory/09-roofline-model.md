# 09 — O Modelo Roofline

> Pré-requisito: [08-compute-bound-vs-memory-bound.md](08-compute-bound-vs-memory-bound.md)
>
> Este arquivo não introduz conceito novo — ele **desenha** tudo que você já calculou nos
> arquivos 07 e 08, num gráfico só. É o arquivo mais curto do módulo por design: se os
> anteriores fizeram sentido, este é só a representação visual deles.

## 1. A ideia do gráfico

O modelo Roofline coloca **arithmetic intensity** no eixo X e **performance alcançável**
(FLOP/s) no eixo Y, os dois em escala **logarítmica** (porque os valores variam em ordens
de magnitude). Sobre esses eixos, desenha-se um "teto" — daí o nome "roofline" (linha de
telhado) — formado por dois segmentos:

```text
performance
(FLOP/s,
 escala log)
   ^
   |                              _______________________  <- teto de CÁLCULO (horizontal)
   |                            /                            pico de FLOP/s do hardware,
   |                          /                                constante, não importa a AI
   |                        /
   |                      /    <- teto de MEMÓRIA (diagonal)
   |                    /         inclinação = pico de bandwidth
   |                  /            (mais bandwidth = teto mais alto em cada AI)
   |________________/________________________________________>
                    ^                                    arithmetic intensity
              ridge point                              (FLOPs/byte, escala log)
        (onde os dois tetos se cruzam)
```

- **Região à esquerda do ridge point** (AI baixa): o teto diagonal (memória) é mais baixo
  que o teto horizontal — memory-bound.
- **Região à direita do ridge point** (AI alta): o teto horizontal (cálculo) é mais baixo —
  compute-bound.

## 2. Por que a parte de memória é uma linha DIAGONAL, e a de cálculo é HORIZONTAL

Isso não é escolha visual arbitrária — vem direto da fórmula do arquivo 08:

```text
Performance alcançável limitada por memória = AI × bandwidth de pico
```

Se `AI` dobra e `bandwidth` é fixo (propriedade do hardware), a performance alcançável
dobra também — uma relação **linear** entre AI e performance, que em escala log-log vira
uma **linha reta com inclinação** (a inclinação sendo o próprio bandwidth de pico).

```text
Performance alcançável limitada por cálculo = pico de FLOP/s (constante, não depende de AI)
```

Isso não depende de AI nenhuma — é um teto fixo, uma **linha horizontal**, porque não
importa quão eficiente seu uso de memória seja, você nunca ultrapassa a capacidade bruta de
cálculo do hardware.

**PERGUNTA RÁPIDA 1**: uma operação com AI = 0.5, num hardware com bandwidth de pico
40 GB/s — qual é a performance máxima teoricamente alcançável **por causa da memória**
(ignorando por um momento o teto de cálculo)?

<details>
<summary>Resposta</summary>

`0.5 FLOPs/byte × 40 GB/s = 20 GFLOP/s`. Esse é o "teto de memória" pra essa AI específica —
a operação nunca vai passar disso, não importa quão bem otimizado o cálculo em si seja,
porque ela literalmente não consegue alimentar a ULA com dado rápido o suficiente pra ir
além. Se o teto de cálculo do hardware for maior que 20 GFLOP/s (o caso comum), essa
operação está confortavelmente no regime memory-bound, e o número que importa é este,
não o pico de FLOP/s do hardware.

</details>

## 3. Colocando um benchmark real no gráfico

O ponto inteiro do roofline não é só desenhar os tetos — é colocar **seus próprios
benchmarks medidos** como pontos no gráfico e ver a distância entre onde você está e o
teto correspondente à sua AI:

```text
performance
   ^
   |         _______________________  <- teto de cálculo
   |       /
   |     /      x  <- seu benchmark medido (bem abaixo do teto de memória pra essa AI!)
   |   /   .............
   | /     ^ teto de memória, na AI do seu benchmark
   |/________________________________>
                                   AI
```

A distância vertical entre o ponto medido e o teto correspondente **é** a oportunidade de
otimização restante. Se seu ponto está bem perto do teto, você já está próximo do limite
físico do hardware pra essa AI — otimizar mais não vai render muito. Se está longe, existe
espaço real.

## 4. Dois tipos válidos de roofline: teórico e empírico (e por que a diferença importa)

Existem, na verdade, **duas** formas legítimas de construir os dois tetos:

- **Roofline teórico**: usa o pico de bandwidth e o pico de FLOP/s **teóricos** — os
  números de especificação do fabricante (datasheet). Útil como limite superior absoluto,
  claramente identificado como tal: "isso é o máximo que o hardware jamais poderia
  entregar, em condições perfeitas que não existem na prática".
- **Roofline empírico**: usa os picos **medidos de verdade**, rodando um benchmark de
  bandwidth pura e um benchmark de cálculo puro no seu próprio hardware (o
  `examples/02-bandwidth-benchmark/` deste módulo mede o primeiro; um benchmark de cálculo
  puro, que você escreve como parte do checkpoint, mede o segundo).

**Nenhum dos dois é "errado"** — a própria documentação da NVIDIA diferencia bandwidth
teórica de bandwidth efetiva e recomenda comparar as duas. Mas eles respondem perguntas
diferentes: o teto teórico diz "o que o hardware promete, no papel"; o teto empírico diz "o
que esse hardware específico, com esse driver, esse clock, essa temperatura, de fato
entrega agora". Pra **decisão prática de otimização** — "meu kernel está perto do limite
físico real, ou tem espaço de verdade?" — o teto empírico é o que importa, porque é contra
ele que sua otimização de verdade compete.

**A regra prática deste módulo**: prefira tetos medidos empiricamente sempre que possível
(é o mesmo princípio do `docs/hardware/README.md` do seu lab — resultados medidos, não só
citados). Se usar valor de datasheet por qualquer motivo (falta de tempo pra rodar o
benchmark, comparação de referência), **identifique explicitamente** que aquele número é
teórico, nunca misture os dois tipos no mesmo gráfico sem rotular qual é qual.

## 5. Lendo o gráfico: as três perguntas que ele responde

1. **"Onde minha operação cai?"** — calcule a AI (arquivo 07), veja se está à esquerda ou
   à direita do ridge point.
2. **"Qual é o teto teórico pra essa AI?"** — leia o valor Y do teto (diagonal se
   memory-bound, horizontal se compute-bound) naquele X.
3. **"Quão perto estou do teto?"** — compare seu benchmark medido com esse teto. A razão
   entre os dois é frequentemente reportada como "% do peak alcançado" — uma métrica que
   você vai ver o tempo todo em profiling de kernel real.

## 6. Por que isso importa para CUDA

O roofline model é usado **exatamente da mesma forma** em GPU — só que com dois tetos
diferentes por dispositivo (RTX 3060 tem um par bandwidth/FLOP-s; A100 tem outro,
tipicamente com proporção de bandwidth-pra-compute diferente, o que muda onde o ridge point
cai). Nsight Compute consegue gerar um roofline automaticamente pra qualquer kernel
perfilado. O que você constrói aqui — o hábito de calcular AI na mão antes de medir, e
depois comparar contra tetos medidos — é exatamente o fluxo de trabalho que o
`KERNEL_WORKFLOW.md` do seu lab pede pra todo kernel, sem exceção.

## 7. Erros comuns

- Usar número de datasheet em vez de medir — invalida qualquer conclusão tirada do gráfico.
- Esquecer que o roofline é específico de **um** hardware — o mesmo código, rodando em
  hardware diferente, pode mudar de lado do ridge point inteiramente (viu isso no arquivo
  08, pergunta 3).
- Tratar "estar longe do teto" automaticamente como "código ruim" — às vezes existe uma
  razão legítima (overhead fixo pequeno demais pra amortizar, tamanho de problema pequeno
  demais pra saturar bandwidth). O gráfico mostra a oportunidade teórica, não garante que
  ela seja sempre alcançável ou que valha o esforço de perseguir.

## PREDICT BEFORE RUNNING (síntese do módulo inteiro)

Um hardware fictício: pico de cálculo 200 GFLOP/s, pico de bandwidth 40 GB/s.

1. Calcule o ridge point desse hardware.
2. `vector_add` (AI ≈ 0.083, do arquivo 07) — qual é o teto de performance alcançável para
   ele nesse hardware, e esse teto vem do lado de cálculo ou de memória?
3. Se você medisse `vector_add` rodando a 2.5 GFLOP/s nesse hardware, isso está perto do
   teto ou longe? Calcule a porcentagem do teto atingida.

<details>
<summary>Explicação</summary>

1. `200 / 40 = 5.0 FLOPs/byte`.
2. AI = 0.083 é muitíssimo menor que o ridge point (5.0) — memory-bound, disparado. Teto =
   `AI × bandwidth = 0.083 × 40 = 3.32 GFLOP/s`. Vem do lado de memória (o teto diagonal),
   não do teto de cálculo (que seria 200 GFLOP/s, muito mais alto e irrelevante aqui).
3. `2.5 / 3.32 ≈ 75%` do teto de memória pra essa AI. Isso é **muito bom** — está
   relativamente perto do limite físico teórico para essa operação, nesse hardware. Ainda
   dá pra melhorar um pouco (25% de margem), mas não é um caso de "algo está gravemente
   errado" — é o tipo de número que, num relatório de benchmark real, você reportaria como
   "achieved bandwidth: 75% of measured peak", exatamente o vocabulário do
   `docs/benchmark-methodology/` do seu lab.

</details>
