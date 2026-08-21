# Module 07 Exercises

**Most of these files are deliberately broken.** That is the point.

## Method

For each exercise, in this order:

1. **Read the file. Predict the error CATEGORY before compiling** — compile error, linker error, or run-time error. Write the prediction down.
2. Compile with `g++ -std=c++17 -Wall -Wextra -g -O0`.
3. Read the actual message. Record it verbatim in the file.
4. Classify it. Were you right?
5. Fix it.
6. Recompile and confirm.

Step 1 is the exercise. Skipping to step 2 turns this into typing.

## The three categories

| Category | Recognize by |
| --- | --- |
| **Compile** | A line number in your source, the word `error:` |
| **Link** | No line number, mangled names, `ld` or `collect2` |
| **Run-time** | The program started, then failed or printed nonsense |

## Files

| File | Category to predict |
| --- | --- |
| `exercise-01.cpp` | Syntax errors |
| `exercise-02.cpp` | Type errors |
| `exercise-03.cpp` | Warnings that are really bugs |
| `exercise-04a.cpp` + `exercise-04b.cpp` | Linker |
| `exercise-05.cpp` + `bad_header.h` | Header problems |
| `exercise-06.cpp` | Run-time |
| `exercise-07.md` | Optimization flags — an investigation, not a broken file |