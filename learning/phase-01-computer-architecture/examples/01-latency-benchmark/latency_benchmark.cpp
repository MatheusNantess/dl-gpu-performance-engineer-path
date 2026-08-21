// Mede latencia real de acesso a memoria usando pointer chasing.
// Cada elemento do buffer guarda o indice do PROXIMO elemento a visitar,
// formando uma cadeia de dependencia real -- o hardware nao consegue
// prever o proximo endereco antes de ler o atual, entao nao ha como
// esconder a latencia com prefetch ou sobreposicao de requisicoes.
//
// Compilar com -O2 (nunca -O0, ver Modulo 07 do Phase 0).

#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <random>

// Cria uma cadeia de pointer-chasing dentro de um buffer de N ints, usando
// o algoritmo de Sattolo: gera uma permutacao ALEATORIA que forma um unico
// ciclo (visita todos os N indices, sem repetir nenhum, antes de voltar ao
// inicio). A aleatoriedade e essencial -- um stride fixo e previsivel
// demais, e o prefetcher de hardware da CPU consegue antecipar o proximo
// endereco e esconder a latencia, o que anularia o proposito do benchmark.
static std::vector<int> build_chase_buffer(size_t n) {
    std::vector<size_t> perm(n);
    for (size_t i = 0; i < n; i++) perm[i] = i;

    std::mt19937_64 rng(12345);  // seed fixa: resultados reproduziveis entre execucoes
    for (size_t i = n - 1; i >= 1; i--) {
        std::uniform_int_distribution<size_t> dist(0, i - 1);
        size_t j = dist(rng);
        std::swap(perm[i], perm[j]);
    }

    // perm agora e uma permutacao aleatoria de ciclo unico: seguir
    // idx -> perm[idx] repetidamente visita todos os N indices antes de
    // repetir qualquer um.
    std::vector<int> buf(n);
    for (size_t i = 0; i < n; i++) buf[i] = static_cast<int>(perm[i]);
    return buf;
}

// Percorre a cadeia `iters` vezes, retornando o tempo total em nanossegundos.
// O resultado da soma e usado para impedir o compilador de eliminar o loop
// inteiro por otimizacao agressiva (dead code elimination).
static double chase_and_time(const std::vector<int>& buf, size_t iters, long long& sink) {
    size_t idx = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < iters; i++) {
        idx = static_cast<size_t>(buf[idx]);
        sink += idx;
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::nano>(end - start).count();
}

int main() {
    // Tamanhos de buffer em KB, cobrindo de "menor que L1" ate "maior que L3".
    const size_t sizes_kb[] = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};

    std::cout << "Tamanho (KB)\tLatencia media (ns/acesso)\n";
    std::cout << "------------\t--------------------------\n";

    long long sink = 0;

    for (size_t kb : sizes_kb) {
        size_t n = (kb * 1024) / sizeof(int);
        auto buf = build_chase_buffer(n);

        // Aquecimento: garante que o buffer esta "resolvido" na hierarquia
        // de memoria antes de medir (primeira passada paga custo de miss
        // frio que nao e o que queremos medir aqui).
        chase_and_time(buf, n, sink);

        // Medicao real: repete varias vezes a cadeia inteira para ter
        // uma amostra estatisticamente razoavel.
        size_t iters = n * 20;
        double total_ns = chase_and_time(buf, iters, sink);
        double ns_per_access = total_ns / static_cast<double>(iters);

        std::cout << std::setw(10) << kb << "\t"
                  << std::fixed << std::setprecision(3) << ns_per_access << "\n";
    }

    // Impede o compilador de otimizar tudo embora por nunca usar `sink`.
    if (sink == -1) std::cout << "unreachable\n";

    return 0;
}
