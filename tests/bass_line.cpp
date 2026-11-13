// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
// Baixo com voz própria (auditoria §7, item 5). Critérios: todas as notas no
// tom da seed; a peça começa na tônica; cada compasso tem a fundamental no
// primeiro tempo; os 7 ciclos têm baixo; o ciclo 3 respira (só tempos
// fortes); determinístico; seeds diferentes dão linhas diferentes.
#include "core/BassLineComposer.hpp"

#include <cstdio>
#include <set>
#include <string>
#include <vector>

using namespace batruc;

namespace {
int failures = 0;
void check(bool ok, const char* what, std::uint32_t seed) {
    if (!ok) { std::printf("FALHOU (%08X): %s\n", seed, what); ++failures; }
}
bool inKey(int midi, int tonic, int scale) {
    const int degree = ((midi - tonic) % 12 + 12) % 12;
    for (const int tone : kBassKeyScales[static_cast<std::size_t>(scale)])
        if (tone == degree) return true;
    return false;
}
} // namespace

int main() {
    std::set<std::string> lines;
    const std::uint32_t seeds[] = {0x5EED2027u, 0xC0FFEE42u, 0xD3BFA3CFu, 0x00000001u,
                                   0x12345678u, 0xCAFEBABEu, 0x0BADF00Du, 0x7777AAAAu,
                                   0x31415926u, 0x27182818u, 0xFEEDBEEFu, 0x10203040u};
    for (const auto seed : seeds) {
        FigureComposerParameters figureParameters;
        figureParameters.seed = seed;
        FigureComposition figures;
        check(composeFigures(figureParameters, figures), "figuras", seed);
        BassLineParameters parameters;
        parameters.seed = seed;
        seedKey(seed, parameters.keyTonic, parameters.keyScale);
        BassPerformance a, b;
        std::string summary;
        check(composeBassLine(figures, parameters, a, &summary), "compõe", seed);
        check(composeBassLine(figures, parameters, b), "compõe de novo", seed);
        bool same = a.count == b.count;
        for (std::size_t i = 0; same && i < a.count; ++i)
            same = a.events[i].midi == b.events[i].midi
                && a.events[i].timeSeconds == b.events[i].timeSeconds;
        check(same, "determinístico", seed);

        int perCycle[7]{};
        bool allInKey = true, downbeats = true, startsOnTonic = false;
        bool barHasDownbeat[7][4]{};
        std::string line;
        for (std::size_t i = 0; i < a.count; ++i) {
            const auto& e = a.events[i];
            ++perCycle[e.formCycle];
            allInKey = allInKey && inKey(e.midi, parameters.keyTonic, parameters.keyScale);
            const int bar = e.sourceNote / 16;
            if (e.sourceNote % 16 == 0 && bar < 4) barHasDownbeat[e.formCycle][bar] = true;
            if (e.formCycle == 0 && e.sourceNote == 0)
                startsOnTonic = (e.midi - parameters.keyTonic) % 12 == 0;
            line += std::to_string(e.formCycle) + ":" + std::to_string(e.sourceNote)
                + "=" + std::to_string(e.midi) + " ";
        }
        for (auto& cycle : barHasDownbeat)
            for (const bool has : cycle) downbeats = downbeats && has;
        bool everyCycle = true;
        for (const int n : perCycle) everyCycle = everyCycle && n > 0;
        check(allInKey, "todas as notas no tom", seed);
        check(startsOnTonic, "começa na tônica", seed);
        check(downbeats, "fundamental no primeiro tempo de cada compasso", seed);
        check(everyCycle, "baixo nos 7 ciclos", seed);
        check(perCycle[2] <= 8, "ciclo 3 respira", seed);
        lines.insert(line);
        std::printf("%08X  %s · por ciclo %d %d %d %d %d %d %d\n", seed, summary.c_str(),
                    perCycle[0], perCycle[1], perCycle[2], perCycle[3],
                    perCycle[4], perCycle[5], perCycle[6]);
    }
    check(lines.size() == std::size(seeds), "seeds diferentes, linhas diferentes", 0);
    if (failures == 0) std::printf("bass_line: OK\n");
    return failures == 0 ? 0 : 1;
}
