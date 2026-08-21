# 08 — Fused Normalization

**Research track:** milestones 3 and 4 toward the [North Star](../../research/north-star.md). This is the project the paper would come from, if it comes from anywhere.

## Goal

Investigate whether normalization fused with its neighbours — the residual add before or after it, as it actually appears in a Transformer block — can reduce memory traffic and improve forward and backward performance relative to the best existing implementations.

**Relative to the best existing implementations.** Not relative to eager PyTorch. That distinction is what makes this a research project rather than an exercise.

## Phase 1 — Prior art (Gate 2)

**This comes first, before writing a fused kernel.** Skipping it is how a project like this becomes reinvention with extra steps.

- Read as source, and benchmark: Apex fused LayerNorm/RMSNorm, Liger Kernel's normalization kernels, the flash-attention repository's fused layer_norm module, and whatever TorchInductor generates for a residual + normalization pattern.
- Write up what each optimizes for, what each trades away, and how they differ in backward strategy.
- Reproduce their performance with my own implementation, within a documented distance.
- Literature search, not only source reading.

**Exit criterion:** I can explain each implementation's strategy and my own kernels land within a stated distance of the best of them. Being unable to match them means any later "improvement" is a comparison against my own inexperience.

## Phase 2 — Find the gap (Gate 3)

Questions to answer with measurement, from [research-questions.md](../../research/research-questions.md):

- What is the theoretical minimum traffic for these fused patterns, and how close does the state of the art get?
- Does fusing eliminate a full round trip over the activation tensor, in both directions?
- What does fusion do to what the forward must save for the backward?
- Is there a save-versus-recompute regime that existing implementations do not occupy?
- Where does the parameter-gradient reduction become the limiter?
- What does Inductor decline to fuse, and why?

**Exit criterion:** a specific, defensible statement of what is not covered — or an honest conclusion that nothing is, which ends the research track and is a valid outcome.

## Phase 3 — Build and measure

Only if Phase 2 finds something. Fused kernels in Triton and CUDA, forward and backward, through the full lifecycle, benchmarked against the strongest available baseline across shapes, batch sizes, sequence lengths and dtypes.

## Phase 4 — Real workload (Gate 4)

Integrate into a real Transformer block and a real training step. Measure forward latency, backward latency, step latency, peak memory, and tokens/sec.

**If the kernel-level improvement does not survive here, that is the finding** — and it gets reported, not buried. Normalization is a small fraction of a training step, and the size of that fraction is the ceiling on any possible improvement. Measure the ceiling before celebrating anything.

## What this project is not

- Not "beat PyTorch eager". That is project 01 and it proves nothing here.
- Not a single-shape result.
- Not a claim of novelty made before the prior art survey.

## Status

Not started. Depends on projects 03 and 07, and on the kernel optimization and PyTorch internals phases.

See [research/paper-roadmap.md](../../research/paper-roadmap.md) for the gates and the kill criteria.

---

Kernels inside this project follow the ten-stage lifecycle in [KERNEL_WORKFLOW.md](../../KERNEL_WORKFLOW.md): correctness gates timing, backward is required, and every optimization needs a profiling-backed hypothesis stated beforehand.