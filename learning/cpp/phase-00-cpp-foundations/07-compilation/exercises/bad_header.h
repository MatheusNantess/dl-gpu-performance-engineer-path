// bad_header.h -- deliberately broken. See exercise-05.cpp.
//
// TWO problems here:
//   1. No include guard.
//   2. A function DEFINITION in a header, rather than a declaration.
//
// Both are fine until the header is included more than once, or from more
// than one translation unit.

int triple(int x) {
    return x * 3;
}