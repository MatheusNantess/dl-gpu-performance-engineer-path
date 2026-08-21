# Checkpoint — Module 07: Compilation

## 📘 Theory

- [ ] I can name the four stages of the build and say what each does
- [ ] I can explain what the preprocessor does to `#include`, literally
- [ ] I can explain why a header guard is necessary
- [ ] I can explain why `main.cpp` compiles knowing only declarations
- [ ] I can explain the difference between a compile error and a linker error, and recognize each on sight
- [ ] I can explain why declaration/definition mismatch fails at link time and not compile time
- [ ] I can explain what `-Wall -Wextra -g -O0` each contribute
- [ ] I can explain why benchmarking at `-O0` is meaningless
- [ ] I can explain why `-Ofast` is a correctness decision

## 💻 Exercises

- [ ] Completed EASY exercises (01–02)
- [ ] Completed MEDIUM exercises (03–06)
- [ ] Completed HARD exercise (07)
- [ ] Predicted the error CATEGORY before compiling, every time
- [ ] Built the multi-file example by hand, stage by stage
- [ ] Caused both linker errors deliberately and read the messages

## 🔬 Mastery

- [ ] Given an error message I have not seen, I can classify its stage in seconds
- [ ] I can build a multi-file project from the command line without reference
- [ ] I can fix a linker error without guessing
- [ ] I can read a compiler warning and say what bug it points at
- [ ] I can inspect preprocessed output and assembly and know what I am looking at
- [ ] I can explain to someone else why C++ needs a separate link step at all

## Error message log

Build this as you go. It becomes a personal reference.

| Message (first line) | Stage | Cause | Fix |
| --- | --- | --- | --- |
| | | | |

## Optimization results — exercise 07

| Flag | Median time | Speedup vs -O0 | Result value |
| --- | --- | --- | --- |
| `-O0` | | 1.0× | |
| `-O2` | | | |
| `-O3` | | | |
| `-Ofast` | | | |

Did `-Ofast` change the numerical result?

## Ready for Module 08 when

- [ ] All three sections honestly ticked
- [ ] I can create a header, a source file and a main, and build them, from an empty directory