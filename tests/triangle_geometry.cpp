// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/TriangleGeometry.hpp"

#include <array>
#include <cmath>
#include <iostream>

namespace {

bool close(const double actual, const double expected,
           const double tolerance = 1.0e-10) {
    return std::abs(actual - expected) <= tolerance;
}

} // namespace

int main() {
    const auto right = batruc::analyzeTriangle(
        {{{0.0, 0.0}, {4.0, 0.0}, {0.0, 3.0}}});
    if (!right.valid || !close(right.area, 6.0)
        || !close(right.heronArea, 6.0)
        || right.areaAgreementError > 1.0e-10
        || !close(right.rightAngleResidual, 0.0)
        || !close(right.medians[0], 2.5)
        || !close(right.circumradius, 2.5)
        || !close(right.eulerDistanceRatio, 3.0)
        || right.eulerLineAlignmentError > 1.0e-10) {
        std::cerr << "3-4-5 triangle metrics failed\n";
        return 1;
    }

    double median = 0.0;
    if (!batruc::stewartCevianLength(5.0, 3.0, 4.0, 0.5, median)
        || !close(median, 2.5)
        || batruc::stewartCevianLength(5.0, 2.0, 3.0, 0.5, median)) {
        std::cerr << "Stewart/Apollonius median failed\n";
        return 1;
    }

    double cevaError = 0.0;
    if (!batruc::cevaConcurrencyError({0.5, 0.5, 0.5}, cevaError)
        || !close(cevaError, 0.0)
        || batruc::cevaConcurrencyError({0.0, 0.5, 0.5}, cevaError)) {
        std::cerr << "Ceva concurrency validation failed\n";
        return 1;
    }

    double ladderW = 0.0;
    double ladderError = 0.0;
    if (!batruc::stengelExtendedLadderFourthArea(6.0, 3.0, 9.0, ladderW)
        || !close(ladderW, 54.0 / 7.0)
        || !batruc::stengelExtendedLadderAreaError(
            ladderW, 6.0, 3.0, 9.0, ladderError)
        || ladderError > 1.0e-12
        || batruc::stengelExtendedLadderFourthArea(1.0, 1.0, 1.0, ladderW)) {
        std::cerr << "Extended ladder area relation failed\n";
        return 1;
    }

    const auto degenerate = batruc::analyzeTriangle(
        {{{0.0, 0.0}, {1.0, 1.0}, {2.0, 2.0}}});
    if (degenerate.valid) {
        std::cerr << "Degenerate triangle was accepted\n";
        return 1;
    }
    return 0;
}
