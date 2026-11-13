#!/bin/sh
# SPDX-License-Identifier: AGPL-3.0-or-later
# Copyright (c) 2026 Lúcio de Araújo
set -eu

if [ "$#" -lt 1 ] || [ "$#" -gt 3 ]; then
  printf '%s\n' 'Uso: tools/render_compare.sh SEED_HEX [diretório-de-saída] [--stems]' >&2
  exit 2
fi

seed=$(printf '%s' "$1" | tr '[:lower:]' '[:upper:]')
case "$seed" in
  [0-9A-F][0-9A-F][0-9A-F][0-9A-F][0-9A-F][0-9A-F][0-9A-F][0-9A-F]) ;;
  *) printf '%s\n' 'A seed precisa ter oito caracteres hexadecimais.' >&2; exit 2 ;;
esac

project_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
output_dir=${2:-.local-build/parity-audio}
export_stems=0
if [ "${3:-}" = "--stems" ]; then
  export_stems=1
elif [ "$#" -eq 3 ]; then
  printf '%s\n' 'Opção desconhecida; use --stems para comparar vozes e baixo separados.' >&2
  exit 2
fi
case "$output_dir" in
  /*) ;;
  *) output_dir="$project_root/$output_dir" ;;
esac
mkdir -p "$output_dir"
cd "$project_root"

render_binary=.local-build/native/linux-debug-juce/batruc_offline_render
if [ ! -x "$render_binary" ]; then
  cmake --build .local-build/native/linux-debug-juce --target batruc_offline_render -j2
fi

web_stem="$output_dir/batruc-$seed-web-offline"
juce_audio="$output_dir/batruc-$seed-juce-core.wav"
comparison="$output_dir/batruc-$seed-comparison.json"
voice_comparison="$output_dir/batruc-$seed-voices-comparison.json"
bass_comparison="$output_dir/batruc-$seed-bass-comparison.json"
check_absent() {
  artifact=$1
  if [ -e "$artifact" ]; then
    printf '%s\n' "Saída já existe; nada foi sobrescrito: $artifact" >&2
    exit 1
  fi
}
for artifact in "$web_stem.wav" "$web_stem-partitura.txt" \
  "$web_stem.json" "$juce_audio" "$juce_audio.json" "$comparison"; do
  check_absent "$artifact"
done
if [ "$export_stems" -eq 1 ]; then
  check_absent "$web_stem-voices.wav"
  check_absent "$web_stem-bass.wav"
  check_absent "$output_dir/batruc-$seed-juce-core-voices.wav"
  check_absent "$output_dir/batruc-$seed-juce-core-bass.wav"
  check_absent "$voice_comparison"
  check_absent "$bass_comparison"
fi

# Produce the native render first so a browser restriction does not prevent
# the local JUCE-core artifact from being useful on its own.
if [ "$export_stems" -eq 1 ]; then
  "$render_binary" "$seed" "$juce_audio" --stems
  node tools/render_web.cjs "$seed" "$output_dir" --stems
else
  "$render_binary" "$seed" "$juce_audio"
  node tools/render_web.cjs "$seed" "$output_dir"
fi

web_audio="$web_stem.wav"
if [ ! -f "$web_audio" ]; then
  printf '%s\n' "Render HTML offline não encontrado para a seed $seed." >&2
  exit 1
fi

python3 tools/compare_audio.py "$web_audio" \
  "$juce_audio" \
  --html-manifest "$web_stem.json" \
  --juce-manifest "$output_dir/batruc-$seed-juce-core.wav.json" \
  --json "$comparison"

if [ "$export_stems" -eq 1 ]; then
  for stem in voices bass; do
    python3 tools/compare_audio.py "$web_stem-$stem.wav" \
      "$output_dir/batruc-$seed-juce-core-$stem.wav" \
      --json "$output_dir/batruc-$seed-$stem-comparison.json"
  done
fi
