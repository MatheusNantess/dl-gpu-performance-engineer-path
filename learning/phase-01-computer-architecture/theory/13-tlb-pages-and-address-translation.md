# 13 — TLB, Páginas e Tradução de Endereço

> Pré-requisito: [04-memory-hierarchy.md](04-memory-hierarchy.md)
>
> Este arquivo é deliberadamente curto — o objetivo é só evitar um erro específico de
> diagnóstico: interpretar tudo que é lento como "latência de RAM", quando às vezes é
> outra coisa completamente diferente.

## 1. O endereço que seu código usa não é o endereço físico de verdade

Todo endereço que você já manipulou até agora neste módulo (`0x1000`, os endereços
impressos com `&x`) é um **endereço virtual** — um endereço dentro do espaço próprio do seu
processo, que o sistema operacional isola de outros processos por segurança e conveniência.
A memória física de verdade (os chips de RAM reais) usa endereços diferentes, e existe uma
etapa de **tradução** entre os dois, em todo acesso à memória, sempre — mesmo que você
nunca veja isso no código.

```text
Seu código pede:  endereço VIRTUAL 0x00007f3a...
                        |
                        v
                  TRADUÇÃO (o assunto deste arquivo)
                        |
                        v
Hardware acessa:  endereço FÍSICO real, na RAM
```

## 2. Páginas: a unidade de tradução

A tradução não acontece byte a byte — seria caro demais. A memória é dividida em blocos de
tamanho fixo chamados **páginas** (tipicamente 4 KB), e a tradução mapeia **página virtual
→ página física** inteira de uma vez, guardada numa estrutura chamada **page table**
(tabela de páginas), mantida pelo sistema operacional.

```text
Página virtual 5  ->  Página física 812
Página virtual 6  ->  Página física 47
Página virtual 7  ->  Página física 813
```

Repare: páginas virtuais **consecutivas** (5, 6, 7) podem mapear pra páginas físicas
**não-consecutivas** (812, 47, 813) — é assim que o sistema operacional consegue dar a cada
processo a ilusão de um espaço de memória contíguo e privado, mesmo com a RAM física
fragmentada e compartilhada entre vários programas.

## 3. TLB: a cache da tradução

Consultar a page table na memória, toda vez que você acessa qualquer endereço, seria um
desperdício enorme — seria, ironicamente, um acesso à memória extra só pra descobrir onde
está o dado que você realmente queria. A solução é o mesmo princípio que você já domina de
sobra: **cachear** as traduções mais usadas recentemente. Essa cache específica se chama
**TLB** (Translation Lookaside Buffer) — é literalmente uma cache, só que guarda "mapeamentos
página virtual → física" em vez de guardar dado.

```text
TLB hit:  tradução já está no TLB -> tradução quase instantânea, acesso segue normal
TLB miss: tradução não está no TLB -> precisa consultar a page table na memória
          ("page walk") -> custo extra real, antes mesmo de chegar no dado que você queria
```

## 4. Por que isso importa pro diagnóstico de performance

Aqui está a razão prática deste arquivo existir: se você está medindo um acesso à memória
que parece **anormalmente lento** — mais lento que o esperado pela latência de RAM que você
mediu no arquivo 04/exemplo 01 — a causa pode não ser "RAM lenta". Pode ser **TLB miss**
seguido de **page walk** (potencialmente vários acessos à memória só pra reconstruir a
tradução, antes de sequer chegar no dado). Working sets muito grandes, que tocam muitas
páginas diferentes espalhadas (o oposto de localidade, arquivo 04), aumentam a taxa de TLB
miss — o TLB, como qualquer cache, também é pequeno e finito.

**PERGUNTA RÁPIDA 1**: por que percorrer um array **gigante** (muito maior que a memória
coberta pelo TLB) com stride grande tende a sofrer tanto cache miss (arquivos 04/10) quanto
TLB miss, ao mesmo tempo?

<details>
<summary>Resposta</summary>

Stride grande já quebra localidade espacial pra cache normal (arquivo 04) — cada acesso cai
numa cache line diferente. Se o stride também é grande o suficiente pra cruzar fronteiras de
página (4 KB) com frequência, cada acesso também tende a cair numa página diferente,
esgotando rapidamente as poucas traduções que cabem no TLB — os dois problemas têm a mesma
causa raiz (padrão de acesso espalhado), só que afetam duas caches diferentes (cache de
dado, e cache de tradução de endereço) ao mesmo tempo. Isso é por que um profiler sério
reporta os dois tipos de miss separadamente — atribuir toda lentidão a "cache de dado" sem
checar TLB pode levar a um diagnóstico errado.

</details>

## 5. Huge pages: uma mitigação (mencionada, não desenvolvida aqui)

Uma forma de reduzir TLB miss em workloads com working set grande é usar **páginas maiores**
(huge pages — 2 MB ou 1 GB, em vez de 4 KB) — cada entrada do TLB cobre uma faixa de memória
muito maior, então o mesmo número de entradas do TLB cobre um working set muito maior antes
de esgotar. Isso é usado de verdade em workloads de deep learning com tensores grandes. Não
é assunto pra aprofundar agora — só vale saber que existe, e que aparece em benchmarks de
memória sérios (o material "Performance Ninja", por exemplo, inclui labs específicos sobre
isso na seção de otimizações memory-bound).

## 6. Por que isso importa para CUDA

GPUs também têm tradução de endereço (memória virtual de dispositivo) e suas próprias
estruturas análogas ao TLB — o princípio é o mesmo, aplicado num contexto onde o espaço de
endereço pode ser ainda maior (memória unificada entre CPU e GPU, em sistemas que suportam
isso). Você não precisa de profundidade nisso agora — só reconhecer, mais tarde, que "essa
transferência de memória está mais lenta que o esperado" pode ter uma causa nessa camada,
não só na banda de memória bruta.

## 7. Erros comuns

- Atribuir toda lentidão inesperada de acesso à memória a "cache miss de dado" sem
  considerar TLB — são duas caches diferentes, com sintomas parecidos (lentidão) mas causas
  e soluções diferentes.
- Achar que precisa estudar gerenciamento de memória virtual do sistema operacional a fundo
  (paginação, swap, copy-on-write) pra entender isso — não precisa, esse não é o objetivo
  aqui. O modelo mental de "páginas → page table → TLB → TLB miss → page walk" é suficiente.
- Confundir TLB com cache de dado comum — TLB guarda **traduções de endereço**, não guarda
  o conteúdo/dado em si.

## O modelo mental completo, numa frase

**Todo acesso à memória paga primeiro o custo de tradução de endereço (rápido se está no
TLB, caro se não está) e só depois o custo de buscar o dado em si (rápido se está em cache
de dado, caro se precisa ir até a RAM) — são duas camadas de cache independentes,
empilhadas, e um workload pode sofrer em uma, na outra, ou nas duas ao mesmo tempo.**
