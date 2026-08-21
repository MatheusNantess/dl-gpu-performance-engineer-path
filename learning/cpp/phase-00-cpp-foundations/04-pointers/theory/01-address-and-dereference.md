# 01 — Address and Dereference

## Start with one variable

```cpp
int x = 10;
```

The compiler reserved 4 bytes somewhere and wrote 10 into them.

```text
Address       Value
0x7ffd1000      10      <-- this is x
```

Addresses here are illustrative. Real ones change every run, because the operating system randomizes where a process is loaded.

## The four things

```cpp
int  x = 10;
int* p = &x;
```

```text
Address       Value
0x7ffd1000      10            <-- x lives here
0x7ffd1008      0x7ffd1000    <-- p lives here, and holds x's address

                p ---------> x
```

| Expression | Meaning | Value in this picture |
| --- | --- | --- |
| `x` | the value of x | `10` |
| `&x` | the **address of** x | `0x7ffd1000` |
| `p` | the value of p — which is an address | `0x7ffd1000` |
| `*p` | the value **at** the address in p | `10` |
| `&p` | the address of p **itself** | `0x7ffd1008` |

Read them out loud until they are automatic:

- `&x` — "address of x"
- `*p` — "the thing at p"
- `p` — "an address"
- `&p` — "where the pointer itself is stored"

**A pointer is a variable.** It occupies memory, it has its own address, and that is what `&p` gives. This is the part people skip, and it is exactly the part `cudaMalloc(&d_ptr, ...)` depends on.

## Declaration syntax

```cpp
int* p;      // pointer to int
int *p;      // identical -- the whitespace is meaningless
int * p;     // also identical
```

One catch:

```cpp
int* a, b;   // a is a pointer, b is a plain int
int *a, *b;  // both pointers
```

The `*` binds to the declarator, not the type. Declaring one per line avoids the whole issue.

## `*` means two different things

```cpp
int* p = &x;   // in a DECLARATION: "p is a pointer to int"
*p = 20;       // in an EXPRESSION: "the thing at p"
```

Same symbol, unrelated jobs. Context decides.

## Writing through a pointer

```cpp
int x = 10;
int* p = &x;

*p = 20;       // writes to x
std::cout << x;   // 20
```

`x` was never mentioned on the left-hand side. The write reached it through the address.

## Pointers have types

```cpp
int    x = 10;
int*   pi = &x;
float* pf = &x;    // error: cannot convert int* to float*
```

The type says how to interpret the bytes at that address, and how far one element is (next file). `void*` is an address with the type discarded — it cannot be dereferenced without casting back.

## Why This Matters for CUDA

```cpp
float* d_data;
cudaMalloc(&d_data, n * sizeof(float));
```

`cudaMalloc` must write an address into the caller's variable. Passing `d_data` by value would give it a copy, and the caller's pointer would stay unchanged. So it takes `&d_data` — the address of the pointer — and writes through it.

This is the pattern from Module 02 (`swap` needs addresses to modify the caller) applied to a pointer. If the picture above is solid, `&d_data` is obvious. If not, it is memorized noise.

The other CUDA-specific fact: a host pointer and a device pointer are both `float*` and are not interchangeable. The type system cannot distinguish them. Which address space a pointer refers to is something the programmer tracks, which is why the naming convention `h_` and `d_` exists.