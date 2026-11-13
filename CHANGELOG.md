# Batruc — notas de versão / changelog

## 0.1.0 — primeira versão pública (em preparação, não publicada)

**English summary.** First public candidate of Batruc, a rhythm-composition
instrument of the RASGO family. Mathematical and physical models (coupled
pendulums, billiard, Lissajous, Fourier, rolling curves) write editable
rhythmic figures. It has RHYTHM and GENERATIVE modes, a seed-chosen key for
the bass, a bass line with its own phrase, snare ornaments stored on the
note (with a per-stem ORN switch), genre snare idioms and measured
microtiming. It shows a bass staff and a drum staff with MusicXML export.
The interface is in Portuguese, English, Spanish and French. Only Linux has
been built and played so far.

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
- **Validação:** 28 testes automatizados. Build, execução e escuta só no
  Linux. Windows e macOS ainda não foram compilados.
