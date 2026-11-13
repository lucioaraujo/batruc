// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/MidiRhythmImport.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iterator>
#include <limits>
#include <memory>
#include <new>
#include <string_view>

namespace batruc {
namespace {

struct Cursor {
    const std::uint8_t* bytes = nullptr;
    std::size_t size = 0;
    std::size_t offset = 0;

    bool readByte(std::uint8_t& value) noexcept {
        if (offset >= size)
            return false;
        value = bytes[offset++];
        return true;
    }

    bool skip(const std::size_t count) noexcept {
        if (count > size - offset)
            return false;
        offset += count;
        return true;
    }

    bool readU16(std::uint16_t& value) noexcept {
        if (size - offset < 2)
            return false;
        value = static_cast<std::uint16_t>(
            (static_cast<std::uint16_t>(bytes[offset]) << 8)
            | static_cast<std::uint16_t>(bytes[offset + 1]));
        offset += 2;
        return true;
    }

    bool readU32(std::uint32_t& value) noexcept {
        if (size - offset < 4)
            return false;
        value = (static_cast<std::uint32_t>(bytes[offset]) << 24)
            | (static_cast<std::uint32_t>(bytes[offset + 1]) << 16)
            | (static_cast<std::uint32_t>(bytes[offset + 2]) << 8)
            | static_cast<std::uint32_t>(bytes[offset + 3]);
        offset += 4;
        return true;
    }

    bool readVlq(std::uint32_t& value) noexcept {
        value = 0;
        for (int index = 0; index < 4; ++index) {
            std::uint8_t byte = 0;
            if (!readByte(byte) || value > 0x0FFFFFFFu >> 7)
                return false;
            value = (value << 7) | static_cast<std::uint32_t>(byte & 0x7Fu);
            if ((byte & 0x80u) == 0)
                return true;
        }
        return false;
    }
};

bool chunkIs(const Cursor& cursor, const char (&id)[5]) noexcept {
    return cursor.size - cursor.offset >= 4
        && std::memcmp(cursor.bytes + cursor.offset, id, 4) == 0;
}

std::uint8_t denominatorFromPower(const std::uint8_t power) noexcept {
    return power <= 6 ? static_cast<std::uint8_t>(1u << power) : 0;
}

std::size_t laneIndex(const MidiDrumLane lane) noexcept {
    return static_cast<std::size_t>(lane);
}

struct HatPedalChange {
    std::uint64_t ticks = 0;
    std::uint8_t value = kNoMidiControllerValue;
};

struct TempoChange {
    std::uint64_t ticks = 0;
    std::uint32_t microsecondsPerQuarter = 500000;
    double secondsAtChange = 0.0;
};

double secondsAtMidiTick(const TempoChange* changes, const std::size_t count,
                         const std::uint16_t ticksPerQuarter,
                         const std::uint64_t ticks) noexcept {
    const auto after = std::upper_bound(changes, changes + count, ticks,
        [](const std::uint64_t time, const TempoChange& change) {
            return time < change.ticks;
        });
    const auto base = after == changes ? nullptr : std::prev(after);
    const auto baseTicks = base == nullptr ? std::uint64_t{0} : base->ticks;
    const auto microseconds = base == nullptr
        ? 500000u : base->microsecondsPerQuarter;
    const double baseSeconds = base == nullptr ? 0.0 : base->secondsAtChange;
    return baseSeconds + static_cast<double>(ticks - baseTicks)
        * static_cast<double>(microseconds)
        / (static_cast<double>(ticksPerQuarter) * 1000000.0);
}

std::uint8_t hatPedalAt(const HatPedalChange* changes,
                        const std::size_t count,
                        const std::uint64_t ticks) noexcept {
    const auto after = std::upper_bound(changes, changes + count, ticks,
        [](const std::uint64_t time, const HatPedalChange& change) {
            return time < change.ticks;
        });
    return after == changes ? kNoMidiControllerValue
                            : std::prev(after)->value;
}

void mergeOrInsertHit(MidiRhythmImportResult& result,
                      const MidiDrumLane lane, const std::uint8_t note,
                      const double position, const float strength,
                      const std::uint8_t hatPedal) noexcept {
    auto& cell = result.cells[laneIndex(lane)];
    auto& notes = result.sourceNotes[laneIndex(lane)];
    constexpr double minimumGap = 1.0 / 1024.0;
    for (std::size_t attack = 0; attack < cell.count; ++attack) {
        if (std::abs(cell.positions[attack] - position) < minimumGap) {
            if (cell.midiNotes[attack] == note) {
                ++result.mergedHits;
                if (strength > cell.strengths[attack]) {
                    cell.strengths[attack] = strength;
                    cell.midiHatPedal[attack] = hatPedal;
                }
                return;
            }
            for (std::size_t collision = 0;
                 collision < cell.midiCollisionCount[attack]; ++collision) {
                if (cell.midiCollisionNotes[attack][collision] != note)
                    continue;
                ++result.mergedHits;
                if (strength > cell.midiCollisionStrengths[attack][collision]) {
                    cell.midiCollisionStrengths[attack][collision] = strength;
                    cell.midiCollisionHatPedal[attack][collision] = hatPedal;
                }
                return;
            }
            ++result.collidingHits;
            if (cell.midiCollisionCount[attack]
                < kMaxMidiCollisionNotesPerAttack) {
                const auto collision = cell.midiCollisionCount[attack]++;
                cell.midiCollisionNotes[attack][collision] = note;
                cell.midiCollisionStrengths[attack][collision] = strength;
                cell.midiCollisionHatPedal[attack][collision] = hatPedal;
                ++result.importedHits;
            } else {
                ++result.inWindowDroppedHits;
                ++result.droppedHits;
            }
            return;
        }
    }

    std::size_t inserted = 0;
    if (!insertRhythmicCellAttack(cell, position, strength, &inserted)) {
        ++result.inWindowDroppedHits;
        ++result.droppedHits;
        return;
    }
    for (std::size_t move = cell.count - 1; move > inserted; --move)
        notes[move] = notes[move - 1];
    cell.midiNotes[inserted] = note;
    cell.midiHatPedal[inserted] = hatPedal;
    notes[inserted] = note;
    ++result.importedHits;
}

MidiRhythmImportError parseTrack(
        Cursor track, const MidiRhythmImportOptions& options,
        const std::uint16_t ticksPerQuarter,
        MidiRhythmImportResult& result, std::uint32_t& parsedEvents,
        std::uint64_t& firstMeterTicks,
        HatPedalChange* collectedPedalChanges,
        std::size_t& collectedPedalCount,
        const HatPedalChange* allPedalChanges,
        const std::size_t allPedalCount,
        TempoChange* tempoChanges,
        std::size_t& tempoChangeCount,
        const bool collectTempoChanges,
        const double tempoWindowSeconds,
        const bool controllersOnly,
        std::uint64_t& firstDrumTicks) noexcept {
    std::uint64_t absoluteTicks = 0;
    std::uint8_t runningStatus = 0;

    while (track.offset < track.size) {
        std::uint32_t delta = 0;
        if (!track.readVlq(delta)
            || absoluteTicks > std::numeric_limits<std::uint64_t>::max() - delta)
            return MidiRhythmImportError::malformedTrack;
        absoluteTicks += delta;
        if (++parsedEvents > kMaxMidiImportEvents)
            return MidiRhythmImportError::eventLimitExceeded;

        std::uint8_t first = 0;
        if (!track.readByte(first))
            return MidiRhythmImportError::malformedTrack;

        if (first == 0xFFu) {
            std::uint8_t type = 0;
            std::uint32_t length = 0;
            if (!track.readByte(type) || !track.readVlq(length)
                || length > track.size - track.offset)
                return MidiRhythmImportError::malformedTrack;
            if (type == 0x58u && length >= 2
                && options.meterNumerator == 0
                && absoluteTicks < firstMeterTicks) {
                const auto numerator = track.bytes[track.offset];
                const auto denominator = denominatorFromPower(
                    track.bytes[track.offset + 1]);
                if (numerator > 0 && denominator > 0) {
                    firstMeterTicks = absoluteTicks;
                    result.meterNumerator = numerator;
                    result.meterDenominator = denominator;
                }
            }
            if (type == 0x51u && length == 3) {
                const auto microseconds =
                    (static_cast<std::uint32_t>(track.bytes[track.offset]) << 16u)
                    | (static_cast<std::uint32_t>(track.bytes[track.offset + 1]) << 8u)
                    | static_cast<std::uint32_t>(track.bytes[track.offset + 2]);
                if (microseconds > 0u) {
                    const double bpm = 60000000.0 / microseconds;
                    if (collectTempoChanges && bpm >= 20.0 && bpm <= 300.0
                        && tempoChangeCount < kMaxMidiImportEvents)
                        tempoChanges[tempoChangeCount++] = {
                            absoluteTicks, microseconds, 0.0};
                }
            }
            if (!track.skip(length))
                return MidiRhythmImportError::malformedTrack;
            runningStatus = 0;
            if (type == 0x2Fu)
                return MidiRhythmImportError::none;
            continue;
        }

        if (first == 0xF0u || first == 0xF7u) {
            std::uint32_t length = 0;
            if (!track.readVlq(length) || !track.skip(length))
                return MidiRhythmImportError::malformedTrack;
            runningStatus = 0;
            continue;
        }

        std::uint8_t status = first;
        std::uint8_t data1 = 0;
        if ((first & 0x80u) == 0) {
            if (runningStatus < 0x80u || runningStatus >= 0xF0u)
                return MidiRhythmImportError::malformedTrack;
            status = runningStatus;
            data1 = first;
        } else {
            if (first >= 0xF0u)
                return MidiRhythmImportError::malformedTrack;
            runningStatus = first;
            if (!track.readByte(data1) || (data1 & 0x80u) != 0)
                return MidiRhythmImportError::malformedTrack;
        }

        const auto message = static_cast<std::uint8_t>(status & 0xF0u);
        const auto channel = static_cast<std::uint8_t>(status & 0x0Fu);
        if (message == 0xC0u || message == 0xD0u)
            continue;

        std::uint8_t data2 = 0;
        if (!track.readByte(data2) || (data2 & 0x80u) != 0)
            return MidiRhythmImportError::malformedTrack;
        if (message == 0xB0u && channel == options.drumChannel
            && data1 == 4u) {
            if (collectedPedalChanges != nullptr) {
                if (collectedPedalCount >= kMaxMidiImportEvents)
                    return MidiRhythmImportError::eventLimitExceeded;
                collectedPedalChanges[collectedPedalCount++] = {
                    absoluteTicks, data2};
            }
            continue;
        }
        if (message == 0x90u && data2 > 0
            && channel == options.drumChannel)
            firstDrumTicks = std::min(firstDrumTicks, absoluteTicks);
        if (controllersOnly || channel != options.drumChannel)
            continue;
        if (message != 0x90u || data2 == 0)
            continue;

        ++result.noteOnEvents;
        const double quarterNotesPerBar = static_cast<double>(
            result.meterNumerator) * 4.0 / result.meterDenominator;
        const double windowTicks = static_cast<double>(ticksPerQuarter)
            * quarterNotesPerBar * options.bars;
        if (static_cast<double>(absoluteTicks) >= windowTicks) {
            ++result.outsideWindowHits;
            ++result.droppedHits;
            continue;
        }
        const double position = std::clamp(
            secondsAtMidiTick(tempoChanges, tempoChangeCount, ticksPerQuarter,
                              absoluteTicks) / tempoWindowSeconds, 0.0,
            std::nextafter(1.0, 0.0));
        const float strength = static_cast<float>(data2) / 127.0f;
        auto lane = generalMidiDrumLane(data1);
        if (options.mapping == MidiDrumMapping::rolandTd11) {
            if (data1 == 22u)
                lane = MidiDrumLane::closedHat;
            else if (data1 == 26u)
                lane = MidiDrumLane::openHat;
            else if (data1 == 58u)
                lane = MidiDrumLane::toms;
        }
        const auto hatPedal = allPedalChanges != nullptr
            ? hatPedalAt(allPedalChanges, allPedalCount, absoluteTicks)
            : kNoMidiControllerValue;
        mergeOrInsertHit(result, lane, data1, position, strength, hatPedal);
    }
    return MidiRhythmImportError::none;
}

} // namespace

MidiDrumLane generalMidiDrumLane(const std::uint8_t note) noexcept {
    switch (note) {
        case 35: case 36:
            return MidiDrumLane::kick;
        case 37: case 38: case 39: case 40:
            return MidiDrumLane::snare;
        case 42: case 44:
            return MidiDrumLane::closedHat;
        case 46:
            return MidiDrumLane::openHat;
        case 41: case 43: case 45: case 47: case 48: case 50:
            return MidiDrumLane::toms;
        case 49: case 51: case 52: case 53: case 55: case 57: case 59:
            return MidiDrumLane::cymbals;
        case 60: case 61: case 62: case 63: case 64: case 65: case 66:
            return MidiDrumLane::handPercussion;
        default:
            // V7 holds hand drums (bongos, congas, timbales). V8 holds
            // tambourine/cowbell/vibraslap (54/56/58), agogo, shakers,
            // whistles, guiro, claves and wood blocks (67–77), as well as
            // unmapped notes. This keeps simultaneous drum and accessory
            // attacks on separate voices. TD-11 note 58 is handled earlier
            // by the explicit source mapping and remains in the tom lane.
            return MidiDrumLane::other;
    }
}

MidiSeedStyleProfile midiSeedStyleProfile(const char* editorialStyle) noexcept {
    if (editorialStyle == nullptr)
        return {};
    const std::string_view style(editorialStyle);
    if (style == "latin/brazilian-samba" || style == "latin/samba")
        return {12, GrooveMode::offbeat, true};
    if (style == "latin/brazilian-sambareggae")
        return {12, GrooveMode::offbeat, true};
    if (style == "latin/brazilian-baiao")
        return {3, GrooveMode::additive, true};
    if (style.size() >= 5 && style.substr(0, 5) == "jazz/")
        return {1, GrooveMode::ternary, true};
    if (style == "jazz")
        return {1, GrooveMode::ternary, true};
    if (style.size() >= 5 && style.substr(0, 5) == "rock/")
        return {0, GrooveMode::straight, true};
    if (style == "rock" || style == "punk")
        return {0, GrooveMode::straight, true};
    if (style.size() >= 5 && style.substr(0, 5) == "funk/")
        return {14, GrooveMode::offbeat, true};
    if (style == "funk")
        return {14, GrooveMode::offbeat, true};
    if (style.size() >= 6 && style.substr(0, 6) == "latin/")
        return {3, GrooveMode::additive, true};
    return {};
}

MidiSeedSpatialProfile midiSeedSpatialProfile(const std::uint32_t seed) noexcept {
    const auto unit = [seed](const std::uint32_t tag) noexcept {
        auto state = seed ^ tag;
        return static_cast<float>(nextSeedWord(state))
            / static_cast<float>(std::numeric_limits<std::uint32_t>::max());
    };
    const auto between = [&unit](const std::uint32_t tag, const float low,
                                 const float high) noexcept {
        return low + (high - low) * unit(tag);
    };
    const float hatSide = unit(0x50414e53u) < 0.5f ? -1.0f : 1.0f;
    const float handSide = unit(0x48414e44u) < 0.5f ? -1.0f : 1.0f;
    MidiSeedSpatialProfile profile;
    // Keep the two structural anchors centered. The stereo image comes from
    // support roles and remains balanced across either hat-side orientation.
    profile.pan[0] = 0.0f;
    profile.pan[1] = 0.0f;
    profile.pan[2] = hatSide * between(0x4833434cu, 0.10f, 0.24f);
    profile.pan[3] = hatSide * between(0x48344f50u, 0.18f, 0.34f);
    profile.pan[4] = between(0x544f4d53u, -0.16f, 0.16f);
    profile.pan[5] = -hatSide * between(0x43594d42u, 0.14f, 0.30f);
    profile.pan[6] = handSide * between(0x48414e50u, 0.16f, 0.34f);
    profile.pan[7] = -handSide * between(0x41434350u, 0.18f, 0.36f);
    profile.motion[0] = 0.0f;
    profile.motion[1] = 0.0f;
    profile.motion[2] = between(0x48334d56u, 0.03f, 0.09f);
    profile.motion[3] = between(0x48344d56u, 0.04f, 0.12f);
    profile.motion[4] = between(0x544f4d56u, 0.12f, 0.30f);
    profile.motion[5] = between(0x43594d56u, 0.04f, 0.12f);
    profile.motion[6] = between(0x48414d56u, 0.10f, 0.26f);
    profile.motion[7] = between(0x41434d56u, 0.08f, 0.22f);
    return profile;
}

std::size_t midiVoiceCountForCells(
        const std::array<RhythmicCell, kMidiDrumLaneCount>& cells) noexcept {
    std::size_t voiceCount = 0;
    for (std::size_t lane = 0; lane < cells.size(); ++lane)
        if (cells[lane].count > 0)
            voiceCount = lane + 1;
    return voiceCount;
}

std::array<RhythmicCell, kMidiDrumLaneCount> seededMidiRhythmCells(
        const std::array<RhythmicCell, kMidiDrumLaneCount>& source,
        const std::uint32_t seed, const float variation) noexcept {
    auto result = source;
    const auto amount = std::isfinite(variation)
        ? std::clamp(variation, 0.0f, 1.0f) : 0.0f;
    constexpr std::array<float, kMidiDrumLaneCount> roleDynamics = {
        0.20f, 0.16f, 0.70f, 0.52f, 0.58f, 0.40f, 0.66f, 0.55f};
    for (std::size_t lane = 0; lane < result.size(); ++lane) {
        auto& cell = result[lane];
        if (!validRhythmicCell(source[lane])) {
            cell = {};
            continue;
        }
        cell.enabled = true;
        const std::uint32_t laneSeed = (seed ^ (0x9e3779b9u
            * static_cast<std::uint32_t>(lane + 1))) * 0x85ebca6bu;
        for (std::size_t attack = 0; attack < source[lane].count; ++attack) {
            std::uint32_t hitSeed = laneSeed
                ^ (0x27d4eb2du * static_cast<std::uint32_t>(attack + 1));
            hitSeed ^= hitSeed >> 15u;
            hitSeed *= 0x2c1b3c6du;
            hitSeed ^= hitSeed >> 12u;
            const float bipolarAccent = static_cast<float>(
                (hitSeed >> 16u) & 0xffffu) / 32767.5f - 1.0f;
            // Make seed dynamics audible at moderate recipe depths, while
            // keeping kick and snare tighter than subdivisions and support.
            const float variationDepth = amount * (0.22f + 0.30f * amount);
            const float delta = bipolarAccent * variationDepth
                * roleDynamics[lane];
            cell.strengths[attack] = std::clamp(
                source[lane].strengths[attack] + delta, 0.01f, 1.0f);

            // Simultaneous GM notes are independent instruments, so give
            // each collision layer its own deterministic accent instead of
            // leaving every secondary note at its imported velocity.
            for (std::size_t collision = 0;
                 collision < source[lane].midiCollisionCount[attack];
                 ++collision) {
                std::uint32_t collisionSeed = hitSeed
                    ^ (0x165667b1u
                       * static_cast<std::uint32_t>(collision + 1));
                collisionSeed ^= collisionSeed >> 15u;
                collisionSeed *= 0x2c1b3c6du;
                collisionSeed ^= collisionSeed >> 12u;
                const float collisionAccent = static_cast<float>(
                    (collisionSeed >> 16u) & 0xffffu) / 32767.5f - 1.0f;
                const float collisionDelta = collisionAccent * variationDepth
                    * roleDynamics[lane];
                cell.midiCollisionStrengths[attack][collision] = std::clamp(
                    source[lane].midiCollisionStrengths[attack][collision]
                        + collisionDelta,
                    0.01f, 1.0f);
            }
        }
    }
    return result;
}

std::array<RhythmicCell, kMidiDrumLaneCount> seededMidiPhraseCells(
        const std::array<RhythmicCell, kMidiDrumLaneCount>& source,
        const std::uint32_t seed, const float variation,
        const std::uint8_t phraseBars) noexcept {
    auto result = seededMidiRhythmCells(source, seed, variation);
    const float amount = std::isfinite(variation)
        ? std::clamp(variation, 0.0f, 1.0f) : 0.0f;
    if (amount <= 0.0f)
        return result;

    // Thin only support material. Kick/snare keep their complete source
    // calendars; support attacks are removed as whole onset groups so GM
    // collisions and hi-hat pedal data cannot become detached from a hit.
    constexpr std::array<float, kMidiDrumLaneCount> densityRoom = {
        0.0f, 0.0f, 0.18f, 0.10f, 0.12f, 0.14f, 0.20f, 0.24f};
    const auto boundedBars = std::clamp<std::uint8_t>(phraseBars, 1u, 16u);
    for (std::size_t lane = 2; lane < source.size(); ++lane) {
        const auto& original = source[lane];
        auto& varied = result[lane];
        const float maxDrop = densityRoom[lane] * amount;
        // Sparse cymbal/tom gestures are identity-bearing events; seed density
        // variation is reserved for a lane with enough material to thin.
        if (maxDrop <= 0.0f || original.count < 8
            || !validRhythmicCell(original))
            continue;

        std::array<std::size_t, kMaxRhythmicCellAttacks> eligible{};
        std::array<float, kMaxRhythmicCellAttacks> rank{};
        std::array<bool, kMaxRhythmicCellAttacks> remove{};
        std::size_t eligibleCount = 0;
        std::uint32_t state = seed ^ (0x9e3779b9u
            * static_cast<std::uint32_t>(lane + 1)) ^ 0x44454e53u;
        for (std::size_t attack = 0; attack < original.count; ++attack) {
            const double barPosition = original.positions[attack]
                * static_cast<double>(boundedBars);
            const double barFraction = barPosition - std::floor(barPosition);
            const bool barDownbeat = barFraction < 1.0e-7
                || barFraction > 1.0 - 1.0e-7;
            if (barDownbeat || original.strengths[attack] >= 0.85f)
                continue;
            eligible[eligibleCount] = attack;
            const auto word = nextSeedWord(state);
            const float randomRank = static_cast<float>(word & 0xffffu)
                / 65535.0f;
            // Weak support hits are slightly more likely to leave space.
            rank[eligibleCount] = randomRank
                + 0.18f * std::clamp(original.strengths[attack], 0.0f, 1.0f);
            ++eligibleCount;
        }
        if (eligibleCount == 0)
            continue;

        std::size_t dropCount = static_cast<std::size_t>(std::floor(
            static_cast<double>(eligibleCount) * maxDrop + 0.5));
        const auto minKeep = std::max<std::size_t>(1u,
            static_cast<std::size_t>(std::ceil(original.count
                * (1.0 - static_cast<double>(maxDrop)))));
        dropCount = std::min(dropCount,
            original.count > minKeep ? original.count - minKeep : 0u);
        for (std::size_t drop = 0; drop < dropCount; ++drop) {
            std::size_t best = eligibleCount;
            for (std::size_t candidate = 0; candidate < eligibleCount;
                 ++candidate) {
                if (remove[candidate])
                    continue;
                if (best == eligibleCount || rank[candidate] < rank[best])
                    best = candidate;
            }
            if (best == eligibleCount)
                break;
            remove[best] = true;
        }
        if (dropCount == 0)
            continue;

        RhythmicCell compact;
        compact.enabled = true;
        std::size_t eligibleIndex = 0;
        for (std::size_t attack = 0; attack < original.count; ++attack) {
            bool omit = false;
            if (eligibleIndex < eligibleCount
                && eligible[eligibleIndex] == attack) {
                omit = remove[eligibleIndex];
                ++eligibleIndex;
            }
            if (omit)
                continue;
            const auto destination = compact.count++;
            compact.positions[destination] = original.positions[attack];
            compact.strengths[destination] = varied.strengths[attack];
            compact.midiNotes[destination] = original.midiNotes[attack];
            compact.midiHatPedal[destination] = original.midiHatPedal[attack];
            compact.midiCollisionCount[destination]
                = original.midiCollisionCount[attack];
            compact.midiCollisionNotes[destination]
                = original.midiCollisionNotes[attack];
            compact.midiCollisionStrengths[destination]
                = varied.midiCollisionStrengths[attack];
            compact.midiCollisionHatPedal[destination]
                = original.midiCollisionHatPedal[attack];
        }
        varied = compact;
    }

    // Keep structural kick and snare anchors. Derive subdued gestures only
    // from support voices and articulations already present in the MIDI.
    constexpr std::array<std::size_t, 5> supportLanes = {2u, 4u, 5u, 6u, 7u};
    const double finalBarStart = 1.0 - 1.0 / boundedBars;
    struct PickupCandidate {
        double position = 0.0;
        std::size_t sourceAttack = 0;
    };
    constexpr double minimumGap = 1.0 / 1024.0;
    for (const auto lane : supportLanes) {
        const auto& original = source[lane];
        auto& varied = result[lane];
        if (!original.enabled || original.count < 2
            || original.count >= kMaxRhythmicCellAttacks
            || !validRhythmicCell(original))
            continue;

        std::array<PickupCandidate, kMaxRhythmicCellAttacks> candidates{};
        std::size_t candidateCount = 0;
        for (std::size_t attack = 0; attack + 1 < original.count; ++attack) {
            const auto note = original.midiNotes[attack];
            if (note == kNoMidiNote || note > 127u)
                continue;
            if (lane == 5u) {
                const auto nextNote = original.midiNotes[attack + 1];
                const bool ride = note == 51u || note == 59u;
                const bool nextRide = nextNote == 51u || nextNote == 59u;
                if (!ride || !nextRide)
                    continue; // Never turn a crash or bell into a ride pulse.
            }
            const double left = original.positions[attack];
            const double right = original.positions[attack + 1];
            if (right - left < minimumGap * 4.0)
                continue;
            const double position = left + (right - left) * 0.5;
            // Toms are used for a short closing fill, not as a continuous
            // subdivision layer throughout the imported phrase.
            if (lane == 4u && position < finalBarStart)
                continue;
            candidates[candidateCount++] = {position, attack};
        }
        if (candidateCount == 0)
            continue;

        std::uint32_t state = seed ^ (0x9e3779b9u
            * static_cast<std::uint32_t>(lane + 1)) ^ 0x5049434bu;
        const auto start = static_cast<std::size_t>(nextSeedWord(state))
            % candidateCount;
        const auto laneMaximum = lane == 4u || lane == 5u ? 2u : 4u;
        const auto maxAdditions = std::min<std::size_t>(laneMaximum,
            std::max<std::size_t>(1u, static_cast<std::size_t>(std::ceil(
                static_cast<double>(candidateCount) * amount / 8.0))));
        const float chance = (lane == 4u ? 0.10f : lane == 5u ? 0.14f : 0.18f)
            + (lane == 4u ? 0.34f : lane == 5u ? 0.40f : 0.62f) * amount;
        std::size_t added = 0;
        for (std::size_t offset = 0; offset < candidateCount
                && added < maxAdditions; ++offset) {
            const auto candidateIndex = (start + offset) % candidateCount;
            const auto& candidate = candidates[candidateIndex];
            const auto draw = static_cast<float>(nextSeedWord(state)
                & 0xffffu) / 65535.0f;
            if (draw >= chance && (added != 0 || offset + 1 < candidateCount))
                continue;

            const auto leftAttack = candidate.sourceAttack;
            const float sourceStrength = std::min(
                original.strengths[leftAttack],
                original.strengths[leftAttack + 1]);
            const float laneStrength = lane == 4u ? 0.16f + 0.08f * amount
                : lane == 5u ? 0.20f + 0.06f * amount
                             : 0.22f + 0.08f * amount;
            const float pickupStrength = std::clamp(
                sourceStrength * laneStrength, 0.035f, 0.20f);
            std::size_t inserted = 0;
            if (!insertRhythmicCellAttack(varied, candidate.position,
                    pickupStrength, &inserted))
                continue;
            varied.midiNotes[inserted] = original.midiNotes[leftAttack];
            varied.midiHatPedal[inserted] = original.midiHatPedal[leftAttack];
            ++added;
        }
    }
    return result;
}

MidiRhythmImportError importMidiRhythm(
        const std::uint8_t* bytes, const std::size_t byteCount,
        const MidiRhythmImportOptions& options,
        MidiRhythmImportResult& result) noexcept {
    result = {};
    if (bytes == nullptr || byteCount == 0)
        return MidiRhythmImportError::emptyInput;
    if (byteCount > kMaxMidiImportBytes)
        return MidiRhythmImportError::fileTooLarge;
    if (options.bars == 0 || options.bars > 16 || options.drumChannel > 15
        || ((options.meterNumerator == 0) != (options.meterDenominator == 0))
        || (options.meterDenominator > 0
            && (options.meterDenominator > 64
                || (options.meterDenominator
                    & static_cast<std::uint8_t>(options.meterDenominator - 1)) != 0)))
        return MidiRhythmImportError::invalidOptions;

    Cursor file{bytes, byteCount, 0};
    if (!chunkIs(file, "MThd") || !file.skip(4))
        return MidiRhythmImportError::malformedHeader;
    std::uint32_t headerLength = 0;
    std::uint16_t format = 0;
    std::uint16_t trackCount = 0;
    std::uint16_t division = 0;
    if (!file.readU32(headerLength) || headerLength < 6
        || headerLength > file.size - file.offset
        || !file.readU16(format) || !file.readU16(trackCount)
        || !file.readU16(division))
        return MidiRhythmImportError::malformedHeader;
    if (format > 1 || trackCount == 0 || trackCount > kMaxMidiImportTracks)
        return MidiRhythmImportError::unsupportedFormat;
    if ((division & 0x8000u) != 0 || division == 0)
        return MidiRhythmImportError::unsupportedTiming;
    if (!file.skip(headerLength - 6))
        return MidiRhythmImportError::malformedHeader;

    result.ticksPerQuarter = division;
    if (options.meterNumerator > 0) {
        result.meterNumerator = options.meterNumerator;
        result.meterDenominator = options.meterDenominator;
    }
    for (auto& cell : result.cells)
        cell.enabled = true;

    std::uint32_t parsedEvents = 0;
    std::array<Cursor, kMaxMidiImportTracks> tracks{};
    // The note window is recalculated from the active meter. A conventional
    // time-signature event before the first note therefore applies to all
    // subsequent performance tracks.
    for (std::uint16_t trackIndex = 0; trackIndex < trackCount; ++trackIndex) {
        if (!chunkIs(file, "MTrk") || !file.skip(4))
            return MidiRhythmImportError::malformedTrack;
        std::uint32_t trackLength = 0;
        if (!file.readU32(trackLength) || trackLength > file.size - file.offset)
            return MidiRhythmImportError::malformedTrack;
        tracks[trackIndex] = {file.bytes + file.offset, trackLength, 0};
        if (!file.skip(trackLength))
            return MidiRhythmImportError::malformedTrack;
    }

    // CC4 can live on a separate format-1 track from the drum notes. Collect
    // the bounded pedal timeline first, then apply the latest value at each
    // note's absolute tick during the note-import pass.
    std::unique_ptr<HatPedalChange[]> pedalChanges(
        new (std::nothrow) HatPedalChange[kMaxMidiImportEvents]);
    if (!pedalChanges)
        return MidiRhythmImportError::resourceAllocationFailed;
    std::unique_ptr<TempoChange[]> tempoChanges(
        new (std::nothrow) TempoChange[kMaxMidiImportEvents]);
    if (!tempoChanges)
        return MidiRhythmImportError::resourceAllocationFailed;
    std::size_t pedalChangeCount = 0;
    std::size_t tempoChangeCount = 0;
    auto firstMeterTicks = std::numeric_limits<std::uint64_t>::max();
    auto firstDrumTicks = std::numeric_limits<std::uint64_t>::max();
    parsedEvents = 0;
    for (std::uint16_t trackIndex = 0; trackIndex < trackCount; ++trackIndex) {
        Cursor track = tracks[trackIndex];
        const auto error = parseTrack(track, options, division,
            result, parsedEvents, firstMeterTicks, pedalChanges.get(),
            pedalChangeCount, nullptr, 0, tempoChanges.get(),
            tempoChangeCount, true, 0.0, true, firstDrumTicks);
        if (error != MidiRhythmImportError::none)
            return error;
    }
    if (options.meterNumerator == 0
        && firstDrumTicks != std::numeric_limits<std::uint64_t>::max()
        && firstMeterTicks > firstDrumTicks) {
        result.meterNumerator = 4;
        result.meterDenominator = 4;
    }
    std::stable_sort(pedalChanges.get(),
        pedalChanges.get() + pedalChangeCount,
        [](const HatPedalChange& left, const HatPedalChange& right) {
            return left.ticks < right.ticks;
        });
    std::stable_sort(tempoChanges.get(),
        tempoChanges.get() + tempoChangeCount,
        [](const TempoChange& left, const TempoChange& right) {
            return left.ticks < right.ticks;
        });

    const double windowTicks = static_cast<double>(division)
        * result.meterNumerator * 4.0 / result.meterDenominator * options.bars;
    const double quarterNotesInWindow = windowTicks / division;
    const auto windowEndTicks = static_cast<std::uint64_t>(windowTicks);
    double currentTempoSeconds = 0.0;
    double minimumWindowTempo = std::numeric_limits<double>::infinity();
    double maximumWindowTempo = 0.0;
    bool hasExplicitTempoInWindow = false;
    std::uint64_t previousTempoTicks = 0;
    std::uint32_t activeMicrosecondsPerQuarter = 500000;
    for (std::size_t index = 0; index < tempoChangeCount; ++index) {
        const auto groupTicks = tempoChanges[index].ticks;
        if (groupTicks > windowEndTicks)
            break;
        currentTempoSeconds += static_cast<double>(groupTicks - previousTempoTicks)
            * static_cast<double>(activeMicrosecondsPerQuarter)
            / (static_cast<double>(division) * 1000000.0);
        const double activeBpm = 60000000.0 / activeMicrosecondsPerQuarter;
        if (groupTicks > previousTempoTicks && groupTicks < windowEndTicks) {
            minimumWindowTempo = std::min(minimumWindowTempo, activeBpm);
            maximumWindowTempo = std::max(maximumWindowTempo, activeBpm);
        }
        if (groupTicks < windowEndTicks) {
            hasExplicitTempoInWindow = true;
        }
        do {
            auto& change = tempoChanges[index];
            change.secondsAtChange = currentTempoSeconds;
            activeMicrosecondsPerQuarter = change.microsecondsPerQuarter;
            ++index;
        } while (index < tempoChangeCount
                 && tempoChanges[index].ticks == groupTicks);
        --index;
        previousTempoTicks = groupTicks;
    }
    const double finalTempoBpm = 60000000.0 / activeMicrosecondsPerQuarter;
    if (windowEndTicks > previousTempoTicks) {
        minimumWindowTempo = std::min(minimumWindowTempo, finalTempoBpm);
        maximumWindowTempo = std::max(maximumWindowTempo, finalTempoBpm);
    }
    currentTempoSeconds += static_cast<double>(windowEndTicks - previousTempoTicks)
        * static_cast<double>(activeMicrosecondsPerQuarter)
        / (static_cast<double>(division) * 1000000.0);
    if (hasExplicitTempoInWindow && currentTempoSeconds > 0.0)
        result.tempoBpm = 60.0 * quarterNotesInWindow / currentTempoSeconds;
    result.containsTempoChanges = maximumWindowTempo - minimumWindowTempo > 1.0e-6;

    parsedEvents = 0;
    for (std::uint16_t trackIndex = 0; trackIndex < trackCount; ++trackIndex) {
        Cursor track = tracks[trackIndex];
        const auto error = parseTrack(track, options, division,
            result, parsedEvents, firstMeterTicks, nullptr, pedalChangeCount,
            pedalChanges.get(), pedalChangeCount, tempoChanges.get(),
            tempoChangeCount, false, currentTempoSeconds, false,
            firstDrumTicks);
        if (error != MidiRhythmImportError::none)
            return error;
    }

    for (const auto& cell : result.cells) {
        if (!validRhythmicCell(cell))
            return MidiRhythmImportError::malformedTrack;
    }
    return MidiRhythmImportError::none;
}

} // namespace batruc
