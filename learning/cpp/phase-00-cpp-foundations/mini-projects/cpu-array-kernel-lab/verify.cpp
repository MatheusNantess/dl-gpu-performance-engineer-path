// Independent reference implementations, used to check kernels.cpp.
//
// Write these DIFFERENTLY from the versions in kernels.cpp -- a different
// loop structure, a different accumulation order, a double accumulator.
// A reference that repeats the same mistake verifies nothing.
//
// This is the same principle as the "correct baseline" stage in
// KERNEL_WORKFLOW.md: deliberately slow, obviously correct.

// TODO: declare these in a header of your own, or extern them from main.cpp.

// TODO: implement reference versions of all six operations.

// TODO: write a comparison helper for floating-point results:
//
//   bool close_enough(float a, float b, float tolerance);
//
// Then decide what tolerance each operation needs, and WRITE DOWN WHY.
// Exact equality will not work for the reductions -- summing in a different
// order gives a different result. Deciding what counts as "the same answer"
// is a real part of numerical kernel work, not a formality.