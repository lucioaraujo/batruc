// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
// Tom da seed (12/11/2026): toda nota quantizada pertence à escala; a tônica
// mais próxima é tônica; tonic < 0 mantém o cromático.
#include "core/BassComposition.hpp"

#include <cstdio>

using namespace batruc;

int main() {
    int failures = 0;
    for (int scale = 0; scale < kBassKeyScaleCount; ++scale)
        for (int tonic = 0; tonic < 12; ++tonic)
            for (int midi = 20; midi <= 64; ++midi) {
                const int q = quantizeBassToKey(midi, tonic, scale);
                const int degree = ((q - tonic) % 12 + 12) % 12;
                bool inScale = false;
                for (const int step : kBassKeyScales[static_cast<std::size_t>(scale)])
                    if (step == degree) inScale = true;
                if (!inScale || q < 24 || q > 60) { ++failures; break; }
            }
    for (int tonic = 0; tonic < 12; ++tonic)
        if ((nearestBassTonic(37, tonic) - tonic) % 12 != 0) ++failures;
    if (quantizeBassToKey(37, -1, 0) != 37) ++failures;   // cromático intacto
    if (failures == 0) std::printf("bass_key: OK\n");
    else std::printf("bass_key: %d falhas\n", failures);
    return failures == 0 ? 0 : 1;
}
