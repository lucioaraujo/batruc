# Batruc — biblioteca de estudo e perguntas de implementação

Atualizado em 04/11/2026. A primeira seleção foi proposta pelo autor; a
pesquisa independente e a checagem da lista obtida no Gemini seguem abaixo.
Dados de autoria, título e escopo conferidos em páginas de editoras, autores
ou instituições; **os livros integrais ainda não foram lidos nesta etapa**.
As relações com o
Batruc abaixo são hipóteses de pesquisa, não resultados atribuídos às
obras. “Leitura online gratuita” não implica licença para incorporar texto,
figuras ou código ao instrumento. Nenhum texto ou código foi incorporado.

## Ritmo e composição generativa

### Referências visuais recebidas em 04/11/2026

- [@maxim_codex — Shapes of Sound](https://www.instagram.com/maxim_codex/):
  o autor enviou capturas locais de **Tribal Techno** e **Afro House**
  (`~/Pictures/Screenshot from 2026-11-04 19-44-56.png` e
  `~/Pictures/Screenshot from 2026-11-04 19-45-19.png`). As imagens
  mostram anéis/polígonos por parte sonora, BPM e compasso no centro, e uma
  grade curta de ataques por instrumento abaixo. Essa relação simultânea
  entre forma, canal e sequência é uma referência de uso para o Editor de
  Ciclos. O software usado para produzir os vídeos, sua disponibilidade e
  sua licença não foram identificados; não inferir que seja um app público.
- [@thel.ab67](https://www.instagram.com/thel.ab67/): o autor enviou quatro
  capturas locais em 04/11/2026, às 19:52–19:55. Os quadros apresentam pontos
  que parecem colidir e disparar notas, uma ordenação progressiva de eventos ao
  longo de 64 compassos, uma função exibida junto a um gesto de timbre e a
  passagem de taxas de pulso 4:5:6 para altura percebida. São ideias para
  investigar comportamento, forma musical e continuidade entre ritmo e timbre.
  As imagens isoladas não demonstram o algoritmo usado nem identificam o
  software, sua disponibilidade ou sua licença; a página não carregou para
  inspeção direta nesta sessão.

**Aplicação original no Batruc:** relacionar o polígono editável com uma
leitura temporal compacta dos mesmos ataques e com a voz correspondente no
Mixer. Manter a identidade gráfica aprovada para Batruc e preservar a
proveniência das referências; não copiar composição visual, presets ou código
dos perfis.

| Fonte | Leitura e experimento no Batruc |
| --- | --- |
| Godfried T. Toussaint, [*The Geometry of Musical Rhythm: What Makes a “Good” Rhythm Good?*, 2ª ed.](https://www.routledge.com/link/link/p/book/9780815370970), 2020 | Geometria e distâncias entre padrões; comparar distribuições de ataques, rotação, agrupamento e percepção, inclusive timing expressivo, sem fazer de “bom” uma regra universal. |
| David Cope, [*The Algorithmic Composer*](https://files.areditions.com/cmdas/DAS%20Flyer%201212.pdf), A-R Editions, 2000 | Construção de motivos, inferência, reconhecimento de padrões, regras e decisão humana; testar variação de célula até frase e peça com proveniência. |

### Processo, indeterminação e interação computacional

| Fonte primária | O que estudar para o Batruc |
| --- | --- |
| [John Cage Trust — autobiographical statement](https://johncage.org/cage-autobiographical-statement) | Contexto das operações de acaso em *Music of Changes*. Distinguir sorteio na composição de comportamento variável durante a execução; não converter acaso em sinônimo de sistema generativo. |
| [Steve Reich — *Piano Phase*](https://stevereich.com/composition/piano-phase/) | Processo de defasagem gradual entre padrões repetidos e retorno ao uníssono. Experimento possível: deslocar uma célula contra uma referência estável e visualizar/escutar os pontos de fase. |
| [IRCAM — George Lewis, *Voyager*](https://brahms.ircam.fr/fr/work/Voyager-1985-01-01) | Improvisação generativa interativa entre músico e sistema computacional. Perguntar que informação o sistema observa, como responde e que decisões continuam com o intérprete. |
| [TidalCycles — mini-notation](https://tidalcycles.org/docs/reference/mini_notation/) | Padrões cíclicos, subdivisões e relações polimétricas como comportamento executável. Usar para estudar ideias; não acrescentar Tidal como dependência do Batruc. |

### Sistemas dinâmicos e modelagem física

| Fonte primária | O que estudar para o Batruc |
| --- | --- |
| [Wolfram MathWorld — Logistic Map](https://mathworld.wolfram.com/LogisticMap.html) | Iteração `x[n+1]=r·x[n]·(1−x[n])`, dependência do parâmetro e transições entre regimes. Prototipar como modulador limitado de densidade/acentuação, mantendo estado e `r` explícitos; não chamar a sequência de ruído aleatório. |
| [IRCAM Forum — Modalys](https://forum.ircam.fr/projects/detail/modalys/) | Modelagem de objetos ressonantes (cordas, placas, tubos, membranas, massas e molas) e de suas interações. Referência de arquitetura e escuta; não é dependência escolhida para o Batruc. |
| [Acoustical Society of America — modos e figuras de Chladni](https://acoustics.org/pressroom/httpdocs/148th/bissinger.html) | Relação entre modos de vibração, amortecimento e padrões nodais. Usar para separar desenho de modo vibratório de simulação acústica efetiva. |

**Decisão de estudo:** começar por processos de eventos e relações entre vozes
que o core já pode calcular; em seguida testar um mapa dinâmico determinístico
com saída limitada. Modelagem física do timbre é uma frente separada e deve
começar com um ressonador simples, comparando áudio, estabilidade e CPU. A
auditoria de estado e o próximo experimento estão registrados em
[`TAREFAS.md`](TAREFAS.md); o contexto de referências e proveniência está no
[Atlas RASGO](../../RASGO_DOCUMENTATION/ATLAS_DE_REFERENCIA_RASGO(4).md).

## Síntese, materiais e áudio digital

| Fonte | Leitura e experimento no Batruc |
| --- | --- |
| Stefan Bilbao, [*Numerical Sound Synthesis: Finite Difference Schemes and Simulation in Musical Acoustics*](https://onlinelibrary.wiley.com/doi/book/10.1002/9780470749012), 2009 | Modelos discretos de oscilador, corda, barra, tubo e placa; ensaiar um modelo pequeno, medir estabilidade, custo e resposta sonora antes de ampliar a malha. |
| Andy Farnell, [*Designing Sound*](https://mitpress.mit.edu/9780262014410/designing-sound/), MIT Press, 2010 | Áudio procedural em Pure Data como estudo de gesto e matéria; criar mapeamentos próprios entre ação gráfica, excitação e timbre. |
| Julius O. Smith III, [*Introduction to Digital Filters: With Audio Applications*](https://www.dsprelated.com/freebooks/filters/), 2007, disponível para leitura online | Filtros, ressonância, fase e estabilidade; medir a resposta de um corpo ressonante e relacionar fórmula, gráfico e som. O livro de filtros não substitui um modelo físico completo. |
| Richard Boulanger e Victor Lazzarini (orgs.), [*The Audio Programming Book*](https://mitpress.mit.edu/9780262014465/the-audio-programming-book/), MIT Press, 2010 | Técnicas de C/C++, DSP, programação espectral e síntese algorítmica; estudar exemplos como referência, verificando licença de cada material antes de reutilizar código. |
| Charles L. Phillips, John Parr e Eve A. Riskin, [*Signals, Systems, & Transforms*, 5ª ed.](https://www.pearson.com/en-gb/subject-catalog/p/Phillips-Signals-Systems-Transforms-Global-Edition-5th-Edition/P200000005231?view=educator), Pearson, 2014 | Séries e transformadas de Fourier, Laplace, tempo discreto e transformada Z; fundamentar animação das ondas, análise espectral e filtros. A autoria varia conforme a edição; esta linha identifica a 5ª. |

## Tecnologias livres para síntese e experimentação

| Tecnologia e licença verificada | O que oferece | Uso possível no Batruc e limites |
| --- | --- | --- |
| [DaisySP](https://github.com/daisyaudio/DaisySP) — licença MIT no núcleo | Biblioteca C++ com osciladores (incluindo variantes PolyBLEP), ADSR, filtros, baterias sintéticas, modelagem física, ressonadores e granulação. O projeto declara módulos por amostra, memória estática e uso fora do hardware ARM; a documentação também lista plugins JUCE. | Candidato direto para comparação de corpos de baixo e percussão. Fixar commit, auditar os headers e dependências usados e confirmar compilação em JUCE e WebAssembly antes de incorporar. O submódulo `DaisySP-LGPL` é separado e não entra automaticamente sob a licença MIT do núcleo. Nenhum código DaisySP foi copiado nesta etapa. |
| [Faust](https://github.com/grame-cncm/faust) — compilador LGPL; licenças de saída dependem das entradas | Linguagem de especificação matemática de DSP; compila descrições para C++ e WebAssembly, entre outras saídas. Bibliotecas e arquivos de arquitetura têm licenças próprias; o manual esclarece que a licença do código gerado depende das entradas usadas. | Candidato para um núcleo DSP único entre JUCE e web, sem substituir o gerador composicional do Batruc. Primeiro ensaio futuro: compilar um oscilador/filtro pequeno para os dois destinos e comparar saída, parâmetros e custo. Auditar bibliotecas e arquitetura exatas antes de distribuir. |
| [Pure Data](https://github.com/pure-data/pure-data) — Standard Improved BSD no núcleo, salvo avisos por arquivo | Ambiente visual em tempo real para montar e ouvir osciladores, envelopes, filtros, modulações e cadeias de síntese. | Adequado a sketches de escuta e comparação de uma hipótese sonora; não é por si só um motor JUCE/web embutido. Auditar cada external e dependência. |
| [JUCE 9.0.3](https://github.com/juce-framework/JUCE/tree/9.0.3) — AGPLv3 ou licença comercial | Framework já usado pelo app nativo Batruc, com utilitários de DSP e suporte à aplicação. | O Batruc declara AGPL-3.0-or-later, coerente com o caminho AGPL do JUCE; manter avisos e dependências de terceiros. JUCE é framework, não um banco de osciladores composicionais. |
| [Antitotem — `CmosVoice`](../ANTITOTEM/src/core/CmosVoice.h) — código interno RASGO sob AGPL-3.0-or-later | Cinco osciladores com relações de fase, forma, modulação e feedback; oferece precedente interno de síntese interdependente. | Fonte RASGO útil para estudar e potencialmente adaptar, não dependência entre projetos. Antes de copiar qualquer parte: identificar commit de origem, hash, licença/cabeçalho e crédito; criar cópia autônoma e adaptar ao conceito do Batruc. Nenhum código foi copiado nesta etapa. |
| [Manual oficial do Korg Volca Bass](https://cdn.korg.com/us/support/download/files/c8dc45a2465c108921a3bfbcf71e6386.pdf) — documentação de produto, não código livre | Descreve três VCOs, agrupamento por voz, sequências independentes/agrupadas, formas saw/square, filtro, envelopes, VCA e destinos do LFO (amplitude, pitch e corte). | Referência funcional para estudar relações entre osciladores e sequência; não fornece licença para reproduzir firmware, circuitos, interface ou materiais proprietários. Usar os princípios gerais e criar regras, controles e som autorais. |

**Decisão de trabalho:** não adicionar DaisySP, Faust ou libpd como dependência
antes de um experimento de compilação e licença por arquivo. O primeiro
incremento autoral de três componentes senoidais já está implementado no core
JUCE e na maquete web, com perfis harmônico, ímpar, inarmônico e de batimento
lento. Ambos usam as razões `1:2:3`, `1:3:5`, `1:√2:√3` ou
`1:1,003:0,997`, pesos `1:0,34:0,16` normalizados e filtro passa-baixas.
Isto é um estudo pequeno e próprio, não uma incorporação de DaisySP nem uma
emulação do Volca Bass. A validação musical ainda depende de escuta comparada.

## Groove, interpretação e percepção

| Fonte | Leitura e experimento no Batruc |
| --- | --- |
| Noah Wardrip-Fruin, [*Expressive Processing: Digital Fictions, Computer Games, and Software Studies*](https://mitpress.mit.edu/9780262517539/expressive-processing/), MIT Press, edição brochura de 2012 | Referência conceitual sobre processos computacionais expressivos em mídia digital; perguntar como regras visíveis na interface adquirem sentido musical. Não é tratado de microtiming. |
| Mark Abel, [*Groove: An Aesthetic of Measured Time*](https://www.historicalmaterialism.org/book-series/groove-an-aesthetic-of-measured-time/), 2014 | Groove como organização musical, histórica e social do tempo; comparar padrões com mesmo BPM e ataques, mas diferentes relações de acento, duração e deslocamento. Não reduzir a tese do livro a “humanização” aleatória. |
| Patrik N. Juslin, Anders Friberg e Roberto Bresin, [“Toward a computational model of expression in music performance: The GERM model”](https://journals.sagepub.com/doi/10.1177/10298649020050S104), 2001 | Separar regras generativas, intenção emocional, variação aleatória e princípios de movimento. Testar cada fator isolado e combinado na **realização** de uma mesma célula; não confundir partitura gerada com sua interpretação. O capítulo posterior de Juslin sobre [GERMS](https://academic.oup.com/book/34989/chapter-abstract/298667928) acrescenta a dimensão de surpresa estilística. |
| Henkjan Honing, [“From Time to Time: The Representation of Timing and Tempo”](https://www.mcg.uva.nl/mmm-2003/papers/mmm-1/mmm-1.html), 2001, e [“Structure and Interpretation of Rhythm and Timing”](https://www.mcg.uva.nl/mcg-2023/papers/honing-2002.pdf), 2002 | Distinguir grade simbólica, tempo de execução e percepção; testar funções de tempo e escuta de adiantamento/atraso sobre o mesmo padrão. O título “The Cognitive Neuroscience of Rhythm” foi recebido como indicação temática, mas não confirmado como livro de Honing. |
| David Huron, [*Sweet Anticipation: Music and the Psychology of Expectation*](https://mitpress.mit.edu/9780262582780/sweet-anticipation/), MIT Press, 2006 | Expectativa, surpresa, síncope, cadência e tensão; comparar retorno esperado e desvio da célula durante uma forma de sete ciclos, com julgamento de escuta. |

## Matemática musical e governança do material

| Fonte | Leitura e experimento no Batruc |
| --- | --- |
| Mariana Montiel e Robert W. Peck (orgs.), [*Mathematical Music Theory: Algebraic, Geometric, Combinatorial, Topological and Applied Approaches to Understanding Musical Phenomena*](https://www.worldscientific.com/worldscibooks/10.1142/10858), 2018 | Fonte para investigar transformações, similaridade e classificação de estruturas musicais. A indicação inicial “Marianne Thomsen” e o subtítulo com “Computational Aspects” não foram confirmados; tampouco foi confirmado que o volume ensine rubato por ruído rosa ou distribuição gaussiana. Verificar capítulos antes de atribuir tais métodos. |
| Patrick Burkart, [*Music and Cyberliberties*](https://www.weslpress.org/9780819569189/music-and-cyberliberties/), Wesleyan University Press, 2010 | Referência transversal para direitos, circulação e autonomia dos músicos em rede. O livro não é de Patrik Juslin e não trata do modelo GERM; manter fora do núcleo técnico de microtiming. |

## Geometria global, física teórica e estruturas quânticas

| Fonte primária | Conteúdo confirmado e pergunta para o Batruc |
| --- | --- |
| Henri Poincaré, conjectura sobre 3-variedades, enunciado e contexto no [Clay Mathematics Institute](https://www.claymath.org/wp-content/uploads/2022/06/poincare.pdf); Grigori Perelman, [*The Entropy Formula for the Ricci Flow and Its Geometric Applications*](https://arxiv.org/abs/math/0211159), [*Ricci Flow with Surgery on Three-Manifolds*](https://arxiv.org/abs/math/0303109) e [*Finite Extinction Time…*](https://arxiv.org/abs/math/0307245) | Distinguir a esfera `S²`, a bola `B³` e a 3-esfera `S³`. A conjectura diz que toda 3-variedade fechada simplesmente conexa é homeomorfa a `S³`; a prova de Perelman usa fluxo de Ricci e cirurgia dentro da geometrização. Para o Batruc, fonte de topologia e evolução geométrica, não um algoritmo pronto de áudio. |
| Mark Kac, [“Can One Hear the Shape of a Drum?”](https://doi.org/10.2307/2313748), *The American Mathematical Monthly*, 1966 | Formula o vínculo entre domínio, Laplaciano e espectro de uma membrana. Pergunta: quais medidas globais da forma aparecem nos modos? O espectro não deve ser tratado como assinatura única da forma. |
| Carolyn Gordon, David Webb e Scott Wolpert, [“One Cannot Hear the Shape of a Drum”](https://arxiv.org/abs/math/9207215), *Bulletin of the AMS*, 1992 | Constrói domínios planos não isométricos com o mesmo espectro de Laplace. Experimento: comparar geometrias distintas que partilham espectro, mas têm percursos e máscaras diferentes. |
| Álvaro Pelayo, [“Hamiltonian and Symplectic Symmetries: An Introduction”](https://arxiv.org/abs/1610.09718), 2016 | Introdução a variedades simpléticas e simetrias Hamiltonianas. Estudar espaço de fase, invariantes e transformações que preservam a estrutura antes de aplicar ao pêndulo. |
| Adam Marsh, [“Gauge Theories and Fiber Bundles: Definitions, Pictures, and Results”](https://arxiv.org/abs/1607.03089), 2016 | Geometria de fibrados, conexões e gauge. Investigar transporte de fase/estado de uma voz ao longo de uma trajetória e transformação ao fechar o ciclo (holonomia). |
| Sudhir R. Jain e Rhine Samajdar, [“Nodal Portraits of Quantum Billiards: Domains, Lines, and Statistics”](https://arxiv.org/abs/1709.03650), 2017 | Revisão de linhas nodais e estatísticas de autofunções em bilhares quânticos, membranas e problemas de Helmholtz. Usar nós/antinós como material visual e composicional somente após fixar geometria e condições de contorno. |
| P. Bocchieri e A. Loinger, [“Quantum Recurrence Theorem”](https://doi.org/10.1103/PhysRev.107.337), *Physical Review*, 1957 | Formula uma recorrência quântica para estados fechados. Comparar com recorrência clássica sem supor repetição musical exata ou tempos curtos; especificar espaço de Hilbert e evolução. |
| M. V. Berry, [“Quantal Phase Factors Accompanying Adiabatic Changes”](https://doi.org/10.1098/rspa.1984.0023), *Proceedings of the Royal Society A*, 1984 | Define a fase geométrica acumulada por um autoestado transportado adiabaticamente num circuito de parâmetros. No Batruc, investigar fase de retorno em um modelo finito de acoplamento entre vozes. |
| D. J. Thouless, M. Kohmoto, M. P. Nightingale e M. den Nijs, [“Quantized Hall Conductance in a Two-Dimensional Periodic Potential”](https://doi.org/10.1103/PhysRevLett.49.405), *Physical Review Letters*, 1982 | Relaciona condutância Hall quantizada ao invariante topológico de uma banda. Experimento: fases de duas vozes como toro de parâmetros, curvatura de Berry e número de Chern para classificar regimes; requer banda isolada e checagem de gap. |
| Takahiro Fukui, Yasuhiro Hatsugai e Hiroshi Suzuki, [“Chern Numbers in Discretized Brillouin Zone”](https://doi.org/10.1143/JPSJ.74.1674), *Journal of the Physical Society of Japan*, 2005 | Algoritmo de elos para Chern number numa malha discreta, manifestamente invariante a gauge. Fonte candidata para protótipo em toro de fases, após teste de convergência e comparação com solução analítica. |
| Michael M. Bronstein, Joan Bruna, Taco Cohen e Petar Veličković, [“Geometric Deep Learning: Grids, Groups, Graphs, Geodesics, and Gauges”](https://arxiv.org/abs/2104.13478), 2021 | Enquadra CNNs, GNNs e modelos em grafos/variedades pelas simetrias e estrutura dos dados. Pergunta: uma GNN acrescenta algo às regras explícitas do grafo de vozes? Para quatro ilhas, começar pelas regras autorais por serem mais leves e legíveis. |
| Maziar Raissi, Paris Perdikaris e George E. Karniadakis, [“Physics-informed Neural Networks”](https://doi.org/10.1016/j.jcp.2018.10.045), *Journal of Computational Physics*, 2019 | Redes treinadas com resíduos de EDP e condições físicas. Candidato a surrogate offline de um solver; comparar com solução de referência, pois restrições de treino não garantem precisão, estabilidade ou conservação. |
| J. C. Maxwell, [“On the Motions and Collisions of Perfectly Elastic Spheres”](https://doi.org/10.1080/14786446008642818), *Philosophical Magazine*, 1860; Ludwig Boltzmann, [paper de 1877 sobre equilíbrio térmico e cálculo de probabilidades — tradução anotada](https://doi.org/10.3390/e17041971) | Origem cinética da distribuição de velocidades e da relação entre probabilidade, energia e equilíbrio. No Batruc, comparar uma amostragem de estados `pᵢ∝gᵢe^(−Cᵢ/τ)` a escolha uniforme, com `τ` composicional adimensional; não chamar o parâmetro de temperatura física nem associar a distribuição automaticamente a groove ou emoção. |

## Geometria rítmica e Study Mode

| Fonte | Uso no estudo do Batruc |
| --- | --- |
| Richard Cohn, [“A Platonic Model of Funky Rhythms”](https://mtosmt.org/issues/mto.16.22.2/mto.16.22.2.cohn.php), *Music Theory Online* 22/2, 2016 | Modela ritmos cíclicos gerados por spans de três unidades sobre molduras binárias, discutindo tresillo e relações com bossa nova. Base para visualizar rotação, ciclos de 8/16/32 posições e sobreposição, preservando as diferenças entre repertórios. |
| Kjetil Klette Bøhler, [“The Political Force of Musical Actants: Grooves, Pleasures, and Politics in Havana D’Primera’s ‘Pasaporte’ Live in Havana”](https://doi.org/10.1017/S1478572220000614), *Twentieth-Century Music* 18/2, 2021 | Analisa rumba clave 3–2/2–3 e a relação entre clave, riffs, campana, baixo e piano. Orienta o Study Mode a mostrar direção e interação das camadas, não só uma célula isolada. |
| Ernesto Trajano de Lima e Geber Ramalho, [“On Rhythmic Pattern Extraction in Bossa Nova Music”](https://ismir2008.ismir.net/papers/ISMIR2008_238.pdf), ISMIR 2008 | Estuda padrões e variações de acompanhamento de violão a partir de performances MIDI e catálogo transcrito. Sustenta que a visualização de bossa deve admitir várias células/variações, articulação e microtiming, em vez de um único padrão fixo. |
| Marc DeBlasie, [Rhythmic Geometry™ — site oficial](https://rhythmicgeometry.com/), página [How it works](https://rhythmicgeometry.com/how-it-works) e [perfil Instagram](https://www.instagram.com/rhythmicgeometry/) | Referência conceitual e de produto: Orbit revela movimento e fase entre ciclos; Study organiza alinhamentos em ciclo comum/LCM; Riff coloca células e frases contra o compasso e mostra deslocamento/retorno. Os trechos “Core idea” e a narrativa de origem citados pelo autor nesta conversa são material do site oficial, não texto autoral do Batruc. Usar os conceitos como referência, com implementação, fórmulas, visual, textos e identidade próprios; não reutilizar código, arte, cenas ou identidade visual. |

Study Mode deve identificar compassos, unidade de subdivisão, orientação,
acentos e procedência de cada transcrição. Antes de tratar células como base de
geração, conferir grafia e contagem com fontes musicais e especialistas das
tradições; a geometria é uma ferramenta analítica/composicional, não substitui
aprendizado contextual nem performance.

Estas fontes orientam estudo; não autorizam incorporar figuras, texto, conjuntos
de dados, pesos treinados ou código. Não há rede neural ou solver espectral
implementado no Batruc nesta etapa.

## Pesquisa independente: fontes para aprofundamento

Seleção pelo vínculo entre conceito, cálculo, escuta e interface do Batruc.
“Aberto” abaixo indica leitura sem pagamento no endereço conferido; quando a
licença de reutilização não está clara, ela permanece por verificar.

### Percussionistas, corpo, escuta e matéria sonora

| Fonte conferida | O que sustenta | Possível investigação no Batruc |
| --- | --- | --- |
| [Naná Vasconcelos, entrevista “The Voice of the Berimbau”](https://www.arcomusical.com/wp-content/uploads/2014/11/Nana_Vasconcelos_The_Voice_of_the_Berimbau.pdf), *Percussive Notes*, 2007; [entrevista à Trip](https://revistatrip.uol.com.br/trip/entrevista-com-nana-vasconcelos) | Relatos do próprio músico sobre explorar ritmos, timbres, voz e técnicas do berimbau e sobre a relação entre corpo, escuta e instrumento. Não documentam uma regra geral de micro-atraso dependente da força do ataque. | Tratar respiração, voz, ataque e silêncio como gestos composicionais controláveis; testar relações entre intensidade e timing como hipótese auditável, não como imitação de Naná. |
| [Milford Graves, “Research”](https://www.milfordgraves.com/pageab) | Graves descreve seu trabalho com gravações de batimentos, dados elétricos do coração e algoritmos que convertem padrões cardíacos em som. Isso sustenta a referência de sonificação cardíaca, mas não a alegação de que ele formulou uma teoria universal do groove baseada em VFC. | Incluir um modo opcional de sequência derivada de dados biológicos documentados ou de séries sintéticas claramente identificadas; preservar origem, faixa e transformação, e comparar com pulso estável. Não chamar de “BPM biológico” sem especificação e validação. |
| [Djalma Corrêa, biografia e acervo](https://djalmacorrea.com.br/biografia/); [Acervo Djalma Corrêa](https://djalmacorrea.com.br/); [perfil do Balafon](https://www.balafon.org.br/djalma-correa/index.html) | Formação em eletrônica e percussão, experimentação com objetos/instrumentos, música eletroacústica, pesquisa e documentação de culturas populares. As fontes consultadas não comprovam a descrição específica de uma teoria de “materiais aquecendo” pelo aumento de velocidade. | Explorar corpos sonoros por parâmetros explícitos de ressonância, massa percebida, amortecimento e densidade de excitação; ritmo pode alterar esses parâmetros apenas como escolha artística do usuário, não como fato físico automático. Respeitar contextos e proveniência de práticas culturais. |
| [Airto Moreira, história oral NAMM](https://www.namm.org/library/oral-history/airto-moreira); [entrevista Red Bull Music Academy](https://daily.redbullmusicacademy.com/2017/06/interview-airto-moreira/) | Fontes sobre a inventividade instrumental e o uso de pequenos objetos, sons encontrados e instrumentos diversos. Não sustentam por si a proposta de pan/reverb aleatórios por nota. | Criar um espaço performável de fontes sonoras com posição, distância e envio de efeito editáveis e com memória entre eventos; preferir trajetórias e relações reproduzíveis a aleatoriedade sem controle. |
| [Cooper e Meyer, *The Rhythmic Structure of Music*](https://press.uchicago.edu/ucp/books/book/chicago/R/bo24515499.html), 1963 | O sumário editorial confirma pulso, métrica, acento, stress, agrupamento, ambiguidade, mobilidade, tensão, continuidade e transformação rítmica. | Expor acentuação e agrupamento em níveis; permitir comparar a mesma célula com mudanças de agrupamento, pausa e acento, sem reduzir interpretação a uma grade única. |

As regras de velocity→timing, VFC/Lorenz→tempo, densidade→“aquecimento” do
material, pan/reverb dependentes da nota anterior e impulsos de síncope→fase do
LFO são propostas de desenho, não resultados atribuíveis a esses artistas ou
fontes. Devem ser opções limitadas, determinísticas por seed quando aplicável,
visíveis e comparáveis em A/B. Para o baixo, priorizar primeiro interação
intencional e editável entre eventos, acentos, envelope, cutoff e LFO; avaliar
desvios de tempo por voz em faixa pequena e contra uma versão sem desvio.

### Ritmo, corpo, swing e intenção

| Fonte conferida | Acesso | Pergunta concreta para o instrumento |
| --- | --- | --- |
| Grosvenor Cooper e Leonard B. Meyer, [*The Rhythmic Structure of Music*](https://press.uchicago.edu/ucp/books/book/chicago/R/bo24515499.html), University of Chicago Press, 1963 | Comercial/prévia | Como agrupamento, acento e pausa mudam a frase mantendo os mesmos ataques? |
| Justin London, [*Hearing in Time: Psychological Aspects of Musical Meter*, 2ª ed.](https://academic.oup.com/book/9899), Oxford University Press, 2012 | Comercial/resumo | Como o ouvinte constrói tactus e metros não isócronos; quando uma razão matemática deixa de funcionar como pulso percebido? |
| Anne Danielsen (org.), [*Musical Rhythm in the Age of Digital Reproduction*](https://www.routledge.com/Musical-Rhythm-in-the-Age-of-Digital-Reproduction-1st-Edition/Danielsen/p/book/9781315596983), 2010 | Comercial/prévia | Como o microrritmo interage com timbre, corpo, produção digital e tolerância rítmica? Priorizar capítulos de Danielsen, Johansson, Bjerke e Clarke para comparação. |
| Anne Danielsen, [*Presence and Pleasure: The Funk Grooves of James Brown and Parliament*](https://www.weslpress.org/9780819501608/presence-and-pleasure/), Wesleyan University Press | Comercial/prévia | Como repetição, contrapulso e diálogo entre linhas criam groove sem depender de desvios aleatórios? |
| Marc Leman, [*Embodied Music Cognition and Mediation Technology*](https://mitpress.mit.edu/9780262122931/embodied-music-cognition-and-mediation-technology/), MIT Press, 2007 | Comercial/prévia | Como gesto, corpo, ação e percepção participam da interface do instrumento e da interpretação da frase? |
| Anders Friberg e Andreas Sundström, [“Swing Ratios and Ensemble Timing in Jazz Performance”](https://doi.org/10.1525/mp.2002.19.3.333), 2002; [resumo público anterior](https://acoustics.org/pressroom/httpdocs/137th/friberg.html) | Resumo aberto; artigo pode exigir acesso | Testar swing dependente do BPM e diferente por voz, em lugar de razão fixa de 2:1. Conferir o artigo integral antes de fixar limites numéricos. |
| Maria A. G. Witek e colaboradores, [“Syncopation, Body-Movement and Pleasure in Groove Music”](https://pmc.ncbi.nlm.nih.gov/articles/PMC3989225/), 2014 | Artigo de leitura aberta | Comparar graus de síncope e relação entre prazer e vontade de mover-se, sem transformá-los em pontuação automática de qualidade musical. |
| Olivier Senn, Lorenz Kilchenmann, Richard von Georgi e Claudia Bullerjahn, [“The Effect of Expert Performance Microtiming on Listeners’ Experience of Groove in Swing or Funk Music”](https://www.frontiersin.org/journals/psychology/articles/10.3389/fpsyg.2016.01487/full), 2016 | Artigo aberto, CC BY | Comparar timing quantizado, timing de músicos e desvios ampliados: neste estudo, as duas primeiras condições receberam avaliações igualmente altas e a ampliação reduziu as avaliações. Não generalizar para toda música. |

### Composição algorítmica, geometria e crescimento

| Fonte conferida | Acesso | Pergunta concreta para o instrumento |
| --- | --- | --- |
| Gerhard Nierhaus, [*Algorithmic Composition: Paradigms of Automated Music Generation*](https://link.springer.com/book/10.1007/978-3-211-75540-2), Springer, 2009 | Comercial/prévia | Qual operação serve ao motivo: Markov, gramática, rede de transições, caos, autômato ou evolução? Comparar seu efeito musical e custo. |
| Fred Lerdahl e Ray Jackendoff, [*A Generative Theory of Tonal Music*](https://mitpress.mit.edu/9780262620499/a-generative-theory-of-tonal-music/), MIT Press, 1982/1985 | Comercial/prévia | Como hierarquia métrica e agrupamento podem ajudar a segmentar célula, frase e seção? Teoria da escuta tonal, não gerador automático pronto. |
| Curtis Roads, [*The Computer Music Tutorial*, 2ª ed.](https://mitpress.mit.edu/9780262361545/the-computer-music-tutorial/), MIT Press, 2023 | Comercial/prévia | Como combinar representação composicional, síntese, análise e interface sem misturar escalas de tempo? |
| Przemyslaw Prusinkiewicz e Aristid Lindenmayer, [*The Algorithmic Beauty of Plants*](https://www.algorithmicbotany.org/papers/), Springer, 1990 | PDF completo disponibilizado pelos autores; licença de reutilização a conferir | Experimentar sistemas de reescrita e crescimento gráfico como geradores de forma; limitar profundidade e eventos e decidir separadamente a tradução musical. |

### Funções, integrais, álgebra, matrizes e famílias numéricas

| Fonte conferida | Acesso | Pergunta concreta para o instrumento |
| --- | --- | --- |
| Gilbert Strang e Edwin Herman, [*Calculus*, volumes 1–3](https://openstax.org/books/calculus-volume-3/pages/preface), OpenStax | Leitura gratuita; CC BY-NC-SA para o conteúdo | Ligar derivada a inclinação/velocidade do traço, integral a acumulação de energia ou duração, curvas paramétricas a movimento 2D/3D e equações diferenciais a estado físico. Definir unidades e mapeamento musical em cada caso. |
| Sheldon Axler, [*Linear Algebra Done Right*, 4ª ed.](https://linear.axler.net/), 2024 | Acesso aberto; CC BY-NC | Matrizes para acoplamento de vozes/modos, transformações geométricas e inversões com domínio explícito. Separar matriz matemática de “matriz de modulação” do sintetizador. |
| Oscar Levin, [*Discrete Mathematics: An Open Introduction*, 4ª ed.](https://discrete.openmathbooks.org/dmoi4.html) | Acesso aberto; CC BY-NC-SA | Sequências e recorrências, contagem, grafos e funções geradoras para células, pausas, rotas entre seções e famílias de números. Fibonacci, primos e outras sequências são materiais, não valores musicais automáticos. |
| Donna Kirk, [*Contemporary Mathematics*](https://openstax.org/books/contemporary-mathematics/pages/10-introduction), OpenStax, 2023 | Leitura gratuita; conferir licença do conteúdo antes de reutilizar | Base visual de polígonos, mosaicos, áreas e volumes para o editor de traços, compasso e formas geométricas; complementar com curvas paramétricas do *Calculus* 3. |
| G. H. Hardy e E. M. Wright, [*An Introduction to the Theory of Numbers*, 6ª ed.](https://academic.oup.com/book/54489), Oxford University Press, 2008 | Comercial/resumo | Primos, congruências, frações contínuas, funções aritméticas e partições como famílias de receitas e durações; não transformar índice de sequência diretamente em nota sem regra composicional. |
| Julius O. Smith III, [*Mathematics of the Discrete Fourier Transform (DFT) with Audio Applications*, 2ª ed.](https://www.dsprelated.com/freebooks/mdft/) | Leitura online gratuita; licença de reutilização a conferir | Distinguir coeficientes espectrais, fase, janela, DFT e FFT ao animar e analisar ondas. |
| Steven H. Strogatz, [*Nonlinear Dynamics and Chaos*](https://ocw.mit.edu/courses/18-385j-nonlinear-dynamics-and-chaos-fall-2004/pages/lecture-notes/), e notas complementares do MIT OCW | Livro comercial; notas de curso abertas | Estudar oscilação, bifurcação e atratores para movimentos gráficos e mudanças de forma com parâmetros controláveis; não converter caos diretamente em alturas sem escuta. |

### Física, material, timbre e simulação

| Fonte conferida | Acesso | Pergunta concreta para o instrumento |
| --- | --- | --- |
| [*University Physics*, vol. 1](https://openstax.org/books/university-physics-volume-1/pages/preface), OpenStax | Leitura gratuita; CC BY-NC-SA para o conteúdo | Separar gravidade, torque, mola, colisão, elasticidade, ondas e som; distinguir movimento visual de vibração audível. |
| Neville H. Fletcher e Thomas D. Rossing, [*The Physics of Musical Instruments*, 2ª ed.](https://link.springer.com/book/10.1007/978-0-387-21603-4), Springer, 1998 | Comercial/prévia | Relacionar cordas, barras, membranas, placas, tubos e sistemas acoplados a modos e espectros; conferir condições físicas antes de nomear um timbre “de material”. |
| Julius O. Smith III, [*Physical Audio Signal Processing*](https://www.dsprelated.com/freebooks/pasp/), leitura online | Leitura gratuita; licença de reutilização a conferir | Começar com modelos de mola e guia de ondas digital de custo limitado; comparar excitação, ressonador, amortecimento e saída. |
| Daniel Shiffman, [*The Nature of Code*, 2ª ed.](https://natureofcode.com/), 2024 | Leitura gratuita pelos autores; conferir licença do material usado | Protótipos de forças, agentes, oscilação e desenho do percurso. O exemplo gráfico precisa de tradução sonora autoral e estabilidade própria. |

## Verificação da lista obtida no Gemini: ondas e DSP

As fontes abaixo foram conferidas separadamente. Mantêm-se como estudo,
com correções importantes de escopo e atribuição.

| Fonte | Estado e uso proposto |
| --- | --- |
| Steven W. Smith, [*The Scientist and Engineer’s Guide to Digital Signal Processing*](https://www.dspguide.com/), 1997 | Livro integral disponibilizado gratuitamente pelo autor/editora; DFT, FFT, filtros e análise. [Termos de uso](https://www.dspguide.com/copyrite.htm) restringem o texto a usos definidos, e os programas são oferecidos para uso não comercial; estudar sem incorporá-los ao Batruc. |
| Ken Steiglitz, [*A Digital Signal Processing Primer: With Applications to Digital Audio and Computer Music*](https://search.worldcat.org/title/A-digital-signal-processing-primer-%3A-with-applications-to-digital-audio-and-computer-music/oclc/1152357570) | Autoria e título confirmados; edição Dover de 2020. Fonte introdutória de filtros, onda e amostragem; não atribuir capítulo específico sem consultar o volume. |
| Carl M. Bender e Steven A. Orszag, [*Advanced Mathematical Methods for Scientists and Engineers I: Asymptotic Methods and Perturbation Theory*](https://link.springer.com/book/10.1007/978-1-4757-3069-2), reedição de 1999 | Método avançado para aproximação de equações diferenciais e integrais; a simulação de corda/membrana exige ainda discretização, contornos e teste de estabilidade. |
| Stanley J. Farlow, [*Partial Differential Equations for Scientists and Engineers*](https://search.worldcat.org/title/1020132960), Dover, 1993 | Título e autoria confirmados; estudar equação da onda e condições de contorno. “Axiomas” não é descrição precisa desses modelos físicos. |
| Udo Zölzer, [*Digital Audio Signal Processing*, 3ª ed.](https://www.wiley.com/en-dk/digital-audio-signal-processing-3rd-edition-p-9781119832690), Wiley, 2022 | DSP, amostragem, interpolação, filtros e efeitos; conferir índice/capítulos antes de atribuir ao livro uma fórmula específica de waveshaping ou oscilador. |
| Vesa Välimäki, [artigo de 2005 sobre serra com aliasing reduzido](https://research.aalto.fi/en/publications/discrete-time-synthesis-of-the-sawtooth-waveform-with-reduced-ali/); Välimäki e Huovilainen, [revisão de 2007](https://research.aalto.fi/en/publications/antialiasing-oscillators-in-subtractive-synthesis/); Välimäki, Pekonen e Nam, [artigo de 2012 sobre interpolação polinomial](https://research.aalto.fi/en/publications/perceptually-informed-synthesis-of-bandlimited-classical-waveform/) | DPW e polyBLEP são métodos distintos. O artigo original de DPW é de Välimäki; não atribuí-lo a Pekonen. A revisão de 2007 e o estudo de 2012 são as referências verificadas para técnicas BLEP/polinomiais. Comparar espectro de serra ingênua, DPW, polyBLEP e tabela com bandas limitadas antes de escolher. |
| Julius O. Smith III, [*Spectral Audio Signal Processing*](https://www.dsprelated.com/freebooks/sasp/); [tutorial de wavetable do JUCE](https://juce.com/tutorials/tutorial_wavetable_synth/) | Wavetable pré-calculada pode reduzir custo por amostra, dependendo de tamanho, número de vozes e interpolação. Interpolação por si só não elimina aliasing; usar tabelas com limite de harmônicos adequado ao registro ou outra estratégia testada. |

## Baixo, LFO, modulação e interpretação rítmica

Esta seção aprofunda a ligação entre baixo, movimento de parâmetros, forma de
onda e groove. Os livros foram verificados por metadados e sumários editoriais;
isso não equivale à leitura integral. Onde a evidência é de artigo, ela vale
para os estímulos e condições estudados, não para toda música.

| Fonte conferida | O que sustenta | Experimento no Batruc |
| --- | --- | --- |
| Andy Farnell, [*Designing Sound*, MIT Press](https://mitpress.ublish.com/book/designing-sound), cap. 20 “Technique 4 — Modulation” | O sumário lista um capítulo de modulação; o material editorial inclui síntese por tabelas, funções não lineares, AM, FM e modulação de fase. A alegação de um capítulo dedicado a “Event-Synchronised LFOs” e sincronização de LFO com BPM não foi confirmada. É uma fonte forte para processos de modulação, não um manual específico de baixo. | Separar modulação de amplitude, frequência/altura, fase e filtro; expor forma, taxa, profundidade e destino em operações audíveis separadas. A sincronização ao pulso será desenho próprio do Batruc, com período expresso em fração de beat/compasso. |
| Charles Dodge e Thomas A. Jerse, [*Computer Music: Synthesis, Composition, and Performance*, 2ª ed.](https://www.prestomusic.com/books/products/7533260--computer-music-synthesis-composition-and-performance), Schirmer/Cengage, 1997 | O sumário confirma fundamentos de síntese, distorção, síntese subtrativa, técnicas baseadas em análise, granular, modelagem física, composição e performance em tempo real. Não confirma, por si, as alegações específicas de “linhas de baixo sintéticas”, PWM ou uma arquitetura de LFO para baixo. | Usar os capítulos de síntese e composição como fundamentos gerais; testar separadamente oscilador, espectro, filtro, envelope e regra de eventos. |
| Curtis Roads, [*Microsound*, MIT Press](https://mitpress.mit.edu/9780262681544/microsound/), especialmente o capítulo “Time Scales of Music” | Roads trata escalas de tempo de amostras, partículas, pulsos, linhas e estruturas. A descrição editorial situa microsom abaixo de aproximadamente um décimo de segundo. A passagem de modulador lento a oscilação audível é contínua e depende da frequência, destino e percepção; 60 Hz é som grave audível, mas não deve ser chamado automaticamente de subgrave. Não há uma fronteira universal que converta “LFO” em “baixo”. | Mostrar a taxa do modulador e permitir escuta do mesmo processo em taxas subáudio, baixa frequência audível e áudio; tornar explícito quando a função passa a afetar afinação/timbre, em vez de apenas controle lento. |
| Guilherme Schmidt Câmara, Kristian Nymoen, Olivier Lartillot e Anne Danielsen, [“Effects of instructed timing on electric guitar and bass sound in groove performance”](https://doi.org/10.1121/10.0000724), *JASA*, 2020 | Experimentos com 21 guitarristas e 21 baixistas experientes compararam intenções “laid-back”, no pulso e “pushed”. Nos baixistas, a intensidade diferiu sistematicamente; o artigo sustenta que timing e características sonoras participam conjuntamente da intenção. | Criar realizações de baixo com timing e dinâmica associados como controles independentes; comparar a combinação, sem codificar uma regra universal de “laid-back” ou “pushed”. |
| Mari Romarheim Haugen, Câmara, Nymoen e Danielsen, [“Instructed timing and body posture in guitar and bass playing in groove performance”](https://journals.sagepub.com/doi/full/10.1177/10298649231182039), 2024 | Investiga relação entre instrução temporal, corpo/postura e execução de guitarra/baixo. Reforça que microrritmo percebido abrange aspectos sonoros e corporais, não apenas posição de onset. | Preservar uma dimensão de gesto/intenção no painel e registrar a escolha do músico. A interface pode propor variações, sem alegar simular o corpo real. |
| Adrian Poole, [“Timing and Groove in the Performance of Cuban Bass and Conga Patterns”](https://journal.iftawm.org/previous/2022-volume-10-no-2/poole/), 2022 | Estudo de três performances de padrões cubanos de baixo/conga; os músicos usaram ajustes relativos pequenos entre partes, e deslocamentos modestos podiam mudar a sensação. Os valores são específicos dessas execuções, não limites universais. | Avaliar deslocamento horizontal relativo do baixo contra outra voz, com amplitude editável e retorno A/B; manter quantização como opção igualmente legítima. |
| Michael J. Hove, Céline Marie, Ian C. Bruce e Laurel J. Trainor, [“Superior time perception for lower musical pitch explains why bass-ranged instruments lay down musical rhythms”](https://pmc.ncbi.nlm.nih.gov/articles/PMC4104866/), *PNAS*, 2014 | Em tarefas de laboratório, ouvintes codificaram melhor desvios de tempo em tons baixos do que em tons altos. É evidência de sensibilidade temporal em contexto controlado, não explicação completa do papel musical do baixo. | Dar prioridade de escuta ao encaixe rítmico do baixo; verificar timing na mixagem e não avaliar a linha só pelo gráfico ou pela altura. |
| Olivier Senn e colaboradores, [“The Effect of Expert Performance Microtiming on Listeners’ Experience of Groove in Swing or Funk Music”](https://www.frontiersin.org/journals/psychology/articles/10.3389/fpsyg.2016.01487/full), 2016 | Para os trechos de funk e swing testados, versões quantizadas e originais tiveram avaliações altas semelhantes; desvios ampliados reduziram as avaliações. Evidência contra adicionar “humanização” aleatória como atalho universal. | Começar por grade estável; adicionar deslocamento intencional e limitado por voz/frase, oferecendo comparação e controle de intensidade da intervenção. |
| Julius O. Smith III, [*Physical Audio Signal Processing*](https://www.dsprelated.com/freebooks/pasp/), e [*Spectral Audio Signal Processing*](https://www.dsprelated.com/freebooks/sasp/) | Textos abertos do autor sobre osciladores, modelos físicos, filtros, ressonância e análise espectral. São referência para distinguir sinal, controle, modelo e descritor, e para escolher uma realização de áudio estável. | LFO é sinal de controle até sua frequência e rota fazerem dele parte do sinal audível; manter fase/estado previsíveis, limitar taxa/profundidade e preparar parâmetros fora do callback quando necessário. |
| [JUCE — Introduction to DSP](https://juce.com/tutorials/tutorial_dsp_introduction/); [Indiana University — The Basic Patch](https://cmtext.indiana.edu/synthesis/chapter4_patches.php); [manual do Moog Matriarch, geradores ADSR](https://api.moogmusic.com/sites/default/files/2019-09/Moog_Matriarch_Manual.pdf); Miller Puckette, [*The Theory and Technique of Electronic Music*, síntese subtrativa](https://msp.ucsd.edu/techniques/latest/book-html/node152.html) | Fontes de ensino e manuais confirmam a cadeia oscilador/fonte → filtro (VCF) → amplificador (VCA), com envelope de amplitude no VCA e envelope separado ou compartilhado no corte do filtro. O tutorial JUCE demonstra osciladores, ladder filter, ganho e LFO modulando cutoff; envelopes não são implementados automaticamente por essa cadeia. | Especificar a pista de baixo como módulos de áudio e controle separados: fonte tonal/matéria → filtro tonal → VCA/ganho de articulação → master; ADSR-A controla nível, ADSR-F opcional controla corte/resonância, LFO opcional modula destinos selecionados. Comparar com o renderer modal atual antes de decidir uma síntese substituta. |

### Decisão de projeto para o baixo e o LFO

- O LFO permanece dedicado ao módulo de baixo nesta etapa, como já previsto
  para a arquitetura do Batruc; não se cria automaticamente um LFO por canal.
- Começar por quatro destinos claramente rotulados: corte do filtro, amplitude,
  afinação/fase e posição de leitura da matéria sonora. Cada destino deve ter
  profundidade própria e faixa segura; “mais modulação” não significa
  “mais groove”.
- A taxa pode ser livre em Hz ou sincronizada a divisões rítmicas calculadas a
  partir do BPM. Reset de fase deve ser uma escolha (livre, início de compasso,
  ou ataque selecionado), não um comportamento obrigatório. Reiniciar em cada
  nota pode tornar o baixo repetitivo e criar descontinuidade audível.
- Swing e microtiming pertencem primeiro à realização de eventos. O LFO pode
  seguir o relógio musical em andamento constante; uma futura opção pode
  deformar sua fase seguindo o swing. Evitar variar sua frequência a cada
  desvio de milissegundos: isso mistura duas escalas e não tem apoio como
  receita geral de groove.
- Sample-and-hold com interpolação é uma fonte de modulação aleatória
  controlável, não um modelo fisiológico da pressão de um baixista. Se usado,
  expor seed, intervalo, frequência de atualização e suavização; não o aplicar
  automaticamente à altura ou ao decaimento.
- **Cadeia de voz sugerida:** fonte (oscilador/wavetable ou ressonador) →
  filtro tonal (VCF) → amplificador de voz (VCA ou ganho digital) → master.
  Um ADSR de amplitude abre e fecha a voz; um segundo envelope de filtro pode
  abrir o cutoff no ataque e fechá-lo durante o decaimento. O LFO é uma fonte
  de controle adicional e não substitui envelopes, eventos de nota ou a
  partitura calculada. O VCA digital é multiplicação de ganho, e “VCF” nomeia
  a função de filtragem, não exige componente analógico.
- Para o primeiro painel, manter controles essenciais (ataque/decay/sustain/
  release de amplitude, cutoff/resonância e quantidade da envoltória de filtro)
  próximos à linha editável; LFO e segundo envelope podem ficar recolhidos em
  controles avançados. A seleção deve ser validada por escuta e leitura da
  interface, sem antecipar implementação.
- Experimento inicial: uma mesma partitura de baixo, com LFO desligado e com
  modulação de corte/amplitude/afinação em profundidade baixa; comparar taxa
  livre e sincronizada, fase livre e reiniciada, em A/B. Registrar intenção,
  swing aplicado, faixa de valores, pico/nível e impressão auditiva. O
  processamento segue limitado e preparado fora do callback.

### Checagem das alegações recebidas sobre três livros

1. **Farnell:** o sumário editorial confirma o capítulo “Modulation” e os
   tópicos de síntese citados. Não confirma “capítulos inteiros de Event-
   Synchronised LFOs” nem uma regra específica de sincronização por BPM.
2. **Dodge/Jerse:** autoria, segunda edição e capítulos de síntese/composição
   estão confirmados. O sumário não confirma foco especial em synth bass, PWM
   ou a arquitetura de LFO alegada; usar como livro-base geral.
3. **Roads:** a discussão de escalas de tempo é pertinente, mas a fórmula
   “2 Hz = ritmo; 60 Hz = subgrave” é apenas uma ilustração simplificada.
   60 Hz já é frequência de áudio percebida como grave; a separação entre
   modulação de controle, ritmo e altura depende de destino, contexto e
   percepção, não de um limiar único.

### Três correções de princípio

1. **Fourier:** linearidade e decomposição são propriedades/teoremas, não
   “axiomas” musicais. Série de Fourier descreve sinais periódicos sob
   condições definidas; DFT descreve amostras finitas e FFT é um algoritmo para
   computá-la. Timbre também depende de transientes, envoltória, inarmonicidade,
   ruído e evolução temporal. Não prometer qualquer timbre só com senos fixos.
2. **Equação da onda:** `∂²u/∂t² = c² ∂²u/∂x²` é um modelo ideal de propagação
   linear em uma dimensão, sob hipóteses e condições de contorno. Mola,
   pêndulo, corda, membrana e tubo não são o mesmo sistema. Discretizar requer
   passo temporal/espacial, contornos, amortecimento, excitação e teste de
   estabilidade; um percurso gráfico lento não é automaticamente uma onda
   audível.
3. **Método de síntese:** iniciar com arquitetura híbrida. A receita e o gesto
   podem controlar osciladores aditivos ou tabelas com bandas limitadas, mais
   um banco pequeno de ressonadores físicos. FFT serve sobretudo à análise e
   visualização espectral neste primeiro ensaio. Preparar tabelas e variantes
   fora do callback, evitar alocação nele e medir CPU, aliasing, pico e escuta
   antes de escolher um motor único.

## Sequência de estudo aplicada

1. **Tempo e composição:** Toussaint, Cope, Honing, Juslin/Friberg/Bresin,
   Abel e Huron; criar três versões da mesma célula: grade original, direção de
   frase e interpretação com deslocamentos limitados. Registrar o resultado da
   escuta sem declarar que variação estatística é intenção humana.
2. **Som e matéria:** Smith, Farnell, Bilbao, Boulanger/Lazzarini e
   Phillips/Parr/Riskin; associar uma fórmula a excitação, ressonador, gráfico,
   medida espectral e saída audível, sob orçamento de CPU e estabilidade.
3. **Forma e interação:** Cope, Montiel/Peck, Wardrip-Fruin e a pesquisa
   existente do Batruc; desenvolver célula, frase, seção e peça com
   inversão, recombinação, recálculo, travas e comparação A/B.
4. **Síntese e ondas:** Smith (DFT), Steven W. Smith, Zölzer e os artigos
   Välimäki/Pekonen/Nam; comparar uma mesma curva no gráfico, na tabela,
   na síntese aditiva e no espectro, documentando o que cada representação
   mostra. Verificar aliasing e custo em três registros de altura.
5. **Corpo e intenção:** London, Danielsen, Leman, Friberg/Sundström e os
   estudos empíricos de groove; comparar a mesma célula em versões reta,
   swing dependente de BPM, deslocamento por voz e rubato de frase. Escutar
   em ordem cega e preservar a escolha do músico.

## Pascal, Fibonacci, Fourier e proporções triangulares

Esta conexão fornece três mecanismos distintos para experimentar no
Batruc; não é uma teoria unificada que determine um resultado musical:

1. **Pascal e Fibonacci.** A soma das diagonais rasas do triângulo de Pascal
   gera os números de Fibonacci. Uma linha de Pascal normalizada, como
   `[1,4,6,4,1]/16`, equivale a aplicar repetidamente a média de dois pontos e
   fornece um filtro FIR binomial passa-baixas de resposta suave. Não é um
   filtro “perfeito”: sua resposta, largura de transição, fase e custo devem
   ser avaliados para o uso pretendido. Referências: [MathWorld — Fibonacci
   Number](https://mathworld.wolfram.com/FibonacciNumber.html),
   [Binomial filters](https://githubharald.github.io/binomial_filters.html).

2. **Onda triangular e Fourier.** Uma onda triangular periódica simétrica tem
   apenas harmônicos ímpares; em amplitude usual, os coeficientes decaem como
   `8/π²·(-1)^((n−1)/2)/n²`. A integração de uma onda quadrada simétrica,
   removido o componente DC e ajustada a escala, produz uma onda triangular.
   Isso descreve uma forma de onda temporal, não o contorno de qualquer
   triângulo geométrico. A série truncada é uma opção de síntese, devendo ser
   comparada com a forma atual e examinada quanto a aliasing e custo.
   Referências: [Carnegie Mellon — Fourier series](https://www.cs.cmu.edu/~15322/book/ch03/03.html),
   [Swarthmore — triangle wave](https://lpsa.swarthmore.edu/Fourier/Series/ExFS.html).

3. **Fibonacci e triângulo retângulo.** Para quatro valores consecutivos
   `F_n,F_(n+1),F_(n+2),F_(n+3)`, vale
   `F_n·F_(n+3)+F_(n+1)²=F_(n+2)²`. Logo, os catetos
   `√(F_n·F_(n+3))` e `F_(n+1)` e a hipotenusa `F_(n+2)` definem um triângulo
   retângulo. As razões aproximam a forma de Kepler conforme os índices
   crescem; a atribuição dos quatro valores aos lados precisa respeitar essa
   identidade, não é a sequência usada diretamente como três lados. Referências:
   [Surrey Fibonacci formulae](https://fibonacci-numbers.surrey.ac.uk/Fibonacci/FibFormulae.html),
   [MathWorld — Kepler triangle](https://mathworld.wolfram.com/KeplerTriangle.html).

4. **Razão áurea em LFO.** Uma razão irracional entre taxas evita um período
   comum exato para os dois osciladores ideais, mas não impede interferência,
   cancelamentos em instantes ou aproximações de fase. “Orgânico” e “humano”
   são hipóteses auditivas/composicionais, não consequências de irracionalidade.
   Tratar como opção de modulação em teste A/B, com taxas, profundidade,
   sincronização e reset de fase explícitos.

5. **Uso previsto no instrumento.** Gerar coeficientes e padrões fora do
   callback; disponibilizar como receitas distintas: diagonal de Pascal para
   pesos de acento, FIR binomial para suavização de controle e parciais de
   Fourier para matéria sonora. O usuário deve poder inspecionar a fórmula,
   alterar ordem/normalização e comparar bypass/resultado. Não somar todas as
   conexões num único preset automático. O painel atual de Stengel é apenas um
   verificador de áreas e ainda não alimenta ritmo, timbre ou LFO.

Próximo passo: escolher capítulos concretos dos livros a que houver acesso,
anotar página/capítulo e criar um experimento pequeno por questão. Começar
pelos recursos gratuitos que permitam leitura imediata: OpenStax, Axler,
Levin, Smith/Smith, Shiffman e os artigos de groove. Acesso online a metadados
ou prévia não equivale a licença livre da obra ou do código.

## Ornamentos da caixa e notação para execução (13/11/2026)

Dossiê próprio: [`ESTUDO_ORNAMENTOS_CAIXA.md`](ESTUDO_ORNAMENTOS_CAIXA.md). Inclui:
- os 40 rudimentos PAS;
- Stone e Wilcoxon;
- Leppaus (ANPPOM 2019) sobre a caixa surda no samba;
- Weinberg (notação padronizada de bateria, PAS 1998);
- TUBS (Harland e Koetting).
