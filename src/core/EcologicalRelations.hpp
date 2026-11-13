// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "core/CompositionCore.hpp"

namespace batruc {

enum class EcologicalRelation : std::uint8_t {
    none,
    support,
    imitation,
    avoidance,
    competition,
    callResponse
};

struct VoiceRelationRule {
    EcologicalRelation relation = EcologicalRelation::none;
    std::uint8_t sourceVoice = 0;
    float strength = 0.0f;
    double memorySeconds = 0.0;
    double responseDelaySeconds = 0.0;
    std::uint32_t seed = 0;
};

struct EcologicalEventTrace {
    std::size_t eventIndex = 0;
    std::size_t perceivedEventIndex = 0;
    bool perceived = false;
    double memoryAgeSeconds = 0.0;
    double originalTimeSeconds = 0.0;
    double resultingTimeSeconds = 0.0;
    float originalIntensity = 0.0f;
    float resultingIntensity = 0.0f;
    EcologicalRelation relation = EcologicalRelation::none;
    bool generated = false;
    std::size_t generatedFromEventIndex = 0;
};

enum class EcologicalPlanError : std::uint8_t {
    none,
    invalidRecipe,
    invalidInputPlan,
    invalidRule,
    eventLimitExceeded,
    traceCapacityExceeded
};

// Derives a bounded, repeatable event plan from an existing plan. Existing
// relations perceive only the most recent earlier source event. callResponse
// can add seeded, delayed copies of source attacks to a target voice. No
// audio-thread work or unseeded randomness is involved.
EcologicalPlanError buildEcologicalEventPlan(
    const CompositionRecipe& recipe,
    const EventPlan& source,
    const std::array<VoiceRelationRule, kMaxVoices>& rules,
    EventPlan& result,
    EcologicalEventTrace* trace = nullptr,
    std::size_t traceCapacity = 0,
    std::size_t* traceCount = nullptr);

} // namespace batruc
