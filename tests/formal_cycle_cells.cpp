// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
// Desenvolvimento ao longo da forma (12/11/2026): com frases por ciclo
// ligadas, a seção do ciclo N toca a frase N. Ciclo 1: ataque no início;
// ciclo 2: ataque no meio.
#include "core/CompositionCore.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

using namespace batruc;

int main() {
    CompositionRecipe recipe = makeInitialRecipe();
    recipe.voiceCount = 2;
    recipe.sectionSeconds = 2.0;
    recipe.rhythmicCells[0].enabled = true;   // célula única vazia
    recipe.rhythmicCells[1].enabled = true;
    EventPlan plan;
    if (buildEventPlan(recipe, plan) != PlanError::none) { std::printf("plano\n"); return 1; }
    ResonatorRenderer renderer;
    if (!renderer.configure(recipe, plan, 48000.0)) { std::printf("configure\n"); return 1; }
    renderer.setChannelSound(0, SoundCharacter::kick);
    renderer.setRhythmMode(true);
    ResonatorRenderer::FormalCycleCells cells{};
    for (auto& cycle : cells)
        for (auto& cell : cycle) cell.enabled = true;
    insertRhythmicCellAttack(cells[0][0], 0.0, 1.0f);   // ciclo 1: início
    insertRhythmicCellAttack(cells[1][0], 0.5, 1.0f);   // ciclo 2: meio
    renderer.setFormalCycleCells(cells, {true, false, false});
    std::vector<float> left(4 * 48000), right(4 * 48000);
    renderer.process(left.data(), right.data(), left.size());
    auto peak = [&](double from) {
        double p = 0.0;
        const auto start = static_cast<std::size_t>(from * 48000.0);
        for (std::size_t i = start; i < start + 2400; ++i)
            p = std::max(p, static_cast<double>(std::abs(left[i])));
        return p;
    };
    const double c1Start = peak(0.0), c1Middle = peak(1.0);
    const double c2Start = peak(2.0), c2Middle = peak(3.0);
    std::printf("ciclo 1: início %.4f meio %.4f · ciclo 2: início %.4f meio %.4f\n",
                c1Start, c1Middle, c2Start, c2Middle);
    const bool ok = c1Start > 0.02 && c2Middle > 0.02
        && c1Middle < 0.25 * c1Start && c2Start < 0.25 * c2Middle;
    std::printf(ok ? "formal_cycle_cells: OK\n" : "formal_cycle_cells: FALHOU\n");
    return ok ? 0 : 1;
}
