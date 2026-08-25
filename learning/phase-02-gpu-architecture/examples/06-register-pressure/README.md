# Exemplo 06 — Register Pressure e Occupancy

## Hipótese antes de rodar

O código instancia kernels com 8, 16, 32 e 64 acumuladores por thread. Preveja:

1. qual variante usará mais registers segundo `ptxas`;
2. quando active blocks/SM deve cair;
3. se o menor tempo precisa coincidir com occupancy máxima.

## Compilar e rodar

```bash
nvcc -O3 -arch=sm_86 -lineinfo -Xptxas=-v -o register_pressure register_pressure.cu
./register_pressure
```

Guarde o output de compilação: ele é parte do experimento. O executável usa a occupancy
API para reportar active blocks/SM e mede 21 repetições com CUDA Events.

| Acumuladores | Registers/thread (`ptxas`) | Spill loads/stores | Blocks/SM | Occupancy estimada | Mediana ms |
| --- | --- | --- | --- | --- | --- |
| 8 | | | | | |
| 16 | | | | | |
| 32 | | | | | |
| 64 | | | | | |

O trabalho aritmético cresce com o número de acumuladores; por isso este não é um A/B puro
de tempo. O valor didático é correlacionar recursos, residência e spills — não declarar que
uma variante “otimizou” a mesma operação.

