# 05 — Scope and Lifetime

Two different questions, easy to conflate:

- **Scope:** where in the source a name is visible.
- **Lifetime:** when the object exists in memory.

Python largely hides the second. C++ makes it the programmer's problem, and this module is where that starts.

## Block scope

```cpp
int main() {
    int a = 1;
    {
        int b = 2;
        // a and b both visible
    }
    // b is GONE -- out of scope and destroyed
}
```

A variable declared inside `{ }` is destroyed at the closing brace. Its memory is reclaimed immediately. This is deterministic — it happens at a point you can identify in the source, not whenever a garbage collector decides.

Python's function-level scoping has nothing this fine-grained, and no destruction point at all.

## Loop scope

```cpp
for (int i = 0; i < 10; ++i) {
    int temp = i * 2;
}
// neither i nor temp exists here
```

In Python, `i` survives the loop. In C++ it does not.

## Shadowing

```cpp
int x = 1;
{
    int x = 2;   // shadows the outer x
    // here x is 2
}
// here x is 1 again
```

Legal, and a good way to confuse yourself. `-Wshadow` warns about it.

## Lifetime and storage

Three storage durations matter now:

| Duration | Where | Lifetime |
| --- | --- | --- |
| **Automatic** | Stack | Created at declaration, destroyed at the closing brace |
| **Static** | Static storage | Entire program run |
| **Dynamic** | Heap | From `new` until `delete` — **manual** |

Everything in this module is automatic. Dynamic is Module 06, and it is where the interesting failures live.

## The dangerous case

```cpp
int* make_pointer() {
    int x = 42;
    return &x;      // returning the address of a local
}                   // x is destroyed HERE
```

The returned address points at memory that no longer belongs to anything. Reading through it might appear to work, might print garbage, might crash. That is a **dangling pointer**, and it is the first genuinely dangerous idea in this phase.

It has no Python analogue. Python keeps an object alive as long as any reference exists. C++ destroys the object and leaves the pointer holding a stale address, with no mechanism to notice.

You cannot write this bug yet — pointers are Module 04. But the *cause* is here: a name went out of scope and its memory was reclaimed.

## Uninitialized variables

```cpp
int x;                    // garbage
std::cout << x << "\n";   // undefined behaviour
```

The memory was reserved but nothing was written to it, so it holds whatever the previous occupant left. The value may be different at `-O0` and `-O2`, and the compiler is permitted to assume this never happens — which means it can optimize on that assumption and produce genuinely bizarre behaviour.

**Initialize everything.** Not a style preference.

## Why This Matters for CUDA

Lifetime becomes multi-dimensional on a GPU. Memory can be:

- a **register**, private to one thread, alive for the kernel's duration
- **shared memory**, visible to a block of threads, alive while the block runs
- **global memory**, allocated by the host, alive until explicitly freed

Every one of those is a scope-and-lifetime question, and getting them wrong produces the GPU version of the dangling pointer — reading memory that a block no longer owns. The mental discipline of asking *"where does this live and when does it die?"* is being built here, on a case simple enough to reason about completely.