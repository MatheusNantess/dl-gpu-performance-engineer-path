# Exemplo 08 — Pico Real de FP32 (FMA)

## Hipótese antes de rodar

O kernel mantém `NUM_ACC` acumuladores independentes por thread, cada um fazendo
`ITERS` FMAs em sequência, sem tráfego de memória durante o trabalho. Preveja:

1. se o resultado medido vai ficar mais perto do teto de datasheet (~12.7 TFLOPs
   pra RTX 3060) ou bem abaixo dele;
2. o que acontece com o GFLOP/s medido se `NUM_ACC` cair pra 1 (cadeia única,
   dependente) — throughput deve cair bastante, porque aí a unidade de FMA
   fica esperando o resultado anterior em vez de ter trabalho independente
   pra emitir a cada ciclo;
3. o que acontece se `NUM_BLOCKS` for pequeno demais pra cobrir os 28 SMs.

## Compilar e rodar

```bash
nvcc -O3 -arch=sm_86 -Xptxas=-v -o fp32_peak fp32_peak.cu
./fp32_peak
```

Guarde o output de `-Xptxas -v` (registros/thread) — se aparecer spill, `NUM_ACC`
está grande demais pro budget de registradores por thread nessa occupancy.

## Preencha depois de rodar

| NUM_ACC | Registers/thread (`ptxas`) | GFLOP/s medido | % do datasheet (~12700 GFLOP/s) |
| --- | --- | --- | --- |
| 1 (cadeia única, pra comparação) | | | |
| 8 (default do arquivo) | | | |

## Depois de medir

O número de `NUM_ACC` que deu o maior GFLOP/s é o seu **pico de FP32 medido**.
Leve esse valor pra dois lugares:

1. [`docs/hardware/rtx-3060.md`](../../../../docs/hardware/rtx-3060.md), na linha
   `FP32 throughput (measured): not measured` — troque por `**X.XX TFLOPs**` com
   método e data, igual as outras linhas dessa seção.
2. `measured_fp32_tflops` em [`scripts/benchmark/roofline.py`](../../../../scripts/benchmark/roofline.py)
   — sai de `None` pro valor medido. A partir daí todo `--flops` calculado por
   esse script já compara contra o teto real, não datasheet.
