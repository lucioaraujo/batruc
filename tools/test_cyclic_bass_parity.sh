#!/bin/sh
# SPDX-License-Identifier: AGPL-3.0-or-later
set -eu

fixture=$1
output_dir=$2
project_root=$3
node_program=${BATRUC_NODE_EXECUTABLE:-node}
mkdir -p "$output_dir"
for spec in '5EED2026 4/4 120 0 0.5' 'A1B2C3D4 4/4 97 0.125 1' 'FFFFFFFF 5/4 220 0.25 0.25'; do
  set -- $spec
  seed=$1
  meter=$2
  bpm=$3
  phase=$4
  variation=$5
  meter_key=${meter%/*}_${meter#*/}
  key="$seed-$meter_key-$bpm-$phase-$variation"
  "$fixture" convolution "$meter" loop "$seed" "$bpm" "$phase" 0 "$variation" 4 0.57 straight steady meet manual 1.5 \
    > "$output_dir/$key.json"
done
cd "$project_root"
"$node_program" tests/cyclic_bass_parity.cjs "$output_dir"
