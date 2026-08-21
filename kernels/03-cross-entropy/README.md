# 03 — Cross Entropy

**Backward difficulty:** Easy to derive, subtle to implement

## Operation

Cross entropy loss over logits, fused with the softmax rather than composed after it.

## Why this kernel

The clearest demonstration in the whole lab of why fusion matters. Computing softmax and then cross entropy separately materializes a full probability tensor; fusing them avoids it entirely. In language models the vocabulary dimension makes that tensor enormous.

## Backward

Fusing softmax into the loss collapses the backward to a strikingly simple expression. Deriving why the complicated softmax Jacobian disappears is the lesson. The implementation subtlety is doing it without materializing probabilities, at vocabulary sizes where that tensor does not fit comfortably.

## Milestones

| Milestone | Status |
|---|---|
| Forward | Not started |
| Backward | Not started |
| Forward optimization | Not started |
| Backward optimization | Not started |

## Stage checklist

Stages defined in [KERNEL_WORKFLOW.md](../../KERNEL_WORKFLOW.md). No timing before stage 4 passes.

| # | Stage | Status |
|---|-------|--------|
| 1 | Correctness baseline          | ☐ |
| 2 | Forward implementation        | ☐ |
| 3 | Backward implementation       | ☐ |
| 4 | Correctness validation        | ☐ |
| 5 | Benchmarking                  | ☐ |
| 6 | Profiling                     | ☐ |
| 7 | Bottleneck analysis           | ☐ |
| 8 | Optimization                  | ☐ |
| 9 | vs PyTorch eager              | ☐ |
| 10 | vs torch.compile / libraries | ☐ |

## Expected bottleneck

Memory bound, dominated by traffic over the logits tensor. Peak memory footprint matters as much as time here.

Stated as a hypothesis to be checked against a profile, not as a conclusion.

## Results

No measurements yet.

## Status

Not started. 

Structure follows [`_template/`](../_template/); subdirectories are created when there is something to put in them.