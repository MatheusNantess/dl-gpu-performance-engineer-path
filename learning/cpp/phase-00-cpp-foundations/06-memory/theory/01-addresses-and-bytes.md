# 01 — Addresses and Bytes

## Memory is a numbered array of bytes

That is the whole model. Every byte has an address, addresses are consecutive integers, and a variable occupies some run of them.

```text
Address     Byte
0x1000      0x0A
0x1001      0x00
0x1002      0x00
0x1003      0x00     <-- these four are one int, holding 10

0x1004      ...
```

A pointer is a variable holding one of those numbers. That is all a pointer has ever been.

## Address width

On a 64-bit machine, addresses are 64 bits — hence `sizeof(int*) == 8`. Every pointer is 8 bytes, whatever it points to, because they all hold the same kind of number.

## Byte ordering

A 4-byte `int` holding 10 can store its bytes low-first (little-endian) or high-first (big-endian). x86 and ARM in practice are little-endian.

Worth knowing it exists. Not worth dwelling on — it matters for serialization and for reading raw memory dumps, and rarely otherwise.

## Addresses change every run

Print the address of a variable, run twice, get different numbers. That is **ASLR** — the operating system randomizes where a process is loaded, as a security measure.

Consequence for every experiment in this module: **the absolute value is meaningless. The difference is the data.**

```cpp
// meaningless: 0x7ffd4a2b3c
std::cout << &x;

// meaningful: 4
std::cout << (const char*)&a[1] - (const char*)&a[0];
```

## Printing addresses

```cpp
int x = 10;
std::cout << &x;                              // fine for int*
char c = 'a';
std::cout << &c;                              // WRONG: treated as a C string
std::cout << static_cast<const void*>(&c);    // correct
```

`operator<<` has an overload for `char*` that prints a string. Cast to `const void*` to print an address as an address. This bites everyone once.

For byte-level arithmetic, cast to `const char*` — a `char` is 1 byte by definition, so pointer differences come out in bytes.

## Alignment, briefly

Types generally sit at addresses that are multiples of their size: a 4-byte `int` at an address divisible by 4, an 8-byte `double` at one divisible by 8. Hardware reads aligned data faster, and some architectures fault on misaligned access.

This is why structs contain padding, which is Module 08.

## Why This Matters for CUDA

- **Coalescing is an address question.** Whether a warp's 32 accesses merge into a few transactions depends entirely on the addresses, and reasoning about that requires this model.
- **Alignment matters more on a GPU.** Vectorized loads — reading 16 bytes per instruction with `float4` — require the address to be 16-byte aligned. Misaligned data silently falls back to the slower path.
- **Device addresses are printable and not dereferenceable on the host.** They are numbers in a different address space, and nothing in the type system says so.