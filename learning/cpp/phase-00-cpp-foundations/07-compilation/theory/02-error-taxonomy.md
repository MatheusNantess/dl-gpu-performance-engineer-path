# 02 — Error Taxonomy

The most useful skill in this module: classify the failure in three seconds.

## Compile errors

The compiler could not turn source into an object file.

```text
error: expected ';' before '}' token
error: 'x' was not declared in this scope
error: cannot convert 'const char*' to 'int'
error: no matching function for call to 'foo(double)'
```

**Recognize by:** a file name and line number in your code, the word `error:`, and the compiler naming the construct.

**Note:** the reported line is where the compiler *noticed*, not necessarily where the mistake is. A missing semicolon typically reports on the following line.

**Rule:** fix the first error and recompile. Later errors are often cascade effects and vanish on their own.

## Linker errors

Every file compiled. Assembling them failed.

```text
undefined reference to `helper()'
multiple definition of `helper()'
collect2: error: ld returned 1 exit status
```

**Recognize by:** no line number in your source, mangled function names, mentions of `ld` or `collect2`.

**Common causes:**
- Declared and called but never defined
- Forgot to pass one `.o` file to the link step
- Forgot a library (`-lm`, `-lcudart`)
- Defined a function in a header included by several files
- A definition whose signature differs from the declaration — so the call refers to a symbol that does not exist

That last one is the confusing case: the code looks defined, and it is — under a different signature.

## Run-time errors

It built. It ran. It went wrong.

```text
Segmentation fault (core dumped)
free(): double free detected
std::bad_alloc
Floating point exception
(or: no message at all, and wrong output)
```

**Recognize by:** the program started.

The worst case is the last one — no crash, wrong answer. Which is precisely why the sanitizers exist.

## Warnings

Not errors. The compiler built it and thinks you made a mistake.

```text
warning: unused variable 'x'
warning: comparison between signed and unsigned integer expressions
warning: 'y' may be used uninitialized
```

**In C++, warnings are usually real bugs.** Compile with `-Wall -Wextra` and treat them as errors in practice; `-Werror` enforces that literally.

## The triage table

| Symptom | Stage | Typical cause |
| --- | --- | --- |
| Line number in your file, `error:` | Compile | Syntax or types |
| `undefined reference` | Link | Missing definition or missing `.o` |
| `multiple definition` | Link | Definition in a header, or duplicated |
| `Segmentation fault` | Run | Bad pointer, out of bounds |
| Wrong output, no crash | Run | Logic error, or undefined behaviour |
| `warning:` and it still built | Compile | Probably a real bug |

## Why This Matters for CUDA

The same three categories, with extra ways to be confusing:

- **Compile errors** can come from the host pass or the device pass. Device-side messages sometimes reference generated code rather than your source.
- **Link errors** appear when a `__device__` function is declared but never defined, or when separate compilation of device code is misconfigured, or when `-lcudart` is missing.
- **Run-time errors are asynchronous.** A kernel launch returns immediately, so a fault inside a kernel surfaces at the *next* CUDA call — often in unrelated code. This is why every CUDA call's return code gets checked, and why `cudaDeviceSynchronize()` appears while debugging.

The habit of asking "which stage produced this?" is worth building now, on errors that are still easy to read.