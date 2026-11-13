// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/BassAudio.hpp"
#include "core/BassComposition.hpp"
#include "core/BassLineComposer.hpp"
#include "core/SnareOrnaments.hpp"
#include "core/FigureComposer.hpp"
#include "core/CompositionCore.hpp"
#include "core/EcologicalRelations.hpp"
#include "core/GenerativeOperations.hpp"
#include "core/OutputSafety.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

namespace {

using namespace batruc;
constexpr double kSectionSeconds = 8.0;
constexpr double kBpm = 120.0;
constexpr double kSwing = 0.50;
constexpr float kVoiceVariation = 0.35f;
constexpr float kBassVariation = 0.35f;
constexpr float kMasterGain = 0.72f;
constexpr float kFormulaBusGainDb = 16.5f;
constexpr float kFormulaBusGain = 6.683439f;
constexpr float kVoiceDecayScale = 0.35f;
constexpr float kVoiceFundamentalLevel = 0.55f;
constexpr double kSampleRate = 48000.0;
constexpr std::size_t kBlock = 512;

std::uint32_t hashSeed(const std::string& seed) {
    std::uint32_t hash = 2166136261u;
    for (const unsigned char character : seed)
        hash = (hash ^ character) * 16777619u;
    return hash;
}

int bassProfileForSeed(std::uint32_t value) noexcept {
    value ^= value >> 16u;
    value *= 0x7feb352du;
    value ^= value >> 15u;
    value *= 0x846ca68bu;
    value ^= value >> 16u;
    return static_cast<int>(value & 3u);
}

double transformedVoiceValue(const std::size_t voice, const double progress,
                             const std::size_t cycle, const double phaseAmount,
                             const SeedVariationResult& variation) {
    const auto segment = formalPhaseSegmentForCycle(cycle, phaseAmount);
    const double phase = voice == 0 ? 0.0 : variation.phases[voice]
        + segment.offsetCycles + segment.slopeCycles * progress;
    const double theta = kTau * (variation.cycles[voice] * progress + phase);
    const double sine = std::sin(theta);
    double own = sine;
    switch (static_cast<VoiceTransform>(variation.transforms[voice])) {
        case VoiceTransform::sine: break;
        case VoiceTransform::invert: own = -sine; break;
        case VoiceTransform::rectify: own = 2.0 * std::abs(sine) - 1.0; break;
        case VoiceTransform::harmonic: own = (sine + 0.5 * std::sin(2.0 * theta)) / 1.5; break;
        case VoiceTransform::amplitude: own = sine * (0.65 + 0.35 * std::cos(theta * 0.5)); break;
        case VoiceTransform::quantized: own = std::floor(sine * 4.0 + 0.5) / 4.0; break;
        case VoiceTransform::cubic: own = sine * sine * sine; break;
        case VoiceTransform::thirdHarmonic: own = std::sin(3.0 * theta); break;
        case VoiceTransform::triangle:
            own = (2.0 / 3.14159265358979323846) * std::asin(std::clamp(sine, -1.0, 1.0));
            break;
        case VoiceTransform::softClip: own = std::tanh(2.0 * sine) / std::tanh(2.0); break;
    }
    if (voice == 0 || variation.couplings[voice] == 0.0f)
        return own;
    return (1.0 - variation.couplings[voice]) * own
        + variation.couplings[voice] * transformedVoiceValue(
            voice - 1, progress, cycle, phaseAmount, variation);
}

void put16(std::ofstream& out, const std::uint16_t value) {
    out.put(static_cast<char>(value & 0xff));
    out.put(static_cast<char>((value >> 8) & 0xff));
}
void put32(std::ofstream& out, const std::uint32_t value) {
    put16(out, static_cast<std::uint16_t>(value & 0xffff));
    put16(out, static_cast<std::uint16_t>(value >> 16));
}

bool writeWav(const std::string& path, const std::vector<float>& interleaved) {
    if (interleaved.size() > (std::numeric_limits<std::uint32_t>::max() - 36u) / 2u)
        return false;
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;
    const auto dataBytes = static_cast<std::uint32_t>(interleaved.size() * 2u);
    out.write("RIFF", 4); put32(out, 36u + dataBytes); out.write("WAVE", 4);
    out.write("fmt ", 4); put32(out, 16); put16(out, 1); put16(out, 2);
    put32(out, static_cast<std::uint32_t>(kSampleRate));
    put32(out, static_cast<std::uint32_t>(kSampleRate) * 4u);
    put16(out, 4); put16(out, 16); out.write("data", 4); put32(out, dataBytes);
    for (float sample : interleaved) {
        sample = std::clamp(sample, -1.0f, 1.0f);
        const auto pcm = static_cast<std::int16_t>(std::lrint(sample * 32767.0f));
        put16(out, static_cast<std::uint16_t>(pcm));
    }
    return static_cast<bool>(out);
}

std::string stemPath(const std::string& audioPath, const std::string& stem) {
    const auto extension = audioPath.rfind(".wav");
    const auto base = extension != std::string::npos
        && extension + 4 == audioPath.size()
        ? audioPath.substr(0, extension) : audioPath;
    return base + "-" + stem + ".wav";
}

bool writeRecipe(const std::string& path, const std::string& seed,
                 const std::size_t bassEvents,
                 const std::array<std::size_t, kFormalCycleCount>& voiceEvents,
                 const std::array<CompositionRecipe, kFormalCycleCount>& recipes,
                 const std::array<EventPlan, kFormalCycleCount>& plans,
                 const std::array<std::vector<SeedBassVoiceEvent>, kFormalCycleCount>& bassSources,
                 const BassPerformance& bassPerformance,
                 const bool bassCycleRecombined,
                 const std::size_t bassDonorOffset,
                 const bool coupledPendulum,
                 const bool voiceCallResponse,
                 const bool logisticBassDynamics,
                 const LogisticMapParameters& logisticMapParameters,
                 const double logisticFinalState,
                 const CoupledPendulumParameters& pendulumParameters,
                 const std::size_t voiceCount, const double phaseAmount,
                 const SeededRecipeSelection& selection,
                 const int bassProfile, const SeedVariationResult& variation,
                 const std::array<float, kMaxVoices>& channelPans,
                 const float voiceDecayScale,
                 const float outputTruePeak, const float limiterReduction) {
    std::ofstream out(path + ".json");
    if (!out) return false;
    out << std::setprecision(9);
    out << "{\n"
        << "  \"format\": \"batruc-offline-render-v1\",\n"
        << "  \"engine\": \"JUCE core offline\",\n"
        << "  \"seed\": \"" << seed << "\",\n"
        << "  \"sound_family_index\": " << selection.soundFamilyIndex << ",\n"
        << "  \"sample_rate\": " << static_cast<int>(kSampleRate) << ",\n"
        << "  \"channels\": 2,\n"
        << "  \"sample_format\": \"PCM16\",\n"
        << "  \"bpm\": " << kBpm << ",\n"
        << "  \"meter\": \"4/4\",\n"
        << "  \"formal_cycles\": 7,\n"
        << "  \"section_seconds\": " << kSectionSeconds << ",\n"
        << "  \"voice_count\": " << voiceCount << ",\n"
        << "  \"seeded_phase\": " << phaseAmount << ",\n"
        << "  \"seeded_pattern_index\": " << selection.patternIndex << ",\n"
        << "  \"seeded_event_rule_index\": " << selection.eventRuleIndex << ",\n"
        << "  \"seeded_intensity_law_index\": " << selection.intensityLawIndex << ",\n"
        << "  \"intensity_amount\": " << selection.intensityAmount << ",\n"
        << "  \"voice_variation\": " << kVoiceVariation << ",\n"
        << "  \"bass_variation\": " << kBassVariation << ",\n"
        << "  \"bass_oscillator_profile_index\": " << bassProfile << ",\n"
        << "  \"bass_cycle_recombined\": "
        << (bassCycleRecombined ? "true" : "false") << ",\n"
        << "  \"logistic_bass_dynamics\": ";
    if (logisticBassDynamics) {
        out << "{\"model\":\"logistic_map\",\"equation\":\"x[n+1]=r*x[n]*(1-x[n])\","
            << "\"r\":" << logisticMapParameters.r
            << ",\"initial_state\":" << logisticMapParameters.initialState
            << ",\"depth\":" << logisticMapParameters.depth
            << ",\"burn_in\":" << logisticMapParameters.burnIn
            << ",\"final_state\":" << logisticFinalState
            << ",\"target\":\"bass_event_velocity_filter_gate\"}";
    } else {
        out << "null";
    }
    out << ",\n"
        << "  \"bass_recombination_donor_offset\": " << bassDonorOffset << ",\n"
        << "  \"bass_recombination_rule\": \"target attacks retained; same-bar nearest phase; donor=(cycle+1+((offset-1+bar)%6))%7\",\n"
        << "  \"voice_relation\": \""
        << (voiceCallResponse ? "seeded_call_response" : "none") << "\",\n"
        << "  \"voice_relation_parameters\": ";
    if (voiceCallResponse) {
        out << "{\"source_voice\":1, \"target_voice\":2,"
            << " \"strength\":0.42, \"delay_seconds\":"
            << (kSectionSeconds / 16.0) << ", \"seed\":"
            << static_cast<std::uint32_t>(std::stoul(seed, nullptr, 16)) << "}";
    } else {
        out << "null";
    }
    out << ",\n"
        << "  \"motion_model\": \""
        << (coupledPendulum ? "seeded_coupled_triple_pendulum" : "formula_curves")
        << "\",\n"
        << "  \"pendulum_parameters\": ";
    if (coupledPendulum) {
        out << "{\"model\": \"three equal small-angle pendula with nearest-neighbour effective springs\","
            << " \"integrator\": \"RK4\", \"sample_step_s\": "
            << pendulumParameters.sampleStepSeconds
            << ", \"gravity_m_s2\": " << pendulumParameters.gravity
            << ", \"length_m\": " << pendulumParameters.lengthMetres
            << ", \"mass_kg\": " << pendulumParameters.massKilograms
            << ", \"spring_n_m\": "
            << pendulumParameters.springConstantNewtonsPerMetre
            << ", \"damping_per_s\": "
            << pendulumParameters.dampingPerSecond
            << ", \"initial_angles_rad\": [";
        for (std::size_t voice = 0; voice < 3; ++voice)
            out << (voice == 0 ? "" : ", ")
                << pendulumParameters.initialAnglesRadians[voice];
        out << "], \"initial_angular_velocities_rad_s\": [";
        for (std::size_t voice = 0; voice < 3; ++voice)
            out << (voice == 0 ? "" : ", ")
                << pendulumParameters.initialVelocitiesRadiansPerSecond[voice];
        out << "]}";
    } else {
        out << "null";
    }
    out << ",\n"
        << "  \"voice_decay_scale\": " << voiceDecayScale << ",\n"
        << "  \"voice_fundamental_level\": " << kVoiceFundamentalLevel << ",\n"
        << "  \"mixer\": {\"master_gain\": " << kMasterGain
        << ", \"formula_bus_gain_db\": " << kFormulaBusGainDb
        << ", \"bass_gain\": 0.2, \"bass_pan\": 0, \"voice_channel_volumes\": [";
    for (std::size_t voice = 0; voice < voiceCount; ++voice)
        out << (voice == 0 ? "" : ", ") << 1.0;
    out << "], \"voice_channel_pans\": [";
    for (std::size_t voice = 0; voice < voiceCount; ++voice)
        out << (voice == 0 ? "" : ", ") << channelPans[voice];
    out << "]},\n"
        << "  \"swing_ratio\": " << kSwing << ",\n"
        << "  \"groove\": \"straight\",\n"
        << "  \"bass_relation\": \"follow\",\n"
        << "  \"bass_duration_law\": \"manual\",\n"
        << "  \"voice_event_count_per_cycle\": [";
    for (std::size_t i = 0; i < kFormalCycleCount; ++i)
        out << (i == 0 ? "" : ", ") << voiceEvents[i];
    out << "],\n  \"voice_cycles\": [";
    for (std::size_t i = 0; i < voiceCount; ++i)
        out << (i == 0 ? "" : ", ") << variation.cycles[i];
    out << "],\n  \"voice_transform_indices\": [";
    for (std::size_t i = 0; i < voiceCount; ++i)
        out << (i == 0 ? "" : ", ") << variation.transforms[i];
    out << "],\n  \"voice_couplings\": [";
    for (std::size_t i = 0; i < voiceCount; ++i)
        out << (i == 0 ? "" : ", ") << variation.couplings[i];
    out << "],\n  \"voice_phase_offsets\": [";
    for (std::size_t i = 0; i < voiceCount; ++i)
        out << (i == 0 ? "" : ", ") << variation.phases[i];
    out << "],\n  \"voice_sound\": \"triangle\",\n"
        << "  \"bass_event_count\": " << bassEvents << ",\n"
        << "  \"event_schedule\": [";
    bool first = true;
    constexpr double quarterNotesPerSection = 16.0;
    for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle) {
        const auto activeVoices = activeVoiceCountInCycle(cycle, voiceCount);
        for (const auto& event : plans[cycle].events) {
            if (event.voice >= activeVoices)
                continue;
            const double localProgress = event.timeSeconds / recipes[cycle].sectionSeconds;
            const double performed = swingProgress(
                localProgress, kSwing, quarterNotesPerSection);
            const double absoluteTime = static_cast<double>(cycle) * kSectionSeconds
                + performed * kSectionSeconds;
            const double localPerformedTime = performed * kSectionSeconds;
            const SeedBassVoiceEvent* bassCandidate = nullptr;
            for (const auto& candidate : bassSources[cycle]) {
                if (candidate.voice == event.voice
                    && std::abs(candidate.localTimeSeconds - localPerformedTime) < 1.0e-6) {
                    bassCandidate = &candidate;
                    break;
                }
            }
            out << (first ? "\n" : ",\n")
                << "    {\"type\":\"voice\",\"voice\":"
                << static_cast<unsigned>(event.voice + 1u)
                << ",\"derived_by_relation\":"
                << (event.generatedByRelation ? "true" : "false")
                << ",\"cycle\":" << cycle
                << ",\"time_seconds\":" << absoluteTime
                << ",\"intensity\":" << event.intensity
                << ",\"field_x\":" << event.fieldX
                << ",\"field_y\":" << event.fieldY;
            if (bassCandidate != nullptr) {
                out << ",\"bass_candidate_x\":" << bassCandidate->x
                    << ",\"bass_candidate_y\":" << bassCandidate->y
                    << ",\"bass_candidate_intensity\":" << bassCandidate->intensity;
            }
            out << "}";
            first = false;
        }
    }
    for (std::size_t index = 0; index < bassPerformance.count; ++index) {
        const auto& event = bassPerformance.events[index];
        const double absoluteTime = static_cast<double>(event.formCycle)
            * kSectionSeconds + event.timeSeconds;
        const SeedBassVoiceEvent* source = nullptr;
        double sourcePriority = -1.0;
        constexpr double bassStepSeconds = 15.0 / kBpm;
        for (const auto& candidate : bassSources[event.formCycle]) {
            const auto slot = static_cast<std::size_t>(std::floor(
                candidate.localTimeSeconds / bassStepSeconds + 1.0e-9));
            if (slot != event.sourceNote)
                continue;
            const double priority = (candidate.voice == 0 ? 2.0 : 1.0)
                * (0.5 + std::clamp(static_cast<double>(candidate.intensity), 0.0, 1.0));
            if (priority > sourcePriority) {
                source = &candidate;
                sourcePriority = priority;
            }
        }
        out << (first ? "\n" : ",\n")
            << "    {\"type\":\"bass\",\"voice\":\"bass\",\"cycle\":"
            << static_cast<unsigned>(event.formCycle)
            << ",\"time_seconds\":" << absoluteTime
            << ",\"pitch_midi\":" << static_cast<unsigned>(event.midi)
            << ",\"duration_seconds\":" << event.durationSeconds
            << ",\"intensity\":" << event.velocity;
        if (logisticBassDynamics) {
            out << ",\"dynamics_map_state\":" << event.dynamicsMapState
                << ",\"dynamics_map_scale\":" << event.dynamicsMapScale;
        }
        if (source != nullptr) {
            out << ",\"source_voice\":" << static_cast<unsigned>(source->voice + 1u)
                << ",\"source_x\":" << source->x
                << ",\"source_y\":" << source->y;
        }
        out << "}";
        first = false;
    }
    out << "\n  ],\n  \"output_safety\": {\"type\": \"stereo lookahead guard\","
        << " \"ceiling_dbfs\": -1.0, \"lookahead_ms\": 3.0,"
        << " \"output_true_peak_estimate\": " << outputTruePeak
        << ", \"max_gain_reduction_db\": " << limiterReduction << "}\n}\n";
    return static_cast<bool>(out);
}

} // namespace

int main(int argc, char** argv) {
    bool writeStems = false;
    bool recombineBassCycles = false;
    bool coupledPendulum = false;
    bool voiceCallResponse = false;
    bool logisticBassDynamics = false;
    bool profile = false;
    bool figures = false;   // camada modelo → figuras (auditoria 12/11, lote 1)
    bool oldDrumBody = false;   // lote 2: corpo de bateria anterior, para A/B
    bool rhythmMode = false;    // modo RITMO: o escrito toca como escrito
    float generativeDepth = 0.3f;   // profundidade do GERATIVO (0–1)
    bool bassPhrase = false;   // baixo com voz própria (auditoria §7, item 5)
    bool keyTuning = false;    // percussão afinada no tom da seed
    int figureModel = 0;       // --modelo: 0 pêndulo … 4 rolantes; -1 = pela seed
    bool snareVocabulary = false;   // --caixa: o vocabulário de caixa da seed na V2, como no app
    int snareIdiom = -1;            // --idioma frevo|maracatu|baiao (ginga da caixa, §4b)
    float ginga = 0.0f;             // --ginga 0..1: microtempo medido (§4c) em V1 (bombo) e V2 (tarol)
    float voiceDecayScale = kVoiceDecayScale;
    std::size_t requestedVoices = 0;
    for (int argument = 3; argument < argc; ++argument) {
        const std::string option = argv[argument];
        if (option == "--stems") {
            writeStems = true;
        } else if (option == "--recombine-bass") {
            recombineBassCycles = true;
        } else if (option == "--coupled-pendulum") {
            coupledPendulum = true;
        } else if (option == "--voice-call-response") {
            voiceCallResponse = true;
        } else if (option == "--logistic-bass") {
            logisticBassDynamics = true;
        } else if (option == "--profile") {
            profile = true;
        } else if (option == "--figuras") {
            figures = true;
        } else if (option == "--corpo-antigo") {
            oldDrumBody = true;
        } else if (option == "--ritmo") {
            rhythmMode = true;
        } else if (option == "--baixo-frase") {
            bassPhrase = true;
        } else if (option == "--afinar") {
            keyTuning = true;
        } else if (option == "--caixa") {
            snareVocabulary = true;
        } else if (option == "--ginga" && argument + 1 < argc) {
            char* end = nullptr;
            const char* rawValue = argv[++argument];
            ginga = std::strtof(rawValue, &end);
            if (end == rawValue || *end != '\0' || !std::isfinite(ginga) || ginga < 0.0f || ginga > 1.0f) {
                std::cerr << "--ginga precisa estar entre 0 e 1.\n";
                return 2;
            }
        } else if (option == "--idioma" && argument + 1 < argc) {
            const std::string name = argv[++argument];
            snareIdiom = name == "frevo" ? 0 : name == "maracatu" ? 1
                : (name == "baiao" || name == "baião") ? 2 : -2;
            if (snareIdiom == -2) {
                std::cerr << "--idioma: frevo, maracatu ou baiao.\n";
                return 2;
            }
        } else if (option == "--modelo" && argument + 1 < argc) {
            const std::string name = argv[++argument];
            static const char* kNames[kFigureModelCount] = {
                "pendulo", "bilhar", "lissajous", "fourier", "rolante"};
            figureModel = name == "seed" ? -1 : -2;
            for (int m = 0; m < kFigureModelCount; ++m)
                if (name == kNames[m]) figureModel = m;
            if (figureModel == -2) {
                std::cerr << "--modelo: pendulo, bilhar, lissajous, fourier, rolante ou seed.\n";
                return 2;
            }
        } else if (option == "--profundidade" && argument + 1 < argc) {
            char* end = nullptr;
            const char* rawValue = argv[++argument];
            const float value = std::strtof(rawValue, &end);
            if (end == rawValue || *end != '\0' || !std::isfinite(value)
                || value < 0.0f || value > 1.0f) {
                std::cerr << "--profundidade precisa estar entre 0 e 1.\n";
                return 2;
            }
            generativeDepth = value;
        } else if (option == "--voices" && argument + 1 < argc) {
            char* end = nullptr;
            const char* rawValue = argv[++argument];
            const auto value = std::strtoul(rawValue, &end, 10);
            if (end == rawValue || *end != '\0' || value < 2
                || value > kMaxVoices) {
                std::cerr << "--voices precisa estar entre 2 e " << kMaxVoices << ".\n";
                return 2;
            }
            requestedVoices = static_cast<std::size_t>(value);
        } else if (option == "--decay-scale" && argument + 1 < argc) {
            char* end = nullptr;
            const char* rawValue = argv[++argument];
            const float value = std::strtof(rawValue, &end);
            if (end == rawValue || *end != '\0' || !std::isfinite(value)
                || value < 0.15f || value > 1.5f) {
                std::cerr << "--decay-scale precisa estar entre 0.15 e 1.5.\n";
                return 2;
            }
            voiceDecayScale = value;
        } else {
            std::cerr << "Uso: batruc_offline_render SEED_HEX arquivo.wav "
                         "[--stems] [--recombine-bass] [--coupled-pendulum] "
                         "[--voice-call-response] [--logistic-bass] "
                         "[--voices 2..12] [--decay-scale VALOR] [--profile] [--figuras] [--corpo-antigo] [--ritmo] [--profundidade 0..1] [--baixo-frase] [--afinar] [--modelo NOME|seed] [--caixa] [--idioma frevo|maracatu|baiao] [--ginga 0..1]\n";
            return 2;
        }
    }
    if (argc < 3) {
        std::cerr << "Uso: batruc_offline_render SEED_HEX arquivo.wav "
                     "[--stems] [--recombine-bass] [--coupled-pendulum] "
                     "[--voice-call-response] [--logistic-bass] "
                     "[--voices 2..12] [--decay-scale VALOR] [--profile] [--figuras] [--corpo-antigo] [--ritmo] [--profundidade 0..1] [--baixo-frase] [--afinar] [--modelo NOME|seed] [--caixa] [--idioma frevo|maracatu|baiao] [--ginga 0..1]\n";
        return 2;
    }
    const std::string seed = argv[1];
    if (seed.size() != 8 || !std::all_of(seed.begin(), seed.end(),
        [](const unsigned char c) { return std::isxdigit(c) != 0; })) {
        std::cerr << "A seed deve conter oito dígitos hexadecimais.\n";
        return 2;
    }
    std::vector<std::string> outputPaths{argv[2], std::string(argv[2]) + ".json"};
    if (writeStems) {
        outputPaths.push_back(stemPath(argv[2], "voices"));
        outputPaths.push_back(stemPath(argv[2], "bass"));
    }
    for (const auto& output : outputPaths) {
        if (std::filesystem::exists(output)) {
            std::cerr << "Saída já existe; nada foi sobrescrito: " << output << "\n";
            return 2;
        }
    }

    CompositionRecipe base = makeInitialRecipe();
    base.sectionSeconds = kSectionSeconds;
    const auto numericSeed = static_cast<std::uint32_t>(std::stoul(seed, nullptr, 16));
    LogisticMapParameters logisticMapParameters;
    logisticMapParameters.initialState = 0.05 + 0.90
        * static_cast<double>(numericSeed)
        / static_cast<double>(std::numeric_limits<std::uint32_t>::max());
    double logisticFinalState = logisticMapParameters.initialState;
    const auto selection = seededRecipeSelection(numericSeed);
    const int bassProfile = bassProfileForSeed(numericSeed);
    const double phaseAmount = static_cast<double>(selection.phaseIndex) * 0.005;
    base.voiceCount = requestedVoices > 0 ? requestedVoices : selection.voiceCount;
    base.patternMode = static_cast<PatternMode>(selection.patternIndex);
    constexpr std::array<double, 12> defaultCycles = {
        3.0, 4.0, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.5, 3.0, 3.5};
    constexpr std::array<double, 10> additionalCycles = {
        0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.5, 3.0, 3.5};
    for (std::size_t voice = 2; voice < base.voiceCount; ++voice) {
        const auto profile = voice - 2;
        auto& recipe = base.voices[voice];
        recipe = base.voices[1];
        recipe.cyclesPerSection = additionalCycles[profile];
        recipe.phaseCycles = std::fmod(0.137 * static_cast<double>(profile + 1), 1.0);
        recipe.resonanceHz = 220.0 + 44.0 * static_cast<double>(profile);
        recipe.gain = 0.34f;
        recipe.pan = -0.82f + 1.64f * static_cast<float>(profile) / 9.0f;
        for (std::size_t mode = 0; mode < kModesPerVoice; ++mode) {
            recipe.resonator.frequencyRatios[mode] = 1.0
                + static_cast<double>(mode) * (1.0 + 0.025 * static_cast<double>(profile));
            recipe.resonator.modeWeights[mode] = 0.86f
                / (1.0f + 0.34f * static_cast<float>(mode));
        }
        recipe.resonator.decaySeconds = 0.44 + 0.09 * static_cast<double>(profile % 6);
        recipe.resonator.frequencyDependentLoss = 0.012;
        recipe.resonator.strikeNoise = 0.035f + 0.012f * static_cast<float>(profile % 5);
    }
    SeedVariationParameters variationParameters;
    variationParameters.seed = hashSeed(seed);
    variationParameters.amount = kVoiceVariation;
    variationParameters.baseTransforms.fill(static_cast<int>(VoiceTransform::sine));
    variationParameters.baseCycles.fill(1.0f);
    variationParameters.baseCouplings.fill(0.28f);
    variationParameters.baseCouplings[0] = 0.0f;
    for (std::size_t voice = 0; voice < base.voiceCount; ++voice) {
        variationParameters.baseCycles[voice] = static_cast<float>(defaultCycles[voice]);
        if (voice == 1)
            variationParameters.baseCycles[voice] = static_cast<float>(selection.voiceTwoCycleIndex + 1u);
        variationParameters.basePhases[voice] = voice == 0 ? 0.0f
            : voice == 1 ? static_cast<float>(phaseAmount)
            : static_cast<float>(static_cast<double>(voice - 1) / 16.0
                + 0.137 * static_cast<double>(voice - 1));
        base.voices[voice].eventRule = selection.eventRuleIndex == 0
            ? EventRule::centreCrossings : EventRule::turningPoints;
    }
    const auto variation = realizeSeedVariation(variationParameters);

    std::array<CompositionRecipe, kFormalCycleCount> recipes{};
    std::array<EventPlan, kFormalCycleCount> plans{};
    std::array<EventPlan, kFormalCycleCount> pendulumPlans{};
    std::array<std::vector<SeedBassVoiceEvent>, kFormalCycleCount> bassSources{};
    CoupledPendulumParameters pendulumParameters;
    pendulumParameters.durationSeconds = kSectionSeconds * kFormalCycleCount;
    const auto seededUnit = [&seed](const std::string& suffix) {
        return static_cast<double>(hashSeed(seed + suffix))
            / static_cast<double>(std::numeric_limits<std::uint32_t>::max());
    };
    pendulumParameters.lengthMetres = 0.65 + 0.70 * seededUnit("-pendulum-length");
    pendulumParameters.massKilograms = 0.75 + 0.50 * seededUnit("-pendulum-mass");
    pendulumParameters.springConstantNewtonsPerMetre = 0.45
        + 0.65 * seededUnit("-pendulum-spring");
    pendulumParameters.dampingPerSecond = 0.025
        + 0.045 * seededUnit("-pendulum-damping");
    for (std::size_t voice = 0; voice < 3; ++voice) {
        const auto suffix = std::to_string(voice);
        pendulumParameters.initialAnglesRadians[voice] =
            (2.0 * seededUnit("-pendulum-angle-" + suffix) - 1.0) * 0.27;
        pendulumParameters.initialVelocitiesRadiansPerSecond[voice] =
            (2.0 * seededUnit("-pendulum-velocity-" + suffix) - 1.0) * 0.16;
    }
    if (coupledPendulum) {
        CoupledPendulumTrajectory trajectory;
        const auto trajectoryError = buildCoupledPendulumTrajectory(
            pendulumParameters, trajectory);
        const auto rule = selection.eventRuleIndex == 0
            ? EventRule::centreCrossings : EventRule::turningPoints;
        const auto planError = trajectoryError == PendulumError::none
            ? buildPendulumEventPlan(trajectory, kSectionSeconds, rule,
                                     pendulumPlans)
            : trajectoryError;
        if (planError != PendulumError::none) {
            std::cerr << "Falha ao preparar a composição do pêndulo acoplado ("
                      << static_cast<int>(planError) << ").\n";
            return 3;
        }
    }
    FigureComposition figureComposition;
    if (figures) {
        FigureComposerParameters figureParameters;
        figureParameters.seed = numericSeed;
        figureParameters.voiceCount = std::min<std::size_t>(3, base.voiceCount);
        figureParameters.sectionSeconds = kSectionSeconds;
        if (figureModel >= 0) {
            figureParameters.model = static_cast<FigureModel>(figureModel);
        } else {   // a seed escolhe o modelo (gene próprio, "MDL1")
            std::uint32_t modelState = numericSeed ^ 0x4D444C31u;
            figureParameters.model = static_cast<FigureModel>(
                nextSeedWord(modelState) % static_cast<std::uint32_t>(kFigureModelCount));
        }
        if (!composeFigures(figureParameters, figureComposition)) {
            std::cerr << "Falha ao compor as figuras do modelo.\n";
            return 3;
        }
        std::cerr << "figuras: " << figureComposition.summary << "\n";
        if (ginga > 0.0f && figureParameters.voiceCount >= 2) {
            // microtempo medido (ISMIR 2020): bombo em V1, tarol em V2 (16 tempos)
            int moved = 0;
            for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle) {
                moved += applyMicrotimingProfile(figureComposition.cells[cycle][0],
                    MicrotimingProfile::bomboBaqueSolto, 16, ginga);
                moved += applyMicrotimingProfile(figureComposition.cells[cycle][1],
                    MicrotimingProfile::tarolBaqueSolto, 16, ginga);
            }
            std::cerr << "ginga: microtempo " << ginga << " (" << moved << " ataques deslocados)\n";
        }
        if (snareIdiom >= 0 && figureParameters.voiceCount >= 2) {
            // ginga da caixa por gênero (V2 de cada ciclo), já expandida
            for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle) {
                auto& cell = figureComposition.cells[cycle][1];
                applySnareIdiom(cell, static_cast<SnareIdiom>(snareIdiom), 4, 4, 4,
                                numericSeed ^ static_cast<std::uint32_t>(0x49444900u + cycle));
                RhythmicCell performed;
                expandSnareOrnaments(cell, performed, kSectionSeconds, 64);
                cell = performed;
            }
            std::cerr << "caixa: idioma " << snareIdiomName(static_cast<SnareIdiom>(snareIdiom)) << "\n";
        }
        if (snareVocabulary && figureParameters.voiceCount >= 2) {
            // como no app: marca a V2 de cada ciclo e expande os enfeites
            SnareOrnament kinds[2];
            float amount = 0.0f;
            const int vocabulary = seedSnareVocabulary(numericSeed, kinds, amount);
            if (vocabulary > 0)
                for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle) {
                    auto& cell = figureComposition.cells[cycle][1];
                    SnareOrnamentParameters p;
                    p.sectionSeconds = kSectionSeconds;
                    p.stepsPerSection = 64;
                    p.amount = amount;
                    p.kind = kinds[0];
                    p.seed = numericSeed ^ static_cast<std::uint32_t>(0x4F524E00u + cycle);
                    markSnareOrnaments(cell, p);
                    if (kinds[1] != kinds[0]) {
                        p.kind = kinds[1];
                        p.seed ^= 0x2Au;
                        markSnareOrnaments(cell, p);
                    }
                    RhythmicCell performed;
                    expandSnareOrnaments(cell, performed, kSectionSeconds, 64);
                    cell = performed;
                }
            std::cerr << "caixa: vocabulário " << vocabulary << " ("
                      << snareOrnamentName(kinds[0]) << " + " << snareOrnamentName(kinds[1])
                      << ", " << static_cast<int>(amount * 100.0f + 0.5f) << "%)\n";
        }
        // as figuras já são a decisão rítmica: o filtro de campo antigo não
        // as recorta na hora de tocar
        base.patternMode = PatternMode::unrestricted;
        for (auto& recipe : recipes) recipe.patternMode = PatternMode::unrestricted;
    }
    constexpr std::array<double, 7> formalStarts = {0, 0, 2, 2, 4, 5, 5};
    constexpr double sectionQuarterNotes = 16.0;
    for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle) {
        recipes[cycle] = base;
        for (std::size_t voice = 1; voice < base.voiceCount; ++voice) {
            const auto segment = formalPhaseSegmentForCycle(cycle, phaseAmount);
            recipes[cycle].voices[voice].cyclesPerSection = variation.cycles[voice]
                + segment.slopeCycles;
            recipes[cycle].voices[voice].phaseCycles = variation.phases[voice]
                + segment.offsetCycles;
        }
        recipes[cycle].voices[0].cyclesPerSection = variation.cycles[0];
        recipes[cycle].voices[0].phaseCycles = variation.phases[0];
        if (figures) {
            for (std::size_t voice = 0; voice < base.voiceCount && voice < 3; ++voice)
                recipes[cycle].rhythmicCells[voice] = figureComposition.cells[cycle][voice];
        }
        if (buildEventPlan(recipes[cycle], plans[cycle]) != PlanError::none) {
            std::cerr << "Falha ao preparar o plano de eventos.\n";
            return 3;
        }
        if (coupledPendulum) {
            EventPlan combined;
            combined.events.reserve(plans[cycle].events.size()
                + pendulumPlans[cycle].events.size());
            for (const auto& event : plans[cycle].events)
                if (event.voice >= 3)
                    combined.events.push_back(event);
            const auto activeVoiceCount = activeVoiceCountInCycle(
                cycle, base.voiceCount);
            for (const auto& event : pendulumPlans[cycle].events)
                if (event.voice < activeVoiceCount)
                    combined.events.push_back(event);
            std::stable_sort(combined.events.begin(), combined.events.end(),
                [](const MusicalEvent& left, const MusicalEvent& right) {
                    if (left.timeSeconds != right.timeSeconds)
                        return left.timeSeconds < right.timeSeconds;
                    return left.voice < right.voice;
                });
            plans[cycle] = std::move(combined);
        }
    }
    if (voiceCallResponse) {
        CompositionRecipe phraseRecipe = base;
        phraseRecipe.sectionSeconds = kSectionSeconds * kFormalCycleCount;
        EventPlan phrase;
        EventPlan eligibleCalls;
        // Select across the whole form; omit calls whose delayed answer lands
        // in a cycle where the target voice is formally absent.
        for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle) {
            for (auto event : plans[cycle].events) {
                event.timeSeconds += static_cast<double>(cycle) * kSectionSeconds;
                const double responseTime = std::fmod(event.timeSeconds
                    + kSectionSeconds / 16.0,
                    phraseRecipe.sectionSeconds);
                const auto responseCycle = std::min(kFormalCycleCount - 1,
                    static_cast<std::size_t>(responseTime / kSectionSeconds));
                if (event.voice == 0
                    && activeVoiceCountInCycle(responseCycle, base.voiceCount) > 1)
                    eligibleCalls.events.push_back(event);
                phrase.events.push_back(event);
            }
        }
        std::stable_sort(phrase.events.begin(), phrase.events.end(),
            [](const MusicalEvent& left, const MusicalEvent& right) {
                if (left.timeSeconds != right.timeSeconds)
                    return left.timeSeconds < right.timeSeconds;
                return left.voice < right.voice;
            });
        std::array<VoiceRelationRule, kMaxVoices> relations{};
        relations[1] = {EcologicalRelation::callResponse, 0, 0.42f,
            0.0, kSectionSeconds / 16.0, numericSeed};
        EventPlan related;
        const auto relationError = buildEcologicalEventPlan(phraseRecipe,
            eligibleCalls, relations, related);
        if (relationError != EcologicalPlanError::none) {
            std::cerr << "Falha ao aplicar a relação de chamada-resposta ("
                      << static_cast<int>(relationError) << ").\\n";
            return 3;
        }
        for (const auto& event : related.events)
            if (event.generatedByRelation)
                phrase.events.push_back(event);
        std::stable_sort(phrase.events.begin(), phrase.events.end(),
            [](const MusicalEvent& left, const MusicalEvent& right) {
                if (left.timeSeconds != right.timeSeconds)
                    return left.timeSeconds < right.timeSeconds;
                return left.voice < right.voice;
            });
        for (auto& plan : plans)
            plan.events.clear();
        for (auto event : phrase.events) {
            const auto cycle = std::min(kFormalCycleCount - 1,
                static_cast<std::size_t>(event.timeSeconds / kSectionSeconds));
            event.timeSeconds -= static_cast<double>(cycle) * kSectionSeconds;
            plans[cycle].events.push_back(event);
        }
        for (auto& plan : plans)
            std::stable_sort(plan.events.begin(), plan.events.end(),
                [](const MusicalEvent& left, const MusicalEvent& right) {
                    if (left.timeSeconds != right.timeSeconds)
                        return left.timeSeconds < right.timeSeconds;
                    return left.voice < right.voice;
                });
    }
    for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle) {
        for (const auto& event : plans[cycle].events) {
            const double progress = event.timeSeconds / kSectionSeconds;
            const double performed = swingProgress(progress, kSwing, sectionQuarterNotes);
            const double elapsedSinceFormalStart = (static_cast<double>(cycle) + performed
                - formalStarts[cycle]) * kSectionSeconds;
            const double voiceGain = event.voice == 0 ? 0.76
                : 0.63 / std::sqrt(static_cast<double>(base.voiceCount - 1));
            const double expression = expressionAtProgress(
                (static_cast<double>(cycle) + progress) / 7.0,
                DynamicCurve::arch, MusicalIntention::meet);
            const auto activeVoiceCount = activeVoiceCountInCycle(cycle, base.voiceCount);
            if (event.voice >= activeVoiceCount) continue;
            const float intensityFormula = intensityFormulaValue(
                static_cast<IntensityLaw>(selection.intensityLawIndex),
                event.fieldX, event.fieldY, variation.cycles[0], variation.cycles[1]);
            const double seededIntensity = 1.0 - selection.intensityAmount
                + selection.intensityAmount * intensityFormula;
            const double intensity = event.intensity * voiceGain
                * std::exp(-0.065 * elapsedSinceFormalStart)
                * seededIntensity * expression
                * grooveAccentAtProgress(performed, GrooveMode::straight, 4, 4);
            const float bassFieldX = coupledPendulum ? event.fieldX
                : transformedVoiceValue(0, progress, cycle, phaseAmount, variation);
            const float bassFieldY = coupledPendulum ? event.fieldY
                : transformedVoiceValue(1, progress, cycle, phaseAmount, variation);
            bassSources[cycle].push_back({performed * kSectionSeconds,
                bassFieldX, bassFieldY,
                static_cast<float>(intensity), event.voice});
        }
        std::stable_sort(bassSources[cycle].begin(), bassSources[cycle].end(),
            [](const auto& left, const auto& right) {
                if (left.voice != right.voice) return left.voice < right.voice;
                return left.localTimeSeconds < right.localTimeSeconds;
            });
    }

    ResonatorRenderer voices;
    if (!voices.configureVariants(recipes.data(), plans.data(), kFormalCycleCount, kSampleRate)) {
        std::cerr << "Falha ao preparar o renderizador de vozes.\n";
        return 4;
    }
    voices.setDecayScale(voiceDecayScale);
    voices.setFundamentalLevel(kVoiceFundamentalLevel);
    voices.setSwingRatio(kSwing);
    voices.setGrooveMode(GrooveMode::straight);
    voices.setPerformanceExpression(DynamicCurve::arch, MusicalIntention::meet);
    voices.setTimeSignature(4, 4);
    voices.setIntensityFormula(static_cast<IntensityLaw>(selection.intensityLawIndex),
                               selection.intensityAmount);
    std::array<float, kMaxVoices> referencePans{};
    for (std::size_t voice = 0; voice < base.voiceCount; ++voice) {
        referencePans[voice] = base.voiceCount < 2 ? 0.0f
            : -0.65f + 1.3f * static_cast<float>(voice)
                / static_cast<float>(base.voiceCount - 1);
    }
    voices.setChannelPans(referencePans.data());
    const auto familySounds = soundFamilyVoiceSounds(
        static_cast<int>(selection.soundFamilyIndex));
    for (std::size_t voice = 0; voice < base.voiceCount; ++voice) {
        voices.setChannelSound(voice, familySounds[voice]);
        voices.setChannelTimbreProfile(voice,
            static_cast<int>(selection.soundFamilyIndex));
    }
    if (figures) {   // o papel de cada voz da camada de figuras: kit
        static constexpr SoundCharacter kKit[3] = {
            SoundCharacter::kick, SoundCharacter::snare, SoundCharacter::hihat};
        for (std::size_t voice = 0; voice < base.voiceCount && voice < 3; ++voice)
            voices.setChannelSound(voice, kKit[voice]);
    }
    voices.setModernDrumBody(!oldDrumBody);
    voices.setRhythmMode(rhythmMode);
    voices.setGenerativeDepth(generativeDepth);
    if (keyTuning) {
        int tonic = 0, scale = 0;
        seedKey(numericSeed, tonic, scale);
        voices.setPercussionKey(tonic, scale);
        std::cerr << "percussão no tom: " << bassKeyTonicName(tonic) << " "
                  << bassKeyScaleName(scale) << "\n";
    }
    std::array<VoiceTransform, kMaxVoices> voiceTransforms{};
    std::array<float, kMaxVoices> voiceCouplings{};
    std::array<float, kMaxVoices> formulaPhaseOffsets{};
    for (std::size_t voice = 0; voice < kMaxVoices; ++voice) {
        voiceTransforms[voice] = static_cast<VoiceTransform>(variation.transforms[voice]);
        voiceCouplings[voice] = variation.couplings[voice];
    }
    voices.setVoiceFormulaData(voiceTransforms.data(), voiceCouplings.data(),
                               formulaPhaseOffsets.data());

    SeedBassScheduleParameters bassSchedule;
    bassSchedule.bpm = kBpm;
    bassSchedule.sectionSeconds = kSectionSeconds;
    bassSchedule.seedHash = hashSeed(seed);
    bassSchedule.pitchVariation = std::max(kVoiceVariation, kBassVariation);
    bassSchedule.phraseMode = BassPhraseMode::loop;
    bassSchedule.relation = BassRhythmRelation::follow;
    bassSchedule.groove = GrooveMode::straight;
    bassSchedule.dynamics = DynamicCurve::arch;
    bassSchedule.intention = MusicalIntention::meet;
    bassSchedule.durationLaw = BassDurationLaw::manual;
    bassSchedule.manualDurationSteps.fill(2);
    BassPerformance bassPerformance;
    if (buildSeedBassPerformance(bassSources, bassSchedule, bassPerformance) != BassError::none) {
        std::cerr << "Falha ao preparar a linha de baixo.\n";
        return 5;
    }
    if (bassPhrase) {   // o baixo escreve a própria frase sobre as figuras
        if (!figures) {
            std::cerr << "--baixo-frase precisa de --figuras.\n";
            return 2;
        }
        BassLineParameters line;
        line.seed = numericSeed;
        line.sectionSeconds = kSectionSeconds;
        seedKey(numericSeed, line.keyTonic, line.keyScale);
        std::string lineSummary;
        if (!composeBassLine(figureComposition, line, bassPerformance, &lineSummary)) {
            std::cerr << "Falha ao compor a frase do baixo.\n";
            return 5;
        }
        std::cerr << lineSummary << "\n";
    }
    constexpr std::size_t stepsPerBar = bassStepsPerBar(4, 4);
    constexpr std::size_t stepsPerSection = stepsPerBar * kBassBarsPerSection;
    constexpr double stepSeconds = 15.0 / kBpm;
    for (std::size_t index = 0; index < bassPerformance.count; ++index) {
        auto& event = bassPerformance.events[index];
        const double absoluteTime = static_cast<double>(event.formCycle)
            * kSectionSeconds + event.timeSeconds;
        double nextAbsoluteTime = kSectionSeconds * kFormalCycleCount;
        for (std::size_t next = 0; next < bassPerformance.count; ++next) {
            if (next == index) continue;
            const auto& candidate = bassPerformance.events[next];
            const double candidateTime = static_cast<double>(candidate.formCycle)
                * kSectionSeconds + candidate.timeSeconds;
            if (candidateTime >= absoluteTime - 1.0e-9
                && candidateTime < nextAbsoluteTime)
                nextAbsoluteTime = candidateTime;
        }
        event.durationSeconds = std::max(0.025,
            std::min(event.durationSeconds, nextAbsoluteTime - absoluteTime));
        const auto firstStep = static_cast<std::size_t>(event.formCycle)
            * stepsPerSection + event.sourceNote + 1;
        const auto lastStep = static_cast<std::size_t>(std::ceil(
            (absoluteTime + event.durationSeconds) / stepSeconds));
        for (std::size_t step = firstStep; step <= lastStep; ++step) {
            const auto bar = step / stepsPerBar;
            const auto stepInBar = step % stepsPerBar;
            const bool phraseRest = bassSchedule.phraseMode == BassPhraseMode::breath
                    ? bar % 2 == 1 && stepInBar >= stepsPerBar - 2
                : bassSchedule.phraseMode == BassPhraseMode::cadence
                    ? bar % 4 == 3 && stepInBar >= stepsPerBar - 4
                : bassSchedule.phraseMode == BassPhraseMode::callResponse
                    ? bar % 2 == 1 : false;
            if (!phraseRest) continue;
            event.durationSeconds = std::max(0.025, std::min(
                event.durationSeconds, step * stepSeconds - absoluteTime));
            break;
        }
        event.durationSeconds = std::min(event.durationSeconds,
            kSectionSeconds - event.timeSeconds);
    }
    std::size_t bassDonorOffset = 0;
    if (recombineBassCycles) {
        bassDonorOffset = 1u + static_cast<std::size_t>(numericSeed % 6u);
        BassPerformance recombined;
        if (!recombineBassPerformanceBars(bassPerformance, bassDonorOffset,
                                           recombined)) {
            std::cerr << "Não foi possível recombinar os ciclos do baixo.\n";
            return 5;
        }
        bassPerformance = recombined;
    }
    if (logisticBassDynamics
        && !applyLogisticBassDynamics(bassPerformance, logisticMapParameters,
                                      &logisticFinalState)) {
        std::cerr << "Não foi possível aplicar a dinâmica do mapa logístico.\n";
        return 5;
    }
    BassAudioRenderer bass;
    if (!bass.configure(bassPerformance, kSampleRate)) {
        std::cerr << "Falha ao preparar o renderizador de baixo.\n";
        return 6;
    }
    BassAudioParameters bassParameters;
    bassParameters.gain = 0.20f;
    bassParameters.cutoffHz = 420.0f;
    bassParameters.filterEnvelopeAmountHz = 900.0f;
    bassParameters.oscillatorProfile = static_cast<BassOscillatorProfile>(bassProfile);

    const auto sectionFrames = static_cast<std::size_t>(std::llround(kSectionSeconds * kSampleRate));
    StereoLookaheadGuard outputGuard;
    outputGuard.prepare(kSampleRate);
    std::vector<float> interleaved;
    interleaved.reserve(sectionFrames * kFormalCycleCount * 2u);
    std::vector<float> voiceStem;
    std::vector<float> bassStem;
    if (writeStems) {
        voiceStem.reserve(sectionFrames * kFormalCycleCount * 2u);
        bassStem.reserve(sectionFrames * kFormalCycleCount * 2u);
    }
    std::array<float, kBlock> voiceLeft{}, voiceRight{}, bassLeft{}, bassRight{};
    using ProfileClock = std::chrono::steady_clock;
    double voiceRenderSeconds = 0.0;
    double bassRenderSeconds = 0.0;
    double mixAndGuardSeconds = 0.0;
    for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle) {
        voices.setActiveVoiceCount(activeVoiceCountInCycle(cycle, base.voiceCount));
        std::size_t done = 0;
        while (done < sectionFrames) {
            const auto frames = std::min(kBlock, sectionFrames - done);
            auto stageStart = ProfileClock::time_point{};
            if (profile)
                stageStart = ProfileClock::now();
            voices.processVariant(voiceLeft.data(), voiceRight.data(), frames, cycle);
            if (profile) {
                const auto stageEnd = ProfileClock::now();
                voiceRenderSeconds += std::chrono::duration<double>(
                    stageEnd - stageStart).count();
                stageStart = stageEnd;
            }
            const double progress = static_cast<double>(done) / sectionFrames;
            // BassAudioRenderer adds into host-owned output buffers. Clear each
            // block, as the JUCE device callback does, instead of retaining and
            // re-adding the preceding block on every iteration.
            std::fill_n(bassLeft.data(), frames, 0.0f);
            std::fill_n(bassRight.data(), frames, 0.0f);
            if (profile)
                stageStart = ProfileClock::now();
            bass.process(bassLeft.data(), bassRight.data(), frames, progress,
                kSectionSeconds, cycle, true, bassParameters);
            if (profile) {
                const auto stageEnd = ProfileClock::now();
                bassRenderSeconds += std::chrono::duration<double>(
                    stageEnd - stageStart).count();
                stageStart = stageEnd;
            }
            for (std::size_t frame = 0; frame < frames; ++frame) {
                if (writeStems) {
                    voiceStem.push_back(voiceLeft[frame] * kFormulaBusGain);
                    voiceStem.push_back(voiceRight[frame] * kFormulaBusGain);
                    bassStem.push_back(bassLeft[frame]);
                    bassStem.push_back(bassRight[frame]);
                }
                float left = (voiceLeft[frame] * kFormulaBusGain
                    + bassLeft[frame]) * kMasterGain;
                float right = (voiceRight[frame] * kFormulaBusGain
                    + bassRight[frame]) * kMasterGain;
                outputGuard.process(left, right);
                interleaved.push_back(left);
                interleaved.push_back(right);
            }
            if (profile)
                mixAndGuardSeconds += std::chrono::duration<double>(
                    ProfileClock::now() - stageStart).count();
            done += frames;
        }
    }
    // Emit the guard's delayed end of the final active block, as the JUCE
    // transport does after Stop. No new synthesis events are generated here.
    for (std::size_t frame = 0; frame < outputGuard.latencySamples(); ++frame) {
        float left = 0.0f, right = 0.0f;
        outputGuard.process(left, right);
        interleaved.push_back(left);
        interleaved.push_back(right);
        if (writeStems) {
            voiceStem.push_back(0.0f); voiceStem.push_back(0.0f);
            bassStem.push_back(0.0f); bassStem.push_back(0.0f);
        }
    }
    if (!writeWav(argv[2], interleaved)) {
        std::cerr << "Não foi possível escrever o WAV.\n";
        return 7;
    }
    if (writeStems && (!writeWav(stemPath(argv[2], "voices"), voiceStem)
        || !writeWav(stemPath(argv[2], "bass"), bassStem))) {
        std::cerr << "Não foi possível escrever os stems separados.\n";
        return 7;
    }
    std::array<std::size_t, kFormalCycleCount> voiceEventCounts{};
    for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle)
        voiceEventCounts[cycle] = plans[cycle].events.size();
    if (!writeRecipe(argv[2], seed, bassPerformance.count, voiceEventCounts,
                     recipes, plans, bassSources, bassPerformance,
                     recombineBassCycles, bassDonorOffset,
                     coupledPendulum, voiceCallResponse, logisticBassDynamics,
                     logisticMapParameters, logisticFinalState, pendulumParameters,
                     base.voiceCount, phaseAmount, selection, bassProfile, variation,
                     referencePans, voiceDecayScale, outputGuard.outputTruePeak(),
                     outputGuard.reductionDb())) {
        std::cerr << "WAV criado, mas não foi possível gravar a receita JSON.\n";
        return 8;
    }
    std::cout << "WAV offline: " << argv[2] << " · seed " << seed
              << " · 7 ciclos + cauda 3 ms · 48 kHz estéreo · " << base.voiceCount
              << " vozes seeded + baixo"
              << (recombineBassCycles ? " · baixo recombinado Δ"
                    + std::to_string(bassDonorOffset) : std::string{})
              << (coupledPendulum ? " · pêndulo triplo acoplado" : std::string{})
              << (logisticBassDynamics ? " · mapa logístico no baixo" : std::string{})
              << (writeStems ? " · stems exportados\n" : "\n");
    if (profile) {
        const double measured = voiceRenderSeconds + bassRenderSeconds
            + mixAndGuardSeconds;
        const double duration = kSectionSeconds * kFormalCycleCount;
        std::cout << std::fixed << std::setprecision(3)
                  << "Perfil core (s): vozes=" << voiceRenderSeconds
                  << " baixo=" << bassRenderSeconds
                  << " mix/guard=" << mixAndGuardSeconds
                  << " soma=" << measured << " · fator tempo real="
                  << (measured > 0.0 ? duration / measured : 0.0) << "x\n";
    }
    return 0;
}
