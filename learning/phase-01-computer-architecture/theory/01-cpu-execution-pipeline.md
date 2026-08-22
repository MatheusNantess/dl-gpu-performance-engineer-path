# 01 — O Pipeline de Execução da CPU

> Pré-requisito: Phase 0 completa. **Você não precisa saber nada de arquitetura de
> computadores ainda** — as quatro seções abaixo (0.1 a 0.4) constroem isso do zero,
> antes de qualquer coisa sobre pipeline. Se você já sabe o que é CPU/instrução/
> registrador/clock, pode pular direto pra seção 1.

## 0.1 — O que é uma CPU, na prática (esqueça tudo que você já ouviu sobre isso)

Uma CPU é um chip físico que faz **uma coisa só, repetidamente, absurdamente rápido**: pega
uma instrução minúscula, faz exatamente o que ela manda, pega a próxima. Ela não "entende"
C++. Ela não sabe o que é um `for`, nem o que é uma função. Tudo que você escreve em C++
precisa primeiro virar uma sequência enorme dessas instruções minúsculas — e isso já
aconteceu com você hoje, sem você perceber que estava vendo isso de perto.

## 0.2 — O que é uma instrução (você já viu instruções de verdade hoje)

Lembra do Módulo 07 (Compilation), quando rodamos `g++ -S` no arquivo com a função `soma`?
Saiu isto:

```
_Z4somaii:
    endbr64
    pushq   %rbp
    movq    %rsp, %rbp
```

**Cada linha dessas é uma instrução.** `pushq` significa "empurra um valor pra pilha".
`movq` significa "move um valor de um lugar pra outro". São operações **absurdamente
simples** — nenhuma delas "calcula uma soma inteira" sozinha. A função `soma(a, b)` inteira,
quando compilada, vira uma sequência de várias dessas instruções minúsculas, uma atrás da
outra. A CPU não vê "somar dois números" como uma coisa só — ela vê uma fila comprida de
passos pequenininhos, e "somar dois números" é o que **emerge** de rodar essa fila inteira,
em ordem.

Isso é literalmente o que o compilador faz: pega seu código C++ (que fala a sua língua) e
traduz pra essa lista de instruções (que fala a língua que o chip entende).

## 0.3 — O que é um registrador (contraste direto com o que você já domina: pointer/RAM)

Repara em `%rbp` e `%rsp` naquele output. Você pode pensar que são endereços de memória
(tipo os `0x1000` que você já manipulou o dia inteiro com pointer) — **não são**. São
**nomes de registradores**.

Um registrador é um espacinho de armazenamento **construído dentro do próprio chip da
CPU**, não na RAM. A diferença não é só "onde fisicamente mora" — é uma diferença de
categoria inteira:

| | Registrador | RAM (o que você já domina) |
| --- | --- | --- |
| Quantos existem | Pouquíssimos (~16 de uso geral, numa CPU x86-64 típica) | Bilhões de posições (gigabytes) |
| Como você "endereça" | Por **nome fixo** (`%rax`, `%rbp`, `%rsp`...) | Por **endereço numérico** (`0x1000`, calculado) |
| Velocidade de acesso | Praticamente instantâneo — literalmente fiado direto na ULA que faz o cálculo | Precisa de uma "viagem" real (arquivo 04 explica o custo disso) |
| Tamanho de cada um | Um número só (8 bytes, numa CPU 64-bit) | O espaço que você quiser, contíguo |

Quando o compilador decide onde uma variável do seu código vai morar durante um cálculo,
ele tem duas opções: deixar ela num registrador (rapidíssimo, mas só cabem pouquíssimas
coisas por vez) ou deixar ela na stack/RAM (mais devagar de acessar, mas espaço
praticamente ilimitado — a stack, aliás, é exatamente a mesma stack que você já estudou a
fundo no Phase 0). `pushq %rbp` está literalmente pegando o que está no registrador `rbp`
e guardando na stack, pra abrir espaço — você já tem o vocabulário de stack pra entender
essa linha, só faltava saber que `%rbp` é registrador, não endereço.

**PERGUNTA RÁPIDA 0.1**: por que só existem "pouquíssimos" registradores (uma dúzia e
pouco), enquanto a RAM tem bilhões de posições?

<details>
<summary>Resposta</summary>

Porque registrador é construído com o material mais caro e mais rápido que existe dentro do
chip — cada um custa espaço físico de silício, energia, e complexidade de fiação direta com
a ULA. Não dá pra ter "bilhões de registradores" pela mesma razão que não dá pra ter uma L1
gigante (arquivo 04 vai formalizar essa mesma tensão: rápido e pequeno vs grande e lento —
aqui você já está vendo a primeira instância dela, um nível acima até da cache).

</details>

## 0.4 — O que é um ciclo de clock

A CPU tem um "relógio" interno — um sinal elétrico que pisca numa taxa fixa e absurdamente
rápida, tipo um metrônomo. "3 GHz" significa **3 bilhões de piscadas por segundo**. Cada
"piscada" é chamada de **ciclo de clock**, e é a unidade de tempo mais básica que existe
dentro do chip — cada passo físico que um circuito faz (mover um sinal elétrico de um lugar
pro outro, por exemplo) acontece sincronizado com essas piscadas.

Quando dissermos, na seção 2, que "buscar uma instrução leva 1 ciclo", isso quer dizer:
leva o tempo de **uma piscada** desse relógio — algo na casa de frações de nanossegundo,
numa CPU moderna.

**PERGUNTA RÁPIDA 0.2**: se uma CPU roda a 3 GHz (3 bilhões de ciclos por segundo), quanto
tempo dura **um único ciclo**?

<details>
<summary>Resposta</summary>

`1 / 3.000.000.000 segundos ≈ 0,33 nanossegundos` por ciclo. Pra comparação: lembra que o
arquivo 04 vai te dizer que um acesso à RAM leva ~100ns? Isso equivale a **~300 ciclos de
CPU** — ou seja, enquanto um acesso à memória acontece, a CPU "perderia" a chance de rodar
umas 300 instruções, se ficasse simplesmente esperando parada. Essa é exatamente a
motivação física por trás de tudo que os arquivos 04 e 05 vão desenvolver depois.

</details>

## O que você já sabe, agora, antes de continuar

- **CPU** roda instruções minúsculas, uma lista comprida delas, gerada pelo compilador a
  partir do seu C++.
- **Instrução** é um passo mínimo (`pushq`, `movq`, `add`, etc.) — você já viu instruções
  reais, geradas do seu próprio código.
- **Registrador** é armazenamento rapidíssimo, dentro do chip, por nome fixo, quantidade
  pequena — diferente de RAM, que você já domina.
- **Ciclo de clock** é a unidade de tempo física mínima dentro do chip — tudo é medido em
  quantos ciclos algo leva.

Com esse vocabulário, a seção 1 (que já existia) agora faz sentido desde a primeira frase.

---

## 1. Intuição: uma instrução não acontece "de uma vez"

Quando você pensa em "a CPU executa uma instrução", a imagem mental costuma ser
instantânea — a instrução chega, acontece, pronto. Isso é falso, e entender **por que** é
falso é a base de tudo que vem depois neste módulo.

Uma instrução de máquina (tipo `ADD R1, R2, R3` — "some R2 com R3, guarde em R1") passa por
**etapas físicas distintas** dentro do chip, cada uma feita por um circuito diferente, uma
depois da outra. Isso não é escolha de design arbitrária — é consequência de como circuitos
elétricos funcionam: cada etapa (buscar a instrução, entender o que ela pede, executar a
operação, guardar o resultado) precisa de um circuito fisicamente diferente, e um circuito
não pode fazer a etapa seguinte antes de a anterior terminar de "assentar" eletricamente.

## 2. As quatro etapas clássicas

```text
FETCH        busca a instrução da memória (endereço vem do program counter)
   |
DECODE       descobre o que a instrução pede: qual operação, quais registradores
   |
EXECUTE      a ULA (unidade lógica-aritmética) faz o cálculo de verdade
   |
WRITEBACK    o resultado é escrito de volta no registrador de destino
```

Cada etapa é um circuito separado dentro do chip. Sem pipelining, a CPU faria: fetch da
instrução 1, decode da instrução 1, execute da instrução 1, writeback da instrução 1, **só
então** fetch da instrução 2. Cada instrução ocupa a CPU inteira, do início ao fim, sozinha.

## 3. Por que isso é um desperdício monstruoso

Enquanto a etapa EXECUTE da instrução 1 está rodando, o circuito de FETCH está **parado,
sem fazer nada**. Ele já terminou seu trabalho da instrução 1 e só vai ser usado de novo
quando a instrução 1 terminar inteira. Isso é físicamente um circuito caríssimo (fabricado
em silício, ocupando espaço no chip, consumindo energia) ficando **ocioso** na maior parte
do tempo.

## 4. Pipelining: sobrepor as etapas

A solução é óbvia uma vez que você vê o desperdício: **por que não deixar o circuito de
FETCH começar a buscar a instrução 2 assim que ele termina de buscar a instrução 1**, sem
esperar a instrução 1 terminar as outras três etapas?

```text
Sem pipeline (uma instrução por vez, do início ao fim):

Ciclo:     1     2     3     4     5     6     7     8
Instr 1:  FETCH DECODE EXEC  WB
Instr 2:                          FETCH DECODE EXEC  WB
                                   (8 ciclos para 2 instruções)


Com pipeline (etapas sobrepostas):

Ciclo:     1     2     3     4     5
Instr 1:  FETCH DECODE EXEC  WB
Instr 2:        FETCH DECODE EXEC  WB
Instr 3:              FETCH DECODE EXEC  WB
                                   (5 ciclos para 3 instruções!)
```

Cada circuito (FETCH, DECODE, EXECUTE, WRITEBACK) está **sempre ocupado**, cada um
trabalhando numa instrução diferente, na mesma "hora do relógio" (ciclo). É a mesma ideia
de uma linha de montagem de fábrica: enquanto a estação 4 finaliza o produto A, a estação 3
já está montando o produto B, a estação 2 já está preparando o produto C.

**PERGUNTA RÁPIDA 1**: numa linha de montagem com 4 estações, depois que a esteira está
"cheia" (regime estável), quantos produtos saem prontos a cada "tick" da esteira? E quantos
"ticks" leva pra montar UM produto do zero, do início ao fim?

<details>
<summary>Resposta</summary>

Um produto sai pronto a cada tick (throughput = 1/ciclo, no regime estável). Mas um produto
específico ainda leva 4 ticks pra passar pelas 4 estações (latência = 4 ciclos). Essa
distinção — quantos saem por tick vs quanto tempo um leva do início ao fim — é exatamente
o assunto do arquivo 05 (latência vs bandwidth). Guarde essa intuição.

</details>

## 5. O nome técnico: throughput vs latência de uma instrução

- **Latência de uma instrução**: quantos ciclos ela leva, do fetch ao writeback, sozinha.
  No exemplo acima, 4 ciclos (uma etapa por ciclo).
- **Throughput do pipeline**: quantas instruções **terminam** por ciclo, em regime
  contínuo. No exemplo acima, 1 instrução terminada por ciclo, depois que o pipeline
  "encheu".

Pipelining **não** deixa uma instrução individual mais rápida — os 4 ciclos de latência
continuam os mesmos. O que melhora é o **throughput**: quantas instruções terminam por
segundo, quando você tem muitas pra rodar em sequência (que é sempre o caso — um programa
real tem milhões de instruções).

## 6. O problema real: pipeline stall (bolha)

Pipelining assume que a instrução seguinte pode começar sem esperar a anterior terminar.
Isso quebra quando existe **dependência** entre instruções:

```text
ADD R1, R2, R3    ; R1 = R2 + R3
SUB R4, R1, R5    ; R4 = R1 - R5   <- precisa do R1 que a instrução anterior calculou!
```

`SUB` não pode fazer seu `EXECUTE` até `ADD` ter feito seu `WRITEBACK` (senão pegaria um
valor velho de `R1`). O pipeline precisa **parar** (inserir uma "bolha") até o valor estar
pronto:

```text
Ciclo:     1     2     3     4     5     6
ADD:      FETCH DECODE EXEC  WB
SUB:            FETCH DECODE [BOLHA] EXEC  WB    <- esperando o R1 ficar pronto
```

Esse tipo de dependência (chamada *data hazard*) é uma das razões pelas quais "mais
instruções" nem sempre significa "mais rápido linearmente" — o pipeline pode ficar
parcialmente parado.

**PERGUNTA RÁPIDA 2**: por que reescrever um loop pra que iterações consecutivas **não**
dependam do resultado uma da outra (ex: somar dois arrays independentes em vez de uma soma
acumulada sequencial) tende a rodar mais rápido numa CPU pipelined, mesmo fazendo o "mesmo
tanto" de trabalho aritmético?

<details>
<summary>Resposta</summary>

Porque instruções independentes não criam data hazards entre si — o pipeline consegue
manter todas as etapas ocupadas sem inserir bolhas, aproximando o throughput real do
throughput teórico (1 instrução terminando por ciclo). Uma soma acumulada sequencial
(`soma += arr[i]`, onde cada iteração depende do resultado da anterior) força uma cadeia de
dependências que impede sobreposição — cada instrução de soma precisa esperar a anterior
terminar. Essa é a raiz de por que reduções bem escritas usam múltiplos acumuladores
independentes (você vai reencontrar essa ideia no Módulo 07 do roadmap, sobre kernels de
redução em GPU).

</details>

## 7. Por que isso importa para CUDA

GPUs não usam pipelining de instrução exatamente do mesmo jeito que CPUs (elas escondem
latência trocando de *warp* em vez de sobrepor etapas de uma única instrução — assunto da
Phase 2), mas o **vocabulário e o raciocínio são os mesmos**: distinguir latência de
throughput, reconhecer que dependência entre operações trava paralelismo, entender que
"tempo de uma operação sozinha" e "quantas operações terminam por segundo em massa" são
perguntas diferentes. Cada uma dessas ideias reaparece, com nome novo, quando você estudar
como um Streaming Multiprocessor esconde a latência de acesso à memória global trocando de
warp.

## 8. Erros comuns

- Achar que pipelining faz cada instrução individual rodar mais rápido. Não faz — melhora
  quantas terminam por unidade de tempo, não quanto tempo uma leva sozinha.
- Ignorar dependência de dado como causa de lentidão — "eu fiz o mesmo número de operações,
  por que ficou mais lento?" quase sempre tem uma cadeia de dependência como resposta.
- Confundir "a CPU tem 4 núcleos" com pipelining — são conceitos diferentes. Pipelining
  acontece **dentro** de um único núcleo, sobrepondo etapas de instruções sequenciais.
  Múltiplos núcleos é paralelismo de outro tipo (você roda programas/threads diferentes
  simultaneamente, cada núcleo com seu próprio pipeline).

## PREDICT BEFORE RUNNING (conceitual, sem código)

Considere um pipeline de 5 estágios (fetch, decode, execute, memory-access, writeback),
rodando 100 instruções **sem nenhuma dependência entre elas**.

1. Quantos ciclos, aproximadamente, o programa inteiro leva? (dica: quantos ciclos pra
   "encher" o pipeline pela primeira vez, mais quantos ciclos depois disso.)
2. Se metade das instruções tivesse uma dependência forçando 2 ciclos de bolha cada, o
   tempo total dobraria, mais que dobraria, ou menos que dobraria? Por quê?

<details>
<summary>Explicação</summary>

1. As primeiras 5 instruções levam 5 ciclos pra "encher" o pipeline completamente (a
   primeira instrução sai pronta no ciclo 5). Depois disso, uma instrução termina por
   ciclo. Total: 5 (para encher) + 99 (as restantes, uma por ciclo) = 104 ciclos, não
   500 — a sobreposição é o que faz a diferença.
2. Menos que dobraria. Cada bolha de 2 ciclos adiciona 2 ciclos de atraso **localizado**,
   mas o pipeline continua funcionando para as instruções não afetadas — não é como se o
   programa inteiro rodasse "do zero, sem pipeline". Metade das instruções (50) causando 2
   ciclos de bolha cada adiciona aproximadamente 100 ciclos extras aos 104 originais — quase
   dobra, mas o raciocínio importa mais que o número exato aqui: bolhas custam **linear**
   ao número de dependências, não multiplicam o tempo total.

</details>
