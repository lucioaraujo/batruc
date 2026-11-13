// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "core/CompositionCore.hpp"

namespace batruc {

constexpr std::size_t kMidiDrumLaneCount = 8;
static_assert(kMidiDrumLaneCount == kMidiLaneDefaultSounds.size(),
              "MIDI lane roles and playable stems must stay aligned");
constexpr std::size_t kMaxMidiImportBytes = 16u * 1024u * 1024u;
constexpr std::size_t kMaxMidiImportTracks = 64;
constexpr std::size_t kMaxMidiImportEvents = 65'536;

enum class MidiDrumLane : std::uint8_t {
    kick,
    snare,
    closedHat,
    openHat,
    toms,
    cymbals,
    handPercussion,
    other
};

enum class MidiDrumMapping : std::uint8_t {
    generalMidi,
    rolandTd11
};

struct MidiSeedStyleProfile {
    // -1 means the editorial label has no conservative family hint.
    int soundFamilyIndex = -1;
    GrooveMode groove = GrooveMode::straight;
    bool hasGrooveHint = false;
};

// Conservative starting hints for known GMD editorial style labels. These
// select a palette/accent treatment; they do not certify genre authenticity
// or rewrite the imported event positions.
MidiSeedStyleProfile midiSeedStyleProfile(const char* editorialStyle) noexcept;

struct MidiSeedSpatialProfile {
    std::array<float, kMidiDrumLaneCount> pan{};
    std::array<float, kMidiDrumLaneCount> motion{};
};

// Seeded, role-aware stereo placement for imported percussion. Kick and snare
// stay centered; supporting lanes receive bounded positions and motion.
MidiSeedSpatialProfile midiSeedSpatialProfile(std::uint32_t seed) noexcept;

// Lowest voice-count limit that keeps every non-empty imported drum lane
// inside the renderer's active voice range. Returns zero for an empty source.
std::size_t midiVoiceCountForCells(
    const std::array<RhythmicCell, kMidiDrumLaneCount>& cells) noexcept;

enum class MidiRhythmImportError : std::uint8_t {
    none,
    emptyInput,
    fileTooLarge,
    malformedHeader,
    unsupportedFormat,
    unsupportedTiming,
    malformedTrack,
    eventLimitExceeded,
    resourceAllocationFailed,
    invalidOptions
};

struct MidiRhythmImportOptions {
    // Import the first 1, 2 or 4 bars. Other bounded values remain useful for
    // tests and future user-defined motifs.
    std::uint8_t bars = 1;
    // Zero uses the first MIDI time-signature meta event, or 4/4 when absent.
    std::uint8_t meterNumerator = 0;
    std::uint8_t meterDenominator = 0;
    // General MIDI percussion is channel 10, stored as zero-based channel 9.
    std::uint8_t drumChannel = 9;
    // GMD recordings use Roland TD-11 notes 22/26 for hi-hat edges and 58
    // for a floor-tom rim; generic MIDI keeps the GM interpretation.
    MidiDrumMapping mapping = MidiDrumMapping::generalMidi;
};

struct MidiRhythmImportResult {
    std::array<RhythmicCell, kMidiDrumLaneCount> cells{};
    std::array<std::array<std::uint8_t, kMaxRhythmicCellAttacks>,
               kMidiDrumLaneCount> sourceNotes{};
    std::uint16_t ticksPerQuarter = 0;
    std::uint8_t meterNumerator = 4;
    std::uint8_t meterDenominator = 4;
    // Average tempo for the selected window; 0 means there was no tempo meta.
    double tempoBpm = 0.0;
    // True when distinct tempo values occur inside the selected MIDI window.
    // Their relative timing is encoded in the imported attack positions.
    bool containsTempoChanges = false;
    std::uint32_t noteOnEvents = 0;
    std::uint32_t importedHits = 0;
    // Repeated copies of the same note at one onset merge at highest velocity.
    std::uint32_t mergedHits = 0;
    // Distinct notes that share one onset in a lane, whether retained or over cap.
    std::uint32_t collidingHits = 0;
    // Events beyond the selected bar window are an intentional crop.
    std::uint32_t outsideWindowHits = 0;
    // Events inside the window lost to attack capacity, minimum spacing, or
    // more than four distinct notes at one onset in a lane.
    std::uint32_t inWindowDroppedHits = 0;
    // Aggregate retained for existing callers; equals the two counts above.
    std::uint32_t droppedHits = 0;
};

MidiDrumLane generalMidiDrumLane(std::uint8_t note) noexcept;

// Create a deterministic, bounded dynamics variation from immutable imported
// cells. Attack positions and GM notes remain intact; the source cells are
// never modified. Empty lanes remain enabled to silence formula-generated
// events when used as a MIDI kit.
std::array<RhythmicCell, kMidiDrumLaneCount> seededMidiRhythmCells(
    const std::array<RhythmicCell, kMidiDrumLaneCount>& source,
    std::uint32_t seed, float variation) noexcept;

// Derive a phrase variation from immutable imported cells. Kick/snare event
// calendars stay structural; dense support lanes may lose a bounded,
// seed-selected set of weaker attacks. Quiet pickups may also be added between
// existing closed-hat, tom, ride, hand-percussion and accessory attacks.
std::array<RhythmicCell, kMidiDrumLaneCount> seededMidiPhraseCells(
    const std::array<RhythmicCell, kMidiDrumLaneCount>& source,
    std::uint32_t seed, float variation, std::uint8_t phraseBars = 1) noexcept;

// Offline-only Standard MIDI File importer. It accepts format 0/1 PPQ files,
// keeps performed positions and velocity, folds GM percussion into the eight
// playable Batruc voices, and never performs file I/O or audio-thread work.
MidiRhythmImportError importMidiRhythm(const std::uint8_t* bytes,
                                       std::size_t byteCount,
                                       const MidiRhythmImportOptions& options,
                                       MidiRhythmImportResult& result) noexcept;

} // namespace batruc
