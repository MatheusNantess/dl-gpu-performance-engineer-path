# Quiz 01 — After Module 02

Covers: C++ basics, functions. Closed book. Write out your answers.

## Part A — Explain

1. Why is `sizeof(int)` known at compile time, and what does that cost at run time?
2. What does `#include` actually do? How does it differ from Python's `import`?
3. What does an uninitialized local variable contain?
4. What is the difference between a declaration and a definition, and why do both exist?
5. What is a stack frame, and what does it contain?
6. Why is returning the address of a local variable a bug?
7. What is the difference between scope and lifetime?

## Part B — Predict the output

For each, write the value before checking.

```cpp
int a = 7, b = 2;
std::cout << a / b;            // 1.
std::cout << a % b;            // 2.
std::cout << (double)a / b;    // 3.
std::cout << (double)(a / b);  // 4.
```

```cpp
int x = 5;
void f(int y) { y = 99; }
f(x);
std::cout << x;                // 5.
```

```cpp
unsigned int u = 0;
std::cout << u - 1;            // 6.
```

```cpp
int n = -1;
unsigned int m = 1;
std::cout << (n < m);          // 7.
```

## Part C — Find the bug

```cpp
// 1.
int average(int a, int b) {
    return a + b / 2;
}

// 2.
void increment(int x) {
    x = x + 1;
}

// 3.
int* get_value() {
    int v = 42;
    return &v;
}

// 4.
for (unsigned int i = 10; i >= 0; --i) {
    std::cout << i;
}
```

## Part D — Write it

1. A function that swaps two ints, so the caller sees the change.
2. A function returning both the larger and smaller of two ints.
3. A prototype for a function taking a read-only float array and its length, returning a float.

## Part E — Connect to CUDA

1. Why does `float` matter more than `double` for GPU work?
2. Why must a `__global__` function return `void`?
3. In `void scale(float* data, int n, float factor)`, which parameters are copied and which are not?

---

**Score: ___ / 24**