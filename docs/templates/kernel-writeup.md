# <Operation> — Write-up

<!-- Template. The portfolio-facing artifact for a kernel that reached stage 10. -->

**Hardware:** <link to docs/hardware record>
**Status:** <stage reached>

## The operation

<!-- Precise mathematical definition. Shapes, dtypes, where it appears in real models. -->

## Backward derivation

<!-- By hand, with the steps shown. For most operations this is the most valuable
     section of the document. State the vector-Jacobian product form actually computed,
     not the Jacobian. -->

## What forward saves

<!-- Saved vs recomputed, with the trade-off in bytes against FLOPs. -->

## Implementation

<!-- Parallelization strategy and why this decomposition. Triton and/or CUDA.
     Block sizes and the constraints that determined them. -->

## Correctness

<!-- How it was validated, at what tolerances, and why those tolerances.
     Edge cases covered. gradcheck results. -->

## Baseline measurements

<!-- Following docs/benchmark-methodology/. Forward and backward separately.
     Shape sweep, dtypes, median and spread. -->

## Bottleneck analysis

<!-- Memory bound or compute bound, and the specific limiter with the profiler
     evidence that identified it. Achieved bandwidth or FLOP/s against the
     device's MEASURED peak. -->

## Optimization attempts

<!-- One entry per attempt, in order, including the ones that failed.

### Attempt N: <name>
- Hypothesis (stated before the change):
- Change:
- Predicted effect:
- Measured effect:
- Verdict: kept / reverted
- Why it worked, or why the hypothesis was wrong:
-->

## Comparison

<!-- vs PyTorch eager, vs torch.compile, vs library implementations where applicable.
     Include the shapes where this implementation loses. -->

## What I would do differently

## Open questions

<!-- Things still not understood. Specific. -->