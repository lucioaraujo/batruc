// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/CompositionCore.hpp"
#include "core/PassageDevelopment.hpp"
#include "core/BassComposition.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

namespace batruc {
namespace {

constexpr double kEpsilon = 1.0e-12;
constexpr std::uint64_t kSectionPhaseScale = 1ull << 32u;
constexpr double kMaximumSectionSeconds = 84.0;
constexpr double kMaximumCyclesPerSection = 512.0;
constexpr double kMinimumResonanceHz = 20.0;
constexpr double kMaximumResonanceHz = 12'000.0;
constexpr std::size_t kMaximumPendulumSamples = 250'000;

bool finite(double value) noexcept { return std::isfinite(value); }
bool finite(float value) noexcept { return std::isfinite(value); }

bool isPercussive(const SoundCharacter sound) noexcept {
    switch (sound) {
        case SoundCharacter::kick:
        case SoundCharacter::snare:
        case SoundCharacter::hihat:
        case SoundCharacter::openHihat:
        case SoundCharacter::ride:
        case SoundCharacter::conga:
        case SoundCharacter::brushes:
        case SoundCharacter::chip8:
        case SoundCharacter::rimClick:
        case SoundCharacter::shaker:
        case SoundCharacter::agogo:
        case SoundCharacter::woodblock:
            return true;
        case SoundCharacter::sine:
        case SoundCharacter::triangle:
        case SoundCharacter::sawtooth:
        case SoundCharacter::square:
        case SoundCharacter::bell:
        case SoundCharacter::membrane:
            return false;
    }
    return false;
}

SoundCharacter soundForGeneralMidiDrum(const std::uint8_t note,
                                       const SoundCharacter fallback) noexcept {
    switch (note) {
        case 35: case 36: return SoundCharacter::kick;
        case 37: return SoundCharacter::rimClick; // side stick / rim click
        case 38: case 39: case 40: return SoundCharacter::snare;
        case 22: case 42: case 44:
            return SoundCharacter::hihat;
        case 26: case 46: return SoundCharacter::openHihat;
        case 41: case 43: case 45: case 47: case 48: case 50:
            return SoundCharacter::membrane;
        case 49: case 51: case 52: case 53: case 55: case 57: case 59:
            return SoundCharacter::ride;
        case 54: case 69: case 70: return SoundCharacter::shaker;
        case 56: return SoundCharacter::bell;
        case 58: case 73: case 74: return SoundCharacter::brushes;
        case 60: case 61: case 62: case 63: case 64:
            return SoundCharacter::conga;
        case 65: case 66:
            return SoundCharacter::membrane;
        case 67: case 68: return SoundCharacter::agogo;
        case 75: case 76: case 77: return SoundCharacter::woodblock;
        case 71: case 72: return SoundCharacter::sine;
        default:
            return fallback;
    }
}

float noiseColourForSound(const SoundCharacter sound) noexcept {
    switch (sound) {
        case SoundCharacter::sine: return 0.0f;
        case SoundCharacter::triangle: return 0.10f;
        case SoundCharacter::sawtooth: return 0.30f;
        case SoundCharacter::square: return 0.15f;
        case SoundCharacter::bell: return 0.20f;
        case SoundCharacter::membrane: return 0.42f;
        case SoundCharacter::kick: return 0.12f;
        case SoundCharacter::snare: return 0.92f;
        case SoundCharacter::hihat: return 1.0f;
        case SoundCharacter::openHihat: return 0.88f;
        case SoundCharacter::rimClick: return 0.10f;
        case SoundCharacter::ride: return 0.68f;
        case SoundCharacter::conga: return 0.38f;
        case SoundCharacter::brushes: return 0.52f;
        case SoundCharacter::chip8: return 0.34f;
        case SoundCharacter::shaker: return 1.0f;
        case SoundCharacter::agogo: return 0.18f;
        case SoundCharacter::woodblock: return 0.08f;
    }
    return 0.42f;
}

SoundCharacter defaultMidiLaneSound(const std::size_t voice) noexcept {
    return voice < kMidiLaneDefaultSounds.size()
        ? kMidiLaneDefaultSounds[voice] : SoundCharacter::triangle;
}

double generalMidiDrumDecaySeconds(const std::uint8_t note) noexcept {
    switch (note) {
        case 35: case 36: return 0.13;
        case 37: return 0.08;
        case 38: case 40: return 0.16;
        case 39: return 0.12;
        case 22: case 42: return 0.045;
        case 44: return 0.065;
        case 26: case 46: return 0.19;
        case 41: return 0.28;
        case 43: return 0.25;
        case 45: return 0.22;
        case 47: return 0.20;
        case 48: return 0.18;
        case 50: return 0.16;
        case 49: case 57: return 0.30;
        case 51: case 59: return 0.22;
        case 52: return 0.24;
        case 53: return 0.16;
        case 55: return 0.22;
        case 54: case 56: case 58: return 0.12;
        case 60: case 61: return 0.12;
        case 62: return 0.07;
        case 63: case 64: return 0.16;
        case 65: case 66: return 0.18;
        case 67: case 68: return 0.22;
        case 69: case 70: return 0.06;
        case 71: return 0.10;
        case 72: return 0.32;
        case 73: return 0.08;
        case 74: return 0.25;
        case 75: return 0.045;
        case 76: case 77: return 0.09;
        default:
            return note >= 60u && note <= 77u ? 0.16 : 0.18;
    }
}

std::pair<double, double> formalPhaseArc(const std::size_t cycle,
                                         const double amount) noexcept {
    const auto segment = formalPhaseSegmentForCycle(cycle, amount);
    return {segment.offsetCycles, segment.slopeCycles};
}

double effectiveVoiceCycles(const std::array<double, kMaxVoices>& cycles,
                            const std::array<double, kMaxVoices>& slopes,
                            const std::size_t voice) noexcept {
    return cycles[voice] + slopes[voice];
}

double wrapUnitCycle(double value) noexcept {
    return value - std::floor(value);
}

bool allowedByPattern(const PatternMode mode,
                      const double x,
                      const double y) noexcept {
    switch (mode) {
        case PatternMode::unrestricted:
            return true;
        case PatternMode::adhesivePads: {
            constexpr std::array<std::array<double, 2>, 4> centres = {{
                {{0.0, -0.58}}, {{0.0, 0.58}}, {{-0.58, 0.0}}, {{0.58, 0.0}}
            }};
            constexpr double radiusSquared = 0.43 * 0.43;
            for (const auto& centre : centres) {
                const double dx = x - centre[0];
                const double dy = y - centre[1];
                if (dx * dx + dy * dy <= radiusSquared)
                    return true;
            }
            return false;
        }
        case PatternMode::diagonalLanes:
            return std::abs(y - 0.55 * x) <= 0.16
                || std::abs(y + 0.55 * x) <= 0.16;
    }
    return false;
}

double distanceToPattern(const PatternMode mode,
                         const double x, const double y) noexcept {
    if (mode == PatternMode::diagonalLanes)
        return std::min(std::abs(y - 0.55 * x), std::abs(y + 0.55 * x));
    if (mode == PatternMode::adhesivePads) {
        constexpr std::array<std::array<double, 2>, 4> centres = {{
            {{0.0, -0.58}}, {{0.0, 0.58}}, {{-0.58, 0.0}}, {{0.58, 0.0}}
        }};
        double nearest = std::numeric_limits<double>::infinity();
        for (const auto& centre : centres) {
            const double dx = x - centre[0];
            const double dy = y - centre[1];
            nearest = std::min(nearest, dx * dx + dy * dy);
        }
        return nearest;
    }
    return 0.0;
}

bool validVoice(const VoiceRecipe& voice) noexcept {
    if (!finite(voice.cyclesPerSection) || voice.cyclesPerSection <= 0.0
        || voice.cyclesPerSection > kMaximumCyclesPerSection
        || !finite(voice.phaseCycles)
        || !finite(voice.resonanceHz)
        || voice.resonanceHz < kMinimumResonanceHz
        || voice.resonanceHz > kMaximumResonanceHz
        || !finite(voice.gain) || voice.gain < 0.0f || voice.gain > 1.0f
        || !finite(voice.pan) || voice.pan < -1.0f || voice.pan > 1.0f
        || !finite(voice.resonator.decaySeconds)
        || voice.resonator.decaySeconds < 0.005
        || voice.resonator.decaySeconds > 20.0
        || !finite(voice.resonator.frequencyDependentLoss)
        || voice.resonator.frequencyDependentLoss < 0.0
        || voice.resonator.frequencyDependentLoss > 1.0
        || !finite(voice.resonator.strikeNoise)
        || voice.resonator.strikeNoise < 0.0f
        || voice.resonator.strikeNoise > 1.0f)
        return false;

    for (std::size_t mode = 0; mode < kModesPerVoice; ++mode) {
        const double ratio = voice.resonator.frequencyRatios[mode];
        const float weight = voice.resonator.modeWeights[mode];
        if (!finite(ratio) || ratio <= 0.0 || ratio > 64.0
            || !finite(weight) || weight < 0.0f || weight > 1.0f
            || voice.resonanceHz * ratio > 20'000.0)
            return false;
    }
    return true;
}

double swungProgress(const double progress, const double ratio,
                     const double quarterNotesPerSection) noexcept {
    return swingProgress(progress, ratio, quarterNotesPerSection);
}

double straightProgress(const double progress, const double ratio,
                        const double quarterNotesPerSection) noexcept {
    const double bounded = std::clamp(progress, 0.0, 1.0);
    const double beatPosition = bounded * quarterNotesPerSection;
    const double within = beatPosition - std::floor(beatPosition);
    const double inverseWithin = within <= ratio
        ? within / (2.0 * ratio)
        : 0.5 + (within - ratio) / (2.0 * (1.0 - ratio));
    return (std::floor(beatPosition) + inverseWithin)
        / quarterNotesPerSection;
}

} // namespace

std::array<SoundCharacter, kMaxActiveVoices> soundFamilyVoiceSounds(
        const int familyIndex) noexcept {
    using S = SoundCharacter;
    if (familyIndex < 0 || familyIndex >= 17)
        return {S::triangle, S::triangle, S::triangle, S::triangle,
                S::triangle, S::triangle, S::triangle, S::triangle};
    // The eight mixer lanes have stable kit roles in every style. A family
    // colors the synthesis profile; it must not turn V1 into a conga, bell,
    // or oscillator and make the stems appear inverted.
    return kMidiLaneDefaultSounds;
}

PatternFieldPose patternFieldPose(const PatternMode mode, double progress,
                                 const std::uint32_t seed) noexcept {
    if (!std::isfinite(progress))
        progress = 0.0;
    progress = std::remainder(progress, 4096.0);
    const double phase = kTau * static_cast<double>(seed & 0xffffu) / 65536.0;
    // Let a pattern breathe over several bars instead of cycling like an LFO
    // locked to each phrase. Seed changes the drift speed without creating
    // abrupt jumps in pose or musical gating.
    const double rate = 0.25
        + 0.125 * static_cast<double>((seed >> 16u) % 4u);
    const double theta = kTau * rate * progress + phase;
    PatternFieldPose pose;
    if (mode == PatternMode::unrestricted) {
        pose.focusX = 0.62 * std::sin(theta * 0.67 + phase * 0.23);
        pose.focusY = 0.62 * std::cos(theta * 0.83 + phase * 0.31);
        pose.focusRadius = 0.30 + 0.08 * (0.5 + 0.5 * std::sin(theta * 0.43));
        return pose;
    }
    pose.offsetX = 0.12 * std::sin(theta);
    pose.offsetY = 0.12 * std::sin(theta * 0.73 + phase * 0.37);
    pose.rotationRadians = 0.16 * std::sin(theta * 0.5 + phase * 0.19);
    pose.halfWidth = 0.13 + 0.035 * (0.5 + 0.5 * std::sin(theta + phase));
    constexpr double goldenAngle = 2.39996322972865332;
    for (std::size_t organism = 0; organism < pose.padRadii.size(); ++organism) {
        const double organismPhase = phase + goldenAngle * organism;
        const double neighbourPhase = phase + goldenAngle * ((organism + 1) % 4);
        // Four coupled, seeded organisms pulse at different phases. The weak
        // neighbour term lets a pulse travel through the colony without
        // random state or discontinuities at section boundaries.
        pose.padRadii[organism] = 0.38
            + 0.027 * std::sin(theta * 0.61 + organismPhase)
            + 0.013 * std::sin(theta * 0.19 + neighbourPhase);
        pose.padShapePhases[organism] = 0.34
            * std::sin(theta * 0.17 + organismPhase * 0.11);
        pose.padMorphologies[organism] = static_cast<std::uint8_t>(
            ((seed >> (organism * 4u)) & 0xffu) % 28u);
        pose.padCentreOffsets[organism] = {
            0.026 * std::sin(theta * 0.31 + organismPhase)
                + 0.010 * std::sin(theta * 0.13 + neighbourPhase),
            0.026 * std::sin(theta * 0.23 + organismPhase + 1.1)
                + 0.010 * std::sin(theta * 0.11 + neighbourPhase + 1.1)};
    }
    pose.padEdgeX = 0.065 + 0.018 * std::sin(theta * 0.37 + phase * 0.71);
    pose.padEdgeXY = 0.052 * std::cos(theta * 0.29 + phase * 0.43);
    pose.padEdgeCos3 = 0.022 * std::sin(theta * 0.23 + phase * 0.53);
    pose.padEdgeSin3 = 0.017 * std::cos(theta * 0.17 - phase * 0.29);
    pose.laneBow = 0.075 * std::sin(theta * 0.31 + phase * 0.83);
    pose.laneWaveA = 0.026 * std::sin(theta * 0.27 + phase * 0.59);
    pose.laneWaveB = 0.015 * std::cos(theta * 0.19 + phase * 0.41);
    pose.laneCentreOffsets = {
        0.024 * std::sin(theta * 0.23 + phase * 0.41),
        0.024 * std::sin(theta * 0.23 + phase * 0.41 + 2.17)};
    pose.laneHalfWidths = {
        pose.halfWidth + 0.008 * std::sin(theta * 0.29 + phase),
        pose.halfWidth + 0.008 * std::sin(theta * 0.29 + phase + 2.17)};
    return pose;
}

double patternFieldPadBoundary(const PatternFieldPose& pose,
                               const double angle,
                               const std::size_t organism) noexcept {
    if (!std::isfinite(angle))
        return 1.0;
    const auto index = std::min(organism, pose.padShapePhases.size() - 1);
    const double localAngle = angle + pose.padShapePhases[index];
    const double cosine = std::cos(localAngle);
    const double sine = std::sin(localAngle);
    const double cosine2 = std::cos(2.0 * localAngle);
    const double sine3 = std::sin(3.0 * localAngle);
    const double profile = [&]() noexcept {
        switch (pose.padMorphologies[index]) {
            case 1: // W / four-lobed contour
                return 0.055 * std::cos(4.0 * localAngle)
                    - 0.025 * cosine2;
            case 2: // Gaussian dome, made periodic on the angular domain
                return 0.10 * (std::exp(2.0 * (cosine - 1.0)) - 0.30);
            case 3: // Agnesi-inspired rational lobe, periodized
                return 0.105 * (1.0 / (3.0 - 2.0 * cosine) - 0.447213595499958);
            case 4: { // Softplus shoulder
                const double x = 2.0 * cosine;
                const double softplus = std::max(x, 0.0)
                    + std::log1p(std::exp(-std::abs(x)));
                return 0.045 * (softplus - 0.75);
            }
            case 5: { // GELU-like activation
                const double x = 1.6 * cosine;
                const double x3 = x * x * x;
                const double gelu = 0.5 * x * (1.0 + std::tanh(
                    0.7978845608028654 * (x + 0.044715 * x3)));
                return 0.055 * (gelu - 0.45);
            }
            case 6: // Absolute sine: intentional cusps with small ripples
                return 0.04 * (std::abs(std::sin(3.0 * localAngle))
                    - 0.6366197723675814)
                    + 0.02 * std::cos(8.0 * localAngle);
            case 7: // Close harmonics create angular beat-like undulations
                return 0.035 * (std::sin(5.0 * localAngle)
                    + std::sin(6.0 * localAngle));
            case 8: // Bounded tangent profile; avoids its singularity
                return 0.09 * std::tan(0.55 * cosine);
            case 9: // Periodic phase-modulated wave, chirp-inspired
                return 0.07 * std::sin(2.0 * localAngle
                    + 0.8 * std::sin(5.0 * localAngle));
            case 10: // Cubic wave profile
                return 0.08 * cosine2 * cosine2 * cosine2;
            case 11: // Logarithmic compression of the radial lobe
                return 0.065 * std::log(2.0 + cosine) - 0.065 * std::log(2.0);
            case 12: // Mirror/reflection symmetry with a folded contour
                return 0.08 * (std::abs(cosine2) - 0.6366197723675814);
            case 13: // Alternating convex and concave sectors
                return 0.065 * (std::cos(3.0 * localAngle)
                    + 0.35 * cosine2);
            case 14: // Anamorphic angular warp, still a closed radial curve
                return 0.065 * std::sin(2.0 * localAngle
                    + 0.32 * sine);
            case 15: // Rocket-like tapered polar silhouette
                return 0.045 * cosine + 0.03 * cosine2;
            case 16: // Sine/cosine interference, bounded as a radial profile
                return 0.045 * (std::sin(2.0 * localAngle)
                    + std::cos(3.0 * localAngle));
            case 17: // Arcsine of cosine: a folded, triangular angular contour
                return 0.075 * std::asin(cosine) / 3.14159265358979323846;
            case 18: // Arccosine of cosine: a reflected angular ramp
                return 0.075 * (std::acos(cosine) / 3.14159265358979323846 - 0.5);
            case 19: // Product of sine and cosine creates cross harmonics
                return 0.06 * std::sin(3.0 * localAngle)
                    * std::cos(2.0 * localAngle);
            case 20: // Cosine arch with a secondary harmonic
                return 0.055 * (cosine - 0.25 * cosine2);
            case 21: // Damped-looking rebound profile, periodized for a closed contour
                return 0.045 * (std::abs(std::sin(2.0 * localAngle))
                    - 0.6366197723675814)
                    + 0.02 * std::sin(5.0 * localAngle);
            case 22: // Parabolic arch
                return 0.06 * (cosine * cosine - 0.5);
            case 23: // Stronger bounded reflection/fold profile
                return 0.07 * (std::abs(cosine2) - 0.6366197723675814)
                    + 0.015 * std::sin(6.0 * localAngle);
            case 24: // Reciprocal curve, bounded away from its pole
                return 0.075 * (1.0 / (1.6 + cosine) - 0.8);
            case 25: // Base-10 logarithm of a positive periodic lobe
                return 0.075 * std::log10(2.0 + cosine) - 0.075 * std::log10(2.0);
            case 26: // Damped cosine with a periodic angular envelope
                return 0.07 * cosine
                    * std::exp(-0.65 * std::abs(std::sin(localAngle)));
            case 27: // Gently convex oval, kept away from inflection
                return 0.045 * cosine2;
            case 0:
            default:
                return pose.padEdgeX * cosine2
                    + pose.padEdgeXY * std::sin(2.0 * localAngle)
                    + pose.padEdgeCos3 * std::cos(3.0 * localAngle)
                    + pose.padEdgeSin3 * sine3;
        }
    }();
    return 1.0
        + profile;
}

double patternFieldPadRadius(const PatternFieldPose& pose,
                             const std::size_t organism) noexcept {
    return pose.padRadii[std::min(organism, pose.padRadii.size() - 1)];
}

PatternFieldBridge patternFieldPadBridge(const PatternFieldPose& pose,
                                         const std::size_t bridge) noexcept {
    constexpr std::array<Point2D, 4> centres = {{
        {0.0, -0.58}, {0.0, 0.58}, {-0.58, 0.0}, {0.58, 0.0}}};
    constexpr std::array<std::array<std::size_t, 2>, 4> neighbours = {{
        {{0, 2}}, {{2, 1}}, {{1, 3}}, {{3, 0}}}};
    const auto& pair = neighbours[std::min(bridge, neighbours.size() - 1)];
    const auto first = pair[0];
    const auto second = pair[1];
    PatternFieldBridge result;
    result.start = {centres[first].x + pose.padCentreOffsets[first].x,
                    centres[first].y + pose.padCentreOffsets[first].y};
    result.end = {centres[second].x + pose.padCentreOffsets[second].x,
                  centres[second].y + pose.padCentreOffsets[second].y};
    const double dx = result.end.x - result.start.x;
    const double dy = result.end.y - result.start.y;
    const double centreDistance = std::hypot(dx, dy);
    if (centreDistance <= kEpsilon)
        return result;
    const double firstAngle = std::atan2(dy, dx);
    const double secondAngle = firstAngle + kTau * 0.5;
    const double firstExtent = patternFieldPadRadius(pose, first)
        * patternFieldPadBoundary(pose, firstAngle, first);
    const double secondExtent = patternFieldPadRadius(pose, second)
        * patternFieldPadBoundary(pose, secondAngle, second);
    const double gap = centreDistance - firstExtent - secondExtent;
    result.radius = std::clamp(0.018 - 0.5 * gap, 0.0, 0.045);
    return result;
}

double patternFieldLaneDisplacement(const PatternFieldPose& pose,
                                    const double x) noexcept {
    if (!std::isfinite(x))
        return 0.0;
    const double boundedX = std::clamp(x, -1.0, 1.0);
    const double envelope = 1.0 - boundedX * boundedX;
    const double pi = 0.5 * kTau;
    return envelope * (pose.laneBow
        + pose.laneWaveA * std::sin(pi * boundedX)
        + pose.laneWaveB * std::sin(kTau * boundedX));
}

double patternFieldLaneSlope(const PatternFieldPose& pose,
                             const double x) noexcept {
    if (!std::isfinite(x))
        return 0.55;
    const double boundedX = std::clamp(x, -1.0, 1.0);
    const double envelope = 1.0 - boundedX * boundedX;
    const double pi = 0.5 * kTau;
    const double wave = pose.laneBow
        + pose.laneWaveA * std::sin(pi * boundedX)
        + pose.laneWaveB * std::sin(kTau * boundedX);
    const double waveSlope = pose.laneWaveA * pi * std::cos(pi * boundedX)
        + pose.laneWaveB * kTau * std::cos(kTau * boundedX);
    return 0.55 - 2.0 * boundedX * wave + envelope * waveSlope;
}

double patternFieldBoundaryDistance(const PatternMode mode, const double x,
                                    const double y, const double progress,
                                    const std::uint32_t seed) noexcept {
    if (!std::isfinite(x) || !std::isfinite(y))
        return std::numeric_limits<double>::infinity();
    if (mode == PatternMode::unrestricted)
        return -1.0;

    const auto pose = patternFieldPose(mode, progress, seed);
    const double dx = x - pose.offsetX;
    const double dy = y - pose.offsetY;
    const double cosine = std::cos(pose.rotationRadians);
    const double sine = std::sin(pose.rotationRadians);
    const double localX = cosine * dx + sine * dy;
    const double localY = -sine * dx + cosine * dy;
    if (mode == PatternMode::adhesivePads) {
        constexpr std::array<std::array<double, 2>, 4> centres = {{
            {{0.0, -0.58}}, {{0.0, 0.58}}, {{-0.58, 0.0}}, {{0.58, 0.0}}
        }};
        double nearest = std::numeric_limits<double>::infinity();
        for (std::size_t organism = 0; organism < centres.size(); ++organism) {
            const auto& centre = centres[organism];
            const double px = localX - centre[0]
                - pose.padCentreOffsets[organism].x;
            const double py = localY - centre[1]
                - pose.padCentreOffsets[organism].y;
            const double radius = std::hypot(px, py);
            const double boundary = patternFieldPadRadius(pose, organism)
                * patternFieldPadBoundary(pose, std::atan2(py, px), organism);
            nearest = std::min(nearest, radius - boundary);
        }
        for (std::size_t index = 0; index < 4; ++index) {
            const auto bridge = patternFieldPadBridge(pose, index);
            if (bridge.radius <= 0.0)
                continue;
            const double vx = bridge.end.x - bridge.start.x;
            const double vy = bridge.end.y - bridge.start.y;
            const double lengthSquared = vx * vx + vy * vy;
            const double amount = lengthSquared > kEpsilon
                ? std::clamp(((localX - bridge.start.x) * vx
                    + (localY - bridge.start.y) * vy) / lengthSquared, 0.0, 1.0)
                : 0.0;
            const double dxBridge = localX - (bridge.start.x + amount * vx);
            const double dyBridge = localY - (bridge.start.y + amount * vy);
            nearest = std::min(nearest,
                std::hypot(dxBridge, dyBridge) - bridge.radius);
        }
        return nearest;
    }

    const double displacement = patternFieldLaneDisplacement(pose, localX);
    const double upperSlope = patternFieldLaneSlope(pose, localX);
    const double upperCentre = 0.55 * localX + displacement
        + pose.laneCentreOffsets[0];
    const double lowerCentre = -0.55 * localX - displacement
        + pose.laneCentreOffsets[1];
    const double upperDistance = std::abs(localY - upperCentre)
        / std::sqrt(1.0 + upperSlope * upperSlope);
    const double lowerDistance = std::abs(localY - lowerCentre)
        / std::sqrt(1.0 + upperSlope * upperSlope);
    return std::min(upperDistance - pose.laneHalfWidths[0],
                    lowerDistance - pose.laneHalfWidths[1]);
}

bool patternFieldAllows(const PatternMode mode, const double x, const double y,
                        const double progress, const std::uint32_t seed) noexcept {
    if (!std::isfinite(x) || !std::isfinite(y))
        return false;
    return mode == PatternMode::unrestricted
        || patternFieldBoundaryDistance(mode, x, y, progress, seed)
            < kPatternFieldEdgeSoftness;
}

float patternFieldWeight(const PatternMode mode, const double x, const double y,
                         const double progress, const std::uint32_t seed) noexcept {
    if (!std::isfinite(x) || !std::isfinite(y))
        return 0.0f;
    const auto pose = patternFieldPose(mode, progress, seed);
    if (mode == PatternMode::unrestricted) {
        const double dx = x - pose.focusX;
        const double dy = y - pose.focusY;
        const double distanceSquared = dx * dx + dy * dy;
        const double radiusSquared = pose.focusRadius * pose.focusRadius;
        return static_cast<float>(0.68 + 0.52 * std::exp(
            -distanceSquared / (2.0 * radiusSquared)));
    }
    const double distance = patternFieldBoundaryDistance(mode, x, y,
        progress, seed);
    const double normalized = std::clamp(
        (distance + kPatternFieldEdgeSoftness)
            / (2.0 * kPatternFieldEdgeSoftness),
        0.0, 1.0);
    const double smooth = normalized * normalized * (3.0 - 2.0 * normalized);
    return static_cast<float>(1.0 - smooth);
}

EffectiveMixerLevels resolveMixerLevels(
    const std::array<float, kMaxVoices>& requestedVoiceLevels,
    const std::array<bool, kMaxVoices>& voiceMuted,
    const std::array<bool, kMaxVoices>& voiceSolo,
    const std::size_t activeVoiceCount, const float requestedBassLevel,
    const bool bassEnabled, const bool bassMuted, const bool bassSolo) noexcept {
    EffectiveMixerLevels result;
    const auto active = std::min(activeVoiceCount, kMaxVoices);
    const bool anySolo = bassSolo || std::any_of(voiceSolo.begin(),
        voiceSolo.begin() + active, [](const bool solo) { return solo; });
    for (std::size_t voice = 0; voice < active; ++voice) {
        const bool audible = !voiceMuted[voice]
            && (!anySolo || voiceSolo[voice]);
        result.voices[voice] = audible
            ? (finite(requestedVoiceLevels[voice])
                ? std::clamp(requestedVoiceLevels[voice], 0.0f, 1.5f) : 0.0f)
            : 0.0f;
    }
    result.bass = bassEnabled && !bassMuted && (!anySolo || bassSolo)
        ? (finite(requestedBassLevel)
            ? std::clamp(requestedBassLevel, 0.0f, 1.5f) : 0.0f)
        : 0.0f;
    return result;
}

float samplePanTrajectory(const PanTrajectory& trajectory,
                          const float sectionProgress) noexcept {
    if (!trajectory.enabled || !finite(sectionProgress))
        return 0.0f;
    const float wrapped = sectionProgress - std::floor(sectionProgress);
    const float point = wrapped * static_cast<float>(PanTrajectory::pointCount);
    const auto first = static_cast<std::size_t>(point)
        % PanTrajectory::pointCount;
    const auto second = (first + 1) % PanTrajectory::pointCount;
    const float fraction = point - std::floor(point);
    const float a = finite(trajectory.points[first])
        ? std::clamp(trajectory.points[first], -1.0f, 1.0f) : 0.0f;
    const float b = finite(trajectory.points[second])
        ? std::clamp(trajectory.points[second], -1.0f, 1.0f) : 0.0f;
    return a + (b - a) * fraction;
}

bool suggestVoiceTrimDeltaDb(const float voiceRms, const float bassRms,
                             const float currentTrimDb,
                             float& deltaDb) noexcept {
    if (!finite(voiceRms) || !finite(bassRms) || !finite(currentTrimDb)
        || voiceRms <= 1.0e-5f || bassRms <= 1.0e-5f
        || currentTrimDb < -12.0f || currentTrimDb > 24.0f)
        return false;
    const float difference = 20.0f * std::log10(bassRms / voiceRms);
    if (!finite(difference))
        return false;
    deltaDb = std::clamp(difference, -12.0f - currentTrimDb,
                         24.0f - currentTrimDb);
    return true;
}

double swingProgress(const double progress, const double ratio,
                     const double quarterNotesPerSection) noexcept {
    if (!finite(progress) || !finite(ratio)
        || !finite(quarterNotesPerSection) || quarterNotesPerSection <= 0.0)
        return 0.0;
    const double bounded = std::clamp(progress, 0.0, 1.0);
    const double boundedRatio = std::clamp(ratio, 0.5, 0.67);
    const double beatPosition = bounded * quarterNotesPerSection;
    const double beatFraction = beatPosition - std::floor(beatPosition);
    const double moved = beatFraction < 0.5
        ? beatFraction * 2.0 * boundedRatio
        : boundedRatio + (beatFraction - 0.5) * 2.0 * (1.0 - boundedRatio);
    return (std::floor(beatPosition) + moved) / quarterNotesPerSection;
}

FormalPhaseSegment formalPhaseSegmentForCycle(const std::size_t cycle,
                                               const double amount) noexcept {
    if (!finite(amount))
        return {};
    switch (cycle % kFormalCycleCount) {
        case 2: return {0.0, 0.5 * amount};
        case 3: return {0.5 * amount, 0.5 * amount};
        case 4: return {amount, 0.0};
        case 5: return {amount, -0.5 * amount};
        case 6: return {0.5 * amount, -0.5 * amount};
        default: return {};
    }
}

std::size_t activeVoiceCountInCycle(const std::size_t cycle,
                                    const std::size_t requestedCount) noexcept {
    const auto count = std::clamp<std::size_t>(requestedCount, 1, kMaxVoices);
    if (cycle % kFormalCycleCount == 0)
        return 1;
    if (cycle % kFormalCycleCount == 1 || cycle % kFormalCycleCount == 5)
        return (count + 2) / 2;
    return count;
}

std::uint32_t nextSeedWord(std::uint32_t& state) noexcept {
    state += 0x9e3779b9u;
    auto value = state;
    value = (value ^ (value >> 16u)) * 0x85ebca6bu;
    value = (value ^ (value >> 13u)) * 0xc2b2ae35u;
    return value ^ (value >> 16u);
}

std::uint32_t freshSeedOutsideRecent(
        const std::uint32_t candidate, const std::uint32_t* recentSeeds,
        const std::size_t recentCount, const bool hasCurrentSeed,
        const std::uint32_t currentSeed) noexcept {
    // The UI retains at most 64 values. Bound the work even if a malformed
    // preference or a future caller supplies a larger count.
    const auto count = std::min<std::size_t>(recentCount, 64u);
    auto state = candidate;
    // nextSeedWord is bijective over uint32 values and advances its state by
    // an odd constant. count + 2 distinct proposals cover count remembered
    // seeds, one possibly-unremembered current seed, and one available value.
    for (std::size_t attempt = 0; attempt < count + 2u; ++attempt) {
        const auto proposed = nextSeedWord(state);
        const bool seen = recentSeeds != nullptr
            && std::find(recentSeeds, recentSeeds + count, proposed)
                != recentSeeds + count;
        if (!seen && (!hasCurrentSeed || proposed != currentSeed))
            return proposed;
    }
    return nextSeedWord(state);
}

double compositionSeedRandom(std::uint32_t& state) noexcept {
    state += 0x6D2B79F5u;
    std::uint32_t value = state;
    value = (value ^ (value >> 15u)) * (value | 1u);
    value ^= value + ((value ^ (value >> 7u)) * (value | 61u));
    return static_cast<double>(value ^ (value >> 14u)) / 4294967296.0;
}

SeedVariationResult realizeSeedVariation(
    const SeedVariationParameters& parameters) noexcept {
    SeedVariationResult result;
    result.cycles = parameters.baseCycles;
    result.couplings = parameters.baseCouplings;
    result.phases = parameters.basePhases;
    result.transforms = parameters.baseTransforms;
    if (!std::isfinite(parameters.amount) || parameters.amount <= 0.0f)
        return result;

    auto state = parameters.seed;
    const int maxCycleDelta = static_cast<int>(std::lround(
        3.0f * std::clamp(parameters.amount, 0.0f, 1.0f)));
    const float amount = std::clamp(parameters.amount, 0.0f, 1.0f);
    for (std::size_t voice = 0; voice < kMaxVoices; ++voice) {
        const int cycleDelta = static_cast<int>(std::floor(
            compositionSeedRandom(state) * static_cast<double>(2 * maxCycleDelta + 1)))
            - maxCycleDelta;
        result.cycles[voice] = std::clamp(parameters.baseCycles[voice]
            + static_cast<float>(cycleDelta), 1.0f, 12.0f);

        if (voice > 0) {
            result.couplings[voice] = std::clamp(parameters.baseCouplings[voice]
                + static_cast<float>((compositionSeedRandom(state) * 2.0 - 1.0)
                    * 0.25f * amount), 0.0f, 0.8f);
            const float jitter = static_cast<float>((compositionSeedRandom(state) * 2.0 - 1.0)
                * 0.125f * amount);
            const float phase = parameters.basePhases[voice] + jitter;
            result.phases[voice] = phase - std::floor(phase + 0.5f);
        }

        if (compositionSeedRandom(state) < amount) {
            int selected = static_cast<int>(std::floor(
                compositionSeedRandom(state) * 9.0));
            if (selected == parameters.baseTransforms[voice])
                ++selected;
            result.transforms[voice] = selected;
        }
    }
    return result;
}

SeededRecipeSelection seededRecipeSelection(const std::uint32_t seed) noexcept {
    constexpr std::array<std::size_t, 5> legacyCycleIndices = {1, 2, 3, 5, 7};
    auto state = seed;
    SeededRecipeSelection result;
    result.voiceTwoCycleIndex = legacyCycleIndices[nextSeedWord(state) % legacyCycleIndices.size()];
    result.phaseIndex = nextSeedWord(state) % 101u;
    result.eventRuleIndex = nextSeedWord(state) % 2u;
    result.patternIndex = nextSeedWord(state) % 3u;

    state = seed;
    for (int i = 0; i < 6; ++i)
        (void) nextSeedWord(state);
    result.voiceCount = std::min<std::size_t>(kMaxActiveVoices,
        2u + nextSeedWord(state) % 11u);

    state = seed;
    for (int i = 0; i < 4; ++i)
        (void) nextSeedWord(state);
    result.intensityLawIndex = nextSeedWord(state) % 8u;

    state = seed;
    for (int i = 0; i < 5; ++i)
        (void) nextSeedWord(state);
    result.intensityAmount = 0.35f
        + static_cast<float>(nextSeedWord(state) % 66u) / 100.0f;

    const auto gene = [seed](const std::uint32_t tag) {
        auto geneState = seed ^ tag;
        return nextSeedWord(geneState);
    };
    result.soundFamilyIndex = gene(0x46414d31u) % 17u; // FAM1
    result.grooveIndex = gene(0x47525631u) % 4u; // GRV1
    result.intentionIndex = gene(0x494e5431u) % 4u; // INT1
    result.dynamicsIndex = gene(0x44594e31u) % 4u; // DYN1
    result.meterIndex = gene(0x4d455431u) % 4u; // MET1
    result.voiceTwoTravelPerBar = 0.005f
        * static_cast<float>(gene(0x54525631u) % 26u); // TRV1
    result.compositionVariation = 0.25f
        + static_cast<float>(gene(0x56415231u) % 76u) / 100.0f; // VAR1
    result.geometryVoiceOneSource = (gene(0x47454f31u) & 1u) != 0u; // GEO1
    result.geometryShapeIndex = gene(0x53485031u) % 17u; // SHP1
    result.geometryFourierTerms = 1
        + static_cast<int>(gene(0x46545231u) % 32u); // FTR1
    result.billiardPatternIndex = gene(0x42494c31u) % 7u; // BIL1
    result.lissajousXFrequency = 1
        + static_cast<int>(gene(0x4c495831u) % 12u); // LIX1
    result.lissajousYFrequency = 1
        + static_cast<int>(gene(0x4c495931u) % 12u); // LIY1
    result.lissajousPhase = static_cast<float>(
        gene(0x4c495031u) % 101u) / 100.0f; // LIP1
    result.lissajousXAmplitude = static_cast<float>(
        25u + gene(0x4c494131u) % 76u) / 100.0f; // LIA1
    result.lissajousYAmplitude = static_cast<float>(
        25u + gene(0x4c494231u) % 76u) / 100.0f; // LIB1
    for (std::size_t voice = 0; voice < kMaxActiveVoices; ++voice) {
        const auto voiceTag = static_cast<std::uint32_t>(voice) * 0x9e3779b9u;
        result.chipDutyCycles[voice] = 0.125f
            + 0.125f * static_cast<float>(gene(0x43445500u + voiceTag) % 7u);
        result.chipClockHz[voice] = 1000.0f
            * static_cast<float>(1u + gene(0x434c4b00u + voiceTag) % 24u);
        result.chipBitDepth[voice] = 2
            + static_cast<int>(gene(0x43424900u + voiceTag) % 7u);
        result.chipNoiseMix[voice] = static_cast<float>(
            gene(0x434e5300u + voiceTag) % 101u) / 100.0f;
        result.voiceBaseCouplings[voice] = voice == 0 ? 0.0f
            : 0.01f * static_cast<float>(gene(0x434f5500u + voiceTag) % 81u);
        const float phaseJitter = voice == 0 ? 0.0f
            : (static_cast<float>(gene(0x50485300u + voiceTag) % 251u)
                / 1000.0f - 0.125f);
        const float phaseStagger = voice == 0 ? 0.0f
            : static_cast<float>(voice - 1) / 16.0f;
        const float phase = phaseStagger + phaseJitter;
        result.voiceBasePhases[voice] = phase - std::floor(phase + 0.5f);
        result.voiceBaseTransforms[voice] = static_cast<int>(
            gene(0x54524600u + voiceTag) % 10u);
        result.formulaPhaseOffsets[voice] = static_cast<float>(
            gene(0x46504800u + voiceTag) % 1001u) / 1000.0f - 0.5f;
        result.voiceVariationDepths[voice] = static_cast<float>(
            gene(0x56445000u + voiceTag) % 101u) / 100.0f;
    }
    return result;
}

std::uint32_t algorithmicNextSeed(const std::uint32_t currentSeed,
                                  const std::uint32_t salt) noexcept {
    const auto current = seededRecipeSelection(currentSeed);
    std::uint32_t state = currentSeed ^ 0xB47A5EEDu ^ salt;
    std::uint32_t bestSeed = currentSeed;
    int bestScore = -1;
    for (std::size_t candidateIndex = 0; candidateIndex < 64; ++candidateIndex) {
        const auto candidateSeed = nextSeedWord(state);
        if (candidateSeed == currentSeed)
            continue;
        const auto candidate = seededRecipeSelection(candidateSeed);
        const auto phaseDelta = current.phaseIndex > candidate.phaseIndex
            ? current.phaseIndex - candidate.phaseIndex
            : candidate.phaseIndex - current.phaseIndex;
        const auto phaseDistance = std::min(phaseDelta, 101u - phaseDelta);
        const int amountDistance = static_cast<int>(std::lround(
            std::abs(current.intensityAmount - candidate.intensityAmount) * 100.0f));
        int score = static_cast<int>(std::abs(static_cast<int>(current.voiceCount)
                                              - static_cast<int>(candidate.voiceCount))) * 100;
        score += static_cast<int>(std::abs(
            static_cast<int>(current.voiceTwoCycleIndex)
            - static_cast<int>(candidate.voiceTwoCycleIndex))) * 80;
        score += static_cast<int>(phaseDistance) * 2;
        score += current.eventRuleIndex != candidate.eventRuleIndex ? 70 : 0;
        score += current.intensityLawIndex != candidate.intensityLawIndex ? 50 : 0;
        score += amountDistance * 2;
        // The native host starts in Campo livre; the seed now applies its
        // sound family, so timbral contrast belongs in the candidate score.
        score += current.soundFamilyIndex != candidate.soundFamilyIndex ? 90 : 0;
        score += current.geometryVoiceOneSource
            != candidate.geometryVoiceOneSource ? 100 : 0;
        score += current.geometryShapeIndex
            != candidate.geometryShapeIndex ? 55 : 0;
        score += current.billiardPatternIndex
            != candidate.billiardPatternIndex ? 25 : 0;
        score += std::abs(current.geometryFourierTerms
            - candidate.geometryFourierTerms);
        score += std::abs(current.lissajousXFrequency
            - candidate.lissajousXFrequency) * 3;
        score += std::abs(current.lissajousYFrequency
            - candidate.lissajousYFrequency) * 3;
        score += static_cast<int>(std::lround(std::abs(
            current.lissajousPhase - candidate.lissajousPhase) * 20.0f));
        score += static_cast<int>(std::lround(std::abs(
            current.lissajousXAmplitude - candidate.lissajousXAmplitude) * 20.0f));
        score += static_cast<int>(std::lround(std::abs(
            current.lissajousYAmplitude - candidate.lissajousYAmplitude) * 20.0f));
        score += current.grooveIndex != candidate.grooveIndex ? 80 : 0;
        score += current.intentionIndex != candidate.intentionIndex ? 65 : 0;
        score += current.dynamicsIndex != candidate.dynamicsIndex ? 65 : 0;
        score += current.meterIndex != candidate.meterIndex ? 70 : 0;
        score += static_cast<int>(std::lround(std::abs(
            current.voiceTwoTravelPerBar - candidate.voiceTwoTravelPerBar)
            * 800.0f));
        score += static_cast<int>(std::lround(std::abs(
            current.compositionVariation - candidate.compositionVariation) * 100.0f));
        for (std::size_t voice = 0; voice < kMaxActiveVoices; ++voice) {
            score += current.voiceBaseTransforms[voice]
                != candidate.voiceBaseTransforms[voice] ? 16 : 0;
            score += static_cast<int>(std::lround(std::abs(
                current.formulaPhaseOffsets[voice]
                    - candidate.formulaPhaseOffsets[voice]) * 16.0f));
            score += static_cast<int>(std::lround(std::abs(
                current.voiceVariationDepths[voice]
                    - candidate.voiceVariationDepths[voice]) * 16.0f));
            score += static_cast<int>(std::lround(std::abs(
                current.voiceBaseCouplings[voice]
                    - candidate.voiceBaseCouplings[voice]) * 20.0f));
            score += static_cast<int>(std::lround(std::abs(
                current.voiceBasePhases[voice]
                    - candidate.voiceBasePhases[voice]) * 24.0f));
        }
        auto differingBits = currentSeed ^ candidateSeed;
        while (differingBits != 0u) {
            score += static_cast<int>(differingBits & 1u);
            differingBits >>= 1u;
        }
        if (score > bestScore) {
            bestScore = score;
            bestSeed = candidateSeed;
        }
    }
    return bestSeed == currentSeed ? nextSeedWord(state) : bestSeed;
}

float grooveAccentAtProgress(const double progress, const GrooveMode groove,
                             int numerator, int denominator) noexcept {
    if (!std::isfinite(progress))
        return 1.0f;
    if (!((denominator == 4 && (numerator == 3 || numerator == 4 || numerator == 5))
          || (numerator == 7 && denominator == 8))) {
        numerator = 4;
        denominator = 4;
    }
    const int eighthsPerBar = numerator * 8 / denominator;
    const int beatStep = 8 / denominator;
    const auto step = static_cast<int>(std::lround(
        std::clamp(progress, 0.0, 1.0) * 4.0 * eighthsPerBar + 1.0e-9))
        % eighthsPerBar;
    bool accented = false;
    switch (groove) {
        case GrooveMode::straight:
            if (step > 0 && step % beatStep == 0)
                return 1.0f;
            accented = step == 0;
            break;
        case GrooveMode::ternary:
            accented = step == 0
                || step == static_cast<int>(std::lround(eighthsPerBar / 3.0)) % eighthsPerBar
                || step == static_cast<int>(std::lround(2.0 * eighthsPerBar / 3.0)) % eighthsPerBar;
            break;
        case GrooveMode::additive:
            accented = step == 0 || step == 3 || step == 6;
            break;
        case GrooveMode::offbeat:
            accented = step % 2 == 1;
            break;
    }
    return accented ? 1.16f : 0.82f;
}

float expressionAtProgress(const double progress, const DynamicCurve dynamics,
                           const MusicalIntention intention) noexcept {
    const double p = std::clamp(std::isfinite(progress) ? progress : 0.0, 0.0, 1.0);
    double curve = 1.0;
    switch (dynamics) {
        case DynamicCurve::steady: break;
        case DynamicCurve::crescendo: curve = 0.35 + 0.9 * p; break;
        case DynamicCurve::arch: curve = 0.38 + 0.85 * std::sin(0.5 * kTau * p); break;
        case DynamicCurve::waves:
            curve = 0.75 + 0.25 * std::sin(2.0 * kTau * p - 0.25 * kTau);
            break;
    }
    switch (intention) {
        case MusicalIntention::build: curve *= 0.68 + 0.58 * p; break;
        case MusicalIntention::thin: curve *= 1.18 - 0.62 * p; break;
        case MusicalIntention::meet: break;
        case MusicalIntention::contrast:
            curve *= (static_cast<int>(std::floor(p * 14.0)) % 2 == 0) ? 1.13 : 0.78;
            break;
    }
    return static_cast<float>(std::clamp(curve, 0.08, 1.35));
}

float intensityFormulaValue(const IntensityLaw law, const float inputX,
                            const float inputY, const double cyclesX,
                            const double cyclesY) noexcept {
    const float x = std::clamp(finite(inputX) ? inputX : 0.0f, -1.0f, 1.0f);
    const float y = std::clamp(finite(inputY) ? inputY : 0.0f, -1.0f, 1.0f);
    const float x2 = x * x;
    const float y2 = y * y;
    const double nx = finite(cyclesX) && cyclesX > 0.0 ? cyclesX : 1.0;
    const double ny = finite(cyclesY) && cyclesY > 0.0 ? cyclesY : 1.0;
    const double wx = nx * nx;
    const double wy = ny * ny;
    const float potential = static_cast<float>((wx * x2 + wy * y2) / (wx + wy));
    switch (law) {
        case IntensityLaw::uniform: return 1.0f;
        case IntensityLaw::projectionY: return 0.5f + 0.5f * y;
        case IntensityLaw::productXY: return 0.5f + 0.5f * x * y;
        case IntensityLaw::quadraticDifference:
            return 0.5f + 0.5f * (x2 - y2);
        case IntensityLaw::gaussianCentre:
            return static_cast<float>(std::exp(-3.0 * (x2 + y2)));
        case IntensityLaw::cosineBands:
            return 0.5f + 0.5f * static_cast<float>(
                std::cos(0.5 * kTau * static_cast<double>(x - y)));
        case IntensityLaw::potentialEnergy: return potential;
        case IntensityLaw::kineticEnergy: return 1.0f - potential;
    }
    return 1.0f;
}

float blendedIntensityFormulaValue(const IntensityLaw lawA,
                                   const IntensityLaw lawB,
                                   const float blend, const float x,
                                   const float y, const double cyclesX,
                                   const double cyclesY) noexcept {
    const float weight = std::clamp(finite(blend) ? blend : 0.0f, 0.0f, 1.0f);
    const float a = intensityFormulaValue(lawA, x, y, cyclesX, cyclesY);
    const float b = intensityFormulaValue(lawB, x, y, cyclesX, cyclesY);
    return std::clamp(a + (b - a) * weight, 0.0f, 1.0f);
}

CompositionRecipe makeInitialRecipe() noexcept {
    CompositionRecipe recipe;
    recipe.sectionSeconds = 8.0;
    recipe.voiceCount = 2;

    auto& first = recipe.voices[0];
    first.cyclesPerSection = 3.0;
    first.phaseCycles = 0.0;
    first.resonanceHz = 110.0;
    first.gain = 0.62f;
    first.pan = -0.28f;
    first.eventRule = EventRule::centreCrossings;
    first.resonator.frequencyRatios = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    first.resonator.modeWeights = {0.90f, 0.46f, 0.30f, 0.22f,
                                   0.17f, 0.13f, 0.10f, 0.08f};
    first.resonator.decaySeconds = 0.72;
    first.resonator.frequencyDependentLoss = 0.008;
    first.resonator.strikeNoise = 0.06f;

    auto& second = recipe.voices[1];
    second.cyclesPerSection = 4.0;
    second.phaseCycles = 0.0;
    second.resonanceHz = 165.0;
    second.gain = 0.55f;
    second.pan = 0.28f;
    second.eventRule = EventRule::centreCrossings;
    second.resonator.frequencyRatios = {1.0, 1.47, 2.13, 2.89,
                                        3.76, 4.72, 5.81, 7.03};
    second.resonator.modeWeights = {0.90f, 0.53f, 0.39f, 0.29f,
                                    0.23f, 0.18f, 0.14f, 0.11f};
    second.resonator.decaySeconds = 1.15;
    second.resonator.frequencyDependentLoss = 0.018;
    second.resonator.strikeNoise = 0.13f;
    return recipe;
}

bool validRhythmicCell(const RhythmicCell& cell) noexcept {
    if (cell.count > kMaxRhythmicCellAttacks)
        return false;
    constexpr double minimumGap = 1.0 / 1024.0;
    double previous = -minimumGap;
    for (std::size_t index = 0; index < cell.count; ++index) {
        const double position = cell.positions[index];
        const float strength = cell.strengths[index];
        if (!std::isfinite(position) || position < 0.0 || position >= 1.0
            || position - previous < minimumGap
            || !std::isfinite(strength) || strength < 0.0f || strength > 1.0f
            || (cell.midiNotes[index] != kNoMidiNote
                && cell.midiNotes[index] > 127u)
            || (cell.midiHatPedal[index] != kNoMidiControllerValue
                && cell.midiHatPedal[index] > 127u)
            || cell.midiCollisionCount[index] > kMaxMidiCollisionNotesPerAttack
            || cell.ornaments[index] > kMaxCellOrnamentCode
            || (cell.midiCollisionCount[index] > 0
                && cell.midiNotes[index] == kNoMidiNote))
            return false;
        for (std::size_t collision = 0;
             collision < cell.midiCollisionCount[index]; ++collision) {
            const auto note = cell.midiCollisionNotes[index][collision];
            const auto pedal = cell.midiCollisionHatPedal[index][collision];
            const auto collisionStrength = cell.midiCollisionStrengths[index][collision];
            if (note == kNoMidiNote || note > 127u
                || note == cell.midiNotes[index]
                || !std::isfinite(collisionStrength)
                || collisionStrength < 0.0f || collisionStrength > 1.0f
                || (pedal != kNoMidiControllerValue && pedal > 127u))
                return false;
            for (std::size_t earlier = 0; earlier < collision; ++earlier)
                if (cell.midiCollisionNotes[index][earlier] == note)
                    return false;
        }
        previous = position;
    }
    return true;
}

namespace {

void copyRhythmicAttack(RhythmicCell& destination, const std::size_t to,
                        const RhythmicCell& source, const std::size_t from) noexcept {
    destination.positions[to] = source.positions[from];
    destination.strengths[to] = source.strengths[from];
    destination.midiNotes[to] = source.midiNotes[from];
    destination.midiHatPedal[to] = source.midiHatPedal[from];
    destination.midiCollisionCount[to] = source.midiCollisionCount[from];
    destination.ornaments[to] = source.ornaments[from];
    destination.midiCollisionNotes[to] = source.midiCollisionNotes[from];
    destination.midiCollisionStrengths[to] = source.midiCollisionStrengths[from];
    destination.midiCollisionHatPedal[to] = source.midiCollisionHatPedal[from];
}

void clearRhythmicAttack(RhythmicCell& cell, const std::size_t index) noexcept {
    cell.positions[index] = 0.0;
    cell.strengths[index] = 0.0f;
    cell.midiNotes[index] = kNoMidiNote;
    cell.midiHatPedal[index] = kNoMidiControllerValue;
    cell.midiCollisionCount[index] = 0;
    cell.midiCollisionNotes[index].fill(kNoMidiNote);
    cell.midiCollisionStrengths[index].fill(0.0f);
    cell.midiCollisionHatPedal[index].fill(kNoMidiControllerValue);
    cell.ornaments[index] = 0;
}

} // namespace

bool insertRhythmicCellAttack(RhythmicCell& cell, const double position,
                              const float strength,
                              std::size_t* insertedIndex) noexcept {
    if (!cell.enabled || cell.count >= kMaxRhythmicCellAttacks
        || !std::isfinite(position) || position < 0.0 || position >= 1.0
        || !std::isfinite(strength) || strength < 0.0f || strength > 1.0f
        || !validRhythmicCell(cell))
        return false;
    constexpr double minimumGap = 1.0 / 1024.0;
    std::size_t index = 0;
    while (index < cell.count && cell.positions[index] < position)
        ++index;
    if ((index > 0 && position - cell.positions[index - 1] < minimumGap)
        || (index < cell.count && cell.positions[index] - position < minimumGap))
        return false;
    for (std::size_t move = cell.count; move > index; --move)
        copyRhythmicAttack(cell, move, cell, move - 1);
    cell.positions[index] = position;
    cell.strengths[index] = strength;
    cell.midiNotes[index] = kNoMidiNote;
    cell.midiHatPedal[index] = kNoMidiControllerValue;
    cell.midiCollisionCount[index] = 0;
    cell.midiCollisionNotes[index].fill(kNoMidiNote);
    cell.midiCollisionStrengths[index].fill(0.0f);
    cell.midiCollisionHatPedal[index].fill(kNoMidiControllerValue);
    cell.ornaments[index] = 0;
    ++cell.count;
    if (insertedIndex != nullptr)
        *insertedIndex = index;
    return true;
}

bool removeRhythmicCellAttack(RhythmicCell& cell,
                              const std::size_t index) noexcept {
    if (!cell.enabled || !validRhythmicCell(cell) || index >= cell.count)
        return false;
    for (std::size_t move = index; move + 1 < cell.count; ++move)
        copyRhythmicAttack(cell, move, cell, move + 1);
    --cell.count;
    clearRhythmicAttack(cell, cell.count);
    return true;
}

bool moveRhythmicCellAttack(RhythmicCell& cell, const std::size_t index,
                            const double position) noexcept {
    if (!cell.enabled || !validRhythmicCell(cell) || index >= cell.count
        || !std::isfinite(position))
        return false;
    constexpr double minimumGap = 1.0 / 1024.0;
    const double lower = index == 0 ? 0.0
        : cell.positions[index - 1] + minimumGap;
    const double upper = index + 1 == cell.count ? 1.0 - minimumGap
        : cell.positions[index + 1] - minimumGap;
    if (lower > upper)
        return false;
    cell.positions[index] = std::clamp(position, lower, upper);
    return true;
}

bool rotateRhythmicCell(RhythmicCell& cell,
                        const double phaseTurns) noexcept {
    if (!cell.enabled || !validRhythmicCell(cell)
        || !std::isfinite(phaseTurns))
        return false;
    const auto original = cell;
    const double offset = phaseTurns - std::floor(phaseTurns);
    if (offset <= kEpsilon || 1.0 - offset <= kEpsilon)
        return false;
    for (std::size_t attack = 0; attack < cell.count; ++attack) {
        cell.positions[attack] += offset;
        if (cell.positions[attack] >= 1.0)
            cell.positions[attack] -= 1.0;
    }
    for (std::size_t attack = 1; attack < cell.count; ++attack) {
        const auto moved = cell;
        const double position = cell.positions[attack];
        std::size_t destination = attack;
        while (destination > 0
            && cell.positions[destination - 1] > position) {
            copyRhythmicAttack(cell, destination, cell, destination - 1);
            --destination;
        }
        copyRhythmicAttack(cell, destination, moved, attack);
    }
    if (validRhythmicCell(cell))
        return true;
    cell = original;
    return false;
}

bool euclideanRedistributeRhythmicCell(RhythmicCell& cell,
                                       const std::size_t steps) noexcept {
    if (!cell.enabled || !validRhythmicCell(cell) || cell.count < 2
        || steps < cell.count || steps > 256)
        return false;

    struct Placement {
        double position = 0.0;
        std::size_t source = 0;
    };
    std::array<Placement, kMaxRhythmicCellAttacks> placements{};
    const auto original = cell;
    const auto firstStep = static_cast<std::size_t>(std::llround(
        original.positions[0] * static_cast<double>(steps))) % steps;
    for (std::size_t attack = 0; attack < cell.count; ++attack) {
        const auto offset = attack * steps / cell.count;
        const auto slot = (firstStep + offset) % steps;
        placements[attack] = {
            static_cast<double>(slot) / static_cast<double>(steps), attack};
    }
    std::sort(placements.begin(), placements.begin() + cell.count,
        [](const Placement& left, const Placement& right) {
            return left.position < right.position;
        });

    auto redistributed = original;
    for (std::size_t attack = 0; attack < cell.count; ++attack) {
        if (std::abs(placements[attack].position
                     - original.positions[placements[attack].source]) > kEpsilon)
            break;
        if (attack + 1 == cell.count)
            return false;
    }
    for (std::size_t attack = 0; attack < cell.count; ++attack) {
        copyRhythmicAttack(redistributed, attack, original,
                           placements[attack].source);
        redistributed.positions[attack] = placements[attack].position;
    }
    if (!validRhythmicCell(redistributed))
        return false;
    cell = redistributed;
    return true;
}

bool reduceRhythmicCellDensity(RhythmicCell& cell, const double keepRatio,
                               const std::uint32_t seed) noexcept {
    if (!cell.enabled || !validRhythmicCell(cell) || cell.count < 2
        || !std::isfinite(keepRatio) || keepRatio <= 0.0 || keepRatio >= 1.0)
        return false;

    const auto keepCount = std::clamp<std::size_t>(
        static_cast<std::size_t>(std::lround(
            static_cast<double>(cell.count) * keepRatio)), 1, cell.count);
    if (keepCount >= cell.count)
        return false;
    std::array<bool, kMaxRhythmicCellAttacks> keep{};
    std::size_t kept = 0;

    // Preserve an existing downbeat, then retain the strongest remaining
    // attacks. The seed only resolves equal-velocity choices; an onset and all
    // of its simultaneous MIDI notes/controllers are treated as one unit.
    if (cell.positions[0] <= kEpsilon) {
        keep[0] = true;
        ++kept;
    }
    while (kept < keepCount) {
        std::size_t best = cell.count;
        std::uint32_t bestTie = 0;
        for (std::size_t attack = 0; attack < cell.count; ++attack) {
            if (keep[attack])
                continue;
            std::uint32_t tie = seed
                ^ (0x9e3779b9u * static_cast<std::uint32_t>(attack + 1));
            tie ^= tie >> 16u;
            tie *= 0x7feb352du;
            tie ^= tie >> 15u;
            tie *= 0x846ca68bu;
            tie ^= tie >> 16u;
            if (best == cell.count
                || cell.strengths[attack] > cell.strengths[best]
                || (cell.strengths[attack] == cell.strengths[best]
                    && tie > bestTie)) {
                best = attack;
                bestTie = tie;
            }
        }
        if (best == cell.count)
            return false;
        keep[best] = true;
        ++kept;
    }

    auto reduced = cell;
    reduced.count = 0;
    for (std::size_t attack = 0; attack < cell.count; ++attack) {
        if (!keep[attack])
            continue;
        copyRhythmicAttack(reduced, reduced.count, cell, attack);
        ++reduced.count;
    }
    for (std::size_t attack = reduced.count; attack < cell.count; ++attack)
        clearRhythmicAttack(reduced, attack);
    if (!validRhythmicCell(reduced))
        return false;
    cell = reduced;
    return true;
}

bool interlockRhythmicCell(RhythmicCell& cell, const RhythmicCell& partner,
                           const std::uint32_t seed) noexcept {
    if (!cell.enabled || !partner.enabled || !validRhythmicCell(cell)
        || !validRhythmicCell(partner) || cell.count == 0 || partner.count < 2)
        return false;

    constexpr double minimumGap = 1.0 / 1024.0;
    constexpr double maximumShift = 0.125;
    const auto circularDistance = [](const double first, const double second) {
        const double distance = std::abs(first - second);
        return std::min(distance, 1.0 - distance);
    };
    auto interlocked = cell;
    bool changed = false;
    for (std::size_t attack = 0; attack < interlocked.count; ++attack) {
        const double original = interlocked.positions[attack];
        bool overlaps = false;
        for (std::size_t other = 0; other < partner.count; ++other)
            overlaps = overlaps
                || circularDistance(original, partner.positions[other]) < minimumGap;
        if (!overlaps)
            continue;

        double bestPosition = original;
        double bestDistance = maximumShift + 1.0;
        std::uint32_t bestTie = 0;
        for (std::size_t gap = 0; gap < partner.count; ++gap) {
            const double left = partner.positions[gap];
            double right = partner.positions[(gap + 1) % partner.count];
            if (gap + 1 == partner.count)
                right += 1.0;
            const double midpoint = std::fmod(0.5 * (left + right), 1.0);
            const double distance = circularDistance(original, midpoint);
            if (distance <= kEpsilon || distance > maximumShift)
                continue;
            bool available = true;
            for (std::size_t other = 0; other < partner.count; ++other)
                if (circularDistance(midpoint, partner.positions[other])
                    < minimumGap) {
                    available = false;
                    break;
                }
            for (std::size_t other = 0; available && other < interlocked.count;
                 ++other)
                if (other != attack
                    && circularDistance(midpoint, interlocked.positions[other])
                        < minimumGap) {
                    available = false;
                    break;
                }
            if (!available)
                continue;
            std::uint32_t tie = seed
                ^ (0x9e3779b9u * static_cast<std::uint32_t>(attack + 1))
                ^ (0x85ebca6bu * static_cast<std::uint32_t>(gap + 1));
            tie ^= tie >> 16u;
            tie *= 0x7feb352du;
            tie ^= tie >> 15u;
            tie *= 0x846ca68bu;
            tie ^= tie >> 16u;
            if (distance < bestDistance - kEpsilon
                || (std::abs(distance - bestDistance) <= kEpsilon
                    && tie > bestTie)) {
                bestPosition = midpoint;
                bestDistance = distance;
                bestTie = tie;
            }
        }
        if (bestDistance <= maximumShift) {
            interlocked.positions[attack] = bestPosition;
            changed = true;
        }
    }
    if (!changed)
        return false;

    std::array<std::size_t, kMaxRhythmicCellAttacks> order{};
    for (std::size_t attack = 0; attack < interlocked.count; ++attack) {
        std::size_t destination = attack;
        while (destination > 0
            && interlocked.positions[order[destination - 1]]
                > interlocked.positions[attack]) {
            order[destination] = order[destination - 1];
            --destination;
        }
        order[destination] = attack;
    }
    auto sorted = interlocked;
    for (std::size_t attack = 0; attack < interlocked.count; ++attack)
        copyRhythmicAttack(sorted, attack, interlocked, order[attack]);
    if (!validRhythmicCell(sorted))
        return false;
    cell = sorted;
    return true;
}

bool recombineRhythmicCell(RhythmicCell& cell, const RhythmicCell& partner,
                           const std::uint32_t seed) noexcept {
    if (!cell.enabled || !partner.enabled || !validRhythmicCell(cell)
        || !validRhythmicCell(partner) || cell.count < 2 || partner.count < 2)
        return false;

    struct Candidate {
        double position = 0.0;
        std::size_t sourceAttack = 0;
        bool donorTiming = false;
    };
    constexpr double minimumGap = 1.0 / 1024.0;
    const auto circularDistance = [](const double first, const double second) {
        const double distance = std::abs(first - second);
        return std::min(distance, 1.0 - distance);
    };
    std::uint32_t phaseHash = seed ^ 0x6d2b79f5u;
    phaseHash ^= phaseHash >> 16u;
    phaseHash *= 0x7feb352du;
    phaseHash ^= phaseHash >> 15u;
    const unsigned sourceParity = cell.positions[0] <= kEpsilon
        ? 0u : phaseHash & 1u;
    const unsigned donorParity = 1u - sourceParity;

    std::array<Candidate, kMaxRhythmicCellAttacks * 2> candidates{};
    std::size_t candidateCount = 0;
    for (std::size_t attack = 0; attack < cell.count; ++attack)
        if ((attack & 1u) == sourceParity)
            candidates[candidateCount++] = {
                cell.positions[attack], attack, false};
    for (std::size_t attack = 0; attack < partner.count; ++attack)
        if ((attack & 1u) == donorParity) {
            const double rank = partner.count <= 1 ? 0.0
                : static_cast<double>(attack)
                    / static_cast<double>(partner.count - 1);
            const auto mapped = std::min<std::size_t>(cell.count - 1,
                static_cast<std::size_t>(std::llround(
                    rank * static_cast<double>(cell.count - 1))));
            candidates[candidateCount++] = {
                partner.positions[attack], mapped, true};
        }
    if (candidateCount == 0)
        return false;

    std::sort(candidates.begin(), candidates.begin() + candidateCount,
        [](const Candidate& left, const Candidate& right) {
            if (left.position != right.position)
                return left.position < right.position;
            return !left.donorTiming && right.donorTiming;
        });
    std::array<Candidate, kMaxRhythmicCellAttacks * 2> unique{};
    std::size_t uniqueCount = 0;
    for (std::size_t index = 0; index < candidateCount; ++index) {
        const auto candidate = candidates[index];
        if (uniqueCount > 0
            && circularDistance(unique[uniqueCount - 1].position,
                                candidate.position) < minimumGap) {
            if (unique[uniqueCount - 1].donorTiming && !candidate.donorTiming)
                unique[uniqueCount - 1] = candidate;
            continue;
        }
        unique[uniqueCount++] = candidate;
    }
    while (uniqueCount > 1
        && circularDistance(unique[0].position,
                            unique[uniqueCount - 1].position) < minimumGap) {
        if (unique[0].donorTiming && !unique[uniqueCount - 1].donorTiming) {
            for (std::size_t index = 0; index + 1 < uniqueCount; ++index)
                unique[index] = unique[index + 1];
        }
        --uniqueCount;
    }
    if (uniqueCount == 0 || uniqueCount > kMaxRhythmicCellAttacks)
        return false;

    auto recombined = cell;
    recombined.count = 0;
    for (std::size_t index = 0; index < uniqueCount; ++index) {
        auto candidate = unique[index];
        copyRhythmicAttack(recombined, recombined.count, cell,
                           candidate.sourceAttack);
        const auto destination = recombined.count++;
        recombined.positions[destination] = candidate.position;
        if (candidate.donorTiming) {
            recombined.strengths[destination] *= 0.82f;
            for (std::size_t collision = 0;
                 collision < recombined.midiCollisionCount[destination];
                 ++collision)
                recombined.midiCollisionStrengths[destination][collision]
                    *= 0.82f;
        }
    }
    for (std::size_t index = recombined.count; index < cell.count; ++index)
        clearRhythmicAttack(recombined, index);
    if (!validRhythmicCell(recombined))
        return false;

    bool changed = recombined.count != cell.count;
    for (std::size_t attack = 0; attack < recombined.count; ++attack)
        changed = changed
            || std::abs(recombined.positions[attack] - cell.positions[attack]) > kEpsilon
            || std::abs(recombined.strengths[attack] - cell.strengths[attack]) > 1.0e-7f
            || recombined.midiNotes[attack] != cell.midiNotes[attack]
            || recombined.midiCollisionNotes[attack] != cell.midiCollisionNotes[attack];
    if (!changed)
        return false;
    cell = recombined;
    return true;
}

bool canRecombineRhythmicPhraseBars(const RhythmicCell& cell,
                                    const RhythmicCell& partner,
                                    const std::uint8_t barCount) noexcept {
    if (barCount < 2 || barCount > 16 || !cell.enabled || !partner.enabled
        || !validRhythmicCell(cell) || !validRhythmicCell(partner)
        || cell.count < 2 || partner.count < 2)
        return false;
    std::array<std::uint8_t, 16> selectedCounts{};
    std::array<std::uint8_t, 16> partnerCounts{};
    for (std::size_t attack = 0; attack < cell.count; ++attack) {
        const auto bar = std::min<unsigned>(barCount - 1u,
            static_cast<unsigned>(cell.positions[attack] * barCount));
        ++selectedCounts[bar];
    }
    for (std::size_t attack = 0; attack < partner.count; ++attack) {
        const auto bar = std::min<unsigned>(barCount - 1u,
            static_cast<unsigned>(partner.positions[attack] * barCount));
        ++partnerCounts[bar];
    }
    for (unsigned bar = 0; bar < barCount; ++bar)
        if (selectedCounts[bar] >= 2 && partnerCounts[bar] >= 2)
            return true;
    return false;
}

bool canAdaptRhythmicCellPairDensity(const RhythmicCell& first,
                                     const RhythmicCell& second,
                                     const std::uint8_t barCount) noexcept {
    if (barCount == 0 || barCount > 16 || !first.enabled || !second.enabled
        || !validRhythmicCell(first) || !validRhythmicCell(second)
        || first.count < 2 || second.count < 2)
        return false;
    std::array<std::uint8_t, 16> firstCounts{};
    std::array<std::uint8_t, 16> secondCounts{};
    for (std::size_t attack = 0; attack < first.count; ++attack) {
        const auto bar = std::min<unsigned>(barCount - 1u,
            static_cast<unsigned>(first.positions[attack] * barCount));
        ++firstCounts[bar];
    }
    for (std::size_t attack = 0; attack < second.count; ++attack) {
        const auto bar = std::min<unsigned>(barCount - 1u,
            static_cast<unsigned>(second.positions[attack] * barCount));
        ++secondCounts[bar];
    }
    for (unsigned bar = 0; bar < barCount; ++bar) {
        const auto low = std::min(firstCounts[bar], secondCounts[bar]);
        const auto high = std::max(firstCounts[bar], secondCounts[bar]);
        if (low >= 2u && high > low + std::max(1u, low / 4u))
            return true;
    }
    return false;
}

bool adaptRhythmicCellPairDensity(RhythmicCell& first, RhythmicCell& second,
                                  const std::uint8_t barCount,
                                  const std::uint32_t seed) noexcept {
    if (!canAdaptRhythmicCellPairDensity(first, second, barCount))
        return false;

    RhythmicCell adaptedFirst;
    RhythmicCell adaptedSecond;
    adaptedFirst.enabled = true;
    adaptedSecond.enabled = true;
    bool changed = false;
    const auto appendBar = [barCount](RhythmicCell& destination,
                                      const RhythmicCell& barCell,
                                      const unsigned bar) noexcept {
        const double start = static_cast<double>(bar) / barCount;
        const double end = static_cast<double>(bar + 1u) / barCount;
        for (std::size_t attack = 0; attack < barCell.count; ++attack) {
            if (destination.count >= kMaxRhythmicCellAttacks)
                return false;
            const auto index = destination.count++;
            copyRhythmicAttack(destination, index, barCell, attack);
            destination.positions[index] = start
                + barCell.positions[attack] / barCount;
            if (destination.positions[index] >= end)
                destination.positions[index] = std::nextafter(end, start);
        }
        return true;
    };

    for (unsigned bar = 0; bar < barCount; ++bar) {
        RhythmicCell firstBar;
        RhythmicCell secondBar;
        firstBar.enabled = true;
        secondBar.enabled = true;
        const auto extractBar = [barCount, bar](RhythmicCell& destination,
                const RhythmicCell& source) noexcept {
            const double start = static_cast<double>(bar) / barCount;
            for (std::size_t attack = 0; attack < source.count; ++attack) {
                const auto eventBar = std::min<unsigned>(barCount - 1u,
                    static_cast<unsigned>(source.positions[attack] * barCount));
                if (eventBar != bar)
                    continue;
                const auto index = destination.count++;
                copyRhythmicAttack(destination, index, source, attack);
                destination.positions[index] = std::clamp(
                    (source.positions[attack] - start) * barCount,
                    0.0, std::nextafter(1.0, 0.0));
            }
        };
        extractBar(firstBar, first);
        extractBar(secondBar, second);

        if (firstBar.count >= 2 && secondBar.count >= 2) {
            const auto low = std::min(firstBar.count, secondBar.count);
            const auto high = std::max(firstBar.count, secondBar.count);
            const auto target = static_cast<std::size_t>(low
                + std::max<unsigned>(1u, low / 4u));
            if (high > target) {
                const double keepRatio = static_cast<double>(target) / high;
                const auto barSeed = seed
                    ^ (0x9e3779b9u * (bar + 1u));
                if (firstBar.count > secondBar.count)
                    changed = reduceRhythmicCellDensity(firstBar, keepRatio,
                        barSeed ^ 0x46495253u) || changed;
                else
                    changed = reduceRhythmicCellDensity(secondBar, keepRatio,
                        barSeed ^ 0x5345434fu) || changed;
            }
        }
        if (!appendBar(adaptedFirst, firstBar, bar)
            || !appendBar(adaptedSecond, secondBar, bar))
            return false;
    }
    if (!changed || !validRhythmicCell(adaptedFirst)
        || !validRhythmicCell(adaptedSecond))
        return false;
    first = adaptedFirst;
    second = adaptedSecond;
    return true;
}

bool recombineRhythmicPhraseBars(RhythmicCell& cell,
                                 const RhythmicCell& partner,
                                 const std::uint8_t barCount,
                                 const std::uint32_t seed) noexcept {
    if (!canRecombineRhythmicPhraseBars(cell, partner, barCount))
        return false;

    RhythmicCell phrase;
    phrase.enabled = true;
    bool recombinedAnyBar = false;
    for (unsigned bar = 0; bar < barCount; ++bar) {
        RhythmicCell selectedBar;
        RhythmicCell partnerBar;
        selectedBar.enabled = true;
        partnerBar.enabled = true;
        const double start = static_cast<double>(bar) / barCount;
        const double end = static_cast<double>(bar + 1u) / barCount;
        for (std::size_t attack = 0; attack < cell.count; ++attack) {
            const double position = cell.positions[attack];
            const auto eventBar = std::min<unsigned>(barCount - 1u,
                static_cast<unsigned>(position * barCount));
            if (eventBar != bar)
                continue;
            const auto destination = selectedBar.count++;
            copyRhythmicAttack(selectedBar, destination, cell, attack);
            selectedBar.positions[destination] = std::clamp(
                (position - start) * barCount, 0.0, std::nextafter(1.0, 0.0));
        }
        for (std::size_t attack = 0; attack < partner.count; ++attack) {
            const double position = partner.positions[attack];
            const auto eventBar = std::min<unsigned>(barCount - 1u,
                static_cast<unsigned>(position * barCount));
            if (eventBar != bar)
                continue;
            const auto destination = partnerBar.count++;
            copyRhythmicAttack(partnerBar, destination, partner, attack);
            partnerBar.positions[destination] = std::clamp(
                (position - start) * barCount, 0.0, std::nextafter(1.0, 0.0));
        }

        RhythmicCell barResult = selectedBar;
        if (selectedBar.count >= 2 && partnerBar.count >= 2) {
            const auto barSeed = seed ^ (0x9e3779b9u * (bar + 1u));
            if (recombineRhythmicCell(barResult, partnerBar, barSeed))
                recombinedAnyBar = true;
        }
        if (!selectedBar.count)
            continue;
        if (phrase.count + barResult.count > kMaxRhythmicCellAttacks)
            return false;
        for (std::size_t attack = 0; attack < barResult.count; ++attack) {
            const auto destination = phrase.count++;
            copyRhythmicAttack(phrase, destination, barResult, attack);
            phrase.positions[destination] = start
                + barResult.positions[attack] / barCount;
            if (phrase.positions[destination] >= end)
                phrase.positions[destination] = std::nextafter(end, start);
        }
    }
    if (!recombinedAnyBar || phrase.count == 0 || !validRhythmicCell(phrase))
        return false;

    bool changed = phrase.count != cell.count;
    for (std::size_t attack = 0; !changed && attack < phrase.count; ++attack)
        changed = std::abs(phrase.positions[attack] - cell.positions[attack])
                > kEpsilon
            || std::abs(phrase.strengths[attack] - cell.strengths[attack])
                > 1.0e-7f
            || phrase.midiNotes[attack] != cell.midiNotes[attack]
            || phrase.midiHatPedal[attack] != cell.midiHatPedal[attack]
            || phrase.midiCollisionCount[attack]
                != cell.midiCollisionCount[attack]
            || phrase.midiCollisionNotes[attack]
                != cell.midiCollisionNotes[attack]
            || phrase.midiCollisionStrengths[attack]
                != cell.midiCollisionStrengths[attack]
            || phrase.midiCollisionHatPedal[attack]
                != cell.midiCollisionHatPedal[attack];
    if (!changed)
        return false;
    cell = phrase;
    return true;
}

bool shapeRhythmicCellDynamics(RhythmicCell& cell,
                               const RhythmicDynamicsShape shape,
                               const std::uint32_t seed) noexcept {
    if (!cell.enabled || !validRhythmicCell(cell) || cell.count == 0
        || (shape != RhythmicDynamicsShape::crescendo
            && shape != RhythmicDynamicsShape::diminuendo
            && shape != RhythmicDynamicsShape::alternating))
        return false;

    std::uint32_t phase = seed;
    phase ^= phase >> 16u;
    phase *= 0x7feb352du;
    phase ^= phase >> 15u;
    const unsigned alternatingPhase = phase & 1u;
    constexpr float contourBlend = 0.45f;
    bool changed = false;
    for (std::size_t attack = 0; attack < cell.count; ++attack) {
        float target = 0.0f;
        const float progress = static_cast<float>(cell.positions[attack]);
        switch (shape) {
            case RhythmicDynamicsShape::crescendo:
                target = 0.25f + 0.75f * progress;
                break;
            case RhythmicDynamicsShape::diminuendo:
                target = 1.0f - 0.75f * progress;
                break;
            case RhythmicDynamicsShape::alternating:
                target = ((attack + alternatingPhase) & 1u) == 0u
                    ? 0.92f : 0.38f;
                break;
        }
        const auto blend = [target, contourBlend](const float strength) {
            return std::clamp((1.0f - contourBlend) * strength
                + contourBlend * target, 0.0f, 1.0f);
        };
        const float main = blend(cell.strengths[attack]);
        changed = changed || std::abs(main - cell.strengths[attack]) > 1.0e-7f;
        cell.strengths[attack] = main;
        for (std::size_t collision = 0;
             collision < cell.midiCollisionCount[attack]; ++collision) {
            const float next = blend(cell.midiCollisionStrengths[attack][collision]);
            changed = changed || std::abs(next
                - cell.midiCollisionStrengths[attack][collision]) > 1.0e-7f;
            cell.midiCollisionStrengths[attack][collision] = next;
        }
    }
    return changed && validRhythmicCell(cell);
}

bool varyRhythmicCellMicrotiming(RhythmicCell& cell,
                                 const double maximumOffsetTurns,
                                 const std::uint32_t seed) noexcept {
    if (!cell.enabled || !validRhythmicCell(cell) || cell.count == 0
        || !std::isfinite(maximumOffsetTurns)
        || maximumOffsetTurns <= 0.0 || maximumOffsetTurns > 0.05)
        return false;

    constexpr double minimumGap = 1.0 / 1024.0;
    auto varied = cell;
    double previousDeviation = 0.0;
    bool changed = false;
    for (std::size_t attack = 0; attack < cell.count; ++attack) {
        std::uint32_t hash = seed
            ^ (0x9e3779b9u * static_cast<std::uint32_t>(attack + 1))
            ^ (static_cast<std::uint32_t>(cell.midiNotes[attack]) << 24u);
        hash ^= hash >> 16u;
        hash *= 0x7feb352du;
        hash ^= hash >> 15u;
        hash *= 0x846ca68bu;
        hash ^= hash >> 16u;
        const double whiteNoise = static_cast<double>(hash & 0xffffu)
            / 32767.5 - 1.0;
        const double deviation = attack == 0 ? whiteNoise
            : std::clamp(0.62 * previousDeviation + 0.38 * whiteNoise,
                         -1.0, 1.0);
        previousDeviation = deviation;
        const double position = cell.positions[attack];
        const double previousGapSlack = attack == 0 ? position
            : std::max(0.0, position - cell.positions[attack - 1] - minimumGap);
        const double nextGapSlack = attack + 1 == cell.count
            ? std::max(0.0, 1.0 - minimumGap - position)
            : std::max(0.0, cell.positions[attack + 1] - position - minimumGap);
        const double strength = std::clamp(
            static_cast<double>(cell.strengths[attack]), 0.0, 1.0);
        const double movementScale = 0.40 + 0.60 * (1.0 - strength);
        const double negativeLimit = std::min({maximumOffsetTurns * movementScale,
            previousGapSlack * (attack == 0 ? 1.0 : 0.5), position});
        const double positiveLimit = std::min({maximumOffsetTurns * movementScale,
            nextGapSlack * (attack + 1 == cell.count ? 1.0 : 0.5),
            1.0 - minimumGap - position});
        const double requestedOffset = deviation * maximumOffsetTurns
            * movementScale;
        const double offset = requestedOffset < 0.0
            ? std::max(requestedOffset, -negativeLimit)
            : std::min(requestedOffset, positiveLimit);
        varied.positions[attack] = position + offset;
        changed = changed || std::abs(offset) > kEpsilon;
    }
    if (!changed || !validRhythmicCell(varied))
        return false;
    cell = varied;
    return true;
}

bool setRhythmicCellStrength(RhythmicCell& cell, const std::size_t index,
                             const float strength) noexcept {
    if (!cell.enabled || !validRhythmicCell(cell) || index >= cell.count
        || !std::isfinite(strength) || strength < 0.0f || strength > 1.0f)
        return false;
    cell.strengths[index] = strength;
    return true;
}

PlanError buildEventPlan(const CompositionRecipe& recipe, EventPlan& plan,
                         const bool sortChronologically) {
    plan.events.clear();
    if (!finite(recipe.sectionSeconds) || recipe.sectionSeconds <= 0.0
        || recipe.sectionSeconds > kMaximumSectionSeconds)
        return PlanError::invalidSectionDuration;
    if (recipe.voiceCount == 0 || recipe.voiceCount > kMaxVoices)
        return PlanError::invalidVoiceCount;

    std::size_t reserveCount = 0;
    for (std::size_t voiceIndex = 0; voiceIndex < recipe.voiceCount; ++voiceIndex) {
        const auto& voice = recipe.voices[voiceIndex];
        if (!validVoice(voice))
            return PlanError::invalidVoiceParameters;
        const auto& cell = recipe.rhythmicCells[voiceIndex];
        if (!validRhythmicCell(cell))
            return PlanError::invalidRhythmicCell;
        std::size_t perVoice = cell.enabled ? static_cast<std::size_t>(cell.count)
            : static_cast<std::size_t>(2.0 * std::ceil(voice.cyclesPerSection) + 2.0);
        if (cell.enabled)
            for (std::size_t attack = 0; attack < cell.count; ++attack)
                perVoice += cell.midiCollisionCount[attack];
        if (reserveCount > kMaxEventsPerSection - perVoice)
            return PlanError::eventLimitExceeded;
        reserveCount += perVoice;
    }
    if (reserveCount > kMaxEventsPerSection)
        return PlanError::eventLimitExceeded;

    plan.events.reserve(reserveCount);
    MusicalEvent closestFirstVoiceEvent{};
    double closestFirstVoiceDistance = std::numeric_limits<double>::infinity();
    bool hasFirstVoiceEvent = false;
    for (std::size_t voiceIndex = 0; voiceIndex < recipe.voiceCount; ++voiceIndex) {
        const auto& voice = recipe.voices[voiceIndex];
        const auto& cell = recipe.rhythmicCells[voiceIndex];
        if (cell.enabled) {
            for (std::size_t attack = 0; attack < cell.count; ++attack) {
                const double sectionPosition = cell.positions[attack];
                const double x = std::sin(kTau * (recipe.voices[0].cyclesPerSection
                    * sectionPosition + recipe.voices[0].phaseCycles));
                const double y = std::sin(kTau * (recipe.voices[1].cyclesPerSection
                    * sectionPosition + recipe.voices[1].phaseCycles));
                MusicalEvent event;
                event.timeSeconds = sectionPosition * recipe.sectionSeconds;
                event.phaseCycles = wrapUnitCycle(voice.cyclesPerSection
                    * sectionPosition + voice.phaseCycles);
                event.voice = static_cast<std::uint8_t>(voiceIndex);
                event.midiNote = cell.midiNotes[attack];
                event.midiHatPedal = cell.midiHatPedal[attack];
                event.rule = voice.eventRule;
                event.intensity = cell.strengths[attack];
                event.fieldX = static_cast<float>(x);
                event.fieldY = static_cast<float>(y);
                // Retain written attacks; the moving field gates them at
                // playback time, after its current pose has been evaluated.
                if (voiceIndex == 0)
                    hasFirstVoiceEvent = true;
                plan.events.push_back(event);
                for (std::size_t collision = 0;
                     collision < cell.midiCollisionCount[attack]; ++collision) {
                    auto layeredEvent = event;
                    layeredEvent.soundLayer = static_cast<std::uint8_t>(collision + 1);
                    layeredEvent.midiNote = cell.midiCollisionNotes[attack][collision];
                    layeredEvent.midiHatPedal =
                        cell.midiCollisionHatPedal[attack][collision];
                    layeredEvent.intensity =
                        cell.midiCollisionStrengths[attack][collision];
                    plan.events.push_back(layeredEvent);
                }
            }
            continue;
        }
        const double cycles = voice.cyclesPerSection;
        const double phase = wrapUnitCycle(voice.phaseCycles);
        const std::array<double, 2> eventPhases = voice.eventRule
            == EventRule::centreCrossings
                ? std::array<double, 2>{0.0, 0.5}
                : std::array<double, 2>{0.25, 0.75};

        for (const double eventPhase : eventPhases) {
            const auto firstIndex = static_cast<std::int64_t>(
                std::ceil(phase - eventPhase - kEpsilon));
            const double endIndex = cycles + phase - eventPhase;
            for (std::int64_t cycle = firstIndex;
                 static_cast<double>(cycle) < endIndex - kEpsilon;
                 ++cycle) {
                const double sectionPosition =
                    (static_cast<double>(cycle) + eventPhase - phase) / cycles;
                if (sectionPosition < -kEpsilon || sectionPosition >= 1.0 - kEpsilon)
                    continue;
                if (plan.events.size() >= kMaxEventsPerSection) {
                    plan.events.clear();
                    return PlanError::eventLimitExceeded;
                }

                MusicalEvent event;
                event.timeSeconds = std::max(0.0, sectionPosition)
                    * recipe.sectionSeconds;
                event.phaseCycles = eventPhase;
                event.voice = static_cast<std::uint8_t>(voiceIndex);
                event.rule = voice.eventRule;
                event.intensity = 1.0f;
                const auto phaseAtEvent = [&](const VoiceRecipe& axisVoice) {
                    return kTau * (axisVoice.cyclesPerSection * sectionPosition
                                   + axisVoice.phaseCycles);
                };
                const double x = std::sin(phaseAtEvent(recipe.voices[0]));
                const double y = std::sin(phaseAtEvent(recipe.voices[1]));
                event.fieldX = static_cast<float>(x);
                event.fieldY = static_cast<float>(y);
                if (voiceIndex == 0 && recipe.patternMode != PatternMode::unrestricted) {
                    const double distance = distanceToPattern(recipe.patternMode, x, y);
                    if (distance < closestFirstVoiceDistance) {
                        closestFirstVoiceDistance = distance;
                        closestFirstVoiceEvent = event;
                    }
                }
                if (!allowedByPattern(recipe.patternMode, x, y))
                    continue;
                if (voiceIndex == 0)
                    hasFirstVoiceEvent = true;
                plan.events.push_back(event);
            }
        }
    }

    // A narrow spatial mask can reject every crossing of V1, which is the
    // only active voice in two formal cycles. Admit one subdued nearest miss
    // without changing the selected mask or the seed's other events.
    if (!hasFirstVoiceEvent && recipe.patternMode != PatternMode::unrestricted
        && std::isfinite(closestFirstVoiceDistance)
        && plan.events.size() < kMaxEventsPerSection) {
        closestFirstVoiceEvent.intensity = 0.4f;
        plan.events.push_back(closestFirstVoiceEvent);
    }

    if (sortChronologically)
        std::sort(plan.events.begin(), plan.events.end(),
                  [](const MusicalEvent& a, const MusicalEvent& b) {
                      if (a.timeSeconds != b.timeSeconds)
                          return a.timeSeconds < b.timeSeconds;
                      return a.voice < b.voice;
                  });
    return PlanError::none;
}

bool invertEventPlanVoices(const CompositionRecipe& recipe,
                           const EventPlan& source,
                           const std::size_t firstVoice,
                           const std::size_t secondVoice,
                           EventPlan& inverted) {
    if (!finite(recipe.sectionSeconds) || recipe.sectionSeconds <= 0.0
        || recipe.sectionSeconds > kMaximumSectionSeconds
        || recipe.voiceCount == 0 || recipe.voiceCount > kMaxVoices
        || firstVoice >= recipe.voiceCount || secondVoice >= recipe.voiceCount
        || firstVoice == secondVoice
        || source.events.size() > kMaxEventsPerSection)
        return false;
    for (const auto& event : source.events) {
        if (event.voice >= recipe.voiceCount || !finite(event.timeSeconds)
            || event.timeSeconds < 0.0 || event.timeSeconds >= recipe.sectionSeconds
            || !finite(event.intensity) || event.intensity < 0.0f
            || event.intensity > 1.0f || !finite(event.fieldX)
            || !finite(event.fieldY) || !finite(event.phaseCycles))
            return false;
    }

    // Build a separate result so a failed request never changes either plan.
    EventPlan prepared = source;
    for (auto& event : prepared.events) {
        if (event.voice == firstVoice)
            event.voice = static_cast<std::uint8_t>(secondVoice);
        else if (event.voice == secondVoice)
            event.voice = static_cast<std::uint8_t>(firstVoice);
    }
    std::stable_sort(prepared.events.begin(), prepared.events.end(),
                     [](const MusicalEvent& a, const MusicalEvent& b) {
                         if (a.timeSeconds != b.timeSeconds)
                             return a.timeSeconds < b.timeSeconds;
                         return a.voice < b.voice;
                     });
    inverted = std::move(prepared);
    return true;
}

PendulumError buildCoupledPendulumTrajectory(
    const CoupledPendulumParameters& parameters,
    CoupledPendulumTrajectory& trajectory) {
    trajectory.samples.clear();
    if (!finite(parameters.durationSeconds) || parameters.durationSeconds <= 0.0
        || parameters.durationSeconds > kMaximumSectionSeconds
        || !finite(parameters.sampleStepSeconds)
        || parameters.sampleStepSeconds < 0.0001
        || parameters.sampleStepSeconds > 0.02
        || !finite(parameters.gravity) || parameters.gravity <= 0.0
        || !finite(parameters.lengthMetres) || parameters.lengthMetres <= 0.0
        || !finite(parameters.massKilograms) || parameters.massKilograms <= 0.0
        || !finite(parameters.springConstantNewtonsPerMetre)
        || parameters.springConstantNewtonsPerMetre < 0.0
        || !finite(parameters.dampingPerSecond) || parameters.dampingPerSecond < 0.0)
        return PendulumError::invalidParameters;
    for (std::size_t i = 0; i < 3; ++i)
        if (!finite(parameters.initialAnglesRadians[i])
            || std::abs(parameters.initialAnglesRadians[i]) > 1.0
            || !finite(parameters.initialVelocitiesRadiansPerSecond[i])
            || std::abs(parameters.initialVelocitiesRadiansPerSecond[i]) > 20.0)
            return PendulumError::invalidParameters;

    const auto requestedSamples = static_cast<std::size_t>(
        std::ceil(parameters.durationSeconds / parameters.sampleStepSeconds)) + 1;
    if (requestedSamples > kMaximumPendulumSamples)
        return PendulumError::sampleLimitExceeded;

    using State = std::array<double, 6>; // θ₁..θ₃, then ω₁..ω₃.
    const auto derivative = [&parameters](const State& state) {
        State result{};
        const double gravityTerm = parameters.gravity / parameters.lengthMetres;
        const double springTerm = parameters.springConstantNewtonsPerMetre
            / parameters.massKilograms;
        for (std::size_t i = 0; i < 3; ++i) {
            const double angle = state[i];
            const double velocity = state[i + 3];
            double springAcceleration = 0.0;
            if (i > 0)
                springAcceleration += springTerm * (state[i - 1] - angle);
            if (i + 1 < 3)
                springAcceleration += springTerm * (state[i + 1] - angle);
            result[i] = velocity;
            result[i + 3] = -gravityTerm * angle + springAcceleration
                - parameters.dampingPerSecond * velocity;
        }
        return result;
    };
    const auto addScaled = [](const State& state, const State& derivativeValue,
                              const double scale) {
        State result{};
        for (std::size_t i = 0; i < result.size(); ++i)
            result[i] = state[i] + derivativeValue[i] * scale;
        return result;
    };
    const auto sampleState = [&parameters](const State& state, const double time) {
        CoupledPendulumSample sample;
        sample.timeSeconds = time;
        double kinetic = 0.0;
        double gravitational = 0.0;
        for (std::size_t i = 0; i < 3; ++i) {
            sample.anglesRadians[i] = state[i];
            sample.angularVelocitiesRadiansPerSecond[i] = state[i + 3];
            const double pendulumKinetic = 0.5 * parameters.massKilograms
                * parameters.lengthMetres * parameters.lengthMetres
                * state[i + 3] * state[i + 3];
            const double pendulumGravitational = 0.5 * parameters.massKilograms
                * parameters.gravity * parameters.lengthMetres * state[i] * state[i];
            sample.kineticEnergyByPendulumJoules[i] = pendulumKinetic;
            sample.potentialEnergyByPendulumJoules[i] = pendulumGravitational;
            kinetic += pendulumKinetic;
            gravitational += pendulumGravitational;
        }
        double spring = 0.0;
        for (std::size_t i = 0; i < 2; ++i) {
            const double displacement = parameters.lengthMetres
                * (state[i + 1] - state[i]);
            const double springEnergy = 0.5 * parameters.springConstantNewtonsPerMetre
                * displacement * displacement;
            spring += springEnergy;
            sample.potentialEnergyByPendulumJoules[i] += 0.5 * springEnergy;
            sample.potentialEnergyByPendulumJoules[i + 1] += 0.5 * springEnergy;
        }
        sample.kineticEnergyJoules = kinetic;
        sample.potentialEnergyJoules = gravitational + spring;
        return sample;
    };

    State state{};
    for (std::size_t i = 0; i < 3; ++i) {
        state[i] = parameters.initialAnglesRadians[i];
        state[i + 3] = parameters.initialVelocitiesRadiansPerSecond[i];
    }
    trajectory.samples.reserve(requestedSamples);
    trajectory.samples.push_back(sampleState(state, 0.0));
    double time = 0.0;
    while (time < parameters.durationSeconds) {
        const double step = std::min(parameters.sampleStepSeconds,
                                     parameters.durationSeconds - time);
        const auto k1 = derivative(state);
        const auto k2 = derivative(addScaled(state, k1, 0.5 * step));
        const auto k3 = derivative(addScaled(state, k2, 0.5 * step));
        const auto k4 = derivative(addScaled(state, k3, step));
        for (std::size_t i = 0; i < state.size(); ++i) {
            state[i] += step * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]) / 6.0;
            if (!finite(state[i])) {
                trajectory.samples.clear();
                return PendulumError::nonFiniteState;
            }
        }
        time += step;
        trajectory.samples.push_back(sampleState(state, time));
    }
    return PendulumError::none;
}

PendulumError buildPendulumEventPlan(const CoupledPendulumTrajectory& trajectory,
                                    const double cycleSeconds,
                                    const EventRule rule,
                                    std::array<EventPlan, kFormalCycleCount>& plans) {
    for (auto& plan : plans)
        plan.events.clear();
    if (trajectory.samples.size() < 2 || !finite(cycleSeconds)
        || cycleSeconds <= 0.0)
        return PendulumError::invalidTrajectory;
    const double formDuration = cycleSeconds * static_cast<double>(kFormalCycleCount);
    if (!finite(formDuration)
        || trajectory.samples.back().timeSeconds + 1.0e-9 < formDuration)
        return PendulumError::invalidTrajectory;

    double maximumEnergy = 0.0;
    std::array<double, 3> maximumEnergyByPendulum{};
    double previousSampleTime = -1.0;
    for (const auto& sample : trajectory.samples) {
        if (!finite(sample.timeSeconds) || sample.timeSeconds <= previousSampleTime
            || !finite(sample.kineticEnergyJoules)
            || !finite(sample.potentialEnergyJoules)
            || sample.kineticEnergyJoules < 0.0
            || sample.potentialEnergyJoules < 0.0)
            return PendulumError::invalidTrajectory;
        previousSampleTime = sample.timeSeconds;
        const double totalEnergy = sample.kineticEnergyJoules
            + sample.potentialEnergyJoules;
        if (!finite(totalEnergy))
            return PendulumError::invalidTrajectory;
        maximumEnergy = std::max(maximumEnergy, totalEnergy);
        double localEnergySum = 0.0;
        for (std::size_t voice = 0; voice < 3; ++voice)
            if (!finite(sample.anglesRadians[voice])
                || !finite(sample.angularVelocitiesRadiansPerSecond[voice]))
                return PendulumError::invalidTrajectory;
            else {
                const double localEnergy = sample.kineticEnergyByPendulumJoules[voice]
                    + sample.potentialEnergyByPendulumJoules[voice];
                if (!finite(localEnergy) || localEnergy < 0.0)
                    return PendulumError::invalidTrajectory;
                localEnergySum += localEnergy;
                maximumEnergyByPendulum[voice] = std::max(
                    maximumEnergyByPendulum[voice], localEnergy);
            }
        if (std::abs(localEnergySum - totalEnergy)
            > 1.0e-8 * std::max(1.0, totalEnergy))
            return PendulumError::invalidTrajectory;
    }
    if (!(maximumEnergy > 0.0))
        return PendulumError::invalidTrajectory;

    for (auto& plan : plans)
        plan.events.reserve(std::min<std::size_t>(
            kMaxEventsPerSection / kFormalCycleCount, trajectory.samples.size() / 7));
    for (std::size_t voice = 0; voice < 3; ++voice) {
        for (std::size_t sample = 1; sample < trajectory.samples.size(); ++sample) {
            const auto& before = trajectory.samples[sample - 1];
            const auto& after = trajectory.samples[sample];
            const double a = rule == EventRule::centreCrossings
                ? before.anglesRadians[voice]
                : before.angularVelocitiesRadiansPerSecond[voice];
            const double b = rule == EventRule::centreCrossings
                ? after.anglesRadians[voice]
                : after.angularVelocitiesRadiansPerSecond[voice];
            if (!((a < 0.0 && b >= 0.0) || (a > 0.0 && b <= 0.0)))
                continue;
            const double fraction = std::clamp(
                std::abs(a) / (std::abs(a) + std::abs(b)), 0.0, 1.0);
            const double time = before.timeSeconds
                + fraction * (after.timeSeconds - before.timeSeconds);
            if (time >= formDuration)
                continue;
            const auto cycle = static_cast<std::size_t>(time / cycleSeconds);
            if (cycle >= kFormalCycleCount)
                continue;
            auto& plan = plans[cycle];
            if (plan.events.size() >= kMaxEventsPerSection) {
                for (auto& item : plans)
                    item.events.clear();
                return PendulumError::sampleLimitExceeded;
            }
            const auto interpolate = [fraction](const double x, const double y) {
                return x + fraction * (y - x);
            };
            const double thetaOne = interpolate(before.anglesRadians[0],
                                                after.anglesRadians[0]);
            const double thetaTwo = interpolate(before.anglesRadians[1],
                                                after.anglesRadians[1]);
            const double localEnergy = interpolate(
                before.kineticEnergyByPendulumJoules[voice]
                    + before.potentialEnergyByPendulumJoules[voice],
                after.kineticEnergyByPendulumJoules[voice]
                    + after.potentialEnergyByPendulumJoules[voice]);
            MusicalEvent event;
            event.timeSeconds = time - static_cast<double>(cycle) * cycleSeconds;
            event.voice = static_cast<std::uint8_t>(voice);
            event.rule = rule;
            const double normalizedEnergy = maximumEnergyByPendulum[voice] > 0.0
                ? std::clamp(localEnergy / maximumEnergyByPendulum[voice], 0.0, 1.0)
                : 0.0;
            event.intensity = static_cast<float>(0.35 + 0.65 * normalizedEnergy);
            event.fieldX = static_cast<float>(std::sin(thetaOne));
            event.fieldY = static_cast<float>(std::sin(thetaTwo));
            plan.events.push_back(event);
        }
    }
    for (auto& plan : plans)
        std::sort(plan.events.begin(), plan.events.end(),
                  [](const MusicalEvent& a, const MusicalEvent& b) {
                      if (a.timeSeconds != b.timeSeconds)
                          return a.timeSeconds < b.timeSeconds;
                      return a.voice < b.voice;
                  });
    return PendulumError::none;
}

bool ResonatorRenderer::configure(const CompositionRecipe& recipe,
                                  const EventPlan& plan,
                                  const double sampleRate) noexcept {
    return configureVariants(&recipe, &plan, 1, sampleRate);
}

bool ResonatorRenderer::configureVariants(const CompositionRecipe& recipe,
                                          const EventPlan* plans,
                                          const std::size_t planCount,
                                          const double sampleRate) noexcept {
    if (planCount == 0 || planCount > kMaxPreparedVariants)
        return false;
    try {
        // The pointer overload expects one recipe per plan. Repeating the
        // single recipe here keeps that contract without reading past it.
        std::vector<CompositionRecipe> repeated(planCount, recipe);
        return configureVariants(repeated.data(), plans, planCount, sampleRate);
    } catch (...) {
        return false;
    }
}

bool ResonatorRenderer::configureVariants(const CompositionRecipe* recipes,
                                          const EventPlan* plans,
                                          const std::size_t planCount,
                                          const double sampleRate) noexcept {
    ready_ = false;
    if (!finite(sampleRate) || sampleRate < 8'000.0 || sampleRate > 192'000.0
        || recipes == nullptr || plans == nullptr || planCount == 0
        || planCount > kMaxPreparedVariants
        || recipes[0].voiceCount == 0 || recipes[0].voiceCount > kMaxVoices
        || !finite(recipes[0].sectionSeconds) || recipes[0].sectionSeconds <= 0.0
        || recipes[0].sectionSeconds > kMaximumSectionSeconds)
        return false;
    const auto& recipe = recipes[0];
    std::size_t totalEvents = 0;
    for (std::size_t variant = 0; variant < planCount; ++variant) {
        const auto& variantRecipe = recipes[variant];
        if (variantRecipe.voiceCount == 0 || variantRecipe.voiceCount > kMaxVoices
            || !finite(variantRecipe.sectionSeconds)
            || std::abs(variantRecipe.sectionSeconds - recipe.sectionSeconds) > 1.0e-9)
            return false;
        for (std::size_t voice = 0; voice < variantRecipe.voiceCount; ++voice)
            if (!validVoice(variantRecipe.voices[voice]))
                return false;
        if (plans[variant].events.size() > kMaxEventsPerSection)
            return false;
        if (plans[variant].events.size() > kMaxPreparedEventsTotal - totalEvents)
            return false;
        totalEvents += plans[variant].events.size();
        for (const auto& event : plans[variant].events)
            if (event.voice >= variantRecipe.voiceCount || !finite(event.timeSeconds)
                || event.timeSeconds < 0.0 || event.timeSeconds >= recipe.sectionSeconds
                || !finite(event.intensity) || event.intensity < 0.0f
                || event.intensity > 1.0f)
                return false;
    }

    recipe_ = recipe;
    sampleRate_ = sampleRate;
    for (std::size_t index = 0; index <= kSineTableSize; ++index)
        sineTable_[index] = static_cast<float>(std::sin(
            kTau * static_cast<double>(index) / static_cast<double>(kSineTableSize)));
    channelSlew_ = static_cast<float>(1.0 - std::exp(-1.0 / (0.005 * sampleRate_)));
    baseSectionSamples_ = static_cast<std::uint64_t>(
        std::llround(recipe.sectionSeconds * sampleRate));
    if (baseSectionSamples_ == 0)
        return false;
    sectionSamples_ = baseSectionSamples_;
    swingRatio_ = 0.5;
    requestedSwingRatio_.store(0.5, std::memory_order_relaxed);
    meterNumerator_ = 4;
    meterDenominator_ = 4;
    quarterNotesPerSection_ = 16.0;
    requestedTimeSignature_.store(404, std::memory_order_relaxed);
    grooveMode_ = GrooveMode::straight;
    requestedGrooveMode_.store(GrooveMode::straight, std::memory_order_relaxed);
    dynamicCurve_ = DynamicCurve::arch;
    musicalIntention_ = MusicalIntention::meet;
    requestedDynamicCurve_.store(DynamicCurve::arch, std::memory_order_relaxed);
    requestedMusicalIntention_.store(MusicalIntention::meet,
                                     std::memory_order_relaxed);

    schedules_.clear();
    variantTimings_.clear();
    try {
        schedules_.resize(planCount);
        variantTimings_.resize(planCount);
        for (std::size_t variant = 0; variant < planCount; ++variant) {
            const auto& variantRecipe = recipes[variant];
            auto& timing = variantTimings_[variant];
            timing.patternMode = variantRecipe.patternMode;
            for (std::size_t voice = 0; voice < variantRecipe.voiceCount; ++voice) {
                timing.cycles[voice] = variantRecipe.voices[voice].cyclesPerSection;
                timing.phases[voice] = wrapUnitCycle(variantRecipe.voices[voice].phaseCycles);
                timing.rules[voice] = variantRecipe.voices[voice].eventRule;
            }
            auto& schedule = schedules_[variant];
            schedule.reserve(plans[variant].events.size());
            for (const auto& event : plans[variant].events) {
                const auto sectionPhase = static_cast<std::uint64_t>(std::llround(
                    event.timeSeconds / recipe.sectionSeconds
                    * static_cast<double>(kSectionPhaseScale)));
                schedule.push_back({std::min(sectionPhase, kSectionPhaseScale - 1), event});
            }
            std::stable_sort(schedule.begin(), schedule.end(),
                             [](const ScheduledEvent& a, const ScheduledEvent& b) {
                                 return a.sectionPhaseQ32 < b.sectionPhaseQ32;
                             });
        }
    } catch (...) {
        schedules_.clear();
        variantTimings_.clear();
        return false;
    }

    for (auto& voiceStates : states_)
        for (auto& state : voiceStates)
            state = {};
    currentChannelLevels_.fill(1.0f);
    decayScale_ = 1.0f;
    fundamentalLevel_ = 1.0f;
    channelSounds_.fill(SoundCharacter::triangle);
    chip8Parameters_.fill(Chip8Parameters{});
    voiceTransforms_.fill(VoiceTransform::sine);
    voiceCouplings_.fill(0.0f);
    voiceFormulaPhaseOffsets_.fill(0.0f);
    requestedVoiceCycles_.fill(0.0f);
    requestedVoicePhaseOffsets_.fill(0.0f);
    requestedVoiceEventRules_.fill(-1);
    requestedRhythmicCells_ = recipe.rhythmicCells;
    activeRhythmicCells_ = recipe.rhythmicCells;
    activeDynamicRhythm_.fill(false);
    dynamicEventStreamCounts_.fill(0);
    nextDynamicRhythmSample_ = std::numeric_limits<std::uint64_t>::max();
    activeGeometryEventCounts_.fill(0);
    activeGeometryEventTiming_ = false;
    for (std::size_t i = 0; i < recipe_.voiceCount; ++i) {
        for (std::size_t layer = 0; layer < kMaxMidiNotesPerAttack; ++layer) {
            activeVoiceSounds_[i][layer] = channelSounds_[i];
            refreshVoiceSpectrum(i, layer);
            states_[i][layer].noiseState = static_cast<std::uint32_t>(
                0x9E3779B9u ^ (i * 0x85EBCA6Bu)
                ^ (static_cast<std::uint32_t>(layer) * 0xC2B2AE35u));
        }
        const auto& voice = recipe_.voices[i];
        panBase_[i] = finiteClamp(voice.pan, -1.0f, 1.0f);
        panBaseLeft_[i] = std::sqrt(0.5f * (1.0f - panBase_[i]));
        panBaseRight_[i] = std::sqrt(0.5f * (1.0f + panBase_[i]));
        panLeft_[i] = panTargetLeft_[i] = panBaseLeft_[i];
        panRight_[i] = panTargetRight_[i] = panBaseRight_[i];
        panMotionDepth_[i] = 0.0f;
        panTrajectories_[i] = {};
        stemVariationDepth_[i] = 1.0f;
        for (std::size_t layer = 0; layer < kMaxMidiNotesPerAttack; ++layer)
            for (std::size_t mode = 0; mode < kModesPerVoice; ++mode) {
                const double ratio = modeFrequencyRatios_[i][layer][mode];
                const double lossScale = 1.0 + voice.resonator.frequencyDependentLoss
                    * ratio * ratio;
                const double modeDecay = voice.resonator.decaySeconds / lossScale;
                decayCoefficients_[i][layer][mode] = static_cast<float>(
                    std::exp(-1.0 / (modeDecay * sampleRate_)));
            }
    }
    sectionPosition_ = 0;
    absoluteSample_ = 0;
    completedBars_ = 0;
    inFormalPause_ = false;
    dynamicFirstVoiceAnchorFired_ = false;
    dynamicFirstVoiceAnchorSample_ = std::numeric_limits<std::uint64_t>::max();
    requestedVoiceCount_ = std::min<std::size_t>(2, recipe_.voiceCount);
    activeVoiceCount_ = 1;
    renderVoiceCount_ = activeVoiceCount_;
    publishedSamples_.store(0, std::memory_order_relaxed);
    publishedBars_.store(0, std::memory_order_relaxed);
    publishedSectionSamples_.store(sectionSamples_, std::memory_order_relaxed);
    publishedSectionProgress_.store(0.0, std::memory_order_relaxed);
    publishedVariant_.store(0, std::memory_order_relaxed);
    smoothedVoiceRms_.fill(0.0f);
    for (auto& meter : publishedVoiceRms_)
        meter.store(0.0f, std::memory_order_relaxed);
    nextEvent_ = 0;
    activeVariant_ = 0;
    ready_ = true;
    refreshRhythmState(0);
    return true;
}

void ResonatorRenderer::reset() noexcept {
    for (std::size_t i = 0; i < states_.size(); ++i) {
        for (std::size_t layer = 0; layer < kMaxMidiNotesPerAttack; ++layer) {
            states_[i][layer] = {};
            states_[i][layer].noiseState = static_cast<std::uint32_t>(
                0x9E3779B9u ^ (i * 0x85EBCA6Bu)
                ^ (static_cast<std::uint32_t>(layer) * 0xC2B2AE35u));
        }
    }
    sectionPosition_ = 0;
    sectionSamples_ = baseSectionSamples_;
    dynamicFirstVoiceAnchorFired_ = false;
    dynamicFirstVoiceAnchorSample_ = std::numeric_limits<std::uint64_t>::max();
    currentChannelLevels_.fill(1.0f);
    absoluteSample_ = 0;
    completedBars_ = 0;
    inFormalPause_ = false;
    applyFormalVoiceCount();
    publishedSamples_.store(0, std::memory_order_relaxed);
    publishedBars_.store(0, std::memory_order_relaxed);
    publishedSectionSamples_.store(sectionSamples_, std::memory_order_relaxed);
    publishedSectionProgress_.store(0.0, std::memory_order_relaxed);
    publishedVariant_.store(0, std::memory_order_relaxed);
    smoothedVoiceRms_.fill(0.0f);
    for (auto& meter : publishedVoiceRms_)
        meter.store(0.0f, std::memory_order_relaxed);
    nextEvent_ = 0;
    activeVariant_ = 0;
    renderVoiceCount_ = activeVoiceCount_;
    refreshRhythmState(0);
}

void ResonatorRenderer::setActiveVoiceCount(const std::size_t count) noexcept {
    requestedVoiceCount_ = std::clamp<std::size_t>(count, 2,
        std::max<std::size_t>(2, recipe_.voiceCount));
    applyFormalVoiceCount();
}

void ResonatorRenderer::setFormalVoiceReductionEnabled(const bool enabled) noexcept {
    if (formalVoiceReductionEnabled_ == enabled)
        return;
    formalVoiceReductionEnabled_ = enabled;
    applyFormalVoiceCount();
}

void ResonatorRenderer::setRhythmMode(const bool enabled) noexcept {
    if (rhythmMode_ == enabled)
        return;
    rhythmMode_ = enabled;
    applyFormalVoiceCount();
}

void ResonatorRenderer::setGenerativeDepth(const float depth) noexcept {
    const float next = std::isfinite(depth) ? std::clamp(depth, 0.0f, 1.0f) : 0.3f;
    if (next == generativeDepth_)
        return;
    const bool crossed = (next >= 0.999f) != (generativeDepth_ >= 0.999f);
    generativeDepth_ = next;
    if (crossed)
        applyFormalVoiceCount();
}

void ResonatorRenderer::setPercussionKey(const int tonic, const int scale) noexcept {
    percussionKeyTonic_ = tonic >= 0 && tonic <= 11 ? tonic : -1;
    percussionKeyScale_ = std::clamp(scale, 0, kBassKeyScaleCount - 1);
}

double ResonatorRenderer::keyTunedPitchScale(const SoundCharacter sound,
                                             const double fundamentalHz) const noexcept {
    if (percussionKeyTonic_ < 0 || !(fundamentalHz > 1.0) || !std::isfinite(fundamentalHz))
        return 1.0;
    bool tuned = false;
    bool rootAndFifthOnly = false;
    switch (sound) {
        case SoundCharacter::kick: tuned = true; rootAndFifthOnly = true; break;
        case SoundCharacter::membrane:
        case SoundCharacter::conga:
        case SoundCharacter::agogo:
        case SoundCharacter::woodblock:
        case SoundCharacter::snare:
            tuned = true;
            break;
        default: break;
    }
    if (!tuned) return 1.0;
    const double midi = 69.0 + 12.0 * std::log2(fundamentalHz / 440.0);
    double best = midi;
    double bestDistance = 1.0e9;
    const auto& tones = kBassKeyScales[static_cast<std::size_t>(percussionKeyScale_)];
    const int centre = static_cast<int>(std::lround(midi));
    for (int candidate = centre - 7; candidate <= centre + 7; ++candidate) {
        const int degree = ((candidate - percussionKeyTonic_) % 12 + 12) % 12;
        bool inKey = false;
        if (rootAndFifthOnly) {
            inKey = degree == 0 || degree == 7;
        } else {
            for (const int tone : tones) inKey = inKey || tone == degree;
        }
        if (!inKey) continue;
        const double distance = std::abs(candidate - midi);
        if (distance < bestDistance) { bestDistance = distance; best = candidate; }
    }
    return std::pow(2.0, (best - midi) / 12.0);
}

void ResonatorRenderer::applyFormalVoiceCount() noexcept {
    const auto cycle = static_cast<std::size_t>((completedBars_ / 4u)
                                                 % kFormalCycleCount);
    const auto formal = formalVoiceReductionEnabled_ && !rhythmMode_
        ? activeVoiceCountInCycle(cycle, requestedVoiceCount_)
        : requestedVoiceCount_;
    // profundidade < 1: a voz que a forma tiraria fica, com nível 1 − d
    formalVoiceLimit_ = formal;
    const auto requested = generativeDepth_ < 0.999f ? requestedVoiceCount_ : formal;
    const auto next = std::clamp<std::size_t>(requested, 1,
        std::max<std::size_t>(1, recipe_.voiceCount));
    if (next > activeVoiceCount_) {
        for (std::size_t voice = activeVoiceCount_; voice < next; ++voice) {
            for (std::size_t layer = 0; layer < kMaxMidiNotesPerAttack; ++layer) {
                states_[voice][layer] = {};
                states_[voice][layer].noiseState = static_cast<std::uint32_t>(
                    0x9E3779B9u ^ (voice * 0x85EBCA6Bu)
                    ^ (static_cast<std::uint32_t>(layer) * 0xC2B2AE35u));
            }
            currentChannelLevels_[voice] = 0.0f;
        }
        renderVoiceCount_ = std::max(renderVoiceCount_, next);
    }
    activeVoiceCount_ = next;
}

void ResonatorRenderer::setChannelPans(const float* channelPans) noexcept {
    if (channelPans == nullptr || !ready_)
        return;
    for (std::size_t voice = 0; voice < recipe_.voiceCount; ++voice) {
        const float pan = finiteClamp(channelPans[voice], -1.0f, 1.0f);
        if (std::abs(pan - panBase_[voice]) <= 1.0e-6f)
            continue;
        panBase_[voice] = pan;
        panBaseLeft_[voice] = std::sqrt(0.5f * (1.0f - pan));
        panBaseRight_[voice] = std::sqrt(0.5f * (1.0f + pan));
        setPanTarget(voice, pan);
    }
}

void ResonatorRenderer::setChannelPanMotion(const float* panMotionDepths) noexcept {
    if (panMotionDepths == nullptr || !ready_)
        return;
    for (std::size_t voice = 0; voice < recipe_.voiceCount; ++voice) {
        const float depth = finiteClamp(panMotionDepths[voice], 0.0f, 1.0f);
        if (std::abs(depth - panMotionDepth_[voice]) <= 1.0e-6f)
            continue;
        panMotionDepth_[voice] = depth;
        if (depth <= 1.0e-6f) {
            panTargetLeft_[voice] = panBaseLeft_[voice];
            panTargetRight_[voice] = panBaseRight_[voice];
        }
    }
}

void ResonatorRenderer::setChannelPanTrajectories(
        const PanTrajectory* trajectories) noexcept {
    if (trajectories == nullptr || !ready_)
        return;
    for (std::size_t voice = 0; voice < recipe_.voiceCount; ++voice) {
        auto& destination = panTrajectories_[voice];
        destination.enabled = trajectories[voice].enabled;
        for (std::size_t point = 0; point < PanTrajectory::pointCount; ++point) {
            const float value = trajectories[voice].points[point];
            destination.points[point] = finite(value)
                ? std::clamp(value, -1.0f, 1.0f) : 0.0f;
        }
    }
}

void ResonatorRenderer::setChannelVariationDepths(
    const float* variationDepths) noexcept {
    if (variationDepths == nullptr || !ready_)
        return;
    for (std::size_t voice = 0; voice < recipe_.voiceCount; ++voice)
        stemVariationDepth_[voice] = finiteClamp(variationDepths[voice], 0.0f, 1.0f);
}

void ResonatorRenderer::setPanTarget(const std::size_t voice,
                                     const float pan) noexcept {
    if (voice >= recipe_.voiceCount)
        return;
    const float boundedPan = finiteClamp(pan, -1.0f, 1.0f);
    panTargetLeft_[voice] = std::sqrt(0.5f * (1.0f - boundedPan));
    panTargetRight_[voice] = std::sqrt(0.5f * (1.0f + boundedPan));
}

void ResonatorRenderer::setChannelSound(const std::size_t voice,
                                       const SoundCharacter sound) noexcept {
    if (voice >= recipe_.voiceCount)
        return;
    const auto boundedSound = static_cast<std::uint8_t>(sound)
            <= static_cast<std::uint8_t>(SoundCharacter::woodblock)
        ? sound : SoundCharacter::triangle;
    if (channelSounds_[voice] == boundedSound)
        return;
    channelSounds_[voice] = boundedSound;
    for (std::size_t layer = 0; layer < kMaxMidiNotesPerAttack; ++layer) {
        activeVoiceSounds_[voice][layer] = boundedSound;
        refreshVoiceSpectrum(voice, layer);
    }
}

void ResonatorRenderer::setChannelTimbreProfile(
        const std::size_t voice, const int familyIndex) noexcept {
    if (voice >= recipe_.voiceCount)
        return;
    const int profile = familyIndex >= 0 && familyIndex < 17
        ? familyIndex : -1;
    if (channelTimbreProfiles_[voice] == profile
        && channelTimbreProfileSampleRates_[voice] == sampleRate_)
        return;
    const float previousNoiseGain = channelTimbreNoiseGains_[voice];
    channelTimbreProfiles_[voice] = profile;
    channelTimbreProfileSampleRates_[voice] = sampleRate_;
    // Tail time, noise balance and partial tilt are independent of a stem's
    // role. This lets seeds/families color a kick as a kick instead of
    // replacing it with an unrelated instrument model.
    constexpr std::array<float, 17> tailSeconds = {
        0.42f, 0.95f, 0.32f, 0.72f, 1.25f, 0.62f, 0.38f, 1.50f,
        0.16f, 0.24f, 0.30f, 1.10f, 0.66f, 0.82f, 0.40f, 0.34f, 0.78f};
    constexpr std::array<float, 17> noiseGains = {
        1.20f, 0.72f, 1.30f, 1.12f, 0.95f, 0.90f, 1.25f, 1.45f,
        0.62f, 0.78f, 1.35f, 1.10f, 1.22f, 0.94f, 1.28f, 1.42f, 1.05f};
    constexpr std::array<float, 17> partialTilts = {
        0.52f, -0.32f, -0.65f, 0.18f, -0.72f, 0.06f, 0.38f, -0.82f,
        0.72f, 0.48f, -0.58f, -0.48f, 0.24f, 0.42f, -0.42f, -0.78f, -0.24f};
    // These ratios act at the onset itself. The older profiles mainly changed
    // long modal tails, which were masked by short percussion and the mixer.
    constexpr std::array<float, 17> bodyGains = {
        1.65f, 0.82f, 0.72f, 1.12f, 0.78f, 1.05f, 0.62f, 0.66f,
        0.52f, 0.68f, 1.75f, 0.72f, 0.92f, 1.18f, 1.34f, 0.60f, 0.88f};
    constexpr std::array<float, 17> noiseDurations = {
        0.64f, 1.18f, 0.46f, 0.82f, 1.48f, 0.92f, 1.62f, 1.75f,
        0.18f, 0.30f, 0.52f, 1.42f, 0.72f, 0.58f, 0.68f, 0.36f, 1.08f};
    if (profile < 0) {
        channelTimbreTailCoefficients_[voice] = 1.0f;
        channelTimbreNoiseGains_[voice] = 1.0f;
        channelTimbreBrightness_[voice] = 0.0f;
        channelTimbreBodyGains_[voice] = 1.0f;
        channelTimbreNoiseDurations_[voice] = 1.0f;
    } else {
        const auto index = static_cast<std::size_t>(profile);
        channelTimbreTailCoefficients_[voice] = static_cast<float>(std::exp(
            -1.0 / (std::max(1.0, sampleRate_) * tailSeconds[index])));
        channelTimbreNoiseGains_[voice] = noiseGains[index];
        channelTimbreBrightness_[voice] = partialTilts[index];
        channelTimbreBodyGains_[voice] = bodyGains[index];
        channelTimbreNoiseDurations_[voice] = noiseDurations[index];
    }
    // A sounding drum may still have a noise tail. Its strike colour was
    // cached at onset; rescale that tail now so the family responds on this
    // audio block instead of waiting for the next MIDI/cell attack.
    for (std::size_t layer = 0; layer < kMaxMidiNotesPerAttack; ++layer) {
        if (previousNoiseGain > 0.0f)
            states_[voice][layer].excitationNoiseColour *=
                channelTimbreNoiseGains_[voice] / previousNoiseGain;
        refreshVoiceSpectrum(voice, layer);
    }
}

void ResonatorRenderer::setChannelChip8Parameters(
    const std::size_t voice, const Chip8Parameters parameters) noexcept {
    if (voice >= recipe_.voiceCount)
        return;
    chip8Parameters_[voice].dutyCycle = finiteClamp(
        parameters.dutyCycle, 0.125f, 0.875f);
    chip8Parameters_[voice].clockHz = finiteClamp(
        parameters.clockHz, 1000.0f, 24000.0f);
    chip8Parameters_[voice].bitDepth = std::clamp(parameters.bitDepth, 2, 8);
    chip8Parameters_[voice].noiseMix = finiteClamp(
        parameters.noiseMix, 0.0f, 1.0f);
}

void ResonatorRenderer::refreshVoiceSpectrum(const std::size_t voice,
                                             const std::size_t layer) noexcept {
    if (voice >= recipe_.voiceCount || layer >= kMaxMidiNotesPerAttack)
        return;
    constexpr std::array<double, kModesPerVoice> bellRatios = {
        1.0, 2.76, 5.40, 8.93, 13.34, 18.64, 24.85, 32.0};
    constexpr std::array<double, kModesPerVoice> membraneRatios = {
        1.0, 1.59, 2.14, 2.30, 2.65, 3.16, 3.60, 4.10};
    constexpr std::array<double, kModesPerVoice> cymbalRatios = {
        1.0, 1.31, 1.73, 2.27, 2.91, 3.68, 4.52, 5.61};
    constexpr std::array<double, kModesPerVoice> congaRatios = {
        1.0, 1.50, 2.03, 2.48, 3.05, 3.62, 4.18, 4.90};
    constexpr std::array<float, kModesPerVoice> bellWeights = {
        1.0f, 0.7118f, 0.5066f, 0.3606f, 0.2567f, 0.1828f, 0.1302f, 0.0927f};
    constexpr std::array<float, kModesPerVoice> membraneWeights = {
        1.0f, 0.7071f, 0.5774f, 0.5f, 0.4472f, 0.4082f, 0.3780f, 0.3536f};
    const auto sound = activeVoiceSounds_[voice][layer];
    // Percussion identities must not inherit the melodic pitch assigned to a
    // mathematical voice. Without this anchor, a kick on V1 could be rendered
    // at the voice's arbitrary resonance (often mid/high), which made the
    // sound selector appear to have the wrong stem or no kick at all.
    // These are deliberately stylised synthesis anchors, not instrument
    // samples or claims of acoustic authenticity.
    double percussionFundamentalHz = 0.0;
    switch (sound) {
        case SoundCharacter::kick: percussionFundamentalHz = 55.0; break;
        case SoundCharacter::snare: percussionFundamentalHz = 185.0; break;
        case SoundCharacter::hihat: percussionFundamentalHz = 720.0; break;
        case SoundCharacter::openHihat: percussionFundamentalHz = 910.0; break;
        case SoundCharacter::rimClick: percussionFundamentalHz = 980.0; break;
        case SoundCharacter::ride: percussionFundamentalHz = 680.0; break;
        case SoundCharacter::conga: percussionFundamentalHz = 205.0; break;
        case SoundCharacter::brushes: percussionFundamentalHz = 220.0; break;
        case SoundCharacter::agogo: percussionFundamentalHz = 620.0; break;
        case SoundCharacter::woodblock: percussionFundamentalHz = 1050.0; break;
        case SoundCharacter::membrane: percussionFundamentalHz = 145.0; break;
        case SoundCharacter::shaker:
        case SoundCharacter::sine:
        case SoundCharacter::triangle:
        case SoundCharacter::sawtooth:
        case SoundCharacter::square:
        case SoundCharacter::bell:
        case SoundCharacter::chip8:
            break;
    }
    const double voiceResonanceHz = std::max(
        1.0, recipe_.voices[voice].resonanceHz);
    const double percussionPitchScale = percussionFundamentalHz > 0.0
        ? percussionFundamentalHz / voiceResonanceHz : 1.0;
    noiseColours_[voice][layer] = 1.0f;
    switch (sound) {
        case SoundCharacter::sine: noiseColours_[voice][layer] = 0.0f; break;
        case SoundCharacter::triangle: noiseColours_[voice][layer] = 0.10f; break;
        case SoundCharacter::sawtooth: noiseColours_[voice][layer] = 0.30f; break;
        case SoundCharacter::square: noiseColours_[voice][layer] = 0.15f; break;
        case SoundCharacter::bell: noiseColours_[voice][layer] = 0.20f; break;
        case SoundCharacter::membrane: noiseColours_[voice][layer] = 0.42f; break;
        case SoundCharacter::kick: noiseColours_[voice][layer] = 0.12f; break;
        case SoundCharacter::snare: noiseColours_[voice][layer] = 0.92f; break;
        case SoundCharacter::hihat: noiseColours_[voice][layer] = 1.0f; break;
        case SoundCharacter::openHihat: noiseColours_[voice][layer] = 0.88f; break;
        case SoundCharacter::rimClick: noiseColours_[voice][layer] = 0.10f; break;
        case SoundCharacter::ride: noiseColours_[voice][layer] = 0.68f; break;
        case SoundCharacter::conga: noiseColours_[voice][layer] = 0.38f; break;
        case SoundCharacter::brushes: noiseColours_[voice][layer] = 0.52f; break;
        case SoundCharacter::chip8: noiseColours_[voice][layer] = 0.34f; break;
        case SoundCharacter::shaker: noiseColours_[voice][layer] = 1.0f; break;
        case SoundCharacter::agogo: noiseColours_[voice][layer] = 0.18f; break;
        case SoundCharacter::woodblock: noiseColours_[voice][layer] = 0.08f; break;
    }
    balanceFundamentalByLevel_[voice][layer] = sound != SoundCharacter::sine;
    for (std::size_t mode = 0; mode < kModesPerVoice; ++mode) {
        double ratio = recipe_.voices[voice].resonator.frequencyRatios[mode];
        switch (sound) {
            case SoundCharacter::bell: ratio = bellRatios[mode]; break;
            case SoundCharacter::membrane:
            case SoundCharacter::snare:
            case SoundCharacter::kick:
                ratio = membraneRatios[mode];
                break;
            case SoundCharacter::hihat:
                ratio = cymbalRatios[mode] * 5.0;
                break;
            case SoundCharacter::openHihat:
                ratio = cymbalRatios[mode] * 4.2;
                break;
            case SoundCharacter::rimClick:
                ratio = membraneRatios[mode] * 1.8;
                break;
            case SoundCharacter::ride:
                ratio = cymbalRatios[mode] * 2.8;
                break;
            case SoundCharacter::agogo:
                ratio = bellRatios[mode] * (mode % 2 == 0 ? 1.0 : 1.37);
                break;
            case SoundCharacter::woodblock:
                ratio = membraneRatios[mode] * 1.7;
                break;
            case SoundCharacter::shaker:
                ratio = cymbalRatios[mode] * 5.5;
                break;
            case SoundCharacter::conga: ratio = congaRatios[mode]; break;
            case SoundCharacter::brushes: ratio = membraneRatios[mode] * 1.3; break;
            case SoundCharacter::chip8:
            case SoundCharacter::sine:
            case SoundCharacter::triangle:
            case SoundCharacter::sawtooth:
            case SoundCharacter::square:
                break;
        }
        ratio *= percussionPitchScale;
        modeFrequencyRatios_[voice][layer][mode] = ratio;
        const double lossScale = 1.0
            + recipe_.voices[voice].resonator.frequencyDependentLoss * ratio * ratio;
        const double decay = recipe_.voices[voice].resonator.decaySeconds
            * static_cast<double>(decayScale_) / lossScale;
        decayCoefficients_[voice][layer][mode] = static_cast<float>(
            std::exp(-1.0 / (decay * sampleRate_)));

        float weight = 1.0f;
        switch (sound) {
            case SoundCharacter::sine:
                weight = mode == 0 ? 1.0f : 0.0f;
                break;
            case SoundCharacter::triangle:
                weight = mode % 2 == 0
                    ? (((mode / 2) % 2 == 0 ? 1.0f : -1.0f)
                       / static_cast<float>((mode + 1) * (mode + 1)))
                    : 0.0f;
                break;
            case SoundCharacter::sawtooth:
                weight = (mode % 2 == 0 ? 1.0f : -1.0f)
                    / static_cast<float>(mode + 1);
                break;
            case SoundCharacter::square:
                weight = mode % 2 == 0
                    ? ((mode / 2) % 2 == 0 ? 1.0f : -1.0f)
                        / static_cast<float>(mode + 1)
                    : 0.0f;
                break;
            case SoundCharacter::bell:
                weight = bellWeights[mode] * (mode % 2 == 0 ? 1.0f : -0.72f);
                break;
            case SoundCharacter::membrane:
                weight = (mode % 2 == 0 ? 1.0f : -0.55f) * membraneWeights[mode];
                break;
            case SoundCharacter::kick:
                weight = mode == 0 ? 1.0f : mode == 1 ? 0.32f : 0.08f;
                break;
            case SoundCharacter::snare:
                weight = (mode % 2 == 0 ? 1.0f : -0.58f) * membraneWeights[mode];
                break;
            case SoundCharacter::hihat:
                weight = bellWeights[mode] * (mode % 2 == 0 ? 0.75f : -0.8f);
                break;
            case SoundCharacter::openHihat:
                weight = bellWeights[mode] * (mode % 2 == 0 ? 0.92f : -0.64f);
                break;
            case SoundCharacter::rimClick:
                weight = (mode % 2 == 0 ? 1.0f : -0.35f)
                    * membraneWeights[mode] / static_cast<float>(mode + 1);
                break;
            case SoundCharacter::ride:
                weight = bellWeights[mode] * (mode % 2 == 0 ? 1.0f : -0.62f);
                break;
            case SoundCharacter::conga:
                weight = (mode % 2 == 0 ? 1.0f : -0.48f) * membraneWeights[mode];
                break;
            case SoundCharacter::brushes:
                weight = (mode % 2 == 0 ? 1.0f : -0.5f)
                    * membraneWeights[mode] * 0.32f;
                break;
            case SoundCharacter::chip8:
                // Odd harmonics form the pulse-like source before rate and
                // amplitude quantisation in renderVoice().
                weight = mode % 2 == 0
                    ? 1.0f / static_cast<float>(mode + 1)
                    : 0.0f;
                break;
            case SoundCharacter::shaker:
                weight = mode < 2 ? 0.015f : 0.0f;
                break;
            case SoundCharacter::agogo:
                weight = bellWeights[mode] * (mode % 2 == 0 ? 1.0f : -0.82f);
                break;
            case SoundCharacter::woodblock:
                weight = (mode % 2 == 0 ? 1.0f : -0.25f)
                    * membraneWeights[mode] / static_cast<float>(mode + 1);
                break;
        }
        const float tilt = channelTimbreBrightness_[voice];
        if (tilt != 0.0f)
            weight *= static_cast<float>(std::pow(
                static_cast<double>(mode + 1), -static_cast<double>(tilt)));
        modeSpectralWeights_[voice][layer][mode] = weight;
    }
}

void ResonatorRenderer::setVoiceFormulaData(const VoiceTransform* transforms,
                                           const float* couplings,
                                           const float* phaseOffsets) noexcept {
    if (transforms == nullptr || couplings == nullptr || phaseOffsets == nullptr || !ready_)
        return;
    for (std::size_t voice = 0; voice < recipe_.voiceCount; ++voice) {
        const auto transform = static_cast<std::uint8_t>(transforms[voice]);
        voiceTransforms_[voice] = transform
                <= static_cast<std::uint8_t>(VoiceTransform::softClip)
            ? transforms[voice] : VoiceTransform::sine;
        voiceCouplings_[voice] = voice == 0 ? 0.0f
            : finiteClamp(couplings[voice], 0.0f, 0.8f);
        voiceFormulaPhaseOffsets_[voice] = finiteClamp(phaseOffsets[voice], -0.5f, 0.5f);
    }
}

void ResonatorRenderer::setFormalPhaseAmount(const float amount) noexcept {
    requestedFormalPhaseAmount_.store(finiteClamp(amount, 0.0f, 0.5f),
                                      std::memory_order_relaxed);
}

void ResonatorRenderer::setGeometryVoiceOneSource(
    const bool enabled, const GeometryCurve curve, const int fourierTerms,
    const int billiardPattern,
    const GeometryLissajousParameters lissajous) noexcept {
    geometryEventTableSequence_.fetch_add(1, std::memory_order_acq_rel);
    if (enabled && (curve == GeometryCurve::fourierTriangle
                    || curve == GeometryCurve::fourierSquare)) {
        Point2D warmup;
        (void) geometryCurvePoint(curve, 0.0, 1.0, 0.0, fourierTerms, warmup);
    }
    const int terms = std::clamp(fourierTerms, 1, kMaximumGeometryFourierTerms);
    const GeometryLissajousParameters boundedLissajous{
        std::clamp(lissajous.xFrequency, 1, 12),
        std::clamp(lissajous.yFrequency, 1, 12),
        std::isfinite(lissajous.yPhaseTurns)
            ? lissajous.yPhaseTurns - std::floor(lissajous.yPhaseTurns) : 0.0,
        std::isfinite(lissajous.xAmplitude)
            ? std::clamp(lissajous.xAmplitude, 0.0, 1.0) : 1.0,
        std::isfinite(lissajous.yAmplitude)
            ? std::clamp(lissajous.yAmplitude, 0.0, 1.0) : 1.0};
    for (std::size_t rule = 0; rule < 2; ++rule) {
        std::array<double, kMaxGeometryEventsPerTraversal> positions{};
        std::size_t count = 0;
        const bool supportedCurve = curve != GeometryCurve::spiral;
        if (enabled && supportedCurve
            && geometryCurveEventPositions(curve, rule == 1, terms,
                positions.data(), positions.size(), count, billiardPattern,
                boundedLissajous)) {
            for (std::size_t i = 0; i < count; ++i) {
                Point2D point;
                if (!geometryCurvePoint(curve, positions[i], 1.0, 0.0,
                                        terms, point, billiardPattern,
                                        boundedLissajous)) {
                    count = 0;
                    break;
                }
                requestedGeometryEventPositions_[rule][i].store(
                    positions[i], std::memory_order_relaxed);
                requestedGeometryEventX_[rule][i].store(
                    point.x, std::memory_order_relaxed);
                requestedGeometryEventY_[rule][i].store(
                    point.y, std::memory_order_relaxed);
            }
        } else {
            count = 0;
        }
        requestedGeometryEventCounts_[rule].store(count, std::memory_order_relaxed);
    }
    geometryVoiceOneCurve_.store(static_cast<int>(curve), std::memory_order_relaxed);
    geometryVoiceOneFourierTerms_.store(terms, std::memory_order_relaxed);
    geometryVoiceOneBilliardPattern_.store(
        std::clamp(billiardPattern, 0, 6), std::memory_order_relaxed);
    geometryLissajousXFrequency_.store(boundedLissajous.xFrequency,
                                        std::memory_order_relaxed);
    geometryLissajousYFrequency_.store(boundedLissajous.yFrequency,
                                        std::memory_order_relaxed);
    geometryLissajousYPhaseTurns_.store(boundedLissajous.yPhaseTurns,
                                         std::memory_order_relaxed);
    geometryLissajousXAmplitude_.store(boundedLissajous.xAmplitude,
                                        std::memory_order_relaxed);
    geometryLissajousYAmplitude_.store(boundedLissajous.yAmplitude,
                                        std::memory_order_relaxed);
    geometryVoiceOneSource_.store(enabled, std::memory_order_release);
    geometryEventTableSequence_.fetch_add(1, std::memory_order_release);
}

void ResonatorRenderer::acquireGeometryEventTables() noexcept {
    const auto before = geometryEventTableSequence_.load(std::memory_order_acquire);
    if ((before & 1u) != 0u)
        return;
    std::array<std::size_t, 2> counts{};
    std::array<std::array<double, kMaxGeometryEventsPerTraversal>, 2> positions{};
    std::array<std::array<Point2D, kMaxGeometryEventsPerTraversal>, 2> points{};
    for (std::size_t rule = 0; rule < counts.size(); ++rule) {
        counts[rule] = std::min(requestedGeometryEventCounts_[rule].load(
            std::memory_order_relaxed), kMaxGeometryEventsPerTraversal);
        for (std::size_t i = 0; i < counts[rule]; ++i)
            positions[rule][i]
                = requestedGeometryEventPositions_[rule][i].load(
                    std::memory_order_relaxed);
        for (std::size_t i = 0; i < counts[rule]; ++i) {
            points[rule][i].x = requestedGeometryEventX_[rule][i].load(
                std::memory_order_relaxed);
            points[rule][i].y = requestedGeometryEventY_[rule][i].load(
                std::memory_order_relaxed);
        }
    }
    const bool enabled = geometryVoiceOneSource_.load(std::memory_order_relaxed);
    const GeometryLissajousParameters lissajous{
        geometryLissajousXFrequency_.load(std::memory_order_relaxed),
        geometryLissajousYFrequency_.load(std::memory_order_relaxed),
        geometryLissajousYPhaseTurns_.load(std::memory_order_relaxed),
        geometryLissajousXAmplitude_.load(std::memory_order_relaxed),
        geometryLissajousYAmplitude_.load(std::memory_order_relaxed)};
    const auto after = geometryEventTableSequence_.load(std::memory_order_acquire);
    if (before != after || (after & 1u) != 0u)
        return;
    activeGeometryEventPositions_ = positions;
    activeGeometryEventPoints_ = points;
    activeGeometryEventCounts_ = counts;
    activeGeometryLissajous_ = lissajous;
    activeGeometryEventTiming_ = enabled
        && (counts[0] > 0 || counts[1] > 0);
}

void ResonatorRenderer::setVoiceRhythmOverrides(const float* cycles,
                                                const float* phaseOffsets,
                                                const int* eventRuleOverrides,
                                                const bool applyImmediately) noexcept {
    if (cycles == nullptr || phaseOffsets == nullptr || eventRuleOverrides == nullptr)
        return;
    bool changed = false;
    for (std::size_t voice = 0; voice < kMaxVoices; ++voice) {
        const float requestedCycle = cycles[voice];
        const float boundedCycle = !std::isfinite(requestedCycle)
            || requestedCycle <= 0.0f ? 0.0f
            : finiteClamp(requestedCycle, 0.5f, 16.0f);
        const float boundedPhase = finiteClamp(
            phaseOffsets[voice], -0.5f, 0.5f);
        const int boundedRule = eventRuleOverrides[voice] >= 0
            && eventRuleOverrides[voice] <= 1 ? eventRuleOverrides[voice] : -1;
        changed = changed || requestedVoiceCycles_[voice] != boundedCycle
            || requestedVoicePhaseOffsets_[voice] != boundedPhase
            || requestedVoiceEventRules_[voice] != boundedRule;
        requestedVoiceCycles_[voice] = boundedCycle;
        requestedVoicePhaseOffsets_[voice] = boundedPhase;
        requestedVoiceEventRules_[voice] = boundedRule;
    }
    if (ready_ && applyImmediately && changed)
        refreshRhythmState(activeVariant_);
}

void ResonatorRenderer::setRhythmicCells(
    const std::array<RhythmicCell, kMaxVoices>& cells) noexcept {
    for (std::size_t voice = 0; voice < kMaxVoices; ++voice)
        if (validRhythmicCell(cells[voice]))
            requestedRhythmicCells_[voice] = cells[voice];
    // Re-arm only attacks still ahead of the playhead. Editing a cell must
    // not restart the form or wait for the next bar to become audible.
    if (ready_)
        refreshRhythmState(activeVariant_);
}

void ResonatorRenderer::setFormalCycleCells(
    const FormalCycleCells& cells,
    const std::array<bool, kFormalCellVoices>& enabled) noexcept {
    for (std::size_t voice = 0; voice < kFormalCellVoices; ++voice) {
        bool valid = enabled[voice];
        for (std::size_t cycle = 0; valid && cycle < kFormalCycleCount; ++cycle)
            valid = validRhythmicCell(cells[cycle][voice]);
        formalCycleCellsEnabled_[voice] = valid;
        if (valid)
            for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle)
                formalCycleCells_[cycle][voice] = cells[cycle][voice];
    }
    if (ready_)
        refreshRhythmState(activeVariant_);
}

void ResonatorRenderer::refreshRhythmState(const std::size_t variant) noexcept {
    if (variant >= variantTimings_.size())
        return;
    acquireGeometryEventTables();
    const auto& timing = variantTimings_[variant];
    const bool drawingFieldChanged = requestedVoiceCycles_[0] > 0.0f
        || requestedVoiceCycles_[1] > 0.0f
        || std::abs(requestedVoicePhaseOffsets_[0]) > 1.0e-6f
        || std::abs(requestedVoicePhaseOffsets_[1]) > 1.0e-6f
        || requestedVoiceEventRules_[0] >= 0
        || requestedVoiceEventRules_[1] >= 0;
    const double formalAmount = static_cast<double>(
        requestedFormalPhaseAmount_.load(std::memory_order_relaxed));
    const auto formalCycle = static_cast<std::size_t>(
        (completedBars_ / 4u) % kFormalCycleCount);
    const auto [formalOffset, formalSlope] = formalPhaseArc(formalCycle, formalAmount);
    for (std::size_t voice = 0; voice < recipe_.voiceCount; ++voice) {
        activeRhythmicCells_[voice] = voice < kFormalCellVoices
                && formalCycleCellsEnabled_[voice]
            ? formalCycleCells_[formalCycle][voice]
            : requestedRhythmicCells_[voice];
        activeVoiceCycles_[voice] = timing.cycles[voice];
        activeVoicePhases_[voice] = timing.phases[voice];
        activeVoiceRules_[voice] = timing.rules[voice];
        if (requestedVoiceEventRules_[voice] >= 0)
            activeVoiceRules_[voice] = static_cast<EventRule>(
                requestedVoiceEventRules_[voice]);
        if (requestedVoiceCycles_[voice] > 0.0f)
            activeVoiceCycles_[voice] = requestedVoiceCycles_[voice];
        activeVoicePhases_[voice] = wrapUnitCycle(
            activeVoicePhases_[voice] + requestedVoicePhaseOffsets_[voice]);
        activeFormalPhaseOffsets_[voice] = voice == 0 ? 0.0 : formalOffset;
        activeFormalPhaseSlopes_[voice] = voice == 0 ? 0.0 : formalSlope;
        const bool voiceTimingChanged = requestedVoiceCycles_[voice] > 0.0f
            || std::abs(requestedVoicePhaseOffsets_[voice]) > 1.0e-6f
            || requestedVoiceEventRules_[voice] >= 0;
        // Changing either drawing axis changes the path used to filter every
        // voice's events, so recalculate those gates while retaining their own
        // cycle and phase unless they also have a local override.
        const bool formalPhaseMoving = voice > 0
            && (std::abs(formalOffset) > 1.0e-9
                || std::abs(formalSlope) > 1.0e-9);
        // Moving masks must gate attacks at onset. In open field, the moving
        // focus only weights each prepared event and does not need a dynamic
        // per-sample crossing scheduler.
        const bool movingPatternMask = timing.patternMode == PatternMode::adhesivePads
            || timing.patternMode == PatternMode::diagonalLanes;
        activeDynamicRhythm_[voice] = activeRhythmicCells_[voice].enabled
            || (!recipe_.rhythmicCells[voice].enabled && (movingPatternMask
            || voiceTimingChanged || drawingFieldChanged
            || formalPhaseMoving
            || (voice == 0 && activeGeometryEventTiming_)));
        if (activeDynamicRhythm_[voice]) {
            dynamicEventStreamCounts_[voice] = activeRhythmicCells_[voice].enabled
                ? activeRhythmicCells_[voice].count
                : voice == 0
                    && activeGeometryEventTiming_
                ? activeGeometryEventCounts_[activeVoiceRules_[voice]
                    == EventRule::centreCrossings ? 0 : 1]
                : 2;
            for (std::size_t stream = 0;
                 stream < dynamicEventStreamCounts_[voice]; ++stream) {
                if (activeRhythmicCells_[voice].enabled) {
                    const double progress = activeRhythmicCells_[voice].positions[stream];
                    const double scheduled = swingRatio_ > 0.500001
                        ? swungProgress(progress, swingRatio_, quarterNotesPerSection_)
                        : progress;
                    const auto sample = static_cast<std::uint64_t>(std::llround(
                        scheduled * static_cast<double>(sectionSamples_)));
                    dynamicEventSamples_[voice][stream] = sample < sectionPosition_
                        || sample >= sectionSamples_
                        ? std::numeric_limits<std::uint64_t>::max() : sample;
                } else {
                    refreshDynamicCrossing(voice, stream);
                }
            }
            for (std::size_t stream = dynamicEventStreamCounts_[voice];
                 stream < kMaxGeometryEventsPerTraversal; ++stream)
                dynamicEventSamples_[voice][stream]
                    = std::numeric_limits<std::uint64_t>::max();
        } else {
            dynamicEventStreamCounts_[voice] = 0;
            dynamicEventSamples_[voice].fill(
                std::numeric_limits<std::uint64_t>::max());
        }
    }
    refreshDynamicFirstVoiceAnchor(timing.patternMode);
    updateNextDynamicEventSample();
}

void ResonatorRenderer::updateNextDynamicEventSample() noexcept {
    auto next = dynamicFirstVoiceAnchorFired_
        ? std::numeric_limits<std::uint64_t>::max()
        : dynamicFirstVoiceAnchorSample_;
    for (std::size_t voice = 0; voice < recipe_.voiceCount; ++voice) {
        if (!activeDynamicRhythm_[voice])
            continue;
        for (std::size_t stream = 0; stream < dynamicEventStreamCounts_[voice]; ++stream)
            next = std::min(next, dynamicEventSamples_[voice][stream]);
    }
    nextDynamicRhythmSample_ = next;
}

void ResonatorRenderer::refreshDynamicFirstVoiceAnchor(
    const PatternMode pattern) noexcept {
    dynamicFirstVoiceAnchorSample_ = std::numeric_limits<std::uint64_t>::max();
    if (dynamicFirstVoiceAnchorFired_ || !activeDynamicRhythm_[0]
        || activeRhythmicCells_[0].enabled
        || pattern == PatternMode::unrestricted || sectionSamples_ == 0)
        return;
    const double cycles = effectiveVoiceCycles(activeVoiceCycles_,
        activeFormalPhaseSlopes_, 0);
    if (cycles <= 0.0)
        return;
    const double phase = wrapUnitCycle(activeVoicePhases_[0]
        + activeFormalPhaseOffsets_[0]);
    const bool geometric = activeGeometryEventTiming_;
    const auto rule = activeVoiceRules_[0];
    const auto ruleIndex = rule == EventRule::centreCrossings ? 0u : 1u;
    double nearest = std::numeric_limits<double>::infinity();
    MusicalEvent candidate{};
    std::uint64_t candidateSample = std::numeric_limits<std::uint64_t>::max();
    for (std::size_t stream = 0; stream < dynamicEventStreamCounts_[0]; ++stream) {
        const double threshold = geometric
            ? activeGeometryEventPositions_[ruleIndex][stream]
            : rule == EventRule::centreCrossings
                ? (stream == 0 ? 0.0 : 0.5)
                : (stream == 0 ? 0.25 : 0.75);
        const auto first = static_cast<std::int64_t>(
            std::ceil(phase - threshold - kEpsilon));
        const double end = cycles + phase - threshold;
        for (auto index = first; static_cast<double>(index) < end - kEpsilon;
             ++index) {
            const double progress = (static_cast<double>(index)
                + threshold - phase) / cycles;
            if (progress < -kEpsilon || progress >= 1.0 - kEpsilon)
                continue;
            const double x = geometric
                ? activeGeometryEventPoints_[ruleIndex][stream].x
                : std::sin(kTau * (effectiveVoiceCycles(activeVoiceCycles_,
                    activeFormalPhaseSlopes_, 0) * progress
                    + activeVoicePhases_[0] + activeFormalPhaseOffsets_[0]));
            const double y = geometric
                ? activeGeometryEventPoints_[ruleIndex][stream].y
                : std::sin(kTau * (effectiveVoiceCycles(activeVoiceCycles_,
                    activeFormalPhaseSlopes_, 1) * progress
                    + activeVoicePhases_[1] + activeFormalPhaseOffsets_[1]));
            const double fieldProgress = static_cast<double>(completedBars_) / 4.0
                + progress;
            if (rhythmMode_
                || patternFieldAllows(pattern, x, y, fieldProgress, developmentSeed_))
                return;
            const double distance = distanceToPattern(pattern, x, y);
            if (distance >= nearest)
                continue;
            nearest = distance;
            candidate.timeSeconds = std::max(0.0, progress) * recipe_.sectionSeconds;
            candidate.phaseCycles = threshold;
            candidate.voice = 0;
            candidate.rule = rule;
            candidate.fieldX = static_cast<float>(x);
            candidate.fieldY = static_cast<float>(y);
            candidate.intensity = 0.4f;
            const double swung = swungProgress(std::max(0.0, progress),
                swingRatio_, quarterNotesPerSection_);
            candidateSample = std::min(sectionSamples_ - 1,
                static_cast<std::uint64_t>(std::llround(
                    swung * static_cast<double>(sectionSamples_))));
        }
    }
    if (candidateSample < sectionPosition_)
        return;
    dynamicFirstVoiceAnchor_ = candidate;
    dynamicFirstVoiceAnchorSample_ = candidateSample;
}

void ResonatorRenderer::refreshDynamicCrossing(const std::size_t voice,
                                               const std::size_t stream,
                                               const bool preserveNextCycle) noexcept {
    if (voice >= recipe_.voiceCount || stream >= kMaxGeometryEventsPerTraversal
        || sectionSamples_ == 0
        || effectiveVoiceCycles(activeVoiceCycles_, activeFormalPhaseSlopes_, voice) <= 0.0) {
        if (voice < kMaxVoices && stream < kMaxGeometryEventsPerTraversal)
            dynamicEventSamples_[voice][stream] =
                std::numeric_limits<std::uint64_t>::max();
        return;
    }
    const bool geometric = voice == 0 && activeGeometryEventTiming_;
    const auto& geometricPositions = activeGeometryEventPositions_[
        activeVoiceRules_[voice] == EventRule::centreCrossings ? 0 : 1];
    const double threshold = geometric
        ? geometricPositions[stream]
        : activeVoiceRules_[voice] == EventRule::centreCrossings
            ? (stream == 0 ? 0.0 : 0.5)
            : (stream == 0 ? 0.25 : 0.75);
    const double cycles = effectiveVoiceCycles(
        activeVoiceCycles_, activeFormalPhaseSlopes_, voice);
    const double phase = wrapUnitCycle(activeVoicePhases_[voice]
        + activeFormalPhaseOffsets_[voice]);
    double progress = static_cast<double>(sectionPosition_)
        / static_cast<double>(sectionSamples_);
    if (swingRatio_ > 0.500001)
        progress = straightProgress(progress, swingRatio_, quarterNotesPerSection_);
    auto index = static_cast<std::int64_t>(std::ceil(
        phase + cycles * progress - threshold - kEpsilon));
    if (preserveNextCycle)
        index = std::max(index, dynamicCycleIndices_[voice][stream]);
    double eventProgress = (static_cast<double>(index) + threshold - phase)
        / cycles;
    while (eventProgress < progress - kEpsilon) {
        ++index;
        eventProgress = (static_cast<double>(index) + threshold - phase)
            / cycles;
    }
    dynamicCycleIndices_[voice][stream] = index;
    if (!std::isfinite(eventProgress) || eventProgress >= 1.0 - kEpsilon) {
        dynamicEventSamples_[voice][stream] =
            std::numeric_limits<std::uint64_t>::max();
        return;
    }
    const double scheduledProgress = swingRatio_ > 0.500001
        ? swungProgress(std::max(0.0, eventProgress), swingRatio_,
                        quarterNotesPerSection_)
        : std::max(0.0, eventProgress);
    const auto candidate = static_cast<std::uint64_t>(std::llround(
        scheduledProgress * static_cast<double>(sectionSamples_)));
    dynamicEventSamples_[voice][stream] = std::max(
        sectionPosition_, std::min(candidate, sectionSamples_ - 1));
}

void ResonatorRenderer::triggerDynamicVoiceEvents(
    const std::size_t voice, const std::uint64_t position,
    const float* targetLevels) noexcept {
    if (voice >= recipe_.voiceCount || !activeDynamicRhythm_[voice])
        return;
    const auto& timing = variantTimings_[activeVariant_];
    if (activeRhythmicCells_[voice].enabled) {
        const auto& cell = activeRhythmicCells_[voice];
        for (std::size_t stream = 0; stream < cell.count; ++stream) {
            if (dynamicEventSamples_[voice][stream] > position)
                continue;
            const double progress = cell.positions[stream];
            const double xPhase = kTau * (effectiveVoiceCycles(
                activeVoiceCycles_, activeFormalPhaseSlopes_, 0) * progress
                + activeVoicePhases_[0] + activeFormalPhaseOffsets_[0]);
            const double yPhase = kTau * (effectiveVoiceCycles(
                activeVoiceCycles_, activeFormalPhaseSlopes_, 1) * progress
                + activeVoicePhases_[1] + activeFormalPhaseOffsets_[1]);
            MusicalEvent event;
            event.timeSeconds = progress * recipe_.sectionSeconds;
            event.phaseCycles = wrapUnitCycle(activeVoiceCycles_[voice] * progress
                + activeVoicePhases_[voice]);
            event.voice = static_cast<std::uint8_t>(voice);
            event.midiNote = cell.midiNotes[stream];
            event.midiHatPedal = cell.midiHatPedal[stream];
            event.rule = activeVoiceRules_[voice];
            event.fieldX = static_cast<float>(std::sin(xPhase));
            event.fieldY = static_cast<float>(std::sin(yPhase));
            float dynamicScale = 1.0f;
            if (!rhythmMode_) {
            dynamicScale = patternFieldWeight(timing.patternMode,
                event.fieldX, event.fieldY,
                static_cast<double>(completedBars_) / 4.0 + progress,
                developmentSeed_);
            const float formula = blendedIntensityFormulaValue(intensityLaw_,
                secondIntensityLaw_, intensityBlend_,
                event.fieldX, event.fieldY,
                effectiveVoiceCycles(activeVoiceCycles_, activeFormalPhaseSlopes_, 0),
                effectiveVoiceCycles(activeVoiceCycles_, activeFormalPhaseSlopes_, 1));
            dynamicScale *= 1.0f - intensityAmount_
                + intensityAmount_ * std::clamp(formula, 0.0f, 1.0f);
            dynamicScale *= 0.6f + 0.4f * voiceFormulaValue(voice, progress);
            dynamicScale *= grooveAccentAtProgress(
                swungProgress(progress, swingRatio_, quarterNotesPerSection_),
                grooveMode_, meterNumerator_, meterDenominator_);
            dynamicScale *= expressionAtProgress(progress,
                dynamicCurve_, musicalIntention_);
            dynamicScale = 1.0f - generativeDepth_ * (1.0f - dynamicScale);
            }   // !rhythmMode_
            event.intensity = cell.strengths[stream] * dynamicScale;
            if (event.intensity > 0.0f && voice < activeVoiceCount_
                && targetLevels[voice] > 0.0f)
                excite(event);
            if (voice < activeVoiceCount_ && targetLevels[voice] > 0.0f)
                for (std::size_t collision = 0;
                     collision < cell.midiCollisionCount[stream]; ++collision) {
                    auto layeredEvent = event;
                    layeredEvent.soundLayer = static_cast<std::uint8_t>(collision + 1);
                    layeredEvent.midiNote = cell.midiCollisionNotes[stream][collision];
                    layeredEvent.midiHatPedal =
                        cell.midiCollisionHatPedal[stream][collision];
                    layeredEvent.intensity =
                        cell.midiCollisionStrengths[stream][collision] * dynamicScale;
                    if (layeredEvent.intensity > 0.0f)
                        excite(layeredEvent);
                }
            dynamicEventSamples_[voice][stream]
                = std::numeric_limits<std::uint64_t>::max();
        }
        return;
    }
    const bool geometric = voice == 0 && activeGeometryEventTiming_;
    const auto& geometricPositions = activeGeometryEventPositions_[
        activeVoiceRules_[voice] == EventRule::centreCrossings ? 0 : 1];
    for (std::size_t stream = 0;
         stream < dynamicEventStreamCounts_[voice]; ++stream) {
        while (dynamicEventSamples_[voice][stream] <= position) {
            const auto cycleIndex = dynamicCycleIndices_[voice][stream];
            const double phase = wrapUnitCycle(activeVoicePhases_[voice]
                + activeFormalPhaseOffsets_[voice]);
            const double eventPosition = geometric
                ? geometricPositions[stream]
                : activeVoiceRules_[voice] == EventRule::centreCrossings
                    ? (stream == 0 ? 0.0 : 0.5)
                    : (stream == 0 ? 0.25 : 0.75);
            const double cycles = effectiveVoiceCycles(
                activeVoiceCycles_, activeFormalPhaseSlopes_, voice);
            const double progress = (static_cast<double>(cycleIndex)
                + eventPosition - phase) / cycles;
            if (progress >= -kEpsilon && progress < 1.0 - kEpsilon) {
                const double xPhase = kTau * (effectiveVoiceCycles(
                    activeVoiceCycles_, activeFormalPhaseSlopes_, 0) * progress
                    + activeVoicePhases_[0] + activeFormalPhaseOffsets_[0]);
                const double yPhase = kTau * (effectiveVoiceCycles(
                    activeVoiceCycles_, activeFormalPhaseSlopes_, 1) * progress
                    + activeVoicePhases_[1] + activeFormalPhaseOffsets_[1]);
                double x = std::sin(xPhase);
                double y = std::sin(yPhase);
                if (geometric) {
                    x = activeGeometryEventPoints_[
                        activeVoiceRules_[voice] == EventRule::centreCrossings
                            ? 0 : 1][stream].x;
                    y = activeGeometryEventPoints_[
                        activeVoiceRules_[voice] == EventRule::centreCrossings
                            ? 0 : 1][stream].y;
                }
                const double fieldProgress = static_cast<double>(completedBars_) / 4.0
                    + progress;
                const float fieldWeight = rhythmMode_ ? 1.0f
                    : patternFieldWeight(timing.patternMode,
                    x, y, fieldProgress, developmentSeed_);
                if (fieldWeight > 0.0f || generativeDepth_ < 0.999f) {
                    MusicalEvent event;
                    event.timeSeconds = std::max(0.0, progress) * recipe_.sectionSeconds;
                    event.phaseCycles = eventPosition;
                    event.voice = static_cast<std::uint8_t>(voice);
                    event.rule = activeVoiceRules_[voice];
                    event.fieldX = static_cast<float>(x);
                    event.fieldY = static_cast<float>(y);
                    const float intensity = blendedIntensityFormulaValue(
                        intensityLaw_, secondIntensityLaw_, intensityBlend_,
                        event.fieldX, event.fieldY,
                        effectiveVoiceCycles(activeVoiceCycles_,
                            activeFormalPhaseSlopes_, 0),
                        effectiveVoiceCycles(activeVoiceCycles_,
                            activeFormalPhaseSlopes_, 1));
                    event.intensity = 1.0f - intensityAmount_
                        + intensityAmount_ * std::clamp(intensity, 0.0f, 1.0f);
                    event.intensity *= fieldWeight;
                    event.intensity *= 0.6f + 0.4f * voiceFormulaValue(voice, progress);
                    event.intensity *= grooveAccentAtProgress(
                        swungProgress(progress, swingRatio_, quarterNotesPerSection_),
                        grooveMode_, meterNumerator_, meterDenominator_);
                    event.intensity *= expressionAtProgress(
                        progress, dynamicCurve_, musicalIntention_);
                    event.intensity = rhythmMode_ ? 1.0f
                        : 1.0f - generativeDepth_ * (1.0f - event.intensity);
                    if (voice < activeVoiceCount_ && targetLevels[voice] > 0.0f)
                        excite(event);
                }
            }
            ++dynamicCycleIndices_[voice][stream];
            refreshDynamicCrossing(voice, stream, true);
            if (dynamicEventSamples_[voice][stream] ==
                std::numeric_limits<std::uint64_t>::max())
                break;
        }
    }
}

float ResonatorRenderer::voiceFormulaValue(const std::size_t voiceIndex,
                                           const double progress) const noexcept {
    if (voiceIndex >= recipe_.voiceCount)
        return 0.0f;
    const double theta = kTau * (effectiveVoiceCycles(
        activeVoiceCycles_, activeFormalPhaseSlopes_, voiceIndex) * progress
        + activeVoicePhases_[voiceIndex] + activeFormalPhaseOffsets_[voiceIndex]
        + voiceFormulaPhaseOffsets_[voiceIndex]);
    float sine = static_cast<float>(std::sin(theta));
    if (voiceIndex == 0) {
        const auto before = geometryEventTableSequence_.load(
            std::memory_order_acquire);
        if ((before & 1u) == 0u) {
            const bool enabled = geometryVoiceOneSource_.load(
                std::memory_order_relaxed);
            const int selected = geometryVoiceOneCurve_.load(
                std::memory_order_relaxed);
            const int terms = geometryVoiceOneFourierTerms_.load(
                std::memory_order_relaxed);
            const int billiardPattern = geometryVoiceOneBilliardPattern_.load(
                std::memory_order_relaxed);
            const auto after = geometryEventTableSequence_.load(
                std::memory_order_acquire);
            if (before == after && (after & 1u) == 0u && enabled) {
                const auto curve = static_cast<GeometryCurve>(
                std::clamp(selected, 0, 16));
                Point2D point;
                if (geometryCurvePoint(curve, progress, effectiveVoiceCycles(
                        activeVoiceCycles_, activeFormalPhaseSlopes_, 0),
                        activeVoicePhases_[0] + activeFormalPhaseOffsets_[0]
                            + voiceFormulaPhaseOffsets_[0],
                        terms, point, billiardPattern,
                        activeGeometryLissajous_))
                    sine = static_cast<float>(point.y);
            }
        }
    }
    float own = sine;
    switch (voiceTransforms_[voiceIndex]) {
        case VoiceTransform::sine:
            break;
        case VoiceTransform::invert:
            own = -sine;
            break;
        case VoiceTransform::rectify:
            own = 2.0f * std::abs(sine) - 1.0f;
            break;
        case VoiceTransform::harmonic:
            own = (sine + 0.5f * static_cast<float>(std::sin(2.0 * theta))) / 1.5f;
            break;
        case VoiceTransform::amplitude:
            own = sine * (0.65f + 0.35f * static_cast<float>(std::cos(theta * 0.5)));
            break;
        case VoiceTransform::quantized:
            own = std::round(sine * 4.0f) / 4.0f;
            break;
        case VoiceTransform::cubic:
            own = sine * sine * sine;
            break;
        case VoiceTransform::thirdHarmonic:
            own = static_cast<float>(std::sin(3.0 * theta));
            break;
        case VoiceTransform::triangle:
            own = static_cast<float>(0.63661977236758134308
                * std::asin(std::clamp(sine, -1.0f, 1.0f)));
            break;
        case VoiceTransform::softClip:
            own = static_cast<float>(std::tanh(2.0 * sine) / std::tanh(2.0));
            break;
    }
    const float coupling = voiceCouplings_[voiceIndex];
    if (voiceIndex == 0 || coupling <= 0.0f)
        return own;
    return (1.0f - coupling) * own
        + coupling * voiceFormulaValue(voiceIndex - 1, progress);
}

void ResonatorRenderer::process(float* left,
                               float* right,
                               const std::size_t frames) noexcept {
    processVariant(left, right, frames, 0);
}

void ResonatorRenderer::processVariant(float* left,
                                      float* right,
                                      const std::size_t frames,
                                      const std::size_t variant) noexcept {
    processWithSwitch(left, right, frames, variant, false, nullptr,
                      baseSectionSamples_);
}

void ResonatorRenderer::processVariantAtNextBar(float* left,
                                               float* right,
                                               const std::size_t frames,
                                               const std::size_t requestedVariant,
                                               const float* channelLevels,
                                               const std::uint64_t requestedSectionSamples) noexcept {
    processWithSwitch(left, right, frames, requestedVariant, true, channelLevels,
                      requestedSectionSamples);
}

void ResonatorRenderer::processVariantLive(float* left,
                                          float* right,
                                          const std::size_t frames,
                                          const std::size_t requestedVariant,
                                          const float* channelLevels,
                                          const std::uint64_t requestedSectionSamples) noexcept {
    processWithSwitch(left, right, frames, requestedVariant, false, channelLevels,
                      requestedSectionSamples);
}

bool ResonatorRenderer::isBarBoundary(const std::uint64_t position) const noexcept {
    return position == 0
        || position == sectionSamples_ / 4
        || position == sectionSamples_ / 2
        || position == sectionSamples_ * 3 / 4;
}

std::uint64_t ResonatorRenderer::sampleAtCurrentTempo(
    const ScheduledEvent& event) const noexcept {
    const double progress = static_cast<double>(event.sectionPhaseQ32) / 4294967296.0;
    const double performedProgress = batruc::swungProgress(
        progress, swingRatio_, quarterNotesPerSection_);
    const auto scaled = static_cast<std::uint64_t>(std::llround(
        performedProgress * static_cast<double>(sectionSamples_)));
    return std::min(scaled, sectionSamples_ == 0 ? 0 : sectionSamples_ - 1);
}

void ResonatorRenderer::setSwingRatio(const double ratio) noexcept {
    requestedSwingRatio_.store(std::clamp(ratio, 0.5, 0.67),
                               std::memory_order_relaxed);
}

void ResonatorRenderer::setGrooveMode(const GrooveMode groove) noexcept {
    requestedGrooveMode_.store(groove, std::memory_order_relaxed);
}

void ResonatorRenderer::setTimeSignature(const int numerator,
                                         const int denominator) noexcept {
    const bool supported = (denominator == 4
        && (numerator == 3 || numerator == 4 || numerator == 5))
        || (numerator == 7 && denominator == 8);
    requestedTimeSignature_.store(supported ? numerator * 100 + denominator : 404,
                                  std::memory_order_relaxed);
}

void ResonatorRenderer::applyRequestedTimeSignature() noexcept {
    const int packed = requestedTimeSignature_.load(std::memory_order_relaxed);
    const int numerator = packed / 100;
    const int denominator = packed % 100;
    if (numerator == meterNumerator_ && denominator == meterDenominator_)
        return;
    meterNumerator_ = numerator;
    meterDenominator_ = denominator;
    quarterNotesPerSection_ = 4.0 * numerator * 4.0 / denominator;
}

void ResonatorRenderer::setPerformanceExpression(
    const DynamicCurve dynamics, const MusicalIntention intention) noexcept {
    requestedDynamicCurve_.store(dynamics, std::memory_order_relaxed);
    requestedMusicalIntention_.store(intention, std::memory_order_relaxed);
}

void ResonatorRenderer::setSectionSamples(
    std::uint64_t sectionSamples) noexcept {
    if (!ready_ || schedules_.empty() || baseSectionSamples_ == 0
        || sectionSamples == 0 || sampleRate_ <= 0.0)
        return;
    const auto maximum = static_cast<std::uint64_t>(
        std::llround(kMaximumSectionSeconds * sampleRate_));
    const auto minimum = static_cast<std::uint64_t>(
        std::max(1.0, std::round(0.01 * sampleRate_)));
    sectionSamples = std::clamp(sectionSamples, minimum, maximum);
    const int oldNumerator = meterNumerator_;
    const int oldDenominator = meterDenominator_;
    applyRequestedTimeSignature();
    if (sectionSamples_ == sectionSamples) {
        if (meterNumerator_ != oldNumerator
            || meterDenominator_ != oldDenominator)
            refreshRhythmState(activeVariant_);
        return;
    }

    const auto previous = sectionSamples_;
    sectionPosition_ = previous == 0 ? 0
        : std::min(sectionPosition_ * sectionSamples / previous, sectionSamples - 1);
    sectionSamples_ = sectionSamples;
    const auto& schedule = schedules_[activeVariant_];
    nextEvent_ = static_cast<std::size_t>(std::lower_bound(
        schedule.begin(), schedule.end(), sectionPosition_,
        [this](const ScheduledEvent& event, const std::uint64_t sample) {
            return sampleAtCurrentTempo(event) < sample;
        }) - schedule.begin());
    publishedSectionSamples_.store(sectionSamples_, std::memory_order_relaxed);
    publishedSectionProgress_.store(
        static_cast<double>(sectionPosition_) / static_cast<double>(sectionSamples_),
        std::memory_order_relaxed);
    refreshRhythmState(activeVariant_);
}

void ResonatorRenderer::activateVariant(const std::size_t variant) noexcept {
    if (!ready_ || variant >= schedules_.size())
        return;
    if (activeVariant_ != variant) {
        activeVariant_ = variant;
        const auto& schedule = schedules_[activeVariant_];
        nextEvent_ = static_cast<std::size_t>(std::lower_bound(
            schedule.begin(), schedule.end(), sectionPosition_,
            [this](const ScheduledEvent& event, const std::uint64_t sample) {
                return sampleAtCurrentTempo(event) < sample;
            }) - schedule.begin());
    }
    refreshRhythmState(variant);
}

void ResonatorRenderer::selectVariantWhilePaused(
    const std::size_t variant,
    const std::uint64_t requestedSectionSamples) noexcept {
    if (requestedSectionSamples > 0) {
        applyRequestedTimeSignature();
        setSectionSamples(requestedSectionSamples);
    }
    activateVariant(variant);
    publishedVariant_.store(activeVariant_, std::memory_order_relaxed);
}

void ResonatorRenderer::setIntensityFormula(const IntensityLaw law,
                                           const float amount,
                                           const IntensityLaw secondLaw,
                                           const float blend) noexcept {
    intensityLaw_ = law;
    intensityAmount_ = finiteClamp(amount, 0.0f, 1.0f);
    secondIntensityLaw_ = secondLaw;
    intensityBlend_ = finiteClamp(blend, 0.0f, 1.0f);
}

void ResonatorRenderer::setDecayScale(const float scale) noexcept {
    const float next = finiteClamp(scale, 0.15f, 1.5f);
    if (!ready_ || std::abs(next - decayScale_) < 1.0e-4f)
        return;
    decayScale_ = next;
    for (std::size_t voiceIndex = 0; voiceIndex < recipe_.voiceCount; ++voiceIndex) {
        const auto& voice = recipe_.voices[voiceIndex];
        for (std::size_t layer = 0; layer < kMaxMidiNotesPerAttack; ++layer)
            for (std::size_t mode = 0; mode < kModesPerVoice; ++mode) {
                const double ratio = modeFrequencyRatios_[voiceIndex][layer][mode];
                const double lossScale = 1.0 + voice.resonator.frequencyDependentLoss
                    * ratio * ratio;
                const double decay = voice.resonator.decaySeconds
                    * static_cast<double>(decayScale_) / lossScale;
                decayCoefficients_[voiceIndex][layer][mode] = static_cast<float>(
                    std::exp(-1.0 / (decay * sampleRate_)));
            }
    }
}

void ResonatorRenderer::setFundamentalLevel(const float level) noexcept {
    fundamentalLevel_ = finiteClamp(level, 0.0f, 1.0f);
}

void ResonatorRenderer::processWithSwitch(float* left,
                                         float* right,
                                         const std::size_t frames,
                                         const std::size_t requestedVariant,
                                         const bool quantized,
                                         const float* channelLevels,
                                         const std::uint64_t requestedSectionSamples) noexcept {
    if (left == nullptr || right == nullptr)
        return;
    if (!ready_ || requestedVariant >= schedules_.size()) {
        std::fill_n(left, frames, 0.0f);
        std::fill_n(right, frames, 0.0f);
        smoothedVoiceRms_.fill(0.0f);
        for (auto& meter : publishedVoiceRms_)
            meter.store(0.0f, std::memory_order_relaxed);
        return;
    }

    std::array<double, kMaxActiveVoices> voiceSquareSums{};

    // Performance controls affect the next attack, without waiting for a bar.
    // Swing reschedules only events still ahead of the playhead.
    dynamicCurve_ = requestedDynamicCurve_.load(std::memory_order_relaxed);
    musicalIntention_ = requestedMusicalIntention_.load(
        std::memory_order_relaxed);
    grooveMode_ = requestedGrooveMode_.load(std::memory_order_relaxed);
    const double requestedSwing = requestedSwingRatio_.load(
        std::memory_order_relaxed);
    if (std::abs(requestedSwing - swingRatio_) > 1.0e-6) {
        swingRatio_ = std::clamp(requestedSwing, 0.5, 0.67);
        refreshRhythmState(activeVariant_);
        const auto& currentSchedule = schedules_[activeVariant_];
        nextEvent_ = static_cast<std::size_t>(std::lower_bound(
            currentSchedule.begin(), currentSchedule.end(), sectionPosition_,
            [this](const ScheduledEvent& event, const std::uint64_t sample) {
                return sampleAtCurrentTempo(event) < sample;
            }) - currentSchedule.begin());
    }

    if (!quantized)
        activateVariant(requestedVariant);
    const std::vector<ScheduledEvent>* schedule = &schedules_[activeVariant_];
    std::array<float, kMaxVoices> targetLevels{};
    for (std::size_t voice = 0; voice < recipe_.voiceCount; ++voice)
        targetLevels[voice] = voice >= activeVoiceCount_ ? 0.0f
            : formalVoiceScale(voice) * (channelLevels == nullptr
                ? 1.0f : finiteClamp(channelLevels[voice], 0.0f, 1.0f));

    for (std::size_t frame = 0; frame < frames; ++frame) {
        if (sectionPosition_ >= sectionSamples_) {
            sectionPosition_ = 0;
            nextEvent_ = 0;
            dynamicFirstVoiceAnchorFired_ = false;
            dynamicFirstVoiceAnchorSample_
                = std::numeric_limits<std::uint64_t>::max();
            if (requestedSectionSamples > 0) {
                applyRequestedTimeSignature();
                setSectionSamples(requestedSectionSamples);
            }
        }
        if (absoluteSample_ > 0 && isBarBoundary(sectionPosition_)) {
            ++completedBars_;
            if (completedBars_ % 4u == 0u) {
                applyFormalVoiceCount();
                for (std::size_t voice = 0; voice < recipe_.voiceCount; ++voice)
                    targetLevels[voice] = voice >= activeVoiceCount_ ? 0.0f
                        : formalVoiceScale(voice) * (channelLevels == nullptr ? 1.0f
                        : finiteClamp(channelLevels[voice], 0.0f, 1.0f));
            }
        }
        if (quantized && activeVariant_ != requestedVariant
            && isBarBoundary(sectionPosition_)) {
            activateVariant(requestedVariant);
            schedule = &schedules_[activeVariant_];
        } else if (absoluteSample_ > 0 && isBarBoundary(sectionPosition_)) {
            refreshRhythmState(activeVariant_);
        }
        while (nextEvent_ < schedule->size()
               && sampleAtCurrentTempo((*schedule)[nextEvent_]) <= sectionPosition_) {
            auto event = (*schedule)[nextEvent_].event;
            const bool usePreparedEvent = !activeDynamicRhythm_[event.voice];
            // Dynamic rhythm voices normally replace their prepared schedule
            // with sample-accurate crossings. Relation-derived events are
            // deliberate responses from another voice and must survive that
            // replacement; their bounded plan was prepared off the audio thread.
            if (!usePreparedEvent && !event.generatedByRelation) {
                ++nextEvent_;
                continue;
            }
            const float x = std::clamp(event.fieldX, -1.0f, 1.0f);
            const float y = std::clamp(event.fieldY, -1.0f, 1.0f);
            const float formulaValue = blendedIntensityFormulaValue(
                intensityLaw_, secondIntensityLaw_, intensityBlend_, x, y,
                effectiveVoiceCycles(activeVoiceCycles_,
                    activeFormalPhaseSlopes_, 0),
                effectiveVoiceCycles(activeVoiceCycles_,
                    activeFormalPhaseSlopes_, 1));
            event.intensity *= 1.0f - intensityAmount_
                + intensityAmount_ * std::clamp(formulaValue, 0.0f, 1.0f);
            event.intensity *= 0.6f + 0.4f * voiceFormulaValue(
                event.voice, event.timeSeconds / recipe_.sectionSeconds);
            event.intensity *= grooveAccentAtProgress(
                swungProgress(event.timeSeconds / recipe_.sectionSeconds,
                             swingRatio_, quarterNotesPerSection_), grooveMode_,
                meterNumerator_, meterDenominator_);
            event.intensity *= expressionAtProgress(
                event.timeSeconds / recipe_.sectionSeconds,
                dynamicCurve_, musicalIntention_);
            if (variantTimings_[activeVariant_].patternMode
                    == PatternMode::unrestricted
                || recipe_.rhythmicCells[event.voice].enabled) {
                const double fieldProgress = static_cast<double>(completedBars_) / 4.0
                    + event.timeSeconds / recipe_.sectionSeconds;
                event.intensity *= patternFieldWeight(
                    variantTimings_[activeVariant_].patternMode,
                    x, y, fieldProgress, developmentSeed_);
            }
            {
                const float written = (*schedule)[nextEvent_].event.intensity;
                if (rhythmMode_)   // o evento preparado toca com o acento escrito
                    event.intensity = written;
                else if (written > 0.0f)
                    event.intensity = written * (1.0f - generativeDepth_
                        * (1.0f - std::clamp(event.intensity / written, 0.0f, 1.0f)));
            }
            if (event.voice < activeVoiceCount_
                && targetLevels[event.voice] > 0.0f)
                excite(event);
            ++nextEvent_;
        }
        if (sectionPosition_ >= nextDynamicRhythmSample_) {
            for (std::size_t voice = 0; voice < recipe_.voiceCount; ++voice)
                if (activeDynamicRhythm_[voice])
                    triggerDynamicVoiceEvents(voice, sectionPosition_, targetLevels.data());
        }
        if (sectionPosition_ >= nextDynamicRhythmSample_
            && sectionPosition_ == dynamicFirstVoiceAnchorSample_) {
            if (activeVoiceCount_ > 0 && targetLevels[0] > 0.0f) {
                auto event = dynamicFirstVoiceAnchor_;
                const double progress = event.timeSeconds / recipe_.sectionSeconds;
                const float formula = blendedIntensityFormulaValue(intensityLaw_,
                    secondIntensityLaw_, intensityBlend_,
                    event.fieldX, event.fieldY,
                    effectiveVoiceCycles(activeVoiceCycles_, activeFormalPhaseSlopes_, 0),
                    effectiveVoiceCycles(activeVoiceCycles_, activeFormalPhaseSlopes_, 1));
                event.intensity *= 1.0f - intensityAmount_
                    + intensityAmount_ * std::clamp(formula, 0.0f, 1.0f);
                event.intensity *= 0.6f + 0.4f * voiceFormulaValue(0, progress);
                event.intensity *= grooveAccentAtProgress(
                    swungProgress(progress, swingRatio_, quarterNotesPerSection_),
                    grooveMode_, meterNumerator_, meterDenominator_);
                event.intensity *= expressionAtProgress(
                    progress, dynamicCurve_, musicalIntention_);
                excite(event);
            }
            dynamicFirstVoiceAnchorFired_ = true;
            dynamicFirstVoiceAnchorSample_
                = std::numeric_limits<std::uint64_t>::max();
        }
        if (sectionPosition_ >= nextDynamicRhythmSample_)
            updateNextDynamicEventSample();

        float leftSample = 0.0f;
        float rightSample = 0.0f;
        // The formal fifth cycle used to be a mandatory suspension. Form is
        // now continuous by default: silence must come from an explicit
        // musical edit, not from a hidden transport rule.
        inFormalPause_ = false;
        for (std::size_t voiceIndex = 0; voiceIndex < renderVoiceCount_; ++voiceIndex) {
            auto& level = currentChannelLevels_[voiceIndex];
            level += (targetLevels[voiceIndex] - level) * channelSlew_;
            if (targetLevels[voiceIndex] <= 1.0e-5f) {
                panTargetLeft_[voiceIndex] = panBaseLeft_[voiceIndex];
                panTargetRight_[voiceIndex] = panBaseRight_[voiceIndex];
            }
            panLeft_[voiceIndex] += (panTargetLeft_[voiceIndex] - panLeft_[voiceIndex])
                * channelSlew_;
            panRight_[voiceIndex] += (panTargetRight_[voiceIndex] - panRight_[voiceIndex])
                * channelSlew_;
            const float mono = renderVoice(voiceIndex)
                * recipe_.voices[voiceIndex].gain * level;
            if (voiceIndex < voiceSquareSums.size())
                voiceSquareSums[voiceIndex] += static_cast<double>(mono) * mono;
            leftSample += mono * panLeft_[voiceIndex];
            rightSample += mono * panRight_[voiceIndex];
        }
        while (renderVoiceCount_ > activeVoiceCount_
               && currentChannelLevels_[renderVoiceCount_ - 1] < 1.0e-4f)
            --renderVoiceCount_;

        // Bounded, symmetric soft clipping is the final safety stage.
        left[frame] = finiteClamp(leftSample, -4.0f, 4.0f)
            / (1.0f + std::abs(finiteClamp(leftSample, -4.0f, 4.0f)));
        right[frame] = finiteClamp(rightSample, -4.0f, 4.0f)
            / (1.0f + std::abs(finiteClamp(rightSample, -4.0f, 4.0f)));
        ++sectionPosition_;
        ++absoluteSample_;
    }
    publishedSamples_.store(absoluteSample_, std::memory_order_relaxed);
    publishedBars_.store(completedBars_, std::memory_order_relaxed);
    publishedVariant_.store(activeVariant_, std::memory_order_relaxed);
    publishedSectionSamples_.store(sectionSamples_, std::memory_order_relaxed);
    publishedSectionProgress_.store(
        static_cast<double>(sectionPosition_) / static_cast<double>(sectionSamples_),
        std::memory_order_relaxed);
    for (std::size_t voice = 0; voice < publishedVoiceRms_.size(); ++voice) {
        const float blockRms = frames > 0
            ? static_cast<float>(std::sqrt(voiceSquareSums[voice] / frames))
            : 0.0f;
        auto& meter = smoothedVoiceRms_[voice];
        meter += (blockRms - meter) * (blockRms > meter ? 0.65f : 0.18f);
        publishedVoiceRms_[voice].store(meter, std::memory_order_relaxed);
    }
}

float ResonatorRenderer::finiteClamp(const float value,
                                     const float low,
                                     const float high) noexcept {
    if (!finite(value))
        return 0.0f;
    return std::clamp(value, low, high);
}

float ResonatorRenderer::lookupSine(const double phase) const noexcept {
    const double position = phase * static_cast<double>(kSineTableSize) / kTau;
    const auto index = static_cast<std::size_t>(position);
    const float fraction = static_cast<float>(position - static_cast<double>(index));
    const float first = sineTable_[index];
    return first + (sineTable_[index + 1] - first) * fraction;
}

float ResonatorRenderer::nextNoise(ModalVoiceState& state) noexcept {
    state.noiseState ^= state.noiseState << 13;
    state.noiseState ^= state.noiseState >> 17;
    state.noiseState ^= state.noiseState << 5;
    const float unit = static_cast<float>(state.noiseState >> 8)
        / 16777216.0f;
    return 2.0f * unit - 1.0f;
}

void ResonatorRenderer::excite(const MusicalEvent& event) noexcept {
    const float calculatedPassageWeight = developmentEnabled_
        ? passageEventWeight(developmentSeed_,
            completedBars_ / (4u * kFormalCycleCount), event.voice,
            event.timeSeconds / recipe_.sectionSeconds)
        : 1.0f;
    const float variationDepth = stemVariationDepth_[event.voice];
    const auto voiceSeed = developmentSeed_
        ^ (static_cast<std::uint32_t>(event.voice) * 0x9e3779b9u);
    const double progress = std::clamp(
        event.timeSeconds / recipe_.sectionSeconds, 0.0, 1.0);
    const double seedPhase = static_cast<double>(voiceSeed & 0xffffu)
        / 65536.0 * kTau;
    const double seedRate = 2.0 + static_cast<double>((voiceSeed >> 16u) % 5u);
    const float calculationScore = static_cast<float>(std::clamp(
        0.55 * std::clamp(event.fieldY, -1.0f, 1.0f)
        + 0.25 * std::clamp(event.fieldX, -1.0f, 1.0f)
        + 0.20 * std::sin(kTau * progress * seedRate + seedPhase), -1.0, 1.0));
    const float calculatedEventWeight = calculatedPassageWeight
        * (1.0f + 0.20f * calculationScore);
    const float passageWeight = 1.0f + variationDepth
        * (calculatedEventWeight - 1.0f);
    if (passageWeight <= 0.0f)
        return;
    const auto layer = std::min<std::size_t>(event.soundLayer,
        kMaxMidiNotesPerAttack - 1);
    auto& state = states_[event.voice][layer];
    state.active = true;
    const bool isHiHatMidiNote = event.midiNote == 22u || event.midiNote == 26u
        || event.midiNote == 42u || event.midiNote == 44u
        || event.midiNote == 46u;
    state.hiHatOpenAmount = isHiHatMidiNote
        && event.midiHatPedal != kNoMidiControllerValue
        ? 1.0f - static_cast<float>(event.midiHatPedal) / 127.0f
        : -1.0f;
    const auto laneSound = channelSounds_[event.voice];
    const auto sound = event.midiNote == 58u && event.voice == 4
            && laneSound == defaultMidiLaneSound(4)
        ? SoundCharacter::membrane // TD-11 floor-tom rim in the tom lane
        : (event.midiNote == 22u || event.midiNote == 26u)
            && event.voice != 2 && event.voice != 3
        ? laneSound // undefined GM notes must not remap a generic lane
        : event.midiNote <= 127u && event.voice < 8
            && laneSound == defaultMidiLaneSound(event.voice)
        ? soundForGeneralMidiDrum(event.midiNote, channelSounds_[event.voice])
        : laneSound;
    const bool crashCymbal = sound == SoundCharacter::ride
        && (event.midiNote == 49u || event.midiNote == 52u
            || event.midiNote == 55u || event.midiNote == 57u);
    const bool rideBell = sound == SoundCharacter::ride
        && event.midiNote == 53u;
    if (activeVoiceSounds_[event.voice][layer] != sound) {
        activeVoiceSounds_[event.voice][layer] = sound;
        refreshVoiceSpectrum(event.voice, layer);
    }
    state.excitationNoiseColour = event.midiNote <= 127u && sound != laneSound
        ? noiseColourForSound(sound) : noiseColours_[event.voice][layer];
    state.excitationNoiseColour *= channelTimbreNoiseGains_[event.voice];
    double bodySeconds = 0.0;
    switch (sound) {
        case SoundCharacter::kick: bodySeconds = 0.18; break;
        case SoundCharacter::snare: bodySeconds = 0.095; break;
        case SoundCharacter::hihat: bodySeconds = 0.012; break;
        case SoundCharacter::openHihat: bodySeconds = 0.065; break;
        case SoundCharacter::rimClick: bodySeconds = 0.035; break;
        case SoundCharacter::shaker: bodySeconds = 0.008; break;
        case SoundCharacter::agogo: bodySeconds = 0.18; break;
        case SoundCharacter::woodblock: bodySeconds = 0.055; break;
        case SoundCharacter::ride:
            bodySeconds = crashCymbal ? 0.24 : rideBell ? 0.16 : 0.12;
            break;
        case SoundCharacter::membrane: bodySeconds = 0.16; break;
        case SoundCharacter::conga: bodySeconds = 0.12; break;
        case SoundCharacter::brushes: bodySeconds = 0.055; break;
        default: break;
    }
    if (state.hiHatOpenAmount >= 0.0f)
        bodySeconds = 0.012 + 0.053 * state.hiHatOpenAmount;
    if (event.midiNote <= 127u && bodySeconds > 0.0
        && state.hiHatOpenAmount < 0.0f)
        bodySeconds = std::min(bodySeconds,
            generalMidiDrumDecaySeconds(event.midiNote));
    state.excitationDecayCoefficient = bodySeconds > 0.0
        ? static_cast<float>(std::exp(-1.0 / (sampleRate_ * bodySeconds)))
        : 1.0f;
    if (isPercussive(sound)) {
        float density = 0.0f;
        if (state.hasPreviousPercussionAttack
            && absoluteSample_ > state.previousPercussionAttackSample) {
            const double interval = static_cast<double>(
                absoluteSample_ - state.previousPercussionAttackSample)
                / sampleRate_;
            density = static_cast<float>(std::clamp(
                (0.55 - interval) / (0.55 - 0.08), 0.0, 1.0));
        }
        const float intensity = std::clamp(event.intensity, 0.0f, 1.0f);
        float intentionScale = 1.0f;
        switch (musicalIntention_) {
            case MusicalIntention::build:
                intentionScale = 0.70f;
                break;
            case MusicalIntention::thin:
                intentionScale = 1.20f;
                break;
            case MusicalIntention::meet:
                intentionScale = 0.85f;
                break;
            case MusicalIntention::contrast: {
                const auto cell = static_cast<unsigned>(
                    std::max(0.0, std::floor(event.timeSeconds * 8.0)));
                intentionScale = ((cell + static_cast<unsigned>(event.voice)) & 1u)
                    == 0u ? 0.70f : 1.20f;
                break;
            }
        }
        const float damping = std::clamp(
            density * 0.35f * (1.10f - 0.50f * intensity) * intentionScale
                * variationDepth,
            0.0f, 0.42f);
        state.percussionDampingCoefficient = static_cast<float>(std::exp(
            -static_cast<double>(damping) / (0.22 * sampleRate_)));
        state.previousPercussionAttackSample = absoluteSample_;
        state.hasPreviousPercussionAttack = true;
    } else if (state.percussionDampingCoefficient < 0.999999f
               || state.hasPreviousPercussionAttack) {
        state.percussionDampingCoefficient = 1.0f;
        state.hasPreviousPercussionAttack = false;
    }
    if (isPercussive(sound) && panMotionDepth_[event.voice] > 1.0e-6f) {
        float trajectorySide = panTrajectories_[event.voice].enabled
            ? samplePanTrajectory(panTrajectories_[event.voice],
                static_cast<float>(event.timeSeconds / recipe_.sectionSeconds))
            : std::clamp(event.fieldX, -1.0f, 1.0f);
        const float eventIntensity = std::clamp(event.intensity, 0.0f, 1.0f);
        float intentionScale = 1.0f;
        switch (musicalIntention_) {
            case MusicalIntention::build:
                intentionScale = 0.35f + 0.65f * eventIntensity;
                break;
            case MusicalIntention::thin:
                intentionScale = 0.18f + 0.32f * eventIntensity;
                break;
            case MusicalIntention::meet:
                intentionScale = 1.0f - 0.65f * std::abs(trajectorySide);
                break;
            case MusicalIntention::contrast: {
                const auto cell = static_cast<unsigned>(
                    std::max(0.0, std::floor(event.timeSeconds * 8.0)));
                if (((cell + static_cast<unsigned>(event.voice)) & 1u) != 0u)
                    trajectorySide = -trajectorySide;
                break;
            }
        }
        const float targetPan = panBase_[event.voice]
            + panMotionDepth_[event.voice] * variationDepth
                * intentionScale * trajectorySide;
        setPanTarget(event.voice, targetPan);
    }
    if (developmentEnabled_) {
        constexpr std::array<double, 5> semitoneRatios = {
            0.8908987181403393, 0.9438743126816935, 1.0,
            1.0594630943592953, 1.122462048309373};
        const auto passage = completedBars_ / (4u * kFormalCycleCount);
        const int passageOffset = passagePitchOffset(voiceSeed, passage);
        const int localOffset = static_cast<int>(std::lround(
            2.0f * calculationScore));
        const int offset = std::clamp(passageOffset + localOffset, -2, 2);
        const double calculatedRatio = semitoneRatios[
            static_cast<std::size_t>(std::clamp(offset, -2, 2) + 2)];
        state.targetPassageFrequencyScale = static_cast<float>(1.0
            + variationDepth * (calculatedRatio - 1.0));
    } else {
        state.targetPassageFrequencyScale = 1.0;
    }
    const auto& profile = recipe_.voices[event.voice].resonator;
    state.transientFrequencyScale = sound == SoundCharacter::kick
        ? (modernDrumBody_ ? 2.4 : 1.85) : 1.0;
    state.notePitchScale = 1.0;
    if (rideBell)
        state.notePitchScale = 1.45;
    if (sound == SoundCharacter::membrane) {
        switch (event.midiNote) {
            case 41: state.notePitchScale = 0.70; break;
            case 43: state.notePitchScale = 0.80; break;
            case 45: state.notePitchScale = 0.90; break;
            case 47: state.notePitchScale = 1.00; break;
            case 48: state.notePitchScale = 1.12; break;
            case 50: state.notePitchScale = 1.26; break;
            case 65: state.notePitchScale = 1.32; break;
            case 66: state.notePitchScale = 0.88; break;
            case 75: state.notePitchScale = 1.55; break;
            case 76: state.notePitchScale = 1.24; break;
            case 77: state.notePitchScale = 0.82; break;
            default: break;
        }
    }
    if (sound == SoundCharacter::conga) {
        switch (event.midiNote) {
            case 60: state.notePitchScale = 1.36; break;
            case 61: state.notePitchScale = 0.96; break;
            case 62: state.notePitchScale = 1.22; break;
            case 63: state.notePitchScale = 1.14; break;
            case 64: state.notePitchScale = 0.78; break;
            default: break;
        }
    }
    if (sound == SoundCharacter::bell) {
        if (event.midiNote == 67u) state.notePitchScale = 1.28;
        else if (event.midiNote == 68u) state.notePitchScale = 0.82;
    }
    if (sound == SoundCharacter::agogo) {
        if (event.midiNote == 67u) state.notePitchScale = 1.18;
        else if (event.midiNote == 68u) state.notePitchScale = 0.84;
    }
    if (sound == SoundCharacter::woodblock) {
        if (event.midiNote == 75u) state.notePitchScale = 1.45; // claves/high
        else if (event.midiNote == 76u) state.notePitchScale = 1.20;
        else if (event.midiNote == 77u) state.notePitchScale = 0.82;
    }
    // A generated percussion phrase can move between a few tuned drum
    // positions. Imported MIDI notes keep their explicit articulation/pitch.
    // Kick, snare and cymbals retain their instrumental register.
    if (developmentEnabled_ && event.midiNote == kNoMidiNote
        && (sound == SoundCharacter::membrane
            || sound == SoundCharacter::conga)) {
        const int semitones = static_cast<int>(std::lround(
            calculationScore * variationDepth
                * (sound == SoundCharacter::membrane ? 4.0f : 2.0f)));
        state.notePitchScale = std::pow(2.0,
            static_cast<double>(semitones) / 12.0);
    }
    if (percussionKeyTonic_ >= 0) {   // percussão no tom da seed
        const double fundamental = recipe_.voices[event.voice].resonanceHz
            * modeFrequencyRatios_[event.voice][layer][0]
            * state.targetPassageFrequencyScale * state.notePitchScale;
        state.notePitchScale *= keyTunedPitchScale(sound, fundamental);
    }
    for (std::size_t mode = 0; mode < kModesPerVoice; ++mode) {
        float modeGain = 1.0f;
        if (sound == SoundCharacter::kick)
            modeGain = mode == 0 ? 1.0f : mode == 1 ? 0.28f : 0.035f;
        else if (sound == SoundCharacter::hihat)
            modeGain = mode < 2 ? 0.18f : 0.72f;
        else if (sound == SoundCharacter::openHihat)
            modeGain = mode < 2 ? 0.25f : 0.86f;
        if (state.hiHatOpenAmount >= 0.0f)
            modeGain = mode < 2
                ? 0.18f + 0.07f * state.hiHatOpenAmount
                : 0.72f + 0.14f * state.hiHatOpenAmount;
        else if (sound == SoundCharacter::rimClick)
            modeGain = mode < 2 ? 0.92f : 0.30f;
        else if (sound == SoundCharacter::ride)
            modeGain = crashCymbal ? (mode < 2 ? 0.18f : 0.90f)
                : rideBell ? (mode < 2 ? 0.76f : 0.58f)
                : (mode < 2 ? 0.42f : 0.78f);
        else if (sound == SoundCharacter::brushes)
            modeGain = 0.22f;
        else if (sound == SoundCharacter::shaker)
            modeGain = mode < 2 ? 0.06f : 0.0f;
        else if (sound == SoundCharacter::agogo)
            modeGain = mode < 5 ? 0.80f : 0.36f;
        else if (sound == SoundCharacter::woodblock)
            modeGain = mode < 3 ? 0.82f : 0.22f;
        else if (sound == SoundCharacter::chip8)
            modeGain = mode < 4 ? 0.72f : 0.18f;
        const float increment = event.intensity * passageWeight
            * 0.24f * modeGain * channelTimbreBodyGains_[event.voice];
        state.pendingExcitation[mode] = std::min(
            std::max(0.0f, 1.5f - state.amplitudes[mode]),
            state.pendingExcitation[mode] + increment);
    }
    state.excitationSamplesLeft = static_cast<std::uint32_t>(
        std::max(1.0, std::round(sampleRate_ * 0.002)));
    double noiseSeconds = 0.003;
    if (sound == SoundCharacter::snare) noiseSeconds = 0.13;
    else if (sound == SoundCharacter::hihat) noiseSeconds = 0.038;
    else if (sound == SoundCharacter::openHihat) noiseSeconds = 0.22;
    else if (sound == SoundCharacter::rimClick) noiseSeconds = 0.008;
    else if (sound == SoundCharacter::ride) noiseSeconds = 0.16;
    else if (sound == SoundCharacter::brushes) noiseSeconds = 0.075;
    else if (sound == SoundCharacter::conga) noiseSeconds = 0.012;
    else if (sound == SoundCharacter::chip8) noiseSeconds = 0.024;
    else if (sound == SoundCharacter::shaker) noiseSeconds = 0.055;
    else if (sound == SoundCharacter::agogo) noiseSeconds = 0.035;
    else if (sound == SoundCharacter::woodblock) noiseSeconds = 0.008;
    if (event.midiNote == 42u) noiseSeconds = 0.045; // closed hat
    else if (event.midiNote == 44u) noiseSeconds = 0.065; // pedal hat
    else if (event.midiNote == 46u || event.midiNote == 26u)
        noiseSeconds = 0.22; // open hat
    else if (event.midiNote == 49u || event.midiNote == 52u
             || event.midiNote == 55u || event.midiNote == 57u)
        noiseSeconds = 0.24; // crash, china, splash
    else if (event.midiNote == 51u || event.midiNote == 53u
             || event.midiNote == 59u)
        noiseSeconds = 0.16; // ride family
    else if (event.midiNote == 54u) noiseSeconds = 0.10;
    else if (event.midiNote == 69u) noiseSeconds = 0.075;
    else if (event.midiNote == 70u) noiseSeconds = 0.045;
    else if (event.midiNote == 58u) noiseSeconds = 0.12;
    else if (event.midiNote == 73u) noiseSeconds = 0.08;
    else if (event.midiNote == 74u) noiseSeconds = 0.25;
    if (state.hiHatOpenAmount >= 0.0f)
        noiseSeconds = 0.038 + 0.182 * state.hiHatOpenAmount;
    noiseSeconds *= channelTimbreNoiseDurations_[event.voice];
    state.noiseSamplesLeft = static_cast<std::uint32_t>(
        std::max(1.0, std::round(sampleRate_ * noiseSeconds)));
    state.noiseDecayCoefficient = static_cast<float>(std::exp(
        std::log(0.004) / std::max(1.0, sampleRate_ * noiseSeconds)));
    float noiseStrike = profile.strikeNoise;
    switch (sound) {
        case SoundCharacter::kick: noiseStrike = 0.14f; break;
        case SoundCharacter::snare: noiseStrike = 0.22f; break;
        case SoundCharacter::hihat: noiseStrike = 0.52f; break;
        case SoundCharacter::openHihat: noiseStrike = 0.66f; break;
        case SoundCharacter::rimClick: noiseStrike = 0.08f; break;
        case SoundCharacter::ride:
            noiseStrike = crashCymbal ? 0.68f : rideBell ? 0.24f : 0.42f;
            break;
        case SoundCharacter::shaker: noiseStrike = 0.72f; break;
        case SoundCharacter::agogo: noiseStrike = 0.16f; break;
        case SoundCharacter::woodblock: noiseStrike = 0.08f; break;
        case SoundCharacter::brushes: noiseStrike = 0.22f; break;
        case SoundCharacter::conga: noiseStrike = 0.11f; break;
        default: break;
    }
    if (state.hiHatOpenAmount >= 0.0f)
        noiseStrike = 0.52f + 0.14f * state.hiHatOpenAmount;
    state.noiseEnvelopeTarget = std::max(state.noiseEnvelopeTarget,
        noiseStrike * event.intensity * passageWeight);
    const bool hatBody = sound == SoundCharacter::hihat
        || sound == SoundCharacter::openHihat || state.hiHatOpenAmount >= 0.0f;
    if (modernDrumBody_ && hatBody) {
        state.metalEnvelope = std::max(state.metalEnvelope,
            0.62f * event.intensity * passageWeight);
        state.metalDecay = state.noiseDecayCoefficient;
    }
    if (sound == SoundCharacter::snare) {
        // Independent, band-limited wire response excited by the skin strike.
        // It decays separately from the membrane and the short attack noise.
        state.snareWireEnvelope = std::max(state.snareWireEnvelope,
            0.62f * event.intensity * passageWeight);
        state.snareWireDecay = static_cast<float>(std::exp(
            std::log(0.004) / std::max(1.0, sampleRate_ * 0.11)));
    }
}

float ResonatorRenderer::renderVoice(const std::size_t voiceIndex) noexcept {
    float sample = 0.0f;
    for (std::size_t layer = 0; layer < kMaxMidiNotesPerAttack; ++layer)
        if (states_[voiceIndex][layer].active)
            sample += renderVoiceLayer(voiceIndex, layer);
    return sample;
}

float ResonatorRenderer::renderVoiceLayer(const std::size_t voiceIndex,
                                           const std::size_t layer) noexcept {
    auto& state = states_[voiceIndex][layer];
    const auto& voice = recipe_.voices[voiceIndex];
    state.passageFrequencyScale += 0.0001
        * (state.targetPassageFrequencyScale - state.passageFrequencyScale);
    const auto sound = activeVoiceSounds_[voiceIndex][layer];
    if (sound == SoundCharacter::kick)
        state.transientFrequencyScale += (modernDrumBody_ ? 0.0009 : 0.00045)
            * (1.0 - state.transientFrequencyScale);
    float sample = 0.0f;
    if (state.excitationSamplesLeft > 0) {
        for (std::size_t mode = 0; mode < kModesPerVoice; ++mode) {
            const float step = state.pendingExcitation[mode]
                / static_cast<float>(state.excitationSamplesLeft);
            state.amplitudes[mode] += step;
            state.pendingExcitation[mode] -= step;
        }
        --state.excitationSamplesLeft;
    }
    for (std::size_t mode = 0; mode < kModesPerVoice; ++mode) {
        const double requestedFrequency = voice.resonanceHz
            * modeFrequencyRatios_[voiceIndex][layer][mode]
            * state.passageFrequencyScale
            * state.notePitchScale
            * (sound == SoundCharacter::kick ? state.transientFrequencyScale : 1.0);
        const bool modeBelowNyquist = requestedFrequency < sampleRate_ * 0.45;
        const double frequency = std::min(requestedFrequency,
                                          sampleRate_ * 0.45);
        const double phaseIncrement = kTau * frequency / sampleRate_;
        const float spectralShape = modeSpectralWeights_[voiceIndex][layer][mode];
        const float firstModeBalance = mode == 0
            && balanceFundamentalByLevel_[voiceIndex][layer]
            ? fundamentalLevel_ : 1.0f;
        if (modeBelowNyquist && spectralShape != 0.0f)
            sample += state.amplitudes[mode] * voice.resonator.modeWeights[mode]
                * spectralShape * firstModeBalance
                * lookupSine(state.phases[mode]);
        state.phases[mode] += phaseIncrement;
        if (state.phases[mode] >= kTau)
            state.phases[mode] -= kTau;
        state.amplitudes[mode] *= decayCoefficients_[voiceIndex][layer][mode]
            * state.excitationDecayCoefficient
            * state.percussionDampingCoefficient
            * channelTimbreTailCoefficients_[voiceIndex];
    }
    if (state.noiseSamplesLeft > 0) {
        state.noiseEnvelope += 0.18f
            * (state.noiseEnvelopeTarget - state.noiseEnvelope);
        const float noiseMix = sound == SoundCharacter::chip8
            ? chip8Parameters_[voiceIndex].noiseMix
            : state.excitationNoiseColour;
        const float rawNoise = nextNoise(state);
        state.noiseLowpass += 0.18f * (rawNoise - state.noiseLowpass);
        if (sound == SoundCharacter::snare) {
            state.snareWireFast += 0.32f * (rawNoise - state.snareWireFast);
            state.snareWireSlow += 0.045f * (rawNoise - state.snareWireSlow);
            sample += (state.snareWireFast - state.snareWireSlow)
                * state.snareWireEnvelope * state.excitationNoiseColour
                * state.percussionDampingCoefficient;
            state.snareWireEnvelope *= state.snareWireDecay;
        }
        const float closedHatNoise = rawNoise - state.noiseLowpass;
        const float openHatNoise = 0.45f * rawNoise
            + 0.55f * closedHatNoise;
        const float shapedNoise = state.hiHatOpenAmount >= 0.0f
            ? closedHatNoise + state.hiHatOpenAmount
                * (openHatNoise - closedHatNoise)
            : sound == SoundCharacter::hihat
            ? rawNoise - state.noiseLowpass
            : sound == SoundCharacter::openHihat
                ? 0.45f * rawNoise + 0.55f * (rawNoise - state.noiseLowpass)
            : sound == SoundCharacter::ride
                ? rawNoise - state.noiseLowpass
                : sound == SoundCharacter::brushes
                    ? state.noiseLowpass
                    : sound == SoundCharacter::shaker
                        ? rawNoise - state.noiseLowpass
                    : rawNoise;
        const bool hatVoice = sound == SoundCharacter::hihat
            || sound == SoundCharacter::openHihat || state.hiHatOpenAmount >= 0.0f;
        const float hatNoiseWeight = modernDrumBody_ && hatVoice ? 0.7f : 1.0f;
        sample += shapedNoise * state.noiseEnvelope * noiseMix * hatNoiseWeight
            * state.percussionDampingCoefficient;
        state.noiseEnvelopeTarget *= state.noiseDecayCoefficient;
        --state.noiseSamplesLeft;
        if (state.noiseSamplesLeft == 0) {
            state.noiseEnvelope = 0.0f;
            state.noiseEnvelopeTarget = 0.0f;
        }
    }
    if (modernDrumBody_ && state.metalEnvelope > 1.0e-5f) {
        // Seis quadrados em razões inarmônicas (as frequências próximas das
        // usadas no chimbal analógico clássico; conceito público, código
        // próprio), com polyBLEP contra aliasing, e dois passa-altas de um
        // polo (~6,5 kHz): o brilho metálico que o ruído sozinho não tem.
        static constexpr double kMetalHz[6] = {205.3, 304.4, 369.6, 522.7, 540.0, 800.0};
        float metal = 0.0f;
        for (std::size_t k = 0; k < 6; ++k) {
            const double dt = std::min(0.45, kMetalHz[k] * state.notePitchScale / sampleRate_);
            double& phase = state.metalPhases[k];
            phase += dt;
            if (phase >= 1.0) phase -= 1.0;
            float square = phase < 0.5 ? 1.0f : -1.0f;
            auto blep = [dt](double t) -> float {
                if (t < dt) { t /= dt; return static_cast<float>(t + t - t * t - 1.0); }
                if (t > 1.0 - dt) { t = (t - 1.0) / dt; return static_cast<float>(t * t + t + t + 1.0); }
                return 0.0f;
            };
            square += blep(phase);
            double shifted = phase + 0.5;
            if (shifted >= 1.0) shifted -= 1.0;
            square -= blep(shifted);
            metal += square;
        }
        metal *= 1.0f / 6.0f;
        const float hpA = static_cast<float>(1.0 / (1.0 + kTau * 6500.0 / sampleRate_));
        const float h1 = hpA * (state.metalHpOut1 + metal - state.metalHpIn1);
        state.metalHpIn1 = metal; state.metalHpOut1 = h1;
        const float h2 = hpA * (state.metalHpOut2 + h1 - state.metalHpIn2);
        state.metalHpIn2 = h1; state.metalHpOut2 = h2;
        // ganho calibrado por simulação (12/11): o metal filtrado sai ~14×
        // mais fraco que o ruído do chimbal antigo; 12× repõe o agudo
        sample += 12.0f * h2 * state.metalEnvelope * state.percussionDampingCoefficient;
        state.metalEnvelope *= state.metalDecay;
    }
    if (sound == SoundCharacter::chip8) {
        const auto parameters = chip8Parameters_[voiceIndex];
        const double pulsePhase = state.phases[0] / kTau;
        const float pulse = pulsePhase < parameters.dutyCycle ? 1.0f : -1.0f;
        sample = 0.72f * sample + 0.28f * pulse * state.amplitudes[0];
        // Bounded sample-and-hold followed by signed amplitude quantisation.
        // This is a native chip body, not a bytebeat expression evaluator.
        if (state.chipHoldSamplesLeft == 0) {
            const float levels = static_cast<float>(
                (1u << static_cast<unsigned>(parameters.bitDepth - 1)) - 1u);
            const float bounded = std::clamp(sample, -1.0f, 1.0f);
            state.chipHeldSample = std::round(bounded * levels) / levels;
            state.chipHoldSamplesLeft = static_cast<std::uint32_t>(
                std::max(1.0, std::round(sampleRate_ / parameters.clockHz)));
        }
        sample = state.chipHeldSample;
        --state.chipHoldSamplesLeft;
    } else if (channelTimbreProfiles_[voiceIndex] == 8) {
        // The 8-bit family keeps each drum's kick/snare/hat body, then gives
        // that role its own bounded digital clock and resolution. Previously
        // the family changed only analogue profile coefficients, so the
        // selected 8-bit kit never actually reached the chip treatment.
        int bits = 5;
        double clockHz = 8000.0;
        switch (sound) {
            case SoundCharacter::kick: bits = 6; clockHz = 12000.0; break;
            case SoundCharacter::snare: bits = 5; clockHz = 8000.0; break;
            case SoundCharacter::hihat: bits = 4; clockHz = 6000.0; break;
            case SoundCharacter::openHihat: bits = 4; clockHz = 7200.0; break;
            case SoundCharacter::rimClick: bits = 6; clockHz = 11000.0; break;
            case SoundCharacter::ride: bits = 5; clockHz = 7000.0; break;
            case SoundCharacter::membrane:
            case SoundCharacter::conga: bits = 6; clockHz = 10000.0; break;
            case SoundCharacter::brushes: bits = 4; clockHz = 6500.0; break;
            default: break;
        }
        if (state.chipHoldSamplesLeft == 0) {
            // Drive before quantisation makes small drum transients reach a
            // code step. Retain the resulting level: dividing it back down
            // made this family nearly 10 dB quieter than Rock on the same
            // two-bar MIDI reference, masking the timbral change.
            // Brush noise is intentionally soft before quantisation. Give it
            // enough headroom to survive the 4-bit code steps without turning
            // the 8-bit family into a silent stem.
            const float drive = sound == SoundCharacter::brushes ? 48.0f : 3.0f;
            const float levels = static_cast<float>((1u << (bits - 1)) - 1u);
            const float bounded = std::clamp(sample * drive, -1.0f, 1.0f);
            state.chipHeldSample = std::round(bounded * levels)
                / levels;
            state.chipHoldSamplesLeft = static_cast<std::uint32_t>(
                std::max(1.0, std::round(sampleRate_ / clockHz)));
        }
        sample = state.chipHeldSample;
        --state.chipHoldSamplesLeft;
    }
    bool stillActive = state.excitationSamplesLeft > 0
        || state.noiseSamplesLeft > 0 || state.chipHoldSamplesLeft > 0
        || state.snareWireEnvelope > 1.0e-7f;
    for (std::size_t mode = 0; mode < kModesPerVoice && !stillActive; ++mode)
        stillActive = state.amplitudes[mode] > 1.0e-7f
            || state.pendingExcitation[mode] > 1.0e-7f;
    state.active = stillActive;
    return sample;
}

} // namespace batruc
