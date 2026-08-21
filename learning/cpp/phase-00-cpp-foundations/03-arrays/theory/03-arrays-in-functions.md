# 03 — Arrays in Functions, and Decay

## Arrays are not copied

```cpp
void f(int arr[4]);
void f(int arr[]);
void f(int* arr);      // all three are THE SAME FUNCTION
```

C++ has no way to pass an array by value. Whatever you write, the parameter is a pointer to the first element. This is **array decay**: in almost every context, an array's name converts to a pointer to its first element.

The `[4]` in the first form is documentation the compiler ignores completely.

## The consequences

**The length is lost.**

```cpp
void print_all(int* arr) {
    int n = sizeof(arr) / sizeof(arr[0]);   // WRONG: 8/4 == 2, always
}
```

So the length travels separately, always:

```cpp
void print_all(const int* arr, int n);
```

That is why every kernel signature in this repository carries an `n`.

**The function writes the caller's array.**

```cpp
void zero_out(int* arr, int n) {
    for (int i = 0; i < n; ++i) arr[i] = 0;
}
```

The caller's data is modified. There was no copy to protect it.

**`const` is how you promise not to.**

```cpp
void print_all(const int* arr, int n);   // reads only
void scale(float* arr, int n, float f);  // writes
```

The signature tells the reader which it is. Use `const` wherever it is true.

## Where decay does not happen

```cpp
int x[4];
sizeof(x);      // 16 -- no decay
&x;             // pointer to array-of-4-int, not to int -- no decay
```

Outside these, assume decay.

## Returning an array

You cannot. This is broken:

```cpp
int* make_array() {
    int arr[4] = {1,2,3,4};
    return arr;             // dangling: the frame dies here
}
```

The two working approaches:

1. The caller allocates and passes it in — **the CUDA pattern**.
2. Allocate on the heap and return that pointer, with someone responsible for freeing it. Module 06.

```cpp
void fill(int* out, int n);   // caller owns the memory
```

## Why This Matters for CUDA

Every kernel signature in existence follows from this module:

```cpp
__global__ void vector_add(const float* a, const float* b, float* out, int n)
```

- Pointers, because arrays cannot be passed any other way.
- An explicit `n`, because the length does not travel with the pointer.
- `const` on inputs, none on the output.
- The caller allocated `out`; the kernel fills it.

There is no version of this that returns an array, and now the reason is structural rather than a CUDA quirk.