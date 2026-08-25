# Exemplo 06 — Bandwidth, Latência e Little's Law na sua própria GPU

> Pré-requisito: [05-latency-vs-bandwidth.md](../../theory/05-latency-vs-bandwidth.md)

Os exemplos 01 e 02 mediram latência e bandwidth da **CPU/RAM**. Este exemplo aplica a
mesma disciplina — nunca usar número de datasheet como se fosse medido — à sua **RTX
3060**, para calcular a concorrência necessária (Little's Law) no hardware que o resto do
lab vai usar.

O número teórico já registrado em [docs/hardware/rtx-3060.md](../../../../docs/hardware/rtx-3060.md)
(360 GB/s, calculado do barramento × clock) é só o **teto de datasheet**. Aqui você mede o
real: a bandwidth sustentada e a latência de um acesso isolado.

## Passo 1 — Bandwidth (roda agora, sem compilar nada)

Ambiente já tem PyTorch+CUDA no WSL ([docs/hardware/rtx-3060.md](../../../../docs/hardware/rtx-3060.md)).

```bash
wsl -d Ubuntu-24.04
cd /mnt/d/deep-learning-lab/learning/phase-01-computer-architecture/examples/06-gpu-bandwidth-latency
python3 bandwidth_gpu.py
```

STREAM Triad (`c = a + scalar*b`) em tensores de 512 MB cada — bem maiores que os 2.25 MB
de L2 da RTX 3060, então os acessos batem em VRAM de verdade, não em cache. Anote o
"Bandwidth sustentável medida" em GB/s.

## Passo 2 — Latência (precisa compilar com nvcc)

```bash
cd /mnt/d/deep-learning-lab/learning/phase-01-computer-architecture/examples/06-gpu-bandwidth-latency
nvcc -O3 -arch=sm_86 -o latency_gpu latency_gpu.cu
./latency_gpu
```

Pointer chasing com **uma única thread** (`<<<1, 1>>>`), de propósito — o objetivo é medir
a latência de UM acesso isolado, sem deixar o hardware esconder isso trocando entre warps
(latency hiding, arquivo de teoria 05, seção 9). Anote o `ns/acesso` do **maior** buffer
(262144 KB = 256 MB) — esse é o que representa VRAM de verdade, análogo ao platô mais alto
do `01-latency-benchmark` de CPU.

## Passo 3 — Little's Law com seus próprios números

Abra `little_law_gpu.py`, preencha `BANDWIDTH_GB_S` e `LATENCY_NS` com o que você acabou
de medir, e rode:

```bash
python3 little_law_gpu.py
```

Isso imprime quantas requisições de memória a RTX 3060 precisa manter **em voo
simultaneamente** para saturar a bandwidth medida — a mesma conta da seção 7 do arquivo de
teoria 05, agora com hardware real.

## Por que "bytes por requisição" é diferente aqui do exemplo de CPU

No arquivo de teoria (seção 7), a conta de CPU usa 64 bytes — o tamanho de uma linha de
cache completa. Na Ampere (GA10x), o L2 busca em granularidade de **setor**, 32 bytes, não
a linha inteira de 128 bytes — por isso `little_law_gpu.py` usa 32, não 64 nem 128. É o
mesmo princípio (o hardware busca um bloco fixo por transação, não só os bytes que o
programa "pediu"), só que a unidade física é outra.

## A interpretação que importa (não só o número)

Depois de calcular a concorrência necessária, compare com quantas threads a RTX 3060
consegue ter em voo ao mesmo tempo: **28 SMs × 1536 threads/SM = 43.008 threads**
([docs/hardware/rtx-3060.md](../../../../docs/hardware/rtx-3060.md)). Isso é ordens de
magnitude maior que a concorrência que uma CPU de poucos núcleos consegue oferecer — é
exatamente por isso que uma GPU tolera latência de memória muito pior por acesso (algumas
centenas de ciclos) do que uma CPU: ela compensa com paralelismo de requisição em escala
muito maior, não com acessos individuais mais rápidos.

Essa é a ponte direta para **occupancy** (Phase 2): ter threads suficientes disponíveis é
o que permite essa concorrência existir de verdade — mas, como o arquivo de teoria já
avisa, occupancy alta não garante sozinha que a bandwidth teórica seja atingida se o
padrão de acesso dentro de cada thread for ruim.

## Onde registrar

Preencha os números medidos (bandwidth, latência no maior buffer, concorrência calculada)
no `checkpoint.md` do módulo, e também na seção "Measured peaks" de
[docs/hardware/rtx-3060.md](../../../../docs/hardware/rtx-3060.md) — isso desbloqueia o
critério de conclusão da Fase 2 do [ROADMAP.md](../../../../ROADMAP.md) ("achievable
bandwidth for my GPU, compared against the spec figure").
