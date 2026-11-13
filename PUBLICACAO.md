# Batruc — prontidão para publicação

Segue a estratégia da família
([`ESTRATEGIA_DE_PUBLICACAO.md`](../../RASGO_DOCUMENTATION/ESTRATEGIA_DE_PUBLICACAO.md)):
não se pula da camada 0 para a 4. O modelo prático é o caminho já feito pelo
Rasgo Modular (`../RASGO_MODULAR/PUBLICACAO.md`).

Decisão do autor (13/11/2026): "avance em todas as etapas para a
publicação, vamos acertando os detalhes do software no caminho".

## Camadas

| Camada | Critério | Estado |
|---|---|---|
| 0. Pesquisa local | estado documentado; nada apresentado como release | ✅ `DESENVOLVIMENTO.md` (antigo README), `TAREFAS.md`, auditorias e estudos |
| 1. Candidato publicável | build e execução multiplataforma, licença, créditos, documentação e limitações revisados | 🟡 Linux ✅; CMake, CPack, scripts e workflow prontos; **Windows e macOS nunca compilados** (falta o repositório próprio para a CI rodar) |
| 2. Release do instrumento | pacote por plataforma, testes, evidência de validação | ❌ falta a CI nos três sistemas e a sessão de escuta documentada |
| 3. Página editorial | texto, autoria, imagens, links e estado correspondem à release | ❌ falta a página no portal da família |
| 4. Publicação | portal, repositório e página liberados juntos | ❌ |

## Gate editorial — item a item

| Item | Estado |
|---|---|
| nome, descrição curta e estado | ✅ `README.md` público em 4 línguas (13/11); o diário virou `DESENVOLVIMENTO.md` |
| autoria, créditos, fontes e licença | ✅ `LICENSE`, [`CREDITS_AND_SOURCES.md`](CREDITS_AND_SOURCES.md) |
| instalação e plataformas | ✅ [`INSTALL.md`](INSTALL.md), com a matriz honesta (só Linux testado) |
| testes e validações realmente executados | 🟡 28 testes no Linux; escuta do autor em lotes; falta a CI nos 3 sistemas |
| screenshots com origem autorizada | ❌ fazer, a partir de execução real |
| links (repositório, documentação, release) | 🟡 repositório público https://github.com/lucioaraujo/batruc; a release depende da CI |
| ausência de áudio privado, gravações e brainstorm | ✅ escutas em `.local-build/` (fora do git) |
| contato oficial | ✅ `rasgo.instruments@gmail.com` (já no CPack) |
| versão publicada = página editorial | ❌ depende da página |
| correção e retirada | 🟡 seguir o procedimento do Rasgo Modular (copiar para cá na release) |
| nome disponível | 🟡 nenhum conflito em web, marcas, software musical, domínios e GitHub (13/11); **falta a busca oficial no INPI e na WIPO** |

## O que falta, separado por quem resolve

### Depende de mim (código e documento)

- [x] CMake: Windows com runtime estático e `/bigobj`; macOS Universal 2
  (10.15); ícone; instalação; CPack (13/11)
- [x] `packaging/linux/` e `.github/workflows/package.yml` (porte do Modular)
- [x] `.deb` gerado e inspecionado localmente
- [x] `INSTALL.md`, `CHANGELOG.md`, `CREDITS_AND_SOURCES.md`, este
  documento
- [x] texto "Sobre" do app nas 4 línguas: versão 0.1.0 candidata, recursos atuais, crédito do GMD, "testado no Linux; Windows e macOS ainda não" (13/11)
- [x] README público curto em 4 línguas (13/11)
- [ ] corrigir o que a CI apontar no Windows e no macOS (o Modular precisou
  de cinco correções na primeira rodada). Rodadas de 13/11:
  - 1–2: macOS ✅. Linux: ICE do GCC 11. Windows: C1060 (memória do MSVC
    no Main.cpp) e C2975 num teste;
  - 3 (Clang no Linux, clang-cl no Windows): macOS ✅; o Windows compila,
    mas 5 testes caem por pilha de 1 MB (os testes usam até 3 MB) e 4
    testes de paridade chamam `/bin/sh`. O Clang 14 do Linux recusa
    structured binding capturado por lambda;
  - 4 (commit `a1e24bc`): pilha de 8 MB no Windows, paridade só fora do
    Windows, locais simples no `drawFunctionGraph`. Run 43523590174.
    Os três pacotes ✅ (macOS, Windows, Linux) e o arranque ✅ em Ubuntu
    24.04, Debian 12 e Arch. No Fedora, o `.tar.gz` não abriu: pedia
    `libatomic.so.1` sem usar símbolo dela (o Clang não liga com
    `--as-needed`; o AppImage já levava a biblioteca);
  - 5: `--as-needed` nas ligações do Linux.
- [ ] screenshots de execução real
- [x] site em `website/` (base do Trioio, 4 línguas, `PUBLICADO = False`,
  noindex; `deploy-site.yml` só manual). Falta ligar o GitHub Pages e virar
  `PUBLICADO = True` na release (13/11)
- [ ] página do Batruc no portal da família (preparada localmente; o
  upload para o HostGator é do autor)

### Depende do autor

- [x] **Catálogo Ritmos / MIDI:** decidido (a), embutir (13/11).
  - O app procura o catálogo nas pastas de instalação.
  - O CMake instala o GMD com a licença dele.
  - `packaging/obter-gmd.sh` baixa e confere o SHA-256; a CI roda esse
    passo antes do build.
  - Conferido: o `.deb` tem 1.150 MIDIs e o binário instalado os carrega.
- [x] **README:** o diário virou `DESENVOLVIMENTO.md` (`git mv`, com o
  histórico preservado), e o README público entrou em 4 línguas (13/11).
- [x] **Repositório próprio e público** `lucioaraujo/batruc` (autor:
  "publico", 13/11/2026). Padrão do Trioio, do Aquorbium e do Maravi:
  - o histórico começa limpo, e o anterior fica no monorepo privado;
  - o e-mail é só o do RASGO (autor: "divulgue somente o email do rasgo");
  - os caminhos locais foram trocados por formas genéricas;
  - `preservation/` e os estudos raster sem origem registrada ficaram de
    fora;
  - varredura de segredos e proteção de push ligadas;
  - primeira rodada da CI nos três sistemas disparada (run 43519082281).
- [x] **Versão** da primeira release: 0.1.0 (13/11).
- [ ] **Busca de marca** no INPI (classes 9, 15 e 41).
- [ ] **Protótipo web em HTML:** vai junto ou fica como histórico.
- [ ] **Sessão de escuta final** documentada.
- [ ] Revisão do espanhol por um falante nativo (desejável).

## Card do portal da família (rascunho, 14/11/2026)

Entra em `RASGO/RASGO_WEBSITE` (4 páginas, JSON-LD posição 9, captura
`assets/screenshots/batruc-0.1.0-1920.jpg` = `website/assets/interface.jpg`)
**só quando a release v0.1.0 existir**, junto com o Pages ligado e
`PUBLICADO = True` (camada 4: portal, repositório e página juntos). O
pacote `rasgo-website.zip` e o envio são do autor. Seção: Instrumentos,
depois do Asterismus. A cena da família no topo (cada instrumento tem a sua)
é decisão estética do autor; não está feita.

- **PT** — kicker `INSTRUMENTO · COMPOSIÇÃO RÍTMICA`. "Modelos matemáticos e
  físicos — pêndulos acoplados, bilhar, figuras de Lissajous, séries de
  Fourier, curvas rolantes — escrevem ritmos que você ouve, edita, toca e
  leva para a partitura. Bumbo, caixa com ornamentos, chimbal e um baixo no
  tom da seed; pautas de baixo e de bateria exportáveis em MusicXML."
  Status: "Disponível para Linux, Windows e Mac." Link: "Site e downloads do
  Batruc →".
- **EN** — `INSTRUMENT · RHYTHM COMPOSITION`. "Mathematical and physical
  models — coupled pendulums, billiards, Lissajous figures, Fourier series,
  rolling curves — write rhythms you hear, edit, play and take to the score.
  Kick, snare with ornaments, hi-hat and a bass in the seed's key; bass and
  drum staves exportable as MusicXML." "Available for Linux, Windows and
  Mac." "Batruc site and downloads →".
- **FR** — `INSTRUMENT · COMPOSITION RYTHMIQUE`. « Des modèles mathématiques
  et physiques — pendules couplés, billard, figures de Lissajous, séries de
  Fourier, courbes roulantes — écrivent des rythmes que vous écoutez,
  modifiez, jouez et portez sur la partition. Grosse caisse, caisse claire
  ornementée, charleston et une basse dans la tonalité de la seed ; portées
  de basse et de batterie exportables en MusicXML. » « Disponible pour
  Linux, Windows et Mac. » « Site et téléchargements de Batruc → ».
- **ES** — `INSTRUMENTO · COMPOSICIÓN RÍTMICA`. «Modelos matemáticos y
  físicos — péndulos acoplados, billar, figuras de Lissajous, series de
  Fourier, curvas rodantes — escriben ritmos que escuchas, editas, tocas y
  llevas a la partitura. Bombo, caja con ornamentos, charles y un bajo en la
  tonalidad de la seed; pentagramas de bajo y batería exportables a
  MusicXML.» «Disponible para Linux, Windows y Mac.» «Sitio y descargas de
  Batruc →».
