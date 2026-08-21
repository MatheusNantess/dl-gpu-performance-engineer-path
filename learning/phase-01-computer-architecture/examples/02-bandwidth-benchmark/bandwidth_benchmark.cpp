// STREAM Triad benchmark: c[i] = a[i] + scalar * b[i]
//
// Mede bandwidth sustentavel de memoria, nao velocidade de calculo -- os
// arrays sao propositalmente maiores que a cache L3, entao os acessos
// batem em RAM de verdade. AI dessa operacao e baixa (2 FLOPs, 12 bytes
// por elemento, ver arquivo 07), entao o gargalo e garantidamente
// memoria, nao calculo -- exatamente o que queremos pra medir bandwidth.
//
// Compilar com -O2 (nunca -O0).

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

int main() {
    // 64 milhoes de floats por array = 256 MB por array, bem maior que
    // qualquer L3 de CPU consumer (tipicamente 8-32 MB).
    const size_t n = 64ULL * 1024 * 1024;
    const int repeats = 20;

    std::vector<float> a(n, 1.0f);
    std::vector<float> b(n, 2.0f);
    std::vector<float> c(n, 0.0f);

    const float scalar = 3.0f;

    // Aquecimento: primeira passada paga custo de alocacao de pagina
    // (o sistema operacional so mapeia memoria fisica na primeira
    // escrita -- "page fault" -- e isso nao e o que queremos medir).
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + scalar * b[i];
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < repeats; r++) {
        for (size_t i = 0; i < n; i++) {
            c[i] = a[i] + scalar * b[i];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    double seconds = std::chrono::duration<double>(end - start).count();

    // Bytes movidos por repeticao: le a[i] (4 bytes) + le b[i] (4 bytes)
    // + escreve c[i] (4 bytes) = 12 bytes por elemento.
    double bytes_per_repeat = static_cast<double>(n) * 12.0;
    double total_bytes = bytes_per_repeat * repeats;
    double gb_per_sec = (total_bytes / seconds) / 1e9;

    std::cout << "Elementos por array: " << n << " (" << (n * sizeof(float)) / (1024 * 1024) << " MB por array)\n";
    std::cout << "Repeticoes: " << repeats << "\n";
    std::cout << "Tempo total: " << std::fixed << std::setprecision(3) << seconds << " s\n";
    std::cout << "Bandwidth sustentavel medida: " << std::fixed << std::setprecision(2) << gb_per_sec << " GB/s\n";

    // Impede o compilador de eliminar o loop inteiro por nunca usar `c`.
    volatile float check = c[n / 2];
    (void)check;

    return 0;
}
