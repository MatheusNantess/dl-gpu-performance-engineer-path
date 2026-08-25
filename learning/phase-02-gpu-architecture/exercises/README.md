# Exercícios — Phase 2: GPU Architecture

## Regra do módulo

```text
PREVISÃO ESCRITA → MECANISMO → MEDIÇÃO → EXPLICAÇÃO
```

Não rode o experimento antes de registrar a previsão no
[`checkpoint.md`](../checkpoint.md). Uma previsão errada explicada depois vale mais que
uma resposta certa copiada.

## A) Mapeamento e contagem — fundamental

1. Um grid tem 100 blocks de 256 threads. Calcule threads lógicas, warps por block e warps
   totais. Quantos blocks podem executar simultaneamente não pode ser respondido só com
   esses dados — liste os recursos que faltam.
2. Para `blockDim.x = 100`, quantos warps são alocados? Quantas lanes do último warp ficam
   inativas durante todo o kernel?
3. Escreva o índice global 1D e use-o para listar os índices das primeiras 40 threads do
   block 2, com `blockDim.x = 128`.
4. A RTX 3060 tem 28 SMs. Um grid de 10 blocks enormes consegue usar os 28 SMs? Explique
   sem falar em número de threads totais.
5. Por que dois blocks podem executar em qualquer ordem? Que tipo de algoritmo estaria
   incorreto se dependesse de “block 0 termina antes de block 1”?

## B) Coalescing no papel — fundamental

Considere um warp completo, `lane = 0..31`, `float` de 4 B e transações de 32 B. Para cada
expressão, liste os endereços relativos, conte segmentos distintos de 32 B e estime bytes
úteis/bytes transferidos:

6. `x[lane]`
7. `x[lane + 1]` com `x` alinhado em 32 B
8. `x[2 * lane]`
9. `x[8 * lane]`
10. `x[32 * lane]`
11. Todas as lanes leem `x[0]`. Isso deve ser tratado como o mesmo caso de stride 32?
12. Um warp lê uma linha de matriz row-major: `a[row][lane]`. Depois lê uma coluna:
    `a[lane][col]`. Qual coalesces melhor e por quê?
13. O último warp de um vetor tem só 5 lanes válidas. O acesso das lanes ativas pode ser
    coalesced mesmo com 27 lanes mascaradas?

## C) Occupancy, recursos e latency hiding — fundamental

Use os limites registrados para a RTX 3060: 1536 threads/SM, 48 warps/SM, 16 blocks/SM,
65536 registradores de 32 bits/SM e 100 KB de shared/SM. Ignore granularidades de alocação
que não foram fornecidas; declare que o resultado é uma estimativa superior.

14. Kernel A: 256 threads/block, 32 registers/thread, 0 shared. Calcule o limite por
    threads, por registers e por blocks. Qual é o primeiro limitador?
15. Kernel B: 256 threads/block, 80 registers/thread, 0 shared. Repita.
16. Kernel C: 256 threads/block, 32 registers/thread, 32 KB shared/block. Repita.
17. Dois kernels têm occupancy de 50% e 100%. Liste três razões pelas quais o de 50% ainda
    pode ser mais rápido.
18. A medição existente indica 305,21 GB/s, 289,7 ns e setores de 32 B. Refaça Little's
    Law. Por que o resultado não pode ser comparado diretamente com “threads residentes”
    como se fossem exatamente a mesma unidade?
19. Explique a diferença entre warp residente, warp ready e warp eligible.

## D) Controle, barreiras e atomics — fundamental

20. `if (threadIdx.x < 16)` diverge em quais warps para blocks de 32, 64 e 128 threads?
21. `if ((threadIdx.x / 32) % 2 == 0)` divide o block em dois caminhos. Existe divergência
    dentro de cada warp? Compare com `threadIdx.x % 2 == 0`.
22. Explique por que isto é incorreto:

```cuda
if (threadIdx.x < 16) {
    __syncthreads();
}
```

23. Depois de `shared[tid] = x[tid]`, por que uma barreira pode ser necessária antes de
    `y[tid] = shared[neighbor]`, mas não antes de ler `shared[tid]` pela própria thread?
24. Mil threads fazem `atomicAdd(&counter, 1)`. O resultado é correto? A ordem é definida?
    O throughput escala linearmente?
25. Proponha uma versão em duas etapas que reduza contenção: agregue localmente e faça
    menos atomics globais. Que custo novo aparece?

## E) Predict before running — aplicado

Antes de executar os exemplos 03–07, escreva:

26. A ordem esperada de bandwidth para strides 1, 2, 4, 8, 16 e 32.
27. A razão aproximada esperada entre branch uniforme por warp e branch par/ímpar.
28. Se register pressure crescer, em qual ponto occupancy deve cair? O tempo precisa piorar
    exatamente no mesmo ponto?
29. Qual é mais caro: uma barreira por iteração ou um `atomicAdd` altamente contendido?
    Declare de que tamanho de grid sua resposta depende.
30. Launch overhead medido com uma sincronização por launch deve representar throughput de
    launches enfileirados? Explique a diferença entre os dois experimentos.

## F) Desafios — opcional/aplicado

31. **Diagnóstico sem profiler:** escolha um resultado inesperado da suíte e escreva três
    causas concorrentes. Para cada causa, proponha uma mudança que a isole.
32. **Amdahl:** suponha que kernels menores que 10 μs ocupem 20% do step e metade desse
    tempo seja launch overhead. Calcule o teto de speedup se você eliminar apenas esse
    overhead. Depois diga que técnica futura poderia atacá-lo (sem implementar).
33. **Roofline + arquitetura:** um kernel tem AI baixa e bandwidth medida muito abaixo de
    305,21 GB/s. Mostre por que “é memory-bound” não explica sozinho a baixa eficiência;
    liste coalescing, concorrência, tamanho do problema e overhead como hipóteses separadas.
34. **Normalização:** para uma linha de RMSNorm, discuta o trade-off entre guardar `x` em
    registradores e em shared memory. Relacione capacidade, register pressure, occupancy,
    cooperação e sincronização — sem implementar o kernel.
35. **Experimento novo:** desenhe um microbenchmark que varie somente shared memory por
    block, mantendo instruções e registradores aproximadamente constantes. Defina hipótese,
    variável independente, métrica e condição que refutaria sua hipótese.

## Registro mínimo de uma resposta experimental

```text
Hipótese (antes):
Unidade de análise (warp/block/SM/GPU):
Variável alterada:
Variáveis controladas:
Comando e ambiente:
Resultado (mediana + dispersão):
Minha hipótese sobreviveu? Por quê?
O que o experimento NÃO prova:
Próxima medição que reduziria a incerteza:
```

