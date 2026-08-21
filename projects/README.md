# Projects

Larger end-to-end work pulling several roadmap phases together. Each project follows [KERNEL_WORKFLOW.md](../KERNEL_WORKFLOW.md) for the kernels inside it.

RT = on the critical path to the [research North Star](../research/north-star.md).

| Project | RT | Focus | Depends on |
| --- | --- | --- | --- |
| [01-beating-pytorch-eager](01-beating-pytorch-eager/) | | The easy baseline; establishes methodology | Phases 3–6 |
| [02-beating-torch-compile](02-beating-torch-compile/) | | The real bar; requires reading Inductor output | Phases 5, 9, 10 |
| [03-rmsnorm-forward-backward](03-rmsnorm-forward-backward/) | **RT** | First complete forward + backward, properly integrated | Phases 7, 8 |
| [04-attention-from-scratch](04-attention-from-scratch/) | | Naive attention, and measuring exactly why it fails | Phases 6, 7 |
| [05-flash-attention-from-scratch](05-flash-attention-from-scratch/) | | Tiling, online softmax, recomputation in the backward | Phases 7, 9 |
| [06-transformer-block-optimization](06-transformer-block-optimization/) | **RT** | Block-level optimization on a real workload | Phases 9–12 |
| [07-layernorm-forward-backward](07-layernorm-forward-backward/) | **RT** | LayerNorm, and the LayerNorm vs RMSNorm comparison | Phases 7, 8 |
| [08-fused-normalization](08-fused-normalization/) | **RT** | Prior art, the gap, and fused normalization kernels | Phases 10, 11 |

## Research critical path

```text
03 (RMSNorm)  →  07 (LayerNorm + comparison)  →  08 (fused normalization)  →  06 (block integration)
```

Projects 03 and 07 clear [Gates 0 and 1](../research/paper-roadmap.md). Project 08 is where the research track stops being preparation — and where it may honestly end, if the prior art survey finds no gap.

## Projects vs kernels vs research

`kernels/` is where one operation goes through the lifecycle. `projects/` is where several are combined into something with a stated goal and a narrative written for a reader. `research/` is where the question of whether any of it is *new* lives.

A project is what someone else gets pointed at. That means it needs: what the goal was, what was measured, what the profiler said, what was tried, what worked, what did not, and what remains open.

## Honesty

The two "beating X" projects carry an obvious incentive to cherry-pick shapes. They do not get to. Both report the shapes where the custom implementation loses, and both are judged on the quality of the explanation rather than the size of the number.

Project 08 carries a stronger version of the same risk: the incentive to find a contribution because one is wanted. Its prior art phase exists specifically to make that harder.