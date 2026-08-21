// main.cpp -- uses math_utils, includes only its declarations.

#include <iostream>
#include "math_utils.h"

int main() {
    std::cout << "square(5) = " << square(5) << "\n";
    std::cout << "cube(3)   = " << cube(3)   << "\n";

    float data[5] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};

    std::cout << "average   = " << average(data, 5) << "\n";

    scale(data, 5, 2.0f);
    std::cout << "after scale by 2: ";
    for (int i = 0; i < 5; ++i) std::cout << data[i] << " ";
    std::cout << "\n";

    std::cout << "average   = " << average(data, 5) << "\n";

    // This file compiled successfully knowing only the DECLARATIONS from
    // the header. It has never seen the body of average() or scale().
    // The linker resolved those symbols against math_utils.o.
    //
    // Remove math_utils.o from the link command and you get an
    // "undefined reference" -- a LINKER error, with no line number in
    // this file, because nothing here is wrong.

    return 0;
}