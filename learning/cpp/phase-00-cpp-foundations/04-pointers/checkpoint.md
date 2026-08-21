# Checkpoint — Module 04: Pointers

**The most important checkpoint in Phase 0.** Do not move on with anything here unticked. Everything after this module assumes it, and CUDA is unreadable without it.

## 📘 Theory

- [ ] I can draw the memory picture for `int x = 10; int* p = &x;` from memory
- [ ] I can state what each of `x`, `&x`, `p`, `*p`, `&p` means, without pausing
- [ ] I can explain why a pointer has its own address
- [ ] I can explain why `p + 1` advances by `sizeof(T)` and not by one byte
- [ ] I can explain why `a[i]` and `*(a + i)` are the same expression
- [ ] I can explain what `p - q` returns, and in what units
- [ ] I can explain the difference between `const float* p` and `float* const p`
- [ ] I can explain why `cudaMalloc` takes `&d_ptr` rather than `d_ptr`
- [ ] I can name the three ways a pointer becomes invalid, and which is most dangerous
- [ ] I can explain why a program that does not segfault may still be wrong

## 💻 Exercises

- [ ] Completed EASY exercises (01–06)
- [ ] Completed MEDIUM exercises (07–14)
- [ ] Completed HARD exercises (15–20)
- [ ] Wrote a prediction **before compiling** on every exercise that asked for one
- [ ] Ran the memory-bug exercises under `-fsanitize=address` and read the reports
- [ ] Implemented at least three algorithms in both index form and pointer form

## 🔬 Mastery

- [ ] Given unfamiliar pointer code, I can predict the output before running it
- [ ] Given a program that segfaults, I can find the cause by inspection
- [ ] I can write array traversal in pointer form without looking anything up
- [ ] I can convert freely between `a[i]` and `*(a + i)` in either direction
- [ ] I can explain, to someone who does not know, why pointers exist at all
- [ ] I can look at `void scale(float* data, int n, float factor)` and state: what `data` holds, where the elements are, how `data[i]` is computed, what `data + i` means, how the function modifies the caller's data, and roughly how many bytes it touches
- [ ] I can apply this somewhere new: given a strided access pattern, compute which addresses are touched

## Prediction record

The most informative table in this phase. Track every prediction exercise:

| Exercise | Predicted correctly? | What I got wrong |
| --- | --- | --- |
| 05 | | |
| 07 | | |
| 09 | | |
| 11 | | |
| 13 | | |
| 15 | | |
| 17 | | |
| 19 | | |

Early misses are expected. If the miss rate is not falling by exercise 15, reread the theory rather than doing more exercises.

## The readiness question

Write out, in full sentences and without notes, what each part of this signature means:

```cpp
void scale(float* data, int n, float factor)
```

- [ ] I did this, in writing, and it was correct

This exact question reappears in [cuda-readiness-checkpoint.md](../cuda-readiness-checkpoint.md). It is the bar for the whole phase.

## Ready for Module 05 when

- [ ] All three sections honestly ticked
- [ ] I can write, from an empty file and without reference, a function that sums an array using pointer arithmetic only