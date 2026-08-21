# Exercícios — Phase 1: Computer Architecture

## Filosofia (leia antes de começar)

Este conjunto de exercícios **não é repetição da teoria**. O ciclo esperado é:

```text
PERGUNTA → CONFUSÃO → PESQUISA → HIPÓTESE → CÓDIGO → MEDIÇÃO → EXPLICAÇÃO
```

Várias perguntas abaixo são marcadas com **🔎 INVESTIGATE** — isso significa que a
resposta **não está** garantida só no que os arquivos de teoria já deram. É proposital.
Pesquisar é parte do exercício, não uma falha do material. Essas perguntas não têm gabarito
colado embaixo — no máximo uma pista pequena, pra usar só se travar de verdade.

Nenhum exercício deste arquivo tem solução neste repositório (mesma regra do resto do
lab). Isso vale ainda mais aqui: entregar a resposta cedo destrói o exercício.

Níveis (a maioria dos exercícios daqui é 3+, de propósito — menos exercícios, mais densos):

```text
1 Understand   2 Predict   3 Investigate   4 Measure   5 Explain   6 Apply a algo novo
```

---

## A) Recall — checagem rápida (nível 1)

Só pra confirmar que os termos básicos estão firmes antes de ir pro resto.

1. Qual a diferença entre *latency* e *bandwidth*?
2. O que Little's Law relaciona, numa frase só?
3. O que significa dizer que uma operação é "memory-bound"?

---

## B) Investigation — 🔎 INVESTIGATE (nível 3)

Informação incompleta de propósito. Se travar, pesquise os termos na pista — não antes.

4. 🔎 Por que percorrer uma linked list (`node = node->next` repetidamente) pode ser
   **muito** mais lento que percorrer um array, mesmo que os dois tenham exatamente o
   mesmo número de elementos e o mesmo tanto de memória total?
   <br>*Pista se travar: cache locality, dependent loads, hardware prefetching, MLP.*

5. 🔎 Uma função recebe dois ponteiros `const float*` como entrada. Isso, sozinho, **não**
   é suficiente pra o compilador vetorizar o loop com confiança total. Por que não? O que
   está faltando?
   <br>*Pista se travar: aliasing, `restrict`.*

6. 🔎 Você aumenta os acumuladores independentes de uma soma de 1 pra 4 e o ganho de
   velocidade é grande. Aumenta de 4 pra 64 e o ganho quase desaparece. O que colocou um
   teto nesse ganho?
   <br>*Pista se travar: unidades de execução físicas, register pressure.*

7. 🔎 Little's Law diz quanta concorrência de requisição você **precisaria** ter pra
   saturar a bandwidth do seu hardware. Mas ter essa concorrência disponível não garante
   que você vai efetivamente saturar a bandwidth teórica. O que mais pode limitar isso,
   mesmo com concorrência de sobra?
   <br>*Pista se travar: limite de fila do hardware, contenção, bandwidth teórica vs empírica.*

---

## C) Predict Before Running (nível 2)

Escreva sua previsão **antes** de rodar. Depois compare com o resultado real — a
divergência entre previsão e resultado é o material da seção D.

8. Duas versões de soma de array de 20M floats: uma com `soma += arr[i]` (1 acumulador),
   outra com 4 acumuladores independentes somados no final. Qual você espera que seja mais
   rápida, e por quanto (ordem de grandeza, não número exato)?
9. Percorrer um array sequencialmente (`arr[i]`) vs percorrer com stride 16
   (`arr[(i*16) % n]`, mesmo total de acessos). Qual mais rápida, e por quê?
10. `01-latency-benchmark` com a versão original (stride fixo) vs a versão corrigida
    (permutação aleatória de Sattolo) — qual reporta números de latência **maiores**?
    Antes de olhar o código já corrigido, tente prever.
11. O mesmo código, compilado com `-O0` e com `-O3`. Qual a ordem de grandeza da diferença
    que você espera — 10%? 2x? 10x?

---

## D) Explain the Result (nível 5)

Depois de rodar os experimentos da seção C, responda estas — sem consultar teoria de novo
antes de tentar.

12. A diferença entre `-O0` e `-O3` no exercício 11 provavelmente foi bem maior do que
    "um pouco mais rápido". Cite pelo menos **dois** mecanismos diferentes (de arquivos
    diferentes deste módulo) que contribuem pra isso.
13. No `01-latency-benchmark`, a latência não sobe suavemente conforme o buffer cresce —
    ela dá saltos bruscos em tamanhos específicos, com platôs entre eles. Por que degraus,
    e não uma reta?

---

## E) Debug the Mental Model (nível 3)

Cada afirmação abaixo é plausível, mas está incompleta ou errada. Diga o que está
faltando — não vale só "está errado", explique o mecanismo que a afirmação ignora.

14. *"Minha RAM tem latência de 60ns. Portanto, cada load sequencial precisa esperar 60ns
    antes do próximo."*
15. *"Se a AI algorítmica calculada na mão bateu exatamente com a razão GFLOP/s÷GB/s
    medida no benchmark, isso prova que o hardware moveu exatamente essa quantidade de
    bytes."*
16. *"Um roofline construído com números de datasheet é sempre inválido pra decisão de
    otimização."*
17. *"Se eu tenho concorrência (MLP) disponível de sobra no meu padrão de acesso, sempre
    vou conseguir saturar a bandwidth teórica do hardware."*

---

## F) Performance Mysteries — 🔎 INVESTIGATE (nível 3–4)

Título genérico de propósito. Descubra sozinho qual conceito resolve cada uma antes de
pesquisar.

18. 🔎 **Por que a Versão B é ~4x mais lenta?** Duas funções somam os mesmos `n` números,
    fazendo a mesma quantidade de operações aritméticas. Versão A usa 4 variáveis
    acumuladoras separadas; Versão B usa 1 só. Implemente as duas, meça, e só depois
    explique o mecanismo (não é sobre memória — é sobre outra coisa que este módulo já
    cobriu).
19. 🔎 **Por que dobrar o array só deixou 5% mais lento, não 2x?** Um loop que soma um
    array streaming (acesso sequencial) tem seu tamanho dobrado, mas o tempo de execução
    quase não muda proporcionalmente. Que combinação de conceitos deste módulo explica
    isso?
20. 🔎 **Por que a Versão A é mais rápida com array pequeno, mas mais lenta com array
    gigante?** Duas implementações da mesma operação, medidas em dois tamanhos de entrada
    bem diferentes, trocam de "vencedora" dependendo do tamanho. Investigue o que muda
    estruturalmente entre os dois tamanhos.

---

## G) Calculation + Interpretation (nível 4)

Cálculo sozinho não é o objetivo — a pergunta de interpretação depois do número é a parte
que importa.

21. Um hardware sustenta 30 GB/s, com requisições de 64 bytes e latência de 90ns cada.
    Calcule a concorrência necessária (Little's Law). **Depois**: o que esse número te diz
    sobre como você precisaria estruturar código pra realmente saturar essa bandwidth?
22. Um kernel ocupa 8% do tempo total de um passo de treino. Calcule o speedup total
    máximo teórico (Amdahl, `S → ∞`). **Depois**: isso muda sua decisão de investir um mês
    de engenharia otimizando esse kernel especificamente? O que mais você precisaria saber
    pra decidir com confiança?
23. Uma operação fundida lê 3 arrays e escreve 1, fazendo 2 FLOPs por elemento. Calcule a
    AI algorítmica. **Depois**: se você fundir com mais uma operação que **reusa** um dos
    arrays já lidos (sem ler nada novo), a AI sobe, desce, ou fica igual? Por que isso
    importa na hora de decidir se vale a pena fundir mais uma operação?

24. Pra um hidden size `n = 4096` (típico de Transformer) e batch de 1024 linhas, calcule a
    diferença de bytes totais entre a Estratégia A (duas passadas) e a Estratégia B (uma
    passada, `x` mantido em memória rápida) do RMSNorm (arquivo de teoria 15). **Depois**:
    em MB, essa diferença é grande o suficiente pra importar numa escala de treino real, ou
    é desprezível perto do resto do modelo? O que você precisaria saber sobre o resto do
    passo de treino pra responder isso com confiança (dica: arquivo 14, Amdahl)?

25. 🔎 LayerNorm ingênuo faz 3 passadas sobre `x` (mean, depois var, depois normalizar). Uma
    versão com o truque `mean(x²) - mean(x)²` faz só 2. Por que essa versão de 2 passadas,
    apesar de mover menos bytes, **não é sempre** a escolha certa em código de produção?
    <br>*Pista se travar: estabilidade numérica, cancelamento catastrófico.*

---

## H) Code Experiments — checklist (nível 4)

Implemente você mesmo (sem pedir o código pronto). Alguns já têm infraestrutura pronta nos
`examples/` deste módulo — reaproveite em vez de duplicar.

- [ ] Acesso sequencial vs acesso aleatório num array grande
- [ ] Diferentes strides (1, 4, 16, 64) sobre o mesmo array — meça e plote tempo vs stride
- [ ] Soma com cadeia de dependência (1 acumulador) vs quebrada (4+ acumuladores)
- [ ] Pointer chasing previsível (stride fixo) vs pointer chasing aleatório (Sattolo) — já
      existe em `examples/01-latency-benchmark/`, mas vale reimplementar sozinho pra fixar
- [ ] `-O0` vs `-O3` no mesmo código-fonte, sem mudar uma linha
- [ ] Um loop puramente memory-bound (`vector_add`) vs um puramente compute-bound
      (`peak_compute_benchmark.cpp`, já existe) — compare o quanto cada um se aproxima do
      teto correspondente no roofline
- [ ] Working set pequeno (cabe em L1) vs gigante (maior que L3) na mesma operação

---

## K) CUDA Bridge — hipótese, não resposta (nível 6)

Estas perguntas **não têm resposta completa esperada agora**. O objetivo é formular uma
hipótese própria, por escrito, **antes** de estudar Phase 2 — depois você compara sua
hipótese com o que a Phase 2 realmente ensina.

27. Se uma CPU usa execução fora de ordem e prefetching de hardware pra esconder latência
    de memória, como você imagina que uma GPU com milhares de threads simultâneas poderia
    atacar o mesmo problema, usando um mecanismo estruturalmente diferente?
28. SIMD processa ~8 valores numa única instrução, numa CPU. Que hipótese você tem sobre
    como uma GPU "escala" essa mesma ideia pra milhares de valores ao mesmo tempo — é
    literalmente SIMD maior, ou é outra coisa?
29. Little's Law diz que você precisa de N requisições de memória em voo, simultaneamente,
    pra saturar uma dada bandwidth. Numa GPU, qual você imagina que seja a "unidade" que
    gera essa concorrência — um núcleo só trabalhando mais rápido, ou algo estrutural
    diferente?

---

## Registro

Exercícios com medição (seção C, D, F, H, e o 24 de normalização) têm seus números e
explicações registrados no `checkpoint.md` — não só "fiz", mas o que você mediu e por que
aconteceu. As hipóteses da seção K também vão pro checkpoint, pra comparar depois com a
Phase 2 real.
