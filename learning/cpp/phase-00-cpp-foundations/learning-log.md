# Phase 0 Learning Log

One entry per session. Newest first. Nothing here is filled in for you.

The two sections that matter most are **Python → C++ differences I noticed** and **Connections to CUDA**. Everything else is bookkeeping; those two are where the understanding actually shows up.

## Template

Copy this block for each session.

```markdown
### YYYY-MM-DD — Module NN

## What I studied today

## Concepts I can explain

## Concepts that are still confusing

## Exercises completed

## Exercises I struggled with

## Bugs I encountered

## Memory insights

## Python -> C++ differences I noticed

## Connections to CUDA

## Questions

## Next step
```

## Rules

- **Concepts I can explain** means without notes, out loud. If it needs the file open, it belongs in the next section instead.
- **Bugs I encountered** includes the ones that were my own fault. Those are the useful ones.
- **Questions** stay in the log until answered. An unanswered question from three weeks ago is a signal.
- **Next step** is one concrete action, specific enough to start without re-deciding anything.
- Wrong predictions get recorded. The prediction tables in each `checkpoint.md` are the same idea in aggregate.

## Entries

<!-- Newest entries directly below this line. -->

### 2026-08-20 — Módulos 04/05 (via trilha paralela em C:\cpp_exercicios_variaveis_operacoes)

## What I studied today

Sessão longa (dia inteiro) cobrindo pointers, references, pass-by-value/reference/pointer,
stack, heap/dynamic memory, out-of-bounds, contiguous memory/sizeof, e review cumulativa —
usando os PDFs 06 a 13 de uma trilha paralela (`C:\Users\mathe\cpp_exercicios_variaveis_operacoes\theory\`),
não os módulos oficiais desta pasta. Descoberto no meio da sessão que essa trilha paralela existe
e cobre terreno parecido aos Módulos 04-08 daqui, só que com bem menos exercícios (7 aqui feitos
manualmente pelo assistente vs 20 no Módulo 04 oficial). Decisão: terminar a trilha paralela
primeiro (feito hoje), migrar pra esta trilha oficial a partir de agora.

## Concepts I can explain

- Array decai para pointer ao ser passado pra função; `a[i]` é literalmente `*(a + i)`
  (mesma instrução, não conversão).
- Reference não é cópia — é o mesmo objeto, outro nome, mesmo endereço sempre. Não pode
  nascer sem inicializar, não pode "religar" depois (`ref = outraVar` copia valor, não religa).
- Pass-by-value (cópia, endereço novo) vs pass-by-reference (mesmo endereço, sem `*`) vs
  pass-by-pointer (endereço próprio guardando outro endereço, precisa de `*` pra escrever).
- Stack: lifetime automático, variável nasce/morre com o escopo; retornar `&variavelLocal`
  é dangling pointer clássico. Confirmado em código real que a stack reaproveita endereço
  entre chamadas de função.
- Heap: `new`/`delete[]` manual; `delete p;` só avisa o alocador, não apaga o conteúdo nem
  zera `p` — por isso `p = nullptr;` depois do delete (nessa ordem, nunca a ordem inversa,
  senão vira leak). `delete nullptr;` é seguro (no-op), protege contra double free.
- Out-of-bounds é undefined behavior de verdade — sem checagem em C++ (diferente de Python).
  Confirmado em código que `arr[4]` fora dos limites não trava, só lê memória arbitrária.
- Fórmula de endereço `base + i * sizeof(T)` e a versão row-major 2D `base + (r*C+c)*sizeof(T)`
  — base de memory coalescing.
- `sizeof(pointer)` nunca revela tamanho de array/bloco alocado — sempre retorna o tamanho
  do próprio pointer (8 bytes em 64-bit).
- Struct padding e por que isso quebra coalescing em AoS — conectado sozinho com a ideia de
  SoA antes mesmo de ler formalmente sobre isso.
- A distinção mais treinada hoje, com 45 exercícios de "predict the output": operações NO
  pointer (`p++`, `p = p+n`, `p = &outro`) nunca escrevem em nada; só `*p = valor` escreve
  de verdade, no lugar pra onde `p` aponta NAQUELE momento (não onde apontava quando foi
  criado). Pointers que apontam pro mesmo endereço compartilham escrita; assim que um se
  move, divergem — mas podem "reconvergir" se movidos de volta ao mesmo lugar.

## Concepts that are still confusing

Nada pendente no fim da sessão — os únicos erros do dia (todos corrigidos com prática)
foram: ler `&` de declaração de reference como se fosse pointer; achar que mover um pointer
altera o array; achar que dois pointers no mesmo endereço eram "independentes" mesmo sem
nenhum ter se movido. Todos resolvidos com repetição, sem lacuna de conceito restante.

## Exercises completed

- `ex21` a `ex27` (custom, na trilha paralela): pointer básico, reference básica, pointer
  arithmetic em array, swap via pointer/reference, comparação value/ref/pointer, const
  pointer, `vector_add` completo (`ex27`, feito pelo próprio usuário sem ajuda).
- 45 exercícios "predict the output" gerados sob demanda, em 6 rodadas (P1-P6, Q1-Q10,
  R1-R7, S1-S10, T1-T5, U1-U7), focados nos erros específicos de cada rodada anterior.
  Último bloco (U1-U7): 7/7 corretas.

## Exercises I struggled with

Q2, Q4 (primeira tentativa), Q9, R1, R3, R5, S2, S3, T1 (primeira tentativa) — todos da
mesma família de confusão (mover pointer vs escrever através dele; endereço compartilhado
vs divergente). Taxa de acerto subiu claramente a cada rodada.

## Bugs I encountered

`int* pA = a;` sem `&` no `ex20` (erro de compilação real, `invalid conversion from int to
int*`) — corrigido depois de entender a diferença entre passar valor vs endereço.

## Memory insights

Rodei vários experimentos reais via `wsl g++` pra provar conceitos com endereço impresso de
verdade: diferença de endereço entre by_value/by_ref/by_ptr, reaproveitamento de endereço da
stack entre chamadas de função, leitura de `arr[4]` fora dos limites (deu `0`, não o valor
"esperado" de uma variável vizinha — reforça que UB é literalmente imprevisível).

## Python -> C++ differences I noticed

Python lista/dict se comportam como referência "de graça"; C++ exige escolha explícita
(value/reference/pointer) a cada função. `IndexError` do Python não existe em C++ — acesso
fora dos limites simplesmente não é checado.

## Connections to CUDA

`vector_add(const float* a, const float* b, float* out, int n)` é quase idêntico, na forma,
a um kernel CUDA real de soma de vetores. `cudaMalloc`/`cudaFree` seguem o mesmo modelo
mental de `new`/`delete` (mesmos bugs: leak, dangling, só que mais caros de errar — podem
destruir um treino de dias). Fórmula de endereço 2D é a base de indexação de kernel de
matmul. Struct padding/AoS vs SoA é decisão de performance real em kernel de deep learning.

## Questions

Nenhuma pendente — sessão fechou com veredito de "pronto pra seguir", validado por 45
exercícios de previsão sem erro de conceito restante (só refinamentos de leitura, todos
corrigidos).

## Next step

Migrar formalmente pra esta trilha oficial. Como os Módulos 04 (Pointers) e 05 (References)
já foram cobertos em profundidade equivalente hoje, avançar rápido por eles (conferir
`checkpoint.md` de cada um) e focar energia real a partir do Módulo 06 (Memory) em diante.

---

### 2026-08-20 (continuação) — Módulos 07 (Compilation) e 08 (Structs/Layout)

## What I studied today

Migração pra trilha oficial concluída na prática: li e discuti os 3 arquivos de teoria do
Módulo 07 (pipeline de build, taxonomia de erro, flags) e os 3 do Módulo 08 (structs,
alignment/padding, AoS intro). Traduzi os 5 arquivos `.md` do Módulo 08 (README, 3 theory,
checkpoint) do inglês pro português, sobrescrevendo os originais no repo.

## Concepts I can explain

- Pipeline de build: preprocessor → compiler → linker → executável. "Compilador só precisa
  da assinatura; linker exige o corpo." Rodei cada etapa separada (`-E`, `-S`, `-c`) num
  arquivo real, vi o preprocessor inflar 8 linhas pra 36.591.
- Triage de 3 segundos: compile error tem linha no arquivo; link error não tem linha, cita
  `ld`/`collect2`, `undefined reference`/`multiple definition`; runtime error só aparece
  depois do programa já ter começado a rodar.
- Causei os dois tipos de link error de propósito (undefined reference: função declarada
  sem definição; multiple definition: definição real dentro de header incluído em 2 `.cpp`s)
  e li as mensagens reais.
- `-O0` nunca serve pra medir performance — só pra debugar. Regra crítica pro lab.
- Alignment/padding: cada campo pede um offset múltiplo do seu próprio alinhamento; `char`
  aceita qualquer offset (alinhamento 1), por isso nunca "causa" padding antes dele; só
  campos exigentes (int=4, double=8) causam. Padding no final existe pra manter arrays do
  struct alinhados (regra 3, tamanho total múltiplo do alinhamento do struct).
- AoS vs SoA: regra condicional ao padrão de acesso (todos os campos de 1 elemento → AoS;
  1 campo de todos os elementos → SoA). GPU/warp está quase sempre no segundo caso. Conectei
  isso sozinho, antes da leitura formal, com a ideia de memory coalescing.
- Mapeei as 17 fases do ROADMAP.md inteiro; entendi que "CUDA" de verdade só começa na
  Phase 3, depois de Phase 1 (Computer Architecture, ainda CPU) e Phase 2 (GPU Architecture,
  conceitual, ainda sem escrever kernel).

## Exercises completed

Build multi-arquivo feito na mão (header.h + .cpp + main.cpp, compilado em etapas
separadas, linkado, rodando). Não fiz os 7 exercícios oficiais numerados do Módulo 07 nem
os 6 do Módulo 08 (considerei a teoria + demos ao vivo suficientes, dado que o módulo é
mais "conhecimento de encanamento" que habilidade central).

## Bugs I encountered

Dois link errors causados de propósito (ver acima), ambos com mensagem real lida e
explicada.

## Connections to CUDA

`nvcc` compila host+device separadamente — erro pode vir de qualquer um dos dois passos ou
do link; `__device__` sem definição falha no link, não no compile. `-arch=sm_86` é a
arquitetura da própria RTX 3060 do lab. Padding desperdiça bandwidth; `alignas(16)`/`float4`
habilitam load vetorizado; AoS vs SoA é decisão real de kernel (inclusive NCHW vs NHWC).

## Next step

Fechar a Parte E do `cuda-readiness-checkpoint.md` (os 6 exercícios de código: vector_add,
scale em duas formas, média/desvio padrão com output params, normalização in-place, matriz
na heap, struct minimizando sizeof) — escritos sem ajuda, sem consulta. Essa é a barra
concreta que falta pra sair oficialmente da Phase 0 e começar Phase 1.

### YYYY-MM-DD — Template entry (copy, do not delete)

## What I studied today

## Concepts I can explain

## Concepts that are still confusing

## Exercises completed

## Exercises I struggled with

## Bugs I encountered

## Memory insights

## Python -> C++ differences I noticed

## Connections to CUDA

## Questions

## Next step