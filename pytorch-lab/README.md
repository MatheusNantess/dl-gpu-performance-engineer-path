# PyTorch Lab

PyTorch internals — the layer between a kernel and a model that actually uses it. A kernel that cannot be integrated properly is a demo, not a contribution.

```text
custom-ops/      # operator registration, torch.library, dispatch
autograd/        # autograd.Function, custom backward, save_for_backward
torch-compile/   # Dynamo, AOTAutograd, graph breaks, guards
inductor/        # reading and dissecting generated Triton
extensions/      # cpp_extension, load_inline, packaging
```

## Why this matters for kernel work

- **Autograd** is how a backward kernel gets called at all. Registering it correctly, and getting `save_for_backward` right, is the difference between a kernel and a usable operation.
- **AOTAutograd** produces the backward graph under `torch.compile`. Understanding it is required to know how a custom backward interacts with compilation.
- **Graph breaks** caused by a custom op can cost more than the kernel saves. A custom op that survives `torch.compile` cleanly is a higher bar than one that merely runs.
- **The dispatcher** decides which implementation runs for which dtype and device.
- **The caching allocator** explains memory behaviour that otherwise looks like a leak.

## Standard

A custom op is done when it is registered properly, has a working and gradient-checked backward, composes with `torch.compile` without causing a graph break, and behaves correctly for non-contiguous inputs and mixed dtypes.