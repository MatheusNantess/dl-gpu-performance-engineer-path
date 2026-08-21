# 01 — References

## What it is

```cpp
int  x = 10;
int& r = x;    // r is another name for x

r = 20;
std::cout << x;   // 20
```

`r` is not a copy and not a pointer holding an address. It is an alias — a second name for the same storage.

```text
Address       Value
0x1000          20      <-- x, and also r. One object, two names.
```

Compare with a pointer, which is its own variable holding an address:

```text
Address       Value
0x1000          20        <-- x
0x1008    0x00001000      <-- p, a separate variable
```

In practice compilers usually implement references as pointers, but the *language* treats them as aliases, and the differences below follow from that.

## Differences from a pointer

| | Pointer | Reference |
| --- | --- | --- |
| Can be null | Yes | No |
| Can be reseated | Yes | **No** — bound at initialization, forever |
| Needs `*` to use | Yes | No |
| Has its own address | Yes | No (`&r` gives x's address) |
| Can be uninitialized | Yes | No — must be initialized |
| Arithmetic | Yes | No |

```cpp
int  a = 1, b = 2;
int& r = a;
r = b;          // does NOT rebind r to b. It assigns b's VALUE to a.
                // a is now 2.
```

That line is the one that catches people. A reference can never be pointed somewhere else after initialization; any assignment to it writes *through* it.

## In function parameters

```cpp
void by_value(int x)     { x = 99; }   // caller unaffected
void by_pointer(int* x)  { *x = 99; }  // caller's value changes
void by_reference(int& x){ x = 99; }   // caller's value changes
```

Called as:

```cpp
by_value(a);
by_pointer(&a);
by_reference(a);      // looks identical to by_value at the call site
```

**That is the trade-off.** References are cleaner to write and invisible at the call site — which means reading `f(a)` tells you nothing about whether `a` will be modified. A pointer forces `&a`, which is a visible signal. Different codebases weigh this differently; CUDA host code tends to prefer explicitness.

## `const` references

```cpp
void print(const BigStruct& s);   // no copy, and cannot modify
```

The main practical use: passing something large read-only without copying it. For an `int`, a `const int&` is pointless — the reference machinery costs as much as copying 4 bytes.

## References to array elements

```cpp
int a[4] = {1,2,3,4};
int& first = a[0];
first = 99;              // a[0] is now 99
```

Legal, occasionally useful, and a reminder that a reference names *storage*, not a value.

## Why This Matters for CUDA

Device code does not use references. Kernel parameters are values and pointers, because device memory is addressed explicitly and the CUDA runtime API is a C API.

So this module is mostly about **recognition**: PyTorch C++ extensions, CUDA host wrappers, and any modern C++ around kernels use references heavily, and `const at::Tensor&` needs to be readable.

The transferable idea is the one from Module 02, now with three answers instead of two: for any parameter, know whether the function got a copy or the original, and how many bytes crossed the boundary. On a GPU that question is about gigabytes and PCIe transfers rather than a few bytes of stack — but it is the same question.