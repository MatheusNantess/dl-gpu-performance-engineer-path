#!/usr/bin/env python3
"""
Desenha o modelo Roofline (arquivo de teoria 09) a partir de picos MEDIDOS
(nunca de datasheet) e posiciona benchmarks reais nele.

Preencha as constantes abaixo com seus proprios numeros medidos antes de
rodar. Cai automaticamente em modo ASCII se matplotlib nao estiver
instalado -- funciona sem instalar nada, so menos bonito.
"""

import math

# ============================================================
# PREENCHA COM SEUS PROPRIOS NUMEROS MEDIDOS (nao de datasheet)
# ============================================================

PEAK_BANDWIDTH_GB_S = 24.64   # do 02-bandwidth-benchmark
PEAK_GFLOPS = 13.94           # do 03-arithmetic-intensity-calc/peak_compute_benchmark.cpp
                               # (troque pelo numero medido na SUA maquina)

# (nome, arithmetic intensity medida, GFLOP/s alcancado medido)
MEASURED_POINTS = [
    ("vector_add", 0.0833, 1.96),
    ("matmul ingenuo", 0.25, 2.26),
]

# ============================================================


def ridge_point(peak_gflops, peak_bandwidth_gb_s):
    return peak_gflops / peak_bandwidth_gb_s


def roofline_ceiling(ai, peak_gflops, peak_bandwidth_gb_s):
    """Teto teorico de performance (GFLOP/s) para uma dada AI."""
    memory_bound_ceiling = ai * peak_bandwidth_gb_s
    return min(memory_bound_ceiling, peak_gflops)


def try_matplotlib_plot():
    import matplotlib.pyplot as plt
    import numpy as np

    ai_range = np.logspace(-3, 3, 500)
    ceilings = [roofline_ceiling(ai, PEAK_GFLOPS, PEAK_BANDWIDTH_GB_S) for ai in ai_range]

    fig, ax = plt.subplots(figsize=(9, 6))
    ax.plot(ai_range, ceilings, color="black", linewidth=2, label="Roofline")

    for name, ai, achieved in MEASURED_POINTS:
        ax.scatter([ai], [achieved], zorder=5, s=80)
        ax.annotate(name, (ai, achieved), textcoords="offset points", xytext=(8, 5))

    rp = ridge_point(PEAK_GFLOPS, PEAK_BANDWIDTH_GB_S)
    ax.axvline(rp, color="gray", linestyle="--", linewidth=1)
    ax.text(rp, PEAK_GFLOPS * 1.05, f"ridge point\n{rp:.3f} FLOPs/byte",
            ha="center", fontsize=8, color="gray")

    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel("Arithmetic Intensity (FLOPs/byte)")
    ax.set_ylabel("Performance (GFLOP/s)")
    ax.set_title("Roofline — CPU (picos medidos, não de datasheet)")
    ax.legend()
    ax.grid(True, which="both", alpha=0.3)

    out_path = "roofline.png"
    fig.savefig(out_path, dpi=150, bbox_inches="tight")
    print(f"Salvo em {out_path}")


def ascii_plot():
    """Fallback sem matplotlib: mostra os numeros e uma barra proporcional simples."""
    rp = ridge_point(PEAK_GFLOPS, PEAK_BANDWIDTH_GB_S)
    print("=" * 60)
    print("ROOFLINE (modo texto -- instale matplotlib para o grafico real)")
    print("=" * 60)
    print(f"Peak bandwidth medido: {PEAK_BANDWIDTH_GB_S:.2f} GB/s")
    print(f"Peak GFLOP/s medido:   {PEAK_GFLOPS:.2f}")
    print(f"Ridge point:           {rp:.4f} FLOPs/byte")
    print()
    print(f"{'Operação':<20}{'AI':>10}{'Regime':>16}{'Teto (GFLOP/s)':>18}{'Alcançado':>14}{'% do teto':>12}")
    print("-" * 90)
    for name, ai, achieved in MEASURED_POINTS:
        ceiling = roofline_ceiling(ai, PEAK_GFLOPS, PEAK_BANDWIDTH_GB_S)
        regime = "memory-bound" if ai < rp else "compute-bound"
        pct = (achieved / ceiling) * 100 if ceiling > 0 else 0.0
        print(f"{name:<20}{ai:>10.4f}{regime:>16}{ceiling:>18.2f}{achieved:>14.2f}{pct:>11.1f}%")
    print()
    print("Instale matplotlib+numpy (`pip install matplotlib numpy`) para gerar")
    print("o gráfico visual — este modo texto é só um fallback funcional.")


def main():
    try:
        try_matplotlib_plot()
    except ImportError:
        ascii_plot()


if __name__ == "__main__":
    main()
