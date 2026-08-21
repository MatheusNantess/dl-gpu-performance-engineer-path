/*
Challenge 07 — Debug memory bugs

Combines: every failure mode from modules 04 and 06

Goal:
Find and fix bugs in code you did not write. The most valuable challenge here.

Requirements:
- The file below contains several deliberate memory bugs -- a leak, a
  use-after-free, an out-of-bounds access, a double free, an uninitialized read,
  and a dangling pointer.
- FIRST: find them by reading, and write down each one and its line.
- THEN: compile and run. Which crash? Which are silent?
- THEN: run under -fsanitize=address and -fsanitize=undefined. What did the
  tools find that you missed? What did you find that they missed?
- Fix everything and verify both sanitizers are clean.

(Write the buggy program yourself from the bug list above, or have someone
 else write it. Reading your own bugs is easier than reading a stranger''s --
 which is why the exercise is worth repeating with unfamiliar code.)

Questions before coding:
1. Which bugs produce no symptom at all on a normal run?
2. Which did the compiler warn about, if any?
3. Which would be hardest to find in a large program, and why?

Byte accounting:
State, in a comment with your solution, how many bytes this reads and how
many it writes, for n elements. This is the habit that becomes roofline
analysis later.

Why this matters for CUDA:
Every one of these has a device equivalent, with worse ergonomics: asynchronous errors, no sanitizer by default, and out-of-bounds writes that corrupt tensors and produce silently wrong training results.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}