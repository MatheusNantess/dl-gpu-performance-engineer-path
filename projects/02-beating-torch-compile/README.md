# 02 — Beating torch.compile

## Goal

Find cases where a hand-written kernel beats `torch.compile` / TorchInductor, and understand precisely what the compiler could not do.

This is a much harder target than eager mode. Inductor already fuses elementwise chains and generates competent Triton. Where it loses, it usually loses for structural reasons: a fusion it will not attempt, a reduction pattern it handles generically, a layout it will not change, or a graph break splitting work that should have been one kernel.

## Planned scope

- Establish `torch.compile` baselines (default and `max-autotune`) for a set of operations.
- Read the Triton that Inductor actually generates for each — this is the core of the project, not a side activity.
- Form a hypothesis about the specific limitation before writing any replacement kernel.
- Implement the hand-written kernel and test the hypothesis.
- Profile both to confirm the mechanism rather than assuming it.

## What "done" means

- Documented Inductor output for each case, with annotations.
- Correctness validated against the compiled reference.
- Benchmarks across shapes, with the compile mode and PyTorch version recorded.
- A write-up that explains the *compiler-level* reason for each result — and honestly documents the cases where Inductor wins, which are expected to be many.

## Notes

The valuable output here is understanding where compiler abstraction leaks, not a scoreboard. A loss that is well explained is worth more than a win that is not.

## Status

Not started. Depends on the Triton and ML compiler phases of the roadmap.

---

Kernels inside this project follow the ten-stage lifecycle in [KERNEL_WORKFLOW.md](../../KERNEL_WORKFLOW.md): correctness gates timing, backward is required, and every optimization needs a profiling-backed hypothesis stated beforehand.
