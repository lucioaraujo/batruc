# Batruc

**English** · [Português](#português) · [Français](#français) · [Español](#español)

Batruc is a rhythm-composition instrument of the
[RASGO](https://rasgoinstruments.arquiviagem.net/) family by Lúcio de Araújo.
Mathematical and physical models write editable rhythms: coupled pendulums,
billiard, Lissajous figures, Fourier series and rolling curves. You then play,
shape and write them down. Its former name was Trucmetry.

**Version:** 0.1.5 · **License:** GNU AGPL-3.0-or-later ([`LICENSE`](LICENSE))
· **Packages:** Linux, Windows, macOS ([releases](https://github.com/lucioaraujo/batruc/releases))

## What it does

- **Seeds that write rhythm.** Each seed composes figures for kick, snare and
  hi-hat from a physical model, with phrase form, ruptures and development
  across seven formal cycles. The figures are editable in the Study.
- **RHYTHM and GENERATIVE modes.** RHYTHM plays exactly what is written.
  GENERATIVE adds the field masks, intensity laws and form, at an adjustable
  depth (GER).
- **A bass with its own voice**, in a key chosen by the seed (12 tonics × 10
  modes).
- **Snare as an instrument.**
  - Ornaments are stored on the note: ghost notes, flam, drag, ruff,
    ricochet, roll, rim and buzz.
  - A per-stem **ORN** switch toggles between the plain beat and the full
    snare.
  - Genre grooves (frevo, maracatu, baião) and measured microtiming are
    available from the Study.
- **Notation.** A bass staff and a drum staff with sticking, both exportable
  as **MusicXML** for MuseScore and other score editors.
- **Rhythms / MIDI.** A catalogue built from the Groove MIDI Dataset (CC BY
  4.0), plus your own MIDI files.
- Recording to WAV, versioned scenes, and an interface in four languages.

## What it does not do (yet)

- **Windows and macOS are built and tested in CI only.** They have not yet
  been opened and played on real Windows or macOS machines.
- The style labels from the MIDI dataset describe drum performances, not
  musical traditions.
- The genre grooves use only what the cited research documents
  ([`ESTUDO_ORNAMENTOS_CAIXA.md`](ESTUDO_ORNAMENTOS_CAIXA.md)).

## Install

See [`INSTALL.md`](INSTALL.md) (packages, building from source, platform
matrix). Credits and sources: [`CREDITS_AND_SOURCES.md`](CREDITS_AND_SOURCES.md).
Changes: [`CHANGELOG.md`](CHANGELOG.md). Development diary:
[`DESENVOLVIMENTO.md`](DESENVOLVIMENTO.md).

Contact: rasgo.instruments@gmail.com

---

## Português

O Batruc é um instrumento de composição rítmica da família
[RASGO](https://rasgoinstruments.arquiviagem.net/), de Lúcio de Araújo.
Modelos matemáticos e físicos escrevem ritmos editáveis: pêndulos acoplados,
bilhar, figuras de Lissajous, séries de Fourier e curvas rolantes. Depois
você toca, molda e escreve esses ritmos. O nome anterior era Trucmetry.

**Versão:** 0.1.5 · **Licença:** GNU AGPL-3.0-or-later · **Pacotes:** Linux,
Windows e macOS ([releases](https://github.com/lucioaraujo/batruc/releases))

- **Seeds que escrevem ritmo:** figuras de bumbo, caixa e chimbal a partir
  de um modelo físico, com forma de frase, rupturas e desenvolvimento em
  sete ciclos. Tudo é editável no Study.
- **Modos RITMO e GERATIVO:** o RITMO toca o que está escrito; o GERATIVO
  acrescenta máscaras, leis de intensidade e forma, com profundidade
  ajustável (GER).
- **Baixo com voz própria,** no tom sorteado pela seed (12 tônicas × 10
  modos).
- **Caixa como instrumento:**
  - ornamentos guardados na nota (fantasmas, flam, drag, rufo, ricochete,
    rulo, aro, toque múltiplo);
  - botão **ORN** por stem, para alternar entre a batida normal e a caixa
    completa;
  - ginga por gênero (frevo, maracatu, baião) e microtempo medido, no Study.
- **Notação:** pauta do baixo e pauta de bateria com manulação, exportáveis
  em **MusicXML**.
- **Ritmos / MIDI:** catálogo do Groove MIDI Dataset (CC BY 4.0) e seus
  próprios arquivos MIDI.
- Gravação em WAV, cenas versionadas e interface em quatro línguas.

**Ainda não:** Windows e macOS são compilados e testados só na integração
contínua; ainda não foram abertos e tocados em máquinas reais.

Instalação: [`INSTALL.md`](INSTALL.md) · Créditos:
[`CREDITS_AND_SOURCES.md`](CREDITS_AND_SOURCES.md) · Diário:
[`DESENVOLVIMENTO.md`](DESENVOLVIMENTO.md)

---

## Français

Batruc est un instrument de composition rythmique de la famille RASGO, de
Lúcio de Araújo. Des modèles mathématiques et physiques écrivent des rythmes
modifiables : pendules couplés, billard, Lissajous, Fourier et courbes
roulantes.

Il propose :
- les modes RYTHME et GÉNÉRATIF ;
- une basse à voix propre ;
- des ornements de caisse claire sur la note, avec ORN par stem ;
- des portées de basse et de batterie exportables en MusicXML.

Version 0.1.5 ; paquets Linux, Windows et macOS (Windows et macOS testés en
CI seulement). Licence
AGPL-3.0-or-later. Installation : [`INSTALL.md`](INSTALL.md).

## Español

Batruc es un instrumento de composición rítmica de la familia RASGO, de
Lúcio de Araújo. Modelos matemáticos y físicos escriben ritmos editables:
péndulos acoplados, billar, Lissajous, Fourier y curvas rodantes.

Ofrece:
- los modos RITMO y GENERATIVO;
- un bajo con voz propia;
- ornamentos de caja en la nota, con ORN por stem;
- pentagramas de bajo y batería exportables a MusicXML.

Versión 0.1.5; paquetes Linux, Windows y macOS (Windows y macOS probados
solo en CI). Licencia
AGPL-3.0-or-later. Instalación: [`INSTALL.md`](INSTALL.md).
