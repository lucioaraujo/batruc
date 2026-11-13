// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/MidiRhythmImport.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace {

using namespace batruc;
constexpr double kSampleRate = 48000.0;
constexpr std::array<const char*, kMidiDrumLaneCount> kRoles = {
    "kick", "snare", "closed_hat", "open_hat", "toms", "cymbals",
    "hand_percussion", "other"};
constexpr std::array<SoundCharacter, kMidiDrumLaneCount> kSounds = {
    SoundCharacter::kick, SoundCharacter::snare, SoundCharacter::hihat,
    SoundCharacter::hihat, SoundCharacter::membrane, SoundCharacter::ride,
    SoundCharacter::conga, SoundCharacter::membrane};

void write16(std::ofstream& output, const std::uint16_t value) {
    output.put(static_cast<char>(value & 0xffu));
    output.put(static_cast<char>((value >> 8u) & 0xffu));
}

void write32(std::ofstream& output, const std::uint32_t value) {
    write16(output, static_cast<std::uint16_t>(value & 0xffffu));
    write16(output, static_cast<std::uint16_t>(value >> 16u));
}

bool writeWav(const std::string& path, const std::vector<float>& stereo,
              const float gain) {
    if (stereo.size() > (std::numeric_limits<std::uint32_t>::max() - 36u) / 2u)
        return false;
    std::ofstream output(path, std::ios::binary);
    if (!output)
        return false;
    const auto bytes = static_cast<std::uint32_t>(stereo.size() * 2u);
    output.write("RIFF", 4);
    write32(output, 36u + bytes);
    output.write("WAVEfmt ", 8);
    write32(output, 16u);
    write16(output, 1u);
    write16(output, 2u);
    write32(output, static_cast<std::uint32_t>(kSampleRate));
    write32(output, static_cast<std::uint32_t>(kSampleRate) * 4u);
    write16(output, 4u);
    write16(output, 16u);
    output.write("data", 4);
    write32(output, bytes);
    for (const float sample : stereo) {
        const auto pcm = static_cast<std::int16_t>(std::lrint(
            std::clamp(sample * gain, -1.0f, 1.0f) * 32767.0f));
        write16(output, static_cast<std::uint16_t>(pcm));
    }
    return static_cast<bool>(output);
}

bool render(const CompositionRecipe& source, const int solo,
            const int familyIndex,
            std::vector<float>& stereo) {
    auto recipe = source;
    if (solo >= 0) {
        for (std::size_t lane = 0; lane < kMidiDrumLaneCount; ++lane) {
            if (static_cast<int>(lane) != solo)
                recipe.rhythmicCells[lane] = RhythmicCell{true};
        }
    }
    EventPlan plan;
    if (buildEventPlan(recipe, plan) != PlanError::none)
        return false;
    ResonatorRenderer renderer;
    if (!renderer.configure(recipe, plan, kSampleRate))
        return false;
    renderer.setActiveVoiceCount(kMidiDrumLaneCount);
    renderer.setFormalVoiceReductionEnabled(false);
    renderer.setDecayScale(0.35f);
    renderer.setFundamentalLevel(0.55f);
    for (std::size_t lane = 0; lane < kMidiDrumLaneCount; ++lane) {
        renderer.setChannelSound(lane, kSounds[lane]);
        renderer.setChannelTimbreProfile(lane, familyIndex);
    }

    const auto frames = static_cast<std::size_t>(
        std::llround(recipe.sectionSeconds * kSampleRate));
    stereo.resize(frames * 2u);
    std::array<float, 512> left{}, right{};
    for (std::size_t position = 0; position < frames;) {
        const auto count = std::min(left.size(), frames - position);
        renderer.process(left.data(), right.data(), count);
        for (std::size_t frame = 0; frame < count; ++frame) {
            stereo[(position + frame) * 2u] = left[frame];
            stereo[(position + frame) * 2u + 1u] = right[frame];
        }
        position += count;
    }
    return true;
}

} // namespace

int main(const int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage: batruc_midi_reference_render input.mid BPM output-prefix"
                     " [--family -1..16] [--gain 0.01..1] [--mix-only]"
                     " [--seed 8hex --variation 0..1]\n";
        return 2;
    }
    int familyIndex = -1;
    float fixedGain = 0.0f;
    bool familyExplicit = false;
    bool mixOnly = false;
    bool hasSeed = false;
    bool hasVariation = false;
    std::uint32_t seed = 0;
    float variation = 0.5f;
    for (int argument = 4; argument < argc; ++argument) {
        const std::string option(argv[argument]);
        if (option == "--mix-only") {
            mixOnly = true;
            continue;
        }
        if (argument + 1 >= argc) {
            std::cerr << "Option needs a value: " << option << '\n';
            return 2;
        }
        char* valueEnd = nullptr;
        if (option == "--family") {
            const long value = std::strtol(argv[++argument], &valueEnd, 10);
            if (valueEnd == argv[argument] || *valueEnd != '\0'
                || value < -1 || value > 16) {
                std::cerr << "Family index must be -1..16\n";
                return 2;
            }
            familyIndex = static_cast<int>(value);
            familyExplicit = true;
        } else if (option == "--gain") {
            const double value = std::strtod(argv[++argument], &valueEnd);
            if (valueEnd == argv[argument] || *valueEnd != '\0'
                || !std::isfinite(value) || value < 0.01 || value > 1.0) {
                std::cerr << "Fixed gain must be 0.01..1\n";
                return 2;
            }
            fixedGain = static_cast<float>(value);
        } else if (option == "--seed") {
            const std::string token(argv[++argument]);
            if (token.size() != 8) {
                std::cerr << "Seed must contain exactly eight hexadecimal digits\n";
                return 2;
            }
            if (!std::all_of(token.begin(), token.end(), [](const char digit) {
                    return std::isxdigit(static_cast<unsigned char>(digit)) != 0;
                })) {
                std::cerr << "Seed must contain exactly eight hexadecimal digits\n";
                return 2;
            }
            valueEnd = nullptr;
            const auto value = std::strtoul(token.c_str(), &valueEnd, 16);
            if (valueEnd == token.c_str() || *valueEnd != '\0'
                || value > std::numeric_limits<std::uint32_t>::max()) {
                std::cerr << "Seed must contain exactly eight hexadecimal digits\n";
                return 2;
            }
            seed = static_cast<std::uint32_t>(value);
            hasSeed = true;
        } else if (option == "--variation") {
            const double value = std::strtod(argv[++argument], &valueEnd);
            if (valueEnd == argv[argument] || *valueEnd != '\0'
                || !std::isfinite(value) || value < 0.0 || value > 1.0) {
                std::cerr << "Variation must be between 0 and 1\n";
                return 2;
            }
            variation = static_cast<float>(value);
            hasVariation = true;
        } else {
            std::cerr << "Unknown option: " << option << '\n';
            return 2;
        }
    }
    if (hasVariation && !hasSeed) {
        std::cerr << "--variation requires --seed\n";
        return 2;
    }
    // Family comparisons need one gain across files. Legacy calls without a
    // family keep the previous peak-normalised reference behavior.
    if (familyExplicit && fixedGain == 0.0f)
        fixedGain = 0.7f;
    char* end = nullptr;
    const double bpm = std::strtod(argv[2], &end);
    if (end == argv[2] || *end != '\0' || !std::isfinite(bpm)
        || bpm < 20.0 || bpm > 300.0) {
        std::cerr << "BPM must be between 20 and 300\n";
        return 2;
    }
    std::ifstream input(argv[1], std::ios::binary | std::ios::ate);
    if (!input || input.tellg() <= 0
        || input.tellg() > static_cast<std::streamoff>(kMaxMidiImportBytes)) {
        std::cerr << "MIDI is missing or exceeds the importer limit\n";
        return 2;
    }
    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(input.tellg()));
    input.seekg(0);
    if (!input.read(reinterpret_cast<char*>(bytes.data()),
                    static_cast<std::streamsize>(bytes.size()))) {
        std::cerr << "Could not read the MIDI file\n";
        return 2;
    }
    MidiRhythmImportOptions options;
    options.bars = 2;
    options.mapping = MidiDrumMapping::rolandTd11;
    MidiRhythmImportResult imported;
    const auto error = importMidiRhythm(bytes.data(), bytes.size(),
                                        options, imported);
    if (error != MidiRhythmImportError::none
        || imported.meterNumerator != 4 || imported.meterDenominator != 4) {
        std::cerr << "Expected a readable 4/4 percussion MIDI; importer error "
                  << static_cast<int>(error) << '\n';
        return 2;
    }
    const auto sourceCells = imported.cells;
    if (hasSeed)
        imported.cells = seededMidiPhraseCells(sourceCells, seed, variation,
            static_cast<std::uint8_t>(options.bars));
    CompositionRecipe recipe = makeInitialRecipe();
    recipe.voiceCount = kMidiDrumLaneCount;
    recipe.sectionSeconds = 2.0 * 4.0 * 60.0 / bpm;
    recipe.patternMode = PatternMode::unrestricted;
    for (std::size_t lane = 0; lane < kMidiDrumLaneCount; ++lane) {
        if (lane >= 2)
            recipe.voices[lane] = recipe.voices[1];
        recipe.voices[lane].pan = 0.0f;
        recipe.rhythmicCells[lane] = imported.cells[lane];
        recipe.rhythmicCells[lane].enabled = true;
    }
    EventPlan plan;
    if (buildEventPlan(recipe, plan) != PlanError::none) {
        std::cerr << "Could not schedule the imported cells\n";
        return 2;
    }
    const std::string prefix = argv[3];
    std::vector<std::string> outputs{prefix + "-mix.wav", prefix + "-trace.tsv"};
    if (!mixOnly)
        for (std::size_t lane = 0; lane < kMidiDrumLaneCount; ++lane)
            if (imported.cells[lane].count > 0)
                outputs.push_back(prefix + "-V" + std::to_string(lane + 1) + ".wav");
    for (const auto& output : outputs)
        if (std::filesystem::exists(output)) {
            std::cerr << "Output exists; nothing was overwritten: " << output << '\n';
            return 2;
        }
    std::ofstream trace(prefix + "-trace.tsv");
    if (!trace)
        return 2;
    trace << "# source\t" << argv[1] << "\n# bpm\t" << bpm
          << "\n# family_index\t" << familyIndex
          << "\n# fixed_output_gain\t" << fixedGain
          << "\n# seed\t" << (hasSeed ? [&] {
                 std::ostringstream formatted;
                 formatted << std::hex << std::uppercase << std::setw(8)
                           << std::setfill('0') << seed;
                 return formatted.str();
             }() : std::string("none"))
          << "\n# variation\t" << (hasSeed ? variation : 0.0f)
          << "\n# source_tempo_bpm\t" << imported.tempoBpm
          << "\n# bars\t2\n# source_note_ons\t" << imported.noteOnEvents
          << "\n# imported_hits\t" << imported.importedHits
          << "\n# merged_hits\t" << imported.mergedHits
          << "\n# distinct_simultaneous_notes\t" << imported.collidingHits
          << "\n# outside_selected_window\t" << imported.outsideWindowHits
          << "\n# lost_inside_selected_window\t" << imported.inWindowDroppedHits
          << '\n';
    for (std::size_t lane = 0; lane < kMidiDrumLaneCount; ++lane)
        trace << "# lane_" << (lane + 1) << "_hits\t"
              << static_cast<unsigned>(sourceCells[lane].count) << " -> "
              << static_cast<unsigned>(imported.cells[lane].count)
              << '\n';
    trace << "time_s\tstem\trole\tgm_note\tvelocity\n";
    trace << std::fixed << std::setprecision(6);
    for (const auto& event : plan.events)
        trace << event.timeSeconds << '\t' << (event.voice + 1) << '\t'
              << kRoles[event.voice] << '\t'
              << static_cast<int>(event.midiNote) << '\t'
              << event.intensity << '\n';
    if (!trace)
        return 2;
    trace.close();
    std::vector<std::vector<float>> rendered(outputs.size() - 1u);
    if (!render(recipe, -1, familyIndex, rendered[0]))
        return 2;
    std::size_t outputIndex = 1;
    if (!mixOnly)
        for (std::size_t lane = 0; lane < kMidiDrumLaneCount; ++lane)
            if (imported.cells[lane].count > 0
                && !render(recipe, static_cast<int>(lane), familyIndex,
                           rendered[outputIndex++]))
                return 2;
    float peak = 0.0f;
    for (const auto& stereo : rendered)
        for (const float sample : stereo)
            peak = std::max(peak, std::abs(sample));
    const float gain = fixedGain > 0.0f ? fixedGain
        : peak > 0.0f ? std::min(6.0f, 0.85f / peak) : 1.0f;
    for (std::size_t index = 0; index < rendered.size(); ++index)
        if (!writeWav(outputs[index == 0 ? 0 : index + 1],
                      rendered[index], gain))
            return 2;
    std::cout << "Imported " << imported.importedHits << " source hits and scheduled "
              << plan.events.size() << " events across two bars (Roland TD-11 mapping); "
              << imported.outsideWindowHits << " outside the selected window; "
              << imported.inWindowDroppedHits << " lost inside the window; "
              << "family " << familyIndex << ", gain " << gain << "; "
              << "output prefix " << prefix << '\n';
    return 0;
}
