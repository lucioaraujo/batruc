// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#pragma once

#include "core/BassComposition.hpp"

#include <array>
#include <cstddef>

namespace batruc {

enum class BassOscillatorProfile : std::uint8_t {
    harmonic, oddPartials, inharmonic, slowBeat
};
enum class BassMaterial : std::uint8_t { sub, round, body, pulse, bright, pluck };
constexpr std::size_t kMaxSimultaneousBassNotes = 8;

struct BassAudioParameters {
    float gain = 0.20f;
    float channelPan = 0.0f;
    float gateRatio = 0.86f;
    float cutoffHz = 420.0f;
    float filterResonance = 0.707f;
    float filterEnvelopeAmountHz = 900.0f;
    float attackSeconds = 0.008f;
    float decaySeconds = 0.18f;
    float sustain = 0.62f;
    float releaseSeconds = 0.12f;
    float filterAttackSeconds = 0.006f;
    float filterDecaySeconds = 0.243f;
    float filterSustain = 0.12f;
    float filterReleaseSeconds = 0.12f;
    BassOscillatorProfile oscillatorProfile = BassOscillatorProfile::harmonic;
    BassMaterial material = BassMaterial::round;
    bool lfoEnabled = false;
    BassLfoShape lfoShape = BassLfoShape::triangle;
    BassLfoTarget lfoTarget = BassLfoTarget::pitch;
    std::uint8_t lfoPeriodCycles = 2;
    std::uint8_t lfoDepthSemitones = 0;
};

// Saneamento antes de publicar (12/11/2026; relato do autor: "essa linha de
// baixo que inicia é sempre a mesma"). `BassAudioRenderer::configure` recusa
// a linha INTEIRA se um único evento sair das faixas, e o host continuava com
// a linha anterior — a fixa da abertura. Metade das seeds tinha alguma nota
// com intensidade > 1 (soma de acentos). Isto leva cada evento às faixas que
// o renderer exige (intensidade 0–1, pan ±0,24, MIDI 24–60, duração dentro
// da seção, ordem no tempo) e descarta só o evento impossível.
void sanitizeBassPerformance(BassPerformance& performance) noexcept;

// A bounded bass renderer with independent envelopes and filters per note.
// Preparation copies the performance; process() performs no allocation,
// locking, I/O, or schedule build.
class BassAudioRenderer final {
public:
    bool configure(const BassPerformance& performance, double sampleRate,
                   bool deferUntilBar = false) noexcept;
    void reset() noexcept;
    void release() noexcept;
    void setDevelopmentSeed(std::uint32_t seed, bool enabled) noexcept {
        developmentSeed_ = seed;
        developmentEnabled_ = enabled;
    }

    void process(float* left, float* right, std::size_t frames,
                 double sectionProgress, double sectionSeconds,
                 std::uint64_t formCycleIndex, bool enabled,
                 const BassAudioParameters& parameters) noexcept;

private:
    enum class Stage : unsigned char { off, attack, decay, sustain, release };
    struct VoiceState {
        std::array<double, 3> phases{};
        double gateEnd = 0.0;
        double pluckDecaySeconds = 0.12;
        double fundamentalHz = 0.0;
        double ampStagePosition = 0.0;
        double filterStagePosition = 0.0;
        double filterX1 = 0.0, filterX2 = 0.0, filterY1 = 0.0, filterY2 = 0.0;
        double filterB0 = 0.0, filterB1 = 0.0, filterB2 = 0.0;
        double filterA1 = 0.0, filterA2 = 0.0;
        double lfoPitchRatio = 1.0;
        double targetLfoPitchRatio = 1.0;
        float lfoValue = 0.0f;
        float targetLfoValue = 0.0f;
        float pluckEnvelope = 1.0f;
        float pluckEnvelopeStep = 1.0f;
        std::uint64_t age = 0;
        int currentMidi = -1;
        float velocity = 0.0f;
        float pan = 0.0f;
        float eventCutoffScale = 1.0f;
        float ampLevel = 0.0f, ampAttackStart = 0.0f;
        float filterLevel = 0.0f, filterAttackStart = 0.0f;
        float ampReleaseStart = 0.0f, ampReleaseMultiplier = 0.0f;
        float filterReleaseStart = 0.0f;
        float lastOutput = 0.0f, stealResidual = 0.0f;
        unsigned filterCoefficientCountdown = 0;
        unsigned lfoCountdown = 0;
        float lfoValueStep = 0.0f;
        double lfoPitchRatioStep = 0.0;
        unsigned stealFadeRemaining = 0, stealFadeTotal = 0;
        Stage ampStage = Stage::off, filterStage = Stage::off;
    };

    void noteOn(VoiceState& voice, int midi, float velocity,
                double eventDurationSeconds, float dynamicsMapState,
                float pan) noexcept;
    void noteOff(VoiceState& voice, float releaseSeconds) noexcept;
    void releaseAll(float releaseSeconds) noexcept;
    void applyPendingPerformance(double positionSeconds,
                                 double sectionSeconds,
                                 float releaseSeconds) noexcept;
    float nextSample(VoiceState& voice, double formProgress,
                     const BassAudioParameters& parameters) noexcept;

    BassPerformance performance_{};
    BassPerformance pendingPerformance_{};
    std::array<VoiceState, kMaxSimultaneousBassNotes> voices_{};
    double sampleRate_ = 0.0;
    double previousPosition_ = -1.0;
    std::size_t nextEvent_ = 0;
    std::uint64_t voiceAge_ = 0;
    bool enabledPrevious_ = false;
    bool hasPendingPerformance_ = false;
    bool ready_ = false;
    std::uint32_t developmentSeed_ = 0;
    bool developmentEnabled_ = false;
};

} // namespace batruc
