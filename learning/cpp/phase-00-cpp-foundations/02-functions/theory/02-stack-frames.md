# 02 — Stack Frames

Conceptual, not assembly-level. The goal is one mental image that explains parameter copying, local lifetime, and why returning a pointer to a local is fatal.

## The call stack

Every function call pushes a **stack frame** — a region of memory holding that call's parameters, local variables, and where to return to.

```text
main() calls process() calls helper()

        higher addresses
    +---------------------+
    | main's frame        |   locals of main
    +---------------------+
    | process's frame     |   parameters + locals of process
    +---------------------+
    | helper's frame      |   <-- currently executing
    +---------------------+
        lower addresses      (the stack typically grows downward)
```

When `helper` returns, its frame is popped. The memory is not erased — it is simply no longer owned, and the next call will reuse it.

## What that explains

**Parameters are copies.** Passing an `int` writes 4 bytes into the new frame. The original is untouched.

**Locals die at the closing brace.** They lived in a frame that no longer exists.

**Returning a pointer to a local is fatal.** The address is real, but it points into a frame that has been popped. The next function call overwrites it.

```cpp
int* broken() {
    int x = 42;
    return &x;     // frame dies at the closing brace
}
```

**Recursion has a limit.** Each call adds a frame. Enough of them and the stack space is exhausted — stack overflow. Python raises `RecursionError`; C++ typically crashes.

**Stack allocation is nearly free.** Making a frame is adjusting a pointer. This is why stack variables are cheap and heap allocation (Module 06) is not.

## Big parameters are expensive

```cpp
void process(BigStruct s);    // copies the whole struct into the frame
void process(BigStruct* s);   // copies one address (8 bytes)
```

For an `int`, copying is cheaper than the indirection. For anything large, it is not. This is the trade-off Module 05 formalizes.

## Stack size is limited

Typically a few megabytes on Linux, per thread. Which means:

```cpp
void f() {
    int huge[10000000];   // ~40 MB on the stack -- crash
}
```

Large arrays go on the heap. That constraint is why Module 06 exists.

## Why This Matters for CUDA

Every GPU thread has its own stack and its own registers, and there are thousands of threads. Consequences:

- **Per-thread local storage is tiny.** A kernel declaring large local arrays spills them to slow "local" memory, which is actually global memory with a friendly name. This shows up in profiling as local memory traffic and is a real performance bug.
- **Register pressure is this idea taken seriously.** The number of registers a thread uses determines how many threads fit on a multiprocessor, which determines occupancy. It is one of the first things Nsight Compute reports.
- **Kernel parameters are copied to every thread**, which is why they are small scalars and pointers, never large structures.

The frame picture above is the CPU version of a constraint that becomes central on the GPU.