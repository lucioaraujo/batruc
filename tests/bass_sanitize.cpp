// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
// Uma linha de baixo com uma nota fora das faixas era recusada INTEIRA pelo
// renderer, e o app seguia com a linha fixa da abertura (relato do autor,
// 12/11/2026). sanitizeBassPerformance leva tudo às faixas.
#include "core/BassAudio.hpp"

#include <cstdio>

using namespace batruc;

int main() {
    BassPerformance performance;
    performance.sectionSeconds = 8.0;
    performance.count = 3;
    performance.events[0] = {0.0, 0.5, 36, 1.5f};        // intensidade > 1
    performance.events[1] = {1.0, 0.5, 70, 0.8f};        // MIDI acima de 60
    performance.events[2] = {7.8, 0.9, 40, 0.6f};        // passa do fim da seção
    performance.events[1].pan = 0.5f;                    // pan fora de ±0,24
    BassAudioRenderer before;
    const bool rejected = !before.configure(performance, 48000.0);
    sanitizeBassPerformance(performance);
    BassAudioRenderer after;
    const bool accepted = after.configure(performance, 48000.0);
    const bool kept = performance.count == 3
        && performance.events[0].velocity == 1.0f
        && performance.events[1].midi == 60 && performance.events[1].pan == 0.24f
        && performance.events[2].durationSeconds <= 0.2 + 1e-9;
    if (!rejected) std::printf("FALHOU: a linha original deveria ser recusada\n");
    if (!accepted) std::printf("FALHOU: a linha saneada deveria ser aceita\n");
    if (!kept) std::printf("FALHOU: o saneamento deveria manter as 3 notas, nas faixas\n");
    const bool ok = rejected && accepted && kept;
    if (ok) std::printf("bass_sanitize: OK\n");
    return ok ? 0 : 1;
}
