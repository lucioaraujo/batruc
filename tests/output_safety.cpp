// SPDX-License-Identifier: AGPL-3.0-or-later
#include "core/Dither.hpp"
#include "core/OutputSafety.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <limits>
#include <string>
#include <vector>

namespace {
// EBU Tech 3341 V4 (2023), table 1, signals 15-23, synthesised from their
// description (16/11/2026; same construction as the RASGO reference,
// REFERENCIAS/SAIDA_TECNICA/tests/EbuTruePeakTests.cpp). Expected +0.2/-0.4 dBTP.
constexpr double kPi = 3.14159265358979323846;
double estimatorDb(const std::vector<double>& x) {
    batruc::TruePeakEstimate4x e;
    e.prepare();
    float peak = 0.0f;
    for (const double v : x) peak = std::max(peak, e.process(static_cast<float>(v)));
    return 20.0 * std::log10(std::max(peak, 1.0e-9f));
}
std::vector<double> ebuTone(double f, double a, double degrees) {
    const std::size_t n = 48000, fade = 480;
    std::vector<double> x(n);
    for (std::size_t i = 0; i < n; ++i) {
        const double g = std::min({1.0, double(i) / fade, double(n - 1 - i) / fade});
        x[i] = g * a * std::sin(2.0 * kPi * f * double(i) / 48000.0 + degrees * kPi / 180.0);
    }
    return x;
}
std::vector<double> ebuSinglePeriod(std::size_t offset) {
    const double high = 4.0 * 48000.0;
    const auto n = static_cast<std::size_t>(high * 0.5);
    std::vector<double> x(n);
    const double slow = high / 8000.0, fast = high / 12000.0;
    const auto start = static_cast<std::size_t>(slow * 2000.0);
    const auto fade = static_cast<std::size_t>(high * 0.005);
    for (std::size_t i = 0; i < n; ++i) {
        double v;
        if (i < start) v = 0.5 * std::sin(2.0 * kPi * double(i) / slow);
        else if (double(i - start) < fast) v = std::sin(2.0 * kPi * double(i - start) / fast);
        else v = 0.5 * std::sin(2.0 * kPi * (double(i - start) - fast) / slow);
        x[i] = std::min({1.0, double(i) / fade, double(n - 1 - i) / fade}) * v;
    }
    constexpr int taps = 255;
    const double cutoff = 0.45 * 48000.0 / high;
    std::array<double, taps> h{};
    double sum = 0.0;
    for (int k = 0; k < taps; ++k) {
        const int m = k - (taps - 1) / 2;
        const double sinc = m == 0 ? 2.0 * cutoff : std::sin(2.0 * kPi * cutoff * m) / (kPi * m);
        h[std::size_t(k)] = sinc * (0.42 - 0.5 * std::cos(2.0 * kPi * k / (taps - 1)) + 0.08 * std::cos(4.0 * kPi * k / (taps - 1)));
        sum += h[std::size_t(k)];
    }
    std::vector<double> out;
    for (std::size_t i = offset + taps; i + taps < n; i += 4) {
        double y = 0.0;
        for (int k = 0; k < taps; ++k) y += h[std::size_t(k)] / sum * x[i - std::size_t(k)];
        out.push_back(y);
    }
    return out;
}
// level of a stereo sine through the guard (after it settles), dB
double guardLevelDb(double hz, double dc, double* meanOut) {
    batruc::StereoLookaheadGuard g;
    g.prepare(48000.0);
    double in = 0.0, out = 0.0, mean = 0.0;
    int count = 0;
    for (int n = 0; n < 48000 * 4; ++n) {
        const float s = static_cast<float>(0.5 * std::sin(2.0 * kPi * hz * n / 48000.0));
        float l = s + static_cast<float>(dc), r = l;
        g.process(l, r);
        if (n >= 48000 * 3) { in += double(s) * s; out += double(l) * l; mean += l; ++count; }
    }
    if (meanOut != nullptr) *meanOut = mean / count;
    return 10.0 * std::log10(out / in);
}
}  // namespace

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
    // 16/11/2026: EBU Tech 3341 true peak (signals 15-23) for the estimator
    const struct { const char* name; double f, a, deg, expected; } tones[] = {
        {"#15", 12000.0, 0.5, 0.0, -6.0}, {"#16", 12000.0, 0.5, 45.0, -6.0}, {"#17", 8000.0, 0.5, 60.0, -6.0},
        {"#18", 6000.0, 0.5, 67.5, -6.0}, {"#19", 12000.0, 1.41, 45.0, 3.0}};
    for (const auto& t : tones) {
        const double db = estimatorDb(ebuTone(t.f, t.a, t.deg));
        std::printf("EBU 3341 %s: %.2f dBTP (expected %.1f +0.2/-0.4)\n", t.name, db, t.expected);
        if (db > t.expected + 0.2 || db < t.expected - 0.4) return 9;
    }
    for (std::size_t offset = 0; offset < 4; ++offset) {
        const double db = estimatorDb(ebuSinglePeriod(offset));
        std::printf("EBU 3341 #%zu: %.2f dBTP (expected 0 +0.2/-0.4)\n", 20 + offset, db);
        if (db > 0.2 || db < -0.4) return 10;
    }
    // 16/11/2026: DC out (5 Hz); the bass register passes
    double mean = 1.0;
    const double at55 = guardLevelDb(55.0, 0.3, &mean);
    const double at41 = guardLevelDb(41.2, 0.0, nullptr);
    std::printf("DC 0.3 in: %.5f out; 55 Hz %.3f dB; 41.2 Hz %.3f dB\n", mean, at55, at41);
    if (std::abs(mean) > 0.001 || std::abs(at55) > 0.06 || std::abs(at41) > 0.1) return 11;
    // 16/11/2026: the REC's dithered integers survive JUCE 9.0.3's 24-bit
    // conversion (round(x * (2^31 - 1)) >> 8) over the whole range
    for (std::int32_t k = -8388608; k <= 8388607; ++k) {
        const double x = batruc::floatForJuceInt24(k);
        const int i32 = x <= -1.0 ? std::numeric_limits<int>::min()
                      : x >= 1.0 ? std::numeric_limits<int>::max()
                      : static_cast<int>(std::lround(std::numeric_limits<int>::max() * x));
        if ((i32 >> 8) != k) return 12;
    }
    return 0;
}
