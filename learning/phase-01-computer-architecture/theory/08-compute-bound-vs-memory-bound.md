# 08 — Compute-Bound vs Memory-Bound

> Pré-requisito: [07-arithmetic-intensity.md](07-arithmetic-intensity.md)

## 1. Intuição: dois "tetos" diferentes, e você bate num ou no outro

Toda operação que roda num processador tem, em teoria, dois limites de velocidade
independentes:

- **O teto de cálculo**: quão rápido o hardware consegue **fazer contas** (medido em
  FLOP/s — operações de ponto flutuante por segundo).
- **O teto de memória**: quão rápido o hardware consegue **mover dado** entre memória e
  unidade de cálculo (medido em bytes/s — bandwidth).

Uma operação real nunca roda mais rápido que o **menor** desses dois tetos, pra ela
especificamente. Qual dos dois te limita depende inteiramente da arithmetic intensity da
operação (arquivo 07) comparada com a proporção entre os dois tetos do seu hardware
específico.

## 2. O raciocínio, passo a passo

```text
Tempo mínimo por causa do cálculo  = FLOPs totais / pico de FLOP/s do hardware
Tempo mínimo por causa da memória  = bytes totais / pico de bandwidth do hardware

Tempo real de execução >= max(tempo mínimo de cálculo, tempo mínimo de memória)
```

O tempo real nunca pode ser menor que o maior desses dois mínimos — mesmo com paralelismo
perfeito e zero overhead, você não escapa do teto mais restritivo. **Compute-bound**
significa que o teto de cálculo é o mais restritivo (você bate nele primeiro).
**Memory-bound** significa o oposto — o teto de memória é o gargalo real, e o hardware de
cálculo fica parcialmente ocioso esperando dado chegar.

## 3. Como decidir qual regime você está em, sem rodar nada

```text
                  AI da operação    vs    (pico de FLOP/s) / (pico de bandwidth)
                                            ^
                                    esse valor tem nome: "ridge point" /
                                    "arithmetic intensity de equilíbrio" do hardware
```

Se a AI da sua operação é **menor** que a AI de equilíbrio do hardware → **memory-bound**
(você não consegue manter a ULA ocupada, porque dado não chega rápido o suficiente).

Se a AI da sua operação é **maior** que a AI de equilíbrio do hardware → **compute-bound**
(dado chega rápido o suficiente pra manter a ULA sempre ocupada; o gargalo é quanto
cálculo o hardware consegue fazer).

## 4. Um exemplo numérico completo, na mão

Suponha um hardware fictício (números redondos, só pra ilustrar o raciocínio):

- Pico de cálculo: 100 GFLOP/s
- Pico de bandwidth: 50 GB/s
- AI de equilíbrio do hardware: `100 GFLOP/s / 50 GB/s = 2 FLOPs/byte`

Agora compare com as operações que você já calculou no arquivo 07:

| Operação | AI calculada | vs equilíbrio (2.0) | Regime |
| --- | --- | --- | --- |
| `vector_add` | 0.083 | muito menor | **memory-bound** |
| cópia simples | 0.0 | menor | **memory-bound** (o caso extremo) |
| matmul ingênuo | 0.25 | menor | **memory-bound** |
| matmul com tiling bom | pode passar de 2.0 | maior, dependendo do tiling | **compute-bound** |

**PERGUNTA RÁPIDA 1**: se você dobrasse a AI do `vector_add` de alguma forma (por exemplo,
fundindo com mais uma operação matemática por elemento, sem aumentar os bytes lidos), ele
sairia do regime memory-bound nesse hardware fictício?

<details>
<summary>Resposta</summary>

Não — dobrar 0.083 dá 0.166, ainda muito menor que 2.0. `vector_add` (e a maioria das
operações elemento-a-elemento simples) tem AI **estruturalmente** baixa demais pra sair de
memory-bound em hardware moderno, não importa quanto você "adicione" cálculo por cima —
seria preciso multiplicar a AI por mais de 20x pra cruzar o ridge point desse hardware
fictício. É por isso que operações desse tipo (normalização, ativação, elemento-a-elemento
em geral) são quase sempre tratadas como memory-bound por padrão, na literatura e na
prática — inclusive é literalmente a premissa do North Star do seu lab ("normalização é
memory-bound; a questão inteira é sobre mover menos dado").

</details>

## 5. O que fazer em cada regime (a consequência prática)

- **Se memory-bound**: otimizar cálculo é inútil — a ULA já está esperando dado boa parte
  do tempo. As alavancas reais são: reduzir bytes movidos (fusão, eliminar intermediários,
  reusar cache), aumentar bandwidth efetivo (coalescing, layout melhor — Módulo 08 do
  Phase 0), ou aumentar a AI da operação (fazer mais cálculo aproveitando o dado que já foi
  trazido, essencialmente "de graça").
- **Se compute-bound**: reduzir bytes movidos não ajuda além de um certo ponto — a ULA já
  é o gargalo. As alavancas reais são: reduzir FLOPs necessários (algoritmo melhor),
  aumentar throughput de cálculo (usar Tensor Cores, precisão menor — FP16 em vez de FP32,
  SIMD/vetorização), melhorar ocupância/paralelismo pra manter mais unidades de cálculo
  ocupadas simultaneamente.

Otimizar na direção errada do regime que você está é a forma mais comum de gastar tempo de
engenharia sem ganhar velocidade nenhuma — e é exatamente o motivo de arithmetic intensity
ser calculada **antes** de qualquer tentativa de otimização, não depois.

## 6. Por que isso importa para CUDA

Essa classificação é literalmente o primeiro passo de qualquer sessão de profiling sério em
GPU — Nsight Compute tem uma visão chamada "Speed of Light" que existe especificamente pra
te dizer, de cara, se um kernel está limitado por compute ou por memória, comparando
utilização medida contra os dois tetos do hardware. O seu próprio `ROADMAP.md` (Phase 4,
Profiling) cita isso nominalmente: "classificar um kernel como memory- ou compute-bound a
partir do Speed of Light" é um critério de conclusão explícito.

## 7. Erros comuns

- Assumir o regime sem calcular — intuição sobre "isso parece pesado computacionalmente"
  frequentemente está errada; a fórmula existe justamente porque intuição falha aqui.
- Esquecer que o "ridge point" (AI de equilíbrio) é uma propriedade do **hardware
  específico**, não universal — a mesma operação pode ser memory-bound numa GPU e
  compute-bound noutra, se a proporção entre pico de cálculo e pico de bandwidth for
  diferente entre os dois dispositivos (é literalmente por isso que a Phase 14 do seu
  roadmap valida resultados entre RTX 3060, A100 e H100 — a proporção muda entre GPUs).
- Tratar "memory-bound" como sinônimo de "ruim" ou "ineficiente". Não é — é uma propriedade
  matemática da operação. Uma soma de vetores **é** memory-bound por natureza; isso não é
  falha de implementação, é a estrutura FLOPs/byte da própria matemática.

## PREDICT BEFORE RUNNING (cálculo)

Continue com o hardware fictício do exemplo acima (pico 100 GFLOP/s, pico 50 GB/s, ridge
point = 2.0 FLOPs/byte).

1. Uma operação com AI = 5.0 FLOPs/byte — qual regime?
2. Uma operação com AI = 2.0 FLOPs/byte exatamente — o que isso significa (dica: é o ponto
   onde os dois tetos se cruzam)?
3. Se você trocasse esse hardware fictício por um com o **dobro** de bandwidth (100 GB/s),
   mantendo o mesmo pico de cálculo, o ridge point sobe ou desce? O que isso implica sobre
   quais operações "migram" de regime?

<details>
<summary>Explicação</summary>

1. Compute-bound (5.0 > 2.0, o teto de cálculo é atingido primeiro).
2. É exatamente o ridge point — a operação atinge os dois tetos ao mesmo tempo, no ponto
   teórico de equilíbrio perfeito entre cálculo e memória. Na prática, quase nada cai
   exatamente nesse ponto; é mais um limite conceitual que separa os dois regimes.
3. Ridge point desce: `100 GFLOP/s / 100 GB/s = 1.0 FLOPs/byte` (metade do valor anterior).
   Isso significa que operações com AI entre 1.0 e 2.0, que antes eram memory-bound nesse
   hardware, agora seriam compute-bound no hardware novo — mais bandwidth "empurra" o
   ridge point pra baixo, tornando mais operações compute-bound relativamente. Isso é
   exatamente por que GPUs de datacenter (com bandwidth muito maior que consumer) mudam
   qual regime um kernel específico cai em — a mesma lógica por trás da validação
   cross-GPU do seu roadmap.

</details>
