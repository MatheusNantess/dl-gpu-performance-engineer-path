# Exemplo 01 — Device Inventory

## Hipótese

Antes de rodar, copie do registro da RTX 3060 os valores que espera para SMs, warp size,
threads/SM, registers/SM, shared/SM e L2. O objetivo é detectar diferença entre memória,
especificação e o que o runtime atual reporta.

## Compilar e rodar

```bash
nvcc -O3 -arch=sm_86 -o device_inventory device_inventory.cu
./device_inventory
```

## O que mede — e o que não mede

`cudaGetDeviceProperties` consulta propriedades/limites expostos pelo runtime. Isso é um
**inventário**, não benchmark: não mede throughput, latência ou utilização.

| Propriedade | Previsto | Reportado | Explicação se divergir |
| --- | --- | --- | --- |
| SMs | | | |
| Warp size | | | |
| Threads/SM | | | |
| Registers/SM | | | |
| Shared/SM | | | |
| L2 | | | |

