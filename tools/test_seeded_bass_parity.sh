#!/bin/sh
# SPDX-License-Identifier: AGPL-3.0-or-later
set -eu

fixture=$1
output_dir=$2
project_root=$3
node_program=${BATRUC_NODE_EXECUTABLE:-node}
seeds='A1B2C3D4 00000001 5EED2026 FFFFFFFF'
phases='0 0.125 0.25'
bass_variations='0 1'
mkdir -p "$output_dir"
for meter in 3/4 4/4 5/4 7/8; do
  meter_key=$(printf '%s' "$meter" | tr '/' '-')
  if [ "$meter" = '5/4' ]; then
    bpms='100 120 160 220'
  else
    bpms='97 100 120 160 220'
  fi
  for phrase in loop breath cadence call-response; do
    for relation in independent follow answer; do
      for seed in $seeds; do
        for bpm in $bpms; do
          for phase in $phases; do
            phase_key=$(printf '%s' "$phase" | tr '.' '_')
            for bass_variation in $bass_variations; do
              "$fixture" "$relation" "$meter" "$phrase" "$seed" "$bpm" "$phase" "$bass_variation" 0 4 0.57 straight steady meet manual 1.5 \
                > "$output_dir/$meter_key-$phrase-$relation-$seed-$bpm-$phase_key-$bass_variation.json"
            done
          done
        done
      done
    done
  done
done
cd "$project_root"
"$node_program" tests/compare_seeded_bass.cjs "$output_dir"
