# 03 — Passing: Copy or Original

The central question of this module.

## Pass by value

```cpp
void increment(int x) {
    x = x + 1;      // modifies the COPY
}

int main() {
    int a = 5;
    increment(a);
    // a is still 5
}
```

The parameter is a separate variable initialized from the argument. Nothing the function does to it is visible outside.

Coming from Python this feels familiar for immutable objects and wrong for everything else — because in Python, passing a list and appending to it *is* visible outside. In C++ the rule is uniform: by value means copy, always.

## Pass by pointer

```cpp
void increment(int* x) {
    *x = *x + 1;    // modifies what x POINTS TO
}

int main() {
    int a = 5;
    increment(&a);
    // a is now 6
}
```

The pointer itself is still copied — 8 bytes — but the copy holds the same address, so `*x` reaches the caller's variable.

Two things become visible at the call site: `&a` announces that the address is being taken, and the signature announces that the function may modify it.

Pointers are Module 04. What matters here is the shape and the consequence.

## Pass by const pointer

```cpp
void print_all(const float* data, int n);
```

The function may read through the pointer but not write through it. It documents intent, and the compiler enforces it.

Note where `const` sits:

```cpp
const float* p;   // pointer to const float -- cannot change *p
float* const p;   // const pointer to float -- cannot change p itself
```

The first is what appears in kernel signatures.

## The cost of copying

```cpp
void f(int x);          // 4 bytes copied -- cheap
void f(BigStruct s);    // sizeof(BigStruct) copied -- possibly expensive
void f(BigStruct* s);   // 8 bytes copied
```

Copying an array is not even possible this way — arrays decay to pointers when passed (Module 03), so an array parameter is *always* effectively by pointer. This is a common source of confusion and Module 03 addresses it directly.

## Choosing

| Want | Use |
| --- | --- |
| Read a small value | by value |
| Modify the caller's variable | pointer (or reference, Module 05) |
| Read something large without copying | `const T*` (or `const T&`) |
| Operate on an array | pointer — there is no other option |

## Why This Matters for CUDA

Read this signature and every part of it is a decision from this module:

```cpp
__global__ void scale(float* data, int n, float factor)
```

- `float* data` — by pointer. There is no copy of the array, and there could not be: it lives in device memory and may be gigabytes. The kernel writes the caller's memory directly.
- `int n`, `float factor` — by value. Four bytes each, copied to every thread. Cheap and correct.
- If it were `const float* data`, the kernel could not write results, so it is not.

And the version that reads without writing:

```cpp
__global__ void vector_add(const float* a, const float* b, float* out, int n)
```

Two read-only inputs, one writable output. The `const` is not decoration — it tells the compiler the input will not change, which enables optimizations and, on some architectures, a faster read-only data path.

Being able to read that signature and immediately know what is copied, what is shared, and what is writable **is the readiness bar for this whole phase.**