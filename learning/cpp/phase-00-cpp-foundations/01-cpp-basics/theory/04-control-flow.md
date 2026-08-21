# 04 — Control Flow

Assumed known: what a loop and a branch are. What follows is what differs.

## `if`

```cpp
if (x > 0) {
    // ...
} else if (x < 0) {
    // ...
} else {
    // ...
}
```

Parentheses required, braces recommended even for one statement — omitting them is a well-known bug source.

The condition must be convertible to `bool`. In practice any nonzero number is `true`, which allows the classic typo:

```cpp
if (x = 5) { }   // ASSIGNS 5, then tests 5, which is true. Always taken.
if (x == 5) { }  // comparison
```

Compilers warn about this with `-Wall`.

## `for`

The C-style loop is the one that matters here:

```cpp
for (int i = 0; i < n; ++i) {
    // ...
}
```

Three parts: initialization, condition checked before each iteration, and the step run after each. `i` exists only inside the loop.

Python's `for i in range(n)` iterates over a sequence. This does not — it is a counter, a test and an increment, and that is precisely why it maps onto how a GPU thread computes its own index. **This is the loop shape that becomes a CUDA kernel.**

Range-based `for` also exists and is useful for containers:

```cpp
for (int value : some_array) { }
```

But the indexed form is the one to be fluent in, because kernels are written in terms of indices.

## `++i` versus `i++`

Both increment. `++i` yields the new value, `i++` yields the old one. In a loop step the result is discarded, so they behave identically; `++i` is the conventional choice.

## `while` and `do-while`

```cpp
while (condition) { }

do { } while (condition);   // body runs at least once
```

`do-while` has no Python equivalent.

## `switch`

```cpp
switch (value) {
    case 1:
        // ...
        break;
    case 2:
    case 3:
        // shared: 2 falls through to 3's body
        break;
    default:
        break;
}
```

Only for integers and enums, not strings or floats. **Forgetting `break` makes execution fall through into the next case** — occasionally intentional, usually a bug. Modern compilers warn.

Worth knowing because it appears in CUDA code; not worth dwelling on.

## `break` and `continue`

Same meaning as Python.

## Why This Matters for CUDA

A CUDA kernel is, in essence, a loop body where the index comes from the hardware instead of from a counter:

```cpp
// CPU
for (int i = 0; i < n; ++i) {
    out[i] = a[i] + b[i];
}
```

```cpp
// GPU: each thread computes one i
int i = blockIdx.x * blockDim.x + threadIdx.x;
if (i < n) {
    out[i] = a[i] + b[i];
}
```

The loop is gone; the index calculation and the bounds check remain. Being completely at home with the indexed `for` loop is what makes that translation feel obvious rather than magical.

**Branches also matter more on a GPU than on a CPU.** Threads execute in groups of 32, and when threads in a group take different branches, both paths run in sequence. That is *warp divergence*, and it is a real performance concern in Phase 2. The `if (i < n)` bounds check above is a divergent branch — a cheap one, but the idea starts here.