// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/BassAudio.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>

using namespace batruc;

namespace {

constexpr std::size_t kFrames = 48000;
using Buffer = std::array<float, kFrames>;

Buffer render(const BassAudioParameters& parameters) {
    BassPerformance performance;
    performance.sectionSeconds = 1.0;
    performance.count = 1;
    performance.events[0] = {0.0, 0.24, 36, 0.8f, 0, 7};

    BassAudioRenderer renderer;
    assert(renderer.configure(performance, 48000.0));
    Buffer left{};
    Buffer right{};
    renderer.process(left.data(), right.data(), kFrames, 0.0, 1.0, 1,
                     true, parameters);
    return left;
}

double rms(const Buffer& buffer) {
    double sum = 0.0;
    for (const auto value : buffer)
        sum += static_cast<double>(value) * value;
    return std::sqrt(sum / static_cast<double>(buffer.size()));
}

double differenceRms(const Buffer& left, const Buffer& right) {
    double sum = 0.0;
    for (std::size_t index = 0; index < left.size(); ++index) {
        const double difference = static_cast<double>(left[index]) - right[index];
        sum += difference * difference;
    }
    return std::sqrt(sum / static_cast<double>(left.size()));
}

} // namespace

int main() {
    BassAudioParameters baselineParameters;
    const auto baseline = render(baselineParameters);
    assert(rms(baseline) > 1.0e-4);

    const auto expectAudibleChange = [&baseline](BassAudioParameters changed) {
        assert(differenceRms(baseline, render(changed)) > 1.0e-5);
    };

    auto changed = baselineParameters;
    changed.channelPan = 1.0f;
    if (rms(render(changed)) > 1.0e-6) {
        std::fputs("bass channel hard-right pan leaked into left output\n", stderr);
        return 1;
    }

    changed = baselineParameters;
    changed.attackSeconds = 0.18f;
    expectAudibleChange(changed);

    changed = baselineParameters;
    changed.decaySeconds = 0.8f;
    expectAudibleChange(changed);

    changed = baselineParameters;
    changed.sustain = 0.12f;
    expectAudibleChange(changed);

    changed = baselineParameters;
    changed.releaseSeconds = 0.8f;
    expectAudibleChange(changed);

    changed = baselineParameters;
    changed.cutoffHz = 110.0f;
    expectAudibleChange(changed);

    changed = baselineParameters;
    changed.filterResonance = 8.0f;
    expectAudibleChange(changed);

    changed = baselineParameters;
    changed.material = BassMaterial::pluck;
    expectAudibleChange(changed);

    for (const auto target : {BassLfoTarget::pitch, BassLfoTarget::filter,
                              BassLfoTarget::amplitude}) {
        changed = baselineParameters;
        changed.lfoEnabled = true;
        changed.lfoTarget = target;
        changed.lfoPeriodCycles = 1;
        changed.lfoDepthSemitones = 12;
        expectAudibleChange(changed);
    }

    // An authored/generated bass attack must not disappear in C1 or C5.
    for (const std::uint64_t formCycle : {0u, 4u}) {
        BassPerformance opening;
        opening.sectionSeconds = 1.0;
        opening.count = 1;
        opening.events[0] = {0.02, 0.20, 36, 0.8f, 0,
                             static_cast<std::uint8_t>(formCycle)};
        BassAudioRenderer openingRenderer;
        if (!openingRenderer.configure(opening, 48000.0))
            return 2;
        std::array<float, 4800> openingLeft{}, openingRight{};
        openingRenderer.process(openingLeft.data(), openingRight.data(),
            openingLeft.size(), 0.0, 1.0, formCycle, true, baselineParameters);
        if (std::none_of(openingLeft.begin(), openingLeft.end(),
            [](const float sample) { return std::abs(sample) > 1.0e-5f; })) {
            std::fputs("bass attack was silenced by the formal cycle\n", stderr);
            return 2;
        }
    }

    // A live score edit between beats must be heard before the next bar.
    BassAudioRenderer liveRenderer;
    BassPerformance emptyPerformance;
    emptyPerformance.sectionSeconds = 1.0;
    if (!liveRenderer.configure(emptyPerformance, 48000.0))
        return 3;
    std::array<float, 4800> liveLeft{}, liveRight{};
    liveRenderer.process(liveLeft.data(), liveRight.data(), liveLeft.size(),
        0.0, 1.0, 1, true, baselineParameters);
    BassPerformance livePerformance;
    livePerformance.sectionSeconds = 1.0;
    livePerformance.count = 1;
    livePerformance.events[0] = {0.15, 0.20, 36, 0.8f, 0, 1};
    if (!liveRenderer.configure(livePerformance, 48000.0, false))
        return 3;
    liveRenderer.process(liveLeft.data(), liveRight.data(), liveLeft.size(),
        0.1, 1.0, 1, true, baselineParameters);
    if (std::none_of(liveLeft.begin(), liveLeft.end(),
        [](const float sample) { return std::abs(sample) > 1.0e-5f; })) {
        std::fputs("live bass edit waited for the next bar\n", stderr);
        return 3;
    }

    // A prepared replacement must remain pending until the next bar. Its
    // first onset is placed after that boundary so the transition is audible.
    BassAudioRenderer scheduledRenderer;
    BassPerformance initialPerformance;
    initialPerformance.sectionSeconds = 4.0;
    assert(scheduledRenderer.configure(initialPerformance, 48000.0));
    std::array<float, 4800> transitionLeft{};
    std::array<float, 4800> transitionRight{};
    BassAudioParameters transitionParameters;
    scheduledRenderer.process(transitionLeft.data(), transitionRight.data(),
        transitionLeft.size(), 0.0, 4.0, 1, true, transitionParameters);

    BassPerformance nextPerformance;
    nextPerformance.sectionSeconds = 4.0;
    nextPerformance.count = 1;
    nextPerformance.events[0] = {1.5, 0.3, 36, 0.8f, 0, 1};
    assert(scheduledRenderer.configure(nextPerformance, 48000.0, true));
    std::array<float, 38400> beforeBoundaryLeft{};
    std::array<float, 38400> beforeBoundaryRight{};
    scheduledRenderer.process(beforeBoundaryLeft.data(), beforeBoundaryRight.data(),
        beforeBoundaryLeft.size(), 0.025, 4.0, 1, true, transitionParameters);
    assert(std::all_of(beforeBoundaryLeft.begin(), beforeBoundaryLeft.end(),
        [](const float value) { return std::abs(value) < 1.0e-12f; }));

    scheduledRenderer.process(transitionLeft.data(), transitionRight.data(),
        transitionLeft.size(), 0.225, 4.0, 1, true, transitionParameters);
    std::array<float, 28800> afterBoundaryLeft{};
    std::array<float, 28800> afterBoundaryRight{};
    scheduledRenderer.process(afterBoundaryLeft.data(), afterBoundaryRight.data(),
        afterBoundaryLeft.size(), 0.25, 4.0, 1, true, transitionParameters);
    assert(std::any_of(afterBoundaryLeft.begin(), afterBoundaryLeft.end(),
        [](const float value) { return std::abs(value) > 1.0e-5f; }));
}
