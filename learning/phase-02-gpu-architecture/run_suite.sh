#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MODE="${1:---build}"
NVCC_FLAGS=(-O3 -arch=sm_86 -lineinfo)

NVCC_BIN="$(command -v nvcc || true)"
if [[ -z "$NVCC_BIN" && -x /usr/local/cuda-12.9/bin/nvcc ]]; then
  NVCC_BIN=/usr/local/cuda-12.9/bin/nvcc
fi
if [[ -z "$NVCC_BIN" ]]; then
  echo "nvcc não encontrado no PATH nem em /usr/local/cuda-12.9/bin/nvcc" >&2
  exit 127
fi

declare -a SOURCES=(
  "examples/01-device-inventory/device_inventory.cu"
  "examples/03-launch-overhead/launch_overhead.cu"
  "examples/04-coalescing/coalescing.cu"
  "examples/05-divergence/divergence.cu"
  "examples/06-register-pressure/register_pressure.cu"
  "examples/07-sync-atomics/sync_atomics.cu"
)

for source in "${SOURCES[@]}"; do
  output="${source%.cu}"
  echo "[build] $source"
  "$NVCC_BIN" "${NVCC_FLAGS[@]}" -Xptxas=-v "$ROOT/$source" -o "$ROOT/$output"
done

if [[ "$MODE" == "--run" ]]; then
  for source in "${SOURCES[@]}"; do
    output="${source%.cu}"
    echo "[run] $output"
    "$ROOT/$output"
  done
elif [[ "$MODE" != "--build" ]]; then
  echo "uso: $0 [--build|--run]" >&2
  exit 2
fi
