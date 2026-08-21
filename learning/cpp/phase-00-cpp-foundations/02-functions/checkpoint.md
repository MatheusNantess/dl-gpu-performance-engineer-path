# Checkpoint — Module 02: Functions

## 📘 Theory

- [ ] I can explain the difference between a declaration and a definition, and why both exist
- [ ] I can explain what a stack frame is and what it contains
- [ ] I can explain why returning the address of a local variable is a bug
- [ ] I can explain why a large local array crashes the program
- [ ] I can explain, for any parameter, whether the function receives a copy or access to the original
- [ ] I can explain what `const float* data` promises and who enforces it
- [ ] I can explain why passing a pointer still involves a copy, and what is copied
- [ ] I can explain why `__global__` functions must return `void`

## 💻 Exercises

- [ ] Completed EASY exercises (01–03)
- [ ] Completed MEDIUM exercises (04–06)
- [ ] Completed HARD exercises (07–08)
- [ ] For each exercise, answered "will this change outside the function?" **before** compiling
- [ ] Recorded every wrong prediction and its cause

## 🔬 Mastery

- [ ] Given an unfamiliar function signature, I can state what is copied and what is shared, without running anything
- [ ] I can write a function that modifies a caller's variable, from memory
- [ ] Given a function that "does nothing", I can identify pass-by-value as the cause
- [ ] I can explain why `void scale(float* data, int n, float factor)` uses a pointer for one parameter and values for the others
- [ ] I can predict which of two signatures moves more bytes
- [ ] I can apply this to something new: design the signature for a function that takes two input arrays and fills a third

## Prediction record

| Exercise | Predicted correctly? | What I got wrong |
| --- | --- | --- |
| 04 | | |
| 05 | | |
| 07 | | |
| 08 | | |

## Ready for Module 03 when

- [ ] All three sections honestly ticked
- [ ] I can read `void f(const float* a, float* b, int n)` and describe, unprompted, exactly what the function can and cannot do