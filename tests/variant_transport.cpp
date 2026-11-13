// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/CompositionCore.hpp"
#include "core/PassageDevelopment.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <vector>

namespace {

template <std::size_t N>
bool silent(const std::array<float, N>& samples) {
    for (const float value : samples)
        if (std::abs(value) > 1.0e-7f)
            return false;
    return true;
}

template <std::size_t N>
bool audible(const std::array<float, N>& samples) {
    return !silent(samples);
}

} // namespace

int main() {
    constexpr std::uint32_t developmentSeed = 0x5EED2027u;
    if (batruc::passageEventWeight(developmentSeed, 0, 0, 0.375) != 1.0f
        || batruc::passageEventWeight(developmentSeed, 1, 0, 0.125)
            == batruc::passageEventWeight(developmentSeed, 1, 1, 0.125)
        || batruc::passageEventWeight(developmentSeed, 2, 0, 0.0)
            == batruc::passageEventWeight(developmentSeed, 2, 0, 0.375)
        || batruc::passageEventWeight(developmentSeed, 3, 0, 0.0)
            == batruc::passageEventWeight(developmentSeed, 3, 1, 0.0)) {
        std::fputs("passage development lost its musical roles\n", stderr);
        return 1;
    }
    for (std::uint32_t seed = 0; seed < 64; ++seed) {
        bool hasActiveExpansion = false;
        for (std::uint64_t passage = 0; passage < 8; ++passage)
            for (const double progress : {0.0, 0.125, 0.5, 0.875})
                for (std::size_t voice = 0; voice < batruc::kMaxVoices; ++voice)
                    if (batruc::passageEventWeight(seed, passage, voice, progress)
                        <= 0.0f) {
                        std::fputs("a long-form passage became silent\n", stderr);
                        return 1;
                    }
        for (std::uint64_t passage = 4; passage < 7; ++passage)
            hasActiveExpansion = hasActiveExpansion
                || batruc::passageRoleFor(seed, passage)
                    == batruc::PassageRole::Expansion;
        if (!hasActiveExpansion
            || batruc::passageRoleFor(seed, 7) != batruc::PassageRole::ReturnA) {
            std::fputs("long-form route lost its active expansion or return\n",
                       stderr);
            return 1;
        }
    }
    auto recipe = batruc::makeInitialRecipe();
    recipe.sectionSeconds = 0.02; // 960 samples at 48 kHz.

    batruc::EventPlan stationaryPlan;
    batruc::EventPlan movedPlan;
    if (batruc::buildEventPlan(recipe, stationaryPlan)
        != batruc::PlanError::none) {
        std::fputs("stationary voice plan failed\n", stderr);
        return 1;
    }
    auto movedRecipe = recipe;
    movedRecipe.voices[1].phaseCycles = 0.125;
    if (batruc::buildEventPlan(movedRecipe, movedPlan)
        != batruc::PlanError::none) {
        std::fputs("moved voice plan failed\n", stderr);
        return 1;
    }

    // Check the rendered event schedule, not only the positive scalar weights:
    // every seven-section long-form passage must retain actual output energy.
    batruc::ResonatorRenderer formRenderer;
    if (!formRenderer.configure(recipe, stationaryPlan, 48'000.0)) {
        std::fputs("form renderer preparation failed\n", stderr);
        return 1;
    }
    std::array<float, 960> formLeft{};
    std::array<float, 960> formRight{};
    for (std::uint32_t seed = 0; seed < 64; ++seed) {
        formRenderer.reset();
        formRenderer.setDevelopmentSeed(seed, true);
        for (std::uint64_t passage = 0; passage < 8; ++passage) {
            double passageEnergy = 0.0;
            for (unsigned section = 0; section < batruc::kFormalCycleCount; ++section) {
                formRenderer.process(formLeft.data(), formRight.data(), formLeft.size());
                for (const float sample : formLeft)
                    passageEnergy += static_cast<double>(sample) * sample;
                for (const float sample : formRight)
                    passageEnergy += static_cast<double>(sample) * sample;
            }
            if (!(passageEnergy > 1.0e-10)) {
                std::fputs("rendered long-form passage contains no output energy\n",
                           stderr);
                return 1;
            }
        }
    }

    std::vector<double> firstOriginal, firstMoved, secondOriginal, secondMoved;
    for (const auto& event : stationaryPlan.events)
        (event.voice == 0 ? firstOriginal : secondOriginal).push_back(event.timeSeconds);
    for (const auto& event : movedPlan.events)
        (event.voice == 0 ? firstMoved : secondMoved).push_back(event.timeSeconds);
    if (firstOriginal != firstMoved || secondOriginal == secondMoved) {
        std::fputs("moving voice 2 changed the wrong rhythm\n", stderr);
        return 1;
    }

    std::array<batruc::EventPlan, 2> plans;
    batruc::MusicalEvent hit;
    hit.timeSeconds = 0.01; // Sample 480 in each section.
    hit.voice = 0;
    plans[1].events.push_back(hit);

    batruc::ResonatorRenderer renderer;
    if (!renderer.configureVariants(recipe, plans.data(), plans.size(), 48'000.0)) {
        std::fputs("variant preparation failed\n", stderr);
        return 1;
    }

    std::array<float, 240> left{};
    std::array<float, 240> right{};
    renderer.processVariant(left.data(), right.data(), left.size(), 0);
    if (renderer.renderedSamples() != 240 || !silent(left)) {
        std::fputs("initial transport or silence incorrect\n", stderr);
        return 1;
    }

    renderer.processVariant(left.data(), right.data(), left.size(), 1);
    if (renderer.renderedSamples() != 480 || !silent(left)) {
        std::fputs("switch rewound transport or fired early\n", stderr);
        return 1;
    }

    std::array<float, 64> shortLeft{};
    std::array<float, 64> shortRight{};
    renderer.processVariant(shortLeft.data(), shortRight.data(), shortLeft.size(), 1);
    if (renderer.renderedSamples() != 544 || !audible(shortLeft)) {
        std::fputs("event at switch boundary was lost\n", stderr);
        return 1;
    }
    if (std::abs(shortLeft.front()) > 0.01f
        || std::abs(shortRight.front()) > 0.01f) {
        std::fputs("voice excitation starts with an audible discontinuity\n", stderr);
        return 1;
    }

    renderer.processVariant(shortLeft.data(), shortRight.data(), shortLeft.size(), 0);
    if (renderer.renderedSamples() != 608 || !audible(shortLeft)) {
        std::fputs("switch reset the clock or existing resonator tail\n", stderr);
        return 1;
    }

    renderer.reset();
    renderer.processVariant(left.data(), right.data(), left.size(), 0);
    renderer.processVariant(left.data(), right.data(), left.size(), 0);
    renderer.processVariant(left.data(), right.data(), left.size(), 0);
    renderer.processVariant(shortLeft.data(), shortRight.data(), shortLeft.size(), 1);
    if (renderer.renderedSamples() != 784 || !silent(shortLeft)) {
        std::fputs("switch replayed an event already passed\n", stderr);
        return 1;
    }

    renderer.reset();
    std::array<float, 450> beforeBarLeft{};
    std::array<float, 450> beforeBarRight{};
    renderer.processVariant(beforeBarLeft.data(), beforeBarRight.data(),
                            beforeBarLeft.size(), 0);
    renderer.processVariantAtNextBar(shortLeft.data(), shortRight.data(),
                                     shortLeft.size(), 1);
    if (renderer.renderedSamples() != 514 || renderer.activeVariant() != 1
        || renderer.completedBars() != 2) {
        std::fputs("quantized switch missed the bar inside an audio block\n", stderr);
        return 1;
    }
    for (std::size_t i = 0; i < 30; ++i)
        if (std::abs(shortLeft[i]) > 1.0e-7f) {
            std::fputs("quantized event sounded before the bar\n", stderr);
            return 1;
        }
    bool soundAfterBar = false;
    for (std::size_t i = 30; i < shortLeft.size(); ++i)
        soundAfterBar = soundAfterBar || std::abs(shortLeft[i]) > 1.0e-7f;
    if (!soundAfterBar) {
        std::fputs("quantized event at the bar was lost\n", stderr);
        return 1;
    }

    renderer.reset();
    if (renderer.completedBars() != 0) {
        std::fputs("bar counter was not reset\n", stderr);
        return 1;
    }
    renderer.processVariant(left.data(), right.data(), left.size(), 0);
    renderer.processVariant(left.data(), right.data(), left.size(), 0);
    renderer.processVariantAtNextBar(shortLeft.data(), shortRight.data(),
                                     shortLeft.size(), 1);
    if (renderer.activeVariant() != 1 || renderer.renderedSamples() != 544) {
        std::fputs("request on a bar boundary was not applied\n", stderr);
        return 1;
    }

    renderer.reset();
    renderer.processVariant(left.data(), right.data(), left.size(), 0);
    renderer.processVariantAtNextBar(shortLeft.data(), shortRight.data(),
                                     shortLeft.size(), 1);
    if (renderer.activeVariant() != 1) {
        std::fputs("request at the first bar boundary was not applied\n", stderr);
        return 1;
    }
    renderer.reset();
    std::array<float, 300> midBarLeft{};
    std::array<float, 300> midBarRight{};
    renderer.processVariant(midBarLeft.data(), midBarRight.data(),
                            midBarLeft.size(), 0);
    renderer.processVariantAtNextBar(shortLeft.data(), shortRight.data(),
                                     shortLeft.size(), 1);
    if (renderer.activeVariant() != 0 || renderer.renderedSamples() != 364
        || !silent(shortLeft)) {
        std::fputs("variant changed before the next bar\n", stderr);
        return 1;
    }

    std::vector<batruc::EventPlan> phasePlans(606);
    for (auto& plan : phasePlans)
        plan.events.push_back(hit);
    if (!renderer.configureVariants(recipe, phasePlans.data(),
                                    phasePlans.size(), 48'000.0)) {
        std::fputs("bounded phase bank preparation failed\n", stderr);
        return 1;
    }
    renderer.processVariantAtNextBar(shortLeft.data(), shortRight.data(),
                                     shortLeft.size(), phasePlans.size() - 1);
    if (renderer.activeVariant() != phasePlans.size() - 1) {
        std::fputs("last phase variant was not selectable\n", stderr);
        return 1;
    }

    if (!renderer.configure(recipe, plans[1], 48'000.0)) {
        std::fputs("single-plan preparation failed\n", stderr);
        return 1;
    }
    std::array<float, batruc::kMaxVoices> levels{};
    std::array<float, 600> mutedLeft{};
    std::array<float, 600> mutedRight{};
    renderer.processVariantAtNextBar(mutedLeft.data(), mutedRight.data(),
                                     mutedLeft.size(), 0, levels.data());
    if (!silent(mutedLeft)) {
        std::fputs("muted event produced sound\n", stderr);
        return 1;
    }
    levels.fill(1.0f);
    std::array<float, 120> unmutedLeft{};
    std::array<float, 120> unmutedRight{};
    renderer.processVariantAtNextBar(unmutedLeft.data(), unmutedRight.data(),
                                     unmutedLeft.size(), 0, levels.data());
    if (!silent(unmutedLeft)) {
        std::fputs("unmute replayed a suppressed event\n", stderr);
        return 1;
    }
    renderer.reset();
    renderer.processVariantAtNextBar(mutedLeft.data(), mutedRight.data(),
                                     mutedLeft.size(), 0, levels.data());
    if (!audible(mutedLeft)) {
        std::fputs("unmuted event was not audible\n", stderr);
        return 1;
    }

    auto developmentRecipe = recipe;
    developmentRecipe.voices[0].resonator.decaySeconds = 0.005;
    developmentRecipe.voices[0].resonator.strikeNoise = 0.0f;
    batruc::ResonatorRenderer developing;
    if (!developing.configure(developmentRecipe, plans[1], 48'000.0)) {
        std::fputs("development renderer preparation failed\n", stderr);
        return 1;
    }
    developing.setDevelopmentSeed(developmentSeed, true);
    std::array<float, 960> passageLeft{};
    std::array<float, 960> passageRight{};
    double firstEnergy = 0.0, responseEnergy = 0.0;
    for (int section = 0; section < 8; ++section) {
        developing.process(passageLeft.data(), passageRight.data(),
                           passageLeft.size());
        double energy = 0.0;
        for (std::size_t sample = 480; sample < 720; ++sample)
            energy += passageLeft[sample] * passageLeft[sample];
        if (section == 0) firstEnergy = energy;
        if (section == 7) responseEnergy = energy;
    }
    if (!(firstEnergy > 1.0e-6 && responseEnergy > 0.0
          && responseEnergy < firstEnergy * 0.5)) {
        std::fputs("seeded passage did not change rendered articulation\n", stderr);
        return 1;
    }

    return 0;
}
