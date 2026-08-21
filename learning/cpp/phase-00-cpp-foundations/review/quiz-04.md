# Quiz 04 — Full Phase Review

Everything. Take this after Module 08 and before the CUDA readiness checkpoint. Closed book.

## Part A — Explain (one or two sentences each)

1. Why is C++ faster than Python for numeric loops?
2. What does the compiler know that a Python interpreter learns only at run time?
3. What does `#include` do?
4. What are the four stages of the build?
5. Compile error, linker error, run-time error — how do you tell them apart?
6. What is a stack frame?
7. What does array decay mean, and what is lost?
8. Explain `p`, `*p` and `&p`.
9. Why does `p + 1` advance by `sizeof(T)`?
10. Name three ways a pointer becomes invalid.
11. Stack versus heap — four differences.
12. What is a memory leak?
13. What is a cache line?
14. What does memory bound mean?
15. Why is `sizeof(struct)` often larger than the sum of its fields?
16. What is the difference between AoS and SoA, and when does each win?
17. Why does field order change a struct's size?
18. What does `alignas(16)` enable?

## Part B — Predict

```cpp
struct A { char x; int y; };
struct B { int y; char x; };
struct C { char a; int b; char c; };
struct D { int b; char a; char c; };
```
1. `sizeof(A)`  2. `sizeof(B)`  3. `sizeof(C)`  4. `sizeof(D)`

```cpp
float f[4];
```
5. `sizeof(f)`  6. address gap between `f[0]` and `f[1]`

```cpp
struct P { float x, y, z; };
P points[10];
```
7. Byte gap between `points[0]` and `points[1]`
8. Byte gap between `points[0].x` and `points[1].x`

## Part C — Memory reasoning

1. A million particles, each with 6 floats. AoS total bytes? SoA total bytes?
2. Reading only the x field of every particle: how many useful bytes per 64-byte cache line, in each layout?
3. `struct {char; double; int;}` — compute the size by hand, showing every padding byte.
4. Reorder it to minimize the size. New size?
5. A 1024×1024 FP32 matrix — size in MB?
6. `out[i] = a[i] + b[i]` over 10 million floats — total bytes moved?
7. At a sustained 300 GB/s, what is the floor on that operation's runtime? Why is it a floor rather than a prediction?

## Part D — Debugging

For each, name the bug and the stage at which it surfaces:

```cpp
// 1.
int f(int x);
int main() { return f(5); }
// (no definition anywhere)

// 2.
int a[5];
a[5] = 10;

// 3.
struct S { int x; };
S* p = nullptr;
p->x = 5;

// 4.
int* p = new int[10];
delete p;

// 5.
for (unsigned i = 4; i >= 0; --i) { }

// 6.
int x;
if (x > 0) { }
```

## Part E — Write it

1. `void vector_add(const float* a, const float* b, float* out, int n)`
2. Normalize an array to zero mean and unit variance, in place.
3. A struct for a 3D particle, ordered to minimize size.
4. Allocate a rows×cols matrix on the heap, fill element (r,c) with r*cols+c, print it, free it.

## Part F — Explain it to someone else

Write a paragraph each, as if to someone who does not know:

1. Why do pointers exist?
2. Why does contiguous memory make programs fast?
3. Why does data layout affect GPU performance?
4. Why is `void scale(float* data, int n, float factor)` written that way, and not some other way?

---

**Score: ___ / 45**

Above 85% here means the [CUDA readiness checkpoint](../cuda-readiness-checkpoint.md) is the next step. Below 70% means specific modules need rereading — identify which from the question numbers.