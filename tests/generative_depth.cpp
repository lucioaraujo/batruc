// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
// Profundidade do GERATIVO (12/11/2026; o autor: "mais próximo do volume do
// modo ritmo, podendo haver dinâmicas, mas não muito acentuadas"). Critérios:
// profundidade 0 toca igual ao RITMO; 1 é o comportamento anterior (difere
// de 0,3); no padrão 0,3 nenhum ataque cai abaixo de ~70 % do RITMO, e as
// camadas ainda agem (dinâmica leve, não nula).
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
std::vector<float> render(bool rhythm, bool hiddenLayers, float depth) {
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
    renderer.setGenerativeDepth(depth);
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
    const auto rhythm = render(true, true, 0.3f);
    const auto depth0 = render(false, true, 0.0f);
    const auto depth03 = render(false, true, 0.3f);
    const auto depth1 = render(false, true, 1.0f);
    check(!rhythm.empty() && !depth0.empty() && !depth03.empty() && !depth1.empty(), "render");
    const double diff0 = maxDifference(depth0, rhythm);
    const double diff03 = maxDifference(depth03, rhythm);
    const double diff1 = maxDifference(depth1, depth03);
    const double peakRhythm = minAttackPeak(rhythm);
    const double peak03 = minAttackPeak(depth03);
    const double peak1 = minAttackPeak(depth1);
    check(diff0 < 1.0e-6, "profundidade 0 = RITMO");
    check(diff03 > 1.0e-4, "profundidade 0,3: as camadas ainda agem");
    check(diff1 > 1.0e-3, "profundidade 1 difere do padrão");
    check(peak03 >= 0.65 * peakRhythm, "profundidade 0,3: nenhum ataque abaixo de ~70 % do RITMO");
    check(peak03 >= peak1, "profundidade 0,3 não é mais fraca que 1");
    std::printf("dif. p0 %.2e · p0,3 %.2e · p1 %.2e · menor pico RITMO %.4f · p0,3 %.4f · p1 %.4f\n",
                diff0, diff03, diff1, peakRhythm, peak03, peak1);
    if (failures == 0) std::printf("generative_depth: OK\n");
    return failures == 0 ? 0 : 1;
}
