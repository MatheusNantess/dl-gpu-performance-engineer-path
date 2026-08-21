# Quiz 03 — After Module 06

Covers everything through memory. Closed book.

## Part A — Explain

1. What is the difference between a reference and a pointer? Give four differences.
2. Given `int& r = x;`, what does `r = y;` do?
3. When would you choose a pointer over a reference?
4. Why do CUDA kernels use pointers and never references?
5. Compare the stack and the heap: allocation cost, lifetime, size limit.
6. What is a memory leak, and why can the memory not be recovered?
7. Why does use-after-free often appear to work?
8. What is a cache line, and why does it make sequential access fast?
9. What does "memory bound" mean?
10. Why is GPU "local memory" not fast?

## Part B — Predict

```cpp
void a(int x)  { x = 99; }
void b(int* x) { *x = 99; }
void c(int& x) { x = 99; }

int v = 1;
a(v);  // 1. v is now?
v = 1;
b(&v); // 2. v is now?
v = 1;
c(v);  // 3. v is now?
```

```cpp
int* p = new int[5];
std::cout << p[0];              // 4. what is printed?

int* q = new int[5]();
std::cout << q[0];              // 5. what is printed?
```

```cpp
int* r = new int[3];
delete[] r;
std::cout << r[0];              // 6. what happens?
```

7. Which of these three loops is fastest on a 100 MB array, and why?
```cpp
for (int i = 0; i < n; ++i)      sum += a[i];
for (int i = 0; i < n; i += 16)  sum += a[i];
for (int i = 0; i < n; ++i)      sum += a[idx[i]];
```

## Part C — Memory reasoning

1. `int arr[1000000]` as a local variable. What happens, and why?
2. Which is more expensive, one allocation of 1000 ints or 1000 allocations of one int? Why?
3. A 64-byte cache line, reading floats with stride 16. How many useful bytes per line?
4. Where does a stack variable live? A heap allocation? Which survives its enclosing function?
5. Give the GPU analogue of: registers, cache, heap.

## Part D — Find the bug

```cpp
// 1.
void f() {
    int* p = new int[100];
}

// 2.
int* p = new int[10];
delete p;

// 3.
int* p = new int[10];
delete[] p;
delete[] p;

// 4.
int* get() {
    int* p = new int[10];
    return p;
}
// called in a loop, return value ignored
```

## Part E — Write it

1. Allocate an array of n floats on the heap, fill it, use it, free it correctly.
2. A function taking `float**` that allocates and sets the caller's pointer.
3. The matching release function that frees and nulls the caller's pointer.

## Part F — Cumulative (Modules 01–04)

1. `sizeof(int*)` versus `sizeof(double*)` — same or different? Why?
2. Given `float a[10]` at 0x1000, what is the address of `a[7]`?
3. Why is `a[i]` the same as `*(a + i)`?
4. What does an uninitialized local `int` contain?
5. Why does `7 / 2` give 3?

---

**Score: ___ / 32**