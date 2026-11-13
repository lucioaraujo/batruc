// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/GenerativeOperations.hpp"
#include "core/CompositionCore.hpp"

#include <array>
#include <cmath>
#include <limits>

using namespace batruc;

namespace {

bool near(const double a, const double b) {
    return std::abs(a - b) < 1.0e-8;
}

bool sameScore(const BassScore& a, const BassScore& b) {
    if (a.count != b.count || !near(a.sectionSeconds, b.sectionSeconds))
        return false;
    for (std::size_t i = 0; i < a.count; ++i) {
        const auto& x = a.notes[i];
        const auto& y = b.notes[i];
        if (!near(x.timeSeconds, y.timeSeconds)
            || !near(x.durationSeconds, y.durationSeconds)
            || x.writtenMidi != y.writtenMidi
            || x.calculatedMidi != y.calculatedMidi
            || x.bar != y.bar || x.step != y.step)
            return false;
    }
    return true;
}

bool samePerformance(const BassPerformance& a, const BassPerformance& b) {
    if (a.count != b.count || !near(a.sectionSeconds, b.sectionSeconds)
        || a.pitchPath != b.pitchPath || a.pathIntegral != b.pathIntegral
        || a.pathDerivative != b.pathDerivative
        || a.pathCurvature != b.pathCurvature || a.pathRoute != b.pathRoute
        || a.pathDensity != b.pathDensity)
        return false;
    for (std::size_t index = 0; index < a.count; ++index) {
        const auto& left = a.events[index];
        const auto& right = b.events[index];
        if (!near(left.timeSeconds, right.timeSeconds)
            || !near(left.durationSeconds, right.durationSeconds)
            || left.midi != right.midi || left.velocity != right.velocity
            || left.sourceNote != right.sourceNote
            || left.formCycle != right.formCycle)
            return false;
    }
    return true;
}

} // namespace

int main() {
    ChordVoicing major;
    major.count = 3;
    major.midi = {60, 64, 67};
    ChordVoicing inverted;
    if (!invertChordVoicing(major, 1, inverted)
        || inverted.midi[0] != 64 || inverted.midi[1] != 67
        || inverted.midi[2] != 72)
        return 1;
    if (!invertChordVoicing(major, 2, inverted)
        || inverted.midi[0] != 67 || inverted.midi[1] != 72
        || inverted.midi[2] != 76)
        return 2;
    if (invertChordVoicing(major, 3, inverted))
        return 3;

    BassRecipe firstRecipe;
    firstRecipe.cells[0] = {BassCellKind::note, 36, 2, 1, BassIntent::normal};
    firstRecipe.cells[4] = {BassCellKind::note, 43, 2, 1, BassIntent::normal};
    firstRecipe.cells[8] = {BassCellKind::note, 36, 2, 1, BassIntent::normal};
    firstRecipe.cells[12] = {BassCellKind::note, 43, 2, 1, BassIntent::normal};
    BassScore original;
    if (buildBassScore(firstRecipe, original) != BassError::none)
        return 4;
    BassScore mirrored;
    if (!invertBassPitchContour(original, 42, mirrored)
        || mirrored.notes[0].writtenMidi != 48
        || mirrored.notes[1].writtenMidi != 41)
        return 5;
    BassScore restored;
    if (!invertBassPitchContour(mirrored, 42, restored)
        || !sameScore(original, restored))
        return 6;
    if (invertBassPitchContour(original, 24, restored)
        || !sameScore(original, restored))
        return 7;

    BassScore retrograde;
    if (!retrogradeBassScore(original, retrograde)
        || retrograde.count != original.count
        || retrograde.notes[0].writtenMidi != original.notes[original.count - 1].writtenMidi)
        return 8;
    if (!retrogradeBassScore(retrograde, restored)
        || !sameScore(original, restored))
        return 9;

    BassPerformance sourcePerformance;
    sourcePerformance.sectionSeconds = 1.0;
    sourcePerformance.count = 3;
    sourcePerformance.events[0] = {0.10, 0.20, 36, 0.6f, 11, 0};
    sourcePerformance.events[1] = {0.40, 0.10, 43, 0.8f, 12, 1};
    sourcePerformance.events[2] = {0.80, 0.15, 48, 0.4f, 13, 0};
    sourcePerformance.pitchPath = {0, 1, -1, 2, 0, -2, 1};
    sourcePerformance.pathIntegral = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f};
    sourcePerformance.pathDerivative = {0.7f, 0.6f, 0.5f, 0.4f, 0.3f, 0.2f, 0.1f};
    sourcePerformance.pathCurvature = {-0.3f, 0.2f, -0.1f, 0.4f, -0.2f, 0.1f, 0.0f};
    sourcePerformance.pathRoute = {0, 1, 2, 3, 0, 1, 2};
    sourcePerformance.pathDensity = {0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f};
    BassPerformance reversedPerformance;
    if (!retrogradeBassPerformance(sourcePerformance, reversedPerformance)
        || reversedPerformance.count != sourcePerformance.count
        || !near(reversedPerformance.events[0].timeSeconds, 0.05)
        || reversedPerformance.events[0].midi != 48
        || reversedPerformance.events[0].formCycle != 0
        || !near(reversedPerformance.events[1].timeSeconds, 0.50)
        || reversedPerformance.events[1].midi != 43
        || !near(reversedPerformance.events[2].timeSeconds, 0.70)
        || reversedPerformance.events[2].midi != 36)
        return 17;
    BassPerformance restoredPerformance;
    if (!retrogradeBassPerformance(reversedPerformance, restoredPerformance)
        || restoredPerformance.count != sourcePerformance.count)
        return 18;
    for (std::size_t index = 0; index < sourcePerformance.count; ++index) {
        const auto& expected = sourcePerformance.events[index];
        const auto& actual = restoredPerformance.events[index];
        if (!near(expected.timeSeconds, actual.timeSeconds)
            || !near(expected.durationSeconds, actual.durationSeconds)
            || expected.midi != actual.midi || expected.velocity != actual.velocity
            || expected.sourceNote != actual.sourceNote
            || expected.formCycle != actual.formCycle)
            return 19;
    }
    BassPerformance invertedPerformance;
    if (!invertBassPerformancePitchContour(sourcePerformance, 42,
                                            invertedPerformance)
        || invertedPerformance.events[0].midi != 48
        || invertedPerformance.events[1].midi != 41
        || invertedPerformance.events[2].midi != 36
        || invertedPerformance.pitchPath != sourcePerformance.pitchPath
        || invertedPerformance.pathIntegral != sourcePerformance.pathIntegral
        || invertedPerformance.pathDerivative != sourcePerformance.pathDerivative
        || invertedPerformance.pathCurvature != sourcePerformance.pathCurvature
        || invertedPerformance.pathRoute != sourcePerformance.pathRoute
        || invertedPerformance.pathDensity != sourcePerformance.pathDensity)
        return 20;
    BassPerformance restoredPitchPerformance;
    if (!invertBassPerformancePitchContour(invertedPerformance, 42,
                                           restoredPitchPerformance))
        return 21;
    for (std::size_t index = 0; index < sourcePerformance.count; ++index) {
        const auto& expected = sourcePerformance.events[index];
        const auto& invertedEvent = invertedPerformance.events[index];
        const auto& restoredEvent = restoredPitchPerformance.events[index];
        if (restoredEvent.midi != expected.midi
            || !near(invertedEvent.timeSeconds, expected.timeSeconds)
            || !near(invertedEvent.durationSeconds, expected.durationSeconds)
            || invertedEvent.velocity != expected.velocity
            || invertedEvent.sourceNote != expected.sourceNote
            || invertedEvent.formCycle != expected.formCycle)
            return 22;
    }
    if (!samePerformance(sourcePerformance, restoredPitchPerformance))
        return 23;
    const auto previousPitchResult = restoredPitchPerformance;
    if (invertBassPerformancePitchContour(sourcePerformance, 24,
                                          restoredPitchPerformance)
        || !samePerformance(restoredPitchPerformance, previousPitchResult))
        return 24;

    BassPerformance cycleMaterial;
    cycleMaterial.sectionSeconds = 4.0;
    constexpr std::size_t eventsPerBar = 2;
    cycleMaterial.count = kFormalCycleCount * kBassBarsPerSection * eventsPerBar;
    for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle) {
        for (std::size_t bar = 0; bar < kBassBarsPerSection; ++bar) {
            for (std::size_t eventInBar = 0; eventInBar < eventsPerBar;
                 ++eventInBar) {
                const auto index = (cycle * kBassBarsPerSection + bar)
                    * eventsPerBar + eventInBar;
                cycleMaterial.events[index] = {
                    static_cast<double>(bar) + (eventInBar == 0 ? 0.25 : 0.75),
                    eventInBar == 0 ? 0.6 : 0.2,
                    static_cast<std::uint8_t>(30 + cycle * 4 + bar + eventInBar),
                    static_cast<float>(0.2 + 0.1 * cycle + 0.05 * eventInBar),
                    static_cast<std::uint16_t>(index),
                    static_cast<std::uint8_t>(cycle)};
            }
        }
    }
    BassPerformance recombined;
    if (!recombineBassPerformanceBars(cycleMaterial, 1, recombined)
        || recombined.count != cycleMaterial.count)
        return 25;
    const auto targetIndex = (1 * kBassBarsPerSection + 1) * eventsPerBar;
    const auto& target = cycleMaterial.events[targetIndex];
    const auto& recombinedTarget = recombined.events[targetIndex];
    if (!near(recombinedTarget.timeSeconds, target.timeSeconds)
        || recombinedTarget.formCycle != target.formCycle
        || recombinedTarget.sourceNote != target.sourceNote
        || recombinedTarget.midi != 43
        || !near(recombinedTarget.durationSeconds, 0.5)
        || !near(recombinedTarget.velocity, 0.5))
        return 26;
    if (recombined.pitchPath != cycleMaterial.pitchPath
        || recombined.pathIntegral != cycleMaterial.pathIntegral
        || recombined.pathDerivative != cycleMaterial.pathDerivative
        || recombined.pathCurvature != cycleMaterial.pathCurvature
        || recombined.pathRoute != cycleMaterial.pathRoute
        || recombined.pathDensity != cycleMaterial.pathDensity)
        return 27;
    for (std::size_t index = 0; index < cycleMaterial.count; ++index)
        if (!near(recombined.events[index].timeSeconds,
                  cycleMaterial.events[index].timeSeconds)
            || recombined.events[index].formCycle
                != cycleMaterial.events[index].formCycle
            || recombined.events[index].sourceNote
                != cycleMaterial.events[index].sourceNote)
            return 28;
    const auto previousRecombined = recombined;
    if (recombineBassPerformanceBars(cycleMaterial, 0, recombined)
        || !samePerformance(previousRecombined, recombined))
        return 29;

    BassScore overlapping;
    overlapping.stepsPerBar = 20;
    overlapping.sectionSeconds = 4.0;
    overlapping.count = 2;
    overlapping.notes[0].timeSeconds = 0.0;
    overlapping.notes[0].durationSeconds = 2.0;
    overlapping.notes[0].writtenMidi = 36;
    overlapping.notes[0].calculatedMidi = 36;
    overlapping.notes[1].timeSeconds = 1.0;
    overlapping.notes[1].durationSeconds = 0.25;
    overlapping.notes[1].writtenMidi = 43;
    overlapping.notes[1].calculatedMidi = 43;
    overlapping.notes[1].step = 0;
    overlapping.notes[1].bar = 1;
    if (!retrogradeBassScore(overlapping, retrograde)
        || !near(retrograde.notes[0].timeSeconds, 2.0)
        || !near(retrograde.notes[1].timeSeconds, 2.75))
        return 16;

    BassRecipe secondRecipe = firstRecipe;
    for (auto& cell : secondRecipe.cells)
        if (cell.kind == BassCellKind::note)
            cell.midi = 48;
    BassScore second;
    if (buildBassScore(secondRecipe, second) != BassError::none)
        return 10;
    BassScore combined;
    if (!recombineBassBars(original, second, {false, true, false, true}, combined)
        || combined.count != original.count)
        return 11;
    for (std::size_t index = 0; index < combined.count; ++index) {
        const auto& note = combined.notes[index];
        const auto expected = note.bar % 2 == 0
            ? original.notes[index].writtenMidi : 48;
        if (note.writtenMidi != expected)
            return 12;
    }

    auto editedRecipe = firstRecipe;
    editedRecipe.durationLaw = BassDurationLaw::fibonacci;
    BassScore recalculated = original;
    if (!recalculateBassScore(editedRecipe, recalculated)
        || recalculated.notes[0].durationSteps != 1)
        return 13;
    const auto previous = recalculated;
    editedRecipe.bpm = 0.0;
    if (recalculateBassScore(editedRecipe, recalculated)
        || !sameScore(previous, recalculated))
        return 14;

    double inverse = 0.0;
    if (!inversePhaseRotation(0.25, inverse) || !near(inverse, 0.75)
        || !inversePhaseRotation(inverse, inverse) || !near(inverse, 0.25)
        || inversePhaseRotation(std::numeric_limits<double>::infinity(), inverse))
        return 15;
    return 0;
}
