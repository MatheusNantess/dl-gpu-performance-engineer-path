# 01 — The Pipeline

## The stages

```text
    source.cpp
        |
        |  PREPROCESSOR       #include, #define, #ifdef
        v                     -- pure text substitution
    translation unit
        |
        |  COMPILER           syntax, types, semantics
        v                     -- produces machine code with unresolved names
    source.o  (object file)
        |
        |  LINKER             resolves names across object files
        v                     -- adds library code
    executable
```

`g++ main.cpp -o program` runs all of it. Stopping partway is instructive:

```bash
g++ -E main.cpp -o main.i     # preprocess only -- look at how big this gets
g++ -S main.cpp -o main.s     # compile to assembly
g++ -c main.cpp -o main.o     # compile to object file, no linking
g++ main.o -o program         # link only
```

Running `-E` on a file that includes `<iostream>` produces tens of thousands of lines. That is why C++ builds are slow, and it is worth seeing once.

## Preprocessor

Text manipulation, no understanding of C++ at all:

- `#include <x>` — paste the contents of file x here
- `#define N 100` — replace every `N` with `100`
- `#ifdef` / `#endif` — conditionally keep or drop text

Header guards exist because pasting the same file twice defines everything twice:

```cpp
#ifndef MYHEADER_H
#define MYHEADER_H
// contents
#endif
```

Or `#pragma once`, which is shorter and universally supported in practice.

## Compiler

Works on one translation unit — one `.cpp` plus everything it included — in isolation. It checks syntax and types and emits machine code, leaving *references to things it has not seen* as unresolved symbols.

This is why a declaration is enough to compile a call. The compiler needs the signature; the linker finds the body.

## Linker

Collects object files and libraries and resolves every symbol. Its two characteristic failures:

- **undefined reference** — declared, called, never defined anywhere
- **multiple definition** — defined in more than one translation unit

Neither is a compile error. Both mean every individual file was fine and the *set* of them is not.

## Headers and source files

```text
math_utils.h     declarations   -- what exists
math_utils.cpp   definitions    -- what it does
main.cpp         uses it        -- includes the header
```

```bash
g++ -c math_utils.cpp -o math_utils.o
g++ -c main.cpp       -o main.o
g++ math_utils.o main.o -o program
```

Compiling `main.cpp` succeeds knowing only the declarations. The linker supplies the bodies.

Putting a function *definition* in a header and including it from two files produces a multiple-definition error — a classic, and worth causing deliberately once.

## Why This Matters for CUDA

`nvcc` splits each `.cu` file into host and device code and compiles them separately:

```text
    kernel.cu
        |
        +-- host code   --> the system C++ compiler --> host object
        |
        +-- device code --> PTX --> SASS ------------> embedded in the object
        |
        v
      linker
```

Which means a single source file can produce errors from either pass, and a `__device__` function that is declared but never defined fails at *link* time with a message that looks nothing like the C++ you wrote.

Knowing which stage produced an error is what makes those messages tractable.