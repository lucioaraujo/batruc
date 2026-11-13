# Arquivo da identidade visual do Batruc

Esta pasta organiza as artes sem substituir ou descartar versões. O cabeçalho
do instrumento web usa somente o lettering original em `active/`.

## Pastas

- `active/` — lettering aprovado usado pelo cabeçalho web.
- `icons/` — favicon, ícones de toque e instalação, derivados do B original.
- `references/` — imagem escolhida, referência enviada pelo autor e estudos
  raster recebidos/gerados durante a exploração.
- `archive/` — lockups raster horizontais e verticais, versões históricas.
- `studies/` — símbolos vetoriais e estudos raster sem lettering.
- `proposals/` — alternativas vetoriais de ritmo e berimbau, inclusive
  desenhos isolados.

## Fonte ativa e proveniência

- Fonte do lettering: `active/batruc-wordmark-original-crop-v01.png`, recorte
  sem redesenho da referência `references/batruc-logo-selected-01.png`.
- Símbolos não estão ativos nos instrumentos por decisão do autor; o
  cabeçalho usa somente o lettering aprovado.
- O alvo JUCE incorpora o mesmo PNG em `BinaryData` durante o build, sem
  depender de caminhos de arquivos externos à instalação.
- Em 02/11/2026, o lettering do cabeçalho JUCE foi reduzido sem redesenho e a
  linha “matemática · física · música” foi retirada; os créditos continuam no
  rodapé. O wireframe `PROPOSTA_LAYOUT_EDITOR_CICLOS_2026-11-02.svg` usa esta
  mesma fonte ativa por referência relativa.
- As propostas e versões recusadas permanecem como documentação do processo.
- Antes da reorganização, o conjunto e os arquivos de origem dos geradores foram
  copiados para `preservation/batruc-logo-before-reorganization-20261030.tar.gz`
  na raiz do projeto. O checksum fica no arquivo adjacente `.tar.gz.sha256`.

## Geradores

Os programas em `tools/logo/` escrevem seus resultados nestas pastas:

- símbolos matemáticos em `studies/`;
- alternativas de composição em `proposals/`;
- ícones web em `icons/`.

Regenerar propostas não altera a fonte ativa do lettering.

## Ícones para publicação web

`icons/` contém `favicon.svg`, `favicon.ico` multirresolução, PNGs de 16/32/48
px, `apple-touch-icon.png` de 180 px e ícones PWA de 192/512 px, incluindo a
variante maskable. O vetor do favicon é traçado do B original, com as duas
contraformas preservadas. O favicon em si tem fundo transparente; os ícones de
toque/instalação usam o papel claro `#FDF9F2` para manter contraste em fundos
variados. `site.webmanifest` e o cabeçalho web apontam para esses arquivos. O
ícone da aplicação JUCE do sistema ainda precisa ser configurado separadamente.
Para regenerar o conjunto web, execute
`python3 tools/logo/generate_brand_icons.py` na raiz do projeto; o gerador usa
ImageMagick (`convert`) e Potrace para vetorizar o recorte aprovado.
