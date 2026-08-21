# 03 — Static Typing

## The core difference

**Python:** a variable is a name bound to an object. The object carries its type. The binding can change at any moment.

```python
x = 5
x = "hello"    # fine
```

**C++:** a variable is a *named region of memory* of a fixed size, decided at compile time. The type is a property of that region, not of the value in it.

```cpp
int x = 5;
x = "hello";   // compile error
```

The type is gone by run time. It was information the compiler used to decide how many bytes to reserve and which machine instructions to emit. The running program sees only bytes.

## What that buys

**Errors caught before running.** A misspelled name, a wrong argument type, a bad conversion — all rejected at compile time. In Python these surface at run time, possibly only on a rare code path.

**Speed.** The compiler knows `x` is an `int` at a known offset, so it emits a single integer-add instruction. Python must look up the object, find its type, dispatch to the right `__add__`, and allocate a result object. This gap is most of why C++ is fast.

**Predictable memory.** `int x` is exactly 4 bytes at a known location. A Python integer is a heap object with a reference count, a type pointer, and a variable-width value — typically 28 bytes for a small int, reached through a pointer.

## What it costs

Rigidity. Types must be written or deducible, generic code needs templates, and the compiler will refuse things you know are fine.

## The two worlds

The single most useful mental habit from this module: **know which world each thing lives in.**

| Compile time | Run time |
| --- | --- |
| Types | Values |
| `sizeof(T)` | Contents of memory |
| Function overload resolution | Which branch is taken |
| Template instantiation | Loop iteration counts |
| Whether the code is legal | Whether it is correct |

`sizeof(int)` is a compile-time constant. The compiler replaces it with `4` and no computation happens when the program runs.

## Type deduction is still static

```cpp
auto x = 5;      // x is int, forever
auto y = 5.0;    // y is double, forever
```

`auto` is not dynamic typing. The type is fixed at compile time; you just did not spell it. `x = "hello";` still fails.

## Implicit conversions

C++ converts between numeric types silently, and not always harmlessly:

```cpp
int i = 3.9;        // 3 -- truncation, no warning by default
float f = 1e40;     // infinity -- out of float range
char c = 300;       // implementation-defined; wraps
```

`-Wall -Wextra` catches many of these. Brace initialization (`int i{3.9}`) rejects them outright. Both are worth using by default.

## Why This Matters for CUDA

A CUDA kernel is compiled ahead of time for a specific GPU architecture. Types, sizes and layouts are all fixed before the kernel ever launches. There is no run-time type information on the device and no dynamic dispatch — the compile-time/run-time split is *sharper* on the GPU, not softer.

The habit of asking "is this known at compile time?" is exactly the habit needed later for template parameters, block sizes, and understanding why a kernel specialized for a known size beats a general one.