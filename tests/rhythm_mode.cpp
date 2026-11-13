// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
// Modo RITMO (decisão do autor, 12/11/2026): o ritmo escrito toca como foi
// escrito. Critério: no RITMO, ligar as camadas escondidas (máscara de campo,
// lei de intensidade, groove e curva de expressão) NÃO muda o áudio; no
// GERATIVO, muda (contraprova). Também: no GERATIVO com máscara, os ataques
// escritos podem sumir; no RITMO, nenhum some.
#include "core/CompositionCore.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

using namespace batruc;

namespace {
int failures = 0;
void check(bool ok, const char* what) {
    if (!ok) { std::printf("FALHOU: %s\n", what); ++failures; }
}
std::vector<float> render(bool rhythm, bool hiddenLayers) {
    CompositionRecipe recipe = makeInitialRecipe();
    recipe.voiceCount = 2;
    recipe.sectionSeconds = 2.0;
    recipe.patternMode = hiddenLayers ? PatternMode::adhesivePads : PatternMode::unrestricted;
    auto& cell = recipe.rhythmicCells[0];
    cell.enabled = true;
    for (const double position : {0.0, 0.25, 0.5, 0.75})
        insertRhythmicCellAttack(cell, position, 0.8f);
    recipe.rhythmicCells[1].enabled = true;   // voz 2 muda
    EventPlan plan;
    if (buildEventPlan(recipe, plan) != PlanError::none) return {};
    ResonatorRenderer renderer;
    if (!renderer.configure(recipe, plan, 48000.0)) return {};
    renderer.setChannelSound(0, SoundCharacter::kick);
    if (hiddenLayers) {
        renderer.setIntensityFormula(IntensityLaw::cosineBands, 1.0f);
        renderer.setGrooveMode(GrooveMode::offbeat);
        renderer.setPerformanceExpression(DynamicCurve::crescendo, MusicalIntention::contrast);
    } else {
        renderer.setIntensityFormula(IntensityLaw::uniform, 0.0f);
        renderer.setGrooveMode(GrooveMode::straight);
        renderer.setPerformanceExpression(DynamicCurve::steady, MusicalIntention::build);
    }
    renderer.setRhythmMode(rhythm);
    std::vector<float> left(96000), right(96000);
    renderer.process(left.data(), right.data(), left.size());
    return left;
}
double maxDifference(const std::vector<float>& a, const std::vector<float>& b) {
    double d = 0.0;
    for (std::size_t i = 0; i < std::min(a.size(), b.size()); ++i)
        d = std::max(d, static_cast<double>(std::abs(a[i] - b[i])));
    return d;
}
double minAttackPeak(const std::vector<float>& x) {
    double lowest = 1e9;
    for (int k = 0; k < 4; ++k) {
        double peak = 0.0;
        for (std::size_t i = static_cast<std::size_t>(k) * 24000; i < static_cast<std::size_t>(k) * 24000 + 3840; ++i)
            peak = std::max(peak, static_cast<double>(std::abs(x[i])));
        lowest = std::min(lowest, peak);
    }
    return lowest;
}
} // namespace

int main() {
    const auto rhythmWithLayers = render(true, true);
    const auto rhythmPlain = render(true, false);
    const auto generativeWithLayers = render(false, true);
    const auto generativePlain = render(false, false);
    check(!rhythmWithLayers.empty() && !generativeWithLayers.empty(), "render");
    const double rhythmDiff = maxDifference(rhythmWithLayers, rhythmPlain);
    const double generativeDiff = maxDifference(generativeWithLayers, generativePlain);
    check(rhythmDiff < 1.0e-6, "RITMO: as camadas escondidas não mudam o áudio");
    check(generativeDiff > 1.0e-3, "GERATIVO: as camadas continuam agindo (contraprova)");
    check(minAttackPeak(rhythmWithLayers) > 0.01, "RITMO: nenhum ataque escrito some");
    std::printf("diferença RITMO %.2e · GERATIVO %.2e · menor pico RITMO %.4f · GERATIVO %.4f\n",
                rhythmDiff, generativeDiff, minAttackPeak(rhythmWithLayers),
                minAttackPeak(generativeWithLayers));
    if (failures == 0) std::printf("rhythm_mode: OK\n");
    return failures == 0 ? 0 : 1;
}
