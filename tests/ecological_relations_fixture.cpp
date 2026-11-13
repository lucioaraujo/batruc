// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/EcologicalRelations.hpp"

#include <array>
#include <cstdio>

int main() {
    using namespace batruc;
    CompositionRecipe recipe;
    recipe.sectionSeconds = 4.0;
    recipe.voiceCount = 5;

    EventPlan source;
    constexpr std::array<double, 10> times = {
        0.5, 0.8, 1.0, 1.2, 1.4, 1.5, 1.9, 2.0, 2.2, 2.4};
    constexpr std::array<std::uint8_t, 10> voices = {0, 1, 2, 3, 4, 0, 1, 2, 3, 4};
    constexpr std::array<float, 10> intensities = {
        0.4f, 0.5f, 0.3f, 0.25f, 0.8f, 0.6f, 0.5f, 0.45f, 0.7f, 0.6f};
    for (std::size_t i = 0; i < times.size(); ++i) {
        MusicalEvent event;
        event.timeSeconds = times[i];
        event.voice = voices[i];
        event.intensity = intensities[i];
        event.fieldX = static_cast<float>(i) / 10.0f;
        event.fieldY = -event.fieldX;
        source.events.push_back(event);
    }

    std::array<VoiceRelationRule, kMaxVoices> rules{};
    rules[0].relation = EcologicalRelation::none;
    rules[1] = {EcologicalRelation::imitation, 0, 0.8f, 1.0, 0.25};
    rules[2] = {EcologicalRelation::avoidance, 0, 0.8f, 1.0, 0.0};
    rules[3] = {EcologicalRelation::support, 0, 0.8f, 1.0, 0.0};
    rules[4] = {EcologicalRelation::competition, 0, 0.8f, 1.0, 0.0};

    EventPlan result;
    std::array<EcologicalEventTrace, 10> trace{};
    std::size_t traceCount = 0;
    const auto error = buildEcologicalEventPlan(recipe, source, rules, result,
        trace.data(), trace.size(), &traceCount);
    if (error != EcologicalPlanError::none || traceCount != source.events.size())
        return 1;

    for (std::size_t i = 0; i < traceCount; ++i) {
        const auto& item = trace[i];
        std::printf("t %zu %d %zu %.17g %.17g %.9g %.9g\n", i,
            item.perceived ? 1 : 0, item.perceivedEventIndex,
            item.memoryAgeSeconds, item.resultingTimeSeconds,
            item.originalIntensity, item.resultingIntensity);
    }
    for (const auto& event : result.events)
        std::printf("e %u %.17g %.9g\n", static_cast<unsigned>(event.voice),
                    event.timeSeconds, event.intensity);
    return 0;
}
