// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/BassComposition.hpp"
#include "core/BassPath.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>

using namespace batruc;

int main() {
    std::array<std::vector<SeedBassVoiceEvent>, 7> pathVoices;
    for (auto& cycle : pathVoices)
        cycle.push_back({0.5, -1.0, 1.0, 0.6f, 0});
    const auto followPath = buildSeedBassPath(pathVoices, 0u);
    assert(followPath == buildSeedBassPath(pathVoices, 0u));
    assert(followPath != buildSeedBassPath(pathVoices, 8u));
    assert(followPath != buildSeedBassPath(pathVoices, 16u));
    assert(followPath != buildSeedBassPath(pathVoices, 24u));
    for (auto& cycle : pathVoices) {
        cycle.front().x = 1.0;
        cycle.front().y = -1.0;
    }
    assert(followPath != buildSeedBassPath(pathVoices, 0u));
    for (const auto offset : followPath)
        assert(offset >= -9 && offset <= 9);
    const auto changedFirstCycle = buildSeedBassPath(pathVoices, 0u);
    pathVoices[1].front().x = -1.0;
    pathVoices[1].front().y = 1.0;
    const auto changedHistory = buildSeedBassPath(pathVoices, 0u);
    assert(changedFirstCycle[2] != changedHistory[2]);

    std::array<std::vector<SeedBassVoiceEvent>, 7> voices;
    constexpr std::array<double, 6> sourceTimes = {0.4, 1.2, 2.3, 3.7, 5.1, 6.5};
    constexpr std::array<double, 6> answerTimes = {0.55, 1.45, 2.48, 3.92, 5.25, 6.72};
    for (std::size_t cycle : {1u, 2u, 3u, 5u, 6u}) {
        for (std::size_t index = 0; index < sourceTimes.size(); ++index) {
            voices[cycle].push_back({sourceTimes[index], -0.4 + 0.12 * index,
                0.2 + 0.1 * index, 0.55f, 0});
            voices[cycle].push_back({answerTimes[index], 0.3 - 0.08 * index,
                -0.2 + 0.06 * index, 0.38f, 1});
        }
    }

    SeedBassScheduleParameters parameters;
    parameters.voiceCount = 2;
    parameters.bpm = 120.0;
    parameters.sectionSeconds = 8.0;
    parameters.seedHash = 0x5eed2027u;
    parameters.relationStrengths[1] = 0.8f;
    parameters.manualDurationSteps.fill(2);
    BassPerformance independent;
    parameters.relation = BassRhythmRelation::follow;
    assert(buildSeedBassPerformance(voices, parameters, independent) == BassError::none);
    assert(independent.count > 0);

    // In the default Loop phrase, bass variation and breathing articulate the
    // percussion-derived calendar without discarding its source attacks.
    std::array<std::vector<SeedBassVoiceEvent>, 7> denseVoices;
    for (auto& cycle : denseVoices)
        for (int step = 0; step < 64; ++step)
            cycle.push_back({step * 0.125, -0.2, 0.3, 0.55f, 0});
    parameters.pathBreath = 2.0f;
    parameters.pitchVariation = 0.0f;
    parameters.variationLevel = 0;
    BassPerformance anchored;
    assert(buildSeedBassPerformance(denseVoices, parameters, anchored)
        == BassError::none);
    assert(anchored.count == 7 * 64);
    parameters.variationLevel = 3;
    BassPerformance wide;
    assert(buildSeedBassPerformance(denseVoices, parameters, wide)
        == BassError::none);
    assert(wide.count == anchored.count);
    for (std::size_t index = 0; index < wide.count; ++index) {
        assert(wide.events[index].durationSeconds
            <= anchored.events[index].durationSeconds);
        assert(wide.events[index].durationSeconds
            >= 0.70 * anchored.events[index].durationSeconds - 1.0e-9);
    }
    parameters.pathBreath = 0.0f;
    BassPerformance noBreath;
    assert(buildSeedBassPerformance(denseVoices, parameters, noBreath)
        == BassError::none);
    assert(noBreath.count == wide.count);
    bool breathingChangedGate = false;
    for (std::size_t index = 0; index < wide.count; ++index) {
        assert(std::abs(wide.events[index].timeSeconds
            - noBreath.events[index].timeSeconds) < 1.0e-9);
        assert(wide.events[index].midi == noBreath.events[index].midi);
        assert(wide.events[index].durationSeconds
            <= noBreath.events[index].durationSeconds + 1.0e-9);
        breathingChangedGate = breathingChangedGate
            || wide.events[index].durationSeconds
                < noBreath.events[index].durationSeconds - 1.0e-9;
    }
    assert(breathingChangedGate);
    parameters.pathBreath = 2.0f;
    for (std::uint8_t cycle = 0; cycle < 7; ++cycle)
        for (int beat = 0; beat < 16; ++beat) {
            const double anchorTime = beat * 0.5;
            const auto found = std::find_if(wide.events.begin(),
                wide.events.begin() + static_cast<std::ptrdiff_t>(wide.count),
                [cycle, anchorTime](const BassPlayedEvent& event) {
                    return event.formCycle == cycle
                        && std::abs(event.timeSeconds - anchorTime) < 1.0e-6;
                });
            assert(found != wide.events.begin()
                + static_cast<std::ptrdiff_t>(wide.count));
        }
    parameters.seedHash ^= 0x12345678u;
    BassPerformance otherSeed;
    assert(buildSeedBassPerformance(denseVoices, parameters, otherSeed)
        == BassError::none);
    bool bassChanged = otherSeed.count != wide.count;
    for (std::size_t index = 0;
         index < std::min(otherSeed.count, wide.count); ++index)
        bassChanged = bassChanged
            || otherSeed.events[index].midi != wide.events[index].midi;
    assert(bassChanged);
    parameters.seedHash ^= 0x12345678u;
    parameters.pathBreath = 1.0f;
    parameters.variationLevel = 2;

    // A distinct bass seed must alter the rhythmic choices as well as pitch.
    // Sparse kick anchors leave eighth-note counterbeats available to the
    // seed-driven phrase builder; anchors themselves remain fixed.
    std::array<std::vector<SeedBassVoiceEvent>, 7> sparseKickVoices;
    for (auto& cycle : sparseKickVoices)
        for (int beat = 0; beat < 16; ++beat)
            cycle.push_back({beat * 0.5, 0.0, 0.0, 1.0f, 0});
    auto sparseParameters = parameters;
    sparseParameters.relation = BassRhythmRelation::follow;
    sparseParameters.meterNumerator = 4;
    sparseParameters.meterDenominator = 4;
    sparseParameters.bpm = 120.0;
    sparseParameters.sectionSeconds = 8.0;
    sparseParameters.variationLevel = 3;
    sparseParameters.pitchVariation = 0.0f;
    sparseParameters.pathBreath = 0.0f;
    sparseParameters.seedHash = 0x13579bdfu;
    BassPerformance sparseA;
    BassPerformance sparseRepeat;
    BassPerformance sparseB;
    assert(buildSeedBassPerformance(sparseKickVoices, sparseParameters, sparseA)
        == BassError::none);
    assert(buildSeedBassPerformance(sparseKickVoices, sparseParameters, sparseRepeat)
        == BassError::none);
    sparseParameters.seedHash = 0x2468ace0u;
    assert(buildSeedBassPerformance(sparseKickVoices, sparseParameters, sparseB)
        == BassError::none);
    bool repeatedRhythmExactly = sparseA.count == sparseRepeat.count;
    for (std::size_t index = 0;
         index < std::min(sparseA.count, sparseRepeat.count); ++index) {
        repeatedRhythmExactly = repeatedRhythmExactly
            && sparseA.events[index].formCycle == sparseRepeat.events[index].formCycle
            && std::abs(sparseA.events[index].timeSeconds
                - sparseRepeat.events[index].timeSeconds) < 1.0e-12;
    }
    bool differentSeedChangedRhythm = sparseA.count != sparseB.count;
    for (std::size_t index = 0;
         index < std::min(sparseA.count, sparseB.count); ++index) {
        differentSeedChangedRhythm = differentSeedChangedRhythm
            || sparseA.events[index].formCycle != sparseB.events[index].formCycle
            || std::abs(sparseA.events[index].timeSeconds
                - sparseB.events[index].timeSeconds) > 1.0e-9;
    }
    bool sparseAnchorsRemain = true;
    bool sixteenthAnticipationGenerated = false;
    std::array<std::array<std::size_t, 4>, 7> counterbeatsByBar{};
    constexpr double sixteenthSeconds = 0.125;
    constexpr std::size_t stepsPerBar = 16;
    for (std::uint8_t cycle = 0; cycle < 7; ++cycle)
        for (int beat = 0; beat < 16; ++beat) {
            const auto found = std::find_if(sparseA.events.begin(),
                sparseA.events.begin() + static_cast<std::ptrdiff_t>(sparseA.count),
                [cycle, beat](const BassPlayedEvent& event) {
                    return event.formCycle == cycle
                        && std::abs(event.timeSeconds - beat * 0.5) < 1.0e-12;
                });
            sparseAnchorsRemain = sparseAnchorsRemain
                && found != sparseA.events.begin()
                    + static_cast<std::ptrdiff_t>(sparseA.count);
        }
    for (std::size_t index = 0; index < sparseA.count; ++index) {
        const auto& event = sparseA.events[index];
        const auto step = static_cast<std::size_t>(std::llround(
            event.timeSeconds / sixteenthSeconds));
        if (step >= stepsPerBar * 4 || event.formCycle >= 7
            || std::abs(event.timeSeconds - step * sixteenthSeconds) > 1.0e-9)
            continue;
        const auto subdivision = step % 4;
        if (subdivision == 3)
            sixteenthAnticipationGenerated = true;
        if (subdivision == 2 || subdivision == 3)
            ++counterbeatsByBar[event.formCycle][step / stepsPerBar];
    }
    bool counterbeatDensityBounded = true;
    for (const auto& cycle : counterbeatsByBar)
        for (const auto count : cycle)
            counterbeatDensityBounded = counterbeatDensityBounded && count <= 3;
    assert(repeatedRhythmExactly);
    assert(differentSeedChangedRhythm);
    assert(sparseAnchorsRemain);
    assert(sixteenthAnticipationGenerated);
    assert(counterbeatDensityBounded);

    for (const auto relation : {
            BassRhythmRelation::ecologicalSupport,
            BassRhythmRelation::ecologicalImitation,
            BassRhythmRelation::ecologicalAvoidance,
            BassRhythmRelation::ecologicalCompetition}) {
        parameters.relation = relation;
        BassPerformance related;
        assert(buildSeedBassPerformance(voices, parameters, related) == BassError::none);
        assert(related.count > 0);
        bool changed = related.count != independent.count;
        const auto common = std::min(related.count, independent.count);
        for (std::size_t index = 0; index < common; ++index) {
            changed = changed
                || std::abs(related.events[index].timeSeconds
                            - independent.events[index].timeSeconds) > 1.0e-6
                || std::abs(related.events[index].velocity
                            - independent.events[index].velocity) > 1.0e-6f;
        }
        assert(changed);
        for (std::size_t index = 0; index < related.count; ++index) {
            assert(std::isfinite(related.events[index].timeSeconds));
            assert(related.events[index].timeSeconds >= 0.0);
            assert(related.events[index].timeSeconds < parameters.sectionSeconds);
            assert(related.events[index].velocity >= 0.0f);
            assert(related.events[index].velocity <= 1.0f);
        }
    }
}
