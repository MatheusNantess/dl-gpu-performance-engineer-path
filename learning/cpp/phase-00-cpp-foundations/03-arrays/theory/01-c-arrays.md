# 01 — C-style Arrays

## Declaration

```cpp
int x[4] = {10, 20, 30, 40};
```

That reserves **four consecutive ints** — 16 contiguous bytes — and fills them.

```text
Addresses are illustrative. Real ones differ every run.

Index    Address     Value
  [0]    0x1000        10
  [1]    0x1004        20
  [2]    0x1008        30
  [3]    0x100C        40

           ^^^^
           each step is +4 bytes, because sizeof(int) == 4
```

The addresses differ by exactly `sizeof(int)`. Nothing sits between elements. There is no header, no length field, no type tag — just 16 bytes.

## How this differs from a Python list

| | Python `list` | C array |
| --- | --- | --- |
| Contents | Pointers to objects scattered on the heap | The values themselves, packed |
| Size | Grows | Fixed at compile time |
| Knows its length | Yes, `len()` | **No** |
| Bounds checked | Yes, raises `IndexError` | **No** |
| Element types | Mixed | All identical |

The last two are the dangerous ones. `x[100]` on a 4-element array compiles, runs, and reads whatever bytes happen to be 400 bytes past the start. Sometimes it prints garbage. Sometimes it crashes. Sometimes it silently corrupts another variable.

A NumPy array is the closer analogue: contiguous, single dtype, fixed element size. If it helps, think of a C array as a NumPy array with the safety and the metadata removed.

## Initialization forms

```cpp
int a[4] = {1, 2, 3, 4};   // all four
int b[4] = {1, 2};         // remaining elements zero-initialized
int c[4] = {};             // all zeros
int d[4];                  // GARBAGE -- uninitialized
int e[] = {1, 2, 3};       // size deduced: 3
```

`int d[4];` gives four ints of whatever was previously in that memory.

## Indexing

```cpp
x[0]   // first
x[3]   // last of four
x[4]   // OUT OF BOUNDS -- compiles, no error at run time either
```

Zero-based, like Python. Unlike Python there is **no negative indexing** — `x[-1]` reads the memory before the array, which is a legal instruction on invalid data.

Nothing in the language checks bounds. Not the compiler, not the run time. This is the single most common source of memory bugs in C and C++.

## Traversal

```cpp
const int n = 4;
int x[n] = {10, 20, 30, 40};

for (int i = 0; i < n; ++i) {
    std::cout << x[i] << "\n";
}
```

The count `n` has to be tracked by hand, because the array does not carry it.

## Arrays live on the stack here

`int x[4]` inside a function is part of that function's stack frame. It is created on entry and gone at the closing brace, and its size must be a compile-time constant.

Which means this fails:

```cpp
int huge[10000000];   // ~40 MB -- exceeds the stack, crashes
```

Big arrays need the heap. That is Module 06.

## Why This Matters for CUDA

Contiguity is the reason GPU memory access has a "right" pattern at all.

A warp of 32 threads reading `data[0]` through `data[31]` touches 128 consecutive bytes, which the memory system fetches in a handful of wide transactions. The same 32 threads reading elements 0, 100, 200, 300… touch 32 separate regions and need many more transactions for the same amount of useful data.

Same instruction count, same arithmetic, several times slower — purely because of where the bytes were. That is coalescing, and this is the module where the ground for it gets laid.

The missing bounds check matters too: on a GPU, an out-of-bounds write corrupts another tensor and produces wrong training results with no crash and no message.