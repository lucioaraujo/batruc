// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/GeometryCurves.hpp"

#include <array>
#include <cstdio>

int main() {
    constexpr std::array<double, 9> progress = {
        0.0, 0.03125, 0.123, 0.25, 0.37, 0.5, 0.749, 0.875, 1.0};
    constexpr std::array<int, 12> terms = {
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 5, 11};
    for (int curve = 0; curve < 12; ++curve) {
        const int patterns = curve == 11 ? 7 : 1;
        for (int pattern = 0; pattern < patterns; ++pattern) {
            for (const double pointProgress : progress) {
                batruc::Point2D point;
                if (!batruc::geometryCurvePoint(
                        static_cast<batruc::GeometryCurve>(curve),
                        pointProgress, 2.75, 0.137, terms[static_cast<std::size_t>(curve)],
                        point, pattern))
                    return 1;
                std::printf("%d %d %.17g %.17g %.17g\n", curve, pattern,
                            pointProgress, point.x, point.y);
            }
        }
    }

    constexpr std::array<int, 3> fourierTerms = {1, 5, 11};
    constexpr std::array<double, 3> fourierProgress = {0.0, 0.137, 0.553};
    for (int curve = 5; curve <= 6; ++curve) {
        for (const int termsCount : fourierTerms) {
            for (const double pointProgress : fourierProgress) {
                std::array<batruc::GeometryFourierVector,
                    2 * batruc::kMaximumGeometryFourierTerms + 1> vectors{};
                std::size_t count = 0;
                if (!batruc::geometryCurveFourierEpicycles(
                        static_cast<batruc::GeometryCurve>(curve), pointProgress,
                        2.5, 0.137, termsCount, vectors.data(), vectors.size(), count))
                    return 2;
                for (std::size_t index = 0; index < count; ++index) {
                    const auto& vector = vectors[index];
                    std::printf("v %d %d %.17g %zu %d %.17g %.17g %.17g %.17g %.17g\n",
                        curve, termsCount, pointProgress, index, vector.harmonic,
                        vector.fromX, vector.fromY, vector.toX, vector.toY,
                        vector.radius);
                }
            }
        }
    }
    return 0;
}
