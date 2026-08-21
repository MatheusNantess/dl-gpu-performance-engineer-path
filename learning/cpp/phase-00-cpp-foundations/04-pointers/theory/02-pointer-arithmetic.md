# 02 — Pointer Arithmetic

## `p + 1` is not one byte

```cpp
int  a[4] = {10, 20, 30, 40};
int* p = a;      // points at a[0]

p + 1;           // points at a[1] -- FOUR bytes further
```

Adding 1 to a pointer advances by **one element**, meaning `sizeof(T)` bytes.

```text
int* p        stride 4 bytes

  p        p+1      p+2      p+3
  |         |        |        |
  v         v        v        v
+--------+--------+--------+--------+
|   10   |   20   |   30   |   40   |
+--------+--------+--------+--------+
0x1000   0x1004   0x1008   0x100C


double* q     stride 8 bytes

  q                 q+1               q+2
  |                  |                 |
  v                  v                 v
+-----------------+-----------------+-----------------+
|      1.0        |      2.0        |      3.0        |
+-----------------+-----------------+-----------------+
0x2000            0x2008            0x2010
```

The rule:

```text
address of (p + i)  ==  address of p  +  i * sizeof(T)
```

The compiler inserts the multiplication. This is the same formula as the array layout rule from Module 03, which is not a coincidence — they are the same fact.

## Operations that exist

```cpp
p + n     // advance n elements
p - n     // go back n elements
++p       // advance one element
--p       // back one element
p - q     // DIFFERENCE: how many elements apart (only within one array)
p == q    // comparison
```

`p - q` yields a count of elements, not bytes. To get bytes, cast to `char*` first — a `char` is 1 byte by definition, which is why `char*` is the standard way to do byte-level address arithmetic.

Adding two pointers is not defined and would be meaningless.

## Traversal by pointer

```cpp
int a[4] = {10, 20, 30, 40};

// by index
for (int i = 0; i < 4; ++i) std::cout << a[i] << "\n";

// by pointer
for (int* p = a; p < a + 4; ++p) std::cout << *p << "\n";
```

Identical output, identical generated code in practice. Being fluent in both matters because CUDA code uses both, often in the same kernel.

`a + 4` points one past the last element. Forming that address is legal and is the standard end sentinel; *dereferencing* it is not.

## Precedence traps

```cpp
*p + 1     // dereference, THEN add 1 to the value
*(p + 1)   // add 1 to the pointer, THEN dereference
*p++       // dereference p, then advance p (postfix binds tighter)
(*p)++     // increment the VALUE at p
++*p       // also increments the value
```

These read very differently and are easy to mix up. Parenthesize when there is any doubt.

## Why This Matters for CUDA

Pointer arithmetic **is** GPU indexing. A kernel computes:

```cpp
int i = blockIdx.x * blockDim.x + threadIdx.x;
float value = data[i];      // == *(data + i)
```

Address `data + i * sizeof(float)`. When 32 threads in a warp compute 32 consecutive `i` values, they touch 128 consecutive bytes and the hardware coalesces the access. When the index calculation produces a stride — `data[i * stride]` — the addresses spread out and the same 32 reads cost several times more.

Strided access is exactly `p + i * stride` from this file, and whether a kernel is fast usually comes down to what that stride is. Which is why this module is worth the extra time.