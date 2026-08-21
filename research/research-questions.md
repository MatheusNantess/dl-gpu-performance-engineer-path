# Research Questions

Open questions. **None of these are answered.** This file is expected to change substantially as the lab produces measurements — a research question that survives contact with real data unchanged usually means the data was not examined.

Status of every question below: **open, no evidence collected.**

---

## Primary question (current formulation)

> **Can specialized fused normalization kernels reduce memory traffic and improve forward and backward performance during Transformer training on Ampere GPUs, compared with general-purpose framework implementations?**

This is a starting formulation. It is almost certainly too broad, and it will be narrowed once the first real measurements exist. Two known weaknesses in it already:

- "General-purpose framework implementations" is the weak baseline. The real bar is the existing *specialized* implementations — Apex, Liger, the flash-attention layer_norm module, and whatever Inductor generates. Beating eager PyTorch proves nothing.
- "Improve performance" is not falsifiable as stated. It needs a specific metric, a specific shape regime, and a threshold.

Fixing both is part of Phase 15.

---

## Sub-questions

Decomposed into things that can actually be measured. Each one is answerable independently, and several are answerable early — that is deliberate, because early answers are what will reshape the primary question.

### On memory traffic

1. What is the **theoretical minimum** global memory traffic for RMSNorm forward and backward at a given shape, and how close do existing implementations get to it?
2. How much traffic does a separate residual add actually cost, as a fraction of the normalization itself?
3. Does fusing residual + normalization eliminate a full round trip over the activation tensor, in both directions, and does the measurement confirm the arithmetic?

### On the backward

4. What is the actual forward/backward cost ratio for normalization kernels in a real training step, measured rather than assumed?
5. Is the parameter-gradient reduction across the batch the backward bottleneck, and at which shapes?
6. Atomics versus two-pass reduction for parameter gradients — where is the crossover on this device, as a function of batch size and hidden dimension?
7. Does fusing the forward make the backward harder, and does the net across both still win?

### On save versus recompute

8. For LayerNorm and RMSNorm, is it faster to save the normalization statistics or to recompute them in the backward, and at which shapes does the answer flip?
9. What does that trade-off do to peak memory across a full Transformer block, not just one kernel?
10. Is there a regime where recomputing *more* than current implementations do is a net win because it removes traffic?

### On the hardware

11. What is the RTX 3060's actual achievable bandwidth, measured, and what fraction do normalization kernels reach?
12. Which constraint binds first for fused normalization on GA106 — registers, shared memory, or occupancy?
13. Does an optimization that wins on consumer Ampere still win on A100, where bandwidth is much higher relative to compute? **This is the question that separates a general principle from an artifact.**

### On existing implementations

14. What exactly does TorchInductor fuse for a residual + normalization pattern, and what does it decline to fuse?
15. How do Apex, Liger, and the flash-attention layer_norm implementation differ in strategy, and what does each optimize for?
16. Where does each of them leave something on the table, if anywhere — and is that gap a real opportunity or a deliberate trade-off they made for a reason?

### On real impact

17. What fraction of a real Transformer training step is normalization, at realistic shapes?
18. Does a measured kernel-level improvement survive integration into a full training step, or get absorbed by other bottlenecks?
19. At what point does the improvement become irrelevant to end-to-end throughput? **A kernel win that does not move tokens/sec is not a training result.**

---

## What would falsify the hypothesis

Stated up front, so the outcome is not rationalized after the fact:

- Existing implementations already reach close to the theoretical minimum traffic, leaving no meaningful headroom.
- Fusion wins at the microbenchmark level but disappears in a full training step.
- The win exists only on consumer Ampere and vanishes on datacenter hardware, making it an artifact of one memory system rather than a principle.
- The backward cost of fusion exceeds the forward saving.
- `torch.compile` already produces equivalent fused kernels automatically, making a hand-written version redundant.

**Any of these outcomes is a legitimate result** and gets written up with the evidence. None of them is a reason to quietly redefine the question.

---

## How this file evolves

- Questions get **answered** — the answer and its evidence recorded, the question moved to a resolved section.
- Questions get **refined** — narrowed to something measurable, with the old formulation kept so the reasoning is visible.
- Questions get **discarded** — with the reason stated.
- New questions get **added**, most often from a measurement that did not match the expectation. Those are the valuable ones.

Every change to this file should be traceable to something that was measured. A question refined on the basis of reading rather than measuring is a hypothesis about a hypothesis.
