"""
roofline.py -- % of hardware peak calculator.

Answers one question: "this kernel moved X bytes / did Y FLOPs in Z seconds --
what % of this GPU's peak is that?"

The hardware numbers below are FIXED reference points, not recomputed per run.
They are copied from docs/hardware/rtx-3060.md, which is the canonical source
in this repo. When a new peak gets measured (FP32 throughput, Tensor Core
throughput, launch overhead -- all still "not measured" as of the last update
to that doc), update BOTH this file and rtx-3060.md so they never drift apart.

Why fixed constants instead of re-deriving them each time: every benchmark
you run from now on should be judged against the *same* denominator. If the
denominator moves around, "39.7% of peak" from one run and "41% of peak" from
another aren't comparable anymore.

Usage
-----
Memory-bound kernel (e.g. vector_add: N=1,000,000, GPU=0.099128 ms measured):

    python scripts/benchmark/roofline.py --bytes 12000000 --ms 0.099128

Compute-bound kernel (e.g. matmul M=256,N=512,O=256, GPU=196.8 us measured):

    python scripts/benchmark/roofline.py --flops 67108864 --us 196.8

Both (to also see arithmetic intensity and which roofline regime applies):

    python scripts/benchmark/roofline.py --flops 67108864 --bytes 1245184 --us 196.8

Units: pass exactly one time flag (--seconds / --ms / --us). Bytes and FLOPs
are raw counts (not GB / GFLOP) -- the script does the conversion.
"""

import argparse
import sys
from dataclasses import dataclass
from typing import Optional


# ---------------------------------------------------------------------------
# Fixed hardware reference. Source: docs/hardware/rtx-3060.md
# Update this block (and that doc) whenever a new peak gets measured.
# ---------------------------------------------------------------------------

@dataclass(frozen=True)
class HardwarePeaks:
    gpu_name: str = "NVIDIA GeForce RTX 3060 (12 GB)"

    # Achieved memory bandwidth. STREAM Triad, 512 MB arrays, torch.cuda.Event
    # timing, 50 repeats, measured 2026-08-23. This is the number to divide
    # by for any memory-bound %-of-peak claim.
    measured_bandwidth_gb_s: float = 305.21

    # Datasheet ceiling (2 x 7501 MHz x 192 bits / 8). rtx-3060.md explicitly
    # says never use this as the denominator in a %-of-peak claim -- kept
    # here only so the achievable/theoretical gap is visible if you want it.
    datasheet_bandwidth_gb_s: float = 360.0

    # Global memory latency at the plateau (262144 KB buffer, single-thread
    # pointer chasing, measured 2026-08-23). Not used in the % calculations
    # below -- kept for reference / Little's Law style checks.
    measured_latency_ns: float = 289.7

    # FP32 throughput, FP16/BF16 Tensor Core throughput and kernel launch
    # overhead are all still "not measured" in rtx-3060.md as of the last
    # update. Leave these as None until you actually run that microbenchmark
    # (Phase 2 deliverable) -- do NOT fill them with a datasheet number.
    # A None here makes compute_bound_percent() refuse to compute a %
    # instead of silently comparing against a number nobody measured.
    measured_fp32_tflops: Optional[float] = None
    measured_fp16_tensorcore_tflops: Optional[float] = None
    measured_launch_overhead_us: Optional[float] = None


PEAKS = HardwarePeaks()


# ---------------------------------------------------------------------------
# Unit helpers
# ---------------------------------------------------------------------------

def to_seconds(seconds: Optional[float], ms: Optional[float], us: Optional[float]) -> float:
    given = [v for v in (seconds, ms, us) if v is not None]
    if len(given) != 1:
        raise ValueError("pass exactly one of --seconds / --ms / --us")
    if seconds is not None:
        return seconds
    if ms is not None:
        return ms / 1e3
    return us / 1e6


# ---------------------------------------------------------------------------
# Roofline math
# ---------------------------------------------------------------------------

def memory_bound_percent(bytes_moved: float, seconds: float) -> dict:
    achieved_gb_s = (bytes_moved / 1e9) / seconds
    pct = 100.0 * achieved_gb_s / PEAKS.measured_bandwidth_gb_s
    return {
        "achieved_gb_s": achieved_gb_s,
        "peak_gb_s": PEAKS.measured_bandwidth_gb_s,
        "pct_of_peak": pct,
    }


def compute_bound_percent(flops: float, seconds: float) -> dict:
    achieved_gflop_s = (flops / 1e9) / seconds
    if PEAKS.measured_fp32_tflops is None:
        return {
            "achieved_gflop_s": achieved_gflop_s,
            "peak_gflop_s": None,
            "pct_of_peak": None,
        }
    peak_gflop_s = PEAKS.measured_fp32_tflops * 1e3
    pct = 100.0 * achieved_gflop_s / peak_gflop_s
    return {
        "achieved_gflop_s": achieved_gflop_s,
        "peak_gflop_s": peak_gflop_s,
        "pct_of_peak": pct,
    }


def arithmetic_intensity(flops: float, bytes_moved: float) -> float:
    return flops / bytes_moved


def ridge_point() -> Optional[float]:
    """FLOP/byte where compute and memory bounds cross. None if FP32 peak
    hasn't been measured yet -- can't place the ridge without it."""
    if PEAKS.measured_fp32_tflops is None:
        return None
    peak_flop_s = PEAKS.measured_fp32_tflops * 1e12
    peak_byte_s = PEAKS.measured_bandwidth_gb_s * 1e9
    return peak_flop_s / peak_byte_s


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description="Compute %% of measured RTX 3060 peak for a benchmark result."
    )
    parser.add_argument("--bytes", type=float, default=None, help="total bytes moved (global memory traffic)")
    parser.add_argument("--flops", type=float, default=None, help="total floating point ops")
    parser.add_argument("--seconds", type=float, default=None, help="elapsed time, seconds")
    parser.add_argument("--ms", type=float, default=None, help="elapsed time, milliseconds")
    parser.add_argument("--us", type=float, default=None, help="elapsed time, microseconds")
    parser.add_argument("--label", type=str, default=None, help="optional name for this run, printed in the output")
    args = parser.parse_args()

    if args.bytes is None and args.flops is None:
        parser.error("pass --bytes and/or --flops")

    try:
        seconds = to_seconds(args.seconds, args.ms, args.us)
    except ValueError as e:
        parser.error(str(e))

    print(f"=== roofline: {args.label or '(unlabeled run)'} ===")
    print(f"GPU: {PEAKS.gpu_name}  (source: docs/hardware/rtx-3060.md)")
    print(f"elapsed: {seconds * 1e6:.3f} us\n")

    if args.bytes is not None:
        r = memory_bound_percent(args.bytes, seconds)
        print("-- memory-bound view --")
        print(f"achieved bandwidth : {r['achieved_gb_s']:.2f} GB/s")
        print(f"measured peak       : {r['peak_gb_s']:.2f} GB/s")
        print(f"% of peak           : {r['pct_of_peak']:.1f}%\n")

    if args.flops is not None:
        r = compute_bound_percent(args.flops, seconds)
        print("-- compute-bound view --")
        print(f"achieved throughput : {r['achieved_gflop_s']:.2f} GFLOP/s")
        if r["peak_gflop_s"] is None:
            print("measured peak       : NOT MEASURED YET")
            print("% of peak           : cannot compute -- run the Phase 2 FP32-peak")
            print("                       microbenchmark, then fill measured_fp32_tflops")
            print("                       in this file and in docs/hardware/rtx-3060.md.\n")
        else:
            print(f"measured peak       : {r['peak_gflop_s']:.2f} GFLOP/s")
            print(f"% of peak           : {r['pct_of_peak']:.1f}%\n")

    if args.bytes is not None and args.flops is not None:
        ai = arithmetic_intensity(args.flops, args.bytes)
        rp = ridge_point()
        print("-- roofline regime --")
        print(f"arithmetic intensity: {ai:.4f} FLOP/byte")
        if rp is None:
            print("ridge point         : unknown (FP32 peak not measured yet)")
        else:
            regime = "memory-bound" if ai < rp else "compute-bound"
            print(f"ridge point         : {rp:.4f} FLOP/byte")
            print(f"regime              : {regime}")


if __name__ == "__main__":
    main()
