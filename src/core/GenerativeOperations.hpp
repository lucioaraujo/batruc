// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#pragma once

#include "core/BassComposition.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace batruc {

// Each operation is typed: "inversion" has no universal musical meaning.
// Inputs remain intact, and every result can be compared with its source.
struct ChordVoicing {
    std::array<std::uint8_t, 8> midi{};
    std::size_t count = 0;
};

struct LogisticMapParameters {
    double r = 3.82;
    double initialState = 0.5;
    float depth = 0.80f;
    std::size_t burnIn = 12;
};

// Apply the deterministic logistic recurrence to bass-event velocity in
// absolute seven-cycle order. The attack grid and pitches remain unchanged.
bool applyLogisticBassDynamics(BassPerformance& performance,
                               const LogisticMapParameters& parameters,
                               double* finalState = nullptr) noexcept;

// Raise the lowest note above the current top once per inversion.
bool invertChordVoicing(const ChordVoicing& source,
                        std::size_t inversionCount,
                        ChordVoicing& result) noexcept;

// Mirror written and calculated bass pitches around a MIDI axis. Reject a
// result outside the instrument's declared 24..60 MIDI range.
bool invertBassPitchContour(const BassScore& source, int axisMidi,
                            BassScore& result) noexcept;

// Reflect complete note spans within the four-bar section. This is temporal
// retrograde; it is distinct from swapping voices or complementing a rhythm.
bool retrogradeBassScore(const BassScore& source, BassScore& result) noexcept;

// Reverse each event's time span within its own section/cycle while retaining
// the formal-cycle tag. Events remain sorted by local time for the renderer.
bool retrogradeBassPerformance(const BassPerformance& source,
                              BassPerformance& result) noexcept;

// Mirror performed bass pitches around an explicit MIDI axis. Rejects an
// inversion that would leave the bass renderer's declared 24..60 range.
bool invertBassPerformancePitchContour(const BassPerformance& source,
                                       int axisMidi,
                                       BassPerformance& result) noexcept;

// Keep each target cycle's attack grid, but take pitch, intensity and duration
// from the nearest event phase in another formal cycle, bar by bar.
bool recombineBassPerformanceBars(const BassPerformance& source,
                                 std::size_t donorCycleOffset,
                                 BassPerformance& result) noexcept;

// Choose each complete bar from either parent score. Sustains are clipped at
// bar boundaries so a rejected source bar cannot sound in the selected bar.
bool recombineBassBars(const BassScore& first, const BassScore& second,
                       const std::array<bool, kBassBarsPerSection>& useSecond,
                       BassScore& result) noexcept;

// Rebuild from an edited recipe. Commit only if the calculation succeeds;
// the previous score remains available for undo or comparison on failure.
bool recalculateBassScore(const BassRecipe& editedRecipe,
                          BassScore& currentScore) noexcept;

// The inverse of a phase rotation is its negative modulo one turn. Other
// algorithms require their own domain-specific inverse, when one exists.
bool inversePhaseRotation(double phaseTurns, double& inverseTurns) noexcept;

} // namespace batruc
