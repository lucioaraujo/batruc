// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
// Percussão no tom da seed. Critérios: sem tom, o áudio é o de antes
// (bit a bit); com tom, a membrana muda de altura e cai numa nota do tom
// (pico espectral medido); o chimbal não muda.
#include "core/BassComposition.hpp"
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
std::vector<float> render(SoundCharacter sound, int tonic, int scale) {
    CompositionRecipe recipe = makeInitialRecipe();
    recipe.voiceCount = 2;
    recipe.sectionSeconds = 2.0;
    recipe.patternMode = PatternMode::unrestricted;
    recipe.rhythmicCells[0].enabled = true;
    insertRhythmicCellAttack(recipe.rhythmicCells[0], 0.0, 0.9f);
    recipe.rhythmicCells[1].enabled = true;
    EventPlan plan;
    if (buildEventPlan(recipe, plan) != PlanError::none) return {};
    ResonatorRenderer renderer;
    if (!renderer.configure(recipe, plan, 48000.0)) return {};
    renderer.setChannelSound(0, sound);
    renderer.setRhythmMode(true);
    renderer.setPercussionKey(tonic, scale);
    std::vector<float> left(48000), right(48000);
    renderer.process(left.data(), right.data(), left.size());
    return left;
}
// frequência dominante por varredura (DFT em grade de 1/8 de semitom)
double dominantHz(const std::vector<float>& x, double lo, double hi) {
    double bestHz = 0.0, bestPower = -1.0;
    for (double m = 69.0 + 12.0 * std::log2(lo / 440.0); m <= 69.0 + 12.0 * std::log2(hi / 440.0); m += 0.125) {
        const double hz = 440.0 * std::pow(2.0, (m - 69.0) / 12.0);
        double re = 0.0, im = 0.0;
        for (std::size_t i = 0; i < 9600; ++i) {
            const double w = 0.5 - 0.5 * std::cos(6.283185307 * i / 9600.0);
            re += w * x[i] * std::cos(6.283185307 * hz * i / 48000.0);
            im += w * x[i] * std::sin(6.283185307 * hz * i / 48000.0);
        }
        const double power = re * re + im * im;
        if (power > bestPower) { bestPower = power; bestHz = hz; }
    }
    return bestHz;
}
double maxDifference(const std::vector<float>& a, const std::vector<float>& b) {
    double d = 0.0;
    for (std::size_t i = 0; i < std::min(a.size(), b.size()); ++i)
        d = std::max(d, static_cast<double>(std::abs(a[i] - b[i])));
    return d;
}
} // namespace

int main() {
    const auto plain = render(SoundCharacter::membrane, -1, 0);
    check(!plain.empty(), "render");
    // Fá♯ maior: a membrana (145 Hz ≈ Ré3 −0,3) deve ir a uma nota do tom
    const int tonic = 6, scale = 0;
    const auto tuned = render(SoundCharacter::membrane, tonic, scale);
    const double before = dominantHz(plain, 60.0, 400.0);
    const double after = dominantHz(tuned, 60.0, 400.0);
    const double midi = 69.0 + 12.0 * std::log2(after / 440.0);
    const int nearest = static_cast<int>(std::lround(midi));
    const int degree = ((nearest - tonic) % 12 + 12) % 12;
    bool inKey = false;
    for (const int tone : kBassKeyScales[0]) inKey = inKey || tone == degree;
    check(std::abs(midi - nearest) < 0.2, "membrana afinada num semitom");
    check(inKey, "membrana numa nota do tom");
    check(std::abs(after - before) > 1.0, "a membrana mudou de altura");
    const auto hatPlain = render(SoundCharacter::hihat, -1, 0);
    const auto hatKey = render(SoundCharacter::hihat, tonic, scale);
    check(maxDifference(hatPlain, hatKey) < 1.0e-7, "o chimbal não muda");
    const auto kickKey = render(SoundCharacter::kick, tonic, scale);
    check(!kickKey.empty(), "bumbo renderiza");
    double energy = 0.0;
    for (const float v : plain) energy += static_cast<double>(v) * v;
    std::printf("dif membrana %.2e · rms %.2e\n", maxDifference(plain, tuned),
                std::sqrt(energy / static_cast<double>(plain.size())));
    std::printf("membrana %.1f Hz → %.1f Hz (MIDI %.2f) · chimbal dif %.1e\n",
                before, after, midi, maxDifference(hatPlain, hatKey));
    if (failures == 0) std::printf("percussion_key: OK\n");
    return failures == 0 ? 0 : 1;
}
