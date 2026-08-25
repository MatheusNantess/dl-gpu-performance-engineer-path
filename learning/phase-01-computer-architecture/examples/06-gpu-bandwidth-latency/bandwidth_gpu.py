# STREAM Triad na GPU: c[i] = a[i] + scalar * b[i]
#
# Mesma logica do bandwidth_benchmark.cpp (exemplo 02), portada pra CUDA via
# PyTorch. Arrays deliberadamente maiores que a L2 da GPU (2.25 MB na RTX
# 3060, ver docs/hardware/rtx-3060.md) para garantir que os acessos batem em
# VRAM de verdade, nao em cache. AI baixa (2 FLOPs, 12 bytes por elemento) --
# o gargalo e garantidamente memoria, nao calculo.
#
# Por que torch.cuda.Event e nao time.perf_counter():
# Kernels CUDA sao lancados de forma assincrona -- o host continua executando
# antes do kernel terminar na GPU. time.perf_counter() em volta do lancamento
# mediria principalmente overhead de lancamento, nao o trabalho real. Eventos
# CUDA sao registrados na propria stream da GPU e medem o tempo decorrido no
# dispositivo, entre dois pontos da fila de execucao.

import torch

assert torch.cuda.is_available(), "CUDA nao disponivel -- confira a instalacao do PyTorch+CUDA no WSL"

device = torch.device("cuda")
print(f"GPU: {torch.cuda.get_device_name(device)}")

# 128 milhoes de floats por array = 512 MB por array, bem maior que os
# 2.25 MB de L2 da RTX 3060. Ajuste pra baixo se sua VRAM for menor.
n = 128 * 1024 * 1024
repeats = 50

a = torch.full((n,), 1.0, dtype=torch.float32, device=device)
b = torch.full((n,), 2.0, dtype=torch.float32, device=device)
c = torch.empty(n, dtype=torch.float32, device=device)
scalar = 3.0

# Aquecimento: primeira execucao paga custo de compilacao/cache de kernel
# (CUDA context, cuBLAS/ATen dispatch) que nao e o que queremos medir.
for _ in range(5):
    torch.add(a, b, alpha=scalar, out=c)
torch.cuda.synchronize()

start_event = torch.cuda.Event(enable_timing=True)
end_event = torch.cuda.Event(enable_timing=True)

start_event.record()
for _ in range(repeats):
    torch.add(a, b, alpha=scalar, out=c)
end_event.record()
torch.cuda.synchronize()

elapsed_ms = start_event.elapsed_time(end_event)
seconds = elapsed_ms / 1000.0

# Bytes movidos por repeticao: le a[i] (4 bytes) + le b[i] (4 bytes) +
# escreve c[i] (4 bytes) = 12 bytes por elemento.
bytes_per_repeat = n * 12
total_bytes = bytes_per_repeat * repeats
gb_per_sec = (total_bytes / seconds) / 1e9

print(f"Elementos por array: {n:,} ({n * 4 / (1024**2):.0f} MB por array)")
print(f"Repeticoes: {repeats}")
print(f"Tempo total (GPU): {seconds:.4f} s")
print(f"Bandwidth sustentavel medida: {gb_per_sec:.2f} GB/s")

# Impede eliminacao do calculo por nunca usar `c` (menos critico em Python/
# PyTorch que em C++, mas mantido pela mesma disciplina).
_ = c[n // 2].item()
