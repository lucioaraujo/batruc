// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/MidiRhythmImport.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

namespace {

constexpr std::array<std::uint8_t, 58> kOneBarMidi = {{
    'M','T','h','d', 0,0,0,6, 0,0, 0,1, 1,0xE0,
    'M','T','r','k', 0,0,0,36,
    0x00, 0xFF,0x58,0x04, 0x04,0x02,0x18,0x08,
    0x00, 0x99,0x24,0x7F,
    0x83,0x60, 0x99,0x2A,0x40,
    0x83,0x60, 0x99,0x26,0x64,
    0x83,0x60, 0x99,0x2A,0x50,
    0x83,0x60, 0x99,0x24,0x78,
    0x00, 0xFF,0x2F,0x00
}};

bool close(const double left, const double right) {
    return std::abs(left - right) < 1.0e-8;
}

} // namespace

int main() {
    using namespace batruc;
    const auto sambaProfile = midiSeedStyleProfile("latin/brazilian-samba");
    const auto baiaoProfile = midiSeedStyleProfile("latin/brazilian-baiao");
    const auto jazzProfile = midiSeedStyleProfile("jazz/swing");
    const auto rockProfile = midiSeedStyleProfile("rock/groove8");
    const auto unknownProfile = midiSeedStyleProfile("ambient/experimental");
    const auto absentProfile = midiSeedStyleProfile(nullptr);
    if (sambaProfile.soundFamilyIndex != 12
        || sambaProfile.groove != GrooveMode::offbeat
        || !sambaProfile.hasGrooveHint
        || baiaoProfile.soundFamilyIndex != 3
        || baiaoProfile.groove != GrooveMode::additive
        || !baiaoProfile.hasGrooveHint
        || jazzProfile.soundFamilyIndex != 1
        || jazzProfile.groove != GrooveMode::ternary
        || !jazzProfile.hasGrooveHint
        || rockProfile.soundFamilyIndex != 0
        || rockProfile.groove != GrooveMode::straight
        || !rockProfile.hasGrooveHint
        || unknownProfile.soundFamilyIndex != -1
        || unknownProfile.hasGrooveHint
        || absentProfile.soundFamilyIndex != -1
        || absentProfile.hasGrooveHint) {
        std::cerr << "GMD editorial labels did not produce conservative seed hints\n";
        return 1;
    }
    const auto spatialProfile = midiSeedSpatialProfile(0x5EED2027u);
    const auto repeatedSpatialProfile = midiSeedSpatialProfile(0x5EED2027u);
    const auto alternateSpatialProfile = midiSeedSpatialProfile(0x5EED2028u);
    bool hasStereoSpread = false;
    bool hatAndCymbalOppose = spatialProfile.pan[2] * spatialProfile.pan[5] < 0.0f
        && spatialProfile.pan[3] * spatialProfile.pan[5] < 0.0f;
    bool handAndAccessoryOppose = spatialProfile.pan[6] * spatialProfile.pan[7] < 0.0f;
    for (std::size_t lane = 0; lane < kMidiDrumLaneCount; ++lane) {
        if (std::abs(spatialProfile.pan[lane]) > 0.5f
            || spatialProfile.motion[lane] < 0.0f
            || spatialProfile.motion[lane] > 0.5f) {
            std::cerr << "seeded MIDI stereo placement exceeded its bounds\n";
            return 1;
        }
        hasStereoSpread = hasStereoSpread
            || std::abs(spatialProfile.pan[lane]) > 0.01f;
    }
    if (spatialProfile.pan != repeatedSpatialProfile.pan
        || spatialProfile.motion != repeatedSpatialProfile.motion
        || spatialProfile.pan == alternateSpatialProfile.pan
        || spatialProfile.pan[0] != 0.0f || spatialProfile.pan[1] != 0.0f
        || spatialProfile.motion[0] != 0.0f
        || spatialProfile.motion[1] != 0.0f || !hasStereoSpread
        || !hatAndCymbalOppose || !handAndAccessoryOppose
        || spatialProfile.motion[2] > 0.10f
        || spatialProfile.motion[3] > 0.13f
        || spatialProfile.motion[4] < 0.10f
        || spatialProfile.motion[5] > 0.13f
        || spatialProfile.motion[6] < 0.08f
        || spatialProfile.motion[7] < 0.06f) {
        std::cerr << "seeded MIDI stereo placement lost repeatability, role anchors, or balanced role pairs\n";
        return 1;
    }

    MidiRhythmImportResult result;
    const auto error = importMidiRhythm(kOneBarMidi.data(), kOneBarMidi.size(),
                                        {}, result);
    if (error != MidiRhythmImportError::none) {
        std::cerr << "valid format-0 MIDI was rejected\n";
        return 1;
    }
    const auto& kick = result.cells[static_cast<std::size_t>(MidiDrumLane::kick)];
    const auto& hat = result.cells[static_cast<std::size_t>(MidiDrumLane::closedHat)];
    const auto& snare = result.cells[static_cast<std::size_t>(MidiDrumLane::snare)];
    if (result.ticksPerQuarter != 480 || result.meterNumerator != 4
        || result.meterDenominator != 4 || result.noteOnEvents != 5
        || result.noteOnEvents - result.outsideWindowHits != 4
        || result.importedHits != 4 || result.droppedHits != 1
        || result.outsideWindowHits != 1 || result.inWindowDroppedHits != 0
        || kick.count != 1 || hat.count != 2 || snare.count != 1
        || !close(kick.positions[0], 0.0)
        || !close(hat.positions[0], 0.25)
        || !close(snare.positions[0], 0.5)
        || !close(hat.positions[1], 0.75)
        || std::abs(snare.strengths[0] - 100.0f / 127.0f) > 1.0e-6f) {
        std::cerr << "MIDI hits were not mapped to editable cells\n";
        return 1;
    }
    if (result.sourceNotes[static_cast<std::size_t>(MidiDrumLane::snare)][0] != 38
        || generalMidiDrumLane(46) != MidiDrumLane::openHat
        || generalMidiDrumLane(64) != MidiDrumLane::handPercussion
        || generalMidiDrumLane(54) != MidiDrumLane::other
        || generalMidiDrumLane(67) != MidiDrumLane::other
        || generalMidiDrumLane(75) != MidiDrumLane::other) {
        std::cerr << "GM drum provenance or lane mapping is incorrect\n";
        return 1;
    }

    std::array<RhythmicCell, kMidiDrumLaneCount> voiceCountFixture{};
    auto& upperMidiLane = voiceCountFixture[static_cast<std::size_t>(
        MidiDrumLane::other)];
    upperMidiLane.enabled = true;
    if (!insertRhythmicCellAttack(upperMidiLane, 0.75, 0.6f)
        || midiVoiceCountForCells(voiceCountFixture) != kMidiDrumLaneCount) {
        std::cerr << "seed voice-count policy could silence an upper MIDI lane\n";
        return 1;
    }

    std::vector<std::uint8_t> denseMidi(kOneBarMidi.begin(),
                                        kOneBarMidi.begin() + 22);
    constexpr int denseHitCount = 65;
    constexpr int denseTrackBytes = denseHitCount * 4 + 4;
    denseMidi[20] = static_cast<std::uint8_t>(denseTrackBytes >> 8);
    denseMidi[21] = static_cast<std::uint8_t>(denseTrackBytes & 0xFF);
    for (int hit = 0; hit < denseHitCount; ++hit)
        denseMidi.insert(denseMidi.end(),
                         {static_cast<std::uint8_t>(hit == 0 ? 0 : 10),
                          0x99u, 36u, 100u});
    denseMidi.insert(denseMidi.end(), {0x00u, 0xFFu, 0x2Fu, 0x00u});
    MidiRhythmImportResult denseResult;
    if (importMidiRhythm(denseMidi.data(), denseMidi.size(), {}, denseResult)
            != MidiRhythmImportError::none
        || denseResult.importedHits != 64
        || denseResult.inWindowDroppedHits != 1
        || denseResult.outsideWindowHits != 0
        || denseResult.droppedHits != 1) {
        std::cerr << "in-window cell capacity loss was not reported\n";
        return 1;
    }

    // A hand drum and an accessory at the same instant use separate lanes;
    // distinct accessory notes in one lane remain same-time collision layers.
    std::vector<std::uint8_t> accessoryMidi(kOneBarMidi.begin(), kOneBarMidi.end());
    accessoryMidi.insert(accessoryMidi.begin() + 34,
                         {0x00u, 0x99u, 64u, 100u}); // conga, V7
    accessoryMidi.insert(accessoryMidi.begin() + 38,
                         {0x00u, 0x99u, 67u, 100u}); // agogo, V8
    accessoryMidi.insert(accessoryMidi.begin() + 42,
                         {0x00u, 0x99u, 69u, 90u}); // different V8 note, retained
    accessoryMidi.insert(accessoryMidi.begin() + 46,
                         {0x00u, 0x99u, 67u, 80u}); // repeated V8 note, merged
    accessoryMidi[21] = 52u; // original 36-byte track plus four note-ons
    MidiRhythmImportResult accessoryResult;
    if (importMidiRhythm(accessoryMidi.data(), accessoryMidi.size(), {},
                         accessoryResult) != MidiRhythmImportError::none
        || accessoryResult.cells[static_cast<std::size_t>(
            MidiDrumLane::handPercussion)].count != 1
        || accessoryResult.cells[static_cast<std::size_t>(
            MidiDrumLane::other)].count != 1
        || accessoryResult.sourceNotes[static_cast<std::size_t>(
            MidiDrumLane::handPercussion)][0] != 64u
        || accessoryResult.sourceNotes[static_cast<std::size_t>(
            MidiDrumLane::other)][0] != 67u
        || accessoryResult.collidingHits != 1
        || accessoryResult.mergedHits != 1
        || accessoryResult.importedHits != 7
        || accessoryResult.inWindowDroppedHits != 0) {
        std::cerr << "simultaneous MIDI notes were not retained\n";
        return 1;
    }
    const auto& accessoryCell = accessoryResult.cells[
        static_cast<std::size_t>(MidiDrumLane::other)];
    if (accessoryCell.midiCollisionCount[0] != 1
        || accessoryCell.midiCollisionNotes[0][0] != 69u
        || std::abs(accessoryCell.midiCollisionStrengths[0][0]
                    - 90.0f / 127.0f) > 1.0e-6f) {
        std::cerr << "simultaneous MIDI note velocity/articulation was lost\n";
        return 1;
    }
    auto rotatedAccessoryCell = accessoryCell;
    if (!rotateRhythmicCell(rotatedAccessoryCell, 0.125)
        || rotatedAccessoryCell.midiCollisionCount[0] != 1
        || rotatedAccessoryCell.midiCollisionNotes[0][0] != 69u
        || rotatedAccessoryCell.midiHatPedal[0]
            != accessoryCell.midiHatPedal[0]) {
        std::cerr << "Study rotation detached a collision note from its onset\n";
        return 1;
    }

    RhythmicCell euclideanCell;
    euclideanCell.enabled = true;
    if (!insertRhythmicCellAttack(euclideanCell, 0.02, 0.95f)
        || !insertRhythmicCellAttack(euclideanCell, 0.31, 0.52f)
        || !insertRhythmicCellAttack(euclideanCell, 0.91, 0.78f)) {
        std::cerr << "could not prepare Euclidean Study fixture\n";
        return 1;
    }
    euclideanCell.midiNotes[0] = 36u;
    euclideanCell.midiNotes[1] = 38u;
    euclideanCell.midiNotes[2] = 42u;
    euclideanCell.midiCollisionCount[1] = 1;
    euclideanCell.midiCollisionNotes[1][0] = 69u;
    euclideanCell.midiCollisionStrengths[1][0] = 0.63f;
    euclideanCell.midiCollisionHatPedal[1][0] = 72u;
    const auto originalEuclideanCell = euclideanCell;
    if (!euclideanRedistributeRhythmicCell(euclideanCell, 8)
        || euclideanCell.count != 3
        || std::abs(euclideanCell.positions[0]) > 1.0e-12
        || std::abs(euclideanCell.positions[1] - 0.25) > 1.0e-12
        || std::abs(euclideanCell.positions[2] - 0.625) > 1.0e-12
        || euclideanCell.midiNotes[0] != 36u
        || euclideanCell.midiNotes[1] != 38u
        || euclideanCell.midiNotes[2] != 42u
        || std::abs(euclideanCell.strengths[1] - 0.52f) > 1.0e-6f
        || euclideanCell.midiCollisionCount[1] != 1
        || euclideanCell.midiCollisionNotes[1][0] != 69u
        || euclideanCell.midiCollisionHatPedal[1][0] != 72u
        || std::abs(euclideanCell.midiCollisionStrengths[1][0] - 0.63f)
            > 1.0e-6f) {
        std::cerr << "Euclidean redistribution lost timing or MIDI attack data\n";
        return 1;
    }
    auto unchangedEuclideanCell = originalEuclideanCell;
    if (euclideanRedistributeRhythmicCell(unchangedEuclideanCell, 2)
        || unchangedEuclideanCell.positions != originalEuclideanCell.positions
        || unchangedEuclideanCell.midiNotes != originalEuclideanCell.midiNotes) {
        std::cerr << "invalid Euclidean grid changed the MIDI cell\n";
        return 1;
    }

    RhythmicCell densitySource;
    densitySource.enabled = true;
    if (!insertRhythmicCellAttack(densitySource, 0.0, 0.70f)
        || !insertRhythmicCellAttack(densitySource, 0.33, 0.70f)
        || !insertRhythmicCellAttack(densitySource, 0.66, 0.70f)) {
        std::cerr << "could not prepare density Study fixture\n";
        return 1;
    }
    densitySource.midiNotes[0] = 36u;
    densitySource.midiNotes[1] = 38u;
    densitySource.midiCollisionCount[1] = 1;
    densitySource.midiCollisionNotes[1][0] = 69u;
    densitySource.midiCollisionStrengths[1][0] = 0.63f;
    densitySource.midiCollisionHatPedal[1][0] = 72u;
    densitySource.midiNotes[2] = 42u;
    const auto originalDensityCell = densitySource;
    auto reducedDensityCell = originalDensityCell;
    auto repeatDensityCell = originalDensityCell;
    if (!reduceRhythmicCellDensity(reducedDensityCell, 0.50, 0xC0FFEEu)
        || !reduceRhythmicCellDensity(repeatDensityCell, 0.50, 0xC0FFEEu)
        || reducedDensityCell.count != 2
        || reducedDensityCell.positions[0] != 0.0
        || reducedDensityCell.positions != repeatDensityCell.positions
        || reducedDensityCell.midiNotes != repeatDensityCell.midiNotes
        || reducedDensityCell.midiNotes[0] != 36u
        || !validRhythmicCell(reducedDensityCell)) {
        std::cerr << "density reduction was not deterministic or lost downbeat\n";
        return 1;
    }
    bool changedTieChoice = false;
    bool retainedCollisionAsUnit = false;
    for (std::uint32_t seed = 1; seed < 64; ++seed) {
        auto candidate = originalDensityCell;
        if (!reduceRhythmicCellDensity(candidate, 0.50, seed)) {
            std::cerr << "valid density reduction failed for a seed\n";
            return 1;
        }
        changedTieChoice = changedTieChoice
            || candidate.midiNotes != reducedDensityCell.midiNotes;
        if (candidate.midiNotes[1] == 38u) {
            if (candidate.midiCollisionCount[1] != 1
                || candidate.midiCollisionNotes[1][0] != 69u
                || candidate.midiCollisionHatPedal[1][0] != 72u
                || std::abs(candidate.midiCollisionStrengths[1][0] - 0.63f)
                    > 1.0e-6f) {
                std::cerr << "density reduction detached simultaneous MIDI data\n";
                return 1;
            }
            retainedCollisionAsUnit = true;
        }
    }
    auto unchangedDensityCell = originalDensityCell;
    if (!changedTieChoice || !retainedCollisionAsUnit
        || reduceRhythmicCellDensity(unchangedDensityCell, 0.0, 1u)
        || unchangedDensityCell.count != originalDensityCell.count
        || unchangedDensityCell.midiNotes != originalDensityCell.midiNotes) {
        std::cerr << "density reduction seed or invalid-input contract failed\n";
        return 1;
    }

    auto crescendoCell = originalDensityCell;
    auto repeatedCrescendoCell = originalDensityCell;
    auto diminuendoCell = originalDensityCell;
    auto alternatingCell = originalDensityCell;
    auto repeatedAlternatingCell = originalDensityCell;
    if (!shapeRhythmicCellDynamics(crescendoCell,
            RhythmicDynamicsShape::crescendo, 0xC0FFEEu)
        || !shapeRhythmicCellDynamics(repeatedCrescendoCell,
            RhythmicDynamicsShape::crescendo, 0xC0FFEEu)
        || !shapeRhythmicCellDynamics(diminuendoCell,
            RhythmicDynamicsShape::diminuendo, 0xC0FFEEu)
        || !shapeRhythmicCellDynamics(alternatingCell,
            RhythmicDynamicsShape::alternating, 0xC0FFEEu)
        || !shapeRhythmicCellDynamics(repeatedAlternatingCell,
            RhythmicDynamicsShape::alternating, 0xC0FFEEu)
        || crescendoCell.strengths != repeatedCrescendoCell.strengths
        || alternatingCell.strengths != repeatedAlternatingCell.strengths
        || crescendoCell.strengths == originalDensityCell.strengths
        || crescendoCell.strengths == diminuendoCell.strengths
        || crescendoCell.positions != originalDensityCell.positions
        || crescendoCell.midiNotes != originalDensityCell.midiNotes
        || crescendoCell.midiCollisionNotes
            != originalDensityCell.midiCollisionNotes
        || crescendoCell.midiCollisionHatPedal
            != originalDensityCell.midiCollisionHatPedal
        || std::abs(crescendoCell.midiCollisionStrengths[1][0]
                    - originalDensityCell.midiCollisionStrengths[1][0]) < 1.0e-7f
        || !validRhythmicCell(crescendoCell)) {
        std::cerr << "dynamics contour changed timing or lost MIDI data\n";
        return 1;
    }
    bool foundAlternateAccentPhase = false;
    for (std::uint32_t seed = 1; seed < 64; ++seed) {
        auto candidate = originalDensityCell;
        if (!shapeRhythmicCellDynamics(candidate,
                RhythmicDynamicsShape::alternating, seed)) {
            std::cerr << "alternating dynamics contour failed for a seed\n";
            return 1;
        }
        foundAlternateAccentPhase = foundAlternateAccentPhase
            || candidate.strengths != alternatingCell.strengths;
    }
    if (!foundAlternateAccentPhase) {
        std::cerr << "alternating dynamics ignored its seeded phase\n";
        return 1;
    }

    RhythmicCell recombinationSource;
    recombinationSource.enabled = true;
    if (!insertRhythmicCellAttack(recombinationSource, 0.05, 0.95f)
        || !insertRhythmicCellAttack(recombinationSource, 0.30, 0.60f)
        || !insertRhythmicCellAttack(recombinationSource, 0.55, 0.85f)
        || !insertRhythmicCellAttack(recombinationSource, 0.80, 0.55f)) {
        std::cerr << "could not prepare rhythm recombination source\n";
        return 1;
    }
    recombinationSource.midiNotes[0] = 36u;
    recombinationSource.midiNotes[1] = 38u;
    recombinationSource.midiCollisionCount[1] = 1;
    recombinationSource.midiCollisionNotes[1][0] = 69u;
    recombinationSource.midiCollisionStrengths[1][0] = 0.63f;
    recombinationSource.midiCollisionHatPedal[1][0] = 72u;
    recombinationSource.midiNotes[2] = 40u;
    recombinationSource.midiNotes[3] = 42u;
    RhythmicCell recombinationPartner;
    recombinationPartner.enabled = true;
    if (!insertRhythmicCellAttack(recombinationPartner, 0.125, 0.9f)
        || !insertRhythmicCellAttack(recombinationPartner, 0.375, 0.8f)
        || !insertRhythmicCellAttack(recombinationPartner, 0.625, 0.7f)
        || !insertRhythmicCellAttack(recombinationPartner, 0.875, 0.6f)) {
        std::cerr << "could not prepare rhythm recombination partner\n";
        return 1;
    }
    recombinationPartner.midiNotes[0] = 49u;
    recombinationPartner.midiNotes[1] = 51u;
    recombinationPartner.midiNotes[2] = 53u;
    recombinationPartner.midiNotes[3] = 55u;
    const auto originalRecombinationSource = recombinationSource;
    auto recombinedCell = originalRecombinationSource;
    auto repeatedRecombinedCell = originalRecombinationSource;
    if (!recombineRhythmicCell(recombinedCell, recombinationPartner, 0xC0FFEEu)
        || !recombineRhythmicCell(repeatedRecombinedCell, recombinationPartner,
                                  0xC0FFEEu)
        || recombinedCell.count != originalRecombinationSource.count
        || recombinedCell.positions == originalRecombinationSource.positions
        || recombinedCell.positions != repeatedRecombinedCell.positions
        || recombinedCell.midiNotes != repeatedRecombinedCell.midiNotes
        || recombinedCell.midiCollisionNotes[1][0] != 69u
        || recombinedCell.midiCollisionHatPedal[1][0] != 72u
        || (std::abs(recombinedCell.midiCollisionStrengths[1][0] - 0.63f)
                > 1.0e-6f
            && std::abs(recombinedCell.midiCollisionStrengths[1][0]
                         - 0.63f * 0.82f) > 1.0e-6f)
        || recombinationSource.positions != originalRecombinationSource.positions
        || recombinationSource.midiNotes != originalRecombinationSource.midiNotes
        || !validRhythmicCell(recombinedCell)) {
        std::cerr << "rhythm recombination was unstable or lost selected MIDI data\n";
        return 1;
    }
    for (std::size_t attack = 0; attack < recombinedCell.count; ++attack)
        if (recombinedCell.midiNotes[attack] == 49u
            || recombinedCell.midiNotes[attack] == 51u
            || recombinedCell.midiNotes[attack] == 53u
            || recombinedCell.midiNotes[attack] == 55u) {
            std::cerr << "recombination replaced the selected stem's instrument\n";
            return 1;
        }
    bool foundAlternativeRecombination = false;
    for (std::uint32_t seed = 1; seed < 64; ++seed) {
        auto candidate = originalRecombinationSource;
        if (!recombineRhythmicCell(candidate, recombinationPartner, seed)) {
            std::cerr << "valid seeded rhythm recombination failed\n";
            return 1;
        }
        foundAlternativeRecombination = foundAlternativeRecombination
            || candidate.positions != recombinedCell.positions;
    }
    if (!foundAlternativeRecombination) {
        std::cerr << "rhythm recombination ignored the seed\n";
        return 1;
    }

    RhythmicCell interlockSource;
    interlockSource.enabled = true;
    if (!insertRhythmicCellAttack(interlockSource, 0.25, 0.90f)
        || !insertRhythmicCellAttack(interlockSource, 0.75, 0.70f)) {
        std::cerr << "could not prepare interlocking source fixture\n";
        return 1;
    }
    interlockSource.midiNotes[0] = 38u;
    interlockSource.midiCollisionCount[0] = 1;
    interlockSource.midiCollisionNotes[0][0] = 69u;
    interlockSource.midiCollisionStrengths[0][0] = 0.63f;
    interlockSource.midiCollisionHatPedal[0][0] = 72u;
    interlockSource.midiNotes[1] = 42u;
    RhythmicCell interlockPartner;
    interlockPartner.enabled = true;
    if (!insertRhythmicCellAttack(interlockPartner, 0.25, 1.0f)
        || !insertRhythmicCellAttack(interlockPartner, 0.50, 0.8f)
        || !insertRhythmicCellAttack(interlockPartner, 0.75, 1.0f)) {
        std::cerr << "could not prepare interlocking partner fixture\n";
        return 1;
    }
    const auto originalInterlockCell = interlockSource;
    auto interlockedCell = originalInterlockCell;
    auto repeatedInterlockCell = originalInterlockCell;
    if (!interlockRhythmicCell(interlockedCell, interlockPartner, 0xC0FFEEu)
        || !interlockRhythmicCell(repeatedInterlockCell, interlockPartner,
                                  0xC0FFEEu)
        || interlockedCell.count != originalInterlockCell.count
        || interlockedCell.positions == originalInterlockCell.positions
        || interlockedCell.positions != repeatedInterlockCell.positions
        || interlockedCell.midiNotes != originalInterlockCell.midiNotes
        || interlockedCell.midiNotes[0] != 38u
        || interlockedCell.midiCollisionCount[0] != 1
        || interlockedCell.midiCollisionNotes[0][0] != 69u
        || interlockedCell.midiCollisionHatPedal[0][0] != 72u
        || std::abs(interlockedCell.midiCollisionStrengths[0][0] - 0.63f)
            > 1.0e-6f
        || interlockedCell.midiNotes[1] != 42u
        || !validRhythmicCell(interlockedCell)) {
        std::cerr << "interlocking changed attack count or detached MIDI data\n";
        return 1;
    }
    for (std::size_t attack = 0; attack < interlockedCell.count; ++attack)
        for (std::size_t partner = 0; partner < interlockPartner.count; ++partner)
            if (std::abs(interlockedCell.positions[attack]
                         - interlockPartner.positions[partner]) < 1.0 / 1024.0) {
                std::cerr << "interlocking left a simultaneous onset\n";
                return 1;
            }
    RhythmicCell separatedPartner;
    separatedPartner.enabled = true;
    if (!insertRhythmicCellAttack(separatedPartner, 0.125, 0.8f)
        || !insertRhythmicCellAttack(separatedPartner, 0.375, 0.8f)
        || !insertRhythmicCellAttack(separatedPartner, 0.625, 0.8f)) {
        std::cerr << "could not prepare separated interlocking partner\n";
        return 1;
    }
    auto unchangedInterlockCell = originalInterlockCell;
    if (interlockRhythmicCell(unchangedInterlockCell, separatedPartner, 1u)
        || unchangedInterlockCell.positions != originalInterlockCell.positions
        || interlockRhythmicCell(interlockSource, RhythmicCell{}, 1u)) {
        std::cerr << "interlocking changed an unavailable or invalid partner\n";
        return 1;
    }

    auto wrappedEuclideanCell = originalEuclideanCell;
    wrappedEuclideanCell.positions[0] = 0.85;
    wrappedEuclideanCell.positions[1] = 0.90;
    wrappedEuclideanCell.positions[2] = 0.95;
    if (!euclideanRedistributeRhythmicCell(wrappedEuclideanCell, 8)
        || std::abs(wrappedEuclideanCell.positions[0] - 0.125) > 1.0e-12
        || std::abs(wrappedEuclideanCell.positions[1] - 0.5) > 1.0e-12
        || std::abs(wrappedEuclideanCell.positions[2] - 0.875) > 1.0e-12
        || wrappedEuclideanCell.midiNotes[0] != 38u
        || wrappedEuclideanCell.midiNotes[1] != 42u
        || wrappedEuclideanCell.midiNotes[2] != 36u
        || wrappedEuclideanCell.midiCollisionNotes[0][0] != 69u
        || wrappedEuclideanCell.midiCollisionNotes[2][0] != kNoMidiNote) {
        std::cerr << "Euclidean wrap detached MIDI data from circular attacks\n";
        return 1;
    }

    auto microtimedCell = originalEuclideanCell;
    auto repeatedMicrotiming = originalEuclideanCell;
    auto differentMicrotiming = originalEuclideanCell;
    if (!varyRhythmicCellMicrotiming(microtimedCell, 0.01, 0xC0FFEEu)
        || !varyRhythmicCellMicrotiming(repeatedMicrotiming, 0.01, 0xC0FFEEu)
        || !varyRhythmicCellMicrotiming(differentMicrotiming, 0.01, 0xBADC0DEu)
        || !validRhythmicCell(microtimedCell)
        || microtimedCell.positions == originalEuclideanCell.positions
        || microtimedCell.positions != repeatedMicrotiming.positions
        || microtimedCell.positions == differentMicrotiming.positions
        || microtimedCell.midiNotes != originalEuclideanCell.midiNotes
        || microtimedCell.strengths != originalEuclideanCell.strengths
        || microtimedCell.midiCollisionNotes
            != originalEuclideanCell.midiCollisionNotes
        || microtimedCell.midiCollisionStrengths
            != originalEuclideanCell.midiCollisionStrengths
        || microtimedCell.midiHatPedal != originalEuclideanCell.midiHatPedal
        || microtimedCell.midiCollisionHatPedal
            != originalEuclideanCell.midiCollisionHatPedal) {
        std::cerr << "microtiming was not deterministic or detached MIDI data\n";
        return 1;
    }
    for (std::size_t attack = 0; attack < microtimedCell.count; ++attack) {
        if (std::abs(microtimedCell.positions[attack]
                     - originalEuclideanCell.positions[attack]) > 0.0100001) {
            std::cerr << "microtiming exceeded its requested displacement\n";
            return 1;
        }
    }
    auto unchangedMicrotiming = originalEuclideanCell;
    if (varyRhythmicCellMicrotiming(unchangedMicrotiming, 0.0, 0xC0FFEEu)
        || varyRhythmicCellMicrotiming(unchangedMicrotiming, 0.1, 0xC0FFEEu)
        || unchangedMicrotiming.positions != originalEuclideanCell.positions) {
        std::cerr << "invalid microtiming changed the MIDI cell\n";
        return 1;
    }

    auto polyphonicRecipe = makeInitialRecipe();
    polyphonicRecipe.voiceCount = 1;
    polyphonicRecipe.rhythmicCells[0] = accessoryCell;
    EventPlan polyphonicPlan;
    if (buildEventPlan(polyphonicRecipe, polyphonicPlan) != PlanError::none
        || polyphonicPlan.events.size() != 2
        || polyphonicPlan.events[0].timeSeconds != polyphonicPlan.events[1].timeSeconds
        || polyphonicPlan.events[0].soundLayer != 0
        || polyphonicPlan.events[1].soundLayer != 1
        || polyphonicPlan.events[1].midiNote != 69u) {
        std::cerr << "same-onset MIDI notes were not scheduled in separate layers\n";
        return 1;
    }
    auto singleNoteRecipe = polyphonicRecipe;
    singleNoteRecipe.rhythmicCells[0].midiCollisionCount[0] = 0;
    EventPlan singleNotePlan;
    ResonatorRenderer layeredRenderer;
    ResonatorRenderer singleNoteRenderer;
    constexpr std::size_t renderFrames = 512;
    std::array<float, renderFrames> layeredLeft{};
    std::array<float, renderFrames> layeredRight{};
    std::array<float, renderFrames> singleLeft{};
    std::array<float, renderFrames> singleRight{};
    if (buildEventPlan(singleNoteRecipe, singleNotePlan) != PlanError::none
        || !layeredRenderer.configure(polyphonicRecipe, polyphonicPlan, 44100.0)
        || !singleNoteRenderer.configure(singleNoteRecipe, singleNotePlan, 44100.0)) {
        std::cerr << "simultaneous-note audio plans could not be prepared\n";
        return 1;
    }
    layeredRenderer.process(layeredLeft.data(), layeredRight.data(), renderFrames);
    singleNoteRenderer.process(singleLeft.data(), singleRight.data(), renderFrames);
    float layerContribution = 0.0f;
    for (std::size_t frame = 0; frame < renderFrames; ++frame) {
        if (!std::isfinite(layeredLeft[frame]) || !std::isfinite(layeredRight[frame])) {
            std::cerr << "simultaneous-note renderer produced non-finite audio\n";
            return 1;
        }
        layerContribution = std::max(layerContribution,
            std::abs(layeredLeft[frame] - singleLeft[frame]));
    }
    if (layerContribution < 1.0e-5f) {
        std::cerr << "collision layer did not contribute separate audio\n";
        return 1;
    }

    std::vector<std::uint8_t> overfullMidi(kOneBarMidi.begin(), kOneBarMidi.end());
    for (const auto note : {67u, 69u, 72u, 74u, 77u})
        overfullMidi.insert(overfullMidi.begin() + 34,
            {0x00u, 0x99u, static_cast<std::uint8_t>(note), 100u});
    overfullMidi[21] = 56u; // source track length plus five simultaneous notes
    MidiRhythmImportResult overfullResult;
    if (importMidiRhythm(overfullMidi.data(), overfullMidi.size(), {}, overfullResult)
            != MidiRhythmImportError::none
        || overfullResult.cells[static_cast<std::size_t>(MidiDrumLane::other)]
            .midiCollisionCount[0] != kMaxMidiCollisionNotesPerAttack
        || overfullResult.inWindowDroppedHits != 1) {
        std::cerr << "simultaneous-note capacity overflow was not reported\n";
        return 1;
    }

    std::vector<std::uint8_t> tempoMidi(kOneBarMidi.begin(), kOneBarMidi.end());
    tempoMidi.insert(tempoMidi.begin() + 30,
                     {0x00u, 0xFFu, 0x51u, 0x03u, 0x07u, 0xA1u, 0x20u});
    tempoMidi[21] = 43u; // original 36-byte track plus seven-byte tempo event
    MidiRhythmImportResult tempoResult;
    if (importMidiRhythm(tempoMidi.data(), tempoMidi.size(), {}, tempoResult)
            != MidiRhythmImportError::none
        || !close(tempoResult.tempoBpm, 120.0)) {
        std::cerr << "MIDI source tempo was not recovered\n";
        return 1;
    }

    constexpr std::array<std::uint8_t, 50> kFormatOneTempoTrackAfterDrums = {{
        'M','T','h','d', 0,0,0,6, 0,1, 0,2, 1,0xE0,
        'M','T','r','k', 0,0,0,9,
        0x00,0x99,36,100, 0x83,0x60,0xFF,0x2F,0x00,
        'M','T','r','k', 0,0,0,11,
        0x00,0xFF,0x51,0x03,0x07,0xA1,0x20,
        0x00,0xFF,0x2F,0x00
    }};
    MidiRhythmImportResult crossTrackTempoResult;
    if (importMidiRhythm(kFormatOneTempoTrackAfterDrums.data(),
                         kFormatOneTempoTrackAfterDrums.size(), {},
                         crossTrackTempoResult) != MidiRhythmImportError::none
        || !close(crossTrackTempoResult.tempoBpm, 120.0)
        || crossTrackTempoResult.containsTempoChanges) {
        std::cerr << "format-1 tempo was lost when its track followed drums\n";
        return 1;
    }
    auto lateTempoMidi = kFormatOneTempoTrackAfterDrums;
    lateTempoMidi[39] = 0x01; // Tempo at tick 1, after the tick-0 drum attack.
    lateTempoMidi[43] = 0x0Au;
    lateTempoMidi[44] = 0x2Cu;
    lateTempoMidi[45] = 0x2Bu; // 90 BPM for the remainder of the window.
    MidiRhythmImportResult lateTempoResult;
    if (importMidiRhythm(lateTempoMidi.data(), lateTempoMidi.size(), {},
                         lateTempoResult) != MidiRhythmImportError::none
        || std::abs(lateTempoResult.tempoBpm - 90.0) > 0.02
        || !lateTempoResult.containsTempoChanges) {
        std::cerr << "tempo change after the first attack was not integrated\n";
        return 1;
    }

    constexpr std::array<std::uint8_t, 62> kFormatOneVariableTempoMidi = {{
        'M','T','h','d', 0,0,0,6, 0,1, 0,2, 1,0xE0,
        'M','T','r','k', 0,0,0,13,
        0x00,0x99,36,100, 0x87,0x40,0x99,36,100, 0x00,0xFF,0x2F,0x00,
        'M','T','r','k', 0,0,0,19,
        0x00,0xFF,0x51,0x03,0x07,0xA1,0x20,
        0x83,0x60,0xFF,0x51,0x03,0x0A,0x2C,0x2B,
        0x00,0xFF,0x2F,0x00
    }};
    MidiRhythmImportResult variableTempoResult;
    const auto& variableKick = variableTempoResult.cells[
        static_cast<std::size_t>(MidiDrumLane::kick)];
    if (importMidiRhythm(kFormatOneVariableTempoMidi.data(),
                         kFormatOneVariableTempoMidi.size(), {},
                         variableTempoResult) != MidiRhythmImportError::none
        || std::abs(variableTempoResult.tempoBpm - 96.0) > 0.001
        || !variableTempoResult.containsTempoChanges
        || variableKick.count != 2
        || std::abs(variableKick.positions[1] - 7.0 / 15.0) > 1.0e-5) {
        std::cerr << "tempo map did not preserve variable-time MIDI performance\n";
        return 1;
    }

    constexpr std::array<std::uint8_t, 55> kFormatOneMeterTrackAfterDrums = {{
        'M','T','h','d', 0,0,0,6, 0,1, 0,2, 1,0xE0,
        'M','T','r','k', 0,0,0,13,
        0x00,0x99,36,100, 0x87,0x40,0x99,36,100, 0x00,0xFF,0x2F,0x00,
        'M','T','r','k', 0,0,0,12,
        0x00,0xFF,0x58,0x04, 0x03,0x02,0x18,0x08,
        0x00,0xFF,0x2F,0x00
    }};
    MidiRhythmImportResult crossTrackMeterResult;
    if (importMidiRhythm(kFormatOneMeterTrackAfterDrums.data(),
                         kFormatOneMeterTrackAfterDrums.size(), {},
                         crossTrackMeterResult) != MidiRhythmImportError::none
        || crossTrackMeterResult.meterNumerator != 3
        || crossTrackMeterResult.meterDenominator != 4
        || crossTrackMeterResult.cells[static_cast<std::size_t>(
            MidiDrumLane::kick)].count != 2
        || !close(crossTrackMeterResult.cells[static_cast<std::size_t>(
            MidiDrumLane::kick)].positions[1], 2.0 / 3.0)) {
        std::cerr << "format-1 meter was lost when its track followed drums\n";
        return 1;
    }
    MidiRhythmImportOptions overrideMeterOptions;
    overrideMeterOptions.meterNumerator = 5;
    overrideMeterOptions.meterDenominator = 4;
    MidiRhythmImportResult overrideMeterResult;
    if (importMidiRhythm(kFormatOneMeterTrackAfterDrums.data(),
                         kFormatOneMeterTrackAfterDrums.size(),
                         overrideMeterOptions, overrideMeterResult)
            != MidiRhythmImportError::none
        || overrideMeterResult.meterNumerator != 5
        || overrideMeterResult.meterDenominator != 4
        || !close(overrideMeterResult.cells[static_cast<std::size_t>(
            MidiDrumLane::kick)].positions[1], 0.4)) {
        std::cerr << "manual meter did not override the MIDI time signature\n";
        return 1;
    }
    auto lateMeterMidi = kFormatOneMeterTrackAfterDrums;
    lateMeterMidi[43] = 0x01; // The 3/4 signature begins after the tick-0 hit.
    MidiRhythmImportResult lateMeterResult;
    if (importMidiRhythm(lateMeterMidi.data(), lateMeterMidi.size(), {},
                         lateMeterResult) != MidiRhythmImportError::none
        || lateMeterResult.meterNumerator != 4
        || lateMeterResult.meterDenominator != 4
        || !close(lateMeterResult.cells[static_cast<std::size_t>(
            MidiDrumLane::kick)].positions[1], 0.5)) {
        std::cerr << "late meter change was treated as the initial time signature\n";
        return 1;
    }

    // CC4 may be authored on a separate format-1 track from the drum notes.
    // The controller value effective at each absolute tick belongs to that hit.
    constexpr std::array<std::uint8_t, 64> kCrossTrackHatPedalMidi = {{
        'M','T','h','d', 0,0,0,6, 0,1, 0,2, 1,0xE0,
        'M','T','r','k', 0,0,0,21,
        0x00,0xFF,0x58,0x04, 0x04,0x02,0x18,0x08,
        0x00,0xB9,0x04,0x00,
        0x83,0x60,0xB9,0x04,0x7F,
        0x00,0xFF,0x2F,0x00,
        'M','T','r','k', 0,0,0,13,
        0x00,0x99,0x2A,0x64,
        0x83,0x60,0x99,0x2A,0x64,
        0x00,0xFF,0x2F,0x00
    }};
    MidiRhythmImportResult crossTrackHatResult;
    if (importMidiRhythm(kCrossTrackHatPedalMidi.data(),
                         kCrossTrackHatPedalMidi.size(), {}, crossTrackHatResult)
            != MidiRhythmImportError::none) {
        std::cerr << "cross-track CC4 MIDI was rejected\n";
        return 1;
    }
    const auto& crossTrackHat = crossTrackHatResult.cells[
        static_cast<std::size_t>(MidiDrumLane::closedHat)];
    if (crossTrackHat.count != 2
        || crossTrackHat.midiHatPedal[0] != 0u
        || crossTrackHat.midiHatPedal[1] != 127u) {
        std::cerr << "CC4 values were not associated with hits across tracks\n";
        return 1;
    }
    auto openHatRecipe = makeInitialRecipe();
    openHatRecipe.voiceCount = 1;
    openHatRecipe.rhythmicCells[0] = crossTrackHat;
    openHatRecipe.rhythmicCells[0].count = 1;
    openHatRecipe.rhythmicCells[0].midiHatPedal[0] = 0u;
    EventPlan openHatPlan;
    auto unarticulatedHatRecipe = openHatRecipe;
    unarticulatedHatRecipe.rhythmicCells[0].midiHatPedal[0]
        = kNoMidiControllerValue;
    EventPlan unarticulatedHatPlan;
    ResonatorRenderer openHatRenderer;
    ResonatorRenderer unarticulatedHatRenderer;
    constexpr std::size_t hatRenderFrames = 512;
    std::array<float, hatRenderFrames> openHatLeft{};
    std::array<float, hatRenderFrames> openHatRight{};
    std::array<float, hatRenderFrames> closedHatLeft{};
    std::array<float, hatRenderFrames> closedHatRight{};
    if (buildEventPlan(openHatRecipe, openHatPlan) != PlanError::none
        || buildEventPlan(unarticulatedHatRecipe, unarticulatedHatPlan)
            != PlanError::none
        || !openHatRenderer.configure(openHatRecipe, openHatPlan, 44100.0)
        || !unarticulatedHatRenderer.configure(unarticulatedHatRecipe,
            unarticulatedHatPlan, 44100.0)) {
        std::cerr << "CC4 hat render plans could not be prepared\n";
        return 1;
    }
    openHatRenderer.setChannelSound(0, SoundCharacter::hihat);
    unarticulatedHatRenderer.setChannelSound(0, SoundCharacter::hihat);
    openHatRenderer.process(openHatLeft.data(), openHatRight.data(), hatRenderFrames);
    unarticulatedHatRenderer.process(closedHatLeft.data(), closedHatRight.data(),
                                     hatRenderFrames);
    float cc4Difference = 0.0f;
    for (std::size_t frame = 0; frame < hatRenderFrames; ++frame)
        cc4Difference = std::max(cc4Difference,
            std::abs(openHatLeft[frame] - closedHatLeft[frame]));
    if (cc4Difference < 1.0e-5f) {
        std::cerr << "CC4 pedal articulation did not alter the hat sound\n";
        return 1;
    }

    // The GMD's Roland TD-11 uses 22/26 for hi-hat edges and 58 for
    // floor-tom rim. Generic GM must retain its different interpretation.
    auto rolandMidi = kOneBarMidi;
    std::size_t noteOrdinal = 0;
    for (std::size_t byte = 0; byte + 2 < rolandMidi.size(); ++byte) {
        if (rolandMidi[byte] != 0x99u)
            continue;
        if (noteOrdinal == 1)
            rolandMidi[byte + 1] = 22u;
        else if (noteOrdinal == 2)
            rolandMidi[byte + 1] = 58u;
        else if (noteOrdinal == 3)
            rolandMidi[byte + 1] = 26u;
        ++noteOrdinal;
    }
    MidiRhythmImportOptions td11Options;
    td11Options.mapping = MidiDrumMapping::rolandTd11;
    MidiRhythmImportResult td11Result;
    MidiRhythmImportResult gmResult;
    if (noteOrdinal != 5
        || importMidiRhythm(rolandMidi.data(), rolandMidi.size(),
                            td11Options, td11Result) != MidiRhythmImportError::none
        || importMidiRhythm(rolandMidi.data(), rolandMidi.size(),
                            {}, gmResult) != MidiRhythmImportError::none
        || td11Result.cells[static_cast<std::size_t>(MidiDrumLane::closedHat)].count != 1
        || td11Result.cells[static_cast<std::size_t>(MidiDrumLane::openHat)].count != 1
        || td11Result.cells[static_cast<std::size_t>(MidiDrumLane::toms)].count != 1
        || td11Result.cells[static_cast<std::size_t>(MidiDrumLane::closedHat)]
               .midiNotes[0] != 22u
        || td11Result.cells[static_cast<std::size_t>(MidiDrumLane::openHat)]
               .midiNotes[0] != 26u
        || td11Result.cells[static_cast<std::size_t>(MidiDrumLane::toms)]
               .midiNotes[0] != 58u
        || gmResult.cells[static_cast<std::size_t>(MidiDrumLane::other)].count != 3
        || gmResult.cells[static_cast<std::size_t>(MidiDrumLane::handPercussion)]
               .count != 0) {
        std::cerr << "TD-11 and GM drum mappings were conflated\n";
        return 1;
    }

    const auto sourceCells = result.cells;
    const auto seededA = seededMidiRhythmCells(sourceCells, 0x12345678u, 1.0f);
    const auto seededAgain = seededMidiRhythmCells(sourceCells, 0x12345678u, 1.0f);
    const auto seededB = seededMidiRhythmCells(sourceCells, 0x87654321u, 1.0f);
    bool seedChangedStrength = false;
    for (std::size_t lane = 0; lane < kMidiDrumLaneCount; ++lane) {
        if (!seededA[lane].enabled || !seededAgain[lane].enabled
            || seededA[lane].count != sourceCells[lane].count
            || seededA[lane].count != seededAgain[lane].count
            || !validRhythmicCell(seededA[lane])) {
            std::cerr << "seeded MIDI cells are invalid or lost attacks\n";
            return 1;
        }
        for (std::size_t attack = 0; attack < seededA[lane].count; ++attack) {
            if (!close(seededA[lane].positions[attack],
                       seededAgain[lane].positions[attack])) {
                std::cerr << "same seed did not reproduce MIDI variation\n";
                return 1;
            }
            if (!close(seededA[lane].positions[attack],
                       sourceCells[lane].positions[attack])
                || !close(seededB[lane].positions[attack],
                          sourceCells[lane].positions[attack])
                || seededA[lane].midiNotes[attack]
                    != sourceCells[lane].midiNotes[attack]
                || seededB[lane].midiNotes[attack]
                    != sourceCells[lane].midiNotes[attack]) {
                std::cerr << "seed changed the imported rhythm or GM note\n";
                return 1;
            }
            seedChangedStrength = seedChangedStrength
                || !close(seededA[lane].strengths[attack],
                          seededB[lane].strengths[attack]);
        }
    }
    if (!seedChangedStrength || sourceCells[0].positions[0] != 0.0) {
        std::cerr << "seed did not vary MIDI dynamics or mutated its source\n";
        return 1;
    }

    std::array<RhythmicCell, kMidiDrumLaneCount> collisionSource{};
    const auto accessoryLane = static_cast<std::size_t>(MidiDrumLane::other);
    collisionSource[accessoryLane] = accessoryCell;
    const auto seededCollision = seededMidiRhythmCells(
        collisionSource, 0x12345678u, 1.0f);
    const auto repeatedCollision = seededMidiRhythmCells(
        collisionSource, 0x12345678u, 1.0f);
    const auto alternateCollision = seededMidiRhythmCells(
        collisionSource, 0x87654321u, 1.0f);
    const auto unchangedCollision = seededMidiRhythmCells(
        collisionSource, 0x12345678u, 0.0f);
    if (seededCollision[accessoryLane].midiCollisionCount[0] != 1
        || seededCollision[accessoryLane].midiCollisionNotes[0][0] != 69u
        || seededCollision[accessoryLane].midiHatPedal[0]
            != accessoryCell.midiHatPedal[0]
        || !close(seededCollision[accessoryLane].midiCollisionStrengths[0][0],
                  repeatedCollision[accessoryLane].midiCollisionStrengths[0][0])
        || close(seededCollision[accessoryLane].midiCollisionStrengths[0][0],
                 alternateCollision[accessoryLane].midiCollisionStrengths[0][0])
        || !close(unchangedCollision[accessoryLane].midiCollisionStrengths[0][0],
                 accessoryCell.midiCollisionStrengths[0][0])) {
        std::cerr << "seed did not vary collision velocity deterministically\n";
        return 1;
    }

    std::array<RhythmicCell, kMidiDrumLaneCount> densityPhraseSource{};
    const auto kickLane = static_cast<std::size_t>(MidiDrumLane::kick);
    const auto snareLane = static_cast<std::size_t>(MidiDrumLane::snare);
    const auto hatLane = static_cast<std::size_t>(MidiDrumLane::closedHat);
    for (const auto lane : {kickLane, snareLane}) {
        auto& cell = densityPhraseSource[lane];
        cell.enabled = true;
        for (const double position : {0.0, 0.25, 0.5, 0.75}) {
            std::size_t inserted = 0;
            if (!insertRhythmicCellAttack(cell, position, 0.8f, &inserted)) {
                std::cerr << "could not prepare structural drum fixture\n";
                return 1;
            }
            cell.midiNotes[inserted] = lane == kickLane ? 36u : 38u;
        }
    }
    auto& denseHat = densityPhraseSource[hatLane];
    denseHat.enabled = true;
    for (std::size_t hit = 0; hit < 32; ++hit) {
        const float strength = hit == 0 ? 1.0f : hit == 16 ? 0.90f : 0.45f;
        std::size_t inserted = 0;
        if (!insertRhythmicCellAttack(denseHat,
                static_cast<double>(hit) / 32.0, strength, &inserted)) {
            std::cerr << "could not prepare dense support-lane fixture\n";
            return 1;
        }
        denseHat.midiNotes[inserted] = 42u;
        denseHat.midiHatPedal[inserted] = 64u;
    }
    // A collision at a protected downbeat must remain attached to that onset.
    denseHat.midiCollisionCount[0] = 1;
    denseHat.midiCollisionNotes[0][0] = 46u;
    denseHat.midiCollisionStrengths[0][0] = 0.72f;
    denseHat.midiCollisionHatPedal[0][0] = 88u;
    const auto densityPhraseOriginal = densityPhraseSource;
    const auto densityPhraseA = seededMidiPhraseCells(
        densityPhraseSource, 0x13579bdfu, 1.0f, 1u);
    const auto densityPhraseAgain = seededMidiPhraseCells(
        densityPhraseSource, 0x13579bdfu, 1.0f, 1u);
    const auto densityPhraseB = seededMidiPhraseCells(
        densityPhraseSource, 0x2468ace0u, 1.0f, 1u);
    const auto densityPhraseZero = seededMidiPhraseCells(
        densityPhraseSource, 0x13579bdfu, 0.0f, 1u);
    bool seedChangedDensityPattern = false;
    bool sameSeedReproduced = densityPhraseA[hatLane].count
        == densityPhraseAgain[hatLane].count;
    for (std::size_t hit = 0; hit < densityPhraseA[hatLane].count; ++hit)
        sameSeedReproduced = sameSeedReproduced
            && close(densityPhraseA[hatLane].positions[hit],
                     densityPhraseAgain[hatLane].positions[hit]);
    for (std::size_t hit = 0; hit < densityPhraseB[hatLane].count; ++hit) {
        bool found = false;
        for (std::size_t other = 0; other < densityPhraseA[hatLane].count; ++other)
            found = found || close(densityPhraseB[hatLane].positions[hit],
                                   densityPhraseA[hatLane].positions[other]);
        seedChangedDensityPattern = seedChangedDensityPattern || !found;
    }
    bool strongHitRetained = false;
    for (std::size_t hit = 0; hit < densityPhraseA[hatLane].count; ++hit)
        strongHitRetained = strongHitRetained
            || close(densityPhraseA[hatLane].positions[hit], 0.5);
    if (!sameSeedReproduced
        || densityPhraseA[hatLane].count >= denseHat.count
        || densityPhraseA[kickLane].count != densityPhraseSource[kickLane].count
        || densityPhraseA[snareLane].count != densityPhraseSource[snareLane].count
        || densityPhraseA[hatLane].positions[0] != 0.0
        || !strongHitRetained
        || densityPhraseA[hatLane].midiCollisionCount[0] != 1
        || densityPhraseA[hatLane].midiCollisionNotes[0][0] != 46u
        || densityPhraseA[hatLane].midiHatPedal[0] != 64u
        || densityPhraseA[hatLane].midiCollisionHatPedal[0][0] != 88u
        || !seedChangedDensityPattern
        || densityPhraseZero[hatLane].count != denseHat.count
        || densityPhraseSource[hatLane].count
            != densityPhraseOriginal[hatLane].count
        || !validRhythmicCell(densityPhraseA[hatLane])) {
        std::cerr << "seeded support density violated role, determinism, or MIDI-data guarantees\n";
        return 1;
    }
    for (const auto lane : {kickLane, snareLane})
        for (std::size_t hit = 0; hit < densityPhraseSource[lane].count; ++hit)
            if (!close(densityPhraseA[lane].positions[hit],
                       densityPhraseSource[lane].positions[hit])
                || densityPhraseA[lane].midiNotes[hit]
                    != densityPhraseSource[lane].midiNotes[hit]) {
                std::cerr << "seeded support density changed a kick/snare event\n";
                return 1;
            }
    for (std::size_t hit = 0; hit < denseHat.count; ++hit)
        if (!close(denseHat.positions[hit],
                   densityPhraseOriginal[hatLane].positions[hit])
            || denseHat.midiNotes[hit]
                != densityPhraseOriginal[hatLane].midiNotes[hit]
            || denseHat.midiHatPedal[hit]
                != densityPhraseOriginal[hatLane].midiHatPedal[hit]) {
            std::cerr << "seeded phrase derivation mutated its imported source\n";
            return 1;
        }

    RhythmicCell phraseSelected;
    RhythmicCell phrasePartner;
    phraseSelected.enabled = true;
    phrasePartner.enabled = true;
    for (const double position : {0.0625, 0.1875, 0.5625, 0.6875}) {
        std::size_t inserted = 0;
        if (!insertRhythmicCellAttack(phraseSelected, position, 0.78f,
                                      &inserted)) {
            std::cerr << "could not prepare selected phrase bars\n";
            return 1;
        }
        phraseSelected.midiNotes[inserted] = 36u;
        phraseSelected.midiHatPedal[inserted] = 55u;
    }
    for (const double position : {0.0, 0.125, 0.5, 0.625}) {
        std::size_t inserted = 0;
        if (!insertRhythmicCellAttack(phrasePartner, position, 0.62f,
                                      &inserted)) {
            std::cerr << "could not prepare partner phrase bars\n";
            return 1;
        }
        phrasePartner.midiNotes[inserted] = 38u;
    }
    const auto originalPhraseSelected = phraseSelected;
    const bool phraseOperationAvailable = canRecombineRhythmicPhraseBars(
        phraseSelected, phrasePartner, 2u);
    auto phraseBarsA = phraseSelected;
    auto phraseBarsAgain = phraseSelected;
    auto phraseBarsB = phraseSelected;
    const bool phraseBarsChanged = recombineRhythmicPhraseBars(
        phraseBarsA, phrasePartner, 2u, 0x13579bdfu);
    const bool phraseBarsRepeat = recombineRhythmicPhraseBars(
        phraseBarsAgain, phrasePartner, 2u, 0x13579bdfu);
    const bool phraseBarsAlternate = recombineRhythmicPhraseBars(
        phraseBarsB, phrasePartner, 2u, 0x2468ace0u);
    bool phraseReproduced = phraseBarsA.count == phraseBarsAgain.count;
    bool phraseSeedChanged = phraseBarsA.count != phraseBarsB.count;
    for (std::size_t hit = 0; hit < phraseBarsA.count; ++hit) {
        phraseReproduced = phraseReproduced
            && close(phraseBarsA.positions[hit], phraseBarsAgain.positions[hit])
            && close(phraseBarsA.strengths[hit], phraseBarsAgain.strengths[hit])
            && phraseBarsA.midiNotes[hit] == phraseBarsAgain.midiNotes[hit];
        phraseSeedChanged = phraseSeedChanged
            || !close(phraseBarsA.positions[hit], phraseBarsB.positions[hit]);
        if (phraseBarsA.midiNotes[hit] != 36u
            || phraseBarsA.midiHatPedal[hit] != 55u) {
            std::cerr << "bar recombination replaced the selected voice's kit\n";
            return 1;
        }
    }
    for (std::uint32_t alternateSeed = 0;
         !phraseSeedChanged && alternateSeed < 32u; ++alternateSeed) {
        auto candidate = phraseSelected;
        if (!recombineRhythmicPhraseBars(candidate, phrasePartner, 2u,
                0x9e3779b9u * (alternateSeed + 1u)))
            continue;
        if (candidate.count != phraseBarsA.count) {
            phraseSeedChanged = true;
            break;
        }
        for (std::size_t hit = 0; hit < candidate.count; ++hit)
            phraseSeedChanged = phraseSeedChanged
                || !close(candidate.positions[hit], phraseBarsA.positions[hit]);
    }
    auto rejectedSingleBar = phraseSelected;
    if (!phraseOperationAvailable || !phraseBarsChanged || !phraseBarsRepeat
        || !phraseBarsAlternate
        || !phraseReproduced || !phraseSeedChanged
        || !validRhythmicCell(phraseBarsA)
        || !close(phraseSelected.positions[0],
                  originalPhraseSelected.positions[0])
        || recombineRhythmicPhraseBars(rejectedSingleBar, phrasePartner,
                                       1u, 0x13579bdfu)
        || canRecombineRhythmicPhraseBars(phraseSelected, phrasePartner, 1u)) {
        std::cerr << "bar-level phrase recombination failed its invariants: changed="
                  << phraseBarsChanged << " repeat=" << phraseBarsRepeat
                  << " alternate=" << phraseBarsAlternate
                  << " reproduced=" << phraseReproduced
                  << " seedChanged=" << phraseSeedChanged
                  << " counts=" << static_cast<int>(phraseBarsA.count) << '/'
                  << static_cast<int>(phraseBarsB.count) << " valid="
                  << validRhythmicCell(phraseBarsA) << '\n';
        return 1;
    }

    RhythmicCell denseSupport;
    RhythmicCell sparseSupport;
    denseSupport.enabled = true;
    sparseSupport.enabled = true;
    for (const double position : {0.0, 0.0625, 0.125, 0.1875,
                                  0.5, 0.5625, 0.625, 0.6875}) {
        std::size_t inserted = 0;
        const float strength = close(position, 0.0625)
                || close(position, 0.5625) ? 0.95f : 0.20f;
        if (!insertRhythmicCellAttack(denseSupport, position, strength,
                                      &inserted)) {
            std::cerr << "could not prepare dense support-lane pair\n";
            return 1;
        }
        denseSupport.midiNotes[inserted] = 42u;
        denseSupport.midiHatPedal[inserted] = 61u;
    }
    for (const double position : {0.0, 0.125, 0.5, 0.625}) {
        std::size_t inserted = 0;
        if (!insertRhythmicCellAttack(sparseSupport, position, 0.70f,
                                      &inserted)) {
            std::cerr << "could not prepare sparse support-lane pair\n";
            return 1;
        }
        sparseSupport.midiNotes[inserted] = 46u;
    }
    denseSupport.midiCollisionCount[1] = 1;
    denseSupport.midiCollisionNotes[1][0] = 46u;
    denseSupport.midiCollisionStrengths[1][0] = 0.66f;
    denseSupport.midiCollisionHatPedal[1][0] = 90u;
    const auto originalDenseSupport = denseSupport;
    const auto originalSparseSupport = sparseSupport;
    auto adaptedDense = denseSupport;
    auto adaptedSparse = sparseSupport;
    auto repeatedDense = denseSupport;
    auto repeatedSparse = sparseSupport;
    const bool densityBalanceAvailable = canAdaptRhythmicCellPairDensity(
        denseSupport, sparseSupport, 2u);
    const bool densityBalanced = adaptRhythmicCellPairDensity(adaptedDense,
        adaptedSparse, 2u, 0x10293847u);
    const bool densityRepeated = adaptRhythmicCellPairDensity(repeatedDense,
        repeatedSparse, 2u, 0x10293847u);
    std::array<std::size_t, 2> densePerBar{};
    std::array<std::size_t, 2> sparsePerBar{};
    bool downbeatPerBarRetained[2] = {false, false};
    bool strongHitsRetained[2] = {false, false};
    bool collisionRetained = false;
    for (std::size_t hit = 0; hit < adaptedDense.count; ++hit) {
        const auto bar = std::min<std::size_t>(1,
            static_cast<std::size_t>(adaptedDense.positions[hit] * 2.0));
        ++densePerBar[bar];
        const double local = adaptedDense.positions[hit] * 2.0 - bar;
        downbeatPerBarRetained[bar] = downbeatPerBarRetained[bar]
            || close(local, 0.0);
        strongHitsRetained[bar] = strongHitsRetained[bar]
            || close(local, 0.125);
        collisionRetained = collisionRetained
            || adaptedDense.midiCollisionCount[hit] == 1u
                && adaptedDense.midiCollisionNotes[hit][0] == 46u
                && adaptedDense.midiCollisionHatPedal[hit][0] == 90u;
    }
    for (std::size_t hit = 0; hit < adaptedSparse.count; ++hit) {
        const auto bar = std::min<std::size_t>(1,
            static_cast<std::size_t>(adaptedSparse.positions[hit] * 2.0));
        ++sparsePerBar[bar];
    }
    bool pairDynamicsReproducible = adaptedDense.count == repeatedDense.count
        && adaptedSparse.count == repeatedSparse.count;
    for (std::size_t hit = 0; pairDynamicsReproducible
            && hit < adaptedDense.count; ++hit)
        pairDynamicsReproducible = close(adaptedDense.positions[hit],
                                          repeatedDense.positions[hit]);
    if (!densityBalanceAvailable || !densityBalanced || !densityRepeated
        || !pairDynamicsReproducible || adaptedDense.count != 6u
        || adaptedSparse.count != sparseSupport.count
        || densePerBar[0] != 3u || densePerBar[1] != 3u
        || sparsePerBar[0] != 2u || sparsePerBar[1] != 2u
        || !downbeatPerBarRetained[0] || !downbeatPerBarRetained[1]
        || !strongHitsRetained[0] || !strongHitsRetained[1]
        || !collisionRetained
        || denseSupport.count != originalDenseSupport.count
        || sparseSupport.count != originalSparseSupport.count
        || !validRhythmicCell(adaptedDense)
        || !validRhythmicCell(adaptedSparse)) {
        std::cerr << "cross-stem adaptive density failed preservation or balance guarantees\n";
        return 1;
    }

    MidiRhythmImportOptions twoBars;
    twoBars.bars = 2;
    MidiRhythmImportResult wider;
    if (importMidiRhythm(kOneBarMidi.data(), kOneBarMidi.size(), twoBars, wider)
            != MidiRhythmImportError::none
        || wider.droppedHits != 0
        || wider.outsideWindowHits != 0
        || wider.inWindowDroppedHits != 0
        || wider.cells[static_cast<std::size_t>(MidiDrumLane::kick)].count != 2
        || !close(wider.cells[static_cast<std::size_t>(MidiDrumLane::kick)].positions[1],
                  0.5)) {
        std::cerr << "bar window was not applied deterministically\n";
        return 1;
    }

    std::array<std::uint8_t, 14> invalid = {{
        'M','T','h','d', 0,0,0,6, 0,2, 0,1, 1,0xE0
    }};
    MidiRhythmImportResult rejected;
    if (importMidiRhythm(invalid.data(), invalid.size(), {}, rejected)
            != MidiRhythmImportError::unsupportedFormat) {
        std::cerr << "unsupported MIDI format was not rejected\n";
        return 1;
    }
    return 0;
}
