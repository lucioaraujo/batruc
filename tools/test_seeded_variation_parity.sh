#!/bin/sh
# SPDX-License-Identifier: AGPL-3.0-or-later
set -eu

fixture=$1
output_dir=$2
project_root=$3
node_program=${BATRUC_NODE_EXECUTABLE:-node}
mkdir -p "$output_dir"
for seed in A1B2C3D4 00000001 5EED2026 FFFFFFFF; do
  for amount in 0 0.25 0.5 1; do
    key=$(printf '%s' "$amount" | tr '.' '_')
    "$fixture" "$seed" "$amount" > "$output_dir/$seed-$key.json"
  done
done
cd "$project_root"
"$node_program" tests/compare_seeded_variation.cjs "$output_dir"
