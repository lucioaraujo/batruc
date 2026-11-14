# Batruc — notas de versão / changelog

## 0.1.2 — 14/11/2026

**English summary.** All menus now open inside the app window, so the desktop
no longer shows through them on Linux; the header fits its labels in every
language.

- **Menus dentro da janela:** no Linux, cada menu do JUCE era uma janela
  própria do sistema, criada transparente; até ser pintada, mostrava a área
  de trabalho (relato do autor na 0.1.1: FAMÍLIA, GLOBAL e outras caixas).
  Agora todas as caixas, os menus próprios e as caixas dos diálogos abrem
  dentro da janela que os chamou. A auditoria abre pelo código as 50–55
  caixas e menus de cada língua e confere que nenhum vira janela do sistema.
- **Cabeçalho nas 4 línguas:** cada botão mede o próprio texto; em espanhol e
  francês os rótulos não saem mais cortados nem miúdos (em 1080 px, a largura
  mínima, volta às larguras fixas).
- **Troca de língua completa:** com o app aberto, a troca de língua deixava
  em português rótulos e itens do Baixo (REPETIÇÕES, LEI DE DURAÇÃO, RAZÃO,
  Respiração, Cadência…), dicas de vários controles, a legenda do piano roll
  e as caixas de valor dos sliders. Todos passam a acompanhar a língua; os
  textos que só existiam em português ganharam inglês, francês e espanhol.
- **Diagnóstico:** `BATRUC_AUDIT_LANGUAGE=0..3` faz as capturas da auditoria
  na língua pedida, sem gravar a preferência do usuário; a auditoria grava
  `textos-componentes.txt` (todos os textos da tela) para comparar as
  línguas. Resultado: nenhum texto em português sobrando em inglês e
  francês; em espanhol, só palavras iguais nas duas línguas.
- **Site:** Tutorial e LEARN com recortes de capturas reais do app, na língua
  de cada página (`tools/recortar_capturas_site.py`).

## 0.1.1 — 14/11/2026

**English summary.** Menus no longer show the desktop through them for the
first seconds while playing; the snare starts a little lower (0.80).

- **Menus transparentes no Linux:** com o transporte tocando, a animação de
  60 Hz ocupava a thread de mensagens, e a janela do menu aparecia
  transparente até ser pintada. Com um menu aberto, a animação cai para
  10 Hz (relato do autor, 14/11/2026).
- **Caixa mais baixa de partida:** o nível inicial de V2 passou de 1,00 para
  0,80 (cerca de −2 dB) em todas as seeds e famílias. Cenas salvas mantêm os
  níveis delas (autor: "a caixa um pouco mais baixo", 14/11/2026). Ouvido e
  aprovado pelo autor ("ficou melhor", 14/11/2026).
- **Diagnóstico:** `--export-learn` exporta o Aprender nas 4 línguas (fonte
  da página LEARN do site); `--audit-layout` salva também uma captura
  1920×1006 para o portal.

## 0.1.0 — primeira versão pública

**English summary.** First public version of Batruc, a rhythm-composition
instrument of the RASGO family. Mathematical and physical models (coupled
pendulums, billiard, Lissajous, Fourier, rolling curves) write editable
rhythmic figures. It has RHYTHM and GENERATIVE modes, a seed-chosen key for
the bass, a bass line with its own phrase, snare ornaments stored on the
note (with a per-stem ORN switch), genre snare idioms and measured
microtiming. It shows a bass staff and a drum staff with MusicXML export.
The interface is in Portuguese, English, Spanish and French. Packages for
Linux, Windows and macOS; Windows and macOS are built and tested in CI but
have not yet been played on real machines.

**Resumo em português.**

- **Composição:**
  - modos **RITMO** (toca o que está escrito) e **GERATIVO** (máscaras, leis e
    forma), com a profundidade **GER** (padrão 30 %);
  - seeds que **escrevem** figuras em V1–V3 a partir dos pêndulos acoplados,
    com forma de frase, ruptura e desenvolvimento nos 7 ciclos;
  - no Study, as figuras também podem vir do bilhar, da Lissajous, de Fourier
    e das curvas rolantes.
- **Baixo:**
  - tom da seed (12 tônicas × 10 modos);
  - **FRASE**, uma voz própria com progressão de graus, apoio ao bumbo,
    respostas e notas de passagem, ligada por padrão.
- **Caixa:**
  - ornamentos guardados **na nota**: fantasmas, flam, drag, rufo, ricochete,
    rulo, aro e toque múltiplo;
  - botão **ORN** por stem: a batida normal ou a caixa com os recursos;
  - ginga por gênero (frevo, maracatu, baião) e microtempo medido, como
    ações do Study.
- **Percussão no tom da seed:** opção do Mixer.
- **Notação:**
  - pauta do baixo (clave de Fá, armadura);
  - pauta de bateria (sinais de ornamento, manulação D/E);
  - exportação MusicXML das duas.
- **Interface:** quatro línguas, cenas versionadas (até o schema 64),
  gravação WAV, metrônomo.
- **Pacotes:** `.deb`, AppImage e `.tar.gz` (Linux), instalador `.exe` e
  `.zip` (Windows), `.dmg` Universal 2 (macOS), com o Groove MIDI Dataset.
- **Validação:** 28 testes automatizados nos três sistemas (CI). Abertura do
  pacote Linux testada em Debian 12, Ubuntu 24.04, Fedora e Arch. Execução e
  escuta em máquina real só no Linux; Windows e macOS ainda não foram
  abertos em máquinas reais.
