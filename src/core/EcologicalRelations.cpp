// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/EcologicalRelations.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

namespace batruc {
namespace {

bool finite(const double value) noexcept {
    return std::isfinite(value);
}

bool finite(const float value) noexcept {
    return std::isfinite(value);
}

bool validRelation(const EcologicalRelation relation) noexcept {
    return static_cast<std::uint8_t>(relation)
        <= static_cast<std::uint8_t>(EcologicalRelation::callResponse);
}

std::uint32_t responseHash(std::uint32_t value) noexcept {
    value ^= value >> 16u;
    value *= 0x7feb352du;
    value ^= value >> 15u;
    value *= 0x846ca68bu;
    value ^= value >> 16u;
    return value;
}

bool selectsResponse(const VoiceRelationRule& rule,
                     const std::size_t targetVoice,
                     const std::size_t sourceOrdinal,
                     const std::size_t sourceEventCount) noexcept {
    if (rule.strength <= 0.0f || sourceEventCount == 0)
        return false;
    const auto quota = std::min(sourceEventCount, std::max<std::size_t>(1,
        static_cast<std::size_t>(std::lround(
            static_cast<double>(rule.strength) * sourceEventCount))));
    if (quota == sourceEventCount)
        return true;
    const auto input = rule.seed
        ^ (static_cast<std::uint32_t>(targetVoice + 1u) * 0x9e3779b9u);
    const auto offset = static_cast<std::size_t>(responseHash(input))
        % sourceEventCount;
    const auto rotatedOrdinal = (sourceOrdinal + offset) % sourceEventCount;
    // Stratified selection returns exactly `quota` responses, distributed
    // across the source cycle; the seed rotates which attacks are selected.
    return ((rotatedOrdinal + 1u) * quota) / sourceEventCount
        != (rotatedOrdinal * quota) / sourceEventCount;
}

} // namespace

EcologicalPlanError buildEcologicalEventPlan(
    const CompositionRecipe& recipe,
    const EventPlan& source,
    const std::array<VoiceRelationRule, kMaxVoices>& rules,
    EventPlan& result,
    EcologicalEventTrace* trace,
    const std::size_t traceCapacity,
    std::size_t* traceCount) {
    if (traceCount != nullptr)
        *traceCount = 0;
    if (recipe.voiceCount == 0 || recipe.voiceCount > kMaxVoices
        || !finite(recipe.sectionSeconds) || recipe.sectionSeconds <= 0.0)
        return EcologicalPlanError::invalidRecipe;
    if (source.events.size() > kMaxEventsPerSection)
        return EcologicalPlanError::eventLimitExceeded;
    if (trace != nullptr && traceCapacity < source.events.size())
        return EcologicalPlanError::traceCapacityExceeded;

    for (std::size_t voice = 0; voice < recipe.voiceCount; ++voice) {
        const auto& rule = rules[voice];
        if (!validRelation(rule.relation) || !finite(rule.strength)
            || rule.strength < 0.0f || rule.strength > 1.0f
            || !finite(rule.memorySeconds) || rule.memorySeconds < 0.0
            || rule.memorySeconds > recipe.sectionSeconds
            || !finite(rule.responseDelaySeconds)
            || rule.responseDelaySeconds < 0.0
            || rule.responseDelaySeconds > recipe.sectionSeconds
            || (rule.relation != EcologicalRelation::none
                && (rule.sourceVoice >= recipe.voiceCount
                    || rule.sourceVoice == voice)))
            return EcologicalPlanError::invalidRule;
    }

    std::array<std::size_t, kMaxVoices> responseSourceCounts{};
    for (std::size_t target = 0; target < recipe.voiceCount; ++target) {
        const auto& rule = rules[target];
        if (rule.relation != EcologicalRelation::callResponse)
            continue;
        responseSourceCounts[target] = static_cast<std::size_t>(std::count_if(
            source.events.begin(), source.events.end(), [&rule](const MusicalEvent& event) {
                return event.voice == rule.sourceVoice;
            }));
    }

    std::size_t generatedCount = 0;
    for (std::size_t target = 0; target < recipe.voiceCount; ++target) {
        const auto& rule = rules[target];
        if (rule.relation != EcologicalRelation::callResponse
            || rule.strength <= 0.0f)
            continue;
        std::size_t sourceOrdinal = 0;
        for (std::size_t index = 0; index < source.events.size(); ++index) {
            if (source.events[index].voice != rule.sourceVoice)
                continue;
            if (selectsResponse(rule, target, sourceOrdinal,
                                responseSourceCounts[target]))
                ++generatedCount;
            ++sourceOrdinal;
        }
    }
    if (generatedCount > kMaxEventsPerSection - source.events.size())
        return EcologicalPlanError::eventLimitExceeded;
    const auto outputCount = source.events.size() + generatedCount;
    if (trace != nullptr && traceCapacity < outputCount)
        return EcologicalPlanError::traceCapacityExceeded;

    double previousTime = -1.0;
    for (const auto& event : source.events) {
        if (event.voice >= recipe.voiceCount || !finite(event.timeSeconds)
            || event.timeSeconds < 0.0
            || event.timeSeconds >= recipe.sectionSeconds
            || event.timeSeconds < previousTime
            || !finite(event.intensity) || event.intensity < 0.0f
            || event.intensity > 1.0f || !finite(event.fieldX)
            || !finite(event.fieldY))
            return EcologicalPlanError::invalidInputPlan;
        previousTime = event.timeSeconds;
    }

    EventPlan prepared;
    prepared.events.reserve(outputCount);
    prepared.events = source.events;
    std::array<std::size_t, kMaxVoices> lastEvent{};
    std::array<double, kMaxVoices> lastEventTime{};
    lastEvent.fill(std::numeric_limits<std::size_t>::max());
    lastEventTime.fill(-1.0);

    for (std::size_t index = 0; index < prepared.events.size(); ++index) {
        auto& event = prepared.events[index];
        const auto voice = static_cast<std::size_t>(event.voice);
        const auto& rule = rules[voice];
        EcologicalEventTrace record;
        record.eventIndex = index;
        record.originalTimeSeconds = event.timeSeconds;
        record.resultingTimeSeconds = event.timeSeconds;
        record.originalIntensity = event.intensity;
        record.resultingIntensity = event.intensity;
        record.relation = rule.relation;

        if (rule.relation != EcologicalRelation::none && rule.strength > 0.0f) {
            const auto sourceVoice = static_cast<std::size_t>(rule.sourceVoice);
            const auto sourceIndex = lastEvent[sourceVoice];
            const double sourceTime = lastEventTime[sourceVoice];
            const double age = event.timeSeconds - sourceTime;
            if (sourceIndex != std::numeric_limits<std::size_t>::max()
                && age > 0.0 && age <= rule.memorySeconds) {
                const double recency = rule.memorySeconds > 0.0
                    ? std::clamp(1.0 - age / rule.memorySeconds, 0.0, 1.0)
                    : 0.0;
                const float influence = rule.strength
                    * static_cast<float>(recency);
                record.perceived = true;
                record.perceivedEventIndex = sourceIndex;
                record.memoryAgeSeconds = age;

                switch (rule.relation) {
                    case EcologicalRelation::none:
                        break;
                    case EcologicalRelation::support:
                        event.intensity += (1.0f - event.intensity) * influence;
                        break;
                    case EcologicalRelation::imitation: {
                        const double echoedTime = sourceTime
                            + rule.responseDelaySeconds;
                        event.timeSeconds += (echoedTime - event.timeSeconds)
                            * static_cast<double>(influence);
                        event.timeSeconds = std::clamp(event.timeSeconds, 0.0,
                            std::nextafter(recipe.sectionSeconds, 0.0));
                        break;
                    }
                    case EcologicalRelation::avoidance: {
                        const double separation = rule.memorySeconds;
                        if (separation > 0.0 && age < separation) {
                            const double forward = sourceTime + separation;
                            const double backward = std::max(0.0,
                                sourceTime - separation);
                            const double target = forward < recipe.sectionSeconds
                                ? forward : backward;
                            event.timeSeconds += (target - event.timeSeconds)
                                * static_cast<double>(influence);
                            event.timeSeconds = std::clamp(event.timeSeconds, 0.0,
                                std::nextafter(recipe.sectionSeconds, 0.0));
                        }
                        break;
                    }
                    case EcologicalRelation::competition:
                        event.intensity *= 1.0f - influence;
                        break;
                    case EcologicalRelation::callResponse:
                        break;
                }
            }
        }

        event.intensity = std::clamp(event.intensity, 0.0f, 1.0f);
        record.resultingTimeSeconds = event.timeSeconds;
        record.resultingIntensity = event.intensity;
        if (trace != nullptr)
            trace[index] = record;

        // The current event becomes available only to strictly later events;
        // simultaneous attacks cannot react to one another retroactively.
        lastEvent[voice] = index;
        lastEventTime[voice] = record.originalTimeSeconds;
    }

    for (std::size_t target = 0; target < recipe.voiceCount; ++target) {
        const auto& rule = rules[target];
        if (rule.relation != EcologicalRelation::callResponse
            || rule.strength <= 0.0f)
            continue;
        std::size_t sourceOrdinal = 0;
        for (std::size_t index = 0; index < source.events.size(); ++index) {
            const auto& sourceEvent = source.events[index];
            if (sourceEvent.voice != rule.sourceVoice)
                continue;
            const bool selected = selectsResponse(rule, target, sourceOrdinal,
                responseSourceCounts[target]);
            ++sourceOrdinal;
            if (!selected)
                continue;

            auto response = sourceEvent;
            response.voice = static_cast<std::uint8_t>(target);
            response.timeSeconds += rule.responseDelaySeconds;
            if (response.timeSeconds >= recipe.sectionSeconds)
                response.timeSeconds -= recipe.sectionSeconds;
            response.intensity = std::clamp(sourceEvent.intensity
                * (0.65f + 0.35f * rule.strength), 0.0f, 1.0f);
            response.generatedByRelation = true;
            const auto generatedIndex = prepared.events.size();
            prepared.events.push_back(response);

            if (trace != nullptr) {
                EcologicalEventTrace record;
                record.eventIndex = generatedIndex;
                record.perceivedEventIndex = index;
                record.perceived = true;
                record.memoryAgeSeconds = rule.responseDelaySeconds;
                record.originalTimeSeconds = sourceEvent.timeSeconds;
                record.resultingTimeSeconds = response.timeSeconds;
                record.originalIntensity = sourceEvent.intensity;
                record.resultingIntensity = response.intensity;
                record.relation = EcologicalRelation::callResponse;
                record.generated = true;
                record.generatedFromEventIndex = index;
                trace[generatedIndex] = record;
            }
        }
    }

    std::stable_sort(prepared.events.begin(), prepared.events.end(),
        [](const MusicalEvent& a, const MusicalEvent& b) {
            if (a.timeSeconds != b.timeSeconds)
                return a.timeSeconds < b.timeSeconds;
            return a.voice < b.voice;
        });
    result = std::move(prepared);
    if (traceCount != nullptr)
        *traceCount = outputCount;
    return EcologicalPlanError::none;
}

} // namespace batruc
