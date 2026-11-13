// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#pragma once

#include <array>

namespace batruc {

struct Point2D {
    double x = 0.0;
    double y = 0.0;
};

// Side convention: a = |BC|, b = |CA|, c = |AB|.
struct TriangleMetrics {
    bool valid = false;
    std::array<Point2D, 3> vertices{};
    std::array<double, 3> sides{};
    std::array<double, 3> medians{};
    double perimeter = 0.0;
    double semiperimeter = 0.0;
    double area = 0.0;
    double heronArea = 0.0;
    double areaAgreementError = 0.0; // |coordinate area - Heron area| / area.
    double compactness = 0.0; // 1 for equilateral; tends to 0 at degeneracy.
    double rightAngleResidual = 0.0; // 0 when the longest-side angle is right.
    double inradius = 0.0;
    double circumradius = 0.0;
    Point2D centroid{};
    Point2D circumcenter{};
    Point2D orthocenter{};
    double eulerLineAlignmentError = 0.0; // normalized cross-product residual.
    double eulerDistanceRatio = 0.0; // |OH| / |OG|; 3 for non-equilateral triangles.
};

// Computes coordinate and side-based triangle descriptors. Degenerate and
// non-finite input is rejected; no artificial epsilon geometry is returned.
TriangleMetrics analyzeTriangle(const std::array<Point2D, 3>& vertices) noexcept;

// Stewart's theorem. t is the fraction along B->C where the cevian lands:
// d^2 = (1-t)c^2 + t b^2 - t(1-t)a^2.
bool stewartCevianLength(double a, double b, double c, double t,
                         double& length) noexcept;

// Ceva's theorem for interior position fractions tA=BD/BC, tB=CE/CA,
// tC=AF/AB. Returns the absolute logarithmic residual of the product
// BD/DC * CE/EA * AF/FB; 0 means concurrent.
bool cevaConcurrencyError(const std::array<double, 3>& splitFractions,
                          double& error) noexcept;

// Extended crossed-ladder/Stengel area relation for four regions:
// 1/S + 1/z = 1/(x+z) + 1/(y+z), S = w+x+y+z.
// All inputs are positive areas. Solving derives w from x,y,z.
bool stengelExtendedLadderFourthArea(double x, double y, double z,
                                    double& w) noexcept;
bool stengelExtendedLadderAreaError(double w, double x, double y, double z,
                                    double& relativeError) noexcept;

} // namespace batruc
