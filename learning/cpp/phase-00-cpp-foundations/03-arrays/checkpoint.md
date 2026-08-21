# Checkpoint — Module 03: Arrays

## 📘 Theory

- [ ] I can explain what "contiguous" means in terms of addresses
- [ ] I can state the address of `a[i]` given the address of `a[0]`, `i`, and the type
- [ ] I can explain why `sizeof(arr)/sizeof(arr[0])` works where the array is declared and fails inside a function
- [ ] I can explain array decay and why arrays cannot be passed by value
- [ ] I can explain why every kernel signature carries an explicit `n`
- [ ] I can write `row * cols + col` without hesitating, and say why it is `cols` and not `rows`
- [ ] I can explain why row-major traversal is faster than column-major on a large matrix
- [ ] I can explain why a C array differs from a Python list in four specific ways
- [ ] I can explain what happens on an out-of-bounds access, and why nothing reports it

## 💻 Exercises

- [ ] Completed EASY exercises (01–05)
- [ ] Completed MEDIUM exercises (06–11)
- [ ] Completed HARD exercises (12–15)
- [ ] Compiled everything with `-Wall -Wextra`
- [ ] Actually measured the row-vs-column difference in exercise 14, rather than assuming it

## 🔬 Mastery

- [ ] Given an array declaration, I can draw its memory layout with addresses and strides
- [ ] I can compute the byte offset of any element of a 2D array in my head
- [ ] Given code that reads past the end of an array, I can spot it by inspection
- [ ] I can write array traversal without looking anything up
- [ ] I can explain, to someone who does not know, why contiguity makes GPUs fast
- [ ] I can apply this to something new: given a 3D tensor shape, write the flat index formula
- [ ] I can compute the theoretical minimum bytes moved by an operation over an array

## Measured results

Fill these in from exercise 14 — this is the first real measurement in the whole lab:

| Matrix size | Row-major time | Column-major time | Ratio |
| --- | --- | --- | --- |
| 512 × 512 | | | |
| 2048 × 2048 | | | |
| 4096 × 4096 | | | |

Does the ratio grow with size? Why?

## Ready for Module 04 when

- [ ] All three sections honestly ticked
- [ ] I can draw the memory layout of `float data[8]` from memory, with strides marked