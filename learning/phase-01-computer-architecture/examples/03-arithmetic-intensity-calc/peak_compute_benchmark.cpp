// Mede o pico de GFLOP/s REAL da CPU, para uso como teto de calculo no
// roofline (arquivo de teoria 09) -- diferente de vector_add ou matmul
// ingenuo (que sao memory-bound e portanto NUNCA revelam o pico de
// calculo real, so o teto de memoria).
//
// A tecnica: manter os dados em variaveis locais (idealmente alocadas em
// registrador pelo compilador), fazendo milhoes de operacoes de
// multiplicacao-e-soma (FMA) SEM tocar memoria dentro do loop quente --
// isso isola o benchmark do gargalo de memoria, medindo so o que a ULA
// consegue fazer.
//
// Usa MULTIPLOS acumuladores independentes (ver arquivo de teoria 02,
// ILP) -- um acumulador so criaria uma cadeia de dependencia que nao
// deixaria o hardware superescalar/pipelined atingir o pico real.
//
// Compilar com -O2 (nunca -O0 -- em -O0 isso mede o interpretador de
// bytecode do compilador, nao o hardware).

#include <iostream>
#include <chrono>
#include <iomanip>

int main() {
    const long long iters = 2'000'000'000LL;

    // 8 acumuladores independentes: quebra a cadeia de dependencia que um
    // unico acumulador criaria, permitindo ao hardware superescalar (e a
    // eventual auto-vetorizacao SIMD) manter multiplas unidades de FMA
    // ocupadas ao mesmo tempo.
    float a0 = 1.0f, a1 = 1.0f, a2 = 1.0f, a3 = 1.0f;
    float a4 = 1.0f, a5 = 1.0f, a6 = 1.0f, a7 = 1.0f;
    const float k = 1.0000001f;  // fator perto de 1, evita overflow/inf ao longo do loop

    auto start = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < iters; i++) {
        a0 = a0 * k + 1.0f;
        a1 = a1 * k + 1.0f;
        a2 = a2 * k + 1.0f;
        a3 = a3 * k + 1.0f;
        a4 = a4 * k + 1.0f;
        a5 = a5 * k + 1.0f;
        a6 = a6 * k + 1.0f;
        a7 = a7 * k + 1.0f;
    }
    auto end = std::chrono::high_resolution_clock::now();

    double seconds = std::chrono::duration<double>(end - start).count();

    // Cada iteracao: 8 acumuladores x 2 FLOPs cada (1 mult + 1 add, se o
    // compilador nao fundir em FMA; se fundir em FMA, ainda contamos 2
    // FLOPs por convencao -- e o padrao da industria para essa contagem).
    double total_flops = static_cast<double>(iters) * 8.0 * 2.0;
    double gflops = (total_flops / seconds) / 1e9;

    std::cout << "Tempo:            " << std::fixed << std::setprecision(4) << seconds << " s\n";
    std::cout << "Pico de calculo medido: " << std::fixed << std::setprecision(2) << gflops << " GFLOP/s\n";
    std::cout << "\nEste numero vai em PEAK_GFLOPS no examples/04-roofline-plot/roofline.py\n";

    // Impede o compilador de eliminar o loop inteiro (dead code elimination)
    // por nunca usar o resultado.
    volatile float sink = a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7;
    (void)sink;

    return 0;
}
