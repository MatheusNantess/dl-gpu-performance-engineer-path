# Módulo 08 — Structs e Memory Layout

O último módulo, e a ponte mais direta pro trabalho de performance em GPU.

Um struct agrupa dados relacionados. A parte interessante não é o agrupamento em si — é que **`sizeof(struct)` costuma ser maior que a soma dos seus campos**, e que o layout que você escolhe determina quanta banda de memória um kernel desperdiça.

## Teoria

| Arquivo | Tópico |
| --- | --- |
| [01-structs.md](theory/01-structs.md) | Definição, campos, acesso, arrays de structs |
| [02-alignment-and-padding.md](theory/02-alignment-and-padding.md) | Por que o `sizeof` surpreende |
| [03-aos-intro.md](theory/03-aos-intro.md) | Array of Structures — e uma prévia de por que isso importa |

## Exemplos

- [`examples/01-padding.cpp`](examples/01-padding.cpp)

## Exercícios

6 exercícios em [`exercises/`](exercises/), a maioria de medição: definir, inspecionar `sizeof`, reordenar, observar.

## Escopo

**AoS** (Array of Structures) é introduzido. **SoA** (Structure of Arrays) é nomeado e propositalmente **não** desenvolvido — ele pertence às medições reais de GPU, na Phase 2. O objetivo aqui é tornar a pergunta *visível*, não respondê-la.

Sem construtores, sem métodos, sem herança. Um struct aqui é um layout — que é exatamente o que ele é em código de kernel.

## Checkpoint

[checkpoint.md](checkpoint.md)

## Por que isso importa para CUDA

Este módulo é onde layout deixa de ser abstração.

Padding desperdiça banda de memória. Um struct que tem 20 bytes de dado e 24 bytes de armazenamento significa que um kernel lendo um milhão deles move 4 MB de nada.

Alinhamento habilita loads vetorizados. Ler 16 bytes por instrução com `float4` exige um endereço alinhado a 16 bytes; dado desalinhado silenciosamente cai no caminho lento.

E AoS versus SoA é uma das decisões de layout padrão em GPU. Quando 32 threads leem, cada uma, um campo de um struct, um layout AoS espalha essas leituras pela memória e o coalescing falha. O mesmo dado em forma SoA é contíguo e faz coalescing perfeitamente.

Essa pergunta — **como esse dado está organizado, e quanto custa o padrão de acesso a ele?** — é a mesma pergunta que o North Star de pesquisa deste repositório faz sobre kernels de normalização. Ela começa aqui, num struct pequeno o suficiente pra raciocinar por completo.
