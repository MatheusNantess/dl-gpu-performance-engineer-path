/*
Challenge 03 — Array normalization

Combines: multi-pass algorithms, numerics, memory traffic

Goal:
The CPU form of the operation this repository's research is about.

Requirements:
- Given a float array, normalize it to zero mean and unit standard deviation.
- Implement it in three passes, then in two.
- Add a small epsilon to the denominator and explain why.
- Verify the output has mean approximately 0 and stddev approximately 1.
- Count the passes over the data in each version and the total bytes moved.
- Then implement RMS normalization -- divide by the root mean square, no mean
  subtraction -- and compare the pass count.

Questions before coding:
1. Can this be done in a single pass? What would that require, and what does it
   cost in accuracy?
2. Why is an epsilon needed at all?
3. How many bytes does each version move, and which is the theoretical minimum?

Byte accounting:
State, in a comment with your solution, how many bytes this reads and how
many it writes, for n elements. This is the habit that becomes roofline
analysis later.

Why this matters for CUDA:
This is LayerNorm and RMSNorm forward, without the learned parameters. They are kernels 04 and 05 in this repository and the subject of its research North Star. The pass-counting done here is exactly the analysis done there.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}