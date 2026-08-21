// Mede GFLOP/s e GB/s alcancados de verdade para vector_add e matmul
// ingenuo, e calcula a razao entre eles -- deve bater aproximadamente
// com a arithmetic intensity calculada na mao no README deste exemplo
// e no arquivo de teoria 07.
//
// Compilar com -O2 (nunca -O0).

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

struct Result {
    double seconds;
    double flops;
    double bytes;
};

void print_result(const std::string& nome, const Result& r) {
    double gflops = (r.flops / r.seconds) / 1e9;
    double gbytes = (r.bytes / r.seconds) / 1e9;
    // ATENCAO: isso e FLOPs_contados_na_mao / bytes_contados_na_mao -- o mesmo
    // tempo medido cancela na razao, entao isto e uma tautologia algebrica,
    // NAO uma confirmacao empirica independente de trafego real de hardware.
    // Ver README.md deste exemplo e o arquivo de teoria 07 (secao 6) para a
    // distincao entre AI algoritmica (isto aqui) e AI de hardware (medida
    // com performance counters, ex: Nsight Compute em GPU).
    double ai_algoritmica = r.flops / r.bytes;

    std::cout << "\n--- " << nome << " ---\n";
    std::cout << "Tempo:                    " << std::fixed << std::setprecision(4) << r.seconds << " s\n";
    std::cout << "GFLOP/s alcancado (real): " << std::fixed << std::setprecision(2) << gflops << "\n";
    std::cout << "GB/s alcancado (real):    " << std::fixed << std::setprecision(2) << gbytes << "\n";
    std::cout << "AI algoritmica (nao e medicao de hardware, ver README): "
              << std::fixed << std::setprecision(4) << ai_algoritmica << "\n";
}

Result run_vector_add(size_t n, int repeats) {
    std::vector<float> a(n, 1.0f), b(n, 2.0f), out(n, 0.0f);

    // aquecimento
    for (size_t i = 0; i < n; i++) out[i] = a[i] + b[i];

    auto start = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < repeats; r++) {
        for (size_t i = 0; i < n; i++) {
            out[i] = a[i] + b[i];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    volatile float check = out[n / 2];
    (void)check;

    double seconds = std::chrono::duration<double>(end - start).count();
    double flops = static_cast<double>(n) * 1.0 * repeats;       // 1 FLOP por elemento
    double bytes = static_cast<double>(n) * 12.0 * repeats;      // 12 bytes por elemento
    return {seconds, flops, bytes};
}

Result run_matmul_naive(int N, int repeats) {
    std::vector<float> A(N * N, 1.0f), B(N * N, 1.0f), C(N * N, 0.0f);

    auto matmul = [&]() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                float acc = 0.0f;
                for (int k = 0; k < N; k++) {
                    acc += A[i * N + k] * B[k * N + j];
                }
                C[i * N + j] = acc;
            }
        }
    };

    matmul();  // aquecimento

    auto start = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < repeats; r++) {
        matmul();
    }
    auto end = std::chrono::high_resolution_clock::now();

    volatile float check = C[0];
    (void)check;

    double seconds = std::chrono::duration<double>(end - start).count();
    double n3 = static_cast<double>(N) * N * N;
    double flops = 2.0 * n3 * repeats;         // N multiplicacoes + N somas, por elemento de C
    // Contagem ingenua de bytes: cada elemento de C le uma linha de A e
    // uma coluna de B do zero (pior caso, sem contar reuso de cache).
    double bytes = 2.0 * n3 * sizeof(float) * repeats;
    return {seconds, flops, bytes};
}

int main() {
    std::cout << "=== vector_add ===";
    Result va = run_vector_add(64ULL * 1024 * 1024, 20);
    print_result("vector_add (n=64M, 20 repeticoes)", va);

    std::cout << "\n=== matmul ingenuo ===";
    Result mm = run_matmul_naive(512, 3);
    print_result("matmul ingenuo (N=512, 3 repeticoes)", mm);

    std::cout << "\nO GFLOP/s e GB/s acima sao medicoes reais de tempo de parede.\n";
    std::cout << "A 'AI algoritmica' e uma tautologia com o calculo na mao (ver README) --\n";
    std::cout << "util para confirmar que a contagem de FLOPs/bytes no codigo nao tem bug,\n";
    std::cout << "mas NAO prova trafego real de memoria. Isso exigiria performance counters\n";
    std::cout << "de hardware (perf stat, ou Nsight Compute em GPU).\n";
    return 0;
}
