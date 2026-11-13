// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/CompositionCore.hpp"

#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <numeric>

namespace {

struct Vector {
    std::uint32_t seed;
    std::size_t cycle;
    std::size_t phase;
    std::size_t rule;
    std::size_t pattern;
    std::size_t voices;
    std::size_t law;
    float amount;
};

constexpr std::array<Vector, 5> vectors = {{
    {0x00000000u, 7, 20, 0, 1, 8, 3, 0.72f},
    {0x4E415632u, 2, 58, 0, 2, 2, 5, 0.57f},
    {0xA1B2C3D4u, 7, 1, 1, 0, 8, 6, 0.54f},
    {0xFFFFFFFFu, 1, 14, 1, 2, 8, 7, 0.80f},
    {0x01234567u, 7, 25, 1, 0, 8, 3, 0.50f}
}};

} // namespace

int main() {
    auto editedRecipe = batruc::makeInitialRecipe();
    auto& cell = editedRecipe.rhythmicCells[0];
    cell.enabled = true;
    if (!batruc::insertRhythmicCellAttack(cell, 0.0, 0.5f)
        || !batruc::insertRhythmicCellAttack(cell, 0.375, 1.0f)
        || !batruc::insertRhythmicCellAttack(cell, 0.75, 0.7f)
        || batruc::insertRhythmicCellAttack(cell, 0.375, 0.8f)
        || !batruc::moveRhythmicCellAttack(cell, 1, 0.5)
        || !batruc::removeRhythmicCellAttack(cell, 2)) {
        std::cerr << "rhythmic cell edit mismatch\n";
        return 9;
    }
    batruc::EventPlan editedPlan;
    if (batruc::buildEventPlan(editedRecipe, editedPlan)
        != batruc::PlanError::none) {
        std::cerr << "rhythmic cell plan failed\n";
        return 9;
    }
    std::size_t editedCount = 0;
    for (const auto& event : editedPlan.events) {
        if (event.voice != 0)
            continue;
        const double expected = editedCount == 0 ? 0.0 : 0.5;
        if (editedCount >= 2
            || std::abs(event.timeSeconds / editedRecipe.sectionSeconds
                - expected) > 1.0e-12
            || std::abs(event.intensity - (editedCount == 0 ? 0.5f : 1.0f))
                > 1.0e-6f) {
            std::cerr << "rhythmic cell event mismatch\n";
            return 9;
        }
        ++editedCount;
    }
    if (editedCount != 2) {
        std::cerr << "rhythmic cell count mismatch\n";
        return 9;
    }

    auto liveRecipe = batruc::makeInitialRecipe();
    liveRecipe.sectionSeconds = 1.0;
    batruc::EventPlan livePlan;
    if (batruc::buildEventPlan(liveRecipe, livePlan)
        != batruc::PlanError::none)
        return 9;
    batruc::ResonatorRenderer liveHitRenderer;
    batruc::ResonatorRenderer liveSilentRenderer;
    if (!liveHitRenderer.configure(liveRecipe, livePlan, 8000.0)
        || !liveSilentRenderer.configure(liveRecipe, livePlan, 8000.0))
        return 9;
    // Integration check: changing only the Mixer coupling must alter the
    // rendered second voice, while the common event calendar stays fixed.
    batruc::ResonatorRenderer uncoupled;
    batruc::ResonatorRenderer coupled;
    if (!uncoupled.configure(liveRecipe, livePlan, 8000.0)
        || !coupled.configure(liveRecipe, livePlan, 8000.0))
        return 9;
    uncoupled.setActiveVoiceCount(2);
    coupled.setActiveVoiceCount(2);
    uncoupled.setFormalVoiceReductionEnabled(false);
    coupled.setFormalVoiceReductionEnabled(false);
    std::array<batruc::VoiceTransform, batruc::kMaxVoices> transforms{};
    transforms.fill(batruc::VoiceTransform::sine);
    std::array<float, batruc::kMaxVoices> couplingValues{}, formulaOffsets{};
    formulaOffsets[1] = 0.19f;
    uncoupled.setVoiceFormulaData(transforms.data(), couplingValues.data(),
                                  formulaOffsets.data());
    couplingValues[1] = 0.8f;
    coupled.setVoiceFormulaData(transforms.data(), couplingValues.data(),
                                formulaOffsets.data());
    std::array<float, 8000> dryLeft{}, dryRight{}, wetLeft{}, wetRight{};
    uncoupled.process(dryLeft.data(), dryRight.data(), dryLeft.size());
    coupled.process(wetLeft.data(), wetRight.data(), wetLeft.size());
    double couplingDifference = 0.0;
    for (std::size_t sample = 0; sample < dryLeft.size(); ++sample)
        couplingDifference += std::abs(dryLeft[sample] - wetLeft[sample]);
    if (couplingDifference < 0.05) {
        std::cerr << "Mixer coupling did not change rendered audio: "
                  << couplingDifference << "\n";
        return 9;
    }
    if (!(uncoupled.voiceRms(0) > 0.0f)
        || !(uncoupled.voiceRms(1) > 0.0f)
        || uncoupled.voiceRms(batruc::kMaxActiveVoices) != 0.0f) {
        std::cerr << "stem signal meter did not follow rendered voices\n";
        return 9;
    }
    uncoupled.reset();
    if (uncoupled.voiceRms(0) != 0.0f
        || uncoupled.voiceRms(1) != 0.0f) {
        std::cerr << "stem signal meter did not reset\n";
        return 9;
    }
    std::array<float, 6000> hitLeft{}, hitRight{}, silentLeft{}, silentRight{};
    liveHitRenderer.process(hitLeft.data(), hitRight.data(), 2500);
    liveSilentRenderer.process(silentLeft.data(), silentRight.data(), 2500);
    std::array<batruc::RhythmicCell, batruc::kMaxVoices> hitCells{}, silentCells{};
    hitCells[0].enabled = true;
    silentCells[0].enabled = true;
    if (!batruc::insertRhythmicCellAttack(hitCells[0], 0.4, 1.0f))
        return 9;
    liveHitRenderer.setRhythmicCells(hitCells);
    liveSilentRenderer.setRhythmicCells(silentCells);
    liveHitRenderer.process(hitLeft.data() + 2500, hitRight.data() + 2500, 3500);
    liveSilentRenderer.process(silentLeft.data() + 2500,
        silentRight.data() + 2500, 3500);
    double differenceBeforeAttack = 0.0;
    double differenceAfterAttack = 0.0;
    for (std::size_t sample = 2500; sample < 6000; ++sample) {
        const double difference = std::abs(hitLeft[sample] - silentLeft[sample]);
        if (sample < 3200)
            differenceBeforeAttack += difference;
        else
            differenceAfterAttack += difference;
    }
    if (differenceBeforeAttack > 1.0e-5
        || differenceAfterAttack < 1.0e-3) {
        std::cerr << "live rhythmic cell did not sound before the next bar\n";
        return 9;
    }
    const double progressBeforeTempoEdit = liveHitRenderer.sectionProgress();
    liveHitRenderer.setTimeSignature(3, 4);
    liveHitRenderer.setLiveSectionSamples(6000);
    if (liveHitRenderer.activeSectionSamples() != 6000
        || std::abs(liveHitRenderer.sectionProgress()
                    - progressBeforeTempoEdit) > 1.0e-4) {
        std::cerr << "live tempo edit lost the playhead phase\n";
        return 9;
    }

    constexpr std::array<batruc::SoundCharacter, batruc::kMaxActiveVoices>
        expectedKitRoles = {batruc::SoundCharacter::kick,
            batruc::SoundCharacter::snare, batruc::SoundCharacter::hihat,
            batruc::SoundCharacter::openHihat, batruc::SoundCharacter::membrane,
            batruc::SoundCharacter::ride, batruc::SoundCharacter::conga,
            batruc::SoundCharacter::brushes};
    auto styleRecipe = liveRecipe;
    styleRecipe.voiceCount = batruc::kMaxActiveVoices;
    for (std::size_t voice = 2; voice < styleRecipe.voiceCount; ++voice) {
        styleRecipe.voices[voice] = liveRecipe.voices[1];
        styleRecipe.voices[voice].pan = -0.4f
            + 0.8f * static_cast<float>(voice - 1u)
                / static_cast<float>(styleRecipe.voiceCount - 2u);
    }
    batruc::EventPlan stylePlan;
    if (batruc::buildEventPlan(styleRecipe, stylePlan)
        != batruc::PlanError::none)
        return 9;
    for (int family = 0; family < 17; ++family) {
        if (batruc::soundFamilyVoiceSounds(family) != expectedKitRoles) {
            std::cerr << "family changed the stable kit role assignment\n";
            return 9;
        }
    }
    std::array<batruc::RhythmicCell, batruc::kMaxVoices> styleCells{};
    styleCells[0].enabled = true;
    if (!batruc::insertRhythmicCellAttack(styleCells[0], 0.0, 1.0f))
        return 9;
    constexpr std::size_t styleRenderSamples = 4096;
    std::array<std::array<float, styleRenderSamples>, 17> familyOutputs{};
    std::array<float, batruc::kMaxVoices> familyLevels{};
    familyLevels[0] = 1.0f;
    batruc::ResonatorRenderer familyRenderer;
    for (int family = 0; family < 17; ++family) {
        if (!familyRenderer.configure(liveRecipe, livePlan, 8000.0))
            return 9;
        familyRenderer.setActiveVoiceCount(batruc::kMaxActiveVoices);
        familyRenderer.setFormalVoiceReductionEnabled(false);
        familyRenderer.setChannelSound(0, batruc::SoundCharacter::kick);
        familyRenderer.setChannelTimbreProfile(0, family);
        familyRenderer.setRhythmicCells(styleCells);
        std::array<float, styleRenderSamples> left{}, right{};
        familyRenderer.processVariantLive(left.data(), right.data(), left.size(),
            0, familyLevels.data());
        familyOutputs[static_cast<std::size_t>(family)] = left;
    }
    for (std::size_t first = 0; first < familyOutputs.size(); ++first) {
        for (std::size_t second = first + 1; second < familyOutputs.size(); ++second) {
            double familyDifference = 0.0;
            double firstEnergy = 0.0;
            double secondEnergy = 0.0;
            for (std::size_t sample = 0; sample < styleRenderSamples; ++sample) {
                const double firstSample = familyOutputs[first][sample];
                const double secondSample = familyOutputs[second][sample];
                firstEnergy += firstSample * firstSample;
                secondEnergy += secondSample * secondSample;
            }
            const double gainMatch = firstEnergy > 1.0e-20
                && secondEnergy > 1.0e-20
                ? std::sqrt(firstEnergy / secondEnergy) : 1.0;
            double firstMagnitude = 0.0;
            for (std::size_t sample = 0; sample < styleRenderSamples; ++sample) {
                familyDifference += std::abs(familyOutputs[first][sample]
                    - gainMatch * familyOutputs[second][sample]);
                firstMagnitude += std::abs(familyOutputs[first][sample]);
            }
            const double normalizedDifference = familyDifference
                / std::max(firstMagnitude, 1.0e-12);
            if (!std::isfinite(normalizedDifference)
                || normalizedDifference < 1.0e-3) {
                std::cerr << "style profiles differ only by gain or negligible "
                             "kick timbre: " << first << " and " << second
                          << " (normalized difference " << normalizedDifference
                          << ")\n";
                return 9;
            }
        }
    }
    for (std::size_t voice = 1; voice < batruc::kMaxActiveVoices; ++voice) {
        auto roleCells = std::array<batruc::RhythmicCell,
            batruc::kMaxVoices>{};
        roleCells[voice].enabled = true;
        if (!batruc::insertRhythmicCellAttack(roleCells[voice], 0.0, 1.0f))
            return 9;
        std::array<std::array<float, styleRenderSamples>, 17> roleOutputs{};
        std::array<float, batruc::kMaxVoices> roleLevels{};
        roleLevels[voice] = 1.0f;
        for (int family = 0; family < 17; ++family) {
            batruc::ResonatorRenderer renderer;
            if (!renderer.configure(styleRecipe, stylePlan, 8000.0))
                return 9;
            renderer.setActiveVoiceCount(batruc::kMaxActiveVoices);
            renderer.setFormalVoiceReductionEnabled(false);
            renderer.setChannelSound(voice, expectedKitRoles[voice]);
            renderer.setChannelTimbreProfile(voice, family);
            renderer.setRhythmicCells(roleCells);
            std::array<float, styleRenderSamples> left{}, right{};
            renderer.processVariantLive(left.data(), right.data(), left.size(),
                0, roleLevels.data());
            roleOutputs[static_cast<std::size_t>(family)] = left;
        }
        for (std::size_t first = 0; first < roleOutputs.size(); ++first) {
            for (std::size_t second = first + 1;
                 second < roleOutputs.size(); ++second) {
                double firstEnergy = 0.0;
                double secondEnergy = 0.0;
                for (std::size_t sample = 0; sample < styleRenderSamples;
                     ++sample) {
                    const double a = roleOutputs[first][sample];
                    const double b = roleOutputs[second][sample];
                    firstEnergy += a * a;
                    secondEnergy += b * b;
                }
                if (firstEnergy <= 1.0e-20 || secondEnergy <= 1.0e-20)
                    continue;
                const double gainMatch = std::sqrt(firstEnergy / secondEnergy);
                double error = 0.0;
                double referenceMagnitude = 0.0;
                for (std::size_t sample = 0; sample < styleRenderSamples;
                     ++sample) {
                    const double a = roleOutputs[first][sample];
                    const double b = roleOutputs[second][sample] * gainMatch;
                    error += std::abs(a - b);
                    referenceMagnitude += std::abs(a);
                }
                if (error / std::max(referenceMagnitude, 1.0e-12) < 1.0e-3) {
                    std::cerr << "two style profiles differ only by gain or "
                                 "negligible timbre on stem V"
                              << (voice + 1u) << ": " << first << " and "
                              << second << '\n';
                    return 9;
                }
            }
        }
        std::size_t audibleProfiles = 0;
        for (std::size_t family = 0; family < roleOutputs.size(); ++family) {
            double energy = 0.0;
            for (const float sample : roleOutputs[family])
                energy += static_cast<double>(sample) * sample;
            audibleProfiles += energy > 1.0e-20 ? 1u : 0u;
            if (family == 8u && energy <= 1.0e-20) {
                std::cerr << "8-bit family silenced stem V" << (voice + 1u)
                          << '\n';
                return 9;
            }
        }
        if (audibleProfiles < 2u) {
            std::cerr << "style profiles did not produce enough audible options on stem V"
                      << (voice + 1u) << '\n';
            return 9;
        }
    }
    // A family edit during an existing snare tail must affect this audio
    // block, with the playhead intact and before any later cell attack.
    batruc::ResonatorRenderer unchangedFamily;
    batruc::ResonatorRenderer changedFamily;
    if (!unchangedFamily.configure(liveRecipe, livePlan, 8000.0)
        || !changedFamily.configure(liveRecipe, livePlan, 8000.0))
        return 9;
    for (auto* renderer : {&unchangedFamily, &changedFamily}) {
        renderer->setChannelSound(0, batruc::SoundCharacter::snare);
        renderer->setChannelTimbreProfile(0, 0);
        renderer->setRhythmicCells(styleCells);
    }
    std::array<float, 256> beforeLeftA{}, beforeRightA{};
    std::array<float, 256> beforeLeftB{}, beforeRightB{};
    unchangedFamily.process(beforeLeftA.data(), beforeRightA.data(),
                            beforeLeftA.size());
    changedFamily.process(beforeLeftB.data(), beforeRightB.data(),
                          beforeLeftB.size());
    if (beforeLeftA != beforeLeftB)
        return 9;
    changedFamily.setChannelTimbreProfile(0, 1);
    std::array<float, 512> afterLeftA{}, afterRightA{};
    std::array<float, 512> afterLeftB{}, afterRightB{};
    unchangedFamily.process(afterLeftA.data(), afterRightA.data(),
                            afterLeftA.size());
    changedFamily.process(afterLeftB.data(), afterRightB.data(),
                          afterLeftB.size());
    std::size_t firstChangedSample = afterLeftA.size();
    for (std::size_t sample = 0; sample < afterLeftA.size(); ++sample)
        if (std::abs(afterLeftA[sample] - afterLeftB[sample]) > 1.0e-7f) {
            firstChangedSample = sample;
            break;
        }
    if (firstChangedSample > 16
        || std::abs(unchangedFamily.sectionProgress()
                    - changedFamily.sectionProgress()) > 1.0e-9) {
        std::cerr << "live family edit waited for a new attack or reset form\n";
        return 9;
    }
    batruc::ResonatorRenderer unchangedLevel;
    batruc::ResonatorRenderer changedLevel;
    if (!unchangedLevel.configure(liveRecipe, livePlan, 8000.0)
        || !changedLevel.configure(liveRecipe, livePlan, 8000.0))
        return 9;
    for (auto* renderer : {&unchangedLevel, &changedLevel}) {
        renderer->setChannelSound(0, batruc::SoundCharacter::snare);
        renderer->setRhythmicCells(styleCells);
    }
    unchangedLevel.process(beforeLeftA.data(), beforeRightA.data(),
                           beforeLeftA.size());
    changedLevel.process(beforeLeftB.data(), beforeRightB.data(),
                         beforeLeftB.size());
    std::array<float, batruc::kMaxVoices> liveLevels{};
    liveLevels.fill(1.0f);
    unchangedLevel.processVariantLive(afterLeftA.data(), afterRightA.data(),
        afterLeftA.size(), 0, liveLevels.data());
    liveLevels[0] = 0.0f;
    changedLevel.processVariantLive(afterLeftB.data(), afterRightB.data(),
        afterLeftB.size(), 0, liveLevels.data());
    std::size_t firstLevelDifference = afterLeftA.size();
    for (std::size_t sample = 0; sample < afterLeftA.size(); ++sample)
        if (std::abs(afterLeftA[sample] - afterLeftB[sample]) > 1.0e-7f) {
            firstLevelDifference = sample;
            break;
        }
    if (firstLevelDifference > 16
        || std::abs(unchangedLevel.sectionProgress()
                    - changedLevel.sectionProgress()) > 1.0e-9) {
        std::cerr << "live level edit waited for a new attack or reset form\n";
        return 9;
    }
    auto digitalKickValues = familyOutputs[8];
    auto rockKickValues = familyOutputs[0];
    std::sort(digitalKickValues.begin(), digitalKickValues.end());
    std::sort(rockKickValues.begin(), rockKickValues.end());
    const auto digitalKickSteps = std::unique(digitalKickValues.begin(),
        digitalKickValues.end()) - digitalKickValues.begin();
    const auto rockKickSteps = std::unique(rockKickValues.begin(),
        rockKickValues.end()) - rockKickValues.begin();
    if (digitalKickSteps < 3 || digitalKickSteps > 65
        || rockKickSteps <= digitalKickSteps) {
        std::cerr << "8-bit family did not quantize the kick body\n";
        return 9;
    }
    // The digital profile must also reach another kit role, without turning
    // that role into the generic Chip 8-bit instrument.
    auto renderSnareFamily = [&](const int profile) {
        batruc::ResonatorRenderer renderer;
        renderer.configure(liveRecipe, livePlan, 8000.0);
        renderer.setChannelSound(0, batruc::SoundCharacter::snare);
        renderer.setChannelTimbreProfile(0, profile);
        renderer.setRhythmicCells(styleCells);
        std::array<float, styleRenderSamples> left{}, right{};
        renderer.process(left.data(), right.data(), left.size());
        return left;
    };
    auto digitalSnare = renderSnareFamily(8);
    auto plainSnare = renderSnareFamily(-1);
    const double digitalSnareEnergy = std::accumulate(digitalSnare.begin(),
        digitalSnare.end(), 0.0,
        [](const double total, const float value) {
            return total + static_cast<double>(value) * value;
        });
    std::sort(digitalSnare.begin(), digitalSnare.end());
    std::sort(plainSnare.begin(), plainSnare.end());
    const auto digitalSnareSteps = std::unique(digitalSnare.begin(),
        digitalSnare.end()) - digitalSnare.begin();
    const auto plainSnareSteps = std::unique(plainSnare.begin(),
        plainSnare.end()) - plainSnare.begin();
    if (digitalSnareEnergy < 1.0e-5 || digitalSnareSteps < 3
        || digitalSnareSteps > 33 || plainSnareSteps <= digitalSnareSteps) {
        std::cerr << "8-bit family did not preserve a quantized snare\n";
        return 9;
    }
    // GM notes in one mixer lane must change the rendered material, not only
    // an excitation parameter while the lane oscillator stays fixed.
    auto midiLaneAudio = [&](const std::uint8_t note) {
        std::array<float, 6000> left{}, right{};
        auto cells = styleCells;
        cells[1] = {};
        cells[1].enabled = true;
        batruc::insertRhythmicCellAttack(cells[1], 0.5, 1.0f);
        cells[1].midiNotes[0] = note;
        batruc::ResonatorRenderer renderer;
        renderer.configure(liveRecipe, livePlan, 8000.0);
        renderer.setActiveVoiceCount(2);
        renderer.setFormalVoiceReductionEnabled(false);
        renderer.setChannelSound(0, batruc::SoundCharacter::kick);
        renderer.setChannelSound(1, batruc::SoundCharacter::snare);
        renderer.setRhythmicCells(cells);
        renderer.process(left.data(), right.data(), left.size());
        return left;
    };
    const auto sideStick = midiLaneAudio(37);
    const auto snare = midiLaneAudio(38);
    double midiArticulationDifference = 0.0;
    for (std::size_t sample = 4000; sample < sideStick.size(); ++sample)
        midiArticulationDifference += std::abs(sideStick[sample] - snare[sample]);
    if (midiArticulationDifference < 0.05) {
        std::cerr << "GM side stick and snare too similar: "
                  << midiArticulationDifference << '\n';
        return 9;
    }

    constexpr std::array<double, 7> expectedOffsets = {0, 0, 0, 0.5, 1, 1, 0.5};
    constexpr std::array<double, 7> expectedSlopes = {0, 0, 0.5, 0.5, 0, -0.5, -0.5};
    for (std::size_t cycle = 0; cycle < batruc::kFormalCycleCount; ++cycle) {
        for (const double amount : {0.0, 0.1, 0.125, 0.25, 0.5}) {
            const auto segment = batruc::formalPhaseSegmentForCycle(cycle, amount);
            if (std::abs(segment.offsetCycles - expectedOffsets[cycle] * amount) > 1e-12
                || std::abs(segment.slopeCycles - expectedSlopes[cycle] * amount) > 1e-12) {
                std::cerr << "formal phase arc mismatch at cycle " << cycle << '\n';
                return 7;
            }
        }
        for (std::size_t voices = 2; voices <= batruc::kMaxVoices; ++voices) {
            const auto expected = cycle == 0 ? 1u
                : cycle == 1 || cycle == 5 ? (voices + 2) / 2 : voices;
            if (batruc::activeVoiceCountInCycle(cycle, voices) != expected) {
                std::cerr << "formal voice entry mismatch at cycle " << cycle << '\n';
                return 8;
            }
        }
    }

    constexpr std::array<std::array<std::size_t, 3>, 7> formalCounts = {{
        {{6, 0, 0}}, {{6, 16, 1}}, {{6, 16, 1}}, {{6, 16, 1}},
        {{6, 16, 1}}, {{6, 16, 1}}, {{6, 16, 1}}
    }};
    constexpr std::array<std::array<double, 3>, 7> formalFirst = {{
        {{0.0, 0.0, 0.0}}, {{0.0, 0.046875, 0.601}},
        {{0.0, 0.046511627906976744, 0.5342222222222222}},
        {{0.0, 0.03875968992248062, 0.4231111111111111}},
        {{0.0, 0.03125, 0.351}}, {{0.0, 0.031496062992125984, 0.40114285714285713}},
        {{0.0, 0.03937007874015748, 0.544}}
    }};
    constexpr std::array<std::array<double, 3>, 7> formalLast = {{
        {{5.0 / 6.0, 0.0, 0.0}}, {{5.0 / 6.0, 0.984375, 0.601}},
        {{5.0 / 6.0, 0.9767441860465116, 0.5342222222222222}},
        {{5.0 / 6.0, 0.9689922480620154, 0.4231111111111111}},
        {{5.0 / 6.0, 0.96875, 0.351}}, {{5.0 / 6.0, 0.9763779527559056, 0.40114285714285713}},
        {{5.0 / 6.0, 0.984251968503937, 0.544}}
    }};
    auto formalBase = batruc::makeInitialRecipe();
    formalBase.sectionSeconds = 8.0;
    formalBase.voiceCount = 8;
    formalBase.patternMode = batruc::PatternMode::unrestricted;
    constexpr std::array<double, 8> formalCycles = {
        3.0, 8.0, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75};
    constexpr std::array<double, 8> formalPhases = {
        0.0, 0.125, 0.1995, 0.399, 0.5985, 0.798, 0.9975, 1.197};
    for (std::size_t voice = 2; voice < formalBase.voiceCount; ++voice)
        formalBase.voices[voice] = formalBase.voices[1];
    for (std::size_t voice = 0; voice < formalBase.voiceCount; ++voice) {
        formalBase.voices[voice].cyclesPerSection = formalCycles[voice];
        formalBase.voices[voice].phaseCycles = formalPhases[voice];
        formalBase.voices[voice].eventRule = batruc::EventRule::centreCrossings;
    }
    for (std::size_t cycle = 0; cycle < formalCounts.size(); ++cycle) {
        auto cycleRecipe = formalBase;
        const auto phaseSegment = batruc::formalPhaseSegmentForCycle(cycle, 0.125);
        for (std::size_t voice = 1; voice < cycleRecipe.voiceCount; ++voice) {
            cycleRecipe.voices[voice].cyclesPerSection += phaseSegment.slopeCycles;
            cycleRecipe.voices[voice].phaseCycles += phaseSegment.offsetCycles;
        }
        batruc::EventPlan cyclePlan;
        if (batruc::buildEventPlan(cycleRecipe, cyclePlan) != batruc::PlanError::none)
            return 9;
        const auto activeCount = batruc::activeVoiceCountInCycle(cycle, 8);
        for (std::size_t voice = 0; voice < 3; ++voice) {
            std::size_t count = 0;
            double first = -1.0;
            double last = -1.0;
            if (voice < activeCount) {
                for (const auto& event : cyclePlan.events) {
                    if (event.voice != voice)
                        continue;
                    const double position = event.timeSeconds / cycleRecipe.sectionSeconds;
                    if (first < 0.0)
                        first = position;
                    last = position;
                    ++count;
                }
            }
            if (count != formalCounts[cycle][voice]
                || (count > 0 && (std::abs(first - formalFirst[cycle][voice]) > 1e-9
                    || std::abs(last - formalLast[cycle][voice]) > 1e-9))) {
                std::cerr << "seven-cycle entry/schedule mismatch at cycle "
                          << cycle + 1 << ", voice " << voice + 1 << '\n';
                return 9;
            }
        }
    }

    constexpr std::array<double, 4> quarterNotesPerBarredSection = {12, 16, 20, 14};
    constexpr std::array<int, 4> tempos = {60, 97, 120, 220};
    constexpr std::array<double, 3> swingRatios = {0.5, 0.57, 0.67};
    for (const auto quarterNotes : quarterNotesPerBarredSection) {
        for (const auto bpm : tempos) {
            for (const auto ratio : swingRatios) {
                for (int beat = 0; beat < static_cast<int>(quarterNotes); ++beat) {
                    const double straight = (static_cast<double>(beat) + 0.5)
                        / quarterNotes;
                    const double performed = batruc::swingProgress(
                        straight, ratio, quarterNotes);
                    const double expectedTime = (static_cast<double>(beat) + ratio)
                        * 60.0 / bpm;
                    const double actualTime = performed * quarterNotes * 60.0 / bpm;
                    if (std::abs(actualTime - expectedTime) > 1.0e-10) {
                        std::cerr << "swing timing mismatch: " << quarterNotes
                                  << " quarter notes, " << bpm << " BPM\n";
                        return 6;
                    }
                    const double downbeat = static_cast<double>(beat) / quarterNotes;
                    if (std::abs(batruc::swingProgress(downbeat, ratio,
                            quarterNotes) - downbeat) > 1.0e-12) {
                        std::cerr << "swing moved a beat boundary\n";
                        return 6;
                    }
                }
            }
        }
    }

    constexpr std::array<const char*, 12> expectedPresetLabels = {
        "4:3", "5:4", "6:4", "7:4", "8:6", "10:4", "10:7",
        "2:3:5", "3:4:5", "4:5:7", "3:4:5:7", "3:4:5:7:8"};
    constexpr std::array<std::array<double, 5>, 12> expectedPresetCycles = {{
        {{4, 3, 0, 0, 0}}, {{5, 4, 0, 0, 0}}, {{6, 4, 0, 0, 0}},
        {{7, 4, 0, 0, 0}}, {{8, 6, 0, 0, 0}}, {{10, 4, 0, 0, 0}},
        {{10, 7, 0, 0, 0}}, {{2, 3, 5, 0, 0}}, {{3, 4, 5, 0, 0}},
        {{4, 5, 7, 0, 0}}, {{3, 4, 5, 7, 0}}, {{3, 4, 5, 7, 8}}
    }};
    constexpr std::array<std::size_t, 12> expectedPresetVoices = {
        2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 4, 5};
    for (std::size_t index = 0; index < batruc::kPolyrhythmPresets.size(); ++index) {
        const auto& preset = batruc::kPolyrhythmPresets[index];
        if (std::strcmp(preset.label, expectedPresetLabels[index]) != 0
            || preset.cycles != expectedPresetCycles[index]
            || preset.voiceCount != expectedPresetVoices[index]) {
            std::cerr << "polyrhythm preset mismatch at " << index << '\n';
            return 5;
        }
    }

    std::array<float, batruc::kMaxVoices> existingCycles{};
    for (std::size_t voice = 0; voice < existingCycles.size(); ++voice)
        existingCycles[voice] = 1.0f + static_cast<float>(voice) * 0.25f;
    const auto appliedPoly = batruc::applyPolyrhythmPreset(11, 8, existingCycles);
    if (!appliedPoly.valid || appliedPoly.voiceCount != 8
        || appliedPoly.cycles[0] != 3.0f || appliedPoly.cycles[1] != 4.0f
        || appliedPoly.cycles[2] != 5.0f || appliedPoly.cycles[3] != 7.0f
        || appliedPoly.cycles[4] != 8.0f
        || appliedPoly.cycles[5] != existingCycles[5]
        || appliedPoly.cycles[11] != existingCycles[11]) {
        std::cerr << "polyrhythm application did not preserve remaining channels\n";
        return 7;
    }
    if (batruc::applyPolyrhythmPreset(12, 8, existingCycles).valid
        || batruc::applyPolyrhythmPreset(0, 1, existingCycles).valid) {
        std::cerr << "invalid polyrhythm application was accepted\n";
        return 8;
    }

    for (const auto& vector : vectors) {
        const auto actual = batruc::seededRecipeSelection(vector.seed);
        if (actual.voiceTwoCycleIndex != vector.cycle
            || actual.phaseIndex != vector.phase
            || actual.eventRuleIndex != vector.rule
            || actual.patternIndex != vector.pattern
            || actual.voiceCount != vector.voices
            || actual.intensityLawIndex != vector.law
            || std::abs(actual.intensityAmount - vector.amount) > 1.0e-6f) {
            std::cerr << "seeded recipe mismatch for " << vector.seed << '\n';
            return 1;
        }
    }

    for (const auto seed : std::array<std::uint32_t, 5>{
             0u, 1u, 0x5EED2027u, 0xA1B2C3D4u, 0xFFFFFFFFu}) {
        const auto selection = batruc::seededRecipeSelection(seed);
        const auto repeated = batruc::seededRecipeSelection(seed);
        if (selection.soundFamilyIndex >= 17 || selection.grooveIndex >= 4
            || selection.intentionIndex >= 4 || selection.dynamicsIndex >= 4
            || selection.meterIndex >= 4
            || selection.voiceTwoTravelPerBar < 0.0f
            || selection.voiceTwoTravelPerBar > 0.125f
            || selection.geometryShapeIndex >= 17
            || selection.billiardPatternIndex >= 7
            || selection.geometryFourierTerms < 1
            || selection.geometryFourierTerms > 32
            || selection.lissajousXFrequency < 1
            || selection.lissajousXFrequency > 12
            || selection.lissajousYFrequency < 1
            || selection.lissajousYFrequency > 12
            || selection.lissajousPhase < 0.0f
            || selection.lissajousPhase > 1.0f
            || selection.lissajousXAmplitude < 0.25f
            || selection.lissajousXAmplitude > 1.0f
            || selection.lissajousYAmplitude < 0.25f
            || selection.lissajousYAmplitude > 1.0f
            || selection.compositionVariation < 0.25f
            || selection.compositionVariation > 1.0f
            || selection.soundFamilyIndex != repeated.soundFamilyIndex
            || selection.grooveIndex != repeated.grooveIndex
            || selection.intentionIndex != repeated.intentionIndex
            || selection.dynamicsIndex != repeated.dynamicsIndex
            || selection.meterIndex != repeated.meterIndex
            || selection.voiceTwoTravelPerBar
                != repeated.voiceTwoTravelPerBar
            || selection.geometryVoiceOneSource
                != repeated.geometryVoiceOneSource
            || selection.geometryShapeIndex != repeated.geometryShapeIndex
            || selection.geometryFourierTerms != repeated.geometryFourierTerms
            || selection.billiardPatternIndex != repeated.billiardPatternIndex
            || selection.lissajousXFrequency != repeated.lissajousXFrequency
            || selection.lissajousYFrequency != repeated.lissajousYFrequency
            || selection.lissajousPhase != repeated.lissajousPhase
            || selection.lissajousXAmplitude != repeated.lissajousXAmplitude
            || selection.lissajousYAmplitude != repeated.lissajousYAmplitude
            || selection.compositionVariation != repeated.compositionVariation) {
            std::cerr << "seeded extended controls mismatch\n";
            return 12;
        }
        for (std::size_t voice = 0; voice < batruc::kMaxActiveVoices; ++voice) {
            if (selection.chipDutyCycles[voice] < 0.125f
                || selection.chipDutyCycles[voice] > 0.875f
                || selection.chipClockHz[voice] < 1000.0f
                || selection.chipClockHz[voice] > 24000.0f
                || selection.chipBitDepth[voice] < 2
                || selection.chipBitDepth[voice] > 8
                || selection.chipNoiseMix[voice] < 0.0f
                || selection.chipNoiseMix[voice] > 1.0f
                || selection.voiceBaseCouplings[voice] < 0.0f
                || selection.voiceBaseCouplings[voice] > 0.8f
                || selection.voiceBasePhases[voice] < -0.5f
                || selection.voiceBasePhases[voice] > 0.5f
                || selection.voiceBaseTransforms[voice] < 0
                || selection.voiceBaseTransforms[voice] > 9
                || selection.formulaPhaseOffsets[voice] < -0.5f
                || selection.formulaPhaseOffsets[voice] > 0.5f
                || selection.voiceVariationDepths[voice] < 0.0f
                || selection.voiceVariationDepths[voice] > 1.0f
                || selection.chipDutyCycles[voice]
                    != repeated.chipDutyCycles[voice]
                || selection.chipClockHz[voice] != repeated.chipClockHz[voice]
                || selection.chipBitDepth[voice] != repeated.chipBitDepth[voice]
                || selection.chipNoiseMix[voice] != repeated.chipNoiseMix[voice]
                || selection.voiceBaseCouplings[voice]
                    != repeated.voiceBaseCouplings[voice]
                || selection.voiceBasePhases[voice]
                    != repeated.voiceBasePhases[voice]
                || selection.voiceBaseTransforms[voice]
                    != repeated.voiceBaseTransforms[voice]
                || selection.formulaPhaseOffsets[voice]
                    != repeated.formulaPhaseOffsets[voice]
                || selection.voiceVariationDepths[voice]
                    != repeated.voiceVariationDepths[voice]) {
                std::cerr << "seeded chip controls mismatch\n";
                return 13;
            }
        }
        const auto next = batruc::algorithmicNextSeed(seed);
        if (next == seed || next != batruc::algorithmicNextSeed(seed)) {
            std::cerr << "contrasting next seed is not distinct and deterministic\n";
            return 9;
        }
    }

    // Startup must exclude the persisted history and the current seed even if
    // every proposal up to that point collides with one of them.
    std::array<std::uint32_t, 64> recentSeeds{};
    constexpr std::uint32_t startupCandidate = 0x13579bdfu;
    auto startupState = startupCandidate;
    for (auto& recent : recentSeeds)
        recent = batruc::nextSeedWord(startupState);
    const auto currentSeed = batruc::nextSeedWord(startupState);
    const auto freshStartupSeed = batruc::freshSeedOutsideRecent(
        startupCandidate, recentSeeds.data(), recentSeeds.size(), true,
        currentSeed);
    const auto repeatedStartupSeed = batruc::freshSeedOutsideRecent(
        startupCandidate, recentSeeds.data(), recentSeeds.size(), true,
        currentSeed);
    if (freshStartupSeed != repeatedStartupSeed
        || freshStartupSeed == currentSeed
        || std::find(recentSeeds.begin(), recentSeeds.end(), freshStartupSeed)
            != recentSeeds.end()) {
        std::cerr << "startup seed selection repeated a recent or current seed\n";
        return 9;
    }

    // A sparse diagonal mask must not make the first formal cycle silent when
    // only V1 is active. These values reproduce the previously silent core of
    // seed 00000010 with two selected voices.
    auto restricted = batruc::makeInitialRecipe();
    restricted.voiceCount = 2;
    restricted.patternMode = batruc::PatternMode::diagonalLanes;
    restricted.voices[0].cyclesPerSection = 2.0;
    restricted.voices[1].cyclesPerSection = 3.0;
    restricted.voices[1].phaseCycles = 0.0816451;
    restricted.voices[0].eventRule = batruc::EventRule::centreCrossings;
    restricted.voices[1].eventRule = batruc::EventRule::centreCrossings;
    batruc::EventPlan restrictedPlan;
    if (batruc::buildEventPlan(restricted, restrictedPlan) != batruc::PlanError::none
        || restrictedPlan.events.size() != 1
        || restrictedPlan.events.front().voice != 0
        || std::abs(restrictedPlan.events.front().intensity - 0.4f) > 1.0e-6f) {
        std::cerr << "sparse geometric mask silenced the first voice\n";
        return 10;
    }
    batruc::ResonatorRenderer restrictedRenderer;
    if (!restrictedRenderer.configure(restricted, restrictedPlan, 48000.0))
        return 11;
    std::array<float, batruc::kMaxVoices> overrideCycles{};
    std::array<float, batruc::kMaxVoices> overridePhases{};
    std::array<int, batruc::kMaxVoices> overrideRules{};
    overrideRules.fill(-1);
    overrideCycles[0] = 2.0f;
    overrideCycles[1] = 3.0f;
    restrictedRenderer.setVoiceRhythmOverrides(overrideCycles.data(),
        overridePhases.data(), overrideRules.data(), true);
    restrictedRenderer.setActiveVoiceCount(2);
    std::array<float, 512> anchorLeft{}, anchorRight{};
    double anchorEnergy = 0.0;
    for (std::size_t block = 0; block < 750; ++block) {
        restrictedRenderer.process(anchorLeft.data(), anchorRight.data(),
                                   anchorLeft.size());
        for (const float sample : anchorLeft)
            anchorEnergy += static_cast<double>(sample) * sample;
    }
    if (anchorEnergy <= 1.0e-8) {
        std::cerr << "dynamic rhythm bypassed the audible anchor\n";
        return 12;
    }

    constexpr std::array<double, 8> baseCycles = {
        3.0, 8.0, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75};
    constexpr std::array<float, 8> expectedCycles = {
        3.0f, 7.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    constexpr std::array<float, 8> expectedCouplings = {
        0.0f, 0.31529078f, 0.22725685f, 0.31561735f,
        0.35834316f, 0.25274628f, 0.26565421f, 0.30497080f};
    constexpr std::array<float, 8> expectedRealizedPhases = {
        0.0f, 0.02093841f, 0.09552807f, 0.11793486f,
        0.18120375f, 0.23107325f, 0.34523529f, 0.36763129f};
    constexpr std::array<int, 8> expectedTransforms = {0, 0, 7, 0, 0, 7, 1, 0};
    std::uint32_t variationState = 2166136261u;
    for (const char character : std::array<char, 8>{
             '0', '1', '2', '3', '4', '5', '6', '7'})
        variationState = (variationState ^ static_cast<std::uint32_t>(character))
            * 16777619u;
    constexpr float variation = 0.35f;
    const int maxCycleDelta = static_cast<int>(std::round(3.0f * variation));
    for (std::size_t voice = 0; voice < expectedCycles.size(); ++voice) {
        const int cycleDelta = static_cast<int>(std::floor(
            batruc::compositionSeedRandom(variationState) * (2 * maxCycleDelta + 1)))
            - maxCycleDelta;
        const float cycles = std::clamp(static_cast<float>(baseCycles[voice])
            + static_cast<float>(cycleDelta), 1.0f, 12.0f);
        float coupling = 0.0f;
        float realizedPhase = 0.0f;
        if (voice > 0) {
            coupling = std::clamp(0.28f + static_cast<float>(
                (batruc::compositionSeedRandom(variationState) * 2.0 - 1.0)
                    * 0.25f * variation), 0.0f, 0.8f);
            const float jitter = static_cast<float>(
                (batruc::compositionSeedRandom(variationState) * 2.0 - 1.0)
                    * 0.125f * variation);
            const float stagger = static_cast<float>(voice - 1) / 16.0f;
            const float phase = stagger + jitter;
            realizedPhase = phase - std::floor(phase + 0.5f);
        }
        int transform = 0;
        if (batruc::compositionSeedRandom(variationState) < variation) {
            transform = static_cast<int>(std::floor(
                batruc::compositionSeedRandom(variationState) * 9.0));
            if (transform == 0)
                ++transform;
        }
        if (std::abs(cycles - expectedCycles[voice]) > 1.0e-6f
            || std::abs(coupling - expectedCouplings[voice]) > 1.0e-6f
            || std::abs(realizedPhase - expectedRealizedPhases[voice]) > 1.0e-6f
            || transform != expectedTransforms[voice]) {
            std::cerr << "seed variation mismatch on voice " << voice + 1 << '\n';
            return 4;
        }
    }

    auto recipe = batruc::makeInitialRecipe();
    recipe.voiceCount = 8;
    recipe.patternMode = batruc::PatternMode::unrestricted;
    constexpr std::array<double, 8> cycles = {
        3.0, 8.0, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75};
    constexpr std::array<double, 8> phases = {
        0.0, 0.125, 0.1995, 0.399, 0.5985, 0.798, 0.9975, 1.197};
    for (std::size_t voice = 2; voice < recipe.voiceCount; ++voice)
        recipe.voices[voice] = recipe.voices[1];
    for (std::size_t voice = 0; voice < recipe.voiceCount; ++voice) {
        recipe.voices[voice].cyclesPerSection = cycles[voice];
        recipe.voices[voice].phaseCycles = phases[voice];
        recipe.voices[voice].eventRule = batruc::EventRule::turningPoints;
    }
    batruc::EventPlan plan;
    if (batruc::buildEventPlan(recipe, plan) != batruc::PlanError::none)
        return 2;
    constexpr std::array<std::size_t, 5> expectedCounts = {6, 16, 1, 1, 2};
    constexpr std::array<std::array<double, 16>, 5> expectedPositions = {{
        {{1.0 / 12.0, 0.25, 5.0 / 12.0, 7.0 / 12.0, 0.75,
          11.0 / 12.0}},
        {{1.0 / 64.0, 5.0 / 64.0, 9.0 / 64.0, 13.0 / 64.0,
          17.0 / 64.0, 21.0 / 64.0, 25.0 / 64.0, 29.0 / 64.0,
          33.0 / 64.0, 37.0 / 64.0, 41.0 / 64.0, 45.0 / 64.0,
          49.0 / 64.0, 53.0 / 64.0, 57.0 / 64.0, 61.0 / 64.0}},
        {{0.101}}, {{0.468}}, {{0.1515, 0.6515}}
    }};
    for (std::size_t voice = 0; voice < expectedCounts.size(); ++voice) {
        std::size_t count = 0;
        std::array<double, 16> actualPositions{};
        std::size_t positionCount = 0;
        for (const auto& event : plan.events) {
            if (event.voice != voice)
                continue;
            const auto position = event.timeSeconds / recipe.sectionSeconds;
            if (positionCount >= actualPositions.size())
                return 3;
            actualPositions[positionCount++] = position;
            ++count;
        }
        if (count != expectedCounts[voice]
            || positionCount != expectedCounts[voice]) {
            std::cerr << "analytic event fixture mismatch on voice " << voice + 1 << '\n';
            return 3;
        }
        for (std::size_t event = 0; event < positionCount; ++event) {
            if (std::abs(actualPositions[event] - expectedPositions[voice][event])
                > 1.0e-9) {
                std::cerr << "analytic event position mismatch on voice "
                          << voice + 1 << ", event " << event << '\n';
                return 3;
            }
        }
    }
    return 0;
}
