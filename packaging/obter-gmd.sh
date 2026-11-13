#!/usr/bin/env bash
# Baixa o Groove MIDI Dataset (MIDI-only, v1.0.0, Google LLC, CC BY 4.0) e
# confere o SHA-256 antes de extrair em midis/groove. É o catálogo Ritmos /
# MIDI do Batruc; o CMake o instala com o app quando está presente. O dado
# é de terceiros e não é versionado neste repositório (ver midis/README.md e
# CREDITS_AND_SOURCES.md).
#
#   packaging/obter-gmd.sh        (na raiz do projeto)
set -euo pipefail
RAIZ=$(cd "$(dirname "$0")/.." && pwd)
URL=https://storage.googleapis.com/magentadata/datasets/groove/groove-v1.0.0-midionly.zip
SHA256=651cbc524ffb891be1a3e46d89dc82a1cecb09a57c748c7b45b844c4841dcc1e
ZIP="$RAIZ/midis/groove-v1.0.0-midionly.zip"

mkdir -p "$RAIZ/midis"
if [ -f "$RAIZ/midis/groove/info.csv" ]; then
    echo "midis/groove já existe; nada a fazer"
    exit 0
fi
[ -f "$ZIP" ] || curl -fsSL -o "$ZIP" "$URL"
if command -v sha256sum >/dev/null 2>&1; then
    echo "$SHA256  $ZIP" | sha256sum -c -
else
    echo "$SHA256  $ZIP" | shasum -a 256 -c -
fi
unzip -q "$ZIP" -d "$RAIZ/midis"
[ -f "$RAIZ/midis/groove/info.csv" ] || { echo "info.csv ausente depois de extrair" >&2; exit 1; }
echo "GMD pronto em midis/groove ($(find "$RAIZ/midis/groove" -name '*.mid' | wc -l) arquivos MIDI)"
