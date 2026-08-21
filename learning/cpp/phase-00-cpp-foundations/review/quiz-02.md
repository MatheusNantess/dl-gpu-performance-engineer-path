# Quiz 02 — After Module 04

Covers everything through pointers. **The most important quiz in the phase.** Closed book.

## Part A — Explain

1. What does "contiguous" mean, in terms of addresses?
2. Why does `sizeof(arr)/sizeof(arr[0])` fail inside a function?
3. What is array decay?
4. Why does every kernel signature carry an explicit `n`?
5. Explain `x`, `&x`, `p`, `*p` and `&p`, given `int x; int* p = &x;`
6. Why does `p + 1` advance by `sizeof(T)` rather than one byte?
7. Why are `a[i]` and `*(a + i)` the same expression?
8. What does `p - q` return, and in what units?
9. Name the three ways a pointer becomes invalid. Which is most dangerous, and why?
10. Why does `cudaMalloc` take `&d_ptr` rather than `d_ptr`?

## Part B — Predict the output

```cpp
int a[5] = {10, 20, 30, 40, 50};
int* p = a + 2;
```

1. `*p`
2. `*(p + 1)`
3. `*(p - 1)`
4. `p[1]`
5. `p - a`
6. `*p + 1`
7. `a[4] - *p`

```cpp
int x = 5;
int* q = &x;
*q = 10;
std::cout << x;          // 8.
```

```cpp
void f(int* p) { p = nullptr; }
int y = 1;
int* r = &y;
f(r);
std::cout << (r == nullptr);   // 9.
```

## Part C — Memory reasoning

1. `float data[8]` starting at address 0x1000. What is the address of `data[5]`?
2. `double d[4]` at 0x2000. What is the address of `d[3]`?
3. A 6×8 matrix stored flat, row-major. What is the flat index of element (3,5)?
4. Same matrix of floats. What is the byte offset of (3,5) from the start?
5. How many bytes does an array of 1,000,000 floats occupy? In MB?
6. A function reads an array of n floats and writes n floats. How many bytes move?

## Part D — Find the bug

```cpp
// 1.
int sum(int* arr) {
    int n = sizeof(arr) / sizeof(arr[0]);
    int s = 0;
    for (int i = 0; i < n; ++i) s += arr[i];
    return s;
}

// 2.
int* make() {
    int arr[10];
    return arr;
}

// 3.
int a[5];
for (int i = 0; i <= 5; ++i) a[i] = i;

// 4.
int* p;
*p = 10;
```

## Part E — Write it

1. Sum an array using pointer arithmetic only, no `[]`.
2. Reverse an array in place.
3. Find both the minimum and maximum in one pass, returning both via output parameters.
4. Compute the flat index for element (r, c) of a rows×cols row-major matrix.

## Part F — The readiness question

Given:

```cpp
void scale(float* data, int n, float factor)
```

Explain, in full sentences:

1. What does `data` contain?
2. Where are the elements it refers to?
3. How is `data[i]` computed?
4. What does `data + i` mean?
5. How does the function modify the caller's data?
6. For n = 1,000,000, roughly how many bytes does it access?
7. Why does contiguous memory matter here?

---

**Score: ___ / 40**

Below 70% on Parts B, C or F: **do not start Module 05.** Reread Module 04's theory and redo the prediction exercises. Everything after this depends on it.