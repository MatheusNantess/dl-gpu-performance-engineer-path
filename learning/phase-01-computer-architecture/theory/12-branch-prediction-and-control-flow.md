# 12 — Branch Prediction e o Custo de Controle de Fluxo

> Pré-requisito: [01-cpu-execution-pipeline.md](01-cpu-execution-pipeline.md)
>
> **Aviso importante logo de cara**: este arquivo é sobre CPU. A tentação natural é pensar
> "ah, isso deve ser a mesma coisa que divergência de warp em GPU". **Não é** — a seção 6
> deste arquivo é dedicada especificamente a desfazer essa comparação errada antes que ela
> se instale. Leia até o final, não só o início.

## 1. O problema: pipeline e `if` não combinam naturalmente

Lembra do arquivo 01: pipelining funciona buscando a próxima instrução **antes** da atual
terminar, sobrepondo etapas. Isso é fácil quando o código é uma sequência linear — a
"próxima instrução" é sempre, literalmente, a linha seguinte. Mas o que acontece com isto:

```cpp
if (x > 0) {
    y = a + b;   // caminho A
} else {
    y = a - b;   // caminho B
}
```

O circuito de FETCH precisa buscar a **próxima** instrução imediatamente, sem esperar —
mas qual delas, a do caminho A ou a do caminho B? Isso só é decidido depois que `x > 0` é
**avaliado**, o que só acontece depois do EXECUTE da comparação — etapas à frente de onde o
FETCH já precisaria estar agindo, se quiser manter o pipeline cheio.

## 2. A solução: adivinhar, e corrigir se errar (branch prediction especulativa)

CPUs modernas não esperam a resposta — elas **adivinham** qual caminho vai ser tomado (com
base em histórico: esse `if` normalmente foi verdadeiro nas últimas vezes? Foi falso?),
continuam buscando e executando instruções **daquele caminho adivinhado**, especulativamente,
enquanto a condição real ainda está sendo calculada. Isso se chama **execução
especulativa**.

```text
Pipeline especulando (assumindo que vai entrar no `if`):

Ciclo:   1        2        3        4        5
Instr:  if(x>0)  y=a+b    y=a+b    y=a+b     <- já está processando o caminho A,
                 (FETCH)  (DECODE) (EXEC)        antes de saber se x>0 é verdade
```

Se a especulação está **certa**, você ganhou tempo de verdade — o pipeline nunca parou. Se
está **errada** (misprediction), tudo que foi processado especulativamente precisa ser
jogado fora, e o pipeline recomeça do caminho certo — isso se chama **pipeline flush**, e o
custo é proporcional à profundidade do pipeline (CPUs modernas têm pipelines de 15-20+
estágios; uma misprediction pode custar dezenas de ciclos perdidos).

**PERGUNTA RÁPIDA 1**: por que um `if` que é quase **sempre** verdadeiro (99% das vezes),
ou quase sempre falso, é muito mais barato pro hardware do que um `if` que é verdadeiro
"aleatoriamente", cerca de 50% das vezes?

<details>
<summary>Resposta</summary>

O preditor de branch aprende padrões a partir de histórico recente. Um `if` com resultado
quase sempre igual é **trivialmente previsível** — o preditor acerta quase sempre, o
pipeline quase nunca é jogado fora. Um `if` com resultado próximo de 50/50, sem padrão
detectável, é o **pior caso possível** — o preditor erra com frequência alta, pagando o
custo de flush repetidamente. Isso é chamado, na literatura de otimização (o material
"Performance Ninja", por exemplo), de **"bad speculation"** — uma categoria própria de
gargalo de performance, distinta de gargalo de cálculo ou de memória.

</details>

## 3. Branchless code: eliminar o `if` do caminho quente

Uma técnica de otimização real, quando um `if` imprevisível está no caminho crítico de um
loop quente, é **reescrever sem branch nenhum**, usando aritmética ou instruções
condicionais que não desviam o fluxo de controle:

```cpp
// Com branch (imprevisível, caro se x for aleatório):
int y = (x > 0) ? a : b;

// Uma forma "branchless" equivalente, usando aritmética:
int mask = -(x > 0);              // 0xFFFFFFFF se verdadeiro, 0x00000000 se falso
int y = (a & mask) | (b & ~mask);
```

Compiladores modernos frequentemente já fazem essa transformação sozinhos quando percebem
que é segura e vantajosa (gerando uma instrução de hardware chamada **CMOV** — move
condicional, que não desvia o pipeline, só escolhe qual valor escrever). Você não
costuma precisar escrever isso manualmente — mas vale saber que existe, porque explica por
que às vezes reescrever um `if` de um jeito aparentemente "mais feio" pode ser mais rápido:
você está removendo a decisão especulativa do caminho quente inteiramente.

## 4. Predication: a versão "estrutural" da mesma ideia

**Predication** é quando o próprio hardware (ou o compilador, gerando código pra hardware
que suporta isso) executa **as duas alternativas** de um `if` pequeno, e no final só
"seleciona" o resultado certo — sem nunca desviar o fluxo de instrução. Isso custa mais
cálculo (as duas alternativas são computadas, mesmo a que não "vale"), mas evita
completamente o risco de misprediction. É uma troca clássica: mais trabalho garantido, em
vez de menos trabalho com risco de flush caro.

## 5. Como isso se conecta com o resto do módulo

Branch prediction é, na prática, uma terceira forma de "gargalo" que você ainda não tinha
formalizado — nem compute-bound (arquivo 08), nem memory-bound, mas **controle de fluxo
imprevisível**. O material "Performance Ninja" trata isso como categoria própria (junto de
"Core Bound" e "Memory Bound"), chamada de "Bad Speculation" — vale ter esse terceiro
balde mental, porque nem todo código lento é lento por cálculo ou por memória.

## 6. Por que isso NÃO é a mesma coisa que divergência de warp em GPU

Aqui está o ponto que este arquivo existe especificamente pra deixar claro, porque a
tentação de generalizar é forte e a analogia **quebra** de um jeito importante.

Divergência de warp (que você já viu de relance no arquivo 03, SIMD) acontece quando
threads **dentro do mesmo warp** tomam caminhos diferentes num `if`. A GPU **não** resolve
isso adivinhando qual caminho a maioria vai tomar e especulando — ela não tem (nem precisa
de) preditor de branch nesse sentido. O modelo é fundamentalmente diferente: SIMT (Single
Instruction, Multiple Threads — o modelo de execução de warp) executa **ambos os caminhos,
sequencialmente**, com as threads que não pertencem àquele caminho **mascaradas** (inativas,
não escrevendo resultado), depois volta a convergir quando os caminhos se reencontram.

```text
Warp de 4 threads, if divergente:

Caminho A (threads 0, 2 ativas; 1, 3 mascaradas):
   t0: executa A    t1: mascarada    t2: executa A    t3: mascarada

Caminho B (threads 1, 3 ativas; 0, 2 mascaradas):
   t0: mascarada    t1: executa B    t2: mascarada    t3: executa B
```

Não existe "acerto" ou "erro" de predição aqui — **os dois caminhos sempre rodam**, sempre,
sequencialmente, sempre que há divergência dentro de um warp. O custo é proporcional a
quantos caminhos diferentes existem, não a "quão bem você adivinhou". É um modelo de custo
completamente diferente do branch misprediction de CPU, mesmo que a causa raiz superficial
("um `if` com resultado diferente pra elementos diferentes") pareça parecida.

**A lição certa a levar daqui**: controle de fluxo (`if`, `switch`, loops com número
variável de iterações) **tem custo real**, em qualquer hardware — isso é universal. **Como**
esse custo se manifesta (misprediction + flush numa CPU; execução sequencial mascarada num
warp de GPU) é **específico de cada arquitetura**, e vale a pena não misturar os dois
modelos mentais. A Phase 2 do seu roadmap vai desenvolver o modelo certo de divergência de
warp, do zero — não tente "importar" o modelo de branch prediction de CPU pra lá.

## 7. Erros comuns

- **O erro mais importante deste arquivo**: assumir que "divergência de warp é tipo branch
  misprediction, só que em GPU". Não é — releia a seção 6 se essa ideia surgiu.
- Reescrever código pra "branchless" sem medir primeiro — se o `if` já é altamente
  previsível (quase sempre mesmo resultado), o custo de misprediction já é baixíssimo, e
  branchless pode até ser mais lento (computando as duas alternativas sempre).
- Achar que todo `if` custa caro. Só `if`s **imprevisíveis**, no caminho quente de um loop
  executado muitas vezes, valem a pena otimizar por esse ângulo.

## PREDICT BEFORE RUNNING (conceitual)

```cpp
// Trecho A: array já ordenado
for (int i = 0; i < n; i++) {
    if (arr[i] > threshold) contador++;   // resultado muda raramente, em bloco
}

// Trecho B: mesmo array, mas embaralhado aleatoriamente antes do loop
for (int i = 0; i < n; i++) {
    if (arr[i] > threshold) contador++;   // resultado imprevisível a cada iteração
}
```

1. Os dois trechos fazem exatamente o mesmo número de comparações e o mesmo trabalho
   "lógico". Qual você espera que rode mais rápido, e por quê?
2. Isso é um exemplo de qual categoria de gargalo (das três que você já conhece: compute,
   memory, ou o terceiro balde deste arquivo)?

<details>
<summary>Explicação</summary>

1. O Trecho A (array ordenado) tende a ser mais rápido — como os elementos estão em ordem,
   o resultado do `if` muda raramente (fica "sim, sim, sim, ..., não, não, não" em vez de
   alternar aleatoriamente), o que é altamente previsível pro branch predictor. O Trecho B,
   com a mesma lógica mas dados embaralhados, tem resultado de `if` essencialmente
   aleatório — o predictor erra com muito mais frequência, pagando pipeline flush repetidas
   vezes. Esse exemplo específico (comparar array ordenado vs não-ordenado) é, inclusive,
   um experimento clássico e amplamente citado na literatura de performance justamente por
   demonstrar esse efeito de forma dramática e fácil de medir.
2. Bad speculation — nenhum dos dois trechos muda quantidade de cálculo nem quantidade de
   dado movido; a única coisa que muda é a previsibilidade do controle de fluxo.

</details>
