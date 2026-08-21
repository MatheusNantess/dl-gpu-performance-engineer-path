# Exemplo 04 — Desenhando o Roofline

Script que pega os picos medidos (exemplos 01-03) e desenha o gráfico roofline descrito no
arquivo de teoria 09, com seus benchmarks posicionados nele.

## Preencher antes de rodar

Edite as constantes no topo de `roofline.py` com os números que **você mediu**:

- `PEAK_BANDWIDTH_GB_S` — do `02-bandwidth-benchmark`
- `PEAK_GFLOPS` — do `03-arithmetic-intensity-calc/peak_compute_benchmark.cpp` (**não** do
  `vector_add`/matmul — esses são memory-bound e nunca revelam o pico real de cálculo, ver
  o README do exemplo 03)
- `MEASURED_POINTS` — lista de `(nome, AI, GFLOP/s alcançado)`, usando os números do
  `03-arithmetic-intensity-calc` (lembrando: essa AI é **algorítmica**, não medição de
  tráfego real de hardware — ver arquivo de teoria 07, seção 6)

**Nunca preencha com número de datasheet** — isso invalida o gráfico inteiro (ver arquivo
09, seção 4).

## Como rodar

Com matplotlib disponível:
```bash
pip install matplotlib numpy   # se ainda não tiver
python3 roofline.py
```

Sem matplotlib, o script cai automaticamente num modo texto (ASCII), que ainda mostra os
tetos e onde seus pontos caem — menos bonito, mas funcional sem instalar nada.

## O que fazer com o resultado

Esse é literalmente o **deliverable** deste módulo. Salve a imagem gerada (ou o output de
texto) e registre no `checkpoint.md`.
