# Módulo 07 — Resumo essencial (o que realmente importa)

> Gerado em 2026-08-20, depois de ler os 3 arquivos de teoria (`01-the-pipeline.md`,
> `02-error-taxonomy.md`, `03-flags.md`). Isto é o filtro do que vale internalizar —
> o resto (flags raras, header guards em detalhe) é referência de manual, não pra decorar.

## 1. A frase que resume o pipeline

**O compilador só precisa da assinatura pra aceitar uma chamada; o linker é quem exige
o corpo de verdade.** Cada `.cpp` compila isolado, sem saber se as funções que chama
existem de fato em outro lugar — isso só é checado na hora do link.

## 2. Triage de 3 segundos

| Sintoma | Etapa | Causa típica |
|---|---|---|
| Linha do seu arquivo + `error:` | **Compile** | Sintaxe ou tipo errado |
| `undefined reference`, sem linha, menciona `ld`/`collect2` | **Link** | Função declarada mas nunca definida, ou faltou linkar um `.o` |
| `multiple definition` | **Link** | Definição (não só declaração) num header incluído em vários arquivos |
| Programa rodou e quebrou/deu errado | **Runtime** | Bug de lógica ou undefined behavior |
| `warning:` mas compilou | **Compile** | Em C++, quase sempre é bug real |

## 3. Regra prática mais crítica pro lab: nunca medir performance em `-O0`

`-O0` é só pra debugar. Todo benchmark de verdade tem que ser em `-O2` ou `-O3` — código
sem otimização não representa nada que roda em produção.

## 4. Sanitizers — usar por padrão ao debugar memória

```bash
g++ -fsanitize=address -g -O0 -o prog main.cpp     # memory errors, com linha exata
g++ -fsanitize=undefined -g -O0 -o prog main.cpp   # undefined behavior
```

## 5. Warning em C++ quase sempre é bug de verdade

Compilar sempre com `-Wall -Wextra`. Linha de base recomendada pra tudo nesta fase:

```bash
g++ -std=c++17 -Wall -Wextra -g -O0 -o program main.cpp
```

## 6. O que carrega direto pra CUDA

- `nvcc` compila **duas vezes** (host + device) — erro pode vir de qualquer um dos dois
  passos, ou do link. Um `__device__` declarado-mas-não-definido falha no **link**, com
  mensagem que não se parece com o C++ escrito.
- `-arch=sm_86` → **arquitetura da própria GPU do lab (RTX 3060, Ampere consumer)**.
  Compilar pra arquitetura errada falha ou roda devagar/genérico.
- `-lineinfo` → praticamente obrigatório pra profiling (Nsight Compute usa isso pra
  mapear instrução de volta pra linha de código).
- `--use_fast_math` (equivalente a `-Ofast`/`-ffast-math`) → muda resultado numérico.
  É decisão de correção, não só de velocidade — nunca ligar sem pensar, especialmente
  neste lab (que é sobre correção numérica de kernel).
- Erros de runtime em CUDA são **assíncronos**: um kernel launch retorna na hora, então
  uma falha dentro do kernel só aparece na **próxima** chamada CUDA — muitas vezes em
  código sem relação nenhuma. É por isso que todo retorno de chamada CUDA é checado, e
  `cudaDeviceSynchronize()` aparece tanto ao debugar.

## O que ignorar / só consultar quando precisar

`-Wshadow`, `-pedantic`, `-lm`, `-I`, `-L`, detalhes de header guard — referência de
manual, não conhecimento pra carregar na cabeça o tempo todo.
