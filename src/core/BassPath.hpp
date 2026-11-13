// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#pragma once

#include "core/BassComposition.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <vector>

namespace batruc {

struct BassPathAnalysis {
    std::array<int, 7> offsets{};
    std::array<double, 7> field{};
    std::array<double, 7> activity{};
    std::array<double, 7> integral{};
    std::array<double, 7> derivative{};
    std::array<double, 7> curvature{};
    std::array<double, 7> correlation{};
    std::array<double, 7> regressionYFromX{};
    std::array<double, 7> speed{};
    std::array<double, 7> signedCurvature{};
    std::array<std::uint8_t, 7> route{};
};

// The seven formal cycles form a path. Each new position weighs the current
// voice field, the previous step and the formal destination. This is a
// Batruc-specific relational pitch calculation, not a fixed scale or a chord
// progression. It is prepared outside the audio callback.
inline BassPathAnalysis analyseSeedBassPath(
    const std::array<std::vector<SeedBassVoiceEvent>, 7>& voiceEvents,
    const std::uint32_t seed,
    const double memoryLeak = 0.72) noexcept {
    BassPathAnalysis analysis;
    constexpr std::array<int, 7> moves = {-5, -3, -2, 0, 2, 3, 5};
    // F + C: the seed supplies a small, reproducible integration constant.
    // A leaky, bounded integral retains the history of the voice field without
    // allowing invisible DC drift to accumulate across the seven-cycle form.
    double accumulatedField = (static_cast<double>(seed & 0xffu) / 255.0 - 0.5)
        * 0.7;
    analysis.integral[0] = accumulatedField;
    double previousField = 0.0;
    double previousDerivative = 0.0;
    int previousMove = 0;
    for (std::size_t cycle = 1; cycle < analysis.offsets.size(); ++cycle) {
        double x = 0.0;
        double y = 0.0;
        double energy = 0.0;
        double speed = 0.0;
        double signedCurvature = 0.0;
        double xx = 0.0;
        double yy = 0.0;
        double xy = 0.0;
        std::size_t count = 0;
        for (const auto& event : voiceEvents[cycle]) {
            if (!std::isfinite(event.x) || !std::isfinite(event.y)
                || !std::isfinite(event.intensity))
                continue;
            x += std::clamp(event.x, -1.0, 1.0);
            y += std::clamp(event.y, -1.0, 1.0);
            xx += std::clamp(event.x, -1.0, 1.0)
                * std::clamp(event.x, -1.0, 1.0);
            yy += std::clamp(event.y, -1.0, 1.0)
                * std::clamp(event.y, -1.0, 1.0);
            xy += std::clamp(event.x, -1.0, 1.0)
                * std::clamp(event.y, -1.0, 1.0);
            energy += std::clamp(static_cast<double>(event.intensity), 0.0, 1.0);
            speed += std::clamp(static_cast<double>(event.pathSpeed), 0.0, 1.0);
            signedCurvature += std::clamp(
                static_cast<double>(event.pathSignedCurvature), -1.0, 1.0);
            ++count;
        }
        const double meanX = count > 0 ? x / static_cast<double>(count) : 0.0;
        const double meanY = count > 0 ? y / static_cast<double>(count) : 0.0;
        const double varianceX = count > 0
            ? std::max(0.0, xx / static_cast<double>(count) - meanX * meanX)
            : 0.0;
        const double varianceY = count > 0
            ? std::max(0.0, yy / static_cast<double>(count) - meanY * meanY)
            : 0.0;
        const double covariance = count > 0
            ? xy / static_cast<double>(count) - meanX * meanY : 0.0;
        const double correlationDenominator = std::sqrt(varianceX * varianceY);
        const double correlation = correlationDenominator > 1.0e-9
            ? std::clamp(covariance / correlationDenominator, -1.0, 1.0) : 0.0;
        // Regression slope is compressed to [-1,1] so a nearly vertical fit
        // remains useful as a bounded compositional descriptor.
        const double regressionYFromX = varianceX > 1.0e-9
            ? std::tanh(covariance / varianceX) : 0.0;
        const double field = std::clamp(meanY - meanX, -1.0, 1.0);
        const double activity = count > 0 ? energy / static_cast<double>(count) : 0.0;
        const double meanSpeed = count > 0 ? speed / static_cast<double>(count) : 0.0;
        const double meanSignedCurvature = count > 0
            ? signedCurvature / static_cast<double>(count) : 0.0;
        const double derivative = field - previousField;
        const double curvature = derivative - previousDerivative;
        const double boundedLeak = std::clamp(memoryLeak, 0.0, 0.95);
        accumulatedField = std::clamp(boundedLeak * accumulatedField + field,
                                      -1.5, 1.5);
        analysis.field[cycle] = field;
        analysis.activity[cycle] = activity;
        analysis.integral[cycle] = accumulatedField;
        analysis.derivative[cycle] = derivative;
        analysis.curvature[cycle] = curvature;
        analysis.correlation[cycle] = correlation;
        analysis.regressionYFromX[cycle] = regressionYFromX;
        analysis.speed[cycle] = meanSpeed;
        analysis.signedCurvature[cycle] = meanSignedCurvature;
        // The seed chooses a starting relation. Energy and the previous move
        // can branch into another relation on the next cycle.
        // Exponential response, trigonometric opposition, angular orbit and
        // a distance-triggered rupture are distinct calculations, not labels.
        const unsigned route = ((seed >> 3u)
            + (previousMove < 0 ? 1u : previousMove > 0 ? 2u : 0u)
            + (activity > 0.7 && (cycle & 1u) ? 2u : 0u)
            + (cycle == 3u ? 1u : 0u)) & 3u;
        analysis.route[cycle] = static_cast<std::uint8_t>(route);
        const double exponential = 4.0 * std::expm1(1.1 * field) / std::expm1(1.1);
        const double opposition = -4.0 * std::sin(1.5707963267948966 * field);
        const double orbit = 4.0 * std::sin(std::atan2(meanY, meanX)
            + static_cast<double>(cycle) * 0.8975979010256552);
        const double radius = std::hypot(meanX, meanY);
        const double rupture = cycle == 3u && radius > 0.55
            ? (field >= 0.0 ? -5.0 : 5.0)
            : 2.0 * std::tanh(2.0 * field);
        const double target = std::clamp((route == 0u ? exponential
            : route == 1u ? opposition
            : route == 2u ? orbit : rupture)
            + (activity - 0.5)
            + 1.15 * accumulatedField
            + 0.55 * derivative
            + 0.22 * curvature
            + 0.85 * correlation
            + 0.65 * regressionYFromX
            + 0.45 * meanSignedCurvature, -5.0, 5.0);
        const bool returnSection = cycle >= 5;
        int bestMove = 0;
        double bestCost = std::numeric_limits<double>::infinity();
        for (const int move : moves) {
            const int next = analysis.offsets[cycle - 1] + move;
            if (next < -9 || next > 9)
                continue;
            std::uint32_t hash = seed ^ (0x9e3779b9u * static_cast<std::uint32_t>(cycle))
                ^ (0x85ebca6bu * static_cast<std::uint32_t>(move + 6));
            hash ^= hash >> 16u;
            hash *= 0x7feb352du;
            hash ^= hash >> 15u;
            const double tieBreak = static_cast<double>(hash & 0xffffu) / 65535.0;
            double cost = std::abs(static_cast<double>(move) - target)
                + 0.22 * std::abs(move - previousMove)
                + 0.12 * std::abs(move)
                + 0.31 * tieBreak;
            if (returnSection)
                cost += (cycle == 6 ? 1.0 : 0.48) * std::abs(next);
            if (cycle == 4) // Expansion: hold the pitch path while the form remains audible.
                cost += 0.75 * std::abs(move);
            if (cost < bestCost) {
                bestCost = cost;
                bestMove = move;
            }
        }
        analysis.offsets[cycle] = analysis.offsets[cycle - 1] + bestMove;
        previousMove = bestMove;
        previousField = field;
        previousDerivative = derivative;
    }
    return analysis;
}

inline std::array<int, 7> buildSeedBassPath(
    const std::array<std::vector<SeedBassVoiceEvent>, 7>& voiceEvents,
    const std::uint32_t seed,
    const double memoryLeak = 0.72) noexcept {
    return analyseSeedBassPath(voiceEvents, seed, memoryLeak).offsets;
}

} // namespace batruc
