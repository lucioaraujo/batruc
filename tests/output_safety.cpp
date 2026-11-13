// SPDX-License-Identifier: AGPL-3.0-or-later
#include "core/OutputSafety.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <limits>

int main() {
    batruc::TruePeakEstimate4x estimator;
    estimator.prepare();
    float sampledPeak = 0.0f, reconstructedPeak = 0.0f;
    for (int i = 0; i < 512; ++i) {
        const float sample = 0.8f * std::sin(2.0 * 3.141592653589793 * 0.125 * i + 0.7853981634);
        sampledPeak = std::max(sampledPeak, std::abs(sample));
        reconstructedPeak = std::max(reconstructedPeak, estimator.process(sample));
    }
    if (!(reconstructedPeak > sampledPeak * 1.002f)) return 3;

    for (int phaseIndex = 0; phaseIndex < 8; ++phaseIndex) {
        batruc::StereoLookaheadGuard highFrequencyGuard;
        highFrequencyGuard.prepare(48000.0);
        float outputPeak = 0.0f;
        const double phase = static_cast<double>(phaseIndex) * 0.3926990817;
        for (int i = 0; i < 4096; ++i) {
            float left = 1.25f * std::sin(2.0 * 3.141592653589793 * 0.43 * i + phase);
            float right = 1.25f * std::sin(2.0 * 3.141592653589793 * 0.43 * i + phase + 0.18);
            highFrequencyGuard.process(left, right);
            outputPeak = std::max(outputPeak, std::max(std::abs(left), std::abs(right)));
        }
        if (outputPeak > 0.891251f || highFrequencyGuard.outputTruePeak() > 0.94f) {
            std::fprintf(stderr, "intersample ceiling failed phase=%d peak=%f tp=%f\n",
                         phaseIndex, outputPeak, highFrequencyGuard.outputTruePeak());
            return 4;
        }
    }
    for (const double rate : {44100.0, 48000.0, 96000.0}) {
        batruc::StereoLookaheadGuard rateGuard;
        rateGuard.prepare(rate);
        const auto inputIndex = static_cast<std::size_t>(rate * 0.005);
        const auto expectedDelay = static_cast<std::size_t>(std::lround(rate * 0.003));
        std::size_t firstOutput = 8192;
        for (std::size_t i = 0; i < 8192; ++i) {
            float left = i == inputIndex ? 0.5f : 0.0f;
            float right = 0.0f;
            rateGuard.process(left, right);
            if (std::abs(left) > 1.0e-6f && firstOutput == 8192) firstOutput = i;
        }
        if (firstOutput != inputIndex + expectedDelay) return 5;
    }

    // A stopped transport feeds one delay-length of silence to release the
    // last active samples, then the audio buffer can remain empty.
    batruc::StereoLookaheadGuard stoppedGuard;
    stoppedGuard.prepare(48000.0);
    for (std::size_t i = 0; i < stoppedGuard.latencySamples(); ++i) {
        float left = i + 1 == stoppedGuard.latencySamples() ? 0.5f : 0.0f;
        float right = 0.0f;
        stoppedGuard.process(left, right);
        if (std::abs(left) > 1.0e-6f) return 6;
    }
    std::size_t tailEvents = 0;
    for (std::size_t i = 0; i < stoppedGuard.latencySamples(); ++i) {
        float left = 0.0f, right = 0.0f;
        stoppedGuard.process(left, right);
        if (std::abs(left) > 1.0e-6f) {
            ++tailEvents;
            if (i + 1 != stoppedGuard.latencySamples()
                || std::abs(left - 0.5f) > 1.0e-3f) return 7;
        }
    }
    if (tailEvents != 1) return 8;

    batruc::StereoLookaheadGuard guard;
    guard.prepare(48000.0);
    std::array<float, 2048> left{}, right{};
    for (std::size_t i = 0; i < left.size(); ++i) {
        const float input = i == 200 ? 4.0f : (i > 200 && i < 700 ? 1.7f : 0.0f);
        left[i] = input;
        right[i] = -input * 0.75f;
        guard.process(left[i], right[i]);
    }
    float peak = 0.0f;
    std::size_t firstNonZero = left.size();
    bool stereoLinkHeld = false;
    for (std::size_t i = 0; i < left.size(); ++i) {
        peak = std::max({peak, std::abs(left[i]), std::abs(right[i])});
        if (std::abs(left[i]) > 1.0e-6f && firstNonZero == left.size())
            firstNonZero = i;
        if (left[i] > 0.8f && right[i] < -0.6f)
            stereoLinkHeld = true;
    }
    if (peak > 0.891251f || guard.reductionDb() < 0.1f || !stereoLinkHeld
        || firstNonZero != 344 || guard.outputTruePeak() < 0.1f
        || guard.outputTruePeak() > 0.92f || guard.inputTruePeak() < 0.1f) {
        std::fprintf(stderr, "output guard ceiling/link/latency failed: peak=%f tp=%f red=%f delay=%zu\n",
                     peak, guard.outputTruePeak(), guard.reductionDb(), firstNonZero);
        return 1;
    }
    float nan = std::numeric_limits<float>::quiet_NaN(), zero = 0.0f;
    guard.process(nan, zero);
    if (!std::isfinite(nan) || !std::isfinite(zero)) return 2;
    return 0;
}
