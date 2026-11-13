// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/BassComposition.hpp"
#include "core/BassPath.hpp"
#include "core/CompositionCore.hpp"
#include "core/EcologicalRelations.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace batruc {
namespace {

static_assert(kBassRelationVoiceCount == kMaxVoices);

constexpr std::array<int, 8> kFibonacci = {1, 1, 2, 3, 5, 8, 13, 16};

bool validCell(const BassCell& cell) noexcept {
    if (cell.kind != BassCellKind::empty && cell.kind != BassCellKind::note
        && cell.kind != BassCellKind::rest)
        return false;
    if (cell.kind != BassCellKind::note)
        return true;
    return cell.midi >= 24 && cell.midi <= 60
        && (cell.durationSteps == 1 || cell.durationSteps == 2
            || cell.durationSteps == 4 || cell.durationSteps == 8
            || cell.durationSteps == 16)
        && (cell.repeats == 1 || cell.repeats == 2
            || cell.repeats == 4 || cell.repeats == 8)
        && (cell.intent == BassIntent::normal || cell.intent == BassIntent::ghost
            || cell.intent == BassIntent::accent);
}

bool phraseRest(const BassPhraseMode mode, const std::size_t bar,
                const std::size_t step, const std::size_t stepsPerBar) noexcept {
    switch (mode) {
        case BassPhraseMode::loop: return false;
        case BassPhraseMode::breath:
            return bar % 2 == 1 && step >= stepsPerBar - 2;
        case BassPhraseMode::cadence:
            return bar % 4 == 3 && step >= stepsPerBar - 4;
        case BassPhraseMode::callResponse: return bar % 2 == 1;
    }
    return true;
}

std::uint32_t nextRandom(std::uint32_t& state) noexcept {
    // Stable local seed mapping, version 1 for this native bass score.
    state += 0x6D2B79F5u;
    std::uint32_t value = state;
    value = (value ^ (value >> 15u)) * (value | 1u);
    value ^= value + (value ^ (value >> 7u)) * (value | 61u);
    return value ^ (value >> 14u);
}

int calculatedDuration(const BassRecipe& recipe, const std::size_t ordinal,
                       const int manual, const int stepsPerBar) noexcept {
    switch (recipe.durationLaw) {
        case BassDurationLaw::manual: return manual;
        case BassDurationLaw::fibonacci:
            return std::min(stepsPerBar, kFibonacci[ordinal % 8]);
        case BassDurationLaw::geometric:
            return std::clamp(static_cast<int>(std::floor(
                std::pow(recipe.geometricRatio, static_cast<double>(ordinal % 8))
                + 0.5)), 1, std::min(16, stepsPerBar));
    }
    return manual;
}

} // namespace

BassError buildBassScore(const BassRecipe& recipe, BassScore& score) noexcept {
    score = BassScore{};
    const bool supportedMeter = (recipe.meterDenominator == 4
        && (recipe.meterNumerator == 3 || recipe.meterNumerator == 4
            || recipe.meterNumerator == 5))
        || (recipe.meterNumerator == 7 && recipe.meterDenominator == 8);
    if (!std::isfinite(recipe.bpm) || recipe.bpm < 20.0 || recipe.bpm > 220.0
        || !supportedMeter
        || !std::isfinite(recipe.geometricRatio)
        || recipe.geometricRatio < 1.2 || recipe.geometricRatio > 2.0
        || !std::isfinite(recipe.pitchVariation)
        || recipe.pitchVariation < 0.0f || recipe.pitchVariation > 1.0f
        || recipe.variationLevel > 3
        || recipe.durationLaw > BassDurationLaw::geometric
        || recipe.phraseMode > BassPhraseMode::callResponse
        || recipe.lfoShape > BassLfoShape::square
        || recipe.lfoTarget > BassLfoTarget::amplitude
        || (recipe.lfoPeriodCycles != 1 && recipe.lfoPeriodCycles != 2
            && recipe.lfoPeriodCycles != 4)
        || recipe.lfoDepthSemitones > 12)
        return BassError::invalidRecipe;

    const auto stepsPerBar = bassStepsPerBar(recipe.meterNumerator,
                                              recipe.meterDenominator);
    if (stepsPerBar == 0 || stepsPerBar > kBassStepsPerBar)
        return BassError::invalidRecipe;
    std::array<bool, 12> pitchClasses{};
    for (const auto& cell : recipe.cells) {
        if (!validCell(cell))
            return BassError::invalidRecipe;
        if (cell.kind == BassCellKind::note)
            pitchClasses[cell.midi % 12] = true;
    }

    std::array<std::uint8_t, kBassStepsPerBar> calculatedPitches{};
    std::uint32_t randomState = recipe.seed ^ 0xBA551EEDu;
    const int radius = 2 + static_cast<int>(std::floor(5.0f * recipe.pitchVariation + 0.5f));
    for (std::size_t step = 0; step < kBassStepsPerBar; ++step) {
        const auto& cell = recipe.cells[step];
        calculatedPitches[step] = cell.midi;
        if (cell.kind != BassCellKind::note || recipe.pitchVariation == 0.0f)
            continue;
        const double chance = static_cast<double>(nextRandom(randomState))
            / 4294967296.0;
        if (chance >= recipe.pitchVariation)
            continue;
        std::array<std::uint8_t, 14> candidates{};
        std::size_t candidateCount = 0;
        for (int midi = std::max(24, static_cast<int>(cell.midi) - radius);
             midi <= std::min(60, static_cast<int>(cell.midi) + radius); ++midi) {
            if (midi != cell.midi && pitchClasses[static_cast<std::size_t>(midi % 12)])
                candidates[candidateCount++] = static_cast<std::uint8_t>(midi);
        }
        if (candidateCount != 0) {
            const double selection = static_cast<double>(nextRandom(randomState))
                / 4294967296.0;
            const auto choice = static_cast<std::size_t>(
                selection * static_cast<double>(candidateCount));
            calculatedPitches[step] = candidates[std::min(choice, candidateCount - 1)];
        }
    }

    const double stepSeconds = 15.0 / recipe.bpm;
    const auto stepsPerSection = stepsPerBar * kBassBarsPerSection;
    score.stepsPerBar = static_cast<std::uint8_t>(stepsPerBar);
    score.sectionSeconds = static_cast<double>(stepsPerSection) * stepSeconds;
    std::size_t ordinal = 0;
    for (std::size_t bar = 0; bar < kBassBarsPerSection; ++bar) {
        for (std::size_t step = 0; step < stepsPerBar; ++step) {
            const auto& cell = recipe.cells[step];
            if (cell.kind != BassCellKind::note
                || phraseRest(recipe.phraseMode, bar, step, stepsPerBar))
                continue;
            if (score.count >= score.notes.size()) {
                score = BassScore{};
                return BassError::eventLimitExceeded;
            }
            auto& note = score.notes[score.count++];
            note.timeSeconds = static_cast<double>(bar * stepsPerBar + step)
                * stepSeconds;
            note.durationSteps = static_cast<std::uint8_t>(std::min(
                stepsPerBar - step, static_cast<std::size_t>(calculatedDuration(
                recipe, ordinal++, cell.durationSteps,
                static_cast<int>(stepsPerBar)))));
            note.durationSeconds = note.durationSteps * stepSeconds;
            note.writtenMidi = cell.midi;
            note.calculatedMidi = calculatedPitches[step];
            note.bar = static_cast<std::uint8_t>(bar);
            note.step = static_cast<std::uint8_t>(step);
            note.repeats = cell.repeats;
            note.intent = cell.intent;
        }
    }

    // Explicit rests and phrase rests stop a sustained note. Empty cells only
    // suppress a new onset; they may lie under an existing note.
    for (std::size_t index = 0; index < score.count; ++index) {
        auto& note = score.notes[index];
        double stop = std::min(score.sectionSeconds,
            note.timeSeconds + note.durationSeconds);
        if (index + 1 < score.count)
            stop = std::min(stop, score.notes[index + 1].timeSeconds);
        const std::size_t start = note.bar * stepsPerBar + note.step;
        for (std::size_t position = start + 1; position < stepsPerSection; ++position) {
            const std::size_t bar = position / stepsPerBar;
            const std::size_t step = position % stepsPerBar;
            if (recipe.cells[step].kind == BassCellKind::rest
                || phraseRest(recipe.phraseMode, bar, step, stepsPerBar)) {
                stop = std::min(stop, position * stepSeconds);
                break;
            }
        }
        note.durationSeconds = std::max(0.0, stop - note.timeSeconds);
    }
    return BassError::none;
}

BassError interpretBassScore(const BassScore& score,
                             const BassInterpretation& interpretation,
                             BassPerformance& performance) noexcept {
    performance = BassPerformance{};
    if (score.count > kMaxBassNotes || score.stepsPerBar == 0
        || score.stepsPerBar > kBassStepsPerBar
        || !std::isfinite(score.sectionSeconds)
        || score.sectionSeconds <= 0.0 || !std::isfinite(interpretation.swing)
        || interpretation.swing < 0.0f || interpretation.swing > 0.67f
        || !std::isfinite(interpretation.velocity)
        || interpretation.velocity < 0.0f || interpretation.velocity > 1.0f)
        return BassError::invalidRecipe;
    performance.sectionSeconds = score.sectionSeconds;
    for (std::size_t index = 0; index < score.count; ++index) {
        const auto& note = score.notes[index];
        if (!std::isfinite(note.timeSeconds) || note.timeSeconds < 0.0
            || !std::isfinite(note.durationSeconds) || note.durationSeconds < 0.0
            || note.timeSeconds + note.durationSeconds > score.sectionSeconds + 1e-9
            || (index > 0 && note.timeSeconds < score.notes[index - 1].timeSeconds)
            || note.bar >= kBassBarsPerSection || note.step >= score.stepsPerBar
            || note.calculatedMidi < 24 || note.calculatedMidi > 60
            || (note.repeats != 1 && note.repeats != 2
                && note.repeats != 4 && note.repeats != 8)
            || (note.intent != BassIntent::normal && note.intent != BassIntent::ghost
                && note.intent != BassIntent::accent))
            return BassError::invalidRecipe;
    }
    const double stepSeconds = score.sectionSeconds
        / static_cast<double>(kBassBarsPerSection * score.stepsPerBar);
    for (std::size_t index = 0; index < score.count; ++index) {
        const auto& note = score.notes[index];
        const double delayedTime = note.timeSeconds
            + ((note.step % 2 == 1) ? interpretation.swing * stepSeconds : 0.0);
        const double nextOnset = index + 1 < score.count
            ? score.notes[index + 1].timeSeconds
                + ((score.notes[index + 1].step % 2 == 1)
                    ? interpretation.swing * stepSeconds : 0.0)
            : score.sectionSeconds;
        const double gate = std::max(0.0, std::min({
            note.timeSeconds + note.durationSeconds - delayedTime,
            nextOnset - delayedTime, score.sectionSeconds - delayedTime}));
        if (gate <= 0.0)
            continue;
        const float intentGain = note.intent == BassIntent::ghost ? 0.42f
            : note.intent == BassIntent::accent ? 1.35f : 1.0f;
        for (std::size_t repeat = 0; repeat < note.repeats; ++repeat) {
            if (performance.count >= performance.events.size()) {
                performance = BassPerformance{};
                return BassError::eventLimitExceeded;
            }
            auto& event = performance.events[performance.count++];
            event.timeSeconds = delayedTime + gate * repeat / note.repeats;
            event.durationSeconds = gate / note.repeats;
            event.midi = note.calculatedMidi;
            event.velocity = std::clamp(interpretation.velocity * intentGain
                * (repeat == 0 ? 1.0f : std::max(0.55f, 0.92f - 0.08f * repeat)),
                0.0f, 1.0f);
            event.sourceNote = static_cast<std::uint16_t>(index);
        }
    }
    return BassError::none;
}

BassError buildSeedBassPerformance(
    const std::array<std::vector<SeedBassVoiceEvent>, 7>& voiceEvents,
    const SeedBassScheduleParameters& parameters,
    BassPerformance& performance) {
    performance = {};
    const auto stepsPerBar = bassStepsPerBar(parameters.meterNumerator,
                                              parameters.meterDenominator);
    if (!std::isfinite(parameters.bpm) || parameters.bpm < 20.0
        || !std::isfinite(parameters.sectionSeconds) || parameters.sectionSeconds <= 0.0
        || stepsPerBar == 0 || stepsPerBar > kBassStepsPerBar
        || parameters.variationLevel > 3
        || !std::isfinite(parameters.pitchVariation)
        || parameters.pitchVariation < 0.0f || parameters.pitchVariation > 1.0f
        || !std::isfinite(parameters.pathMemory)
        || parameters.pathMemory < 0.0f || parameters.pathMemory > 0.95f
        || !std::isfinite(parameters.pathBreath)
        || parameters.pathBreath < 0.0f || parameters.pathBreath > 2.0f
        || parameters.durationLaw > BassDurationLaw::geometric
        || parameters.relation > BassRhythmRelation::ecologicalCompetition
        || parameters.voiceCount < 2 || parameters.voiceCount > kMaxVoices
        || !std::isfinite(parameters.geometricRatio)
        || parameters.geometricRatio < 1.2 || parameters.geometricRatio > 2.0)
        return BassError::invalidRecipe;

    const auto slotsPerCycle = stepsPerBar * kBassBarsPerSection;
    const double stepSeconds = 15.0 / parameters.bpm;
    const double cycleSeconds = parameters.sectionSeconds;
    performance.sectionSeconds = cycleSeconds;

    struct Influence {
        double x = 0.0;
        double y = 0.0;
        double intensity = 0.0;
        double priority = -1.0;
        double timeSeconds = 0.0;
        std::uint8_t voice = 0;
        bool occupied = false;
        bool countertime = false;
    };

    std::uint32_t randomState = parameters.seedHash ^ 0x42545243u;
    const double seedBase = 31.0 + 12.0 * compositionSeedRandom(randomState);
    const double seedX = 2.0 + 6.0 * compositionSeedRandom(randomState);
    const double seedY = 4.0 + 8.0 * compositionSeedRandom(randomState);
    // Give the dedicated bass seed a clearly audible musical consequence.
    // The previous continuous mapping could collapse different seeds onto
    // the same rounded/clamped MIDI notes, especially when pitch variation
    // was low. Use a bounded diatonic-like shift for the whole generated
    // phrase, then retain the per-note path and variation below.
    constexpr std::array<int, 11> seedPhraseShifts = {
        -7, -5, -4, -3, -2, 0, 2, 3, 4, 5, 7};
    std::uint32_t phraseShiftState = parameters.seedHash ^ 0x53484946u;
    const auto phraseShiftIndex = static_cast<std::size_t>(
        compositionSeedRandom(phraseShiftState) * seedPhraseShifts.size());
    const int seedPhraseShift = seedPhraseShifts[std::min(
        phraseShiftIndex, seedPhraseShifts.size() - 1)];
    const auto pathAnalysis = analyseSeedBassPath(voiceEvents,
        parameters.seedHash, parameters.pathMemory);
    const auto& pitchPath = pathAnalysis.offsets;
    performance.pitchPath = pitchPath;
    for (std::size_t cycle = 0; cycle < performance.pitchPath.size(); ++cycle) {
        performance.pathIntegral[cycle] = static_cast<float>(
            pathAnalysis.integral[cycle]);
        performance.pathDerivative[cycle] = static_cast<float>(
            pathAnalysis.derivative[cycle]);
        performance.pathCurvature[cycle] = static_cast<float>(
            pathAnalysis.curvature[cycle]);
        performance.pathCorrelation[cycle] = static_cast<float>(
            pathAnalysis.correlation[cycle]);
        performance.pathRegressionYFromX[cycle] = static_cast<float>(
            pathAnalysis.regressionYFromX[cycle]);
        performance.pathSpeed[cycle] = static_cast<float>(pathAnalysis.speed[cycle]);
        performance.pathSignedCurvature[cycle] = static_cast<float>(
            pathAnalysis.signedCurvature[cycle]);
        performance.pathRoute[cycle] = pathAnalysis.route[cycle];
    }
    const float levelPitchVariation = parameters.variationLevel == 0 ? 0.0f
        : parameters.variationLevel == 1
            ? std::min(parameters.pitchVariation, 0.5f)
        : parameters.variationLevel == 2 ? parameters.pitchVariation
        : std::max(parameters.pitchVariation, 0.55f);
    const int pitchRadius = static_cast<int>(std::lround(
        6.0 * levelPitchVariation));
    std::size_t bassNoteOrdinal = 0;

    const bool ecologicalRelation = parameters.relation
            >= BassRhythmRelation::ecologicalSupport
        && parameters.relation <= BassRhythmRelation::ecologicalCompetition;
    const auto ecologicalKind = [&]() noexcept {
        switch (parameters.relation) {
            case BassRhythmRelation::ecologicalSupport:
                return EcologicalRelation::support;
            case BassRhythmRelation::ecologicalImitation:
                return EcologicalRelation::imitation;
            case BassRhythmRelation::ecologicalAvoidance:
                return EcologicalRelation::avoidance;
            case BassRhythmRelation::ecologicalCompetition:
                return EcologicalRelation::competition;
            default:
                return EcologicalRelation::none;
        }
    }();

    for (std::size_t cycle = 0; cycle < voiceEvents.size(); ++cycle) {
        std::uint32_t cycleRandomState = parameters.seedHash ^ 0x42545243u
            ^ (static_cast<std::uint32_t>(cycle) * 0x9e3779b9u);
        std::array<Influence, kBassStepsPerBar * kBassBarsPerSection> slots{};
        std::vector<double> voiceTimes;
        voiceTimes.reserve(voiceEvents[cycle].size());
        std::vector<SeedBassVoiceEvent> cycleEvents = voiceEvents[cycle];
        if (ecologicalRelation && !cycleEvents.empty()) {
            CompositionRecipe relationRecipe;
            relationRecipe.voiceCount = parameters.voiceCount;
            relationRecipe.sectionSeconds = cycleSeconds;
            EventPlan sourcePlan;
            sourcePlan.events.reserve(cycleEvents.size());
            std::stable_sort(cycleEvents.begin(), cycleEvents.end(),
                [](const SeedBassVoiceEvent& left, const SeedBassVoiceEvent& right) {
                    if (left.localTimeSeconds != right.localTimeSeconds)
                        return left.localTimeSeconds < right.localTimeSeconds;
                    return left.voice < right.voice;
                });
            for (const auto& event : cycleEvents) {
                MusicalEvent musical;
                musical.timeSeconds = event.localTimeSeconds;
                musical.voice = event.voice;
                musical.intensity = std::clamp(event.intensity, 0.0f, 1.0f);
                musical.fieldX = static_cast<float>(event.x);
                musical.fieldY = static_cast<float>(event.y);
                sourcePlan.events.push_back(musical);
            }
            std::array<VoiceRelationRule, kMaxVoices> rules{};
            for (std::size_t voice = 1; voice < parameters.voiceCount; ++voice) {
                auto& rule = rules[voice];
                rule.relation = ecologicalKind;
                rule.sourceVoice = static_cast<std::uint8_t>(voice - 1);
                rule.strength = std::clamp(parameters.relationStrengths[voice],
                                           0.0f, 1.0f);
                rule.memorySeconds = cycleSeconds * 0.5;
                rule.responseDelaySeconds = cycleSeconds / 16.0;
            }
            EventPlan transformedPlan;
            std::vector<EcologicalEventTrace> trace(sourcePlan.events.size());
            std::size_t traceCount = 0;
            if (buildEcologicalEventPlan(relationRecipe, sourcePlan, rules,
                    transformedPlan, trace.data(), trace.size(), &traceCount)
                    != EcologicalPlanError::none
                || traceCount != cycleEvents.size())
                return BassError::invalidRecipe;
            for (std::size_t index = 0; index < cycleEvents.size(); ++index) {
                auto& event = cycleEvents[index];
                const auto& influence = trace[index];
                const float boundedOriginal = std::clamp(event.intensity, 0.0f, 1.0f);
                const float ratio = boundedOriginal > 1.0e-9f
                    ? influence.resultingIntensity / boundedOriginal : 1.0f;
                event.localTimeSeconds = influence.resultingTimeSeconds;
                event.intensity = std::clamp(event.intensity * ratio, 0.0f, 1.25f);
            }
            std::stable_sort(cycleEvents.begin(), cycleEvents.end(),
                [](const SeedBassVoiceEvent& left, const SeedBassVoiceEvent& right) {
                    if (left.localTimeSeconds != right.localTimeSeconds)
                        return left.localTimeSeconds < right.localTimeSeconds;
                    return left.voice < right.voice;
                });
        }
        for (const auto& event : cycleEvents) {
            if (event.voice >= kMaxVoices || !std::isfinite(event.localTimeSeconds)
                || event.localTimeSeconds < 0.0
                || event.localTimeSeconds >= cycleSeconds
                || !std::isfinite(event.x) || !std::isfinite(event.y)
                || !std::isfinite(event.intensity))
                continue;
            voiceTimes.push_back(event.localTimeSeconds);
            const auto slotIndex = std::min(slotsPerCycle - 1,
                static_cast<std::size_t>(std::floor(
                    event.localTimeSeconds / stepSeconds + 1.0e-9)));
            auto& slot = slots[slotIndex];
            const double priority = (event.voice == 0 ? 2.0 : 1.0)
                * (0.5 + std::clamp(static_cast<double>(event.intensity), 0.0, 1.0));
            if (slot.occupied && priority <= slot.priority)
                continue;
            slot.x = event.x;
            slot.y = event.y;
            slot.intensity = static_cast<double>(event.intensity);
            slot.priority = priority;
            slot.timeSeconds = event.localTimeSeconds;
            slot.voice = event.voice;
            slot.occupied = true;
        }

        auto scheduleSlots = slots;
        std::array<double, kBassStepsPerBar * kBassBarsPerSection> convolutionGains{};
        if (parameters.relation == BassRhythmRelation::convolution) {
            std::array<double, kBassStepsPerBar * kBassBarsPerSection> weights{};
            std::array<Influence, kBassStepsPerBar * kBassBarsPerSection> sources{};
            std::uint32_t hash = parameters.seedHash
                ^ (static_cast<std::uint32_t>(cycle + 1) * 0x9e3779b9u);
            hash ^= hash >> 16;
            hash *= 0x7feb352du;
            hash ^= hash >> 15;
            hash *= 0x846ca68bu;
            hash ^= hash >> 16;
            const auto firstOffset = 1u + hash % static_cast<std::uint32_t>(slotsPerCycle - 1);
            auto secondOffset = 1u + (hash >> 16)
                % static_cast<std::uint32_t>(slotsPerCycle - 1);
            if (secondOffset == firstOffset)
                secondOffset = secondOffset % static_cast<std::uint32_t>(slotsPerCycle - 1) + 1u;
            const std::array<std::size_t, 3> offsets = {0, firstOffset, secondOffset};
            constexpr std::array<double, 3> taps = {0.5, 0.32, 0.18};
            for (std::size_t sourceIndex = 0; sourceIndex < slotsPerCycle; ++sourceIndex) {
                if (!slots[sourceIndex].occupied)
                    continue;
                for (std::size_t tap = 0; tap < offsets.size(); ++tap) {
                    const auto target = (sourceIndex + offsets[tap]) % slotsPerCycle;
                    const double contribution = taps[tap]
                        * std::clamp(slots[sourceIndex].intensity, 0.0, 1.0);
                    weights[target] += contribution;
                    if (!sources[target].occupied || contribution > sources[target].priority) {
                        sources[target] = slots[sourceIndex];
                        sources[target].priority = contribution;
                        sources[target].timeSeconds = static_cast<double>(target) * stepSeconds;
                    }
                }
            }
            const double peak = *std::max_element(weights.begin(),
                weights.begin() + static_cast<std::ptrdiff_t>(slotsPerCycle));
            for (std::size_t target = 0; target < slotsPerCycle; ++target) {
                scheduleSlots[target] = weights[target] > 0.0
                    ? sources[target] : Influence{};
                convolutionGains[target] = peak > 0.0 ? weights[target] / peak : 0.0;
            }
        }

        // Compose counterbeats from the percussion's empty subdivisions.
        // The bass answers nearby kick/snare events, but only proposes an
        // onset on an unoccupied offbeat; strong pulse anchors remain intact.
        if (parameters.variationLevel >= 1) {
            const std::size_t pulseSteps = std::max<std::size_t>(1,
                16u / parameters.meterDenominator);
            const std::size_t offbeatInPulse = std::max<std::size_t>(1,
                pulseSteps / 2);
            const double probability = parameters.variationLevel == 1 ? 0.16
                : parameters.variationLevel == 2 ? 0.34 : 0.62;
            Influence fallbackSource;
            std::size_t fallbackTarget = slotsPerCycle;
            std::uint32_t fallbackScore = std::numeric_limits<std::uint32_t>::max();
            bool addedCountertime = false;
            std::array<std::size_t, kBassBarsPerSection> counterbeatsPerBar{};
            const std::size_t maxCounterbeatsPerBar =
                parameters.variationLevel == 1 ? 1u
                : parameters.variationLevel == 2 ? 2u : 3u;
            for (std::size_t target = 0; target < slotsPerCycle; ++target) {
                if (scheduleSlots[target].occupied)
                    continue;
                const auto stepInBar = target % stepsPerBar;
                const auto pulsePosition = stepInBar % pulseSteps;
                const bool eighthOffbeat = pulsePosition == offbeatInPulse;
                const bool sixteenthAnticipation = parameters.variationLevel >= 2
                    && pulseSteps >= 4 && pulsePosition == pulseSteps - 1;
                if ((!eighthOffbeat && !sixteenthAnticipation)
                    || counterbeatsPerBar[target / stepsPerBar]
                        >= maxCounterbeatsPerBar)
                    continue;
                std::uint32_t decision = parameters.seedHash
                    ^ (0x9e3779b9u * static_cast<std::uint32_t>(cycle + 1))
                    ^ (0x85ebca6bu * static_cast<std::uint32_t>(target + 1));
                decision ^= decision >> 16u;
                decision *= 0x7feb352du;
                decision ^= decision >> 15u;

                Influence source;
                const std::size_t searchRadius = std::max<std::size_t>(2,
                    pulseSteps);
                for (std::size_t distance = 1; distance <= searchRadius; ++distance) {
                    for (const int direction : {-1, 1}) {
                        const auto signedIndex = static_cast<std::ptrdiff_t>(target)
                            + direction * static_cast<std::ptrdiff_t>(distance);
                        if (signedIndex < 0
                            || signedIndex >= static_cast<std::ptrdiff_t>(slotsPerCycle))
                            continue;
                        const auto& candidate = slots[static_cast<std::size_t>(signedIndex)];
                        if (!candidate.occupied)
                            continue;
                        const double priority = candidate.priority
                            / static_cast<double>(distance);
                        if (!source.occupied || priority > source.priority) {
                            source = candidate;
                            source.priority = priority;
                        }
                    }
                }
                if (!source.occupied)
                    continue;
                source.timeSeconds = static_cast<double>(target) * stepSeconds;
                source.intensity *= parameters.variationLevel == 1 ? 0.42
                    : parameters.variationLevel == 2 ? 0.62 : 0.78;
                if (sixteenthAnticipation)
                    source.intensity *= 0.82;
                source.countertime = true;
                if (static_cast<double>(decision & 0xffffu) / 65535.0
                    < probability) {
                    scheduleSlots[target] = source;
                    ++counterbeatsPerBar[target / stepsPerBar];
                    addedCountertime = true;
                } else if (decision < fallbackScore) {
                    fallbackScore = decision;
                    fallbackTarget = target;
                    fallbackSource = source;
                }
            }
            // Keep seed changes rhythmically perceptible even when no
            // probabilistic counterbeat fired. The hash chooses its location;
            // pulse anchors and occupied percussion subdivisions are untouched.
            if (!addedCountertime && fallbackTarget < slotsPerCycle) {
                scheduleSlots[fallbackTarget] = fallbackSource;
                ++counterbeatsPerBar[fallbackTarget / stepsPerBar];
            }
        }

        std::size_t occupiedSlotCount = 0;
        std::size_t emittedSlotCount = 0;
        for (std::size_t slotIndex = 0; slotIndex < slotsPerCycle; ++slotIndex) {
            const auto& slot = scheduleSlots[slotIndex];
            if (!slot.occupied)
                continue;
            ++occupiedSlotCount;
            const double absoluteVoiceTime = static_cast<double>(cycle) * cycleSeconds
                + slot.timeSeconds;
            const auto absoluteStep = static_cast<std::size_t>(
                std::floor(absoluteVoiceTime / stepSeconds + 1.0e-9));
            const auto bar = absoluteStep / stepsPerBar;
            const auto stepInBar = absoluteStep % stepsPerBar;
            const bool phraseRest = parameters.phraseMode == BassPhraseMode::breath
                    ? bar % 2 == 1 && stepInBar >= stepsPerBar - 2
                : parameters.phraseMode == BassPhraseMode::cadence
                    ? bar % 4 == 3 && stepInBar >= stepsPerBar - 4
                : parameters.phraseMode == BassPhraseMode::callResponse
                    ? bar % 2 == 1 : false;
            if (phraseRest)
                continue;

            // Memory-derived breathing shapes note articulation, never the
            // event calendar. Dropping seeded attacks here made the same source
            // groove acquire recurring holes at the same positions.
            const double memory = std::abs(pathAnalysis.integral[cycle]) / 1.5;
            const double motion = std::min(1.0,
                std::abs(pathAnalysis.derivative[cycle])
                    + 0.35 * std::abs(pathAnalysis.curvature[cycle]));
            const double breathAmount = std::clamp(parameters.pathBreath
                * (static_cast<double>(parameters.variationLevel) / 2.0) * (
                0.025 + 0.13 * memory - 0.06 * motion
                    + (cycle == 3 ? 0.035 : 0.0)), 0.0, 0.30);
            const bool rhythmicAnchor = !slot.countertime && slot.voice == 0
                && stepInBar % 4 == 0;

            const auto manualDuration = parameters.manualDurationSteps[stepInBar];
            std::size_t durationSteps = manualDuration == 1 || manualDuration == 2
                    || manualDuration == 4 || manualDuration == 8
                    || manualDuration == 16
                ? manualDuration : 2;
            switch (parameters.durationLaw) {
                case BassDurationLaw::manual:
                    break;
                case BassDurationLaw::fibonacci: {
                    constexpr std::array<std::size_t, 8> fibonacci =
                        {1, 1, 2, 3, 5, 8, 13, 16};
                    durationSteps = fibonacci[bassNoteOrdinal % fibonacci.size()];
                    break;
                }
                case BassDurationLaw::geometric:
                    durationSteps = static_cast<std::size_t>(std::max(1.0,
                        std::floor(std::pow(parameters.geometricRatio,
                            static_cast<double>(bassNoteOrdinal % 8)) + 0.5)));
                    durationSteps = std::min<std::size_t>({durationSteps, 16,
                        stepsPerBar});
                    break;
            }
            ++bassNoteOrdinal;
            durationSteps = std::min<std::size_t>(durationSteps,
                stepsPerBar - stepInBar);
            if (parameters.durationLaw != BassDurationLaw::manual) {
                const double kinematicScale = 1.15
                    - 0.30 * pathAnalysis.speed[cycle];
                durationSteps = std::clamp<std::size_t>(
                    static_cast<std::size_t>(std::max(1.0, std::floor(
                        static_cast<double>(durationSteps) * kinematicScale + 0.5))),
                    1, stepsPerBar - stepInBar);
            }

            int midi = std::clamp(static_cast<int>(std::lround(
                seedBase + seedX * slot.x + seedY * slot.y
                    + pitchPath[cycle])) + seedPhraseShift, 24, 60);
            auto& pitchState = parameters.relation == BassRhythmRelation::convolution
                ? cycleRandomState : randomState;
            if (pitchRadius > 0
                && compositionSeedRandom(pitchState) < levelPitchVariation) {
                const int offset = static_cast<int>(std::floor(
                    compositionSeedRandom(pitchState) * (2 * pitchRadius + 1)))
                    - pitchRadius;
                midi = std::clamp(midi + offset, 24, 60);
            }
            midi = quantizeBassToKey(midi, parameters.keyTonic, parameters.keyScale);

            double eventTime = parameters.relation == BassRhythmRelation::independent
                ? static_cast<double>(slotIndex) * stepSeconds : slot.timeSeconds;
            if (parameters.relation == BassRhythmRelation::convolution)
                eventTime = static_cast<double>(slotIndex) * stepSeconds;
            if (parameters.relation == BassRhythmRelation::answer
                && !slot.countertime && !voiceTimes.empty()) {
                double nearestDelta = 0.0;
                double nearestDistance = std::numeric_limits<double>::infinity();
                for (const double voiceTime : voiceTimes) {
                    const double delta = std::fmod(voiceTime - slot.timeSeconds
                        + cycleSeconds * 1.5, cycleSeconds) - cycleSeconds * 0.5;
                    if (std::abs(delta) < nearestDistance) {
                        nearestDistance = std::abs(delta);
                        nearestDelta = delta;
                    }
                }
                if (nearestDistance <= stepSeconds * 1.5)
                    eventTime = std::clamp(slot.timeSeconds + std::clamp(
                        nearestDelta + stepSeconds * 0.5, -stepSeconds, stepSeconds),
                        0.0, cycleSeconds - 1.0e-6);
            }
            // Higher levels loosen the bass against the source grid. Strong
            // V1 beat anchors stay exact; other events move deterministically
            // within a bounded range of neighboring sixteenth-note positions.
            // Apply after relation timing so every relation keeps its variation.
            if (parameters.variationLevel > 0 && !rhythmicAnchor
                && !slot.countertime) {
                std::uint32_t timingHash = parameters.seedHash
                    ^ (0x27d4eb2du * static_cast<std::uint32_t>(cycle + 1))
                    ^ (0x165667b1u * static_cast<std::uint32_t>(slotIndex + 1))
                    ^ (0xd3a2646cu * static_cast<std::uint32_t>(slot.voice + 1));
                timingHash ^= timingHash >> 15u;
                timingHash *= 0x2c1b3c6du;
                timingHash ^= timingHash >> 12u;
                const double draw = static_cast<double>(timingHash & 0xffffu)
                    / 65535.0;
                const double amount = parameters.variationLevel == 1 ? 0.25
                    : parameters.variationLevel == 2 ? 0.70 : 1.0;
                const double signedOffset = (draw * 2.0 - 1.0) * amount;
                eventTime = std::clamp(eventTime + signedOffset * stepSeconds,
                    0.0, cycleSeconds - 1.0e-6);
            }
            if (performance.count >= performance.events.size())
                return BassError::eventLimitExceeded;
            auto& output = performance.events[performance.count++];
            output.timeSeconds = eventTime;
            output.durationSeconds = static_cast<double>(durationSteps)
                * stepSeconds * (1.0 - breathAmount);
            output.midi = static_cast<std::uint8_t>(midi);
            const double globalProgress = (static_cast<double>(cycle) * cycleSeconds
                + slot.timeSeconds) / (cycleSeconds * 7.0);
            const double relatedProgress = eventTime / cycleSeconds;
            const float expressiveGain = expressionAtProgress(globalProgress,
                parameters.dynamics, parameters.intention);
            const float grooveGain = grooveAccentAtProgress(relatedProgress,
                parameters.groove, parameters.meterNumerator,
                parameters.meterDenominator);
            const bool pulseAnchor = !slot.countertime && slot.voice == 0
                && stepInBar % 4 == 0;
            const float sourceVelocity = std::clamp(
                0.18f + 0.72f * static_cast<float>(slot.intensity),
                pulseAnchor ? 0.72f : slot.countertime ? 0.32f : 0.46f,
                slot.countertime ? 0.65f : 1.0f);
            output.velocity = std::clamp(sourceVelocity
                * expressiveGain * grooveGain
                * static_cast<float>(1.0 + 0.08 * std::min(1.0,
                    std::abs(pathAnalysis.curvature[cycle]))
                    * (1.0 + 0.06 * std::abs(
                        pathAnalysis.signedCurvature[cycle])))
                * static_cast<float>(convolutionGains[slotIndex] > 0.0
                    ? convolutionGains[slotIndex] : 1.0), 0.0f, 1.0f);
            output.pan = pulseAnchor ? 0.0f : static_cast<float>(
                std::clamp(slot.x * 0.24
                    * static_cast<double>(parameters.variationLevel) / 3.0,
                    -0.24, 0.24));
            output.sourceNote = static_cast<std::uint16_t>(slotIndex);
            output.formCycle = static_cast<std::uint8_t>(cycle);
            ++emittedSlotCount;
        }
        performance.pathDensity[cycle] = occupiedSlotCount > 0
            ? static_cast<float>(emittedSlotCount)
                / static_cast<float>(occupiedSlotCount)
            : 0.0f;
    }

    std::sort(performance.events.begin(),
        performance.events.begin() + static_cast<std::ptrdiff_t>(performance.count),
        [](const BassPlayedEvent& left, const BassPlayedEvent& right) {
            if (left.timeSeconds != right.timeSeconds)
                return left.timeSeconds < right.timeSeconds;
            return left.formCycle < right.formCycle;
        });
    return BassError::none;
}

const char* bassKeyScaleName(const int scale) noexcept {
    static constexpr const char* kNames[kBassKeyScaleCount] = {
        "maior", "menor", "dórico", "mixolídio", "frígio", "lídio",
        "menor harmônica", "pentatônica maior", "pentatônica menor", "blues"};
    return scale >= 0 && scale < kBassKeyScaleCount ? kNames[scale] : "cromático";
}

const char* bassKeyTonicName(const int tonic) noexcept {
    static constexpr const char* kNames[12] = {
        "Dó", "Dó♯", "Ré", "Mi♭", "Mi", "Fá", "Fá♯", "Sol", "Lá♭", "Lá", "Si♭", "Si"};
    return tonic >= 0 && tonic < 12 ? kNames[tonic] : "—";
}

int quantizeBassToKey(const int midi, const int tonic, const int scale) noexcept {
    const int bounded = std::clamp(midi, 24, 60);
    if (tonic < 0 || tonic > 11 || scale < 0 || scale >= kBassKeyScaleCount)
        return bounded;
    int best = bounded;
    int bestDistance = 1000;
    for (int candidate = 24; candidate <= 60; ++candidate) {
        const int degree = ((candidate - tonic) % 12 + 12) % 12;
        bool inScale = false;
        for (const int step : kBassKeyScales[static_cast<std::size_t>(scale)])
            if (step == degree) inScale = true;
        if (!inScale) continue;
        const int distance = std::abs(candidate - bounded);
        if (distance < bestDistance) { bestDistance = distance; best = candidate; }
    }
    return best;
}

int nearestBassTonic(const int midi, const int tonic) noexcept {
    if (tonic < 0 || tonic > 11)
        return std::clamp(midi, 28, 48);
    int best = 36 + tonic;
    for (int candidate = 28; candidate <= 48; ++candidate)
        if ((candidate - tonic) % 12 == 0
            && std::abs(candidate - midi) < std::abs(best - midi))
            best = candidate;
    return best;
}

} // namespace batruc
