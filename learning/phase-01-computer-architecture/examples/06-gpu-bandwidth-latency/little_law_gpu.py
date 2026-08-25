# Aplica Little's Law (teoria: arquivo 05, secao 6) aos numeros MEDIDOS da
# sua propria RTX 3060 -- nao aos numeros de datasheet.
#
# Concorrencia necessaria = throughput desejado (requisicoes/s) x latencia
#                            por requisicao (s)
#
# "Bytes por requisicao": numa GPU Ampere, o L2 busca em granularidade de
# SETOR (32 bytes), nao a linha inteira de 128 bytes -- diferente da CPU
# (64 bytes por linha completa). Um acesso disperso e dependente, como o do
# latency_gpu.cu, aciona a busca de um unico setor de 32 bytes, mesmo lendo
# so 4 bytes (um int) dele. Ver theory/04-memory-hierarchy.md e a nota sobre
# sector caching >= Volta.
#
# Preencha os dois numeros abaixo com o que VOCE mediu, depois rode:
#   python little_law_gpu.py

BANDWIDTH_GB_S = 305.21  # medido em bandwidth_gpu.py, RTX 3060, 2026-08-23
LATENCY_NS = 289.748     # medido em latency_gpu.cu, buffer 262144 KB (VRAM), RTX 3060, 2026-08-23

BYTES_PER_REQUEST = 32  # tamanho de um setor de memoria na Ampere (GA10x)

if BANDWIDTH_GB_S is None or LATENCY_NS is None:
    raise SystemExit(
        "Preencha BANDWIDTH_GB_S e LATENCY_NS no topo deste arquivo com os "
        "numeros que voce mediu em bandwidth_gpu.py e latency_gpu.cu antes "
        "de rodar."
    )

bandwidth_bytes_s = BANDWIDTH_GB_S * 1e9
latency_s = LATENCY_NS * 1e-9

throughput_req_s = bandwidth_bytes_s / BYTES_PER_REQUEST
concurrency_needed = throughput_req_s * latency_s

print(f"Bandwidth medida:            {BANDWIDTH_GB_S:.2f} GB/s")
print(f"Latencia medida (maior buffer): {LATENCY_NS:.2f} ns/acesso")
print(f"Bytes por requisicao (setor Ampere): {BYTES_PER_REQUEST} bytes")
print()
print(f"Throughput desejado:  {throughput_req_s:,.0f} requisicoes/s")
print(f"Concorrencia necessaria (Little's Law): {concurrency_needed:,.0f} requisicoes em voo")
print()
print(
    "Compare com o numero de threads que sua RTX 3060 consegue ter em voo "
    "ao mesmo tempo (28 SMs x 1536 threads/SM = 43008, ver "
    "docs/hardware/rtx-3060.md) -- isso mostra por que a GPU precisa de "
    "MUITO mais paralelismo de requisicao que a CPU pra esconder a mesma "
    "latencia relativa, e por que occupancy alta existe."
)
