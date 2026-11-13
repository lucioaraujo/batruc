// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <vector>

namespace batruc {

class TruePeakEstimate4x {
public:
    void prepare() noexcept {
        constexpr double pi = 3.14159265358979323846;
        constexpr double centre = 31.5;
        std::array<double, 64> prototype{};
        for (std::size_t tap = 0; tap < prototype.size(); ++tap) {
            const double position = static_cast<double>(tap) - centre;
            const double phase = 2.0 * pi * static_cast<double>(tap) / 63.0;
            const double window = 0.35875 - 0.48829 * std::cos(phase)
                + 0.14128 * std::cos(2.0 * phase) - 0.01168 * std::cos(3.0 * phase);
            const double x = position * 0.25;
            const double sinc = std::abs(x) < 1.0e-12 ? 1.0 : std::sin(pi * x) / (pi * x);
            prototype[tap] = sinc * window;
        }
        for (std::size_t p = 0; p < 4; ++p) {
            double sum = 0.0;
            for (std::size_t t = 0; t < 16; ++t) sum += prototype[p + t * 4];
            for (std::size_t t = 0; t < 16; ++t)
                coeffs_[p][t] = static_cast<float>(prototype[p + t * 4] / sum);
        }
        reset();
    }
    void reset() noexcept { history_.fill(0.0f); write_ = 0; }
    float process(float x) noexcept {
        history_[write_] = std::isfinite(x) ? x : 0.0f;
        write_ = (write_ + 1) % history_.size();
        float peak = 0.0f;
        for (const auto& phase : coeffs_) {
            float value = 0.0f;
            std::size_t read = write_;
            for (std::size_t tap = 0; tap < 16; ++tap) {
                read = read == 0 ? history_.size() - 1 : read - 1;
                value += phase[tap] * history_[read];
            }
            peak = std::max(peak, std::abs(value));
        }
        return peak;
    }
private:
    std::array<std::array<float, 16>, 4> coeffs_{};
    std::array<float, 16> history_{};
    std::size_t write_ = 0;
};

// First local safety stage, informed by the RASGO output-stage design.
// This is not a standards-compliant true-peak limiter.
class StereoLookaheadGuard {
public:
    void prepare(const double sampleRate) {
        const auto delay = std::max(1, static_cast<int>(std::lround(sampleRate * 0.003)));
        left_.assign(static_cast<std::size_t>(delay), 0.0f);
        right_.assign(static_cast<std::size_t>(delay), 0.0f);
        const float sr = static_cast<float>(sampleRate);
        release_ = std::exp(-1.0f / std::max(1.0f, sr * 0.12f));
        gainAttack_ = 1.0f - std::exp(-1.0f / std::max(1.0f, sr * 0.0005f));
        truePeakLeft_.prepare(); truePeakRight_.prepare();
        truePeakOutLeft_.prepare(); truePeakOutRight_.prepare();
        reset();
    }
    void reset() noexcept {
        std::fill(left_.begin(), left_.end(), 0.0f);
        std::fill(right_.begin(), right_.end(), 0.0f);
        index_ = holdSamples_ = 0;
        envelope_ = 0.0f; gain_ = 1.0f; minimumGain_ = 1.0f;
        inputTruePeak_ = outputTruePeak_ = 0.0f;
        truePeakLeft_.reset(); truePeakRight_.reset();
        truePeakOutLeft_.reset(); truePeakOutRight_.reset();
    }
    void process(float& l, float& r) noexcept {
        const float inL = std::isfinite(l) ? l : 0.0f;
        const float inR = std::isfinite(r) ? r : 0.0f;
        const float tpL = truePeakLeft_.process(inL);
        const float tpR = truePeakRight_.process(inR);
        const float peak = std::max({std::abs(inL), std::abs(inR), tpL, tpR});
        inputTruePeak_ = std::max(inputTruePeak_, std::max(tpL, tpR));
        envelope_ = std::max(peak, envelope_ * release_);
        constexpr float ceiling = 0.89125094f;
        constexpr float detectorCeiling = 0.84139514f; // −1.5 dBFS, margin for reconstruction
        const float target = envelope_ > detectorCeiling ? detectorCeiling / envelope_ : 1.0f;
        if (target < gain_) {
            gain_ += (target - gain_) * gainAttack_;
            holdSamples_ = left_.size();
        } else if (holdSamples_ > 0) {
            --holdSamples_;
        } else {
            gain_ += (1.0f - gain_) * (1.0f - release_);
        }
        const float delayedL = left_[index_], delayedR = right_[index_];
        left_[index_] = inL; right_[index_] = inR;
        index_ = (index_ + 1) % left_.size();
        l = std::clamp(delayedL * gain_, -ceiling, ceiling);
        r = std::clamp(delayedR * gain_, -ceiling, ceiling);
        outputTruePeak_ = std::max(outputTruePeak_, std::max(
            truePeakOutLeft_.process(l), truePeakOutRight_.process(r)));
        minimumGain_ = std::min(minimumGain_, gain_);
    }
    float reductionDb() const noexcept {
        return -20.0f * std::log10(std::max(minimumGain_, 1.0e-6f));
    }
    std::size_t latencySamples() const noexcept { return left_.size(); }
    float inputTruePeak() const noexcept { return inputTruePeak_; }
    float outputTruePeak() const noexcept { return outputTruePeak_; }
private:
    std::vector<float> left_, right_;
    std::size_t index_ = 0, holdSamples_ = 0;
    float envelope_ = 0.0f, gain_ = 1.0f, release_ = 0.999f;
    float gainAttack_ = 0.04f, minimumGain_ = 1.0f;
    float inputTruePeak_ = 0.0f, outputTruePeak_ = 0.0f;
    TruePeakEstimate4x truePeakLeft_, truePeakRight_, truePeakOutLeft_, truePeakOutRight_;
};

} // namespace batruc
