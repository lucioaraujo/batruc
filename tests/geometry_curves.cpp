// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/GeometryCurves.hpp"
#include "core/CompositionCore.hpp"

#include <algorithm>
#include <cmath>

using namespace batruc;

namespace {

bool near(const double a, const double b, const double tolerance = 1.0e-8) {
    return std::abs(a - b) <= tolerance;
}

} // namespace

int main() {
    Point2D point;
    if (!geometryCurvePoint(GeometryCurve::compass, 0.0, 1.0, 0.0, 8, point)
        || !near(point.x, 1.0) || !near(point.y, 0.0))
        return 1;

    if (!geometryCurvePoint(GeometryCurve::triangle, 0.0, 1.0, 0.0, 8, point)
        || !near(point.x, -0.9) || !near(point.y, -0.62))
        return 2;
    if (!geometryCurvePoint(GeometryCurve::square, 0.25, 1.0, 0.0, 8, point)
        || !near(point.x, 0.78) || !near(point.y, -0.78))
        return 3;
    Point2D billiardStart;
    Point2D billiardEnd;
    if (!geometryCurvePoint(GeometryCurve::billiardSquare, 0.0, 1.0, 0.0,
                            8, billiardStart)
        || !geometryCurvePoint(GeometryCurve::billiardSquare, 1.0, 1.0, 0.0,
                               8, billiardEnd)
        || !near(billiardStart.x, billiardEnd.x)
        || !near(billiardStart.y, billiardEnd.y))
        return 18;

    Point2D lowOrder;
    Point2D highOrder;
    double lowError = 0.0;
    double highError = 0.0;
    if (!geometryCurvePoint(GeometryCurve::fourierTriangle, 0.17, 1.0, 0.0,
                           2, lowOrder)
        || !geometryCurvePoint(GeometryCurve::fourierTriangle, 0.17, 1.0, 0.0,
                               24, highOrder)
        || !geometryCurveFourierError(GeometryCurve::fourierTriangle, 2, lowError)
        || !geometryCurveFourierError(GeometryCurve::fourierTriangle, 24, highError)
        || !(highError < lowError))
        return 4;

    if (geometryCurvePoint(GeometryCurve::fourierSquare, 0.0, 1.0, 0.0, 33, point)
        || geometryCurveFourierError(GeometryCurve::compass, 8, highError)
        || geometryCurvePoint(GeometryCurve::compass, 0.0, 0.0, 0.0, 8, point))
        return 5;

    std::array<double, 128> eventPositions{};
    std::size_t eventCount = 0;
    if (!geometryCurveEventPositions(GeometryCurve::compass, false, 8,
                                     eventPositions.data(), eventPositions.size(),
                                     eventCount)
        || eventCount != 2 || !near(eventPositions[0], 0.0, 1.0e-3)
        || !near(eventPositions[1], 0.5, 1.0e-3))
        return 6;
    if (!geometryCurveEventPositions(GeometryCurve::billiardSquare, false, 8,
                                     eventPositions.data(), eventPositions.size(),
                                     eventCount)
        || eventCount != 6
        || !std::is_sorted(eventPositions.begin(), eventPositions.begin() + eventCount))
        return 19;
    constexpr std::array<double, 6> expectedBilliardHits = {
        0.1939102564102564, 0.3493589743589744, 0.4439102564102564,
        0.6939102564102564, 0.8493589743589745, 0.9439102564102563};
    for (std::size_t hit = 0; hit < expectedBilliardHits.size(); ++hit)
        if (!near(eventPositions[hit], expectedBilliardHits[hit]))
            return 20;
    constexpr std::array<std::size_t, 7> billiardHitCounts = {6, 6, 8, 8, 8, 10, 10};
    for (int pattern = 0; pattern < 7; ++pattern) {
        Point2D start;
        Point2D end;
        if (!geometryCurvePoint(GeometryCurve::billiardSquare, 0.0, 1.0,
                                0.0, 8, start, pattern)
            || !geometryCurvePoint(GeometryCurve::billiardSquare, 1.0, 1.0,
                                   0.0, 8, end, pattern)
            || !near(start.x, end.x) || !near(start.y, end.y)
            || !geometryCurveEventPositions(GeometryCurve::billiardSquare, false,
                8, eventPositions.data(), eventPositions.size(), eventCount,
                pattern)
            || eventCount != billiardHitCounts[static_cast<std::size_t>(pattern)])
            return 23;
    }
    if (!geometryCurveEventPositions(GeometryCurve::compass, true, 8,
                                     eventPositions.data(), eventPositions.size(),
                                     eventCount)
        || eventCount != 2 || !near(eventPositions[0], 0.25, 1.0e-3)
        || !near(eventPositions[1], 0.75, 1.0e-3))
        return 7;
    if (!geometryCurveEventPositions(GeometryCurve::lissajous, false, 8,
                                     eventPositions.data(), eventPositions.size(),
                                     eventCount)
        || eventCount != 4 || !near(eventPositions[0], 0.0, 1.0e-3)
        || !near(eventPositions[1], 0.25, 1.0e-3)
        || !near(eventPositions[2], 0.5, 1.0e-3)
        || !near(eventPositions[3], 0.75, 1.0e-3))
        return 8;
    if (!geometryCurveEventPositions(GeometryCurve::lissajous, true, 8,
                                     eventPositions.data(), eventPositions.size(),
                                     eventCount)
        || eventCount != 4)
        return 9;
    if (!geometryCurveEventPositions(GeometryCurve::triangle, false, 8,
                                     eventPositions.data(), eventPositions.size(),
                                     eventCount)
        || eventCount != 2
        || !near(eventPositions[0], 1.0 / 3.0 + 0.62 / 1.56 / 3.0, 1.0e-3)
        || !near(eventPositions[1], 2.0 / 3.0 + 0.94 / 1.56 / 3.0, 1.0e-3))
        return 10;
    if (geometryCurveEventPositions(GeometryCurve::lissajous, false, 8,
                                    eventPositions.data(), 1, eventCount)
        || geometryCurveEventPositions(GeometryCurve::compass, false, 8,
                                    nullptr, eventPositions.size(), eventCount))
        return 11;

    auto recipe = makeInitialRecipe();
    recipe.sectionSeconds = 2.0;
    recipe.voices[0].cyclesPerSection = 2.0;
    EventPlan plan;
    if (buildEventPlan(recipe, plan) != PlanError::none)
        return 12;
    ResonatorRenderer plain;
    ResonatorRenderer geometric;
    if (!plain.configure(recipe, plan, 8000.0)
        || !geometric.configure(recipe, plan, 8000.0))
        return 13;
    geometric.setGeometryVoiceOneSource(true, GeometryCurve::fourierTriangle, 12);
    std::array<float, 16000> plainLeft{};
    std::array<float, 16000> plainRight{};
    std::array<float, 16000> geometryLeft{};
    std::array<float, 16000> geometryRight{};
    plain.process(plainLeft.data(), plainRight.data(), plainLeft.size());
    geometric.process(geometryLeft.data(), geometryRight.data(), geometryLeft.size());
    double difference = 0.0;
    for (std::size_t i = 0; i < plainLeft.size(); ++i)
        difference += std::abs(static_cast<double>(plainLeft[i]) - geometryLeft[i])
            + std::abs(static_cast<double>(plainRight[i]) - geometryRight[i]);
    if (!(difference > 1.0e-4))
        return 14;

    const auto renderMaterial = [&](const SoundCharacter material,
                                    auto& left, auto& right) {
        ResonatorRenderer renderer;
        if (!renderer.configure(recipe, plan, 8000.0))
            return false;
        renderer.setChannelSound(0, material);
        renderer.process(left.data(), right.data(), left.size());
        return true;
    };
    std::array<float, 16000> bellLeft{};
    std::array<float, 16000> bellRight{};
    std::array<float, 16000> membraneLeft{};
    std::array<float, 16000> membraneRight{};
    if (!renderMaterial(SoundCharacter::bell, bellLeft, bellRight)
        || !renderMaterial(SoundCharacter::membrane,
                           membraneLeft, membraneRight))
        return 26;
    double bellDifference = 0.0;
    double membraneDifference = 0.0;
    double materialDifference = 0.0;
    for (std::size_t i = 0; i < plainLeft.size(); ++i) {
        bellDifference += std::abs(static_cast<double>(bellLeft[i]) - plainLeft[i])
            + std::abs(static_cast<double>(bellRight[i]) - plainRight[i]);
        membraneDifference += std::abs(
            static_cast<double>(membraneLeft[i]) - plainLeft[i])
            + std::abs(static_cast<double>(membraneRight[i]) - plainRight[i]);
        materialDifference += std::abs(
            static_cast<double>(bellLeft[i]) - membraneLeft[i])
            + std::abs(static_cast<double>(bellRight[i]) - membraneRight[i]);
    }
    if (bellDifference < 1.0e-3 || membraneDifference < 1.0e-3
        || materialDifference < 1.0e-3)
        return 27;

    constexpr std::array<SoundCharacter, 7> percussionSounds = {
        SoundCharacter::kick, SoundCharacter::snare, SoundCharacter::hihat,
        SoundCharacter::ride, SoundCharacter::conga, SoundCharacter::brushes,
        SoundCharacter::chip8};
    std::array<std::array<float, 16000>, percussionSounds.size()> percussionLeft{};
    std::array<std::array<float, 16000>, percussionSounds.size()> percussionRight{};
    for (std::size_t sound = 0; sound < percussionSounds.size(); ++sound) {
        if (!renderMaterial(percussionSounds[sound], percussionLeft[sound],
                            percussionRight[sound]))
            return 28;
        double energy = 0.0;
        double differenceFromTriangle = 0.0;
        double peak = 0.0;
        for (std::size_t frame = 0; frame < percussionLeft[sound].size(); ++frame) {
            const double leftSample = percussionLeft[sound][frame];
            const double rightSample = percussionRight[sound][frame];
            if (!std::isfinite(leftSample) || !std::isfinite(rightSample))
                return 29;
            energy += std::abs(leftSample) + std::abs(rightSample);
            peak = std::max(peak, std::max(std::abs(leftSample), std::abs(rightSample)));
            differenceFromTriangle += std::abs(leftSample - plainLeft[frame])
                + std::abs(rightSample - plainRight[frame]);
        }
        if (energy < 1.0e-3 || differenceFromTriangle < 1.0e-3 || peak > 0.9)
            return 30;
        for (std::size_t previous = 0; previous < sound; ++previous) {
            double pairDifference = 0.0;
            for (std::size_t frame = 0; frame < percussionLeft[sound].size(); ++frame)
                pairDifference += std::abs(percussionLeft[sound][frame]
                                               - percussionLeft[previous][frame])
                    + std::abs(percussionRight[sound][frame]
                                   - percussionRight[previous][frame]);
            if (pairDifference < 1.0e-3)
                return 31;
        }
    }

    ResonatorRenderer alteredChipRenderer;
    if (!alteredChipRenderer.configure(recipe, plan, 8000.0))
        return 32;
    alteredChipRenderer.setChannelSound(0, SoundCharacter::chip8);
    alteredChipRenderer.setChannelChip8Parameters(
        0, Chip8Parameters{0.25f, 1000.0f, 3, 0.80f});
    std::array<float, 16000> alteredChipLeft{};
    std::array<float, 16000> alteredChipRight{};
    alteredChipRenderer.process(alteredChipLeft.data(), alteredChipRight.data(),
                                alteredChipLeft.size());
    double chipParameterDifference = 0.0;
    for (std::size_t frame = 0; frame < alteredChipLeft.size(); ++frame) {
        if (!std::isfinite(alteredChipLeft[frame])
            || !std::isfinite(alteredChipRight[frame]))
            return 33;
        chipParameterDifference += std::abs(alteredChipLeft[frame]
            - percussionLeft.back()[frame]) + std::abs(alteredChipRight[frame]
            - percussionRight.back()[frame]);
    }
    if (chipParameterDifference < 1.0e-3)
        return 34;

    ResonatorRenderer billiardRenderer;
    if (!billiardRenderer.configure(recipe, plan, 8000.0))
        return 21;
    billiardRenderer.setGeometryVoiceOneSource(
        true, GeometryCurve::billiardSquare, 8);
    std::array<float, 16000> billiardLeft{};
    std::array<float, 16000> billiardRight{};
    billiardRenderer.process(billiardLeft.data(), billiardRight.data(),
                             billiardLeft.size());
    double billiardDifference = 0.0;
    for (std::size_t i = 0; i < billiardLeft.size(); ++i)
        billiardDifference += std::abs(static_cast<double>(billiardLeft[i])
                                       - geometryLeft[i])
            + std::abs(static_cast<double>(billiardRight[i]) - geometryRight[i]);
    if (!(billiardDifference > 1.0e-3))
        return 22;
    ResonatorRenderer alternateBilliard;
    if (!alternateBilliard.configure(recipe, plan, 8000.0))
        return 24;
    alternateBilliard.setGeometryVoiceOneSource(
        true, GeometryCurve::billiardSquare, 8, 5);
    std::array<float, 16000> alternateLeft{};
    std::array<float, 16000> alternateRight{};
    alternateBilliard.process(alternateLeft.data(), alternateRight.data(),
                              alternateLeft.size());
    double pathDifference = 0.0;
    for (std::size_t i = 0; i < alternateLeft.size(); ++i)
        pathDifference += std::abs(static_cast<double>(alternateLeft[i])
                                   - billiardLeft[i])
            + std::abs(static_cast<double>(alternateRight[i]) - billiardRight[i]);
    if (!(pathDifference > 1.0e-3))
        return 25;

    recipe.voices[0].cyclesPerSection = 1.0;
    recipe.voices[1].cyclesPerSection = 1.0;
    if (buildEventPlan(recipe, plan) != PlanError::none)
        return 15;
    ResonatorRenderer geometricScheduler;
    if (!geometricScheduler.configure(recipe, plan, 8000.0))
        return 16;
    geometricScheduler.setGeometryVoiceOneSource(
        true, GeometryCurve::triangle, 8);
    geometricScheduler.selectVariantWhilePaused(0);
    std::array<float, 16000> scheduledLeft{};
    std::array<float, 16000> scheduledRight{};
    const std::array<float, kMaxVoices> levels = {1.0f, 0.0f};
    geometricScheduler.processVariantAtNextBar(
        scheduledLeft.data(), scheduledRight.data(), scheduledLeft.size(),
        0, levels.data(), scheduledLeft.size());
    std::size_t firstAttack = scheduledLeft.size();
    for (std::size_t i = 0; i < scheduledLeft.size(); ++i)
        if (std::abs(scheduledLeft[i]) + std::abs(scheduledRight[i]) > 1.0e-8f) {
            firstAttack = i;
            break;
        }
    if (firstAttack < 7000 || firstAttack > 8000)
        return 17;

    return 0;
}
