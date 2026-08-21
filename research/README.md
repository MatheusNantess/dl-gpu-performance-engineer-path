# Research

The scientific track of this lab. Everything else here answers *can I build this?* — this directory asks *is this new, and why does it work?*

Those are different questions with different standards of evidence, and keeping them in separate places is what stops the lab from drifting into treating a fast kernel as a finding.

## Contents

| File | What it holds |
| --- | --- |
| [north-star.md](north-star.md) | The long-term research goal, what it would investigate, and what would make it a contribution rather than a blog post |
| [research-questions.md](research-questions.md) | The current question and its sub-questions — open, unanswered, and expected to change |
| [paper-roadmap.md](paper-roadmap.md) | The gates between here and any attempt at a paper, including the criteria for concluding there is no paper |

## Where research sits relative to everything else

The repository has seven distinct activities. Confusing them is the main way a lab like this goes wrong.

| Activity | Directory | Question it answers | Output |
| --- | --- | --- | --- |
| **Learning** | `notes/`, `fundamentals/` | How does this work? | Notes, exercises, derivations |
| **Kernels** | `kernels/` | Can I implement this correctly and make it fast? | Validated, optimized forward and backward kernels |
| **Experiments** | `experiments/` | What does *this one technique* cost or gain, in isolation? | A measured answer to a narrow question |
| **Benchmarks** | `benchmarks/` | How fast is it, fairly measured? | Numbers with methodology and hardware attached |
| **Profiling** | `profiling/` | *Why* is it that fast? | Bottleneck attribution backed by metrics |
| **Projects** | `projects/` | Can I build something substantial end to end and explain it? | Portfolio-facing write-ups |
| **Research** | `research/` | Is there something here that is not already known? | Refined questions, falsifiable hypotheses, eventually a paper |

The dependency runs left to right. Research is last for a reason: a research question asked before the measurement skills exist produces an opinion, not a finding.

## Standards

- **Nothing here is a conclusion until it is measured.** The North Star is a hypothesis about where a contribution might be, not a claim that one exists.
- **Prior art is surveyed before novelty is claimed.** Fused normalization kernels already exist in several production libraries. Any contribution has to be located relative to them, which means reading and benchmarking them first.
- **The question is expected to change.** A research question that survives contact with the measurements unchanged usually means the measurements were not looked at.
- **A negative result is a result.** "This fusion does not help on Ampere, and here is precisely why" is a legitimate and useful outcome. It is written up, not buried.

## Status

Open. No experiments run, no questions answered, no hypothesis tested. Everything in this directory is a plan.
