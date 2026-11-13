#!/bin/sh
# SPDX-License-Identifier: AGPL-3.0-or-later
set -eu

fixture=$1
output_dir=$2
project_root=$3
node_program=${BATRUC_NODE_EXECUTABLE:-node}
mkdir -p "$output_dir"
for seed in A1B2C3D4 00000001 5EED2026 FFFFFFFF; do
  for variation in 0.25 0.5 1; do
    for phrase in loop breath cadence call-response; do
      for relation in independent follow answer; do
        for meter in 3/4 4/4 5/4 7/8; do
          for bpm in 97 120 220; do
            if [ "$meter" = 5/4 ] && [ "$bpm" = 97 ]; then
              continue
            fi
            for phase in 0 0.125 0.25; do
              meter_key=${meter%/*}_${meter#*/}
              key="$seed-$variation-$phrase-$relation-$meter_key-$bpm-$phase-v4-s0.57-gstraight-dsteady-imeet-lmanual-r1.5"
              "$fixture" "$relation" "$meter" "$phrase" "$seed" "$bpm" "$phase" "$variation" "$variation" 4 0.57 straight steady meet manual 1.5 \
                > "$output_dir/$key.json"
            done
          done
        done
      done
    done
  done
done

for seed in A1B2C3D4 00000001 5EED2026 FFFFFFFF; do
  for variation in 0.5 1; do
    for phrase in loop call-response; do
      for relation in independent follow answer; do
        for voice_count in 2 4 8 12; do
          for swing in 0.5 0.57 0.67; do
            for groove in straight ternary additive offbeat; do
              key="$seed-$variation-$phrase-$relation-4_4-120-0.125-v$voice_count-s$swing-g$groove-dsteady-imeet-lmanual-r1.5"
              "$fixture" "$relation" 4/4 "$phrase" "$seed" 120 0.125 "$variation" "$variation" "$voice_count" "$swing" "$groove" steady meet manual 1.5 \
                > "$output_dir/$key.json"
            done
          done
        done
      done
    done
  done
done

for seed in A1B2C3D4 00000001 5EED2026 FFFFFFFF; do
  for phrase in loop call-response; do
    for relation in independent follow answer; do
      for groove in straight ternary additive offbeat; do
        for dynamics in steady crescendo arch waves; do
          for intention in build thin meet contrast; do
            key="$seed-0.5-$phrase-$relation-4_4-120-0.125-v4-s0.57-g$groove-d$dynamics-i$intention-lmanual-r1.5"
            "$fixture" "$relation" 4/4 "$phrase" "$seed" 120 0.125 0.5 0.5 4 0.57 "$groove" "$dynamics" "$intention" manual 1.5 \
              > "$output_dir/$key.json"
          done
        done
      done
    done
  done
done

for seed in A1B2C3D4 00000001 5EED2026 FFFFFFFF; do
  for variation in 0.5 1; do
    for phrase in loop breath cadence call-response; do
      for relation in independent follow answer; do
        for duration_law in manual fibonacci geometric; do
          ratios='1.5'
          [ "$duration_law" = geometric ] && ratios='1.2 1.5 1.618 2'
          for ratio in $ratios; do
            key="$seed-$variation-$phrase-$relation-4_4-120-0.125-v4-s0.57-gstraight-dsteady-imeet-l$duration_law-r$ratio"
            "$fixture" "$relation" 4/4 "$phrase" "$seed" 120 0.125 "$variation" "$variation" 4 0.57 straight steady meet "$duration_law" "$ratio" \
              > "$output_dir/$key.json"
          done
        done
      done
    done
  done
done
cd "$project_root"
"$node_program" tests/compare_seeded_voice_bass.cjs "$output_dir"
