#include <iostream>
#include "kernels.h"

// Driver for the CPU array kernel lab.
//
// TODO: build this up in stages:
//
//   1. Allocate float arrays on the heap at a size given at run time.
//      Check for allocation failure.
//
//   2. Fill them with known values -- use a fixed seed, so runs are comparable.
//
//   3. Call each of the six operations.
//
//   4. Verify each against the reference implementations in verify.cpp,
//      with an explicit tolerance.
//
//   5. Time each one: warm up first, then several repetitions, report the
//      MEDIAN, not the mean. Use <chrono> steady_clock.
//
//   6. For each operation, print: time, bytes moved, and implied bandwidth
//      in GB/s.
//
//   7. Free everything. Verify with -fsanitize=address that there are no
//      leaks and no invalid accesses.
//
// Run it at several sizes -- try 1e4, 1e6, 1e8 elements -- and record the
// results in RESULTS.md.
//
// Note on step 5: build with -O2 for the timing runs. Timing at -O0 measures
// unoptimized code that nobody runs.
//
// Note on step 6: make sure the results are actually USED -- printed, or
// accumulated into something printed. At -O2 the compiler will delete a
// computation whose result is never read, producing an impossibly fast
// measurement. That trap is real, and avoiding it is part of the
// benchmarking methodology this whole repository runs on.

int main() {
    // TODO: implement
    return 0;
}