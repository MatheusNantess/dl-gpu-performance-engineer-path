import torch

def bandwidth_gbs(n_bytes=512*1024*1024, iters=50):
    n = n_bytes // 4
    src = torch.empty(n, dtype=torch.float32, device='cuda')
    dst = torch.empty_like(src)

    for _ in range(10):                      # warmup
        dst.copy_(src)
    torch.cuda.synchronize()

    start = torch.cuda.Event(enable_timing=True)
    end   = torch.cuda.Event(enable_timing=True)
    start.record()
    for _ in range(iters):
        dst.copy_(src)
    end.record()
    torch.cuda.synchronize()

    ms = start.elapsed_time(end) / iters
    return (2 * n_bytes) / (ms * 1e-3) / 1e9   # 2x: le + escreve

print(f"{bandwidth_gbs():.1f} GB/s")