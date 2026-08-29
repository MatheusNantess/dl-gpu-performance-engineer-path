# Bitwise operators — o que faltou do C++

Não é CUDA. É C/C++ puro, só que seu Phase 0 não cobriu (é comum ficar de
fora de curso introdutório). Aparece bastante em código de sistemas/GPU
porque trabalhamos com potências de 2 e máscaras de bits o tempo todo.

## Os operadores

| Operador | Nome | O que faz |
| --- | --- | --- |
| `<<` | shift left (desloca pra esquerda) | Move os bits pra esquerda, preenche com 0 |
| `>>` | shift right (desloca pra direita) | Move os bits pra direita |
| `&` | AND bit a bit | 1 só se os dois bits forem 1 |
| `|` | OR bit a bit | 1 se pelo menos um bit for 1 |
| `^` | XOR bit a bit | 1 se os bits forem diferentes |
| `~` | NOT bit a bit | Inverte todos os bits |

**Cuidado pra não confundir com os operadores lógicos** (`&&`, `||`) que
você já conhece de `if` — esses aqui operam bit por bit num número
inteiro, não em `true`/`false`.

## `<<` (o que apareceu no seu código)

```cuda
1 << 20
```

Pega o número `1` em binário e desloca 20 casas pra esquerda:

```
1  em binario: 000...0001
1 << 1  =  10   (binario)  =  2   (decimal)
1 << 2  =  100  (binario)  =  4
1 << 3  =  1000 (binario)  =  8
1 << 20 = 1 seguido de 20 zeros (binario) = 1.048.576 (decimal)
```

**Regra geral: `1 << n` é sempre igual a `2^n`.** É só uma forma mais
idiomática de escrever potência de 2 em código de sistemas — você vai ver
isso toda hora (tamanho de buffer, número de threads, máscaras).

`>>` é o inverso: `1048576 >> 20` volta pra `1`.

## `&`, `|`, `^` com um exemplo concreto que você já viu

Lembra do exercício `05-active-mask`, que imprimiu `0x000fffff`? Isso é um
número em **hexadecimal** (cada dígito hex = 4 bits), e cada bit
representa "essa lane está ativa (1) ou não (0)":

```
0x000fffff em binario = 00000000000011111111111111111111
                                      ^^^^^^^^^^^^^^^^^^^^
                                      20 uns = lanes 0..19 ativas
```

Com `&` você consegue **perguntar** se uma lane específica está ativa:

```cuda
unsigned mask = 0x000fffff;
bool lane5_ativa = (mask & (1 << 5)) != 0;  // desloca um "1" pra posicao 5, compara
```

`(1 << 5)` cria um número com só o bit 5 ligado (`0b100000`). O `&` com a
máscara zera todo o resto, sobrando só o que interessa na posição 5.

## Hexadecimal (`0x...`), já que apareceu junto

`0x` na frente de um número diz "isso está em base 16, não base 10".
Cada dígito hex vale 4 bits (`0-9` e depois `a-f` = 10-15):

```
0xff       = 1111 1111           (8 bits, todos ligados = 255 em decimal)
0xffffffff = 32 bits todos ligados = 4.294.967.295 (usado pra "todas as 32 lanes ativas")
```

Por isso `active_mask.cu` esperava `0xffffffff` quando o warp inteiro
estava reconvergido — 32 bits, um por lane, todos ligados.

## Não precisa decorar tabela de conversão binário↔decimal

O que importa fixar é:
- `<<` e `>>` deslocam bits — na prática, quase sempre você vai usar `1 << n` como jeito de escrever `2^n`.
- `&`, `|`, `^` operam bit a bit — `&` é o mais comum, usado pra "testar" ou "isolar" bits específicos (como numa máscara de warp).
- `0x...` é só uma notação pra escrever número em hexadecimal, comum quando o valor representa bits/flags em vez de uma quantidade.

Isso é o suficiente pra ler o resto do código sem travar. Se aparecer algo
mais específico de bitwise depois, a gente resolve na hora, pontual —
igual esse aqui.
