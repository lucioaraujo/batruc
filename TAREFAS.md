# Batruc — tarefas e continuidade

Atualizado em: 2026-11-14.
Estado: incubação; maquete local executável; core C++ e host JUCE Release
compilados localmente; fixtures comparam agendas seeded. A interface foi
inspecionada por capturas do próprio app em três resoluções; a escuta dos
parâmetros de síntese e o ensaio dos gestos na janela de uso seguem pendentes.
Fonte conceitual ativa: [DESENVOLVIMENTO.md](DESENVOLVIMENTO.md) (até
13/11/2026 era o `README.md`; as menções a "README.md" nas entradas
anteriores a essa data referem-se a ele). O `README.md` atual é a
apresentação pública.

## ▶ Repositório próprio e público (13/11/2026)

**Este diretório é agora o repositório `lucioaraujo/batruc`** (público). O
monorepo `rasgo-instruments` deixou de rastreá-lo (`.gitignore`) e guarda o
histórico anterior, que tem o e-mail pessoal e os caminhos locais. Daqui em
diante, commits, push, versões e releases são feitos só aqui (RASGO/AGENTS
§4).
- Autor e committer: `Lúcio de Araújo <rasgo.instruments@gmail.com>`,
  configurados neste repositório.
- Fora do público: `preservation/` e os estudos raster `exec-*.png` e
  `ideia.png` (origem não registrada).
- CI (13–14/11): rodadas 1–5 registradas em [`PUBLICACAO.md`](PUBLICACAO.md).
  Na 4ª, os três pacotes passaram; o Fedora pediu `--as-needed` (5ª).
  O site está em `website/` (em preparação, noindex, deploy manual), e o
  card do portal tem rascunho em 4 línguas no `PUBLICACAO.md`.
- Próximo passo: com a CI verde, o autor decide sobre a tag `v0.1.0`
  (release), o GitHub Pages, o `PUBLICADO = True` e o card no portal.

## ▶ Publicação — em andamento (13/11/2026, "avance em todas as etapas")

Feito (local, sem commit):
- **CMakeLists:**
  - runtime estático no Windows, `/bigobj`;
  - Universal 2 com alvo 10.15 no macOS;
  - ícone embutido; nome e versão do app;
  - regras de instalação e CPack (.deb, NSIS, .dmg; nome
    `batruc-<versão>-<plataforma>`).
- **`packaging/linux/`:** `batruc.desktop`, `empacotar.sh` (AppImage e
  .tar.gz), `install.sh` (sem root), `LEIA-ME.txt`, `testar-arranque.sh`.
  Portes do Rasgo Modular.
- **`.github/workflows/package.yml`:** porte do Modular. Fica inerte no
  monorepo e roda quando houver repositório próprio.
- **Validação:** `.deb` gerado e inspecionado aqui (binário, .desktop,
  ícones, dependências; glibc 2.38 local, 2.35 na CI).

Falta (na ordem):
1. ~~Catálogo Ritmos/MIDI~~: GMD embutido nos pacotes (13/11), baixado e
   conferido por `packaging/obter-gmd.sh`. O binário instalado carrega as
   1.150 performances.
2. ~~INSTALL.md, CHANGELOG.md, CREDITS_AND_SOURCES.md, PUBLICACAO.md~~
   feitos em 13/11. O checklist completo agora está em
   [`PUBLICACAO.md`](PUBLICACAO.md).
3. ~~Texto "Sobre" do app~~ atualizado nas 4 línguas (13/11); a versão
   0.1.0 aguarda confirmação.
4. ~~README público~~: o diário virou `DESENVOLVIMENTO.md`, e o README
   público em 4 línguas entrou (13/11). Versão: 0.1.0.
5. **Decisões do autor:**
   - criar o repositório `lucioaraujo/batruc` (privado ou público);
   - rodar a CI nos três sistemas;
   - busca de marca no INPI;
   - página no portal;
   - escuta final.

## ▶ Ponto de retomada — 13/11/2026 (ler primeiro)

Feito em 12–13/11, depois da auditoria (detalhes nas entradas abaixo):
- **Ritmo e modos:** modos RITMO/GERATIVO; GER 30 %; seeds escrevendo
  figuras (pêndulos) com desenvolvimento nos 7 ciclos; tom da seed.
- **Caixa:** ornamentos como propriedade da nota; ORN por stem.
- **Notação:** pauta do baixo e pauta de bateria, ambas com MusicXML
  (validado no MuseScore).
- **Interface:** espanhol completo; correções de texto e de
  sobreposição (Compor, Mixer).

**Decisões do autor após a escuta (13/11; "escutei" → proposta → "avance",
"sim"):**
- **FRASE do baixo:** ligado por padrão nas seeds novas; cenas antigas
  mantêm o seu.
- **Percussão no tom:** botão **PERC. NO TOM** no Mixer (linha da
  polirritmia), desligado por padrão; cena schema 64 (`percussion_key`).
  O renderizador recebe o tom da seed por atômicos, no bloco de áudio.
- **Modelos de ritmo:** a seed continua no pêndulo. No Study, RITMO →
  "Escrever figuras de outro modelo (V1–V3)" oferece bilhar, Lissajous,
  Fourier e curvas rolantes; as figuras agora seguem o compasso escolhido.
- **Microtempo medido e idiomas da caixa:** continuam como ações opcionais
  do Study.
- **GER:** 30 %, sem mudança.
- **Validação:** round-trip da cena 64; captura do Mixer em 1080 px;
  auditoria com 0 sobreposições; suíte 28/28; build sem avisos.

Esperando o autor:
- **Escuta:**
  - FRASE do baixo (`.local-build/escuta-baixo/`);
  - percussão no tom (`escuta-afinacao/`);
  - modelos de ritmo (`escuta-modelos/`);
  - GER 30 % (`escuta-modos/`).
- **Decisões:**
  - ginga por família de estilo;
  - o que toca no RITMO numa voz sem nada escrito;
  - manulação seguindo o rudimento;
  - se a seed escolhe o modelo de ritmo.

Commit local de 13/11 no monorepo, sem push (o autor confirmou: "avance").
Ficaram fora `--archive-smoke/` e `midis/groove*`, que não são deste
trabalho (o GMD é um conjunto de dados de terceiros).

Decisões finais de 13/11:
- ginga por família: idiomas e microtempo ficam como escolha no Study, sem
  ligar a família (as famílias do Batruc não são gêneros);
- voz sem nada escrito no modo RITMO: continua tocando o pulso da fórmula,
  sem máscaras.

## Ponto de retomada — 12/11/2026: auditoria musical

O autor: "esse até agora é o instrumento que menos gostei do resultado,
precisamos melhorar". A auditoria
[`AUDITORIA_MUSICAL_2026-11-12.md`](AUDITORIA_MUSICAL_2026-11-12.md) faz o
diagnóstico a partir do código e dos relatos do autor:
- o núcleo gera pulsos igualmente espaçados (cruzamentos de senoides), e as
  fórmulas só filtram ou ponderam esses pulsos;
- a seed muda pouco o ritmo;
- (um suposto teto de 65 eventos por seção era erro de leitura: o teto é 65.536);
- forma fixa em sete ciclos;
- baixo derivado das vozes;
- bateria sintética indistinta;
- tempo consumido por infraestrutura, paridade HTML/JUCE e MIDI.

A auditoria propõe:
- **Processo:** congelar a infraestrutura; só JUCE; A/B gravado com OK do
  autor a cada mudança musical.
- **Música:** uma camada modelo → figuras, motivo e desenvolvimento; seed que
  muda o ritmo; baixo com frase própria; corpo sonoro da bateria; GMD como
  estudo, não motor.

Primeiro lote sugerido: um modelo só gerando figuras para duas vozes e
baixo, com bumbo, caixa e chimbal melhorados, e A/B de três seeds.
**Decisão do autor (12/11): "inicie a modernização", "acerte os pontos que
elencou na auditoria".** Em lotes, cada um com A/B gravado para o autor.

**Lote 1 — camada modelo → figuras (12/11), aguardando escuta:**
- **Código:** `src/core/FigureComposer.{hpp,cpp}`. Os três pêndulos
  acoplados já existentes viram figuras:
  - TRASTES no ângulo: o cruzamento de uma faixa gera um ataque, e o acento
    vem da velocidade;
  - grade por papel: V1 âncora em colcheias; V2 em semicolcheias, atraída
    para o tempo; V3 livre;
  - frase A A' A B, com A' girado pelo próprio modelo e B com cadência;
  - densidade de cada voz conduzida pela troca de energia da mola ao longo
    da peça;
  - encaixe: V2 e V3 desviam dos ataques de V1.
- **Entrega:** como `RhythmicCell`; o agendador não muda.
- **Render offline:** `batruc_offline_render SEED arq.wav --voices 3
  --figuras`; com as figuras, o filtro de campo antigo fica desligado.
- **Teste:** `batruc_figure_composer`. A suíte passa 19/19.
- **A/B em `.local-build/escuta-lote1/`:** seeds 5EED2027, 1A2B3C4D e
  C0FFEE42, `-A-atual` contra `-B-figuras`.
- **Medidas** (`event_schedule` do JSON):
  - A: de 4 a 31 ataques por voz em 56 s, e compassos quase todos iguais;
  - B: de 76 a 170 ataques por voz, de 6 a 13 intervalos distintos, de 7 a 20
    compassos diferentes de 24–28.
  - O B soa de 3 a 7 dB mais alto (RMS): igualar o volume ao comparar.
- **Ainda não está no app:** entra depois do OK da escuta.
- **Próximos lotes:** corpo sonoro (bumbo, caixa, chimbal), baixo com frase
  própria, seed que muda o ritmo, forma como percurso.

**Lote 2 — corpo da bateria (12/11), aguardando escuta:**
- **Chimbal metálico:** seis quadrados inarmônicos (polyBLEP), dois
  passa-altas de ~6,5 kHz e 70 % do ruído antigo por cima. O ganho foi
  calibrado por simulação.
- **Bumbo:** a queda de altura começa em 2,4× (antes 1,85×), com τ de
  ~23 ms.
- **Chave:** `ResonatorRenderer::setModernDrumBody`; ligado por padrão, o
  app herda.
- **Render offline:** com `--figuras` as vozes viram o kit (bumbo, caixa,
  chimbal); `--corpo-antigo` grava o A/B.
- **Arquivos:** `.local-build/escuta-lote2/` (`-C-kit-corpo-antigo` contra
  `-D-kit-corpo-novo`).

**Modos RITMO × GERATIVO (12/11).** O autor: "a parte de ritmos veio depois
… os códigos anteriores de matemática e física impedem o funcionamento dos
ritmos de modo tradicional, para estudo"; "há máscaras que impedem o bom
funcionamento"; "algoritmos fantasmas". Aprovado: "ok".
- **Diagnóstico** (`CompositionCore.cpp`): um ataque escrito passava por cinco
  camadas escondidas:
  - máscara de campo, que pode zerar o ataque;
  - lei de intensidade;
  - fórmula da voz;
  - groove e curva de expressão sorteados;
  - redução formal de vozes.

  O teste mostra quatro ataques escritos **todos apagados** no GERATIVO com
  máscara.
- **Feito:**
  - `ResonatorRenderer::setRhythmMode`: no RITMO as cinco camadas ficam
    neutras nos três caminhos (célula, pulso de fórmula e evento preparado),
    e o swing continua;
  - botão RITMO/GERATIVO no início do cabeçalho, nas 4 línguas, com dica;
  - cena **schema 58** (`composition_mode`); cenas antigas abrem no
    GERATIVO;
  - `--ritmo` no render offline.
- **Testes:**
  - `batruc_rhythm_mode`: no RITMO, ligar as camadas não muda o áudio
    (diferença 0); no GERATIVO, muda (contraprova);
  - a ida e volta da cena cobre o modo.

  A suíte passa 20/20.
- **Pendente:** decidir o que toca no RITMO numa voz sem nada escrito (hoje,
  o pulso da fórmula, sem máscaras).

**Reordenação do instrumento — proposta (12/11), aguardando o autor.** O autor:
"talvez os códigos de cálculo tenham vindo cedo demais, precisamos reordenar
o instrumento … nem deixar no limbo trechos de código … reaproveitar … nada
se perde". A ordem proposta, de baixo para cima:
1. **Ritmo escrito:** células, MIDI, editor Study, contagens (Gramani),
   metrônomo. Toca exatamente.
2. **Interpretação e desvio**, sempre visível e com desfazer: swing, groove,
   microtempo, acentos e as operações do Study (rotação, densidade,
   encaixe, recombinação, `GenerativeOperations`).
3. **Geração:** os modelos **escrevem** ritmo na camada 1 como material
   editável, no padrão do `FigureComposer`. Nada é filtrado na hora de
   tocar.
   - Pêndulo acoplado (já no FigureComposer).
   - Bilhar, Lissajous, Fourier, cicloide e catenária (`GeometryCurves`):
     a mesma tradução em trastes ou eventos de curva.
   - `EcologicalRelations`: entre células.
   - `TriangleGeometry` (hoje isolado): proporções de figura.
4. **Máscaras e forma** como efeitos nomeados, só no GERATIVO e com
   intensidade visível: campo, leis de intensidade, forma em 7 ciclos.
5. **Corpo sonoro e Mixer.**

**Reordenação, passo 1 (12/11, "continue"):** o modelo passa a escrever
no app. O botão **RITMO** do Study (menu de operações rítmicas) ganhou
**"Escrever figuras do pêndulo (V1–V3)"**: o `FigureComposer` compõe com a
seed atual, e a frase (ciclo 1) é gravada como células escritas e editáveis
de V1–V3.
- O registro de derivados MIDI marca `pendulum_figures_v1:SEED`.
- Entra no desfazer.
- O menu abre mesmo com a célula vazia; as operações que precisam de
  ataques continuam desabilitadas nesse caso.
- O "Estudo físico · pêndulo triplo", que só visualizava, agora tem
  consequência sonora pelo mesmo modelo.

Build Release e CTest 20/20; diagnóstico de cena aprovado. Ainda falta: o
desenvolvimento entre ciclos (as 7 frases do FigureComposer; hoje o app
grava só a primeira) e os outros modelos na camada 3.

**Baixo sempre igual no início — causa achada e corrigida (12/11).** O autor:
"essa linha de baixo que inicia é sempre a mesma, já pedi alterar isso …
nunca fez" (seed D3BFA3CF).
- **Diagnóstico** (`--diagnose-bass=ARQ`, novo no app): a geração por seed
  variava, mas metade das seeds produzia alguma nota com intensidade > 1
  (soma de acentos). `BassAudioRenderer::configure` recusava a linha
  **inteira** em silêncio (`(void)` no áudio), e o app seguia com
  `makeDefaultBassRecipe()`, a linha fixa da abertura (Dó Dó Sol Dó Dó Mi♭ Fá
  Sol Dó Sol). D3BFA3CF era uma dessas seeds.
- **Correção:** `sanitizeBassPerformance()` (BassAudio) antes de publicar:
  intensidade 0–1, pan ±0,24, MIDI 24–60, duração dentro da seção, ordem.
  Teste `batruc_bass_sanitize`; o diagnóstico mostra as 6 seeds aceitas.
- **"Nova seed" repetia a corrente:** `algorithmicNextSeed(seed)` era uma
  função fixa da seed atual. Agora recebe um sorteio novo a cada clique
  (`salt`) e continua buscando contraste; com salt 0, o comportamento antigo
  dos testes.
- **Ainda aberto:** a mesma seed dá outro baixo em outra abertura, porque a
  seed do baixo vem da primeira seed sorteada na sessão. Vai no próximo lote.

Suíte 21/21.

**Lote das seeds (12/11; "refazer os cálculos do seed … outros parâmetros,
mais variáveis, mais invenção, menos previsibilidade e determinismo";
"pode avançar"):**
- **Identidade do baixo:** a seed do baixo vem sempre da seed da composição
  (antes, só da primeira seed da sessão). Verificado: o mesmo baixo em duas
  aberturas.
- **FigureComposer v2:**
  - compasso da seed (12/14/16/20 passos; tempo na colcheia em /8);
  - forma da frase sorteada (A A' A B · A B A B' · A A A' B · A A' B B');
  - período de ½ a 2 compassos;
  - acoplamento de 0,15 a 2,5;
  - energia de 0,25 a 1;
  - mais trastes;
  - **ruptura** (chance de 15 a 50 % por ciclo): no último compasso, V1 e V2
    trocam de figura (inversão de papéis, Gramani).
- **A seed nova escreve as figuras em V1–V3** (sem MIDI carregado). Vozes 4+
  seguem o motor das fórmulas.
- **Medido em 12 seeds:** 4 formas, energia de 0,25 a 0,96, períodos de 1 a
  4 s, densidade de 3 a 57 ataques por voz e ciclo, rupturas em ciclos
  diferentes.
- **Suíte 21/21.**
- **A observar na escuta:**
  - o app grava só a frase do ciclo 1, então o desenvolvimento dos 7 ciclos
    ainda não chega ao app;
  - em algumas seeds as contagens por ciclo variam pouco;
  - a ruptura pode estar frequente demais.

**Tom da seed (12/11; o autor: "necessário que seja em todos os tons, variar
os tons na seed").**
- **Antes:** as alturas do baixo eram `seedBase + seedX·x + seedY·y`
  arredondadas ao semitom; cromáticas, sem tônica nem escala.
- **Agora:** a seed da composição sorteia **12 tônicas × 10 modos** (maior,
  menor, dórico, mixolídio, frígio, lídio, menor harmônica, pentatônica maior
  e menor, blues). `quantizeBassToKey` leva cada nota ao grau mais próximo,
  no editor, no agendamento seedado e na nota de abertura (tônica).
- **Distribuição em 1.200 seeds:** uniforme (de 85 a 130 por tônica ou modo).
- **Na tela:** "TOM Si frígio", no início da linha do percurso, no painel
  Baixo, nas 4 línguas.
- **Teste:** `batruc_bass_key`. Suíte 22/22.
- **Ainda não segue o tom:** as membranas e os toms das vozes (altura das
  percussões afinadas).

**Desenvolvimento ao longo da forma no app (12/11, "avance").**
- **Renderizador:** `ResonatorRenderer::setFormalCycleCells` guarda uma frase
  por ciclo formal para V1–V3. A cada compasso, `refreshRhythmState` toma a
  frase do ciclo atual.
- **App:** a seed nova e "Escrever figuras do pêndulo" entregam as 7 frases
  do FigureComposer, pelo mesmo slot das células. Editar uma voz no Study
  desliga o desenvolvimento só dela (a edição vale para todos os ciclos);
  composição manual e MIDI desligam tudo.
- **Teste:** `batruc_formal_cycle_cells`: o ciclo 1 toca o ataque no início e
  o ciclo 2, no meio. Suíte 23/23.
- **Limites:** as frases por ciclo ainda não são salvas na cena (ao reabrir,
  a voz toca a frase do ciclo 1); o baixo segue derivado da frase do ciclo 1.

**Cortes e sobreposições da interface (12/11; o autor: "ainda há muitas
sobreposições").** A auditoria automática só mede o Mixer ("0
sobreposições"); os cortes estavam na tela Compor. Corrigido, conferido nas
capturas de 1080 e 1920 px:
- SWING mostra só o rótulo (o valor já está na caixa);
- COMPASSO e a caixa mais largos (mostram "7/8", não "…");
- DESFAZER e REFAZER inteiros, nas três disposições do Study, com a linha
  RITMOS/MIDI deslocada;
- limite do REC "5 min" (o rodapé fixo é outra função; as duas ajustadas);
- "Y" do zoom vertical;
- volume do metrônomo sem caixa de texto quando estreito (valor na dica);
- abas dos ciclos uniformes: "C1 · CONSTRUÇÃO…" se couber em todas, senão
  "C1…C7".

Ainda apertados, sem corte grave: a caixa do campo ("Cardioide · roda
exter…"), a lei de intensidade ("Bandas coss…") e o rótulo "ONDA / f(t) · …".
Suíte 23/23.

**Frases por ciclo salvas na cena (12/11):** schema 59. Cada canal V1–V3
grava `formal_cycle_phrases` (enabled + 7 frases de [posição, força]); cenas
até 58 abrem como antes. O diagnóstico de cena cobre a ida e volta.

**Ornamentos da caixa (12/11; o autor: "faltam os ornamentos, rufos,
ricochetes e outros elementos que são característicos da caixa"; "avance").**
- **Código:** `src/core/SnareOrnaments.{hpp,cpp}`. A operação escreve na
  célula:
  - notas fantasmas: 15–30 % em semicolcheias vazias, fora dos tempos;
  - flam: ~25 ms antes de um acento (acento = ≥ 75 % do mais forte);
  - drag: ~60 e ~30 ms;
  - rufo: ~75, ~50 e ~25 ms;
  - ricochete: 3–8 rebotes, intervalo × 0,8 e força × 0,6;
  - rulo: fusas até o próximo ataque (até um tempo), com crescendo;
  - aro: GM 37 nas notas fracas.

  Determinístico pela seed. O que não cabe é pulado e nunca desloca ataques
  existentes.
- **App:** submenu **Ornamentar (caixa)** no botão RITMO do Study (voz
  selecionada; também nas 7 frases, mantendo o desenvolvimento). A seed
  escolhe um vocabulário de caixa para a V2 (nenhum · fantasmas+flam ·
  fantasmas+drag · ricochete · rulo · rufo+aro) com quantidade de 25 a 60 %.
- **Teste:** `batruc_snare_ornaments`. Suíte 24/24.
- **Escuta pendente:**
  - o enfeite usa o mesmo corpo da voz, mais fraco; não há ainda um timbre
    próprio de nota de enfeite;
  - a V2 só soa como caixa se a família sonora lhe der caixa.

**Profundidade do GERATIVO (12/11).** O autor: "no modo generativo a
bateria fica mais baixa, em certos momentos chega a sumir, e depois volta";
aprovou a proposta ("ok") e afinou: "mais próximo do volume do modo ritmo,
podendo haver dinâmicas, mas não muito acentuadas".
- **Causa medida** (voices stem, por compasso): no GERATIVO o nível ficava
  10 dB abaixo do RITMO e variava 14 dB. Duas origens:
  - o produto das camadas escondidas (campo, lei, fórmula da voz, groove,
    expressão) zerava ataques;
  - a redução formal de vozes calava vozes inteiras no primeiro ciclo.
- **Código:** `ResonatorRenderer::setGenerativeDepth(d)`, com padrão 0,3.
  - Os ataques passam por 1 − d·(1 − produto) nos caminhos da célula, da
    fórmula e da agenda preparada, e a fórmula não descarta mais o ataque
    de campo zero.
  - A voz que a forma tiraria continua tocando com nível 1 − d.
  - d = 1 reproduz o comportamento anterior bit a bit; d = 0 é idêntico ao
    RITMO.
- **App:** controle **GER 30 %** no rodapé, ao lado do volume, nas quatro
  línguas. Fica apagado no modo RITMO. Cena schema 60 (`generative_depth`);
  cenas antigas abrem em 30 %. Offline: `--profundidade 0..1`.
- **Medição** (`.local-build/escuta-modos/`; a média é a do stem de vozes,
  e a faixa, a variação entre compassos):

  | seed | RITMO | GERATIVO antigo (= 100 %) | GERATIVO 30 % |
  |---|---|---|---|
  | 5EED2027 | −17,6 dB, faixa 1,7 | −28,2 dB, faixa 14,3 | −19,7 dB, faixa 2,8 |
  | C0FFEE42 | −22,8 dB, faixa 5,3 | −32,6 dB, faixa 13,9 | −24,9 dB, faixa 7,1 |

  A 0 % os dois perfis são idênticos ao RITMO; a 100 %, idênticos aos
  arquivos `-gerativo` antigos.
- **Testes e validação:**
  - teste novo `batruc_generative_depth`, que inclui a contraprova: a 100 %
    um ataque escrito chega a pico 0, e a 30 % fica em 72 % do RITMO;
  - suíte 25/25;
  - build Release completo sem avisos;
  - round-trip do schema 60 no `--diagnose-midi-scene`;
  - captura do rodapé em 1080 e 1440 px sem sobreposição.
- **Escuta pendente** do autor: os arquivos `-gerativo30` contra `-ritmo` e
  `-gerativo`.

**Baixo com voz própria — lote 1 (12/11, "avance"; auditoria §7, item 5),
aguardando escuta.**
- **Código:** `src/core/BassLineComposer.{hpp,cpp}`. O baixo escreve a
  própria frase sobre as figuras dos 7 ciclos e entrega uma
  `BassPerformance` comum.
  - **Harmonia:** uma progressão de graus por seed (10 opções, um acorde por
    compasso), no tom da seed (`seedKey`, o mesmo gene "KEY1" do app).
  - **Ritmo:**
    - fundamental no primeiro tempo de cada compasso;
    - apoio em 35–85 % dos ataques do bumbo;
    - resposta no contratempo nas lacunas do bumbo (25–75 %).
  - **Alturas:**
    - notas do acorde perto da anterior (contorno conjunto);
    - fundamental ou quinta nos tempos fortes;
    - uma nota de passagem antes de cada troca de acorde.
  - **Forma:**
    - o ciclo 3 respira (só tempos fortes);
    - o ciclo 4 troca um acorde pelo relativo;
    - o ciclo 7 termina na dominante.
- **Offline:** `--baixo-frase` (exige `--figuras`).
- **Teste novo:** `batruc_bass_line`. Verifica tom, tônica inicial,
  fundamental em cada compasso, 7 ciclos, respiração, determinismo e 12 seeds
  com 12 linhas diferentes. Suíte 26/26; build Release completo sem avisos.
- **A/B** em `.local-build/escuta-baixo/` (A = baixo atual derivado da
  agenda, B = frase própria), seeds 5EED2027, C0FFEE42 e D3BFA3CF:
  - A: 12–14 notas por compasso, 25–31 alturas cromáticas (o render offline
    não aplica o tom);
  - B: 3–4,4 notas por compasso, 12–14 alturas, todas no tom;
  - o baixo do B soa 1,3–2,3 dB mais alto (RMS); o mix varia menos de 0,6 dB.
- **No app como opção (12/11, "avance"), desligada por padrão:** botão
  **FRASE** no painel Baixo, ao lado do balanço, com dica nas 4 línguas.
  - Só acende com seed de figuras (sem MIDI e sem edição manual da V1).
  - Ligado, troca as notas do baixo da seed pela frase; o percurso
    calculado continua no gráfico.
  - Cena schema 61 (`bass_phrase`).
- **Ajuste:** o primeiro tempo de cada compasso puxa a linha para o centro
  do registro. Antes, a seed 00000001 ficava presa no Mi 52; agora fica em
  torno do Mi 40.
- **Validação:**
  - `--diagnose-bass` mostra o A e a FRASE das 6 seeds: as 6 linhas da
    FRASE foram aceitas pelo renderizador (75–122 notas, contra 324–487 do
    baixo atual);
  - round-trip do schema 61;
  - capturas em 1080, 1440 e 1920 px sem sobreposição;
  - suíte 26/26, build Release sem avisos.
- **Padrão só depois do OK da escuta.** Pontos a decidir ouvindo:
  - se a densidade de 3–4 notas por compasso está boa;
  - quanta presença dar à passagem e às respostas.

**Percussão no tom da seed — lote 1 (12/11, "avance"), aguardando escuta.**
Pendência registrada no lote do tom ("as membranas e os toms ainda não
seguem o tom").
- **Código:** `ResonatorRenderer::setPercussionKey(tônica, modo)`. Desligado
  (tônica −1) reproduz o comportamento anterior bit a bit.
  - Cada ataque de membrana/toms, conga, agogô, wood block e caixa vai à
    nota do tom mais próxima.
  - O bumbo vai à tônica ou à quinta mais próxima (no máximo ±3,5 semitons
    dos 55 Hz).
  - Chimbais, prato e shaker não mudam.
  - A correção multiplica a altura da nota, então os toms MIDI 41–50 e a
    variação da membrana continuam valendo, só que no tom.
- **Offline:** `--afinar` (usa `seedKey`, o mesmo tom do baixo).
- **Teste novo:** `batruc_percussion_key`.
  - Sem tom, o áudio é idêntico ao anterior.
  - Em Fá♯ maior, a membrana vai de 144,8 Hz a 138,6 Hz (Dó♯3, nota do tom).
  - O chimbal fica idêntico.
  - Suíte 27/27; build Release completo sem avisos.
- **A/B** em `.local-build/escuta-afinacao/` (A sem tom, B no tom), com 5
  vozes: bumbo, caixa, chimbal, chimbal aberto e membrana.
  - Seeds 5EED2027 (Si♭ lídio), C0FFEE42 (Lá dórico) e D3BFA3CF (Dó lídio).
  - O nível é idêntico (a mudança é só de altura).
- **Ainda não está no app.** Depois da escuta: opção no app (como o FRASE)
  ou padrão. Também a decidir: se a caixa deve mesmo ser afinada.

**Outros modelos escrevem ritmo — reordenação, camada 3 (12/11, "avance"),
aguardando escuta.**
- **Código:** `FigureComposer` generalizado. O modelo entrega um sinal por
  voz, e a tradução em trastes, a forma da frase, a ruptura e o encaixe
  continuam os mesmos.
  - `FigureModel`: pêndulos (o original), bilhar, Lissajous, Fourier
    (quadrada ou triangular, 2–8 termos) e curvas rolantes (epi e
    hipocicloide, cardioide, astroide, ranunculoide, trocoide), todos do
    `GeometryCurves`.
  - Nas curvas, um ponto percorre a curva e as três vozes são as suas
    sombras: V1 em x, V2 em y, V3 na diagonal. O ponto de retorno de cada
    sombra (a tabela do bilhar, a volta da Lissajous) é ataque acentuado.
  - A velocidade do percurso respira (±30 %, 1 a 2 ondas na peça). A
    duração da volta é calibrada para a V2 ter de 4 a 10 ataques por
    compasso, conforme a energia da seed. Sem isso a grade enchia.
  - **O pêndulo saiu idêntico:** 8 seeds × 4 compassos (12/14/16/20 passos)
    com o mesmo hash de células antes e depois.
- **Offline:** `--modelo pendulo|bilhar|lissajous|fourier|rolante|seed`
  (`seed` = gene próprio "MDL1").
- **Teste:** `batruc_figure_composer` estendido. Os 4 modelos são válidos,
  determinísticos e diferentes do pêndulo, a V1 abre cada frase e a
  densidade fica entre o vazio e a grade cheia. Suíte 27/27; build Release
  sem avisos.
- **Diversidade medida** (8 seeds, média por voz):

  | modelo | intervalos distintos | compassos distintos (de 28) |
  |---|---|---|
  | pêndulos | 7,1 | 16,9 |
  | bilhar | 5,9 | 19,0 |
  | Lissajous | 6,4 | 16,5 |
  | Fourier | 7,5 | 23,4 |
  | rolantes | 6,4 | 22,1 |

- **A/B** em `.local-build/escuta-modelos/`: seeds 5EED2027 e C0FFEE42, os
  5 modelos, modo RITMO, 3 vozes (bumbo, caixa, chimbal).
- **A observar na escuta:** em algumas combinações a V1 enche todas as
  colcheias (bilhar na 5EED2027, Lissajous 2:1 na C0FFEE42).
- **Ainda não está no app.** A decidir depois da escuta:
  - se a seed escolhe o modelo, o que muda o ritmo de todas as seeds;
  - ou se o modelo vira escolha no Study, ao lado de "Escrever figuras do
    pêndulo".

**Ideia do autor, 12/11 (anotada, não implementada):** "cada família de
estilos musicais deveria ter sua própria ginga (feeling, groove, swing)".
- **Hoje:** a família muda só o timbre. O groove é um seletor de 4 modos
  genéricos, e o swing é um controle global.
- **Base de pesquisa (13/11):** o dossiê §4b traz o idioma da caixa no
  frevo, no maracatu e no baião (Galvão, *Per Musi* 2023).
- **Desenho possível**, a propor ao autor: uma assinatura de ginga por
  família, aplicada como desvio visível (camada 2 da reordenação), com:
  - microtempo por passo do compasso;
  - perfil de acentos;
  - quanto o baixo segue a ginga.

**Ornamentos da caixa no gráfico (12/11):** o autor achou "muitos toques no
gráfico". Proposta em aberto, aguardando escolha:
- **(1)** só no desenho: um ponto e um sinal (`///` rulo, `ƒ` flam, `d`
  drag, `r` rufo, `⋰` ricochete, `°` fantasmas, `×` aro);
- **(2)** o ornamento como propriedade da nota, com os toques gerados ao
  tocar, como na partitura. Esta é a recomendada.

**Partitura do baixo · 16 tempos (12/11; o autor: "desenvolva a partitura
do baixo por compassos ou sequência de 4 compassos … visível acima do
gráfico na aba baixo … os 16 tempos").**
- **App:** faixa `BassScoreStrip` na linha do título "03 · BAIXO /
  COMPOSIÇÃO", acima do piano roll. É só de leitura e mostra o que o baixo
  toca no ciclo:
  - a mesma `BassPerformance` publicada para o áudio, incluindo o baixo da
    seed e a FRASE;
  - os 16 tempos, com barras e números de compasso;
  - o nome das notas quando cabe e o cursor;
  - parado, um clique mostra o próximo ciclo (C1–C7); tocando, segue o
    ciclo.
- O editor de um compasso continua igual.
- **Validação (13/11):**
  - build Release sem avisos; suíte 27/27;
  - capturas em 1080, 1440 e 1920 px sem sobreposição;
  - notas com traço mínimo legível (6 × 5 px), acima da linha dos números
    de compasso;
  - em 5/4 a faixa mostra 20 tempos (4 compassos × 5).

**Texto e rótulos na tela Compor (13/11, "avance"; pendência dos cortes).**
- **Caracteres trocados:** textos com · → λ × — ✎ φ ζ ● entravam em
  `juce::String` sem `utf8()` e apareciam embaralhados. Exemplos: "A Â·
  GAUSS", a fórmula "x,yâF=…", "MANUAL â/11". Corrigidos cerca de 45 pontos:
  - legenda e fórmula da intensidade;
  - status do Study;
  - nomes curtos das leis;
  - resumo GMD;
  - dica do editor de pan;
  - passos do baixo;
  - texto do pêndulo.
  Os textos dentro de `localized()` já estavam certos.
- **Bug da lei B:** a lista usava `getItemText(id)` em vez da posição. Cada
  lei B aparecia com o nome da seguinte, e "Energia cinética" ficava sem
  nome (a caixa vazia das capturas).
- **Rótulos:**
  - "λ · intensidade" (só em português, desenhado embaixo do controle)
    virou "λ", entre a caixa e o controle;
  - "0.48 B" aparece inteiro;
  - a linha de status do Study cabe inteira;
  - o título do gráfico ("ONDA / f(t) · …") cede o espaço medido do texto
    da passagem e não se sobrepõe mais a ele.
- **Ainda cortados:** os nomes longos nas caixas estreitas ("Catenária ·
  corrente s…", "Gauss · cen…"). A lista aberta mostra o nome inteiro.
- **Validação:** build sem avisos, suíte 27/27, capturas em 1080 e 1440 px.

**Espanhol completo (13/11, "avance").** `localized()` recebe pt/en/fr e
busca o espanhol num dicionário; o que faltava caía no inglês. Eram 379
textos distintos (de 663 chamadas), a maioria anterior a esta sessão. Todos
foram traduzidos e inseridos no dicionário.
- **Verificação:** 0 textos sem espanhol; build sem avisos; suíte 27/27.
- **Restam** 7 chaves repetidas, anteriores a esta sessão, que valem pela
  primeira entrada: "Parar"→"Parar" (e não "Detener"), "Extremos",
  "Triangular", "Quadrada", "Triângulo", "Áreas · Stengel" e o texto das n
  posições.
- **Revisão:** as traduções são minhas. Uma revisão por falante nativo fica
  como pendência.

**Ornamentos e notação (13/11).** O autor: "os ornamentos precisam ser
investigados em estudos de ritmos, percussão, baterias, caixa"; "como o
batruc pode incorporar esses recursos (algo como sistemas de notação) para
facilitar a execução, sugira algo". A proposta está na conversa de 13/11 e
aguarda a escolha do autor. **Decisão (13/11, "sim"):** primeiro o dossiê.
Feito: [`ESTUDO_ORNAMENTOS_CAIXA.md`](ESTUDO_ORNAMENTOS_CAIXA.md).
- Os 40 rudimentos PAS (lista oficial lida no PDF; figuras descritas com
  palavras próprias; 5 manulações marcadas "a conferir").
- Stone, Wilcoxon e Gramani como método.
- A caixa surda no samba (Leppaus, ANPPOM 2019): articulações, "condução
  implícita", levada + variação.
- Notação: pauta PAS/Weinberg, TUBS, sinais brasileiros, contagem.
- Proposta em 6 etapas.

**Próximo passo:** o autor confirma a etapa seguinte (tabela de rudimentos
com o ornamento como propriedade da nota). Antes, conferir as 5 manulações
marcadas.

**Mixer: sobreposição do editor de trajeto (13/11; o autor: "há
sobreposições em mixer (trajeto - desenho)").**
- **Causa:** o editor TRAJETO · CAMPO/DESENHO (cena 56, 12/11) ocupava a
  faixa do cabeçalho desenhado de cada canal (V1…V8 + medidor) e do rótulo
  FÓRMULA, que ficavam embaixo dele. A auditoria automática não via, porque
  compara só componentes, não texto desenhado.
- **Correção:**
  - `kMixerPanPathRow` = 48 px desce o bloco da voz (controles e cabeçalho
    desenhado);
  - as alturas do Mixer sobem igual: 1270 / 1060;
  - o título do editor cabe inteiro (`drawFittedText`).
- **Validação:** capturas em 1080 e 1920 px; auditoria com 0 sobreposições;
  suíte 27/27.

**Notação: código já existente na família (13/11; o autor: "verifique os
códigos já elaborados no maravi"; "há muita coisa codada por lá").**
- **Antitotem** (C++/JUCE): `writeMusicXmlCaptureToFile`
  (`ANTITOTEM/src/app/Main.cpp:1146–1312`) quantiza, decompõe em figuras e
  liga através da barra. O estudo `PESQUISA_REPRESENTACAO_MUSICAL_RASGO.md`
  propõe o RASGO Score (partitura / execução / interpretação).
- **Maravi** (Python): `tools/musicxml_writer.py` tem:
  - 24 subdivisões por tempo;
  - escrita binária ou quiáltera decidida tempo a tempo;
  - pontos e anacruse;
  - grafia dos acidentes pela armadura;
  - clave de Fá.

  `webapp/score_preview.py` desenha a pauta em SVG sem fonte (clave,
  armadura, fórmula, hastes, colchetes, ligaduras, pausas; só clave de
  Sol). `tools/piano_score.py` faz duas pautas.
- **Sem escrita de percussão** em nenhum dos dois.
- **Proposta, aguardando o autor:**
  - núcleo `ScoreNotation` no Batruc: porte do C++ do Antitotem com as
    melhorias do Maravi;
  - pauta do baixo em clave de Fá no lugar da faixa "PARTITURA";
  - depois, MusicXML do baixo e a pauta de percussão da caixa.

  Os repositórios oficiais ficam só como fonte de leitura.

**Pauta tradicional do baixo (13/11, "avance").**
- **Núcleo `src/core/ScoreNotation.{hpp,cpp}`**, porte com crédito:
  - do ANTITOTEM `writeMusicXmlCaptureToFile`: decomposição gulosa em
    figuras com ponto e ligadura através da barra;
  - do MARAVI `musicxml_writer.py`: grafia pela armadura;
  - acréscimo: nada fora do tempo atravessa o tempo, e em compasso par a
    metade só é cruzada a partir do 1º tempo;
  - `keyFifthsForMode`: armadura pelo tom e pelo modo da seed;
  - acidentes e bequadro dentro do compasso, sem repetir em nota ligada.
- **App:** a faixa "PARTITURA" virou **PAUTA**, no mesmo lugar acima do
  gráfico. O desenho é em traço, como `score_preview.py` do MARAVI:
  - clave de Fá com "8" (baixo elétrico, escrito uma 8ª acima);
  - armadura, fórmula de compasso, 4 compassos;
  - cabeças cheias e vazias, hastes, colchetes por tempo (2ª barra de
    semicolcheia), bandeirolas, pontos, ligaduras;
  - linhas suplementares, ♯ ♭ ♮ e pausas;
  - cursor; o clique troca o ciclo quando parado.
- **Validação:**
  - teste `batruc_score_notation`: soma por compasso, figuras, ligadura,
    tempo, armaduras por modo, acidentes, 7/8;
  - suíte 28/28; build sem avisos;
  - captura em 1080 px (Si♭ lídio, 7/8).
- **MusicXML do baixo (13/11, "avance"):** `musicXmlForBars` no núcleo.
  - A peça inteira (7 ciclos × 4 compassos), em MusicXML 4.0 partwise.
  - Clave de Fá, armadura, compasso e andamento, com `<transpose>
    octave-change −1` (baixo elétrico escrito uma 8ª acima).
  - Ligaduras, pontos, acidentes e barra final.
  - **App:** clique direito na pauta → "Exportar MusicXML (7 ciclos)…".
    Abre a janela de salvar (nome `batruc-baixo-SEED.musicxml`) e pede
    confirmação antes de substituir um arquivo.
  - Rótulo da pauta nas 4 línguas.
- **Validação:**
  - teste estendido: 28 compassos, uma `<note>` por figura, clave,
    armadura, transposição, ligaduras pareadas, barra final;
  - `--diagnose-bass` grava `<relatório>.musicxml`: XML válido, 28
    compassos com a soma certa (5/4);
  - o **MuseScore** (`mscore -o`, sem janela) abriu e desenhou a partitura
    (5/4, 2♯, ♩=120);
  - suíte 28/28; build sem avisos.
- **Próximo:** pauta de percussão da caixa, com os sinais e a manulação do
  dossiê.

**Ornamento como propriedade da nota (13/11, "avance"; dossiê §6, etapa 2;
o autor: "ficou muitos toques no gráfico … mantém uma só marcação").**
- **Núcleo:**
  - `RhythmicCell::ornaments` guarda o código do ornamento (tipo + 1) em
    cada nota escrita;
  - `markSnareOrnaments`: flam, drag, rufo, ricochete e rulo **marcam** a
    nota; fantasmas e aro continuam escrevendo (são notas e articulação);
  - `expandSnareOrnaments`: célula escrita → célula tocada, com os mesmos
    toques da inserção antiga (`ornamentStrokes`, compartilhado).
- **App:**
  - o Study e as cenas guardam a nota com o sinal;
  - `tryPublishRhythmicCells` envia ao áudio a célula expandida, e o
    renderizador não mudou;
  - mudar BPM ou compasso refaz a célula tocada (os enfeites são em ms);
  - no gráfico do Study, um ponto por nota com sinal ("///" rulo, f flam,
    d drag, r rufo, ri ricochete);
  - cena schema 62: `ornament` por ataque e 3º elemento nas frases por
    ciclo.
- **Validação:**
  - teste: marcar + expandir dá a mesma célula tocada que a inserção antiga
    em flam, drag, rufo e rulo; a escrita não ganha toques; a tocada não
    carrega marca;
  - round-trip da cena 62;
  - captura: a V2 com rulo mostra um ponto e "///";
  - suíte 28/28; build sem avisos.
- **Limites:**
  - operações do Study que recriam ataques (redistribuir, recombinar) não
    levam o ornamento;
  - o render offline não aplica os ornamentos;
  - o ricochete expandido usa 6 rebotes fixos (antes dependia da
    quantidade).
- **Etapa 3 feita (13/11, "avance"): pauta de bateria no Study.**
  - O chip "♩ PAUTA ⇄ ◯ CÍRCULO", no canto do gráfico do modo STUDY, troca
    o círculo pela pauta. Ela é só de leitura; editar continua no círculo.
  - V1–V3 em pauta de percussão (clave de duas barras):
    - bumbo no 1º espaço, haste para baixo;
    - caixa no 3º espaço e chimbal × acima da pauta, hastes para cima;
    - posições por som (toms, ride, aro, shaker).
  - Na caixa: notinhas de flam, drag e rufo; barras do rulo; pontos do
    ricochete; `>` acima de 0,85; `( )` abaixo de 0,35; aro ×; manulação
    D/E alternada pela grade, em pt/en/es/fr (D/E, R/L, D/I, D/G).
  - Ritmo escrito por `ScoreNotation`, sem ligaduras: a duração vai até o
    próximo ataque ou o fim do tempo, e o resto vira pausa.
  - Um compasso por linha quando há altura (1440/1920); dois em 1080.
  - A auditoria de layout grava `<largura>-drum-staff.png`.
  - Validação: capturas em 1080 e 1920 px; suíte 28/28; build sem avisos.
  - **Ainda não:** a manulação não segue o rudimento (paradiddle etc.).

**Pendências resolvidas (13/11, "avance nas pendencias").**
- **Ornamentos nas operações do Study:** o limite registrado antes estava
  errado. Todas as operações do núcleo copiam o ataque com
  `copyRhythmicAttack`, que leva o ornamento. Teste novo: rotação,
  redistribuição e redução mantêm as marcas.
- **MusicXML da bateria:** `musicXmlForDrumStaff` no núcleo.
  - Parte de percussão em duas vozes (mãos para cima, pés para baixo).
  - Cabeça ×; notinhas de flam (cortada), drag e rufo; rulo como trêmolo
    de 3 barras; acento; fantasma entre parênteses; manulação como texto
    embaixo (D/E, R/L, D/I, D/G).
  - App: clique direito na pauta de bateria do Study → "Exportar MusicXML
    (bateria)…".
  - `--diagnose-bass` também grava `<relatório>-bateria.musicxml`.
  - O MuseScore desenhou um teste e a seed de diagnóstico sem erro.
- **Espanhol:** as 7 chaves repetidas foram removidas (as que valiam
  continuam).
- **Dossiê:** 4 das 5 manulações "a conferir" foram confirmadas em fontes
  de ensino (Single Flammed Mill, Pataflafla, Swiss Army Triplet, Inverted
  Flam Tap). Flam Drag continua sem fonte em texto, e no Single
  Dragadiddle as fontes divergem.
- **Validação:** suíte 28/28; build sem avisos.

**ORN por stem: batida normal ou caixa com recursos (13/11; o autor: "alguns
seeds deixam a caixa extremamente exagerada"; "um mecanismo no stem em que
se possa escutar a batida normal e uma vez ligada … a caixa com os rufos e
outros recursos de caixa").**
- **Núcleo:**
  - fantasmas (notas acrescentadas) e aro (nota que virou GM 37) também
    ficam marcados;
  - nota fantasma e nota de aro não recebem outro enfeite por cima;
  - `expandSnareOrnaments(..., enabled)`: desligado, toca a batida normal
    (sem enfeites, sem fantasmas, aro de volta ao toque normal); as marcas
    ficam na célula escrita.
- **App:**
  - botão **ORN** em cada stem do Mixer, ao lado de M e S; ligado por
    padrão;
  - a escolha afeta a célula tocada da voz e das 7 frases;
  - dica nas 4 línguas;
  - cena schema 63: `ornaments` por canal;
  - no Study, a nota fantasma e a de aro não ganham sinal (só flam, drag,
    rufo, ricochete e rulo);
  - o liga/desliga provisório no Study (nunca compilado) foi retirado.
- **Render offline com a caixa da seed (13/11):** o sorteio do vocabulário
  saiu do app para o núcleo (`seedSnareVocabulary`, gene "SNR1"), com o
  mesmo resultado.
  - `--caixa` marca e expande a V2 de cada ciclo como o app; desligado por
    padrão para não alterar os A/B já gravados.
  - Conferido: C0FFEE42 → vocabulário 3 (ricochete, 44 %).
- **Validação:**
  - teste: desligado volta à batida escrita e tira o aro; ligado tem
    fantasmas, aro e rulo;
  - round-trip da cena 63 (canal 2 desligado);
  - captura do Mixer em 1080 px; auditoria com 0 sobreposições;
  - suíte 28/28; build sem avisos.

**Ginga da caixa por gênero — lote 1 (13/11, "avance"; dossiê §4b),
aguardando escuta.**
- **Núcleo:**
  - `SnareIdiom` + `applySnareIdiom`, só com o que a fonte documenta (Galvão,
    *Per Musi* 2023):
    - **frevo:** rulo de 1 tempo, acentuado, no último tempo de cada frase
      de 2 compassos;
    - **maracatu:** toque múltiplo e acento na 2ª nota de cada tempo;
    - **baião:** acento na 2ª colcheia do 2º tempo e notas fantasmas.
  - Ornamento novo **toque múltiplo** (`SnareOrnament::buzz`): 4 rebotes a
    ~12 ms. Aparece no menu Ornamentar, como "z" no Study e na pauta, e
    como `tremolo type="unmeasured"` no MusicXML. Os códigos antigos não
    mudaram (as cenas continuam válidas).
- **Offline:** `--idioma frevo|maracatu|baiao`.
- **Testes:** frevo com 2 rulos no lugar certo; maracatu com 16 toques
  múltiplos na 2ª nota do tempo, acentuados; baião com 4 acentos; células
  válidas; o toque múltiplo expande. Suíte 28/28.
- **A/B** em `.local-build/escuta-ginga-caixa/`: 5EED2027 e C0FFEE42 ×
  (sem, frevo, maracatu, baião), em modo RITMO. O nível fica igual (até
  +1 dB), pico 0,86.
- **No app como ação opcional (13/11, "avance"):** menu RITMO do Study →
  "Ginga da caixa (gênero)" → frevo, maracatu ou baião. Aplica na voz
  selecionada e nas 7 frases dela, com desfazer; nenhuma seed muda sozinha.
- **Microtempo (13/11):** dossiê §4c. Davies et al. (ISMIR 2020, CC BY 4.0)
  medem o tarol do baque solto em [0,25 · 0,47 · 0,715] por tempo (3ª e 4ª
  semicolcheias adiantadas ~3 %). É base para a outra metade da ginga.
- **Microtempo medido, opcional (13/11, "avance"):**
  `applyMicrotimingProfile` escreve o desvio nas posições, sem oscilação
  (o artigo não dá números para ela).
  - Perfis: tarol [0,47 · 0,715] e bombo [0,46 · 0,69] para a 3ª e a 4ª
    semicolcheia do tempo.
  - Só move ataques na grade (±30 % de passo); profundidade 0–1.
  - App: RITMO do Study → "Microtempo medido" → tarol ou bombo · baque
    solto (voz selecionada e 7 frases, com desfazer).
  - Offline: `--ginga 0..1` (bombo em V1, tarol em V2).
  - Teste: posições exatas na mediana, 50 % de profundidade, a 2ª
    semicolcheia fica, células válidas. Suíte 28/28.
  - A/B em `.local-build/escuta-ginga-microtempo/` (`-ginga0` contra
    `-ginga1`, 5EED2027 e C0FFEE42, modo RITMO; 145 e 201 ataques
    deslocados na peça).
- Depois da escuta, decidir:
  - como o idioma entra por padrão: por família de estilo, ou pela seed;
  - a outra metade da ginga (microtempo e acentos), que depende de fontes
    ainda não lidas (Thalwitzer).
- **Limites:**
  - a célula característica do frevo (Saldanha 2008) e os baques completos
    do maracatu (Santos e Resende 2005) não foram lidos: os idiomas usam só
    o que Galvão descreve;
  - o aro em dois timbres não foi feito.

Princípio: **a geração escreve, não filtra**. O que é gerado vira ritmo
escrito e editável (com A/B e desfazer), e o código de cálculo existente
passa a alimentar a camada 3 em vez de agir escondido.
`AUDITORIA_CODIGO_SUBUTILIZADO.md` vira a lista de chegada de cada módulo:
nada é apagado sem lugar definido.

**Trajetórias manuais de pan por stem — 12/11.** Cada canal do Mixer agora
mostra um editor cíclico de oito pontos, com eixo de seção, playhead em
execução e alternância entre **Desenho** e **Campo**. Arrastar desenha o
percurso independente; pan-base, MOV, VAR e intenção continuam modulando o
resultado por ataque. O renderer interpola a curva sem alocação no callback e
mantém o alvo entre ataques. Schema 56 guarda estado e pontos; cenas até schema
55 continuam no percurso de X do campo. O diagnóstico
`--diagnose-midi-scene=/tmp/batruc-pan-path-scene-roundtrip-20261112-final`
confirmou round-trip e fallback schema 55; `batruc_mixer_routing` passou (1/1),
incluindo um caso em que a curva manual prevalece sobre `fieldX` mesmo com
ataque no lado oposto. O build Release completo e a suíte CTest passaram
(18/18, 79,10 s). `git diff --check` passou. SHA-256 do app Release:
`6e49d0ac8124dc46aa2bec522672a8df09a2db65563eaa827cb1ece7f41d7c92`. A inspeção
visual no app e a escuta do desenho são pendentes.

**Render de referência de seeds MIDI — 12/11.** `batruc_midi_reference_render`
agora aceita `--seed 8hex --variation 0..1`, aplica `seededMidiPhraseCells()`
somente à cópia das células e registra no TSV seed, profundidade, contagem por
lane antes/depois e eventos. Preservei o caminho sem seed. Compilei o alvo
Release e gerei pares samba/Baião com family 12, ganho 0,7 e duas seeds cada em
`/tmp/batruc-midi-seed-audition-verified-20261112/`. Os diffs dos traços
confirmam mudanças determinísticas nos ataques e velocities; todos os WAVs mix
e stems foram gerados sem sobrescrever arquivos e tiveram RMS/peak PCM não nulos.
Repeti a seed `5EED2027` no samba: mix WAV e TSV tiveram hashes idênticos aos da
primeira renderização. Os arquivos usam ganho fixo, sem normalização
independente por seed. Build do renderizador passou;
`git diff --check` passou. SHA-256:
`2028cc071d6111fc50aba107f3885dc391ec4a8da813070ed65ea955d8909026`. A prévia
não inclui o pan seedado do Mixer nem valida autenticidade ou qualidade musical;
a escuta comparativa ainda depende do autor. Comandos e limites estão em
[midis/README.md](midis/README.md).

**Idioma da interface persistido — 11/11.** A preferência PT/EN/ES/FR agora
fica em `ui_language_v1` no arquivo local de configurações já usado por outras
preferências do Batruc. O construtor restaura o idioma antes de montar e
localizar os controles; trocar o seletor salva a opção sem alterar cenas. Build
Release compilado e `git diff --check` passou. SHA-256 do app:
`a24711f9dd57b9e481b4479b0ecbed3a23f48efe951984a222122c6d5c3e6271`. Teste de
reinicialização e inspeção dos quatro idiomas na GUI ainda pendentes.

**Tercina no explorador Aprender — 11/11.** A aba Contagens agora inclui
divisão em três partes iguais, com prévia sonora e prática usando os oito
padrões possíveis de uma batida. O preset de compasso alterna entre 4/4 em 16
semicolcheias e 3/4 com tercinas em nove posições sobre três tempos. A prévia
4/4 toca em quatro tempos; a ternária toca em três. O sorteio de padrão mantém
o preset e o exercício guarda/restaura essa escolha. O texto didático explicita
que uma batida em tercina tem `2³ = 8` padrões, enquanto um compasso 3/4 com
tercinas tem `2⁹ = 512`: a fórmula `2ⁿ` é a mesma; muda o número total de
posições. A localização da explicação cobre PT/EN/ES/FR; a caixa apresenta “3
posições · tercina/triplet/tresillo/triolet”. Build Release compilado;
`git diff --check` passou. SHA-256 do app:
`9eb2b8ac3c05c85c720ec007edb61366cd2f1627256990231ab06826a2aa7f41`. A
validação visual e a conferência auditiva do exercício continuam pendentes.

**Colisão máxima no histórico de seeds — 11/11.** A escolha automática agora
usa uma função do core que percorre propostas distintas e exclui as 64 seeds
recentes mais a seed atual mesmo se ela ainda não estiver na lista persistida.
Antes, o limite de tentativas contava apenas o histórico e não cobria esse
caso-limite. A regressão constrói precisamente 64 colisões históricas e uma
colisão atual; `batruc_seeded_recipe` passou (1/1). O app Release recompilou e a
suíte CTest serial passou 18/18; `git diff --check` passou. SHA-256 do app:
`712918c45f0ba1e9ad2270b4a803e99f7fdeddac1cb1afbeaea57b86225b4564`. A
persistência continua local às preferências da instalação; abrir em outra
instalação não compartilha esse histórico. Confirmar várias aberturas reais
segue como ensaio de GUI pendente.

**Panorama seedado por papel no MIDI — 11/11.** Antes, a seed não produzia
panorama para os stems importados; todos herdavam valores estáticos/manuais, e
o movimento era igual por padrão. Agora uma função determinística deriva pan e
movimento moderados por papel a partir da seed: bumbo e caixa permanecem
centrados, enquanto chimbais, tons, pratos, percussão de mão e acessórios
recebem posições distribuídas e movimento limitado. A aplicação atualiza os
controles do Mixer e os valores usados pelo renderer; o músico pode alterá-los
depois. O build Release completo e a suíte CTest serial passaram: 18/18,
incluindo `batruc_midi_rhythm_import` (1/1). Depois fortalecei a fixture para
exigir chimbais/pratos em lados opostos, percussão/acessórios complementares e
limites de movimento distintos por papel; o teste focal passou novamente
(1/1). `git diff --check` também passou.
SHA-256 do app Release: `ab44ba24328f81adffae8e578c845a73d6601a6caf2dee3bfb113642e2596eba`.
O trajeto automático usa a posição horizontal dos ataques no campo comum e é
modulado por papel, intenção e profundidade. **Atualização em 12/11:** o Mixer
agora também oferece trajetória cíclica desenhada por stem, com playhead,
alternância Campo/Desenho e persistência schema 56. A fixture
`batruc_mixer_routing` cobre campo à esquerda, à direita e ao centro, VAR=0 e
um percurso manual que prevalece sobre `fieldX`; o round-trip de cena e o
fallback schema 55 também passaram. O build Release completo e o CTest passaram
18/18. Isso não substitui a inspeção visual no Mixer nem a escuta, ainda
pendentes.

**Precisão do texto de ajuda do pan — 11/11.** Corrigi o tooltip PT/EN/FR do
controle MOV: o renderer atualiza o alvo de pan a cada ataque e mantém a
posição até outro ataque; o retorno ao pan-base ocorre quando o canal fica
inativo, não em qualquer pausa musical. A alteração alinha a explicação ao
comportamento existente. O editor de trajetória por stem foi implementado em
12/11 (schema 56); sua inspeção visual e avaliação auditiva continuam
pendentes.

**Round-trip de célula manual e leis de intensidade — 12/11.** Ampliei o
diagnóstico headless de cena para gravar e reler uma célula rítmica editada,
seed/variação, leis A e B, blend e profundidade, além dos dados MIDI já cobertos.
O Release recompilou; `--diagnose-midi-scene=/tmp/batruc-midi-scene-schema54-roundtrip-20261111`
retornou `midi_scene_round_trip: true`, sem erro. O mesmo diagnóstico migra uma
cópia schema 54 e confirma que ela mantém célula manual, seed, leis/blend, CC4
e colisões MIDI, sem inventar os metadados editoriais de schema 55.
Incluí também schema 41, anterior à mistura de duas leis: a migração mantém a
célula manual e configura lei B = lei A, blend 0. Schema 48 guarda a nota MIDI
principal, mas antecede CC4 e colisões; schema 52 preserva também CC4, mas não
as colisões. Os diagnósticos confirmam essas fronteiras e mantêm ausentes os
campos e metadados adicionados depois de cada versão. Acrescentei os marcos
44–47 e verifiquei a progressão: referência/células-fonte; SHA-256/estado de
direitos; licença/atribuição; versão/histórico de operações. Os casos 41, 44,
45, 46, 47, 48, 52, 54 e 55 passaram em
`--diagnose-midi-scene=/tmp/batruc-midi-scene-schema44-47-20261111`
(`midi_scene_round_trip: true`); `git diff --check` passou. Isso cobre o
round-trip atual e esses marcos de migração, não toda a série histórica. A GUI,
edição após restauração e outros esquemas antigos continuam pendentes.
SHA-256 do app Release:
`3aaec60b51664ba2d6870794e5352b99ad13c99e2460e0bc4fac6cd374518929`.

**Revalidação do baixo seedado — 11/11.** Recompilei os alvos focados e rodei
`batruc_ecological_bass_composition`, `batruc_seeded_bass_parity`,
`batruc_seeded_voice_bass_parity` e `batruc_cyclic_bass_parity`; os quatro
passaram em 72,15 s. Isso confirma determinismo, variação rítmica e paridade
dos agendadores cobertos pelas fixtures, não a percepção musical no dispositivo.
Escuta comparativa de seeds e níveis diferentes continua pendente.

**Regressão de fonte MIDI em cena — 11/11.** O diagnóstico de round-trip agora
prepara células-fonte com bumbo e chimbal, incluindo nota GM, CC4, colisão
polifônica, velocity e posição, e compara esses dados depois de serializar e
ler a cena. Acrescentei o modo CLI `--diagnose-midi-scene=DIR`, que cria o
componente sem abrir janela nem dispositivo de áudio. O app Release compilou e
o comando foi executado: `midi_scene_round_trip: true`, sem erro; o relatório
e a cena schema 55 ficaram em `/tmp/batruc-midi-scene-diagnostic`. Corrigi
comparações de `double` para tolerâncias e os rótulos de seta para UTF-8
explícito. A inspeção visual e auditiva do estado restaurado continuam
pendentes.

**Mapa canônico de papéis MIDI — 11/11.** A paleta do Mixer, o som fallback do
renderer e a rotina de importação mantinham cópias do mesmo mapa V1–V8. Agora
compartilham `kMidiLaneDefaultSounds`, com uma asserção de compilação que exige
oito papéis para as oito lanes tocáveis. Recompilei o app Release e os testes
`batruc_seeded_recipe` e `batruc_midi_rhythm_import`; ambos passaram. A
regressão continua comparando as famílias contra os papéis esperados sem
depender da constante de produção. `git diff --check` passou. Isso evita
divergência de código, mas não substitui a escuta de cada stem.

**Mapa de tempo MIDI no recorte — 11/11.** O parser já localizava mudanças de
BPM, mas apenas mostrava um aviso e aplicava o primeiro andamento. Agora coleta
metaeventos de todas as pistas formato 0/1, integra o tempo por ticks até o fim
da janela selecionada e normaliza a posição dos ataques pelo tempo decorrido.
O BPM equivalente do recorte define a duração do ciclo quando cabe no controle;
se não houver metaevento ou o valor ficar fora de 20–220, mantém-se o BPM atual
e preserva-se a proporção interna. Mudanças após o primeiro ataque também
entram na integração. Testes incluem tempo em pista posterior às notas, mudança
em tick 1, janela com 120→90 BPM (equivalente 96 BPM e segundo ataque em 7/15
do ciclo), além de compasso e ataques existentes. `batruc_midi_rhythm_import`
passou (1/1), o build Release do app passou, e a suíte CTest serial passou
18/18 em 86,36 s. `git diff --check` passou. O SHA-256 do binário Release é
`1e678746b5cceccd6eefac73bd20eab9b070e7256e6f90fd70c9d5f657e2ce7d`.
O resultado é uma representação por fase normalizada: o mapa MIDI bruto não é
guardado como curva de andamento independente na cena. Escuta comparativa no
app ainda pendente.

**Seed não corta stems MIDI superiores — 11/11.** A aplicação de uma seed
reduzia `activeVoiceCount` ao número aleatório da receita mesmo com um MIDI
importado. O renderer usa esse limite para decidir que vozes podem soar; uma
fonte com ataques em V5–V8 podia, portanto, perder material ao trocar de seed.
Agora o limite seedado é o maior entre a receita e a última lane MIDI com
ataques, preservando todos os stems ocupados sem forçar oito vozes em arquivos
que usam poucas. `midiVoiceCountForCells()` e o caso de regressão com ataque em
V8 estão cobertos por `batruc_midi_rhythm_import` (1/1). O app Release compilou;
SHA-256: `f3c42fc68f2a8820e086aee374d6d54b2f917b43daf603c3bf204ad7a327e799`.
`git diff --check` passou. Isso confirma a política do limite no código, mas
não substitui ouvir a mudança de seed em um MIDI com material nas lanes altas.

**Seed com contexto editorial MIDI — 11/11.** Ao importar MIDI com rótulo de
estilo conhecido no índice GMD local, a seed aplica uma família e um groove
iniciais correspondentes a regras conservadoras para rock, jazz, funk, samba,
baiao e demais rótulos `latin/`. Isso não muda os onsets importados, não atesta
autenticidade de gênero e não bloqueia edição. Rótulos desconhecidos mantêm a
seleção comum da seed. A escolha contextual aparece no tooltip da seed; o texto
é reconstruído a cada seed para evitar duplicações. Importar agora seleciona
3/4, 4/4, 5/4 ou 7/8 quando disponível; ao gerar outra seed com MIDI ativo, o
compasso fica preservado. Para métricas fora dessas opções, o compasso de
trabalho permanece selecionado e o tooltip informa a diferença. O teste focal
`batruc_midi_rhythm_import` passou (1/1), o build Release serial de
`batruc_app` passou e `git diff --check` passou. SHA-256 do binário:
`dba49cd4a9a2e753841566088fc38d024e62cdc4cb4b5a4c79c0177363eb3012`.
O mapa é um ponto de partida editorial, não conhecimento musical validado por
escuta; conferir no app e rever as associações após comparação com as fontes.

**Compasso inicial do MIDI formato 1 — 11/11.** Corrigi o importador para
selecionar a assinatura de compasso válida mais cedo pelo tempo absoluto em
todas as pistas, desde que ela não comece depois do primeiro ataque de bateria.
Se começar depois, a janela inicial fica em 4/4; um compasso definido pelo
músico tem prioridade. Fixtures cobrem assinatura 3/4 em pista posterior,
assinatura tardia e override manual 5/4. `batruc_midi_rhythm_import` passou
(1/1); o app Release recompilou serialmente. `git diff --check` passou. SHA-256
do app:
`463f16d2bacbbe91e8df6584209d467866643610ed0e4f020dcd15b0e7b22b77`.
Mudanças de compasso após o primeiro ataque e escuta no app permanecem pendentes.

**Resumo da importação MIDI — 11/11.** A contagem exibida de stems agora mede
lanes que realmente têm ataques, sem confundir isso com as oito lanes que ficam
habilitadas para bloquear eventos gerados vazios. O tooltip passa a mostrar o
compasso aplicado e se o BPM de origem foi aplicado ou mantido. Build Release
serial de `batruc_app` passou; `git diff --check` passou. A conferência visual
do tooltip no app segue pendente. SHA-256 do app:
`2910ee5484654e905b62eeda57a17fcb6e7e14d0700cf80ea7e3758876e6395a`.

**Variação rítmica seedada do baixo — 11/11.** Nos níveis 2–3, o gerador pode
adicionar antecipações de semicolcheia antes da pulsação, além dos contratempos
de colcheia; limita novos ataques por compasso a 2/3 (nível 1: até 1). O teste
`batruc_ecological_bass_composition` agora exige que a mesma seed reproduza os
tempos, que outra seed altere o desenho rítmico, e que âncoras de bumbo e o
limite de densidade sejam mantidos. O teste focal e a paridade nativo/maquete
web passaram (1/1 cada); `node --check prototype/score.js` e
`git diff --check` passaram. O app Release foi recompilado serialmente; SHA-256:
`642da9e85827e79089eaa0e400573aa41dd9deadf6ed4ff6984b825381e3393c`.
A adequação e a percepção dessas antecipações dependem de escuta no app.

**Learn — mapa dos stems MIDI — 11/11.** O tópico **Som e modelo** agora
explica em PT/EN/ES/FR os papéis V1–V8, as notas GM correspondentes, as
exceções do mapa TD-11 no GMD e propõe escutar cada stem isoladamente para
identificar pulso, guia e resposta. A seed escolhe a família sonora sem mudar
esses papéis; o músico continua responsável pela máscara. A revisão confirmou
que a esteira sintética da caixa já existe no core; falta escutar e avaliar o
equilíbrio entre pele, esteira e aro. Build Release serial de `batruc_app`
passou. SHA-256: `15202346e9acd54684ab9493789e59845c5eb192ed6f1e72fa1a3310d93bc61b`.
`git diff --check` passou; não rodei a suíte nesta alteração de texto/tutorial.
Próximo passo: conferir no app as quatro traduções, a legibilidade e a escuta
dos stems.

**Sem passagens inteiras de silêncio — 11/11.** A forma longa mantém as oito
passagens ativas; o teste `batruc_variant_transport` cobre 64 seeds, todas as
passagens, 12 vozes e quatro posições por passagem e exige peso audível maior
que zero. O roteiro usa uma passagem de **Expansão** ativa no lugar da antiga
pausa. Corrigi também uma fonte separada de lacunas recorrentes: `RESPIROS`
descartava ataques fracos do baixo por probabilidade. Agora preserva o calendário
completo e articula as notas encurtando o gate em até 30%. Os modos de frase que
o músico escolhe explicitamente (`Respiração`, `Cadência`, `Chamada/resposta`)
continuam podendo criar pausas locais deliberadas; nenhum deles silencia uma
passagem inteira. Build Release serial e suíte CTest passaram **18/18** em
68,63 s; `node --check prototype/score.js` e `git diff --check` passaram.
SHA-256 do binário:
`3d6f66114784a99819cd6e98d34b1982564fd20b4a196d3d53337b54503d3741`.
Ampliei `batruc_variant_transport` para processar a agenda real do renderer em
64 seeds × 8 passagens completas de sete seções; cada passagem precisa produzir
energia de saída acima do limiar, além de manter pesos de evento positivos.
O teste focal passou (1/1, 0,77 s). Isso cobre a forma automática com a receita
inicial e níveis de stem abertos; mute deliberado continua podendo silenciar
uma voz. Escuta no app segue pendente.

**Regra reafirmada — 11/11:** nenhuma passagem da forma gerada pode ser
inteiramente silenciosa. A pausa voluntária do transporte continua sendo uma
ação do músico e, quando REC está ativo, seu intervalo é gravado como silêncio;
o texto de ajuda foi corrigido em PT/EN/FR para separar claramente esses dois
casos. A garantia da forma segue coberta pelo teste de energia renderizada para
64 seeds × 8 passagens completas. O alvo Release `batruc_app` compilou após a
correção; SHA-256 do binário: `ab3ba8e39fa5791eed3266cc0d9763b3c231fe356ff38d29c633d61110562ec8`.
`git diff --check` passou. Não repeti a suíte, pois a alteração de código foi
somente texto localizado.

**Densidade seedada por papel de stem — 11/11:** `seededMidiPhraseCells()` agora
rarefaz de modo determinístico apenas ataques fracos de apoio quando a lane tem
material suficiente. Os limites máximos variam de 10% a 24% conforme o papel;
bumbo e caixa mantêm o calendário, e ataques de downbeat, ataques fortes e grupos
de notas simultâneas/CC4 não são descartados (as velocities ainda recebem a
variação normal da seed). A fonte importada permanece intacta; a
mesma seed reproduz a seleção e seeds distintas podem abrir espaços diferentes.
O controle interativo em pares de lanes também está implementado; a escuta por
estilo continua pendente. Build Release do app e do teste MIDI passaram; teste focal
passou e a suíte completa CTest passou **18/18** em 69,58 s. SHA-256 do app:
`239db3618313a696b9c42da7e63ac845d5476978c59e010e24fb4dba37ba6fa1`.

**Operações rítmicas no Study — 11/11.** O botão **RITMO** da célula selecionada
abre redistribuição euclidiana (8, 12, 16, 24 ou 32 passos), microtiming
(±5, ±10 ou ±20 ms), redução de densidade para cerca de 75% ou 50% dos ataques
e interlocking com outra célula MIDI ativa. **Recombinar com** intercala metade
dos ataques das duas células escolhidas por seed, preservando o kit da voz
selecionada. O submenu **Desenho de intensidade**
aplica crescendo, decrescendo ou acentos alternados na célula selecionada;
**Equilibrar densidade com Vx** compara V3–V8 por compasso e rarefaz a lane mais
densa para perto de 1,25× a contagem da parceira.
A redução atua só na voz selecionada, conserva o primeiro ataque quando ele
está no tempo zero, prioriza velocities maiores e usa a seed para desempatar.
Notas GM simultâneas, CC4 e velocity das colisões permanecem agrupados no onset;
a operação entra no undo/redo e não altera a fonte MIDI preservada para novas
seeds. A redistribuição mantém o número de ataques, primeiro passo aproximado,
ordem cíclica e metadados. O microtiming segue determinístico e correlaciona
parcialmente deslocamentos vizinhos, limitados por acento e espaço disponível.
O interlocking compara a voz selecionada a outra voz com ao menos dois ataques
e leva sobreposições para o ponto médio mais próximo de um intervalo livre do
parceiro, sem deslocar mais de 1/8 do ciclo; se não houver espaço, conserva o
ataque. Onsets, notas simultâneas e dados MIDI acompanham o deslocamento. Os
testes focais verificam esses contratos e a redução discreta da densidade.
O app e os testes compilaram em Release; após acrescentar recombinação por
célula, a suíte CTest completa passou **18/18** em 82,76 s. `node --check
prototype/score.js` e `git diff --check` passaram. A avaliação musical ainda
depende de escuta. SHA-256 do app:
`a661e9278b05b6c2cf310b387f61279f2a2cff4333f26e70d0be40117e3db76f`.

**Adaptação de densidade entre stems MIDI — 11/11/2026:** o submenu **Equilibrar
densidade com Vx** adapta lanes de apoio V3–V8 compasso a compasso, reduzindo a
mais densa em cada par para perto de 1,25× a contagem da parceira. Preserva a
voz mais esparsa, downbeats e ataques mais fortes; compassos sem material
suficiente em ambas não mudam. O undo/redo trata o par como uma edição e o
histórico registra a operação. Teste focal MIDI e suíte CTest passaram (18/18 em
72,13 s); build Release do app passou novamente após revisar a ajuda localizada.
SHA-256 final do app: `c36ad8b41d1d4f963334e3272a499fef396ca38ae7827cab6176c794d1a2e750`.
A escuta comparativa no app segue pendente.

**Regressão de paletas com nível casado — 11/11/2026:** o teste de seed agora
renderiza o mesmo bumbo com cada um dos 17 perfis e compara os sinais após
normalizar o RMS, para que mudar apenas o volume não satisfaça a cobertura de
diferença de timbre. O alvo Release `batruc_seeded_recipe_test` compilou e o
CTest focal passou (1/1); `git diff --check` passou. A mudança não altera o app
Release; a audição e a validação estilística ainda dependem do autor.

**Filtros do catálogo GMD — 11/11.** A janela de ritmos locais agora filtra
por estilo editorial declarado, tipo `beat`/`fill`, compasso e faixa de BPM;
“todos” permite combinar os filtros sem forçar um estilo. Os rótulos vêm do
`info.csv` local e não são deduzidos do nome do MIDI. O filtro de densidade
agora conta ataques no primeiro compasso de cada arquivo, limitado a 64 MiB
por abertura do catálogo: baixa (0–8), média (9–16) e alta (17+). O item da
lista mostra a contagem; MIDI inválido ou não analisado fica marcado como
indisponível e não entra nas faixas de densidade. O índice contém 76 estilos
declarados, os tipos `beat`/`fill` e compassos 3/4, 4/4, 5/4, 5/8 e 6/8.
Build Release serial do `batruc_app` passou com o cálculo e os controles;
SHA-256 `2edd814022f7c8a158f0896bc740f64139d612aaf45beb964ae5a53b5120d376`.
A checagem `noteOnEvents - outsideWindowHits` foi acrescentada ao teste focal
do importador; `batruc_midi_rhythm_import` passou (1/1).
A densidade usa ataques da primeira barra conforme o compasso editorial, não
um rótulo inferido do arquivo. A leitura fica limitada a 64 MiB por abertura;
arquivos inválidos/fora do limite seguem listados sem densidade e não entram
nas três faixas. Falta inspeção visual do diálogo e conferência humana de que
os limites baixa/média/alta são úteis musicalmente.

**Validação atual — 11/11/2026:** build Release serial de `batruc_app`,
`batruc_midi_rhythm_import_test`, `batruc_seeded_recipe_test` e
`batruc_seeded_bass_fixture` passou; `node --check prototype/score.js` e
`git diff --check` também passaram. CTest serial completo: **18/18 passaram**
em 66,49 s.
As duas paridades do baixo que falhavam foram corrigidas: em
`prototype/score.js`, a agenda gerada recebia os contratempos dentro do mesmo
array dos ataques-fonte; assim, um contratempo recém-criado podia servir de
origem para outro, ao contrário da cópia independente usada no C++. A agenda
fonte agora fica intacta e a paridade voltou a passar sem mudanças nos fixtures.
Isso valida a correspondência algorítmica exercitada pela suíte, não substitui
escuta crítica do baixo nem avaliação musical dos níveis de variação.

**Seeds de abertura sem repetição — 11/11.** O app já sorteava a seed no
construtor, mas não guardava histórico para evitar que uma abertura seguinte
reutilizasse uma seed. Agora as preferências locais mantêm as 64 últimas seeds
de composição aplicadas. A seed de abertura, **Nova seed** e o retorno do modo
manual percorrem candidatos até encontrar um valor fora desse conjunto. Seeds
digitadas e seeds de cenas carregadas continuam explícitas e são registradas
para orientar a próxima escolha automática. O histórico é local à instalação e
não altera cenas salvas. Build Release serial de `batruc_app` concluído em
11/11; SHA-256 `f41d0d2a6b099bd604ff6295317c161cd98d8ec9ea4d553f1c2aac4f6884b534`.
Não executei testes. A confirmação de que várias aberturas sucessivas não
repetem continua pendente; o app aberto antes desta compilação precisa ser
reaberto para receber a mudança.

**Finalização imediata de edição no Study — 11/11.** A auditoria do fluxo
confirmou que células de percussão são publicadas para o próximo bloco de áudio
e que a nova performance do baixo preserva a posição atual, sem reiniciar a
forma. Durante o arraste, a recomposição do baixo continua coalescida em seis
ticks do timer de 60 Hz (~100 ms); ao soltar o gesto e após ajustes por teclado,
agora a recomposição final é solicitada imediatamente, em vez de esperar o
próximo tick do lote. O callback do áudio aplica a nova performance na próxima
chamada de áudio. Essa é uma conclusão do caminho de código; o tempo real até
a saída física e a ausência de pausas ainda não foram medidos. Build Release
serial de `batruc_app` concluído em 11/11; SHA-256
`9f01268ac623016ae473a1289e19e95ba4302a3c09faab5366e0a386c8548cc5`.
Não rodei testes nem fiz ensaio auditivo.

**Sincronização do repositório — 11/11.** Commit `c62ed60` (“Batruc: revisar
seeds, MIDI e baixo”) foi enviado para `origin/main`; a branch ficou alinhada
com o remoto. Esse envio também carregou os 17 commits anteriores que já
estavam pendentes na branch, incluindo commits de Asterismus. O commit do
Batruc reuniu 20 arquivos versionados do projeto; os arquivos MIDI externos
em `midis/groove/`, o ZIP do corpus e alterações de outros núcleos ficaram
fora. O envio não é release do Batruc.

**Repetição entre seeds no baixo — 11/11.** O autor relatou que a linha de
baixo voltava a soar igual e que as seeds eram previsíveis. A auditoria
encontrou que `generateSeedBassLineFromVoicePlan()` usava somente
`bassRecipe_.seed`; essa seed independente era inicializada na primeira seed
de composição e permanecia igual nas seguintes. Agora o agendamento do baixo
combina a seed editável do baixo com a seed atual da composição por uma mistura
determinística de 32 bits. Assim, ambas influenciam a linha sem sobrescrever o
valor mostrado no campo de seed do baixo. A fonte ainda é a agenda das vozes,
portanto a diversidade continua limitada pelos eventos e papéis presentes
nessa agenda; a combinação remove uma causa de repetição, mas ainda precisa de
validação auditiva com seeds distintas. Arquivos: `src/app/Main.cpp` e este
registro. Build Release serial de `batruc_app` concluído em 11/11. Não executei
testes nem fiz escuta; o app que já estava aberto não foi manipulado e precisa
ser reiniciado para carregar o binário recompilado.

**Seed dedicada sem mudança audível no canal de baixo — 11/11.** O autor
confirmou que trocar especificamente a seed do baixo não altera o que ouve.
O recálculo chegava ao canal, mas o mapeamento contínuo de alturas podia
arredondar ou limitar seeds diferentes às mesmas notas, tornando a mudança
inaudível. `buildSeedBassPerformance()` agora deriva da seed combinada uma
transposição limitada para a frase; preserva o desenho rítmico-fonte,
os acentos e as variações evento a evento. Em níveis 1–3, o gerador também
garante um contratempo por ciclo formal quando encontra subdivisão livre junto
a um ataque-fonte; a seed determina a posição. Ataques e pulsos fortes de origem
seguem preservados. Arquivos: `src/core/BassComposition.cpp`, `README.md` e este
registro. Build Release serial de `batruc_app` concluído em 11/11; SHA-256
`5c3e5c86daca336d62909e4425349a7fd4cc03a91fe8754415de85a59373bc1f`.
Não executei testes nem ensaio auditivo. Próximo passo: comparar duas seeds
dedicadas no canal de baixo.

**Ajuda do baixo no Learn — 11/11.** Acrescentei um nono tópico ao painel
Aprender, explicando a seed própria do baixo, os níveis de variação, a edição
manual do piano roll e os controles da faixa BAIXO no Mixer. O conteúdo está
localizado em PT/EN/FR e incluído no dicionário ES. O painel também ganhou um
explorador visual de padrões na oitava aba; audição, exercícios e percurso
didático seguem pendentes. Arquivos: `src/app/Main.cpp`, `README.md` e este
registro. Build Release serial de `batruc_app` concluído em 11/11; SHA-256
`a55bc45eecdc485f2635033db5f18dd117cc8da187a5f510e443caab4328258a`.
Não executei testes nem fiz revisão visual do painel.

**Exercício de reconstrução rítmica no Learn — 11/11.** Acrescentei um modo de
prática que sorteia um padrão oculto, permite ouvi-lo pelo botão **Ouvir**,
reconstruí-lo nas células e conferir a resposta. A interface explica a ordem
das ações em PT/EN/FR e ES. Iniciar o exercício interrompe qualquer prévia
anterior; sair restaura a subdivisão e o padrão que estavam selecionados.
Arquivos: `src/app/Main.cpp`, `README.md` e este registro. `git diff --check`
passou; build Release serial de `batruc_app` concluído em 11/11. SHA-256
`a9d3a3965649b96f02b1a77c4272c62168fa801a8c71c79a222b2166df1e1659`.
Não abri a interface nem fiz validação auditiva. Próximo passo: conferir
manualmente áudio, interação e disposição visual.

**Percurso progressivo de ritmos no Learn — 11/11.** Adicionei cinco etapas
(4, 6, 8, 12 e 16 posições). Cada etapa sorteia um padrão não trivial; o músico
ouve, reconstrói e confere. Um acerto libera a etapa seguinte, um erro conserva
o desafio para nova tentativa, e o término mostra conclusão. Ao sair, a seleção
anterior de subdivisão, padrão e modo de compasso é restaurada. Arquivos:
`src/app/Main.cpp`, `README.md` e este registro. `git diff --check` passou;
build Release serial de `batruc_app` concluído em 11/11. SHA-256
`b0a70ebb50d23c65da0c454eb08e65354ca9b721a2014615acc3b7146cfa70f8`.
Não fiz validação auditiva nem visual. Próximo passo: conferir manualmente
áudio, interação e disposição do percurso.

**Contexto musical inicial no Learn e índice de fontes MIDI — 11/11.**
Acrescentei ao tópico Contagens uma trilha textual: conferir proveniência,
compasso/BPM, ouvir stems em separado, identificar funções pela escuta e
comparar uma alteração. Os rótulos brasileiros do GMD são explicitamente
limitados a metadados de performances de bateria. Registrei fontes de contexto
do IPHAN para Maracatu Nação e da Paraná Cultura para Ijexá/Afoxé, sem codificar
ou reivindicar toques tradicionais. A documentação `midis/README.md` agora
lista cinco arquivos GMD rotulados baião, samba, ijexá, maracatu e frevo com
BPM, compasso e caminho local verificado. Arquivos: `src/app/Main.cpp`,
`README.md`, `RITMOS_E_CONTAGENS.md`, `midis/README.md` e este registro. Build
Release serial de `batruc_app` concluído em 11/11; SHA-256
`88c1903b5e06f08a2a0d421ac1fdae1f04c5524539a96a8c719740ec7754f92c`.
`git diff --check` passou e os cinco caminhos MIDI correspondem ao CSV e
existem. Não fiz escuta nem inspeção da interface. Permanecem
pendentes exemplos didáticos completos, validação por músicos e audição dos
MIDI no Batruc.

**Correção factual da paleta sonora na documentação — 11/11.** Na data deste
registro, o README e o panorama de estado em TAREFAS diziam 12 matérias sem
incluir Chip 8-bit. A correção distinguiu então seis harmônicas/modais, seis
percussivas e a matéria digital, registrando que Chip 8-bit foi acrescentado em
06/11 sem renumerar os doze índices anteriores; seus controles persistem a
partir do schema 41. A paleta cresceu depois: a retificação atual consta no
estado de abertura e no inventário ativo abaixo. Conferi a enumeração em
`CompositionCore.hpp` e os registros de cena.

**Explorador visual de ritmos no Learn — 11/11.** A oitava aba agora permite
selecionar de 4 a 16 posições binárias, percorrer os padrões em ordem, editar
cada célula, sortear um padrão e sortear um compasso 4/4 (16 semicolcheias,
65.536 combinações). Adicionei **Ouvir**, que toca uma prévia sintetizada em
pulsos no tempo atual; não inicia transporte nem altera a composição. O modo de
  prática guiada e o percurso progressivo foram acrescentados nos registros
  subsequentes. Texto atualizado em PT/EN/FR e dicionário ES. Arquivos:
`src/app/Main.cpp`, `README.md` e este registro. Build Release serial de
`batruc_app` concluído em 11/11; SHA-256
`81cb3aa0e0a1a5d1558d5fc8aa0505f3820dbab38c1b72b4577a2d25fb0b2820`.
Não rodei testes nem conferi o painel visualmente; a escuta da prévia ainda
precisa ser validada no app.

**Auditoria da diversidade das seeds — 11/11.** `algorithmicNextSeed()` já
procura uma receita numericamente contrastante em vários parâmetros, incluindo
família, groove, geometria e intenção; isso não garante contraste auditivo.
Quando existe MIDI importado, `seededMidiRhythmCells()` varia dinâmica sem
alterar ataques, posições e notas GM. `seededMidiPhraseCells()` também pode
rarefazer de forma determinística ataques fracos em lanes de apoio densas,
limitados por papel (10–24% conforme a lane), preservando bumbo, caixa,
downbeats, acentos fortes e grupos de colisão/CC4. Pode acrescentar pickups
discretos com notas GM existentes em chimbal fechado, toms, ride, percussão de
mão e acessórios. A fonte importada não é modificada; cada seed reproduz sua
própria seleção de eventos. O controle interativo de densidade comparada entre
stems segue pendente, assim como escuta em diferentes estilos.
Build Release serial de `batruc_app` concluído em 11/11; SHA-256
`78d13eba3bbc78e41b1e05dc605200099f3cf935cedf8e6d019178ac9b99438e`.
Não executei testes nem fiz escuta; o binário precisa ser reaberto para ouvir a
mudança. Refinamento por estilo continua necessário antes de considerar
resolvida a repetitividade. Também alinhei o tooltip de “Nova seed”: ela
escolhe a família sonora, que pode ser substituída manualmente no Mixer.

**Variação generativa da percussão — 10/11.** A auditoria separou camadas:
seed já escolhe ciclos, fases, transformações, profundidade por stem, família,
leis de intensidade, groove, dinâmica e intenção global. O renderer já move
pan por ataque segundo a posição X, modula força e amortecimento pela
intensidade/densidade e muda articulações quando a fonte MIDI traz notas GM.
Faltava variação de altura para os tambores gerados sem nota MIDI: agora V5
(`membrane`) percorre até ±4 semitons e V7 (`conga`) até ±2, em passos
calculados da geometria e escalados pela profundidade do stem. Notas MIDI
explícitas são preservadas; bumbo, caixa e pratos não recebem essa transposição.
Ainda faltam intenção por evento, timbres/articulações por estilo e gesto
explícito editável, com travas por dimensão. A ginga deverá coordenar esses
parâmetros com a função de cada voz e o ritmo, não sorteá-los separadamente.
Build Release concluído em 10/11; SHA-256
`0f54cc5a6b1fa1f5c178d45d08a94bc7a2e40b9b2a15a4a8ae0c42c13ed565aa`.
Escuta das novas alturas e comparação entre sementes ainda pendentes.

**Variação do baixo e stem no Mixer — 10/11.** Auditoria do código: a seed do
baixo já varia alturas, início dos ataques, duração por lei, velocidade,
respiros, relação com as vozes, sequência por sete ciclos e caminho de altura.
Intenções `normal/ghost/accent` existem nas células editáveis; o gerador seeded
usa intensidade, mas ainda não produz uma intenção explícita por nota. O LFO
oferece modulação de síntese. O baixo saía em mono duplicado nos dois canais;
agora cada evento seeded recebe panorama discreto (máximo ±0,24) a partir da
posição X do percurso, escalado pelo nível 0–3, com âncoras de V1 no centro.
Eventos manuais/legados permanecem centrados pelo valor padrão. O renderer
aplica panorama por nota, com centro no nível anterior; o contrato rejeita
valores não finitos ou fora do limite. O Mixer agora tem faixa própria para o
baixo com nível, pan, mute, solo, ativação e medidor, sincronizados com a página
Baixo e armazenados nas cenas. Build Release e
`batruc_bass_audio_response` passaram em 10/11; escuta estéreo pendente.

**Verificação pendente do stem de baixo no Mixer:** a faixa independente foi
implementada e mantém os oito stems de percussão. O código sincroniza nível,
pan, mute, solo e ativação com os controles da página Baixo e inclui medidor de
sinal. Faltam validação visual nas resoluções 1080/1440/1920 px e escuta de
ganho/pan/mute/solo no app atualizado. A confirmação auditiva da variação de
seed e timbre do canal também continua pendente.

**Presença do baixo seeded — 09/11.** A inspeção do gerador mostrou que a
velocidade de cada nota era `0,62 × intensidade da fonte` antes da expressão e
do groove, o que podia deixar todos os ataques fracos quando a fonte tinha
intensidades modestas. A receita agora diferencia apoios de V1, notas
principais e respostas em contratempo com pisos de velocidade antes da
expressão; contratempos mantêm teto próprio para não ocultar a linha. Uma
nova seed sobe o ganho do baixo de 0,20 para 0,32 apenas quando ele ainda
está no valor padrão (ou zero), preservando ajustes explícitos diferentes.
Ainda é necessário ouvir o baixo solo e no conjunto em diferentes seeds;
essa alteração não valida a ginga rítmica ou a qualidade do timbre. Build
Release e `batruc_ecological_bass_composition` passaram. Binário Release
SHA-256 `0bc64ed3ea7b18bda5d1f999e3faaed6715ca7d4f9eb32ed0acbc3169c28e31d`.

**Caixa com esteira sintetizada — 09/11.** O modelo `snare` recebeu uma
resposta de esteira independente do corpo ressonante: ruído limitado por duas
suavizações de frequências distintas, com envelope próprio disparado pelo
ataque da pele e cauda de cerca de 110 ms. O ruído curto do ataque foi
reduzido para que a esteira tenha função reconhecível. A implementação não usa
samples, alocação ou filtros novos no callback de áudio. Falta confirmar por
escuta a relação entre pele, esteira e outros stems; o modelo ainda é uma
síntese estilizada, não uma simulação física de uma caixa acústica específica.
Build Release e render MIDI de referência GMD rock 65 BPM concluídos; o WAV
isolado de V2 teve pico PCM de 0,824, sem corte amostral nesse caso. O
renderizador de referência usa seu próprio ganho de normalização, portanto
esse pico não mede a saída master do app.

**Diferenciação imediata dos stems — 09/11.** O renderizador agora distingue
o chimbal fechado de V3 do aberto de V4 na duração do corpo, da cauda de
ruído e na cor espectral. Escovas recebeu ruído mais filtrado e ataque menor
para reduzir sua semelhança com o chimbal. No Mixer, o botão de cada stem
mostra o nome do som escolhido, inclusive quando há override manual; V3/V4
são identificados como fechado/aberto quando usam o modelo de chimbal. Esses
ajustes usam os modelos sintéticos atuais e ainda requerem escuta; a palheta
ampla por estilo continua pendente. A esteira sintética da caixa foi
implementada no bloco anterior e ainda requer escuta comparativa. Build
Release concluído; auditoria visual do próprio app registrou `captured=true`,
zero sobreposições do Mixer em 1080/1440/1920 px e mostrou V1 Bumbo, V2 Caixa,
V3 Ch. fechado, V4 Ch. aberto e V8 Escovas. SHA-256 do binário:
`2754a904fd15e4ac80f48ae2f4baecc1a9565a23cdb47fb150ad437733c0ce01`.
Ainda não houve escuta comparativa nesta sessão.

**Direção musical da seed e palheta — pesquisa inicial, 09/11.** A seed deve
funcionar como inteligência musical do instrumento: partir de um caráter ou
estilo rítmico, compor a máquina de ritmos e escolher, para cada stem, uma
fonte/timbre e uma função coerentes. Não basta sortear uma família nem mudar
som sem relação com o padrão. A escolha precisa considerar papel, registro,
articulação, densidade, acentos, subdivisão, contratempos, variação e relação
entre vozes. O músico conserva controles para editar ou travar cada decisão.

A pesquisa inicial mostra por que os papéis não podem ser um kit universal:
na batucada de samba, o surdo sustenta e orienta a pulsação, caixa/ganzá/
pandeiro/reco-reco mantêm padrões regulares com variações, enquanto tamborim,
agogô e cuíca podem assumir linhas-guia e partes solistas; no baião, zabumba e
triângulo articulam grave, subdivisão e contratempos, com acentos que mudam a
cor do triângulo; no jazz, o ride conduz o pulso e caixa/bumbo pontuam e
interagem; em grooves de rock/pop, bumbo e caixa estruturam o backbeat e
chimbal conduz a subdivisão. Referências iniciais: artigo CInBalada
(https://new.scielo.br/j/jbcos/a/ptVmGGJx4RqngpgvffsGYmt/?ilang=pt_BR&lang=en),
análise de samba e time-line (https://www.scielo.br/j/pm/a/Grtn6Svp4rvRLrC6NsPyTrn/),
análise do baião (https://www.scielo.br/j/pm/a/6sW6mgh4bZhTRngXKxnRygH/),
papéis da bateria no jazz (https://mtosmt.org/issues/mto.10.16.4/mto.10.16.4.butterfield.html)
e corpus de bateria popular (https://pmc.ncbi.nlm.nih.gov/articles/PMC6025871/).
O Dataset Brasileiro de Instrumentos Rítmicos contém gravações solo e em
conjunto de dez classes de instrumentos em samba, partido-alto, samba-enredo,
capoeira e marcha (https://www.colibri.udelar.edu.uy/jspui/handle/20.500.12008/43550).

Critério de projeto derivado: “família” precisa selecionar uma palheta de
fontes realmente distintas — instrumentos acústicos/percussivos, bateria,
eletrônicos, sínteses e sonoplastias — e cada preset/estilo deve declarar o
papel e as possibilidades de articulação por stem. A ginga deve emergir da
relação entre acentos, microtiming, subdivisão e interação das vozes, não de
randomização ou de um único swing global. Respeitar diferenças regionais e
tratar esses mapeamentos como modelos editáveis, não como definição universal
de um gênero.

Lacuna confirmada no código: `seededRecipeSelection()` já calcula
`soundFamilyIndex`, mas `applySeedInput()` conservava a família escolhida
manualmente; `soundFamilyVoiceSounds()` também devolve o mesmo mapa básico
para as 17 famílias. Nesta etapa, a seed passou a aplicar a família que
calculou, atualizando o Mixer e os perfis de timbre por stem; depois disso, o
músico pode editar os sons. Cenas carregadas continuam restaurando sua própria
família. A busca de “Nova seed” voltou a pontuar diferença de família, pois a
escolha agora altera o som aplicado. Build Release do app e
`batruc_seeded_recipe` passaram em 09/11; escuta no app segue pendente. A
mudança documentada em 11/11 separa chimbal fechado e aberto em modelos sonoros
e elimina a dependência do índice do stem na síntese. Ainda não é ginga
completa: famílias compartilham tipos-base, `brushes` continua sendo ruído
filtrado genérico, a esteira da caixa usa uma resposta sintética única e ainda
faltam modelos articulados para percussões brasileiras. Próxima etapa: matriz auditável estilo → funções de
stem → fontes/articulações → padrão e microtiming, seguida por validação de
áudio. Não afirmar fidelidade de gênero sem validação de músicos e escuta.

**Kit inicial e balanço — 09/11.** Objetivo: tornar audíveis os papéis
percussivos desde uma composição nova. `src/app/Main.cpp` agora abre com a
família Percussão selecionada e aplica os timbres de V1–V8 antes da primeira
seed: bumbo, caixa, dois chimbais, tom, prato, percussão de mão e apoio. O
nível inicial de V3 passou a 0,45 e o de V4 a 0,50; os demais continuam em
1,00. A importação MIDI aplica esse balanço somente se encontrar o antigo
mix intacto (todos os oito níveis em 1,00 e sem mute/solo); cenas e ajustes
existentes conservam seus valores. Build Release com `-j1` concluído em
09/11/2026; a auditoria do app registrou `captured=true`,
`initial_kit_balance=true` e zero sobreposições no Mixer em 1080, 1440 e
1920 px. A captura de 1080 px mostra Percussão, V1 Bumbo e os níveis de V3/V4.
Binário: SHA-256
`bc9c5fd3aad762df36f9d252e1d3e7c220b057fc35bd74da0f1c7ede8b6fa689`.
Limite: a auditoria valida estado e imagem, não a percepção sonora do balanço.
Próximo passo: reabrir o app recém-compilado e ouvir V1, V3 e V4 em solo e
no conjunto com ritmos gerados e MIDI; ajustar níveis e síntese por escuta.

**Gramani e papéis dos stems — auditoria de 09/11.** A suspeita do autor é
pertinente ao modelo: trocar agendas rítmicas entre vozes pode fazer um stem
de bumbo receber ataques antes associados ao chimbal. No estado atual, porém,
`invertEventPlanVoices()` só é chamado pelo teste de inversão; o app não o
invoca durante a reprodução. `PassageDevelopmentState::inverted` inverte a
direção/ordem de uma variação dentro da passagem, sem trocar os IDs de voz.
As famílias mantêm papéis fixos V1 bumbo, V2 caixa, V3/V4 chimbais etc.; a
importação MIDI distribui notas GM por lane. Os testes locais
`batruc_seeded_recipe`, `batruc_voice_inversion` e
`batruc_midi_rhythm_import` passaram (3/3), mas não substituem escuta por
solo do caminho evento → nota MIDI → síntese. Recompilei o renderizador de
referência e repeti o estudo GMD rock 65 BPM de dois compassos em
`/tmp/batruc-stem-audit-20261109-*`: 30 ataques; V1 recebeu oito notas 36
(bumbo), V2 cinco notas 38/40 (caixa), V3 dezesseis notas 22/42/44
(chimbal fechado) e V4 uma nota 26 (chimbal aberto). Nos WAVs isolados, a
fração de energia de V1 em 35–250 Hz foi 99,9%; a de V3 em 2–12 kHz foi
93,8%. Assim, esse caso não apresenta inversão de roteamento. O RMS dos
trechos acima de −60 dBFS foi 0,034 em V1 e 0,077 em V3, diferença de
aproximadamente 7 dB a favor do chimbal; RMS e bandas espectrais não medem
sozinhos a percepção de volume. Próximo passo: ouvir V1–V8 em solo e no
mix do app, com e sem fonte MIDI, antes de calibrar o balanço inicial do kit.

**Campo animado à esquerda — 09/11.** O gráfico que o botão CAMPO revelava
na área central foi movido para o cartão esquerdo e fica sempre visível na
tela Compor. A primeira mudança substituiu indevidamente a prévia da curva:
o seletor e a seed mudavam a geometria, mas o gráfico à esquerda já não a
mostrava. A prévia da **curva selecionada** foi restaurada acima do campo
animado, com áreas separadas. O botão foi retirado e o Study voltou a ocupar
a largura integral do cartão central. A curva agora mostra um cursor e o
trajeto percorrido no mesmo progresso musical do campo. Ambos mudaram entre
0,125 e 0,625 da seção no diagnóstico visual (`curve_motion=true` e
`pattern_field_motion=true`); a atualização da
tela já acompanha o transporte; não há um relógio visual independente. A
captura do próprio app confirmou a posição do campo em 1080 e 1920 px;
o diagnóstico também comparou Triângulo/Quadrado
(`curve_selection_changes=true`) e Campo livre/
Ilhas (`pattern_mode_changes=true`). O timer atualiza a área Compor a 60 Hz
durante a reprodução. As seeds permanecem em Campo livre conforme a decisão
do músico; a máscara muda somente por escolha no rodapé. Build Release
concluído; auditoria de layout `captured=true` em 1080, 1440 e 1920 px;
SHA-256 do binário
`265cd60b405f6cc1b14d0d895dfd6c5bf91213b6e32b7175ea5f61af634a7ef5`.
Próximo passo: conferir o movimento e a leitura dos dois gráficos na janela
normal com áudio.

**Compor em três cartões — 09/11.** Objetivo: dar ao Study a maior área,
mostrar o campo geométrico desde a abertura e usar o espaço lateral para
células legíveis. A primeira implementação ainda desenhava campo e Study em
um só cartão; após a correção, há três cartões separados. O da esquerda reúne
seletor, prévia da curva, campo animado, variação e grade de 16 posições; quando há altura,
exibe também um resumo compacto de ciclos por voz. O cartão central reserva o
maior gráfico ao Study e concentra seus comandos. A Vista vinculada ocupa o
cartão direito; seu gráfico foi dimensionado para chegar perto dos controles,
sem se tornar a vista principal. A largura da grade e os alvos de clique são
calculados juntos. Capturas do próprio app em 1080×800, 1440×900 e 1920×1080
foram inspecionadas: os três cartões aparecem e não há colisão visual entre
gráfico e controles. O diagnóstico registrou `captured=true` e zero
sobreposições no Mixer nas três larguras; não houve escuta de áudio nem teste
manual dos gestos nesta alteração. Build Release concluído; SHA-256
`003ebf2820b85c1f7489a169fadf0f3360424d1a08b86f49bc8a2cc6c7ccbd19`.
Próximo passo: ensaiar os controles e gestos na janela normal após reabertura.

**Etapas anteriores do layout — 09/11.** A primeira tentativa moveu só o
seletor; a seguinte desenhou a prévia, mas a ocultava na largura inicial. Essas
duas limitações motivaram a distribuição em três colunas descrita acima. Nesse
trabalho, a grade passou de 6×10 para 8×14 px, os anéis do Study ganharam
contraste, e a Vista vinculada teve a área de desenho reduzida em 44 px.
Arrastar/zoom no gráfico vinculado altera só a navegação; os controles do painel
alteram parâmetros sonoros. A prévia geométrica não recebe gestos.

**Ajuste de 09/11 — textos fora do gráfico Estudo.** A área do ciclo agora
reserva faixas próprias para título, estado e instruções; a área do desenho não
é mais calculada por trás dessas linhas. Clique/arraste e hit-tests usam a mesma
geometria redesenhada. Build Release concluído.
Inspeção visual após abrir o Release confirmou que o texto de estado e as
instruções ficam fora do círculo, sem a sobreposição da captura anterior. O
Batruc já possui master geral no rodapé (0–100%, salvo na cena), além dos volumes
individuais dos stems.

**Tutorial do gráfico Estudo — 09/11.** O tópico **Fórmula e fase** agora
explica, em PT/EN/ES/FR, os gestos do ciclo compartilhado: arrastar o anel
desloca a fase do stem inteiro; arrastar um ponto move um ataque; clique vazio
cria ataque; Shift+arraste radial ajusta a força; botão direito/duplo clique
remove; Alt+clique no anel desativa a célula. Release recompilado e
`git diff --check` passou. SHA-256 do binário:
`1252354b48508eda566fd564a36f76e9a4f8c7332df194d751f03a04966ca2c1`.
O Release atualizado foi aberto; os gestos explicados no Learn ainda precisam
de ensaio manual e confirmação auditiva pelo músico.

**Seed MIDI — ajuste de dinâmica, 09/11.** A escala de acentos passou de ±0,08
linear para uma curva de profundidade até 0,52 antes dos pesos por lane, para
que seeds de variação moderada não produzam diferenças quase imperceptíveis.
Posições, notas GM e relação entre stems seguem preservadas. A compilação
Release foi concluída com o ajuste visual acima; escuta comparativa ainda
pendente, portanto não declarar a diferença audível antes de ouvir no
instrumento. `git diff --check` passou; testes não foram executados.

Auditoria de recuperação do trabalho existente:
[AUDITORIA_COESAO_REAPROVEITAMENTO_2026-11-06.md](AUDITORIA_COESAO_REAPROVEITAMENTO_2026-11-06.md).
O adendo de 08/11, na seção 11, define a prioridade musical: verificar uma
célula MIDI de referência, o papel e som de cada stem, a resposta em tempo real
e a variação audível antes de ampliar forma, timbre ou interfaces paralelas.

## Reconciliação do histórico — desenvolvimento desde 10/10/2026

Os registros abaixo preservam passagens desde a abertura do projeto em
10/10/2026. A lacuna encontrada não foi a ausência do histórico, mas a lista
operacional do início deste arquivo: havia tarefas iniciais ainda marcadas
como se nada tivesse sido implementado, regressões conhecidas fora do P0 e
descrições de interface/limites que já envelheceram. Esta reconciliação mantém
os relatos históricos e usa a lista atual como fonte única do próximo trabalho.

| Período | Trabalho materializado no histórico | Retomada ainda necessária |
| --- | --- | --- |
| 10–24/10 | identidade e situação musical; contratos iniciais; core C++ próprio, renderer e host JUCE; pesquisa de tempo, geometria, ritmo e síntese | ensaio musical do autor; limites de recursos em uso real; código externo só será auditado se uma dependência for escolhida |
| 25–28/10 | Mixer, cenas, gravação, metrônomo, baixo, idiomas, protótipo web e comparação HTML/JUCE | cinco divergências conhecidas na última suíte integral registrada; a interface web continua fonte duplicada e não deve avançar em paralelo |
| 29/10–03/11 | operações generativas, forma, geometria, modos Orbit/Study, proposta visual SVG e auditorias de layout | executar a proposta aprovada preservando os controles atuais; o Editor de Ciclos circular completo ainda é proposta, não função entregue |
| 04–08/11 | células editáveis, famílias sonoras, Chip 8-bit, duas leis de intensidade, pacote GMD e importação MIDI, layout por páginas, expansão da seed, fonte MIDI preservada e articulações GM em cenas schema 48 | ensaio de escuta/GUI; samples licenciados; manifesto detalhado de proveniência/licença; operações Study sobre células MIDI; auditoria integral de textos/tutorial; acessibilidade e validação responsiva |

As tarefas originais 1–4 da etapa inicial estão concluídas no nível de
especificação e implementação correspondente; a tarefa 5 é condicional ao uso
de código externo; a tarefa 6 foi parcialmente entregue e não equivale a um
instrumento validado. As tarefas 7–9 continuam representadas abaixo em
validação técnica, escuta do autor e decisão do próximo incremento. Os portões
de marca, distribuição, empacotamento e publicação são pré-release e não
autorizam publicação, remotos ou expansão de escopo neste momento.

## Pendências atuais — ordem de trabalho revista em 08/11/2026

Esta é a lista viva de retomada. Os registros abaixo dela preservam o histórico
das decisões e validações; uma tarefa só sai desta lista depois de implementada
e verificada no estado real do instrumento.

### Critério para fechar a lista

Vou concluir implementação, correções, documentação, auditorias estáticas e
validações locais que não dependam de julgamento perceptivo. Tarefas de timbre,
musicalidade, resposta auditiva e desempenho em sessão ficam abertas até o
autor escutar o binário; tarefas de layout/legibilidade ficam abertas até a
conferência visual no app. Direitos, compatibilidade de pacotes, nome e
publicação também exigem evidência ou decisão própria e não serão inferidos de
um build. Ao retomar, priorizar primeiro o que é implementável localmente e
manter esses pontos como portões explícitos, sem declarar o projeto inteiro
concluído enquanto restarem.

### P0 musical — provar um ritmo que funciona

- [x] Escolher um `beat` 4/4 de dois compassos no GMD local, registrar fonte,
  licença/atribuição, janela e notas GM; comparar célula importada, agenda e
  áudio. Os estudos técnicos rock e samba estão em [midis/README.md](midis/README.md).
  Os rótulos editoriais de estilo ainda não foram validados por escuta.
- [ ] Confirmar por solo e por escuta a função V1–V8 desde C1, começando por
  bumbo, caixa e chimbais. Construir um kit de referência coerente e comparar
  duas paletas com o mesmo padrão e nível perceptivo equivalente.
- [ ] Fazer uma variação de seed reversível e limitada por função sobre essa
  célula, preservando as âncoras. **Nova seed** busca contraste também na
  família sonora; a máscara continua sob decisão do músico e as famílias podem
  ser substituídas no Mixer. A primeira derivação MIDI já preserva bumbo/caixa
  e pode acrescentar pickups às vozes de apoio existentes; falta validar no
  Study que a mudança é musical, audível e adequada a mais de um estilo. No
  baixo, níveis 2–3 agora podem antecipar a pulsação seguinte em semicolcheia,
  além do contratempo de colcheia; o limite é 1/2/3 novos ataques por compasso
  conforme o nível. O teste focal verifica reprodução da mesma seed, diferença
  rítmica entre seeds, preservação de âncoras e o limite de densidade.
- [ ] Medir tempo entre gesto e resultado sonoro e identificar se o reset de
  forma após edição causa pausas ou reinícios indesejados. Critério de pronto:
  o autor reconhece ritmo, função dos stems e efeito dos controles no app.
  No core, uma troca de família ou nível durante a cauda de uma caixa já muda
  o áudio em até 16 amostras a 8 kHz, sem reiniciar a seção; falta medir o
  caminho gesto → callback → dispositivo no app. O arraste do Study faz
  recomposição periódica do baixo (~100 ms) e agora força a publicação final
  ao soltar; medir a resposta percebida e confirmar que não há pausa audível.

**Avanço de 08/11 — roteamento TD-11 e estudo de referência.** Objetivo:
testar a cadeia fonte MIDI → papel do stem → evento → áudio. O catálogo GMD
agora interpreta 22/26 como chimbais e 58 como tom, segundo o mapa Roland
TD-11 publicado pelo dataset; arquivos externos mantêm GM. O renderer conserva
a nota original e usa o som correspondente à lane, inclusive o aro do tom
na V5. O importador também recupera o primeiro tempo antes do primeiro ataque
pelo tempo absoluto, mesmo se a pista de tempo vier depois da bateria num MIDI
formato 1, e o aplica ao BPM do app quando cabe na faixa 20–220. Uma regressão
focal cobre ordem de pistas invertida e ignora um tempo que só começa após o
primeiro ataque; mapas de tempo variáveis continuam pendentes.
O suporte ao mapa variável foi acrescentado em 11/11; ver o registro atual no
início deste documento.
Corrigi o teste de variação MIDI para exigir posições/notas preservadas
e força variável; a busca de próxima seed deixou de pontuar família, máscara
e controles Chip 8-bit que o host conserva. O tooltip da seed foi alinhado.
Arquivos afetados: `src/core/MidiRhythmImport.*`, `CompositionCore.cpp`,
`src/app/Main.cpp`, `src/tools/midi_reference_render.cpp`, `CMakeLists.txt`,
`tests/midi_rhythm_import.cpp` e documentação local. O estudo GMD de dois
compassos, a fonte/licença e os WAVs temporários estão em `midis/README.md`.
Validação: build Release serial de app, ferramenta e dois testes; CTest 2/2;
traço com 30 ataques nos stems V1–V4, 65 BPM lidos do arquivo e WAVs de
mix/stems não silenciosos. Binário Release de 08/11 às 22:50 CET,
SHA-256 `126f853ec303ca0279057c9d71decc54e23131166bfbd8efbf85306ca22ea185`.
Limites registrados em 08/11: ausência de escuta humana, CC4 do chimbal não lido,
gesto de abertura apenas binário e kit ainda estilizado. CC4 e abertura contínua
foram implementados em 11/11 (registro ao fim do documento); escuta humana e
validação do kit continuam pendentes. Próximo passo: abrir a versão Release,
ouvir o estudo no app e medir reação a edições sem reset audível.

**Avanço de 08/11 — resposta ao vivo, baixo independente e limpeza MIDI.**
Objetivo: retirar a espera de uma barra e permitir variar o baixo a partir da
percussão sem trocar o beat. Alterações em células, overrides de ciclo/fase/
regra, swing, compasso/BPM e variante passam ao próximo bloco de áudio sem
reiniciar a forma; o baixo é recomposto durante o arraste do Study com limite
de uma atualização a cada ~100 ms, além da atualização final ao soltar.
Mudanças de timbre, célula e família deixaram de solicitar reinício da forma.
O áudio do baixo passou a aceitar eventos em C1 e C5, antes bloqueados por uma
condição fixa apesar de constarem na partitura. O painel Baixo oferece seed
hexadecimal própria, **Novo baixo** e níveis 0–3; o gerador usa os ataques
vocais como estímulo, mantém os apoios da V1, varia altura/respiros de forma
determinística e preserva o MIDI e a seed geral. A nova seed geral conserva a
seed do baixo já existente. O seletor **Ritmos / MIDI** agora tem **Remover
MIDI**; a ação limpa a referência da cena, mantém o arquivo no disco e
reconstrói o ritmo da seed atual. Schema 49 persiste seed e nível do baixo;
cenas 1–48 continuam legíveis. O gerador também pode usar uma fonte MIDI sem
seed geral após o músico selecionar **Novo baixo**.
Referência conceitual consultada: `../RASGO_MODULAR/src/dsp/Phrase.hpp`, onde
a identidade do motivo (`seed`) fica separada da intensidade de interpretação
(`variation`). O Batruc adotou essa separação para o baixo com quatro níveis
discretos; não copiou o DSP do módulo.
Arquivos afetados: `src/core/CompositionCore.*`, `src/core/BassAudio.cpp`,
`src/core/BassComposition.*`, `src/app/Main.cpp`, testes de receita, transporte,
áudio do baixo e composição ecológica, além desta documentação e `README.md`.
Validação: build Release serial do app e dos testes; CTest dirigido 5/5
(`seeded_recipe`, `variant_transport`, `bass_composition`,
`bass_audio_response`, `ecological_bass_composition`). O teste de níveis
confirma determinismo, alteração entre seeds, redução controlada de ataques e
preservação dos pulsos V1 em nível 3. Mais 3/3 comparações de agenda
(`seeded_bass_parity`, `seeded_voice_bass_parity`, `cyclic_bass_parity`)
passaram após a mudança. `git diff --check` limpo. Binário Release
final SHA-256 `5eec1d1ee94e2b5a6d866d124e0512f46966f3029f2d929c285efac2180e8ce5`.
Limite: sem escuta humana nem inspeção
visual da nova faixa de controles; uma nota já ultrapassada no compasso soa na
próxima ocorrência, não retroativamente. Próximo passo: reiniciar `./run`,
ouvir o beat MIDI com **Novo baixo** nos quatro níveis, verificar a ação de
remoção e a persistência após salvar/reabrir uma cena schema 49.

**Ajuste de 09/11 após revisão do cálculo temporal.** A inspeção encontrou que
a relação **Responder** substituía o deslocamento temporal aplicado pela seed.
O deslocamento foi movido para depois de todas as relações. Também foi ampliado
de microtiming subquântico para variação de posição: nível 1 desloca até ¼ de
semicolcheia; nível 2, até 0,7; nível 3, até uma semicolcheia antes/depois.
Os apoios de pulso ancorados da V1 ficam fixos. A mesma seed conserva os tempos;
outra seed pode escolher posições diferentes. Release recompilado. Não rodei
testes nesta revisão; a aceitação auditiva do autor ainda está pendente.

**Correção de escopo após escuta do autor.** O deslocamento de ataques existentes
não atendia ao pedido de composição e contratempos. Agora os níveis 2 e 3 também
examinam subdivisões vazias de contratempo, procuram estímulos próximos na grade
da percussão e podem compor nelas um ataque de baixo com dinâmica reduzida e
altura/identidade derivadas do evento-fonte. A chance cresce do nível 2 para o
3; os apoios fortes da V1 não são substituídos. A relação Responder preserva
esses novos ataques. Build Release concluído em 09/11. Na escuta, o autor
percebeu pouca diferença e pediu para manter assim; não ampliar esta regra por
enquanto. A composição ainda não passou pelo critério auditivo de variação
perceptível. Próximo trabalho musical permanece o ensaio controlado do beat GMD
e a identificação audível das funções V1–V8, antes de reabrir a arquitetura do
gerador do baixo.

**Auditoria de resposta em tempo real — 09/11.** O motor já atualizava ataques,
ciclos, fase, swing, BPM e baixo no próximo bloco de áudio, mas tooltips e
descrições do Study ainda diziam “próxima barra” ou “fim da seção”. Corrigi as
mensagens ativas de BPM, swing, ciclos, fase por voz, regra de ataque, relação
do baixo e aplicação de polirritmia, além das descrições correspondentes no
README. O texto explica que a mudança preserva o progresso musical e entra no
bloco seguinte. Build Release compilado; `git diff --check` limpo. SHA-256 do
app: `9dc23abebd462e0811851f21d59262eed30a7371f44a12c4c14bb17bf14e2f9d`.
Sem teste nesta edição.

### P0 — fechar regressões e validar a interface atual

- [x] Recuperar o algoritmo contrastivo já existente para **Nova seed** no
  aplicativo nativo. Desde 06/11, a ação usa `algorithmicNextSeed()` em vez de
  sortear outro `uint32` sem relação com a realização atual; startup permanece
  aleatório. O teste `batruc_seeded_recipe_test` cobre determinismo e diferença
  entre a receita atual e a próxima proposta. Em 07/11, a receita também passou
  a escolher entre as oito leis A, família sonora, groove, compasso, intenção,
  dinâmica, profundidade de variação, movimento cíclico de V2, fonte/forma
  geométricas, bases de acoplamento/fase/transformação, fases de fórmula,
  regras por stem e parâmetros Chip 8-bit por stem. A busca contrastiva agora
  pontua essas dimensões. Reaplicar a mesma seed reconstrói os valores e limpa
  células Study editadas; com MIDI, recompõe desde a fonte preservada. Regras
  locais voltam aos genes. BPM, swing, panorama, ganhos e mute/solo continuam
  manuais. Família sonora é preservada em cenas schema 43 e a fonte MIDI/hash e
  declarações de licença/atribuição, versão da fonte e histórico básico de
  operações em schema 47; schema 48 preserva nota GM por ataque (schemas
  1–47 continuam legíveis). Falta validar a repetição, as famílias/articulações
  audíveis e o round-trip MIDI na GUI.
  **Estado revisto em 11/11:** a seed aplica a família sonora que calculou e
  `algorithmicNextSeed()` também pontua a diferença de família; o músico pode
  trocar os timbres depois. A máscara continua sob escolha do músico e cada
  seed começa em Campo livre. BPM, swing, ganhos, mute/solo e os parâmetros
  personalizados de Chip 8-bit permanecem manuais. Panorama no MIDI agora tem
  ponto de partida e movimento por papel, editáveis pelo músico; fora do modo
  MIDI, panoramas seguem manuais. Corrigi tooltips
  que contradiziam esse comportamento. O BPM inicial MIDI também passou a ser
  associado pelo tempo absoluto entre pistas formato 1. Build Release serial do
  app concluído; a suíte CTest passou **18/18** em 68,86 s após a mudança do
  importador; `git diff --check` passou. Hash atual do app:
  `bfa0e4dc9d90900ef651f97afaad9af4deebbcdea1368b30888ad461c7e4e51d`.
  A comparação auditiva das famílias e seeds ainda está pendente.
- [ ] Fazer ensaio humano no **Study**: clique curto cria ataque; arraste do
  anel desloca a fase; arraste do vértice muda o tempo; `Shift` altera força;
  clique direito ou duplo clique remove ponto manual; `Alt` restaura a fonte
  calculada. Confirmar também atualização sonora na fronteira musical e
  persistência ao salvar/reabrir cena.
- [ ] Reabrir **Sobre** e **Aprender** em 1920×1080 após a correção final de
  camadas e confirmar que nenhum gráfico atravessa o modal, o cabeçalho ou o
  rodapé.
- [x] Auditoria estática de coerência factual dos textos ativos e tutoriais
  principais, concluída em 06/11: corrigi o limite de vozes em Sobre (2–8), o
  total então vigente de modelos sonoros, a distinção entre 12 slots persistidos
  e oito stems ativos, o tooltip espanhol de ciclos (V1–V8) e as referências
  README que confundiam guias estáticos com Learn completo. O marco de áudio
  inicial foi identificado como histórico. Arquivos nativos e README corrigidos.
  **Retificação em 11/11:** cinco modelos percussivos foram acrescentados depois
  daquela auditoria. O total atual é 18 (seis harmônicos/modais, onze
  percussivos e um Chip 8-bit); o Mixer oferece 17 famílias de timbre mais a
  opção Original/manual. Sobre e README já exibem os números atuais.
- [ ] Revisão final de gramática, acentuação e legibilidade dos textos no app
  aberto, com teste humano em PT/EN/ES/FR. A localização espanhola é parcial e
  recorre ao inglês para strings sem entrada no dicionário; verificar também
  truncamentos nos quatro idiomas. A escolha do idioma agora persiste nas
  preferências locais (`ui_language_v1`); confirmar o round-trip após reiniciar.
- [ ] Projetar e desenvolver **Learn** como experiência didática completa. O
  painel nativo “Aprender” tem nove tópicos; o oitavo agora enumera padrões
  binários de 3–16 posições, permite navegar pelo número, editar células,
  sortear padrões/compasso e ouvir uma prévia sintetizada sem alterar a
  composição. O nono explica baixo e Mixer. Exercício de reconstrução e
  percurso progressivo foram acrescentados e compilados em 11/11. Ainda faltam
  sequências didáticas completas com ritmos/contextos musicais e demonstrações
  guiadas; a orientação inicial de escuta contextual entrou em 11/11. O tópico
  **Som e modelo** agora explica em PT/EN/ES/FR os papéis MIDI V1–V8, notas GM,
  exceções TD-11 do GMD e um exercício de escuta por stem. A esteira sintética
  da caixa já existe; conferir no app sua relação com o corpo continua pendente.
  Revisar o conteúdo nos quatro idiomas e validar manualmente áudio, interação
  e apresentação visual também seguem pendentes.
- [x] Reconciliar as paridades de baixo e da curva `cycloid`. Em 07/11/2026,
  os três testes de baixo e o de geometria passaram após alinhar o trajeto
  relacional. A suíte CTest completa passou 18/18 após essas correções.
- [x] Corrigir o falso positivo da verificação de hard-pan: o áudio suaviza a
  mudança por 5 ms; o teste agora mede a energia após a rampa e exige vazamento
  inferior a 1e-5 da energia do canal esperado. `batruc_mixer_routing_test`
  passou em 07/11/2026.
- [ ] Completar a matriz de layout de Editor, Mixer e Baixo em 1080×800,
  1440×900 e 1920×1080, com diferentes DPI, idiomas e todos os estados
  visíveis. A auditoria automática atual cobre somente parte desses estados.
- [x] Fazer Editor, Mixer e Baixo caberem como páginas discretas em 1080×800,
  abertas pelo cabeçalho e sem navegação pela roda sobre o fundo. Capturas das
  três páginas foram inspecionadas em 06/11; o Viewport interno ainda deve ser
  substituído por um host de páginas numa refatoração arquitetural posterior.
- [ ] Concluir a padronização visual dos controles. Os onze knobs do Baixo já
  foram ampliados e reorganizados em duas linhas; ainda faltam unidades,
  larguras e estados consistentes para sliders, caixas e botões, além da
  distinção clara entre ação momentânea, alternância e seleção. A proposta SVG
  usa rótulos de 12–14 px; várias legendas atuais estão abaixo disso. Aumentos
  recentes de bounds precisam ser conferidos no binário, pois a largura do
  componente nem sempre aumenta o traço visível do LookAndFeel JUCE.
  Em 08/11, os textos de botões, caixas e labels passaram a ter mínimo de 13 px
  e os textos desenhados no grid Study foram ampliados; a compilação e a inspeção
  visual ainda são necessárias para detectar colisões, cortes e espaços vazios.
- [ ] Inventariar controles ocultos ou abaixo da área útil e decidir, para cada
  um, entre inspetor, aba local ou remoção. Confirmar em especial que **Campo
  livre**, Orbit, Study e os demais modos/controles continuam acessíveis depois
  do ajuste visual; nenhum controle essencial deve depender de descoberta
  acidental. Auditoria de código em 07/11: Campo livre fica no seletor fixo do
  rodapé; Orbit/Study ficam na vista vinculada e Study também tem atalho fixo.
  Diferenciei o seletor do campo de fases, mas falta confirmar esses bounds no
  binário e continuar a matriz dos demais controles.
- [ ] Executar a proposta de layout aprovada em
  `PROPOSTA_LAYOUT_EDITOR_CICLOS_2026-11-02.svg` sem tratá-la como mera
  referência: comparar o app ao wireframe, manter todas as funções existentes
  e registrar, item por item, o que foi implementado, adaptado ou continua
  pendente. Os painéis e a relação Editor/vista vinculada existem, mas a
  equivalência funcional e visual total não está demonstrada.
- [ ] Reorganizar o fluxo do produto em **Compor → Estudar → Mixer** sobre um
  único material musical. Compor reúne origem, células, relações entre stems,
  transformações e forma; Estudar expõe ciclo, geometria, função, fórmula,
  parâmetros e consequências nos ataques; Mixer e Baixo reúnem performance e
  síntese. Os atalhos atuais são **Compor**, **Mixer** e **Baixo**; Study é uma
  vista no Editor, com atalho de navegação próprio no rodapé. Os rótulos foram
  alinhados à preferência do autor, mas a reorganização funcional permanece.

### P1 — tornar os gráficos instrumentos de composição

- [ ] Consolidar o **Study** como editor principal: seleção inequívoca de voz e
  ataque, estado manual/calculado visível, desfazer/refazer, limpar/restaurar e
  edição numérica/teclado além dos gestos diretos. Em 07/11, a vista passou a
  manter e destacar o ataque selecionado, mostrar voz, índice/total e fonte
  manual/calculada; foram adicionados comandos visíveis de undo/redo, limpar a
  voz selecionada e restaurar sua fonte calculada, com histórico limitado a 32
  snapshots. Setas, Home/End, PageUp/PageDown, Delete e Ctrl/Cmd+Z/Y também
  editam pelo teclado. A tecla `E` abre entrada numérica de posição normalizada
  e força; falta ensaio visual/humano dos controles.
- [ ] Tornar visíveis a equação ativa, parâmetros, medidas e o encadeamento
  `fonte → operação → consequência musical`; o seletor de função reapareceu,
  mas ainda não constitui um inspetor completo de funções e cálculos.
- [ ] Criar variação explícita de funções por seed e por processos generativos,
  com travas por dimensão, intensidade, escopo temporal e reprodução exata.
  Em 07/11, a seed passou a escolher as oito leis, família, groove, compasso,
  expressão, geometria e parâmetros por stem, e reaplicá-la reconstrói esses
  valores. Ainda faltam travas por dimensão/escopo e escolha de quais objetos
  um processo generativo pode alterar. O modo manual sem seed deve continuar
  livre e não ser sobrescrito.
- [ ] Ligar Orbit, Study, ondas e demais vistas à mesma agenda sonora e indicar
  honestamente quando uma vista é apenas analítica. Todo gesto composicional
  deve produzir resposta visual e auditiva compreensível.
- [ ] Consolidar a fonte autoral em `CompositionDocument`/`CellLibrary`, com
  ID, origem, proveniência, operações e histórico bounded. Extrair o codec de
  cenas sem romper a leitura de esquemas antigos; o codec de 12 slots deve ser
  tratado como compatibilidade, e oito é o teto musical do host JUCE atual.
- [ ] Levar ao app operações já existentes no core, começando por rotação,
  densidade, força e interlocking; dar prévia, comparar, desfazer e publicar o
  mesmo snapshot para gráfico, áudio, cena e exportação. Não expor funções que
  só têm teste e não atuam no modelo autoral.
- [ ] Congelar o protótipo web como referência histórica até haver decisão
  explícita de produto web. Corrigir divergências de paridade úteis como
  contrato documental; não continuar duas implementações composicionais em
  paralelo.
- [x] Retirar **Áreas / relação de Stengel** da superfície de composição por
  não possuir efeito musical claro. O atalho do rodapé conduz ao **Study** e,
  em 06/11, foram
  removidos também o componente gráfico, os três knobs, o seletor e todo o
  encadeamento de layout que permanecia inacessível. O cálculo puro continua
  isolado no core como pesquisa/teste histórico, sem ocupar a interface.
- [ ] Criar editor de forma longa para sequências, repetições, pausas, retornos
  e seções A/B/A′/C, preservando a possibilidade de surpresa e performance.

### P1 — som, Mixer e criação de timbres

- [x] Criar o seletor de 17 famílias, com paleta de oito papéis estáveis — V1
  bumbo, V2 caixa, V3/V4 chimbais, V5 tons, V6 pratos, V7 percussão de mão e
  V8 apoio — e primeira variação sonora por stem. As famílias colorem o modelo
  com ataque/corpo, duração de ruído, cauda e inclinação espectral; não são kits/sample packs nem
  imitações de tradições. Ritmo/articulação permanecem separados.
- [ ] Fazer escuta comparativa do autor e aprofundar as receitas de timbre por
  estilo e papel de stem: excitação/ataque, corpo, espectro/modalidade, ruído e
  cauda, com compensação de nível e comparação A/B sobre a mesma célula. O teste
  compara os 17 perfis sobre o mesmo bumbo após casar o RMS, para uma simples
  diferença de volume não passar como timbre distinto. Isso não prova identidade
  sonora ou adequação estilística convincente; a escuta do autor segue pendente.
  O renderizador MIDI de referência já aceita família e ganho fixo para uma
  comparação offline com eventos idênticos; a escuta do autor segue pendente.
  Considerar uma engine percussiva mais rica ou samples quando houver fonte
  sonora licenciada; não declarar correspondência de gênero antes da escuta.
  O seletor superior **Global** segue como fallback apenas para stems em Global.
  Em MIDI, a família agora colore os sons sem substituir os papéis das lanes.
  Recompilei `batruc_seeded_recipe_test` em 11/11 e a regressão dos 17 perfis
  por stem passou (1/1); isso não substitui a escuta A/B do autor.
  O renderer ancora as fundamentais percussivas (bumbo 55 Hz, caixa 185 Hz,
  chimbal/prato, membrana/tom e percussões de mão) para que não herdem a altura arbitrária
  da voz matemática. Ainda não cria kits sampleados.
- [ ] Confirmar no app a entrada dos stems MIDI na forma. O código atual
  desativa a redução formal quando existe fonte MIDI, portanto C1 deve manter
  todas as lanes ativas; ensaiar com solo de V2–V8 em C1/C2 antes de encerrar.
- [ ] Diferenciar melhor os timbres dentro das lanes GM. V5 já afina os tons
  por nota; V6 separa crash/ride/bell por ataque e corpo; os acessórios GM
  agora têm durações e alturas distintas. Ainda faltam fontes sonoras próprias
  e escuta comparativa de V5–V8 para confirmar a identidade de cada peça.
- [x] Criar uma matéria sonora 8-bit própria, selecionável manualmente.
  `SoundCharacter::chip8` combina fonte
  harmônica tipo pulso, ruído digital curto, sample-and-hold a 8 kHz e
  quantização assinada de 8 bits. É selecionável globalmente e por stem; foi
  anexado ao enum para preservar os valores das cenas antigas e entrou no
  teste comparativo de materiais. A família **8-bit** deixou de selecionar
  `chip8` automaticamente quando a paleta V1–V8 passou a preservar os papéis
  de bateria; hoje aplica um perfil digital de cor aos modelos percussivos.
- [ ] Projetar modelos digitais distintos de bumbo, caixa, chimbal e demais
  papéis para que a família 8-bit tenha identidade própria sem perder a
  função musical de cada stem. Primeiro passo concluído em 10/11: clock e
  quantização específicos por papel sobre os corpos percussivos existentes;
  ainda faltam fontes/excitações digitais próprias e avaliação por escuta.
- [x] Expor no inspetor do stem quatro controles próprios e audíveis de 8-bit:
  `DUTY` 12,5–87,5%, `CLOCK` 1–24 kHz, `BITS` 2–8 e `RUÍDO` 0–100%. Eles
  aparecem somente quando a matéria efetiva é Chip 8-bit, cabem na página sem
  scroll e persistem no schema de cena 41. A auditoria confirmou round-trip e
  zero sobreposição em 1080×800 e 1440×900.
- [ ] Completar o editor 8-bit com LFSR configurável, envelope, variações
  seedadas e, somente após definir uma linguagem bounded, fórmulas bytebeat
  A/B. O estudo do Caustic 8BitSynth é precedente conceitual; não há código
  copiado nem avaliador bytebeat implementado no Batruc.
- [x] Ligar duas leis de intensidade ao core, ao campo geométrico e ao áudio:
  seleção A/B, mistura 0–100%, parâmetro de profundidade λ separado e cena
  schema 42. Cenas antigas migram mantendo sua resposta de lei única. O botão
  A/B alterna entre A isolada e a mistura com mesma seed, fase e ataques; o
  controle altera o resultado/volta à versão B, servindo como reversão imediata.
- [ ] Completar a composição por cálculos visíveis: curvas A, B e mistura já
  aparecem na vista de intensidade, com seleção e blend ligados ao core e à
  cena schema 42. Em 07/11, a vista passou a exibir as equações ativas de A e B,
  `F=(1-b)A+bB`, `I_field=(1-λ)+λF` e os valores de `b` e `λ`; ainda faltam
  controles de coeficientes próprios das leis (os controles atuais escolhem
  leis, mistura e profundidade), histórico próprio além do toggle A/B e
  comparação com condições de nível/duração constantes. A vista agora identifica
  o multiplicador `I_field` de cada ataque selecionado, com marcador e valor;
  falta validar visualmente/por interação, e isso não representa a intensidade
  sonora final. O campo e o áudio usam as mesmas coordenadas.
- [ ] Fazer uma composição curta com célula-fonte, três variações nomeadas,
  papéis distintos entre stems e forma editável de introdução, desenvolvimento,
  retorno e coda. A seed deve propor relações e transformações reproduzíveis;
  o músico pode travar ritmo, função, timbre ou forma separadamente.

### Frentes para o banco rítmico e détournement

| Fonte | Aplicação possível | Proveniência e direitos |
| --- | --- | --- |
| [Groove MIDI Dataset](https://magenta.tensorflow.org/datasets/groove) | Referência prioritária: performance humana, timing e velocity; 57 arquivos do GMD local têm rótulos `latin/brazilian-*` | CC BY 4.0 indicado na documentação consultada; atribuição e metadados; os rótulos não atestam autenticidade cultural |
| [Lucerne Groove Research Library](https://www.grooveresearch.ch/) | Estudo de microtiming e relação bateria/baixo | Auditar termos por tipo de arquivo antes de redistribuir |
| [Dataset of Drum Patterns 2, Universidade de Trento](https://zenodo.org/records/18395007) | Variações tocadas para estudar transformação humana | Confirmar licença no registro/pacote |
| [CInBalada, Centro de Informática da UFPE](https://doi.org/10.1007/BF03192563) | Dissertação/artigo descrevem cerca de 200 MIDI monofônicos de percussão, incluindo fontes de samba e maracatu; pesquisa prioritária para recuperar a base | MIDI não localizado; licença do artigo não cobre automaticamente a coleção; ver [registro da investigação](midis/README.md) |
| [BRID, UFRJ/UFAM e colaboradores](https://zenodo.org/records/14051323) | Áudio e anotações de samba, partido-alto, samba-enredo, capoeira e marcha; referência de escuta/análise, não MIDI | Conferir licença e escopo no registro antes de derivar |
| [Banco de Partituras Brasileiras, Funarte](https://partiturasbrasileiras.funarte.gov.br/) | Partituras PDF como apoio a estudo e transcrição, não MIDI direto | Autorizações variam por obra |
| [DrumMachinePatterns](https://github.com/stephenhandley/DrumMachinePatterns) | Padrões textuais potencialmente convertíveis em células | Licença do código/repositório não prova direitos da origem editorial |
| [POP-ROK](https://github.com/patchbanks/WaivOps-POP-ROK) | Material pop/rock e referência de dados simbólicos | CC BY 4.0 declarado; áudio WAV grande fica fora do app |

As fontes adicionais e os padrões derivados ainda não foram importados nem
liberados para distribuição. O GMD já está preservado e é lido pelo importador;
cada célula derivada deverá manter fonte, licença, recorte, transformações e
seed.
- [ ] Criar editor de timbre original por voz, combinando excitação, corpo ou
  ressonador, espectro/modalidade, ruído, envelopes, distorção e modulação;
  permitir salvar, comparar e recuperar receitas em cenas.
- [ ] Acrescentar processos performáticos de glitch, stutter, bitcrush,
  granularidade, feedback e recombinação com limites de ganho, CPU e densidade.
- [x] Implementar medidores reais por stem no Mixer. As barras V1–V8 leem o
  RMS após o fader de cada voz, antes do pan e do master; o slider de nível
  continua sendo controle, não medidor. Validar visualmente na próxima
  abertura normal do app.
- [x] Implementar editor de trajetória cíclica independente por stem, playhead,
  retorno ao Campo, modulação por pan-base/MOV/VAR, atualização no próximo
  bloco e persistência schema 56; teste focal e round-trip passaram.
- [ ] Inspecionar o editor de pan no Mixer em janela real: confirmar leitura,
  arraste, playhead, alternância Campo/Desenho, salvamento/reabertura e ausência
  de sobreposições. Confirmar pela escuta que cada material percussivo responde
  com movimento discreto e útil.
- [ ] Tornar a escuta **A/B** inequívoca: indicar versão ativa, manter ganho
  comparável, produzir diferença audível e preservar a mesma célula/seed para
  comparação. O autor ainda não percebeu diferença no ensaio anterior.
- [ ] Validar musicalmente as 18 matérias existentes e as relações entre
  percurso, timbre, movimento, acoplamento e profundidade antes de expandir a
  biblioteca.

### P1 — Baixo, metrônomo e gravação

- [ ] Recalibrar o módulo Baixo. O registro anterior reporta knobs ADSR/VCF de
  116×84 em duas linhas; a revisão de layout em curso altera bounds na fonte e
  ainda precisa de confirmação no binário. Uniformizar também Gate, nível, LFO
  e percurso sem reduzir o piano roll e os gráficos.
- [ ] Fazer escuta controlada de envelopes, filtro, LFO, leis de duração,
  percurso e dinâmica logística; registrar quais controles mudam realmente o
  áudio e quais ainda são somente visualização ou hipótese.
- [x] Implementar contagem de entrada de um compasso pelo botão **ENT**; só
  inicia com o metrônomo ativo, segura o transporte durante a contagem, permite
  cancelar e persiste no schema 51. Cenas anteriores migram com a opção
  desligada.
- [ ] Validar acentos, subdivisões, swing, TAP, ligar/desligar, contagem de
  entrada, cena e presença do metrônomo na gravação no app.
- [ ] Validar `REC` no dispositivo real: destaque vermelho, relógio, interrupção
  pelo mesmo botão, limite ajustável (padrão de cinco minutos), WAV/sidecar,
  teto, latência e encerramento seguro.

### P1 — fluidez, segurança e aceitação musical

- [ ] Medir tempo de frame, repaints e cadência efetiva dos gráficos. O medidor
  **CPU AUDIO** não mede UI/GPU; CPU baixa não explica, sozinha, movimento
  truncado.
- [ ] Desacoplar atualização visual do callback, invalidar somente regiões que
  mudaram e reduzir trabalho fora da tela; verificar compositor/GPU apenas com
  evidência, sem pressupor que a placa de vídeo seja a causa.
- [ ] Fazer sessões de escuta e performance com diferentes seeds, modo manual,
  oito vozes, Mixer, Baixo, metrônomo e gravação; verificar ausência de clique,
  silêncio inesperado, clipping e picos de CPU.
- [ ] Adicionar testes de regressão para cenas com células manuais e funções,
  compatibilidade de esquemas antigos e transições entre edição manual e
  geração seeded.

### P2 — forma longa, seção e duração

- [ ] Separar comprimento do motivo, repetições/ciclos totais e duração-alvo;
  representar peças de 65 ciclos, 4:20 e motivos repetidos até 5:00, com final
  em fronteira musical e estimativa antes de tocar.
- [ ] Criar `PiecePlan` com introdução, desenvolvimento, transição, clímax,
  retorno e coda; cada seção pode declarar extensão e operações próprias.
  Oferecer origem manual, seeded e híbrida, mantendo edição autoral posterior.

### P3 — portões antes de distribuição pública (adiados)

**Estado em 11/11/2026: não publicar ainda.** O app e o core têm build Release
local, mas isso não constitui pacote de distribuição nem validação de release.
O build mais recente confirma compilação, não aceitação musical. Ainda faltam
escuta comparativa de stems/famílias e seeds, variação de frase sobre MIDI,
  ensaio dos controles e cenas schema 56 no binário candidato e revisão de
  Learn/tutorial.
O baixo agora combina sua seed própria com a seed principal, mas essa correção
segue sem validação auditiva. Commit ou push versiona/sincroniza código; não
equivale a autorizar ou fazer uma release pública.

- [ ] **Aceitação funcional e musical:** concluir os ensaios P0/P1 da lista
  acima, incluindo Mixer e famílias audíveis, interação Study, salvar/reabrir
  cenas e MIDI schema 56; revisar regressões e conteúdo Learn/tutorial. O teste
  precisa ocorrer no binário que se pretende distribuir.
- [ ] **Direitos e créditos:** auditar dependências, avisos, licença do projeto,
  termos do JUCE e créditos. Para MIDI/áudio de terceiros, conferir proveniência
  e direitos de redistribuição; campos de licença/atribuição declarados pelo
  usuário no schema 47 não foram verificados nem concedem autorização.
  **Inventário preliminar (07/11):** o projeto declara AGPL-3.0-or-later e o
  CMake exige JUCE 9.0.3; o build atual não inclui a fonte JUCE no repositório
  nem define instaladores/empacotamento. O Groove MIDI Dataset está em
  `midis/` como fonte externa local, declara CC BY 4.0 e não é incorporado ao
  binário. Antes de distribuir cenas, exemplos ou corpus, ainda é necessário
  verificar avisos, créditos e proveniência do material redistribuído. Este
  inventário é leitura preliminar, não auditoria jurídica completa.
- [ ] **Nome e identidade:** verificar Batruc e nomes semelhantes nos registros
  pertinentes; disponibilidade continua sem confirmação.
  - **Checagem de 13/11/2026** (reforça a de 25/10; não é busca jurídica):
    - web: nenhum produto, empresa ou artista "Batruc";
    - marcas (busca indireta): nenhum "Batruc"; o mais próximo é BRATUC
      (EUA, classe 25, vestuário, 2025), sem conflito aparente;
    - domínios livres (RDAP): .com, .net, .org, .app, .io, .art, .music e
      .com.br; o batruc.com já foi registrado no passado e expirou;
    - GitHub: só os repositórios do autor;
    - software e instrumentos musicais: nenhum plugin, sintetizador, app ou
      produto "Batruc" (busca web dirigida, KVR Audio, lojas de apps). Nomes
      parecidos sem conflito direto: eBatuque (app brasileiro de loops de
      percussão; som próximo de "batuque"), Batalsoft (apps de
      instrumentos).
  - **Falta:** busca oficial no INPI (classes 9, 15 e 41) e no WIPO Global
    Brand Database, que exigem consulta interativa (autor ou agente de
    propriedade industrial).
- [ ] **Fontes e proveniência:** se entrar código externo, fixar versão/commit e
  auditar licença e origem por arquivo. Decidir o que pode entrar em remoto e
  catálogo sem publicar brainstorm, mídia externa ou dados autorais privados.
- [ ] **Pacote e compatibilidade:** definir sistemas e formatos da primeira
  edição; gerar instaladores/pacotes reproduzíveis, conferir conteúdo, avisos e
  dependências e validar instalação, inicialização, áudio, salvar/reabrir cenas
  e remoção em cada sistema escolhido. O alvo conceitual inclui Linux, Windows
  e macOS standalone; o CMake hoje configura o app standalone JUCE, sem regras
  de instalador/distribuição; nenhum desses pacotes foi aprovado por validação.
- [ ] **Materiais de lançamento:** revisar os quatro idiomas, instruções,
  acessibilidade e páginas de suporte para a versão efetivamente distribuída;
  decidir assinatura, canal e estratégia de atualização.
- [ ] **Decisão final:** só depois dos portões acima, autorizar explicitamente
  versão, pacote, canal e publicação. Até lá, Batruc permanece em incubação;
  não há release pública autorizada.

### P4 — memória de escuta (posterior à composição integrada)

- [ ] Depois de haver versões audíveis e comparáveis, permitir aceitar,
  transformar ou rejeitar trechos com proveniência e esquecimento limitado.
  Não chamar um banco de escolhas de aprendizado automático.

### Critério de encerramento desta fase

A fase de interface só estará concluída quando a navegação não depender de
rolagem oculta, todos os controles essenciais estiverem acessíveis e legíveis,
nenhum estado testado tiver sobreposição ou corte, os gestos do Study forem
confirmados pelo autor e os gráficos alterarem material musical de forma
audível, reversível e persistente. Expansão de timbres e processos generativos
vem depois desse fechamento, sem bloquear a investigação conceitual.

## Continuidade formal, famílias e knobs — 05/11/2026

Foi retirada a suspensão automática da quinta seção. C1 e C5 deixam de excluir
o baixo, as sustentações não são mais cortadas ao entrar em C5 e o estágio
passou a se chamar **Expansão**. Silêncios continuam possíveis como decisões
explícitas de célula, voz ou frase, mas não são mais impostos por uma regra
oculta da forma.

O Mixer ganhou um seletor visível com 17 famílias: rock, jazz, eletrônica,
latina, experimental, percussão, sonoplastia, efeitos especiais, 8-bit,
16-bit, metal, avant-garde, samba, folk, funk, computer music e Rasgo Modular.
Cada escolha aplica uma receita aos oito stems usando os doze modelos sonoros
atuais; os canais permanecem editáveis individualmente. Isso é a primeira
camada funcional, não a biblioteca final de síntese nem uma alegação de
reprodução fiel de instrumentos ou tradições.

No Baixo, os 11 controles rotativos passaram de três linhas de 88×64 para duas
linhas de 116×84. Build Release serial concluído e auditoria em
`/tmp/batruc-sound-families-knobs-20261105` retornou `captured: true` e zero
interseções do Mixer para V1/V2 em 1080 e 1440. As capturas Mixer/Baixo nas duas
larguras foram inspecionadas.

### Próxima implementação: duração e arquitetura da peça

Esta intenção foi incorporada à lista ativa **P2 — forma longa, seção e
duração**, no início do arquivo. Este cabeçalho é mantido como registro da
decisão de 05/11; não é uma segunda lista de tarefas.

### Corpus rítmico livre para recombinação — decisão de 05/11/2026

- [x] Auditar fontes primárias e escolher o **Groove MIDI Dataset (GMD)** como
  primeiro corpus simbólico candidato: MIDI-only pequeno, performances humanas,
  metadados de estilo/BPM/compasso e licença CC BY 4.0. O pacote oficial
  [MIDI-only de 3,11 MB](https://storage.googleapis.com/magentadata/datasets/groove/groove-v1.0.0-midionly.zip)
  dispensa WAVs; SHA-256 conferido com a página oficial: `651cbc524ffb891be1a3e46d89dc82a1cecb09a57c748c7b45b844c4841dcc1e`.
- [x] Preservar o ZIP MIDI-only oficial intacto em `midis/` e extrair uma cópia
  de acesso em `midis/groove/`, sem inclusão no binário. Em 08/11, os 1.189
  arquivos extraídos foram comparados por caminho e conteúdo com o ZIP (todos
  coincidem), e a verificação CRC passou. Registrar em
  `midis/README.md` a separação entre fonte, derivados e materiais não
  autorizados. A inspeção de `AUDIOS_RASGO/Zoutros` encontrou transcrições de
  repertório conhecido, padrões genéricos sem licença comprovada e bancos
  proprietários/crackeados; nada foi copiado para o Batruc.
- [x] Identificar no `info.csv` do GMD local o recorte `latin/brazilian-*`: 57
  entradas rotuladas, com todos os caminhos MIDI verificados presentes; 32
  baião, 5 samba, 5 songo, 3 ijexá, 3 samba-reggae, 2 bossa, 2 maracatu, 1
  frevo e 4 sem subtipo. A classificação é a do dataset, não uma validação de
  autenticidade ou cobertura cultural. Próximo: ouvir e avaliar essas células
  no Batruc, mantendo os rótulos como metadados de origem.
- [x] Criar e validar o núcleo offline de importação MIDI. O leitor Standard
  MIDI File formato 0/1 tem limites de 16 MiB, 64 tracks e 65.536 eventos,
  preserva posição/velocity, distribui percussão GM em oito lanes e oferece
  janelas de 1–16 compassos. Compilação Release serial e teste focado passaram.
- [x] Expor o fluxo MIDI no Study: seletor **Importar MIDI**, escolha de janela
  de 1–16 compassos, resumo visível, prévia com confirmação e aplicação ao
  modelo editável das oito vozes. O importador funciona também sem o GMD.
- [x] Associar metadados editoriais do GMD quando o arquivo selecionado tem
  correspondência de caminho com uma linha do `midis/groove/info.csv` local.
  A prévia mostra estilo, beat/fill, compasso, BPM e divisão; a cena schema 55
  preserva os campos separados da licença/atribuição. Arquivos sem
  correspondência não recebem rótulo inferido; a associação por caminho não
  equivale a validar o checksum ou os direitos do arquivo.
- [x] Na importação, associar cada lane rítmica ao timbre correspondente do
  Mixer seguindo a ordem comum de pistas: V1 bumbo, V2 caixa, V3 chimbal
  fechado, V4 chimbal aberto, V5 toms, V6 pratos, V7 percussão de mão e V8
  outros. A atribuição é editável depois e fica persistida na cena. **Limite
  verificado em 08/11:** essa associação escolhe modelos sintetizados do Batruc;
  MIDI contém eventos (nota, tempo e velocity), não o áudio/sample da bateria.
  A observação histórica de 08/11 sobre descarte da nota GM foi resolvida depois:
  schema 48 preserva a nota principal, schema 52 CC4 e schema 54 até quatro
  notas simultâneas com articulação por camada. A validação auditiva dos
  timbres continua pendente.
- [x] Preservar a identidade da nota GM por ataque até o plano de eventos, a
  reprodução e a cena, incluindo edição, seed e undo; selecionar articulação
  sintetizada por nota sem trocar o papel musical da lane. A cena schema 48
  guarda `midi_note` no ataque e continua aceitando schemas 1–47. Tooltips do
  Mixer agora explicam a função musical pretendida de cada lane; são papéis
  convencionais editáveis, e V8 deixa a função auxiliar a critério do músico.
- [ ] Fazer ensaio de escuta e confirmar timbres/articulações: o renderer ainda
  usa ressonadores sintetizados, não samples gravados. A nota GM orienta famílias
  de articulação e durações (por exemplo, chimbal fechado/pedal/aberto e crash/
  ride); verificar se as diferenças são audíveis e musicais no instrumento real.
- [ ] Projetar reprodução MIDI com samples/banco de bateria, com seleção de
  arquivo, fallback sintético explícito e controle de velocity/articulação.
  Verificar licença, redistribuição, latência, limites de memória e dependências
  antes de escolher um SoundFont ou incluir qualquer banco no pacote. O MIDI-only
  do GMD continua sendo fonte de eventos e não fornece os samples do GMD.
- [x] Registrar a versão da fonte declarada e um histórico limitado às últimas
  64 operações MIDI em cenas schema 48: importação, variação de acentos por
  seed, tipo de edição Study por voz, desfazer/refazer. Os direitos permanecem
  não verificados.
- [ ] Completar o manifesto por célula/derivado. **12/11 (Claude Code, retomando
  a tarefa interrompida do Codex):** cada operação MIDI agora grava um
  `MidiDerivedRecord` com:
  - identificador `drv-` + 16 hex, encadeado: SHA-256 da fonte, identificador
    anterior, operação e resultado;
  - operação e parâmetros (o que vinha após ":", como seed e voz);
  - resultado (ataques por lane depois da operação).

  Limite de 64 registros, como o histórico. Persistência em **schema 57**
  (`midi_source.derived`); cenas até 56 abrem com o registro vazio. O
  diagnóstico `--diagnose-midi-scene` cobre a ida e volta e o fallback do
  schema 56 (passou). Build Release e CTest passaram, 18/18. Ainda falta: os
  parâmetros detalhados de cada operação além do que o nome já carrega, e a
  verificação independente de licença e atribuição, que não é tarefa de
  código.
- [x] Preservar a fonte MIDI imutável ao aplicar seed e variar somente os
  acentos/velocities de forma determinística, com fator até 0,52, modulada pela
  variação da receita e ponderada pelo papel de cada lane. O seed mantém posições,
  relações entre stems e notas GM intactas. A cena schema 44 armazenava nome,
  caminho, janela e oito células-fonte; lanes vazias ficam habilitadas para
  impedir eventos de fórmula.
  Schema 45 acrescenta o SHA-256 e o estado explícito de direitos não verificados;
  schema 46 acrescenta licença e atribuição declaradas pelo usuário; schema 47
  registra versão declarada da fonte e histórico de operações; schema 54 guarda
  até quatro notas simultâneas com velocity/CC4 por camada; schema 55 registra
  metadados editoriais locais do GMD, sem alterar cenas anteriores.
- [x] Implementar a primeira derivação de frase do MIDI por seed: manter fonte
  intacta e bumbo/caixa; criar pickups discretos entre ataques existentes de
  chimbal fechado, percussão de mão ou acessórios, quando houver nota GM e
  espaço. O histórico identifica a operação como
  `seeded_phrase_variation_v2`.
- [x] Variar por seed a densidade de lanes de apoio MIDI, com limites por papel
  e preservação de kick/snare, downbeats, acentos fortes e payload simultâneo.
- [ ] Ouvir múltiplas seeds e estilos; ajustar densidade e limites por papel de
  stem conforme a escuta. A implementação pode criar pickups de tom no
  último compasso e pulsos de ride GM 51/59, mas não varia outros pratos nem
  ritmos sem vozes auxiliares.
- [x] Rotacionar a célula MIDI inteira arrastando um trecho vazio do anel no
  modo Study · Ciclo compartilhado; nota GM, CC4 e forças acompanham cada
  ataque e o gesto entra no undo/redo ao soltar.
- [x] Redistribuir ataques em grades euclidianas de 8, 12, 16, 24 ou 32 passos
  pelo Study, mantendo velocity, notas MIDI, CC4, colisões e undo/redo.
- [x] Disponibilizar redistribuição euclidiana e microtiming determinístico
  (±5/10/20 ms) no menu Study com undo/redo e preservação dos dados MIDI.
- [x] Reduzir densidade por stem no Study, preservando downbeat, acentos fortes,
  onsets simultâneos e histórico de desfazer/refazer.
- [x] Intertravar uma célula selecionada com outra voz MIDI: reposicionar
  sobreposições em pontos médios livres próximos, preservando número de ataques,
  notas simultâneas e undo/redo.
- [x] Recombinar células MIDI entre stems por seed, preservando a matéria sonora
  do stem selecionado e registrando a operação para undo/redo.
- [x] Recombinar por compasso em frases MIDI com dois ou mais compassos,
  quando ambas as vozes têm pelo menos dois ataques no compasso correspondente;
  manter as notas/articulações da voz selecionada e registrar no undo/redo.
- [x] Adaptar densidade em pares de lanes MIDI de apoio compasso a compasso;
  manter downbeats e ataques mais fortes e registrar ambas as células como uma
  única edição de undo/redo.
- [ ] Escutar as transformações disponíveis em estilos diferentes e avaliar se
  o primeiro passo euclidiano atende ao fluxo musical.
- [x] Filtrar catálogo local pelo estilo declarado, beat/fill, compasso e faixa
  de BPM usando `info.csv`, sem inferir estilo pelo nome do arquivo.
- [x] Filtrar também pela contagem de ataques no primeiro compasso (0–8, 9–16,
  17+), calculada dos eventos MIDI e limitada a 64 MiB por abertura do catálogo.
- [ ] Conferir visualmente o fluxo da janela e validar por escuta/análise se as
  faixas baixa, média e alta são úteis musicalmente. Não transformar rótulos
  gerais em representação cultural fiel.
- [ ] Manter E-GMD adiado: é útil para áudio/timbre/transcrição, mas replica o
  material simbólico do GMD e custa 90 GB na versão completa.
- [x] Rejeitar Lakh para incorporação/redistribuição no Batruc enquanto não
  houver proveniência individual confiável, apesar da licença do conjunto.
- [ ] Auditar padrões Hydrogen item a item; a presença pública do repositório
  não substitui a licença registrada nos metadados de cada contribuição.

**Registro histórico da etapa MIDI — 06/11/2026.** Implementação em
`src/core/MidiRhythmImport.*`, com teste determinista em
`tests/midi_rhythm_import.cpp`. A variação seeded é sempre recalculada desde a
fonte capturada na importação; edições Study posteriores são substituídas ao
reaplicar uma seed. O pacote MIDI-only oficial está preservado
localmente em `midis/`, mas não é baixado pelo aplicativo, extraído no projeto
ou incorporado ao binário. O preflight inicial encontrou 4,5 GiB livres em `/`; a limpeza
segura do cache APT liberou espaço suficiente, e `batruc_midi_rhythm_import_test`
e `batruc_app` compilaram em Release com `--parallel 1`. O CTest focado do MIDI
passou 1/1. Em 06/11, o fluxo também foi ligado à interface JUCE:
**Editor > Study > Importar MIDI** abre a seleção, exibe uma prévia explícita
e só aplica a célula após confirmação. O botão **Study** do rodapé navega até
essa vista sem abrir o seletor.
Próximo passo: preservar no manifesto a origem/licença de toda célula aplicada.

Naquela validação, a suíte mostrou divergências entre o protótipo
JavaScript e o núcleo C++ na geração do baixo. O contrato atual — máximo de oito
vozes e nenhum silêncio formal oculto em C1/C5 — foi alinhado e os testes de
receita, variação seeded e importação MIDI passaram. Permanecem pendentes três
paridades do baixo (`seeded_bass`, `seeded_voice_bass` e `cyclic_bass`), que não
devem ser escondidas ajustando fixtures: é preciso decidir qual agenda musical
é canônica e corrigir a outra implementação. Essa pendência foi resolvida em
11/11/2026, conforme a validação atual no início deste arquivo.

Resultado integral daquela passagem: build Release serial completo passou;
`node --check` de `app.js`, `score.js` e `interface-i18n.js` passou;
`prototype/test-score.cjs` passou; CTest terminou com 13/18 verdes. Além das
três paridades do baixo acima, continuam falhando duas regressões já existentes
e fora do importador: paridade da curva `cycloid` e hard-pan do mixer. O teste
MIDI passou dentro da suíte completa. Não houve escuta nem abertura da janela
naquela etapa.

## Correção de Study e navegação sem barra — 05/11/2026

O autor rejeitou a barra de rolagem como solução. A barra vertical foi
desativada; Editor, Mixer e Baixo continuam como destinos diretos dos botões
fixos no cabeçalho, enquanto a roda do mouse permanece disponível para os
grupos que excedem a altura da janela. No Study, arrastar um anel sem Shift
volta a deslocar a fase da voz; Shift preserva o ajuste radial de intensidade.
O seletor de curva/função permanece visível no Study para que as funções e seus
cálculos não desapareçam ao inspecionar os ciclos. A área foi reposicionada na
linha de controles do Study, junto dos botões de vista.

Validação: `git diff --check`, build Release JUCE serial e auditoria
`--audit-layout=/tmp/batruc-no-scroll-study-20261105` concluídos. A captura
1080×800 foi inspecionada: seletor, gráfico e navegação estão visíveis, não há
barra vertical e a auditoria reporta zero interseções entre controles visíveis
do Mixer para V1/V2 em 1080 e 1440. Binário Release atualizado em
05/11/2026 00:06 +0100, SHA-256
`432f86df07814587fef45429cff1e229fabe72368525fe4abe1a6db8b43e1d2d`.
Interação de mouse real no Study e uso no desktop do autor ainda precisam de
confirmação; a auditoria programática não substitui esse ensaio.

### Correção do painel flutuante em janela maximizada

A captura do autor em 1920×1080 revelou que o painel **Sobre** usava dimensões
fixas de 1140×752, começava sob o cabeçalho e calculava parte dos controles
internos pela largura da página, em vez da própria moldura. O painel textual
agora é compacto (até 760×360), centralizado somente na área útil entre o
cabeçalho e o rodapé e limitado a essa área durante o arraste. Aprender e os
painéis técnicos mantêm dimensões próprias, também limitadas ao viewport. O
layout interno passa a usar a moldura real do painel; fundo, título e botão
Fechar ficam no mesmo sistema de coordenadas.

A primeira verificação em 1920×1080 confirmou a dimensão compacta, mas revelou
uma segunda causa: o `resized()` promovia o gráfico Study depois de o modal ser
trazido à frente. A ordem foi invertida: o layout responsivo termina primeiro;
depois fundo, título, conteúdo e Fechar estabelecem a camada modal definitiva.
Durante essa correção, a moldura compacta chegou a ser reutilizada
indevidamente como base de largura do Mixer e do Baixo. As coordenadas foram
separadas: `overlay` governa somente o painel flutuante; `panel` conserva a
largura integral das seções Mixer/Baixo antes do deslocamento vertical. Isso
remove a regressão de módulos estreitos ou desalinhados em janela ampla.

### Gestos diretos de pontos no Study

Clique e arraste no anel deixaram de competir. Um clique curto no anel ou numa
fatia vazia cria um ataque; movimento acima de 5 px passa a ser arraste de fase.
Arrastar um vértice muda seu tempo, Shift com movimento radial muda a força, e
clique direito ou duplo clique sobre um vértice manual o remove. A legenda do
próprio gráfico apresenta esse vocabulário. Pontos derivados da fórmula não
são apagados individualmente até a voz ser convertida em célula manual; isso
preserva a distinção entre fonte calculada e edição persistida.

Validação desta revisão: `git diff --check`, build Release serial e auditoria
`--audit-layout=/tmp/batruc-layout-fixed-20261105` concluídos. As capturas de
Mixer e Baixo em 1440×900 foram inspecionadas e recuperaram a largura integral;
o diagnóstico registrou zero interseções dos controles visíveis do Mixer para
V1/V2 em 1080 e 1440. Release gerado em 05/11/2026 01:26 +0100, SHA-256
`7500238b9d434d64e09c897a49c2e1dfec95071752d7924349d46c5a5a352c86`.
Ainda falta ensaio humano dos quatro gestos no Study e nova confirmação do
modal maximizado depois da correção final de camadas.

## Auditoria ergonômica e proposta de gráficos composicionais — 02/11/2026

Auditei capturas do app JUCE em 1080×800 e 1440×900, com topo, Mixer e Baixo.
O diagnóstico `--audit-layout=DIR` foi adicionado a `src/app/Main.cpp` apenas
para capturas locais, sem alterar o fluxo normal nem o áudio. Evidência:
`/tmp/batruc-layout-audit-20261102/` (`captured: true`). As capturas mostraram
uma faixa vazia no topo da janela maior, gráficos que não crescem com a altura,
controles comprimidos no Mixer e colisões reais de bounds no Baixo: a linha de
transformações cruza ADSR-F, e os knobs avançam sobre LFO/percurso. A inspeção
completa e o redesenho estão descritos em
[AUDITORIA_LAYOUT_COMPOSICIONAL_2026-11-02.md](AUDITORIA_LAYOUT_COMPOSICIONAL_2026-11-02.md).
O wireframe vetorial está em
[PROPOSTA_LAYOUT_EDITOR_CICLOS_2026-11-02.svg](PROPOSTA_LAYOUT_EDITOR_CICLOS_2026-11-02.svg);
ele representa a proposta; a organização visual principal foi implementada
em 03/11, enquanto a edição por vértice permanece pendente.
Por orientação do autor, o wireframe usa o lettering aprovado em escala menor,
sem a linha “matemática · física · música”, e preserva o rodapé de créditos.
O cabeçalho JUCE recebeu a mesma redução e retirada de texto; nenhuma
alteração foi feita no rodapé de créditos. Build Release serial concluído;
capturas de 1080×800 e 1440×900 em `/tmp/batruc-header-brand-20261102/`
retornaram `captured: true` e confirmaram logo, cabeçalho limpo e créditos
visíveis. O SVG validou como XML e foi renderizado com Inkscape. A busca de
espaços finais não encontrou ocorrências. SHA-256 do app Release:
`3b2f20a8abfada0c3058dfb82c3300ee935e344f0947f85e92d25b1f79cb4674`.

Direção escolhida: editor circular de ataques por voz como superfície principal
de composição, com Onda/Orbit/Study/curvas vinculados à mesma agenda sonora;
Mixer com miniaturas rítmicas e inspetor de stem; Baixo com piano roll/lei
maiores e controles sonoros em áreas sem colisão. O modelo de onset individual
por vértice ainda precisa ser codado; a fase arrastável em Study não equivale a
isso. A próxima etapa é refatorar as áreas responsivas e resolver a colisão do
Baixo antes de adicionar interação gráfica nova. Build Release serial passou;
SVG validado como XML e renderizado localmente com Inkscape; a busca de espaços
finais não encontrou ocorrências nos arquivos editados. SHA-256 do binário
Release: `5acd875f1bbb9db7829e2c5069f99ff7805dfe783adfc0a49964f22ce5e18c78`.
Não fiz ensaio humano de mouse nem matriz completa de DPI/perfis. O diagnóstico
fechou o app após as capturas; nenhuma instância ficou aberta.

## Metrônomo de estudo no JUCE — 02/11/2026

Foi integrado um metrônomo sintetizado e independente do transporte: acompanha
o BPM e o compasso atuais, pode tocar com a composição parada, oferece 1–8
subdivisões por tempo, sons de madeira/digital/sino/aro/grave, acento do
primeiro tempo, swing opcional vinculado ao controle Swing e nível separado de
−48 a 0 dB antes do master e do limitador. O controle liga/desliga é o mute do
próprio canal. O metrônomo começa desligado para não reintroduzir cliques
inesperados. Quando gravando, o áudio do metrônomo acompanha a saída master.
As configurações passam a ser persistidas em cenas JUCE no esquema 38; versões
anteriores carregam os padrões seguros (desligado, 1 subdivisão, −18 dB).

O callback de áudio usa estado fixo e valores atômicos, sem alocação, lock ou
I/O. Os cinco sons são síntese local de dois parciais com decaimentos distintos.
O botão TAP calcula BPM pela mediana de até cinco intervalos recentes, descarta
intervalos fora da faixa de 20–220 BPM e atualiza o andamento compartilhado pela
composição, baixo e metrônomo. Corrigi também a colisão entre o rótulo Swing e o
slider; o TAP ocupa o espaço entre os controles de fase e BPM, com separação
geométrica explícita. Ainda não há contagem de entrada, equivalente no HTML ou
validação auditiva e visual feita pelo autor. Depois do build, testar acentos,
subdivisões ímpares/pares, swing, início/parada, TAP, gravação e carregamento de
cena; confirmar que o metrônomo não clipa nem deixa caudas ao ser desligado.

## Operações rítmicas e matéria glitch — pesquisa incorporada — 02/11/2026

O Atlas ampliou a pauta para flashcore, speedcore, extratone, splittercore,
gabber, crossbreed, darkstep/techstep, breakstep, wonky, skweee, illbient,
microsound, Onkyo, improvisação eletroacústica, live coding/algorave, gqom,
singeli, Shangaan electro, kuduro, batida lisboeta, hard drum, UK funky, Jersey
club e outras cenas de club experimental. A lista do autor é uma pauta de
escuta, não uma taxonomia fixa: alguns termos nomeiam cenas, outros técnicas e
práticas; eles não devem virar presets que imitam artistas nem categorias
culturais intercambiáveis.

Para composição, o que interessa são operações concretas: (1) aceleração e
fusão de microeventos com limites; (2) corte, repetição e recombinação de
breaks/células; (3) acento, massa, saturação e pulso; (4) ciclos e acentos
assimétricos ligados ao gesto corporal; (5) baixo instável e groove deslocado;
(6) espaço, silêncio, ruído e decaimento; (7) algoritmo como gesto
performático. O Batruc deve deixar combinar essas operações sem prender a seed
a um gênero. Tradições rítmicas locais e afro-diaspóricas exigem pesquisa
contextualizada e proveniência, não extração de padrões como material exótico.

Próximo trabalho de código: inventariar os módulos livres já codados no RASGO
e desenhar uma camada autoral Glitch por stem, com retriggers/ratchets,
microcortes, omissões, bursts, deslocamento curto, recombinação de células e
tratamentos de matéria (redução de taxa/resolução, sample-and-hold, saturação,
delay curto). Limitar densidade e taxa por Nyquist/eventos, usar fades e ganho
compensado e manter seed reprodutível; não introduzir custo variável,
alocação, lock ou I/O no callback. Ainda não foi codado nem validado
musicalmente. A próxima experiência deve comparar três operações contrastantes
em vozes separadas, observando a agenda, o áudio e o custo de CPU.

**Referências e limites:** ver a entrada ampliada no
[Atlas de Referência RASGO](../../RASGO_DOCUMENTATION/ATLAS_DE_REFERENCIA_RASGO(4).md).
Entre as novas fontes estão a história do [extratone no Bandcamp Daily](https://daily.bandcamp.com/features/an-introduction-to-extratone-the-worlds-fastest-music-genre),
o panorama de [flashcore no The HARD DATA](https://www.theharddata.com/2017/08/20/initiatory-speedcore-developments/)
e a pesquisa acadêmica sobre [improvisação eletroacústica na LSU](https://repository.lsu.edu/gradschool_dissertations/6369/).
Para o arco histórico, consultar também a [história dos breaks no MusicRadar](https://www.musicradar.com/news/the-history-of-breaks),
a pesquisa sobre [EBM belga em Oxford Academic](https://academic.oup.com/book/12082/chapter-abstract/161438175),
o percurso composicional de [Xenakis no IRCAM](https://brahms.ircam.fr/en/composer/iannis-xenakis/biography)
e a [árvore Kicktime](https://kicktime.fr/) como índice exploratório, não
autoridade de genealogia. As referências informam escuta e pesquisa; nenhuma
técnica, padrão, áudio ou código dessas fontes foi incorporado.

### Complexidade rítmica instrumental e eletroacústica

A pauta de escuta agora atravessa também mathcore/math rock, djent e metal
técnico, prog/Zeuhl/RIO, jazzcore e M-Base, Totalism, Nancarrow, Xenakis,
breakbeats, música concreta e práticas eletroacústicas. Esses campos pedem
comportamentos composicionais diferentes: riffs contra compasso, ciclos
aninhados, cânones de andamento, aceleração, fragmentação, improvisação e
transformação da matéria sonora. O modo futuro de células A/B/C/D deve permitir
construir e recombinar frases completas, com pausas, duração, acentos e regra
de retorno; não deve reduzir a geração a um loop ou a um preset de estilo.

O modelo e a interface devem distinguir ciclo/contagem, polirritmia,
polimetria, métrica irregular, politempo e deslocamento de acento, expondo
onsets, duração, acentuação e convergências para que a terminologia corresponda
ao cálculo real. Referências acrescentadas ao Atlas: [IRCAM sobre Nancarrow](https://resources.ircam.fr/en/composer/conlon-nancarrow/workcourse),
[Music Theory Online sobre pontos de convergência](https://mtosmt.org/issues/mto.14.20.1/mto.14.20.1.nemire.html),
[Kyle Gann sobre Totalism](https://www.kylegann.com/PC060101-TotalismParadigm.html)
e [Steve Coleman/M-Base](https://m-base.com/music-collectives/). M-Base e tradições
afro-diaspóricas devem ser estudados com contexto, autoria e fontes próprias;
não serão abstraídos em padrões prontos sem proveniência. Isto amplia a
pesquisa e o roadmap, mas não significa que os recursos já estejam codados.

### Ritmos de tradições brasileiras, latino-americanas, africanas e asiáticas

Nova frente de pesquisa: ciclos aditivos e assimétricos, timeline patterns,
interlocking, partes complementares, chamada/resposta, ornamentação,
improvisação situada, variação de densidade e retorno. A interface deve poder
mostrar grupos e camadas separadamente e em conjunto, editar pausas e ataques e
fazer as alterações chegarem à mesma agenda usada pelo áudio. Candidatas para
um primeiro experimento autoral: três camadas abstratas de interlocking; grupos
aditivos com comprimentos distintos; deslocamento de entrada e uma frase de
retorno calculada. Não codificar um preset nomeado por tradição ou toque
ritual: pesquisar contexto, terminologia, detentores, proveniência e direitos
antes de qualquer uso específico.

Fontes contextuais ingressaram no Atlas: [IPHAN sobre Maracatu Nação](https://bcr.iphan.gov.br/bens-culturais/maracatu-nacao/),
[UNESCO sobre Gamelan](https://ich.unesco.org/en/RL/gamelan-01607),
[Smithsonian Folkways sobre música Ewe](https://folkways.si.edu/ewe-music-of-ghana/world/album/smithsonian),
Willie Anku sobre [ciclos e organização do ritmo](https://doi.org/10.30535/mto.6.1.2),
[CompMusic/UPF sobre tāla](https://compmusic.upf.edu/node/185),
[Encyclopaedia Iranica sobre īqāʿ](https://www.iranicaonline.org/articles/iqa/)
e [Music Theory Online sobre aksak](https://mtosmt.org/issues/mto.20.26.1/mto.20.26.1.clayton.html).
São entradas para pesquisa, não fontes suficientes para transcrever todos os
estilos listados pelo autor. Comparações entre tradição e matemática ocidental
devem descrever relações delimitadas, sem apagar contextos ou declarar
equivalências estéticas.

### Pesquisa brasileira — ciclos, camadas e viração

A pauta brasileira foi aprofundada no Atlas com fontes do IPHAN e estudos de
ritmo. Para o Batruc, os experimentos autorais podem investigar sustentação,
variação e resposta entre vozes; deslocamentos de acento sobre um pulso/ciclo;
células com pausas, interrupção/retomada e viração; densidade e troca de função
entre camadas; e forma com seções e retornos. Começar com material abstrato
novo, sem transformar tradições em presets. O mesmo evento calculado deve
aparecer no editor e no áudio.

### Pesquisa europeia — aksak, compás, diálogo e microtempo

Nova frente registrada no Atlas em 02/11/2026. Usar aksak/danças balcânicas,
compás flamenco, txalaparta e polska/springar como perguntas composicionais
distintas, junto às pesquisas europeias de rock experimental e eletrônica que
já constam no Atlas. Não presumir que uma fórmula de compasso ou agrupamento
represente uma tradição inteira.

**Proposta de experimento (ainda não codada):**

1. Criar uma célula abstrata editável com agrupamento aditivo configurável
   (primeiro caso de estudo: 2+2+3), acentos, pausas e início de fase.
2. Criar duas vozes alternadas em chamada/resposta, podendo responder com
   transformação da célula, pausa ou deslocamento, e não somente copiar o
   evento anterior.
3. Investigar pulsos de durações desiguais com soma do ciclo controlada;
   manter separado esse modo de microtiming humano e de alteração de BPM.
4. Desenhar eventos, grupos, acentos, fase e pontos de retorno no gráfico
   editável e verificar que o áudio usa exatamente a mesma agenda.
5. Comparar estabilidade, diversidade e clareza musical com a geração atual;
   limitar alterações por ciclo e oferecer retorno ao estado/seed inicial.

Fontes de partida: [Clayton sobre aksak](https://mtosmt.org/issues/mto.20.26.1/mto.20.26.1.clayton.html),
[Bridges 2013 sobre síntese aksak/Euclidiana](https://archive.bridgesmathart.org/2013/bridges2013-593.pdf),
[Instituto Andaluz del Flamenco sobre bulerías](https://www.juntadeandalucia.es/aaiicc/flamenco/node/3867),
[capítulo acadêmico sobre compás flamenco](https://uvadoc.uva.es/bitstream/handle/10324/67366/Global%20Music%20Cultures%202020.pdf?isAllowed=y&sequence=4),
[estudo de Hurtado sobre txalaparta e interação generativa](https://users.sussex.ac.uk/~thm21/ICLI_proceedings/2016/Papers/Long_Papers/104_ICLI2016_enrike_06.pdf)
e [pesquisa sobre assimetria de polska](https://doi.org/10.1080/17459737.2021.2002446).
São referências de estudo, não autorização para reutilizar áudio, partituras,
transcrições ou padrões. Próximo passo prático: escolher uma operação, documentar
seu modelo de eventos e implementá-la como material autoral sem preset cultural.

### Pesquisa norte-americana — groove, interação e microtiming

Pauta de escuta registrada no Atlas em 02/11/2026: funk, soul/R&B/gospel,
go-go, second line/New Orleans, blues/jazz, hip-hop e sampling, disco/house/
techno e rock experimental. Tratar como histórias e práticas distintas. A
continuidade do go-go e sua chamada/resposta ao vivo, os papéis comunitários da
second line e o foco no groove do funk não se reduzem a um padrão de bateria ou
a compasso irregular. Microtiming não significa deslocar aleatoriamente a
caixa: pesquisas encontram variações correlacionadas e dependentes de contexto.

**Experimento proposto (ainda não codado):** comparar, com uma célula e seed
fixas, (a) agenda quantizada, (b) microtiming correlacionado por papel/posição/
acentuação e (c) diálogo responsivo entre vozes. Manter o pulso de referência;
mostrar no gráfico os onsets reais, a diferença em milissegundos, acentos e
causalidade da resposta. Em etapa separada, testar recorte, omissão, repetição,
rotação e recombinação de células, com fades para evitar cliques. Avaliar
diferença auditiva, estabilidade, variedade e custo de CPU; não promover
“swing”, “The One” ou timing de um artista a regra universal.

Fontes de partida: [Smithsonian sobre James Brown](https://www.si.edu/spotlight/james-brown),
[análise de second line, funk e timing de New Orleans](https://ethnomusicologyreview.ucla.edu/journal/volume/18/piece/699),
[Smithsonian sobre go-go](https://www.americanhistory.si.edu/explore/stories/go-go-funky-percussive-music-invented-washington-dc),
[microtiming correlacionado em jazz e rock/pop](https://arxiv.org/abs/1710.05608)
e [experimento sobre microtiming e swing no jazz](https://arxiv.org/abs/1904.03442).
Não reutilizar samples, áudio, transcrições ou padrões das fontes; associação a
práticas nomeadas exigiria pesquisa e direitos adequados. Próximo passo prático:
especificar uma transformação de microtiming e validá-la no modelo de eventos
antes de construir controles de interface.

### Pesquisa — música computacional, microsom e noise

O Atlas registra a pauta trazida pelo autor: composição algorítmica/estocástica,
processos generativos e autômatos, síntese granular/microsom/pulsar, modelagem
física, composição espectral, wave terrain, concatenação, sonificação, live
coding, improvisação interativa, síntese neural e diferentes práticas de
noise. São técnicas/campos que se cruzam, não vinte gêneros independentes.
Separar no Batruc geração de forma/eventos, produção da matéria sonora e
interação/feedback. A hipótese ritmo→textura→altura emergente depende do som e
da percepção; não usar frequências-limite universais.

**Ordem de investigação, ainda sem implementação:**

1. Auditar o scheduler, as matérias sonoras atuais e os custos por stem. Mapear
   o que já compõe eventos, o que apenas sintetiza e o que já modula sinal.
2. Selecionar um protótipo de baixo custo entre (a) nuvem de grãos sincronizada
   a evento/célula, (b) pulsar em que forma/densidade de pulsos molda timbre ou
   (c) parâmetro de composição alterado por descritor de áudio simples e
   limitado. Não começar por um banco de modelos neurais.
3. Para qualquer relação som→decisão, definir descritor, janela, latência,
   ganho, limites, estado/seed e comportamento na ausência de sinal; mostrar a
   relação no gráfico e manter botão de bypass.
4. Para ruído/distorção, separar intenção musical de falha técnica; controlar
   nível e caudas, prevenir DC/não finitos/clipping e impedir feedback acústico
   aberto por padrão. Processamento do callback deve usar memória e filas
   limitadas, sem alocação, lock ou I/O.
5. Comparar uma célula com matéria sonora atual e com o protótipo, em níveis
   equiparados. Medir diversidade, estabilidade, CPU e cliques; decidir pela
   audição se a mudança amplia composição e timbre.

Fontes iniciais: [MIT Press — *Microsound*](https://mitpress.mit.edu/9780262182157/microsound/),
[biografia/notas de Curtis Roads](https://www.curtisroads.net/bio/),
[Peter Hoffmann sobre síntese estocástica de Xenakis](https://www.iannis-xenakis.org/syntheses-stochastiques/),
[catálogo IRCAM de GENDY3](https://ressources.ircam.fr/en/work/gendy3)
e [Agostino Di Scipio sobre feedback situado](https://echo.orpheusinstituut.be/article/a-relational-ontology-of-feedback).
Elas fundamentam a pesquisa, não autorizam uso de áudio/código. Próximo passo
concreto: auditoria do código Batruc para identificar o menor protótipo que
conecte evento e matéria sonora sem adicionar dependências nem degradar CPU.

### Auditoria aplicada — composição generativa, matemática e física — 02/11/2026

Consultei o core JUCE e distingui o que já acontece no som daquilo que é apenas
representação ou possibilidade de pesquisa:

- `CompositionCore::buildEventPlan` calcula a agenda de eventos a partir da
  receita, ciclos/fases, regra e máscara. A seed escolhe uma receita; o botão
  de próximo seed busca deterministicamente uma candidata contrastante. Isso
  oferece receitas diferentes, mas não equivale a transformação autônoma da
  frase durante a reprodução.
- `PassageDevelopment.hpp` acrescenta memória seedável limitada e um arco de
  oito passagens ativas (A, B, A′, C, B′, Expansão, C/C′, retorno). O peso dos
  eventos das
  vozes é alterado em células da passagem; o baixo também recebe deslocamentos
  de registro. Assim existe desenvolvimento formal reproduzível, mas em grande
  parte a lista-base de ataques permanece a mesma. A avaliação de quanto isso
  se percebe musicalmente continua pendente de audição.
- `EcologicalRelations` já modela suporte, imitação, evasão e competição, mas a
  busca no core encontrou sua aplicação na geração do baixo em
  `BassComposition.cpp`; não encontrei essas relações no plano principal
  voz-a-voz. `GenerativeOperations` oferece inversão temporal, inversão de
  alturas e recombinação de compassos para o baixo, conectadas a controles do
  módulo do baixo, não a uma gramática geral das oito vozes.
- Orbit, Study e curvas/pêndulo mostram relações ou cálculos, mas não alteram
  todos a agenda. Cada gráfico só deve ser chamado de composicional quando
  produzir uma saída de eventos/controle usada pelo áudio.

**Incremento realizado:** o core possui a relação `callResponse`, que copia
ataques selecionados para uma voz-alvo com atraso, intensidade limitada,
seleção determinística por seed e teto de eventos. Os eventos derivados levam
marcador de proveniência no plano/trace. No scheduler do callback, um ramo
permite que eventos preparados por relação sobrevivam ao recálculo dinâmico da
voz; não acrescenta alocação, lock ou I/O. O renderizador offline expõe
`--voice-call-response` para V1→V2. A seleção é
feita sobre a forma inteira de sete ciclos, com quota arredondada de 42% das
chamadas cuja resposta pode ocorrer enquanto V2 está presente; a seed escolhe
as chamadas. O atraso é 1/16 da seção (0,5 s a 120 BPM), com passagem entre
ciclos quando necessário. As respostas são redistribuídas aos planos de ciclo,
entram na derivação do baixo e aparecem no manifesto JSON.

**Validação e limite:** alvo Release `batruc_offline_render` compilado
serialmente. A/B offline, seed `5EED2027`, em
`.local-build/offline-call-response-20261102/iteration-06/`: 5 chamadas V1
elegíveis produziram 2 respostas V2; V1 permaneceu em 7 eventos, V2 passou de
14 para 16, e V3–V5 mantiveram 4/3/2. O baixo passou de 23 para 24 eventos. Nos
stems PCM16, 581.285 de 5.376.288 amostras de voz e 511.954 de 5.376.288
amostras de baixo diferem do controle; RMS de vozes 0,02607→0,02661 e de baixo
0,003315→0,003347; diferenças máximas 0,0585 e 0,1342. Isso confirma que os
eventos chegam ao sinal e à derivação do baixo, mas não substitui audição nem
aprova musicalmente a relação. Os WAVs e manifestos são evidência local; a
ferramenta não reproduz áudio para avaliação auditiva nesta sessão. A relação
ainda não está ligada à interface JUCE ou ao transporte ao vivo. Antes de
integrar relações ao vivo, falta a escuta A/B do autor; depois dela, prototipar
controles e visualizações explícitos sem alterar a agenda ao vivo antes da
aprovação musical.

**Experimento 2 — mapa logístico na dinâmica do baixo:** o core recebeu
`applyLogisticBassDynamics`, uma operação determinística e limitada que aplica
`x[n+1] = r·x[n]·(1−x[n])` à velocidade dos eventos do baixo em ordem temporal
absoluta. A seed fornece `x₀`; o protótipo usa `r=3,82`, aquecimento de 12
iterações e profundidade 0,28, com fator `1 + depth·(2x−1)` limitado à faixa
segura da velocidade. Não altera quantidade, posição, altura ou duração dos
ataques. O estado e o multiplicador ficam registrados por evento no manifesto,
para futura representação gráfica. O renderizador oferece `--logistic-bass`;
isso ainda não está conectado à interface ou ao callback de reprodução ao vivo.

**Validação e limite:** builds Release seriais de `batruc_app` e
`batruc_offline_render` concluídos. A/B com seed `5EED2027` em
`.local-build/logistic-bass-20261102/iteration-02/`: os 23 ataques mantiveram
tempos, alturas e durações; o stem de vozes ficou PCM idêntico. No baixo,
464.592 de 5.376.288 amostras diferem, RMS 0,003315→0,003637 e diferença de
pico 0,01093. Os estados registrados vão de 0,2702 a 0,9234 e os fatores de
0,8713× a 1,2371×. O pico estimado do master passou de 0,2735 para 0,2745,
sem redução do limitador. A evidência confirma modulação mensurável e
estabilidade do restante da composição; a escuta crítica ainda é necessária
para julgar se o padrão de acentos é musicalmente útil.

**Integração JUCE opt-in — 02/11/2026:** o botão `MAPA LOGÍSTICO` ativa a
mesma transformação durante a preparação do plano do baixo; desativado por
padrão e salvo em cenas schema 39. Não há recorrência no callback de áudio. A
seed da receita e a constante XOR `0x4C4F4749` determinam o estado inicial; os
demais parâmetros mantêm `r=3,82`, 12 iterações e profundidade 0,28. Alturas,
posições, durações e contagens permanecem intactas. O gráfico do percurso do
baixo ganha uma faixa inferior com os estados por evento da última cópia
publicada ao áudio; um plano ainda à espera do slot não aparece. Em composição
manual, a fase usa o tempo da seção. A faixa some quando
a opção está desligada e não substitui a curva F+C de altura/integral.

**Validação da integração:** o alvo Release `batruc_app` compilou em job único,
sem avisos na compilação final. Binário:
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
gerado em 02/11/2026 às 20:32; SHA-256
`904a75ca33ac4d659b19593a01f22e30ca57e0164886ba4fec327ff7c7d36272`.
Abri a compilação anterior da mesma integração e inspecionei o módulo do baixo
em janela 1920×1006: botão
logístico e controles cabem na linha sem sobreposição aparente. Não confirmei
visualmente a faixa com o mapa ligado nem fiz round-trip de cena; ambas seguem
pendentes. Ao tentar clicar no toggle, a coordenada atingiu o transporte e a
reprodução começou; parei-a logo em seguida, sem iniciar gravação. O app ficou
pausado. Depois dessa inspeção, corrigi a origem do gráfico de plano calculado
para plano publicado e recompilei; a inspeção visual não foi repetida após
essa correção de dados. A A/B numérica offline não valida a integração ao vivo. A avaliação
musical do autor continua pendente; o botão opt-in evita alterar o som por
padrão.

**Inspeção de 02/11, 20:29:** o binário acima abriu em dois monitores. A
automação X11 voltou a acionar `Tocar` ao tentar atingir o módulo do baixo;
encerrei a instância sem gravar nem salvar cena. A faixa do mapa ligado segue
sem inspeção visual. Uma sondagem sem clique do ponteiro não pôde abrir o
display no sandbox, então a próxima conferência deve usar interação local
confiável ou um ambiente de tela único. Nenhum código do instrumento mudou
nessa segunda inspeção.

**Correção visual de 02/11, 20:32:** a auditoria das cores encontrou o traço
do mapa branco (`#f8f6ec`) sobre o fundo quase branco do gráfico (`#f1f1e8`).
A faixa agora recebe fundo claro próprio, linha escura e pontos vermelhos, sem
ocupar o gráfico F+C. Release recompilado sem avisos. A correção ainda não teve
inspeção visual com o mapa ligado. Pela luminância relativa das cores declaradas,
o contraste do traço passou de 1,05:1 para 13,95:1; os pontos têm 4,19:1.
Essa medida de fonte não substitui a inspeção renderizada. `Xvfb` e `xvfb-run`
não estão instalados, e a automação nos dois monitores acionou o transporte
duas vezes. A instância de teste foi encerrada; não repetir cliques por
coordenadas nesse desktop.

**Fechamento do experimento 2 — 02/11, 20:50:** acrescentei um diagnóstico
local por linha de comando (`--diagnose-logistic=DIR`) que abre o app, ativa o
mapa, salva e reabre uma cena schema 39, verifica a leitura de uma cena schema
38 sem a nova opção e captura o módulo do baixo, encerrando a janela em
seguida. A primeira execução expôs uma incompatibilidade preexistente no
leitor de cena: o slider de ciclos aceita 0,5–16, mas a validação de
`variation_base_cycles` aceitava apenas 1–12. O leitor e o valor capturado sem
seed agora usam 0,5–16, coerentes com o controle. O diagnóstico final retornou
`scene_round_trip: true` e gerou
`/tmp/batruc-logistic-final-20261102c/logistic-bass-view.png`. A inspeção da
captura confirmou o botão `MAPA LOGÍSTICO · ATIVO` e a faixa inferior de
estados visível, com traço escuro e pontos vermelhos, sem sobreposição nessa
região. A captura foi feita sem clique automatizado e não avalia fluidez nem
qualidade musical. O app foi encerrado ao fim do diagnóstico. Build Release
serial e `git diff --check` concluíram. SHA-256 do binário final:
`a9553ff8f049f7d89cfb7298aa466d3590dc54f79c19278623c3d69e0dcc9827`.
Próximo passo musical: escuta A/B pelo autor e ajuste de profundidade/lei de
acentos somente se houver ganho composicional perceptível.

**Revisão auditiva do contraste — 02/11/2026:** após o autor não perceber
diferença no A/B, confirmei que o mapa altera apenas a velocidade dos ataques,
sem mudar notas, tempos, duração ou timbre. A profundidade 0,28 produzia
RMS do stem do baixo 0,003315→0,003637. Aumentei a profundidade opt-in
para 0,80 (limite validado 0,85) e recompiliei app e renderizador Release
em job único. Com seed `5EED2027`, o stem do baixo passou de RMS
0,003315 para 0,004294, com multiplicadores 0,632–1,677 e pico PCM
0,10886; o stem das vozes permaneceu byte a byte idêntico. No master,
o RMS passou apenas de 0,018912 para 0,019012, e a estimativa de pico real
ficou em 0,2764, sem redução do limitador. A variação continua discreta na
mistura completa por mascaramento; para avaliar os acentos, usar Solo no stem
do baixo com a mesma seed e alternar o botão. Não há evidência de que o novo
ajuste seja musicalmente melhor sem escuta do autor. Evidência local:
`/tmp/batruc-logistic-strong-20261102.wav` e stems/JSON adjacentes.
Arquivos de código: `src/core/GenerativeOperations.hpp` e `.cpp`;
documentação: `README.md` e `TAREFAS.md`. Próximo passo: escuta comparativa
em Solo e, se o objetivo for uma mudança perceptível na música inteira,
projetar uma operação composicional distinta que altere acentos, densidade
ou timbre com controle explícito. A busca por espaços finais nos quatro
arquivos alterados não encontrou ocorrências. SHA-256 do app Release:
`bd91f7b084bd6c886b9fd30321c02bd044e1a18031a2b07c075dbc3c7b388b11`;
do renderer Release:
`4f9d3fdda0638ce2d0a77f0bd9125e1f1e062e9684e2b61e480ad5a30c54b395`.

**Articulação tímbrica pelo mapa — 02/11/2026:** atendendo à percepção de
diferença insuficiente na mistura, o estado `x` já calculado para cada ataque
passou a controlar também a abertura do filtro (`0,28 + 3,2x²`) e o
comprimento do gate (`0,28 + 0,72x`). O modo desligado mantém escala de filtro
e gate originais. A agenda, as alturas e a duração escrita não mudam; só a
realização de cada ataque. Não há cálculo de recorrência novo no callback:
as multiplicações extras são locais à nota ou ao filtro que já existia.
A validação offline com seed `5EED2027` confirmou que o stem do baixo desligado
é byte a byte idêntico ao controle anterior, e que o stem das vozes permanece
idêntico entre as duas versões. O baixo com mapa tem RMS 0,003999 frente a
0,003315 do controle e 507.738 amostras PCM diferentes; o master passou de
0,018912 para 0,018981. Pico real estimado 0,27665, sem redução de limitador.
A diferença na mistura ainda pode ficar mascarada; não afirmo melhora
perceptiva sem escuta. O tooltip JUCE e o manifesto offline passaram a indicar
intensidade, filtro e gate. Evidência local: arquivos
`/tmp/batruc-logistic-articulation-control-20261102.wav` e
`/tmp/batruc-logistic-articulation-final-on-20261102.wav`, com stems/JSON.
Arquivos de código: `src/core/BassAudio.hpp`, `.cpp`, `BassComposition.hpp`,
`src/tools/offline_render.cpp`, `src/app/Main.cpp`; documentação: `README.md`,
`TAREFAS.md`. Próximo passo: escutar as duas versões em Solo e na mistura;
decidir se o mapa deve evoluir para uma transformação de densidade/pausas
explicitamente composicional. Build Release serial de app e renderer concluído;
diagnóstico JUCE de cena 39/38 retornou `scene_round_trip: true` em
`/tmp/batruc-logistic-articulation-gui-20261102/`. A busca de espaços finais
nos arquivos alterados não encontrou ocorrências. SHA-256 do app:
`c86857e0a1aed80d3a8ed7237d8e41a78191870ea9e4968c2cd13633ae31bf08`;
do renderer:
`329f81732541139a8a1dd68919faafb5b3c9038b2acc3589ee458b62fdc517cb`.

O roteiro matemático/físico e as referências primárias — [IRCAM Modalys](https://forum.ircam.fr/projects/detail/modalys/),
[Wolfram MathWorld — mapa logístico](https://mathworld.wolfram.com/LogisticMap.html)
e [ASA — modos/figuras de Chladni](https://acoustics.org/pressroom/httpdocs/148th/bissinger.html)
— seguem registrados no Atlas e na
[bibliografia local](BIBLIOGRAFIA_DE_ESTUDO.md). Nenhum código ou mídia externa
foi incorporado.

**Nota histórica:** a auditoria de 30/10 abaixo registrou corretamente o
comportamento observado naquela revisão. Desde então `PassageDevelopment` foi
incorporado: a forma já possui variação de peso/omissões e deslocamento do
baixo por passagem. A lacuna atual é recomposição de células/ataques e relação
voz-a-voz; portanto, a frase antiga “falta composição generativa evolutiva”
fica refinada por esta auditoria, não deve ser lida como se não houvesse
qualquer desenvolvimento formal hoje.

Cuidados registrados: baque virado/baque solto têm histórias e práticas
distintas; samba não se reduz a um compasso ou padrão único; os próprios
conceitos de síncope e contrametricidade são discutidos na literatura. Não
reutilizar toques de Candomblé nem repertórios indígenas sem estudo, contexto,
proveniência e autorização adequados. A disponibilização pública de cantos
tōlo pela FUNARTE não equivale a uma licença ampla de uso dos fonogramas. O
rótulo “rumba brasileira de terreiro” enviado na lista aguarda fonte e
esclarecimento antes de qualquer uso.

Fontes adicionais: [IPHAN — Maracatu Nação](https://bcr.iphan.gov.br/bens-culturais/maracatu-nacao/),
[Tambor de Crioula](https://bcr.iphan.gov.br/bens-culturais/tambor-de-crioula-do-maranhao/),
[Jongo no Sudeste](https://bcr.iphan.gov.br/acoes-de-salvaguarda/revalidacao-do-titulo-de-patrimonio-cultural-do-brasil-do-jongo-no-sudeste/)
e [Matrizes do Samba do Rio](https://bcr.iphan.gov.br/bens-culturais/matrizes-do-samba-no-rio-de-janeiro-partido-alto-samba-de-terreiro-e-samba-enredo/);
Carlos Sandroni sobre [conceitos rítmicos no samba](https://doi.org/10.5622/illinois/9780252044021.003.0001);
e [FUNARTE — cantos tōlo](https://funartemaisdigital.funarte.gov.br/cantos-tolo/),
cuja nota de acesso/proveniência consta no Atlas. Nenhum áudio ou padrão foi
copiado ou adicionado ao instrumento.

**Validação de código/UI anterior a esta nota:** build Release serial de
`batruc_app` passou após as mudanças no layout, knobs ADSR/VCF e integração de
Áreas/Stengel ao painel geométrico; preflight antes/depois não mostrou pressão
de swap nem crescimento relevante de memória. Binário
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
02/11/2026 18:49 CET, SHA-256
`576754586df63c122b1823aea63cf77ba726279478da755bf8df1d7955ec5e44`. A janela
não foi inspecionada: o ambiente não disponibilizou display para a abertura
controlada. Portanto, layout, rolagem, resposta dos knobs e sonoridade seguem
pendentes de validação na interface aberta pelo autor. Não rodei testes
automatizados nesta etapa.

**Validação de implementação:** build Release serial do aplicativo JUCE passou
após a inclusão do metrônomo e da persistência de cenas. O preflight registrou
10 GiB disponíveis, swap sem uso e 963 GiB livres. O build foi compilado, mas
não abri outra janela nem fiz audição ou inspeção visual manual; esses testes
seguem pendentes. Não rodei a suíte automatizada, conforme o escopo desta etapa.
Binário `.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
02/11/2026 18:11 CET, SHA-256
`5b7728088bf51702235297887cdcb7d2d98257c2f00477d5ee44c2968356a4bf`.

## Prioridade visual: Orbit/Study e editor circular — 02/11/2026

Na janela JUCE a partir de 1080 px, o gráfico temporal usa 40% da largura; a
coluna ao lado reserva 74% da largura útil para Orbit/Study e 26% para o campo
de padrão. Ocultar o campo faz Orbit/Study ocupar toda a coluna. A vista tem
360 px de altura. A opção Áreas substitui essa vista pelo estudo de Stengel no
mesmo lugar, com knobs `x`, `y` e `z`; assim o cálculo integra o primeiro campo
de Gráficos e Cálculos e não cria outra seção após o compasso. Os controles
ocupam duas linhas limitadas à coluna geométrica, sem invadir a forma, o gráfico
temporal ou a faixa de transporte.
O Study liga os pulsos por stem em polígonos, além de mostrar a grade temporal e
os alinhamentos. Arrastar um anel altera `Δφ ataque` dessa voz e o polígono
acompanha o mesmo deslocamento; não é ainda edição independente de cada onset.

Direção autoral para o próximo editor: um círculo dividido representa o ciclo
comum; até oito stems formam polígonos distintos. A quantidade de vértices
corresponde à contagem de ataques e cada vértice poderá ser movido para alterar
um onset e a forma da célula. O gráfico deverá desenhar a mesma estrutura que o
motor agenda, com edição por arraste e alternativa por clique/teclado ou valor,
sem sobrepor alvos. A alteração deve atualizar o som pela fronteira segura do
transporte, persistir na cena e permanecer sob limites fixos de eventos. Isso
exige estender o modelo de eventos e sua publicação ao áudio; desenhar vértices
móveis sem essa ligação não atende ao requisito. O atual gesto de fase no Study
é uma etapa intermediária, não substitui esse editor.

O gráfico de ondas estava ocupando largura excessiva e reduzia a leitura do
percurso. A reorganização diminui o gráfico temporal para 40% da janela e ativa
a composição lado a lado desde 1080 px. A coluna de geometria ganha 74% do
próprio espaço quando o campo de padrão está visível. Na largura mínima, a
coluna mede 564 px; a primeira linha de controles usa 536 px e a segunda termina
nos limites da coluna. Fourier, bilhar e Lissajous compartilham a segunda linha
por modo, sem ocupar simultaneamente a área uns dos outros. A opção Áreas agora
mostra Stengel nessa mesma coluna, com knobs `x`, `y` e `z`, e remove a seção
separada abaixo do compasso. Os knobs do Baixo organizam ADSR-A, ADSR-F e VCF em
fileiras próprias; nível/Gate continuam sliders. O cabeçalho e o rodapé fixos
voltam à frente quando a rolagem atualiza sua posição, para o Orbit não os
cobrir. O editor independente de vértices por voz continua pendente.

**Validação desta etapa:** build Release serial de `batruc_app` passou. O
preflight registrou 10 GiB disponíveis, swap sem uso e 964 GiB livres no volume;
após o build havia 10 GiB disponíveis e nenhum processo pesado ativo. `git diff
--no-index --check` não encontrou whitespace problemático nos três arquivos.
Não rodei testes automatizados, não abri outra instância e não confirmei a
interação na janela. Fontes alteradas: `src/app/Main.cpp`, `README.md` e
`TAREFAS.md`. Binário `.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
02/11/2026 18:01 CET, SHA-256
`ab8a3429b5d2d8edd58000fd3c76169969443cdc1bb67da586489454e9b158c5`.

### Gráfico de ondas mais estreito e percurso em destaque — 02/11/2026

Reduzi a largura do gráfico temporal de 52% para 40% e passei a exibir o
percurso ao lado dele desde 1080 px. O Orbit/Study recebe 74% da coluna
geométrica quando o campo auxiliar está ligado. Reorganizei os controles do
percurso em duas linhas: na largura mínima de 1080 px, a coluna mede 564 px; a
primeira linha usa 530 px e a segunda termina exatamente dentro dos limites da
coluna. Fourier, bilhar e Lissajous compartilham a segunda linha por modo, sem
ocupar simultaneamente a área uns dos outros. Também substituí a ordenação
parcial do TAP por ordenação de inserção em array fixo, removendo o aviso de
limite de intervalo visto na compilação anterior.

**Validação:** build Release serial de `batruc_app` passou sem avisos. O
preflight teve 9,9 GiB disponíveis, swap zerada e 963 GiB livres; após o build
os valores permaneceram estáveis. `git diff --no-index --check` não apontou
whitespace no fonte C++ (código de saída 1 apenas porque o arquivo é comparado
com `/dev/null`). Não abri outra janela, não fiz inspeção visual manual e não
rodei a suíte automatizada; portanto, a validação confirma compilação e limites
calculados dos componentes, não o aspecto final no monitor. Arquivos: `src/app/Main.cpp`,
`README.md` e `TAREFAS.md`. Binário Release: `.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
02/11/2026 18:27 CET, SHA-256
`afb2228630d6791edcdc6aa0c9b128780922e874c32ee33a95f538561f308ab3`.

## Organização em três campos e composição sem seed — 02/11/2026

A interface JUCE passa a apresentar o percurso do instrumento em três campos:
**Gráficos e cálculos** (curvas, fórmulas, geometria e percursos), **Mixer**
(até oito stems, com o mostrador de fase no próprio canal) e **Baixo**. A seção
Stengel continua dentro do primeiro campo conceitual, abaixo dos gráficos
principais. As oito vozes ativas são o limite da interface; o armazenamento
interno maior continua apenas para ler cenas antigas.

Enter com o campo de seed vazio ativa o modo manual. Os sliders rítmicos por
stem passam a exibir **toques por seção**, de 1 a 32, e convertem a contagem em
ciclos no mesmo agendador usado pelas receitas; fase e regras de evento
continuam participando do cálculo. As máscaras e regras podem descartar alguns
ataques, portanto o número selecionado é a contagem-base antes desses filtros,
não uma promessa de que todos os ataques serão ouvidos. Acrescentei no Mixer um
atalho visível **Modo manual**; pressioná-lo remove a seed, e pressioná-lo de
novo gera uma seed aleatória e retorna à composição seeded. Presets
polirrítmicos e troca de idioma preservam a unidade correta dos controles. O
cabeçalho da fórmula agora exibe os ciclos efetivamente ativos.

Validação desta etapa: compilação Release serial de `batruc_app`, `bash -n ./run`
e `git diff --check` passaram. A primeira compilação revelou um acesso incorreto
aos campos do par retornado pelo cálculo da fase; corrigi-o antes da compilação
final. Não executei testes automatizados nem abri outra janela. A inspeção
visual e a confirmação auditiva do modo manual continuam pendentes. Binário:
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
02/11/2026 02:15 CET, SHA-256
`9a3b504926dd9722c91cb667dee3c1c9101c4a81d5861a7af0b1dbb327584053`.

## Inventário ativo de pendências — 11/11/2026

Este mapa resume o que falta no instrumento inteiro. “Codado” não significa
integrado musicalmente, responsivo na janela ou validado por escuta. A matriz de
[paridade](PARIDADE_HTML_JUCE.md) conserva os critérios cruzados HTML/JUCE;
este inventário inclui também frentes próprias do Batruc.

1. **Composição generativa e memória:** já existem seeds, calendários de vozes,
   transformações, polirritmias, sete ciclos e forma longa determinística.
   Os presets incluem `3:4:5:7` no JUCE e no protótipo. O painel JUCE agora
   tem Orbit multivoz (anéis/fases e rastro vetorial resultante) e Study
   (pulsos sobre grade comum com LCM limitado), usando vozes, fases, overrides
   e transporte ativos. Falta verificação funcional/visual na janela e testar
   razão fracionária/alta em execução; o protótipo web ainda não tem esses
   modos. Riff ainda precisa do editor geral de células e da leitura visual de
   frase contra compasso.
   Falta ampliar a diferença musical entre seeds, criar mais caminhos e
   recombinações de células/frases/trechos, um editor ou banco de arranjos A/B/C,
   aprofundar inversões (ritmo, altura, tempo e algoritmo) e validar pausas,
   retornos e deriva em peças longas.
2. **Matemática que compõe:** já existem leis de intensidade, curvas de voz,
   relações de fase, Lissajous, Fourier/epiciclos e cálculos geométricos.
   Homeomorfismos foram registrados como princípio futuro para transformar
   contornos preservando conectividade, sem presumir que métricas ou som
   permaneçam invariantes.
   Falta transformar mais integrais/derivadas, teoremas (incluindo triângulos),
   teoria de anéis, séries, álgebra, matrizes, probabilidade,
   correlação/regressão, fractais e curvas rolantes em operações audíveis de
   ritmo, altura, dinâmica ou timbre. Abordagens quânticas ficam como pesquisa
   exploratória até terem modelo e interpretação verificáveis; muitos gráficos
   ainda são estudos visuais e não fontes composicionais.
3. **Física e espaço sonoro:** há pêndulo triplo, molas efetivas, energia e
   campos móveis como estudos/regras composicionais. A interseção, o arrasto e
   os cortes de dois polígonos convexos de 3–8 vértices já existem no JUCE
   para os estudos de triângulo/quadrado; a edição agora também é preservada
   em cenas nativas. Falta um equivalente web, polígonos não convexos/volumes,
   outros cortes, operações próprias de gravidade/força/deformação/ruptura e
   experimentos
   de acústica espacial; não alegar modelo físico validado sem medição.
   Reverberação e delay ainda não são efeitos dedicados; amortecimento por
   densidade existe, mas não simula materiais e pede projeto sonoro e escuta.
4. **Vozes, baixo e timbres:** já há 18 matérias virtuais (seis
   harmônicas/modais, onze percussivas e Chip 8-bit), baixo calculado,
   LFO, VCA/VCF, ADSR, controles de canal e famílias percussivas iniciais.
   Faltam repertório tímbrico mais contrastante — famílias de percussão/objetos
   e graves acústicos, fretless, sintetizados, sopros graves e voz —, interação
   audível com as vozes, revisão de ganho/transientes/cliques, receitas sonoras,
   equilíbrio entre baixo e percussão e comparação de síntese JUCE/web. A
   diferença de arquitetura modal JUCE versus osciladores Web Audio ainda é audível.
5. **Micropatterns e percurso:** movimento seeded, 28 perfis de borda
   selecionados independentemente por ilha, deformações harmônicas,
   afinidade gradual, quatro ilhas móveis com pulsação de vizinhança, conexões
   transitórias entre ilhas próximas e duas faixas assimétricas estão nos dois
   motores. Falta conferir a animação em execução,
   escutar a distribuição de ataques e aprofundar outros organismos e
   transformações sem tornar o campo uma animação sem consequência sonora.
6. **Gráficos e interface:** há gráficos temporais, XY, Fourier, percursos e
   estudos pendulares. Faltam inspeção das janelas JUCE/web, revisão de
   sobreposições/escala/expansão, resposta dos cliques, leitura de fórmulas,
   atualização de todos os gráficos ao mudar seed/parâmetros e prova de que cada
   visual comunica o cálculo que realmente controla o som.
7. **Mixer e performance:** mute/solo, níveis, pan, variação e medição RMS têm
   implementação parcial ou testes de core. Falta validar pela interface os
   canais isolados, o baixo, balanço, master, persistência das cenas, automações
   e resposta instantânea; medir CPU/RAM em repouso e tocando, com máscaras e
   muitas vozes, em janelas consecutivas. O recálculo da seed ainda corre na
   thread da interface e precisa ser isolado ou dividido em etapas; uma busca
   quadrática foi removida e a avaliação geométrica do baixo foi limitada à
   grade que ele pode tocar, mas isso ainda não comprova que a lentidão esteja
   resolvida.
8. **Arquivo local do músico:** há gravação e formatos de cena/TXT nos dois
   destinos, mas faltam fluxos de sistema completos e compatíveis: escolher e
   reabrir diretório real, localizar/gravar/apagar seeds e composições,
   recuperar projetos, relacionar WAV com partitura/fórmulas/seed e abrir os
   formatos cruzados sem perda.
9. **Paridade e validação:** P1 seed/fase/receita, P2 ritmo/forma/baixo, P3
   mixer, P4 arquivos/conteúdo e P5 geometria/Fourier continuam abertos na
   matriz. Fixtures e builds cobrem cálculos pontuais; faltam interação real nos
   dois apps, comparação A/B de stems e mixes, escuta musical do autor e aceitação
   visual. O número histórico de 69% não foi recalculado.
10. **Didática e saída pública:** Learn, Sobre, tutoriais, créditos e idiomas
    existem parcialmente. Faltam revisão dos quatro idiomas, exemplos que
    expliquem a ligação fórmula→evento→som, pacote multiplataforma e os critérios
    de release. Batruc continua em incubação e não está pronto para publicar.

### Auditoria estática de CPU — 01/11/2026

Escopo: inspeção dos loops de áudio, recálculo de seed, preparação dos planos e
animação JUCE. Isto identifica trabalho por amostra e caminhos potencialmente
quentes; não é um perfil detalhado do executável. Durante o teste do autor,
`ps` reportou o processo Batruc em 116% de CPU após 1 min 42 s de execução
(média acumulada do processo, não uma amostra instantânea). A namespace não
permitiu inspecionar suas threads em `/proc`, portanto ainda não localizei qual
thread domina nem se o maior custo é DSP ou interface.

1. **Custo recorrente provável — guarda true-peak.** Em cada frame estéreo,
   `StereoLookaheadGuard::process` executa quatro estimadores 4× (entrada e
   saída, L/R). Cada estimador percorre 4 fases × 16 taps: aproximadamente 256
   multiply-accumulate por frame, ou 12,3 milhões/s a 48 kHz. A conversão
   `log10` da redução agora é calculada ao consultar o medidor: o guard acumula
   o ganho mínimo, que representa a mesma redução máxima. É a proteção de saída, não um
   custo visual. Perfilar primeiro; otimização deve preservar o teto e a
   estimativa de pico, ou documentar claramente qualquer alteração de garantia.
2. **Custo recorrente provável — vozes modais.** `processWithSwitch` renderiza
   até 12 vozes e `renderVoice` percorre oito modos por voz por amostra: até 96
   iterações/frame (4,6 milhões/s a 48 kHz). Cada modo atualiza fase/amplitude,
   escolhe razões e forma espectral e pode fazer consulta interpolada de seno.
   Há trabalho evitável: perfis como seno e ondas de harmônicos ímpares deixam
   vários pesos espectrais zerados. O código pula a consulta de seno para peso
   exatamente zero, preservando atualização de fase e decaimento; razões,
   pesos espectrais e cor do ruído agora são preparados quando o timbre muda,
   em vez de selecionar fórmulas/switches em cada modo e amostra. Envelopes quase
   extintos ainda continuam atualizados. A quantidade real depende do número de
   vozes ativas, timbre, caudas e taxa de amostragem.
3. **Baixo — custo dependente do material.** A síntese soma três parciais por
   amostra. No material `pluck`, o coeficiente de queda é agora calculado uma
   vez por nota e o envelope avança por multiplicação recorrente, em vez de
   recalcular `pow(0.12, t)` a cada amostra. O filtro recalcula seno/cosseno a
   cada 16 amostras; o LFO também é atualizado em blocos de 16 amostras.
4. **Pico na interface — trocar seed/variação.** `requestBassPerformanceRebuild`
   chama a geração na thread da interface. São montados cinco planos formais,
   percorridos os eventos, avaliadas fórmulas/intensidades e construída/sorteada
   a agenda derivada. As avaliações por evento de posição e intensidade são
   agora compartilhadas entre a seleção de influência e a agenda completa, sem
   remover subdivisões. A avaliação de curvatura já foi reduzida à grade do baixo;
   após revisão para preservar riqueza, todos os eventos admitidos voltaram a
   `scheduleSources`. O trabalho restante cresce com eventos/vozes e ainda pode
   bloquear cliques. Não foi isolado em worker nem medido por seed nesta auditoria.
5. **Pico de inicialização/preparação, não custo contínuo.** O JUCE cria
   `kRecipeVariantCount = 9.696` combinações de ciclo/fase/regra/campo no
   construtor e monta cada `EventPlan`. `prepareToPlay` valida e copia os eventos
   para outras 9.696 agendas, ordenando-as. O teto global é 1.048.576 eventos
   preparados. Isso pode aumentar tempo de abertura e memória; não explica por
   si só CPU alta durante reprodução depois da preparação. Planos e agendas
   duplicados também consomem memória.
6. **Interface — custo secundário provável.** O timer roda a 24 Hz, atualiza
   estados e solicita redesenho da faixa superior durante o transporte. A vista
   de geometria reconstrói 512 pontos por pintura; Orbit soma até 12 ciclos em
   385 amostras. São cargas delimitadas e muito menores que o trabalho de áudio
   acima, mas podem piorar a responsividade se o sistema já estiver no limite ou
   se várias vistas forem redesenhadas.

**Ordem para confirmar e otimizar:** (a) medir callback de áudio e thread JUCE
com 2/6/12 vozes, baixo ligado/desligado e guarda ativa; (b) comparar custo do
true-peak e da síntese modal sem desativar proteções por padrão; (c) retirar
modos de peso zero e encerrar trabalho de modos inaudíveis com limiar/fade
controlado; (d) trocar `pow` por recorrência no pluck e medir a troca de seed,
depois movê-la para worker versionado se continuar bloqueando; (e) medir custo de
preparação inicial e avaliar cache/geração sob demanda. Registrar CPU média e
pico, duração de callback, xruns/dropouts e latência, sempre com configuração,
taxa de amostragem e tamanho de bloco. Não iniciar render ou profiling pesado
até poder observar o host e confirmar margem de recursos.

**Ambiente da auditoria inicial:** 8,6 GiB disponíveis, swap 0/2 GiB e 964 GiB
livres no volume do workspace. O volume `<disco-local>/2tb_interno` aparece como
somente leitura nesta namespace; não foi necessário escrever nele. **Avanço
seguinte:** removida a conversão `log10` por amostra do guard, mantendo a mesma
medida de redução máxima via ganho mínimo. Também se evita a consulta à tabela de
seno para pesos espectrais exatamente nulos, mantendo fase e decaimento desses
modos. Neste avanço, razões, pesos espectrais e cor de ruído também passaram a
ser preparados em mudança de timbre/configuração e lidos diretamente no render
modal. A compilação serial Debug do alvo `batruc_app` passou sem avisos após
estas mudanças. Último preflight: 6,9 GiB disponíveis, swap 0/2 GiB e 964 GiB
livres. Nenhum teste automatizado, render ou gravação foi executado. A
compilação comprova integração, não igualdade auditiva nem redução mensurada de
CPU. Próximo passo: capturar perfil do Batruc no host em execução normal e numa
troca de seed, sem abrir uma segunda instância.

Ordem de trabalho imediata: concluir e observar a afinidade gradual das
superfícies; depois validar responsividade/CPU e áudio por seed e modo; então
fechar as diferenças de som, mixer e arquivo da matriz. As frentes de matemática
e física novas entram como experimentos composicionais com critério sonoro, não
como catálogo de gráficos.

### Lentidão ao trocar seed — 01/11/2026

O autor relatou travamento/lentidão ao selecionar outra seed. A inspeção do
processo chegou a registrar 100% de um núcleo; em captura posterior o transporte
e os gráficos ainda avançavam. O processo/janela de diagnóstico desapareceu
durante a inspeção, sem core dump ou registro de OOM recente. A tentativa de
capturar pilhas foi bloqueada por `ptrace`; a tentativa de clique automatizado
não confirmou a troca e não deve ser tratada como teste funcional.

No caminho de código, `applySeedInput()` chama `requestBassPerformanceRebuild()`
na thread da interface; essa rotina recalcula a agenda do baixo. Encontrei ainda
uma busca de próximo evento quadrática ao limitar a duração de cada nota. Troquei
essa busca por ordenação única de índices por tempo absoluto e consulta do vizinho
temporal, reduzindo essa etapa de O(n²) para O(n log n), preservando desempates e
a tolerância temporal existentes. O limite do calendário é 640 eventos tocados,
portanto esta remoção é uma otimização localizada, não prova da causa principal.

Na geração do baixo, encontrei outro excesso: a receita podia conter até 65.536
eventos de voz por seção, mas o baixo só tem até 80 posições por seção. A
avaliação de velocidade e curvatura do percurso agora é calculada uma vez por
voz e posição rítmica (máximo de 960 amostras por ciclo formal), e a chamada de
`buildEventPlan` usada nessa redução dispensa a ordenação cronológica. A primeira
versão dessa otimização também descartava eventos simultâneos na mesma posição
ao montar a fonte do baixo. Na revisão de 01/11, isso foi corrigido: todos os
eventos admitidos pelo plano de voz voltam a alimentar `scheduleSources`, com
seus tempos, intensidades e coordenadas de fórmula; só os descritores de
velocidade/curvatura usam a amostra representativa da posição. Assim, subdivisões
e cruzamentos continuam disponíveis às relações answer/ecológicas e à análise
da trajetória. Limite composicional restante: `buildSeedBassPerformance` mapeia
as vozes para uma grade de até 20 posições por compasso (semicolcheias em 4/4)
e escolhe um evento prioritário por posição. Em colisões, portanto, a grade de
saída ainda funde ataques; relações ecológicas veem a lista completa antes dessa
redução. Aumentar resolução ou permitir eventos deslocados é uma frente própria,
que exige limite explícito de densidade e controle contra cliques/clipping. O
custo de fórmulas/intensidades continua proporcional ao total de eventos e
precisa ser perfilado antes de declarar a responsividade resolvida.

Arquivos: `src/app/Main.cpp`, `src/core/CompositionCore.hpp`,
`src/core/CompositionCore.cpp` e este registro. Preflight antes da primeira
compilação: 8,5 GiB disponíveis, swap vazia (0/2 GiB) e 964 GiB livres. O alvo
JUCE Debug `batruc_app` passou em duas compilações seriais; a segunda passou
sem avisos e atualizou o executável local. Preflight antes dela: 7,7 GiB
disponíveis e swap vazia. Não reabri o app nem medi CPU/latência: o autor relatou
que a máquina estava quase travando. O recálculo continua na thread da interface
e a construção da lista ainda cresce com a densidade da receita; não afirmar que
o congelamento foi resolvido. Próximo passo: observar uma troca de seed com
transporte parado e 2/12 vozes, depois durante reprodução; se o bloqueio
persistir, separar o cálculo da thread da interface com snapshot imutável e
resultados versionados. Na compilação após preservar todos os eventos de entrada
do baixo, o alvo `batruc_app` passou serialmente; a primeira execução revelou
uma variável local sem uso, removida antes da compilação final. A compilação
final terminou sem avisos. Preflight: 8,4 GiB disponíveis, swap 0/2 GiB e 964
GiB livres. Esta checagem confirma construção do código, não a resposta do app
nem a equivalência sonora; não rodei testes automatizados ou reprodução de áudio.

### Formas matemáticas, desvio e movimento — 01/11/2026

Ampliei a biblioteca de contorno das quatro ilhas para 28 perfis, escolhidos
deterministicamente pela seed e de modo independente por ilha: harmônicos,
curva W, domo gaussiano, Agnesi, logaritmos (incluindo base 10), recíproca,
softplus, GELU, seno/cosseno/arcos seno/arcos cosseno, seno absoluto, parábola,
batimentos, tangente limitada, chirp circular por modulação de fase, cubo,
simetria/reflexo, concavidades, anamorfose angular, rebote, cosseno amortecido,
oval convexo e perfil afilado apelidado “foguete”. Fórmulas com polos foram
limitadas ou reparametrizadas para preservar curva fechada e raio positivo.
A função comum de fronteira faz essas formas alterarem a máscara de eventos,
e o desenho JUCE/web já consulta essa fronteira; não são apenas ilustrações.

“Efeito borboleta” ainda não foi implementado como dinâmica caótica: os desvios
atuais são limitados, periódicos e reproduzíveis. Próxima etapa é comparar
visualmente/auditivamente seeds com perfis distintos, verificar CPU e então
estudar bifurcação/sensibilidade inicial com limites e consequência musical
explícita. Movimentos ópticos, reflexos físicos e truques de cinema/anamorfose
também precisam de regras e controles próprios; os nomes atuais são inspirações
de contorno, não simulações desses fenômenos.

A próxima pesquisa inclui geometria rítmica do ciclo (distâncias circulares,
simetria, transformação, sobreposição e relação entre percurso animado e
eventos audíveis). No Study Mode — Rhythmic Geometry, prototipar visualização
de faixas reais: decompor por instrumento/camada, marcar ciclos, compassos,
pulsos, subdivisões, acentos e mudanças de arranjo; converter a leitura em
gráfico didático sincronizado. Incluir clave, rumba clave, bossa nova e tresillo
como comparações contextuais, com variantes identificadas; mostrar
ataques/acentos/pausas, subdivisões concêntricas, playhead, rotação/espelho,
polígonos inscritos no círculo (vértices = divisões e lados = percurso),
polígonos sobrepostos `N:M` para coincidências e retorno, isolamento de camadas
e polirritmias audíveis. Preservar variantes e contexto;
bossa não é uma única célula, e clave não é sinônimo de todos esses padrões.
Examinar como referência funcional o Rhythmic Geometry (Orbit/Study/Riff),
sem copiar implementação, textos, cenas ou identidade; distinguir a proposta
do Batruc pela integração explícita com cálculos matemáticos/físicos e o
ecossistema RASGO.
Tratar separadamente o Orbit generativo mostrado na captura do autor: quatro
ciclos `3:4:5:7`, órbitas com fase/velocidade próprias e rastros persistentes.
Definir a convenção das razões, ligar desenho e áudio ao mesmo relógio, e
permitir comparar/limpar/atenuar rastros e isolar camadas.
Adicionar breakdown de faixa com fonte, trecho, autoria da análise, camadas,
confiança e leituras alternativas. Caso inicial 3:7 de “Invincible” (descrição
fornecida pelo autor): septágono/guitarra, triângulo/toms, acentos 1-4-6 →
1-3-5, transferência toms→caixa e superciclo de 21 passos; sincronizar
playhead/áudio e cotejar a interpretação com a gravação. Adicionar “Pitch
Black”, Meshuggah, a partir da captura de post fornecida pelo autor: tercinas,
ciclo de dez notas, polígono e retorno alegado ao primeiro tempo após quatro
compassos. Mostrar compasso, unidade de subdivisão e conta de realinhamento;
em 4/4 ternário, 48 subdivisões em quatro compassos não fecham por si só um
ciclo de dez.
Caso “ObZen” a partir da captura do autor: três células encadeadas em 68
semicolcheias (`17/4`) contra bateria em `4/4`; desenhar células e métrica em
camadas distintas. Não inventar duração das células ausente na fonte. Estudar
um Cell Sequencer multivoz: o piano roll de baixo atual é especializado e não
substitui edição/encadeamento de células de todas as vozes.
Além disso, avançar classificação topológica, diferenciável,
métrica, espectral, simplética e fibrada; homologia/cohomologia e Betti;
análise estrutural; variedades; cálculo diferencial; curvas implícitas e
conexões verificadas entre Stokes, Poincaré, conexão/curvatura de Berry e
número de Chern. Para Berry/Chern, definir um modelo de banda isolada sobre
parâmetros periódicos, conferir o gap e validar o invariante integral com o
método discreto de Fukui–Hatsugai–Suzuki antes de qualquer mapeamento musical.
Física estatística (Maxwell–Boltzmann), redes geométricas/neuronais e PINNs
entram como hipóteses de composição e aproximação, com seleção por seed, custo
limitado, referência de comparação e sem antropomorfismo. São direções
documentadas, ainda sem efeito sonoro dedicado.

Arquivos tocados nesta etapa: `src/core/CompositionCore.hpp`,
`src/core/CompositionCore.cpp`, `prototype/score.js`, `README.md`,
`OPERACOES_GENERATIVAS.md` e este registro. Preflight: 7,3 GiB disponíveis,
360 MiB de swap em 2 GiB e 963 GiB livres no volume de trabalho. Build Debug
serial do alvo `batruc_app`, `node --check prototype/score.js` e `git diff --check`
passaram. Não fiz teste auditivo nem inspeção visual; instâncias já abertas
continuam com o binário anterior até reiniciar.

### Micropatterns como organismos acoplados — 01/11/2026

Ampliei as ilhas adesivas de uma deformação comum para quatro organismos
seeded com deriva de centro (até 0,036 unidade por eixo), pulsação própria,
uma contribuição fraca vinda do organismo vizinho, fase de contorno
independente. O raio permanece aproximadamente em 0,34–0,42; as quatro ilhas
têm expansão/contração fora de fase, de modo que diferentes partes do percurso podem encontrar cada ilha em
momentos diferentes. Pares vizinhos formam conectores transitórios quando os
contornos se aproximam; o corredor passa a aceitar ataques e desaparece quando
as ilhas se afastam. A função de distância e o desenho usam o mesmo segmento e
raio, preservando o vínculo campo→evento. Nas microlinhas, as duas membranas têm deriva e
largura próprias, sem alterar as extremidades fixas da deformação harmônica.
As equações de pose, contorno, seleção e renderização foram mantidas alinhadas
entre `CompositionCore.cpp`, `score.js`, o canvas web e o desenho JUCE. O
acoplamento é determinístico e sem memória de evento, com custo limitado a
senos e vetores fixos; trata-se de uma metáfora composicional de organismos,
não de simulação celular.

Arquivos: `src/core/CompositionCore.hpp`, `src/core/CompositionCore.cpp`,
`src/app/Main.cpp`, `prototype/score.js`, `prototype/app.js`,
`prototype/index.html`, `prototype/interface-i18n.js`, `README.md`,
`OPERACOES_GENERATIVAS.md` e este
registro. O build Debug serial de `batruc_app` passou; `node --check` passou
nos três arquivos JS alterados. A janela que já estava aberta continua usando
o binário anterior até reiniciar. Não fiz validação visual/auditiva nem teste
de equivalência de execução; não reivindico paridade fechada.

### Modelo poligonal editável preservado em cena — 01/11/2026

O formato de cena JUCE avançou para schema 37. Agora registra os vértices
normalizados e as translações manuais dos dois polígonos convexos do estudo de
triângulo/quadrado. Ao abrir uma cena, o modelo é restaurado depois de aplicar
seed e geometria; os dados só são aceitos dentro dos mesmos limites do editor
(3–8 vértices, coordenadas `[-1.4, 1.4]`, translação `[-0.48, 0.48]`) e a
restauração confirma convexidade antes de substituir a forma atual. Cenas 1–36
continuam abrindo e geram a geometria padrão a partir da seed. Duplo clique
continua reconstruindo a forma seeded; salvar depois do reset registra a forma
reconstruída.

Arquivos: `src/app/Main.cpp`, `README.md`, `PARIDADE_HTML_JUCE.md` e este
registro. Build `batruc_app` Debug serial passou; ainda falta salvar uma forma,
fechar/reabrir a aplicação e conferir no uso que vértices/translação voltam.
Não houve execução visual nem escuta. O recurso permanece JUCE-only e não
altera a avaliação de paridade.

**Campos XY móveis e aproximação da Geometria — 01/11/2026:** o campo deixa
de ser estático. Em cada forma, seed e índice da seção alimentam uma fase
determinística; as ilhas se deslocam/rotacionam e respiram, e as microlinhas
deslizam, giram e variam de largura. O motor recalcula a máscara no instante
de cada ataque. Em campo livre, uma Lupa virtual percorre o plano e pesa todos
os eventos por distância (0,68–1,20), sem os suprimir. Desenho e engine
compartilham as funções de pose/aceitação/peso. A referência de agência visual
foi a Lupa do Aquorbium, documentada em
`../AQUORBIUM/docs/DESENVOLVIMENTO.md`; adaptei o foco espacial ao cálculo do
Batruc sem copiar código ou dependência. O exemplo e o limite da ligação
gráfico→som estão registrados em `OPERACOES_GENERATIVAS.md`.

Após relato de lentidão associada às máscaras, encontrei que Campo livre também
forçava a agenda dinâmica de todos os ataques. A Lupa aberta não precisa mover
nem filtrar os tempos: o scheduler preparado permanece e o foco modula o peso
do evento quando ele dispara. Ilhas adesivas e microlinhas continuam como
micropatterns que admitem/suprimem ataques segundo a posição do percurso, mas o
áudio agora espera pelo próximo ataque dinâmico em vez de consultar todas as
vozes a cada amostra. A deriva passou a percorrer 0,25–0,625 ciclos por seção
formal, mantendo translação, rotação e variação suave do tamanho. A forma das
ilhas adesivas agora recebe deformação radial de baixa ordem; as microlinhas
ganham uma curvatura simétrica. A mesma equação que desenha os contornos decide
se o evento está dentro do micropattern. O alvo JUCE Debug compilou em série
(`-j1`); o binário resultante tem SHA-256
`58b75b9ace627f3eb47edeeb097a88b5517b1ca78843ea72cc1bd5816f08b75a`.
Preflight: 7,2 GiB disponíveis e 130 MiB de swap usada. A investigação do
arranque encontrou strings UTF-8 gregas passadas diretamente aos construtores
JUCE (`φy` no rótulo e tooltip; `×` na leitura do VCA); foram convertidas pela
função UTF-8 do app. A execução sob GDB passou 20 s sem asserção e foi encerrada
pelo limite de tempo. Uma execução posterior apareceu com média acumulada de
11,2% de CPU e 100 MiB de RSS aos 2:25, mas o processo terminou antes de duas
amostras consecutivas; isso não mede carga sustentada nem permite comparar os
três modos. O transporte começa parado (`playing_ = false`), embora o dispositivo
de áudio permaneça aberto. A carga anterior de 55,4% foi medida em execução
diferente, sem controle e antes das correções, portanto não serve como referência
de desempenho.

Pendente para fechar a validação: medir janelas consecutivas com transporte
parado e tocando, mesma seed e número de vozes, alternando Campo livre, ilhas e
microlinhas; conferir no áudio e no desenho se cada máscara seleciona os ataques
previstos. A inspeção automatizada do estado de GUI não ficou disponível nesta
sessão; não declarar os modos auditivamente validados até fazer essa comparação.

**Contornos micropatternados com harmônicos — 01/11/2026:** as ilhas combinam
deformação radial de segunda e terceira ordem, com coeficientes lentos ligados
à seed e ao percurso. As microlinhas combinam duas ondulações com envelope
`1−x²`, que mantém as extremidades fixas. O core expõe as funções de contorno e
inclinação; a distância assinada à borda e o desenho JUCE usam essas mesmas
funções, eliminando a duplicação de equações entre áudio e visualização. O
build JUCE Debug passou em série (`--parallel 1`) às 18:42 CET. SHA-256:
`16b65a02f0e6157f864f7df3a055fb785ff507cdaa9b4dadd2f3cf3aba00980e`.
Preflight: 7,9 GiB disponíveis, 363 MiB de swap usada e 964 GiB livres; após o
build: 7,6 GiB disponíveis, swap estável. Não rodei testes nem fiz validação
visual ou auditiva. Próximo passo: observar os contornos com diferentes seeds e
confirmar, em execução, que ataques junto à borda seguem a forma desenhada.

**Portabilidade dos campos micropatternados para a web — 01/11/2026:** o
`score.js` agora calcula a mesma pose de máscara por seed e progresso, usa o
campo móvel na seleção/peso dos ataques e a animação do `geometry-trace` desenha
ilha, microlinha ou foco ativo sobre a trajetória. As fórmulas foram portadas
com os mesmos coeficientes do core JUCE. A equivalência foi conferida no código;
validação de execução, escuta e correspondência visual entre as duas interfaces
ainda está pendente. `node --check` passou em `score.js`, `app.js` e
`interface-i18n.js`; esses checks confirmam sintaxe, não comportamento em
navegador.

**Afinidade gradual na borda dos micropadrões — 01/11/2026:** máscaras agora
usam distância de contorno aproximada (radial nas ilhas, perpendicular nas
faixas) e smoothstep numa banda de `η=0,055`; a intensidade
transita de 1 para 0, enquanto a trajetória cruza a fronteira. O JUCE calcula
o peso uma vez por evento e reutiliza o resultado na decisão de disparo e no
ganho; a web usa a mesma banda e a mesma curva. O desenho adiciona um halo de
baixa opacidade para indicar a região de influência. Campo livre mantém sua
Lupa gaussiana. A etapa remove a escolha abrupta de ligar/desligar na margem,
mas precisa de audição para confirmar que os ataques atenuados continuam
perceptíveis e não desequilibram a mixagem. Build JUCE Debug passou em série
(`--parallel 1`) às 18:51 CET; SHA-256
`b7cf5532430299ae42ee3b608c6b9f02a55e2c15e43c5792b3955c942ca35c07`.
`node --check` passou em `score.js`, `app.js` e `interface-i18n.js`. Preflight:
7,5 GiB disponíveis, swap 363 MiB/2 GiB e 963 GiB livres; após build, 7,8 GiB
disponíveis. Não rodei testes nem fiz escuta ou inspeção visual em execução.
Próximo: validar visual e auditivamente os limites e a transição do campo nos
dois destinos.

Também reorganizei o fluxo visual: Geometria/Percursos vem primeiro depois da
área inicial, antes do Mixer e do Baixo; a página caiu de 3500 para 3200 px.
Os controles geométricos e o percurso foram agrupados lado a lado, e os
controles Fourier/Lissajous e os botões ficam em linhas separadas para evitar
colisão. A reorganização do layout é nativa JUCE; a web conserva seu próprio
layout. As máscaras agora têm movimento e contornos equivalentes nos motores
JUCE e web, embora a estrutura das interfaces e o fluxo de agenda não sejam
idênticos. Ainda falta escuta comparativa entre campo livre, microlinhas e ilhas
em vários ciclos.

**Profundidade de variação calculada por stem — 01/11/2026:** acrescentei ao
Mixer `VAR` (0–100%) em cada canal. O valor interpola do comportamento-base
para uma transformação determinística por evento: score limitado combina
posição X/Y do percurso e seno de fase/frequência derivados da seed e do
índice da voz. O resultado altera acentuação (±20%) e deslocamentos de altura
(até dois semitons); também dosa o pan móvel e o amortecimento de ataques
percussivos. Zero desliga os deltas de variação do canal; 100% usa o cálculo
integral. A implementação atualiza os valores por evento no renderer e não
introduz sorteio aleatório contínuo. Cenas passam a schema 36; cenas anteriores
abrem em 100% para manter a resposta que já tinham. Asterismus (HEAD
`ccaa13a424b52ee3176c7cd9c211f0166e18eb35`) serviu de referência para
profundidade local por ator e caching por mudança de estado; Rasgo Modular
(HEAD `4ce898f7024cb8adb67f97333e9f7f980cdbb8eb`) para variação seeded,
limitada e em fronteiras de evento/clock. Foram adaptados conceitos: nenhum
código foi copiado e nenhuma dependência foi introduzida.

**Validação realizada:** build JUCE Debug de `batruc_app` passou em série
(`--parallel 1`) em 01/11/2026. Binário
`.local-build/native/linux-debug-juce/batruc_app_artefacts/Debug/Batruc`, SHA-256
`848fd3865bf08caec42c7e2ecaf6049c30ee0a87d33695069d18c068a4fbc7fe`.
Preflight: 7,9 GiB disponíveis, swap 126,3 MiB/2 GiB e 964 GiB livres; sem
compiladores/builds concorrentes. Depois: 7,3 GiB disponíveis, swap 128 MiB e
963 GiB livres. Não executei testes nem validação auditiva/visual. Para avaliar
o novo Mixer e comparar 0%, intermediário e 100% em stems distintos, reinicie
o processo do app para carregar este binário; cenas antigas migram para 100%.

**Panorama percussivo condicionado por intenção e dinâmica — 01/11/2026:**
adicionei no Mixer um controle `MOV` de profundidade por canal. Para ataques
com bumbo, caixa, chimbal, ride, conga ou escovas, a posição X do evento move o
pan em torno do valor-base por ganhos de potência constante. A intensidade do
evento já inclui a curva dinâmica; construir amplia esse movimento conforme a
intensidade, rarefazer o reduz, encontrar aproxima os eventos do centro e
contrastar alterna o lado por subdivisão. Timbres não percussivos não são
afetados. Na pausa formal, em canal inativo ou com profundidade zero, o pan
retorna ao valor-base por uma rampa de 5 ms, evitando salto entre lados. O novo
campo `pan_motion` é persistido em cenas schema 35; cenas antigas migram com
profundidade zero. O mecanismo usa a trajetória como entrada composicional,
sem atribuir essa regra a artistas ou tradições específicas.

**Validação desta etapa:** o build JUCE Debug de `batruc_app` passou em série
(`--parallel 1`) após o ajuste de código e localização. Binário:
`.local-build/native/linux-debug-juce/batruc_app_artefacts/Debug/Batruc`,
modificado em 01/11/2026 às 13:28 CET, SHA-256
`a66d926121d253e51073348e894f3dd308cb87cb2f500e6e3c7b66d0f239e191`.
Preflight antes do build final: 8,0 GiB disponíveis, 126,5 MiB de swap em 2
GiB e 963 GiB livres; não havia compilador ou build concorrente. Após o build,
7,9 GiB permaneciam disponíveis e swap/disco ficaram estáveis. Não executei
testes nem abri o app. A inspeção do controle e a resposta mono/estéreo, bem
como a audição dos quatro modos de intenção, continuam pendentes. Próximo:
testar movimento em 0, parcial e máximo, verificando pausa e retorno ao
pan-base. Reverberação, delay e amortecimento relacionado a materiais/sala
continuam como frentes posteriores; o damping implementado é uma regra de
density dinâmica, não um modelo de material.

**Amortecimento percussivo por densidade, dinâmica e intenção — 01/11/2026:**
no próximo passe do renderer, a cauda percussiva passa a responder ao intervalo
entre ataques. A densidade `q=clamp((0,55−Δt)/(0,55−0,08),0,1)` define o
amortecimento adicional, reduzido por intensidade forte e modulado pela
intenção; construção sustenta mais, rarefação seca mais, encontro fica entre
ambos e contraste alterna duas respostas por subdivisão. O coeficiente
exponencial por amostra atua nos modos ressonantes e na cauda de ruído, sem
recalcular séries de exponenciais por modo a cada golpe. A pausa formal limpa a
memória e restaura o coeficiente neutro; o controle Cauda permanece como
decaimento-base. É um gesto procedural autoral, não uma simulação física
validada de material. O build JUCE Debug passou em série (`--parallel 1`).
Binário `.local-build/native/linux-debug-juce/batruc_app_artefacts/Debug/Batruc`,
modificado em 01/11/2026 às 13:36 CET, SHA-256
`44ec3d0b5cd8a97fa83f7feef2528e748f37849b7719f4738ad8d903dd539767`.
Preflight do build final: 7,9 GiB disponíveis, 126,5 MiB de swap em 2 GiB e
963 GiB livres; após o build, 7,5 GiB disponíveis e swap/disco estáveis. Não rodei testes nem
fiz escuta. Próximo: reiniciar a janela que estava aberta para usar o binário
novo e conferir golpes espaçados/densos, intenções e pausa/retorno.

**Forma longa A/B/A′/C — primeira camada no core, 01/11/2026:** a consulta ao
Atlas apontou três precedentes úteis: o Studio preserva identidade de célula
enquanto muda direção por frase/seção; o Antitotem distingue passo, padrão,
motivo, forma e histórico; e a pesquisa de Gramani pede associação/dissociação
entre vozes em vez de mera repetição de grade. A forma atual de sete ciclos
permanece como unidade interna. Para reprodução longa, o core agora percorre
oito passagens determinísticas sobre a mesma composição: A, B, A′, C, B′,
Expansão, C ou C′ e retorno A, sem passagem inteira de silêncio. A abertura
A/B/A′/C é comum; a seed escolhe uma entre quatro ordens para os papéis ativos
da segunda metade, sempre mantendo eventos e o retorno.
Os papéis modulam articulação/acentuação comum às vozes e ao baixo; não trocam a
seed nem reescrevem o calendário de notas. A posição atual aparece no status
como `P#/8 · A/B/A'/C/...`. A escolha de forma e a memória de passagem são
reproduzíveis para a mesma seed. Isso é geração automática, não um editor de
ordem: não há ainda escolha manual de trechos, duração por trecho nem banco de
arranjos nomeados. Um banco de cenas guarda snapshots, não uma forma longa.
O build JUCE Debug de `batruc_app` passou em modo serial (`-j1`) após a seleção
de forma por seed. Preflight: 8,1 GiB disponíveis, 126,8 MiB de swap em 2 GiB e
964 GiB livres. Não rodei testes automatizados, não abri a janela e não comparei
áudio. Portanto os rótulos ainda precisam de inspeção e o contraste dos papéis
precisa de escuta. Próximo passo: conferir rótulos e contraste sem apagar o
motivo; depois ampliar as operações para alturas, densidade e transformações
temporais ou oferecer a lista editável A/B/C.

**Playhead no piano-roll do baixo — 01/11/2026:** o gráfico de notas e da lei
de duração agora recebe a posição do transporte do core JUCE. Uma linha vertical
percorre a barra exibida durante a reprodução e congela na posição ao pausar;
ela representa o progresso dentro da barra atual, calculado a partir da fase
da seção de quatro compassos. O desenho não altera eventos, duração nem áudio.
O timer existente atualiza o marcador; não foi criado um relógio paralelo.
`batruc_app` compilou em Debug, série `-j1`; SHA-256 do binário:
`5dc11b75e673bf5cddf3e60aa563df1b07dfbaf5ea2a388127f134dc395303de`.
Preflight antes do build: 8,2 GiB disponíveis, 126,8 MiB de swap em 2 GiB,
963 GiB livres no volume e nenhum processo pesado concorrente. Não rodei testes
nem abri o app; a inspeção visual continua pendente. Próximo: conferir a
sincronização no app, sobretudo em compassos diferentes de 4/4, e ajustar
contraste/posição se o marcador encobrir informações do piano-roll.

**Auditoria pontual de cliques JUCE — 01/11/2026:** reabri o app Debug e
confirmei um clique real no botão do baixo: `Desativar baixo` alternou para
`Ativar baixo`, e o segundo clique restaurou `Desativar baixo`. O estado inicial
do baixo ficou restaurado. Isso prova que o botão/callback responde; não valida
todos os controles. A captura mostrou o painel de mixer/baixo visível e nenhum
objeto cobrindo esse botão. Durante a reprodução, o processo chegou a 90–100%
de um núcleo em amostras curtas (cerca de 110 MiB de RSS); isso pode contribuir
para atraso perceptível, mas não prova que causou cliques perdidos. O Debug
imprimiu duas asserções JUCE na inicialização. Algumas mudanças são adiadas por
projeto: fórmula/ciclos/swing para a barra seguinte; BPM para a seção seguinte;
ADSR e controles de filtro são lidos diretamente a cada bloco de áudio. Faltam
perfil sustentado de CPU, identificar as asserções e verificar controles
representativos em execução; não foi feita auditoria exaustiva de mouse.

**Paleta percussiva JUCE — 01/11/2026:** acrescentei perfis selecionáveis por
voz para bumbo (membrana com queda de afinação), caixa, chimbal, prato ride,
conga e escovas. A escolha global também aceita os doze perfis; overrides por
canal continuam disponíveis. A seed agora pode selecionar qualquer perfil por
voz e novas cenas usam schema 34; cenas antigas conservam a interpretação dos
índices anteriores. A síntese usa osciladores modais e ruído procedural já
existentes no Batruc, sem samples e sem dependência de outro instrumento. A
implementação é uma primeira paleta de sound design: ainda não representa
articulação completa, kits, estilos ou sala, e requer escuta para ajustar
ataque, cauda e balanço. O app JUCE compilou em Debug com `-j1`; binário
`.local-build/native/linux-debug-juce/batruc_app_artefacts/Debug/Batruc`,
SHA-256 `9b93559290a4b28d524601c83e53d92c5227cd2e632dbe76928859b7fbc40e8c`.
Não rodei testes, não abri o app e não fiz escuta; logo, a compilação confirma
integração de código, mas não valida a resposta musical dos seis perfis.

**Direção indicada pelo autor para a pesquisa sonora:** aprofundar a
reapropriação de mecanismos próprios do Antitotem e do Rasgo Modular, além do
Trioio. Para a próxima camada, avaliar reverberação, delay, amortecedores e
absorção por material como processos que respondam às trajetórias e aos
eventos, com controles de cauda/damping e nível seguro. O inventário aponta
como candidatos o `DRUM` e outros módulos de matéria do Modular, e a cauda FDN
de quatro linhas do Aquorbium; são referências a auditar e adaptar, não
dependências nem garantia de integração direta. Primeiro definir o contrato de
sinal e o orçamento de CPU/memória no callback; depois comparar bypass/efeito e
escutar em mono e estéreo. A animação geométrica deve explicar a propagação e
as reflexões que realmente alimentam o efeito, sem ser decorativa.

Referência consultada: Trioio, `docs/MAQUINA_DE_TIMBRES.md` e
`src/core/AcousticVoice.cpp`, commit `cfbd74409f4d5e48106cebc4a595d3324844cc95`
(AGPL-3.0-or-later). Foram estudadas as receitas de kick, snare/brushes, ride,
hi-hat e conga; nenhum código foi copiado nesta etapa. O inventário RASGO também
aponta o ressonador modal/striker do Aquorbium e o módulo DRUM do Rasgo Modular
como fontes para uma etapa posterior. Validação de compilação e escuta ainda
pendentes.

**Cinemática da trajetória influindo no baixo — 01/11/2026:** além da
correlação/regressão já registradas, a preparação seed amostra o caminho
`r(t)=(V1(t),V2(t))` ao redor de cada evento, estima `r′` e `r″` por diferenças
centrais e deriva velocidade normalizada `ṽ=tanh(||r′||/8,8858)` e curvatura
assinada limitada `κ̃=tanh(κ/4)`. A média por ciclo entra na receita do baixo:
`ṽ` escala durações Fibonacci/geométricas em ±15% (a duração manual fica
intacta), e `|κ̃|` acrescenta até 6% ao acento. O readout do percurso, tooltip,
manifesto e eventos de partitura registram as medidas. Build Debug de
`batruc_app` passou em série (`-j1`) após a mudança. Não rodei testes, não abri
uma nova instância, e não fiz escuta. Como Batruc já estava aberto antes do
build, sua janela atual ainda executa o binário anterior; use `./run` depois de
fechá-la para experimentar. Próximo passo: conferir a leitura visual, comparar
renderizações on/off em seeds iguais e só então ajustar os pesos pela escuta.

**Matemática/estatística conectada ao baixo — 01/11/2026:** o percurso de sete
ciclos agora também calcula, a partir dos eventos de voz, a correlação de
Pearson entre `x` e `y` e a inclinação `y(x)` da regressão linear, comprimida
por `tanh` para um domínio limitado. Ambos passam a influenciar o alvo de
deslocamento da altura; são gravados no manifesto/partitura e aparecem na
leitura do gráfico do percurso. Variância nula produz zero e não introduz
valores indefinidos. A regra não fixa escala ou harmonia. **Validação ainda
pendente:** `batruc_app` recompilou em Debug com `-j1`; ainda falta conferir
visualmente o readout e abrir um manifesto para confirmar os campos. Não rodei
testes nem fiz escuta nesta etapa. A consequência musical requer comparação
auditiva em seeds diferentes e avaliação da intensidade dos pesos. Próximas
frentes registradas no README: cinemática de curvas, álgebra linear entre
vozes, probabilidades condicionadas, acústica ligada ao sintetizador e forma
entre frases; o mixer permanece pós-geração, com nível/pan/mute/solo/master.

**Polígonos em movimento e modelagem direta — 01/11/2026:** o gráfico JUCE de
triângulo/quadrado agora compõe duas formas convexas (3–8 vértices), animadas
por rotação e translação acopladas às fases/ciclos de V1 e V2. A interseção é
calculada por recorte convexo; a área usa a fórmula do cadarço; `H_area` é a
entropia de Shannon normalizada das regiões `A\\B`, `A∩B`, `B\\A`. Arrastar
vértices deforma mantendo convexidade; arrastar o interior translada; duplo
clique restaura a configuração determinada pela seed. É um estudo visual e
analítico, ainda sem efeito no áudio. `batruc_app` compilou em Debug após a
implementação. A interação e a legibilidade na janela real ainda não foram
inspecionadas; a edição manual ainda não é persistida nas cenas. Próximo passo:
abrir a geometria, testar arrasto/reset e, depois, decidir o mapeamento
composicional selecionável para interseção/área/entropia e persistência dos
modelos.

**LFO do baixo — continuação JUCE, 01/11:** substituí a modulação de altura
amostrada somente no ataque por uma curva contínua ligada à fase do transporte.
O seletor oferece três destinos: altura (semitons), corte VCF (100 Hz por
unidade de profundidade) e amplitude VCA (até 65%). Os valores de controle são
interpolados em 16 amostras e o filtro recalcula coeficientes na mesma cadência,
mantendo custo limitado no callback. A prévia desenha a forma e identifica o
destino; a leitura de altura mostra o MIDI contínuo, sem arredondar a curva para
um degrau de semitom. O destino foi adicionado às cenas schema 29; cenas
anteriores leem como altura. O TXT de composição também registra destino,
unidade e equação. É uma ampliação JUCE, ainda sem equivalente HTML, portanto
não altera a estimativa de paridade.

Build Debug dos alvos `batruc_app` e `batruc_bass_audio_response_test` passou.
O teste existente agora compara a saída do LFO desligado com cada destino
ligado; `batruc_bass_composition` e `batruc_bass_audio_response` passaram (2/2).
`git diff --check` passou. Não houve inspeção visual nem escuta pelo dispositivo;
os testes provam que cada destino muda o sinal renderizado, não que a escala
esteja musicalmente calibrada ou que o VCF soe como um filtro analógico.
Próximos passos: inspecionar os controles no app e ouvir pitch/filter/amplitude
com a mesma frase, depois continuar com a interação rítmica do baixo e a
telemetria de mix/master pendente.

**Operação generativa no baixo — 01/11:** conectei ao painel JUCE a
retrogradação temporal da performance. O botão `Retrogradar tempo` reflete cada
intervalo de evento dentro do seu ciclo formal; alturas, duração, intensidade,
origem e etiqueta do ciclo permanecem. `Tempo original` desfaz o gesto ao
recalcular a performance a partir da mesma seed/receita, sem acumular novas
transformações. O plano ordena novamente os eventos por tempo local para
respeitar o contrato do renderer e entra na fronteira de compasso usada pelo
handoff de áudio. O estado é persistido em cenas schema 30; cenas anteriores
abrem com a operação desligada.

`retrogradeBassPerformance` valida limites/valores antes de escrever e preserva
os metadados do plano. O teste confirma a reflexão dos eventos por ciclo e que
aplicar a operação duas vezes restaura tempos, alturas, duração, intensidade,
origem e ciclo. Build JUCE passou; quatro testes focados passaram (operações
generativas, composição do baixo, resposta do áudio e transporte). Não houve
inspeção visual nem escuta no dispositivo. A retrogradação muda a relação
temporal com as vozes por definição; a possibilidade de combiná-la com cada
modo de relação precisa de avaliação musical. Próxima etapa da máquina de
operações: expor recombinação dos ciclos formais sem perder os ataques ligados
às vozes.

**Inversão do contorno do baixo — 01/11:** conectei ao mesmo painel o espelho
de alturas MIDI em torno do eixo 42 (F♯2), pela regra `m′=2·42−m=84−m`.
O botão alterna entre o contorno original e o invertido; o plano seeded é
recalculado a partir da mesma origem, então ligar/desligar não acumula
transposições. A operação preserva tempos, durações, intensidades, origem dos
eventos e ciclo formal. A função rejeita o resultado se qualquer altura sair
de MIDI 24–60 e não altera o plano de entrada. Cenas schema 31 guardam o estado
da operação e registram eixo/equação; cenas anteriores abrem com a inversão
desligada.

O teste cobre o exemplo 36/43/48 → 48/41/36 e a aplicação dupla, confirma que
tempo, duração, intensidade, origem, ciclo formal e os seis vetores do percurso
permanecem intactos, e verifica que uma inversão fora da tessitura não deixa
resultado parcial. App e teste do módulo compilaram; quatro testes focados
passaram (`generative_operations`, `bass_composition`, `bass_audio_response`
e `variant_transport`). Ainda não houve escuta ou inspeção visual. Com
retrogradação e espelhamento, duas operações do módulo generativo já afetam a
performance JUCE; a recombinação e o histórico de operações com comparação
continuam na fila.

**Recombinação entre ciclos do baixo — 01/11:** integrei ao painel JUCE um
operador para linhas geradas por seed. Os ataques e os ciclos-alvo são mantidos;
em cada compasso, altura, intensidade e duração vêm do evento de fase mais
próxima no mesmo compasso de outro ciclo formal. O deslocamento-base é
`1 + seed mod 6`; em cada compasso avança uma posição, garantindo que o doador
seja sempre outro ciclo. Se o ciclo doador não tiver evento naquele compasso,
o evento-alvo conserva seus parâmetros. A duração é limitada pelo próximo
ataque do ciclo-alvo e pelo fim do compasso. Assim a linha muda de material sem
substituir sua grade de ataques derivada das vozes. O botão `DESFAZER RECOMB.`
desliga a transformação e recompõe o material base da seed, mantendo as outras
operações ativas. Editar manualmente a linha desliga a recombinação, pois ela
precisa dos sete ciclos formais gerados. O cabeçalho do gráfico de percurso
indica `REC Δn` com o deslocamento doador ativo; o tooltip registra a regra.

Cenas schema 32 salvam a opção; cenas anteriores abrem com ela desligada. A
função do core valida o plano antes de escrever, preserva `sourceNote`, eixo de
tempo e os seis vetores de percurso, e não aumenta a quantidade de eventos.
Teste dedicado cobre a seleção do doador, a preservação dos ataques e a rejeição
de deslocamento inválido. O app e o teste compilaram; quatro testes focados
passaram (`generative_operations`, `bass_composition`, `bass_audio_response` e
`variant_transport`). `git diff --check` também passou. Ainda falta conferir
visualmente o espaço do botão e ouvir comparações on/off em várias seeds. O
binário Debug foi produzido em 01/11 às 01:42, SHA-256
`12a5bb8043b730118f6ad502254610a0130add10e28d80c46f88baa04bfb27f1`.

### Render A/B reproduzível da recombinação do baixo — 01/11/2026

Ampliei `batruc_offline_render` com `--recombine-bass`. A opção usa a mesma
seed e a mesma composição do render de controle, aplica a recombinação antes
do sintetizador e registra no JSON o estado, o deslocamento doador e a regra.
Com `--stems`, o render exporta vozes e baixo separados para comparação local.
Os arquivos de auditoria ficam em `.local-build/offline-recombine-audit-20261101`
e não são material de publicação.

Gerei pares controle/candidato a 48 kHz, estéreo, PCM16 e sete ciclos para
três seeds. Nas três, o WAV de vozes foi bit a bit idêntico e a agenda das
vozes também. Os ataques do baixo foram preservados, enquanto o material
recombinado variou: `5EED2027` (5 vozes, offset 4) manteve 23 ataques e mudou
12 alturas, 12 intensidades e 5 durações; `00000000` (10 vozes, offset 1)
conservou 98 ataques e mudou 49 alturas, 67 intensidades e 9 durações;
`A17E2026` (9 vozes, offset 1) conservou 96 ataques e mudou 49 alturas,
68 intensidades e 19 durações. Os stems de baixo diferem em todos os pares.
Na seed `5EED2027`, o RMS controle/candidato/diferença foi
0,0042166 / 0,0038008 / 0,0041294. Os picos verdadeiros estimados do mix
foram 0,211044/0,217945, 0,308739/0,308601 e 0,545574/0,552274 para cada par,
todos abaixo de 1,0. Isso confirma mudança do sinal do baixo sem alterar as
vozes nestas três receitas; não substitui escuta nem inspeção do painel.

Validação desta etapa: o alvo `batruc_offline_render` compilou. Os WAVs, stems
e JSONs foram renderizados e comparados por script local. Depois, o autor
escutou os renders e relatou que o baixo ficou “mais encaixado”; não indicou
semente ou trecho específico, então registro isso como avaliação auditiva
favorável da operação, sem ampliar a afirmação para validação de cada caso.
Ainda faltam inspeção visual e comparação auditiva documentada por seed. Nenhum
commit ou push foi feito.

### Pêndulo triplo acoplado como receita composicional offline — 01/11/2026

Transformei o solver RK4 já existente em uma segunda receita do render JUCE
offline, selecionável por `--coupled-pendulum`. Ao longo dos sete ciclos, os
cruzamentos ou extremos das três coordenadas angulares substituem as agendas de
V1–V3; as vozes a partir de V4 mantêm suas fórmulas. O estado é composto por
três pêndulos iguais em aproximação linear de ângulo pequeno, acoplados aos
vizinhos por molas efetivas e amortecimento. A energia cinética e gravitacional
de cada pêndulo, mais metade da energia de cada mola adjacente, forma uma
contribuição local. Cada voz normaliza sua contribuição pelo próprio máximo e
usa isso para acentuar seus eventos entre 0,35 e 1. A soma das três
contribuições é conferida contra a energia total em cada amostra. Os campos `sin(θ₁)` e
`sin(θ₂)` alimentam a geração do baixo, que recalcula seus ataques a partir dos
eventos físicos; a duração da forma continua em 56 s. A seed escolhe condições
iniciais e parâmetros dentro de limites declarados. O manifesto inclui RK4,
passo de 5 ms, unidades, ângulos e velocidades iniciais para reproduzir a
receita.

Gerei, com a mesma seed `5EED2027`, um controle por fórmulas e um candidato de
pêndulo, ambos a 48 kHz estéreo PCM16 e com stems. O plano de voz passou de
`[7,14,4,3,2]` para `[51,36,36,3,2]` eventos por canal; o baixo mudou de 23
para 40 ataques. As intensidades locais variaram de 0,402–0,986 em V1,
0,408–0,824 em V2 e 0,400–0,785 em V3. RMS do mix: 0,018138 no controle e
0,022715 no pêndulo; picos estimados: 0,209110 e 0,283900. O manifesto foi
lido e os WAVs têm 56 s, 48 kHz, estéreo. Isso mostra que o modelo gera uma
composição e afeta também o baixo; não demonstra que o resultado soe
musicalmente melhor.

Arquivos: `src/core/CompositionCore.cpp` e `src/tools/offline_render.cpp`.
O alvo `batruc_offline_render` compilou em série e gerou o par
`.local-build/physics-pendulum-20261101/5EED2027-formula-final.wav` e
`5EED2027-pendulum-transfer.wav`. Ainda não executei testes unitários,
abri a GUI ou fiz escuta. Esta receita existe somente no renderizador offline;
falta levá-la à interface e ao transporte JUCE, expor seus parâmetros, desenhar
as três trajetórias e validar estabilidade/escuta em mais seeds. O modelo é
idealizado: não representa o pêndulo serial do artigo nem um instrumento
acústico medido. Nenhum commit ou push foi feito.

### Gráfico animado do modelo físico na GUI JUCE — 01/11/2026

Acrescentei ao painel de geometria um alternador **Estudo físico · pêndulo
triplo**. O gráfico mostra três massas suspensas, molas efetivas entre elas,
rastros das trajetórias, ângulos individuais e energia local normalizada de
cada voz. A posição do sistema e o cursor de energia acompanham o transporte;
a seed da composição seleciona deterministicamente parâmetros e condições
iniciais, apresentados no próprio gráfico. O cálculo reutiliza o solver RK4 do
core e é preparado ao mudar a seed, fora do callback de áudio e do `paint`.

O painel é um estudo do modelo, não a visualização do render físico offline em
reprodução, e não altera a fonte sonora JUCE atual. A interface declara isso no
tooltip e no rodapé do gráfico. Os controles de curva/Fourier e a opção de
usar a geometria em V1 ficam ocultos enquanto o estudo físico está ativo; a
disposição usa uma linha separada com largura calculada para evitar a
sobreposição do alternador e do controle de fonte. `batruc_app` compilou em
série após a edição. Não abri a GUI nem fiz inspeção visual em execução; escuta
e validação de enquadramento na resolução do autor continuam pendentes. Próximo
passo: ligar o plano de eventos físicos à receita JUCE ao vivo, para que o
gráfico possa acompanhar a mesma composição sonora, antes de apresentar esse
modo como instrumento integrado.

### Derivada e integral no gráfico temporal — 01/11/2026

Acrescentei ao cabeçalho do gráfico de ondas um controle compacto que alterna
entre `f(t)`, `f′(τ)` e `∫₀ᵗ f(u)du`. A análise usa a curva que a voz realmente
apresenta no desenho — depois de transformação, fase, forma geométrica e
acoplamento —, amostrada em 512 pontos por seção. A derivada usa diferenças
finitas centrais (unilaterais nas bordas); a integral usa a regra trapezoidal.
Derivada e integral são normalizadas pelo máximo de cada voz para caber nas
faixas existentes. O cursor continua acompanhando o transporte e os
marcadores mantêm os tempos dos ataques da receita original.

O tooltip e o título identificam essa vista como análise, sem alterar áudio ou
agenda de eventos. A linha do controle foi posicionada ao lado dos botões de
movimento e zoom; a largura do título se adapta ao espaço restante para evitar
colisão em janelas menores. `batruc_app` compilou em série. Não fiz captura nem
abri a GUI; inspeção visual no tamanho mínimo de janela e escuta continuam
pendentes. Em conjunto com os estudos existentes de Fourier, bilhar e pêndulo,
isso cria uma sequência visual de sinal → derivada → integral → eventos. A
próxima etapa composicional é permitir escolher quais dessas quantidades
controlam ataques, dinâmica ou duração, com mapeamento explícito e A/B.

### Cursor de voz e trajetória XY com direção — 01/11/2026

Ampliei os gráficos existentes sem adicionar painéis. No gráfico temporal, cada
voz agora recebe um ponto de estado no playhead, calculado na mesma curva
exibida (sinal, derivada ou integral); ele acompanha a onda móvel e permanece
no tempo correto na linha do tempo fixa.

No campo XY, a trajetória e o cursor agora usam o mesmo offset e a mesma
inclinação do arco formal de fase em todos os ciclos. Antes, o traço de fundo
usava apenas as fases-base, podendo divergir do cursor nos ciclos de avanço e
retorno. O cursor também mostra uma cauda curta com atenuação progressiva,
vetor de velocidade instantânea e projeções pontilhadas nos eixos. Esses
elementos codificam percurso, direção e coordenadas atuais; não alteram a
geração sonora. `batruc_app` compilou em série após a correção de alinhamento.
A inspeção visual na janela real e a confirmação de legibilidade com 12 vozes
seguem pendentes; nenhum teste unitário ou escuta foi executado nesta etapa.

**Histórico do item 1 antes do guard — 31/10:** o callback JUCE mede RMS
separado de vozes e baixo, mas não mede pico verdadeiro nem redução no estágio
final. O master aplica `tanh` amostra a amostra depois de somar os buses; é uma
saturação audível, não um limitador transparente com teto/telemetria. O trim
inicial das vozes é +16,5 dB. Métricas já arquivadas do render de referência
`5EED2027` (28/10) mostram amostras sem clipping digital, mas RMS de vozes
JUCE 0,08295 contra HTML 0,00687 e RMS de mix JUCE 0,05920 contra HTML
0,01041; o baixo fica mais baixo no JUCE (0,00311 contra 0,00657). Correlação
de envelope do mix: 0,369, com 50% de silêncio no JUCE e 90% no HTML. Esses
renders são antigos e têm diferenças de janela/silêncio; demonstram que a
mistura não está equilibrada entre motores, não identificam sozinhos a causa
percebida como clipping nem descrevem o binário atual. Próxima implementação:
telemetria pré e pós master (pico amostral, pico verdadeiro aproximado, RMS e
redução), teto de saída com estágio apropriado e ajuste de headroom sem
normalização automática do material. Em seguida, renders curtos comparáveis
dos buses isolados e do mix em 2/9/12 vozes, mute/solo/pan e baixo ligado/
desligado; só então calibrar os ganhos. Não elevar a paridade por métricas
antigas ou somente por ausência de amostras em full scale.

**Estado atualizado — 31/10:** o `tanh` final foi substituído por guard estéreo
com lookahead de 3 ms, teto amostral de −1 dBFS e estimador polifásico 4×
(Blackman-Harris, 16 taps/fase), adaptado do estimador autoral do Modular. O
callback e o render offline usam o mesmo estágio; o WAV gravado é escrito após
ele. O medidor segura máximos da sessão e reinicia ao reconfigurar dispositivo.
Não há LUFS nem filtro DC; a estimativa 4× ainda não foi certificada com
vetores EBU.

`tests/output_safety.cpp` passou: impulso, sinal sustentado, link estéreo, NaN,
seno intersample em oito fases, teto estimado e latência 3 ms a 44,1/48/96 kHz.
App, renderizador e teste compilam; CTest focado passou 1/1. Renders em
`.local-build/parity-audio/output-stage-20261031/`: cinco vozes, RMS 0,0178,
sample/true-peak estimados 0,207, redução 0 dB; doze vozes, pico estimado
0,134, redução 0 dB. Essas receitas não reproduzem clipping nem acionam o
guard. Contra o HTML histórico 5EED2027, o render JUCE teve RMS 0,0178 contra
0,0104, pico 0,207 contra 0,530 e silêncio 51,5% contra 90,3%; diferenças de
janela e início impedem calibrar ganhos ou declarar equilíbrio por esta medida.
Pendente: gravar WAV curto pelo app atualizado, conferir medidor/atraso/teto no
arquivo, ouvir transientes e então capturar 2/9/12 vozes com estados idênticos
de mixer, pan, mute/solo e baixo. O clipping ouvido na sessão real segue sem
causa reproduzida.

**Continuação do item 1 — 31/10, áudio e resposta da interface:** no JUCE, o
guard agora drena só os 3 ms pendentes após Parar, preservando a última cauda
sem processar silêncio indefinidamente. O pico amostral de entrada do medidor
foi movido para depois do ganho master, no mesmo ponto que o true peak de
entrada. O teste de segurança cobre essa cauda. O render offline ganhou
`--voices 2..12` e também escreve a cauda do guard. Com a seed `00000010`, a
combinação de duas vozes e máscara diagonal produzia **zero eventos e zero
áudio**; 9 e 12 vozes produziam áudio. O plano agora conserva a máscara e
admite um ataque discreto de V1 na passagem mais próxima dela quando a máscara
rejeita todos os cruzamentos de V1. O mesmo caso foi coberto para ritmo estático
e ritmo recalculado ao vivo. O render de 2 vozes passou de silêncio integral
para RMS de vozes 0,00696 e baixo 0,00539; nenhum sample clipado. Isso resolve
um caso concreto de seed muda, não prova equilíbrio ou ausência de cliques.

Na navegação, o timer de 30 Hz deixou de invalidar os 3500 px da página quando
o transporte está parado; durante reprodução invalida só o campo de performance.
Gráfico da onda calcula cada amostra de curva uma vez, e o desenho pesado é
pulado quando a seção não está visível. LFO e traço geométrico evitam repintura
sem mudança. Trocar seed não recompõe duas vezes o baixo. O novo binário abriu
em uma segunda instância local sem erro de inicialização observado; essa
instância de teste foi fechada, preservando a que já estava aberta. Build JUCE
e 17/17 testes locais passaram. **Ainda falta medir latência de cliques e carga
de CPU em uso real**, navegar pelas seções e conferir o WAV gravado pelo app;
o processo que já estava aberto usa binário anterior até ser reiniciado.

**Medição de navegação — 31/10:** a compilação Release de JUCE 9.0.3 abriu no
desktop; em repouso usou cerca de 5–9% de um núcleo e 94 MB RSS. Ao tocar no
layout maximizado de nove vozes, usou cerca de 40–47% de um núcleo e 102 MB
RSS, confirmando carga contínua capaz de retardar cliques sob concorrência.
Uma primeira otimização do gráfico reutiliza, por quadro, as amostras já
calculadas das vozes anteriores quando há acoplamento, evitando recursão
repetida nas 512 amostras de cada voz. Em medição inicial de reprodução,
ficou em torno de 37–40% de um núcleo; amostra curta, sem controle exato da
mesma seed, portanto não é benchmark conclusivo. O timer visual passou de 30
para 24 Hz, mantendo animação e reduzindo a frequência de pintura. Ainda
falta medir essa alteração final, verificar rolagem e cliques em mixer/baixo/
geometria, e separar custo da interface do custo da síntese em perfilagem.

**Layout — 31/10:** captura localizada do rodapé da janela revelou o medidor
master sobre os botões Baixo, REC e Geometria. Em `src/app/Main.cpp`, o rótulo
do medidor foi deslocado para a linha seguinte, com largura ampliada. Após
novo preflight (8,8 GiB disponíveis, swap 333 MiB/2 GiB, 964 GiB livres),
`timeout 300 cmake --build .local-build/native/linux-debug-juce -j1 --target
batruc_app` passou e atualizou o binário Debug. A inspeção dos bounds no código
mostra os botões em y=738–766 e o medidor em y=771–791, sem interseção. Ainda
falta conferir visualmente a linha inferior em 1080 px e maximizado e testar
os atalhos Baixo, Mixer e Geometria no aplicativo novo. O build não comprova
esses gestos nem a ausência de sobreposições nas demais seções.

**Conferência da janela mínima — 31/10:** o estado do transporte tinha apenas
64 px no slot à direita em 1080 px. `src/app/Main.cpp` agora usa a linha livre
abaixo dos controles, à esquerda do medidor, nessa largura. O app Debug
recompilou com `-j1`; uma única captura da janela 1080×800 mostrou estado,
medidor e botões legíveis, e dez linhas de voz no gráfico. A instância de
verificação foi fechada. A rolagem das seções Mixer, Baixo e Geometria ainda
precisa de teste interativo; a investigação do percurso do baixo foi iniciada,
sem alteração de código nessa frente.

**Avanço integrado dos itens 2, 3, 5 e 6 — 31/10:** o percurso do baixo deixou
de guardar somente o passo anterior. `src/core/BassPath.hpp` publica agora
campo, atividade, integral discreta limitada `F+C`, derivada, curvatura, rota
e offset para cada ciclo. A constante `C` vem da seed; fuga 0,72 e limites
±1,5 tornam a memória explícita e impedem deriva sem controle. O cálculo atua
na altura, abre respiros determinísticos em eventos menos fortes e acrescenta
um acento limitado quando a curvatura muda. `BassPerformance` carrega esse
plano; o novo gráfico do painel Baixo desenha exatamente os dados entregues ao
áudio, incluindo densidade, ciclo ativo e rotas EXP/SIN/ORB/RUP. A seção ganhou
20 px e o LFO foi compactado para não sobrepor o novo gráfico.

O desenvolvimento das vozes entre passagens também ganhou memória limitada de
oito passagens em `PassageDevelopment.hpp`. Seed e macroíndice inicializam o
estado; uma recorrência com fuga produz rotação, inversão, célula de repouso e
acento herdado. Vozes, baixo e marcadores gráficos já consultavam a mesma
função, portanto os três recebem a mudança sem agendas visuais separadas.
Build Debug do app passou em série (`-j1`). Não foram feitos nesta etapa teste
auditivo, teste interativo da rolagem nem certificação visual do painel; falta
ouvir várias seeds, conferir se os respiros são musicalmente suficientes e
comparar o ciclo indicado no gráfico com ataques reais. Próximo passo: levar o
mesmo plano publicado à partitura/arquivo de composição e permitir controle
autoral da fuga, memória e força dos respiros dentro de limites seguros.
Binário Debug desta etapa: SHA-256
`5e7f7e513444f507b3eb79a25de399ff52461d9b88ab67819e53e1f7c484a620`.

**Persistência e controles do percurso — 01/11 (comportamento atualizado em 11/11):** o painel Baixo ganhou
`MEMÓRIA` (0–95%) e `RESPIROS` (0–200%). O primeiro controla a retenção da
integral entre ciclos; o segundo, após a correção de 11/11, articula os ataques
encurtando o gate em até 30%, sem removê-los do calendário. Ambos recompõem a agenda
seeded fora do callback, aparecem na fórmula visível, são guardados em
`BassRecipe` e entram no esquema 27 das cenas. O leitor aceita o esquema 27 e
mantém 72%/100% ao abrir cenas anteriores.

Exportações `.batruc.txt` com plano seeded passam a guardar os sete estados
calculados (`pitch_offset_semitones`, integral, derivada, curvatura, rota e
densidade). A configuração inicial do TXT companheiro do WAV também os inclui,
e cada evento de baixo da agenda inicial recebe o estado do seu ciclo. Isso
leva o mesmo plano do áudio e do gráfico à partitura matemática registrada.
A seção Baixo foi ampliada para 790 px e Geometria deslocada para y=2390; seu
fim permanece em y=3490 dentro da página de 3500 px. Build Debug em série
passou; SHA-256
`4f5ef23188ac1c1299f01f26b002b022be7fdaa1efba987a5810ffbd1aa9d68e`.
Não houve escuta, interação com os novos sliders nem ciclo real de
exportar/reabrir nesta etapa; a compilação e a revisão estática não substituem
essas verificações.

**Gráfico editável — 01/11:** `BassPathView` deixou de ser somente leitura.
Com seed ativa, arrastar horizontalmente escolhe a retenção da memória
(0–95%) e verticalmente escolhe a força dos respiros (0–200%). O ponto de
controle e o readout M/R acompanham o gesto, mas a agenda é recomposta somente
no `mouseUp`; assim o gráfico atua no cálculo sem reconstruir centenas de
eventos a cada pixel percorrido. Os sliders permanecem sincronizados e
oferecem edição precisa. O tooltip descreve o mapeamento. Build Debug passou;
SHA-256
`bd4f9582f0b14f021f9b4ba0b599c5b13cab260376da39460c98fa5c33ef2df4`.
Ainda falta validar o gesto no app e ouvir sua consequência em seeds densas e
esparsas.

**Estado da passagem visível e registrado — 01/11:** o cabeçalho interno do
gráfico das vozes mostra agora a passagem ativa e o estado exato calculado por
`passageDevelopmentState`: memória M, rotação de célula, direção/inversão e
célula de repouso. Os marcadores do gráfico e os renderers de vozes/baixo já
consultavam `passageEventWeight`; a legenda torna essa transformação legível,
sem criar outra agenda visual. A cena inclui a equação da memória e a descrição
das operações. Ao iniciar REC, a agenda inicial anota em cada evento a passagem
de captura e seus quatro estados, e a partitura matemática copia a fórmula.
Build Debug passou; SHA-256
`fa68d40c78e676f767708af3f737d8cc7413657c00b02b5772772d5ff7cef798`.
Pendente: observar a troca no final de uma forma completa, confirmar a
legibilidade em 1080 px e escutar se rotação/inversão de acentos formam uma
derivação reconhecível em vez de variação arbitrária.
Na revisão da legenda encontrei que algumas células de repouso eram duplicadas
pela antiga comparação módulo quatro, embora a interface identificasse oito
posições. A regra agora silencia exatamente uma das oito células escolhidas,
com deslocamento entre vozes. Build Debug recompilado; SHA-256
`e2a021908593e56d48ae4a4aafea4d2810ab682bfdc0771482e9dd1fd8957f93`.
Também distingui a primeira passagem (`BASE`) das posteriores (`DEV`) no
gráfico: P1 preserva os ataques-base, embora os parâmetros determinísticos
seguintes já estejam preparados. O plano da gravação registra
`passage_development_applied` em cada evento e explica essa convenção. Build
Debug passou; SHA-256
`fc035cc26de50b329fa35e7293e45b5b25c00cfb36765d9d3e5c065358d149d4`.
Ainda falta abrir o binário e ouvir a transição BASE→DEV ao completar sete
ciclos; a renderização inicial da partitura documenta a passagem no momento
de início da gravação e não prevê mudanças futuras durante a captura.
Na agenda inicial gravada, eventos de voz agora guardam também o peso de
passagem aplicado e se o evento soa. Isso permite reconstituir a articulação
calculada para cada ataque junto com os metadados de baixo. Build Debug passou;
SHA-256 `1727e1508cfe2fd32c8390ba271cace8ffa8d6531ae055982946af02cf684919`.
No gráfico das vozes, ataques mantidos aparecem como pontos, acentos de
desenvolvimento recebem um aro exterior, ataques atenuados um aro discreto e
repousos um X. A legenda traduz os quatro casos em PT/EN/FR. Isso torna o
estado composicional verificável visualmente no mesmo lugar da curva. Build
Debug passou; SHA-256
`fa64ded4d77499f636044cce218bef65c39c9d932b42bfefa29e66e0e7c54fd2`.
Reservei a faixa superior do gráfico em três zonas proporcionais para título,
legenda e estado P/M/rotação/direção/repouso, com a largura da legenda
adaptada ao painel. Isso reduz a colisão dos rótulos em janelas estreitas sem
esconder o estado composicional. Build Debug passou; SHA-256
`e8321c93f702d44823a0baf0de9485bcc11adbcebe16083768145b376f272a34`.
Arquivos: `src/core/CompositionCore.{hpp,cpp}`, `src/core/OutputSafety.hpp`,
`src/app/Main.cpp`, `src/tools/offline_render.cpp`, testes `seeded_recipe.cpp`
e `output_safety.cpp`. Derivados de áudio ficam em
`.local-build/parity-audio/output-stage-20261031/`, sem substituir referências.

Histórico do medidor antes do guard: o rodapé do JUCE mostrava pico amostral pré e pós
master e a redução adicional da curva `tanh`, descontando o ganho do slider.
O rótulo identifica “SAMPLE PEAK”; não é true peak nem medição BS.1770. Build
local passou. A leitura ao vivo e sua comparação com WAV gravado ainda precisam
ser conferidas; não ajustar ganhos com a redução isolada.

Incremento de proteção, agora atualizado abaixo: substituí a curva `tanh` final por um limitador estéreo
local com lookahead de 3 ms e teto nominal de −1 dBFS. Os buffers são alocados
em `prepareToPlay`; gravação acontece após o mesmo estágio, portanto inclui a
mesma latência/sinal enviado à saída. A recuperação de ganho aguarda o buffer
de lookahead esvaziar e há um teto amostral final de segurança. O detector usa
reconstrução polifásica 4× com janela Blackman-Harris e 16 taps por fase,
adaptada do estimador autoral do Modular; ainda não foi certificada contra os
vetores EBU de true peak. Não há
filtro DC nem medição LUFS; o valor LIMIT é redução máxima desde o
início da sessão. É uma primeira proteção contra clipping, ainda não a saída
comum de excelência. Build passou; medição comparativa do WAV e escuta de
transientes continuam pendentes antes de aceitar esse estágio.

O guard foi movido para `src/core/OutputSafety.hpp`, com teste próprio
`tests/output_safety.cpp`: impulso acima do teto, sinal sustentado, link
estéreo, redução reportada e saneamento de NaN passaram. CMake recompilou app e
teste; `ctest -R '^batruc_output_safety$'` passou (1/1), incluindo oito fases
de seno próximo a Nyquist e latência a 44,1/48/96 kHz. O medidor mantém picos
máximos durante a sessão e reinicia quando o dispositivo reconfigura. A
estimativa ainda não foi certificada com vetores EBU. O teste não valida áudio do
dispositivo ou arquivo WAV gravado. Também conferiu impulso em 48 kHz: a saída
surge após 144 amostras (3 ms), o pico de amostra fica no teto e o pico
reconstruído de saída permanece abaixo de −0,7 dBFS neste caso de estresse.
Próxima verificação: capturar um WAV curto do app e comparar duração, latência,
picos e medidor; depois ouvir os transientes.

**Página única JUCE e paleta sonora — 31/10:** o autor pediu que mixer, baixo
e geometria apareçam no mesmo fluxo rolável da janela; “Sobre” e “Aprender”
permanecem no cabeçalho. A implementação em `src/app/Main.cpp` posiciona
esses controles em seções fixas, com áreas e curvas simultâneas na geometria;
os antigos botões conduzem à seção. A inspeção da janela maximizada revelou
que a página interna continuava com 1080 px: `Viewport::getViewWidth()`
informava a largura visível do conteúdo antigo. A janela agora dimensiona a
página pela largura real do viewport, reservando espaço para sua barra de
rolagem. Build e inspeção visual em 1920 px confirmaram que gráficos, órbitas
e seções se expandem; a navegação por todas as seções e a verificação de
sobreposições ainda precisam de teste. A seleção por seed
agora alcança caráter global, perfis por voz e matéria do baixo, preservando
ajuste manual; o incremento tímbrico compilou, mas ainda falta ouvi-lo.
Famílias pedidas: bateria, percussão, xilofone, natureza, objetos, sonoplastia;
graves acústico, fretless, Moog, piano grave, tuba, fagote, sax barítono e voz
baixo. Aprofundar LFO/oscilladores e mapeamentos. Ilhas adesivas, microlinhas
diagonais e máscaras de pausa devem evoluir para campos deformáveis por seed
que alterem eventos, com visualização e áudio alinhados. Contrato conceitual
no README; essas novas famílias e máscaras ainda não foram codadas.

**Percursos relacionais de altura — 31/10:** a leitura dos instrumentos irmãos
confirmou três referências operacionais: `BassFunctionPlan`/`BassPhrasePlan`
do Studio separam função, frase e corpo sonoro; `ScaleMachine` e
`HarmonyWriting` do Trioio encadeiam transformações e harmonia escrita;
`Harmony` do Modular muda centro tonal por seção. Nenhum código irmão foi
copiado e o Batruc não ganhou dependência desses projetos. O novo
`src/core/BassPath.hpp` é um cálculo próprio: a seed escolhe resposta
exponencial, contraposição trigonométrica, órbita angular ou ruptura por
distância; a relação inicial vem da seed e pode mudar com energia e direção
anterior. Cada ciclo lê o campo e a energia das vozes, o deslocamento
anterior e a função formal, produzindo um offset limitado para o baixo seeded.
`BassComposition.cpp` aplica o percurso no calendário tocado; `Main.cpp`
mostra seu offset no resumo da barra editada. `tests/ecological_bass_composition.cpp`
verifica reprodutibilidade, relações distintas, resposta ao campo, memória e
limites. Build core/app e 3 testes focados passaram (incluindo fixture de
paridade, que não prova escuta nem igualdade de alturas web/JUCE). O processo
JUCE já aberto usa binário anterior até reabertura. Pendências: publicar o
percurso e a fórmula na interface, garantir que o resumo do editor corresponda
nota a nota ao áudio (inclusive nas relações ecológicas), ouvir e medir vários
seeds; ampliar a lógica entre passagens e os mapeamentos autorais de altura.
Após o ajuste das quatro funções e da ramificação entre ciclos, core e app
recompilaram, e o teste focado passou. Binário Debug final de 31/10 00:21:
SHA-256 `0c89f473cd6a3e403cc38606e631d629528b346eb97c0bce557e92c83d9cd730`.
Se uma instância JUCE estava aberta antes desse build, ela ainda usa o binário
antigo. A audição do novo percurso não foi executada nesta sessão.

**Integrais indefinidas de Vygodski — hipótese, ainda sem código:** pesquisar
famílias de primitivas `F+C` como memória de força/gesto e ponto inicial,
ligando área acumulada a ataque, resposta ou transição. Um integrador cíclico
deverá explicitar média, limites, reset, unidades, fórmula e efeito audível;
evitar deriva invisível e associação ornamental. O catálogo confirma a edição
francesa de 1973; o PDF indicado pelo autor não ficou acessível nesta sessão,
portanto páginas e fórmulas específicas ainda não foram verificadas. Próximo
experimento: integrador de área de intensidade com saída de eventos e gráfico,
comparando `C`, média removida e acumulação livre na mesma seed.

**Formalismo quântico como hipótese — 31/10:** estudar superposição e
interferência de amplitudes complexas para a decisão entre ramos musicais;
converter a soma normalizada em probabilidades somente na fronteira de frase,
com seed reprodutível. Ainda não há módulo quântico, estados físicos nem
simulação. O próximo experimento deve comparar esse mecanismo com um sorteio
ponderado clássico e expor fases e probabilidades para o compositor.

**Tractriz/pseudoesfera — 31/10:** o autor propôs esta dupla de geometria
diferencial. Registrar como futuro gerador de trajetória e superfície com
`a`, arrasto, fase de rotação e posição; mapear arco/velocidade/curvatura a
tempo, acento, articulação e espaço, usando a mesma amostragem no gráfico e
no plano de eventos. A pseudoesfera não é um modelo de reverberação por si;
simulação acústica exigirá domínio, condições de contorno e testes separados.
Formulação e fontes matemáticas estão no README. Sem código nesta etapa.

**Rótulo de ciclos e vozes — 31/10:** o autor observou somente V1/V2 no gráfico
ao aumentar `n₂`. O código confirma que `n₂` controla ciclos por seção da V2;
o seletor `VOZES` controla quantas linhas são desenhadas, até doze. O rótulo
visível agora diz `n₂ · ciclos V2` e o tooltip de `VOZES` explica a diferença.
Build JUCE passou. Ainda falta inspeção da janela nova; o processo aberto antes
do build mantém a interface antiga até ser reiniciado. O espaço vertical para
8–12 vozes segue insuficiente e permanece tarefa de layout.

**Correções de uso — 31/10:** a variação voltava a 0 porque
`clearSeedAfterManualEdit()` a zerava em toda edição com seed; a aplicação de
uma polirritmia também fazia reset direto. Esses resets foram removidos: o
valor visível permanece e a próxima alteração do slider toma o estado editado
como base. A fórmula selecionada da Lei I era desenhada em `x=1010` com apenas
30 px disponíveis na janela de 1080 px. Agora aparece na faixa acima dos
controles, com 548 px livres, sem cobrir Intenção e Dinâmica. Build JUCE passou.
O processo aberto antes deste build ainda executa a versão anterior; preservar
a sessão atual e reabrir quando terminar o teste. Falta inspecionar a nova
posição no aplicativo e verificar a continuidade musical da variação após
edições manuais sucessivas. As fórmulas individuais de todas as vozes ainda
precisam de um painel mais completo e legível.

**Abertura local — 30/10, 23h:** o aplicativo foi aberto no desktop após o
build. O depurador localizou uma asserção `juce_String.cpp:327` no tooltip do
perfil de osciladores do baixo; a string UTF-8 passou a usar `utf8(...)`.
O rótulo da passagem também foi corrigido para conversão explícita. Após novo
build, a janela abriu sem novas asserções observadas nos primeiros segundos.
Binário Debug: SHA-256
`7832b0f3e38f864fd6448caf1a360badd2300d8d28637058ed86e18ce47fad89`.
O processo permanece aberto para teste do autor. Escuta musical e inspeção de
todos os painéis continuam pendentes.

## Desenvolvimento entre passagens e inventário conceitual — 30/10/2026

**Objetivo:** começar a transformar a repetição da forma em desenvolvimento
musical verificável. `src/core/PassageDevelopment.hpp` define uma articulação
determinística por seed, passagem, voz e posição: a primeira passagem estabelece
o material; a segunda alterna chamado/resposta; a terceira abre espaços; a
quarta desloca o peso para as outras vozes; a quinta retorna. O ciclo reinicia
com variação de posição nas passagens posteriores. O áudio das vozes e o baixo
consultam essa mesma regra em cada ataque; a partitura base, notas e tempos
permanecem intactos. Sem seed, a articulação extra fica desligada. O rodapé
indica `P1`, `P2` etc. em repetição contínua. A mudança começa após os sete
ciclos da primeira passagem (cerca de 56 s em 4/4 a 120 BPM), e não deve ser
confundida com uma nova composição completa.

| Conceito recebido | Estado executável hoje | Falta para uso composicional profundo |
| --- | --- | --- |
| Polirritmia e fase | Razões por voz, fase, swing e deslocamento de V2 alteram eventos | Frases com memória, pausas dirigidas, contraponto entre grupos e baixo |
| Fórmulas, campos e micropatterning | Lei I pondera ataques; superfície XY filtra eventos | Editor de função/campo, mapeamentos escolhidos e limites audíveis |
| Geometria, bilhar, Fourier | Trajetórias animadas; V1 pode usar percurso geométrico | Contornos editáveis, reflexão em outras formas, curvas que afetem mais vozes e timbre |
| Física de materiais e acústica | Ressonador modal, amortecimento e seis materiais de baixo | Modelos mensuráveis de corpos/salas, força/ruptura e ligação de parâmetros físicos à escuta |
| Cálculo diferencial/integral, matrizes e fractais | Conceitos e fórmulas no README; alguns cálculos de área/curva | Operadores estáveis de velocidade, arco, deformação e realimentação com teste musical |
| Ecologia, corpo, intenção e confiança | Modos de relação, dinâmica e intenção afetam acentos; desenvolvimento novo entre passagens | Memória de gesto, resposta adaptativa, microtiming causal, decisão sob incerteza e espaço de interpretação |
| Timbre e qualidade de saída | Quatro perfis espectrais de voz; baixo com ADSR/VCF/LFO | Paleta por seed/voz, ganho calibrado, saída técnica, teste auditivo A/B e sem saturação indesejada |

O gráfico das vozes e o campo XY agora consultam a mesma função para dimensionar
os marcadores de ataque e desenhar um `x` nos espaços. É uma prévia da
articulação da agenda-base; eventos modificados por trajetória geométrica ou
por controles de ritmo durante a execução ainda exigem uma fonte única de
eventos publicados para equivalência exata. A partitura salva continua sem a
articulação de cada passagem.

**Validação técnica desta etapa:** build JUCE e três testes focados passaram.
`tests/variant_transport.cpp` verifica a mudança no áudio renderizado entre a
primeira e a segunda passagem. O build após a alteração visual também passou.
**Limites:** ainda não há escuta da nova versão no aplicativo aberto; a
qualidade da prévia visual precisa de inspeção no JUCE; os sete ciclos
continuam usando a mesma agenda-base; a
qualidade de som e o layout não foram certificados. Próximo foco: expor no
gráfico/partitura o plano efetivamente tocado e criar frases e respostas com
memória, depois calibrar a cadeia de saída e reorganizar o painel de baixo.

## Reavaliação do JUCE: composição, som e interface — 30/10/2026

O critério de trabalho deixa de ser porcentagem de paridade web/JUCE. O JUCE é
o instrumento prioritário. A documentação conceitual já define o contrato
`conceito → modelo → evento → mapeamento → realização → escuta`, mas o app
executa sobretudo uma agenda fixa de sete ciclos. Repeti-la não desenvolve
motivos nem responde às decisões musicais feitas durante a execução.

**Soluções por ordem de dependência e aceite:**

1. **Saída audível confiável:** medir separadamente vozes, baixo e soma antes
   do master; rever o ganho padrão de +16,5 dB e a dupla saturação após
   calibrar stems; testar a saída técnica prevista em
   `architecture/SAIDA_AUDIO_COMUM.md`. Examinar `RM-OUTPUT-STAGE` do Modular
   como fonte interna, sem dependência de build entre projetos. Aceite: sem
   cliques em ataques/transporte, sem não-finitos, teto e equilíbrio medidos,
   escuta A/B real de três seeds e sessões com 2/9/12 vozes.
2. **Compositor por passagem e frase:** separar seed-base, estado da frase,
   escolhas do músico e índice da passagem. Preparar planos imutáveis com
   memória de motivo, lacunas, acentos, direção, repouso, operações de
   inversão/recombinação e relação baixo–vozes; publicar na fronteira musical
   sem cálculo pesado no callback. Consultar `CompositionIdentity` e
   `GrooveDialogue` do Studio, `PatchSeed` do Modular e código rítmico do
   Antitotem; adaptar só contratos com proveniência/licença. Aceite: mesma
   seed + mesmas ações reproduzem a peça, passagens consecutivas diferem de
   modo reconhecível, e baixo responde à forma sem perder a frase editada.
3. **Gráfico como instrumento:** áudio, curva, eventos, partitura e traço
   consultam o mesmo plano. Cada animação informa quais parâmetros musicais
   lê ou escreve. `Áreas` permanece análise até haver mapeamento escolhido;
   `Curvas` pode alimentar V1. Aceite: comparar cálculo, evento e escuta na
   mesma janela temporal, com fórmula e limites visíveis.
4. **Superfície de trabalho:** separar preparação, composição, performance e
   síntese. Em 1080×800, o gráfico atual tem 202 px para até 12 vozes
   (≈17 px por linha), insuficiente para edição; criar visão geral e foco de
   vozes com altura útil, mantendo acesso a todas. O painel de baixo reúne
   treze parâmetros de síntese mais a partitura numa janela de 720 px; dividir
   por tarefa, preservar controles e impedir sobreposição por teste de bounds
   em estados cheio/compacto. Aceite: alcançar os controles sem ocultação e
   editar ouvindo/verificando a consequência.

Uma animação não será eliminada só por ser complexa; será mantida quando sua
relação composicional ou analítica for explícita. A marca aprovada é o
lettering incorporado no JUCE, mas a identidade gráfica completa não está
fechada pela existência desse arquivo. Não declarar a interface ou o som
validados por compilação ou por percentuais.

**Fontes internas efetivamente lidas nesta reavaliação:**

- [OutputStage do Modular](../RASGO_MODULAR/src/dsp/OutputStage.hpp): ponto de partida para proteção de
  saída e telemetria. Exige adaptação local e ensaio de latência, true peak,
  bypass e carga; não substituir o master do Batruc por cópia direta.
- [CompositionIdentity do Studio](../RASGO_SYNTH_STUDIO/rasgo-synth-core/src/engine/CompositionIdentity.hpp): aproveitável como método
  de genes perceptíveis e streams independentes por decisão, preservando
  outras escolhas quando um gene novo for acrescentado. A taxonomia de
  escala/gênero do Studio não deve ser imposta ao Batruc.
- [GrooveDialogue do Studio](../RASGO_SYNTH_STUDIO/rasgo-synth-core/src/engine/GrooveDialogue.hpp): estudar memória de frase,
  mudança de família por seção e resposta limitada. As máscaras de bateria
  são específicas do Studio e não constituem modelo cultural a copiar.
- [PatchSeed do Modular](../RASGO_MODULAR/apps/panel/PatchSeed.hpp): aproveitar a garantia de que toda
  rota gerada chega a uma saída audível; o Batruc não precisa da gramática de
  cabos de um rack para sua topologia atual.
- [CmosVoice](../ANTITOTEM/src/core/CmosVoice.h) e
  [MaterialFilter](../ANTITOTEM/src/core/MaterialFilter.h) do Antitotem: candidatos de
  técnicas de oscilação/filtro; a estética CMOS do Antitotem não é preset de
  timbre do Batruc. Auditar licença, commit e API antes de portar.

## Correção JUCE: seed visível, estado do baixo e ataques — 30/10/2026

**Objetivo:** responder a quatro falhas percebidas na sessão real: falta de botão
para uma nova seed, variação desativada após edição, rótulo enganoso do botão
do baixo e cliques sincronizados
com o cruzamento do ponto turquesa no gráfico.

- `src/app/Main.cpp`: botão `Nova seed` no espaço entre o lettering e o seletor
  de vozes; gera oito dígitos hexadecimais e aplica a composição, incluindo o
  baixo. O botão do baixo agora mostra a ação seguinte: `Desativar baixo`
  quando ligado e `Ativar baixo` quando desligado, inclusive após troca de
  idioma, abertura do painel e carregamento de cena. Edições manuais deixam
  a seed de origem visível e fazem a próxima variação partir do estado atual;
  uma linha de baixo editada manualmente não é substituída pelo gerador da
  mesma seed. Cena schema 26 registra esse estado e lê esquemas anteriores.
- O gráfico principal foi ampliado de 44% para 56% da largura da janela;
  o campo geométrico continua ao lado. Isso melhora a leitura horizontal,
  mas não resolve as 12 faixas comprimidas no perfil 1080×800; a visão geral
  com foco vertical continua pendente na reavaliação acima.
- `src/core/CompositionCore.hpp/.cpp`: a excitação modal de cada ataque é
  distribuída por 2 ms e a entrada do ruído é suavizada. Antes, a amplitude
  mudava instantaneamente na fase em curso, criando descontinuidade no exato
  evento que o gráfico assinala. O caminho permanece sem alocação no callback.
- `tests/variant_transport.cpp`: regressão para limitar o primeiro salto de
  amostra no disparo de um evento.

**Evidência:** a instância anterior produziu saltos de até 0,133 no fluxo
capturado; os maiores ocorreram em intervalos de cerca de 0,802 s, coerentes
com ataques. O render offline atualizado da seed `5EED2027` teve pico de
0,204 e maior salto adjacente de 0,0153 em 56 s. Esses dois contextos têm
configurações diferentes e não constituem uma comparação A/B de timbre. O
teste `batruc_variant_transport` passou; o aplicativo JUCE recompilou.
**Limite:** o processo que já estava aberto continua executando código antigo;
precisa ser reiniciado para escutar e ver estas alterações. A escuta do autor
após reinício e a medição do master em sessões densas ainda são necessárias.
O novo comportamento da variação e a cena schema 26 compilaram, mas ainda
precisam de validação funcional na interface JUCE aberta.
Binário Debug de 20h40 (horário local): SHA-256
`fa0e37c6d3f2679ace4ffa162fafe3cf4921e02d881208047cabf423a5f10d80`.
Três testes focados passaram: `batruc_variant_transport`,
`batruc_bass_audio_response` e `batruc_mixer_routing`.
O pico medido de 0,35 na sessão capturada não indica clipping digital, embora
o autor ouça saturação; a cadeia atual usa dois estágios não lineares e trim
das vozes de +16,5 dB por padrão. Próximo trabalho: medir buses pré-master,
recalibrar headroom/equilíbrio com stems e substituir a proteção final por
uma saída técnica conforme `architecture/SAIDA_AUDIO_COMUM.md`.

## Diagnóstico arquitetural do JUCE — 30/10/2026

**Objetivo:** explicar a baixa variação musical, o desaparecimento do baixo e
o custo excessivo de mudanças pequenas antes de acrescentar módulos.

- `src/app/Main.cpp` concentra 8.741 linhas: interface, estado da cena,
  construção de planos, baixo, mixer, gravação e callback de áudio. Um ajuste
  musical atravessa funções distantes no mesmo componente; os testes do core
  não cobrem automaticamente o fluxo real dos widgets até a saída.
- Vinte e sete caminhos de edição chamavam `clearSeedAfterManualEdit()`, que
  removia a seed e desativava a variação. A função foi corrigida nesta sessão:
  a seed permanece como origem e os ajustes atuais viram nova base da próxima
  variação. O resultado funcional ainda requer teste na interface reaberta.
- A seed atual seleciona uma receita fixa. A forma de sete ciclos se repete
  sem uma nova realização por volta. `GenerativeOperations` oferece inversão
  e recombinação, mas essas funções são usadas apenas nos testes, não no app.
  `algorithmicNextSeed()` também existe no core sem controle nativo que o use.
- O app prepara 9.696 variantes de receita/plano na inicialização. Essa
  arquitetura favorece a troca entre combinações discretas, mas não é um
  modelo de evolução composicional de frases, nem torna explícita a origem de
  cada transformação. Não há evidência aqui de que seja gargalo de CPU; o
  problema demonstrado é a falta de uma camada composicional entre receita e
  execução.
- A geração de baixo possui caminho de preparo, publicação por slot e render
  próprios; o mute/solo/enable condiciona a execução no callback. Na sessão
  observada, o baixo voltou a ser ouvido após C5, ciclo de pausa formal. Falta
  um diagnóstico integrado e visível: contagem de eventos, razão de silêncio,
  RMS do baixo antes do master e confirmação do snapshot ativo.
- O render offline monta uma receita própria e parcialmente fixa; não lê o
  estado completo do app. Sua saída não valida, sozinha, a configuração que o
  músico está ouvindo. O master soma vozes e baixo e aplica `tanh`, com trim
  padrão de +16,5 dB no barramento das vozes; o autor relata saturação, que
  ainda precisa de medição na configuração real.

**Direção JUCE:** (1) manter seed e overrides no estado da composição;
(2) preparar um plano imutável por frase/volta com seed-base e índice da volta,
incluindo baixo e pausas; (3) publicar o plano na fronteira musical com fila
limitada, deixando o callback sem alocação; (4) alimentar áudio, gráficos,
partitura e gravação do mesmo snapshot; (5) expor medidores e motivo de mute;
(6) calibrar headroom, proteção e equilíbrio por stems. Primeira entrega:
restaurar a continuidade da seed durante a edição e tornar o estado do baixo
observável no JUCE. A evolução musical vem depois da confiabilidade do fluxo.

**Validação desta etapa:** leitura de código e documentação, captura local da
janela aberta e fluxo de áudio do processo anterior. O novo binário foi
compilado, mas a escuta direta dele ainda depende de reabrir o aplicativo.
Nenhuma porcentagem de paridade foi alterada.

## Layout JUCE: separar controles, órbitas e gráficos — 30/10/2026

A captura do autor mostrou colisões na tela principal: os controles de variação,
compasso e swing ocupavam `y=171–197`, enquanto órbitas compactas começavam em
`y=182`; a equação da lei de intensidade também era desenhada atrás dos
seletores de intenção/dinâmica. Recalculei a altura das órbitas conforme o raio
real e reservei faixas distintas para órbitas, controles de composição e
gráficos. A fórmula agora fica à direita dos seletores. O limite inferior do
gráfico também foi elevado para abrir 19 px entre o controle de foco e a linha
dos sete ciclos.

O app JUCE recompilou em 30/10/2026. O cálculo dos bounds garante 8 px entre
controles e órbitas, 8 px entre os rótulos inferiores das órbitas e a faixa de
lei/intenção, e 12 px antes do gráfico; para uma janela de 1080 px de altura, o
controle de foco termina 19 px antes da forma. **A captura pós-alteração não
foi validada:** este ambiente não tem acesso ao display gráfico; a tentativa
de abrir a janela terminou com falha de display/asserção JUCE. A compilação não
substitui essa inspeção visual. Revisar também o perfil compacto antes de
considerar o layout fechado.

## Auditoria JUCE: repetição, baixo e saturação — 30/10/2026

O motor não evolui continuamente enquanto toca: a seed prepara uma composição
determinística de sete ciclos, e o transporte a repete sem criar uma nova
realização a cada volta. Aos 120 BPM em 4/4, a forma de 56 s volta ao início;
as mudanças entre ciclos são formais e pré-calculadas. Portanto, ouvir a mesma
passagem por vários minutos é o comportamento atual, não evidência de que a
seed falhou. Falta composição generativa evolutiva, limitada e reproduzível por
seed + número da volta.

O baixo tem pausas formais em C1 e C5. Fora delas, o core deve produzir som.
O roteamento JUCE também pode silenciá-lo se o baixo estiver desativado/mutado
ou se uma voz estiver em solo sem o baixo em solo. Nesta auditoria, recompus o
executável porque `CompositionCore.cpp` era mais novo que o binário anterior.
O build `batruc_app` terminou; `batruc_seeded_recipe`,
`batruc_bass_composition` e `batruc_bass_audio_response` passaram (3/3). Isso
confirma o cálculo e a resposta de áudio isolada, mas não confirma o estado do
mixer nem o som ouvido na sessão ao vivo; se o baixo continuar ausente em C2,
C3, C4, C6 e C7 no binário atual, ainda há um defeito de integração a localizar.

O master aplica `tanh` à soma final. Isso evita estouro duro acima de 0 dBFS,
mas satura quando várias vozes acumulam nível e pode soar como clipping. O
barramento das fórmulas tem ganho padrão de +16,5 dB antes da soma; falta medir
headroom e picos do master e substituir a saturação audível por ganho de soma
adequado e proteção transparente, se confirmada na medição.

**Próximo passo JUCE:** abrir o executável recompilado e observar o medidor do
baixo nas fases formais; registrar o estado de mute/solo/enable e o nível do
barramento. Depois medir true peak e redução do master com todas as vozes. Só
então implementar mutação composicional por volta, preservando controle do
compositor e reprodução determinística.

## Nova referência matemática: Vygodski — 29/10/2026

O autor encontrou numa caixa de livros doados uma cópia de *Aide-mémoire de
mathématiques supérieures*, de Marc Vygodski. Registrei a referência no
[README](README.md#vygodski--geometria-séries-e-equações-diferenciais): edição
francesa Mir, Moscou, 1973, tradução de Guy Der-Mégréditchian, 861 páginas
segundo catálogo bibliográfico. O escopo catalogado inclui geometria analítica,
cálculo, curvas, séries/Fourier, integrais múltiplas e equações diferenciais.

Aplicação de estudo para o Batruc: curvas e trajetórias, aproximações de
Fourier, áreas/volumes e movimento por equações diferenciais. A obra serve de
base matemática; não fornece, por si, um modelo de composição musical ou
acústica. Estado físico e localização da cópia ainda não inventariados.

O autor também destacou o cálculo diferencial. Registrei no README derivada,
aceleração, velocidade do traço, curvatura e oscilador amortecido como
possibilidades para movimento, densidade, acento e articulação, sempre com
normalização e limites. O mapeamento musical é hipótese autoral. Para molas e
outros sistemas dinâmicos, estabilidade do integrador precisa ser validada
antes de qualquer execução em tempo real.

Em seguida, o autor forneceu o link direto para o scan integral francês. O
arquivo foi acessado e conferido: 863 páginas PDF, com camada de texto
pesquisável. A diferença para o registro bibliográfico de 861 páginas parece
vir das folhas preliminares/editoriais incluídas no scan; não foi feita
colação física folha a folha. Inspecionei visualmente uma amostra de figuras:
regra do polígono para soma vetorial (p. 126), construção de curvas rolantes
(cicloide, epicicloide/hipocicloide), espirais, catenária/tractriz e secções
planas de superfícies (cone, paraboloide hiperbólico e superfícies de
revolução). As imagens são esquemas explicativos estáticos, não animações nem
material composicional pronto.

Registrei no README uma proposta de visualização autoral para o Batruc:
construção animada de polígonos e seus perímetro/área; plano móvel cortando
superfícies e expondo a curva de intersecção; e rolagem geradora de cicloides.
Esses desenhos podem alimentar regras de eventos por relações geométricas,
mas a ligação sonora ainda precisa ser desenhada e testada. Não copiei nem
incorporei imagens do livro ao projeto.

Implementei o primeiro recorte de curvas rolantes em ambos os motores:
epicicloide/nefróide com `R=2r` e hipocicloide/deltóide com `R=3r`. As funções
paramétricas, os nomes/formas selecionáveis, o traço em execução e a roda
geradora estão ligados ao mesmo seletor de geometria; o modo de fonte
geométrica pode encaminhar `y(t)` para os eventos de V1. Salvar/abrir cenas
JUCE aceita os novos índices sem alterar os IDs antigos. Os parâmetros `R/r`
continuam fixos; ainda falta conferir visualmente e ouvir a influência na V1.
O `node --check` passou em `score.js`, `app.js`, `interface-i18n.js` e
`guide.js`; `GeometryCurves.cpp` passou por compilação sintática isolada; o
alvo nativo compilou com `timeout 240 cmake --build
.local-build/native/linux-debug-juce --target batruc_app -j1`. Não executei a
suíte automatizada nem abri a interface; a resposta visual/auditiva das duas
formas ainda precisa ser conferida em execução. Próximo: comparação focada dos
dois formatos e então uma razão rolante configurável ou cortes de superfícies.

O autor acrescentou o conceito de **tempo maleável**. Registrei uma proposta
de warping de fase `τ(u)` por integral de uma velocidade positiva normalizada:
preserva a ordem, o início e o fim do ciclo, ao mesmo tempo que derivada e
aceleração da transformação expressam expansão/compressão temporal. O README
distingue essa deformação de frase do swing local e prevê modo compartilhado
ou mapas autônomos por voz. Próxima etapa de código: prototipar um único campo
de rubato determinístico, visualizá-lo e comparar a agenda antes de acoplar
forças, vozes ou fontes caóticas.

O autor destacou a seção final, “Some Remarkable Curves”. Registrei um recorte
de estudo no README: cicloide, epicicloide/hipocicloide, lemniscata, espirais,
catenária e tractriz, com fórmulas usuais e hipóteses de mapeamento para
percurso, arco, curvatura, simetria e acentos. Prioridade de protótipo: cicloide
e epicicloide; não misturar o movimento geométrico com uma alegação de
simulação física até especificar forças e condições de contorno.

## Validação funcional iniciada, síntese ainda divergente — 28/10/2026

Fiz render A/B funcional, em vez de encerrar a checagem em inspeção estática:
duas seeds, 56 s, 48 kHz, HTML OfflineAudioContext contra core JUCE. As agendas
coincidem em 77/77 e 20/20 eventos, inclusive baixo; todos os 10 testes CTest
passaram após corrigir o limite de duração no fim do ciclo. O comparador de
áudio também expôs diferenças de envelope, RMS e silêncio entre motores. Os
WAVs/JSON de evidência estão em
`.local-build/parity-audio/parity-audit-fixed-20261028/` e
`.local-build/parity-audio/parity-audit-second-seed-20261028/`.

Não fiz escuta subjetiva. A omissão anterior foi operacional: eu havia parado
em código/fixtures embora o render A/B local estivesse disponível. Agora a
validação funcional foi executada; cliques, equilíbrio percebido, caráter e
intenção ainda requerem audição humana. A nota global permanece 69%, sem
reponderar a métrica com base em somente duas seeds.

Arquivos alterados nesta etapa: `prototype/score.js`,
`tests/seeded_bass_fixture.cpp`, `tools/render_compare.sh`,
`PARIDADE_HTML_JUCE.md` e este documento. `tools/render_compare.sh` agora passa
ao comparador o manifesto HTML que o renderer realmente gera. Não alterei o
JUCE nem os parâmetros sonoros.

**Próximo passo:** criar verificações funcionais dedicadas para mute/solo,
gravação e abrir/salvar; em seguida medir a resposta de ADSR/VCF/matéria por
voz e comparar stems. Não aumentar a porcentagem até esses fluxos e a escuta
crítica serem aceitos.

### Diagnóstico de timbre, stems e início A/B — 28/10/2026

O autor escutou os WAVs `5EED2028` e confirmou que os timbres diferem. O render
HTML offline também adicionava 70 ms ao início; removi o atraso do OfflineAudioContext
e confirmei onset em 0 s para ambos na seed `5EED2031`, com 302/302 eventos
pareados. O player em tempo real preserva seu agendamento antecipado.

`tools/render_compare.sh` agora aceita `--stems` e grava comparações separadas
do mix, vozes e baixo. Em duas seeds, as vozes tiveram correlação de envelope
0,507–0,527; o baixo, 0,984–0,993. As vozes são sintetizadas de maneiras
diferentes: Web Audio usa oscilador com varredura de frequência e envelope por
evento; JUCE usa oito modos ressonantes persistentes com decay modal. O baixo
acompanha bem os ataques, mas seus níveis RMS também diferem. Não alterei os
timbres ou ganhos ainda, pois são diferenças de arquitetura e os WAVs servem
como referência para definir um modo de compatibilidade sem remover o caráter
modal do JUCE.

Adicionei teste de resposta de áudio JUCE: ataque, decay, sustain, release,
cutoff, ressonância e matéria alteram o sinal. A suíte completa passou 11/11.
O teste cobre o render core, não comprova por si só que cada widget de interface
altera o som na reprodução ao vivo.

Arquivos acrescentados/alterados nesta etapa: `prototype/app.js`,
`tools/render_compare.sh`, `CMakeLists.txt`, `tests/bass_audio_response.cpp`,
`PARIDADE_HTML_JUCE.md` e este registro. Artefatos preservados em
`.local-build/parity-audio/stem-audit-20261028/` e
`.local-build/parity-audio/stem-audit-offset-fixed-20261028/`.

**Próximo passo:** validar controles mute/solo/pan em execução e os fluxos de
salvar, abrir, apagar e gravar; depois definir e implementar síntese de voz
compatível entre HTML e JUCE, e repetir o teste de escuta A/B.

### Smoke funcional do mixer HTML — 28/10/2026

Acrescentei `--mixer-smoke` ao renderer headless. Com seed `5EED2027` e a
composição real de 56 s, renderizei o mix após acionar mute V1, solo V1, solo
baixo e mute master pelo DOM. Os cinco checks passaram: baseline audível
(RMS 0,01041), mute V1 altera o mix (0,00897), solo V1 isola o canal
(0,00539), solo baixo permanece audível (0,00730) e mute master zera a saída.
Isso valida os botões e o roteamento Web Audio do mixer da página, incluindo
solo sem deixar o baixo no master.

**Limite:** o mesmo teste headless não cobre ainda a janela de mixer JUCE, pan,
trim, sensor RMS ou automação de equilíbrio. O ganho nativo segue verificável
no callback, mas precisa de um harness de host com saída capturada.

Arquivos alterados: `tools/render_web.cjs` (novo smoke test) e os dois
documentos de paridade/continuidade. Artefatos numéricos foram impressos pelo
smoke test; não foi gravado WAV adicional.

**Próximo passo:** montar captura headless equivalente para o mixer JUCE e
validar pan/master; então cobrir persistência e gravação nas duas interfaces.

## Idiomas da interface e caderno web — 28/10/2026

### Conteúdo didático da Lei I alinhado — 28/10/2026

Ampliei o tópico “Som e lei I” do caderno web nos quatro idiomas para nomear as
oito funções implementadas, explicar `Iλ=(1−λ)+λf(x,y)` e apresentar
`U=(n₁²x²+n₂²y²)/(n₁²+n₂²)`, `K=1−U`. O texto esclarece que x/y são
deslocamentos normalizados de um modelo abstrato de dois osciladores e que a
Lei I calcula intensidade de ataque, não altura nem energia acústica medida.
O tópico de geometria também esclarece que o Fourier usa soma finita, que H
altera a aproximação e o erro RMS, e que a opção de fonte y altera eventos de
V1. Isso melhora a correspondência didática sem atribuir à maquete uma
simulação física que ela não executa.

Arquivos alterados: `prototype/guide.js`, `prototype/README.md`,
`PARIDADE_HTML_JUCE.md` e este registro. Não alterei algoritmo, parâmetro ou
saída de áudio.

**Validação:** `node --check prototype/guide.js` passou; não rodei a suíte nem
revisei a leitura visual do modal. A estimativa global permanece em 69,0%.
Próximo passo: conferir a correspondência conteúdo/ações nos seis tópicos,
marcando diferenças reais entre controles web e JUCE antes de declarar essa
frente fechada.

A página agora tem um seletor global PT/EN/ES/FR sincronizado com o seletor do
caderno. Além dos rótulos estáticos, a localização cobre mensagens variáveis do
arquivo local, confirmação de exclusão, leitura RMS, estados de solo/mute,
entrada e pausa das vozes, status de reprodução/gravação, traçado, equações
geométricas, contagem de termos de Fourier, fases da forma, comprimento do
ciclo, resumo da seed e instruções do editor do baixo. Em telas estreitas, os
controles geométricos passam a uma coluna e os cabeçalhos do desenho podem
quebrar linha. O tradutor segue os nós recriados pelo app e os atributos `title`,
`aria-label` e `placeholder`; o diálogo de exclusão consulta a mesma função
pública de tradução. O rodapé também apresenta nos quatro idiomas o escopo e as
limitações da maquete e os créditos. O caderno mantém os seis tópicos e o
conteúdo Sobre nos quatro idiomas. A preferência é armazenada localmente.

Arquivos alterados nesta etapa: `prototype/interface-i18n.js`,
`prototype/app.js` (confirmação traduzida antes de apagar um cálculo),
`prototype/style.css` (controles geométricos em uma coluna no celular e melhor
quebra das ferramentas do caderno), `PARIDADE_HTML_JUCE.md`,
`prototype/README.md` e este registro. A etapa anterior alterou
`prototype/index.html` e `prototype/guide.js`. Não alterei o conteúdo do motor
JUCE; a página mantém síntese e cálculos inalterados.

**Validação:** `node --check` passou nos três scripts, o HTML foi lido pelo
parser da biblioteca padrão e as chaves CSS estão balanceadas; não rodei a
suíte nem fiz revisão visual no navegador. Permanecem revisão humana das
traduções e conferência visual de títulos longos, estados e caderno nos quatro
idiomas. Detalhes técnicos de exceções do navegador e nomes de arquivos/pastas
fornecidos pelo usuário continuam literais de propósito. A estimativa global
permanece 69,0%; a localização da interface, isoladamente, não prova paridade
composicional ou sonora.

**Próximo passo:** revisar o seletor, os modais e os textos longos em janela
estreita e larga; depois fechar o inventário dos conteúdos que ainda variam
entre HTML e JUCE.

## Contrato de mixer alinhado no A/B offline — 28/10/2026

O render JUCE agora replica o caminho de saída do app: trim de vozes `+16,5
dB`, master `0,72`, canais de voz em panorama distribuído, níveis de canal em
1, baixo em `0,20`, cauda modal `0,35` e fundamental `0,55`. O render HTML
aplica os mesmos valores de master, trim, nível e panorama durante a captura.
O comparador inclui esses campos na receita e confirmou 28 campos coincidentes.

Com mixer e defaults nativos equiparados, a seed `5EED2027` mede fullmix RMS
0,01041 no HTML e 0,01766 no JUCE; picos 0,5302 e 0,2055; silêncio 90,33% e
51,46%; correlação do envelope 0,551. Os stems de voz medem RMS 0,00687 no HTML
e 0,02442 no JUCE, com correlação 0,478. O baixo mede 0,00657 e 0,00311,
respectivamente, e a correlação de envelope é 0,989. Ambos os fullmixes não
clipam. O mixer por si só não explica as diferenças: as sínteses e as caudas
modais continuam diferentes.

O renderer de referência usa agora os defaults reais do JUCE (`decay=0,35`,
`fundamental=0,55`), e a CLI permite testar escalas de cauda entre 0,15 e 1,5
sem mudar os defaults do instrumento. Um render diagnóstico em `0,15` chegou a
RMS fullmix 0,01162 e correlação 0,660; as vozes ficaram em RMS 0,01592 com
correlação 0,618. Não apliquei esse valor à interface: a melhoria métrica
precisa ser confrontada com a intenção sonora.

Ampliei a varredura diagnóstica para `0,20`, `0,25` e `0,30`, sem tocar no
instrumento. As correlações de envelope fullmix foram 0,630, 0,601 e 0,575;
para o stem de vozes, 0,573, 0,536 e 0,505. A faixa `0,15` segue como melhor
candidato medido, mas ainda deixa o stem de vozes com RMS 0,01592 contra
0,00687 no HTML e silêncio de 74,11% contra 91,66%. Logo, a cauda global não
resolve a diferença por si só. A leitura do código aponta uma diferença de
modelo: a web dispara osciladores com varredura de frequência e release por
evento; o JUCE excita modos ressonantes persistentes, acumula amplitudes e
aplica decaimento modal. A próxima fatia deve alinhar a resposta por evento ou
definir uma camada de síntese equivalente antes de ajustar a cauda global.
Os renders e relatórios desta varredura estão em
`.local-build/parity-audio/decay-sweep-20261028/`; os arquivos anteriores foram
preservados. Preflight antes do áudio: 13 GiB disponíveis, swap usada em 12 MiB
de 2 GiB, 982 GiB livres no volume e nenhum render concorrente. Os três renders
terminaram; não rodei testes nem fiz escuta subjetiva. Estimativa de paridade
inalterada em 69,0%.

Também acrescentei proteção contra sobrescrita dos WAVs, manifestos e stems
existentes no renderer JUCE. Artefatos e comparações estão em
`.local-build/parity-audio/mixer-aligned-20261028/`,
`mixer-native-default-20261028/` e `decay-min-candidate-20261028/`.

**Validação:** `batruc_offline_render` compilou; `node --check` e parse AST de
`compare_audio.py` passaram. Renderizei e comparei mix e stems; os eventos
seguem 47/47, com erro de ataque máximo de 0,05 ms e duas caudas finais até
8,54 ms. Não rodei a suíte nem fiz escuta subjetiva. Estimativa permanece
69,0%.

**Próximo passo:** aproximar no JUCE o envelope e a varredura de frequência por
evento que a web usa nas vozes matemáticas; comparar envelopes isolados por voz
e só depois reavaliar a cauda modal. A escuta dirigida de `0,15` e `0,35`
continua necessária antes de qualquer alteração estética. Não alterar defaults
nem elevar paridade somente por RMS.

## Corrigido o acúmulo de blocos no baixo offline — 28/10/2026

A comparação por stems revelou um bug no renderer de diagnóstico JUCE: o
`BassAudioRenderer::process()` soma no buffer de saída, mas o renderizador
offline reutilizava `bassLeft`/`bassRight` sem zerá-los entre blocos. O áudio
antigo acumulava blocos anteriores, elevando o RMS do stem de baixo a 0,497 e
causando 5,39% de amostras clipadas. Agora os buffers são limpos antes de cada
bloco, como já ocorre no callback do host. Não alterei o ganho musical do baixo.

Na primeira comparação após a correção da seed `5EED2027`, o baixo JUCE mede RMS 0,00311, sem
clipping, contra 0,00657 no HTML; a correlação de envelope é 0,989. O nível
continua diferente, mas a dinâmica temporal acompanha bem. As vozes medem RMS
0,01242 no JUCE e 0,00687 no HTML, com correlação 0,307 e caudas/silêncios
diferentes. O fullmix JUCE caiu de RMS 0,41847 (render defeituoso) para 0,01278;
o HTML fica em 0,01682. A correlação do fullmix é 0,478, então timbre, envelope,
panorama, master e escuta dirigida seguem pendentes. Comparações por stem são
diagnósticas. Essas medições usavam um contrato de mixer e defaults de cauda
incompletos; foram substituídas pela comparação mais recente acima.

Adicionei exportação opcional de stems no renderer JUCE (`--stems`) e no
renderizador HTML (`--stems`), além de isolar vozes/baixo no gancho offline. Os
arquivos ficam no lote `stem-isolation-20261028/`; os WAVs anteriores foram
preservados. O encerramento do Chromium agora sinaliza o grupo inteiro do
processo antes de remover o perfil. Uma renderização headless autorizada
terminou com status zero e não deixou perfil novo; os perfis residuais anteriores
foram removidos depois de confirmar que não havia processos Chromium ativos.

**Arquivos afetados:** `src/tools/offline_render.cpp`, `prototype/app.js`,
`tools/render_web.cjs`, este registro e `PARIDADE_HTML_JUCE.md`.

**Validação:** o alvo `batruc_offline_render` compilou; `node --check` passou
para `app.js` e `render_web.cjs`. Renderizei fullmix e stems dos dois motores e
comparei RMS, pico, clipping, silêncio, ataque e envelope. A receita segue igual
nos 22 campos; 47 eventos continuam pareados, com duas durações finais ainda
divergentes até 8,54 ms. Não rodei a suíte de testes nem fiz escuta subjetiva.

**Próximo passo daquele registro:** equalizar mixer e defaults de cauda, trabalho
concluído e documentado na seção mais recente acima.

## Duração seeded do baixo alinhada — 27/10/2026

O core JUCE agora aplica à agenda ligada às vozes as mesmas leis de duração
usadas pela página: duração manual da célula, Fibonacci e potência geométrica
arredondada. Todas são limitadas à borda do compasso. A interface preserva a
duração da agenda seeded em vez de recalculá-la depois da compactação de notas;
isso evita deslocar o ordinal das fórmulas. O modo geométrico cobre as razões
1,20, 1,50, φ≈1,618 e 2,00.

A matriz de agendas cruzadas cresceu de 8.592 para 9.168. Os 576 novos casos
cruzam quatro seeds, duas variações, quatro frases, três relações do baixo e
seis escolhas de lei/razão (manual, Fibonacci e quatro razões geométricas).
Todos os eventos coincidiram em quantidade, ciclo, ataque, altura MIDI,
duração final e intensidade.

Arquivos afetados: `src/core/BassComposition.hpp/.cpp`, `src/app/Main.cpp`,
`tests/seeded_bass_fixture.cpp`, `tests/compare_seeded_voice_bass.cjs`,
`tools/test_seeded_voice_bass_parity.sh`, `tools/test_seeded_bass_parity.sh`,
`PARIDADE_HTML_JUCE.md` e este registro.

**Validação:** `batruc_app`, `batruc_seeded_bass_fixture` e
`batruc_bass_composition_test` compilaram. CTest seletivo passou 2/2; a fixture
confirmou 9.168 agendas. `node --check` e `node prototype/test-score.cjs`
passaram; os scripts shell passaram em `sh -n`. Não abri interface, renderizei
áudio ou fiz escuta A/B.

**Próximo passo:** medir a resposta de cada parâmetro e comparar renders. O
pool JUCE tem limite fixo de oito vozes, enquanto a web não limita as notas
sobrepostas.

## Release de amplitude e material pluck alinhados — 27/10/2026

O release de amplitude do ADSR JUCE agora é exponencial, como no HTML. O
coeficiente é calculado ao iniciar o release e multiplicado por amostra, sem
executar `pow()` no callback. O release do envelope de filtro segue linear, em
conformidade com a rampa HTML. Para o material pluck, o nível agora decai até
0,12 durante o intervalo da nota (12–120 ms), em vez de usar sempre 120 ms.

Arquivos alterados: `src/core/BassAudio.hpp/.cpp`, `PARIDADE_HTML_JUCE.md` e
este documento. `batruc_app` compilou com sucesso; não rodei testes, render ou
escuta A/B. Naquele ponto o JUCE ainda era monofônico; a seção seguinte registra
a implementação posterior do pool simultâneo.

**Próximo passo:** criar uma medição reproduzível que varie ADSR de amplitude,
ADSR de filtro, cutoff, ressonância e matéria sonora; comparar a resposta dos
dois motores e decidir se o JUCE precisa manter caudas de nota independentes.

## Caudas simultâneas no baixo JUCE — 27/10/2026

Substituí o único estado monofônico por um pool fixo de oito vozes. Cada nota
mantém fase dos osciladores, filtro, gates e envelopes próprios, então uma nova
nota não reinicia a cauda de release da anterior. A escolha de slot é limitada
e determinística: usa um slot desligado; sem slot livre, prefere vozes em
release e menor nível, usando idade como desempate. Não há alocação nem lock no
callback. A troca de performance conserva os tempos de gate relativos e inicia
release de todas as vozes ativas.

Arquivos alterados: `src/core/BassAudio.hpp/.cpp`, `PARIDADE_HTML_JUCE.md` e
este documento. `batruc_app` recompilou com sucesso. Não rodei testes, render ou
escuta A/B; por isso não alterei a estimativa de paridade. A web não impõe o
limite de oito sobreposições, então ainda pode divergir em densidade extrema.

**Próximo passo:** medir respostas determinísticas dos parâmetros e comparar
renders. Escutar densidades altas, transições de seed e releases longos para
confirmar o limite e a reciclagem.

## Crossfade ao reciclar uma voz — 27/10/2026

Quando o pool de oito vozes está cheio, a voz escolhida para reciclagem agora
preserva o último valor de saída e o esvazia em 4 ms enquanto começa a nova
nota. Se já havia um fade residual em andamento, ele é somado à cauda retida;
isso evita um salto seco ao sobrepor duas reciclagens próximas. O trabalho é
limitado a operações escalares no callback.

O app compilou com sucesso após essa alteração. Não rodei testes, render ou
escuta. O efeito do fade em níveis altos e o risco de saturação da soma das
vozes continuam sem medição.

Para comparar os renders ainda faltam dois caminhos de saída: desacoplar o
grafo de áudio HTML do `AudioContext` ao vivo para usar `OfflineAudioContext`, e
renderizar o motor JUCE por blocos em memória, escrevendo WAV determinístico
sem `ThreadedWriter` ou dispositivo de áudio. Os dois caminhos precisam ler a
mesma receita/seed e exportar a agenda/configuração junto do WAV. Depois entram
alinhamento, RMS/true peak, clipping, silêncio e diferenças de transientes; a
escuta dirigida fica para trechos onde as métricas mostram diferença.

**Próximo passo:** implementar o caminho JUCE offline sobre o renderer de
composição já existente; em seguida, separar a construção do grafo HTML e
renderizar com `OfflineAudioContext`.

## Render JUCE-core e captura web automatizada — 27/10/2026

Adicionado `batruc_offline_render SEED_HEX arquivo.wav`. O executável prepara
sete ciclos, aplica as escolhas seeded de quantidade de vozes, fase, padrão,
lei de intensidade, transformações, acoplamentos e perfil do oscilador do
baixo, processa o áudio por blocos sem dispositivo de áudio e grava PCM16
estéreo a 48 kHz, mais manifesto JSON. O restante da receita é
fixo. `tools/render_web.cjs` automatiza a captura HTML em Chromium headless
local, com a mesma seed e duração de 56 s, e guarda o master MediaRecorder e a
partitura JSON. `tools/render_compare.sh` orquestra render JUCE, captura e
comparação em uma chamada. O WAV JUCE é produzido primeiro e as saídas
existentes não são sobrescritas; se o loopback falhar, o WAV permanece útil.
`tools/compare_audio.py` mede duração, RMS, pico,
clipping, silêncio, primeiro ataque, alinhamento e correlação do envelope; lê
os dois manifestos, compara os campos comuns da receita e lista divergências.
Usa FFmpeg com limite de duração e timeout para WebM/OGG. Os derivados ficam em
`.local-build/parity-audio/`.

O renderer JUCE-core não importa o estado completo do app e ainda utiliza uma
receita parcialmente fixa. A web grava em tempo real com codec comprimido,
enquanto o core escreve PCM offline. Por isso estes artefatos facilitam uma
comparação repetível, mas ainda não comprovam paridade sonora.

Arquivos alterados: `CMakeLists.txt`, `src/tools/offline_render.cpp`,
`tools/render_web.cjs`, `tools/render_compare.sh`, `tools/compare_audio.py`,
`PARIDADE_HTML_JUCE.md` e este registro. Execute `sh tools/render_compare.sh
SEED_HEX` para a cadeia completa (Chromium precisa permitir um loopback local).
O alvo compilou e foi renderizado em
`.local-build/parity-audio/batruc-5EED2026-juce-core.wav`; FFprobe confirmou
56 s, 48 kHz e estéreo, com 9 vozes seeded. `node --check` e `python3 -m
py_compile` passaram. A captura HTML não foi executada: o sandbox bloqueou a
porta loopback temporária necessária ao DevTools do Chromium. O Chromium normal
do usuário pode permanecer fechado: a cadeia inicia um perfil temporário. O
bloqueio é da porta loopback do processo, não da janela do navegador. Nenhuma
métrica A/B foi calculada. A comparação do manifesto agora trata índices fora
da faixa sem encerrar o relatório. A primeira tentativa de preparar o baixo detectou gates que
cruzavam a borda de ciclo; o renderer agora limita esses gates ao fim do ciclo.

**Próximo passo:** executar a captura web num ambiente que permita o loopback,
ler a receita efetiva do navegador e fechar as diferenças de configuração com
o render JUCE antes de tratar o comparador como evidência de paridade.

## Captura web por pipes DevTools — 28/10/2026

O Chromium comum pode permanecer fechado. `tools/render_web.cjs` agora usa o
canal `--remote-debugging-pipe` do Chromium headless isolado, sem abrir porta
loopback, e intercepta localmente os `Blob`s de áudio e partitura para escrevê-
los no diretório de saída sem depender de downloads do navegador. Fechamento
gracioso reduz mensagens de pipe durante a limpeza. `BATRUC_RENDER_SECONDS`
aceita 28, 42, 56, 70 ou 84 segundos; `render_compare.sh` continua na receita
de 56 s. A cadeia preserva a recusa de sobrescrita.

Validação: captura real com seed `5EED2027`, duração registrada de 56,138 s,
47 eventos de partitura, WebM/Opus (16.213 bytes) e TXT JSON (29 KiB). A
captura foi obtida no contexto autorizado porque o sandbox bloqueia a execução
normal do Chromium. Não foi feita decodificação, comparação A/B ou escuta deste
áudio: depois da captura o swap chegou a 1,5/2 GiB (75%), acima do limite de
50% do procedimento de recursos. O render JUCE para esta seed e a comparação
ficam pendentes até o swap voltar abaixo do limite. O arquivo pequeno também
precisa ser verificado quanto a silêncio/baixo nível antes de servir como
evidência de paridade. O artefato está em `.local-build/parity-audio/`.

Arquivos de fonte/documentação afetados: `tools/render_web.cjs`,
`PARIDADE_HTML_JUCE.md` e `TAREFAS.md`. O registro anterior de 27/10 descreve
corretamente a tentativa inicial por porta; esta etapa substitui esse caminho
por pipes e captura direta dos Blobs.

**Retomada naquele momento:** conferir recursos antes de decodificar/verificar
o WebM, renderizar `5EED2027` pelo core JUCE e gerar o relatório com os dois
manifestos. O método foi substituído pela etapa offline abaixo.

## Render offline HTML e primeiro confronto A/B — 28/10/2026

Substituí a captura em tempo real como método de comparação técnica por um
render HTML em `OfflineAudioContext`. `tools/render_web.cjs` agora exporta WAV
PCM estéreo, manifesto e partitura, sem depender de loopback, gravação do
desktop ou do Chromium habitual aberto. A aplicação continua mantendo seu
recurso próprio de `MediaRecorder`; ele não é usado neste A/B. O render nativo
JUCE e o render HTML foram gerados para a seed `5EED2027`, duração de 56 s.

O primeiro confronto confirma que os 22 campos comparáveis da receita
coincidem, mas revela diferença acústica importante: correlação de envelope
RMS alinhado 0,289; RMS HTML 0,01682 contra JUCE 0,03273; pico HTML 0,73975
contra JUCE 0,17865. Ambos ficaram sem clipping. Portanto, igualdade de seed e
eventos/configuração não é paridade de áudio. A estimativa geral permanece em
69,0%, sem crédito por esse resultado. A captura WebM anterior, silenciosa,
permanece preservada e não é considerada evidência válida.

**Correções feitas antes do confronto:** eventos das vozes de fórmula agora
iniciam seus osciladores no caminho de render; o perfil inarmônico do baixo
deixou de usar `Math.SQRT3` inexistente e calcula `Math.sqrt(3)`. O render web
valida frequência e tempo finitos antes de agendar o oscilador para produzir
diagnóstico explícito em caso de evento inválido.

**Arquivos afetados:** `prototype/app.js`, `tools/render_web.cjs`,
`tools/render_compare.sh`, `tools/compare_audio.py`,
`PARIDADE_HTML_JUCE.md` e este registro. Os WAVs, manifestos, partitura e
relatório permanecem em `.local-build/parity-audio/` como evidência local; não
foram apagados.

**Validação:** `batruc_offline_render` compilou; render HTML OfflineAudioContext
e render JUCE produziram WAV estéreo PCM16 a 48 kHz. O comparador terminou e
reportou coincidência dos 22 campos de receita e correlação 0,289. Não houve
escuta subjetiva nem mudança da estimativa de paridade. `node --check` para
`prototype/app.js` e `tools/render_web.cjs`, `sh -n tools/render_compare.sh` e
`git diff --check` passaram. O preflight após o render mostrou cerca de 10 GiB
de RAM disponível, swap abaixo de 4 MiB de 2 GiB e sem processos pesados.

**Próximo passo:** rastrear a diferença acústica em blocos pequenos e
reprodutíveis: primeiro comparar evento a evento e envelopes de amplitude,
depois osciladores/filtros, ganho dos canais e compressor/master. Manter a
mesma seed, sem sobrescrever os artefatos atuais; só gerar novos nomes de
evidência quando alterar o renderer.

## Agenda e baixo conferidos; divergência de síntese localizada — 28/10/2026

O manifesto JUCE agora exporta cada ataque de voz e baixo, inclusive ciclo,
tempo, altura/duração, coordenadas que alimentam o baixo, voz de origem e
intensidade usada na seleção. `tools/compare_audio.py` normaliza a numeração de
ciclos dos dois formatos e compara esses dados, além das métricas de áudio.
Isso corrigiu uma falha da auditoria anterior: comparar apenas os campos da
receita não garantia que a agenda e as notas fossem iguais.

A comparação da seed `5EED2027` confirmou os 22 campos de receita, 47 ataques
em cada motor e erro temporal máximo de 0,05 ms. Detectei no renderer JUCE do
baixo um uso fixo de fase `0.20` na avaliação das coordenadas; a composição
usa fase `0.325`. O cálculo agora recebe a fase real da seed. Após a correção,
as 22 alturas e as coordenadas de origem coincidem com o HTML (erro máximo
0,000001); as 22 intensidades coincidem com erro máximo 0,0000005. Duas
durações de cauda na borda final ainda diferem até 8,54 ms.

O primeiro fullmix JUCE anotado nesta etapa mediu RMS 0,41847 e correlação
−0,169. A inspeção por stems revelou que esse resultado estava contaminado pelo
reuso sem limpeza dos buffers de saída do baixo no renderer offline; ver a
correção e as medições substitutivas no registro mais recente, no início deste
arquivo. A diferença de síntese das vozes e os níveis entre motores continuam
em aberto. Estimativa geral mantida em 69,0%.

Também corrigi a reconstituição de snapshots em `score.js`: quando o arquivo
contém bases e valores seeded finais, a normalização volta às bases e não
aplica a variação pela segunda vez. A leitura do snapshot da renderização
reproduziu os 47 eventos, ciclos e alturas sem diferenças (tempo máximo
0,00000044 s). O export do plano registra agora voz de origem e coordenadas
usadas pelo baixo. A limpeza automática do perfil foi reforçada e, numa nova
renderização headless, terminou sem deixar um perfil temporário novo. Um perfil
residual de uma execução anterior foi removido após confirmar que não havia
processos Chromium ativos.

**Arquivos alterados:** `src/tools/offline_render.cpp`, `prototype/score.js`,
`prototype/app.js`, `tools/compare_audio.py`, `tools/render_web.cjs`,
`PARIDADE_HTML_JUCE.md` e este registro. WAVs, stems, scores, manifestos e
relatórios novos estão em `.local-build/parity-audio/`; as evidências anteriores
foram preservadas.

**Validação executada:** o alvo `batruc_offline_render` compilou; verificações
de sintaxe passaram em `score.js`, `app.js`, `render_web.cjs` e
`compare_audio.py`. Foram feitas renderizações locais HTML/JUCE e comparações
diretas de receita, ataques, coordenadas, alturas, durações, intensidades e
envelope. O render adicional de confirmação produziu os três artefatos esperados;
o perfil temporário remanescente foi removido após verificar que nenhum processo
Chromium o utilizava. Não rodei a suíte de testes nem fiz escuta subjetiva.

**Próximo passo:** equiparar o contrato de ganho/mixer, revisar a envoltória das
vozes modais e escutar as diferenças materiais. Não aumentar a estimativa antes
dessas verificações.

## Receita de variação seeded compartilhada — 27/10/2026

Extraí para `CompositionCore` a realização determinística da variação global:
alteração de ciclos, acoplamentos, fases wrapped e escolha de transformação.
`Main.cpp` agora usa essa mesma função no JUCE. Uma fixture cruzada compara o
resultado com a normalização da página para quatro seeds e quatro níveis (0%,
25%, 50%, 100%), cobrindo as 12 vozes. As 16 receitas coincidem nos quatro
grupos de parâmetros. A fixture ponta a ponta compara 9.168 agendas
HTML/JUCE. A matriz principal tem 4.752 casos: quatro seeds, três níveis de
variação, quatro frases, três relações do baixo, compassos 3/4, 4/4, 5/4 e
7/8, BPM 97/120/220 e três fases formais. O caso 5/4 a 97 BPM foi excluído
porque ultrapassa o limite temporal da receita. Outros 2.304 casos cruzam
quatro seeds, duas variações, frases loop/chamada-resposta, três relações,
2/4/8/12 vozes, swing 0,50/0,57/0,67 e os grooves reto/ternário/aditivo/
contratempo, em 4/4 a 120 BPM. Quantidade, ciclo, tempos e alturas MIDI
conferem nos dois grupos. Outros 1.536 casos exercitam as quatro dinâmicas e
quatro intenções em todos os grooves, duas frases e três relações do baixo.
Agora também coincidem duração e intensidade finais, não apenas notas/tempos.

Arquivos afetados: `src/core/CompositionCore.hpp/.cpp`, `src/app/Main.cpp`,
`tests/seeded_variation_fixture.cpp`, `tests/compare_seeded_variation.cjs`,
`tools/test_seeded_variation_parity.sh`, `tests/seeded_bass_fixture.cpp`,
`tests/compare_seeded_voice_bass.cjs`, `tools/test_seeded_voice_bass_parity.sh`,
`CMakeLists.txt`,
`PARIDADE_HTML_JUCE.md` e este registro.

**Validação:** `batruc_app` e `batruc_seeded_variation_fixture` compilaram.
`node --check` passou nos comparadores e scripts JS, `node
prototype/test-score.cjs` passou. CTest passou 10/10. As fixtures cruzadas
confirmaram 16 receitas de parâmetros, 5.472 agendas do baixo e 8.592 agendas
de baixo com vozes globalmente variadas; a comparação usa tolerância de 1e-7 s
para arredondamento float. O fixture nativo recompilou; a suíte CTest completa
passou 10/10. Não houve inspeção visual nem comparação de áudio nesta etapa.

**Estado e próximo passo:** todas as quatro curvas dinâmicas e intenções foram
comparadas em cruzamento dedicado; duração manual, Fibonacci e geométrica
agora também têm matriz própria. Próximo: comparar envelope e timbre, depois
automatizar renderizações e conferir a interface em execução.

## Paridade de intensidade e duração do baixo — 27/10/2026

A comparação passou a verificar também duração do gate e intensidade final
dos eventos. Isso expôs e corrigiu diferenças entre HTML e JUCE: o core nativo
agora reaplica ao baixo a curva dinâmica, intenção e acento de groove; o
arredondamento de acentos usa a mesma tolerância nas duas implementações; o
gate mínimo é 25 ms; a duração é limitada pelo próximo evento global, pelo
início da pausa formal e pela borda de compassos irregulares. Em 7/8, a nota
manual de dois passos não avança para o compasso seguinte. Corrigi também a
influência de vozes acima da unidade: o cálculo de prioridade continua limitado
para seleção da voz dominante, mas a intensidade bruta alimenta o evento final
como no HTML.

Arquivos afetados: `src/core/BassComposition.hpp/.cpp`,
`src/core/CompositionCore.cpp`, `src/app/Main.cpp`, `prototype/score.js`,
`tests/seeded_bass_fixture.cpp`, `tests/compare_seeded_voice_bass.cjs`,
`tools/test_seeded_bass_parity.sh` e este registro.

**Validação:** `batruc_app` e `batruc_seeded_bass_fixture` compilaram; CTest
passou 10/10. A fixture de ponta a ponta conferiu 8.592 agendas, incluindo
ciclo, ataque, altura, duração e intensidade. Tolerâncias: 1e-7 s para tempo e
duração, 1e-6 para intensidade. Não houve renderização nem escuta A/B.

**Limite e próximo passo:** a matriz dedicada cobre as quatro dinâmicas e as
quatro intenções; a duração usa a receita manual padrão. Ainda faltam as leis
de duração Fibonacci/geométrica, envelope, timbre, renderização e interface.

## Mínimo de variação de altura do baixo e fixture expandida — 27/10/2026

A página web agora expõe no módulo do baixo “Mínimo de variação de altura”,
persistido junto à composição. O valor efetivo segue o JUCE: máximo entre esse
mínimo e a variação global da composição. Assim a pessoa pode variar alturas do
baixo mesmo com a variação global em zero, sem mudar o comportamento padrão
quando o novo controle está em zero. A partitura matemática exportada registra
separadamente variação global, mínimo do baixo e valor efetivo.

Ampliei a fixture HTML/JUCE para 5.472 combinações, acrescentando ao cruzamento
existente valores de variação de altura do baixo 0% e 100%. O controle global
fica em zero nessa dimensão para isolar o algoritmo de altura. Todas as
agendas coincidem em quantidade, ciclo, tempo e MIDI. As fases formais
0/0,125/0,25 já fazem parte da matriz; o jitter de fase e a mudança de ciclos
produzidos pela variação global continuam pendentes.

Arquivos alterados nesta etapa: `prototype/index.html`, `prototype/app.js`,
`prototype/score.js`, `prototype/test-score.cjs`, `tests/seeded_bass_fixture.cpp`,
`tests/compare_seeded_bass.cjs`, `tools/test_seeded_bass_parity.sh`,
`PARIDADE_HTML_JUCE.md` e este registro.

**Validação:** os três scripts JavaScript passaram em `node --check`,
`node prototype/test-score.cjs` passou e CTest passou 8/8; a fixture isolada
relatou `5472 agendas HTML/JUCE`. A interface foi validada por código e
persistência do modelo, sem inspeção manual no navegador nesta etapa. A
estimativa geral permanece 67,1% até conferir a apresentação e concluir as
dimensões expressivas e a comparação sonora.

**Próximo passo:** fechar a atualização de ciclos e fases individuais da seed
em ambos os motores e compará-los por fixture; depois conferir rapidamente o
controle novo no navegador e seguir para intensidade/duração e A/B de áudio.

## Fixture ampliada do baixo — 27/10/2026

Ampliei a comparação cruzada seeded HTML/JUCE para 2.736 combinações: compassos
3/4, 4/4, 5/4 e 7/8; frases loop, respiração, cadência e chamada/resposta;
baixo independente, seguindo ataques ou respondendo; e quatro seeds
(A1B2C3D4, 00000001, 5EED2026, FFFFFFFF). Em cada combinação, a fixture
compara todos os ataques por quantidade, ciclo, tempo absoluto e altura MIDI.
As seeds são A1B2C3D4, 00000001, 5EED2026 e FFFFFFFF; BPMs 97, 100, 120, 160 e
220 (97 não se aplica a 5/4 porque quatro compassos passam de 12 s). Com quatro
vozes e swing 57%, e fases formais 0, 0,125 e 0,25, as 2.736 agendas coincidem.

O caso 3/4 a 97 BPM encontrou ataques exatamente na fronteira de subdivisão
atribuídos a slots vizinhos por arredondamento. Apliquei a mesma tolerância de
1e-9 subdivisão em `BassComposition.cpp` e `prototype/score.js`, inclusive no
índice de subdivisão usado para frases; a atribuição HTML do ciclo formal
também ganha essa tolerância e limita o progresso local a zero nessa fronteira.
O novo caso de 97 BPM permanece na matriz como regressão.

Arquivos afetados: `tests/seeded_bass_fixture.cpp`,
`tests/compare_seeded_bass.cjs`, `tools/test_seeded_bass_parity.sh`,
`PARIDADE_HTML_JUCE.md` e este registro.

**Validação:** o alvo `batruc_seeded_bass_fixture` compilou; CTest completo
passou 8/8; a execução detalhada confirmou
`seeded bass parity: 2736 agendas HTML/JUCE`; `node --check
tests/compare_seeded_bass.cjs` e `node prototype/test-score.cjs` passaram. O
preflight local desta etapa encontrou cerca de 10 GiB disponíveis, swap em 124
MiB/2 GiB, 983 GiB livres no volume do projeto e 6,8 GiB em `/`, sem processos
pesados concorrentes. Não houve renderização nem escuta A/B.

**Estado e próximo passo:** a nota provisória permanece em 67,1%; a matriz
confirma o calendário em várias seeds, BPMs e fases formais, mas não verifica
variação aleatória de altura, jitter individual de fase/ciclos da variação
global, intensidade, duração, timbre ou áudio. Próxima tarefa: compartilhar o
cálculo de receita variada entre os testes nativos e web, comparar essas
variações e depois validar o render de áudio determinístico; não declarar
equivalência geral do baixo antes disso.

## Baixo seeded JUCE — agenda e comparação cruzada — 27/10/2026

Revisei o gerador nativo contra `prototype/score.js`. O caminho JUCE agora
mantém os ataques derivados da seed sem reaplicar quantização, swing, variação
de altura e relação seguir/responder; aplica expressão pelo progresso global
da forma de sete ciclos e conta o ciclo formal nas pausas de frase. A seleção
do slot usa as intensidades do agendador HTML; não acrescenta o fator extra
presente apenas no plano de gravação nativo. A compactação só funde eventos do
mesmo ciclo, preservando ataques locais coincidentes entre ciclos. A regra de
colisão e o cálculo de slots, alturas seeded e relações baixo/vozes foram
extraídos para o core.

Adicionei uma fixture cruzada para a mesma seed e composição em 4/4, 120 BPM,
quatro compassos por ciclo e swing de 57%. Ela compara todos os ataques nas
relações independente, seguir e responder: quantidade, ciclo, instante
absoluto e altura MIDI. As três agendas HTML/JUCE coincidem.

Validações: preflight com 10 GiB disponíveis, swap 124 MiB/2 GiB, 983 GiB
livres no volume do projeto e 6,8 GiB em `/`; sem processos pesados. Build de
`batruc_app`, `batruc_seeded_bass_fixture` e `batruc_bass_composition_test`
passou; CTest 8/8, incluindo a fixture cruzada. `node --check` do comparador e
`node prototype/test-score.cjs` passaram. Não houve renderização de áudio A/B
nem escuta. A matriz sobe provisoriamente para 66,6%; a fixture cobre uma
receita rítmica fixa e não certifica a igualdade acústica nem todas as
configurações.

Próximo passo técnico: ampliar a fixture para outras métricas, fases, frases,
variações e durações; depois integrar a agenda validada ao render offline e
comparar o áudio com o mesmo mix.

## Validação de áudio sem cliques — 26/10/2026

Para as próximas comparações HTML/JUCE, substituir a inspeção repetida das
janelas por um harness local invocado por um comando: mesma receita/seed,
render offline estéreo de ambos os motores, relatório de eventos e métricas de
nível, alinhamento, clipping e silêncio. WAVs e relatório ficam sob
`.local-build/parity-audio/`; não são fontes arquivísticas. A escuta humana
fica restrita aos casos em que as métricas apontarem divergência. Não usar
screenshots como validação.

A tentativa de gravação via interface nesta retomada não gerou dois WAVs
isolados; nenhuma conclusão A/B foi validada. Criado `tools/compare_audio.py`
como auxiliar externo, restrito à saída HTML WebM/OGG e à saída JUCE WAV, para
analisá-las em uma só execução (nível, duração, clipping, silêncio e
alinhamento do envelope), sem screenshots. Ainda falta
automatizar a geração isolada dos arquivos: expor render offline no JUCE e
ligar o `OfflineAudioContext` HTML ao mesmo formato de receita; só então rodar
tudo sem interação e atualizar a matriz com resultados observados. Detalhes e
critérios em [`PARIDADE_HTML_JUCE.md`](PARIDADE_HTML_JUCE.md).

## Avanço das cinco frentes de paridade — 26/10/2026

1. **Seed/fase/receitas:** a web agora preserva ciclos de 0,5 a 16 em passos
   de 0,25, com as mesmas bases sub-harmônicas nativas para V3–V12. Teste de
   geração cobre ciclos fracionários. Seleção seed→receita e fase em uma
   composição comum seguem pendentes.
2. **Ritmo/polirritmia/transições/baixo:** os 12 presets nativos (4:3, 5:4,
   6:4, 7:4, 8:6, 10:4, 10:7 e relações de 3–5 vozes) agora estão na web. A
   lista, ativação das vozes necessárias e preservação dos canais seguintes
   estão cobertas por teste; salvamento e aplicação durante reprodução ainda
   precisam de validação.
3. **Mixer:** os dois alvos já têm mixer e sensor RMS; faltam comparar mute,
   solo, pan, ganho e equilíbrio do baixo por uma execução controlada.
4. **Arquivos/conteúdo/idiomas:** gravação e armazenamento local existem nos
   dois alvos; seguem pendentes a compatibilidade de composição web↔JUCE e a
   conferência de conteúdo/tutorial nos quatro idiomas.
5. **Gráficos/geometria/Fourier:** os recursos estão implementados, mas a
   correspondência de traço, marcadores, epiciclos e movimento em execução não
   foi validada.

Alterações desta etapa: `prototype/score.js`, `prototype/app.js`,
`prototype/index.html`, `prototype/style.css`, `prototype/test-score.cjs`,
`PARIDADE_HTML_JUCE.md` e este arquivo. `node --check` passou nos dois scripts;
`node prototype/test-score.cjs` passou. A estimativa continua em 65,1% porque
essas mudanças de código não substituem o aceite funcional/auditivo. Próximo:
alinhar o mapa seed→receita na web/JUCE e criar uma fixture compartilhada para
ritmo e baixo.

## Seed nova e composição variada ao iniciar — 26/10/2026

Corrigido o estado que fazia a seed parecer inoperante: a variação global
começava em 0%, valor que preserva ciclos, transformações e alturas-base. O
controle agora inicia em 35% no HTML e no JUCE. Cada abertura cria uma seed
visível nova; o botão “Nova seed” também gera outra realização. Ao substituir
por uma seed, se o usuário tiver reduzido explicitamente a variação a 0%, ela
volta a 35%; valores acima de 0% são preservados. Assim, a mesma seed e o mesmo
nível reproduzem a mesma realização, enquanto seeds diferentes recalculam as
vozes e o baixo determinístico. Ainda é possível selecionar 0% depois para
congelar a base e explorar uma realização sem variação adicional.

Arquivos afetados: `prototype/index.html`, `prototype/app.js`,
`src/app/Main.cpp`, `PARIDADE_HTML_JUCE.md` e este arquivo. O alvo nativo
`batruc_app` compilou com `timeout 180s cmake --build
.local-build/native/linux-debug-juce --target batruc_app --parallel 2`; `node
--check` passou em `prototype/app.js` e `prototype/score.js`. Preflight:
10 GiB disponíveis, 153 MiB de swap usada/2 GiB e 983 GiB livres no volume do
projeto, sem processo pesado concorrente. Não abri interfaces nem ouvi a saída.
Ainda falta validar as mesmas seeds e níveis de variação em reprodução e
comparar o resultado sonoro das duas saídas; não declaro paridade fechada nesta
etapa.

## Fase-base seeded alinhada e migração de cenas — 26/10/2026

Alinhei um componente da recombinação que os dois geradores calculavam de
formas diferentes: a fase-base individual das vozes. No JUCE, cada canal agora
parte do deslocamento escalonado `(voz−2)/16` (zero-based `(i−1)/16`), como na
web; o desvio seeded continua usando o mesmo hash FNV-1a, PRNG Mulberry32,
amplitude `±0,125 × variação` e agora dá a volta módulo uma volta completa em
vez de saturar nos limites do slider. A base continua não cumulativa ao trocar
seed diretamente; assim o pequeno deslocamento não acumula realizações antigas.

Cenas nativas foram para schema 23, que marca essa interpretação da fase-base.
Ao ler schema 1–22 com seed e base de variação, o JUCE migra a base antiga
somando o deslocamento por voz uma única vez; cenas novas não recebem a migração
novamente. A fase global/formal da seed e a seleção da receita continuam com
diferenças entre HTML e JUCE, então esta mudança alinha a fase individual e não
certifica paridade total de geração.

Arquivos afetados: `src/app/Main.cpp`, `README.md`, `TAREFAS.md` e
`PARIDADE_HTML_JUCE.md`. Compilação JUCE após preflight: 10 GiB disponíveis,
154 MiB de swap usada/2 GiB e 983 GiB livres; nenhum processo pesado ativo.
`timeout 180s cmake --build .local-build/native/linux-debug-juce --target
batruc_app --parallel 2` concluiu. Não rodei testes nem abri ou ouvi os alvos.
Falta comparar seeds e fases comuns, inclusive cenas schema antigo e novo.

## Sensor RMS e ajuste de equilíbrio — mixer HTML/JUCE — 26/10/2026

O mixer mede em reprodução o RMS do barramento das vozes e do baixo em separado.
Na web, analisadores ficam depois do trim das vozes e do fader do baixo; no
JUCE, os níveis RMS são calculados no callback a partir do sinal das fórmulas e
de um buffer de baixo temporário pré-alocado em `prepareToPlay`. O callback não
aloca memória para a medição. Um controle mostra a compensação RMS sugerida e,
ao ser acionado, aplica-a ao trim das vozes (limitado a −12…+24 dB). O ajuste é
estático e explícito; a leitura não faz automação contínua nem bombeia o ganho
nota a nota. O ajuste continua sujeito a audição e julgamento musical.

O trim de −12…+24 dB, padrão +16,5 dB, permanece aplicado antes da soma com o
baixo e é salvo em composições/snapshots web e cenas nativas. Na tela web, os
níveis são dBFS aproximados dos analisadores; no JUCE, são RMS linear suavizados
com constante de 450 ms e exibidos como dBFS no tooltip. A recomendação compara
esses níveis e respeita os limites do slider. A leitura não é LUFS e não mede
percepção de loudness.

Arquivos afetados: `prototype/index.html`, `prototype/style.css`,
`prototype/app.js`, `src/app/Main.cpp`, `PARIDADE_HTML_JUCE.md` e este arquivo.
O alvo JUCE `batruc_app` compilou com `timeout 180s cmake --build
.local-build/native/linux-debug-juce --target batruc_app --parallel 2` após um
ajuste de API indicado pela primeira compilação. Preflight antes da compilação:
10 GiB disponíveis, 156 MiB de swap usada/2 GiB e 983 GiB livres; nenhum
processo pesado concorrente. Não rodei testes nem abri interfaces ou fiz
escuta. Próximo: validar o indicador e a recomendação ao ouvir as duas saídas,
incluindo o efeito do compressor web. A estimativa de paridade permanece em
65,1% até essa validação.

## Acompanhamento da gravação e partitura local — 26/10/2026

O acompanhamento JUCE da gravação agora usa o contrato JSON versionado
`batruc-recording-score` (schema 1), em paralelo ao formato da maquete web. O
WAV PCM estéreo 24-bit recebe um TXT `-partitura.txt` com configuração inicial,
equações/partitura, agenda preparada dos sete ciclos (vozes e baixo), dados
técnicos do WAV, quadros efetivamente aceitos pelo writer/duração, blocos
perdidos e revisões de configuração.
Revisões são observadas na thread da interface a cada ~100 ms; o áudio continua
fora da serialização. O navegador e o JUCE agora medem `effective_at_seconds`
a partir do início da gravação. A agenda está limitada a 8.192 eventos e o
histórico a 512 revisões, com indicadores de truncamento. O comando JUCE
“Abrir composição” também aceita o `-partitura.txt` nativo e restaura a
configuração inicial ao histórico da sessão; não tenta reconstruir o áudio nem
as revisões a partir desse gesto.

Após corrigir um acesso a método privado apontado pela primeira compilação, o
build JUCE `batruc_app` passou com dois jobs e timeout de 180 s. Preflight final:
10 GiB disponíveis, swap 160 MiB/2 GiB, 983 GiB livres no volume do projeto e
7,8 GiB em `/`; sem processo pesado concorrente. `git diff --check` passou. Não
rodei testes, não abri a interface, não gravei nem conferi o áudio/TXT
resultante. O plano nativo registra pitch MIDI nulo para vozes formulares
contínuas; a nota MIDI do baixo não inclui o movimento posterior do LFO.
Revisões são amostras aproximadas e movimento de fase intrabar não é expandido
como revisão própria. O leitor agora limita cenas e partituras a 8 MiB; o
`-partitura.txt` restaura somente a configuração inicial. Próximo passo:
conferir uma gravação curta no JUCE e na web, ler os dois TXT, comparar
conteúdo/destino e verificar recuperação sem confundir partitura com cena de
banco. A composição web `batruc-composition` continua sem importação direta no
JUCE: seu modelo inclui movimento pendular e duração de ciclo independentes que
não cabem sem perda na cena nativa; projetar um contrato comum fiel é uma
pendência separada.

O leitor de recuperação foi compilado após essa alteração: preflight com 10 GiB
disponíveis, swap 159 MiB/2 GiB, 983 GiB livres no volume do projeto, 7,8 GiB
em `/` e nenhum processo pesado. `batruc_app` passou com dois jobs; `git diff
--check` passou. Não abri o app nem tentei restaurar um arquivo real.

## Relação do baixo consulta a fase e swing do ciclo — 26/10/2026

“Seguir ataques” e “Responder” no JUCE usavam uma agenda vocal única como
referência, embora os sete ciclos da forma alterem fases e ciclos das vozes.
Agora a preparação calcula, fora do callback, os ataques de cada ciclo formal
com a transformação de fase e o swing da agenda vocal. Cada evento seeded de
baixo consulta os ataques do próprio ciclo antes de seguir ou responder. O
modo independente e eventos manuais continuam com suas regras próprias. A
síntese recebe uma agenda pronta; nenhuma busca de eventos ou alocação foi
acrescentada ao processamento de áudio.

Preflight: 10 GiB disponíveis, swap 161 MiB/2 GiB, 983 GiB livres no volume do
projeto e 7,8 GiB em `/`; nenhum processo pesado ativo. `batruc_app` compilou
com dois jobs e sem erros. Não rodei testes, não abri a interface nem ouvi o
som. Falta comparar as três relações e escutar swing e transições com a mesma
seed nos dois alvos; por isso, a estimativa provisória continua em 65,1%.

## Baixo seeded acompanha a forma em sete ciclos — 26/10/2026

Fechada uma diferença estrutural entre os alvos: o HTML recalculava os ataques
vocais e a linha de baixo em cada ciclo formal, enquanto o JUCE repetia nos sete
ciclos uma frase derivada apenas da agenda-base. A preparação JUCE agora
recalcula a linha a partir dos eventos e coordenadas vocais de cada ciclo,
aplicando o arco de fase 2+2+1+2. Cada evento gerado carrega a identificação do
ciclo; o renderer seleciona a agenda correspondente durante a execução e
continua tratando eventos manuais sem identificação como comuns a todos os
ciclos. A grade editável resume o primeiro compasso da seção de mudança.

Também preservei o ciclo ao ordenar, aproximar, deduplicar e limitar as durações
dos eventos, para não fundir ataques coincidentes de ciclos formais diferentes.
O processamento de áudio continua sem alocação ou montagem de agenda no callback.

Preflight: 11 GiB disponíveis, swap 163 MiB/2 GiB, 983 GiB livres no volume do
projeto e 7,8 GiB livres em `/`; nenhum processo pesado ativo. O alvo
`batruc_app` compilou com dois jobs, sem warnings. Não rodei testes, não abri a
interface e não ouvi o instrumento. Falta comparar uma mesma seed nos dois
alvos e escutar construção, mudança, pausa e retorno. A estimativa geral de
paridade permanece em 65,1% até a aceitação auditiva e o fechamento das demais
áreas.

## Inventário de fechamento da paridade HTML/JUCE — 26/10/2026

A matriz de [paridade](PARIDADE_HTML_JUCE.md) estima provisoriamente 65,1% e
organiza o saldo de 34,9 pontos em quatro lotes aproximados de 9–10 pontos:
P1 geração/forma; P2 geometria/Fourier; P3 vozes/baixo/mixer; P4
arquivos/gravação/conteúdo.
Não são incrementos automáticos: cada lote exige os critérios de aceite da
matriz, e comparação em execução/escuta é necessária para o total.

Lacunas inventariadas: diferenças de semântica da fase-base seeded;
comparação auditiva de baixo e mixer; compatibilidade de cena/partitura;
conferência visual em execução, inclusive em janelas compactas; cobertura de
conteúdo/idiomas.

Avanços nesta retomada: o gráfico, o arco formal, o traço geométrico e os
epiciclos seguem o transporte. HTML e JUCE agora calculam a linha de baixo
seeded de quatro compassos a partir dos ataques, alturas e coordenadas
geométricas do plano vocal. O modo
“Seguir ataques” agora busca os ataques vocais mais próximos. A publicação de
uma linha nova preserva fase e envelope do baixo; início/pausa usam fade de
5 ms. O fechamento do mixer esconde os controles de movimento V2 e
polirritmia; a altura mínima da janela agora comporta os módulos inferiores.
Restam comparações auditivas e visuais para aceitar essas mudanças e fechar a
paridade. A estimativa segue provisória em 65,1%.

### Controle geral de variação da seed — 26/10/2026

O JUCE recebeu variação global de 0 a 100%, determinística e não cumulativa:
ciclos, acoplamento, fase de evento, transformação e variação de alturas do
baixo são recalculados sobre a base da seed. Cenas schema 23 preservam o valor
do controle e as bases por voz e baixo; esquemas anteriores continuam aceitos.
O controle só fica ativo com uma seed válida. Uma edição manual remove o vínculo
com a seed e congela os valores gerados. Uma seed nova volta à receita-base e
mantém o nível de variação escolhido. O baixo é refeito a partir dos eventos
vocais: seus tempos seguem os ataques e as alturas usam coordenadas x/y, sem
impor uma escala. “Seguir ataques” alinha cada nota ao evento vocal mais
próximo. A fase nos dois alvos desloca a trajetória e, por consequência, seus
cruzamentos/ataques; os valores iniciais derivados da seed ainda seguem regras
distintas e precisam ser comparados com uma receita comum.

### Cliques, sincronismo do baixo e fechamento dos painéis — 26/10/2026

A troca da linha do baixo deixa de zerar fase, filtro e envelopes no meio do
som. Pausa e retomada aplicam um fade de 5 ms, para voz e baixo desaparecerem
juntos. O painel do mixer agora oculta os controles de movimento V2 e
polirritmia ao fechar. A janela inicia maior e não pode ser reduzida abaixo da
altura necessária para os módulos inferiores não transbordarem sobre a
interface principal.

Preflight: 8,8 GiB disponíveis, swap 169 MiB/2 GiB, 983 GiB livres no volume do
projeto, 7,9 GiB livres em `/`; nenhum build ou processo pesado ativo.
`timeout 180s cmake --build .local-build/native/linux-debug-juce --target
batruc_app --parallel 2` compilou sem erros. Executável:
SHA-256 `12dfffb46a6de722ca0f9b48b55f2f71fd1409f049c3b731f04f26c26ec3f1b1`.
Não rodei testes, não abri o app nem ouvi o áudio. Falta conferir com o usuário
os cliques, o sincronismo do baixo e o fechamento visual antes de aceitar estes
critérios.

### Seed vocal integrada ao calendário completo do baixo — 26/10/2026

Corrigido um erro estrutural nos dois alvos: no JUCE, ataques das quatro barras
eram projetados em uma grade de 20 passos e depois repetidos; na web, a seed
alterava alturas escritas, mas não usava os ataques vocais para formar o
calendário. Agora, cada seed calcula eventos de baixo pelos ataques vocais ao
longo dos quatro compassos, usando coordenadas x/y e intensidade para definir
alturas livres, dinâmica e intenção. BPM, compasso e parâmetros vocais
recalculam o baixo. Swing, pausas/frase, leis Fibonacci e geométrica e relação
com as vozes transformam a linha sem desligar a seed. Na página web, o gráfico
mostra o primeiro compasso do primeiro ciclo de mudança; editar uma célula
troca para o modo manual. No JUCE, o editor de uma barra é um resumo da agenda
completa de quatro compassos.

As edições manuais de células, repetição, intenção e duração removem o vínculo
da seed antes de recompor o calendário. No JUCE, o renderer troca a agenda no
limite do próximo compasso e solta a nota ativa pelo envelope. Arquivos web
antigos sem marca do novo modo continuam usando a grade manual.

Preflight antes da compilação final: 7,3 GiB disponíveis, swap 165 MiB/2 GiB,
983 GiB livres no volume do projeto, 7,9 GiB livres em `/`; sem processo pesado
ativo. `batruc_app` compilou com dois jobs. Hash SHA-256 final:
`a77ee24be7e3792ab9e9c31d5044b939a0eb4fb8a3fa68dfb9f3684d6fde6c07`.
`node --check prototype/score.js` e `node --check prototype/app.js` passaram.
Não rodei testes automatizados, não abri a interface nem ouvi o áudio. A
sincronia, os cliques e o layout precisam ser conferidos em execução.

### Arco de fase formal sonoro no JUCE — 26/10/2026

O controle de fase existente também define a profundidade do arco formal para
V2–V12: C1–C2 mantêm a fase-base; C3–C4 avançam; C5 sustenta; C6–C7 retornam.
O renderer resolve os ataques com a inclinação da fase ao longo do ciclo, e a
intensidade usa o estado atualizado. A fase-base escalonada e o jitter
determinístico por voz agora correspondem à regra web; o mapeamento seed→receita
e fase global ainda diferem. O desenho nativo ainda não acompanha o arco.

Build local: preflight com 7,3 GiB disponíveis, swap 12 MiB/2 GiB, 983 GiB
livres no volume do projeto, 7,9 GiB em `/` e nenhum processo pesado; alvo
`batruc_app` compilou com dois jobs. Binário SHA-256
`c4512121368f40a18541f278d3119086b105c292de08a1f73edf047b9d12939c`.
Não rodei testes automatizados, nem abri ou ouvi o app. Próximo: portar o arco
ao desenho das curvas/órbitas e validar som e ataques com o autor.

### Estimativa da paridade global antes da variação de seed — 26/10/2026

Pela matriz ponderada após o controle de variação de seed, a estimativa
provisória é **65,1% alcançado e 34,9% a fazer**. Os pesos, notas e rubrica estão em
[`PARIDADE_HTML_JUCE.md`](PARIDADE_HTML_JUCE.md); a nota inclui diferença de
comportamento e validação pendente, não apenas presença de controles.

### Apagar cena do banco local JUCE — 26/10/2026

O banco local JUCE agora tem uma ação para apagar uma cena arquivada, em
paridade com o navegador: escolher a cena no submenu e confirmar antes da
exclusão permanente. O código valida que o arquivo continua dentro da pasta do
banco selecionado e atualiza a lista depois da remoção.

Preflight: 7,7 GiB disponíveis, swap 12 MiB/2 GiB, 983 GiB livres no volume do
projeto, 7,9 GiB livres em `/` e nenhum processo pesado. O alvo `batruc_app`
compilou em 26/10/2026 com dois jobs. Binário SHA-256
`78f77dc14ffd4fa4ef2decb1fbb0280e649b7864a5c612f79bf3ceab6418e649`.
`git diff --no-index --check` não apontou whitespace. Não abri a interface nem
apaguei arquivos; falta validar a confirmação, remoção e atualização da lista
em execução.

### Modo finito ou loop para a forma — 26/10/2026

O transporte JUCE ganhou botão `7×/∞`: uma passada finita encerra depois de
28 compassos (sete ciclos de quatro compassos); loop contínuo segue como opção
de performance. Ao terminar a passada, a gravação ativa é finalizada. Iniciar
novamente no modo finito reinicia o transporte no começo da forma. O HTML já
faz uma passada finita; portanto, a opção JUCE acrescenta equivalência e também
mantém o fluxo performático existente.

Preflight: 8,2 GiB disponíveis, swap 12 MiB/2 GiB, 983 GiB livres no disco do
projeto, 7,9 GiB livres em `/` e nenhum processo pesado. `timeout 180s cmake
--build .local-build/native/linux-debug-juce --target batruc_app --parallel 2`
compilou sem erros. Binário de 26/10/2026, SHA-256
`74f056d7284046c70633cfe77f082b6c84d3af24657a9279d12972266495137d`.
Não rodei testes automatizados nem abri a interface. A parada automática,
reinício e finalização de gravação precisam de conferência manual antes de
considerar a paridade desta operação validada.

### Centro do trecho do gráfico JUCE e UTF-8 confirmado — 26/10/2026

O autor confirmou que a correção de acentuação UTF-8 da aba do baixo resolveu o
problema. A validação anterior compilou o binário novo; esta sessão registra a
confirmação e segue para outra lacuna de paridade global.

O gráfico nativo agora oferece um slider para escolher o centro do trecho
visível. Ele acompanha zoom por roda/botões, arraste e reset. Mover o slider em
zoom 1× abre a visualização em 2×, como na página web. Rótulo e dica acompanham
os quatro idiomas. A documentação de paridade agora reconhece que o cálculo e
a visualização Fourier existem nas duas saídas; falta comparar os controles e
a leitura didática entre elas.

Preflight antes do build: 9,2 GiB disponíveis, swap em 12 MiB/2 GiB, 983 GiB
livres no disco do projeto, 7,9 GiB livres em `/` e nenhum processo pesado.
`timeout 180s cmake --build .local-build/native/linux-debug-juce --target
batruc_app --parallel 2` compilou sem erros. Binário de 26/10/2026, SHA-256
`4b549a5c905260056db36ba8cc8e58387e2f96129ba1da2796bbe554cbfb1b7c`.
`git diff --no-index --check` não apontou whitespace nos três arquivos
alterados. Não rodei testes automatizados nem abri a janela; falta confirmar o
slider em execução e comparar sua interação com a página web.

### Correção UTF-8 na aba do baixo — 26/10/2026

Corrigi conversões implícitas de `const char*` para `juce::String` nos rótulos
e opções portuguesas da aba do baixo. Repetições, intenção, lei de duração,
razão geométrica, frase e matéria sonora agora usam `utf8(...)`; também apliquei
a conversão explícita a `×`, `φ`, `Acento`, `Geométrica`, `Respiração` e
`Cadência`. Os textos que passam por `localized(...)` já usam o helper UTF-8.

Validação: preflight com 6,6 GiB disponíveis, 8,8 MiB de swap usada/2 GiB,
983 GiB livres no disco do projeto, 8,0 GiB livres na raiz e sem processo
pesado visível. `timeout 180s cmake --build
.local-build/native/linux-debug-juce --target batruc_app --parallel 2` compilou
sem warnings. Executável de 26/10/2026, SHA-256
`70f7de64c839737b29497cd432a6d9fef2b4af5a302cd4ec64fbd240f4c68506`.
Ainda falta validar visualmente a aba no binário novo; não abri a interface.

### Paridade integral HTML/JUCE — escopo atualizado, 26/10/2026

A meta é a paridade de todo o instrumento, não somente do baixo. A matriz
[`PARIDADE_HTML_JUCE.md`](PARIDADE_HTML_JUCE.md) passa a organizar voz e
fórmulas, ritmo, forma, geometria, mixer, gravação/arquivo e conteúdo didático.
Ela registra o que já está presente em ambos, o que existe apenas em uma saída
e o que ainda precisa de conferência funcional, visual ou auditiva. Não atribuir
percentagem enquanto cada área não tiver peso e critério de verificação.

Nesta fatia, alinhei no JUCE a amplitude vertical do gráfico temporal à faixa
disponível na web (0,5×–3×); curvas e marcadores acompanham o ajuste, sem mudar
o áudio. Também alinhei os passos de edição de 13 parâmetros de áudio do baixo
com os passos usados no HTML e ajustei as casas decimais exibidas para refletir
essa resolução. O baixo permanece uma área da matriz geral, não o critério
isolado de conclusão.
`batruc_app` compilou com sucesso em 26/10/2026. Não abri a interface nem fiz
comparação visual/auditiva.

Lacunas globais já identificadas para orientar as próximas fatias: progressão
gradual de fase na forma de sete ciclos no JUCE, comparação detalhada do estudo
de Fourier/epiciclos entre as saídas, equivalência das
operações de arquivo local e gravação, e inventário tela a tela de
Learn/tutorial/Sobre/idiomas. A fila
integral e os estados estão em
[`PARIDADE_HTML_JUCE.md`](PARIDADE_HTML_JUCE.md).

### Forma de sete ciclos atuando no JUCE — 26/10/2026

Portada ao renderer JUCE a primeira camada sonora da forma `2+2+1+2`, com um
ciclo nativo definido como quatro compassos. A entrada de vozes acompanha o
HTML: C1 e C5 tocam somente V1; C2 e C6 tocam `ceil((N+1)/2)` vozes; C3, C4 e
C7 usam todas as vozes escolhidas. O baixo respeita os silêncios de C1 e C5.
O JUCE repetia os sete ciclos para performance, enquanto o estudo HTML tocava
uma passada finita. O botão `7×/∞` passou a oferecer ambas as opções na versão
nativa, conforme a atualização acima. A faixa inferior e o status nativos
identificam a seção e o ciclo atuais. A curva gradual de fase entre mudança e
retorno ainda está pendente.

Arquivos alterados: `src/core/CompositionCore.hpp`,
`src/core/CompositionCore.cpp`, `src/core/BassAudio.cpp`, `src/app/Main.cpp`,
`PARIDADE_HTML_JUCE.md` e este registro.

Validação: preflight final antes do build com 6,6 GiB disponíveis, swap em
6,3 MiB/2 GiB, 983 GiB livres no disco do projeto, 8,0 GiB livres na raiz e
sem processos pesados. `timeout 180s cmake --build
.local-build/native/linux-debug-juce --target batruc_app --parallel 2` concluiu
sem warnings na compilação final. Não rodei testes automatizados nem abri a
interface ou escutei a saída. Executável de 26/10/2026, SHA-256
`b8858c26a12d7157d49a45fc69d7e23229679b80e927f5c202dadca7dda576ec`. Próximo:
conferir o ciclo formal em execução e
portar a trajetória de fase; depois avançar em arquivo/gravação e Fourier.

### Primeiro commit do Batruc no repositório central privado, 25/10/2026

Por autorização expressa do autor, a fonte do Batruc passa a ser versionada
no repositório central privado `lucioaraujo/rasgo-instruments`. O commit
abrange código, protótipo, licença e documentação do projeto; exclui
`.local-build/`, gravações e outros derivados locais. Esta etapa não representa
lançamento público nem autoriza distribuir áudio ou binários. A exclusão do
diretório-fonte foi removida do `.gitignore`; builds locais permanecem fora do
controle de versão. Commit inicial `b663d66`, enviado em 25/10/2026 para a
branch `batruc/initial` de `lucioaraujo/rasgo-instruments`. O push não incluiu
os dois commits locais de Maravi nem alterou `main`.

### Controle “Centro do trecho” no HTML, 25/10/2026

O slider ficava desabilitado no zoom inicial de 1×, pois o ciclo inteiro já
estava visível e deslocar a janela não teria efeito. Isso fazia o controle
parecer permanentemente indisponível. Ele agora permanece ativo: ao mover o
centro com zoom 1×, o gráfico passa automaticamente a 2× e enquadra o trecho
escolhido. Alterar o zoom manualmente continua permitindo o ajuste fino do
centro. `node --check prototype/app.js` passou; não houve teste de navegador.

### Gráfico JUCE acompanha as transformações das vozes, 25/10/2026

O gráfico temporal nativo desenhava curvas senoidais fixas, mesmo quando a
voz selecionada usava inversão, retificação, combinação harmônica, quantização
ou outra transformação. Agora a curva e os marcadores de ataque são calculados
com a transformação da voz; o desenho inclui deslocamento de fase, acoplamento
com a voz anterior e, para V1, a curva geométrica escolhida. A avaliação é
limitada aos pontos já usados para desenhar as linhas, na thread da interface;
nenhum cálculo foi movido para o callback de áudio. O desenho não tenta
representar a resposta acústica modal, que continua separada da função.

Validação: preflight em 25/10/2026, 14 GiB disponíveis, swap sem uso, 983 GiB
livres no volume do projeto e nenhum processo pesado. O alvo JUCE
`batruc_app` compilou com dois jobs. SHA-256:
`ad7bb58af52f3144f22b73bf412d0acbdd6a8433c3c7077b25b6c36dbae9bb0a`.
Não executei testes nem abri a janela; falta conferir a leitura visual das dez
formas e ouvir o efeito das transformações. Próximo: validar essa visualização
e continuar a paridade dos controles rítmicos/expressivos.

### Paridade de transformações matemáticas no HTML e JUCE, 25/10/2026

A maquete HTML recebeu as quatro transformações que já existiam no JUCE:
`sin³(θ)`, terceiro harmônico `sin(3θ)`, triangular `(2/π)·arcsen(sen θ)` e
saturação suave `tanh(2·sen θ)/tanh(2)`. A lista de transformações usada pela
variação determinística da seed também foi ampliada; cenas antigas continuam
usando seus identificadores existentes. A fórmula aplicada aparece na prévia
de cada voz. Nos dois alvos, essas transformações alteram o valor/intensidade
da voz, não o instante dos ataques. `node --check` passou para `score.js` e
`app.js`, e `node prototype/test-score.cjs` passou com novos casos que conferem
os quatro resultados matemáticos em `sin(θ)=0,5`. Não houve abertura visual da
página nem escuta comparativa. Próximo: validar a interface no navegador e
avançar para paridade dos controles de compasso/expressão, sem afirmar
equivalência sonora antes de comparação por escuta.

### Renomeação de Trucmetry para Batruc, 25/10/2026

Por decisão do autor, **Batruc** passa a ser o nome atual do instrumento,
aplicativo e diretório. **Trucmetry** permanece como nome histórico do mesmo
projeto. Foi preservada uma cópia integral anterior à migração, incluindo a
cache de build, em
`RASGO/RASGO_ARQUIVO/.local-preservation-20261025/TRUCMETRY-pre-rename/`
(807 arquivos; cópia conferida por comparação de diretórios). A fonte ativa
agora está em `RASGO/PROJETOS_AUTORAIS/BATRUC/`.

Foram atualizadas as referências ativas no código, interface, documentação,
protótipo, CMake, nomes de targets, identificadores de cenas, filtros de
arquivos, diretórios de projeto e índices RASGO. Novas cenas usam
`.batruc.txt`/`batruc-scene`; a aplicação e a maquete continuam aceitando
cenas antigas `.trucmetry.txt` e os formatos legados `trucmetry-scene` e
`trucmetry-composition`. Arquivos existentes não foram renomeados. Pesquisa
web preliminar não localizou outro instrumento ou software musical chamado
Batruc, mas não constitui busca jurídica de anterioridade; a disponibilidade
de marca ainda precisa de verificação oficial antes de publicação.

Validação da migração: a cache ativa antiga foi removida após a cópia integral
ser conferida; CMake foi reconfigurado do zero com JUCE 9.0.3 e o alvo
`batruc_app` compilou e ligou com sucesso. Executável:
`.local-build/native/linux-debug-juce/batruc_app_artefacts/Debug/Batruc`,
SHA-256 `ffb66ef733c6f56f9c989c45e5aed63e5543db48a918ad3c1edd0059b6771752`.
O leitor JUCE e a maquete web aceitam os formatos legados; as novas gravações
usam somente o nome Batruc. A revisão textual encontrou “Trucmetry” apenas
como histórico e nos pontos de compatibilidade. Ainda falta abrir a interface
e confirmar visualmente o nome atual. Nenhum outro projeto RASGO foi alterado. O repositório RASGO
já tinha mudanças não relacionadas e o diretório do instrumento é ignorado
pelo Git do contêiner; nenhuma delas foi revertida.

### Continuação — leitura nominal das alturas do baixo, 25/10/2026

O editor JUCE agora mostra o nome da nota e seu número MIDI no passo
selecionado (por exemplo, C2 / MIDI 36). Passos vazios são identificados como
pausas; altura e duração ficam desativadas enquanto não houver nota. O gráfico
do LFO compara a nota escrita com a altura calculada na fase atual, incluindo
arredondamento para semitom e os limites MIDI do renderer. Labels de passo e
pausa estão localizados para os quatro idiomas. A implementação compilou com
sucesso em `batruc_app`; SHA-256:
`e15b7e13a521aeab67fa0b670e1da3d9f02b798e4908078265257ed9cc50a125`.
Não houve teste automatizado, abertura da interface nem escuta; conferir
visualmente os nomes das notas e ouvir o LFO ainda é necessário.

Pesquisa preliminar do nome **Batruc**: buscas exatas e combinadas com música,
instrumento, sintetizador e software não localizaram instrumento musical ou
software de música com esse nome. Surgiram usos não musicais, entre eles um
clube de bilhar chamado Le Bar Batruc, listado pela
[Federação Francesa de Bilhar](https://m.ffbillard.com/clubs/le-bar-batruc-02122.html),
e uma ocorrência histórica de Batruc como local no atual Vietnã, em um
[artigo sobre representações coloniais de instrumentos](https://musicweb.ucsd.edu/~jpasler/wp/wp-content/uploads/2012/06/Pasler-JRMA-2004.pdf).
É uma busca web preliminar, não busca de anterioridade nem liberação de marca.
O nome atual do projeto é Batruc. Antes de publicação, verificar marcas
idênticas e semelhantes nas bases oficiais pertinentes; a
[INPI explica a diferença entre busca idêntica e busca de similaridade](https://www.inpi.fr/ressources/propriete-intellectuelle/rechercher-une-marque-base-marques).

### Continuação — ritmo corporal, batuque e paridade JUCE/HTML, 25/10/2026

Pesquisa aprofundada sobre “batuque” registrada no README: o termo tem usos
musicais amplos, mas também designa tradições afro-brasileiras específicas e
vivas, e sua história inclui usos de racialização sonora. O instrumento não
tratará toques religiosos como presets genéricos. A pesquisa composicional
seguirá por mecanismos observáveis — acento, contratiempo, densidade, pausas,
intertravamento, chamada/resposta, memória e microtiming — com diálogo situado
se um contexto cultural identificável vier a ser representado. Fontes
acadêmicas estão ligadas no README.

Na paridade com a maquete HTML, foi portada a primeira camada do LFO de altura
do baixo (liga/desliga, forma triangular/senoidal/quadrada, período de 1/2/4
ciclos da forma e profundidade de até 12 semitons), com persistência em cenas
de esquema 11. A revisão do código encontrou e corrigiu um erro: a nota MIDI
modulada era registrada, mas a frequência fundamental ainda era calculada a
partir da nota original; agora o oscilador recebe a altura modulada. Corrigida
também a tradução espanhola de “Triangular”.

Preflight para a compilação: 14 GiB disponíveis, swap 0/2 GiB, 984 GiB livres
no volume do projeto, montagem `rw` e nenhum build/render pesado identificado.
Próximo passo imediato: compilar somente `batruc_app` com dois jobs e timeout;
depois registrar o resultado. Não abrir a janela nesta etapa. Permanecem
pendentes escuta A/B pelo autor, desenho visual do LFO no gráfico e comparação
das formas de onda/timbres entre JUCE e HTML.

Validação executada: `timeout 180s cmake --build
.local-build/native/linux-debug-juce --target batruc_app --parallel 2`
concluiu com sucesso em cerca de 9 s. O executável recompilado está em
`.local-build/native/linux-debug-juce/batruc_app_artefacts/Debug/Batruc`,
SHA-256 `e9b7595ddd3ac77cc8b6a93a3c9d2ba71f17a99ef29f6f62425a173314041407`.
Depois do build havia 14 GiB disponíveis e swap continuava em 0 B. A verificação
de espaços em branco não encontrou ocorrências. Não executei testes nem abri a
interface/áudio. O bug de frequência do LFO está corrigido em código e compilado,
mas a audibilidade e o comportamento musical ainda dependem de escuta manual.

### Continuação — desenho da fase do LFO no painel Baixo, 25/10/2026

Para aproximar a apresentação JUCE da maquete, o painel Baixo recebeu um
gráfico de modulação: curva triangular, senoidal ou quadrada; linha zero;
alcance em semitons; e cursor de fase baseado nos ciclos de forma concluídos e
no progresso da seção atual. A curva representa o deslocamento de altura por
ataque, não a oscilação acústica do áudio. Com o LFO desligado, a prévia continua
visível e aparece em estado de bypass, facilitando comparar configuração e
efeito. O indicador é atualizado pelo timer de interface, fora do callback de
áudio.

Alterados `src/app/Main.cpp`, README e este registro. Próximo: preflight de
recursos e compilação limitada do alvo `batruc_app`; sem abrir a aplicação,
testes ou áudio nesta etapa. Ainda não há confirmação visual do layout nem
escuta de que a fase percebida acompanha exatamente os ataques; verificar esses
aspectos manualmente na próxima sessão.

Preflight: 14 GiB disponíveis, swap 0/2 GiB, 984 GiB livres e volume de
trabalho montado `rw`; sem outro build/render pesado detectado. Validação:
`timeout 180s cmake --build .local-build/native/linux-debug-juce --target
batruc_app --parallel 2` concluiu com sucesso. SHA-256 do executável:
`256072beca79c4d058ce667c0842da2b9aa43582ae3caf7a73b0712babb1fc9d`.
Após o build, 14 GiB permaneciam disponíveis e a swap continuava sem uso. A
verificação de espaços em branco passou. Não executei testes nem abri a
interface; validação visual e auditiva seguem pendentes.

### Verificação dos relatos de volume, envelope e travamento, 25/10/2026

Os quatro testes locais existentes passaram (transporte/variantes, composição
do baixo, inversão e operações generativas). Uma renderização isolada de oito
segundos a 48 kHz, com os níveis e a receita iniciais de duas vozes, mediu RMS
0,01133 nas vozes de fórmula e 0,07983 no baixo: o baixo está aproximadamente
16,96 dB acima, antes do ganho master comum. O relato de desequilíbrio é
reproduzível no core.

Os controles ADSR-A chegam ao renderer por atomics e são lidos a cada bloco;
não encontrei falha de conexão. A limitação é que o envelope do filtro não é
independente: ataque = 0,7 × ataque de amplitude, decaimento = 1,35 ×
decaimento de amplitude, sustentação fixa em 0,12 e soltura ligada à de
amplitude. Assim, o ADSR-A altera sobretudo volume; falta ADSR-F editável para
controle tímbrico explícito. Sobre o congelamento ao mudar a forma, callbacks
da forma e da transformação são limitados e o teste de transporte passou; não
foi reproduzido na interface, pois a aplicação foi fechada a pedido do autor.
Sem execução de GUI, a causa do travamento permanece por confirmar.

### Continuação — trim de vozes e ADSR-F independente, 25/10/2026

Adicionado ao mixer um controle `TRIM VOZES (dB)`, de −12 a +24 dB, aplicado
somente ao barramento das fórmulas antes da soma com o baixo. O valor inicial
de +16,5 dB compensa aproximadamente o desequilíbrio de 16,96 dB medido com
duas vozes; não é uma calibração final para todas as quantidades/receitas.
O master agora usa saturação suave por `tanh` para conter picos da soma, então
o resultado pode comprimir em ajustes densos. Cenas avançam para esquema 9 e
guardam o trim, mantendo o padrão para cenas antigas.

O painel de baixo ganhou os quatro controles ADSR-F independentes. Ataque,
decaimento, sustentação e soltura chegam ao renderer pelo callback de áudio,
sem alocação; também ficam guardados nas cenas de esquema 9. Foram ampliados
os arrays de controles para 12 parâmetros. Ainda falta escuta humana para
ajustar o trim inicial e confirmar a resposta tímbrica; a interface não foi
aberta nesta etapa, e a causa do congelamento relatado segue não reproduzida.

Preflight antes da compilação: 15 GiB disponíveis, swap 0/2 GiB e 984 GiB
livres; sem compiladores/builds ativos detectáveis. Próximo passo: compilar o
alvo JUCE e executar os testes locais existentes; depois abrir o app para
escutar o equilíbrio com baixo e verificar a resposta independente do ADSR-F.

Validação desta etapa: o alvo `batruc_app` recompilou com dois jobs e os
quatro testes CTest passaram. A janela não foi aberta; portanto, disposição dos
novos controles, resposta ao ouvido e congelamento ao mudar a forma continuam
pendentes de teste manual. O projeto está ignorado pelo `.gitignore` do
container, então as mudanças estão apenas nos arquivos locais, sem commit.

### Pesquisa e primeiro núcleo de geometria triangular, 25/10/2026

Conferidas as relações matemáticas e seus domínios: Pitágoras (`a²+b²=c²`,
somente triângulo retângulo); Herón (`A=√(s(s−a)(s−b)(s−c))`); Stewart para
uma ceviana; Apolônio como caso mediano de Stewart; Ceva como produto de três
razões dos segmentos opostos igual a 1; reta de Euler com `O`, `G`, `H`
colineares e `OH=3·OG` em triângulo não equilátero. A terminologia de
Apolônio varia entre fontes. A denominação “teorema da escada” pode referir-se
a escadas cruzadas clássicas ou à versão estendida de Stengel para áreas de
regiões de cevianas; a fórmula precisa foi posteriormente especificada pelo
autor e está registrada na atualização abaixo.

Criado `src/core/TriangleGeometry.hpp/.cpp`: análise de triângulo por
coordenadas, área calculada também por Herón, erro de concordância das duas
áreas, lados, medianas de Apolônio, compacidade de área, desvio da relação de
Pitágoras, raios, centroide, circuncentro, ortocentro e resíduos da reta de
Euler. Funções independentes calculam comprimento de ceviana por Stewart e
erro logarítmico de Ceva. Entradas degeneradas/não finitas são rejeitadas. O
módulo é matemática de base; nenhum desses descritores está ainda conectado ao
áudio ou à interface JUCE.

Aplicações composicionais possíveis: área e sua variação para densidade/energia
de frase; mediana/ceviana para duração ou deslocamento; erro de Ceva para
tensão entre três trajetórias e seu encontro para acento, pausa ou mudança de
camada; movimento dos centros e erro da reta de Euler para fases e aproximação
de eventos. São relações composicionais editáveis, não equivalências físicas
ou musicais naturais. Referências verificadas: [Pitágoras e Herón — MathWorld](https://mathworld.wolfram.com/PythagoreanTheorem.html),
[Stewart — MathWorld](https://mathworld.wolfram.com/StewartsTheorem.html),
[Ceva — Encyclopedia of Mathematics](https://encyclopediaofmath.org/wiki/Ceva_theorem),
[reta de Euler — MathWorld](https://mathworld.wolfram.com/EulerLine.html),
[escadas cruzadas — MathWorld](https://mathworld.wolfram.com/CrossedLaddersTheorem.html),
[Routh e área do triângulo central formado por cevianas](https://archive.lib.msu.edu/crcmath/math/math/r/r402.htm).

Validação: `batruc_app` compilou; acrescentado teste do triângulo 3–4–5,
mediana, Stewart, Ceva e rejeição de degeneração. Cinco testes CTest passaram.
Próximo passo: acrescentar à interface um modo geométrico triangular que
mostre construção e grandezas, permita deslocar os pontos e selecione
explicitamente quais valores modulam ritmo, altura ou timbre; depois integrar
essa receita ao renderizador e às cenas.

### Correção do teorema da escada — áreas de Stengel

O autor especificou a configuração/forma algébrica usada:
`1/S + 1/z = 1/(x+z) + 1/(y+z)`, com `S=w+x+y+z`. Isso corresponde à versão estendida de
Stengel sobre áreas de regiões produzidas por cevianas, e não apenas ao
teorema clássico de comprimentos entre escadas cruzadas. Para regiões
positivas, o cálculo inverso é `q=1/(x+z)+1/(y+z)-1/z`, `S=1/q` e
`w=S−x−y−z`; o caso é válido para o módulo quando `q>0` e `w>0`.

Adicionadas ao core `stengelExtendedLadderFourthArea` e
`stengelExtendedLadderAreaError`. O teste usa `x=6, y=3, z=9`, obtém
`S=180/7`, `w=54/7` e verifica resíduo próximo de zero; também cobre valores
que não produzem uma quarta área positiva. A identidade foi confrontada com a
versão de áreas atribuída ao corolário de Stengel na configuração triangular.
[Enunciado da versão estendida e áreas](https://en.wikipedia.org/wiki/Crossed_ladders_problem#Extended_crossed_ladders_theorem),
[exemplo numérico correspondente](https://math.stackexchange.com/questions/5128041/what-is-the-area-of-the-triangle-formed-by-three-cevians-when-three-other-areas).
O teste não confirma por si só a geometria de uma disposição desenhada; a
interface ainda precisa representar as quatro regiões e sua incidência.

Validação depois da correção: alvo JUCE compilado; os cinco testes CTest
passaram. Próxima etapa: acrescentar a construção triangular visual com os
quatro campos de área, recalcular `w` ao editar `x,y,z` e oferecer a área
resultante como fonte composicional selecionável, sem ativar mapeamento de som
por padrão.

### Continuação — painel matemático e pesquisa Pascal/Fibonacci/Fourier, 25/10/2026

Iniciado o painel flutuante `Geometria` no host JUCE. Três controles editam
`x,y,z`; o core existente recalcula a quarta área positiva `w`, o total `S` e
o resíduo da identidade estendida de Stengel. O painel apresenta a equação e
barras proporcionais de áreas, explicitamente identificadas como comparações
de magnitude, não como desenho ou validação da disposição das cevianas. Não há
roteamento para áudio nesta etapa. Próximo passo geométrico continua sendo
obter/configurar a incidência exata antes de desenhar a construção espacial.

Pesquisa registrada em `BIBLIOGRAFIA_DE_ESTUDO.md`: diagonais rasas de Pascal
somam Fibonacci; coeficientes binomiais normalizados geram filtros FIR
binomiais, úteis mas não “perfeitos”; a onda triangular simétrica tem
harmônicos ímpares com coeficientes alternados proporcionais a `1/n²`; quatro
Fibonacci consecutivos satisfazem a identidade para os catetos
`sqrt(F_n·F_(n+3))`, `F_(n+1)` e hipotenusa `F_(n+2)`. A razão áurea entre LFOs
impede repetição periódica exata ideal, mas não elimina interferência nem
garante organicidade. Essas relações ficam como três receitas independentes a
experimentar (acentuação Pascal, suavização FIR e parciais de Fourier), fora
do callback e com bypass/A-B; não serão combinadas automaticamente.

Arquivos desta continuação: `src/app/Main.cpp`, `README.md`,
`BIBLIOGRAFIA_DE_ESTUDO.md` e `TAREFAS.md`. O alvo JUCE compilou com dois jobs
e os cinco testes CTest passaram, incluindo o teste de geometria. Preflight:
14 GiB disponíveis e swap zerada antes da compilação; após o build, 13 GiB
disponíveis e swap zerada. Não abri a janela nem escutei áudio; traduções,
disposição visual e recálculo ao manipular os controles ainda exigem
verificação manual.

### Princípio composicional — trigonometria e matemática generativa, 25/10/2026

Orientação do autor: trigonometria e outros ramos da matemática também devem
servir para efetuar cálculos necessários a modelos geométricos e físicos, além
de apoiar composição generativa. Seus resultados podem transformar ou organizar
eventos, células, ritmos, alturas, timbres ou forma; fórmula/gráfico exibidos
sem função calculável ou composicional não bastam. Cada mecanismo deve ser
inspecionável, declarar os limites do modelo e permitir comparação audível/
bypass. O cálculo físico descreve o modelo adotado; a relação com música é uma
decisão composicional. Esse critério orientará a integração futura de Stengel,
Pascal/Fibonacci, Fourier, trigonometria e modelos de física. Nenhum mapeamento
será ativado por padrão sem escolha explícita.

### Continuação — percursos geométricos e Fourier no JUCE, 25/10/2026

Portadas para o core nativo as funções geométricas calculáveis da maquete:
compasso, triângulo, quadrado, espiral, Lissajous e aproximações Fourier de
triângulo/quadrado. O núcleo usa coeficientes discretos sobre 2048 amostras,
`H=1..32` e calcula o erro RMS relativo em 512 amostras, como conceito e
normalização da maquete web. O painel `Geometria` agora alterna entre o cálculo
de áreas Stengel e `Curvas · Fourier`, desenhando a trajetória e apresentando
`H` e erro. Uma caixa opcional agora usa `y(t)` como fonte da fórmula de
intensidade da voz 1, com ativação/curva/ordem guardadas em cenas JUCE esquema
10. As cenas 1–9 mantêm a configuração padrão (fonte geométrica desligada).
O banco Fourier é inicializado no caminho de controle antes de ser consultado
no callback de áudio.

Arquivos: `src/core/GeometryCurves.hpp/.cpp`, `tests/geometry_curves.cpp`,
`CMakeLists.txt`, `src/app/Main.cpp`, `README.md` e `TAREFAS.md`. O teste novo
verifica pontos de compasso/polígonos, entradas fora de faixa e redução do erro
ao aumentar `H`, e confirma saída de áudio diferente com a fonte V1 ligada. Build JUCE e os seis testes CTest passaram; preflight: 14 GiB
disponíveis e swap zerada antes, 13 GiB e swap zerada após. Não abri a janela.
Limite e próximo passo de paridade: a opção explícita usa `y(t)` como fonte da
intensidade calculada de V1, mas os ataques ainda seguem a regra senoidal
nativa. Recalcular seus cruzamentos/extremos e aplicar alterações de
forma/ordem/ciclos/fase em fronteira segura, sem trabalho Fourier no callback.
Depois portar a dinâmica pendular e os controles físicos do estudo web.

### Continuação — detecção de eventos em percursos geométricos, 25/10/2026

O core agora calcula posições normalizadas aproximadas dos cruzamentos de
`y(t)=0` ou dos extremos locais de um percurso, em buffer fixo e sem heap. O
detector usa 4096 amostras por volta e remove duplicatas na emenda circular;
portanto, é uma estimativa numérica para marcar candidatos, não uma solução
analítica dos zeros. Testes cobrem os dois eventos do compasso, quatro
cruzamentos e quatro extremos de Lissajous, ordenação implícita e rejeição de
buffer insuficiente.

O renderer agora substitui os ataques preparados de V1 pelos cruzamentos ou
extremos dessa tabela, conforme a regra Centro/Extremos. Posições e coordenadas
XY dos candidatos são calculadas na thread de interface, transferidas em
buffers atômicos com sequência de publicação e copiadas pelo áudio na próxima
barra. Assim, as máscaras espaciais e leis de intensidade também recebem o
ponto geométrico do evento. Ciclos e fase continuam ajustáveis no scheduler. A
espiral permanece com ataques regulares e `y(t)` como acento, explicitado no
texto dinâmico do botão. O índice de ciclo do scheduler é monotônico para não
repetir um evento quando o arredondamento cai entre amostras.

Arquivos afetados nesta etapa: `src/core/GeometryCurves.hpp/.cpp`,
`src/core/CompositionCore.hpp/.cpp`, `src/app/Main.cpp`,
`tests/geometry_curves.cpp`, `README.md` e este registro. O build JUCE com dois
jobs passou. A suíte de seis testes passou antes do último ajuste de transporte
XY; não foi reexecutada após esse ajuste. O teste de áudio cobre o deslocamento
do primeiro ataque triangular. Não foi aberta a interface para escuta ou
conferência visual. Próximo passo: validar manualmente as curvas e a entrada de
Centro/Extremos na próxima barra, comparar a animação com a agenda audível e
então avançar para a dinâmica pendular do estudo web.

### Máquina inicial de polirritmia e novos materiais de movimento, 25/10/2026

O Mixer JUCE ganhou um seletor/aplicador de relações de ciclos para duas a
cinco vozes: `4:3`, `5:4`, `6:4`, `7:4`, `8:6`, `10:4`, `10:7`, `2:3:5`,
`3:4:5`, `4:5:7`, `3:4:5:7` e `3:4:5:7:8`. Aplicar uma relação escreve os
ciclos nos primeiros canais, ativa pelo menos a quantidade de vozes necessária
e deixa os outros canais intactos. Os controles individuais continuam
disponíveis em V1–V12. A interface esclarece que esses números são ciclos por
seção de 16 tempos/quatro compassos, não contagens literais de notas; cada ciclo
gera dois candidatos antes de máscaras e regras. Relações equivalentes como
`4:3`, `6:4` e `8:6` podem ter densidades diferentes. Reprodução aplica os novos
ataques na próxima barra.

A documentação de movimento agora formaliza epicíclos Fourier, com
`z(t)=Σcₙe^{inω₀t}`, raio/fase/velocidade angular por componente e equações de
velocidade/aceleração; distingue coeficientes de reconstrução de novos pesos
generativos. Também separa receitas Fibonacci, espiral de Fibonacci e espiral
áurea, e registra transformações por matrizes móveis, iteração fractal,
comprimento de traço, área orientada e volume por secções. São propostas de
pesquisa, ainda não controles implementados no JUCE.

Arquivos desta etapa: `src/app/Main.cpp`, `README.md`,
`MOVIMENTO_GRAFICO_E_SOM.md` e este registro. Build do app JUCE passou com dois
jobs após preflight de 14 GiB disponíveis, swap zerada e 984 GiB livres no
volume de trabalho; o destino de backup estava somente leitura e não foi usado.
Não rodei testes nem abri a GUI nesta etapa. Próximo passo: validar a máquina
com escuta e desenho de ataques em presets de 2–5 vozes; depois desenvolver a
visualização animada dos epicíclos e conectar medidas do traço às operações
composicionais sem perder limites claros para os cálculos.

### Forças, materiais e ruptura como processo composicional, 25/10/2026

O autor ampliou o campo físico para molas/elásticos, pêndulos, alavancas,
pressão, empuxo, magnetismo, força/peso, radiação, deformação, transferência
de forças e movimentos, consequências de ações, ruptura/rasgo e limites. O
README agora conecta esses conceitos como uma cadeia possível de ação,
resultante, deformação/movimento, armazenamento ou perda de energia, limiar,
ruptura/liberação e consequência. Distingue pressão de tensão, massa de peso,
emissão acústica de radiação eletromagnética e modelo físico de analogia
composicional; traz relações de referência, condições de uso e limites do
mapeamento musical.

Micropatterning, lembrado pelo autor, permanece ligado a esta pesquisa. A
referência ibidi e seus limites estão no Atlas; a primeira prova JUCE já usa
superfícies de afinidade binárias (campo livre, ilhas e microlinhas) para
selecionar ataques. Próxima extensão a estudar: zonas desenháveis e campos de
afinidade graduais, com tamanho/espaçamento editáveis, e sua interação com
trajetórias, deformações, limiares e consequências de eventos. Manter a adesão
celular como proveniência conceitual; não declarar simulação biológica.

A governança RASGO §8.2 agora registra o aprofundamento conceitual como método
transversal: fonte/proveniência, problema, derivação, unidades e hipóteses,
validade, processo, consequência composicional e validação, preservando a
interpretação local de cada instrumento. Arquivos alterados: README,
`MOVIMENTO_GRAFICO_E_SOM.md`, `TAREFAS.md` e `RASGO_DOCUMENTATION/GOVERNANCA_E_TRANSVERSALIDADE.md`.
Validação documental; não houve mudança de código, build ou teste. Próximo
passo de pesquisa/código: selecionar um sistema acoplado limitado, por exemplo
massa–mola–amortecedor com força aplicada, e implementar primeiro estado,
transferência e energia com unidades; depois estudar limiar de falha/histerese
e seus efeitos em ataque, pausa, timbre e forma. Pressão, empuxo e magnetismo
ficam para modelos próprios com geometria e condições explícitas.

### Continuação — edição nativa da linha de baixo, 25/10/2026

O painel JUCE `Baixo` agora traz uma grade de 16 passos. Cada célula pode ser
ativada/desativada; a célula selecionada permite editar altura MIDI (24–60) e
duração (1, 2, 4, 8 ou 16 subdivisões). A edição recalcula `BassScore` e
`BassPerformance` na thread da interface e publica um snapshot limitado para
o callback de áudio, que troca a performance na fronteira do bloco sem
alocação ou preparação musical. Cenas avançaram para esquema 8 e guardam a
receita das células, lei de duração, frase, seed e variação, além dos controles
de áudio já incluídos no esquema 7; os esquemas 1–7 continuam aceitos. O alvo
JUCE compilou localmente sem avisos após corrigir comparações de ponto
flutuante. Preflight do build: 13 GiB disponíveis, swap 0/2 GiB e 984 GiB
livres. Não rodei testes nem abri a interface/áudio. Próximo passo: validar
na aplicação edição durante reprodução, continuidade da troca de performance
e ciclo salvar/abrir de cenas.

### Continuação — persistência do módulo de baixo nas cenas, 25/10/2026

O esquema de cena JUCE avançou de 6 para 7. Captura, exportação, gravação e
restauração agora incluem ativação, Mute/Solo e os oito parâmetros do baixo
(nível, ADSR-A, cutoff, envelope do filtro e gate). A leitura valida cada valor
contra o intervalo do controle. Cenas de esquemas 1–6 mantêm compatibilidade e
usam os valores padrão do baixo. A próxima validação é compilar o alvo JUCE e,
na instância recém-compilada, salvar/abrir uma cena para conferir a restauração
visual e sonora; teste de interface e áudio ainda não executado.

### Continuação — resposta ao teste de ADSR e solo do baixo, 25/10/2026

O autor relatou que ADSR não parecia alterar o som e que o baixo seguia tocando
ao colocar V1/V2 em solo. Causa confirmada no código: os gates da linha-base
eram contíguos e o release era reiniciado pelo próximo ataque; além disso, o
baixo não participava do cálculo de mute/solo do mixer. O renderer agora reduz
o gate por uma proporção editável (40–100%, inicial 86%) para deixar uma janela
de soltura; o painel do baixo ganhou Mute e Solo. A lógica de solo reúne o baixo
e as doze vozes: solo em uma voz silencia o baixo, e solo também no baixo permite
ouvir ambos. Os controles de amplitude/filtro continuam sendo lidos a cada
bloco. Build do alvo `batruc_app` passou com dois jobs; não rodei testes nem
escuta automatizada. Próximo passo é confirmar estes dois comportamentos na
instância nova e ajustar a curva pelo ouvido do autor.

### Continuação — primeira voz de baixo no host JUCE, 25/10/2026

Implementado o primeiro caminho de áudio JUCE para o baixo. O app prepara o
score/performance fora do callback a partir de uma célula de 16 passos repetida
por quatro compassos; `BassAudioRenderer` recebe a performance fixa e a
sincroniza com o progresso e andamento ativos do transporte principal. A voz
monofônica usa quatro parciais senoidais, VCA controlado por ADSR-A, VCF
passa-baixas de um polo e envelope de filtro separado. O painel flutuante
`Baixo` expõe ativação (desligada por padrão), nível, ataque/decaimento/
sustentação/soltura de amplitude, cutoff e intensidade do envelope do filtro.
O envelope de filtro ainda deriva ataque/decaimento de ADSR-A, tem sustentação
fixa e compartilha o release: os controles não representam dois ADSRs completos.

Arquivos: `src/core/BassAudio.hpp/.cpp`, extensão de `BassPerformance` com a
duração de seção, `src/app/Main.cpp` e `CMakeLists.txt`. Nenhum código irmão
foi copiado. A compilação local do alvo JUCE `batruc_app` passou com dois
jobs; não rodei testes nem abri a janela/áudio. Preflight antes da compilação:
14 GiB disponíveis, swap 0/2 GiB, 984 GiB livres; durante a compilação,
disponibilidade observada em 13 GiB. Limitações: linha inicial fixa baseada
na maquete, sem edição de células no JUCE, sem persistência dos controles em
cenas, sem LFO e sem validação auditiva. Próximo passo: ligar o editor de
células/seed à preparação de `BassPerformance` sem trabalho no callback e
guardar os parâmetros em cenas; então testar a janela, nível e articulação ao
ouvido.

### Continuação — referências de percussionistas e intenção, 25/10/2026

Foi checada a nova lista de referências. Naná Vasconcelos documenta em
entrevistas a exploração de timbre/ritmo do berimbau, voz e corpo; Milford
Graves documenta sonificação de dados de batimento cardíaco; Djalma Corrêa
documenta experimentação eletroacústica, objetos, pesquisa de campo e acervo;
Airto Moreira relata fontes sonoras encontradas e objetos. Cooper/Meyer cobrem
acento, stress, agrupamento, ambiguidade e transformação. As fontes e os
limites das atribuições foram incluídos em `BIBLIOGRAFIA_DE_ESTUDO.md`.

Correção de escopo: a lista recebida mistura documentação biográfica com
propostas algorítmicas próprias. Não se confirmou uma teoria de groove/VFC de
Graves, micro-atraso de Naná determinado por velocity, “aquecimento” material
por densidade, nem pan/reverb de Airto derivados da nota anterior. São hipóteses
para experimentos do Batruc, não declarações dos músicos nem simulações
físicas validadas. Próximo estudo: criar comparações reproduzíveis A/B entre
pulso estável e elasticidade editável por voz/frase, e entre corpos sonoros sob
a mesma célula; expor seed, limites e causa de cada transformação.

### Continuação — mapa dos módulos de baixo da família RASGO, 25/10/2026

O autor redirecionou a pesquisa para os módulos que já existem na família,
antes de criar uma nova cadeia DSP. A inspeção local encontrou no Rasgo Synth
Studio a separação função/frase/performance/corpo (`BassFunctionPlan`,
`BassPhrasePlan`, `BassPerformancePlan`, `BassBodyAdapters`), a interação
baixo-bumbo, `AcidBasslineVoice` (oscilador + Polivoks, decaimentos separados,
acento e slide), `BassPhraseTimbre` (espectro por frase/transiente), ADSR, LFOs,
bus de modulação e filtros Moog/Polivoks. Parte da validação estrutural existe,
mas coortes e aceitação auditiva permanecem pendentes no Studio. Rasgo Modular
traz envelope AD/ASR com VCA embutido e saída CV, filtro SVF multi-saída e
FunctionGenerator; Antitotem traz contorno ADSR e formas LFO. Aquorbium contém
`BiomaBassVoice` com polyBLEP, sub relacionado, FM/detune, sine-fold 4x e
filtro, mas segue em stand-by e foi consultado somente em leitura.

Decisão de desenho para o Batruc: adaptar a separação função → interpretação
→ corpo, preservando a partitura de `BassComposition`; mapear eventos e gestos
para envelopes/filtro/ganho e depois escolher a matéria. A inovação proposta é
fazer a fórmula visível governar também curvas de articulação e timbre; ligar
dinâmica, gate, cutoff e transiente a um campo de acento/intenção com valores
editáveis; animar cálculo e modulação; comparar o mesmo score por diferentes
corpos mantendo a fundamental grave estável e experimentando movimento nos
harmônicos. Isso é direção de projeto, ainda não implementada nem validada
musicalmente.

Proveniência: nenhum arquivo ou código irmão foi copiado ou alterado. O
inventário global classifica `BassPhraseTimbre` e `BassKickInteraction` como
GPLv3, enquanto o `LICENSE` consultado no `rasgo-synth-core` declara AGPLv3;
os arquivos Studio inspecionados não têm SPDX individual. Resolver esse
apontamento por componente antes de qualquer cópia direta. `README.md` e Atlas
receberam o mapa de reaproveitamento e as oportunidades próprias do Batruc.
Próximo passo de código: conectar a performance do baixo atual a uma voz nativa com
ADSR-A/VCA, ADSR-F/VCF e uma fonte sonora, com limites explícitos; depois
acrescentar LFO e corpos alternativos por comparação A/B.

### Continuação — ADSR, VCA e VCF para o baixo, 25/10/2026

O autor observou que o desenho do baixo também pede módulos clássicos de
síntese. A pesquisa confirma a cadeia conceitual fonte/oscilador ou
ressonador → filtro (VCF) → ganho de articulação (VCA) → master, com ADSR de
amplitude controlando o VCA e um segundo envelope opcional modulando o cutoff
do filtro. LFO permanece como fonte de controle distinta, não substitui o
envelope de nota. Para áudio digital, VCA é ganho variável e VCF é função de
filtragem; não implicam hardware analógico.

Fontes incorporadas à bibliografia e ao Atlas: [tutorial oficial de DSP do
JUCE](https://juce.com/tutorials/tutorial_dsp_introduction/), que demonstra
oscillator, ladder filter, gain e LFO para cutoff; [Indiana University —
basic synthesizer patch](https://cmtext.indiana.edu/synthesis/chapter4_patches.php);
[manual do Moog Matriarch sobre envelopes](https://api.moogmusic.com/sites/default/files/2019-09/Moog_Matriarch_Manual.pdf);
Miller Puckette, [síntese subtrativa](https://msp.ucsd.edu/techniques/latest/book-html/node152.html).
Decisão: considerar ADSR-A, ADSR-F, VCF e VCA como arquitetura candidata da
voz de baixo, comparar com o renderer modal atual, e manter os controles
avançados do LFO em camada separada. Nenhum código ou áudio foi alterado;
sem build ou teste sonoro. Próximo passo: ao integrar o baixo à interface JUCE,
prototipar esses módulos e comparar articulação, faixa dinâmica, cutoff e
nível de saída com a partitura fixa.

### Continuação — pesquisa de baixo, LFO e groove, 25/10/2026

Foi aprofundada a bibliografia para baixo procedural, modulação e expressão
rítmica em [BIBLIOGRAFIA_DE_ESTUDO.md](BIBLIOGRAFIA_DE_ESTUDO.md), e os
resultados foram acrescentados ao Atlas de Referência RASGO. Farnell confirma
um capítulo geral de modulação (AM, FM e fase), mas não foi confirmada a
alegação de capítulo sobre “Event-Synchronised LFOs” ou de sincronização
específica por BPM. O sumário de Dodge/Jerse confirma síntese subtrativa,
distortion, modelagem física e composição, sem confirmar o foco em baixo/PWM
alegado. Roads trata escalas de tempo; 60 Hz é áudio grave, não uma fronteira
universal entre LFO e subgrave.

Incluídos estudos empíricos de baixo/groove: timing intencional associa-se
também a intensidade; o baixo pode interagir temporalmente com conga; estudos
de percepção apontam sensibilidade temporal em registro baixo; e desvios
aleatórios ou ampliados não garantem mais groove. Decisão de projeto: LFO
opcional dedicado ao baixo; controlar destinos por separado; oferecer taxa
livre/sincronizada e fase livre/reset como escolhas; manter swing na realização
dos eventos e não atribuir humanização fisiológica ao sample-and-hold.
Experimento futuro: A/B com uma mesma partitura, LFO desligado/ligado em
corte, amplitude e afinação, taxas sincronizada/livre e reinício opcional.
Nenhum código, cena ou áudio foi alterado; não houve build nem teste de som.
Validação documental por sumários editoriais, artigos e páginas de autor; os
livros integrais não foram lidos. Próximo passo: implementar ou prototipar
somente após a integração do baixo/JUCE, medindo e ouvindo cada destino
separadamente.

### Continuação — pesquisa independente de ritmo, matemática, física e ondas, 25/10/2026

Ampliei [BIBLIOGRAFIA_DE_ESTUDO.md](BIBLIOGRAFIA_DE_ESTUDO.md) com fontes
primárias verificadas: Cooper/Meyer, London, Danielsen, Leman, Nierhaus,
Lerdahl/Jackendoff, Roads, Prusinkiewicz/Lindenmayer, OpenStax, Axler,
Levin, Julius O. Smith III, Strogatz/MIT OCW, Fletcher/Rossing, Shiffman e
estudos empíricos de swing/groove. A lista do Gemini também foi checada:
Steven W. Smith, Steiglitz, Bender/Orszag, Farlow, Zölzer e os artigos de
Välimäki/Pekonen/Nam. Registrei acesso e riscos de reutilização. Distinções
críticas: FFT é algoritmo de DFT; a equação da onda requer modelo e condições
de contorno; wavetable não elimina aliasing por si; DPW original é de
Välimäki; microtiming aleatório não garante groove. Decisão de arquitetura
inicial: receita composicional separada de realização; síntese híbrida com
aditiva/tabelas de banda limitada e ressonadores físicos pequenos, análise
FFT para escuta/visualização, tudo sujeito a medições locais e sem cálculo de
preparação no callback. Nenhum código, áudio ou modelo foi implementado agora.
Validação documental: títulos, autores, escopos e links de origem conferidos;
livros completos não lidos, sem build ou teste de áudio nesta etapa. Próximo
ensaio: uma célula de baixo em quatro interpretações temporais e três motores
sonoros, com A/B cego, medição de aliasing/CPU/pico e registro de intenção.

### Continuação — biblioteca de estudo, 25/10/2026

O autor indicou fontes para geometria rítmica, composição generativa,
modelagem física, DSP, groove, expressão e cognição. As referências foram
registradas em [BIBLIOGRAFIA_DE_ESTUDO.md](BIBLIOGRAFIA_DE_ESTUDO.md), com
links para editoras/autores/instituições, perguntas de implementação e ordem
de leitura aplicada. Verificados títulos e atribuições; os livros completos
não foram lidos nem baixados. Corrigidos: *Music and Cyberliberties* é de
Patrick Burkart, não Patrik Juslin; o modelo GERM pertence ao artigo de
Juslin, Friberg e Bresin; o volume *Mathematical Music Theory* é organizado
por Mariana Montiel e Robert W. Peck, sem evidência nesta etapa para a
atribuição a Marianne Thomsen ou para o conteúdo sobre ruído rosa/rubato;
“The Cognitive Neuroscience of Rhythm” permanece tema de pesquisa, não
título confirmado de livro de Henkjan Honing. Nenhum código ou áudio foi
alterado; não houve build. Próximo passo: selecionar capítulos acessíveis e
executar pequenos ensaios A/B de célula, expressão temporal e ressonância.

### Continuação — operações generativas e escalas de forma, 25/10/2026

O autor ampliou a pesquisa para inversão de acordes, ritmos, alturas, tempo e
algoritmos; recombinação e recálculo; células que possam crescer até músicas
inteiras. O core ganhou `GenerativeOperations.hpp/.cpp`: inversão de voicing
de acorde, espelho de alturas da partitura de baixo em torno de eixo MIDI,
retrógrado de notas com duração, recombinação de quatro compassos de duas
fontes, recálculo transacional da receita e inversa de rotação de fase. As
operações mantêm entrada original para A/B, validam faixas e recusam saída
inválida. `tests/generative_operations.cpp` cobre inversões, retorno,
recombinação, recálculo válido/inválido e fase. Ainda são operações de dados,
sem painel ou áudio nativo ligados a elas.
O retrógrado ordena os eventos pelo novo início depois de espelhar também a
duração; foi conferido com notas sobrepostas de durações diferentes.

Foi criado `OPERACOES_GENERATIVAS.md` para distinguir célula, compasso,
frase, seção e peça; operação, parâmetros, seed, invariantes, versão e escuta.
Como precedentes internos, leitura focada do Studio (célula compartilhada
core/FM, direção por frase/seção e peça), Rasgo Modular (VARIA, MUDA, EVOLUI,
CRUZA, PHRASE, DECISION, FUNCTION, EUCLID, SWITCH), Antitotem (regras de
passo/motivo, groove e microeventos) e Trioio (julgamento e memória de
versões). Nenhum código irmão foi copiado. O Atlas registra função,
proveniência, risco e próximo experimento. A memória de álbum do Studio é
pendência dele e não foi tomada como recurso pronto.

Preflight: 15 GiB disponíveis, swap 0/2 GiB, 984 GiB livres no projeto,
9,4 GiB em `/`; backup `rw`, não usado. Core compilado com dois jobs;
os quatro testes locais passaram (4/4), incluindo o recálculo transacional.
Não houve build do app, GUI ou áudio. Próximo passo: definir representação
versionada de célula/frase/seção/peça e operação com travas, preparar uma
variação lenta coerente em taxa de controle e permitir selecionar A/B no JUCE
sem cálculo nem alocação no callback de áudio.

### Continuação — inversão de eventos no core, 25/10/2026

Implementada `invertEventPlanVoices` em `src/core/CompositionCore.hpp/.cpp`.
Ela valida receita, índices e eventos, troca as agendas de duas vozes sem
alterar a fonte, mantém tempo e dados de cada evento, reordena o resultado e
permite recuperar o original por nova inversão. O renderer recebe original e
invertido como duas variantes preparadas, conservando timbre/pan/ressonador dos
canais. Ao verificar esse caminho, foi corrigida a sobrecarga de
`configureVariants` que recebia uma receita única: ela agora fornece uma
cópia da receita para cada plano, evitando leitura além do objeto. Sem mudança
de esquema de cena ou interface nesta etapa.

Arquivos: `src/core/CompositionCore.hpp`, `src/core/CompositionCore.cpp`,
`tests/voice_inversion.cpp`, `CMakeLists.txt`, README, `RITMOS_E_CONTAGENS.md`
e este registro. Preflight: 15 GiB disponíveis, swap 0/2 GiB, 984 GiB livres
no disco do projeto, 9,4 GiB em `/`; backup `rw`, não usado. Build do core com
dois jobs. Passaram `batruc_voice_inversion` e
`batruc_variant_transport` (2/2). Não houve build do app, GUI ou áudio.
Próximo passo: definir como o baixo encaixa alturas ao receber outra agenda;
em seguida, expor a operação no JUCE com estado de cena e troca na fronteira
de compasso, sem multiplicar o banco de 9.696 variantes.

### Continuação — inversão de vozes, 25/10/2026

O autor apontou a frase atribuída a Gramani em *Rítmica* (1988, p. 12) pelo
texto de Lima e Fiaminghi hospedado na UDESC: trabalhar inversões das vozes
abre novas associações e solicita sensibilidade musical. A citação foi
consultada nesse texto secundário; o livro original não foi conferido nesta
etapa. No Batruc, o primeiro experimento de inversão troca ostinato e série
entre duas vozes, com ataques e pausas, preservando seus registros/timbres e
guardando proveniência para retorno A/B. Acentos/articulação têm decisão
separada. Para o baixo, resolver explicitamente o encaixe entre novos ataques
e células de altura. Especificação registrada em README e
`RITMOS_E_CONTAGENS.md`; nenhuma mudança de código, build ou escuta. Próximo
passo: representar material rítmico com identidade independente do canal,
definir limite de eventos e preparar uma troca reversível fora do callback
antes de expor o comando no JUCE.

### Continuação — organicidade e sentido durante a geração, 25/10/2026

O autor pediu que o orgânico e o semântico entrem também no cálculo, além da
execução posterior. O contrato foi ampliado: intenção, função da voz, relação
com outras vozes e gesto podem orientar candidatos, pausas e escolha da agenda.
O README propõe um experimento limitado com até oito variantes de uma linha de
baixo que mantém sua fórmula de duração e alterna apoio, resposta e primeiro
plano. Critérios legíveis: lacunas ocupadas, coincidências, contorno, saltos e
respiração; seed para desempate reproduzível. Isso é especificação, não recurso
implementado nem medição de valor artístico. O core atual do baixo prepara uma
única partitura e só depois aplica swing/intensidade/repetições. Próximo
incremento de código: definir a voz de referência e gerar candidatos limitados
fora do callback, registrar suas medidas e permitir A/B antes de promover uma
alternativa a cena. Alterados README, `RITMOS_E_CONTAGENS.md` e este registro;
revisão documental, sem build, teste ou áudio nesta etapa.

### Continuação — ecologia, percepção e organicidade, 25/10/2026

O autor relacionou as investigações de ecossistemas do Aquorbium à dimensão
biológica e orgânica que deseja desenvolver no Batruc. Foram consultados,
somente para referência, `AQUORBIUM/ECOSYSTEM_DESIGN.md` e
`AQUORBIUM/ECOLOGICAL_MUSIC_DESIGN.md`. O princípio transferível é de pesquisa:
relações locais baseadas no que cada agente percebe, tendências em vez de
eventos impostos, diferenças individuais, memória e consequências coletivas.
No Batruc isso pode virar uma máquina composicional própria em que vozes
com estado e fórmula preservados se atraem, afastam, imitam, disputam, apoiam,
pausam ou mudam de densidade por regras audíveis e reversíveis. A intenção do
músico deverá orientar as relações permitidas; micropatterning permanece eixo
relacionado, porém distinto, pois trata campos espaciais de afinidade.

Primeiro protótipo C++ adicionado a `src/core/EcologicalRelations.hpp/.cpp` e
incluído na biblioteca `batruc_core`. A função
`buildEcologicalEventPlan` preserva o plano de origem, valida limites e deriva
ataques/intensidades por regras locais de apoio, imitação, afastamento e
competição. Cada evento consulta somente o ataque anterior da voz-fonte dentro
da janela de memória; a struct `EcologicalEventTrace` registra a percepção e a
transformação. Determinístico, sem alocação no callback porque roda como
preparação de plano. O alvo `batruc_core` compilou com sucesso após preflight
(14 GiB disponíveis, swap 0, 984 GiB livres no volume do projeto; nenhum
processo pesado). Testes e escuta não foram executados. Ainda não está conectado
ao app JUCE, então o som/interface atuais não mudaram. Próximo: integrar uma
seleção pequena de relação no mixer/cenas, preparar variantes fora do callback,
mostrar o rastro e permitir A/B/reversão; então avaliar musicalmente antes de
expandir. Aquorbium permaneceu intacto e nenhum código foi copiado.

### Continuação — partitura calculada do baixo e Gramani, 25/10/2026

O princípio conceito → cálculo → estrutura → interpretação também governa as
linhas de baixo. Foi acrescentado ao core C++ um preparador limitado para
quatro compassos de 4/4 e 16 posições por compasso: nota, posição vazia ou
pausa explícita; altura escrita e altura derivada por seed; duração manual,
Fibonacci ou geométrica; modos de frase com respiração/cadência/resposta. A
etapa separada de interpretação produz swing, intensidade, ghost/accent e
repetições, mantendo a partitura calculada consultável. Limites: 64 notas e
512 ataques; cálculo fora do callback. Arquivos: `src/core/BassComposition.hpp`,
`src/core/BassComposition.cpp`, `tests/bass_composition.cpp`, `CMakeLists.txt`,
README, `RITMOS_E_CONTAGENS.md` e este registro. Ainda não há editor, LFO nem
áudio de baixo no app JUCE; a seed nativa do baixo tem mapeamento próprio e a
paridade com a maquete web ainda precisa ser definida.

Fonte adicional indicada pelo autor: Francisco Gabriel da Silva Lima e Luiz
Henrique Fiaminghi, “Gramani, o método ‘anti-metódico’ de rítmica: a assimetria
no estudo e na composição rítmica musical” (PDF de três páginas, UDESC). O
texto distingue os dois livros e interpreta a prática de Gramani como espaço
para associação/dissociação, inversão de papéis entre vozes, composição e
improvisação. Hipótese para o instrumento: comparar o baixo calculado como
apoio, resposta e primeiro plano sem apagar sua partitura estrutural. A fonte
é análise dos autores e não especificação de algoritmo de Gramani. Leitura
documental; nenhum conteúdo externo foi incorporado ao código.

Correção bibliográfica do autor: são dois livros, *Rítmica* (1988) e
*Rítmica viva: a consciência musical do ritmo* (1ª edição, 1996). A ficha
editorial da Unicamp confirma o ano da primeira edição do segundo título. A
pesquisa rítmica e o Atlas registram a distinção, com foco em interpretação,
contraponto e escuta; nenhum exercício foi copiado. Próximo passo na fila:
integrar partitura/interpretador do baixo às cenas e ao editor JUCE, mantendo
LFO opcional e fórmula visível, após resolver o contrato de versão das cenas.

Preflight: 15 GiB de memória disponível, swap 0/2 GiB, 984 GiB livres no
projeto e 9,4 GiB em `/`; o volume de backup estava `rw` e não foi usado.
Build do core com dois jobs; teste específico `batruc_bass_composition`
passou após corrigir um índice errado na própria asserção. Nenhum app, GUI ou
áudio foi executado.

### Continuação — ciclos e fases de ataque por voz no JUCE, 25/10/2026

V3–V12 recebem edição independente de ciclos por seção e fase dos ataques.
O gerador calcula cruzamentos em curso, preserva os filtros geométricos ativos
e aplica alterações feitas durante a reprodução na barra seguinte. O callback
usa estado pré-alocado e não reconstrói o plano. O Mixer identifica o controle
como ciclos da forma, pois os filtros podem reduzir a quantidade final de
ataques. As cenas passam ao esquema 5 e mantêm leitura dos esquemas 1–4. O
plano visual é atualizado na thread da interface.

Validação: o preflight encontrou 15 GiB de memória disponível, swap sem uso e
984 GiB livres no volume de trabalho; o volume de backup aparece montado como
somente leitura e não foi usado. O alvo `batruc_app` compilou e linkou com
dois jobs; depois havia 14 GiB disponíveis, swap sem uso e 984 GiB livres. SHA-256
do executável: `872208cce464319cd214684639af2f5acb1e4cabc566f14f8a76beb66d02fdc9`.
Não rodei testes, não abri a interface e não iniciei áudio. A revisão sonora e
a confirmação visual permanecem para uma sessão com o autor.

### Continuação — autonomia rítmica em V1–V12, 25/10/2026

Os controles de ciclos e fase rítmica agora estão habilitados nas doze vozes.
Um ciclo definido por canal substitui a duração cíclica da receita; a fase por
canal é somada à fase da receita. Os ataques da voz editada são recalculados
independentemente, enquanto as outras mantêm seus tempos. Como V1 e V2 definem
os eixos do desenho, ao alterá-los o renderer recalcula os filtros geométricos
de todas as vozes, preservando ciclos e fases próprios das demais sem override.
A cena esquema 5 já guardava esses campos nos doze canais, então não foi
necessário alterar o esquema. A prévia visual aplica os mesmos valores à receita.

Preflight: 15 GiB disponíveis, swap 0/2 GiB e 984 GiB livres; o volume de
backup estava somente leitura e não foi usado. `batruc_app` compilou e linkou
com dois jobs; depois havia 14 GiB disponíveis, swap sem uso e 984 GiB livres.
SHA-256 final após a atualização dos filtros dos eixos: `5ed289cbf65c011f10928e0d50e13d190c16ab2d10f170d3c35f8345f35b475a`.
Nenhum teste, GUI ou áudio foi executado. Escuta e revisão visual das doze vozes
com valores distintos seguem pendentes.

### Continuação — regra de ataque por canal e referência Gramani, 25/10/2026

O Mixer inclui um seletor compacto por canal: herdar a regra da receita,
cruzamentos do centro ou pontos de inversão. O renderer substitui somente os
eventos daquela voz, aplica a alteração na barra seguinte e preserva a geometria;
se a regra de V1/V2 mudar, recalcula os filtros das demais vozes. Cenas passam
ao esquema 6; versões 1–5 continuam aceitas e herdam a regra da receita.
O controle ocupa a faixa inferior do painel, sem mover sliders existentes.

A pesquisa de contagens recebeu a referência indicada pelo autor: José Eduardo
Gramani, *Rítmica*, e foi lido o capítulo “O ritmo além da regra: o conceito de
time line e rítmica aditiva em Gramani”, de Bianca Thomaz Ribeiro e Luiz Henrique
Fiaminghi, capítulo 13 de *Estudos Latino-Americanos sobre Música*, vol. I
(2020), DOI `10.37572/EdArt_14910092013`. O artigo usa como exemplos rotações
`3+3+2`, ostinatos de 12 pulsos, séries por adição de valores longos/curtos e
contraponto a duas vozes. Crucial: os autores declaram que a ligação com
*time lines* africanas e rítmica aditiva é uma leitura analítica deles; Gramani
não declarou usar essas referências. Registrar contexto cultural e não
converter padrões específicos em presets descontextualizados. O capítulo
ressalta gesto, escuta e execução, não apenas notação. PDF preservado pelo autor
em `~/Downloads/o-ritmo-alem-da-regra-o-conceito-de-time-line-e-ritmica-aditiva-em-gramani.pdf`;
não movido nem alterado.

Validação: preflight com 15 GiB de memória disponível, swap sem uso e 984 GiB
livres; o volume de backup estava somente leitura e não foi usado. `batruc_app`
compilou e linkou com dois jobs; depois havia 15 GiB disponíveis, swap sem uso
e 984 GiB livres. SHA-256: `64409bba5867bbca6ccc6c996f4aff79aec5b2ff1a2f938fdd50113624992033`.
Não rodei testes, não abri a interface e não iniciei áudio; a inspeção visual e
musical do novo seletor segue pendente.

### Cadeia de tradução matemática/física para composição — 25/10/2026

Avançado o princípio para um contrato de receita: conceito → modelo →
estado/evento → mapeamento → realização → escuta. A cadeia separa resultado
matemático, simulação física definida e analogia composicional; pede que cada
mapeamento exponha origem, operação, faixa, destino musical e limites, podendo
ser trocado sem perder o modelo ou a comparação. Uma proposta de pêndulo duplo
relaciona estado/velocidade a altura, dinâmica e articulação, deixando
groove/swing e intenção como uma camada de realização com bypass e A/B. Está
explicitamente marcada como hipótese a experimentar, ainda sem validação
musical ou implementação. A inspeção do JUCE identificou um primeiro caso já
implementado: Lei I calcula `U=(n₁²x²+n₂²y²)/(n₁²+n₂²)` e `K=1−U` a partir dos
deslocamentos normalizados e ciclos/seção, modulando intensidade sem alterar
instantes de ataque. A documentação agora esclarece que é um modelo idealizado
de osciladores independentes, sem unidades físicas nem integração dinâmica no
app; não representa energia acústica nem um pêndulo real. Próximo passo:
planejar A/B que compare intensidade uniforme, U e K com os mesmos eventos, e
separar no gráfico cálculo, eventos e realização expressiva. Nenhum build,
teste, GUI ou áudio foi executado.

### Princípio de interpretação e realização, 25/10/2026

O autor destacou que uma notação pode fixar relações musicais sem prescrever
por completo a execução, usando a interpretação barroca como referência. A
documentação passa a separar a estrutura algorítmica da sua realização e a
nomear groove, swing, intenção, acentuação, dinâmica, articulação, fraseado,
microtemporização e balanço entre vozes como dimensões composicionais editáveis.
Essa comparação é uma analogia de projeto sobre espaço interpretativo, não uma
afirmação de equivalência histórica com Gramani ou tradições rítmicas situadas.
O próximo passo é desenhar controles que revelem precisamente quais aspectos
da execução são alterados, preservando a agenda estrutural para comparação e
recuperação. Alteração documental apenas; nenhum build, teste, GUI ou áudio foi
executado.

### Continuação — quatro transformações matemáticas por voz, JUCE, 25/10/2026

O seletor de fórmula por voz foi ampliado de seis para dez opções. As novas
transformações são cúbica `sin³(θ)`, terceiro harmônico `sin(3θ)`, triangular
`(2/π) asin(sin θ)` e saturação suave `tanh(2 sin θ)/tanh(2)`. Todas mantêm
saída em `[−1,1]`, que a lei existente converte em intensidade de ataque.
A prévia e o tooltip mostram a expressão; instantes de ataque, agenda e
geometria permanecem inalterados.

As cenas passaram ao esquema 4. O parser ainda aceita esquemas 1–3, limita as
transformações antigas ao intervalo original 0–5 e permite 0–9 somente no
esquema novo. A atualização foi aplicada em
`src/core/CompositionCore.hpp`, `src/core/CompositionCore.cpp` e
`src/app/Main.cpp`; inclui rótulos nos quatro idiomas.

Validação: preflight em 25/10/2026 com 15 GiB disponíveis, swap 0/2 GiB,
984 GiB livres no volume do projeto e sem processo pesado. O core e o alvo
`batruc_app` compilaram e linkaram sem avisos com dois jobs; após o build,
havia 15 GiB disponíveis e swap sem uso. SHA-256 do executável:
`859dff21c669aaf61bd707a340751892b82eb652160192fdd03ecd2991868ae9`.
Não rodei testes, não abri a interface nem iniciei áudio. Revisão manual das
cenas v1–v4, legibilidade das dez opções, traduções e audição de cada função
seguem pendentes.

### Continuação — matéria sonora por canal no JUCE, 25/10/2026

O Mixer permite agora que cada voz herde a matéria sonora global ou escolha
uma exceção entre seno, triângulo, dente de serra e quadrada. O renderizador já
aceitava perfil por voz; o host passa a enviar a escolha efetiva de cada canal,
mantendo a seleção global como padrão herdado. O seletor por canal fica no
Mixer junto dos demais controles da voz.

As cenas passam ao esquema 3 e registram a exceção por canal. Arquivos de
esquema 1 e 2 continuam carregando com todos os canais herdando a matéria
global. O valor é limitado a `−1` (herdar) ou aos quatro perfis existentes; a
mudança ocorre no limite do bloco de áudio e não cria novas formas de síntese.
Arquivos afetados: `src/app/Main.cpp`, `README.md` e este registro.

Validação: preflight em 25/10/2026 com 15 GiB disponíveis, swap sem uso,
983 GiB livres antes do build e sem processo pesado. O alvo
`batruc_app` compilou e linkou sem avisos com limite de dois jobs; após o
build havia 14 GiB disponíveis, swap sem uso e 983 GiB livres. SHA-256 do
executável final: `925a746cec2b320673afe1e7722c646eab7db42f9a776b12601aaf72457a8167`.
A guia de som foi ampliada nos quatro idiomas para explicar perfil global e
exceções por canal, e o mesmo build compilou essa alteração. Não rodei testes,
não abri a GUI nem iniciei áudio. Conferir visualmente os doze seletores,
alternar idioma, testar cenas v1/v2/v3 e ouvir os perfis por canal permanece
para revisão manual.

### Registro — 25/10/2026, fase própria da fórmula por voz no JUCE

O Mixer ganhou, em cada canal, um controle `φ fórmula` de −0,5 a +0,5 volta.
Ele desloca a fase usada pela transformação matemática que calcula a
intensidade dos ataques daquela voz. O ponteiro do respectivo círculo acompanha
a fase; os pontos previstos continuam na agenda original. Portanto, este
controle muda a curva de acentuação, mas não desloca os instantes de ataque nem
substitui movimento/ciclo rítmico independente. A expressão matemática
apresentada e a dica do controle tornam esse escopo visível. A configuração é
incluída no histórico e em cenas exportadas.

O esquema de cena passou à versão 2 para incluir o valor por canal; a leitura
da versão 1 é mantida e assume deslocamento zero. O valor segue limitado,
finito e copiado do controle por atomics para o renderer, sem alocação ou
reconstrução da agenda no callback. Implementação em
`src/app/Main.cpp`, `src/core/CompositionCore.hpp` e
`src/core/CompositionCore.cpp`.

Validação: preflight em 25/10/2026 com 15 GiB disponíveis, swap 0/2 GiB,
983 GiB livres no volume do projeto e sem processo pesado. O build
`timeout 180s cmake --build .local-build/native/linux-debug-juce --target
batruc_app --parallel 2` compilou e linkou sem avisos; após o build havia
14 GiB disponíveis e swap sem uso. SHA-256 do executável:
`65f1a82fba5ef0511a3311101103d85d325f3f2252c7fc3696408218be7b1008`.
Não rodei testes, não abri a GUI nem iniciei áudio. Próximo: inspecionar o
Mixer, testar uma cena antiga de esquema 1 e uma nova de esquema 2, e ouvir
separadamente o efeito da mudança de φ fórmula, confirmando que os ataques
não mudam de posição.

### Registro — 25/10/2026, importação e exportação de cenas JUCE

O host nativo ganhou ações para exportar a composição atual como arquivo local
`.batruc.txt` e abrir um desses arquivos. O JSON versionado guarda receita,
seed, andamento, parâmetros sonoros e os 12 canais do mixer: nível, panorama,
mute, solo, transformação e acoplamento. A leitura limita o arquivo a 64 KiB,
confere versão, faixas e quantidade de canais antes de aplicar qualquer dado;
salvamento e abertura acontecem pela interface, fora do callback de áudio.
Restaurar uma cena também atualiza os controles e o estado de áudio. O histórico
de sessão continua disponível e agora captura esses mesmos parâmetros.

Validação: preflight local em 25/10/2026 (15 GiB disponíveis, 2 GiB de swap
livres, 983 GiB livres no volume do projeto e 9,4 GiB na raiz; sem processo
pesado em execução); `timeout 180s cmake --build
.local-build/native/linux-debug-juce --target batruc_app --parallel 2`
concluiu sem avisos após corrigir seis comparações numéricas. Não abri a GUI,
não testei manualmente o ciclo salvar/abrir e não rodei testes; portanto a
compilação não valida interação nem conteúdo restaurado. Teste manual permanece
pendente.

### Registro — 25/10/2026, gravação WAV local no JUCE

O botão `REC` inicia uma gravação estéreo do master em WAV PCM 24-bit, na taxa
de amostragem do dispositivo; `STOP` finaliza e cria um `.batruc.txt`
companheiro com a cena capturada no início. Arquivos são escolhidos pelo
usuário, pausas do transporte são registradas como silêncio e a gravação tem
limite de 15 minutos. O callback apenas tenta inserir blocos numa fila JUCE de
até dois segundos, sem escrita em disco; se a fila estiver cheia, a perda é
contabilizada e apresentada ao finalizar. O writer de disco roda numa thread
de segundo plano e o fechamento esvazia a fila fora do callback.

Validação: preflight em 25/10/2026 (15 GiB disponíveis, swap livre e 983 GiB
livres no volume do projeto); build `batruc_app` concluiu sem avisos após
ajustes à API JUCE 9.0.3. A GUI não foi aberta; gravação, reprodução, WAV/TXT,
limite de duração, perda por fila cheia e UX seguem sem teste manual.

### Registro — 25/10/2026, banco local de cenas JUCE

O menu Cenas permite escolher uma pasta e navegar cenas `.batruc.txt`
válidas. A leitura é não recursiva, inspeciona até 256 arquivos de no máximo
64 KiB, valida o esquema antes de listar e mostra até 64 resultados ordenados
pela data de modificação. Abrir pelo banco aplica a composição e também a
guarda no histórico da sessão. O caminho da pasta fica nas preferências locais
do app; essas preferências guardam somente o caminho, não os dados das cenas ou
áudios. Se o diretório deixar de existir, será preciso escolher outro. Exportar
cena e iniciar WAV sugerem a pasta escolhida; exportações e gravações recusam
nomes já existentes para preservar WAV, TXT e cenas previamente arquivados.

Validação: preflight em 25/10/2026 com 14 GiB disponíveis, swap sem uso, 983
GiB livres no volume do projeto e montagem `rw`; build de `batruc_app`
compilou e linkou. A primeira compilação expôs um argumento de localização e
uma API de iterador depreciada; foram corrigidos, assim como um aviso de cópia.
Ainda falta validar seleção de pasta, persistência da preferência, navegação,
abertura e destino sugerido na GUI. A lista não percorre subdiretórios. Próximo:
verificar esses fluxos manualmente e confirmar que um caminho removido ou
indisponível pede nova seleção sem afetar os arquivos existentes.

### Continuação — finalização assíncrona do WAV, 25/10/2026

O botão STOP transfere o writer e a cena para uma ThreadPool de um único
worker; drenar a fila e gravar o TXT não bloqueia a thread da interface. O
estado indica “Finalizando WAV…” e mantém REC desativado até a conclusão. O
limite de uma gravação em finalização impede acumular tarefas. Ao fechar o app,
a destruição da ThreadPool espera o trabalho corrente terminar, preservando o
fechamento do arquivo.

Validação: preflight em 25/10/2026 (14 GiB disponíveis, swap sem uso, 983 GiB
livres e montagem `rw`); build `batruc_app` concluiu sem avisos. Não executei
testes nem abri a GUI. Próximo: confirmar visualmente o estado de finalização,
testar persistência/recuperação da pasta salva, fazer uma gravação curta,
conferir o WAV/TXT e testar o fechamento do app durante a drenagem.

## Retomada atual — 2026-10-25

### Registro — 25/10/2026, variação de alturas do baixo por seed

**Correção da conversa:** a resposta inicial disse que as seeds já variavam
alturas do baixo; a inspeção do gerador mostrou que isso era incorreto. Antes
desta mudança, a seed afetava ciclos, transformações, fase e mistura das vozes,
mas deixava intactas as alturas escritas no baixo.

Agora a maquete cria uma camada derivada e determinística para as alturas das
células ativas. Um gerador pseudoaleatório independente das vozes mantém a
reprodução do baixo estável mesmo quando se altera a quantidade de vozes. A
variação usa as classes de nota já presentes no padrão escrito, altera cada
célula com probabilidade proporcional ao controle e limita o deslocamento a
2–7 semitons conforme o nível. O padrão manual, pausas, durações, repetições e
intenções continuam intactos. A célula mostra a nota-base editável e a nota
resultante da seed; o resumo informa quantas alturas mudaram. A variação zero
mantém a linha exatamente como escrita.

Arquivos alterados: `prototype/score.js`, `prototype/app.js`,
`prototype/index.html`, `prototype/style.css`, `prototype/README.md`,
`prototype/test-score.cjs`, `README.md` e este registro. A primeira execução
da suíte encontrou também que a cópia do padrão inicial convertia pausas em
objetos vazios; corrigi a cópia preservando `null`. Atualizei duas expectativas
históricas do teste que não correspondiam ao modelo atual (contagem fixa de 90
eventos e fase integral no retorno). `timeout 20s node prototype/test-score.cjs`
passou após os ajustes, incluindo as novas verificações. Não abri o navegador
na etapa de implementação. Depois, o autor informou que ouviu o resultado e
considerou coerente a variação da linha: “por hora está bom”. Esse retorno
qualitativo cobre a audição realizada pelo autor, não todos os seeds e níveis.
A leitura visual no navegador e a comparação sistemática de outras seeds e
intensidades permanecem pendentes.

### Registro — 25/10/2026, primeiro instrumento de reconstrução de Fourier

**Objetivo:** fazer a matemática participar do gesto visual e da composição.
Na maquete web, triângulo e quadrado agora podem ser aproximados por uma soma
complexa truncada `z_H(t)=Σ cₙe^(i2πnt)`, `n=−H…+H`. `H` varia de 1 a 32; a
UI informa as `2H+1` componentes. O canvas compara a curva de referência
pontilhada com a trajetória reconstruída e mostra os círculos/vetores da soma
girando no ponto atual do ciclo. Com “Usar y do desenho”, a coordenada vertical
da aproximação alimenta V1; assim a mudança de H altera o sinal que dirige os
eventos da voz, além do desenho.

A leitura exibe erro RMS relativo XY amostrado em 512 pontos. É uma métrica
local desta implementação; não deve ser comparada aos percentuais/definições
das publicações do Matharium. Não foram copiados algoritmos, imagens ou código
dessas publicações. Implementação no estudo HTML/Canvas; ainda não portada ao
JUCE/core. Não rodei testes, build, navegador nem áudio, portanto a correção
visual, a resposta sonora e o custo durante reprodução seguem sem validação.

Arquivos alterados: `prototype/score.js`, `prototype/app.js`,
`prototype/index.html`, `prototype/style.css`, `prototype/README.md`,
`README.md` e este registro. Próximo passo: abrir a maquete para inspecionar se os vetores estão
legíveis, comparar H baixo/alto e ouvir a influência isolada de V1; depois
avaliar converter a excursão de `y` em articulação, dinâmica ou densidade, sem
aplicar essa regra automaticamente a outras vozes.

### Registro — 25/10/2026, arquivo de gravação da maquete web

O autor observou que a reprodução termina e gera um arquivo WebM. Inspeção do
código confirmou que a conclusão natural dos sete ciclos para o transporte,
finaliza `MediaRecorder` e inicia o download. O navegador prioriza WebM/Opus e
usa Ogg/Opus como alternativa. O comando **Finalizar e salvar** encerra só a
gravação; **Parar** encerra o transporte e a gravação. A documentação do
protótipo foi ajustada para distinguir esses caminhos. Não executei o
transporte nem gerei áudio nesta etapa.

Continuação: ao concluir uma captura não vazia, o navegador inicia o download
do áudio e do TXT versionado para downloads se nenhuma pasta tiver sido
selecionada. O TXT inclui nome/MIME do áudio, configuração/seed, agenda inicial
de eventos e revisões posteriores com tempo aproximado (até 512 revisões). Com
pasta selecionada, ambos são escritos nela. A maquete usa WebM/Opus ou Ogg/Opus;
WAV/PCM sem perdas permanece pendente no produto nativo. A nova gravação direta
na pasta ainda precisa de validação no navegador.

### Registro — 25/10/2026, arquivo em diretório escolhido pelo usuário

O autor esclareceu que “arquivo” significa uma pasta para reunir gravações e
cálculos de composição. A implementação intermediária em `localStorage` foi
removida. Agora a interface permite escolher uma pasta; **Guardar cálculo**
cria nela um `.batruc.txt` com JSON de seed, fórmulas, parâmetros, baixo e
mixer; a lista lê esses arquivos para reabertura/exclusão. A gravação, quando
a pasta está ativa, escreve WebM/Ogg e TXT irmão diretamente nela. A referência
da pasta fica apenas na memória da sessão: ao reabrir a página, o usuário escolhe
a pasta novamente. Não há banco local nem envio a servidor. Documentação
ajustada; o fluxo de File System Access ainda não foi exercitado no navegador.
Os testes anteriores do `localStorage` são históricos e não validam esta
implementação baseada em diretório. O foco de produto continua sendo a
composição generativa; o arquivo é suporte para preservar realizações.

Correção de prioridade registrada pelo autor: o eixo do Batruc é o
instrumento e a composição a partir dos conceitos matemáticos/físicos criados.
O sistema de pasta e registros fica subordinado a esse trabalho e serve para
preservar resultados musicais interessantes. README principal atualizado.

### Registro — 24/10/2026, forma em sete ciclos e aproximação pendular na maquete web

Objetivo: aplicar a forma indicada pelo autor — construção, mudança, pausa e
retorno — e relacioná-la ao estudo do pêndulo acoplado sem atribuir ao
protótipo uma fidelidade que ele não tem.

Mudança: os ciclos 1–2 agora se chamam Construção, 3–4 Mudança, o ciclo 5
Pausa e 6–7 Retorno. Apenas V1 gera eventos no ciclo 5; baixo e vozes
secundárias são recolhidos, e gates de baixo que atravessariam a fronteira são
encerrados nela. As entradas agora são escalonadas: C1 começa com V1, C2 soma
metade das vozes e baixo, C3 completa o conjunto; no retorno, C6 recupera essa
camada e C7 completa todas as vozes. A fase avança em C3–4 e volta ao ponto
inicial progressivamente em C6–7. O modelo web nomeia três pêndulos individuais ligados por
molas efetivas e declara a aproximação linear de ângulo pequeno; esclarece que
não reproduz o pêndulo serial mostrado nas capturas. As coordenadas continuam
calculadas durante a pausa. O autor identificou [@integral_killer_](https://www.instagram.com/integral_killer_/)
como perfil de publicação do carrossel; o registro não atribui a autoria do
artigo à conta. A página não pôde ser consultada pela pesquisa web nesta sessão.

Documentação atualizada: [protótipo web](prototype/README.md),
[movimento, gráfico e som](MOVIMENTO_GRAFICO_E_SOM.md) e interface em
`prototype/index.html`. Capturas locais nos modos de funções e pendular
confirmaram o layout geral e revelaram controles de desenho sem função naquele
modo, além da leitura repetida da energia total nos cartões das três vozes.
Corrigi esses pontos: os controles geométricos somem no modo pendular; os
cartões exibem `θᵢ` e `ωᵢ`, e a energia total fica no painel compartilhado. A
primeira captura de revisão encontrou uma regra CSS que mantinha visível o
toggle de fonte geométrica; a regra foi ajustada e a captura seguinte confirmou
que ele desaparece. O plano de fase colore trechos por grupo formal, marca
fronteiras, mostra cursor e escala angular; sua altura aumenta nesse modo sem
deformar os eixos. A faixa formal numera C1–C7 e informa por acessibilidade a
seção, a contagem de vozes e a presença do baixo; os cartões mostram atividade
ou ciclo de entrada/retorno por voz. No modo de funções, o painel volta à geometria. Capturas
temporárias: `/tmp/batruc-functions.png` e `/tmp/batruc-pendulum.png`.
Não executei testes nem build. A reprodução e a escuta ainda não foram
avaliadas; próximo passo é comparar a entrada das camadas, a pausa e a volta de
fase no instrumento. A maquete não tem validação sonora nem de fidelidade
física.

### Registro — 24/10/2026, deslocamento rítmico autônomo da voz 2

Objetivo: atender à correção do autor de que mover uma voz deve poder mudar
seu ritmo sem deslocar os tempos das demais. A inspeção da maquete web mostrou
controles de ciclos e acoplamento por voz, mas o controle de fase atual é
compartilhado pelas vozes 2–12; a versão JUCE já possuía uma fase editável
para V2, aplicada no compasso seguinte. O botão de fluxo do gráfico era apenas
navegação visual.

Mudança: o Mixer JUCE ganhou `Movimento V2` (0–0,125 volta por compasso). Com
valor acima de zero, o transporte escolhe a cada compasso a fase de V2 pela
fórmula `φ₂(b) = (φ₂,base + b·Δφ₂) mod 0,5`. A seleção usa as agendas
existentes, sem preparar eventos dentro do callback de áudio. O renderer
publica o número de compassos completos; a interface desenha a agenda ativa.
A voz 1 e as vozes 3–12 conservam seus tempos de ataque quando apenas a fase
de V2 muda. A Lei I ou acoplamentos ainda podem alterar suas intensidades.
O valor é guardado nas cenas da sessão; zero mantém o comportamento anterior.
Learn, quatro idiomas da interface, README e o estudo de movimento foram
atualizados. Arquivos: `src/app/Main.cpp`, `src/core/CompositionCore.hpp/.cpp`,
`tests/variant_transport.cpp`, `README.md`, `MOVIMENTO_GRAFICO_E_SOM.md` e
este registro.

Preflight: 13 GiB de RAM disponível, swap 0/2 GiB, 984 GiB no disco de
trabalho e 274 GiB no backup montado `rw`. Build local limitado a dois jobs
concluído. `ctest -R batruc_variant_transport`: 1/1 passou, cobrindo
contagem de compassos, reset e a preservação dos tempos da voz 1 quando só V2
se desloca. Captura com Mixer aberto em 1080 × 760 confirmou que o novo
controle cabe acima dos 12 canais. Não houve avaliação auditiva do movimento
nem captura de sua transição durante a reprodução.
O executável final deste incremento tem SHA-256
`a451a28edd8ef440b8684bf64c04528babfb297a7d0553e2d78c54a87f3fac8c`;
a instância de inspeção foi encerrada após a captura.

Próximo passo: levar deslocamento e direção editáveis às vozes 3–12 sem
explodir as combinações de agendas pré-calculadas; investigar preparação
limitada fora do áudio e troca segura no compasso. Comparar ritmicamente V2
parada e V2 em movimento, com A/B e escuta do autor.

Complemento do autor em 24/10: identificou a referência como **spring coupled
triple pendulum** e compartilhou cinco capturas do artigo *Spring-coupled
triple pendulum: Lagrangian structure, equilibria, and full-rotation dynamics
of the bob* (atribuição visível: P. Prasad). A proposta de tradução musical,
os limites de integração e a pendência bibliográfica foram registrados em
[MOVIMENTO_GRAFICO_E_SOM.md](MOVIMENTO_GRAFICO_E_SOM.md). O artigo ainda não
foi localizado em fonte bibliográfica pública; não implementar o modelo como
física validada até identificar seus parâmetros e equações completos.

### Registro — 24/10/2026, círculos, fluxo, mixer, Lei I e caudas

Objetivo: responder à observação do autor de que só duas vozes tinham círculos,
as curvas das demais se confundiam, um slider cobria a voz 1, controles do
gráfico atravessavam o Mixer e o som apresentava uma cauda/frequência
dominante. A captura da janela ativa confirmou 12 vozes selecionadas, apenas
dois círculos e o controle de λ junto ao rótulo da voz 1. Pela inspeção do DSP,
as excitações somavam amplitude em modos com caudas de 0,44–1,15 s e primeiros
modos fortes; algumas frequências dos perfis coincidem. Não houve medição
acústica do som ouvido pelo autor nesta etapa.

Mudanças no código: cada voz ativa recebe um círculo de fase e uma faixa
temporal própria. A vista da onda agora pode fluir à esquerda ou à direita
com cursor no centro, além da linha fixa para navegação. O controle de λ foi
afastado dos rótulos. O fundo do painel flutuante foi tornado opaco para cobrir
sliders e botões `+`/`−` que pertencem ao gráfico atrás dele. O Mixer ganhou
controles globais **Cauda** (0,15–1,5×, inicial 0,35×) e **Fundamental**
(0–1, inicial 0,55) para reduzir o acúmulo modal percebido; cenas da sessão
guardam esses valores. A Lei I ganhou Gauss central, bandas cosseno e energias
potencial/cinética normalizadas de dois osciladores de mesma massa. Renderer e
marcadores visuais chamam a mesma função de intensidade; a seed v1 continua
escolhendo as quatro leis antigas.

Pesquisa pedida pelo autor sobre cálculos derivados do deslocamento gráfico:
[MOVIMENTO_GRAFICO_E_SOM.md](MOVIMENTO_GRAFICO_E_SOM.md) define onda viajante,
velocidade relativa, encontros, gradiente e curvatura como candidatos de
receita. O fluxo já implementado é de leitura da vista. O próximo passo
composicional é dar ao deslocamento sua própria velocidade/direção de receita,
com agenda preparada fora do áudio, fórmula visível e comparação A/B.

Arquivos afetados: `src/app/Main.cpp`, `src/core/CompositionCore.hpp`,
`src/core/CompositionCore.cpp`, `README.md`, `TAREFAS.md` e
`MOVIMENTO_GRAFICO_E_SOM.md`. Preflight anterior à compilação: 13 GiB de
memória disponível, swap sem uso e 984 GiB livres em disco. Compilação local
`cmake --build .local-build/native/linux-debug-juce --target batruc_app
--parallel 2` concluída; SHA-256 do executável:
`2bb4313878c027b8d61d192ee19b47e940dba9594f8d5e1dfc5a36e08120d6e5`.
A execução de inspeção iniciou sem a asserção JUCE encontrada antes na
conversão de UTF-8 do sufixo λ, agora corrigida. A captura da janela de
1080 × 760 confirmou que o círculo da voz 2 não encosta mais no slider de
fase: seu topo está abaixo da faixa dos controles, e as legendas das duas
vozes permanecem visíveis. A instância de inspeção foi encerrada após as
capturas, sem alterar o executável.
O Mixer foi inspecionado visualmente na sequência abaixo. O áudio requer
escuta do autor para julgar cauda, equilíbrio modal e dinâmica. Não há
validação musical desta versão.

Inspeção posterior na mesma versão: a captura com o Mixer aberto em 1080 ×
760 mostrou os 12 faders, panoramas, transformações e acoplamentos dentro do
painel, sem o slider horizontal nem os botões de zoom do gráfico sobre eles.
Ao fechar o painel, os dois círculos e os controles principais continuaram
visíveis. A geometria calculada para 12 vozes dispõe seis círculos por linha,
com rótulos abaixo de cada linha e intervalo até a Lei I; a tentativa de
selecionar 12 na interface maximizou a janela e deixou a seleção em duas
vozes, portanto a disposição de 12 ainda carece de captura visual direta.
Próximo passo: verificar a seleção de 12 vozes na interface, sua disposição
real e a escuta das caudas e do equilíbrio modal com o autor.

### Registro — 24/10/2026, contagens e pesquisa de ritmos avançados

O seletor de ciclos da voz 2 foi ampliado de 1–9 para 1–16, cobrindo
contagens pares e ímpares na seção atual de quatro compassos em 4/4. O banco
passa a preparar 9.696 agendas (`16 × 101 fases × 2 regras × 3 superfícies`);
os tetos de preparação do core comportam essa quantidade. A seed hexadecimal
v1 conserva o subconjunto e os valores de ciclo antigos `{2, 3, 4, 6, 8}`;
assim, as realizações guardadas permanecem compatíveis, mas as contagens novas
ainda não são escolhidas pela seed. O Sobre e a ajuda de cenas foram
atualizados para distinguir o catálogo total (9.696) do conjunto de 3.030
receitas alcançado pelo mapeamento v1. Build local de `batruc_app` concluído
em 24/10/2026 às 21:40 CEST; preflight com 14 GiB disponíveis, swap em 0 B e
983 GiB livres. SHA-256 do executável:
`d426b827d0fea96d29c17adb77440c763149f84718c70e137211dd526655ad04`.
Nenhum teste foi executado e o app não foi aberto; a janela já ativa do usuário
foi preservada.

Pesquisa convertida em [RITMOS_E_CONTAGENS.md](RITMOS_E_CONTAGENS.md): o
primeiro modo especificado para implementação é `E(k,n,r)`, com até 64 slots
por seção, cálculo inteiro reproduzível, rotação declarada, pausa nos slots
vazios e agenda compartilhada entre animação e áudio. Fica separado das voltas
de fase do controle 1–16. Agrupamentos aditivos que definem métrica aguardam
transporte de duração/compasso variável; sieves e sequências de duração vêm
depois. Próximo incremento rítmico: incorporar esse contrato ao core e ao painel
de controles sem alterar o formato da seed v1.

### Registro — 24/10/2026, correção de layout e arquivo de seeds

Ao revisar a observação de sobreposição, uma captura da janela aberta confirmou
que os controles “Matéria sonora” e prévia da fórmula estavam visíveis sobre a
área principal mesmo fechando o mixer. Agora ficam ocultos até abrir o mixer.
A prévia tem uma faixa própria e os controles de panorama, transformação e
acoplamento ganham separação vertical. No gráfico, cada voz ativa já tinha sua
própria curva contínua no mesmo eixo temporal; o desenho não dependia de as
linhas se tocarem. Para tornar as vozes mais discerníveis, acrescentei contorno
às curvas com muitas vozes, legenda colorida V1–V12 e pequeno escalonamento
vertical dos marcadores de ataque. O app JUCE recompilou com sucesso. A janela
ativa não foi fechada nem reaberta; os ajustes visuais novos aguardam
conferência do autor.

No seguimento, a fileira de controles do cabeçalho subiu para a faixa livre
acima do divisor em janelas com pelo menos 1.240 px; perto da largura mínima,
permanece abaixo dos títulos para evitar colisão. O acesso ao mixer de todas
as doze vozes agora se chama “Mixer” e descreve os controles por canal.
Após nova observação, o painel de mixer foi ampliado para dar mais espaço entre
as 12 colunas e as linhas de panorama, transformação e acoplamento; o botão
“Fechar” passou a ter 60 px de largura para acomodar o texto localizado.
Mixer, Sobre e Aprender usam painéis flutuantes sem encaixe ou bloqueio da
tela principal: abrem centrados na primeira vez e podem ser arrastados pela
barra de título, compartilhando a última posição enquanto a janela principal
permanece aberta.

Decisão de continuidade: o banco de seeds será uma função visível ao usuário,
com ações para guardar a configuração atual e escolher/reabrir uma seed
arquivada. Cada realização registrada deverá produzir um WAV local e um TXT
irmão legível e versionado com seed, fórmulas, parâmetros e partitura
computacional. Usar gravação assíncrona com FIFO limitado; não escrever no
disco pelo callback de áudio e não enviar arquivos a serviços externos.
O host JUCE já exporta/abre composições `.batruc.txt`, grava o master em WAV
com TXT companheiro e navega um banco de cenas escolhido por pasta, descritos
nos registros de 25/10/2026 acima. Carregamento de versões antigas do formato e
validação manual seguem pendentes. Próximo incremento: validar seleção e
navegação do banco e persistência da pasta, destino sugerido, gravação curta,
pausa, cabeçalho/duração do WAV e restauração pelo TXT. Depois, avançar nos
controles rítmicos individuais por voz.

### Ordem de execução — aproximação progressiva à maquete web

Esta é a fila ativa, que prevalece sobre os próximos passos mais antigos
registrados abaixo. A maquete web é a referência funcional; a versão JUCE
preserva a identidade visual e deve aprofundar o instrumento. A fila começa
pelas dependências que permitem acrescentar controles sem recalcular planos no
callback de áudio.

1. **Em curso — receitas editáveis e cenas preparadas.** Primeiro passo feito:
   a interface salva/recupera até 16 versões na sessão, e a seed hexadecimal
   mantém o mapeamento v1 das seeds antigas, enquanto o seletor manual passa
   a oferecer 1–16 ciclos por seção. Com as regras, superfícies e fases, o
   banco agora tem 9.696 agendas preparadas. O usuário confirmou em
   24/10/2026 que a seed está funcionando. Ela agora determina uma receita
   geométrica, lei de intensidade, `λ` e quantidade de vozes. Próximo:
   **Maquete web:** pasta selecionada pelo autor para salvar cálculos
   `.batruc.txt` e gravações WebM/Ogg + TXT versionado com agenda de
   eventos/revisões; validar File System Access e escrita de gravação no
   navegador. WAV/PCM ainda não existe neste alvo. **JUCE:** exportação/abertura
   de cenas individuais `.batruc.txt`, banco local por pasta e gravação do
   master em WAV com TXT companheiro estão implementados; falta teste manual.
   Depois, implementar carregamento das versões antigas, desacoplar edição do
   banco fixo e preparar cenas imutáveis com identidade/proveniência e agenda
   recalculada fora do áudio. O
   mapeamento não gera fórmulas livres e as cenas atuais duram somente na sessão.
2. **Em curso — vozes/stems e parâmetros por voz.** A primeira camada permite
   selecionar 2–12 vozes e abre mixer com nível, mute e solo por canal. As
   receitas têm ciclos, fases, panorama e ressonância inicial distintos; as
   doze vozes agora permitem editar ciclo e fase de ataque. Ainda falta editar outros
   parâmetros por voz, separar/exportar stems e mostrar as doze partes sem
   sobrecarregar os gráficos. Manter transporte comum e tetos
   de eventos/recursos. Ciclos, fases e regra de ataque independentes por canal
   agora são calculados em curso, com estado limitado e aplicação na barra seguinte; as
   agendas preparadas continuam atendendo às vozes sem overrides. Se V1/V2
   mudam o desenho, os filtros de todas as vozes são recalculados sobre o novo
   percurso. Próximos: validar independência rítmica e filtros geométricos,
   estudar stems e medir CPU em sessões longas antes de ampliar limites.
3. **Em curso — transformações e relações matemáticas.** O JUCE já oferece dez
   transformações de intensidade por voz, com expressão visível; as quatro
   mais novas são `sin³(θ)`, `sin(3θ)`, triangular e saturação suave. Próximos
   passos: derivar cruzamentos/extremos de `y(t)` para os instantes de ataque
   e atualizar o scheduler em forma/H/ciclos/fase sem trabalho Fourier no
   callback. A opção atual liga `y(t)` à intensidade calculada de V1 e fica
   guardada nas cenas esquema 10. Depois comparar A/B, acrescentar variações
   reproduzíveis de seed, acoplamento mais expressivo, fórmulas de pausas,
   parâmetros de geometria e inversão relacional de vozes. A troca reversível
   das agendas já foi preparada no core; falta ligá-la integralmente às cenas
   e aos controles JUCE. Cada transformação
   deve declarar sua regra e separar mudanças de intensidade de mudanças na
   agenda de eventos.
4. **Em curso — andamento, expressão e forma.** Primeira fatia nativa: slider de
   20–220 BPM para seção fixa de quatro compassos em 4/4; mudança durante
   reprodução entra no próximo início de seção, e em pausa preserva a posição
   relativa. Cenas guardam BPM. **Incremento atual de paridade:** swing global
   de 50–67% no JUCE; o escalonador desloca ataques das vozes pela mesma
   transformação por ponto médio usada na maquete web, o gráfico desloca os
   marcadores de evento, e o baixo recebe ao menos o swing global combinado
   com seu ajuste local. A proporção entra na próxima barra e é preservada em
   cenas esquema 14; esquemas 1–13 mantêm 50% (reto). Segundo passo: os quatro
   grooves da maquete (reto, ternário, aditivo e contratempo) agora modulam os
   acentos das vozes e do baixo, aparecem nos marcadores e são salvos no esquema
   15. A tradução nativa ainda usa a seção fixa em 4/4; a maquete calcula os
   acentos conforme o compasso escolhido. O alvo `batruc_app` compilou com
   swing e grooves. **Terceiro passo:** intenção (construir, rarear, encontrar,
   contrastar) e dinâmica (estável, crescendo, arco, ondas) agora modulam a
   expressão das vozes e do baixo e o tamanho dos marcadores. As cenas guardam
   ambos no esquema 16; os esquemas anteriores mantêm os padrões encontrar/arco.
   Intenção e dinâmica passam a valer no próximo bloco de áudio, sem esperar
   a seção virar.
   As curvas seguem as equações da maquete web, mas percorrem a seção nativa de
   quatro compassos em loop, não a forma completa de sete ciclos. A compilação
   passou; falta validação manual de audição/interface. Próximo: compasso/forma
   selecionáveis, acentuação sensível ao compasso, articulação, pausas e duração
   por voz/seção.
   O gerador rítmico seguinte deve seguir `RITMOS_E_CONTAGENS.md`: primeiro
   Euclidiano `E(k,n,r)` sobre a seção fixa; métricas aditivas dependem da
   futura duração de compasso/forma variável.
5. **Em curso — baixo e registro local.** O core gera a partitura limitada e o
   JUCE oferece edição por passo, duração, pausas, fraseado, áudio monofônico,
   ADSR-A/ADSR-F e, agora, o primeiro LFO de altura. Falta desenhar a curva e a
   altura tocada no gráfico, portar/comparar as matérias tímbricas da maquete e
   avaliar a linha por escuta. Depois ampliar o registro local para master/stems
   e manter o WAV acompanhado de TXT com partitura computacional, seed e
   parâmetros de reconstrução. Arquivos permanecem locais e selecionáveis pelo
   usuário.
6. **Depois — edição web experimental.** Compartilhar o core matemático onde
   a compilação multiplataforma permitir, manter a página como versão básica e
   testar exemplos determinísticos entre web e JUCE. Não publicar sem decisão
   própria de lançamento.
7. **Portão de maturidade — escuta e acabamento.** Ouvir cada incremento,
   comparar timbres/receitas, corrigir níveis, sincronismo e didática, conferir
   os quatro idiomas e só então ampliar a pesquisa de superfícies/volumes.

O zoom gráfico, mixer de até doze canais, seleção de 2–12 vozes, seed que inclui
lei/`λ`/quantidade, BPM global e primeiras quatro leis matemáticas de
intensidade já entraram. O deslocamento da fase de intensidade por voz também
foi iniciado; ainda faltam ciclos próprios e fases que alterem agendas/ataques,
controles expressivos por voz, stems isolados, receitas editáveis, fórmulas
livres e transformações temporais, compasso/forma, swing/groove, articulação e duração, timbres
aprofundados, baixo editável, gravação e exportação local. O editor livre de
fórmulas continua pendente. A próxima evolução deve acrescentar parâmetros em
grupos utilizáveis, mostrados nas fórmulas e animações; esta lista não declara
equivalência concluída com o instrumento planejado.

Prioridade atual definida pelo autor: aproximar primeiro a versão JUCE das
funções já materializadas na maquete web, mantendo clara a diferença entre
protótipo e instrumento final. Evitar acrescentar módulos novos antes dessa
paridade. O LFO pertence ao baixo e não deve ser replicado como um LFO por
canal.

### Decisão arquitetural — relógio, receita e gesto musical

**Problema que motivou o incremento:** as seis combinações de regra e
superfície tinham seis renderizadores, cada um com seu contador de amostras.
Trocar a combinação durante uma pausa podia selecionar um contador antigo e
mudar a fase mostrada e ouvida. Isso contrariava a continuidade de fase já
especificada. O primeiro reparo abaixo removeu esses contadores independentes.

**Decisão de arquitetura:** um único transporte em amostras é a
autoridade temporal. O áudio publica posição, versão ativa e próxima fronteira
para a interface; animação, fórmulas e gráficos apenas representam esse estado.
Uma única engine sonora conserva as caudas das vozes ao mudar a receita.
`CompositionRecipe` passa a descrever também BPM, compasso e comprimento da
seção em compassos. A referência inicial fica explícita: 120 BPM, 4/4,
quatro compassos por seção = oito segundos, com três e quatro oscilações por
seção. Mudar BPM altera a duração da seção, não a razão 3:4.

Cada edição produz uma **cena preparada imutável**: receita, versão, seed,
proveniência, plano de eventos validado, perfis de voz e limites de recursos.
O preparo, inclusive alocações e ordenação, ocorre fora do callback de áudio.
Três slots limitados permitem cena ativa, uma pendente e uma livre; o slot
antigo só volta a ser gravável após confirmação do áudio. O callback lê um
índice publicado atomicamente e troca a cena na fronteira escolhida, sem
`shared_ptr`, liberação de memória, lock ou recálculo de planos. Uma segunda
edição fica como rascunho; substituir uma cena já pendente exige ação explícita
do músico. O histórico de até 32 versões guarda receitas e metadados, não
cópias de todos os planos; um plano é refeito fora do áudio ao recuperar uma
versão.

| Alteração ao tocar | Fronteira inicial | Consequência |
| --- | --- | --- |
| Regra centro/extremos, superfície ou deslocamento de fase | Próximo compasso | O transporte e as caudas seguem; a regra nova governa inclusive um ataque exatamente na fronteira. Uma fase editada desloca a relação deliberadamente. |
| Razão de ciclos, BPM, compasso ou comprimento da seção | Próximo início de seção | A seção nova nasce de uma origem declarada; nenhum salto de duração ou razão é escondido no meio do percurso. |
| Mute/solo | Próximo bloco de áudio, com rampa curta de ganho | O tempo e o plano de eventos seguem. Eventos durante mute não excitam o corpo e não reaparecem ao desmutar; a próxima entrada obedece à fase corrente. |
| Pausa/retorno | No comando de transporte | O contador musical congela e continua da mesma amostra; nenhuma receita volta à amostra zero por ter ficado inativa. |

**Ordem da tradução:** modelo/fórmula → candidatos com instante ideal e
coordenada → filtro espacial (que transforma rejeição em pausa) → acento,
duração, articulação e timbre → deslocamento de swing/groove → renderização.
O filtro XY consulta a coordenada *ideal*; swing não redesenha a função nem
muda retroativamente a região que aceitou o evento. A interface deverá mostrar
separadamente a marca matemática e o ataque executado quando houver desvio
temporal. O plano preparado deve guardar ambas as posições e a origem da
decisão; a seed de qualquer variação é armazenada na cena, nunca sorteada no
callback. Nenhuma destas camadas expressivas está implementada no app nativo.

**Vozes e stems:** todas as vozes compartilham o mesmo transporte e a mesma
cena. Um stem é a saída isolada de uma voz antes do processamento master,
obtida dos mesmos eventos; o master é registrado à parte, pois processamento
não linear pode impedir que a soma dos stems reproduza exatamente o master.
Exportação e gravação precisam registrar versão, seed, BPM e mapeamentos para
permitir reconstruir o resultado. O protótipo atual ainda não exporta stems.

**Primeiro incremento implementado em 24 out. 2026:** seis agendas são
preparadas fora do callback, mas uma única `ResonatorRenderer` guarda o contador
de amostras, as fases dos modos e as caudas. Trocar regra ou superfície enquanto
o app está pausado seleciona a agenda correspondente na retomada, procurando
o primeiro evento ainda não passado. O gráfico lê esse contador comum. O core
foi compilado com o app JUCE, e o teste focado
`batruc_variant_transport` passou (1/1): verificou continuidade, evento na
fronteira, cauda preservada e ausência de repetição de evento passado. A janela
e a escuta não foram verificadas nesta etapa. Também se alinhou a altura do
campo XY à do gráfico de onda, evitando avançar sobre a legenda inferior.

**Segundo incremento implementado:** os seletores de regra e superfície ficam
ativos durante a reprodução. A escolha pendente entra na primeira amostra do
compasso seguinte, inclusive se a fronteira cair dentro de um bloco de áudio;
na referência fixa de 120 BPM e 4/4, são quatro compassos na seção de oito
segundos. Até essa amostra, gráficos e legenda mostram a agenda ativa, e o
estado informa “Próximo compasso”. Durante pausa, a escolha entra sem avançar
o relógio. O callback consulta agendas já preparadas, sem alocação ou lock.
O teste do core cobre pedido no meio do compasso, fronteira dentro do bloco,
ataque exatamente na fronteira e ausência de antecipação. Build local e teste
passaram; falta conferir a interação e a escuta na janela atualizada.

**Terceiro incremento implementado:** a voz 2 ganhou controle nativo de fase
`φ₂` entre 0 e 0,5 volta, em passos de 0,005 como na maquete web. As 101
posições combinadas com as duas regras e as três superfícies formam 606
agendas preparadas fora do callback. O core limita explicitamente o banco a
1.024 agendas e 65.536 eventos preparados no total; a seleção durante execução
continua a entrar no compasso seguinte. Fórmula, órbita, onda temporal e campo
XY consultam a mesma receita ativa; enquanto houver escolha pendente, mostram
o estado que ainda soa. O build do app e o teste de transporte passaram (1/1),
e o executável iniciou sem asserts do JUCE. Não houve nova captura visual nem
escuta desta fase a pedido do autor; seu efeito musical ainda precisa ser
avaliado. A fase inicial nativa continua em zero, embora a maquete web abra
com 0,125 volta. Este controle é um primeiro gesto editável, não o editor de
fórmulas ou o conjunto de até 12 vozes da web.
Esse registro descreve o banco inicial: o sétimo incremento ampliou o teto do
core a 4.096 agendas e o banco do app a 3.030 combinações ao adicionar opções
de ciclo à voz 2.

**Quarto incremento implementado:** cada uma das duas vozes nativas recebeu
nível, mute e solo compactos. O host publica os dois níveis efetivos por
atômicos; o callback os lê por bloco e o core aplica rampa de cerca de 5 ms.
Mute e solo silenciam a emissão e impedem novos golpes ocultos de excitar o
ressonador, mas conservam relógio e fase; caudas existentes continuam a decair.
O master segue independente. O teste do core passou (1/1), incluindo mute,
retorno sem replay e golpe audível quando a voz está ativa. O app compilou e
iniciou sem asserts do JUCE. Os controles novos não tiveram revisão visual nem
escuta nesta etapa, conforme o pedido de reduzir capturas e pausas.

**Quinto incremento implementado:** o gráfico temporal da janela JUCE ganhou
ampliação de 8 s até 1 s, com botões `+`/`−` e roda do mouse ancorada no ponto
apontado. Arrastar desloca a janela visível e clique duplo restaura os oito
segundos. A escala, as marcas dos ataques aceitos e o cursor de execução usam
o mesmo intervalo visível; a onda desenhada mostra a receita ativa. O zoom é
somente de vista: não altera plano de eventos, transporte ou áudio. O guia
“Aprender” explica o gesto. Build local sem avisos do código do Batruc e
teste de transporte `1/1` passaram. Não houve nova abertura da janela, captura
ou escuta nesta etapa; a interação visual e o resultado musical permanecem
pendentes, conforme o pedido de não interromper o desenvolvimento com capturas
repetidas. Binário local de 24/10/2026 18:41:36 CEST, SHA-256
`0a0de162ce917f1739d2b9bce1413c1547534a0ff08e95d72940ab53e910eeea`.

**Sexto incremento implementado:** o botão “Cenas” permite guardar a
configuração selecionada (regra, superfície e fase) e recuperar até 16
versões durante a sessão. A recuperação sincroniza os controles e entra no
próximo compasso durante reprodução; em pausa, aplica sem avançar o transporte.
O menu “Aprender” explica o alcance atual: as cenas referenciam uma das 3.030
combinações de ciclo, regra, superfície e fase já preparadas, não guardam uma
receita arbitrária, seed ou arquivo e
somem ao fechar o app. O app JUCE recompilou com sucesso. Não rodei testes,
abri a janela nem fiz escuta nesta etapa. Build concluído em 24/10/2026
18:52:32 CEST. SHA-256 do binário:
`6e157b6e7bfc0f70e8f0e6847ac818b56ef20670e1147788006df47c9bc2463c`.

**Sétimo incremento implementado:** a voz 2 pode escolher 2, 3, 4, 6 ou 8
ciclos por seção. As cinco escolhas combinadas às regras, superfícies e fases
geram 3.030 agendas pré-calculadas; a seleção preserva os demais parâmetros e
entra no próximo compasso. O teto do core passou a 4.096 agendas preparadas.
A contagem superior antes do filtro XY é 46.056 eventos no banco, abaixo do
limite total de 65.536. O build do app JUCE passou em 24/10/2026 19:02:38 CEST.
Não rodei testes, não abri o app e não fiz escuta/captura. SHA-256 do binário:
`2a0d39c6c95b789160a1e25a7bd971ad49bbeae84a27b7588ee002560f9e6b07`.

**Critério de pronto da arquitetura completa:** alternar regra e superfície durante
pausa e execução sem retrocesso de fase; aplicar mudança exatamente uma vez na
fronteira, inclusive com evento coincidente; confirmar que pause/retorno,
mute/solo e duas vozes não criam ataques acumulados; confrontar desenho,
amostras e escuta A/B. O histórico atual é só de sessão e aponta para receitas
precalculadas. Próximo passo de código: cenas editáveis com seed e agendas
próprias preparadas fora do áudio; depois expor razões, BPM e quantização
adequada a cada tipo de mudança.
A maquete web poderá usar outra implementação, mas deverá respeitar a mesma
semântica de receita/evento e comparar exemplos determinísticos. BPM, compasso,
histórico persistente e stems ainda são contrato, sem implementação nativa;
mute/solo já funcionam nas duas vozes atuais.

**Nova referência — micropadrões de adesão (ibidi):** a página sobre adesão
celular controlada acrescenta uma hipótese de composição espacial. Manchas,
linhas e espaçamentos definidos podem inspirar zonas de afinidade, exclusão e
transição que condicionem percursos de eventos/vozes, produzindo agrupamento,
alinhamento, densidade e pausa. A analogia é explicitamente composicional, não
modelo biológico. Registro de fonte, proveniência, risco e experimento está no
[Atlas RASGO](../../RASGO_DOCUMENTATION/ATLAS_DE_REFERENCIA_RASGO(4).md#micropadrões-de-adesão-como-referência-para-campos-composicionais--24-out-2026).
Primeira prova implementada no app: campo livre, quatro ilhas circulares e duas
microlinhas diagonais filtram ataques segundo `(sin θ₁, sin θ₂)`; a vista XY
anima o percurso e mostra os ataques que atravessam cada superfície. Os
centros/raios e largura das linhas são fixos nesta etapa; a pessoa ainda não
desenha formas nem regula a força de afinidade. Não houve abertura do app nem
escuta da filtragem após esta alteração. A inspeção visual em execução confirmou
a paleta e o percurso; também revelou que o cabeçalho de três linhas exibia só
a primeira, corrigido com três rótulos separados. Próximo passo: explorar os
três campos, comparar os ataques e ouvir se as diferenças se tornam musicais;
só então decidir se vale expor desenho livre e parâmetros de zona.

Direção visual solicitada pelo autor: preservar na janela JUCE a paleta e a
hierarquia tipográfica da página web. A implementação usa as cores CSS
originais, Georgia para títulos e DejaVu Sans Mono para rótulos/controles;
cores e fontes são locais ao Batruc, sem alterar a maquete. O build compila;
falta revisão visual em execução nos demais sistemas. As fontes não foram
copiadas nem empacotadas.

Itens de interface solicitados pelo autor também estão implementados: crédito
permanente de rodapé; janela “Sobre” com versão, autoria, licença do Batruc,
licença própria do JUCE e limites do protótipo; menu “Aprender” com quatro
guias curtos; e seletor PT/EN/ES/FR. O espanhol cobre controles, legendas e
estados e o diálogo “Sobre”; os textos extensos dos guias ainda recorrem ao inglês.
A escolha de idioma não persiste ao reiniciar.
**Atualização de 11/11:** a preferência agora persiste no arquivo local como
`ui_language_v1`; confirmar o round-trip em reinicialização continua pendente.
O executável que inclui essa camada ainda não foi reaberto para revisar os
menus, diálogos e traduções em execução.

Foi corrigida a sobreposição indicada pelo autor: o início dos gráficos XY e de
onda encobria a legenda “110 Hz · modos harmônicos”. Os dois gráficos foram
baixados 12 px, preservando sua borda inferior e a faixa dos controles. Os nomes
dos idiomas receberam conversão UTF-8 explícita; a janela agora limita o
redimensionamento a 900 × 680 px para não produzir gráficos com altura negativa.
O app recompilou sem erros. A tentativa de inspeção em execução não trouxe a
janela à captura da tela ativa; falta confirmar visualmente a correção na janela
do autor.

O autor ampliou a hipótese de pesquisa: uma máquina de composição generativa
capaz de criar **algoritmos musicais** a partir de conceitos, teorias, axiomas
e problemas matemáticos, agora também de modelos e fenômenos da física. O
Batruc é o lugar de investigação mais provável, pela matemática explícita
em sua identidade; o alcance definitivo ainda depende de experimentação e de
decisão artística. A proposta operacional e o primeiro experimento estão no
fim deste documento. A maquete executável já existe; a validação musical
permanece pendente.

Decisão de produto mais recente: desenvolver o instrumento completo em C++/JUCE
para download multiplataforma e também uma edição de página web, mais básica,
para usuários experimentarem sem instalar. O escopo e a publicação da edição
web não estão implementados nem autorizados como lançamento; a intenção de
produto está registrada abaixo.

Na etapa mais recente, compilou-se o primeiro app standalone JUCE 9.0.3 a partir
da cópia existente `RASGO/third_party/JUCE-9.0.3` (tag `9.0.3`, commit
`be29c81`, árvore limpa). A configuração apontou para a sondagem arquitetural
intencional `JUCE_ARCH x86_64`; não era erro. O core e o aplicativo compilaram
com dois jobs em 24 out. 2026. Corrigiram-se conversões UTF-8 explícitas para
texto visível e construtores de fonte depreciados. O executável local está em
`.local-build/native/linux-debug-juce/batruc_app_artefacts/Debug/Batruc`.
Nesta continuação foi acrescentado o campo composicional XY, com três padrões e
filtragem de eventos no plano calculado antes da reprodução. O build incremental
terminou sem avisos do código do Batruc. Também se alinhou a janela nativa à
paleta e tipografia da versão web; a janela foi aberta para inspeção visual,
que confirmou a linguagem gráfica e levou à correção do bloco de edição no
cabeçalho. Por fim, acrescentaram-se créditos, “Sobre”, quatro guias “Aprender”
e idiomas PT/EN/ES/FR. Nesta continuação, espanhol entrou no seletor e nos
controles/legendas/estados. A comparação de checksums confirmou que o `LICENSE`
do Batruc é idêntico ao do Rasgo Modular: GNU AGPL-3.0-or-later; a licença do
JUCE segue própria e separada. O build incremental terminou sem avisos do
código do projeto. Menus, traduções e diálogos ainda não foram revistos em
execução. Não houve escuta: interação, dispositivo, qualidade sonora e
sincronização percebida continuam pendentes. JUCE 9.0.1 não foi alterado; tarefa
5 permanece aberta e nenhum código irmão foi copiado.

### Base anterior — 2026-10-11

Documentação consolidada: desenho técnico e trigonometria, relações rítmicas,
composição generativa com memória, referências do Atlas, metrônomos circulares
a identificar e requisitos de corpo sonoro expressivo/acústica.

Próxima ação: tarefa 1, uma situação musical curta que conecte esses eixos;
depois, tarefa 2, seu mapeamento geométrico/temporal. Permanecem abertas a
escolha das primeiras vozes, a auditoria seletiva de fontes, a arquitetura,
a implementação e a escuta. Os links dos metrônomos ainda não foram recebidos;
isso não impede formular a situação musical.

Nesta atualização foram revistos README e TAREFAS, links locais, datas e
exclusão do Git. Nenhuma implementação, pesquisa web, build, teste de áudio
ou alteração de instrumento irmão. Não há publicação autorizada.

## Etapa inicial concluída

- [x] Criar diretório próprio `RASGO/BATRUC/`, por autorização do autor.
- [x] Registrar identidade rítmica, desenho técnico, geração e filosofia do tempo.
- [x] Distinguir instrumento `batruc` do conceito `batruc`.
- [x] Fixar direção tecnológica C++ / JUCE e licença AGPL-3.0-or-later.
- [x] Preservar o brainstorm original no lugar atual, exclusivamente local.
- [x] Excluir o novo diretório do Git nesta fase; nenhum remoto ou publicação.
- [x] Consultar o Atlas e seus inventários sobre tempo, ritmo e composição;
  registrar referências, código de origem, limites da consulta e lacunas no
  [README](README.md#consulta-ao-atlas-rasgo--2026-10-10).
- [x] Registrar a pista do autor sobre metrônomos circulares, vozes de bateria
  e estudos de polirritmia avançada; exemplos específicos ainda não verificados.
- [x] Registrar requisitos de geradores percussivos expressivos, variação
  tímbrica e distinção entre acústica do corpo e do ambiente, sem antecipar
  implementação ou aprovação sonora.

## Plano inicial e histórico — 10–11/10/2026

Este checklist registra o estado daquele momento; não é a fonte atual de
prioridade. A situação reconciliada está no início deste arquivo: tarefas
1–4 concluídas no escopo correspondente, tarefa 5 condicional a código externo,
tarefa 6 parcialmente entregue, tarefas 7–9 ainda cobertas pela validação,
escuta e seleção de próximos passos.

1. [x] Escrever uma situação musical curta: apresentação, transformação,
   pausa e retorno. Definir o que a pessoa constrói, o que a geração propõe
   e quais consequências devem ser ouvidas. Usar os achados do Atlas como
   apoio, sem transformar a situação em uma lista de funcionalidades.
2. [x] Especificar uma primeira construção pequena, suas restrições e seu
   mapeamento temporal. Usar três contra quatro como referência comparável,
   não como alegação de invenção. Distinguir fase, subdivisão, andamento,
   polirritmia e polimetria.
3. [x] Especificar os gestos da primeira fatia: construção, percurso,
   deslocamento e recuperação. Definir versões imutáveis e aplicação de
   mudanças em fronteiras musicais, sem sobrescrever escolhas nem confundir
   animação com relógio musical. O core e o host iniciais já implementam parte
   desse contrato; cenas editáveis/histórico continuam pendentes.
4. [x] Especificar o core próprio, interfaces nativa/web, padrão C++, versão
   fixada do framework, plano de build local e tetos iniciais de
   geometria/eventos/vozes. Separar gesto percussivo (intensidade, articulação
   e matéria) do relógio e da interface, com reexcitação/abafamento e limite de
   caudas. O core C++17, renderer limitado e host JUCE já existem; os tetos
   ainda precisam de benchmark, e gravação, camadas expressivas e alvo web
   permanecem pendentes.
5. [ ] Auditoria de reutilização externa, somente se uma receita do roadmap
   precisar de código de terceiro ou de instrumento irmão. Nesse caso, fixar
   versão/commit, licença por arquivo, dependências e proveniência antes de
   adaptar; comparar o resultado no próprio Batruc. A triagem conceitual já
   está registrada acima. Esta auditoria não bloqueia a implementação autoral
   de cenas, vozes e transformações. As referências proprietárias de Patterning
   e metrônomos servem apenas para estudar comportamento; não autorizam portar
   código ou assets. Confirmar fontes e licenças dos módulos só no escopo que
   for efetivamente escolhido.
6. [ ] Completar e aprofundar a primeira fatia C++/JUCE: uma construção observável,
   geração de eventos, corpo sonoro com qualidade de saída e expressividade, e
   desenho sincronizado. Manter recursos de CPU/memória limitados sem reduzir a
   ambição sonora ou composicional. Registrar caminho de build/execução somente
   quando existirem e forem verificados. Selecionar vozes contrastantes após
   consulta do inventário; avaliar timbre e articulação por escuta. Não marcar
   a primeira fatia como bateria completa nem simulação acústica validada.
   Progresso parcial em 2026-10-24: criado core sem dependência de JUCE, com
   eventos de centro/extremos por fase e renderer modal virtual; primeiro host
   JUCE compilado com play/stop, volume, escolha de regra, três superfícies,
   dois dials, gráfico temporal, plano XY animado, rodapé, Sobre, guias Learn,
   idiomas PT/EN/ES/FR, edição limitada da fase da segunda voz e mixer de
   nível/mute/solo para as duas vozes e zoom do gráfico temporal. O campo XY
   filtra eventos antes da preparação do renderer e não recalcula no callback
   de áudio. Resta abrir a versão atualizada,
   conferir interação, menus, traduções e dispositivo, e avaliar por escuta os
   efeitos antes de considerar a fatia musicalmente funcional.
7. [ ] Validar cálculo e tempo, entradas inválidas, repetibilidade quando
   aplicável, carga limitada, saída segura e interrupção cooperativa.
8. [ ] Fazer escuta A/B e exploração pelo autor: consequência dos gestos,
   fraseado, timbre, dinâmica, silêncio e diferença entre tocar e observar;
   verificar se fórmulas, gráficos e controles ensinam de modo claro como o
   cálculo se transforma em gesto e forma musical.
   Comparar a mesma frase com articulações/intensidades diferentes e som
   seco/espacializado; avaliar equilíbrio, leitura rítmica e repetição com nuances.
9. [ ] Decidir o próximo incremento com base na escuta; não ampliar catálogo
   ou superfície antes de avaliar o potencial composicional dessa fatia.
   Depois da avaliação da construção inicial, escolher uma segunda pesquisa
   geométrica (por exemplo, curva plana ou secção móvel de um sólido) pelo que
   acrescenta à composição, não apenas pela complexidade visual.

## Auditoria seletiva — Rasgo Modular, primeira leitura — 2026-10-24

**Escopo:** leitura, sem execução, dos módulos `FunctionGenerator`,
`EuclidClock`, `Decision` e `Phrase`, respectivos dossiês e testes indicados,
no commit `4ce898f` de `RASGO_MODULAR`. Os arquivos-fonte/testes consultados
estavam sem alterações locais no estado verificado. Foram consultados ainda
`CREDITS_AND_SOURCES.md`, `LICENSE` e a configuração CMake. Nenhum teste foi
executado, nenhuma fonte foi copiada e o projeto irmão não foi alterado.

| Módulo | Padrão potencialmente útil ao primeiro protótipo | Limite para uso no Batruc |
| --- | --- | --- |
| `EuclidClock` | Pulso euclidiano calculado por passo, rotação, acento por divisores, swing e clock externo; fornece uma comparação concreta para ciclos independentes e eventos. | É um nó `Signal` do grafo Modular, com parâmetros e semântica próprias. Estudar comportamento e reimplementar a necessidade do Batruc; não transportar a classe inteira. |
| `Phrase` | Motivo limitado a 2–16 passos, seed, pausas, retorno/cadência, acento e entradas de estímulo; bom repertório de critérios para que uma sequência matemática tenha memória e fraseado. | Saída em `v/oct` e contrato modular; escala/quantização são delegadas. O primeiro protótipo pode precisar de eventos rítmicos, não de altura. |
| `Decision` | Aleatoriedade determinística por seed, memória circular, distribuição, probabilidade de gate e slew; referência para variação reproduzível com memória. | Código dependente do contrato `Signal`, com semântica de tensão/gate e valores iniciais fixos. Não substituir pela classe inteira nem chamar isso de composição validada. |
| `FunctionGenerator` | Uma função atravessa escalas de envelope, LFO e áudio; forma, inclinação e deriva sugerem separar lei matemática, trajetória visual e mapeamento sonoro. | DSP de oscilador com PolyBLEP e modulação no grafo de sinais. Não demonstra por si só uma receita composicional ou a animação geométrica proposta. |

Todos os quatro cabeçalhos dependem de `core/SignalGraph.hpp` e da biblioteca
padrão; `Phrase` usa também `std::array`. Não foi identificada dependência
terceira direta nesses módulos. A camada de I/O do Modular é separada e usa
`dr_wav`, sem necessidade para esses comportamentos. A licença geral do
Modular é AGPL-3.0-or-later; o dossiê de PHRASE declara implementação original
e os cabeçalhos descrevem os estudos como comportamentos/algoritmos, não código.
Isso é compatível em princípio com a licença escolhida para o Batruc, mas
não substitui inspeção de proveniência do arquivo exato e crédito caso se
decida derivar código. A decisão atual é reutilizar apenas requisitos e ideias,
com implementação própria.

Os testes existentes incluem gerador, clock e decisão; o dossiê de PHRASE
documenta testes de comportamento e deixa explícita a necessidade de escuta do
autor. A presença desses testes foi inventariada, não executada nesta etapa.
Isso não valida timbre, fraseado ou adequação composicional ao Batruc.

A busca e a comparação das páginas oficiais agora identificaram os candidatos
mais próximos, embora não confirmem quais foram vistos pelo autor:

| Referência | Visualização/edição e consequência sonora | Licença e utilidade para o Batruc |
| --- | --- | --- |
| [Patterning 3](https://olympianoiseco.com/apps/patterning-for-iphone/) e [manual Patterning 2](https://www.olympianoiseco.com/manuals/Patterning_2.pdf) | O encaixe conceitual mais próximo: anéis concêntricos por instrumento, oito loops por padrão, comprimentos/durações independentes; o toque edita passos e parâmetros como velocidade, afinação e ratchet, que se ouvem na voz correspondente. | Produto proprietário; nenhuma licença de código para reutilização foi encontrada. Referência de interação, não fonte para portar. A página atual confirma loops independentes e automação; o manual 2 descreve semântica detalhada da versão anterior. |
| [Dato DRUM](https://dato.audio/drum) | Oito passos circulares para quatro vozes, samples e pitch por voz, efeitos Random/Repeat, filtro e crush. Mostra um instrumento físico simples em que cada voz tem resposta sonora imediata. | Hardware/software comercial, sem código ou licença de reutilização publicada na página consultada. Referência de ergonomia e material sonoro, não implementação. |
| [The Circular Metronome](https://www.thecircularmetronome.com/) | Até 64 passos circulares, quatro tipos de passo incluindo silêncio/acento e três modos sonoros; a página não descreve loops independentes por voz nem polimetria elaborada. | App comercial; não foi localizada licença de código. Referência de leitura e edição de uma roda, com menor aderência à ideia de vozes independentes. |
| [Groovie](https://github.com/maximecb/groovie) | Sequenciador web em grade, até 16 linhas e padrões de 1–64 passos; comprimentos livres correm sem reiniciar juntos, com timeline, swing, humanize, samples por linha e compartilhamento por URL. Edição de passo/linha/timeline modifica diretamente os ataques. | O README declara código MIT e samples CC0; sem frameworks ou dependências de runtime segundo o projeto. É uma referência aberta de modelo de dados, relógio e scheduler, mas não tem a interface circular nem corresponde a vozes matemáticas. A licença geral foi confirmada; arquivos individuais e histórico ainda exigem inspeção se houver intenção de derivar código. |
| [Multinome](https://multinome.app/) | Canais independentes com som e fader próprios, em grade temporal comum; útil para ouvir camadas de razões diferentes. A página consultada não apresenta o desenho circular como elemento central. | Serviço web; código e licença não foram auditados. Referência funcional apenas. |

O melhor estudo visual é Patterning; o melhor candidato aberto para uma leitura
posterior do scheduler e do modelo de padrões é Groovie. Nenhum foi identificado
como o exemplo específico do autor. Por decisão expressa do autor, o Batruc
terá implementação própria e não copiará código proprietário ou sem licença
livre. Patterning permanece referência para estudar a relação entre anéis por
voz, gesto de edição e consequência sonora; o desenho, a interação e a
implementação serão reconstruídos segundo a identidade e a matemática do
Batruc. Uma fonte livre só poderá ser adaptada após fixar versão e auditar
licença, arquivos, dependências e proveniência. A recomendação conceitual é
preservar a ideia de uma órbita por voz com duração e fase próprias e fazer o
traço explicar a construção matemática (por exemplo, pontos da curva que geram
eventos, pausas e repetições), em vez de copiar a interface de um produto.
Esse passo continua sendo proposta, sem protótipo ou decisão visual final.

A tarefa 5 permanece aberta: ainda é preciso confirmar os links vistos pelo
autor e, se Groovie for selecionado para derivação, fixar commit e inspecionar
os arquivos/fontes efetivamente reutilizados, além dos samples. Para Patterning,
Dato DRUM e The Circular Metronome, a pesquisa consultada serve apenas para
análise de comportamento e não permite reutilização de código.

**Próximo passo:** obter do autor os exemplos exatos (link ou captura) e então
selecionar quais operações entram no primeiro contrato do core; só depois
fazer auditoria de arquivo e proveniência para qualquer adaptação de código.

## Proposta de contrato — órbitas de fase por voz — 2026-10-24

Esta proposta aplica a pesquisa circular à primeira situação já especificada
(duas oscilações com razões 3:4 num ciclo comum de oito segundos). Não é uma
decisão de layout nem uma validação musical. A metáfora radial deve explicar a
matemática da voz, em vez de substituir as fórmulas e gráficos temporais que já
são centrais na maquete.

| Elemento | Estado/regra propostos | O que a pessoa consegue ler ou mudar |
| --- | --- | --- |
| Uma órbita por voz | Cada voz mostra um dial de fase. A posição avança segundo `θᵢ(t) = 2π nᵢ t/T + φᵢ`, com `T` como ciclo comum, `nᵢ` como número de oscilações e `φᵢ` como fase. Na prova inicial: `T=8 s`, `n₁=3`, `n₂=4`. | A rotação mostra a fase local; voltas completas visíveis mostram quantas oscilações cabem no ciclo comum. A curva temporal e a fórmula continuam visíveis junto ao dial. |
| Pontos de evento | No modo Centro, marcar `θ = 0, π` (cruzamentos do eixo). No modo Extremos, marcar `θ = π/2, 3π/2` (inversões). Converter cada marca para tempo musical; não agendar áudio pela animação. | Trocar Centro/Extremos altera a localização e a densidade de ataques; o cálculo e as marcas previstas ficam inspecionáveis antes da reprodução. |
| Relação entre vozes | O deslocamento `φᵢ` move os eventos de uma voz em relação à outra, sem mudar a frequência. Aplicar durante execução na fronteira musical já especificada; guardar a versão anterior para retorno reversível. | Arrastar o controle de fase, ou editar seu valor, deixa perceptível onde as vozes se aproximam e se afastam. O dial mostra os dois ângulos e o próximo encontro calculado. |
| Traço e transporte | O ângulo do cursor é uma projeção do relógio musical autoritativo. A trilha de cada dial e o gráfico temporal consultam o mesmo tempo; pausa/resume não recomeça fase. | A pessoa vê onde a trajetória está, o que já soou e os próximos eventos. A animação nunca cria, posterga ou corrige ataques. |
| Tradução sonora | Cada evento carrega instante, voz, fase, modo de disparo, intensidade e proveniência da receita. Timbre, duração/articulação e nível continuam parâmetros explícitos da voz; amortecimento é envelope de seção, não propriedade implícita da órbita. | Alterar um parâmetro produz diferença audível identificável e deixa a origem matemática do evento rastreável. Mute/solo afeta audição, não a evolução da órbita nem os eventos calculados. |
| Silêncio e retorno | Suspender uma voz interrompe sua emissão, mas preserva fase e versão. No retorno, a voz reentra no estado da relação transformada; não reinicia em fase zero. | Comparar suspensão/retorno com a mesma fórmula e distinguir silêncio musical de reset matemático. |

O dial é calculado, não uma sequência discreta fixa: eventos são derivados das
interseções/valores da função. Isso dá uma interpretação própria à forma
circular e separa visualização matemática de edição de passos. A primeira
versão pode usar só duas órbitas; a meta de até 12 vozes é teto de arquitetura,
não requisito visual para esta experiência. Ainda não foram escolhidos escala,
cores, interação por toque, densidade de marcas nem maneira de representar
eventos simultâneos. Esses detalhes dependem de protótipo e escuta.

**Critério de validação futuro:** com o gráfico temporal e os dials à vista,
prever o próximo ataque e a próxima coincidência; fazer a mesma mudança de fase
com a tela ocultada e avaliar se o resultado é musicalmente inteligível; pausar,
retomar, suspender e recuperar sem divergência entre posição visual e agenda de
eventos. A proposta não muda o estado das tarefas: a auditoria de fontes segue
aberta. A implementação posterior do core, ainda sem interface nem validação
de áudio, está registrada em “Primeiro incremento de código”.

## Expansão do campo geométrico — 2026-10-24

O autor propôs aprofundar a pesquisa para além da forma circular, incluindo
formas geométricas variadas, cortes de volumes e modelagem, além de equilíbrio,
gravidade e conceitos escultóricos como meios para aprofundar a composição. A
direção foi incorporada ao README. A roda por voz permanece uma possibilidade
para relações de fase; não é o recipiente obrigatório de toda operação
matemática.

Campo de pesquisa a explorar, sem converter este catálogo em requisito de
implementação:

| Família/gesto | O que o instrumento pode tornar visível | Grandeza candidata a uma regra composicional |
| --- | --- | --- |
| Curvas planas e paramétricas | Equação, domínio, trajetória, fechamento, tangentes e cruzamentos | Passagens, velocidade do percurso, encontros, direção e curvatura |
| Polígonos, polígonos estrelados e mosaicos | Construção, subdivisão, simetria, repetição e deformação | Ciclos, acentos, agrupamentos, periodicidade e variação entre células |
| Fractais e percursos recursivos | Regra iterativa, escala, nível e limite da aproximação | Densidade por escala, duração de níveis e transições entre detalhe e forma |
| Superfícies e sólidos | Regra de geração, eixos, projeções e transformação do objeto | Evolução de contorno, área, volume, centroide e relações entre eixos |
| Corte por plano móvel | Plano de corte, secção resultante, posição e mudanças topológicas | Área/perímetro da secção, eventos de interseção, entrada/saída e densidade |
| Equilíbrio e gravidade | Apoios, centro de massa, braços de alavanca, forças, vetores de torque e estado de movimento | Soma de forças/torques, deslocamento, aceleração, sustentação, perturbação e retorno |
| Escultura móvel | Elementos suspensos, conexões, contrapesos, balanço e projeções que mudam no tempo | Relação entre elementos, tensão/compensação, rotação, mudança de forma e resposta ao movimento |
| Matéria e acústica | Geometria, dimensões, apoios, densidade, rigidez, anisotropia, amortecimento, excitação e contato | Frequências modais, relações harmônicas/inharmônicas, amplitude por modo, espectro e decaimento |

Para cada estudo, documentar: objeto e fonte matemática; domínio e parâmetros;
invariantes e transformações; algoritmo limitado; representação gráfica e
animação; grandezas realmente calculadas; mapeamento explícito para ritmo,
altura, dinâmica, duração, articulação, timbre ou forma; e pergunta de escuta
que pode refutar a proposta. Uma secção que desaparece, muda de componentes ou
se torna descontínua exige uma regra definida para silêncio, transição ou
retenção; não se deve suavizar a matemática sem indicá-lo. Um volume geométrico
não é por si só um modelo físico nem uma fonte acústica: nomes como massa,
ressonância ou energia requerem modelo e parâmetros apropriados.

Equilíbrio e gravidade ampliam esse campo. Como referência física, um corpo
rígido em equilíbrio estático requer resultantes de forças e torques nulas;
quando uma força ou torque não compensado atua, um modelo dinâmico especificado
pode calcular aceleração. A relação de torque depende do ponto de apoio e da
posição da força. Referência: [OpenStax, condições de equilíbrio estático](https://openstax.org/books/university-physics-volume-1/pages/12-1-conditions-for-static-equilibrium).
Como referência escultórica, os móbiles de Calder permitem estudar partes
suspensas, contrapeso, movimento causado por correntes de ar e mudança contínua
da composição visual; a [National Gallery of Art](https://www.nga.gov/educational-resources/counting-art/calders-balancing-acts)
apresenta atividades sobre construção e equilíbrio de móbiles.

Para uma receita do Batruc, cada elemento pode representar uma voz ou parte
geométrica; seus pesos/valores, posições e apoios definem as relações. A soma
equilibrada pode sustentar um estado musical, e a alteração de uma posição pode
deslocar os ataques ou iniciar um percurso de perturbação e resolução. Outros
mapeamentos possíveis incluem distribuição de acentos, densidade, articulação,
altura, timbre, suspensão e retorno. São propostas, não equivalências
universais: equilíbrio musical não significa necessariamente silêncio, e
instabilidade não significa necessariamente queda.

Separar rigorosamente **simulação física** (modelo definido, parâmetros,
unidades e dinâmica calculada) de **analogia escultórico-composicional** (regra
musical inspirada por peso, apoio, gravidade ou tensão, sem pretensão de
simular um objeto). Para cada uma, indicar o que a fórmula prevê, o que a
interface representa e que consequência a escuta deve permitir avaliar. A
referência a Calder informa princípios de movimento e balanço; não autoriza
copiar formas específicas, obras ou identidade visual.

### Matéria, modos e timbre

O autor propôs investigar propriedades materiais, harmônicos e criação de
timbres. Uma fonte de acústica física consultada descreve propriedades como
densidade, elasticidade, dimensões e perdas; a documentação de Modalys também
relaciona amortecimento dependente da frequência à mudança de brilho no
decaimento. Referências de pesquisa: [Modalys — propriedades de objetos](https://support.ircam.fr/docs/Modalys/3.4.0/co/object_properties_general.html)
e [estudo de Brémaud sobre acústica da madeira](https://pubmed.ncbi.nlm.nih.gov/22280703/).

Para uma peça vibrante, a receita deve separar pelo menos corpo, contorno/apoio,
material, excitação e radiação. Dimensões e rigidez com densidade influenciam
frequências e formas modais; o ponto e tipo de excitação pesam quais modos
aparecem; amortecimento determina duração e pode mudar por frequência. Série de
parciais aproximadamente inteiras pode ser útil para sons harmonicamente
relacionados; corpos como barras e placas podem ter modos inarmônicos. Não
forçar a classificação “harmônico” a todo espectro ressonante.

Experimentos comparativos candidatos: mesma geometria e excitação, variando
densidade/rigidez/perdas; mesmo material e apoio, variando forma/dimensão;
mesmo objeto e parâmetros, variando local ou gesto de excitação; mesmo espectro
inicial, variando apenas as perdas por modo. Mostrar espectro e modos, evolução
do decaimento e parâmetros alterados, e ouvir A/B com nível percebido comparável
para distinguir a mudança tímbrica de mera diferença de volume.

Separar três estados de conhecimento: (a) **modelo físico**, com unidades,
condições de contorno, parâmetros rastreáveis e método de síntese explicitado;
(b) **material virtual estilizado**, com parâmetros controláveis mas sem
pretensão de representar fielmente um material real; (c) **analogia
composicional**, em que “peso”, “metal”, “madeira” ou “gravidade” são nomes de
controles musicais e a interface deve deixar isso claro. Não atribuir uma
sonoridade fixa a uma espécie ou classe de material, pois amostra, orientação,
umidade, tratamento, forma, montagem e excitação interferem no resultado.

A conexão composicional pode ocorrer quando uma transformação de equilíbrio,
apoio ou corte geométrico modifica modos, excitação ou decaimento — somente se
essa relação estiver codificada no modelo. Outras ligações podem ser mapeadas
intencionalmente para densidade rítmica, acentos, duração, registros, timbre ou
forma. A pergunta de escuta é se as mudanças materiais produzem diferenças
audíveis e úteis para fraseado, tensão, repouso ou desenvolvimento. Nenhum
modelo físico de material, preset ou biblioteca de síntese foi escolhido. O
renderer modal virtual do primeiro incremento abaixo é apenas uma base inicial
de código para timbre, sem representar material real.

Ordem de pesquisa sugerida: fechar e avaliar o primeiro experimento em 2D;
selecionar uma operação nova (projeção, intersecção ou corte de um sólido); só
então investigar uma modelagem espacial mais ampla. O gráfico radial pode
coexistir com vistas cartesianas, projeções ou secções, escolhidas conforme a
pergunta. Esta ampliação conceitual não implementa geometria espacial, nem
altera a especificação fechada da primeira fatia ou valida sua música.

### Decisão de autoria e fontes — 2026-10-24

O autor reafirmou os valores da família RASGO: o Batruc desenvolverá código
próprio ao estudar Patterning, sem copiar código que não seja livre. Aqui,
“estudar” significa observar comportamentos e relações de interface/sonoridade;
não implica copiar código, assets, identidade visual ou textos. Eventual código
livre só entra após auditoria da versão e dos arquivos efetivamente usados,
conferência de licença e dependências, créditos e adaptação ao conceito do
instrumento. A regra segue a autonomia dos instrumentos e a auditoria de fontes
da [governança RASGO, §7](../../RASGO_DOCUMENTATION/GOVERNANCA_E_TRANSVERSALIDADE.md)
e do [inventário de fontes do Atlas](../../RASGO_DOCUMENTATION/architecture/INVENTARIO_FONTES_ATLAS.md).
Atualizados README e TAREFAS. Nenhum código, asset ou conteúdo externo foi
incorporado; nenhuma fonte irmã foi alterada.

## Primeiro incremento de código — core C++ — 2026-10-24

Criados `CMakeLists.txt`, `src/core/CompositionCore.hpp` e
`src/core/CompositionCore.cpp`. O core inicial define uma receita de dois
osciladores no ciclo comum de oito segundos (3 e 4 ciclos), gera eventos nas
passagens pelo centro ou nos pontos de inversão e ordena um plano limitado de
eventos. O renderer processa uma voz harmônica e outra inarmônica, com oito
modos amortecidos por voz, ataque curto de ruído, panorama, caudas contínuas
entre ciclos e contenção suave do pico. O perfil de modos representa um timbre
virtual, não madeira, metal ou um modelo físico medido. Eventos e perfis são
preparados fora do callback; o processamento não aloca memória nem usa locks.

**Validação executada:** preflight do procedimento de recursos antes do build:
15 GiB disponíveis, swap 0, 984 GiB livres no volume do workspace e volume de
backup montado `rw`; após o build, 16 GiB disponíveis e swap 0. Configurado com
`cmake -S RASGO/PROJETOS_AUTORAIS/BATRUC -B RASGO/PROJETOS_AUTORAIS/BATRUC/.local-build/native/linux-debug -DCMAKE_BUILD_TYPE=Debug`
e compilado com
`cmake --build RASGO/PROJETOS_AUTORAIS/BATRUC/.local-build/native/linux-debug --target batruc_core --parallel 2`.
Uma tentativa intermediária falhou por sintaxe incorreta do comentário SPDX
em C++; o cabeçalho foi corrigido. A compilação final terminou com
`Built target batruc_core`. Não foram criados ou executados testes; não
houve aplicação JUCE, áudio, escuta ou benchmark.

A cópia presente em `RASGO/third_party/JUCE` declara 9.0.1 em seu
`CMakeLists.txt`, abaixo da versão 9.0.3 exigida pela especificação. Nenhum
arquivo JUCE foi alterado e nenhuma versão foi baixada. Para construir a
interface nativa, será necessário localizar ou instalar 9.0.3 lado a lado,
apontar `JUCE_DIR` para esse caminho e conferir licença/avisos antes da
distribuição. Arquivos do incremento: core e CMake acima, README e este TAREFAS;
nenhum outro instrumento RASGO foi editado. Próxima etapa: criar a interface e
o host JUCE quando a dependência exata estiver disponível; avaliar o som por
escuta antes de ampliar o modelo material.

## Portões posteriores — não são tarefas desta etapa

Registro do plano inicial de 10/10. A lista operacional atual está em
**P3 — portões antes de distribuição pública**; os itens abaixo preservam o
escopo original e não são um segundo checklist ativo.

- [ ] Demonstrar geração com memória e relações entre vozes, não apenas
  variações independentes de parâmetros.
- [ ] Desenvolver identidade gráfica, ergonomia e conteúdo de aprendizado
  a partir do gesto validado; manter controles legíveis e sem sobreposição.
- [ ] Documentar créditos, avisos e dependências reais antes de distribuição.
- [ ] Verificar o nome em registros de marcas e nomes semelhantes antes de
  uma decisão de publicação; a busca web inicial não confirmou disponibilidade.
- [ ] Decidir versionamento local/remoto e catalogação transversal, sem
  transferir o brainstorm privado para documentação publicada.
- [ ] Só depois de protótipo e validação: planejar formatos, idiomas,
  compatibilidade, empacotamento e release. Não há prazo ou release anunciada.

## Ponto de retomada — abertura do projeto, 2026-10-10

Objetivo: materializar a autorização do autor para criar um projeto RASGO,
preservando o campo de pesquisa inicial e suas restrições de acesso.

Criados README, TAREFAS e LICENSE. `.gitignore` do RASGO ganhou a exclusão
`/BATRUC/`; o brainstorm recebeu somente o registro de passagem, sem mover
ou apagar sua formulação histórica. Nenhum instrumento irmão foi editado.

Validação: existência dos arquivos e destinos relativos, licença integral
idêntica à fonte padrão local, exclusão do diretório e ausência de arquivos
rastreados nele. Não houve build, teste DSP, áudio ou validação musical.
Nenhum Git init, commit, push, pull, dependência instalada ou remoto criado.

Próximo passo: tarefa 1 — situação de composição que torne audível a relação
entre construção geométrica, transformação generativa e memória temporal.

## Ponto de retomada — consulta ao Atlas, 2026-10-10

Objetivo: encontrar conhecimentos já disponíveis no RASGO que possam apoiar
o batruc, sem confundir referências com componentes prontos.

Atualizados somente README e TAREFAS do batruc. O README recebeu o mapa
das referências Grids, Stages/Tides, OpenMusic, ossia, ChucK, OMax e separação
tempo/sequência; candidatos internos do Modular, Antitotem e Trioio; relação
com a Arquitetura Temporal; lacuna da construção geométrica e ressalvas de
licença/tempo real. As tarefas incorporam esses achados como apoio à primeira
situação musical e à auditoria seletiva futura.

Validação documental: conferidos arquivos locais citados, destinos dos links,
formatação básica e exclusão do Git. Consulta parcial de EuclidClock,
RhythmField e OracleMemory; não houve auditoria integral de fonte, build,
teste DSP ou escuta nesta etapa. Atlas, inventários, brainstorm e instrumentos
irmãos não foram editados; as alterações preexistentes foram preservadas.
O registro detalhado continua local, dentro do diretório ignorado pelo Git.

Próximo passo: tarefa 1, seguida do mapeamento geométrico da tarefa 2.
Nenhum módulo foi escolhido para importação ou implementado.

Registro complementar — 2026-10-10: adicionada ao README a pista do autor
sobre metrônomos circulares que separam peças de bateria e abordam polirritmia
avançada. Tarefa 5 inclui localizar e comparar os exemplos. Conferidos os
trechos alterados e a exclusão do Git; nenhuma consulta web, fonte externa
incorporada, alteração de instrumento irmão ou implementação nesta etapa.
Próximo foco permanece a situação musical da tarefa 1; os links dos exemplos
podem enriquecer a comparação quando forem identificados.

## Ponto de retomada — corpo sonoro, 2026-10-11

Objetivo: incorporar a exigência do autor de bateria/percussão com qualidade
sonora, dinâmica, variação de timbres e acústica à concepção do instrumento.

Atualizados README e TAREFAS: registrados geradores expressivos, intensidade
com consequência tímbrica, articulação, matéria, história entre golpes e
evolução por frase; distinguidas acústica do corpo e do ambiente. As tarefas
de arquitetura, primeira implementação e escuta incorporam esses critérios.
Não foi adotada biblioteca, técnica de síntese ou coleção fechada de vozes.

Validação documental: leitura dos trechos alterados, datas atualizadas,
formatação básica e exclusão do Git conferidas. Nenhum código, build, áudio,
teste musical ou alteração de instrumento irmão nesta etapa.
Próximo passo permanece a tarefa 1: formular uma situação de composição em
que as relações geométricas também conduzam gestos percussivos expressivos.

## Triagem do Atlas — matemática audível e composição generativa, 2026-10-24

**Objetivo:** confrontar o interesse do autor por funções, geometria, Fourier
e sonificação com as pesquisas livres já localizadas no Atlas, sem abrir
outro instrumento nem incorporar código nesta etapa. O exemplo fornecido pelo
autor é uma publicação do Matharium em que curvas e fórmulas orientam alturas
e gestos sonoros; doze capturas foram examinadas, mas o áudio não foi ouvido.
O Atlas não contém ainda um núcleo dedicado à sonificação matemática como
prática composicional completa. As referências mais próximas oferecem partes
complementares do problema:

| Fonte do Atlas e fonte primária | Contribuição para o batruc | Licença / limite constatado | Decisão desta triagem |
| --- | --- | --- | --- |
| [OpenMusic, §§53–55](<../../RASGO_DOCUMENTATION/ATLAS_DE_REFERENCIA_RASGO(4).md>) — [projeto](https://github.com/openmusic-project/openmusic) e [bibliotecas](https://openmusic-project.github.io/libraries/) | Estruturas musicais, maquettes, sistemas dinâmicos e restrições para transformar cálculo em frase e forma. | Núcleo GPLv3; as bibliotecas Chaos/Situation/Clouds precisam de auditoria própria. Build a partir do código requer LispWorks. | Estudar representação e regras; não fazer dele dependência do primeiro protótipo. |
| [ossia score, §99](<../../RASGO_DOCUMENTATION/ATLAS_DE_REFERENCIA_RASGO(4).md>) — [repositório](https://github.com/ossia/score) e [licença](https://github.com/ossia/score/blob/master/LICENSE.txt) | Cenários, estados, processos e transições; a documentação atual inclui Faust, Pure Data, Bytebeat e sonificação de dados. | Distribuição GPLv3, com componentes CeCILL e dependências de terceiros. | Referência para organizar a peça e a intervenção; sem embutir o aplicativo. |
| [Faust, §67](<../../RASGO_DOCUMENTATION/ATLAS_DE_REFERENCIA_RASGO(4).md>) — [documentação](https://faustdoc.grame.fr/manual/introduction/) e [FAQ de licença](https://faustdoc.grame.fr/manual/faq/) | Descrever matematicamente síntese, envelopes, osciladores e a passagem de função a som; gerar código DSP para comparação. | Compilador livre; licença do código gerado depende dos arquivos de entrada, bibliotecas e arquitetura usados. | Protótipo isolado de matéria sonora; auditar cada arquivo antes de reutilizar código. |
| [Pure Data, §7](<../../RASGO_DOCUMENTATION/ATLAS_DE_REFERENCIA_RASGO(4).md>) — [licença do núcleo](https://github.com/pure-data/pure-data/blob/master/LICENSE.txt) | Patch curto para escutar mapeamentos de curva, eventos, camadas e percussão. | Núcleo sob Standard Improved BSD; externals e assets têm termos próprios. | Ferramenta de ensaio local, sem definir a arquitetura final. |
| [ChucK, §69](<../../RASGO_DOCUMENTATION/ATLAS_DE_REFERENCIA_RASGO(4).md>) — [repositório oficial](https://github.com/ccrma/chuck) | Tempo explícito no programa, concorrência de vozes e teste de ciclos, fase e interrupções. | Núcleo com escolha entre MIT e GPL-2.0-or-later, segundo o README oficial; componentes externos exigem revisão. | Referência e possível maquete temporal, sem incorporá-lo ao core C++/JUCE. |
| [Mutable Instruments: Tides/Grids, §§44–45](<../../RASGO_DOCUMENTATION/ATLAS_DE_REFERENCIA_RASGO(4).md>) — [fonte oficial](https://github.com/pichenettes/eurorack) | Funções contínuas deformáveis e campos rítmicos como material para relações entre vozes. | Código STM32F MIT e AVR GPL-3.0; identificar diretório e arquivo exatos antes de qualquer adaptação. | Estudo algorítmico seletivo, preservando identidade própria e sem copiar nesta fase. |

**Leitura musical:** a função matemática pode produzir contorno, pulso,
densidade, acento ou excitação, mas o instrumento precisa ainda organizar
motivos, contraste, pausas, memória, vozes e intervenção do músico. A primeira
prova recomendada é uma construção de dois ciclos com razões ajustáveis:
o músico muda um comprimento ou fase, ouve o encontro e o desencontro de
duas vozes percussivas, interrompe uma delas e recupera a relação alterada.
Uma curva contínua pode controlar articulação ou timbre sobre essa estrutura.
Isso se alinha ao centro geométrico e rítmico já documentado para o batruc;
fica como hipótese até escuta e decisão do autor.

**Segundo exemplo do Matharium trazido pelo autor:** uma curva de Moore
finita percorre 256 pontos de uma grade 16 × 16 e fecha um circuito. O texto
descreve duas cadeias de círculos rotativos que aproximam separadamente as
coordenadas `x(t)` e `y(t)` por séries de Fourier; informa erros de 32,3%,
3,34% e 0,47% com 1, 63 e 255 círculos por eixo, respectivamente, e diz que
os dois canais sonoros apresentam as duas coordenadas. Esses números são
alegações da descrição fornecida, não medições nossas. A relação exata entre
coordenada e áudio não foi observada: não recebemos nem ouvimos o vídeo.

Isso sugere uma segunda maquete própria para o batruc: o músico desenha
um percurso fechado simples, escolhe quantos componentes de Fourier o
reconstroem e compara a trajetória original, a aproximação visual e suas
consequências sonoras. O percurso pode governar encontros, acentos e
articulação entre vozes; `x(t)` e `y(t)` podem ser fluxos de controle
distintos, com mapeamentos sonoros explícitos. A prova musical será ouvir
se aumentar ou retirar componentes transforma a frase de modo expressivo,
além de reduzir o erro geométrico. Manter os cálculos fora do callback de
áudio, limitar componentes e eventos e tratar aliasing e valores extremos.
Nenhuma imagem, fórmula específica, áudio ou código do Matharium foi copiado.

**Terceiro exemplo do Matharium trazido pelo autor:** um percurso fechado
de 729 passos pelos vértices do triângulo de Sierpinski é aproximado por
componentes de Fourier para `x(t)` e `y(t)`. A legenda informa erro de
34,5% com um círculo por eixo, 7,91% com 16 e 1,31% mesmo com 128;
novamente são valores declarados na publicação, não medidos aqui. O interesse
musical adicional é a repetição de detalhe em escalas diferentes: uma
construção do músico poderia produzir relações rítmicas aninhadas, e a
quantidade de componentes controlaria quanto detalhe reaparece em cada
releitura. Isso é hipótese de composição, não uma propriedade musical já
demonstrada pela imagem. Comparar a curva de Moore e essa estrutura em uma
maquete própria ajudará a avaliar se a auto-semelhança altera de fato frase,
acentos e percepção do tempo. O áudio citado não foi ouvido nesta sessão.

**Validação e próximo passo:** leitura pontual do Atlas, do inventário local e
das fontes primárias acima; nenhuma instalação, cópia de código, build,
renderização, teste de áudio ou alteração de instrumento irmão. Especificar
um roteiro curto de escuta A/B para a construção proposta; só depois escolher
uma ferramenta de maquete. Aplicar o preflight transversal antes de qualquer
execução de áudio. Reavaliar a necessidade de novo instrumento a partir da
identidade musical demonstrada, sem abrir um projeto por semelhança temática.

## Hipótese ampliada — máquina de composição matemática e física, 2026-10-24

**Objetivo:** formular a expansão pedida pelo autor sem reduzir a máquina a
uma tradução automática de gráficos em alturas. O material de entrada pode
ser um conceito, uma teoria, um axioma, um problema matemático ou um modelo
físico. Cada entrada deve ser transformada em uma **receita musical legível**:
quais relações serão preservadas, quais regras governam tempo, vozes,
articulação e forma, quais escolhas ficam com o músico e qual resultado foi
gerado. Um mesmo objeto pode oferecer receitas diferentes; a máquina propõe,
o músico compara, modifica e decide.

| Campo de partida | Relação investigada | Possível consequência musical |
| --- | --- | --- |
| Aritmética modular e teoria dos números | ciclos, congruências, encontros | padrões de ataque, acento e deslocamento entre vozes |
| Geometria, topologia e curvas | percurso, vizinhança, interseção, fechamento | frase, espacialização, retorno e transformação de trajetórias |
| Fourier e análise | decomposição e reconstrução em escalas | camadas, densidade, timbre e mudança gradual de detalhe |
| Combinatória e grafos | escolhas e relações permitidas | organização de motivos, transições e contraponto |
| Sistemas dinâmicos | estados, recorrência e sensibilidade | variação com memória e desenvolvimento de seções |
| Lógica e restrições | condições e consequências | gramática de composição, proibições e exceções editáveis |
| Oscilações e acoplamento físico | fase, transferência de energia, sincronização | diálogo rítmico e influência recíproca entre vozes |
| Ondas, interferência e ressonância | superposição, reforço e cancelamento | tensão, repouso, dinâmica e corpo sonoro |
| Amortecimento e dissipação | perda de energia ao longo do tempo | articulação, rarefação e duração de uma seção |

Essas linhas são **propostas de mapeamento**, não equivalências naturais nem
afirmações de que um modelo físico já produz música interessante. Quando uma
receita usar uma lei física, registrar se ela é uma simulação de um modelo
definido ou uma analogia composicional; parâmetros como massa, energia e
frequência só devem receber esses nomes na interface se o cálculo os sustentar.
Evitar apresentar aleatoriedade como física ou usar uma fórmula sem explicar
que relação dela se conserva na música.

**Contrato de uma receita candidata:** fonte e hipótese; domínio e limites do
modelo; algoritmo de transformação; papéis musicais de cada variável; forma
temporal; parâmetros editáveis; seed quando houver acaso; limites de eventos,
vozes e valores; rastro que permita reconstruir o resultado. Uma primeira
versão pode partir de famílias de modelos escolhidas e verificáveis. Interpretar
qualquer texto científico livre e transformá-lo em código executável fica fora
do primeiro experimento.

**Primeira situação de composição para testar:** dois movimentos oscilatórios
simples, com frequências e fases ajustáveis, iniciam duas vozes percussivas.
A máquina oferece duas receitas para o mesmo movimento: ataques nas passagens
pelo centro ou nos extremos; a distância de fase influencia os acentos. Um
envelope de amortecimento governa intensidade e duração da seção. O músico
altera a relação, silencia uma voz, ouve a outra ocupar o espaço e recupera a
primeira com o estado transformado. A construção geométrica mostra os
movimentos e seus encontros. O roteiro de escuta deve comparar as receitas,
observar fraseado, silêncio, contraste e intervenção, e verificar se a ideia
física continua reconhecível sem olhar a tela. Os dois movimentos são um
modelo idealizado para a maquete; não declarar acoplamento físico antes de
especificar e calcular essa interação.

**Partitura de eventos para a maquete, ainda sem áudio:** tomar um ciclo comum
de oito segundos, com três oscilações na primeira voz e quatro na segunda.
Preparar duas versões do mesmo material: A dispara nos cruzamentos do eixo
central, B nos pontos de inversão do movimento. A fase da segunda voz pode
ser deslocada pelo músico; cada evento guarda tempo, voz, posição no ciclo,
intensidade e receita de origem. O amortecimento reduz a intensidade ao longo
de cada seção, com limite inferior audível definido na escuta. Estrutura
inicial: apresentação das duas vozes por dois ciclos; transformação de fase
por dois; suspensão de uma voz por um; retorno de ambas por dois. Guardar a
fase escolhida no retorno, em vez de reiniciar a construção. Esta duração é
um ponto de partida para comparar gestos, não forma definitiva da obra.

**Critérios A/B:** escutar A e B com a mesma duração, vozes, fase e curva de
intensidade; avaliar se a mudança de pontos de disparo produz fraseado
perceptivelmente distinto. Depois comparar fase original e fase alterada,
inclusive durante suspensão e retorno; verificar se o músico consegue prever
e controlar a consequência. Anotar momentos de encontro, densidade,
monotonia, silêncio e diferenças de articulação. Se as receitas não gerarem
decisões musicais distinguíveis, revisar a transformação antes de ampliar o
catálogo de modelos. A correção do cálculo e a avaliação musical são provas
separadas.

**Estado e próximo passo:** somente especificação documental neste incremento.
O preflight de recursos anterior identificou outra aplicação consumindo cerca
de um núcleo de CPU; áudio, compilação e renderização aguardam nova checagem
conforme o procedimento transversal. Próximo passo: implementar uma maquete
limitada, ouvir com o autor e decidir a identidade e o alcance da máquina.
Nenhuma fonte
externa ou código foi incorporado; os exemplos Matharium continuam como
referências trazidas pelo autor, com áudio não verificado nesta sessão.

## Ponto de retomada — maquete animada local, 2026-10-24

**Objetivo:** materializar o pedido do autor de um instrumento em cujo layout
as fórmulas e os gráficos apareçam e se movam junto com a composição.

Foi criada [prototype/](prototype/README.md), uma maquete local sem
dependências externas: de duas a doze fórmulas e curvas animadas, cursor de
tempo, pontos de disparo, fase ajustável, silêncio individual e forma de sete
ciclos. `score.js` gera duas receitas a partir de funções senoidais combinadas;
`app.js` usa Web Audio para sons percussivos provisórios. As tarefas 1 e 2
foram especificadas para esta maquete; a tarefa 3 já tem uma primeira
interação, mas o contrato de estado para C++/JUCE continua aberto.

**Validação executada:** preflight com 18 GiB de memória disponível, swap
zerada e backup em `rw`; o Aquorbium foi encerrado pelo autor antes das
verificações. `node prototype/test-score.cjs` passou: 90 eventos em cada
receita, tempos diferentes, suspensão da voz 2, retorno com fase preservada,
determinismo e rejeição de parâmetros inválidos. `node --check` passou para
`score.js` e `app.js`. O Chrome headless abriu o arquivo local e produziu
capturas em 1440 × 1100 e 430 × 1000; fórmulas completas e gráficos visíveis
nos dois tamanhos. Não houve escuta do áudio pelo autor nem avaliação de
qualidade musical, latência real, acessibilidade completa ou carga prolongada.

**Limites:** HTML/Canvas/Web Audio é uma ferramenta de pesquisa local; a
direção C++/JUCE do instrumento permanece. O modelo é oscilação idealizada
com envelope, sem acoplamento físico. Os timbres atuais servem à comparação
A/B e não satisfazem o requisito de percussão expressiva final. O material
segue excluído do Git; não houve commit, push ou publicação.

**Próximo passo:** o autor abrir a maquete, ouvir as receitas A/B e experimentar
quantidade de vozes, ciclos, mistura `λ`, fase, silêncio e retorno. Registrar
quais relações matemáticas e físicas conduzem a decisões musicais perceptíveis.
Com essa escuta, especificar o estado do core C++, a sincronização de
áudio/visual e a primeira superfície JUCE.

## Ponto de retomada — fórmulas combináveis e pesquisa composicional, 2026-10-24

O autor pediu que cálculos se recombinem num painel cujas fórmulas interagem e
se recalculam, e que matemática e física se liguem claramente a conceitos de
composição. A maquete agora apresenta de duas a doze vozes senoidais
encadeadas: cada voz possui frequência própria (1–12 oscilações por ciclo) e
`λ` controla quanto herda da curva anterior. O desvio de fase avança nos dois
ciclos de mudança e retorna ao ponto inicial ao longo dos ciclos 6–7. A forma
em sete ciclos também escalona as entradas: V1, camada intermediária, conjunto
completo, pausa em V1, reentrada intermediária e conjunto completo no retorno.
Mudanças recalculam desenho e eventos. A leitura proposta é combinação linear →
influência entre movimentos idealizados → imitação/variação; fica identificada
como hipótese artística, não como equivalência física ou musical comprovada.

O Atlas recebeu uma pesquisa de fontes livres, incluindo OpenMusic/MathTools e
bibliotecas gerativas, Patterns do SuperCollider, transformações Euclidianas
do Tidal, Faust, Pure Data e mecanismos internos do RASGO. O resultado é uma
rede de referências complementares, não um banco único de fórmulas. Uma tabela
adicional associa recursos formais a operações de composição e indica que
elas precisam ser testadas pela escuta.

Arquivos atualizados: `prototype/score.js`, `app.js`, `index.html`, `style.css`,
`prototype/README.md`, este TAREFAS, o Atlas, o inventário de fontes e o
inventário global de módulos. Nenhum código de terceiro foi copiado ou
instalado. A maquete continua local e ignorada pelo Git. As validações de
código da atualização para múltiplas vozes ainda não foram executadas; a
escuta do autor continua pendente. A direção C++/JUCE permanece.

## Ponto de retomada — intenção e execução matemática, 2026-10-24

O autor acrescentou à investigação intenção, dinâmica, swing, forma de tocar,
duração, timbre e groove, e pediu para aproveitar o que já está codado na
família RASGO. A maquete local agora expõe essas dimensões: intenção enfatiza
encontros, construção, rarefação ou contraste; curva dinâmica modela o arco
global; swing desloca a subdivisão longa; groove aplica mapas de acento;
articulação altera o decaimento; formas de onda mudam a matéria eletrônica; a
duração total seleciona 28–84 s em sete ciclos. São mapeamentos experimentais
da maquete, ainda sem escuta autoral ou validação musical.

A consulta confirmou a lembrança do autor: o Caustic 3 tem a máquina
**8BitSynth**, que avalia expressões aritméticas/bitwise em um contador `t` e
emite áudio bytebeat limitado a 8 bits; oferece duas expressões A/B e uma
mistura. O manual público está ligado em
[README § Caustic 8BitSynth](README.md#caustic-8bitsynth-precedente-de-fórmula-executável).
Ela é um precedente útil para fórmulas que soam, mas não é um gerador geral de
composição por conceitos nem converte livremente gráficos em notas. Não há
arquivos locais do Caustic neste workspace e nenhum código ou fórmula foi
copiado.

O repertório interno consultado inclui `RasgarAccentField`, `GrooveDialogue`,
`TemporalGrooveImprint`, `GrooveRepeat` e `DurationTarget`, além do estudo do
Studio sobre groove/swing. `RasgarAccentField` relaciona acento a velocidade,
gate, microaltura e timbre com limites; os módulos de groove oferecem padrões,
imprint de baixo e subdivisões limitadas; `DurationTarget` ajusta a duração.
Permanecem no instrumento de origem; são
referências para mapear contratos e fazer adaptação própria, sem dependência de
build entre projetos.

Arquivos alterados nesta etapa: `prototype/score.js`, `app.js`, `index.html`,
`prototype/README.md`, README e este TAREFAS. Corrigida também uma chamada antiga
de leitura da curva para `valueAt`. Não foram executados testes ou reprodução
de áudio após estas alterações. A maquete segue local/ignorada pelo Git; sem
commit ou push.

Próximo passo: definir uma linguagem limitada de fórmulas executáveis para o
Batruc — incluindo semântica, limites de custo e saídas para ritmo, altura,
dinâmica e gesto — usando Caustic como precedente e os contratos RASGO como
referências. Só depois decidir quanto dessa linguagem entra na interface;
escutar as escolhas expressivas atuais antes de tratá-las como eficazes.

### Critério reafirmado pelo autor e controles de exploração

O autor reafirmou que os instrumentos RASGO visam excelência de áudio,
profundidade e variedade de recursos, e função didática. Isso é requisito do
instrumento, não promessa sobre esta maquete. O estudo simples deve conduzir à
arquitetura; não fixa o teto de qualidade do Batruc.

Após notar o nível baixo de saída da maquete, foi adicionado ganho master
ajustável (padrão 120%) com compressor suave de picos. A antiga área de mute
foi substituída por uma mesa real de até doze canais, com volume, panorama,
mute, solo múltiplo e mute do master; os canais têm buses próprios para que
volume/pan/mute atuem também nas caudas já soando. O gráfico ganhou zoom de 1×
a 8× e escala vertical da amplitude Y de 0,5× a 3×, controle de centro, roda do
mouse ancorada no ponteiro, arraste para percorrer e botão de retorno à vista
integral. Os zooms só mudam a visualização; não alteram volume, relógio nem
geração musical. Também foram adicionados BPM (40–240)
e compassos 3/4, 4/4, 5/4 e 7/8. Andamento e compasso organizam a grade de
swing/acentos e aparecem como pulsos/barras no gráfico; a duração formal fica
independente. A semínima é a unidade de BPM nesta maquete.

O autor perguntou se o Batruc é um sequenciador. Decisão conceitual
registrada: sequenciamento é camada de execução/edição; a hipótese maior é uma
máquina de composição generativa que transforma conceitos matemáticos/físicos
em materiais, relações de vozes e forma. A interface atual ainda é uma maquete
sequenciadora para provar os primeiros mapeamentos, não a arquitetura final.

Arquivos afetados nesta extensão: `prototype/score.js`, `app.js`, `index.html`,
`style.css`, `prototype/README.md`, README e este TAREFAS. Não foram executados
testes, renderização ou escuta após as mudanças do mixer, BPM e zoom. Próximo
passo: testar BPM, compasso, swing, acentos e escala vertical; testar
nível/master mix em 2 e 12 vozes; experimentar zoom/arraste num trecho; avaliar
as relações didáticas ao ouvir.

### Módulo de baixo: linha editável e LFO opcional

A linha de baixo agora é uma pista editável, em vez de notas geradas somente
pelo LFO. A interface oferece editor gráfico por passos: clique vazio insere
uma nota na altura apontada; arraste o corpo para mudar altura; arraste a
borda direita para escolher a duração; clique numa nota sem arrastar remove-a.
Seletores de nota/pausa e duração oferecem edição precisa. A grade ajusta o
número de passos ao compasso (12 em 3/4, 16 em 4/4, 20 em 5/4, 14 em 7/8).
O módulo repete o compasso e mantém monofonia: cada nota termina no próximo
ataque. O LFO (senoidal, triangular ou quadrado) é desligado por padrão e,
quando ativado, desloca a altura escrita numa profundidade de semitons. Sua
fase e seus parâmetros aparecem na faixa do baixo no gráfico principal. O
canal dedicado continua disponível no mixer.

Referências conceituais: `LfoBank`, `BassFunctionPlan` e `BassPhrasePlan` do
Rasgo Synth Studio; nenhum DSP foi portado. A linha editável segue o princípio
dos planos de baixo da família: passos discretos são intenção composicional;
LFO é uma modulação opcional, não a frase inteira. Ainda faltam harmonia,
memória de frase e timbres de qualidade final.

A tipografia das fórmulas foi reduzida e o gráfico principal ganhou mais
altura útil. Arquivos afetados: `prototype/score.js`, `prototype/app.js`,
`prototype/index.html`, `prototype/style.css`, `prototype/README.md`, README e
TAREFAS. Não foram executados testes, build, renderização ou escuta; verificar
o editor no navegador e escutar permanece pendente.

Em seguida, o módulo recebeu seis materiais Web Audio provisórios para o
baixo (subgrave, redondo, corpo, pulso, brilhante e pluck), com formas de onda,
filtros e envelopes próprios. A mesa de mixagem foi reposicionada imediatamente
após o gráfico; equações foram reduzidas e a área do gráfico ampliada. Não houve
teste ou escuta dessas alterações.

O estudo de repetição do Navalha 2 (`NAVALHA2_PD/app/index.html` e
`NAVALHA2_JUCE/README.md`) também mapeia REPEAT, STUTTER ×4, BURST ×8,
TRACE LOOP, variação e GAP. No Batruc, a tradução para eventos musicais
agora inclui repetição por passo (1/2/4/8 retriggers), intenção normal/ghost/
accent e fraseados de pausa: respiração a cada dois compassos, cadência de
silêncio a cada quatro ou chamada-resposta por compassos alternados. O padrão
exato continua disponível; estas ações operam sobre notas e duração, não sobre
fragmentos de gravação. A faixa principal desenha os ataques e as lacunas
resultantes. A implementação não foi testada nem ouvida ainda.

**Correspondências adicionais identificadas no Navalha 2:** MEMORY oferece
captura/recall/variação explícitos; locks protegem dimensões escolhidas enquanto
outras variam; snapshots com KEEP/RESTORE tornam a edição automática reversível;
FORM organiza cenas por frases com NEXT/HOLD/seleção humana; GAP preserva
silêncio como escolha; seed/rewind torna a sequência de decisões reproduzível;
TRACE/LOOP repete um gesto de controle XY sem gravar áudio. Para o Batruc,
isso sugere slots nomeados de construções, travas por parâmetro (por exemplo,
manter a topologia enquanto varia fase), recuperação não destrutiva e forma por
seções. O traço de baixo já pode ser desenhado, mas ainda não há gravação/reloop
de gestos geométricos. STUTTER/BURST referem-se no Navalha a fragmentos de
áudio; só a ideia abstrata de repetição foi traduzida para retriggers de notas
e eventos. Não copiar módulos ou código: manter identidade e licença próprias;
qualquer reutilização futura exigiria auditoria explícita de versão, arquivos,
dependências e licença. Para a tarefa 4, também é útil estudar o padrão do
Navalha de planos de evento com capacidade fixa e execução sample-accurate sem
alocação no callback; adaptar o princípio só depois de desenhar o contrato de
tempo e o limite máximo de eventos do Batruc.

O autor perguntou sobre fórmulas matemáticas para os tempos das notas. O módulo
agora oferece duração manual, Fibonacci e progressão geométrica, ambas
limitadas ao comprimento do compasso, com razão selecionável (1,20; 1,50; φ;
2,00). A expressão e os valores derivados no compasso aparecem na interface;
durante execução, o gate é limitado pelo próximo ataque para manter a linha
monofônica. Isso foi implementado na maquete e ainda requer validação no
navegador e escuta.

O autor pediu mais transformações matemáticas por voz, uma máquina de seeds com
variação e gravação dos áudios. Cada voz passou a escolher seno original,
inversão, retificação, segundo harmônico, modulação de amplitude ou quantização;
a transformação ocorre antes da mistura com a voz anterior. Seed hexadecimal
mais controle de variação deriva ciclos, escolhas de transformação, fases e
misturas de modo determinístico; as expressões efetivas aparecem nos cartões.
O botão de gravação captura o master processado e baixa o arquivo ao finalizar.
Não houve validação no navegador, teste ou escuta destes recursos.

O autor pediu que o próprio traçado fosse animado conforme a execução, em vez
de mostrar a curva inteira desde o início. O desenho do gráfico agora revela
cada função até o cursor temporal; eventos futuros também ficam ocultos durante
a reprodução, e a forma completa volta quando o transporte está parado. Como
`valueAt` já recebe a transformação efetiva da voz, o traçado acompanha seno,
inversão, retificação, harmônicos, envelope ou quantização. A alteração ainda
não foi conferida visualmente no navegador.

O autor reafirmou que desenhar e acompanhar o percurso do traço é parte da
identidade do Batruc. Além do traço algorítmico revelado durante a execução,
o editor do baixo ganhou modo de desenho contínuo: o gesto do usuário interpola
alturas entre passos, quantiza-as ao compasso e produz notas; o gráfico conecta
os pontos como percurso visual. O modo de edição precisa permanece. Ainda não
houve conferência no navegador ou escuta.

### Segundo gráfico: percurso geométrico

O autor sugeriu separar os desenhos geométricos em um segundo gráfico. A
maquete ganhou um canvas próprio, independente do gráfico temporal das vozes,
com circunferência de compasso, triângulo, quadrado, espiral e curva de
Lissajous. O percurso é revelado em sincronia com a execução; um cursor indica
a ponta do instrumento de desenho e compasso/espiral mostram o braço desde o
centro. A interface apresenta a equação ou regra ativa e permite conectar o
eixo Y à voz 1. Sem essa opção, o gráfico é visual. Os termos Math Art, Sound
Design e Mathematical Sonification identificam campos de pesquisa, não uma
declaração de acabamento. Alterados `prototype/app.js`, `prototype/score.js`,
`prototype/index.html`, `prototype/style.css`, `prototype/README.md`, README do
projeto e este TAREFAS. A assinatura de fase agora recebe a configuração
opcional para contemplar offsets de seed sem quebrar chamadas antigas.
Não houve teste, build, inspeção em navegador ou escuta; sincronismo visual e
conexão geométrica-sonora permanecem por conferir. A gravação é local no
navegador e não envia áudio a servidor.

### Princípio composicional: tornar audíveis as relações — 2026-10-24

Registrado no README conceitual: fórmulas, algoritmos e leis físicas não são
música em si; tornam-se material composicional quando o instrumento escolhe
relações a preservar e as traduz em parâmetros musicais, organizados em forma.
Cada proposta deve explicitar (1) modelo, domínio e limites; (2) tradução para
altura, ritmo, duração, timbre, intensidade, articulação, vozes, pausas e
interação; (3) desenvolvimento formal, com contraste, suspensão, retorno e
possibilidades de intervenção. O gráfico revela o cálculo e o percurso; a
escuta deve permitir avaliar o efeito musical. A pesquisa deve distinguir
consequência matemática, simulação de modelo físico e analogia composicional,
sem insinuar que exista uma música pronta dentro da fórmula. Critério de
investigação: produzir diferenças audíveis e editáveis de fraseado, densidade,
tensão, repouso, contraste ou forma. Isso não substitui avaliação musical.
Arquivos alterados: README e TAREFAS. Nenhuma validação técnica foi necessária
ou executada; próximo passo: aplicar este princípio ao avaliar as receitas
generativas e registrar exemplos de escuta comparativa.

### Licença do instrumento

O autor confirmou que o Batruc usa a licença adotada nos demais instrumentos
RASGO: GNU AGPL-3.0-or-later. O projeto já possui o texto integral em `LICENSE`
e a licença declarada no README; esta confirmação registra que ela se aplica
ao código autoral, inclusive à direção de desenvolvimento C++/JUCE. JUCE e
quaisquer outros componentes de terceiros conservarão suas licenças e avisos
próprios, a documentar para distribuição. Conferidos a licença local e a
convenção dos instrumentos RASGO; nenhum arquivo de terceiros foi alterado.

### Direção de produto: edição nativa e edição web — 2026-10-24

O autor definiu que o instrumento completo será desenvolvido em C++/JUCE e
disponibilizado para download multiplataforma, mantendo também uma edição em
página web para experimentação de usuários, com escopo potencialmente mais
básico. O protótipo HTML/Canvas/Web Audio já existente é pesquisa local, não é
ainda a edição web pronta para usuários. As duas edições devem expressar o
mesmo vocabulário de receitas e relações composicionais; para cada receita
compartilhada, comparar as saídas com os mesmos parâmetros. A estratégia de
compartilhamento do core ainda está aberta: investigar compilação do core para
navegador versus implementação web própria limitada a receitas definidas. Não
presumir que o WebView embutido em JUCE produza por si só uma aplicação de
navegador. JUCE documenta destinos nativos multiplataforma e um componente de
navegador embutido ([plataformas](https://juce.com/features/),
[WebBrowserComponent](https://docs.juce.com/develop/classjuce_1_1WebBrowserComponent.html));
confirmar opções de distribuição quando a arquitetura for especificada.
Próximos passos: ouvir e validar a maquete atual; definir a primeira receita
comum; provar a viabilidade do core compartilhado; decidir escopo mínimo da
edição web e matriz de sistemas/formatos para a edição nativa. Hospedagem e
publicação da página serão planejadas separadamente. Atualizados README e
TAREFAS; nenhum build, teste, empacotamento ou publicação foi executado.

### Tarefa 3 concluída no nível de especificação — gestos e estado

Especificada a sequência: construir uma receita de duas vozes; percorrê-la com
o relógio musical como autoridade; deslocar fase ou razão/ciclo; recuperar uma
versão anterior sem apagar o estado atual. Durante execução, a edição fica
pendente e aplica-se no começo do próximo compasso; fora da execução, aplica-se
imediatamente. Cada mudança gera versão identificável; recuperar conserva a
versão de saída para permitir novo retorno. A animação consulta posição musical
e não agenda som. Esta regra deverá valer nas edições JUCE e web, embora a
maquete atual ainda não implemente histórico versionado nem troca quantizada.
Os precedentes do Navalha 2 — MEMORY, locks, KEEP/RESTORE, FORM, GAP e
TRACE/LOOP — ajudam a especificar memória, proteção seletiva, mudança reversível,
forma e gesto; repetição foi traduzida para eventos musicais, não para cópia de
fragmentos. README e TAREFAS atualizados. Validação documental; nenhum teste,
build ou áudio executado. Próximo item: tarefa 4, especificar core, ponte JUCE,
tempo real e limites, incluindo a relação com uma edição web básica.

### Triagem de recursos dos instrumentos irmãos — 2026-10-24

Consulta somente de leitura aos índices, READMEs e ao inventário global; não é
auditoria completa de código nem autorização de reutilização.

| Instrumento consultado | O que pode informar o Batruc | Aplicação possível e limite |
|---|---|---|
| [Navalha 2 PD/Web e JUCE](../NAVALHA2_PD/README.md) · [JUCE](../NAVALHA2_JUCE/README.md) | MEMORY, locks, variação reversível, KEEP/RESTORE, forma por cenas, GAP explícito, TRACE/LOOP e planos de evento limitados/sample-accurate. | Informar snapshots de receitas, parâmetros protegidos e aplicação por frase. Retriggers já foram adaptados a eventos; STUTTER/BURST de áudio não são copiados. A licença/código do Navalha permanece separado. |
| [Rasgo Synth Studio](../RASGO_SYNTH_STUDIO/README.md) | Geração de peça por seed, forma narrativa, relações entre vozes, catálogo de mecanismos, timbre por frase, seed lineage e observação de eventos expressivos. | Informar a gramática de receitas, reprodutibilidade e memória/proveniência composicional. O Batruc deve conservar intervenção explícita; licenças precisam ser verificadas por arquivo, incluindo fontes históricas GPLv3 e fontes autorais atuais AGPL. |
| [Rasgo Synth Performance](../RASGO_SYNTH_PERFORMANCE/README.md) | Campo performativo em que seed, gesto humano, módulos e relações transformam uma trajetória; distinção entre módulos/lanes e quatro stems de saída. | Informar gestos que deslocam relações sem apagar âncoras e a separação voz geradora/mix bus. O teto de quatro stems e a identidade visual do Performance não definem o Batruc. |
| [Aquorbium](../AQUORBIUM/README.md) | Diretor formal A/A′/B/suspensão, memória de seção e eventos coletivos; energia, fase, geometria e ressonadores físicos. | Forte referência para transformar um modelo em desenvolvimento formal e explorar matéria sonora. Ecologia e física só devem ser nomeadas se a receita do Batruc sustentar os modelos; diversos DSPs seguem experimentais ou com escuta pendente. |
| [Antitotem](../ANTITOTEM/README.md) | Osciladores, scanner de 16 passos, divisores e lógica de clock CMOS, S&H, feedback, memória e registro de ações. | Informar receitas discretas de divisões, portas e padrões temporais e uma apresentação didática de causa/efeito. Manter a genealogia material CMOS/Lunetta do Antitotem; não clonar o instrumento nem a breadboard. |
| [Trioio](../TRIOIO/README.md) | Conversa escuta→composição→julgamento, comparação A/B no mesmo trecho, avaliação que alimenta memória; arquivo de 2.576 escalas e 26 ciclos rítmicos. | O fluxo de comparação é um bom modelo de avaliação humana das receitas. Escalas e ciclos podem ser material opcional de conhecimento, com fonte/contexto preservados; não transformar tradições em presets aleatórios. O arquivo é um recurso autoral do Trioio declarado transversal, mas qualquer cópia/adaptação segue proveniência. |
| [Maravi](../MARAVI/README.md) | Pipeline de transcrição com candidatas, incerteza, revisão humana e proveniência de áudio/partitura. | Não é gerador composicional. Pode informar no futuro uma exportação/checagem de partituras e a exposição de incerteza do resultado, sem transplantar seu pipeline AMT para o core musical. |
| [Rasgo Modular](../RASGO_MODULAR/README.md) | 60 módulos em oito famílias; Function Generator, clock Euclidiano, Decision determinística e PHRASE com pausas/cadências/acentos; patch legível como conexões. | Candidato operacional mais próximo para primitives matemáticas limitadas e eventos. Estudar `FunctionGenerator`, `EuclidClock`, `Decision` e `Phrase`; preservar a composição por receitas e trajetórias do Batruc, sem torná-lo apenas outro patchbay. Código AGPL, cópia fixada e testes no consumidor antes de adaptação. |
| [Risco](../RISCO/README.md) | Escrita, caracteres, traço, sobreposição, apagamento, leitura e matemática como operações de composição com memória. | Afinidade conceitual com o desenho como ação, mas o material central é inscrição/escrita. Referência de fronteira para manter projetos distintos; compartilhar perguntas sobre gesto e vestígio, não fundir identidades. |

**Contribuição da triagem à tarefa 4:** especificar uma receita própria e pequena com
primitives de função/clock/frase do Rasgo Modular; memória e proteção seletiva
inspiradas por Navalha 2; arco formal comparável ao diretor do Aquorbium; e um
protocolo de comparação/avaliação humana inspirado no Trioio. Studio e
Performance oferecem exemplos de seed, trajetória e expressividade; Antitotem
e Risco ficam como referências de operações e gesto; Maravi informa uma etapa
posterior de notação/checagem. Isso é uma ordem de estudo, não adoção de módulos.
Nenhum código foi copiado ou alterado. Licenças e maturidade variam: confirmar
fonte, arquivo, commit, dependências e autorização compatível antes de qualquer
reuso. Validação: leitura documental/preflight; nenhum build, teste ou áudio.

### Oitavo incremento implementado — seed para seleção de receitas, 2026-10-24

O host JUCE aceita uma seed de oito caracteres hexadecimais, normaliza letras
para maiúsculas e aplica um mapeamento determinístico v1 aos quatro eixos já
preparados: ciclos da voz 2, fase, regra de eventos e superfície. A seed pode
ser guardada na cena e restaurada junto com a combinação exata. Editar qualquer
um desses quatro controles remove a atribuição de seed; entradas inválidas não
alteram a receita ativa. Teste manual do usuário em 24/10/2026 confirmou que
está funcionando. Isso torna a seleção reprodutível neste mapeamento,
mas não cria fórmulas, parâmetros ou agendas novos. O histórico continua
limitado a 16 cenas em memória de sessão. Não foram executados testes, GUI nem
escuta. Preflight antes do build: 15 GiB disponíveis, swap em 0 B, sem outro
processo pesado; imediatamente antes da compilação final: 14 GiB disponíveis e
swap em 0 B. `cmake --build .local-build/native/linux-debug-juce --target
batruc_app --parallel 2` concluiu em 24/10/2026 às 19:39 CEST, validando
compilação/link depois dos ajustes à API de `TextEditor`. SHA-256 do executável:
`a421bf2ce264ca15e46a8c0e5b0db34e4629775320e8b69acfb6026d4307907c`.
Próximo passo: receitas editáveis com identidade e proveniência, mantendo
preparação de agendas fora do callback e versionando o contrato de seed antes
de persistência ou compatibilidade entre versões.

### Ajuste da janela “Sobre” após relato de instabilidade — 2026-10-24

O usuário observou que a instabilidade pode ter ocorrido ao abrir “Sobre”. A
falha não foi reproduzida; na inspeção, o diálogo não estava associado ao botão
que o abre e seu texto descrevia um estado anterior do protótipo. O diálogo
agora recebe o botão como componente associado para posicionamento, e o texto
nos quatro idiomas descreve as duas vozes, o banco de 3.030 receitas, seed e
cenas, deixando explícito que edição livre e parâmetros avançados continuam
pendentes. Build local de `batruc_app` concluído às 19:50 CEST; não foram
executados testes nem aberta a interface para reproduzir o sintoma. SHA-256 do
executável: `c953c6493d4db6b9cfdf28ff9bcd6718f7470b4ab91db1053c420066408aac76`.
Próximo passo: reabrir a aplicação e verificar “Sobre”; se a instabilidade
persistir, registrar se houve congelamento, fechamento ou falha de layout.

### Nono incremento — andamento global por seção, 2026-10-24

O host oferece controle inicial de 60–180 BPM para a seção fixa de 16 tempos (quatro
compassos de 4/4), com `T = 960/BPM` segundos. Os planos preparados permanecem
em posições relativas à seção; o renderer converte essas posições para a
duração ativa, sem reconstruir vetores no callback. Em pausa, a troca de BPM
mantém a posição normalizada; durante a execução, entra no início da próxima
seção. As cenas de sessão agora preservam BPM junto da receita e da seed. Ainda
não há fórmula de compasso variável, swing, groove, forma ou editor livre de
fórmulas. Preflight: 15 GiB disponíveis, swap 0 B e nenhum processo pesado.
`cmake --build .local-build/native/linux-debug-juce --target batruc_app
--parallel 2` concluiu em 24/10/2026 às 19:50 CEST. Não foram executados testes
nem aberta a interface; a validação manual do BPM fica pendente. SHA-256 do
executável: `f61de69c03be2ea0b253c069572f48bea230998ca81e408936148097d252b6cf`.
Próximo: usuário testar a transição de andamento e então iniciar o painel de
fórmulas/transformações sobre receitas editáveis.

### Décimo incremento — extensão de andamento e escala temporal do gráfico

A faixa de BPM foi ampliada para 20–220 após orientação do usuário. O eixo
temporal do gráfico agora usa a duração ativa da seção, e o zoom pode enquadrar
um segundo mesmo nas seções mais longas; ao encurtar a seção, a janela é
ajustada para permanecer dentro do limite de um segundo. A faixa corresponde a
seções de 48 s a aproximadamente 4,36 s, dentro do teto de 84 s do core. A
agenda usa posição normalizada Q32 e escala para a duração ativa por
multiplicação/deslocamento, sem divisão ou reconstrução de agenda no callback.
Build local concluído em 24/10/2026 às 20:04 CEST após preflight com 15 GiB
disponíveis, swap 0 B e sem outro processo pesado; após o build, 14 GiB
disponíveis e swap 0 B. Não foram executados testes nem aberta a interface.
SHA-256 do executável:
`90def3f500b5abd0fd981742d723bfcacdb9dcf5f766bd060a92f95a89a7a6cb`. Próximo:
usuário validar visualmente/auditivamente 20, 120 e 220 BPM; a máquina de
fórmulas e transformações permanece pendente.

### Décimo primeiro incremento — diálogos de aprendizagem associados à janela

Após o relato de lentidão no botão “Aprender” e instabilidade ao abrir janelas,
os diálogos de tópico agora usam o botão “Aprender” como componente associado,
assim como “Sobre” usa seu botão. O callback do menu protege a referência à
janela principal. O sintoma não foi reproduzido; a instância que o usuário
mantém aberta não foi encerrada, e a nova compilação só será usada na próxima
abertura. Build de `batruc_app` concluído em 24/10/2026 às 20:06 CEST;
preflight: 15 GiB disponíveis, swap 0 B, sem processo pesado visível; após o
build: 14 GiB disponíveis, swap 0 B. Não houve teste nem inspeção de GUI.
SHA-256: `8c78d526894995cab716e33362567d467f4a8a3eaf27efc264722d8fad77a5e9`.
Se ainda houver lentidão, o próximo passo é separar atraso ao abrir o menu,
abrir o tópico e fechar a janela, e então medir a interação em execução.

### Décimo segundo incremento — painel interno de Aprender/Sobre

“Aprender” agora apresenta os seis tópicos em abas dentro de um painel sobre a
janela principal; “Sobre” usa o mesmo painel, sem abrir outro diálogo. Um
componente de fundo intercepta cliques enquanto o painel está aberto, e trocar
de tópico só atualiza o conteúdo, sem reordenar a janela e todos os controles.
Isso reduz a criação e destruição de janelas e evita que os controles do
instrumento recebam cliques por trás do painel. A hipótese de que novas janelas
causavam a lentidão/instabilidade relatada não foi reproduzida; a correção é
estrutural, não uma medição de desempenho. A instância já aberta pelo usuário
permanece intacta e precisa ser reaberta para usar este binário. Build e hash a
registrados: preflight com 15 GiB disponíveis, swap 0 B, 983 GiB livres no
volume de trabalho e sem processo pesado visível; após o build, 14 GiB
disponíveis e swap 0 B. `timeout 180s cmake --build
.local-build/native/linux-debug-juce --target batruc_app --parallel 2`
concluiu em 24/10/2026 às 20:11 CEST. Não houve testes nem inspeção da
interface. SHA-256: `699dfd0f796a548285f749c6d1cda52c5ee535409b0685574bef60fe15e41982`.
Próximo: usuário reabrir o novo executável e verificar resposta de Aprender,
troca de abas, Sobre e fechamento; se persistir lentidão, medir cada interação
separadamente.

### Décimo terceiro incremento — primeira camada da máquina de fórmulas

O host agora oferece quatro leis para calcular intensidade por ataque a partir
das coordenadas já preparadas: constante, projeção de `y`, produto `xy` e
diferença quadrática `x²−y²`. O controle `λ` mistura o valor normalizado da lei
com intensidade uniforme. `λ=0` mantém intensidades iguais; `λ=1` aplica o
resultado completo. A fórmula é avaliada em cada evento no callback com
operações limitadas, sem trigonometria, alocação ou reconstrução da agenda.
O som recebe a intensidade calculada, e os marcadores nos gráficos crescem com
ela. Seed escolhe deterministicamente uma lei e `λ` além da receita geométrica;
cenas de sessão guardam ambos. Esta é uma transformação de dinâmica, não ainda
um editor de fórmulas arbitrárias ou de funções temporais. Próximo: ampliar as
leis para articulação e duração, mantendo o transporte comum às vozes.

Preflight: 15 GiB disponíveis, swap 0 B, 984 GiB livres no volume do projeto e
nenhum processo pesado visível. Após o build: 14 GiB disponíveis, swap 0 B.
`timeout 180s cmake --build .local-build/native/linux-debug-juce --target
batruc_app --parallel 2` concluiu em 24/10/2026 às 20:29 CEST. SHA-256 do
executável: `139b2599a96cc2cdb7885c810bd32fabd579e8e2f5e75b88d911da25dad18251`.
Não foram executados testes nem aberta a interface; som, resposta do controle e
clareza didática aguardam revisão auditiva/visual do usuário. A instância que
já estava aberta foi preservada; o novo binário só vale após reabrir o app.

### Décimo quarto incremento — seleção de vozes e mixer de doze canais

As 3.030 receitas agora incluem doze trajetórias de ataque no transporte
comum. Além das duas primeiras vozes, há dez perfis virtuais com ciclos,
deslocamentos de fase, panoramas, frequências e distribuições modais distintos.
O controle “Vozes” ativa de 2 a 12 partes; seed determina a quantidade, e as
cenas salvam/restauram esse valor. O painel Mixer mostra os doze canais com
volume, mute e solo. Vozes desativadas não recebem novos ataques; ao reativar,
seus estados modais começam limpos. Ao desativar canais durante a reprodução,
um fade curto esvazia a saída antes de suspender seu processamento. Os gráficos
temporais mostram as vozes ativas; no campo XY, os eventos compartilham a
trajetória das duas primeiras vozes.

O teto total de eventos preparados passou a 262.144; o teto por seção continua
65.536. Para o banco atual, a soma dos limites de reserva dos planos é 246.036
eventos, antes dos filtros de superfície, abaixo do novo teto. Esse é um limite
calculado a partir das receitas, não uma medição de memória ou CPU. A saída
continua em um master estéreo: stems graváveis/exportáveis, ciclos e fases
editáveis por voz e mais controles de intenção/articulação continuam pendentes.

Build local de `batruc_app` concluiu em 24/10/2026 às 20:42 CEST, após
preflight com 15 GiB disponíveis, swap 0 B e 984 GiB livres no volume de
trabalho; durante/depois, 14 GiB disponíveis e swap 0 B. `git diff --check`
passou. Não rodei testes nem abri a interface ou áudio; a validação de uso e a
confirmação de que a preparação das 12 vozes completa com sucesso ficam para a
próxima abertura manual. SHA-256: `1a42f02c4cff87f9514bb2167ee5680f49aa20285099b7f1b2a35ea97f6b87a7`.
A instância em uso foi preservada.

**Revisão de integração — canais inativos no mixer:** controles de volume,
mute e solo agora ficam desativados quando a voz correspondente está fora da
contagem selecionada. A avaliação de solo já considera apenas vozes ativas;
assim, um solo residual de canal inativo não interfere no conjunto audível.
Ainda é necessária conferência manual na janela e escuta do novo binário.

**Incremento de paridade com a maquete web — matéria sonora e panorama:** o
Mixer agora permite panorama estéreo por voz, como na maquete. A matéria sonora
global oferece quatro interpretações modais das formas senoidal, triangular,
dente de serra e quadrada; elas ponderam os oito modos do ressonador virtual e
não são osciladores Web Audio nem uma síntese acústica validada. Cenas da sessão
guardam panorama por voz e a matéria global. O LFO continua reservado ao baixo,
cuja pista editável ainda precisa ser portada. Próxima paridade recomendada:
transformações por voz e suas fórmulas/gráficos, depois baixo com LFO e controles
de compasso/expressão que já existem na maquete.

Build local do alvo `batruc_app` concluiu em 24/10/2026 às 20:54 CEST após
preflight com 14 GiB disponíveis, swap 0 B e 984 GiB livres. Binário em
`.local-build/native/linux-debug-juce/batruc_app_artefacts/Debug/Batruc`,
SHA-256 `73c27405e104b13af89074db3d603901c70fd6d388dd4cb8f076e47286617986`.
Não rodei testes nem abri a janela; a escuta e a revisão visual continuam
pendentes. A instância já aberta foi preservada.

### Tarefa 4 concluída no nível de especificação; implementação parcial

Foi especificado core C++17 independente de interface, JUCE 9.0.3 fixado para
o aplicativo standalone e alvo web candidato que compila o mesmo core para
WebAssembly/AudioWorklet com UI HTML/Canvas. O caminho local usa CMake, JUCE
externo e `.local-build`. O contrato separa receita, geometria/eventos,
renderer e host; define timestamp musical como autoridade e atualização de
receita em fronteiras; e especifica planos de capacidade fixa sem heap, locks
ou I/O no callback. Intensidade, articulação, matéria, reexcitação e
abafamento ficam no evento/renderer, não no relógio nem na animação. Tetos
previstos: 12 vozes, 84 s, 65.536 eventos, 64 comandos, 4.096 pontos visuais
por ciclo, pool de 48 corpos e cauda máxima de 20 s; são valores provisórios
para benchmark e escuta, não garantias medidas. Referências:
[JUCE 9.0.3](https://github.com/juce-framework/JUCE/releases/tag/9.0.3),
[política de suporte JUCE](https://juce.com/security/) e
[Wasm AudioWorklets do Emscripten](https://emscripten.org/docs/api_reference/wasm_audio_worklets.html).
Esse plano foi parcialmente implementado: há core sem JUCE, renderer modal
limitado, host standalone, agendas preparadas fora do callback, transporte de
amostras, duas vozes, três superfícies, regras Centro/Extremos, fase da segunda
voz, mixer, zoom, seletor seed básico e andamento global. Ainda faltam
cenas/versionamento persistentes, receitas editáveis, transformações livres,
baixo, forma completa, stems/gravação e alvo WebAssembly/AudioWorklet. A
interface nativa agora seleciona de 2 a 12 vozes e inclui mixer para doze
canais; a seed escolhe a quantidade, mas ainda não gera fórmulas ou agendas
novas. Os tetos desta especificação
são provisórios e precisam de benchmark/escuta; os limites efetivos do core
estão registrados na decisão arquitetural acima. O próximo trabalho de código
é concluir o item 1 da fila ativa: ampliar a memória de versões da interface
para cenas editáveis com plano preparado e seed, antes de expor parâmetros que
exigem novo plano de eventos.

### Perfis espectrais próprios e auditoria DaisySP/Faust — 25/10/2026

**Referências verificadas:** DaisySP declara MIT para o núcleo e oferece
osciladores PolyBLEP, ADSR, filtros, percussão sintética e modelagem física;
`DaisySP-LGPL` é submódulo separado e não foi incorporado. Faust gera C++ e
WebAssembly, mas a licença dos resultados depende dos arquivos e bibliotecas
usados. Pure Data fica como bancada de prototipagem, com licenças por arquivo
e external a conferir. As fontes oficiais, licenças e limites estão em
[`BIBLIOGRAFIA_DE_ESTUDO.md`](BIBLIOGRAFIA_DE_ESTUDO.md) e no Atlas RASGO.
Decisão: não trazer uma biblioteca completa antes de fixar versão e conferir
licença por arquivo e compilação nos dois destinos.

**Mudança:** implementei no core C++/JUCE e na maquete web um perfil espectral
de três senos, ponderados por `1:0,34:0,16` e normalizados, com quatro relações:
harmônica `1:2:3`, ímpar `1:3:5`, inarmônica `1:√2:√3` e batimento lento
`1:1,003:0,997`. A soma passa pelo filtro do baixo. A maquete registra a
escolha e a equação na partitura matemática; cenas JUCE persistem o perfil no
schema 13 e cenas anteriores abrem em perfil harmônico. O código é autoral;
nenhuma fonte externa ou código de outro instrumento foi copiado.

**Arquivos:** `src/core/BassAudio.hpp/.cpp`, `src/app/Main.cpp`,
`prototype/index.html`, `prototype/app.js`, `prototype/style.css`,
`BIBLIOGRAFIA_DE_ESTUDO.md`, este documento e o Atlas RASGO.

**Validação:** preflight antes do build: 12 GiB disponíveis, 512 KiB de swap
usados de 2 GiB, 983 GiB livres no volume e sem build/render pesado visível.
`cmake --build .local-build/native/linux-debug-juce --target batruc_app
--parallel 2` concluiu; `git diff --check` passou. Nenhum teste, interface ou
áudio foi executado. O perfil está implementado, mas não validado ao ouvido nem
comparado perceptualmente entre as saídas. `node --check prototype/app.js`
passou. SHA-256 do executável local:
`4f669da18b0cf47a694d6121b1ed2e965f0540f06a49cd36baf3abd2d33c2e73`.

**Próximo passo:** ouvir a mesma linha e seed nos quatro perfis em JUCE e web;
confirmar os níveis e decidir se a inarmonicidade e os batimentos são
composicionalmente úteis. DaisySP permanece candidato a um experimento
pequeno e isolado, não dependência do Batruc.

### Paridade HTML/JUCE — fórmula de compasso, 25/10/2026

O seletor nativo agora oferece 3/4, 4/4, 5/4 e 7/8, como a maquete. As quatro
barras da seção têm a duração correspondente ao BPM e à fórmula; swing e mapas
de acento das quatro levadas usam a métrica selecionada. A grade temporal do
gráfico marca subdivisões e limites de barra. A alteração acompanha o transporte
na próxima fronteira de seção durante a execução, e entra imediatamente quando
está pausado. Cenas novas guardam `meter_index` no esquema 17; versões anteriores
continuam abrindo em 4/4.

O baixo JUCE recebe BPM, métrica e groove, mas ainda representa cada barra por
16 células normalizadas. A maquete web usa 12, 16, 20 ou 14 passos efetivos em
3/4, 4/4, 5/4 e 7/8. Essa diferença de resolução e a edição visual por passos
continuam pendentes; este incremento não declara paridade completa do baixo.

**Arquivos:** `src/app/Main.cpp`, `src/core/CompositionCore.hpp/.cpp`,
`src/core/BassComposition.hpp/.cpp`, README e este registro.

**Validação:** preflight de 25/10/2026: 11 GiB disponíveis, 768 KiB usados de
2 GiB de swap e 983 GiB livres; nenhum processo de build concorrente observado.
`cmake --build .local-build/native/linux-debug-juce --target batruc_app -j2`
concluiu. `git diff --check` passou. Não rodei testes nem abri a interface ou
áudio; a validação auditiva, visual e comparação com o HTML permanecem pendentes.

**Próxima fatia de paridade:** conferir repetição de célula, pausa, duração e
fraseado com a mesma seed no HTML e no JUCE; aproximar as regras de duração
Fibonacci/geométrica e registrar as diferenças restantes.

### Paridade HTML/JUCE — grade editável do baixo, 25/10/2026

O core nativo agora reserva 20 posições por compasso e calcula a grade ativa
pela fórmula: 12 passos em 3/4, 16 em 4/4, 20 em 5/4 e 14 em 7/8. Cada passo
equivale a uma semicolcheia (`15/BPM`); quatro compassos determinam a duração da
partitura. A execução limita as durações algorítmicas ao restante do compasso e
respeita pausas explícitas. O editor mostra dez células por linha e só apresenta
as posições válidas no compasso escolhido. Clicar em uma célula percorre vazio,
nota e pausa, para que silêncio explícito possa interromper uma nota sustentada.

Cenas novas usam esquema 18 e preservam 20 células. O leitor aceita os esquemas
anteriores com 16 células e completa as demais com células vazias. Operações de
retrogradação e recombinação também passam a manter a resolução métrica do score.
O script `./run` abre o executável Linux já compilado e informa o comando de
build caso ele não exista; não recompila por conta própria.

**Arquivos:** `src/core/BassComposition.hpp/.cpp`,
`src/core/GenerativeOperations.cpp`, `src/app/Main.cpp`, `run`, README e este
registro.

**Validação:** preflight de 25/10/2026: 11 GiB disponíveis, swap em 768 KiB de
2 GiB e 983 GiB livres; sem processos pesados concorrentes. O alvo
`batruc_app` compilou com `-j2`. Não executei testes nem abri o app. A leitura
visual, o comportamento ao trocar o compasso e a escuta comparativa continuam
pendentes.

### Fila ativa — conteúdos do baixo

Paridade ainda incompleta. Lei/razão de duração, fraseado, repetição e intenção
por célula, relação rítmica baixo/vozes, seis matérias sonoras, perfil dos três
osciladores, VCF, envelopes ADSR e variação de alturas por seed estão
representados nas duas saídas. Falta conferir audição e persistência em
execução, comparar comportamento/valores e portar o editor gráfico ao JUCE.
Não declarar o módulo equivalente antes de conferir os dois destinos.

### Implementação — controles de composição do baixo e fechamento do painel, 25/10/2026

A JUCE agora expõe a lei de duração (manual, Fibonacci, geométrica), razão
geométrica, frase/pausas e, para a célula selecionada, repetição (1/2/4/8) e
intenção (normal/ghost/acento). Esses valores atualizam a receita e a partitura
preparada; as cenas já serializavam os campos do core. Reorganizei a faixa em
cinco colunas para respeitar a largura real reduzida do painel e evitar controles
espremidos. A maquete valida os parâmetros do VCF também quando recebe uma
configuração antiga sem esses campos, adotando 420 Hz e Q 0,707.

Corrigi ainda o fechamento do painel: swing, LFO, perfil espectral, gráfico e
controles do baixo agora são ocultados explicitamente ao fechar. O relato do
usuário indicava que esses componentes permaneciam visíveis como fragmentos.

**Validação:** preflight: 10 GiB disponíveis, swap usado 768 KiB/2 GiB, 983 GiB
livres e sem processos pesados concorrentes. `batruc_app` compilou com `-j2`;
`node --check` passou em `prototype/score.js` e `prototype/app.js`. Não rodei
testes automatizados nem abri a interface; portanto, o conserto visual ao
fechar e a leitura auditiva ainda precisam de conferência manual. SHA-256 do
executável: `78a0b24ea960eb0e941cf1c25dce0b4b90691ee832b86aa80a2f8dfcc4831b3f`.

**Pendente na paridade do baixo:** matérias sonoras equivalentes, confirmar VCF
e ADSR no ouvido, variação de alturas por seed e edição gráfica nativa; a
interação rítmica baixo/vozes será conferida por audição e ao alternar receitas,
fases, ciclos e mute. Não marcar a paridade como fechada até completar e
conferir esses itens nos dois destinos.

### Implementação — relação rítmica do baixo com as vozes, 25/10/2026

HTML e JUCE agora oferecem três modos: independente, seguir ataques e responder
após um ataque vocal. Seguir desloca o ataque do baixo até metade do caminho
para o ataque vocal mais próximo, desde que esteja a até 1,5 semicolcheia;
responder procura colocar o baixo meio passo após a voz, limitando o ajuste a
uma semicolcheia. A regra preserva ataques fora dessa vizinhança e mantém cada
evento dentro de sua seção/ciclo. No JUCE, alterações de ciclos, fases, regra de
ataque, número de vozes e receita recalculam a agenda do baixo fora do callback.
Cenas novas guardam a relação no esquema 20; cenas anteriores abrem em “seguir”.

**Validação:** preflight: 9,1 GiB disponíveis, swap 1 MiB/2 GiB, 983 GiB livres
e sem processos pesados. `batruc_app` compilou com `-j2`; `node --check` passou
nos dois scripts web. Não rodei a suíte de testes nem fiz audição/interface. A
regra é implementada, ainda sem validação perceptiva; revisar especialmente a
transição de ataques movidos perto dos limites dos ciclos. SHA-256 do
executável: `7bd4986ab14e3d4da2549652c208668868a9c961969cbf1b92c55ebb591f2095`.

### Implementação — matérias sonoras do baixo no JUCE, 25/10/2026

Portadas ao renderer nativo as seis matérias já oferecidas no HTML (Subgrave,
Redondo, Corpo, Pulso, Brilhante e Pluck), com o mesmo corte-base de filtro.
Pluck também recebe um decaimento de amplitude próprio. A escolha é imediata no
áudio, fica registrada em cenas a partir do esquema 21 e cenas anteriores
continuam abrindo com a matéria Redondo. O painel mantém o seletor no layout e
oculta tanto o rótulo quanto o controle ao fechar o módulo, evitando deixar
fragmentos visuais.

Validação: preflight em 25/10/2026: 8,8 GiB disponíveis, swap 1 MiB/2 GiB,
983 GiB livres e nenhum processo pesado concorrente. O alvo `batruc_app`
compilou com `timeout 180s cmake --build
.local-build/native/linux-debug-juce --target batruc_app --parallel 2`;
`node --check prototype/app.js` e `node --check prototype/score.js` passaram.
SHA-256 do executável: `680394eaa8c7bbe3b85c42beb6d1627c9fff3a6ece5406f69320532ead4b24d7`.
Não rodei testes, não abri as interfaces e não fiz escuta; a correspondência dos
controles não comprova igualdade perceptiva. Próxima fatia: expor no HTML os
envelopes de amplitude e filtro já ajustáveis no JUCE, persistir os mesmos
valores nas duas saídas e depois comparar a resposta sonora.

### Implementação — envelopes ADSR e gate na maquete HTML, 25/10/2026

A maquete agora expõe nível, ataque, decaimento, sustentação e soltura da
amplitude; ataque, decaimento, sustentação, soltura e profundidade do envelope
do filtro; e proporção de gate. Os mesmos valores, limites e padrões do renderer
JUCE são validados pelo gerador e guardados no objeto de composição e na
partitura matemática. Ao tocar, os envelopes são agendados por nota; o gate
determina quando começa a soltura, e o envelope do filtro move o corte a partir
da frequência-base. Pluck preserva seu decaimento adicional de matéria sonora.
As alterações dos controles afetam novas notas do estudo em execução; eventos
que já entraram na janela de agendamento (120 ms) mantêm os valores anteriores.

Validação: `node --check prototype/app.js` e `node --check prototype/score.js`
passaram. Não executei testes nem abri a página/áudio; conferir o layout em
telas estreitas e a audibilidade dos envelopes continua pendente. Próximo:
alinhar a variação de alturas do baixo entre HTML e JUCE e revisar o editor
gráfico nativo.

### Implementação — alturas do baixo derivadas da seed no JUCE, 25/10/2026

O controle de variação de altura agora existe no painel nativo, de 0 a 100%,
com o mesmo padrão (0%) e faixa da maquete HTML. A aplicação da seed usa no
baixo o mesmo hash FNV-1a dos oito caracteres hexadecimais normalizados; o
renderer mantém o mesmo gerador determinístico, chance por célula, raio de
vizinhança e ordenação de candidatos por classe de nota. A seleção de seed
reconstrói a linha imediatamente. O editor e a leitura do LFO mostram a altura
escrita, a altura derivada e a altura após LFO, sem substituir a nota original.
O valor de variação e a seed interna já são preservados no estado de cena.

Validação: preflight em 25/10/2026: 9,3 GiB disponíveis, swap usada 1 MiB/2 GiB,
983 GiB livres; processos de navegador estavam ativos, sem outro build/render.
`batruc_app` compilou com timeout de 180 s e dois jobs. `node --check` passou
para `prototype/app.js` e `prototype/score.js`. SHA-256 do executável:
`f184717e324a134da57a3f2bdab7bc0fdb0c7c32cfb91308276d2a38573b894b`.
Não rodei testes nem abri a interface para escuta; a igualdade de valores deve
ser conferida com seeds de referência nos dois destinos. Próximo: revisar a
paridade do editor gráfico de notas e conferir manualmente a seleção de seed,
pausas e células alteradas.

### Implementação — piano-roll gráfico do baixo no JUCE, 26/10/2026

Adicionei ao painel nativo um piano-roll gráfico de 150 px com o editor de
alturas e durações que existia na maquete HTML. Clique em célula vazia cria uma nota; arrastar o corpo altera a altura e
arrastar perto da borda direita altera a duração para os valores disponíveis
(1, 2, 4, 8 ou 16 passos). Botão direito grava pausa; Shift+botão direito limpa
a célula. Clique em nota seleciona sem apagá-la. A grade numerada de passos
continua no painel e mantém a alternância nota/pausa/vazio, também para uso por
teclado. O gráfico mostra a lei de duração calculada e limita a barra ao próximo
evento e ao final do compasso.
O modo **Desenhar traço**, ausente no primeiro piano-roll nativo, agora percorre
passos contínuos e interpola a altura entre as posições do gesto. Cria uma nota
de um passo nos vazios e move as notas existentes sem alterar suas durações;
pausas atravessadas tornam-se notas, como no editor web, e os demais passos não
são alterados.

Arquivos alterados: `src/app/Main.cpp`, `README.md` e `TAREFAS.md`. O callback
edita a mesma `BassRecipe` usada pelos controles e pede a reconstrução da
performance; não existe estado paralelo do piano-roll. O painel flutuante oculta
o gráfico junto dos outros controles ao fechar.

**Validação:** preflight em 26/10/2026: 7,0 GiB disponíveis, swap usada cerca
de 3 MiB de 2 GiB, 983 GiB livres e nenhum processo pesado visível. O comando
`timeout 180s cmake --build .local-build/native/linux-debug-juce --target
batruc_app --parallel 2` concluiu com sucesso. SHA-256 do executável:
`0d8eb336c00cdf1b31e54a94a669fe4d09798c24da0000daf2a64eef810d6b1a`.
Não rodei testes automatizados, não abri a janela e não fiz escuta. O layout,
gestos, pausas e correspondência de alturas/durações com o HTML precisam ser
conferidos manualmente. Próximo: comparar as duas interfaces com as mesmas
seeds, incluindo notas derivadas, pausas e durações sob cada lei; corrigir os
desvios encontrados antes de fechar a paridade do baixo.

### Implementação — altura efetiva da seed no piano-roll, 26/10/2026

Corrigi uma diferença visual entre áudio e gráfico: a performance já tocava a
altura calculada pela seed, enquanto os dois piano-rolls desenhavam apenas a
altura escrita. Agora a barra e a trajetória mostram a altura efetiva; quando
ela diverge, um conector liga a posição original à calculada e o rótulo informa
`escrita>calculada`. Os controles e gestos continuam editando a altura escrita,
que serve de entrada para a variação. No JUCE, a altura calculada e a duração
efetiva vêm da mesma `BassScore` preparada para a receita, sem uma segunda
construção para atualizar o gráfico.

Arquivos alterados: `prototype/app.js`, `src/app/Main.cpp`, `README.md` e
`TAREFAS.md`.

**Validação:** `node --check prototype/app.js` passou; o alvo `batruc_app`
compilou com `timeout 180s cmake --build
.local-build/native/linux-debug-juce --target batruc_app --parallel 2`. No
preflight: 7,1 GiB disponíveis antes da compilação, swap usada cerca de 3 MiB de
2 GiB e 983 GiB livres; após compilar, 8,0 GiB disponíveis. SHA-256 do
executável: `5a72efe0ae945c610298db42ed0d01640381768fb5283ae710259f531501ed0f`.
Não abri as interfaces nem comparei a saída sonora. Próximo: conferir à mão as
alturas escritas/calculadas em seeds iguais nos dois destinos e verificar se a
escala e os conectores continuam legíveis quando há muitas notas.

### Implementação — edição de duração coerente com a lei ativa, 26/10/2026

Alinhei o comportamento do editor nos modos Manual, Fibonacci e Geométrica.
Quando uma lei calculada está ativa, os seletores de duração individual ficam
desativados e a borda da nota deixa de iniciar um redimensionamento; arrastar a
nota continua alterando a altura. No HTML, a instrução abaixo do gráfico passa a
explicar a diferença conforme a lei. No JUCE, a indicação do piano-roll e o
estado habilitado do seletor acompanham a lei selecionada. Isso elimina a
edição de um valor manual que a fórmula não usaria.

Arquivos alterados: `prototype/index.html`, `prototype/app.js`,
`src/app/Main.cpp`, `README.md` e `TAREFAS.md`.

**Validação:** preflight: 7,3 GiB disponíveis, swap usada cerca de 3 MiB de 2
GiB, 983 GiB livres e sem processo pesado visível. `node --check
prototype/app.js` passou e `timeout 180s cmake --build
.local-build/native/linux-debug-juce --target batruc_app --parallel 2` compilou
com sucesso. SHA-256 do executável:
`7cb31ee235739e80698fdfa5343749d6741720492367562ef6dd0bf837df24a7`. Não abri
as interfaces nem fiz audição. Próximo: conferir a apresentação das durações
calculadas nos dois gráficos para cada lei e comparar manualmente com as
durações efetivamente tocadas.

## Fechamento computacional P1 — seed, fase e receita HTML/JUCE — 27/10/2026

Concentrei o mapa determinístico em `seededRecipeSelection()` no core e
implementei o mesmo algoritmo em `prototype/score.js`. Uma seed válida de oito
hexadecimais agora escolhe, nos dois alvos, o índice de ciclo de V2, fase-base
e arco formal, regra de ataque, campo de patterning, quantidade de vozes e
lei/profundidade de intensidade. A página aplica esses valores aos controles
visíveis; eles entram no cálculo de eventos e são guardados/restaurados nas
composições locais.

Alinhei a variação por voz: mesmos sorteios e ordem de consumo da PRNG, ciclos,
acoplamentos, jitter de fase com wrap e escolha de transformação. As agendas
regulares do HTML passaram a resolver analiticamente cruzamentos e extremos,
sem depender de amostragem de 512 pontos nem deslocar ataques por acoplamento ou
transformação; o modo pendular e a origem geométrica de V1 continuam
amostrados e requerem validação própria. A filtragem espacial e as oito leis de
intensidade do core também estão disponíveis na web.

Testes fixos cruzados cobrem cinco seeds, a realização seeded em oito vozes e
todos os ataques de cinco vozes em uma agenda com pontos de inversão e ciclos
fracionários. Corrigi ainda
uma fixture preexistente de retrogradação: declarava MIDI inválido e grade de
16 passos com coordenada incompatível; agora usa 20 passos e notas válidas.

Arquivos afetados: `src/core/CompositionCore.hpp/.cpp`, `src/app/Main.cpp`,
`prototype/score.js`, `prototype/app.js`, `prototype/index.html`,
`prototype/test-score.cjs`, `tests/seeded_recipe.cpp`,
`tests/generative_operations.cpp`, `CMakeLists.txt`, `README.md`,
`PARIDADE_HTML_JUCE.md` e este registro.

**Validação:** `node --check` nos dois scripts, `node prototype/test-score.cjs`,
`git diff --check` e toda a suíte CTest passaram. Os 7 testes C++ passaram; o
build incremental do app JUCE e dos alvos de teste concluiu. Preflight antes do
build: 12 GiB disponíveis, swap em 135 MiB/2 GiB, 983 GiB livres no volume do
projeto, 7,3 GiB em `/` e nenhum processo pesado além desta sessão. Não abri
interfaces nem comparei áudio.

**Estado e próximo passo:** P1 está concluído no cálculo comum da receita
regular. A matriz sobe provisoriamente para 66,1% (Geração/seed: 85/100); essa
nota ainda não certifica a execução dos controles nem a escuta. Seguir para P2:
conferir o mesmo BPM/compasso/swing, as 12 polirritmias, transições e agenda de
baixo em uma fixture única; incluir pendências reais de fase/formal e medir os
eventos antes de qualquer comparação auditiva.

## P2 — presets polirrítmicos com tabela nativa canônica — 27/10/2026

Retirei a segunda cópia da lista polirrítmica do `Main.cpp`: JUCE agora monta
seu seletor e aplica relações usando `batruc::kPolyrhythmPresets`, definida no
core. O teste C++ compara rótulos/ciclos e quantidade de vozes dos 12 presets
com a fixture da página. A implementação HTML já mantinha os mesmos nomes e
valores; seu teste confirma também que a aplicação seleciona as vozes
necessárias, mantém os canais excedentes e fixa variação em zero para que o
preset exato não seja imediatamente alterado pela seed.

Arquivos afetados: `src/core/CompositionCore.hpp`, `src/app/Main.cpp`,
`tests/seeded_recipe.cpp`, `prototype/score.js`, `prototype/test-score.cjs`,
`PARIDADE_HTML_JUCE.md` e este registro.

**Validação:** preflight em 27/10: 11 GiB disponíveis, swap 132,5 MiB/2 GiB,
982 GiB livres no volume do projeto e 7,3 GiB em `/`; volume montado `rw`, sem
processo pesado concorrente. `node --check` passou para `score.js` e `app.js`,
`node prototype/test-score.cjs` passou, o app JUCE e `batruc_seeded_recipe_test`
compilaram com dois jobs; CTest dos casos `seeded_recipe` e
`generative_operations` passou (2/2). Nenhuma interface foi aberta e nenhum
áudio foi comparado.

**Estado e próximo passo:** os valores dos presets agora têm uma fonte nativa
única e fixtures dos dois lados; isso fecha a equivalência dos dados estáticos
da máquina de polirritmia, sem concluir a área P2 nem elevar a porcentagem.
O mapeamento matemático do swing foi validado em fixture separada abaixo.
Próximo: comparar agendas completas do HTML e `EventPlan` JUCE, incluindo
transições e baixo. O aceite auditivo continua separado.

## P2 — fixture de swing por BPM e compasso — 27/10/2026

Expus no core a função pura `swingProgress()` e a web usa `swingProgress()`
compartilhada pelo agendador de eventos. A fixture atravessa compassos 3/4,
4/4, 5/4 e 7/8, BPM 60, 97, 120 e 220 e razões de swing 50%, 57% e 67%.
Verifica os ataques no contratempo e confirma que as fronteiras de semínima
permanecem fixas. O ciclo da fixture equivale a quatro compassos completos em
cada combinação.

A fixture achou e corrigiu um erro de arredondamento: ao reconstruir o número
de semínimas a partir de duração e BPM, a web podia contar o ciclo como um
compasso parcial por um erro de ponto flutuante. Nesse caso, deixava o último
contratempo sem swing, enquanto o JUCE o deslocava. A margem numérica agora
reconhece compassos completos. Para ciclos web com duração arbitrária que
terminam no meio de uma semínima, a web preserva o trecho parcial sem swing;
essa situação ainda não tem equivalente direto na duração nativa fixa em
quatro compassos.

Arquivos afetados: `src/core/CompositionCore.hpp/.cpp`,
`prototype/score.js`, `prototype/test-score.cjs`, `tests/seeded_recipe.cpp`,
`PARIDADE_HTML_JUCE.md` e este registro.

**Validação:** preflight: 11 GiB disponíveis, swap 129 MiB/2 GiB, 982 GiB
livres no volume do projeto, 7,3 GiB em `/` e nenhum processo concorrente
pesado. `node --check` passou nos scripts; `node prototype/test-score.cjs`
passou; app e fixture C++ compilaram com dois jobs e CTest dos casos
`seeded_recipe`/`generative_operations` passou inicialmente (2/2); ao final,
a suíte CTest completa passou (7/7). Nenhum render sonoro ou escuta A/B foi
feito.

**Estado e próximo passo:** a transformação do swing em compassos completos tem
os mesmos valores nos testes HTML/C++. Continuar P2 confrontando agendas de
eventos inteiras — incluindo swing aplicado a posições de ataque, forma de
sete ciclos e pausas — antes de comparar alturas e durações do baixo.

## P2 — arco de fase e entradas da forma conferidos nos sete ciclos — 27/10/2026

Unifiquei no core as duas regras restantes da forma 2+2+1+2: a variação linear
da fase por ciclo e a contagem de vozes que entra em cada ciclo. O renderer
JUCE, os mapas/partituras exportados pela interface e o desenho usam agora o
arco formal canônico; o HTML espelha a mesma função. A quantidade ativa do
renderer e a filtragem dos eventos exportados usam a mesma regra de entrada.

Fixtures HTML/C++ percorrem todos os sete ciclos e contagens de 2 a 12 vozes,
com várias profundidades de fase. Para a receita comum de oito vozes, conferem
contagem e primeiro/último ataques de V1–V3 em cada ciclo, incluindo a pausa C5
e o retorno. Isso testa o plano analítico e as regras formais; não substitui a
comparação de todos os ataques, a execução contínua, nem a escuta.

Arquivos afetados: `src/core/CompositionCore.hpp/.cpp`, `src/app/Main.cpp`,
`prototype/score.js`, `prototype/test-score.cjs`, `tests/seeded_recipe.cpp`,
`PARIDADE_HTML_JUCE.md` e este registro.

**Validação:** preflight: 11 GiB disponíveis, swap 128 MiB/2 GiB, 982 GiB
livres no volume do projeto, 7,3 GiB em `/`; nenhum processo pesado
concorrente. O app e a fixture C++ compilaram com dois jobs; CTest completo
passou (7/7); `node --check` passou para `score.js` e `app.js`, e
`node prototype/test-score.cjs` passou. Uma primeira fixture C++ revelou que
as vozes V3+ precisavam herdar parâmetros acústicos válidos para construir o
plano; a fixture passou a inicializar os mesmos parâmetros-base usados na
receita web. Não abri interfaces nem comparei áudio.

**Estado e próximo passo:** as regras da forma e as posições-limite de V1–V3
coincidem nos sete ciclos testados. Continuar com comparação integral dos
ataques das vozes, depois alinhar os tempos e relações do baixo aos mesmos
eventos antes de declarar P2 fechado.

## Geometria: cicloide e catenária nas duas saídas — 29/10/2026

Acrescentei um arco de cicloide com retorno horizontal contínuo e catenária
normalizada ao core geométrico C++, ao modelo web, aos seletores HTML/JUCE, às
equações mostradas e ao material Learn. Ambas são fonte opcional de `y(t)` para
eventos/intensidade da voz 1. Os índices anteriores permanecem iguais; cenas
JUCE agora aceitam os ids 9 e 10. A catenária percorre a curva em ida e volta
para manter o traço fechado; não simula dinâmica de corrente suspensa. A
cicloide segue a curva matemática no arco de ida; o segmento de retorno é um
gesto explícito do traçador para reiniciar sem salto.

Arquivos alterados: `src/core/GeometryCurves.hpp/.cpp`, `src/app/Main.cpp`,
`prototype/score.js`, `prototype/app.js`, `prototype/index.html`,
`prototype/interface-i18n.js`, `prototype/guide.js`, `README.md`,
`prototype/README.md`, `PARIDADE_HTML_JUCE.md` e este registro.

**Validação:** `node --check` passou em `score.js`, `app.js`,
`interface-i18n.js` e `guide.js`; compilação sintática isolada de
`GeometryCurves.cpp` passou; o alvo JUCE compilou com sucesso usando um job.
Não executei a suíte nem abri os apps: a inspeção visual e a escuta do efeito
em V1 continuam pendentes, portanto não conto este avanço na estimativa de
paridade.

Próxima extensão visual recomendada pelo estudo do livro: ferramenta de
polígono com vértices editáveis, perímetro e área vinculados ao traço; depois,
investigar cortes planos de superfícies.

Na mesma sessão, avancei o primeiro descritor geométrico: triângulo e quadrado
exibem área pelo método do cadarço e perímetro pela soma das arestas, em
unidades normalizadas, na web e JUCE. Esses valores são didáticos e não
modulam o áudio. Após essa alteração, `node --check` passou nos quatro
scripts, a sintaxe isolada do core passou e o alvo JUCE foi reconstruído com
`-j1`. Não abri a GUI nem conferi a leitura visual dos valores.

## Pesquisa: ring theory / théorie des anneaux e anneaux geométricos — 29/10/2026

Distinção registrada no README e no Atlas: (1) teoria algébrica dos anéis,
possível base para rotações, sobreposição e convolução circular de células por
`R[x]/(x^N−1)`; (2) anel geométrico e teorema de Mamikon, em que a área varrida
`πL²` permanece igual enquanto um segmento de comprimento fixo `L`, com uma
extremidade no ponto de tangência, percorre curvas convexas fechadas suaves
diferentes. Para uma corda simétrica inteira `a=2L`, isso equivale a `πa²/4`.
A segunda propriedade não cobre deformações arbitrárias. A aplicação
algébrica foi definida como convolução ponderada, limitada e selecionável; a
aplicação geométrica continua sendo uma pesquisa sem integração ao app.

Fontes consultadas: [MIT, Abstract Algebra](https://math.mit.edu/~mckernan/Teaching/12-13/Spring/18.703/book.pdf),
[Tolimieri, cap. 6](https://ece.uprm.edu/~domingo/INEL8396/TOLIMIERI%20-%20Algorithms%20for%20Discrete%20Fourier%20Transform%20and%20Convolution%20%281989%29.pdf)
e [Mamikon/VisualCalc](https://www.cco.caltech.edu/~mamikon/VisualCalc.html).

### Modo de baixo por convolução cíclica — 29/10/2026

Converti a referência algébrica em uma opção de composição no encaixe do baixo
com as vozes, nas versões web e JUCE. Ataques ocupam uma célula de `N` passos;
três taps de pesos 0,50, 0,32 e 0,18 são deslocados circularmente. A seed e o
ciclo escolhem os deslocamentos não nulos; todo passo alcançado por um tap
gera ataque, colisões acumulam peso e a soma normalizada pelo pico modula sua
intensidade. A contribuição dominante leva os valores x/y e a intensidade do
ataque de origem ao cálculo de altura, mantendo a ligação com os eventos das vozes. Os modos independente, seguir e
responder permanecem disponíveis; cenas antigas conservam os ids 0–2 e o novo
modo usa id 3.

**Estado:** algoritmo conectado ao calendário do baixo nos dois motores; o
JUCE não aplica uma segunda aproximação temporal sobre as notas convolvidas.
Rótulos, leitura de cenas e nota didática foram atualizados. A matriz pareada
encontrou e corrigiu um erro de sinal no hash JS: após o último XOR, o valor
virava inteiro assinado antes do módulo, criando deslocamentos negativos em
algumas seeds. O hash agora é normalizado para `uint32` como no C++. Build
JUCE, verificações de sintaxe JS e o teste pareado de convolução passaram para
três casos, incluindo variação máxima, transições de fase e compassos 4/4 e
5/4. Não houve renderização/escuta de áudio. Próximo passo: validar por escuta
o modo no app e depois ampliar o gerador para polirritmias e compassos ímpares.

### Pesquisa aplicada: bilhar poligonal e espaço acústico — 29/10/2026

O autor propôs acompanhar uma trajetória que rebate nas paredes de um
polígono e usar sua relação com Fourier, movimentos periódicos e aritmética
modular na composição. A dimensão espacial deve incluir gráficos animados do
percurso e das colisões. Registrei três camadas que não devem ser confundidas:

1. **Bilhar geométrico:** uma partícula ideal percorre segmentos e reflete pela
   lei especular; cada colisão pode acionar nota, acento, timbre ou deslocamento
   estéreo. O ciclo fechado ou a sequência de lados fornece dados rítmicos.
2. **Acústica geométrica:** raios representam aproximação da propagação quando
   o comprimento de onda é pequeno perante o recinto. Distância, absorção por
   banda e difusão alteram atraso e energia; isso pode gerar reflexões iniciais
   e uma resposta impulsiva aproximada.
3. **Acústica ondulatória:** interferência e modos próprios dependem da forma,
   das condições de contorno e dos materiais. Requer resolver a equação de
   onda/Helmholtz ou usar uma aproximação numérica; não será apresentada como
   consequência automática do traçado do bilhar.

Trajetória que atinge exatamente um vértice é singular no modelo ideal e não
tem reflexão unívoca; uma simulação de ondas pode incluir difração, mas exige
outro modelo. Polígonos irracionais não recebem promessa geral de periodicidade.
Referências iniciais: [Vorobets, Gal'perin e Stepin, *Periodic billiard
trajectories in polygons*](https://www.mathnet.ru/links/bff77e1d22eb067fe6464edc97bf38ca/rm4663_eng.pdf);
[Hubert e Troubetzkoy, *Polygonal Billiards: Some Open
Problems*](https://math.iupui.edu/~mmisiure/open/ht1.pdf); [overview de modelos
de acústica geométrica de salas, JASA](https://doi.org/10.1121/1.4926438).

**Próximo passo visual/composicional:** prototipar uma única trajetória
limitada num polígono configurável, desenhando a reflexão passo a passo e
convertendo colisões em eventos com mapeamentos editáveis. Depois incluir
atraso/ganho por percurso e absorção como efeito acústico claramente separado.
Só então avaliar modos próprios; não há ainda implementação desse sistema.

### Primeiro operador espacial generativo: bilhar quadrado — 29/10/2026

Implementei um percurso periódico num quadrado nas duas versões. A posição é
calculada por movimento linear num espaço desdobrado e reflexão por dobra
triangular, equivalente à lei especular em cada parede. A velocidade percorre
duas larguras no eixo x e quatro no eixo y; o percurso fecha em posição e
direção, com seis colisões não coincidentes por ciclo. O gráfico desenha o
contorno, o caminho ainda não percorrido, o traço já percorrido, os pontos de
colisão alcançados e o móvel atual.

Com a fonte geométrica da voz 1 ligada, as colisões tornam-se eventos
musicais. A filtragem espacial passou a usar `(x,y)` da geometria para esta
voz, não as antigas senoides que só se pareciam visualmente com o trajeto. As
outras formas e vozes mantêm suas regras. `billiard-square` foi acrescentado
ao seletor, às cenas salvas e à interface traduzida em quatro idiomas.

O operador generativo usado aqui tem contrato observável: entrada = seed da
composição, ciclos e regra de eventos; cálculo = posição por reflexão,
eventos nas colisões; saídas = tempos, coordenadas e intensidades da V1. A seed
continua determinando as demais escolhas da composição; a trajetória é
determinística e parametrizada, ainda sem seed própria para origem e direção.
Este é um protótipo quadrado fixo; editor de formas, velocidade, origem,
coeficiente de restituição, parede absorvente e resposta acústica continuam
pendentes. O modelo é de raio geométrico, não reverberador nem solver de onda.

Atualizei também o princípio didático no README: recorrência, combinatória,
aritmética modular, fases, convolução cíclica, Fourier e sistemas diferenciais
devem ser apresentados como operadores de composição com entradas, regras,
limites e saídas musicais explícitos. Uma fórmula ou animação só conta como
generativa quando modifica eventos ou som de modo verificável.

Arquivos alterados: `src/core/GeometryCurves.hpp/.cpp`, `src/app/Main.cpp`,
`tests/geometry_curves.cpp`, `prototype/score.js`, `prototype/app.js`,
`prototype/index.html`, `prototype/interface-i18n.js`, `README.md`, este
registro e o Atlas de Referência RASGO. O teste `batruc_geometry_curves`
confirma seis colisões ordenadas e fechamento do percurso; build do core e do
app JUCE passou com um job. `node --check` passou nos três scripts tocados e
um cálculo JS verificou que 3 ciclos produzem os mesmos 18 eventos previstos.
`git diff --check` não apontou erros. Não abri as interfaces nem escutei a
mudança: a inspeção visual e a paridade auditiva permanecem pendentes. A
dimensão acústica segue apenas documentada.

Próximo passo: inspeção visual no HTML/JUCE e comparação auditiva dos eventos;
depois tornar origem e inclinação contínua controles de composição antes de
avançar para uma sala acústica com atrasos/absorção.

### Razões de percurso do bilhar e correção do cálculo contínuo JUCE — 29/10/2026

Ampliei o primeiro operador espacial com sete pares de voltas inteiras no
reticulado desdobrado: `2:4`, `4:2`, `2:6`, `6:2`, `4:4`, `4:6` e `6:4`.
Como ambos os números são pares, cada trajetória retorna à posição e direção
iniciais; a quantidade de impactos varia de seis a dez. O seletor de razão
aparece apenas quando o bilhar está escolhido, e a equação informa razão e
contagem. HTML e JUCE usam a mesma tabela, e o arquivo web e a cena JUCE
persistem a escolha.

Durante a extensão localizei e corrigi um limite antigo no cálculo contínuo do
JUCE: ele truncava o id de curvas em 6, de modo que índices posteriores não
alimentavam de fato a função selecionada. Agora o cálculo aceita os 12 ids.
Mantive o teste focal de seis colisões exatas e acrescentei fechamento e
contagem para as sete razões, além da diferença de áudio renderizado entre
bilhar e Fourier e entre duas razões. O teste deve assegurar que o operador
alterou o calendário e o áudio renderizado; não substitui audição humana.

Arquivos: APIs e cálculo em `src/core/GeometryCurves.hpp/.cpp` e
`src/core/CompositionCore.hpp/.cpp`; cena, controle, desenho e visualização de
gráfico em `src/app/Main.cpp`; entrada/configuração/visualização/salvamento no
browser em `prototype/index.html`, `prototype/app.js`, `prototype/score.js`,
`prototype/interface-i18n.js`, `prototype/style.css`; regressão focal em
`tests/geometry_curves.cpp`; esta continuidade, README e Atlas atualizados.

**Validação desta etapa:** rebuild JUCE com um job e suíte `ctest` completa:
12/12 passaram, incluindo o teste de geometria, que verifica sete razões,
fechamento, colisões e diferença no áudio renderizado entre razões e entre
bilhar/Fourier. As verificações `node --check` de `score.js`, `app.js` e
`interface-i18n.js` passaram. Um erro encontrado durante essa validação
ordenava o buffer inteiro de impactos, incluindo posições vazias; agora só a
faixa preenchida é ordenada. A conferência algorítmica web para três ciclos
continua coerente com 18, 24 ou 30 eventos conforme a razão.

Acrescentei regressão em `prototype/test-score.cjs` para as sete razões: testa
contagem/ordem de impactos, número exato de ataques da V1 ao longo dos sete
ciclos e reprodução determinística da mesma seed. O teste passou. O tutorial
local em PT/EN/ES/FR agora explica a diferença entre o bilhar de raios e uma
reverberação de sala.

A inspeção das janelas e a comparação auditiva continuam pendentes. O teste de
diferença de amostras confirma que o algoritmo altera o áudio calculado; não
confirma que o resultado soa musicalmente bem. Próxima etapa: tornar origem e
inclinação parâmetros composicionais contínuos com persistência e paridade
HTML/JUCE; depois implementar reflexões iniciais com atraso e absorção como um
efeito separado. A geometria do bilhar ainda não é reverberador nem solver de
ondas.

## Fechamento parcial das frentes P3/P4 — 29/10/2026

P3 recebeu duas verificações sem interação manual nas janelas. `--mixer-smoke`
no Chromium headless mediu RMS 0,01845 no baseline; 0,01586 com mute V1;
0,00969 em solo V1; 0,01296 em solo do baixo; e zero com mute master. No
JUCE, extraí a resolução de mute/solo para `resolveMixerLevels`, agora usada
por `Main.cpp`, e acrescentei `batruc_mixer_routing`: canais inativos/muted,
solo de voz, solo do baixo, faders e pan extremo esquerdo/direito. Rebuild do
app passou e `ctest` passou 13/13. Isso fecha a consequência funcional dessas
ações no core e na web; não fecha trims/master/sensor JUCE no host, persistência
em execução ou loudness percebida.

P4 ganhou smoke headless do banco web usando File System Access mockado: escolher
pasta, salvar composição com formato/schema/seed, listar, mudar seed, reabrir a
configuração original e apagar. Todos os cinco critérios passaram. O teste não
usa pasta real nem valida permissões do sistema. Uma tentativa de acrescentar
gravação via MediaRecorder real travou no Chromium headless: `stop()` foi
solicitado, mas o navegador não emitiu o evento `stop` no timeout. Removi essa
parte do smoke e não atribuo a falha ao instrumento; gravação web/native continua
pendente de validação adequada. Os formatos de composição web/JUCE também não
foram unificados.

Arquivos desta etapa: `src/core/CompositionCore.hpp/.cpp` (roteamento comum),
`src/app/Main.cpp` (host usa a regra testável), `tests/mixer_routing.cpp`,
`CMakeLists.txt`, `tools/render_web.cjs` (smokes de mixer e banco), este registro
e `PARIDADE_HTML_JUCE.md`. Artefatos temporários de smoke foram escritos apenas
em `.local-build/parity-audio/`; o banco simulado vive na memória do Chromium.
Não abri janelas nem gerei capturas. **Estimativa de paridade permanece 69%**:
os testes fecham comportamento parcial, mas os portões P3/P4 exigem master e
sensor no host, gravação real, formatos e inspeção auditiva/visual.

Próxima tarefa objetiva: implementar teste/serialização isolados para o banco
nativo e cobrir metadados WAV+partitura sem abrir GUI; depois completar a
comparação visual dos gráficos e Fourier.

### Consolidação do sensor RMS de mixagem — 29/10/2026

Reauditei o host e corrigi uma pendência incorreta na síntese do estado: o JUCE
já mede RMS separado de vozes/baixo, mostra uma sugestão e aplica compensação
estática ao trim; master e trim também já são persistidos. Em vez de marcar
essa capacidade como ausente, extraí `suggestVoiceTrimDeltaDb` para o core que o
botão usa. A função não altera o valor de saída quando falta sinal, calcula a
diferença em dBFS e limita o delta ao intervalo permitido do trim.

O teste `batruc_mixer_routing` cobre silêncio, correspondência de +6,02 dB,
limites superior/inferior, roteamento mute/solo, faders e pan extremo. O app
JUCE recompilou. A suíte inteira passou 13/13; `node --check prototype/app.js`,
`node prototype/test-score.cjs` e `git diff --check` também passaram. Isto
valida a matemática do sensor e os caminhos do core, não o botão em janela,
restauração de trim/master numa cena aberta, nem loudness musical.

Arquivos: `src/core/CompositionCore.hpp/.cpp`, `src/app/Main.cpp`,
`tests/mixer_routing.cpp`, `PARIDADE_HTML_JUCE.md` e este registro. A estimativa
permanece 69%; a auditoria corrige a descrição da lacuna, mas os critérios de
execução, persistência e audição de P3 continuam abertos.

### Paridade numérica das curvas e sensor de balanço — 29/10/2026

Adicionei fixture C++/Node para comparar as coordenadas de 12 curvas entre o
core e a página web. Foram amostrados nove progressos por curva, cobrindo as
sete razões de bilhar (162 pontos ao todo), com fase, ciclos e ordem Fourier
definidos. O erro máximo C++/JS foi `8.881784197001252e-16`. A paridade
geométrica numérica passa; a verificação de desenho/animação em execução ainda
não foi feita.

No mixer, extraí `suggestVoiceTrimDeltaDb`, o cálculo de compensação que o
botão JUCE usa. A regressão cobre silêncio (sem modificar a saída), igualdade
alvo de +6,02 dB, limites do fader, pan, mute e solo. A revisão do código
confirmou que os dois alvos já tinham medição RMS separada de vozes e baixo,
compensação estática e persistência do trim. A lacuna aqui é validar o botão e
a restauração em execução e escutar o equilíbrio; o sensor mede o barramento
vocal agregado, não cada voz individual.

Validação: `batruc_app` recompilado; CTest completo passou 14/14,
`node --check prototype/app.js`, `node --check
tests/geometry_curve_parity.cjs`, o teste de regressão web de score passaram;
uma varredura das alterações por espaços finais não encontrou ocorrências.
Estimativa geral mantida em 69%: cálculos cruzados
estão cobertos, mas os critérios de interface, arquivo, conteúdo e audição
continuam abertos.

Arquivos: `src/core/CompositionCore.hpp/.cpp`, `src/app/Main.cpp`,
`tests/mixer_routing.cpp`, `tests/geometry_curve_fixture.cpp`,
`tests/geometry_curve_parity.cjs`, `CMakeLists.txt`,
`PARIDADE_HTML_JUCE.md` e este registro. Próximo passo: smoke de reabertura do
banco de cenas nativo e teste dos metadados de WAV/partitura; depois fazer a
inspeção visual do caderno de sete abas e dos gráficos em execução.

### Tópico de espaço acústico nos cadernos — 29/10/2026

Ampliei os cadernos HTML e JUCE de seis para sete tópicos, em quatro idiomas.
O tópico novo descreve o modelo de raios e reflexão especular, sua adequação
limitada a frequências cujo comprimento de onda é pequeno perante a sala, a
formação de modos próprios em baixas frequências e o uso de modos/resposta
impulsiva para modelar o espaço. Especifica que o bilhar atual altera eventos
composicionais e não acrescenta reverberação ao sinal.

Arquivos: `src/app/Main.cpp`, `prototype/guide.js`,
`prototype/README.md`, `README.md`, `PARIDADE_HTML_JUCE.md` e este registro.
O app JUCE recompilou; `node --check` passou em `guide.js` e `app.js`, e a
revisão de conteúdo encontrou sete entradas em cada um dos quatro idiomas. Não
abri os painéis; legibilidade, disposição das sete abas, revisão humana das
traduções e inspeção visual continuam pendentes. Não alterei os 69%.

### Fourier: comparação cruzada de epiciclos — 29/10/2026

Estendi a fixture C++/Node para comparar, além dos pontos finais das curvas,
cada vetor intermediário de Fourier. Foram confrontados 222 vetores de
triângulo e quadrado em H=1, 5 e 11 e três progressos/fases. Harmônicos, origem,
extremidade e raio coincidem; o erro máximo total permaneceu
`8.881784197001252e-16`. O teste `batruc_geometry_curve_parity` passou.

Arquivos: `tests/geometry_curve_fixture.cpp`,
`tests/geometry_curve_parity.cjs` e `PARIDADE_HTML_JUCE.md`. Isto verifica a
geometria dos vetores que alimenta os epiciclos, não tamanho, ordem visual,
recorte ou animação na janela. A estimativa continua 69%; resta a verificação
visual em execução e a influência sonora.

### Paridade numérica inicial das relações entre vozes — 29/10/2026

Portada para `prototype/score.js` a regra de suporte, imitação, afastamento e
competição já existente em `EcologicalRelations.cpp`. A API recebe eventos,
regras por voz, duração e número de vozes; devolve uma cópia transformada e um
rastro causal. Mantém o último evento de origem, não reage a ataques
simultâneos, valida limites e ordena o resultado. A função ainda não é chamada
pela composição web/JUCE nem exposta na interface.

Adicionada fixture C++/Node com 10 eventos cobrindo os quatro tipos de relação.
O primeiro A/B revelou que arredondamento float32 mudava a ordem entre eventos
quase simultâneos; o JS agora espelha os pontos de arredondamento do C++. O
alvo `batruc_ecological_relations_fixture` compilou e
`batruc_ecological_relations_parity` passou (10 rastros, 10 eventos, tolerância
2e-6); `node --check` passou nos arquivos JS. Não houve teste de UI ou áudio.

Arquivos: `prototype/score.js`, `tests/ecological_relations_fixture.cpp`,
`tests/ecological_relations_parity.cjs`, `CMakeLists.txt`, este registro e
[`AUDITORIA_CODIGO_SUBUTILIZADO.md`](AUDITORIA_CODIGO_SUBUTILIZADO.md). Próximo:
ligar a configuração por voz e persistência nos dois alvos, aplicar a
transformação antes da geração do baixo e mostrar qual evento causou cada
mudança. A auditoria completa continua válida; este avanço cobre apenas a
equivalência do cálculo, não a paridade funcional geral.

### Relações ecológicas ligadas à composição web — 29/10/2026

O protótipo HTML agora expõe, nas vozes V2–V12, as relações independente,
apoio, imitação, afastamento e competição. A seleção integra a configuração
normalizada da composição e sua persistência local. A voz responde à voz
anterior; o acoplamento λ define a força, a memória é ½ ciclo e o atraso de
imitação ¹⁄₁₆ de ciclo. O plano de eventos é transformado antes de gerar o
baixo, e a intensidade resultante também modula a contribuição herdada pelo
baixo. V1 permanece raiz e independente por construção.

Validação: `node --check` passou em `score.js`, `app.js` e
`interface-i18n.js`; `node prototype/test-score.cjs` passou com uma regressão
que confirma que cada uma das quatro relações altera também a agenda/intensidade
do baixo no modo guiado pelas vozes. `ctest -R
^batruc_ecological_relations_parity$` passou (1/1; 10 eventos e 10 rastros,
tolerância 2e-6). Não houve build completo, teste visual no navegador ou
audição; estes resultados não validam o timbre nem a implementação JUCE.

Atualizei `AUDITORIA_CODIGO_SUBUTILIZADO.md` e `README.md`. A nota global de
paridade permanece 69%: esta fatia só cobre HTML e ainda carece de rastro causal
visível, validação auditiva e ligação segura no JUCE. Próxima etapa: integrar no
renderer JUCE atualizando agendas na fronteira de compasso sem mutar dados lidos
pelo callback; depois comparar a mesma composição nos dois motores.

### Auditoria de código isolado e subaproveitado — 29/10/2026

Foi feita uma leitura estática do core, JUCE, protótipo web, CMake e testes.
Curvas/Fourier e o caminho de eventos da voz 1 estão conectados ao motor; as
métricas de área/perímetro são apenas informativas no painel JUCE. Já
`EcologicalRelations`, `TriangleGeometry` e as transformações de
`GenerativeOperations` compilam, mas não têm chamadas de produção encontradas:
há uma máquina de relações entre vozes e várias transformações generativas
codadas, porém ainda fora do fluxo composicional. O bilhar segue como operador
de eventos; não é reverb nem modelo acústico de sala.

O inventário, critérios e proposta de integração gradual foram registrados em
[`AUDITORIA_CODIGO_SUBUTILIZADO.md`](AUDITORIA_CODIGO_SUBUTILIZADO.md). Não
houve edição funcional, build, teste ou audição nesta auditoria. Próxima tarefa
de código: especificar uma fixture de relações ecológicas reproduzível e
integrar o operador ao fluxo composicional, incluindo rastro causal; planejar
desde o início a fixture equivalente para HTML/JUCE. Preservar as funções
existentes enquanto não forem substituídas e validadas.

**Atualização do estado:** a auditoria inicial foi seguida pela integração web
registrada acima. A afirmação de que a ecologia ainda não tinha chamada de
produção descreve o estado no momento daquela leitura e foi superada no HTML;
continua verdadeira para o JUCE.
### Relações ecológicas JUCE com aplicação no próximo compasso — 29/10/2026

O seletor JUCE de relação do baixo agora inclui apoio ecológico, imitação,
afastamento e competição. No modo de baixo seedado, os eventos das vozes são
transformados antes de derivar a linha, fora do callback de áudio; os
acoplamentos λ servem como forças por voz. A performance preparada é publicada
pelo handoff já existente e só substitui a ativa no próximo compasso. A cena
avança para schema 25, preservando a leitura dos valores antigos.

A mudança não cobre baixo manual nem estabelece paridade completa: no JUCE a
relação é global para a cadeia de vozes, enquanto o HTML permite relação por
voz. Também faltam rastro causal visível e comparação auditiva HTML/JUCE.

Validação: `batruc_app` compilou após a atualização de idioma; passaram os
testes focados `batruc_ecological_bass_composition`,
`batruc_bass_audio_response`, `batruc_seeded_bass_parity`,
`batruc_seeded_voice_bass_parity`, `batruc_cyclic_bass_parity` e
`batruc_ecological_relations_parity`. O teste de resposta confirma que o novo
baixo não é aceito antes da fronteira e passa a valer no compasso seguinte.
Sem audição comparativa nem inspeção visual nesta etapa.

Arquivos: `src/core/BassComposition.hpp/.cpp`, `src/app/Main.cpp`,
`tests/ecological_bass_composition.cpp`, `tests/bass_audio_response.cpp`,
`tests/seeded_bass_fixture.cpp`, `CMakeLists.txt`, `README.md`,
`PARIDADE_HTML_JUCE.md` e `AUDITORIA_CODIGO_SUBUTILIZADO.md`. Próximo passo:
persistir relação/força por voz nas cenas JUCE, expor relação por voz e
realizar comparação auditiva com a mesma composição no HTML.

### Revisão geral de layout e coerência — 29/10/2026

Auditei a maquete web em Chrome headless a 1440, 1024, 900, 768, 721, 720,
470 e 390 px. Corrigi os breakpoints dos sete controles de LFO, dos controles
geométricos e do arquivo local, limitei os seletores à coluna e removi o
overflow intrínseco dos seletores geométricos em mobile. A medição também
detectou e ajudou a corrigir a invasão do seletor de forma sobre o checkbox de
fonte geométrica no layout desktop. Repetida a medição: nenhum dos módulos verificados
(`instrument`, arquivo, geometria, LFO, ADSR/VCF e mixer) excede sua área nos
oito tamanhos.

Na interface JUCE, revisão estática dos limites encontrou sobreposição entre o
slider de foco do gráfico e a faixa formal dos ciclos no rodapé. Desloquei a
faixa para baixo, deixando 1 px de separação do slider e encostando-a no início
da barra de transporte; a janela já impede reduzir abaixo de 1080×800, requisito
dos controles de posição fixa.

Coerência com a intenção: o conjunto já apresenta cálculo, gráfico animado,
composição por vozes e relações, forma, tempo, baixo editável, gravação e
armazenamento local. Permanecem limites importantes, não bugs de layout: área
e perímetro de algumas formas são informativos e não geram som; a geometria só
alimenta V1 quando a fonte está ligada; o desenho do bilhar não é reverberação
nem modelo de sala; HTML e JUCE ainda diferem em funções e síntese. A inspeção
do JUCE foi estática, sem janela gráfica disponível neste ambiente; portanto
não declaro ausência completa de defeitos visuais em execução.

Validação após as correções: medição DOM/CSS do navegador passou nos oito
viewports para os módulos avaliados, sem overflow nem interseção entre controles
clicáveis; `node --check` de `app.js` e `score.js`, mais
`node prototype/test-score.cjs`, passaram. O alvo JUCE recompilou com sucesso.
Não foi possível inspecionar a janela nativa visualmente porque este ambiente
não tem display acessível; a correção do rodapé foi revisada pelas coordenadas.

### Logo escolhida — 29/10/2026

O autor escolheu a primeira de três propostas gráficas geradas para o Batruc.
Preservei a imagem original fora do projeto e copiei uma referência de acesso
para `prototype/assets/brand/references/batruc-logo-selected-01.png` (PNG RGB, 1254 × 1254 px,
SHA-256 `b9da42d395092f09122aabb942f56ec07baaf81e5d511ed50230253a0b7c2269`).
O lettering faz parte do raster, não é uma fonte. Registrei a escolha e os
limites no README. As propostas 2 e 3 não foram descartadas; favicon e vetor
seguem pendentes.

### Integração da logo no cabeçalho web — 30/10/2026

A imagem escolhida foi integrada ao cabeçalho da maquete web em
`prototype/index.html` e `prototype/style.css`; substitui o título visual
duplicado e mantém um H1 acessível. O app JUCE nativo, favicon e variantes
vetoriais continuam pendentes. Conferi a renderização e o encaixe no Chrome
headless nos viewports 1440, 1024, 768, 721, 720, 390 e 320 px: o arquivo
carrega, não colide com a edição e não cria overflow horizontal. O ensaio de
320 px revelou overflow antigo nos controles de seed e master; ajustei esses
controles para caberem. Inspecionei uma captura do cabeçalho em desktop. Ainda
faltam revisão no navegador interativo e inspeção gráfica do app JUCE.

### Símbolo à esquerda do lettering — 30/10/2026

Atendendo ao pedido do autor, a marca web ganhou uma composição horizontal.
A primeira proposta (símbolo de berimbau mais figurativo) foi preservada como
`batruc-symbol-berimbau-math-v01.png` e
`batruc-logo-horizontal-v01.png`. A segunda abstrai o instrumento em um arco,
uma corda tensa e um nó; trajetórias curvas cruzadas sugerem funções,
integração, movimento e eventos sonoros. Usa preto, coral queimado e teal, sem
degradê. O símbolo v2 é vetor em `batruc-symbol-curves-math-v02.svg`; o
lettering não foi redesenhado e continua sendo o recorte raster da arte
aprovada (`batruc-wordmark-original-crop-v01.png`). O lockup transparente é
`batruc-logo-horizontal-v02.png` (950 × 242 px) e esta versão está ativa no
cabeçalho web. A logo quadrada original e a versão v1 permanecem intactas.
Descrição e título acessível foram preservados. Validação visual por
renderização headless em 1440 × 900 e 390 × 844; próximos passos: avaliar
derivação de favicon e aplicação no app JUCE.

### Símbolo baseado na referência do autor — 30/10/2026

O autor delimitou a nova direção usando `prototype/assets/brand/references/ideia.png`: somente
a onda, o traço horizontal e hastes transversais com círculos nas pontas. A
proposta v3 segue esse recorte, sem os vetores diagonais nem a trajetória
pontilhada da referência. O lettering aprovado permanece inalterado. Arquivos
novos: `batruc-symbol-wave-stems-v03-source.png` (geração integral),
`batruc-symbol-wave-stems-v03.png` (recorte para uso) e
`batruc-logo-horizontal-v03.png` (950 × 180 px, lockup transparente). A
imagem de referência, o símbolo original e as propostas v1/v2 foram mantidos.
O cabeçalho web aponta para o símbolo v3. Renderização headless em 1440 × 900
confirmou a posição à esquerda do lettering e o encaixe no cabeçalho. A escolha
estética ainda depende da avaliação do autor; não considerar esta direção
aprovada até lá.
### Refinamento do símbolo — V4 — 30/10/2026

Para aproximar a identidade visual da proposta do instrumento, o gráfico agora
mostra uma relação matemática legível: o eixo horizontal representa a variável
temporal, a onda representa a função, e quatro hastes transversais medem seus
valores em picos e vales. Os círculos ficam exatamente sobre a curva. Hierarquia
reduzida a três cores da interface, traços uniformes e geometria vetorial; sem
novos motivos decorativos. Lettering segue inalterado. O símbolo v4 está em
`prototype/assets/brand/studies/batruc-symbol-wave-sampling-v04.svg`, com lockup exportado em
`prototype/assets/brand/archive/batruc-logo-horizontal-v04.png` (950 × 180 px). v1–v3 e a
imagem original permanecem preservadas. Renderizei a maquete em Chrome
headless para verificar o SVG no cabeçalho; a geometria não foi distorcida e o
lettering permanece à direita sem alteração. A proposta segue aguardando a
avaliação do autor.

### Símbolo gerado por equação — V5 — 30/10/2026

O autor perguntou se a ideia poderia nascer de uma equação. A onda do símbolo
agora vem de uma soma finita de Fourier,
`f(t)=0,66 sen(2πt)+0,24 sen(4πt+π/3)+0,10 sen(6πt−π/5)`. O gerador calcula
a curva e amostra cinco instantes `tₖ=(k+0,5)/5`; cada haste parte do eixo
horizontal e termina em um círculo colocado no valor de `f(tₖ)`. O lettering
segue o recorte original. O script reproduzível é
`tools/logo/generate_equation_symbol.py`; os derivados são
`prototype/assets/brand/studies/batruc-symbol-fourier-sampling-v05.svg` e
`prototype/assets/brand/archive/batruc-logo-horizontal-v05.png`. V1–V4 e a arte original
continuam intactas. A V5 ficou preservada como estudo matemático visual, não
como partitura nem representação sonora literal; versões posteriores passaram
a ocupar o cabeçalho.

### Hastes atravessam a onda — V6 — 30/10/2026

Por indicação do autor, as hastes passaram a atravessar a curva e continuar
além dela; os círculos foram para as pontas e ficaram sem contorno branco.
V5 continua preservada. Esta variação está em
`prototype/assets/brand/studies/batruc-symbol-fourier-sampling-v06.svg`.

### Espessura variável e acabamento — V7/V8 — 30/10/2026

O autor pediu variação de espessura como na primeira proposta. O gerador passou
a calcular a curvatura da função no plano e usa essa medida para variar a
largura da onda, afinando suas extremidades. Na V8, a escala do símbolo e a
hierarquia entre onda, eixo, hastes e nós foram refinadas; os círculos são
sólidos, sem contorno branco. Lettering inalterado. V6 e V7 permanecem como
etapas anteriores. O arquivo ativo é
`prototype/assets/brand/studies/batruc-symbol-fourier-sampling-v08.svg`, composto com o
lettering em `prototype/assets/brand/archive/batruc-logo-horizontal-v08.png`. A renderização
headless em tela larga confirmou o encaixe no cabeçalho. V8 é proposta para
avaliação, sem aprovação presumida.

### Forma de onda baseada na referência — V9 — 30/10/2026

O autor reenviou `prototype/assets/brand/references/ideia.png` delimitando a forma desejada:
duas oscilações, eixo horizontal e hastes com círculos nas extremidades. V9
mantém só esses componentes. A equação dá predominância ao segundo harmônico:
`f(t)=0,16 sen(2πt−π/4)+0,52 sen(4πt+π/12)+0,11 sen(6πt−π/6)`, produzindo
duas oscilações assimétricas. Oito hastes uniformemente espaçadas atravessam a
curva e prolongam-se além dela; os nós sólidos não têm contorno. A espessura
do traço varia com a curvatura calculada e afina nas extremidades. O script
`tools/logo/generate_equation_symbol.py` mantém presets V8 e V9. Arte em
`prototype/assets/brand/studies/batruc-symbol-fourier-sampling-v09.svg`, lockup em
`prototype/assets/brand/archive/batruc-logo-horizontal-v09.png`. V1–V8, a referência e o
lettering original continuam preservados. V9 foi substituída no cabeçalho pela
composição vertical V10; continua preservada como alternativa horizontal.

### Composição vertical — V10 — 30/10/2026

O autor pediu o símbolo sobre o lettering, ambos na mesma largura. A V10
mantém a série de Fourier e as hastes da V9, recorta o viewBox ao eixo gráfico
para que a onda ocupe toda a largura do lettering e empilha os dois elementos.
O lettering continua vindo do raster aprovado, sem edição. O cabeçalho web
usa `prototype/assets/brand/studies/batruc-symbol-fourier-sampling-v10.svg` acima do
lettering; o lockup transparente para exportação é
`prototype/assets/brand/archive/batruc-logo-vertical-v10.png` (640 × 590 px). V9 horizontal,
V1–V9, arte original e referência do autor permanecem preservadas. A V10 foi
renderizada e revelou espaço vertical excessivo; a V11 recorta esse vazio.

### Cores da referência e proporção vertical — V11 — 30/10/2026

O autor pediu para manter sem desvio as cores da imagem de referência. Usei os
valores amostrados dela: marinho `#0C1525`, coral `#E44533` e teal `#0D8192`.
Também recortei o viewBox ao desenho, sem espaço vazio, e empilhei o símbolo
acima do lettering com ambos na mesma largura. O símbolo SVG está em
`prototype/assets/brand/studies/batruc-symbol-fourier-sampling-v11.svg`; o lockup transparente
em `prototype/assets/brand/archive/batruc-logo-vertical-v11.png` (640 × 330 px). O cabeçalho
web foi atualizado. A renderização headless em 1440 × 900 confirmou o símbolo
acima do lettering, larguras iguais e cores da referência. V9 horizontal e
V1–V10 permanecem preservadas; o lettering e a referência não foram alterados.

### Traçado da referência enviado pelo autor — V12 — 30/10/2026

O autor pediu que a composição não mudasse as cores, o foco nem a disposição
das linhas e círculos da referência. A V12 traça em vetor o eixo horizontal,
a onda de espessura variável e suas 17 hastes com círculos terminais, mantendo
as posições relativas e a paleta marinho `#0C1525`, coral `#E44533` e teal
`#0D8192`. O símbolo fica sobre o lettering original, ambos na mesma largura.
O cabeçalho aponta para `prototype/assets/brand/studies/batruc-symbol-reference-trace-v12.svg`
e a exportação composta é `prototype/assets/brand/archive/batruc-logo-vertical-v12.png`.
O lockup raster tem 640 × 376 px.
V1–V11, o lettering e a referência `prototype/assets/brand/references/ideia.png` permanecem
intocados. Trata-se de traçado vetorial baseado na referência visual; validar
com o autor a leitura final da composição.

### Símbolo reduzido a pulso e geometria — V13 — 30/10/2026

Após a avaliação negativa da V12, foi criada uma proposta nova, sem sobrescrever
as artes anteriores. A curva assimétrica cruza o próprio eixo e quatro impulsos
circulares marcam dois ciclos, com variação de fase e amplitude. A paleta usa
marinho `#0C1525`, coral `#E44533` e teal `#0D8192`; o lettering original foi
mantido. O cabeçalho usa `prototype/assets/brand/studies/batruc-symbol-pulse-geometry-v13.svg`
e o lockup exportado é `prototype/assets/brand/archive/batruc-logo-vertical-v13.png`.

### Curva de Fourier isolada acima do lettering — V14 — 30/10/2026

O autor pediu uma composição reduzida somente à curva de Fourier acima do
lettering: sem linhas horizontais ou verticais, hastes ou círculos. A V14 usa
`f(t)=0,72 sen(2t)+0,22 sen(3t+0,55)`, em marinho `#0C1525`. O cabeçalho usa
`prototype/assets/brand/studies/batruc-symbol-fourier-curve-v14.svg`, gerado por
`tools/logo/generate_fourier_curve_v14.py`; o lockup transparente mede
640 × 278 px e está em `prototype/assets/brand/archive/batruc-logo-vertical-v14.png`.
V1–V13, o lettering e a referência permanecem preservados. Após detectar que
a primeira exportação do lockup tinha branco opaco nos cantos, foi refeita a
conversão com canal alfa; transparência conferida nos pixels de borda.

### Cabeçalho do instrumento somente com lettering — 30/10/2026

Por decisão do autor, o cabeçalho da página do instrumento exibe somente o
lettering aprovado em `prototype/assets/brand/active/batruc-wordmark-original-crop-v01.png`.
O símbolo foi removido da interface, sem apagar ou substituir os arquivos V1–V14;
eles permanecem como estudos gráficos arquivados.

### Organização do acervo de logos — 30/10/2026

O conteúdo antes misturado diretamente em `prototype/assets/` foi inventariado
e distribuído sob `prototype/assets/brand/`: lettering ativo, referências,
lockups históricos, estudos vetoriais e propostas. Os nomes dos arquivos foram
preservados. As páginas de propostas, o cabeçalho e os geradores foram ajustados
para os novos caminhos. A preservação pré-mudança está em
`preservation/batruc-logo-before-reorganization-20261030.tar.gz`, com checksum
adjacente. O catálogo está em `prototype/assets/brand/README.md`.

### Ícones web para publicação — 30/10/2026

Foram criados favicon SVG/ICO/PNG (16, 32 e 48 px), Apple touch icon (180 px)
e ícones PWA (192/512 px, incluindo maskable), todos derivados do B do
lettering aprovado; as contraformas internas foram verificadas no vetor. O
favicon mantém fundo transparente; os ícones de instalação usam o fundo papel
`#FDF9F2`. O cabeçalho inclui os links de ícone e o manifesto, com início e
escopo apontando para a raiz do protótipo. A origem e a regeneração estão
documentadas em `prototype/assets/brand/README.md` e
`tools/logo/generate_brand_icons.py`.

### Captura curta do REC web — 30/10/2026

Ampliei `tools/render_web.cjs` com `--recording-smoke`: o botão REC é acionado
por clique DevTools, a captura real de `MediaRecorder` dura quatro segundos,
o WebM/Opus é decodificado no Chromium e o smoke mede duração, pico e RMS. O
Chromium salva o áudio e, após clicar no link disponibilizado pela interface,
o TXT em uma pasta local isolada; o teste confere o tamanho idêntico entre
blob e WebM baixado, o schema JSON e a referência ao nome do áudio. Usei a seed
`5EED2030`, cujo primeiro evento acontece em `t=0`. Resultado: WebM/Opus de
44.216 bytes, 3,78 s decodificados, pico 0,329 e RMS 0,0195; TXT JSON de
77.318 bytes; todas as verificações passaram. A tentativa anterior de 1,2 s
com seed `5EED2027` produziu silêncio porque o primeiro evento daquela agenda
só começa em 18,142 s — não era defeito de captura.

O smoke de arquivo também passou: guardar, listar, alterar seed, reabrir e
apagar usando File System Access simulado. `node --check tools/render_web.cjs`
passou. A chamada inicial sem o argumento obrigatório de diretório executou,
em vez do smoke, um render offline de 56 s da seed `5EED2027`; seus arquivos
locais permanecem em `.local-build/parity-audio/` como evidência e não foram
apagados. O render mostrou o primeiro evento em 18,141755 s.

Os artefatos conferidos estão preservados localmente em
`.local-build/parity-audio/recording-smoke-20261030b/`. Limite: os arquivos
foram baixados em pasta local de teste; a
permissão de pasta escolhida pelo usuário, a reabertura dos arquivos, a captura
nativa e a audição humana não foram exercitadas. A paridade permanece em 69%.
Preflight às 02:38 CEST: swap 0/2 GiB, 10,9 GiB disponíveis, 6,7 GiB livres
em `/`; tarefas executadas em processo único e com timeout de 90 s.

Após esse registro, acrescentei `--recording-folder-smoke` ao mesmo renderer.
O teste usa File System Access simulado e passa pelo seletor de pasta, grava
4 s, fecha o WebM e o TXT no destino, confirma que os nomes estão relacionados
e decodifica sinal do WebM. Passou com os dois arquivos na pasta virtual;
duração decodificada 3,78 s, pico 0,329, RMS 0,0195. `node --check` também
passou. Isso cobre o ramo de escrita da aplicação, mas não a permissão nem a
escrita numa pasta física escolhida pelo usuário. Não alterei a estimativa de
paridade.

**Próximo passo:** verificar gravação e sidecar numa pasta real sem tocar em
arquivos preexistentes; depois exercitar captura/reabertura nativas e definir
o contrato de composição que permita importar a sessão web sem descartar os
campos de duração e pêndulo. A inspeção confirmou que o importador JUCE também
rejeita o sidecar HTML: ele remove o envelope de `batruc-recording-score` e
exige que `initial_configuration` tenha o schema nativo, mas a web grava ali
seu objeto camelCase sem `format`. Registrar isso como incompatibilidade
explícita, não como validação da abertura.

### Diagnóstico de formato ao abrir no JUCE — 30/10/2026

O importador agora distingue `batruc-composition` HTML e
`batruc-recording-score` cujo `initial_configuration` contém o objeto web
camelCase, e explica que a importação entre motores ainda está pendente. Não
converte nem descarta dados. O target `batruc_app` recompilou; o binário
`.local-build/native/linux-debug-juce/batruc_app_artefacts/Debug/Batruc` foi
gerado às 02:49 CEST (111.593.952 bytes; SHA-256
`c107330cb0c174dcab8921c757bf2e644e43b31f859eddaadbff5ad5baf96842`) e contém
a nova mensagem. `ctest --test-dir .local-build/native/linux-debug-juce
--output-on-failure -j1` passou 16/16 em 61,12 s. A mensagem foi confirmada
no executável; não abri a janela para validação visual. Esta melhoria torna a
lacuna explícita, mas não reduz a incompatibilidade nem muda a estimativa de
paridade (69%).

Preflight antes do build: swap 0/2 GiB, 11,5 GiB disponíveis, 6,9 GiB livres
em `/`, nenhum processo pesado concorrente. O build foi serial (`-j1`), com
timeout de 180 s; a suíte existente também foi serial, com timeout de 120 s.

### P1 da sequência ponderada — cursor de eventos em revisão ao vivo — 30/10/2026

Retomei o fechamento de seed/fase/receita. A página já recompunha a agenda
durante o transporte e saltava para `elapsed + 130 ms`, deixando intactos os
eventos dentro da janela de agendamento Web Audio (120 ms). Extraí a busca do
cursor para `eventCursorAtOrAfter()` no modelo de score, com busca binária, e
liguei o scheduler existente ao helper. Assim, a atualização não volta ao
começo da agenda nem dispara ataques antigos; o novo plano começa no primeiro
evento elegível após a janela já comprometida.

Acrescentei casos de fronteira (igualdade no corte, imediatamente depois do
corte, agenda esgotada/vazia e valor inválido) em
`prototype/test-score.cjs`. `node --check prototype/app.js`,
`node --check prototype/score.js` e `node prototype/test-score.cjs` passaram.
Preflight: 10 GiB disponíveis, swap 0/2 GiB, 7,5 GiB livres em `/`, sem
processo pesado concorrente. A validação foi unitária; ainda falta mudar
seed/fase durante uma reprodução real nos dois apps e confirmar por escuta que
a troca preserva o fluxo musical. P1 permanece aberto e não altera os 69%.

Arquivos alterados nesta fatia: `prototype/app.js`, `prototype/score.js`,
`prototype/test-score.cjs`, `PARIDADE_HTML_JUCE.md` e `TAREFAS.md`. Próximo na
ordem: avançar P2 com comparação de ritmo/polirritmia/forma/baixo usando a
mesma composição e verificar transições durante execução; manter a nota
ponderada sem aumento até os portões de uso real passarem.

**Validação adicional P2 —** reexecutei serialmente as fixtures
`batruc_seeded_bass_parity`, `batruc_seeded_voice_bass_parity`,
`batruc_cyclic_bass_parity`, `batruc_seeded_recipe`,
`batruc_voice_inversion` e `batruc_generative_operations`; 6/6 passaram. As
duas fixtures cruzadas extensas finalizaram em 36,31 s e 0,13 s. Elas cobrem
agendas/alturas/durações e operações determinísticas, mas não exercitam os
controles ao vivo nem substituem escuta A/B. A matriz continua em 69%; o
próximo avanço exige observar a mesma alteração de seed/fase/ritmo em execução
e comparar o resultado sonoro nos dois alvos.

**Checagem P3 do mixer —** `batruc_mixer_routing` passou em nova execução
seletiva (0,01 s). Isso revalida o roteamento de mute/solo, nível e panorama
no teste do host/core; não valida a interação pela janela JUCE nem salvamento
dos ajustes. A medição funcional web já registrada permanece como evidência
separada. P3 segue sem crédito adicional até uso do mixer nativo e conferência
auditiva.

### P4 e P5 — conteúdo de geometria e fixtures visuais — 30/10/2026

Ampliei o tópico Learn “Superfície e geometria” do JUCE para explicar a
geometria como percurso visual, o vínculo opcional de `y(t)` com ataques da
V1, a aproximação de Fourier controlada por H e o papel apenas informativo de
área/perímetro. Também explicita que o bilhar compõe eventos, mas não é um
efeito de reverberação. O texto entrou em PT/EN/ES/FR, seguindo o helper UTF-8
do app, e o título do tópico foi alinhado à página web.

Validação P4: o alvo `batruc_app` compilou em série. Executável gerado em
30/10/2026 às 18:55 CEST, 111.600.600 bytes, SHA-256
`09d3f180d6bb7201f8728cc4be3e48bb4b6a68ed547dd874818cf8da2f13ac1b`; a nova
mensagem está embutida. Não abri a janela, portanto tradução e layout ainda
precisam de inspeção visual. A incompatibilidade de importação entre arquivos
HTML/JUCE continua explícita e sem perda parcial.

Validação P5: `batruc_triangle_geometry`, `batruc_geometry_curves` e
`batruc_geometry_curve_parity` passaram (3/3, 0,24 s). Isso revalida o core e
as coordenadas comparadas, não os gráficos renderizados nem o som. Preflight
antes do build: 10 GiB disponíveis, swap 0/2 GiB, 7,7 GiB livres em `/`, sem
processos pesados concorrentes; build serial com timeout de 180 s. As frentes
P1–P5 seguem sem aumento de pontos ponderados até validação funcional e
auditiva nas interfaces. Estimativa permanece 69%.

### P1 — troca de seed durante reprodução no Chromium — 30/10/2026

Acrescentei `--live-seed-smoke` a `tools/render_web.cjs` e um traço de eventos
de diagnóstico limitado a 4.096 registros, ativado somente pelo próprio teste.
O smoke inicia o transporte com clique DevTools, troca a seed usando o evento
real do campo, observa o cursor e os eventos agendados e então interrompe a
reprodução. O caso `5EED2030 → A1B2C3D4` passou: cursor em 0,252 s; evento da
nova revisão presente; nenhum ataque novo antes do corte; sem duplicatas; o
transporte seguiu ativo. A tentativa dentro do sandbox encerrou o canal do
Chromium (`ECONNRESET`); a repetição autorizada fora do sandbox passou. O teste
não depende de captura de tela e não deixou arquivos de áudio.

Validação: `node --check tools/render_web.cjs`, `node --check
prototype/app.js` e `node prototype/test-score.cjs` passaram. Pós-teste:
11 GiB disponíveis, swap 0/2 GiB, 7,7 GiB livres em `/`, sem Chromium ou
processo pesado ativo. P1 permanece parcial: falta repetir a troca em execução
na interface JUCE, mudar fase em ambos e escutar A/B. A nota global continua
69%; este smoke comprova só o scheduler web.

### P2 ao vivo — fase, polirritmia e recomposição do baixo — 30/10/2026

Ampliei o smoke para uma sequência durante a mesma reprodução: seed
`5EED2030 → A1B2C3D4`, fase `0,25` e preset `5:4`. O Chromium confirmou três
revisões do scheduler; os cortes foram aproximadamente `0,743 s` e `2,033 s`
para fase e polirritmia. Em todas, os eventos da nova versão ficaram fora do
horizonte já comprometido, sem duplicatas, e o transporte permaneceu ativo.
O preset preservou os 12 canais existentes, definiu `voiceCycles=[5,4]` nas
duas primeiras vozes e zerou a variação manual, conforme a regra já codificada.
O plano de baixo gerado mudou ao recalcular a polirritmia (`bass_plan_changed:
true`), confirmando que essa mudança afeta a composição derivada e não só o
desenho.

Para observar essa relação, o smoke envolve `BatrucScore.generate` apenas na
página de teste e guarda uma amostra limitada de 64 snapshots de configuração;
o traço do scheduler também permanece limitado a 4.096 eventos. Ambos são
diagnósticos condicionais de teste, não mudam o formato salvo nem criam áudio.
Validação: `node --check tools/render_web.cjs`, `node --check
prototype/app.js`, `node prototype/test-score.cjs` e smoke headless passaram.
Isto valida a execução web e a recomposição matemática do baixo; não confirma
o stem audível nem o comportamento JUCE ao vivo. Próximo: reproduzir a mesma
sequência na interface JUCE e comparar eventos/áudio antes de atribuir pontos.
Estimativa permanece 69%.

**Regressão final desta etapa —** as oito fixtures selecionadas de seed/baixo,
receita, inversões, operações generativas, mixer e geometria passaram (8/8,
63,99 s). `git diff --check` e as verificações sintáticas JavaScript também
passaram. Pós-teste: cerca de 10 GiB disponíveis, swap 0/2 GiB, 7,7 GiB livres
em `/` e nenhum processo pesado ativo. A próxima validação continua sendo a
mesma troca ao vivo no JUCE e a escuta comparativa; a paridade fica em 69%.

### Preset de polirritmia JUCE preserva seed e recompõe o baixo — 30/10/2026

Na revisão do caminho nativo, encontrei uma divergência concreta: aplicar um
preset chamava os callbacks dos sliders de ciclos, que interpretavam a
mudança programática como edição manual, apagavam a seed e desmontavam o vínculo
com o baixo seedado. A web aplica a mesma relação sem perder a seed. Corrigi o
JUCE para atualizar controles/overrides sem notificação em cascata, manter a
seed, zerar a variação global como na web e solicitar uma única recomposição do
baixo. O número de canais continua preservado/expandido conforme a relação.

Arquivos: `src/app/Main.cpp`, `PARIDADE_HTML_JUCE.md` e esta continuidade.
Validação: `batruc_app` compilou em série e `batruc_seeded_recipe` passou.
Depois, incluí uma fixture unitária para verificar que o preset altera apenas
os ciclos cobertos, conserva canais restantes e rejeita índices/contagens
inválidos. O fluxo ainda precisa de validação ao vivo na janela JUCE; sem essa
etapa, a paridade total continua em 69%.

### Marca e estado audível do baixo no JUCE — 30/10/2026

O autor apontou que a logo não aparecia na aplicação. A inspeção confirmou que
o web exibia o lettering aprovado, mas o JUCE ainda desenhava `batruc` com uma
fonte do sistema: o alvo nativo não empacotava imagens de marca. Adicionei o
PNG aprovado ao `BinaryData` do JUCE e o cabeçalho agora o desenha sem depender
de arquivo externo. Os ícones do executável/desktop continuam pendentes.

Na checagem do relato “não estou escutando o baixo”, o fluxo ALSA/PipeWire do
Batruc estava ativo e sem mute no mixer do sistema. Encontrei que o módulo de
baixo nativo iniciava desligado, embora a web sempre inclua o baixo. Ajustei o
estado padrão do JUCE para ligado; cenas que salvam explicitamente o módulo
desligado continuam respeitando essa escolha.

O primeiro build foi encerrado pelo timeout aos 240 s durante a recompilação
dos módulos JUCE, sem atualizar o binário. O build retomado concluiu, e o
incremental após habilitar o baixo também passou. Executável final atualizado
às 19:41, aproximadamente 111,9 MB. `batruc_seeded_recipe` passou com a nova
fixture de aplicação/preservação de canais. O binário anterior (19:20) e a
janela já aberta pelo autor continuam sendo instâncias antigas; fechar e abrir
novamente carrega a marca empacotada, o preset corrigido e o baixo ativo por
padrão.

### Desenvolvimento de altura do baixo entre passagens — 01/11/2026

Acrescentei ao renderizador JUCE uma transformação pequena de registro ligada à
memória determinística da forma. A primeira passagem de sete ciclos mantém as
alturas geradas pela seed; nas seguintes, a direção da memória desloca o baixo
em ±1 semitom, ou ±2 quando a memória supera o limiar definido, com o sinal
invertido quando a própria passagem é invertida. A operação mantém cada
intervalo e não quantiza a linha a uma escala. O deslocamento é aplicado no
momento de disparo, permanece limitado à faixa MIDI do baixo (24–60) e não
altera a célula editada nem a receita salva.

Arquivos: `src/core/PassageDevelopment.hpp`, `src/core/BassAudio.cpp`,
`README.md` e este registro. A regra ainda não está espelhada na maquete web;
esta é uma exploração composicional específica da execução JUCE, não uma
afirmação de paridade. Também faltam comparação auditiva e inspeção visual na
janela real.

Na continuação desta etapa, as vozes ressonantes também receberam deriva de
registro por passagem. Cada canal usa uma variação determinística do seed e
move todos os modos do seu ressonador pela mesma razão, preservando as relações
espectrais. A razão alvo muda no evento e converge suavemente no renderizador,
sem reinicializar fase ou envelope. As regras não alteram o relógio nem o
material escrito. Arquivos adicionais: `src/core/CompositionCore.hpp` e
`src/core/CompositionCore.cpp`. O alvo `batruc_app` compilou em série com
`timeout 300 cmake --build .local-build/native/linux-debug-juce --target
batruc_app -j1`. O binário resultante tem SHA-256
`48eefec74b7283ad2c0478c41398c0911d08f6a42efb0ce0eacc04c6e64bb9ed`.
`git diff --check` também passou. Não executei testes nem fiz renderização ou
escuta; a avaliação auditiva da deriva e a inspeção visual no app continuam
pendentes.

### Duas matérias modais novas para as vozes — 01/11/2026

Ampliei o sintetizador modal do JUCE com as escolhas Sino (parciais inarmônicos)
e Membrana (conjunto modal próprio), além das quatro formas já existentes.
Cada canal continua podendo herdar a matéria global ou escolher a sua. As
escolhas passam a integrar seeds novas e cenas salvas no schema 28; cenas
anteriores seguem aceitas com valores 0–3. O seletor explica que são modelos
virtuais estilizados, não medições de objetos reais.

Os modos acima de 45% da taxa de amostragem agora deixam de contribuir para a
saída, evitando aliasing nas relações inarmônicas mais altas. O acumulador de
fase continua limitado e o callback não aloca. Os perfis e ganhos ainda
precisam de avaliação auditiva, e a escrita/leitura de cenas 1–28 necessita da
validação já prevista para o produto.

Arquivos: `src/core/CompositionCore.hpp`, `src/core/CompositionCore.cpp`,
`src/app/Main.cpp` e `README.md`. Essa capacidade nativa não existe ainda na
maquete web e não aumenta a estimativa de paridade.

### Estado visual dos stems no mixer — 01/11/2026

As colunas do mixer agora mostram `Vn ATIVO/ON/ACTIF` para stems habilitados,
`Vn MUTE` quando habilitados mas silenciados, e `Vn INATIVO/OFF/INACTIF`
para os canais acima da contagem escolhida. Os canais inativos têm controles e
identificação atenuados; os habilitados mantêm a cor própria do stem. Mute não
é confundido com desativação. A atualização acompanha a contagem de vozes, os
botões Mute e a troca de idioma. Alteração limitada ao JUCE. O alvo
`VARIAÇÃO` agora ocupa uma linha própria acima do slider de cada stem, em vez
de dividir a mesma linha estreita com o controle. Os elementos inferiores foram
reposicionados em sequência, sem ultrapassar a altura do painel. O alvo
`batruc_app` compilou em modo serial em 01/11/2026 às 13:59; o executável
Debug tem SHA-256
`593af5c6022d83421fa4b578b92dc783eed472b49bf90ed7774097a897fe2a5c`.
Não abri nem inspecionei a janela; é necessário reiniciar o app para ver este
binário e confirmar visualmente o espaçamento e o contraste.

### Geometria rítmica e invariantes de Berry/Chern — 01 nov. 2026

Registrei a distinção matemática entre conexão de Berry, curvatura e número de
Chern, incluindo superfície fechada de parâmetros, banda isolada/gap e método
discreto de Fukui–Hatsugai–Suzuki como validação futura. Acrescentei a
geometria rítmica ao Study Mode: clave, rumba clave, bossa nova e tresillo com
contexto/variantes, divisões e subdivisões, polígonos inscritos no círculo,
camadas `N:M`, coincidências, playhead e camadas isoláveis. A documentação
esclarece que essa geometria rítmica não implica invariante de Chern e que a
bossa não deve ser reduzida a uma célula única. Consultei o site oficial de
Rhythmic Geometry e registrei Orbit/Study/Riff como referência de produto; o
Instagram não carregou diretamente, portanto os Reels indexados permanecem
pistas a verificar, não fontes validadas de transcrição.
O autor forneceu a descrição do breakdown de “Invincible” (3:7); registrei
septágono, triângulo, pistas de acento, troca de timbre e superciclo de 21
passos como estudo de caso provisório, pendente de cotejo com a gravação. Após
esclarecimento do autor (impressão baseada em alguns posts, não descrição
confirmada de todo o perfil), reformulei o Study Mode como ferramenta de
análise de faixas reais: decompor instrumentos e relações rítmicas em gráficos
didáticos, com proveniência e interpretações alternativas.
O autor também forneceu captura do breakdown de “Pitch Black” (Meshuggah):
registrei o ciclo de dez notas em tercinas e o retorno alegado em quatro
compassos como caso a validar, incluindo a checagem condicional `4/4 × 3`
(48 subdivisões) contra o período 10.
Uma segunda captura mostrou o Orbit generativo `3:4:5:7`; documentei isso como
modo distinto da análise de faixas: órbitas por camada, fase/velocidade e
rastros sincronizados com eventos, com convenção de razão declarada.
Inspeção de código confirmou que o preset `3:4:5:7` já atua no core JUCE e
protótipo; o `GeometryTraceView` recebe no momento apenas o movimento XY de
V1/V2. Registrei a extensão visual multivoz como próximo passo, reutilizando
calendário e transporte existentes. A captura de “ObZen” acrescentou o
requisito de um sequenciador de células/frases multivoz: três células, 68
semicolcheias (`17/4`) contra bateria `4/4`; os comprimentos internos não foram
inferidos. O alinhamento natural das duas grades, sob semicolcheia comum, é 68
tempos de semínima (quatro frases ou dezessete compassos).

Arquivos: `OPERACOES_GENERATIVAS.md`, `BIBLIOGRAFIA_DE_ESTUDO.md`, este
`TAREFAS.md` e `RASGO_DOCUMENTATION/ATLAS_DE_REFERENCIA_RASGO(4).md`.
Validação: fontes acadêmicas consultadas e referências registradas; revisão
documental. Nenhuma interface, cálculo de Berry/Chern ou renderização foi
implementada nesta etapa; não executei build nem teste. Próximo passo: desenhar
o Study Mode com relógio compartilhado entre grade visual e eventos de áudio,
começando por polígonos sobrepostos e relações polirrítmicas simples antes de
adicionar transcrições culturais identificadas.

### Orbit e Study sobre os ciclos ativos — 01/11/2026

Implementei dois modos de estudo no gráfico de geometria JUCE. **Orbit** mostra
uma órbita/fase colorida por voz e o caminho resultante da soma vetorial
normalizada das fases ao longo da seção. **Study** posiciona pulsos dos stems
num ciclo compartilhado, calcula o LCM quando representável e indica a janela
comum e a proporção usada. O cálculo da grade tem limites (razões aproximadas
com janela máxima de 32 seções; grade máxima de 512 divisões) e a interface
marca aproximações. Ambos seguem receita, overrides, fases e progresso do
transporte atual; pausados preservam a posição. São estudos visuais e não
mudam o áudio.

Os conceitos de Orbit, Study e Riff, incluindo a narrativa de origem, foram
registrados como referência ao material oficial
[How it works](https://rhythmicgeometry.com/how-it-works), fornecido pelo autor.
O autor observou partes A/B/C/D no exemplo de “ObZen” e esclareceu que é apenas
um caso interessante; registrar como possibilidade opcional de seções, sem
transformar essas letras em requisito geral nem assumir como se relacionam às
três células mencionadas na descrição. A implementação do Batruc é própria.
Riff permanece tarefa separada: a forma de sete ciclos e o
editor de baixo não substituem um editor geral de células/frases contra
compasso. O Study de gravações concretas também não foi implementado.

Arquivos: `src/app/Main.cpp`, `OPERACOES_GENERATIVAS.md`,
`BIBLIOGRAFIA_DE_ESTUDO.md`, este registro e o Atlas RASGO.
Validação: preflight final com 6,7 GiB disponíveis, swap em 616 MiB de 2 GiB e
963 GiB livres antes da inspeção; o alvo JUCE Debug `batruc_app` compilou em
modo serial; após ajustes, `Main.cpp` foi recompilado e o link concluiu sem avisos. Binário
Debug SHA-256: `020626bc766e50ce88dcb3415076d95f69be513103b8a313b7e2d27728e26e14`.
Depois, abri uma instância JUCE e selecionei Orbit e Study: ambos desenharam
com a janela ativa; no enquadramento observado, seus controles não se
sobrepuseram ao gráfico nem ao mixer. Study mostrou 512 divisões aproximadas
para a composição seed ativa (12 vozes), não uma prova do caso simples `3:4`;
falta conferir esse preset, testar fases/retorno e razões fracionárias e
inspecionar comportamento durante reprodução/pausa. Não houve escuta A/B, e o
HTML ainda não possui os modos. A instância permanece aberta em Study para
retomada. A captura temporária de conferência está em `/tmp/batruc-study-mode.png`.
Próximo passo: conferir a leitura `3:4`/`3:4:5:7`, os alinhamentos e a
responsividade durante reprodução; depois aplicar a mesma convenção ao
protótipo web ou avançar para o editor Riff de células.

### Esferas, homeomorfismo e Poincaré — 01/11/2026

Acrescentei à pesquisa a distinção entre a superfície `S²`, a bola sólida `B³`
e a 3-esfera `S³`; registrei o enunciado correto da conjectura de Poincaré e
que Grigori Perelman a provou com fluxo de Ricci com cirurgia, no contexto da
geometrização. A aplicação ao Batruc fica como hipótese: trajetórias em
superfícies e reflexão de raios numa sala são experimentos diferentes; o
teorema não calcula reverberação nem som. Também registrei o homeomorfismo como
base para deformar percursos preservando conectividade, deixando explícito que
isso não conserva métricas nem propriedades acústicas.

Arquivos: `OPERACOES_GENERATIVAS.md`, `BIBLIOGRAFIA_DE_ESTUDO.md` e este
registro. Validação: conferi o enunciado e a atribuição histórica na página do
Clay Mathematics Institute e nos preprints de Perelman; revisão documental,
sem build ou teste, pois não houve alteração de código. Próximo passo: se
implementarmos um estudo esférico, definir primeiro dimensão, domínio,
condições de contorno e o mapeamento de trajetória para eventos sonoros.

### Otimização estática do baixo e compilação — 01/11/2026

No render do baixo, movi a exponencial do envelope `pluck` para o início da
nota. O fator por amostra é calculado uma vez e aplicado por recorrência,
mantendo o mesmo decaimento de 1 até 0,12 ao longo da duração configurada;
há somente diferenças possíveis de arredondamento acumulado. O estado só
avança enquanto `pluck` está selecionado, como antes. O alvo JUCE Debug
compilou em série (`cmake --build .local-build/native/linux-debug-juce
--parallel 1`); não rodei testes automatizados nem medi áudio/CPU.

A revisão estática reduziu custos internos do áudio (espectro modal preparado
ao trocar timbre; exponencial do pluck por recorrência) e retirou o `log10` por
amostra da leitura do limitador. A regeneração também reutiliza as avaliações
de posição e intensidade por evento. Durante o relato de animação parada,
encontrei que o áudio era governado pelo atômico `playing_`, enquanto as
animações consultavam `status_`; alinhei atualização do gráfico principal,
rolagem de baixo e geometria ao mesmo atômico do transporte. O alvo Debug
compilou serialmente depois dessa correção; é necessário reiniciar o app para
conferi-la. O processo em execução chegou a 116% de CPU média acumulada, mas
isso não identifica o gargalo e não prova ganho após as otimizações. Próxima
etapa: confirmar animação após reinício e obter amostras separadas de CPU no
transporte parado, em reprodução e durante troca de seed; então mover a
geração de seed para worker limitado, preservando publicação segura ao áudio.

### Medidor de carga do callback — 01/11/2026

Acrescentei no rodapé JUCE o indicador `CPU AUDIO`, atualizado pela interface
com uma média suavizada de 7/8 do valor anterior e 1/8 do bloco mais recente.
O callback mede sua própria duração com ticks de alta resolução e compara com
`frames/sampleRate`; 100% significa consumir o orçamento de tempo do bloco, e
valores acima de 100% significam exceder o prazo de áudio. A medida cobre o
`getNextAudioBlock` inteiro (incluindo renderização, baixo, limitador e
encaminhamento de gravação), não uso geral do processador do sistema. A
publicação usa um atômico de 32 bits; nenhuma operação de interface ou
alocação foi introduzida no callback. Tooltip em português, inglês e francês.
O alvo `batruc_app` compilou em Debug serial, sem testes automatizados nem
medição auditiva. Aplicativo: 0.1.0; SHA-256 do executável:
`59df86914ed1d07098d34802d521d724398b57f6479bfac81932f92cef8c355f`.
Próximo passo: conferir no app que o indicador atualiza no rodapé com o
dispositivo ativo e retorna a zero/baixo quando o dispositivo para.

### Cabeçalho e rodapé fixos na janela JUCE — 02/11/2026

Fixei a faixa superior (marca e controles de seed/cenas/conteúdo/idioma) e a
barra inferior (transporte, receita, padrão, baixo, gravação, geometria,
volume, estado do master, créditos e CPU AUDIO) ao viewport; os painéis centrais
continuam roláveis. A área fixa é reposta ao rolar e redimensionar a janela. A
reprodução agora invalida a fatia atualmente visível, em vez de sempre repintar
somente os primeiros 800 px do documento; isso deve manter os traços animados
ativos ao explorar os painéis inferiores. Atualizei também o README. Ainda falta
conferir o comportamento visual no desktop; esta sessão não abriu outra janela
do app para não interromper o uso do autor. O alvo `batruc_app` compilou em Debug
serial em 02/11/2026, sem testes automatizados. SHA-256
do executável: `6a9fe8be8105a1133df5ae20064b98d3593928f78a723b91ae8555fa0a3a4aa5`.
Próximo passo: reiniciar o Batruc e conferir a faixa fixa ao rolar e redimensionar.

### Execução mais leve no Linux — 02/11/2026

O launcher `./run` sempre abria o executável Debug, mesmo quando havia uma
compilação Release disponível. Agora prioriza Release otimizada e mantém Debug
como fallback. Recompilei `batruc_app` em Release serialmente. O executável caiu
de 112,8 MB (Debug) para 16,5 MB (Release), reduzindo seu tamanho em disco. A
otimização de compilador do Release deve reduzir o trabalho de CPU em relação
ao Debug, mas isso ainda precisa ser medido em execução.
Também limitei o repaint do canvas principal à interseção entre a área visível e
os primeiros 800 px animados; os painéis inferiores continuam atualizando seus
próprios gráficos sem repintar a página inteira a 24 Hz. `bash -n ./run` passou.
Nenhum teste automatizado ou audição foi executado.
Release SHA-256: `c24dcb218e9d9eb70cc7fa105d0bbf8aad61f53057788214d8339baf4009b7b6`.
Próximo passo: reabrir por `./run` e comparar o CPU AUDIO e a resposta dos
controles; em seguida medir o processo em repouso, tocando e trocando seed antes
de escolher outras otimizações.

### Faixas fixas opacas e cabeçalho compacto — 02/11/2026

Após o teste do autor, corrigi a translucidez: os fundos do cabeçalho e rodapé
agora são componentes irmãos opacos, sobre o conteúdo rolável e sob os controles
fixos. Removi o segundo rótulo `VOZES`, que se sobrepunha ao botão de seed, e
posicionei o rótulo acima do seletor de quantidade. Comprimi a faixa superior
para 106 px, usei toda a altura do topo (sem a margem vazia anterior) e mantive
os controles na mesma linha mesmo na largura mínima suportada. O Release
`batruc_app` compilou em série; `bash -n ./run` passou; nenhuma validação visual
foi feita nesta sessão. SHA-256 do Release:
`7d54cd7c7632426ed9e00bf8a9ea6d43d3e2660ab7672511c93f01a2e833fd5f`.
Reiniciar pelo `./run` para ver a correção; depois conferir contraste, encaixe e
cliques nas duas faixas durante a rolagem.

### Alinhamento superior e assinatura ao lado da marca — 02/11/2026

Reposicionei a faixa “Matemática / Física / Música” para junto do lettering
Batruc. Na largura ampla, os controles do cabeçalho sobem para a mesma faixa
superior; na largura mínima, permanecem numa segunda linha abaixo da assinatura,
com separação vertical para não colidir com o rótulo “VOZES”. A altura do
cabeçalho se adapta: 80 px em telas amplas, 106 px na largura mínima. O alvo
Release `batruc_app` compilou serialmente e `bash -n ./run` passou. A checagem
estática de linhas não encontrou espaços finais; validação visual nos dois
limites de largura continua pendente. SHA-256 do Release:
`0b6f2aa1698a4622302a74f9a6af943493c4a3af15e9209ca9f221086b1f8fae`.

### Geometria integrada à área de composição — 02/11/2026

O autor pediu um primeiro ecrã mais ergonômico: ondas e percursos no mesmo
espaço, sem rolar, e Mixer/Baixo nas seções abaixo. A geometria anterior
concentrava percursos, áreas e controles num bloco de 660 px. Na tela JUCE com
largura a partir de 1680 px, a coluna que mostrava o campo de fases agora recebe
o gráfico geométrico animado, ao lado das ondas. Uma faixa compacta reúne forma,
Orbit, Study, pêndulo, vínculo opcional `V1 ← y(t)` e alternância para o campo
de fases. Controles de Fourier, bilhar e Lissajous aparecem conforme a curva
escolhida. O desenho do percurso acompanha o transporte; triângulo e quadrado
mantêm os gestos de arrastar vértices/corpo já codificados. Em larguras menores,
os controles continuam no painel rolável para não invadir andamento, intenção
ou dinâmica.

A seção inferior foi reduzida a 450 px e agora trata áreas/medidas de Stengel;
Mixer e Baixo foram aproximados sem remover seus controles. Ao exibir o percurso
inline, o campo de fases deixa de ser pintado por trás dele; a curva geométrica
usa 320 amostras em vez de 512. O autor relatou melhor resposta, mas movimento
visual ainda truncado apesar do medidor `CPU AUDIO` próximo de 4%. A causa
confirmada é que esse medidor cobre somente o callback de áudio e não informa a
cadência do desenho. O timer da GUI foi elevado de 24 para 60 Hz; publicação do
baixo, captura de revisão, sensor do mixer, medidores e estados não visuais são
atualizados a cada terceiro tick (20 Hz), enquanto os cursores e gráficos
animados recebem as atualizações do timer. Isso busca suavizar a animação sem
levar as tarefas de manutenção para 60 Hz. O JUCE ainda depende da thread da
interface e do compositor do sistema; 60 Hz é a cadência solicitada, não uma
garantia de 60 quadros renderizados.

Arquivos alterados: `src/app/Main.cpp`, `README.md` e `TAREFAS.md`. O alvo
Release `batruc_app` compilou em série sem avisos; `bash -n ./run` passou e a
busca por espaços finais não encontrou ocorrências. Binário:
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
2026-11-02 00:34 CET, SHA-256
`20d9bdb35973def9d433bd751ee8466390a2677ef1c19e998da8c19bae9a4822`.
Nenhuma janela foi aberta para preservar a sessão de teste do autor. Falta
confirmar visualmente tela ampla, modo campo, seleção de curvas, gestos de
edição e fluidez percebida. Próximo foco: medir responsividade visual e CPU do
processo sem abrir outra instância e corrigir colisões observadas.

### REC performático e limite de duração — 02/11/2026

O REC JUCE agora indica a captura com fundo vermelho e o rótulo `STOP mm:ss`,
atualizado uma vez por segundo. Uma ComboBox compacta adjacente oferece limites
de 1, 2, 3, 5, 10 ou 15 minutos; 5 minutos é o padrão e a escolha persiste nas
preferências locais. Atingido o tempo, a interface finaliza a gravação. O
seletor fica desativado durante captura e fechamento. O WAV/TXT continuam sendo
escritos e fechados fora do callback de áudio. O término normal deixa uma
mensagem breve no status, sem janela modal; alertas continuam reservados para
falha de metadados ou blocos perdidos. Também esclareci o modo de forma `∞`/`7×`:
`∞` repete, enquanto `7×` toca uma passagem pelos sete ciclos e para.

Arquivos alterados: `src/app/Main.cpp`, `README.md` e `TAREFAS.md`. O alvo Release
`batruc_app` compilou serialmente sem erros; `bash -n ./run` passou e a busca por
espaços finais não encontrou ocorrências. Binário:
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
2026-11-02 00:46 CET, SHA-256
`2d691da3e5ffb6b852180957b3919d969d915b91afd54795b97eab9a562566e8`.
Não executei testes automatizados nem abri uma janela adicional. Falta conferir
visualmente o alinhamento do novo seletor e testar a contagem/auto-parada na
sessão do autor.

### Bancada de áreas e medidas — 02/11/2026

Reorganizei a seção independente de áreas de Stengel para ocupar 260 px em
telas amplas: os três sliders numéricos ficam curtos no topo e o gráfico, com
148 px de altura, usa a largura disponível. `x`, `y` e `z` também podem ser arrastados diretamente nas
barras; o gesto atualiza os mesmos sliders e recalcula `w`, `S` e o resíduo da
relação. O gráfico identifica que `w` é calculada e que as barras representam
magnitudes, não a disposição espacial das cevianas. A bancada é um estudo
matemático didático e ainda não altera o áudio.

Em largura abaixo de 1680 px, a área matemática permanece no início da seção e
os controles/percurso geométrico passam a uma faixa posterior dentro do mesmo
bloco; o Mixer e o Baixo deslocam-se com a altura do bloco para não cobrir essa
faixa. O campo de fases e o percurso geométrico voltam a aparecer lado a lado
no layout amplo; o botão `CAMPO` oculta somente o campo. Corrigi também o
rodapé: Baixo, REC, limite de REC, Áreas, volume, medidor e status agora têm
separação explícita; antes, o Baixo invadia o botão de forma em 14 px.

Acrescentei em `RASGO_DOCUMENTATION/design/INTERFACES_E_LAYOUTS.md §4.1` a regra
transversal de não encobrir controles/gráficos e de manter hit targets distintos
em gráficos editáveis. No modo Orbit, arrastar o ponto móvel de uma voz agora
desloca a fase da fórmula, atualiza o slider `φ fórmula` e a respectiva curva de
acentuação; o tempo dos ataques fica fixo. Study segue como leitura sem edição
de pontos. Fontes alteradas: `src/app/Main.cpp`, `README.md`, este registro e a
diretriz transversal. O primeiro build Release apontou uso de `setTooltip` numa
vista JUCE que não implementa esse método; removi a chamada e deixei a instrução
diretamente visível no gráfico. A compilação serial final do alvo `batruc_app`
passou. `bash -n ./run` passou. Não executei testes automatizados, não abri
outra instância e não fiz captura: validação visual e teste manual do arraste
continuam pendentes na sessão do autor. Binário Release:
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
2026-11-02 01:15 CET, SHA-256
`18ef29a58899bb0e4e040927529fae44b00fe006a8b942c21d7ac77b0950fa84`.

### Gestos musicais no Orbit — 02/11/2026

Ampliei a edição direta para além da fase: arrastar o ponto de uma voz edita
`φ fórmula`; `Shift` + arraste vertical sobre o ponto altera `ciclos/forma` em
passos de 0,25 por 8 px. Ambos os gestos atualizam o slider da voz, reaproveitando
seus callbacks, atualização do motor, estado manual e indicação existente de
quando a mudança de ciclos passa a valer. O cursor muda sobre os pontos e a
legenda do Orbit descreve os gestos. `Study` permanece somente para leitura; a
edição da grade compartilhada exigirá um modelo próprio para evitar confundir
fase de acento e fase de ataque.

Validação: compilação Release serial de `batruc_app` concluída, `bash -n ./run`
passou e não há espaços finais nos arquivos alterados. Nenhum teste automatizado
foi executado. Falta validação visual e teste manual do mouse na sessão do
autor; nenhuma janela adicional foi aberta. Binário:
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
2026-11-02 01:27 CET, SHA-256
`e18334faa40e9e9928d8bdf8ed8293c03221e1a0f1e23a602cb31f8ff2364ecc`.

### Redução da bancada de Stengel — 02/11/2026

Após o pedido reiterado para diminuir o quadro, reduzi a seção de 330 para 260
px (21% menos altura) e o gráfico de 194 para 148 px. Compactei o espaçamento
das quatro barras e mantive x/y/z arrastáveis, os resultados calculados e a
escala numérica; aumentei a parcela horizontal destinada ao gráfico ao encurtar
a faixa dos sliders. Mixer e Baixo sobem junto; os demais gráficos não mudam.
O build Release serial de `batruc_app`, `bash -n ./run` e a checagem de espaços
finais passaram. Não executei testes automatizados nem abri outra janela; falta
conferência visual do quadro reduzido. Binário:
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
2026-11-02 01:30 CET, SHA-256
`8a607575b1f502a792fef37fd49b38f1c4ab9540910deb030252e8ffa9e04075`.

### Manipulação direta da curva de Lissajous — 02/11/2026

Acrescentei alças `Ax`, `Ay` e `φ` na curva de Lissajous. Arrastar cada alça
atualiza o slider correspondente e redesenha a função pelo mesmo caminho de
atualização já usado pelos controles. A fase é calculada pela ordenada do ponto
inicial e seleciona o ramo angular mais próximo da fase atual para evitar saltos
de 180°. A curva só afeta o áudio se `V1 ← y(t)` estiver ativo. Compilação
Release serial, `bash -n ./run`, busca por espaços finais e `git diff --check`
passaram. Não executei testes automatizados, não abri outra janela e não testei o
mouse; a validação visual permanece pendente. Binário:
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
2026-11-02 01:41 CET, SHA-256
`2642d01880edb81b65b8d4da04941ff7f1429948ab78b89c27cb44e27371bed7`.

Refinamento: desloquei visualmente a alça `φ` 12 px para a direita e acrescentei
uma guia curta até sua posição calculada na curva. O hit test acompanha a alça
deslocada, permitindo selecionar `φ` e `Ay` separadamente quando os pontos
matemáticos coincidem. O arraste ainda calcula a fase pela posição vertical real
do cursor. A compilação Release serial terminou com sucesso em 02/11/2026 às
01:46 CET; `bash -n ./run`, busca por espaços finais e `git diff --check`
passaram. Não houve teste manual da GUI. Binário:
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
SHA-256 `bbba201e5d7e35f0661cc51ecd0f4fb462fff0ebcee730b1b73c9c2a155e568f`.

### Correção do arraste das barras de Stengel — 02/11/2026

Corrigi a conversão entre a posição do ponteiro e as barras x/y/z: havia um
deslocamento vertical de 10 px, que associava algumas alturas à variável da
linha anterior. O cálculo horizontal agora usa o mesmo inset de pintura, e o
arraste só começa sobre a extensão visível das barras; cliques nos rótulos e
espaços entre linhas são ignorados. O redesenho continua acionando os sliders
existentes e recalcula w, S e o resíduo. Build Release serial de `batruc_app`
concluído; `bash -n ./run`, busca por espaços finais e `git diff --check`
passaram. Não fiz teste manual de mouse nem captura da janela. Binário:
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
2026-11-02 01:51 CET, SHA-256
`edac19dbc85f356414045c2800ecb7b8fdd7b54d411721a13edc738808cb6869`.

### Fim da rolagem no módulo do Baixo — 02/11/2026

A altura da página agora é calculada pela largura efetiva e termina após o
módulo do Baixo. Em telas largas, removi a cauda vazia fixa de 3200 px; em
telas menores, a altura inclui o percurso geométrico adicional. Reservei o
rodapé fixo mais 24 px de respiro para que ele não cubra os últimos controles
quando a barra de rolagem chega ao limite. A seção usa a mesma constante para
desenhar seu cartão e calcular o fim da página. Build Release serial de
`batruc_app` concluído; `bash -n ./run` e `git diff --check` passaram; a busca
por espaços finais não encontrou ocorrências. Não abri a GUI; falta conferir
visualmente o limite final da rolagem. Binário:
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
2026-11-02 01:54 CET, SHA-256
`40c71b7e42e0c4120548cb004586fd512cd9c047d1657c29693d3e0b1b312ef8`.

### Atalho explícito para composição manual sem seed — 02/11/2026

O Mixer agora tem um botão alternável que entra em modo manual sem exigir que o
usuário apague o campo de seed e pressione Enter. No modo manual, o botão passa
a oferecer a volta por uma seed aleatória. Atualizei também a dica do campo de
seed e as traduções espanholas das novas legendas. O botão ocupa o espaço
reservado no fim da linha dos presets polirrítmicos; a prévia da fórmula termina
8 px antes dele para evitar sobreposição. Também corrigi a resposta visual: ao
alterar a contagem com o transporte parado, o gráfico principal agora é
repintado junto com o novo plano de eventos. Build Release serial e `bash -n ./run`
passaram, assim como a busca de espaços finais. Não abri a GUI; `wmctrl` não pôde
acessar o display nesta sessão. Falta conferir visualmente o botão e testar as
duas transições no app. Binário:
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
02/11/2026 17:33 CET, SHA-256
`fe2a7fcb477989e7c6b4ac3c7fb8fd5a061f555a346c5c0d67676f4262b1ec1c`.

### Títulos dos gráficos animados e auditoria de geometria rítmica — 02/11/2026

Dei título explícito às vistas JUCE de vozes/onda-fluxo, percurso geométrico,
Orbit e Study, e ao piano roll do baixo. O campo de padrão já apresenta um
título que muda com o modo; LFO, percurso do baixo, pêndulo e energia já tinham
rótulos próprios, preservados. Usei as faixas dos gráficos para os títulos;
não criei componentes nem sobrepus controles aos traços.

Também comparei as referências com o código efetivamente usado. A agenda sonora
já combina ciclos/fases por voz, presets polirrítmicos e seed; máscaras de
campo filtram ataques; a curva geométrica pode alimentar V1 quando `V1 ← y(t)`
está ativo; e o percurso de bilhar pode fornecer eventos. O baixo é calculado
com base nos eventos/estados rítmicos configurados. Orbit, Study e pêndulo são
leituras visuais/estudos e não alteram a agenda sonora; a bancada de áreas de
Stengel também não produz som. Portanto, não trato animação como prova de
composição generativa.

O Atlas agora registra RHYTHMOS como referência acadêmica e RTG (Xavier
Góngora, FARM 2024) como referência de transformações geométricas de ritmos.
RTG está sob GPL-3.0 e se declara experimental; o artigo relata questões ainda
abertas sobre leis de grupo e coerência musical. Nada foi copiado. JUCE
Graphics/Path e Canvas/SVG nativos cobrem o desenho; D3 (ISC) é uma opção
web livre mas redundante agora, e Tone.js (MIT) é áudio web, não motor
geométrico. Não acrescentei dependências.

Próximo incremento composicional recomendado: formalizar células de ataques e
pausas e executar rotação, reversão, complemento e sobreposição com regras
explícitas para colisões. O mesmo resultado deve aparecer no círculo, no piano
roll, na partitura e no áudio. Antes de promover cada operação, validar suas
invariantes e seu efeito musical em um modelo compartilhado C++/JS; não presumir
que essas operações formam um grupo.

Arquivos alterados: `src/app/Main.cpp`, `README.md`, `TAREFAS.md` e
`RASGO_DOCUMENTATION/ATLAS_DE_REFERENCIA_RASGO(4).md`. A compilação Release
serial de `batruc_app` concluiu às 17:42 CET; `bash -n ./run` passou e não há
espaços finais nos três arquivos locais. A checagem `git diff --check` do Atlas
também passou. Não rodei testes automatizados nem abri uma janela do app; falta
conferir a leitura visual no display do autor (`wmctrl -lG` retornou
`Cannot open display`). Binário:
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
2026-11-02 17:42 CET, SHA-256
`52fdfe3d385a303e1af80c2101451d378afb259a654a53dc29184a7e85a79bc6`.

### Primeiro incremento real do redesenho — 02/11/2026

Correção de estado após inspeção do autor: o wireframe aprovado existia só como
SVG/documento; recompilar o app não o implementava. A interface geral continua
na hierarquia antiga. Apliquei apenas dois incrementos funcionais e visuais:
Study abre como vista geométrica inicial, expondo ciclos/ataques por voz e o
arraste de fase que já altera os ataques; o título de Onda/Fluxo foi limitado
à coluna livre para não cruzar controles de zoom/análise.

Validação: Release serial compilou; `--audit-layout=/tmp/batruc-layout-final-20261102`
capturou 1080×800 e 1440×900 (`captured: true`); inspeção visual confirmou Study
como vista inicial e o título sem cruzar o zoom; `bash -n ./run` e `git diff
--check` passaram. Uma inicialização normal por `./run` retornou SIGSEGV; uma
execução normal sob GDB permaneceu aberta até interrupção, e o modo de auditoria
terminou corretamente. A causa da diferença não foi isolada; não orientar o
autor a tratar o redesenho amplo como pronto. SHA-256 do Release:
`73c21c148de23d990dac7c5b52440fbe05a6553260e477229196bff2a6cacbe9`.

Próximo passo prioritário: reproduzir e resolver a falha de inicialização normal;
então implementar as áreas semânticas do wireframe e eliminar as sobreposições
medidas no Baixo. Editor de Ciclos com edição de ataques independentes continua
pendente; Study desloca a fase global da voz e não substitui essa função.

### Correção visível de colisões na primeira tela — 03/11/2026

Após conferir a captura real em 1080×800, corrigi os defeitos que ainda
apareciam: título de onda e toolbar disputavam a mesma faixa; o painel Study
começava atrás da segunda fileira de controles; e os rótulos de intenção e
dinâmica eram desenhados duas vezes. Agora a toolbar fica na faixa superior do
gráfico, título/estado ficam abaixo dela, os traços começam depois do título,
Study começa em y=284 (abaixo dos controles) e as escolhas inferiores ficam em
faixas separadas da fórmula e da linha C1–C7. O campo lateral começa fechado,
cedendo largura ao percurso geométrico. Em janelas altas, as vistas aumentam e
a faixa de fórmula/controles/seções desce proporcionalmente; corrigi o cálculo
para usar o tamanho do viewport, não a altura total da página de rolagem. O
relatório de auditoria agora inclui bounds e estado visível do percurso, o que
detectou e permitiu corrigir o desenho ausente na largura mínima.

Arquivos alterados: `src/app/Main.cpp`, `README.md`, este registro e
`AUDITORIA_LAYOUT_COMPOSICIONAL_2026-11-02.md`. Validação: build Release serial
de `batruc_app`; modo `--audit-layout` capturou 1080×800 e 1440×900, incluindo
topo, Mixer e Baixo; `audit-result.json` retornou `captured: true`; inspeção
visual da captura 1080 confirmou separação dos elementos descritos. Ainda não
estão implementados o novo layout semântico completo, a edição independente
de cada ataque no círculo, nem a auditoria corretiva do módulo Baixo. O teste
de captura tampouco substitui a verificação do autor em uso contínuo.

Uma execução direta do modo de auditoria falhou uma vez com SIGSEGV; a execução
subsequente sob GDB e outra execução direta concluíram normalmente. A
intermitência de inicialização segue registrada, sem causa confirmada.

Ponto de retomada: executar `./run`, confirmar que a janela aberta corresponde
ao Release recém-compilado e avaliar a navegação/edição com o autor; em seguida
resolver o layout do Baixo e implementar o Editor de Ciclos ligado ao mesmo
modelo de eventos do áudio e das cenas.

Binário Release gerado em 03/11/2026 20:10 CET; SHA-256
`75b89c00a61af4a278749c7109e1f56659afe9ec75c244ce0bcec21561abe6bd`.
`./run` aponta para esse caminho Release. `bash -n ./run` passou; a checagem
visual do modo de auditoria concluiu com `captured: true`.

### Reorganização do layout nas três regiões — 03/11/2026

Motivo: o incremento anterior ainda abria com a hierarquia antiga. Nesta
passagem, a coluna principal da primeira tela passou a ser o Study, à esquerda;
Onda/Fluxo ficou à direita. O cálculo das duas colunas usa a largura do app,
e ambas crescem na altura disponível ao maximizar. O Study ganhou legenda de
cor, voz e número de ataques. Reduzi a logo aprovada e mantive os créditos.
No Baixo, separei knobs, variação, LFO e percurso em linhas sem interseção e
ampliei a seção até depois do último controle. Os saltos de Mixer/Baixo agora
respeitam o cabeçalho fixo. Não houve alteração de áudio nem de cena.

Arquivos: `src/app/Main.cpp`, `README.md`, `TAREFAS.md` e
`AUDITORIA_LAYOUT_COMPOSICIONAL_2026-11-02.md`. Build Release serial concluído;
auditoria de layout em 1080×800 e 1440×900 capturou topo, Mixer e Baixo em
`/tmp/batruc-layout-gate-20261103` com `captured: true`. Inspeção visual
confirmou a hierarquia e a separação das áreas antes sobrepostas no Baixo.
O modo de auditoria agora inclui capturas de início de Mixer e Baixo para
verificar os saltos. Falta conferir a interação real em monitor do autor,
implementar edição independente de ataques, reduzir densidade do Mixer com
inspetor de stem e investigar a inicialização intermitente. Essas pendências
impedem tratar o redesenho completo como aceito.

Correção de inicialização nesta mesma passagem: o coredump de 03/11 indicou
`juce::Component::centreWithSize` na criação da janela. A janela agora usa
bounds iniciais explícitos; duas execuções diretas consecutivas do Release
capturaram as telas e encerraram normalmente. `bash -n ./run`, checagem de
espaços finais e `git diff --check` não apontaram erros. Binário Release final
de 03/11/2026 20:30 CET, SHA-256
`c7a922b9e8d7367202804b8bdead6744431ea38fb08165b1912943f714acbc1e`.
Ainda falta observar uma sessão longa de uso real para declarar a estabilidade
de inicialização resolvida em todas as condições.

### Mixer com inspetor do stem — 03/11/2026

Implementado em `src/app/Main.cpp`: V1–V8 são selecionáveis; parâmetros de
fórmula, timbre, evento, acoplamento, fase, ciclos e variação usam um único
inspetor. Nível, mute/solo, pan, movimento e miniatura permanecem no stem.
A seleção é só de interface, e o mixer conserva os dados de todas as vozes.
Build Release serial passou e a auditoria de 1080×800/1440×900 capturou V1 e
V2 em `/tmp/batruc-mixer-bounds-zero-20261103`; o JSON informa
`captured: true` e zero interseções de bounds dos controles visíveis do Mixer
para V1/V2 nas duas larguras. Dois pares de rótulo/slider foram corrigidos
depois que a primeira contagem os detectou. SHA-256 do Release:
`49a04e6f437b76ba5984a534aa86a1031684455367a943e07fe3b93b332a1f0b`.
Permanecem pendentes medidores de nível reais,
edição individual de ataques no Editor de Ciclos, ensaio manual de interação e
matriz de DPI/idiomas. A direção continua sendo o wireframe aprovado.

### Navegação sem barra visível e cabeçalho revisado — 03/11/2026

Por correção do autor, a barra de rolagem vertical foi ocultada e os três
destinos principais, Editor, Mixer e Baixo, passaram a ocupar botões do
cabeçalho. O botão ativo acompanha a posição da vista; Baixo saiu do rodapé.
Redistribuí seed, nova seed, vozes, cenas, sobre, aprender e idioma numa única
linha sem colisão em 1080 px. O espaço livre no rodapé foi usado para alinhar
REC, limite, Áreas e master. A paleta da primeira tela passou a usar fundo
verde claro e cartões claros próximos do SVG aprovado; as ações principais
do cabeçalho têm preenchimento escuro e estado ativo turquesa.

O conteúdo de Mixer e Baixo excede a altura de 800 px; a roda do mouse segue
necessária para alcançar todos os controles, sem barra visível. Validação:
build Release serial sem aviso; capturas locais de Editor, topo do Mixer e
topo do Baixo em 1080×800 e 1440×900, com destaque correto da navegação,
em `/tmp/batruc-navigation-final-20261103`; JSON `captured: true` e zero
interseções de bounds dos controles do Mixer nos estados V1/V2 testados.
SHA-256 do Release:
`fc6134351109a0ebfaf13bdcdb30eb94a7733d6374f996783e65b2d59209c8df`.
Falta confirmar cliques no desktop do autor, além dos outros estados, idiomas
e DPI.

### Direção visual do Editor de Ciclos — 04/11/2026

O autor confirmou que gosta da linguagem gráfica e das fontes da proposta
[`PROPOSTA_LAYOUT_EDITOR_CICLOS_2026-11-02.svg`](PROPOSTA_LAYOUT_EDITOR_CICLOS_2026-11-02.svg).
Preservar essa tipografia, paleta clara, traços finos, anéis legíveis e
polígonos coloridos como identidade do Editor; não substituir por uma grade
genérica nem por animação decorativa. A hierarquia atual JUCE, com Editor,
Mixer e Baixo no cabeçalho fixo e créditos no rodapé, continua valendo; o SVG é
referência visual/composicional, não especificação para remover esses acessos.

Gap funcional prioritário: o Study atual permite deslocar a fase global de uma
voz. O gesto aprovado no SVG é diferente: cada vértice representa um ataque
editável, e a alteração precisa atualizar a agenda realmente tocada e salva na
cena. Próximo incremento deve começar pelo contrato de célula/onsets limitado
no core e sua persistência; depois ligar a seleção/arraste de ataque no gráfico
e confirmar que áudio, waveform e dados da cena refletem o mesmo evento. Até
essa ligação existir, identificar Study como leitura/ajuste de fase, sem
apresentá-lo como editor individual já concluído.

Primeira fatia do contrato implementada em `src/core/CompositionCore.hpp` e
`CompositionCore.cpp`: `RhythmicCell` aceita até 32 posições normalizadas,
ordenadas e com intensidade individual; validação e operações limitadas
inserem, removem, movem e acentuam ataques. `buildEventPlan` consome a célula
como agenda real quando habilitada. O renderizador preserva esses ataques
preparados quando uma regra dinâmica está ativa e aplica a máscara móvel no
instante tocado. Células desativadas seguem a geração por fórmulas.

Validação: build Release serial de `batruc_app` e do teste focado
`batruc_seeded_recipe_test`; este passou após atualizar os três vetores de
seed que ainda esperavam mais de oito vozes. O teste também confere inserção,
remoção, deslocamento, posições e intensidades dos eventos da célula.

Continuação de 04/11/2026: o schema de cena 40 agora grava, por canal,
habilitação, posições e intensidades da `RhythmicCell`. O parser valida
quantidade, ordem, distância mínima e limites dos valores. Cenas de schema 39
ou anterior carregam com as células desativadas; o diagnóstico de cena verifica
ida e volta do schema 40 e leitura legada 39/38. O plano de exibição e a
partitura inicial de gravação recebem as células restauradas. Arquivo afetado:
`src/app/Main.cpp`. Naquele ponto, faltava expor a célula na GUI e publicar
alterações ao vivo no renderizador.
Validação desta continuação: build Release serial de `batruc_app`, teste
`batruc_seeded_recipe_test` (exit 0) e diagnóstico
`--diagnose-logistic=/tmp/batruc-cell-scene-20261104`
(`scene_round_trip: true`, schema 40/39/38); o diagnóstico também confirma a
rejeição de dois ataques sobrepostos no mesmo canal. `git diff --check` limpo.

Continuação de 04/11/2026 — edição audível no Study: duplo clique no anel
insere ataque para a voz; arraste do vértice muda sua posição, Shift+arraste
ajusta a intensidade, botão direito remove, Alt+clique no anel retorna à regra
por fórmula. O polígono desenha as posições da célula e o diâmetro de cada
vértice traduz sua intensidade. A cena 40 guarda esse estado. Ao escolher uma
nova seed, as células manuais são limpas para a receita nova soar livremente.
O gráfico de onda usa o plano de exibição reconstruído com as mesmas células.
O cálculo do baixo derivado da seed também recebe as células e só é refeito ao
fim do arraste ou em edições discretas.

Publicação de áudio: a interface copiava até 8 × 32 ataques nesta etapa; a
capacidade atual é 8 × 64, após o ensaio MIDI de 10/11. Usa uma caixa de
passagem de estado fixo; o callback consome a cópia sem alocar memória. O
renderizador troca a agenda ativa na próxima fronteira de compasso e agenda
os ataques da célula na amostra correspondente, preservando swing, acento,
expressão, lei de intensidade e campo. Um teste focado compara dois
renderizadores idênticos, um com ataque em 0,5 seção e outro com célula vazia:
os sinais coincidem antes da posição e divergem depois dela. Arquivos:
`src/core/CompositionCore.hpp`, `src/core/CompositionCore.cpp`,
`src/app/Main.cpp`, `tests/seeded_recipe.cpp`.

Na captura de layout a 1080 px, V8 encostava em Variação no painel lateral do
Editor; a lista de vozes foi compactada de 31 para 26 px por linha e o fundo
do painel limitado à sua área. Captura recompilada em 1080×800 confirma V8
separada de Variação; auditoria em 1080/1440 informa zero interseções nos
controles do Mixer. `batruc_seeded_recipe_test` e `git diff --check` passam.
Pendente: escuta manual de edição durante reprodução no app aberto, inclusive
o contraste de intensidade por Shift+arraste e a volta à fórmula por Alt+clique.

Continuação de 04/11/2026 — grade temporal compacta: o Editor agora mostra
16 divisões por voz ao lado do polígono. O clique alterna um ataque da célula;
a primeira edição converte a agenda por fórmula em célula manual e o arraste
do vértice permite posição não quantizada. A grade e o polígono usam o mesmo
estado; edição atualiza visualização, baixo derivado e publicação ao áudio.
Referências recebidas de `@maxim_codex` e `@thel.ab67` foram registradas em
`BIBLIOGRAFIA_DE_ESTUDO.md` com limites de proveniência e sem atribuir licença
ao software mostrado. Build Release serial de app e teste focado concluído;
`batruc_seeded_recipe_test` terminou com exit 0; captura do Editor e auditoria
1080×800/1440×900 repetidas após aumentar as células para 9 px de altura em
`/tmp/batruc-grid-final-20261104/` (`captured: true`; zero interseções
apontadas no Mixer). Inspeção visual do Editor com oito vozes não mostrou
colisão entre grade, Variação, equação e rodapé. `git diff --check` limpo.
Ainda falta verificar o clique real e a escuta ao vivo na máquina do autor.
As novas capturas de `@thel.ab67` também sugerem experimentos futuros de
eventos por colisão, transformação de uma seção durante dezenas de compassos
e uma continuidade controlável entre densidade de pulsos e timbre; esses
procedimentos precisam de desenho composicional próprio antes de codificação.

Correção de enquadramento em 04/11/2026: uma captura do autor em 1920×1080
mostrou Editor e Mixer parcialmente visíveis durante a rolagem; a captura de
diagnóstico em 1080×800 revelou um problema adicional: mesmo com o Mixer
alinhado pelo botão do cabeçalho, a base do inspetor de stem ficava sob o
rodapé fixo. Em `src/app/Main.cpp`, subi o inspetor 22 px, reduzi a seção do
Mixer de 675 para 635 px e passei a marcar no cabeçalho a seção que ocupa o
centro da janela. A nova captura `1080-mixer-top.png` em
`/tmp/batruc-mixer-fit-20261104/` mostra a última linha de controles e a
borda inferior do inspetor acima do rodapé. Build Release serial concluído.
Depois da captura, a roda do viewport passou a saltar entre Editor, Mixer e
Baixo, com intervalo curto para não atravessar duas seções num gesto; dentro
do Baixo mantém rolagem normal para seus controles inferiores. Os botões do
cabeçalho continuam alinhando as seções ao topo útil. Ainda requer ensaio
manual de roda e touchpad no desktop do autor após reabrir o binário. O build
Release foi repetido após essa mudança; o diagnóstico final gerou capturas em
`/tmp/batruc-fit-final-20261104/` e `git diff --check` não apontou erros.

### Vista gráfica das leis de intensidade A/B — 06/11/2026

Ampliei a análise cíclica do gráfico de ondas com a vista `I` (intensidade).
Ela desenha as curvas das leis A e B e, com traço mais espesso, o multiplicador
resultante depois do peso global da fórmula. As curvas usam as duas fases que
alimentam as coordenadas geométricas do renderer e acompanham a janela de tempo
do gráfico. A vista é apenas analítica e não modifica áudio. `I` foi incluída
no ciclo do seletor existente, preservando as vistas de sinal, derivada e
integral.

Arquivos: `src/app/Main.cpp`, `OPERACOES_GENERATIVAS.md` e este registro.
Validação: build Release serial passou; `git diff --check` passou; o diagnóstico
de layout capturou 1080×800 e 1440×900 sem erro e confirmou o enquadramento
geral. Não consegui acionar a vista `I` na instância visual depois de o processo
de captura fechar a janela; portanto a curva específica ainda precisa de
inspeção visual manual. Limite conhecido:
as leis são escolhidas de uma lista fechada; edição de coeficientes e expressões
livres continua como tarefa futura. Próximo passo: compilar, inspecionar a
vista `I` em resolução de janela real e, se ela estiver legível, prosseguir
para tornar os parâmetros das leis editáveis sem introduzir uma linguagem de
expressões insegura no callback de áudio.

Continuação em 06/11/2026: o diagnóstico de layout ganhou seleção interna da
vista `I` e captura dedicada em 1080×800, 1440×900 e 1920×1080. As três imagens
foram inspecionadas: curvas A e B, resultado misturado, legenda, escala e
controles permanecem legíveis e contidos na coluna vinculada. O JSON retornou
`captured: true`, `intensity_graph_captured: true`, round-trip da cena 8-bit e
zero interseções do Mixer nos estados V1, V2 e Chip 8-bit nas três resoluções.
O build Release serial de `batruc_app` e o teste
`batruc_seeded_recipe_test` passaram. Binário de 06/11/2026 22:22 CET,
SHA-256 `1935b09e68efad2a8bdb67c29893d37a81e93b36600769fce4c55563f8ffed7f`.
Próximo passo técnico: tornar coeficientes e parâmetros das leis editáveis por
um contrato bounded fora do callback; permanece pendente a escuta A/B humana.

### Pasta da última importação MIDI — 06/11/2026

O seletor **Importar MIDI** agora inicia na pasta do último arquivo MIDI válido
selecionado. O caminho fica salvo como `last_midi_directory` nas preferências
locais do Batruc; se a pasta tiver sido removida ou não houver preferência,
o seletor volta a iniciar em Documentos. O caminho é atualizado após a
validação do MIDI e antes da prévia de aplicação.

Continuação: para remover a duplicidade de ação, **Study** no rodapé agora
apenas abre a vista Study; **Importar MIDI** no Editor é o único botão que
abre o seletor de arquivos.

O campo de BPM também ganhou largura: o slider passou de 93 para 140 px, com
o toque de andamento deslocado para a esquerda na mesma linha. A caixa numérica
de BPM permanece embutida no slider.

Revisão de layout: removi a faixa decorativa e não interativa `MIXER · 8 STEMS`
que aparecia sob o Editor; os 58 px antes reservados a ela voltaram às vistas
principais. Campo livre e demais modos de padrão continuam no seletor fixo do
rodapé; o Study continua disponível no Editor. Limitei algumas barras que
cresciam junto com a janela e aumentei o fader de stem, os knobs do Baixo/Chip
e os rótulos PAN, MOV e VAR para melhorar a proporção e leitura. O atalho
superior voltou a se chamar **Mixer** (com o texto escolhido pelo autor) e só
navega à seção. Build Release passou. Próximo: inspecionar visualmente o
resultado e continuar a execução do wireframe aprovado, que inclui o Editor de
Ciclos circular e uma vista vinculada; os modos atuais não foram removidos.

Arquivo: `src/app/Main.cpp`. Validação pendente: build Release após a alteração.

### Reconciliação documental do backlog — 06/11/2026

Objetivo: recuperar a lista ativa desde a abertura do Batruc em 10/10 e não
confundir o histórico de implementação com trabalho ainda por fazer. Mantive
os registros de passagem e acrescentei no início deste arquivo uma linha do
tempo, prioridades P0–P4 e os portões originais de pré-release, sem criar
funções novas no instrumento.

Conferi os marcos e pendências de `TAREFAS.md`, a auditoria de coesão, trechos
do README, a matriz HTML/JUCE, o estado do código de vozes/timbres e o caderno
web. A lista operacional agora inclui: validação humana e regressões; auditoria
de textos/tutorial; execução comparada ao SVG; modelo documental/células,
operações e proveniência; forma longa; timbres; aceitação de áudio; e gates de
distribuição. Checklists datados antigos são identificados como históricos e
remetem às prioridades atuais.

Achados de texto já confirmados para correção posterior: o **Sobre** JUCE diz
2–12 vozes, mas a seleção ativa do host é 2–8; há rótulos de “doze vozes” e
descrições de doze perfis sonoros que não refletem o limite ativo e o enum
atual com 13 modelos; “12 canais” precisa distinguir slots de compatibilidade
de oito stems musicais. O app contém versões PT/EN/ES/FR, mas a revisão de
gramática, acentos, coerência de termos e completude dos quatro idiomas ainda
não terminou.

Validação documental: busca dos checklists/status, leitura dos marcos inicial,
recente e da auditoria de coesão; conferência do limite `kMaxVoices=12` versus
`kMaxActiveVoices=8` e enum sonoro. Não executei build nem testes nesta
reconciliação. O último relatório de auditoria visual disponível retornou
`captured: true`, zero sobreposições do Mixer em 1080/1440/1920, gráfico I
capturado e round-trip Chip 8-bit. A captura observada ainda mostra o atalho
superior como **Tocar**, embora a fonte atual diga **Mixer**; a compilação e a
inspeção do binário correspondente permanecem pendentes. **Campo livre** segue
presente no seletor fixo do rodapé; confirmar os demais modos após o layout.

Arquivos alterados nesta reconciliação: `TAREFAS.md`. Próximo: terminar a
auditoria dos textos e tutoriais e corrigir as afirmações antigas sem mexer em
funções do app; depois compilar/inspecionar a revisão visual e atualizar o
registro com a versão efetivamente executada.

### Verificação do estado de Learn — 06/11/2026

O código nativo (`src/app/Main.cpp`, `learnTopicContent`, `openLearnPanel` e
`setLearnTopic`) confirma um painel de ajuda/referência: sete artigos fixos,
selecionados por abas e apresentados num painel compartilhado com Sobre. Não
há percurso pedagógico, exercícios, interação didática guiada ou registro de
progresso. Portanto, os guias foram implementados, mas o produto Learn não.
Atualizei o README para explicitar essa distinção e mantive Learn como trabalho
de produto pendente. Não alterei o aplicativo, não compilei e não rodei testes.

Arquivos alterados nesta verificação: `README.md` e `TAREFAS.md`. Próximo passo:
definir com o autor os objetivos e a sequência didática de Learn, em paralelo à
auditoria já pendente dos textos e dos idiomas.

### Auditoria factual de textos e tutoriais — 06–07/11/2026

Verifiquei textos do host em `src/app/Main.cpp`, o README e as instruções do
protótipo/tutorial. Corrigi o Sobre PT/EN/ES/FR para refletir o limite ativo de
2–8 vozes e 13 modelos sonoros; atualizei também o tooltip espanhol de ciclos e
o rótulo de oito stems. O README agora diferencia os 12 slots de compatibilidade
do formato de cena dos oito stems ativos, descreve os 13 modelos, identifica
como histórico o retrato de áudio monofônico de 24/10 e esclarece que o painel
Aprender é ajuda estática, não Learn desenvolvido. A seleção de idioma oferece
quatro opções, mas a localização espanhola é incompleta e usa inglês como
fallback; isso está explicitado no README. Os textos do caderno web mantêm seus
quatro conjuntos de idioma próprios.

Validação executada: buscas direcionadas para limites, contagens, canais e
idiomas nos documentos principais e na interface; build Release de
`batruc_app` concluído em 07/11; `git diff --check` passou. Abri o executável e
inspecionei a tela principal em 1080×800. A janela encerrou antes da inspeção
visual de Sobre/Aprender e da passagem por PT/EN/ES/FR; não reabri outra
instância para evitar interferir no uso do app. A auditoria factual estática
está concluída; revisão linguística visual e de truncamentos em execução
continua pendente. Arquivos tocados nesta etapa: `src/app/Main.cpp`, `README.md`
e `TAREFAS.md`.

Continuidade em 07/11: o executável Release contém as correções textuais. O
próximo passo de auditoria é inspecionar Sobre e os sete tópicos Aprender nos
quatro idiomas em uma janela disponível, registrar strings ainda em fallback e
corrigir clipping/gramática visíveis. Não é necessário compilar novamente sem
mudança de código.

### Investigação de famílias sonoras do Mixer — 07/11/2026

Confirmei no fluxo que as famílias gravam `voiceSoundOverrides_` por stem e o
callback escolhe o override antes do modelo global; o renderer também produz
saídas diferentes para os sete corpos percussivos no teste
`batruc_geometry_curves_test`, que passou. Não encontrei precedência do modelo
global sobre os overrides ativos. Encontrei, porém, um defeito real: escolher
**Original / manual** enviava o índice `-1`, e `applySoundFamily()` retornava
sem limpar os overrides. Além disso, aplicar uma família substituía o modelo
global pelo primeiro timbre da lista. Corrigi ambos: o modo manual agora limpa
os overrides para que o material global volte a valer; as famílias passam a
preservar esse material global.

Também ajustei o tooltip e o README para declarar o alcance atual: famílias são
mapas de timbre, não padrões rítmicos ou articulações. Isso explica por que um
mapa pode não “soar como samba/rock” mesmo quando a seleção de timbres funciona.
Build Release de `batruc_app` concluído após a mudança. O teste de Mixer
`batruc_mixer_routing_test` falhou no caso já conhecido de hard-pan; isso não
testa a atribuição de timbres e permanece no P0. A confirmação auditiva da
família na interface segue pendente, pois não alterei a instância aberta do
autor.

Continuação em 07/11: abri o Release recompilado para verificar a ação na GUI,
mas a janela do processo de teste desapareceu durante a inspeção. Não foi
possível confirmar o reset visualmente nem por escuta. Não há instância do
Batruc aberta pelo teste agora; ao reabrir, usar o Release atualizado e
verificar **Original / manual**, os seletores por stem e depois um A/B de
famílias. Não declarar o comportamento validado até essa checagem.

### Hard-pan: validação após a rampa — 07/11/2026

Investigação da falha em `batruc_mixer_routing_test`: `ResonatorRenderer`
aplica `channelSlew_`, com constante temporal de 5 ms, tanto ao nível quanto ao
pan. O teste anterior avaliava desde o primeiro sample e tratava a transição
suavizada como vazamento permanente. Mantive o ramp livre de clique e alterei o
teste para conferir energia na cauda, depois da transição, exigindo no máximo
1e-5 da energia do canal dominante no lado oposto.

Validação: build Release do alvo `batruc_mixer_routing_test` e execução do teste
passaram. Não alterei o DSP de pan; as três diferenças de paridade de baixo
continuam pendentes. Arquivo alterado nesta correção: `tests/mixer_routing.cpp`.

### Paridade da cicloide — 07/11/2026

O teste focado mostrou que o protótipo JavaScript ainda desenhava uma antiga
forma normalizada de arco e retorno, enquanto o núcleo C++ já usava a roda de
raio `r = 1/(π+1)` rolando sobre uma reta. Alinhei `geometryPoint()` ao modelo
do núcleo e atualizei o rótulo e a fórmula apresentados no seletor/descrição,
com traduções EN/ES/FR.

Validação: `batruc_geometry_curve_parity` passou (162 pontos e 222 vetores
epicíclicos); `node --check` passou nos três arquivos JavaScript alterados.
Também repeti os quatro testes focados: os três casos de baixo continuam
falhando por contagens diferentes de ataques; a curva passou após recompilar o
fixture. Arquivos alterados: `prototype/score.js`, `prototype/app.js`,
`prototype/index.html`, `prototype/interface-i18n.js` e este registro.

### Paridade do baixo: trajeto relacional — 07/11/2026

Ao recompilar `batruc_seeded_bass_fixture`, confirmei que as falhas não eram
apenas artefatos de binário antigo. O core C++ aplica `analyseSeedBassPath()`:
memória entre ciclos, seleção de deslocamento melódico e respirações
determinísticas. Portei esse cálculo para `score.js` e separei a geração da
agenda vocal da emissão do baixo, porque a análise depende dos sete ciclos.
A comparação da primeira nota mostrou outra diferença: a intensidade destinada
ao baixo não inclui a ponderação visual do campo, embora a intensidade audível
da voz a inclua. O protótipo agora mantém `bassSourceIntensity` separada,
preservando a resposta ecológica. Também alinhei arredondamento float, fator de
curvatura, duração cinemática e o clamp da velocidade final.

O total de casos do comparador seeded por voz foi atualizado de 9.168 para
8.592, conforme a matriz atual de 2, 4 e 8 vozes (12 vozes não são suportadas
pelo contrato vigente). O fixture inclui o slot de origem na saída de
diagnóstico.

Validação: build serial de `batruc_seeded_bass_fixture`; os três testes
`batruc_seeded_bass_parity`, `batruc_seeded_voice_bass_parity` e
`batruc_cyclic_bass_parity` passaram (3/3; matriz seeded por voz: 8.592 casos).
`node --check prototype/score.js` e `node prototype/test-score.cjs` passaram.
Em seguida, CTest completo passou 18/18 em 73,73 s. Preflight antes da suíte:
16 GiB disponíveis, swap zerada e 973 GiB no volume do projeto; `/tmp` tinha
7,2 GiB livres (94% ocupado), por isso a execução foi serial e os artefatos de
teste ficaram no volume do projeto.

Arquivos alterados nesta investigação: `prototype/score.js`,
`tests/seeded_bass_fixture.cpp`, `tests/compare_seeded_voice_bass.cjs` e este
registro. A paridade técnica está fechada; permanecem as validações humanas de
escuta e interface descritas no P0.

### Campo livre no rodapé — 07/11/2026

Auditoria estática confirmou que **Campo livre** continua como opção inicial do
`patternBox_`, presente no rodapé fixo, sem condicionais de visibilidade. Orbit
e Study são botões da vista geométrica vinculada; o botão Study fixo leva a essa
vista e a ativa. Havia, porém, outro controle vizinho chamado “CAMPO”, que
mostra/oculta o campo de fases, confundindo dois conceitos. Renomeei-o para
“FASES” (EN/FR “PHASE”) e acrescentei ao seletor de padrão um tooltip que explica
que Campo livre conserva ataques e modula sua força pelo foco, enquanto Ilhas e
Microlinhas filtram por posição.

Validação: build Release do alvo `batruc_app` passou. A inspeção estática
confirmou construtor, visibilidade e bounds. Não foi possível abrir a janela
nesta sessão (`wmctrl`: `Cannot open display`); a legibilidade do rodapé segue
pendente junto com a matriz de larguras e DPI. Arquivo alterado:
`src/app/Main.cpp`.

### Seleção explícita no Study — 07/11/2026

O Study agora mantém a seleção do ataque enquanto a vista está aberta, desenha
um contorno no ponto selecionado e informa no cabeçalho a voz, a posição do
ataque no total e se a célula exibida é manual ou calculada. A seleção acompanha
a inserção e o arraste temporal; ao alternar de voz, remover ou perder um ataque
por atualização dos dados, o índice inválido é limpo. Isto cobre a camada de
seleção/estado da pendência, mas não implementa histórico, limpar/restaurar nem
edição numérica ou por teclado nesta primeira etapa.

Validação: `git diff --check -- src/app/Main.cpp` passou e o build Release serial
de `batruc_app` concluiu. Não rodei testes nem abri a interface; portanto, o
contorno e os rótulos ainda precisam de conferência visual e de ensaio humano
dos gestos. Arquivos alterados nesta etapa: `src/app/Main.cpp` e `TAREFAS.md`.
Continuação na etapa seguinte: quatro ações visíveis — desfazer, refazer,
limpar a voz selecionada e restaurar seus ataques calculados. Limpar cria uma
célula manual vazia (silêncio naquela voz); restaurar desativa a célula manual.
O histórico mantém até 32 estados anteriores, limpa o redo ao surgir uma nova
edição e reinicia ao trocar seed ou carregar cena. Inserção/importação, remoção,
movimento e força passam pelo mesmo registro de snapshots.

Validação: o primeiro build Release acusou avisos de shadowing no layout e
comparação exata de ponto flutuante; renomeei os locais e usei tolerâncias
menores que a resolução musical dos controles. O segundo build serial de
`batruc_app` passou sem avisos; `git diff --check` passou. Não rodei testes nem
abri a interface. Preflight: 15 GiB de memória disponível, swap sem uso, 973
GiB livres no volume do projeto e 7,2 GiB em `/tmp`; nenhum processo pesado
concorrente visível. Build serial com timeout de 180 s. Arquivos afetados:
`src/app/Main.cpp` e `TAREFAS.md`; próximo passo: revisar visualmente os bounds
dos quatro botões e ensaiar as ações na janela quando o ambiente gráfico estiver
disponível.

### Edição de ataques por teclado no Study — 07/11/2026

Ao focar o gráfico com clique, o teclado agora permite deslocar o ataque
selecionado no tempo (←/→; `Shift` grosso, `Ctrl/Cmd` fino e `Alt` muito fino),
alterar a força (↑/↓; `Shift` usa passos menores), escolher primeiro/último
ataque (`Home`/`End`), trocar de voz (`PageUp`/`PageDown`) e remover (`Delete` ou
`Backspace`). `Ctrl/Cmd+Z`, `Ctrl/Cmd+Shift+Z` e `Ctrl/Cmd+Y` chamam o histórico
existente. Editar um ataque calculado primeiro cria sua célula manual, mantendo
a distinção de origem e registrando a operação no mesmo undo/redo. A legenda
curta de teclado foi adicionada abaixo do gráfico.

Validação: `git diff --check` e build Release serial de `batruc_app` passaram
sem avisos. Preflight: 15 GiB disponíveis, swap sem uso, 973 GiB no volume do
projeto, 7,2 GiB em `/tmp`, sem processo pesado concorrente. Não rodei testes
nem abri a GUI; portanto foco, atalhos e legenda requerem ensaio na janela.
Arquivos alterados: `src/app/Main.cpp` e este registro; em seguida, conferir
estes atalhos e os limites dos comandos do Study no ensaio visual/humano.

### Entrada numérica de ataque no Study — 07/11/2026

Com um ataque selecionado, `E` abre um painel temporário sobre o gráfico com
campos numéricos de posição normalizada na seção (`0 ≤ posição < 1`) e força
(`0–1`). Vírgula ou ponto decimal são aceitos. `Enter` ou **Aplicar** confirma;
`Escape` ou **Cancelar** fecha sem alterar o material. A posição precisa
respeitar a ordem temporal e a distância mínima dos ataques vizinhos; entradas
que violem essa restrição são sinalizadas no campo. Se o ataque vier da fonte
calculada, o app constrói sua célula manual e aplica os dois valores numa só
operação de histórico. Em gráficos estreitos, os botões passam para uma segunda
linha; sair do Study fecha o painel.

Validação: `git diff --check` passou e `batruc_app` compilou em Release, em série.
Não rodei testes nem abri a GUI; portanto, não confirmei foco, legibilidade,
limites do painel ou a validação dos campos por interação. Preflight: 15 GiB
disponíveis, swap zerada, 973 GiB no volume do projeto e 7,1 GiB em `/tmp`, sem
processo pesado concorrente. Arquivos alterados: `src/app/Main.cpp` e este
registro. Próximo passo: ensaio visual e funcional do painel e dos atalhos na
interface; só depois disso fechar a consolidação do Study.

### Equações ativas na vista de intensidade — 07/11/2026

A vista de intensidade agora escreve as funções A e B selecionadas como
equações, incluindo os ciclos da seção nas leis de energia potencial/cinética.
Uma terceira linha explicita a cadeia `x,y → A/B → F=(1-b)A+bB →
I_field=(1-λ)+λF`, com os valores atuais de `b` (mistura) e `λ` (profundidade).
O plot foi deslocado para baixo para abrir espaço às equações; sua altura útil
é menor nesse modo. O rótulo distingue `I_field`, que é o multiplicador
desenhado, da intensidade final de evento, que também recebe a articulação da
voz.

Validação: build Release serial de `batruc_app` passou sem avisos e
`git diff --check` passou. Não rodei testes. A interface gráfica continua
inacessível nesta sessão (`DISPLAY=:0`, mas `wmctrl -l` retorna `Cannot open
display`), então não inspecionei truncamentos nem legibilidade. Preflight do
build desta etapa: 15 GiB disponíveis, 2 GiB de swap livre, 973 GiB livres no
volume do projeto, 7,1 GiB em `/tmp` e sem processo pesado concorrente.

Na seleção de um ataque em Study, a linha da equação informa `V# # I_field×n`;
o gráfico marca o ponto correspondente na curva do multiplicador. O tooltip
esclarece em português, inglês e francês que este número é apenas o fator do
campo, não intensidade final, velocity MIDI nem dB. O renderer aplica outros
fatores depois do campo, incluindo articulação da voz, groove e expressão; por
isso não se apresenta esse valor como nível sonoro resultante.

Build Release serial de `batruc_app` passou; `git diff --check -- src/app/Main.cpp
TAREFAS.md` passou antes do build. Não rodei testes nem conferi a interação na
GUI. Tentei iniciar o binário e consultar a janela com acesso gráfico
escalonado: o processo do Batruc não permaneceu ativo e não apareceu na lista de
janelas, portanto não foi possível confirmar que a interface abriu; não infiro
falha de áudio ou crash sem log. Arquivos afetados: `src/app/Main.cpp` e
`TAREFAS.md`. Próximo passo: abrir o Batruc na sessão gráfica do usuário,
validar visual/funcionalmente o marcador em janela larga e estreita, e depois
retomar os coeficientes próprios das leis e a comparação sob nível/duração
constantes.

### Expansão determinística da seed — 07/11/2026

A auditoria inicial levou `Nova seed` a escolher uma família, mas a escuta e o
uso mostraram que isso tirava do músico o controle direto sobre a matéria
sonora. A implementação atual preserva família e caráter sonoro, perfil,
material e nível do baixo, além dos parâmetros Chip 8-bit. Toda seed inicia com
a máscara **Campo livre**; ilhas adesivas e microlinhas passam a ser escolhas
explícitas do músico. A seed continua gerando groove, compasso, intenção,
dinâmica, leis de intensidade, variação, movimento cíclico de V2, fonte e forma
geométricas, transformações, acoplamentos, fases e profundidade de variação.
Reaplicar uma seed, inclusive a atual, restaura seus genes e limpa células Study
editadas; com fonte MIDI, recalcula as células pela fonte preservada. Regras
locais voltam aos genes. BPM, swing, níveis, panorama, mute/solo e demais
controles de execução/mixagem permanecem manuais. A família é salva no schema
43 e a proveniência MIDI no 44; cenas 1–42 continuam aceitas.
cenas 1–42 continuam aceitas.

Validação: `batruc_app` e `batruc_seeded_recipe_test` compilaram em Release,
em série; `ctest -R '^batruc_seeded_recipe$'` passou (1/1) e `git diff --check`
passou. Não abri a GUI nesta etapa, então falta confirmar escuta comparativa
entre famílias, repetição da mesma seed depois de outra seed e retorno visual da
família ao abrir cena schema 43. Preflight: 14 GiB disponíveis, 2 GiB de swap
livre, 973 GiB no volume do projeto e 7,1 GiB em `/tmp`, sem processo pesado
concorrente. Arquivos afetados: `src/core/CompositionCore.hpp`,
`src/core/CompositionCore.cpp`, `src/app/Main.cpp`, `tests/seeded_recipe.cpp`,
`README.md` e este registro. Próximo passo: fazer o ensaio musical/GUI das
dimensões geradas e depois acrescentar travas dimensionais para o autor limitar
quais objetos a seed pode alterar.

### Fonte MIDI e variação pela seed — 07/11/2026

O fluxo anterior apagava as células importadas quando o músico aplicava uma
seed. Na primeira implementação, a importação passou a conservar suas oito
células como fonte em memória; cada seed derivava delas uma mudança de fase
determinística por lane (até ±1/8 de ciclo), dimensionada pela variação da
receita. Em 08/11, isso foi substituído pela variação de acentos descrita na
continuidade atual abaixo. Lanes sem ataques continuam
ativas para que fórmula alguma preencha silenciosamente esses stems. As cenas
schema 44 guardavam nome, caminho, recorte em compassos e células-fonte; schema
45 acrescenta o SHA-256 do arquivo e o estado de direitos (não verificado por
padrão), mantendo compatibilidade de leitura com schemas 1–44. A fonte MIDI
não é editada pelo seed. Edições Study feitas após a importação são substituídas ao reaplicar uma
seed, pois a variação é sempre refeita desde a fonte original.

Validação: `batruc_app` e `batruc_midi_rhythm_import_test` compilaram em Release
serial; `ctest -R '^batruc_midi_rhythm_import$'` passou (1/1), cobrindo
reprodutibilidade, contraste entre seeds, preservação da fonte, células vazias
ativas e validade/contagem dos ataques. `git diff --check` passou. O preflight
mostrou 15 GiB disponíveis, swap sem uso, 973 GiB livres no volume do projeto e
7,1 GiB em `/tmp`, sem processo pesado concorrente. Não validei visualmente nem
por escuta na GUI. Próximo passo: abrir uma cena MIDI, alternar/reaplicar seeds,
salvar e reabrir para conferir a fonte e escutar se o deslocamento limitado é
musicalmente útil; depois ampliar as operações Study para recombinação,
interlocking e densidade e completar o manifesto de proveniência/licença.

### Identificação da fonte no manifesto MIDI — 07/11/2026

A importação calcula SHA-256 dos bytes já carregados (limite existente: 16 MiB)
e a cena schema 45 conserva esse identificador junto ao nome, caminho, recorte e
células-fonte. O campo `rights_status` começa como `unverified`: o Batruc não
deduz licença ou atribuição pelo nome do arquivo nem declara que o checksum
comprove direitos. Cenas schema 44 continuam legíveis; como não continham hash,
ao salvá-las novamente o campo permanece vazio e os direitos não verificados.
O manifesto ainda precisa registrar versão/identificador do corpus, licença e
atribuição verificadas e histórico detalhado das operações Study.

A prévia MIDI também recebeu campos opcionais para licença e atribuição
declaradas pelo usuário. Esses valores são guardados em schema 46, mas não
alteram `rights_status`: continuam sem verificação independente.

Validação: build Release serial de `batruc_app` concluiu com sucesso após a
inclusão do checksum e dos campos de metadados. O CMake regenerou o alvo com
`juce_cryptography`. Não rodei testes nem abri a GUI. Preflight antes do build:
15 GiB disponíveis, swap zerada, 973 GiB livres no volume do projeto e 6,9 GiB
em `/tmp`; após o build: 15 GiB disponíveis e 6,9 GiB em `/tmp`. Arquivos afetados:
`src/app/Main.cpp`, `CMakeLists.txt`,
`README.md`, `TAREFAS.md` e `midis/README.md`. Próximo passo: compilar novamente e
validar na GUI a prévia com metadados e o round-trip schema 46; depois completar
identificador de derivado e histórico das operações Study sem transformar uma
declaração em autorização presumida.

### Versão da fonte e histórico básico de operações — 07/11/2026

A prévia da importação agora aceita a identificação declarada de fonte/corpus e
versão. Cenas schema 47 preservam esse campo e um histórico limitado às 64
operações MIDI mais recentes. A lista registra `midi_import`, a variação seeded
com sua seed, as categorias de edição Study com o stem, além de desfazer/refazer.
Ao importar outra fonte, o histórico anterior é reiniciado. Cenas schema 44–46
continuam legíveis e iniciam sem esses dados; a fonte, hash e células existentes
permanecem compatíveis.

Esse registro identifica a sequência e a categoria das ações; ainda não guarda
um snapshot de parâmetros/resultados para cada operação nem os estados antes e
depois. As edições continuam representadas pelo estado ativo da célula na cena,
e a fonte importada fica separada. A licença/atribuição digitadas seguem
declaradas e não verificadas.

Validação: build Release serial de `batruc_app` passou; `git diff --check` passou.
Não rodei testes nem abri a GUI. Preflight: 14 GiB disponíveis, swap zerada,
973 GiB livres no volume do projeto e 6,9 GiB em `/tmp`; após a compilação:
15 GiB disponíveis e 6,9 GiB em `/tmp`. Arquivos afetados: `src/app/Main.cpp`,
`README.md`, `TAREFAS.md` e `midis/README.md`. Próximo passo: validar a prévia e o
round-trip de cenas schema 47 na GUI; depois salvar parâmetros/resultados das
operações Study e avançar para as transformações de recombinação, interlocking,
densidade e microtiming.

### Confirmação do binário Release — 07/11/2026

Objetivo: confirmar que há um binário Release local correspondente ao código
atual antes da inspeção visual. Preflight: 15 GiB de memória disponível, swap
zerada, 973 GiB livres no volume de trabalho, 6,9 GiB em `/tmp`; sem processo
pesado concorrente. Comando: `cmake --build
.local-build/native/linux-release-juce --target batruc_app --parallel 1`.
Resultado: CMake confirmou `batruc_app` atualizado, sem recompilar unidades;
`src/app/Main.cpp` (07/11 23:13:23 +0100) é anterior ao executável Release
(07/11 23:15:29 +0100). Executável:
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
16.894.760 bytes, SHA-256
`9ac6b463993c8b691015b6ddc0b5f36274c5e4ada01800d221e4f45dfa40e473`.
`git diff --check` passou. Não executei testes nem abri o app; portanto, isto
confirma apenas o estado do build, não a presença funcional das mudanças na GUI,
o round-trip MIDI/schema 47 ou a aceitação musical. Próximo passo: abrir este
binário identificado, inspecionar controles e cenas MIDI e registrar resultados;
depois fazer o ensaio de escuta.

### Abertura da GUI Release — 07/11/2026

Abri o executável acima na janela 1080×800 e confirmei visualmente a tela do
Editor de Ciclos, os controles de navegação e o botão Importar MIDI. A captura
local de observação ficou em `/tmp/batruc-publication-gui.png`; ela mostra a
interface aberta, sem comprovar comportamento dos controles. O processo está
mantido aberto para inspeção. Não consegui automatizar os cliques: o app não
aparece na árvore AT-SPI do desktop e `xdotool` não está instalado. Não importei
MIDI, não salvei/reabri cena, não executei testes e não fiz escuta. Assim, o
round-trip schema 47 e a aceitação funcional/musical permanecem pendentes.
Próximo passo: interação manual no app aberto para importar um MIDI local
autorizado, salvar/reabrir a cena e conferir metadados; em seguida, escutar o
resultado e registrar as observações.

## Correção da leitura GM e continuidade de timbres — 08/11/2026

O mapa GM em `src/core/MidiRhythmImport.cpp` foi corrigido: notas 42/44 são
chimbal fechado/pedal, 46 é chimbal aberto; notas de efeitos 22/26 deixam de
ser classificadas como chimbal. A família sonora agora permanece em
**Original / manual** ao importar MIDI e ao variar a fase por seed, preservando
as atribuições de papel aos stems. Build `batruc_app` Release passou serialmente
em 08/11; `git diff --check` passou. Não foram executados testes nem houve
escuta manual nesta passagem.

A captura da GUI após aumentar o mínimo de fontes confirmou melhora nos rótulos
e controles principais, mas ainda há elementos secundários do gráfico pequenos;
a janela disponível na captura também era reduzida. O ajuste visual fica
pendente de uma matriz legível sem colisões em tamanhos/DPI distintos.

**Limitação sonora central registrada naquele momento:** embora MIDI forneça
eventos (nota, tempo e velocity), não traz o áudio/sample da bateria, e a nota
GM ainda se perdia no modelo editável do Batruc. Essa perda foi tratada na
continuidade de 08/11 descrita abaixo; reprodução sample-based, fallback
explícito e licenças verificadas continuam pendentes. O SoundFont local
encontrado no sistema não foi incorporado nem escolhido como dependência.

## Nota GM por ataque e função dos stems — 08/11/2026

O modelo `RhythmicCell` agora mantém a nota GM alinhada a posição e velocity;
inserir/remover ataques mantém os arrays juntos, a variação de seed ajusta
acentos sem mover a nota de seu ataque e undo/redo inclui a identidade no
snapshot. `MusicalEvent` transporta a
nota ao renderer e à partitura JSON de gravação. A cena foi avançada para schema
48: salva a nota tanto na célula editável quanto na fonte MIDI imutável, e o
leitor aceita cenas antigas 1–47 preenchendo o campo ausente como “sem nota
MIDI”.

O renderer usa a nota para variar a articulação sintetizada dentro do papel do
stem, incluindo durações distintas de chimbal fechado, pedal e aberto e de
crash/splash/china versus ride. Uma seleção de timbre diferente do modelo
convencional do stem continua prevalecendo. Os tooltips do Mixer agora informam
as funções convencionais: V1 ancoragem grave, V2 contratempo/resposta, V3
subdivisão, V4 abertura/transição, V5 movimento/viradas, V6 acentos/marcadores,
V7 contrapadrão e V8 auxiliar editável. Essas descrições orientam a leitura e
não impõem comportamento rítmico fixo.

Esta é articulação procedural, não reprodução da bateria gravada no MIDI. A
identidade da nota já chega ao renderer e à partitura, mas timbres naturais,
articulação por sample e round-trip real da cena schema 48 ainda precisam de
escuta e ensaio na GUI. Não executei testes automatizados nesta etapa.

Validação executada: build Release serial de `batruc_app` passou após as
alterações e `git diff --check` passou. Binário em
`.local-build/native/linux-release-juce/batruc_app_artefacts/Release/Batruc`,
16.895.048 bytes, SHA-256
`67d3d3a3ea8145a3b299802104a5cb685dfa58976c6ab2ddf663a63b83a2f3a7`.
Arquivos de implementação/documentação desta continuação: `src/core/CompositionCore.hpp`,
`src/core/CompositionCore.cpp`, `src/core/MidiRhythmImport.cpp`,
`src/app/Main.cpp`, `README.md` e `TAREFAS.md`. Próximo passo: ensaiar importação,
seed, edição, undo/redo, gravação e round-trip de uma cena com MIDI autorizado;
escutar cada articulação e ajustar os modelos segundo o resultado. A integração
de samples só deve começar após escolher fonte com direitos compatíveis e
dependência apropriada.

## Seed preserva o groove MIDI — 08/11/2026

A variação anterior deslocava cada lane MIDI independentemente em até ±1/8 do
ciclo. Isso podia separar o bumbo, a caixa e o chimbal que formam juntos o
groove de origem. Foi substituída por uma variação determinística de intensidade
por ataque, escalada pela profundidade da receita (fator máximo 0,52), com peso
menor para bumbo/caixa e maior para subdivisão e percussões de apoio. A seed conserva posições, relações entre os
stems, velocity original como base e nota GM. O `midi_source` permanece imutável;
aplicar outra seed recompõe os acentos desde ele. Mudanças de posição e densidade
passam a pertencer às operações explícitas de Study, não a um efeito colateral
da seed.

A entrada de histórico agora identifica essa operação como
`seeded_lane_dynamics_v1`. Build Release serial de `batruc_app` passou em
08/11/2026 e `git diff --check` passou. O binário atualizado tem 16.895.048
bytes e SHA-256
`62ac164f1280c98ce7f14817e05bbec015167ae09279537f7c36ffeb8709a51f`.
Não foram executados testes automatizados nem escuta manual. Próximo passo:
abrir uma cena MIDI e comparar a seed em 0% e em variação alta, confirmando que
as posições e notas continuam iguais e que os acentos variam sem desmontar o
groove; depois ouvir as articulações e prosseguir à seleção de samples
compatíveis com a licença do produto.

## Referência de combinatória rítmica no Learn — 08/11/2026

Foi acrescentada uma oitava aba estática, **Contagens / Patterns / Comptes**,
com a regra 2ⁿ, decomposição dos 16 padrões de quatro posições por quantidade
de ataques, exemplos 5/6/7/12 e compasso de 16 posições (65.536 padrões
binários). O texto explica que mãos, acentos e ghost notes aumentam o espaço de
estados conforme as opções permitidas. A lição não reproduz literalmente o
roteiro de referência e declara o limite atual: é conteúdo matemático, não o
explorador/treino que ele descreve.

Pendência de implementação: grade navegável por página para enumeração total,
seleção/prática de padrão e sorteio de compasso isolado da composição ativa.
Build Release serial de `batruc_app` passou em 08/11/2026 e `git diff --check`
passou. O binário ficou com 16.899.408 bytes, SHA-256
`c336c7eaeebb960f7f77cb6808cea6b102fb5ecea15c5e5ef8ea39115e975cf2`.
Não executei testes automatizados nem abri a GUI nesta etapa.

## Correção de timbre e papel dos stems — 08/11/2026

Foi encontrada a causa do bumbo sem grave: `refreshVoiceSpectrum()` calculava
uma escala para ancorar a fundamental percussiva, mas os `switch` seguintes
substituíam a razão já escalada pelos intervalos modais originais. A escala
agora é aplicada depois da seleção do espectro: bumbo em 55 Hz, caixa em 185 Hz,
chimbal/pratos e percussões de mão em suas faixas próprias, independentes da
afinação matemática do stem.

Seeds aplicadas a MIDI mantêm as funções GM de cada lane. Antes, isso também
forçava a família sonora para o neutro, deixando todas as seeds com a mesma
matéria. A família agora aplica, sem remapear bumbo/caixa/chimbal, perfis
distintos de cauda, ruído e inclinação espectral por stem. O seletor Global e os
overrides individuais continuam disponíveis. Tooltips/README foram alinhados
com o novo comportamento e explicitam que são sínteses estilizadas, não kits
sampleados nem imitações fiéis.

Acrescentado ao `batruc_seeded_recipe_test` um ensaio com o mesmo stem de bumbo
e a mesma célula sob perfis Rock/Samba; a saída precisa ser diferente. Build
Release do app e desse teste passou, e o teste passou. A escuta comparativa do
autor ainda é necessária para ajustar o caráter e o balanço entre estilos; não
declarei autenticidade estilística.
Na continuação, o mapa de família foi movido para uma única função do core,
usada pelo Mixer e pelo renderer offline. O exportador antes ignorava a família
da seed e fixava todas as vozes em triângulo; agora aplica a paleta e grava o
índice no manifesto JSON. Os renders de `5EED2027` (família Folk) e `A1B2C3D4`
(Eletrônica) foram concluídos em PCM16/48 kHz e seus manifestos indicaram
índices diferentes. A primeira comparação expôs um problema que também estava
nos mapas do Mixer: Folk atribuía Membrana ao V1. As 17 famílias agora usam a
mesma ordem estável — V1 bumbo, V2 caixa, V3/V4 chimbais, V5 tons, V6 pratos,
V7 percussão de mão, V8 apoio — e variam as cores por perfil. O teste confirma
essa invariância e que Rock/Samba geram saídas diferentes para o mesmo bumbo.
Build Release de app e renderer offline passou; `batruc_seeded_recipe` passou;
`git diff --check` passou. Os WAVs de comparação foram temporários e removidos.
Binário Release: SHA-256 `0c6b7909626e0cb6c77c5aa4dd891b7ae64530048cefa774f4f6f5e56ddf824a`.

## Separação auditável dos 17 perfis — 08/11/2026

O `batruc_seeded_recipe_test` agora renderiza o mesmo bumbo e a mesma célula
nos 17 perfis e compara as 136 combinações par a par. O teste passou: nenhum
par gera a mesma saída. Isso verifica que cada perfil chega ao áudio, mas não
substitui avaliação perceptiva nem afirma que a diferença basta para reconhecer
um gênero. O teste foi recompilado e executado; o binário do app não mudou nesta
etapa. Próximo passo: escuta A/B pelo autor e decidir se a síntese modal pode
ser aprofundada ou se a camada de percussão precisa de outra engine ou de
samples com direitos compatíveis.

## Retomada de timbres por canal — 08/11/2026

O autor ouviu a Release e relatou que mudar a família quase não muda o som e
que outros stems parecem invertidos. A inspeção confirmou duas causas: as
famílias variavam sobretudo parâmetros discretos de cauda/espectro; e a nota
GM de um ataque era reconhecida, mas `renderVoice()` continuava usando o
timbre fixo da lane. Foram ampliadas as diferenças de ataque, corpo, duração
do ruído e inclinação espectral. O ataque MIDI agora seleciona o material de
síntese correspondente à nota dentro do mesmo stem. Membrana/tom recebeu
fundamental grave ancorada. Não há sample acústico nem garantia de que um
perfil chamado Samba, Rock ou Jazz represente esse estilo de forma fiel.

Indicação por canal para MIDI GM, a partir do [mapa GM de percussão](https://musescore.org/sites/musescore.org/files/2024-10/General%20MIDI%20Standard%20Percussion%20Set%20Key%20Map.pdf)
e das funções de pista do importador local:

| Stem | Papel sugerido | Notas MIDI GM | Material atual | Requisito sonoro |
| --- | --- | --- | --- | --- |
| V1 | bumbo | 35–36 | kick | grave curto, ataque definido |
| V2 | caixa e aro | 37–40 | snare com esteira sintética; aro seco por nota | escutar e ajustar a relação pele/esteira; confirmar clique do aro |
| V3 | chimbal fechado/pedal | 42, 44 | hi-hat | metal curto |
| V4 | chimbal aberto | 46 | hi-hat | metal de V3 com cauda maior |
| V5 | tons | 41, 43, 45, 47, 48, 50 | membrana afinada por nota | confirmar alturas por escuta |
| V6 | pratos | 49, 51–53, 55, 57, 59 | crash/ride/bell com corpo e ataque distintos | modelos próprios por peça ainda pendentes |
| V7 | tambores de mão | 60–66 | bongo/conga/timbale por nota | confirmar identidade por escuta |
| V8 | acessórios e demais notas | 54, 56, 58, 67–77 e demais | sino, ruído, membrana etc. por nota | fontes próprias e escuta ainda pendentes |

Essa é uma ordem funcional para **lanes internas**, não uma regra universal de
que a pista 1 de qualquer MIDI seja bumbo. Em GM, a identidade instrumental
vem do **número da nota**, normalmente no canal MIDI 10. Em MIDI sem mapa GM
documentado, a atribuição automática é incerta e deve continuar editável.
Próxima validação: ouvir V1–V8 isolados em MIDI Rock e Samba, confirmar
V3/V4 e V5/V6 e medir se a troca de família é perceptível com ritmo fixo.

### Controle de máscara e matéria sonora pela pessoa musicista — 08/11/2026

As seeds agora começam com a máscara **Campo livre**: não filtram ataques por
Ilhas adesivas ou Microlinhas diagonais. A pessoa musicista pode escolher
explicitamente essas máscaras depois de gerar o ritmo. A seed também deixou de
alterar família/caráter sonoro, perfil e material do Baixo e parâmetros Chip
8-bit; essas escolhas permanecem manuais. Se o Baixo estava sem nível, a seed
define um nível audível de 0,32, sem sortear novo nível a cada seed. As
animações de fase no Mixer foram ampliadas moderadamente (raio 13,5→16 px, com
marcadores e ponteiro proporcionais) e deslocadas 2 px para cima para caber na
faixa entre o título e os controles dos stems.

Arquivos alterados nesta etapa: `src/app/Main.cpp`, `README.md` e `TAREFAS.md`.
Validação: build Release de `batruc_app` completado com `--parallel 1` em
08/11/2026; `git diff --check` passou. A conferência visual no app em execução
e a validação auditiva continuam pendentes.

### Redistribuição do Baixo e auditoria de sobreposições — 08/11/2026

Após conferir as capturas do autor, a revisão encontrou texto do LFO sem altura
vertical suficiente, rótulos de rota e valores competindo no gráfico de memória
F+C, e uma leitura de nota do LFO longa demais. Dei ao LFO uma faixa de 72 px,
reorganizei abaixo dele o gráfico de percurso (64 px) e os controles de memória,
e ampliei a altura mínima da página Baixo para 680 px. O gráfico F+C mantém as
curvas sem rótulos numéricos por cima; a leitura curta das sete alturas fica ao
lado e a equação completa permanece na dica do controle. O piano roll identifica
somente o passo selecionado, reduzindo texto em cima das notas vizinhas. A
descrição da vista vinculada e o resumo de estado da seed foram encurtados para
caber em telas menores sem cortar palavras.

A auditoria também revelou que `0.8f` serializava como `0.8000000119`, acima do
limite decimal estrito de cena. O parser agora aceita a margem de representação
e normaliza o valor para 0,8, permitindo validar a ida e volta da cena.

Validação: build Release de `batruc_app`; auditoria de layout em 1080×800,
1440×900 e 1920×1080 capturou todas as páginas; contagem de sobreposições de
controles do Mixer: zero em V1, V2 e Chip 8-bit nos três tamanhos; round-trip
Chip 8-bit passou; `git diff --check` passou. Capturas e relatório de diagnóstico
temporários em `/tmp/batruc-layout-overlap-final-v2-20261108/`. A leitura
visual foi conferida também no painel LFO/F+C; a audição continua pendente.

Validação desta etapa: build Release serial de `batruc_app` e
`batruc_seeded_recipe_test` passou; `ctest -R '^batruc_seeded_recipe$'` passou;
`git diff --check` passou. O teste adicional compara nota 37 (aro) e 38
(caixa) no mesmo V2 quando a fase formal admite esse stem. O binário Release
foi gerado em 08/11/2026 às 16:10 CET, SHA-256
`66837311535ae35a63f6f39a8c14a18310a437b4c2508b601d7920b32483e86f`.
A instância aberta antes desse build permanece antiga e precisa ser reaberta
para escuta. Descoberta adicional: C1 reduz a forma a V1, mesmo quando o
arranjo MIDI contém oito lanes. Avaliar se MIDI importado deve preservar todos
os stems desde C1 ou se essa redução deve ser uma opção explícita. A escuta
humana e a identificação estilística permanecem pendentes.
`git diff --check` passou. Não rodei testes nem fiz escuta nesta retomada.

### Mixer: controles próprios em cada stem — 10/11/2026

Objetivo: eliminar a caixa única que mostrava apenas V1 ao abrir o Mixer.
Redistribuí fórmula, timbre, regra de evento, acoplamento, fase da fórmula,
ciclos, fase dos ataques e variação nas colunas V1–V8. Cada coluna passa a
exibir e editar seus próprios valores simultaneamente. Os quatro parâmetros
do timbre 8-bit passaram a existir também por coluna, visíveis quando aquela
voz usa o timbre. O Mixer ficou mais alto e requer rolagem vertical nas telas
menores; manter os controles legíveis teve prioridade sobre comprimi-los.

O acoplamento foi verificado em `tests/seeded_recipe.cpp` com dois renderizadores
de receita e calendário idênticos: alterar somente λ em V2 produz áudio
distinto. A ação é uma mistura de curvas de intensidade entre a voz e sua
antecessora; não muda os instantes dos ataques. V1 permanece em λ=0 porque
não há voz anterior. Arquivos desta etapa: `src/app/Main.cpp`,
`tests/seeded_recipe.cpp`, `README.md` e este registro. Validação final de
compilação e layout: build Release de `batruc_app` concluído, teste
`batruc_seeded_recipe` aprovado e auditoria de 1080×800, 1440×900 e
1920×1080 com zero interseções de controles nos estados V1, V2 e 8-bit.
Relatório em `/tmp/batruc-mixer-columns-release-20261110/audit-result.json`.
A captura dos parâmetros 8-bit foi feita antes do ajuste final de visibilidade;
essa área requer revisão visual na próxima sessão de uso.

Posição inicial da janela: o autor esclareceu que “ecrã principal” significa
**monitor principal**. Após criar a janela nativa, o app consulta o monitor
principal do sistema e centraliza nele a janela inicial; Compor continua sendo
a página inicial. O ambiente atual marca HDMI-0 (1920×1080, origem x=1368)
como principal. Build Release passou, mas não executei novamente o app na
sessão gráfica após essa correção: as execuções de auditoria anteriores tomaram
o foco do terminal enquanto o autor escrevia. A posição real deve ser
confirmada na próxima abertura feita pelo autor, sem teste invasivo meu.

### Acesso aos controles inferiores do Mixer — 10/11/2026

Revisão estática após a redistribuição revelou que o Mixer ficou mais alto,
mas a roda do viewport ainda era ignorada: os controles inferiores não podiam
ser alcançados pelo músico. Agora a roda percorre apenas a página ativa
(Mixer ou Baixo), limitada ao início e fim da seção; não troca de página.
Um aviso no topo do Mixer indica que há mais ajustes abaixo. Também corrigi
o diagnóstico `initial_kit_balance`: após a seed inicial, ele compara os
timbres com a família efetivamente escolhida, em vez de exigir a família fixa
5. A revisão identificou que os botões de navegação mudam a posição do viewport
sem ativar os antigos estados de painel modal; a rolagem usa agora a seção
realmente visível, não esses estados. As alterações estão em `src/app/Main.cpp`
e `README.md`. Build Release e `git diff --check` passaram; o teste
`batruc_seeded_recipe` também passou. Não abri janela de auditoria para não
roubar o foco do terminal do autor. Conferir a interação da roda e a posição
no monitor principal na próxima abertura normal do app.

Confirmação de janela após abertura pelo autor em 10/11/2026: consulta
somente de leitura à árvore X11 encontrou a janela `Batruc` dentro da região
do HDMI-0 principal (origem x=1368, 1920×1080), ocupando a área útil desse
monitor. Não houve clique, troca de foco ou nova execução do app. A interação
da roda do Mixer segue sem teste manual registrado.

### Canal do baixo no Mixer e alinhamento das colunas — 10/11/2026

O autor observou que o Mixer ainda não mostrava o baixo e que as cores das
colunas atravessavam objetos. A pintura dos fundos V1–V8 usava largura
`(janela−56)/8`, enquanto os controles usavam `(janela−104)/8`; a diferença
acumulava até V8. Os fundos agora seguem a mesma grade dos controles.

Adicionei uma faixa própria de Baixo no alto do Mixer, antes das colunas de
percussão, com estado ligado, nível, mute, solo, panorama e medidor RMS. Nível,
mute, solo e ligado compartilham o estado da página Baixo. O panorama geral
desloca o panorama de cada nota no renderer e é salvo na cena schema 50;
cenas anteriores carregam panorama zero. A faixa não reduz a largura das oito
colunas. A página do Mixer cresceu 82 px e usa a rolagem interna já prevista.

Arquivos: `src/app/Main.cpp`, `src/core/BassAudio.hpp`,
`src/core/BassAudio.cpp`, `tests/bass_audio_response.cpp`, `README.md` e este
registro. Validação de compilação e áudio será anotada após o build. Não há
captura visual nesta passagem para não interromper o uso do app pelo autor.

Validação: build Release serial de `batruc_app` e
`batruc_bass_audio_response_test` concluído; teste de resposta do baixo
passou, incluindo panorama totalmente à direita sem vazamento no canal
esquerdo; `git diff --check` passou. O app que já estava aberto antes desse
build ainda executa o binário anterior. A revisão visual da nova faixa de
baixo e das cores corrigidas fica para a próxima abertura normal do app.

### Articulações dos pratos MIDI no V6 — 10/11/2026

O mapeamento GM já enviava crash, china, splash, ride e bell ao V6, mas quase
todos compartilhavam o mesmo corpo modal. O renderer agora usa excitação mais
aberta e cauda maior para crash/china/splash, mantém o ride com ataque mais
contido e dá ao bell uma componente modal mais presente e aguda. As notas MIDI
continuam determinando a duração e a identidade da peça. Arquivo:
`src/core/CompositionCore.cpp`. A escuta A/B por nota ainda depende da próxima
sessão de audição; nenhuma janela do app foi aberta nesta etapa.

### Preservação dos stems MIDI na partitura exportada — 10/11/2026

Na pendência de C1, o áudio MIDI já mantinha as oito lanes e a geração do baixo
já considerava todas elas. Encontrei uma divergência na exportação da partitura:
o loop ainda descartava V2–V8 segundo a redução formal, inclusive quando havia
fonte MIDI. Agora usa a contagem integral de vozes para cenas com MIDI. A vista
vinculada recebe a contagem ativa de oito vozes; a confirmação auditiva dos
stems isolados em C1/C2 continua pendente. Arquivo: `src/app/Main.cpp`.
Build Release serial concluído; `git diff --check` sem erros. Não abri o app
durante esta etapa.

### Proporção dos controles globais do Mixer — 10/11/2026

Na largura de 1920 px, Fundamental e Trim de vozes se estendiam por quase toda
a área restante da faixa superior, desproporcionais aos demais controles.
Limitei Fundamental a 260 px e Trim a 300 px; em 1080 px, ambos preservam o
tamanho anterior. Acima de 1450 px, os controles da primeira faixa ocupam
quatro células distribuídas pela largura, sem deixar todo o espaço livre à
direita. Arquivo: `src/app/Main.cpp`. Build Release serial concluído e
`git diff --check` passou. A revisão visual na instância nova ainda falta,
pois esta passagem não abriu o app na sessão do autor.

### Articulações GM dos acessórios — 10/11/2026

Objetivo: reduzir a uniformidade dos instrumentos reunidos nas lanes de
percussão e apoio. Em `src/core/CompositionCore.cpp`, as notas GM 60–77
receberam durações específicas; as notas de percussão afinada 60–68 e 75–77
também recebem razões de altura distintas. As notas 54, 69, 70, 58, 73 e 74
passam a ter duração de ruído própria no ataque. A identidade da nota MIDI
continua preservada na célula e no evento; sementes sem MIDI seguem o timbre
escolhido para cada stem.

Validação: build Release serial de `batruc_app` concluído. A escuta em solo
de V7/V8 e a comparação entre fontes MIDI continuam pendentes; não abri uma
janela nem alterei o foco do app ou do terminal do autor. Próximo passo:
avaliar por áudio quais desses acessórios ainda exigem um modelo ou sample
licenciado próprio, antes de afirmar correspondência instrumental.

### Medidores de sinal por stem — 10/11/2026

Objetivo: mostrar no Mixer se cada voz efetivamente produz áudio. O renderer
agora acumula o RMS de V1–V8 após ganho e fader, antes do panorama e do master,
publica valores atômicos para a interface e os zera ao reiniciar. A pintura de
cada coluna ganhou uma barra no cabeçalho existente, sem acrescentar altura
nem apertar os controles. O baixo mantém seu medidor próprio. Arquivos:
`src/core/CompositionCore.hpp`, `src/core/CompositionCore.cpp`,
`src/app/Main.cpp`, `tests/seeded_recipe.cpp` e este registro.

O teste de integração verifica sinal em duas vozes e zeragem no reset. Aguardei
o término da compilação de outro projeto RASGO antes de iniciar este build.
Validação: build Release serial de `batruc_app` e
`batruc_seeded_recipe_test` concluído; o teste passou; `git diff --check`
passou. Binário Batruc 0.1.0 gerado em 10/11/2026 às 21:03 CET, SHA-256
`eb9d0e5d39080df30a8c1fc0a56e42a868c0ec2997fa2e8ed75edd6f0396a275`.
Não abri a interface nem troquei o foco do terminal. Próximo passo: conferir
as barras visualmente na abertura normal do Batruc pelo autor.

### Separação MIDI de tambores de mão e acessórios — 10/11/2026

Objetivo: evitar que o V7 reúna peças de funções diferentes e mescle um
tambor de mão com um acessório tocados no mesmo instante. O importador GM
agora envia bongo/conga/timbale (notas 60–66) ao V7 e os acessórios 54/56/58,
67–77 e notas restantes ao V8. No mapa Roland TD-11, a nota 58 continua no
V5 como aro do tom grave. A identidade da nota MIDI permanece na célula para
selecionar a articulação sonora no renderer.

Arquivos: `src/core/MidiRhythmImport.cpp`, `tests/midi_rhythm_import.cpp`,
`midis/README.md`, `README.md` e este registro. Build Release serial de
`batruc_app` e `batruc_midi_rhythm_import_test` concluído; teste passou com
dois ataques simultâneos separados em V7/V8; `git diff --check` passou.
Binário Batruc 0.1.0 gerado em 10/11/2026 às 21:26 CET, SHA-256
`9f47e04129d9b53fce579f6c0b2100e2e3fecb0da22ecaf337aabbb6e7705118`.
Limite daquele estado: dois acessórios simultâneos no V8 ainda podiam ser
mesclados. A implementação foi concluída na atualização “Polifonia de notas
MIDI no mesmo stem”, no fim deste documento. A escuta autoral de V7/V8 isolados
continua pendente.

### Colisões MIDI visíveis na importação — 10/11/2026

Objetivo: impedir que a perda de uma nota simultânea dentro do mesmo stem
passe despercebida. A célula rítmica atual admite uma nota por instante e o
renderer mantém um estado sonoro por stem; simplesmente guardar uma segunda
nota sem ampliar a síntese declararia reprodução que não acontece. O
importador agora distingue duplicatas da **mesma** nota (`mergedHits`) de
notas **diferentes** que disputam o mesmo instante (`collidingHits`). Continua
conservando a nota de maior velocity, mas o resumo da importação antepõe
`PERDA MIDI N` quando há colisões, com explicação no tooltip. O traço do
renderizador offline também registra a contagem.

Arquivos: `src/core/MidiRhythmImport.hpp`, `src/core/MidiRhythmImport.cpp`,
`src/app/Main.cpp`, `src/tools/midi_reference_render.cpp`,
`tests/midi_rhythm_import.cpp`, `midis/README.md` e este registro. Teste com
conga e agogô simultâneos em lanes distintas, seguido de duas notas diferentes
e uma duplicata no V8, passou. Build Release serial de `batruc_app`,
`batruc_midi_rhythm_import_test` e `batruc_midi_reference_render` concluído.
`git diff --check` passou. Binário Batruc 0.1.0 gerado em 10/11/2026 às
21:42 CET, SHA-256
`9945a309ba90c00a53d453a1b3cf7fe32c43deaf052fcfcb8c82c674cd3720d4`.
Não abri o app. Próximo passo técnico: projetar célula polifônica e estados
sonoros independentes para preservar dois acessórios no mesmo instante,
incluindo persistência de cena e edição Study; não deslocar ataques no tempo.

### Recorte GMD samba e capacidade das células — 10/11/2026

Objetivo: cumprir o primeiro ensaio P0 com fonte, janela, notas, agenda e
áudio, e verificar perdas reais na importação. Escolhi os dois primeiros
compassos do arquivo GMD v1.0.0
`drummer5/session1/3_latin-brazilian-samba_89_beat_4-4.mid`, 4/4 a 89 BPM,
SHA-256 `8031a990b4bc02b242fbfde370ff75c462e3b6945298088e1db16ffe6e3e78e7`.
O rótulo é editorial; não atesta autenticidade de samba. Direitos, recorte,
notas e localização dos artefatos estão em `midis/README.md`.

O diagnóstico anterior reunia `droppedHits` para 2263 notas sem distinguir
o resto do arquivo longo da perda no trecho selecionado. O importador agora
informa `outsideWindowHits` e `inWindowDroppedHits`, preservando o contador
agregado para compatibilidade. O recorte tinha **2 perdas reais** porque V2
atingia o limite de 32 ataques. Ampliei a capacidade interna da célula para
64; o gesto manual de 1–32 toques continua com sua faixa própria. O novo
render offline contém **63 ataques** (V1=17, V2=34, V3=8, V5=4), zero colisões
e zero perdas dentro da janela. Os 2261 ataques restantes ficaram fora da
janela por escolha. Todas as quatro lanes presentes geraram áudio não nulo.

Arquivos: `src/core/CompositionCore.hpp`, `src/core/MidiRhythmImport.hpp`,
`src/core/MidiRhythmImport.cpp`, `src/app/Main.cpp`,
`src/tools/midi_reference_render.cpp`, `tests/midi_rhythm_import.cpp`,
`README.md`, `midis/README.md` e este registro. Build Release serial de
`batruc_app`, `batruc_midi_rhythm_import_test`,
`batruc_midi_reference_render` e `batruc_seeded_recipe_test` concluído;
ambos os testes passaram; `git diff --check` passou. Binário Batruc 0.1.0
gerado em 10/11/2026 às 22:54 CET, SHA-256
`b220078f30ff8f2e1a18ec726831a9b81f7baa16f93905d3a4fcbc6a6a04ea6f`.
Não abri o app. Próximo passo musical: ouvir este recorte no instrumento e
ajustar a relação V1/V2/V3 sem inferir a identidade do gênero só pelo rótulo.

### Tratamento digital por papel na família 8-bit — 10/11/2026

Objetivo: fazer a família 8-bit mudar o sinal de cada stem mantendo bumbo,
caixa, chimbal, tons, pratos e percussão em suas funções. A família antes
alterava apenas coeficientes de corpo/ruído/cauda; o sample-and-hold e a
quantização existiam somente na matéria manual `Chip 8-bit`. Em
`src/core/CompositionCore.cpp`, o perfil de família 8-bit agora processa a
saída de cada voz com clock e resolução próprios por papel, sem remapear a
nota GM nem trocar o instrumento da lane. A matéria manual `Chip 8-bit`
continua com seus controles próprios.

O teste em `tests/seeded_recipe.cpp` verifica que o bumbo da família 8-bit
tem número limitado de níveis de amplitude e que a caixa conserva sinal com
quantização distinta; a saída analógica usada como comparação mantém mais
níveis. Build Release serial de `batruc_app` e `batruc_seeded_recipe_test`
concluído; teste passou. Arquivos afetados: core, teste, `README.md` e este
registro. `git diff --check` passou. Binário Batruc 0.1.0 gerado em
10/11/2026 às 23:00 CET, SHA-256
`a73ebf4a14bcc8baae2d05efcdf2565331df9e27d0978aacbbe83213710b10c9`.
Não houve escuta humana nem abertura do app. Próximo passo:
comparar famílias sobre a mesma célula e nível perceptivo, depois projetar
excitações digitais próprias se o caráter ainda não for suficiente.

### Comparação controlada de famílias no MIDI — 11/11/2026

Objetivo: verificar a diferença entre famílias com o mesmo ritmo e sem
normalização independente de cada resultado. O renderizador de referência
`src/tools/midi_reference_render.cpp` agora aceita `--family -1..16`,
`--gain 0.01..1` e `--mix-only`; aplica o perfil de família às oito lanes,
grava família e ganho no traço e mantém o modo antigo quando nenhuma família
é indicada. Com uma família indicada, o ganho padrão é fixo em 0,7.

Renderizei Rock (família 0) e 8-bit (família 8) com ganho fixo 0,7 sobre o
mesmo beat GMD rock 65 BPM, dois compassos: ambos têm 30 eventos idênticos;
o RMS do mix foi −45,96 e −46,63 dBFS, respectivamente, e os picos 0,1104
e 0,1112, sem corte PCM. Repeti sobre o beat GMD rotulado samba 89 BPM,
63 eventos idênticos: RMS −45,70 e −46,37 dBFS, picos 0,0599 e 0,0803.
Antes da calibração, o 8-bit estava quase 10 dB abaixo de Rock no primeiro
recorte. Em `src/core/CompositionCore.cpp`, o processamento digital mantém
o nível após drive/quantização em vez de dividi-lo novamente, preservando
uma diferença de timbre mensurável sem essa queda de nível nestes dois casos.

Arquivos: renderizador de referência, core, `midis/README.md` e este
registro. Build Release serial de `batruc_midi_reference_render`,
`batruc_app` e `batruc_seeded_recipe_test` concluído; teste passou. Os WAVs
e traços locais estão em `/tmp/batruc-ab-rock-family{0,8}-20261111*` e
`/tmp/batruc-ab-samba-family{0,8}-20261111*`. O RMS de janela inteira não
garante loudness perceptivo igual; o renderer de referência também não inclui
todos os faders e o master do app. Próximo passo: escuta A/B no Batruc e
ajuste por stem se alguma peça ainda desaparecer no mix. `git diff --check`
passou. Binário Batruc 0.1.0 gerado em 11/11/2026 às 00:43 CET, SHA-256
`327cc09e305199c58a254d50329d088e21b7cee92492e34fdf16f70ce155c914`.
Nenhuma janela foi aberta nesta etapa.

### Resposta ao vivo de família e nível — 11/11/2026

Objetivo: verificar uma parte da queixa de alterações tardias sem interromper
o uso do app. O callback lê família e níveis a cada bloco e só reinicia a
forma quando começa uma nova passagem solicitada pelo transporte; a troca
manual desses controles não aciona esse reset. Encontrei uma exceção pequena:
o ruído de um ataque em curso guardava a cor da família anterior até o
próximo golpe. `src/core/CompositionCore.cpp` agora reescala a cauda de ruído
ativa quando `setChannelTimbreProfile` muda de perfil, preservando seu estado
e o playhead.

Em `tests/seeded_recipe.cpp`, dois renderizadores reproduzem a mesma caixa.
Após 256 amostras, um muda a família ou o nível; a comparação exige diferença
sonora em até 16 amostras do bloco seguinte, sem diferença de progresso da
seção. O teste passou a 8 kHz, estabelecendo um limite de 2 ms **dentro do
core em execução**, antes da latência do bloco, da interface e do dispositivo.
Build Release serial de `batruc_app` e `batruc_seeded_recipe_test` concluído.
`git diff --check` passou. Binário Batruc 0.1.0 gerado em 11/11/2026 às
00:56 CET, SHA-256
`7d25213323b629b7cecfbfea6a7bf5035b4a78e4a9f56267aebaa8a661218692`.
Arquivos afetados: core, teste e este registro. A medição ponta a ponta no
app e a confirmação por escuta permanecem pendentes. Não abri janelas.

### Modelos separados de chimbal e fallback de acessórios — 11/11/2026

Objetivo: corrigir uma causa concreta de timbres indistintos e dos nomes de
stem que dependiam do índice da voz. `SoundCharacter` ganhou `openHihat` como
valor novo no fim do enum, preservando os índices salvos anteriormente. V3
usa o modelo fechado e V4 o aberto por padrão; a síntese agora dá a cada um
fundamental, espectro modal, mistura/cauda de ruído e duração próprios, sem
consultar `voiceIndex` para decidir qual chimbal tocar. As notas GM 22/42/44
continuam no fechado; 26/46 acionam o aberto. O seletor por stem e o seletor
global mostram ambos os nomes. O leitor de cenas agora aceita o novo índice 13;
os doze valores anteriores permanecem iguais. O fallback MIDI de V8 mudou de membrana para
escovas, enquanto os demais papéis padrão permanecem bumbo, caixa, fechado,
aberto, tons, pratos e percussão de mão.

Atualizei `README.md` e as descrições Sobre em PT/EN/ES para 14 modelos. A
validação executada foi `git diff --check` e build Release serial do alvo
`batruc_app`, ambos concluídos; SHA-256 do binário:
`58a861d59cca45221ed1a67abbe00c9beb76549012172cf7f7fb2ced6f82cc4f`. Não
executei testes nem abri ou interrompi o app. Falta escuta A/B de V3/V4 e V8
no Batruc, checagem prática dos nomes na interface e validação da resposta
musical com diferentes famílias. A paleta ainda não resolve a falta de fontes e
articulações próprias por estilo. A esteira sintetizada da caixa já está no
renderer; ela não deve ser reimplementada sem uma lacuna demonstrada pela
escuta. Próximo: ouvir o mesmo padrão e as mesmas notas MIDI alternando
fechado/aberto, conferir caixa/esteira e só então priorizar a matriz estilo →
papéis → fontes conforme o resultado.

### Matéria de aro para side stick — 11/11/2026

Objetivo: impedir que a nota GM 37 (side stick/aro) use `brushes`, que é o
modelo genérico de escovas. Acrescentei `SoundCharacter::rimClick` ao fim do
enum, com corpo modal curto e ataque de ruído reduzido; a nota 37 agora o
aciona. O material pode ser escolhido manualmente pelo Mixer e aparece com
rótulo próprio. O leitor de cenas aceita o índice 14 e preserva os índices
anteriores. A família 8-bit também aplica ao aro um clock/resolução próprios.

Atualizei as contagens do Sobre e do README para 15 materiais (seis harmônicos,
oito percussivos e um digital). `git diff --check` e build Release serial de
`batruc_app` passaram; SHA-256 do binário:
`ac2c6cebbe9b1e311f8887b01a8d68d060ae934f22c7a385e435ea1890319942`. Não rodei
testes nem ouvi o timbre; falta comparar side stick, caixa e escovas no mesmo
padrão MIDI. O modelo é síntese estilizada, não sample nem reprodução validada
de aro. Próximo: conferir a persistência/seleção e seguir para validação
auditiva ou outra lacuna da paleta.

### Contagem de entrada do metrônomo — 11/11/2026

Implementei o botão **ENT** para armar uma contagem de um compasso antes do
transporte. A contagem só começa com o metrônomo ligado, segue BPM e compasso,
mantém o transporte parado até o downbeat seguinte e pode ser cancelada pelo
botão principal; desligar o metrônomo durante a contagem cancela a espera. A
opção é salva na cena; o schema avançou de 50 para 51, e cenas anteriores
carregam a contagem desligada. Atualizei o guia do metrônomo no README.

Build Release serial de `batruc_app` concluído e `git diff --check` passou. SHA-256:
`acf0fdf2b771059f4ff0f15b4933fdef506e4282db797cd85747e28cbbb93a12`. Não
executei testes nem abri o app; a conferência auditiva e a verificação visual
da nova opção permanecem pendentes.

### Articulação contínua de chimbal via MIDI CC4 — 11/11/2026

O importador captura CC4 no canal de bateria e guarda o valor vigente por
ataque junto à célula e à nota GM. O sinal chega ao renderer, que interpola
abertura/fechamento no corpo modal, na cauda e no equilíbrio entre ruído
fechado e aberto. A orientação é 0 aberto e 127 fechado, conforme a
[implementação MIDI Roland TD-27](https://static.roland.com/assets/media/pdf/TD-27_MIDI_Imple_eng04_W.pdf),
quando o módulo está configurado para transmitir o pedal como CC4. Arquivos sem
CC4 mantêm as articulações indicadas pelas notas GM.

O importador faz uma passagem limitada para recolher as mudanças CC4 de todas
as pistas; aplica a mudança mais recente pelo tempo absoluto em ticks, inclusive
quando controlador e notas estão em pistas diferentes. O buffer comporta até
65.536 mudanças, e falha ao reservá-lo retorna um erro ao usuário sem encerrar
o processo. Cenas schema 52 persistem CC4 na fonte MIDI e nas células editáveis;
cenas anteriores recebem o valor ausente. Pickups geradas e comparação de
células conservam a articulação.

`git diff --check` e build Release serial de `batruc_app` passaram; SHA-256:
`57f92a5ed1ee49d30657f35c64870fb88ba653908826b041a53a2d290aed0e2b`.
Não rodei testes nem abri o app. Falta ouvir um arquivo com CC4 e verificar a
associação dos valores ao chimbal no instrumento.

### Pickups seeded de tom no MIDI importado — 11/11/2026

A variação MIDI considera agora a lane de toms, desde que a fonte já contenha
pelo menos dois ataques identificados por nota GM. A seed pode inserir até dois
pickups discretos, com velocity baixa, no último compasso da janela importada;
a contagem de compassos vem dos metadados da importação. Não adiciona material a
uma lane vazia e mantém bumbo, caixa e a fonte imutáveis. As variações de
chimbal fechado, percussão de mão e acessórios continuam nos espaços entre
ataques existentes. Para pratos, a seed pode acrescentar até dois pulsos baixos
entre pares consecutivos de ride GM 51/59; crashes e campanas não viram pulsos
repetidos.

Arquivos: `src/core/MidiRhythmImport.*`, `src/app/Main.cpp`, README e este
registro. `git diff --check` passou e o build Release serial de `batruc_app`
concluiu; SHA-256: `cd27640a6c371500fd9d1db7d1e2510362b5574818a491de0746633f7c669a3d`.
Não rodei testes nem abri o app; falta ouvir seeds e confirmar que os toms e
rides funcionam como variações discretas dentro do arranjo.

### Rotação temporal de células MIDI no Study — 11/11/2026

Corrigi o gesto de arrastar o anel vazio em **Study · Ciclo compartilhado**.
Antes, ele alterava o deslocamento de fase da fórmula, mas células explícitas
(incluindo MIDI importado) continuavam com os mesmos tempos. O gesto agora
desloca circularmente todas as posições da célula selecionada e reordena os
ataques sem separar posição, força, nota GM ou CC4. A edição é publicada durante
o arraste e gera uma única entrada de undo/histórico ao soltar. Rotação integral
da volta é tratada como operação sem alteração; em caso de célula inválida, a
transformação é revertida.

Arquivos: `src/core/CompositionCore.*`, `src/app/Main.cpp` e este registro. O
texto de Learn já descrevia o gesto, portanto alinhei a lista de tarefas com o
comportamento. Build Release serial de `batruc_app` e `git diff --check`
passaram; SHA-256: `09bce5c4766c9e4900dbdd32a579d0eeca93f58e5f02b5b67e2edda83b761875`.
Não rodei testes nem abri o app; falta confirmar visualmente e por escuta a
rotação, a associação das articulações MIDI e o desfazer/refazer.

### Resposta de baixo variável desde o primeiro compasso — 11/11/2026

O início da linha continuava igual porque o baixo derivado seguia os mesmos
ataques vocais. Nos níveis de variação 1–3, o rebuild agora acrescenta no
primeiro compasso uma resposta curta e discreta num contratempo livre. A seed
combinada escolhe posição e pequena alteração de altura; nenhum ataque original
é substituído. No nível 0, a resposta não é inserida.

Arquivos: `src/app/Main.cpp`, README e este registro. Build Release serial de
`batruc_app` e `git diff --check` passaram; SHA-256:
`5005f1cdef521545841d027e7f2dbba6ba361bfdf3ae3a58874e16dc107c3571`.
Não rodei testes nem abri o app. Falta escutar no app e confirmar que a resposta
fica musical e varia entre seeds; o app aberto antes da compilação ainda usa o
binário antigo.

### Remoção das pausas periódicas entre passagens — 11/11/2026

Localizei outra fonte dos ciclos sem som em `PassageDevelopment.hpp`: a cada
forma de sete ciclos, a camada de desenvolvimento podia selecionar uma
passagem inteira chamada `Pause`; em A' também anulava uma célula inteira. A
seleção agora usa uma passagem ativa com contraste de densidade, e A' apenas
suaviza a célula. O papel legado de pausa, se chamado por estado antigo, também
fica audível. A forma continua variando sem inserir silêncio automático.
Removi também o corte residual no limite C5 dos renders de fixture, protótipo
e render offline; esse corte era redundante com o gate por ciclo e podia
produzir uma lacuna audível mesmo sem uma passagem classificada como pausa.
Na revalidação, a comparação dos 17 perfis foi estendida aos oito papéis de
percussão. Ela encontrou que a quantização 4-bit do perfil 8 apagava o ruído
suave do timbre de escovas no V8; o ganho de entrada desse timbre agora compensa
a quantização. O teste exige saída audível do perfil 8 em cada papel de stem e
compara os demais perfis após igualar RMS, para que só uma diferença de volume
não conte como mudança de timbre.

Arquivos: `src/core/PassageDevelopment.hpp`, `src/core/CompositionCore.cpp`,
`src/tools/offline_render.cpp`, `tests/seeded_bass_fixture.cpp`,
`tests/seeded_recipe.cpp`, `prototype/score.js`, `tests/variant_transport.cpp`,
README e este registro. Build Release serial do app e render; suíte completa
CTest: 18/18 aprovada em 71,89 s; `git diff --check` passou. SHA-256 do app:
`27912f8714c41e35c5afd11a4f0e48d9a9933518a0392af1e036cbd7b8f1b94a`.
O teste comprova que a camada de forma não zera passagens e que o perfil 8-bit
não apaga o stem de escovas; ainda falta escutar no app reaberto para confirmar
o resultado musical no sistema de áudio real.

### Reconciliação do tutorial da forma e do rótulo do Mixer — 11/11/2026

Após remover as pausas automáticas, corrigi duas referências residuais: o
README ainda chamava uma das oito passagens longas de “pausa” e descrevia C5
como pausa; o dicionário de interface ainda continha a tradução inativa
“Mixer · 12 vozes”. O guia agora descreve a redução parcial de textura em C6,
C5 como Expansão, e informa que a camada longa não insere silêncio automático.
O rótulo residual foi removido; o rótulo ativo já diz “Mixer · 8 stems”.

Arquivos: README, `src/app/Main.cpp` e este registro. `git diff --check` e build
Release serial de `batruc_app` concluídos. A revisão de textos continua aberta
para os demais idiomas e para confirmação visual no app.

### Três novas matérias para percussão MIDI — 11/11/2026

Ampliei a paleta do renderer com modelos sintetizados separados para chocalho,
agogô e bloco de madeira. No mapeamento GM, 54/69/70 usam o chocalho, 67/68 o
agogô e 75/76/77 o bloco, com registros distintos para agogô alto/baixo e para
claves/bloco alto/baixo. Os novos valores foram acrescentados ao fim de
`SoundCharacter`; schema 53 aceita os índices novos e mantém a leitura dos
anteriores. Os rótulos do Mixer e os textos Sobre em PT/EN/FR foram atualizados,
assim como o README. Esses modelos são síntese estilizada, sem samples; a
adequação tímbrica e a inteligibilidade no arranjo ainda precisam de escuta.

Arquivos: `CompositionCore.hpp/.cpp`, `Main.cpp`, README e este registro.
`git diff --check` e build Release serial de `batruc_app` passaram; SHA-256:
`922db7acfad540ca1759bc4ccf3a1cfc298f1251546b7eabf4ecab15c269c088`. Não rodei
testes nem abri o app. Falta comparar estas notas em solo e no stem V8, e
confirmar a persistência de cenas schema 53 na interface.

### Polifonia de notas MIDI no mesmo stem — 11/11/2026

Implementei até quatro notas GM distintas por onset numa lane, mantendo o
tempo original. Cada camada guarda velocity e CC4 próprios. Repetições da mesma
nota se fundem pela maior velocity; notas excedentes ao limite são contadas
como perda real e informadas na importação. O EventPlan cria eventos simultâneos
em camadas fixas e o renderer mantém estados modais independentes por stem e
camada, sem alocar memória no callback. As rotinas de derivação do baixo ignoram
as camadas adicionais para não contar um acorde percussivo como vários ataques.
Edição, rotação, undo/redo e scene schema 54 serializam e leem os dados.

Arquivos: `CompositionCore.hpp/.cpp`, `MidiRhythmImport.hpp/.cpp`, `Main.cpp`,
`midi_reference_render.cpp`, `midis/README.md` e este registro. Build Release
serial de `batruc_app` e `batruc_midi_rhythm_import_test` concluído; o teste
focal passou cobrindo duas notas simultâneas, contribuição distinta ao áudio e
perda reportada no quinto note-on; também confirmou que a rotação Study mantém
a segunda nota ligada ao ataque. `git diff --check` passou. Binário Release
gerado às 18:22 CET, SHA-256
`55f22a71d58e8e75c373726546c1ee486ebc04ecce4e75096171ee7bd8d6ef9d`.
Não abri o app nem fiz escuta. Falta o autor ouvir ataques simultâneos no V8 e
conferir visualmente uma cena schema 54 salva e reaberta.

### Verificação focal de CC4 entre pistas MIDI — 11/11/2026

Ampliei `batruc_midi_rhythm_import_test` com um MIDI formato 1 de duas pistas:
as notas de chimbal ficam numa pista e as mudanças CC4 (0 aberto, 127 fechado)
na outra. O teste verifica a associação pelo tempo absoluto e renderiza o mesmo
ataque com CC4 aberto e sem CC4, exigindo contribuição sonora diferente. O teste
passou junto com os casos de polifonia, rotação Study, renderização de camadas e
limite de quatro notas. Não alterei o código do app nem recompilei o binário
Release nesta etapa; a escuta de um arquivo real com CC4 no Batruc permanece
pendente.

Arquivos: `tests/midi_rhythm_import.cpp` e este registro. Build Release serial
do alvo `batruc_midi_rhythm_import_test` e execução passaram; `git diff --check`
passou. O binário do app segue no estado compilado às 18:22 CET, SHA-256
`55f22a71d58e8e75c373726546c1ee486ebc04ecce4e75096171ee7bd8d6ef9d`.

### Variação de velocity em notas MIDI simultâneas — 11/11/2026

A auditoria do caminho seeded mostrou que a seed alterava apenas `strengths`
da nota principal de cada ataque. Notas GM simultâneas mantinham sempre a
velocity importada, mesmo quando sua camada sonora e seu CC4 eram preservados.
Agora cada camada simultânea recebe uma variação de dinâmica independente e
determinística, ponderada pelo papel da lane e pelo nível da receita. Nota GM,
posição e CC4 não mudam; no nível de variação zero, toda velocity original é
mantida.

Adicionei cobertura para reprodutibilidade com a mesma seed, mudança com seed
distinta, preservação da nota/articulação e bypass no nível zero. Arquivos:
`src/core/MidiRhythmImport.cpp`, `tests/midi_rhythm_import.cpp`, README e este
registro. Build e execução de `batruc_midi_rhythm_import_test`, build de
`batruc_app` e `git diff --check` passaram. O teste verifica mesma seed
reprodutível, seed distinta com velocity diferente, nota/articulação
preservadas e nível zero sem alteração. Binário Release gerado às 18:29 CET,
SHA-256 `53b4c17386e199253564fd53eaf4ce2476983baf8fedca1e05ef0b270beb0fa0`.
Não houve escuta; a variação deve ser avaliada no app com um MIDI que contenha
notas simultâneas.

### Reconciliação do guia MIDI com o estado atual — 11/11/2026

Corrigi duas descrições defasadas: o README ainda atribuía à seed uma
translação de fase que foi retirada em 08/11 para preservar o groove, e
`midis/README.md` ainda dizia que CC4 não era interpretado, embora o importador,
renderer e schema já ofereçam esse caminho desde 11/11. A documentação agora
separa o que a seed faz (dinâmica por ataque/camada e pickups compatíveis) das
operações gerais ainda pendentes (densidade adaptativa/multistem, microtiming
seeded independente e recombinação de frases/compassos); redução de densidade,
interlocking, recombinação por células e contorno de intensidade estão no Study.
Também distingue validação automatizada de CC4 da
escuta de arquivo real no app.

Arquivos: `README.md`, `midis/README.md` e este registro. Conferi os trechos
contra `seededMidiRhythmCells()`, `seededMidiPhraseCells()` e as notas de
continuidade do CC4; buscas direcionadas não encontraram outras afirmações
atuais de que CC4 esteja ausente. `git diff --check` passou. Não houve build,
pois esta etapa alterou somente documentação.

### Metadados editoriais do GMD na importação — 11/11/2026

Ao importar um MIDI cujo caminho corresponde exatamente a uma entrada do
`midis/groove/info.csv` local, a prévia agora mostra estilo, beat/fill, compasso,
BPM e divisão train/test. O diálogo de importação de arquivo também sugere a
fonte, a atribuição e CC BY 4.0, mantendo os direitos como `unverified`; MIDI
sem correspondência não recebe rótulos inferidos. As cenas schema 55 persistem
os metadados editoriais em campos próprios. Acrescentei ao diagnóstico de
round-trip da cena uma verificação de gravação/leitura schema 55 e compatibilidade
schema 54 sem esses campos.

Arquivos: `src/app/Main.cpp`, README, `midis/README.md` e este registro. O
primeiro preflight caiu para 5,0 GiB disponíveis e swap subiu de 546 para 698
MiB, então adiei o build. Depois a memória recuperou para 8,8 GiB disponíveis;
compilei `batruc_app` serialmente em Release e `git diff --check` passou. Binário
gerado às 18:45 CET, SHA-256
`ad11dcab84b883bb84860933348e8a77d406991dd3e0e56166bcce29a2ea63a1`.
O diagnóstico de round-trip schema 55/compatibilidade schema 54 foi acrescentado,
mas não executado; a prévia no app também aguarda inspeção manual. A associação
é pelo caminho do catálogo e não verifica o checksum do MIDI contra uma cópia
conhecida do GMD.

Na revisão seguinte encontrei que `applySceneVersion()` restaurava os campos,
mas o rótulo MIDI permanecia no estado inicial “Nenhum MIDI aplicado”. A
restauração agora recompõe resumo de arquivo/compassos/stems e tooltip com
caminho, hash, metadados editoriais e estado de direitos. O número de ataques
não é reconstruído porque cenas não guardam a contagem original de note-ons
fundidos. Build Release serial do app concluído às 18:49 CET; SHA-256
`cbd170b9c711eb6125305ce1553d0fde28df48419d0912ae98db546662854d02`.
O diagnóstico de round-trip não foi executado e a exibição restaurada ainda
precisa de inspeção no app.

### Redução de densidade por stem no Study — 11/11/2026

Acrescentei ao menu **RITMO** a redução local para aproximadamente 75% ou 50%
dos ataques. O core mantém o ataque do tempo zero quando existente, prioriza
velocities maiores e desempata ataques equivalentes por seed. Cada onset é
copiado como uma unidade, com notas simultâneas, velocity e CC4 preservados;
a fonte MIDI segue intacta e a edição passa pelo undo/redo já existente. O
seletor desativa reduções que não mudariam células pequenas. No estado deste
registro inicial, densidade adaptativa entre stems e recombinação de frases/
compassos ainda estavam pendentes; as entradas posteriores deste mesmo dia
registram a implementação das duas operações. A recombinação por células foi
registrada separadamente abaixo.

Arquivos: `src/core/CompositionCore.hpp/.cpp`, `src/app/Main.cpp`,
`tests/midi_rhythm_import.cpp`, README e este registro. Build Release serial de
`batruc_app` e `batruc_midi_rhythm_import_test` passou; o teste focal passou
(1/1); a suíte completa CTest passou **18/18** em 69,23 s. `node --check
prototype/score.js` e `git diff --check` passaram. SHA-256 do app:
`412ac0480c5bda0fb2445dd908ee16adf107108c302dd0db0b253350d3bb147a`. Falta
ensaio visual e auditivo no app, especialmente ao comparar cada redução em
V1–V8 e conferir desfazer/refazer.

### Interlocking MIDI entre células no Study — 11/11/2026

O mesmo menu **RITMO** agora lista as outras células MIDI ativas como parceiras.
Para cada onset do stem selecionado que coincide com um ataque parceiro, o core
procura os pontos médios dos intervalos do parceiro e move o onset para o mais
próximo que esteja livre e a até 1/8 de ciclo. Mantém a contagem, a ordenação e
as notas/velocities/CC4 simultâneas agrupadas. Se não houver posição adequada,
mantém aquele ataque; onsets que já não coincidem não são alterados. A operação
é determinística pela seed e passa pelo undo/redo. Isto é um primeiro
interlocking por reposicionamento de colisões, não recombinação geral de frases.

Arquivos: `src/core/CompositionCore.hpp/.cpp`, `src/app/Main.cpp`,
`tests/midi_rhythm_import.cpp`, README e este registro. Build Release serial do
app e do teste importador passou; suíte CTest completa: 18/18; o teste MIDI
confirma deslocamento, preservação de ataques/dados simultâneos, determinismo e
no-op sem colisões. `git diff --check` e `node --check prototype/score.js`
passaram. SHA-256 do app:
`b414f69e7a5e2267c03ca1371d01181b3708b3ad4e0af668f8620371914e2e3d`. Falta
conferir no app a lista de parceiras, o movimento musical em dois stems reais e
o undo/redo.

### Contornos de intensidade por stem no Study — 11/11/2026

Acrescentei ao mesmo menu os desenhos **Crescendo**, **Decrescendo** e
**Alternada**. Crescendo e decrescendo aproximam progressivamente a intensidade
de cada onset a um contorno ascendente ou descendente; Alternada alterna dois
níveis e a seed escolhe qual começa forte. A transformação atua só sobre as
forças da célula selecionada, preserva tempos e notas e aplica o mesmo contorno
às notas simultâneas, mantendo suas diferenças relativas. A operação entra no
undo/redo.

Arquivos: `src/core/CompositionCore.hpp/.cpp`, `src/app/Main.cpp`,
`tests/midi_rhythm_import.cpp`, README e este registro. O teste focal verifica
formas diferentes e determinísticas, mudança de seed na fase alternada,
preservação de tempos/notas/CC4 e aplicação às colisões simultâneas. Compilei
app e teste em Release; o teste focal e a suíte completa passaram (18/18 em
72,20 s). `git diff --check` e `node --check prototype/score.js` passaram.
SHA-256 do app: `97344654f35bdde80c05788952e932ad9d6261ede727d78c195ec6ddfcaab5df`.
Falta conferir visualmente o menu e escutar os contornos em vários stems no app.

### Recombinação de células MIDI no Study — 11/11/2026

Acrescentei o submenu **Recombinar com Vx**. A seed escolhe uma paridade de
ataques de cada célula; a união dos tempos produz um novo ritmo na voz
selecionada. Sobreposições próximas são fundidas. Os ataques que vêm do parceiro
recebem a nota e o grupo MIDI mais próximo por posição relativa da voz
selecionada, com força reduzida a 82%; não importam timbre nem notas GM do
parceiro. Assim a operação mistura a organização temporal sem inverter a função
do stem, preserva notas simultâneas e mantém a fonte original para refazer ou
desfazer a transformação.

Arquivos: `src/core/CompositionCore.hpp/.cpp`, `src/app/Main.cpp`,
`tests/midi_rhythm_import.cpp`, README e este registro. Build Release serial do
app e do teste importador passou; o teste focal e a suíte CTest completa passaram
(18/18 em 82,76 s). O teste verifica determinismo, diferença entre fases de
seed, contagem, preservação de dados MIDI e identidade da voz selecionada.
`node --check prototype/score.js` e `git diff --check` passaram. SHA-256 do app:
`a661e9278b05b6c2cf310b387f61279f2a2cff4333f26e70d0be40117e3db76f`.
Recombinação de frases/compassos, comparação auditiva, e conferência do menu e
do undo/redo no app continuam pendentes.

### Recombinação MIDI por compasso — 11/11/2026

O menu **RITMO** ganhou **Recombinar por compasso**, disponível apenas para uma
fonte MIDI com dois ou mais compassos e quando a voz selecionada e uma parceira
têm pelo menos dois ataques em algum compasso correspondente. A operação aplica
a recombinação de células em cada compasso elegível, usando uma variação de seed
por compasso; os demais compassos conservam a célula selecionada. Os tempos vêm
da voz parceira, mas notas GM, articulações, velocities e CC4 continuam sendo os
da voz selecionada. A fonte MIDI original não é alterada; a transformação entra
no undo/redo e no histórico da importação. A disponibilidade da ação usa o
mesmo critério para evitar oferecer uma opção sem efeito.

Arquivos: `src/core/CompositionCore.hpp/.cpp`, `src/app/Main.cpp`,
`tests/midi_rhythm_import.cpp`, README e este registro. Build Release do app e
do teste importador passou; o teste focal cobre seleção por seed, repetição,
preservação da matéria sonora e rejeição de fontes de um compasso. O teste focal
passou; a suíte CTest completa passou **18/18** em 69,30 s. `git diff --check`
passou. SHA-256 do app Release:
`2e3ae3509e763841fc83f3d198bd1f2e2a6fd2ac1513febaa2104bd9318c9b1e`. O menu,
undo/redo e a musicalidade precisam de confirmação visual/auditiva no app.

## Receptor do Maestruc — 14/11/2026

- **Objetivo:** primeira prova do Maestruc (autor: "avance").
- **Mudança:** `src/app/MaestrucLink.h` (cópia com proveniência), `juce_osc`
  no `CMakeLists.txt`, `enableMaestrucLink()` no `Main.cpp`: play/stop
  (`togglePlayback`, só quando muda o estado; início agendado até 10 s),
  tempo (`bpmSlider_`, 20–220), seed (o mesmo caminho do campo SEED),
  REC (`startRecording` com `Batruc-<take>.wav` na pasta dada,
  `finishRecording(false)`), estado. Liga com `--maestruc` ou
  `BATRUC_MAESTRUC=1`; desligado por padrão.
- **Validação:** build Release (3 min 41 s, 1,3 GB de pico); o app aberto
  com `--maestruc` se anunciou ao Maestruc (`transport,tempo,seed,rec`),
  aceitou tempo 97 e seed `1A2B3C4D` (estado confirmou), recusou seção
  ("capacidade ausente") e voltou a 120 BPM; CPU medida pelo Maestruc 0,24
  núcleo parado. **Play e REC não foram ensaiados** (fariam som e gravariam
  na máquina do autor). CTest não rodado (o núcleo não mudou). Nada
  commitado nem enviado.
- **Próximo passo:** o autor ensaia play/stop e REC regidos; depois, decidir
  o que tom, seção e intensidade significam no Batruc.

## Abertura regida pelo Maestruc — 15/11/2026

- **Objetivo:** o Maestruc definir seed e abrir peças (autor: "por exemplo
  definir seeds, abrir peças"; "ok, comece pelo batruc"; "cuide para não
  bugar os instrumentos que já funcionam").
- **Mudança:** `Main.cpp`: `applySeedFromOutside` (o caminho do campo SEED,
  agora usado também pelo handler de seed) e `openSceneFromOutside`
  (`parseSceneFile` → `appendSceneVersion` → `applySceneVersion`, como Abrir
  cena, sem janela); handler `open` do receptor; `--abrir` e `--seed` no
  `initialise` (tokens com aspas; caminho absoluto). `MaestrucLink.h`
  recopiado do Maestruc (mensagem `open`; nota "tônica" em UTF-8);
  `#include <cstdio>`.
- **Validação:** build Release do app (43 s, 1,2 GB de pico); CTest **28/28**
  em 70,8 s. App aberto de verdade, sem tocar: sem opções abre normalmente e
  não abre porta UDP; `--seed 1A2B3C4D --maestruc` → estado parado com
  `1A2B3C4D`; `--abrir chip8-scene.batruc.txt` → seed da cena `5EED2027`;
  pelo Maestruc, seed → `1A2B3C4D`, `open` de outra cena → ok e `5EED2027`,
  arquivo inexistente e arquivo inválido → ok = 0 com o motivo e estado
  mantido. SHA-256 do app:
  `c55c235577016521a299c6cdda73bcf65daeba282f63e98fbce109a12f8b5bd5`.
- **Acentos nas mensagens de erro** (autor: "pode corrigir o batruc"): as
  172 atribuições `error = "…"` com acento (leitor de cenas, MIDI, polígonos,
  diagnósticos) passaram a usar `utf8(…)`, o mesmo helper do `localized()`;
  antes o `juce::String` as lia como ASCII e a janela "Não foi possível abrir
  a cena" mostrava "invÃ¡lido". Troca mecânica (texto e lógica iguais);
  declaração de `utf8` antes de `parseSceneFile`. **Validação:** CTest 28/28
  (73,8 s); recusa de cena inválida pelo Maestruc chegou como "Formato de
  cena inválido ou versão não suportada." SHA-256 do app:
  `2309398c50c37b408f5495abe73e7fb9c32d1053cc544e0331fb9a8b89276432`.
- **Limite:** play/stop e REC regidos continuam sem ensaio com som. Nada
  commitado nem enviado.
