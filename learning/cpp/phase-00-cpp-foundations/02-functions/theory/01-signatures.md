# 01 — Signatures, Declarations, Definitions

## The shape

```cpp
int add(int a, int b) {
    return a + b;
}
```

`int` is the return type, `add` the name, `(int a, int b)` the parameters — each with its own explicit type.

Everything is stated. No inference, no duck typing, and the compiler checks every call site against this signature.

## `void`

```cpp
void print_banner() {
    std::cout << "---\n";
}
```

`void` means no return value. `return;` with no expression exits early.

## Declaration versus definition

```cpp
int add(int a, int b);              // declaration (prototype) -- promise

int add(int a, int b) {             // definition -- the body
    return a + b;
}
```

The compiler reads a translation unit top to bottom and must know a function's signature before the call. So either define it above its first use, or declare it above and define it anywhere.

```cpp
int add(int a, int b);              // prototype

int main() {
    std::cout << add(1, 2) << "\n"; // legal: signature is known
}

int add(int a, int b) { return a + b; }
```

This split is the entire reason header files exist. Headers hold declarations; `.cpp` files hold definitions. Module 07 covers what goes wrong when they disagree.

Python has no equivalent: a function must simply exist by the time the call *executes*, which is a run-time condition, not a compile-time one.

## Parameter names are optional in declarations

```cpp
int add(int, int);   // legal, and common in headers
```

Names in a declaration are documentation. Include them anyway.

## Overloading

Several functions may share a name if their parameter lists differ:

```cpp
int    square(int x)    { return x * x; }
double square(double x) { return x * x; }
```

The compiler picks based on argument types, at compile time. Impossible in Python, where a second `def square` simply replaces the first.

**Return type alone cannot distinguish overloads.** These two conflict:

```cpp
int    f(int x);
double f(int x);   // error
```

Useful to recognize; not worth dwelling on now.

## Default arguments

```cpp
void scale(float* data, int n, float factor = 1.0f);
```

Defaults go in the declaration, not in the definition, when both exist.

## Why This Matters for CUDA

A kernel is a function with a marker:

```cpp
__global__ void vector_add(const float* a, const float* b, float* out, int n);
```

`__global__` means: called from the host, runs on the device, must return `void`. That last constraint is not arbitrary — the call is asynchronous, so there is nothing to return a value *to*. Results come back through the pointers.

`nvcc` compiles host and device code in separate passes, so a mismatch between a declaration and a definition can surface as a linker error that mentions neither file you expected. Understanding the declaration/definition split now makes that error readable later.