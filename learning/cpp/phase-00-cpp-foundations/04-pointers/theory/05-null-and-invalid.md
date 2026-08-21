# 05 — nullptr and Invalid Pointers

## `nullptr`

```cpp
int* p = nullptr;    // points at nothing, explicitly
```

A defined "no address" value, testable:

```cpp
if (p != nullptr) { /* safe to dereference */ }
if (p)            { /* same thing, idiomatic */ }
```

Prefer `nullptr` over `NULL` or `0`. It has a distinct type and cannot be confused with an integer.

Dereferencing it reliably crashes with a segmentation fault — which, compared to the alternatives below, is the *good* outcome. A crash is information.

## The three ways a pointer goes bad

**Uninitialized.**

```cpp
int* p;      // holds garbage -- some address-shaped bit pattern
*p = 5;      // writes to an arbitrary location
```

Worse than null, because it may not crash. It may write into memory the program legitimately owns and corrupt something unrelated. Always initialize to `nullptr` at minimum.

**Dangling.**

```cpp
int* p;
{
    int x = 42;
    p = &x;
}                 // x destroyed here
*p;               // dangling: the address is stale
```

The address is well-formed and points at memory that no longer belongs to anything. Reading it often "works" — the value is still lying there until something overwrites it. This is what makes the bug so hard: it is intermittent, and it appears to be fine right up until it is not.

Also produced by using memory after `delete`.

**Out of bounds.**

```cpp
int a[4];
int* p = a + 10;     // forming it is already undefined
*p = 5;              // writes past the array
```

No bounds check exists. If the address happens to be valid memory the program owns, the write silently corrupts something else.

## Segmentation fault

The OS raises SIGSEGV when a process touches memory it does not own. It means "you went far enough outside your memory that the hardware noticed" — not "you made a memory error", because most memory errors stay inside the process's own memory and go undetected.

**A program that does not segfault is not therefore correct.**

## Tools

```bash
g++ -fsanitize=address -g -O0 -o prog prog.cpp && ./prog
```

AddressSanitizer catches out-of-bounds access, use-after-free, and leaks, and reports the exact line. `-fsanitize=undefined` catches signed overflow and other undefined behaviour. `valgrind` is the heavier alternative.

Use them by default in this phase. Several exercises require them.

## Defensive habits

```cpp
int* p = nullptr;        // always initialize
delete[] p;
p = nullptr;             // null it after freeing, so a later use crashes loudly
```

Setting a freed pointer to `nullptr` converts a silent use-after-free into an immediate crash. That is a trade worth making every time.

## Why This Matters for CUDA

The GPU versions are the same bugs with worse ergonomics:

- **No sanitizer by default.** `compute-sanitizer` exists and must be run deliberately.
- **Out-of-bounds writes corrupt other tensors.** Training continues; the loss curve is subtly wrong; nothing reports anything.
- **Host and device pointers are indistinguishable to the type system.** Dereferencing a device pointer on the host segfaults; passing a host pointer to a kernel corrupts memory. Both are `float*`.
- **Errors are asynchronous.** A kernel launch returns immediately, so the error surfaces at the *next* CUDA call — often in unrelated code. Checking every return code is not paranoia, it is the only way to localize the failure.

The `h_`/`d_` naming convention exists because the compiler cannot help. The discipline built in this module is what replaces it.