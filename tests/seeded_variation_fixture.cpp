// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/CompositionCore.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

namespace {

std::uint32_t hashSeed(const std::string& seed) {
    std::uint32_t hash = 2166136261u;
    for (const unsigned char character : seed)
        hash = (hash ^ character) * 16777619u;
    return hash;
}

} // namespace

int main(int argc, char** argv) {
    using namespace batruc;
    if (argc != 3)
        return 2;
    const std::string seed(argv[1]);
    if (seed.size() != 8)
        return 2;
    char* end = nullptr;
    const float amount = std::strtof(argv[2], &end);
    if (end == argv[2] || *end != '\0' || !std::isfinite(amount)
        || amount < 0.0f || amount > 1.0f)
        return 2;

    constexpr std::array<float, 12> baseCycles = {
        3.0f, 4.0f, 0.5f, 0.75f, 1.0f, 1.25f,
        1.5f, 1.75f, 2.0f, 2.5f, 3.0f, 3.5f};
    constexpr std::array<float, 12> baseCouplings = {
        0.0f, 0.28f, 0.28f, 0.28f, 0.28f, 0.28f,
        0.28f, 0.28f, 0.28f, 0.28f, 0.28f, 0.28f};
    SeedVariationParameters parameters;
    parameters.seed = hashSeed(seed);
    parameters.amount = amount;
    parameters.baseTransforms.fill(static_cast<int>(VoiceTransform::sine));
    for (std::size_t voice = 0; voice < kMaxActiveVoices; ++voice) {
        parameters.baseCycles[voice] = baseCycles[voice];
        parameters.baseCouplings[voice] = baseCouplings[voice];
        const float stagger = voice == 0 ? 0.0f : static_cast<float>(voice - 1) / 16.0f;
        const float basePhase = voice >= 2
            ? 0.137f * static_cast<float>(voice - 1) : 0.0f;
        const float phase = stagger + basePhase;
        parameters.basePhases[voice] = phase - std::floor(phase + 0.5f);
    }

    const auto result = realizeSeedVariation(parameters);
    std::cout << std::setprecision(9) << "{\"cycles\":[";
    for (std::size_t voice = 0; voice < kMaxActiveVoices; ++voice) {
        if (voice) std::cout << ',';
        std::cout << result.cycles[voice];
    }
    std::cout << "],\"couplings\":[";
    for (std::size_t voice = 0; voice < kMaxActiveVoices; ++voice) {
        if (voice) std::cout << ',';
        std::cout << result.couplings[voice];
    }
    std::cout << "],\"phases\":[";
    for (std::size_t voice = 0; voice < kMaxActiveVoices; ++voice) {
        if (voice) std::cout << ',';
        std::cout << result.phases[voice];
    }
    std::cout << "],\"transforms\":[";
    for (std::size_t voice = 0; voice < kMaxActiveVoices; ++voice) {
        if (voice) std::cout << ',';
        std::cout << result.transforms[voice];
    }
    std::cout << "]}\n";
    return 0;
}
