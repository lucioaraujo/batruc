# Batruc — créditos, fontes e licenças

## Autoria e licença

- **Autoria e direção artística:** Lúcio de Araújo, família RASGO.
- **Licença do código do Batruc:** GNU AGPL-3.0-or-later, com texto integral em
  [`LICENSE`](LICENSE).
- **Nome histórico:** Trucmetry. O mesmo instrumento foi renomeado em
  25/10/2026.
- **Marca:** o "B" original e o logotipo são do autor
  (`prototype/assets/brand/`). Os ícones dos pacotes são derivados do B.

## Código de terceiros

| Componente | Uso | Licença |
|---|---|---|
| [JUCE](https://juce.com) 9.0.3 | interface, áudio e janelas do app nativo | AGPLv3 ou comercial. Não é versionado neste repositório: o build aponta para um checkout local, e a CI usa a tag 9.0.3. |

Nenhum outro código de terceiros está incorporado. As ideias de bibliotecas
estudadas (DaisySP, Faust, Pure Data) estão em
[`BIBLIOGRAFIA_DE_ESTUDO.md`](BIBLIOGRAFIA_DE_ESTUDO.md) e não são
dependências.

## Código da própria família RASGO (porte com crédito)

Todo este código é da mesma autoria e da mesma licença (AGPL-3.0-or-later).

- **ANTITOTEM**, `writeMusicXmlCaptureToFile`: quantização numa grade,
  decomposição em figuras com ponto e ligadura através da barra. Base de
  `src/core/ScoreNotation.cpp`.
- **MARAVI**, `tools/musicxml_writer.py` e `webapp/score_preview.py`:
  - grafia dos acidentes pela armadura;
  - escrita pensada tempo a tempo;
  - desenho da pauta em traço, sem fonte musical.
- **Rasgo Modular**:
  - empacotamento (`packaging/linux/*`, regras do CPack);
  - workflow de integração contínua (`.github/workflows/package.yml`).

## Dados

- **Groove MIDI Dataset (GMD)**, Google LLC, licença
  [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/). Referência:
  J. Gillick, A. Roberts, J. Engel, D. Eck e D. Bamman, "Learning to Groove
  with Inverse Sequence Transformations", *ICML 2019*,
  <https://magenta.tensorflow.org/datasets/groove>.
  - É usado pelo catálogo Ritmos / MIDI e **vai nos pacotes** (decisão do
    autor, 13/11/2026), com o `LICENSE` e o `README` originais. É baixado
    do endereço oficial e conferido por SHA-256
    (`packaging/obter-gmd.sh`). Não é versionado neste repositório.
  - Cada célula derivada guarda origem, versão, licença e atribuição (cenas,
    schema 44 em diante).
  - Os rótulos de estilo descrevem performances de bateria; não definem
    tradições.

## Pesquisa que orientou recursos (citada, não copiada)

- **Rudimentos:** os nomes e conceitos vêm dos 40 *International Drum
  Rudiments*, © 1984 Percussive Arts Society. A folha não é reproduzida;
  as descrições são próprias. Ver
  [`ESTUDO_ORNAMENTOS_CAIXA.md`](ESTUDO_ORNAMENTOS_CAIXA.md).
- **Idiomas da caixa** (frevo, maracatu, baião): C. L. Galvão, *Per Musi*,
  2023, DOI 10.35699/2317-6377.2023.45902, CC BY 4.0; cita Saldanha (2008)
  e Santos e Resende (2005).
- **Microtempo medido:** M. Davies, M. Fuentes, J. Fonseca, L. Aly, M.
  Jerónimo e F. Bonini Baraldi, "Moving in time: computational analysis of
  microtiming in Maracatu de baque solto", *ISMIR 2020*, Zenodo 4245554,
  CC BY 4.0.
- **Caixa surda no samba:** A. T. Leppaus, ANPPOM 2019.
- **Notação:** padronização de bateria da PAS (N. Weinberg, 1998) e TUBS
  (Harland e Koetting).
- **Método:**
  - José Eduardo Gramani: dissociação e ciclos;
  - G. L. Stone, *Stick Control* (1935), e C. Wilcoxon: manulação.
- **Modelos físicos e matemáticos** (pêndulos acoplados, bilhar, Lissajous,
  Fourier, curvas rolantes): implementações próprias. As fontes estão em
  [`BIBLIOGRAFIA_DE_ESTUDO.md`](BIBLIOGRAFIA_DE_ESTUDO.md).

## O que este repositório não contém

- áudios de teste, escutas e gravações (ficam fora do git);
- o brainstorm do autor;
- o JUCE (é baixado na hora do build).
