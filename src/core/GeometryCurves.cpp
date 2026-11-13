// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/GeometryCurves.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <utility>

namespace batruc {
namespace {

constexpr double kTau = 6.283185307179586476925286766559;
constexpr double kBilliardHalf = 0.78;
constexpr double kBilliardStartX = -0.31;
constexpr double kBilliardStartY = -0.43;
constexpr int kReferenceSamples = 2048;
constexpr int kErrorSamples = 512;
constexpr int kHarmonicCount = 2 * kMaximumGeometryFourierTerms + 1;

struct ComplexPoint {
    double real = 0.0;
    double imaginary = 0.0;
};

using FourierTable = std::array<ComplexPoint, kHarmonicCount>;

struct BilliardLaps { int x; int y; };
constexpr std::array<BilliardLaps, 7> kBilliardPatterns = {{
    {2, 4}, {4, 2}, {2, 6}, {6, 2}, {4, 4}, {4, 6}, {6, 4}}};

BilliardLaps billiardLaps(const int pattern) noexcept {
    return kBilliardPatterns[static_cast<std::size_t>(
        std::clamp(pattern, 0, static_cast<int>(kBilliardPatterns.size()) - 1))];
}

bool isFinite(const double value) noexcept { return std::isfinite(value); }

double foldBilliard(const double position) noexcept {
    const double width = 2.0 * kBilliardHalf;
    double folded = std::fmod(position + kBilliardHalf, 2.0 * width);
    if (folded < 0.0)
        folded += 2.0 * width;
    return folded <= width ? -kBilliardHalf + folded
                           : 3.0 * kBilliardHalf - folded;
}

Point2D polygonPoint(const GeometryCurve curve, const double progress) noexcept {
    constexpr std::array<Point2D, 3> triangle = {{
        {-0.9, -0.62}, {0.9, -0.62}, {0.0, 0.94}}};
    constexpr std::array<Point2D, 4> square = {{
        {-0.78, -0.78}, {0.78, -0.78}, {0.78, 0.78}, {-0.78, 0.78}}};
    const auto* vertices = curve == GeometryCurve::triangle
        ? triangle.data() : square.data();
    const std::size_t count = curve == GeometryCurve::triangle
        ? triangle.size() : square.size();
    const auto position = progress * static_cast<double>(count);
    const auto integer = static_cast<int>(std::floor(position));
    const auto segment = static_cast<std::size_t>(integer)
        % count;
    const double fraction = position - std::floor(position);
    const auto start = vertices[segment];
    const auto end = vertices[(segment + 1) % count];
    return {start.x + (end.x - start.x) * fraction,
            start.y + (end.y - start.y) * fraction};
}

GeometryCurve polygonForFourier(const GeometryCurve curve) noexcept {
    return curve == GeometryCurve::fourierTriangle
        ? GeometryCurve::triangle : GeometryCurve::square;
}

FourierTable makeFourierTable(const GeometryCurve polygon) noexcept {
    FourierTable table{};
    for (int harmonic = -kMaximumGeometryFourierTerms;
         harmonic <= kMaximumGeometryFourierTerms; ++harmonic) {
        auto& coefficient = table[static_cast<std::size_t>(
            harmonic + kMaximumGeometryFourierTerms)];
        for (int sample = 0; sample < kReferenceSamples; ++sample) {
            const auto point = polygonPoint(polygon,
                static_cast<double>(sample) / kReferenceSamples);
            const double angle = kTau * harmonic * sample / kReferenceSamples;
            const double cosine = std::cos(angle);
            const double sine = std::sin(angle);
            coefficient.real += point.x * cosine + point.y * sine;
            coefficient.imaginary += point.y * cosine - point.x * sine;
        }
        coefficient.real /= kReferenceSamples;
        coefficient.imaginary /= kReferenceSamples;
    }
    return table;
}

const FourierTable& coefficientsFor(const GeometryCurve curve) noexcept {
    static const auto triangle = makeFourierTable(GeometryCurve::triangle);
    static const auto square = makeFourierTable(GeometryCurve::square);
    return curve == GeometryCurve::fourierTriangle ? triangle : square;
}

Point2D fourierPoint(const GeometryCurve curve, const double progress,
                     const int terms) noexcept {
    const auto& coefficients = coefficientsFor(curve);
    Point2D point{};
    for (int harmonic = -terms; harmonic <= terms; ++harmonic) {
        const auto coefficient = coefficients[static_cast<std::size_t>(
            harmonic + kMaximumGeometryFourierTerms)];
        const double angle = kTau * harmonic * progress;
        const double cosine = std::cos(angle);
        const double sine = std::sin(angle);
        point.x += coefficient.real * cosine - coefficient.imaginary * sine;
        point.y += coefficient.real * sine + coefficient.imaginary * cosine;
    }
    return point;
}

} // namespace

bool geometryCurveRollingState(const GeometryCurve curve, const double progress,
                               const double cycles, const double phaseTurns,
                               GeometryRollingState& state) noexcept {
    if (!isFinite(progress) || !isFinite(cycles) || !isFinite(phaseTurns)
        || cycles <= 0.0)
        return false;

    double fixedToRollingRatio = 0.0;
    double penToRollingRatio = 1.0;
    GeometryRollingSupport support = GeometryRollingSupport::line;
    switch (curve) {
        case GeometryCurve::cardioid:
            support = GeometryRollingSupport::outsideCircle;
            fixedToRollingRatio = 1.0;
            break;
        case GeometryCurve::epicycloid: // two-cusp nephroid
            support = GeometryRollingSupport::outsideCircle;
            fixedToRollingRatio = 2.0;
            break;
        case GeometryCurve::ranunculoid:
            support = GeometryRollingSupport::outsideCircle;
            fixedToRollingRatio = 5.0;
            break;
        case GeometryCurve::hypocycloid: // three-cusp deltoid
            support = GeometryRollingSupport::insideCircle;
            fixedToRollingRatio = 3.0;
            break;
        case GeometryCurve::astroid:
            support = GeometryRollingSupport::insideCircle;
            fixedToRollingRatio = 4.0;
            break;
        case GeometryCurve::cycloid:
            penToRollingRatio = 1.0;
            break;
        case GeometryCurve::curtateTrochoid:
            penToRollingRatio = 0.65;
            break;
        case GeometryCurve::prolateTrochoid:
            penToRollingRatio = 1.35;
            break;
        default:
            return false;
    }

    const double unwrapped = progress * cycles + phaseTurns;
    const double phase = unwrapped - std::floor(unwrapped);
    const double angle = kTau * phase;
    state = {};
    state.support = support;
    if (support == GeometryRollingSupport::outsideCircle) {
        const double normalizer = fixedToRollingRatio + 2.0;
        state.fixedRadius = fixedToRollingRatio / normalizer;
        state.rollingRadius = 1.0 / normalizer;
        const double centreDistance = (fixedToRollingRatio + 1.0) / normalizer;
        state.circleCentre = {centreDistance * std::cos(angle),
                              centreDistance * std::sin(angle)};
        const double wheelAngle = (fixedToRollingRatio + 1.0) * angle;
        state.pen = {state.circleCentre.x - state.rollingRadius * std::cos(wheelAngle),
                     state.circleCentre.y - state.rollingRadius * std::sin(wheelAngle)};
    } else if (support == GeometryRollingSupport::insideCircle) {
        state.fixedRadius = 1.0;
        state.rollingRadius = 1.0 / fixedToRollingRatio;
        const double centreDistance = (fixedToRollingRatio - 1.0)
            / fixedToRollingRatio;
        state.circleCentre = {centreDistance * std::cos(angle),
                              centreDistance * std::sin(angle)};
        const double wheelAngle = (fixedToRollingRatio - 1.0) * angle;
        state.pen = {state.circleCentre.x + penToRollingRatio
                         * state.rollingRadius * std::cos(wheelAngle),
                     state.circleCentre.y - penToRollingRatio
                         * state.rollingRadius * std::sin(wheelAngle)};
    } else {
        // One revolution traverses a straight track of length 2πr. The scale
        // keeps the full trochoid in the normalized viewport for d/r ≤ 1.35.
        const double rollingRadius = 1.0
            / (0.5 * kTau + penToRollingRatio);
        state.rollingRadius = rollingRadius;
        state.circleCentre = {rollingRadius * (kTau * phase - 0.5 * kTau), 0.0};
        state.pen = {state.circleCentre.x - penToRollingRatio * rollingRadius
                         * std::sin(angle),
                     -penToRollingRatio * rollingRadius * std::cos(angle)};
    }

    return isFinite(state.circleCentre.x) && isFinite(state.circleCentre.y)
        && isFinite(state.pen.x) && isFinite(state.pen.y)
        && isFinite(state.rollingRadius) && isFinite(state.fixedRadius);
}

bool geometryCurvePoint(const GeometryCurve curve, const double progress,
                        const double cycles, const double phaseTurns,
                        const int fourierTerms, Point2D& point,
                        const int billiardPattern,
                        const GeometryLissajousParameters lissajous) noexcept {
    if (!isFinite(progress) || !isFinite(cycles) || !isFinite(phaseTurns)
        || cycles <= 0.0 || fourierTerms < 1
        || fourierTerms > kMaximumGeometryFourierTerms)
        return false;

    GeometryRollingState rolling;
    if (geometryCurveRollingState(curve, progress, cycles, phaseTurns, rolling)) {
        point = rolling.pen;
        return true;
    }

    const double unwrapped = progress * cycles + phaseTurns;
    const double wrapped = unwrapped - std::floor(unwrapped);
    switch (curve) {
        case GeometryCurve::compass: {
            const double angle = kTau * unwrapped;
            point = {std::cos(angle), std::sin(angle)};
            break;
        }
        case GeometryCurve::triangle:
        case GeometryCurve::square:
            point = polygonPoint(curve, wrapped);
            break;
        case GeometryCurve::billiardSquare: {
            if (billiardPattern < 0
                || billiardPattern >= static_cast<int>(kBilliardPatterns.size()))
                return false;
            const auto laps = billiardLaps(billiardPattern);
            const double width = 2.0 * kBilliardHalf;
            const double x = foldBilliard(kBilliardStartX
                + laps.x * width * unwrapped);
            const double y = foldBilliard(kBilliardStartY
                + laps.y * width * unwrapped);
            point = {x / kBilliardHalf, y / kBilliardHalf};
            break;
        }
        case GeometryCurve::spiral: {
            const double angle = kTau * unwrapped;
            const double radius = std::clamp(progress, 0.0, 1.0);
            point = {radius * std::cos(angle), radius * std::sin(angle)};
            break;
        }
        case GeometryCurve::lissajous: {
            if (lissajous.xFrequency < 1 || lissajous.xFrequency > 12
                || lissajous.yFrequency < 1 || lissajous.yFrequency > 12
                || !isFinite(lissajous.yPhaseTurns)
                || !isFinite(lissajous.xAmplitude)
                || !isFinite(lissajous.yAmplitude)
                || lissajous.xAmplitude < 0.0 || lissajous.xAmplitude > 1.0
                || lissajous.yAmplitude < 0.0 || lissajous.yAmplitude > 1.0)
                return false;
            const double angle = kTau * unwrapped;
            point = {lissajous.xAmplitude
                         * std::sin(lissajous.xFrequency * angle),
                     lissajous.yAmplitude * std::sin(
                         lissajous.yFrequency * angle
                         + kTau * lissajous.yPhaseTurns)};
            break;
        }
        case GeometryCurve::epicycloid: {
            // Nephroid: fixed radius R=2r, rolling circle radius r.
            const double angle = kTau * unwrapped;
            point = {(3.0 * std::cos(angle) - std::cos(3.0 * angle)) / 4.0,
                     (3.0 * std::sin(angle) - std::sin(3.0 * angle)) / 4.0};
            break;
        }
        case GeometryCurve::hypocycloid: {
            // Deltoid: fixed radius R=3r, rolling circle radius r.
            const double angle = kTau * unwrapped;
            point = {(2.0 * std::cos(angle) + std::cos(2.0 * angle)) / 3.0,
                     (2.0 * std::sin(angle) - std::sin(2.0 * angle)) / 3.0};
            break;
        }
        case GeometryCurve::cycloid: {
            // One normalized cycloid arch followed by a horizontal pen return.
            // This closes the animation cycle without a discontinuous jump.
            if (wrapped <= 0.5) {
                const double angle = kTau * wrapped;
                point = {(angle - std::sin(angle)) / kTau - 1.0,
                         -std::cos(angle)};
            } else {
                const double returnProgress = (wrapped - 0.5) * 2.0;
                point = {1.0 - 2.0 * returnProgress, -1.0};
            }
            break;
        }
        case GeometryCurve::catenary: {
            // Trace the graph y=cosh(x) in both directions, continuously.
            const double x = std::sin(kTau * unwrapped);
            const double height = (std::cosh(x) - 1.0)
                / (std::cosh(1.0) - 1.0);
            point = {x, 2.0 * height - 1.0};
            break;
        }
        case GeometryCurve::fourierTriangle:
        case GeometryCurve::fourierSquare:
            point = fourierPoint(curve, wrapped, fourierTerms);
            break;
        default:
            return false;
    }
    return isFinite(point.x) && isFinite(point.y);
}

bool geometryCurvePolygonMetrics(const GeometryCurve curve,
                                 GeometryPolygonMetrics& metrics) noexcept {
    constexpr std::array<Point2D, 3> triangle = {{
        {-0.9, -0.62}, {0.9, -0.62}, {0.0, 0.94}}};
    constexpr std::array<Point2D, 4> square = {{
        {-0.78, -0.78}, {0.78, -0.78}, {0.78, 0.78}, {-0.78, 0.78}}};
    const Point2D* vertices = nullptr;
    std::size_t count = 0;
    if (curve == GeometryCurve::triangle) {
        vertices = triangle.data();
        count = triangle.size();
    } else if (curve == GeometryCurve::square) {
        vertices = square.data();
        count = square.size();
    } else {
        return false;
    }

    double twiceSignedArea = 0.0;
    double perimeter = 0.0;
    for (std::size_t index = 0; index < count; ++index) {
        const auto& current = vertices[index];
        const auto& next = vertices[(index + 1) % count];
        twiceSignedArea += current.x * next.y - next.x * current.y;
        perimeter += std::hypot(next.x - current.x, next.y - current.y);
    }
    metrics.area = std::abs(twiceSignedArea) * 0.5;
    metrics.perimeter = perimeter;
    return isFinite(metrics.area) && isFinite(metrics.perimeter);
}

bool geometryCurveFourierError(const GeometryCurve curve, const int fourierTerms,
                               double& errorPercent) noexcept {
    if ((curve != GeometryCurve::fourierTriangle
         && curve != GeometryCurve::fourierSquare)
        || fourierTerms < 1 || fourierTerms > kMaximumGeometryFourierTerms)
        return false;

    const auto referenceCurve = polygonForFourier(curve);
    double errorPower = 0.0;
    double referencePower = 0.0;
    for (int sample = 0; sample < kErrorSamples; ++sample) {
        const double progress = static_cast<double>(sample) / kErrorSamples;
        const auto reference = polygonPoint(referenceCurve, progress);
        const auto approximation = fourierPoint(curve, progress, fourierTerms);
        const double dx = reference.x - approximation.x;
        const double dy = reference.y - approximation.y;
        errorPower += dx * dx + dy * dy;
        referencePower += reference.x * reference.x + reference.y * reference.y;
    }
    if (!(referencePower > 0.0) || !isFinite(errorPower))
        return false;
    errorPercent = std::sqrt(errorPower / referencePower) * 100.0;
    return isFinite(errorPercent);
}

bool geometryCurveFourierEpicycles(const GeometryCurve curve,
                                   const double progress,
                                   const double cycles,
                                   const double phaseTurns,
                                   const int fourierTerms,
                                   GeometryFourierVector* vectors,
                                   const std::size_t capacity,
                                   std::size_t& count) noexcept {
    count = 0;
    if ((curve != GeometryCurve::fourierTriangle
         && curve != GeometryCurve::fourierSquare)
        || !isFinite(progress) || !isFinite(cycles) || cycles <= 0.0
        || !isFinite(phaseTurns) || fourierTerms < 1
        || fourierTerms > kMaximumGeometryFourierTerms || vectors == nullptr)
        return false;
    const auto required = static_cast<std::size_t>(fourierTerms * 2 + 1);
    if (capacity < required)
        return false;

    const auto& coefficients = coefficientsFor(curve);
    const double t = progress * cycles + phaseTurns;
    double x = 0.0;
    double y = 0.0;
    std::size_t output = 0;
    const auto append = [&](const int harmonic) {
        const auto coefficient = coefficients[static_cast<std::size_t>(
            harmonic + kMaximumGeometryFourierTerms)];
        const double angle = kTau * harmonic * t;
        const double cosine = std::cos(angle);
        const double sine = std::sin(angle);
        const double vectorX = coefficient.real * cosine
            - coefficient.imaginary * sine;
        const double vectorY = coefficient.real * sine
            + coefficient.imaginary * cosine;
        auto& vector = vectors[output++];
        vector.harmonic = harmonic;
        vector.fromX = x;
        vector.fromY = y;
        vector.toX = x + vectorX;
        vector.toY = y + vectorY;
        vector.radius = std::hypot(coefficient.real, coefficient.imaginary);
        x += vectorX;
        y += vectorY;
    };
    append(0);
    for (int harmonic = 1; harmonic <= fourierTerms; ++harmonic) {
        append(harmonic);
        append(-harmonic);
    }
    count = output;
    return true;
}

bool geometryCurveEventPositions(const GeometryCurve curve,
                                 const bool turningPoints,
                                 const int fourierTerms,
                                 double* positions,
                                 const std::size_t capacity,
                                 std::size_t& count,
                                 const int billiardPattern,
                                 const GeometryLissajousParameters lissajous) noexcept {
    constexpr std::size_t sampleCount = 4096;
    count = 0;
    if (positions == nullptr || capacity == 0 || fourierTerms < 1
        || fourierTerms > kMaximumGeometryFourierTerms)
        return false;

    if (curve == GeometryCurve::billiardSquare) {
        if (billiardPattern < 0
            || billiardPattern >= static_cast<int>(kBilliardPatterns.size()))
            return false;
        const auto laps = billiardLaps(billiardPattern);
        constexpr double width = 2.0 * kBilliardHalf;
        std::array<double, 12> collisions{};
        std::size_t collisionCount = 0;
        const double deltaX = laps.x * width;
        const double deltaY = laps.y * width;
        for (int hit = 0; hit < laps.x; ++hit)
            collisions[collisionCount++] =
                (kBilliardHalf - kBilliardStartX + hit * width)
                / deltaX;
        for (int hit = 0; hit < laps.y; ++hit)
            collisions[collisionCount++] =
                (kBilliardHalf - kBilliardStartY + hit * width)
                / deltaY;
        std::sort(collisions.begin(), collisions.begin() + collisionCount);
        if (collisionCount > capacity)
            return false;
        std::copy_n(collisions.begin(), collisionCount, positions);
        count = collisionCount;
        return true;
    }

    std::array<double, sampleCount + 1> values{};
    for (std::size_t sample = 0; sample <= sampleCount; ++sample) {
        Point2D point;
        if (!geometryCurvePoint(curve,
                static_cast<double>(sample) / static_cast<double>(sampleCount),
                1.0, 0.0, fourierTerms, point, billiardPattern, lissajous))
            return false;
        values[sample] = point.y;
    }

    const auto append = [&](double position) {
        position -= std::floor(position);
        if (position >= 1.0 - 1.0e-10 || position < 1.0e-10)
            position = 0.0;
        if (count >= capacity)
            return false;
        positions[count++] = position;
        return true;
    };

    if (turningPoints) {
        std::array<double, sampleCount> slopes{};
        for (std::size_t sample = 0; sample < sampleCount; ++sample)
            slopes[sample] = values[sample + 1] - values[sample];

        std::size_t first = sampleCount;
        for (std::size_t i = 0; i < sampleCount; ++i)
            if (std::abs(slopes[i]) > 1.0e-12) {
                first = i;
                break;
            }
        if (first == sampleCount)
            return true;

        std::size_t previousIndex = first;
        double previousSlope = slopes[first];
        for (std::size_t offset = 1; offset <= sampleCount; ++offset) {
            const std::size_t index = (first + offset) % sampleCount;
            const double slope = slopes[index];
            if (std::abs(slope) <= 1.0e-12)
                continue;
            if ((previousSlope > 0.0 && slope < 0.0)
                || (previousSlope < 0.0 && slope > 0.0)) {
                double endPoint = static_cast<double>(index);
                double startPoint = static_cast<double>(previousIndex + 1);
                if (index <= previousIndex)
                    endPoint += static_cast<double>(sampleCount);
                if (startPoint >= static_cast<double>(sampleCount))
                    startPoint -= static_cast<double>(sampleCount);
                if (!append((0.5 * (startPoint + endPoint))
                            / static_cast<double>(sampleCount)))
                    return false;
            }
            previousIndex = index;
            previousSlope = slope;
        }
    } else {
        constexpr double zeroTolerance = 1.0e-10;
        for (std::size_t sample = 0; sample < sampleCount; ++sample) {
            const double a = values[sample];
            const double b = values[sample + 1];
            if (std::abs(a) <= zeroTolerance) {
                const double before = values[sample == 0 ? sampleCount - 1 : sample - 1];
                if (before * b < 0.0
                    && !append(static_cast<double>(sample) / sampleCount))
                    return false;
            } else if (a * b < 0.0) {
                const double fraction = std::abs(a) / (std::abs(a) + std::abs(b));
                if (!append((static_cast<double>(sample) + fraction)
                            / sampleCount))
                    return false;
            }
        }
    }
    std::sort(positions, positions + count);
    std::size_t uniqueCount = 0;
    for (std::size_t i = 0; i < count; ++i) {
        if (uniqueCount == 0
            || positions[i] - positions[uniqueCount - 1] > 1.5 / sampleCount)
            positions[uniqueCount++] = positions[i];
    }
    if (uniqueCount > 1
        && positions[0] + 1.0 - positions[uniqueCount - 1]
            <= 1.5 / sampleCount)
        --uniqueCount;
    count = uniqueCount;
    return true;
}

} // namespace batruc
