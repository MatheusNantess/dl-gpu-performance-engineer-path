# Backward Passes

Gradient derivations and the mechanics of backward kernels. This directory exists because backward kernels are the priority of this lab and the thing most GPU kernel material skips.

## Why backward gets its own section

Most public CUDA and Triton material stops at the forward pass. That is the easy half, and it is not the half that dominates training:

- Backward typically moves **roughly twice the memory** of forward, so it usually costs more time in a training step.
- Backward reductions run along **axes the forward never touches** — weight gradients reduce across the batch dimension. There is no forward analogue to optimize by copying.
- Backward forces a real design decision the forward does not have: **save the intermediate, or recompute it?** That trade-off is memory footprint against redundant FLOPs, and it has no universally right answer.
- Backward can be **wrong in ways that pass a smoke test** — a subtly incorrect gradient still trains, just worse, and only shows up after many steps.

Being able to write correct, fast backward kernels is the specific skill that separates someone who has done CUDA tutorials from someone who can do this work professionally.

## What goes here

Per-operation derivation notes, written **before** the corresponding kernel is implemented. Each derivation should cover:

1. **Forward definition** — the operation, stated precisely.
2. **Derivation** — gradients w.r.t. every input, worked out by hand. Show the steps; the value of this file is the derivation, not the final formula.
3. **Vector-Jacobian product form** — the Jacobian is never materialized. Write the gradient as the product actually computed.
4. **What forward must save** — and what could be recomputed instead, with the trade-off stated in bytes vs FLOPs.
5. **Reduction structure** — which gradients require reductions, along which axes, and how they map onto the GPU's execution model.
6. **Numerical stability** — where error compounds, which terms are dangerous in low precision, what must stay in FP32.
7. **Validation plan** — how correctness will be checked, and at what tolerance.

## Cross-cutting topics

- Reverse-mode automatic differentiation: what a backward kernel computes and where it sits in the graph
- Why gradients are vector-Jacobian products, never explicit Jacobians
- The chain rule as a composition of kernels, and where fusion across it is possible
- Save-vs-recompute as a general strategy (activation checkpointing is the same idea at model scale)
- How PyTorch's autograd calls into a custom backward: `autograd.Function`, `save_for_backward`, `torch.library`
- How AOTAutograd produces the backward graph, and what that implies for custom kernels under `torch.compile`
- Validation methodology: `gradcheck`, float64 references, per-dtype tolerances, and what a "passing" gradient test actually proves

## Validation standard

A backward kernel is not correct because the loss goes down. Before any timing:

- `torch.autograd.gradcheck` in float64 where the operation allows it
- Comparison against PyTorch's own backward at the working dtype, with stated tolerances
- Edge cases: zeros, large magnitudes, values that overflow a naive formulation, non-contiguous inputs, shapes that do not divide evenly by the block size

## Operations to cover

In the order of `kernels/`, easiest backward first:

| Operation | Backward difficulty | Why |
| --- | --- | --- |
| Activations | Trivial | Pure elementwise; the whole lifecycle without the derivation being the obstacle |
| Softmax | Moderate | The Jacobian is dense — the VJP trick is the entire lesson |
| Cross entropy | Easy, subtle | Fusing softmax into the loss collapses the backward to `p - y` |
| LayerNorm | Hard | Three reductions; weight/bias gradients across the batch |
| RMSNorm | Moderate | LayerNorm's structure, less machinery — the best first serious backward |
| Fused activations | Moderate | Fusion changes what must be saved |
| GEMM | Conceptually easy, hard to make fast | Two more GEMMs with different transpose/layout requirements |
| Attention | Hard | Softmax backward composed with two matmuls |
| FlashAttention | Very hard | Backward built on recomputation instead of stored attention weights |
| Fused MLP / Transformer block | Very hard | Fusion decisions spanning forward and backward together |
