# 02 — Choosing Between the Three

## The decision

| Situation | Use | Why |
| --- | --- | --- |
| Small value, read only | `int x` | Copying 4 bytes is cheaper than any indirection |
| Large object, read only | `const T&` | No copy, cannot modify, clean syntax |
| Must modify the caller's variable | `T&` or `T*` | Both work; see below |
| Might be absent | `T*` | Only a pointer can be null |
| Operating on an array | `T*` | Arrays decay; there is no other option |
| C-compatible interface | `T*` | C has no references |
| Reassigning the caller's pointer | `T**` | Take the address of what you modify |

## Reference or pointer for mutation?

Both do the job. The usual guidance:

**Reference when** the argument is always present, always valid, and the syntax is worth it.

**Pointer when** it may be null, when the call site should visibly signal mutation, when it is an array, or when the interface must be callable from C.

```cpp
void normalize(float& value);              // always valid
void normalize(float* values, int n);      // an array -- no choice
void try_get(int* out);                    // out may be nullptr
```

## Why CUDA is all pointers

Kernel signatures use pointers exclusively:

```cpp
__global__ void scale(float* data, int n, float factor)
```

Three reasons:

1. **It is array data.** Arrays decay to pointers; there is nothing to alias.
2. **The CUDA runtime API is C.** `cudaMalloc` and `cudaMemcpy` deal in `void*`.
3. **The address space is explicit.** A device pointer refers to GPU memory. Hiding that behind an alias would obscure the single most important property of the variable.

The scalar parameters — `n`, `factor` — are by value: a few bytes copied to every thread, which is exactly right.

## The question that generalizes

Every level of this stack asks the same thing:

| Level | The question |
| --- | --- |
| Function parameter | Copy or original? |
| Kernel parameter | Copy to every thread, or a shared address? |
| Host to device | Do these bytes cross the PCIe bus? |
| Kernel fusion | Does this intermediate go to global memory, or stay in registers? |

That last row is the research North Star of this repository. It is this module's question, asked about a tensor instead of an `int`.

## Why This Matters for CUDA

Passing a large object by value on the CPU wastes a memcpy. The GPU equivalent — copying a tensor to the device that did not need copying, or writing an intermediate to global memory that could have stayed in registers — wastes bandwidth, which is the resource that actually limits deep learning kernels.

Learning to ask "does this need to be copied?" on a 4-byte `int` is cheap practice for asking it about 40 MB of activations.