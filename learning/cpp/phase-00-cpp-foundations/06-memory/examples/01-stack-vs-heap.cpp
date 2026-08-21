/*
Example 01 — Stack, heap, and lifetime

Complete, working program. Predict the address RELATIONSHIPS before running.
Absolute values change every run; that is ASLR, and it is expected.

    g++ -std=c++17 -Wall -Wextra -g -O0 -o 01-stack-vs-heap 01-stack-vs-heap.cpp
    ./01-stack-vs-heap

Then run it again with the sanitizer and confirm it is clean:

    g++ -std=c++17 -Wall -Wextra -g -O0 -fsanitize=address -o 01-san 01-stack-vs-heap.cpp
    ./01-san
*/

#include <iostream>

void show_frame_addresses(int depth) {
    int local = depth;
    std::cout << "  depth " << depth << ": local at " << static_cast<const void*>(&local) << "\n";
    if (depth < 3) show_frame_addresses(depth + 1);
}

int main() {
    std::cout << "--- stack ---\n";
    int a = 1;
    int b = 2;
    int arr[4] = {0,0,0,0};
    std::cout << "  a       " << static_cast<const void*>(&a) << "\n";
    std::cout << "  b       " << static_cast<const void*>(&b) << "\n";
    std::cout << "  arr     " << static_cast<const void*>(arr) << "\n";

    std::cout << "\n--- each call gets its own frame ---\n";
    show_frame_addresses(1);
    // Note the direction the addresses move as the stack deepens.

    std::cout << "\n--- heap ---\n";
    int* h1 = new int[4]();     // () means zero-initialized
    int* h2 = new int[4]();
    std::cout << "  h1      " << static_cast<const void*>(h1) << "\n";
    std::cout << "  h2      " << static_cast<const void*>(h2) << "\n";
    std::cout << "  gap between h1 and h2: "
              << reinterpret_cast<const char*>(h2) - reinterpret_cast<const char*>(h1)
              << " bytes (requested 16)\n";

    // The gap is usually larger than requested: the allocator keeps its own
    // bookkeeping alongside each block. This overhead is one reason PyTorch
    // has a caching allocator instead of calling cudaMalloc per tensor.

    std::cout << "\n--- how far apart are the regions? ---\n";
    long long distance = reinterpret_cast<const char*>(h1) - reinterpret_cast<const char*>(&a);
    std::cout << "  |stack - heap| is on the order of "
              << (distance < 0 ? -distance : distance) / (1024 * 1024) << " MB\n";

    std::cout << "\n--- heap contents survive the scope that created them ---\n";
    {
        int* inner = new int[4];
        for (int i = 0; i < 4; ++i) inner[i] = i * 10;
        h1[0] = inner[3];        // copy a value out before losing the pointer
        delete[] inner;          // free it HERE; otherwise the pointer is lost
        inner = nullptr;         // defensive: a later use now crashes loudly
    }
    std::cout << "  value carried out of the inner scope: " << h1[0] << "\n";

    // A stack array declared in that block would already be gone. The heap
    // array had to be freed explicitly -- and had we not, the pointer would
    // have gone out of scope with the memory still allocated. That is a leak:
    // unreachable and unreleasable for the rest of the process.

    std::cout << "\n--- uninitialized versus zero-initialized ---\n";
    int* raw  = new int[4];      // garbage
    int* zero = new int[4]();    // zeros
    std::cout << "  new int[4]   first element: " << raw[0]  << "  (undefined -- may vary)\n";
    std::cout << "  new int[4]() first element: " << zero[0] << "\n";

    delete[] raw;   raw  = nullptr;
    delete[] zero;  zero = nullptr;
    delete[] h1;    h1   = nullptr;
    delete[] h2;    h2   = nullptr;

    // Every new[] paired with exactly one delete[]. cudaMalloc/cudaFree works
    // the same way, on a device with far less memory and no sanitizer running
    // by default.

    return 0;
}