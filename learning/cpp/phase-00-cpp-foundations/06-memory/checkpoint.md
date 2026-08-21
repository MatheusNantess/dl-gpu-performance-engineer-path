# Checkpoint — Module 06: Memory

## 📘 Theory

- [ ] I can explain what an address is and why they change between runs
- [ ] I can explain why the difference between two addresses is meaningful and the absolute value is not
- [ ] I can describe the stack and heap: allocation cost, lifetime, size limit, and when to use each
- [ ] I can explain what a leak is, and why the memory cannot be recovered afterwards
- [ ] I can explain what use-after-free does and why it often appears to work
- [ ] I can explain why `new[]` must be paired with `delete[]`
- [ ] I can explain what a cache line is and why it makes sequential access fast
- [ ] I can explain the difference between memory-bound and compute-bound
- [ ] I can explain why GPU "local memory" is not fast, despite the name

## 💻 Exercises

- [ ] Completed EASY exercises (01–02)
- [ ] Completed MEDIUM exercises (03–05)
- [ ] Completed HARD exercise (06)
- [ ] Ran every allocation exercise under `-fsanitize=address` or valgrind
- [ ] Produced, and then fixed, a leak, a use-after-free and a double free
- [ ] Found this machine's actual stack limit empirically

## 🔬 Mastery

- [ ] For any variable, I can state where it lives, how long it lives, and who frees it
- [ ] Given code with a memory bug, I can find it by inspection before the sanitizer confirms it
- [ ] I can write a correct allocate/use/free cycle from an empty file
- [ ] I can predict which of two access patterns is faster, and by roughly how much
- [ ] I can explain to someone else why data movement dominates GPU performance
- [ ] I can apply this somewhere new: given a kernel description, say which data is read once and which is reused

## Measured results — exercise 06

The first cache-locality measurement in this lab. Predictions first, then actuals:

| Access pattern | Predicted ratio | Measured ratio (64 MB array) | Measured ratio (small array) |
| --- | --- | --- | --- |
| Sequential | 1.0× (baseline) | 1.0× | 1.0× |
| Stride 16 | | | |
| Random | | | |

Why do the ratios shrink when the array fits in cache?

## Ready for Module 07 when

- [ ] All three sections honestly ticked
- [ ] I can write, from an empty file, a program that allocates on the heap, uses the memory, and frees it with no sanitizer complaints