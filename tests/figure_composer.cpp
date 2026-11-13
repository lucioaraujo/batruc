// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
// Camada modelo → figuras (AUDITORIA_MUSICAL_2026-11-12.md, lote 1):
// determinismo, validade das células, âncora da voz 1, diferença entre seeds
// e frase A A' A B (o 3º compasso repete o 1º).
#include "core/FigureComposer.hpp"

#include <cmath>
#include <cstdlib>
#include <cstdio>

using namespace batruc;

namespace {
int failures = 0;
void check(bool ok, const char* what) {
    if (!ok) { std::printf("FALHOU: %s\n", what); ++failures; }
}
bool sameCells(const FigureComposition& a, const FigureComposition& b) {
    for (std::size_t c = 0; c < kFormalCycleCount; ++c)
        for (std::size_t v = 0; v < 3; ++v) {
            const auto& x = a.cells[c][v];
            const auto& y = b.cells[c][v];
            if (x.count != y.count) return false;
            for (std::size_t i = 0; i < x.count; ++i)
                if (x.positions[i] != y.positions[i] || x.strengths[i] != y.strengths[i])
                    return false;
        }
    return true;
}
} // namespace

int main() {
    FigureComposerParameters p;
    p.seed = 0x5EED2027u;
    FigureComposition a, b, c;
    check(composeFigures(p, a), "compõe a seed 5EED2027");
    check(composeFigures(p, b), "compõe de novo");
    check(sameCells(a, b), "mesma seed, mesmas figuras");
    p.seed = 0x1A2B3C4Du;
    check(composeFigures(p, c), "compõe a seed 1A2B3C4D");
    check(!sameCells(a, c), "seeds diferentes, figuras diferentes");

    for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle) {
        for (std::size_t v = 0; v < 3; ++v)
            check(validRhythmicCell(a.cells[cycle][v]), "célula válida");
        const auto& v1 = a.cells[cycle][0];
        check(v1.count > 0 && v1.positions[0] == 0.0 && v1.strengths[0] >= 0.9f,
              "a voz 1 abre a frase no primeiro tempo");
        // em toda forma da frase algum compasso repete o motivo (A ou A',
        // que é A girado): a voz 3 (sem âncora) tem um compasso com a mesma
        // contagem do primeiro
        {
            const auto& cell = a.cells[cycle][2];
            int counts[4] = {0, 0, 0, 0};
            for (std::size_t i = 0; i < cell.count; ++i)
                ++counts[std::min(3, static_cast<int>(cell.positions[i] * 4.0))];
            // tolerância de 2: o encaixe com a âncora da V1 desloca ataques
            auto near = [&](int k) { return std::abs(counts[k] - counts[0]) <= 2; };
            check(near(1) || near(2) || near(3), "a frase retoma o motivo");
        }
    }
    // as vozes 2 e 3 não batem junto com a voz 1 (encaixe)
    for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle)
        for (std::size_t v = 1; v < 3; ++v)
            for (std::size_t i = 0; i < a.cells[cycle][v].count; ++i)
                for (std::size_t j = 0; j < a.cells[cycle][0].count; ++j)
                    check(std::abs(a.cells[cycle][v].positions[i]
                                   - a.cells[cycle][0].positions[j]) > 1.0e-9,
                          "encaixe: V2/V3 fora dos ataques da V1");
    // outros modelos (bilhar, Lissajous, Fourier, rolantes): válidos,
    // determinísticos, diferentes do pêndulo e entre si, com densidade que
    // não enche a grade inteira de toda voz em todo ciclo
    for (int m = 1; m < kFigureModelCount; ++m) {
        FigureComposerParameters q = p;
        q.model = static_cast<FigureModel>(m);
        FigureComposition x, y;
        check(composeFigures(q, x), "outro modelo compõe");
        check(composeFigures(q, y) && sameCells(x, y), "outro modelo determinístico");
        check(!sameCells(x, a), "outro modelo difere do pêndulo");
        std::size_t total = 0;
        bool anchored = true;
        for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle) {
            for (std::size_t v = 0; v < 3; ++v) total += x.cells[cycle][v].count;
            anchored = anchored && x.cells[cycle][0].count > 0
                && x.cells[cycle][0].positions[0] < 1.0e-9;
        }
        check(anchored, "outro modelo: V1 abre cada frase");
        check(total > 7 * 3 * 4 && total < 7 * (33 + 64 + 64),
              "outro modelo: densidade entre o vazio e a grade cheia");
        std::printf("  %s: %zu ataques (%s)\n", figureModelName(q.model), total,
                    x.summary.substr(0, 60).c_str());
    }
    if (failures == 0) std::printf("figure_composer: OK (%s)\n", a.summary.c_str());
    return failures == 0 ? 0 : 1;
}
