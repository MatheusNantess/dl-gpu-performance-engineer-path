/*
Exercise 06 — AoS versus SoA, measured
Difficulty: HARD

Goal:
Measure the layout difference on the CPU. The last experiment of the phase.

Requirements:
- Define a Particle struct with six float fields (position and velocity).
- Build TWO representations of one million particles: an AoS array, and an SoA
  structure holding six separate float arrays.
- Verify both hold identical data.
- Time three operations on each: sum all x values; scale all velocities;
  update all positions using all six fields.
- Warm up, repeat, report medians.
- Record everything in checkpoint.md.

Questions before coding:
1. Predict which layout wins each of the three operations, BEFORE measuring.
2. For the x-sum, how many useful bytes per cache line does each layout give?
3. Which operation should favour AoS, and did your measurement agree?

Why this matters for CUDA:
This is one of the standard GPU layout decisions, and the penalty there is far steeper. NCHW versus NHWC is the same argument for image tensors. experiments/memory-access/ measures it on real hardware.
*/

#include <iostream>
#include <cstddef>

int main() {
    // TODO: implement
    return 0;
}