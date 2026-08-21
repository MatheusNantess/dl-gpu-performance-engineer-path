# 02 — Stack and Heap

## Two regions

```text
        high addresses
    +-------------------+
    |       STACK       |   locals, parameters, return addresses
    |         |         |   grows DOWN
    |         v         |
    |                   |
    |     (unused)      |
    |                   |
    |         ^         |
    |         |         |   grows UP
    |       HEAP        |   new / malloc allocations
    +-------------------+
    |   static / globals|
    +-------------------+
    |       code        |
    +-------------------+
        low addresses
```

Illustrative; real layouts vary. The useful part is that they are separate regions with different rules.

## Stack

```cpp
void f() {
    int x = 10;
    int arr[100];
}   // both gone here
```

- **Allocation:** adjusting a pointer. Essentially free.
- **Lifetime:** automatic — destroyed at the closing brace.
- **Size:** limited, typically a few MB per thread. Check with `ulimit -s`.
- **Requires:** the size known at compile time.

Exceeding it is a stack overflow, which usually crashes without a helpful message.

## Heap

```cpp
int* p = new int[1000000];
// ...
delete[] p;
```

- **Allocation:** the allocator searches for a suitable block. Expensive relative to the stack.
- **Lifetime:** manual — alive until `delete`.
- **Size:** limited by available RAM.
- **Requires:** nothing known at compile time; the size can be a run-time value.

## Choosing

| Use the stack when | Use the heap when |
| --- | --- |
| Size is known at compile time | Size is a run-time value |
| Small (kilobytes) | Large (megabytes or more) |
| Lifetime matches a scope | Lifetime outlives the creating function |

Default to the stack. Reach for the heap when one of the right-hand conditions applies.

## What Python hides

Every Python object is on the heap, and the garbage collector frees them when the last reference disappears. There is no stack/heap decision to make and no `delete` to forget.

C++ gives the choice and the responsibility. Getting it wrong produces a leak, a dangling pointer, or a stack overflow.

## Why This Matters for CUDA

The GPU has an analogous hierarchy, with sharper trade-offs:

| CPU | GPU | Speed | Size |
| --- | --- | --- | --- |
| Registers | Registers | Fastest | ~tens of KB per multiprocessor |
| Stack | Local memory | **Actually global memory** | — |
| Cache | Shared memory / L1 | Fast | ~100 KB per multiprocessor |
| Heap | Global memory | Slow | 12 GB on this device |

Two things to note now:

**"Local memory" on a GPU is a lie.** It is global memory used for per-thread spills. A kernel declaring a large local array does not get fast storage — it gets slow global memory, and profiling shows it as local memory traffic. This is a real performance bug with a misleading name.

**Register pressure is the stack-size question, taken seriously.** The registers a thread uses determine how many threads fit on a multiprocessor, which determines occupancy, which is one of the first things Nsight Compute reports.

The reason `experiments/` in this repository has a directory called `occupancy-and-registers` is this paragraph.