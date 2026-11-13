// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/GenerativeOperations.hpp"
#include "core/CompositionCore.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace batruc {
namespace {

bool validScore(const BassScore& score) noexcept {
    if (score.count > score.notes.size() || !std::isfinite(score.sectionSeconds)
        || score.sectionSeconds <= 0.0)
        return false;
    double lastTime = -1.0;
    for (std::size_t index = 0; index < score.count; ++index) {
        const auto& note = score.notes[index];
        if (!std::isfinite(note.timeSeconds) || note.timeSeconds < 0.0
            || note.timeSeconds < lastTime
            || !std::isfinite(note.durationSeconds) || note.durationSeconds < 0.0
            || note.timeSeconds + note.durationSeconds > score.sectionSeconds + 1.0e-9
            || score.stepsPerBar == 0 || score.stepsPerBar > kBassStepsPerBar
            || note.bar >= kBassBarsPerSection || note.step >= score.stepsPerBar
            || note.writtenMidi < 24 || note.writtenMidi > 60
            || note.calculatedMidi < 24 || note.calculatedMidi > 60)
            return false;
        lastTime = note.timeSeconds;
    }
    return true;
}

bool setGridPosition(BassNote& note, const double sectionSeconds,
                     const std::size_t stepsPerBar) noexcept {
    const auto stepsPerSection = stepsPerBar * kBassBarsPerSection;
    const double stepSeconds = sectionSeconds / stepsPerSection;
    const auto position = static_cast<long long>(
        std::llround(note.timeSeconds / stepSeconds));
    if (position < 0 || position >= static_cast<long long>(stepsPerSection)
        || std::abs(position * stepSeconds - note.timeSeconds) > 1.0e-7)
        return false;
    note.bar = static_cast<std::uint8_t>(position / stepsPerBar);
    note.step = static_cast<std::uint8_t>(position % stepsPerBar);
    return true;
}

} // namespace

bool invertChordVoicing(const ChordVoicing& source,
                        const std::size_t inversionCount,
                        ChordVoicing& result) noexcept {
    if (source.count == 0 || source.count > source.midi.size()
        || inversionCount >= source.count)
        return false;
    for (std::size_t index = 1; index < source.count; ++index)
        if (source.midi[index - 1] >= source.midi[index])
            return false;
    ChordVoicing prepared = source;
    for (std::size_t inversion = 0; inversion < inversionCount; ++inversion) {
        int lifted = prepared.midi[0];
        const int top = prepared.midi[prepared.count - 1];
        while (lifted <= top)
            lifted += 12;
        if (lifted > 127)
            return false;
        for (std::size_t index = 1; index < prepared.count; ++index)
            prepared.midi[index - 1] = prepared.midi[index];
        prepared.midi[prepared.count - 1] = static_cast<std::uint8_t>(lifted);
    }
    result = prepared;
    return true;
}

bool invertBassPitchContour(const BassScore& source, const int axisMidi,
                            BassScore& result) noexcept {
    if (!validScore(source) || axisMidi < 24 || axisMidi > 60)
        return false;
    BassScore prepared = source;
    for (std::size_t index = 0; index < prepared.count; ++index) {
        auto& note = prepared.notes[index];
        const int written = 2 * axisMidi - note.writtenMidi;
        const int calculated = 2 * axisMidi - note.calculatedMidi;
        if (written < 24 || written > 60 || calculated < 24 || calculated > 60)
            return false;
        note.writtenMidi = static_cast<std::uint8_t>(written);
        note.calculatedMidi = static_cast<std::uint8_t>(calculated);
    }
    result = prepared;
    return true;
}

bool retrogradeBassScore(const BassScore& source, BassScore& result) noexcept {
    if (!validScore(source))
        return false;
    BassScore prepared = source;
    for (std::size_t index = 0; index < source.count; ++index) {
        auto note = source.notes[source.count - 1 - index];
        note.timeSeconds = source.sectionSeconds
            - (note.timeSeconds + note.durationSeconds);
        if (!setGridPosition(note, source.sectionSeconds, source.stepsPerBar))
            return false;
        prepared.notes[index] = note;
    }
    std::sort(prepared.notes.begin(), prepared.notes.begin() + prepared.count,
              [](const BassNote& left, const BassNote& right) {
                  return left.timeSeconds < right.timeSeconds;
              });
    result = prepared;
    return true;
}

bool retrogradeBassPerformance(const BassPerformance& source,
                              BassPerformance& result) noexcept {
    if (source.count > source.events.size()
        || !std::isfinite(source.sectionSeconds) || source.sectionSeconds <= 0.0)
        return false;
    for (std::size_t index = 0; index < source.count; ++index) {
        const auto& event = source.events[index];
        if (!std::isfinite(event.timeSeconds) || event.timeSeconds < 0.0
            || !std::isfinite(event.durationSeconds) || event.durationSeconds <= 0.0
            || event.timeSeconds + event.durationSeconds
                > source.sectionSeconds + 1.0e-9
            || !std::isfinite(event.velocity) || event.velocity < 0.0f
            || event.velocity > 1.0f || event.midi < 24 || event.midi > 60
            || event.formCycle > 7)
            return false;
    }

    BassPerformance prepared = source;
    for (std::size_t index = 0; index < prepared.count; ++index) {
        auto& event = prepared.events[index];
        event.timeSeconds = std::max(0.0, source.sectionSeconds
            - (event.timeSeconds + event.durationSeconds));
    }
    std::sort(prepared.events.begin(),
        prepared.events.begin() + static_cast<std::ptrdiff_t>(prepared.count),
        [](const BassPlayedEvent& left, const BassPlayedEvent& right) {
            if (left.timeSeconds != right.timeSeconds)
                return left.timeSeconds < right.timeSeconds;
            return left.formCycle < right.formCycle;
        });
    result = prepared;
    return true;
}

bool invertBassPerformancePitchContour(const BassPerformance& source,
                                       const int axisMidi,
                                       BassPerformance& result) noexcept {
    if (source.count > source.events.size()
        || !std::isfinite(source.sectionSeconds) || source.sectionSeconds <= 0.0
        || axisMidi < 24 || axisMidi > 60)
        return false;
    for (std::size_t index = 0; index < source.count; ++index) {
        const auto& event = source.events[index];
        if (!std::isfinite(event.timeSeconds) || event.timeSeconds < 0.0
            || !std::isfinite(event.durationSeconds) || event.durationSeconds <= 0.0
            || event.timeSeconds + event.durationSeconds
                > source.sectionSeconds + 1.0e-9
            || !std::isfinite(event.velocity) || event.velocity < 0.0f
            || event.velocity > 1.0f || event.midi < 24 || event.midi > 60
            || event.formCycle > 7)
            return false;
        const int invertedMidi = 2 * axisMidi - event.midi;
        if (invertedMidi < 24 || invertedMidi > 60)
            return false;
    }
    BassPerformance prepared = source;
    for (std::size_t index = 0; index < prepared.count; ++index)
        prepared.events[index].midi = static_cast<std::uint8_t>(
            2 * axisMidi - prepared.events[index].midi);
    result = prepared;
    return true;
}

bool recombineBassPerformanceBars(const BassPerformance& source,
                                  const std::size_t donorCycleOffset,
                                  BassPerformance& result) noexcept {
    if (source.count > source.events.size()
        || !std::isfinite(source.sectionSeconds) || source.sectionSeconds <= 0.0
        || donorCycleOffset == 0 || donorCycleOffset >= kFormalCycleCount)
        return false;
    for (std::size_t index = 0; index < source.count; ++index) {
        const auto& event = source.events[index];
        if (!std::isfinite(event.timeSeconds) || event.timeSeconds < 0.0
            || event.timeSeconds >= source.sectionSeconds
            || !std::isfinite(event.durationSeconds) || event.durationSeconds <= 0.0
            || event.timeSeconds + event.durationSeconds
                > source.sectionSeconds + 1.0e-9
            || !std::isfinite(event.velocity) || event.velocity < 0.0f
            || event.velocity > 1.0f || event.midi < 24 || event.midi > 60
            || event.formCycle >= kFormalCycleCount)
            return false;
    }

    const double barSeconds = source.sectionSeconds
        / static_cast<double>(kBassBarsPerSection);
    BassPerformance prepared = source;
    for (std::size_t index = 0; index < source.count; ++index) {
        const auto& target = source.events[index];
        const auto bar = std::min(kBassBarsPerSection - 1,
            static_cast<std::size_t>(target.timeSeconds / barSeconds));
        const double barStart = static_cast<double>(bar) * barSeconds;
        const double targetPhase = (target.timeSeconds - barStart) / barSeconds;
        const auto barDonorOffset = 1 + (donorCycleOffset - 1 + bar)
            % (kFormalCycleCount - 1);
        const auto donorCycle = (static_cast<std::size_t>(target.formCycle)
            + barDonorOffset) % kFormalCycleCount;
        const BassPlayedEvent* donor = nullptr;
        double nearestPhaseDistance = std::numeric_limits<double>::infinity();
        for (std::size_t candidateIndex = 0; candidateIndex < source.count;
             ++candidateIndex) {
            const auto& candidate = source.events[candidateIndex];
            if (candidate.formCycle != donorCycle
                || candidate.timeSeconds < barStart
                || candidate.timeSeconds >= barStart + barSeconds)
                continue;
            const double candidatePhase = (candidate.timeSeconds - barStart)
                / barSeconds;
            const double distance = std::abs(candidatePhase - targetPhase);
            if (distance < nearestPhaseDistance) {
                nearestPhaseDistance = distance;
                donor = &candidate;
            }
        }
        if (donor == nullptr)
            continue;

        auto& event = prepared.events[index];
        event.midi = donor->midi;
        event.velocity = donor->velocity;
        const double barEnd = barStart + barSeconds;
        double nextTargetTime = barEnd;
        for (std::size_t nextIndex = 0; nextIndex < source.count; ++nextIndex) {
            const auto& next = source.events[nextIndex];
            if (next.formCycle == target.formCycle
                && next.timeSeconds > target.timeSeconds + 1.0e-9
                && next.timeSeconds < nextTargetTime)
                nextTargetTime = next.timeSeconds;
        }
        event.durationSeconds = std::max(0.001,
            std::min(donor->durationSeconds, nextTargetTime - target.timeSeconds));
    }
    result = prepared;
    return true;
}

bool recombineBassBars(const BassScore& first, const BassScore& second,
                       const std::array<bool, kBassBarsPerSection>& useSecond,
                       BassScore& result) noexcept {
    if (!validScore(first) || !validScore(second)
        || first.stepsPerBar != second.stepsPerBar
        || std::abs(first.sectionSeconds - second.sectionSeconds) > 1.0e-9)
        return false;
    BassScore prepared;
    prepared.sectionSeconds = first.sectionSeconds;
    prepared.stepsPerBar = first.stepsPerBar;
    const double barSeconds = first.sectionSeconds / kBassBarsPerSection;
    for (std::size_t bar = 0; bar < kBassBarsPerSection; ++bar) {
        const auto& selected = useSecond[bar] ? second : first;
        const double barStart = bar * barSeconds;
        const double barEnd = (bar + 1) * barSeconds;
        for (std::size_t index = 0; index < selected.count; ++index) {
            const auto& note = selected.notes[index];
            if (note.timeSeconds < barStart || note.timeSeconds >= barEnd)
                continue;
            if (prepared.count >= prepared.notes.size())
                return false;
            auto copied = note;
            copied.durationSeconds = std::min(copied.durationSeconds,
                                             barEnd - copied.timeSeconds);
            copied.bar = static_cast<std::uint8_t>(bar);
            prepared.notes[prepared.count++] = copied;
        }
    }
    result = prepared;
    return true;
}

bool recalculateBassScore(const BassRecipe& editedRecipe,
                          BassScore& currentScore) noexcept {
    BassScore prepared;
    if (buildBassScore(editedRecipe, prepared) != BassError::none)
        return false;
    currentScore = prepared;
    return true;
}

bool inversePhaseRotation(const double phaseTurns,
                          double& inverseTurns) noexcept {
    if (!std::isfinite(phaseTurns))
        return false;
    const double wrapped = std::fmod(-phaseTurns, 1.0);
    inverseTurns = wrapped < 0.0 ? wrapped + 1.0 : wrapped;
    return true;
}

bool applyLogisticBassDynamics(BassPerformance& performance,
                               const LogisticMapParameters& parameters,
                               double* finalState) noexcept {
    if (performance.count > performance.events.size()
        || !std::isfinite(performance.sectionSeconds)
        || performance.sectionSeconds <= 0.0
        || !std::isfinite(parameters.r) || parameters.r < 3.5
        || parameters.r > 4.0
        || !std::isfinite(parameters.initialState)
        || parameters.initialState <= 0.0 || parameters.initialState >= 1.0
        || !std::isfinite(parameters.depth) || parameters.depth < 0.0f
        || parameters.depth > 0.85f || parameters.burnIn > 256)
        return false;

    std::array<std::size_t, kMaxBassPerformedEvents> order{};
    for (std::size_t index = 0; index < performance.count; ++index) {
        const auto& event = performance.events[index];
        if (event.formCycle >= kFormalCycleCount
            || !std::isfinite(event.timeSeconds) || event.timeSeconds < 0.0
            || event.timeSeconds >= performance.sectionSeconds
            || !std::isfinite(event.velocity) || event.velocity < 0.0f
            || event.velocity > 1.0f)
            return false;
        order[index] = index;
    }
    std::sort(order.begin(), order.begin()
        + static_cast<std::ptrdiff_t>(performance.count),
        [&performance](const std::size_t left, const std::size_t right) {
            const auto& a = performance.events[left];
            const auto& b = performance.events[right];
            const double aTime = static_cast<double>(a.formCycle)
                * performance.sectionSeconds + a.timeSeconds;
            const double bTime = static_cast<double>(b.formCycle)
                * performance.sectionSeconds + b.timeSeconds;
            return aTime == bTime ? left < right : aTime < bTime;
        });

    double state = parameters.initialState;
    for (std::size_t index = 0; index < parameters.burnIn; ++index)
        state = parameters.r * state * (1.0 - state);
    for (std::size_t ordinal = 0; ordinal < performance.count; ++ordinal) {
        state = parameters.r * state * (1.0 - state);
        const float multiplier = 1.0f + parameters.depth
            * static_cast<float>(2.0 * state - 1.0);
        auto& event = performance.events[order[ordinal]];
        event.dynamicsMapState = static_cast<float>(state);
        event.dynamicsMapScale = multiplier;
        event.velocity = std::clamp(event.velocity * multiplier, 0.0f, 1.0f);
    }
    if (finalState != nullptr)
        *finalState = state;
    return true;
}

} // namespace batruc
