# Distributed

Multi-GPU training and inference.

```text
nccl/                    # collectives and their cost models
multi-gpu/               # single-node multi-GPU
tensor-parallelism/
pipeline-parallelism/
distributed-training/    # DDP, FSDP/ZeRO
```

## Status: blocked on hardware

The lab runs on a single RTX 3060. There is no second GPU and no NVLink, so nothing here can be measured honestly on current hardware. **This directory is expected to stay empty until cloud instances are in use.**

That is stated rather than papered over with theory notes. A distributed section full of summarized reading and no measurements would misrepresent what has actually been done.

## Why it is on the roadmap anyway

Communication overlap is largely a kernel-level and scheduling problem, and at scale the boundary between kernel engineering and distributed systems dissolves. It is the natural extension of this work rather than a separate field — but it comes after the single-GPU kernel work is solid, and only when it can be measured.