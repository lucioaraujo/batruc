// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/CompositionCore.hpp"

#include <array>
#include <cmath>
#include <cstdio>

namespace {

bool rendersPan(const float pan, bool expectLeft) {
    auto recipe = batruc::makeInitialRecipe();
    recipe.voiceCount = 2;
    recipe.sectionSeconds = 1.0;
    batruc::EventPlan plan;
    const auto appendEvent = [&plan](const double timeSeconds,
                                     const std::uint8_t voice) {
        batruc::MusicalEvent event;
        event.timeSeconds = timeSeconds;
        event.phaseCycles = timeSeconds;
        event.voice = voice;
        event.rule = batruc::EventRule::centreCrossings;
        event.intensity = 1.0f;
        plan.events.push_back(event);
    };
    appendEvent(0.0, 0);
    appendEvent(0.0, 1);
    appendEvent(0.04, 0);
    appendEvent(0.04, 1);
    batruc::ResonatorRenderer renderer;
    if (!renderer.configure(recipe, plan, 48'000.0))
        return false;
    std::array<float, batruc::kMaxVoices> pans{};
    pans[0] = pan;
    pans[1] = pan;
    renderer.setChannelPans(pans.data());
    std::array<float, 4096> left{};
    std::array<float, 4096> right{};
    renderer.process(left.data(), right.data(), left.size());
    // Pan changes use a 5 ms exponential ramp. Measure the tail after that
    // transition, where hard pan should leave negligible energy on the other
    // output without rejecting the click-free movement into position.
    constexpr std::size_t steadyStart = 3584;
    double leftEnergy = 0.0;
    double rightEnergy = 0.0;
    for (std::size_t frame = steadyStart; frame < left.size(); ++frame) {
        leftEnergy += static_cast<double>(left[frame]) * left[frame];
        rightEnergy += static_cast<double>(right[frame]) * right[frame];
    }
    constexpr double maximumLeakageRatio = 1.0e-5;
    if (expectLeft)
        return leftEnergy > 1.0e-8
            && rightEnergy < leftEnergy * maximumLeakageRatio;
    return rightEnergy > 1.0e-8
        && leftEnergy < rightEnergy * maximumLeakageRatio;
}

bool renderPanTrajectory(const float fieldX, const float variationDepth,
                        const batruc::PanTrajectory* customTrajectory,
                        double& leftEnergy, double& rightEnergy) {
    auto recipe = batruc::makeInitialRecipe();
    recipe.voiceCount = 1;
    recipe.sectionSeconds = 0.5;
    batruc::EventPlan plan;
    batruc::MusicalEvent event;
    event.timeSeconds = 0.0;
    event.phaseCycles = 0.0;
    event.voice = 0;
    event.rule = batruc::EventRule::centreCrossings;
    event.intensity = 1.0f;
    event.fieldX = fieldX;
    plan.events.push_back(event);

    batruc::ResonatorRenderer renderer;
    if (!renderer.configure(recipe, plan, 48'000.0))
        return false;
    renderer.setActiveVoiceCount(1);
    renderer.setFormalVoiceReductionEnabled(false);
    renderer.setChannelSound(0, batruc::SoundCharacter::kick);
    renderer.setPerformanceExpression(batruc::DynamicCurve::steady,
                                      batruc::MusicalIntention::build);
    std::array<float, batruc::kMaxVoices> pans{};
    std::array<float, batruc::kMaxVoices> movement{};
    std::array<float, batruc::kMaxVoices> variation{};
    movement[0] = 1.0f;
    variation[0] = variationDepth;
    renderer.setChannelPans(pans.data());
    renderer.setChannelPanMotion(movement.data());
    renderer.setChannelVariationDepths(variation.data());
    if (customTrajectory != nullptr) {
        std::array<batruc::PanTrajectory, batruc::kMaxVoices> trajectories{};
        trajectories[0] = *customTrajectory;
        renderer.setChannelPanTrajectories(trajectories.data());
    }

    std::array<float, 24'000> left{};
    std::array<float, 24'000> right{};
    renderer.process(left.data(), right.data(), left.size());
    leftEnergy = 0.0;
    rightEnergy = 0.0;
    for (std::size_t frame = 4096; frame < left.size(); ++frame) {
        leftEnergy += static_cast<double>(left[frame]) * left[frame];
        rightEnergy += static_cast<double>(right[frame]) * right[frame];
    }
    return leftEnergy + rightEnergy > 1.0e-8;
}

} // namespace

int main() {
    float trimDelta = 99.0f;
    if (batruc::suggestVoiceTrimDeltaDb(0.0f, 0.2f, 0.0f, trimDelta)
        || trimDelta != 99.0f
        || !batruc::suggestVoiceTrimDeltaDb(0.1f, 0.2f, 0.0f, trimDelta)
        || std::abs(trimDelta - 6.0206f) > 0.001f
        || !batruc::suggestVoiceTrimDeltaDb(0.1f, 0.2f, 22.0f, trimDelta)
        || std::abs(trimDelta - 2.0f) > 1.0e-6f
        || !batruc::suggestVoiceTrimDeltaDb(0.2f, 0.1f, -10.0f, trimDelta)
        || std::abs(trimDelta + 2.0f) > 1.0e-6f
        || batruc::suggestVoiceTrimDeltaDb(0.0f, 0.2f, 0.0f, trimDelta)
        || batruc::suggestVoiceTrimDeltaDb(0.1f, 0.2f, 24.1f, trimDelta)) {
        std::fputs("mixer RMS balance suggestion is invalid\n", stderr);
        return 7;
    }

    std::array<float, batruc::kMaxVoices> levels{};
    levels.fill(0.8f);
    std::array<bool, batruc::kMaxVoices> muted{};
    std::array<bool, batruc::kMaxVoices> solo{};
    auto route = batruc::resolveMixerLevels(levels, muted, solo, 3,
        0.6f, true, false, false);
    if (route.voices[0] != 0.8f || route.voices[1] != 0.8f
        || route.voices[2] != 0.8f || route.voices[3] != 0.0f
        || route.bass != 0.6f)
        return 1;

    muted[1] = true;
    route = batruc::resolveMixerLevels(levels, muted, solo, 3,
        0.6f, true, false, false);
    if (route.voices[1] != 0.0f || route.voices[0] != 0.8f)
        return 2;

    muted[1] = false;
    solo[2] = true;
    route = batruc::resolveMixerLevels(levels, muted, solo, 3,
        0.6f, true, false, false);
    if (route.voices[0] != 0.0f || route.voices[1] != 0.0f
        || route.voices[2] != 0.8f || route.bass != 0.0f)
        return 3;

    solo[2] = false;
    route = batruc::resolveMixerLevels(levels, muted, solo, 3,
        0.6f, true, false, true);
    if (route.voices[0] != 0.0f || route.voices[1] != 0.0f
        || route.voices[2] != 0.0f || route.bass != 0.6f)
        return 4;

    route = batruc::resolveMixerLevels(levels, muted, solo, 3,
        0.6f, true, true, true);
    if (route.bass != 0.0f || route.voices[0] != 0.0f)
        return 5;

    if (!rendersPan(-1.0f, true) || !rendersPan(1.0f, false)) {
        std::fputs("hard pan leaked to the opposite output or silenced the signal\n",
                   stderr);
        return 6;
    }
    double leftwardLeft = 0.0;
    double leftwardRight = 0.0;
    double rightwardLeft = 0.0;
    double rightwardRight = 0.0;
    double stationaryLeft = 0.0;
    double stationaryRight = 0.0;
    double neutralLeft = 0.0;
    double neutralRight = 0.0;
    if (!renderPanTrajectory(-1.0f, 1.0f, nullptr,
                             leftwardLeft, leftwardRight)
        || !renderPanTrajectory(1.0f, 1.0f, nullptr,
                                rightwardLeft, rightwardRight)
        || !renderPanTrajectory(0.0f, 1.0f, nullptr,
                                stationaryLeft, stationaryRight)
        || !renderPanTrajectory(-1.0f, 0.0f, nullptr,
                                neutralLeft, neutralRight)
        || leftwardLeft <= leftwardRight * 2.0
        || rightwardRight <= rightwardLeft * 2.0
        || std::abs(stationaryLeft - stationaryRight)
            > (stationaryLeft + stationaryRight) * 0.01
        || std::abs(neutralLeft - neutralRight)
            > (neutralLeft + neutralRight) * 0.01) {
        std::fprintf(stderr,
            "field-driven pan trajectory failed: left %.9g/%.9g, right %.9g/%.9g, center %.9g/%.9g, VAR=0 %.9g/%.9g\n",
            leftwardLeft, leftwardRight, rightwardLeft, rightwardRight,
            stationaryLeft, stationaryRight, neutralLeft, neutralRight);
        return 8;
    }

    batruc::PanTrajectory drawnTrajectory;
    drawnTrajectory.enabled = true;
    drawnTrajectory.points.fill(-1.0f);
    double drawnLeft = 0.0;
    double drawnRight = 0.0;
    if (std::abs(batruc::samplePanTrajectory(drawnTrajectory, 0.0f) + 1.0f)
            > 1.0e-6f
        || std::abs(batruc::samplePanTrajectory(drawnTrajectory, 0.37f) + 1.0f)
            > 1.0e-6f
        || !renderPanTrajectory(1.0f, 1.0f, &drawnTrajectory,
                                drawnLeft, drawnRight)
        || drawnLeft <= drawnRight * 2.0) {
        std::fprintf(stderr,
            "drawn pan path did not override field-X routing: %.9g/%.9g\n",
            drawnLeft, drawnRight);
        return 9;
    }
    return 0;
}
