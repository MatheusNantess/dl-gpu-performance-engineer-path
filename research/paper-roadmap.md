# Paper Roadmap

The path from where the lab is now to any attempt at the [North Star](north-star.md). Structured as **gates**: each one must be passed before the next is meaningful, and each has a concrete criterion rather than a feeling of readiness.

**Current position: before Gate 0.**

The engineering roadmap in [../ROADMAP.md](../ROADMAP.md) is what produces the capability. This file is what says whether the capability is sufficient yet.

---

## Gate 0 — Can implement and validate

**Criterion:** RMSNorm forward and backward, in Triton and in CUDA, numerically correct against PyTorch including `gradcheck`.

- [ ] RMSNorm mathematically understood, backward derived by hand
- [ ] PyTorch reference implementation
- [ ] Triton forward + backward
- [ ] CUDA forward + backward
- [ ] Correctness validated, gradients included
- [ ] Same for LayerNorm

**Until this is passed, there is no research — only study.** Roadmap phases 0–8.

---

## Gate 1 — Can measure and explain

**Criterion:** for each of those kernels, a profiler-backed explanation of exactly what limits it, and how close it gets to the device's measured ceiling.

- [ ] Benchmark methodology written and consistently applied
- [ ] Device peaks measured, not quoted — bandwidth, launch overhead
- [ ] Achieved bandwidth as a fraction of measured peak, forward and backward separately
- [ ] Theoretical minimum traffic computed by hand and compared against measured traffic
- [ ] Bottleneck named specifically, with metrics
- [ ] The gap between achieved and theoretical explained

**The distinguishing skill.** Anyone can write a kernel; explaining precisely why it performs as it does is the research prerequisite. Roadmap phases 4, 9, 13.

---

## Gate 2 — Can reproduce the state of the art

**Criterion:** existing fused normalization implementations read as source, benchmarked, and their strategies explained.

- [ ] Apex fused LayerNorm/RMSNorm — read and benchmarked
- [ ] Liger Kernel normalization kernels — read and benchmarked
- [ ] flash-attention's fused layer_norm module — read and benchmarked
- [ ] TorchInductor's generated kernels for residual + normalization — read and benchmarked
- [ ] A written comparison of what each optimizes for and what each trades away
- [ ] My own implementation within a documented distance of the best of them

**The most-skipped gate, and the one that decides whether the project is research or reinvention.** Being unable to match existing implementations means any "improvement" is a comparison against my own inexperience. Roadmap phases 10, 11.

---

## Gate 3 — Can find a real gap

**Criterion:** a specific, defensible statement of what existing implementations do not do, and why that might matter.

- [ ] Prior art survey written, including a literature search, not only source reading
- [ ] A specific gap identified — a shape regime, a backward strategy, a trade-off point, an architectural constraint
- [ ] Evidence that the gap is real rather than a deliberate trade-off made for a reason I have not understood
- [ ] Research question narrowed to something falsifiable with a threshold
- [ ] The falsification criteria from [research-questions.md](research-questions.md) restated for this specific gap

**This is where most attempts should honestly stop.** Reaching Gate 2 and finding no gap is a successful outcome for the engineering goal and a correct decision for the research one.

---

## Gate 4 — Can show it matters on a real workload

**Criterion:** the improvement survives integration into a real Transformer training step.

- [ ] Integrated into a real Transformer block
- [ ] Full training step measured: forward, backward, optimizer
- [ ] Peak memory measured
- [ ] Throughput measured in tokens/sec
- [ ] Normalization's share of step time measured, so the ceiling on any improvement is known
- [ ] The improvement still present end to end, or an explanation of where it was absorbed

**A microbenchmark result is not a training result.** If a 30% normalization improvement moves the training step by 0.5%, that is the finding, and it has to be reported as such. Roadmap phase 12.

---

## Gate 5 — Can show it generalizes

**Criterion:** evidence about whether the result is a principle or an artifact of one memory system.

- [ ] Validated on datacenter Ampere (A100) via cloud
- [ ] Tested on a later architecture (H100) via cloud
- [ ] Behaviour across architectures characterized and explained mechanically
- [ ] If it does not transfer: the reason identified, which is itself a result

Requires cloud hardware and is only worth paying for after Gate 4. Roadmap phase 14.

---

## Gate 6 — Can write it

**Criterion:** the work is written to a standard where a knowledgeable reader could reproduce and disagree with it.

- [ ] Complete methodology, reproducible by someone else
- [ ] All shapes and dtypes reported, including where the approach loses
- [ ] Mechanism explained, not merely demonstrated
- [ ] Related work positioned honestly
- [ ] Limitations stated explicitly
- [ ] Code released

---

## Kill criteria

Deciding in advance when to stop is what keeps this scientific rather than motivated.

**Stop pursuing the paper if:**

- Gate 2 shows existing implementations are already near the theoretical traffic minimum, with no meaningful headroom.
- Gate 3 finds no gap after an honest survey.
- Gate 4 shows the improvement is absorbed at training-step level.
- Gate 5 shows the result is purely an artifact of consumer Ampere and nothing more.
- `torch.compile` closes the gap on its own during the work — a realistic outcome over a multi-year timeline.

**Stopping is not failure.** Every gate passed produces exactly the skills the engineering goal requires. The paper is the optional upside; the capability is the actual deliverable.

What does happen on a stop: the negative result gets written up in `docs/writeups/`. "Here is what I measured, here is why the obvious idea does not work, here is the mechanism" is a genuinely valuable artifact — and a better portfolio piece than most positive results, because it demonstrates judgment rather than luck.

---

## Milestones in order

| # | Milestone | Gate | Where |
| --- | --- | --- | --- |
| 1 | RMSNorm forward + backward, optimized | 0, 1 | [kernels/05-rmsnorm](../kernels/05-rmsnorm/), [projects/03](../projects/03-rmsnorm-forward-backward/) |
| 2 | LayerNorm forward + backward, optimized; compared against RMSNorm | 0, 1 | [kernels/04-layernorm](../kernels/04-layernorm/), [projects/07](../projects/07-layernorm-forward-backward/) |
| 3 | Prior art survey and reproduction | 2 | [projects/08](../projects/08-fused-normalization/) |
| 4 | Fused normalization kernels | 3 | [kernels/06-fused-normalization](../kernels/06-fused-normalization/) |
| 5 | Transformer block integration | 4 | [kernels/12-transformer-block](../kernels/12-transformer-block/), [projects/06](../projects/06-transformer-block-optimization/) |
| 6 | Cross-GPU validation | 5 | Cloud |
| 7 | Research question refinement | 3, 6 | [research-questions.md](research-questions.md) |
| 8 | Paper experimentation | 6 | — |

Milestone 1 is the next thing that matters. Everything before it is preparation for it.
