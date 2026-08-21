# Example — A multi-file build

A working three-file program. Build it by hand, stage by stage, before using any shortcut.

## Files

| File | Contains |
| --- | --- |
| `math_utils.h` | Declarations — what exists |
| `math_utils.cpp` | Definitions — what it does |
| `main.cpp` | Uses it, includes the header |

## Build it in stages

```bash
# 1. Preprocess. Look at how large this becomes.
g++ -E main.cpp -o main.i
wc -l main.i

# 2. Compile each translation unit separately. No linking yet.
g++ -std=c++17 -Wall -Wextra -g -O0 -c math_utils.cpp -o math_utils.o
g++ -std=c++17 -Wall -Wextra -g -O0 -c main.cpp       -o main.o

# 3. Link.
g++ math_utils.o main.o -o program

./program
```

Then all at once, which is what you will normally do:

```bash
g++ -std=c++17 -Wall -Wextra -g -O0 math_utils.cpp main.cpp -o program
```

## Things to try

Each produces a specific, recognizable failure. Cause them deliberately and read the message.

1. **Link only `main.o`.** `g++ main.o -o program` — which error, and from which stage?
2. **Delete a definition** from `math_utils.cpp`, leaving its declaration. Do both files compile? Does it link?
3. **Move a function definition into the header** and include it from both `.cpp` files. What happens, and why?
4. **Change a parameter type in the header only**, so declaration and definition disagree. Predict the failing stage before trying.
5. **Remove the header guard** and include `math_utils.h` twice from `main.cpp`.

Number 4 is the instructive one: the function looks defined, and it is — under a signature nobody calls.

## Why This Matters for CUDA

This split — declarations in headers, definitions in `.cu` files, separate compilation, then linking — is exactly how CUDA projects are organized. A `__device__` function declared and never defined fails at the same stage, with a similarly indirect message.