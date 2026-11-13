// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/TriangleGeometry.hpp"

#include <algorithm>
#include <cmath>

namespace batruc {
namespace {

double distance(const Point2D a, const Point2D b) noexcept {
    return std::hypot(a.x - b.x, a.y - b.y);
}

bool finitePoint(const Point2D point) noexcept {
    return std::isfinite(point.x) && std::isfinite(point.y);
}

} // namespace

TriangleMetrics analyzeTriangle(const std::array<Point2D, 3>& vertices) noexcept {
    TriangleMetrics result;
    if (!finitePoint(vertices[0]) || !finitePoint(vertices[1])
        || !finitePoint(vertices[2]))
        return result;

    result.vertices = vertices;
    const auto aPoint = vertices[0];
    const auto bPoint = vertices[1];
    const auto cPoint = vertices[2];
    result.sides = {distance(bPoint, cPoint), distance(cPoint, aPoint),
                    distance(aPoint, bPoint)};
    const double a = result.sides[0];
    const double b = result.sides[1];
    const double c = result.sides[2];
    const double largestSide = std::max({a, b, c});
    if (!(largestSide > 0.0) || !std::isfinite(largestSide))
        return result;

    const double twiceSignedArea = (bPoint.x - aPoint.x) * (cPoint.y - aPoint.y)
        - (bPoint.y - aPoint.y) * (cPoint.x - aPoint.x);
    result.area = 0.5 * std::abs(twiceSignedArea);
    if (!(result.area > 1.0e-14 * largestSide * largestSide))
        return result;

    result.perimeter = a + b + c;
    result.semiperimeter = result.perimeter * 0.5;
    const double s = result.semiperimeter;
    const double heronProduct = s * (s - a) * (s - b) * (s - c);
    if (heronProduct < -1.0e-12 * largestSide * largestSide
            * largestSide * largestSide || !std::isfinite(heronProduct))
        return TriangleMetrics{};
    result.heronArea = std::sqrt(std::max(0.0, heronProduct));
    result.areaAgreementError = std::abs(result.area - result.heronArea)
        / result.area;

    result.medians = {
        0.5 * std::sqrt(std::max(0.0, 2.0 * b * b + 2.0 * c * c - a * a)),
        0.5 * std::sqrt(std::max(0.0, 2.0 * c * c + 2.0 * a * a - b * b)),
        0.5 * std::sqrt(std::max(0.0, 2.0 * a * a + 2.0 * b * b - c * c))};
    result.compactness = 4.0 * std::sqrt(3.0) * result.area
        / (a * a + b * b + c * c);

    std::array<double, 3> sortedSides = result.sides;
    std::sort(sortedSides.begin(), sortedSides.end());
    const double pythagoreanDelta = sortedSides[2] * sortedSides[2]
        - sortedSides[1] * sortedSides[1] - sortedSides[0] * sortedSides[0];
    result.rightAngleResidual = std::abs(pythagoreanDelta)
        / (sortedSides[2] * sortedSides[2]);

    result.inradius = result.area / s;
    result.circumradius = a * b * c / (4.0 * result.area);
    result.centroid = {(aPoint.x + bPoint.x + cPoint.x) / 3.0,
                       (aPoint.y + bPoint.y + cPoint.y) / 3.0};

    const double determinant = 2.0 * (aPoint.x * (bPoint.y - cPoint.y)
        + bPoint.x * (cPoint.y - aPoint.y)
        + cPoint.x * (aPoint.y - bPoint.y));
    if (std::abs(determinant) <= 1.0e-14 * largestSide * largestSide)
        return TriangleMetrics{};
    const double aSquared = aPoint.x * aPoint.x + aPoint.y * aPoint.y;
    const double bSquared = bPoint.x * bPoint.x + bPoint.y * bPoint.y;
    const double cSquared = cPoint.x * cPoint.x + cPoint.y * cPoint.y;
    result.circumcenter = {
        (aSquared * (bPoint.y - cPoint.y)
         + bSquared * (cPoint.y - aPoint.y)
         + cSquared * (aPoint.y - bPoint.y)) / determinant,
        (aSquared * (cPoint.x - bPoint.x)
         + bSquared * (aPoint.x - cPoint.x)
         + cSquared * (bPoint.x - aPoint.x)) / determinant};
    result.orthocenter = {
        aPoint.x + bPoint.x + cPoint.x - 2.0 * result.circumcenter.x,
        aPoint.y + bPoint.y + cPoint.y - 2.0 * result.circumcenter.y};

    const double gToO = distance(result.centroid, result.circumcenter);
    const double oToH = distance(result.circumcenter, result.orthocenter);
    if (gToO > 1.0e-14 * largestSide) {
        const double gToH = distance(result.centroid, result.orthocenter);
        const double cross = (result.circumcenter.x - result.centroid.x)
                * (result.orthocenter.y - result.centroid.y)
            - (result.circumcenter.y - result.centroid.y)
                * (result.orthocenter.x - result.centroid.x);
        result.eulerLineAlignmentError = std::abs(cross) / (gToO * gToH);
        result.eulerDistanceRatio = oToH / gToO;
    }
    result.valid = std::isfinite(result.area) && std::isfinite(result.heronArea)
        && std::isfinite(result.circumradius);
    return result;
}

bool stewartCevianLength(const double a, const double b, const double c,
                         const double t, double& length) noexcept {
    if (!std::isfinite(a) || !std::isfinite(b) || !std::isfinite(c)
        || !std::isfinite(t) || !(a > 0.0) || !(b > 0.0) || !(c > 0.0)
        || t < 0.0 || t > 1.0 || a >= b + c || b >= a + c || c >= a + b)
        return false;
    const double squared = (1.0 - t) * c * c + t * b * b
        - t * (1.0 - t) * a * a;
    if (squared < -1.0e-12 * std::max({a * a, b * b, c * c}))
        return false;
    length = std::sqrt(std::max(0.0, squared));
    return std::isfinite(length);
}

bool cevaConcurrencyError(const std::array<double, 3>& splitFractions,
                          double& error) noexcept {
    double logProduct = 0.0;
    for (const double fraction : splitFractions) {
        if (!std::isfinite(fraction) || fraction <= 0.0 || fraction >= 1.0)
            return false;
        logProduct += std::log(fraction) - std::log1p(-fraction);
    }
    error = std::abs(logProduct);
    return std::isfinite(error);
}

bool stengelExtendedLadderFourthArea(const double x, const double y,
                                    const double z, double& w) noexcept {
    if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z)
        || !(x > 0.0) || !(y > 0.0) || !(z > 0.0))
        return false;
    const double reciprocalTotal = 1.0 / (x + z) + 1.0 / (y + z)
        - 1.0 / z;
    if (!(reciprocalTotal > 0.0) || !std::isfinite(reciprocalTotal))
        return false;
    const double totalArea = 1.0 / reciprocalTotal;
    const double candidate = totalArea - x - y - z;
    if (!(candidate > 0.0) || !std::isfinite(candidate))
        return false;
    w = candidate;
    return true;
}

bool stengelExtendedLadderAreaError(const double w, const double x,
                                    const double y, const double z,
                                    double& relativeError) noexcept {
    if (!std::isfinite(w) || !std::isfinite(x) || !std::isfinite(y)
        || !std::isfinite(z) || !(w > 0.0) || !(x > 0.0)
        || !(y > 0.0) || !(z > 0.0))
        return false;
    const double totalArea = w + x + y + z;
    const double left = 1.0 / totalArea + 1.0 / z;
    const double right = 1.0 / (x + z) + 1.0 / (y + z);
    const double scale = std::abs(left) + std::abs(right);
    if (!(scale > 0.0) || !std::isfinite(scale))
        return false;
    relativeError = std::abs(left - right) / scale;
    return std::isfinite(relativeError);
}

} // namespace batruc
