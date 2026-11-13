// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/CompositionCore.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <map>
#include <utility>

using namespace batruc;

namespace {

using EventKey = std::pair<double, std::uint8_t>;

std::map<EventKey, std::size_t> countByTimeAndVoice(const EventPlan& plan) {
    std::map<EventKey, std::size_t> result;
    for (const auto& event : plan.events)
        ++result[{event.timeSeconds, event.voice}];
    return result;
}

bool sameEvents(const EventPlan& first, const EventPlan& second) {
    if (first.events.size() != second.events.size())
        return false;
    for (std::size_t index = 0; index < first.events.size(); ++index) {
        const auto& a = first.events[index];
        const auto& b = second.events[index];
        if (a.timeSeconds != b.timeSeconds || a.voice != b.voice
            || a.rule != b.rule || a.phaseCycles != b.phaseCycles
            || a.intensity != b.intensity || a.fieldX != b.fieldX
            || a.fieldY != b.fieldY)
            return false;
    }
    return true;
}

} // namespace

int main() {
    auto recipe = makeInitialRecipe();
    recipe.voiceCount = 3;
    recipe.voices[0].cyclesPerSection = 3.0;
    recipe.voices[1].cyclesPerSection = 4.0;
    recipe.voices[2] = recipe.voices[1];
    recipe.voices[2].cyclesPerSection = 5.0;
    recipe.voices[1].eventRule = EventRule::turningPoints;

    EventPlan original;
    if (buildEventPlan(recipe, original) != PlanError::none)
        return 1;
    const auto originalSnapshot = original;
    EventPlan inverted;
    if (!invertEventPlanVoices(recipe, original, 0, 1, inverted)
        || inverted.events.size() != original.events.size()
        || !sameEvents(original, originalSnapshot))
        return 2;

    auto expected = countByTimeAndVoice(original);
    std::map<EventKey, std::size_t> swapped;
    for (const auto& entry : expected) {
        auto voice = entry.first.second;
        if (voice == 0) voice = 1;
        else if (voice == 1) voice = 0;
        swapped[{entry.first.first, voice}] += entry.second;
    }
    if (countByTimeAndVoice(inverted) != swapped)
        return 3;
    for (std::size_t index = 1; index < inverted.events.size(); ++index) {
        const auto& previous = inverted.events[index - 1];
        const auto& current = inverted.events[index];
        if (previous.timeSeconds > current.timeSeconds
            || (previous.timeSeconds == current.timeSeconds
                && previous.voice > current.voice))
            return 4;
    }

    EventPlan restored;
    if (!invertEventPlanVoices(recipe, inverted, 0, 1, restored)
        || !sameEvents(original, restored))
        return 5;
    if (invertEventPlanVoices(recipe, original, 0, 0, restored)
        || !sameEvents(original, restored))
        return 6;

    EventPlan invalid = original;
    invalid.events[0].timeSeconds = recipe.sectionSeconds;
    if (invertEventPlanVoices(recipe, invalid, 0, 1, restored)
        || !sameEvents(original, restored))
        return 7;

    std::array<EventPlan, 2> variants = {original, inverted};
    ResonatorRenderer renderer;
    if (!renderer.configureVariants(recipe, variants.data(), variants.size(), 48'000.0)) {
        std::fputs("prepared inversion was rejected by renderer\n", stderr);
        return 8;
    }
    return 0;
}
