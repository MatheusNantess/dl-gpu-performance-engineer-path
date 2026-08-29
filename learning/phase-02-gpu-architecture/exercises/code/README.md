# Exercícios de código — Phase 2

15 programas CUDA pequenos, um conceito por programa. Diferente de
`examples/` (medição séria, hipótese registrada no checkpoint) e de
`exercises/README.md` (cálculo no papel), estes são pra **observar o
comportamento na tela** — mais parecidos com um experimento de bancada que
com um benchmark.

Compile qualquer um com:

```bash
export PATH=/usr/local/cuda-12.9/bin:$PATH
nvcc -O0 -arch=sm_86 <arquivo>.cu -o <nome>
./<nome>
```

`-O0` é importante nos que usam `clock64()` (06) — em `-O3` o compilador
pode eliminar loops que não têm efeito observável.

| # | Pasta | Capítulo da teoria | O que observar |
| --- | --- | --- | --- |
| 01 | `01-global-index-map` | 02 | Como blockIdx/threadIdx/blockDim viram 1 índice global |
| 02 | `02-partial-warp` | 02/03 | Block com tamanho não-múltiplo de 32 — lanes mascaradas |
| 03 | `03-warp-formation` | 03 | Threads consecutivas viram warps de 32, automaticamente |
| 04 | `04-simt-reconvergence` | 03/09 | Branch divergente: ordem de execução dos dois caminhos |
| 05 | `05-active-mask` | 03/09 | `__activemask()` mostrando quais lanes estão ativas |
| 06 | `06-memory-hierarchy-latency` | 04 | Ciclos gastos: registrador vs shared vs global |
| 07 | `07-coalescing-addresses` | 05 | Endereços reais tocados por um warp, por stride |
| 08 | `08-register-spill` | 06 | Registradores e spill via `nvcc -Xptxas -v` |
| 09 | `09-occupancy-calculator` | 06 | Occupancy real via API do CUDA runtime |
| 10 | `10-bank-conflict` | 07 | Shared memory com/sem conflito de banco, tempo medido |
| 11 | `11-latency-hiding` | 08 | Poucos warps residentes vs grid cheio |
| 12 | `12-syncthreads-race` | 10 | Race condition real entre warps, sem/com `__syncthreads()` |
| 13 | `13-atomic-contention` | 11 | `atomicAdd` contendido vs sharded |
| 14 | `14-tensor-vs-cuda-core` | 12 | Diferença conceitual CUDA core vs Tensor core |
| 15 | `15-diagnosis-capstone` | 12 | 3 anti-padrões por flag — diagnostique qual pesa mais |

## Notas específicas

- **08**: a demonstração está na *compilação*, não na execução. Compare
  `nvcc -Xptxas -v` de `fewRegs` vs `manyRegs` — na RTX 3060 medi 10 vs 39
  registradores. Para forçar spill de verdade, aperte ainda mais com
  `--maxrregcount=16` (ou menor) e procure `bytes spill stores/loads` > 0.
- **15**: recompile com combinações de `-DUSE_UNCOALESCED -DUSE_DIVERGENT
  -DUSE_HEAVY_REGS` (nenhuma, uma, duas, todas — 8 no total) e compare os
  tempos. Escreva sua hipótese de qual pesa mais antes de rodar.

## Regra de uso

Mesma regra do resto da fase: **previsão antes de rodar**. Antes de
compilar cada um, escreva (mentalmente ou no seu `checkpoint.md`) o que
você espera ver — só assim um resultado surpreendente ensina alguma coisa.
