# Checkpoint — Módulo 08: Structs e Layout

## 📘 Teoria

- [ ] Consigo explicar por que `sizeof(struct)` costuma ser maior que a soma dos campos
- [ ] Consigo enunciar as três regras de posicionamento e aplicá-las na mão
- [ ] Consigo explicar por que existe padding no final
- [ ] Consigo prever o `sizeof` de um struct antes de compilar
- [ ] Consigo explicar por que a ordem dos campos muda o tamanho, e dar a heurística de ordenação
- [ ] Consigo explicar o que `alignas` faz e por que `float4` precisa disso
- [ ] Consigo explicar a diferença entre AoS e SoA, e qual padrão de acesso favorece cada um
- [ ] Consigo explicar por que padding custa banda de memória num kernel memory-bound
- [ ] Consigo explicar por que um array de structs torna as leituras de um warp não-contíguas

## 💻 Exercícios

- [ ] Completei os exercícios FÁCEIS (01–02)
- [ ] Completei os exercícios MÉDIOS (03–05)
- [ ] Completei o exercício DIFÍCIL (06)
- [ ] Previ todo `sizeof` antes de compilar
- [ ] Usei `offsetof` pra localizar padding em vez de inferir

## 🔬 Mastery

- [ ] Dado a definição de um struct, consigo calcular seu tamanho e o offset de cada campo na mão
- [ ] Consigo reordenar campos pra minimizar o tamanho, e dizer quanto foi economizado
- [ ] Dado uma estrutura de dados e um padrão de acesso, consigo dizer se AoS ou SoA se encaixa melhor
- [ ] Consigo calcular os bytes desperdiçados para um array de N structs
- [ ] Consigo explicar pra outra pessoa por que layout afeta performance
- [ ] Consigo aplicar isso em algo novo: dado a descrição de um kernel, proponho um layout e justifico

## Previsões de sizeof

O registro que importa aqui — preveja, depois meça:

| Struct | Previsto | Real | Bytes desperdiçados |
| --- | --- | --- | --- |
| `{char, int}` | | | |
| `{int, char}` | | | |
| `{char, int, char}` | | | |
| `{int, char, char}` | | | |
| `{double, char, int}` | | | |
| `{char, double, int}` | | | |

## Phase 0 está quase completa

Depois deste módulo:

1. [challenges/](../challenges/) — oito problemas integrativos
2. [mini-projects/](../mini-projects/) — o CPU array kernel lab e o memory explorer
3. [review/](../review/) — os quizzes cumulativos, se ainda não feitos
4. [cuda-readiness-checkpoint.md](../cuda-readiness-checkpoint.md) — o critério de verdade

Só o último decide se CUDA começa.
