// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#pragma once

#include "core/TriangleGeometry.hpp"

#include <cstddef>

namespace batruc {

enum class GeometryCurve {
    compass,
    triangle,
    square,
    spiral,
    lissajous,
    fourierTriangle,
    fourierSquare,
    epicycloid,
    hypocycloid,
    cycloid,
    catenary,
    billiardSquare,
    cardioid,
    ranunculoid,
    astroid,
    curtateTrochoid,
    prolateTrochoid
};

enum class GeometryRollingSupport { line, outsideCircle, insideCircle };

struct GeometryRollingState {
    Point2D circleCentre{};
    Point2D pen{};
    double rollingRadius = 0.0;
    double fixedRadius = 0.0;
    GeometryRollingSupport support = GeometryRollingSupport::line;
};

struct GeometryLissajousParameters {
    int xFrequency = 3;
    int yFrequency = 2;
    double yPhaseTurns = 0.0;
    double xAmplitude = 1.0;
    double yAmplitude = 1.0;
};

constexpr int kMaximumGeometryFourierTerms = 32;

struct GeometryFourierVector {
    int harmonic = 0;
    double fromX = 0.0;
    double fromY = 0.0;
    double toX = 0.0;
    double toY = 0.0;
    double radius = 0.0;
};

struct GeometryPolygonMetrics {
    double area = 0.0;
    double perimeter = 0.0;
};

// Normalized parametric curves used by the web study. Invalid inputs return
// false; coordinates are bounded to approximately [-1, 1].
bool geometryCurvePoint(GeometryCurve curve, double progress, double cycles,
                        double phaseTurns, int fourierTerms,
                        Point2D& point, int billiardPattern = 0,
                        GeometryLissajousParameters lissajous = {}) noexcept;

// Kinematics for a no-slip circle roulette. The returned pen and circle
// centre share the same normalized coordinates used by geometryCurvePoint.
bool geometryCurveRollingState(GeometryCurve curve, double progress,
                               double cycles, double phaseTurns,
                               GeometryRollingState& state) noexcept;

// Normalized shoelace area and closed-path perimeter for the built-in
// triangle and square. Other curves return false.
bool geometryCurvePolygonMetrics(GeometryCurve curve,
                                 GeometryPolygonMetrics& metrics) noexcept;

// Relative RMS error in percent against the polygonal reference path.
bool geometryCurveFourierError(GeometryCurve curve, int fourierTerms,
                               double& errorPercent) noexcept;

// Returns the rotating-vector chain in [0, +1, -1, ...] order without
// allocating. Only Fourier triangle and square curves have epicycles.
bool geometryCurveFourierEpicycles(GeometryCurve curve, double progress,
                                   double cycles, double phaseTurns,
                                   int fourierTerms,
                                   GeometryFourierVector* vectors,
                                   std::size_t capacity,
                                   std::size_t& count) noexcept;

// Finds normalized section positions of y(t)=0 crossings or local extrema by
// sampling the declared path. Values are sorted in [0,1); no heap is used.
// Returns false for invalid inputs or when the caller's buffer is too small.
bool geometryCurveEventPositions(GeometryCurve curve, bool turningPoints,
                                 int fourierTerms, double* positions,
                                 std::size_t capacity, std::size_t& count,
                                 int billiardPattern = 0,
                                 GeometryLissajousParameters lissajous = {}) noexcept;

} // namespace batruc
