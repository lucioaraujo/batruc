// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace batruc {

enum class GrooveMode : std::uint8_t;
enum class DynamicCurve : std::uint8_t;
enum class MusicalIntention : std::uint8_t;

constexpr std::size_t kBassStepsPerBar = 20;
constexpr std::size_t kBassRelationVoiceCount = 12;
constexpr std::size_t kBassBarsPerSection = 4;
constexpr std::size_t kMaxBassNotes = kBassStepsPerBar * kBassBarsPerSection;
constexpr std::size_t kMaxBassPerformedEvents = kMaxBassNotes * 8;

constexpr std::size_t bassStepsPerBar(const int numerator,
                                     const int denominator) noexcept {
    return denominator > 0
        ? static_cast<std::size_t>((numerator * 16 + denominator / 2) / denominator)
        : 16;
}

enum class BassCellKind : std::uint8_t { empty, note, rest };
enum class BassDurationLaw : std::uint8_t { manual, fibonacci, geometric };
enum class BassPhraseMode : std::uint8_t { loop, breath, cadence, callResponse };
enum class BassIntent : std::uint8_t { normal, ghost, accent };
enum class BassRhythmRelation : std::uint8_t {
    independent, follow, answer, convolution,
    ecologicalSupport, ecologicalImitation, ecologicalAvoidance,
    ecologicalCompetition
};
enum class BassLfoShape : std::uint8_t { triangle, sine, square };
enum class BassLfoTarget : std::uint8_t { pitch, filter, amplitude };
enum class BassError : std::uint8_t { none, invalidRecipe, eventLimitExceeded };

struct BassCell {
    BassCellKind kind = BassCellKind::empty;
    std::uint8_t midi = 36;
    std::uint8_t durationSteps = 2;
    std::uint8_t repeats = 1;
    BassIntent intent = BassIntent::normal;
};

struct BassRecipe {
    double bpm = 120.0;
    std::uint8_t meterNumerator = 4;
    std::uint8_t meterDenominator = 4;
    BassDurationLaw durationLaw = BassDurationLaw::manual;
    BassPhraseMode phraseMode = BassPhraseMode::loop;
    double geometricRatio = 1.5;
    std::uint32_t seed = 0;
    // 0 keeps every admitted rhythmic anchor; 1–3 progressively vary the
    // bass response while the percussion event calendar stays untouched.
    std::uint8_t variationLevel = 2;
    float pitchVariation = 0.0f;
    bool lfoEnabled = false;
    BassLfoShape lfoShape = BassLfoShape::triangle;
    BassLfoTarget lfoTarget = BassLfoTarget::pitch;
    std::uint8_t lfoPeriodCycles = 2;
    std::uint8_t lfoDepthSemitones = 0;
    float pathMemory = 0.72f;
    float pathBreath = 1.0f;
    std::array<BassCell, kBassStepsPerBar> cells{};
};

struct BassNote {
    double timeSeconds = 0.0;
    double durationSeconds = 0.0;
    std::uint8_t writtenMidi = 36;
    std::uint8_t calculatedMidi = 36;
    std::uint8_t bar = 0;
    std::uint8_t step = 0;
    std::uint8_t durationSteps = 1;
    std::uint8_t repeats = 1;
    BassIntent intent = BassIntent::normal;
};

struct BassScore {
    std::array<BassNote, kMaxBassNotes> notes{};
    std::size_t count = 0;
    std::uint8_t stepsPerBar = 16;
    double sectionSeconds = 0.0;
};

struct BassInterpretation {
    // An offbeat is delayed within its pair of sixteenth-note slots.
    float swing = 0.0f;
    float velocity = 0.7f;
};

struct BassPlayedEvent {
    double timeSeconds = 0.0;
    double durationSeconds = 0.0;
    std::uint8_t midi = 36;
    float velocity = 0.7f;
    std::uint16_t sourceNote = 0;
    // 0–6 selects a formal cycle; 7 keeps legacy/manual events in every cycle.
    std::uint8_t formCycle = 7;
    // Optional event-level trace for mathematical articulation; -1 means unused.
    float dynamicsMapState = -1.0f;
    float dynamicsMapScale = 1.0f;
    float pan = 0.0f;
};

inline bool bassEventsCollideWithinSameCycle(const BassPlayedEvent& previous,
                                             const BassPlayedEvent& candidate,
                                             const double tolerance) noexcept {
    return previous.formCycle == candidate.formCycle
        && candidate.timeSeconds >= previous.timeSeconds
        && candidate.timeSeconds - previous.timeSeconds < tolerance;
}

struct BassPerformance {
    std::array<BassPlayedEvent, kMaxBassPerformedEvents> events{};
    std::size_t count = 0;
    double sectionSeconds = 0.0;
    // Published generative plan: the interface and the audio consume the same
    // seven-cycle calculation instead of maintaining a decorative duplicate.
    std::array<int, 7> pitchPath{};
    std::array<float, 7> pathIntegral{};
    std::array<float, 7> pathDerivative{};
    std::array<float, 7> pathCurvature{};
    std::array<float, 7> pathCorrelation{};
    std::array<float, 7> pathRegressionYFromX{};
    std::array<float, 7> pathSpeed{};
    std::array<float, 7> pathSignedCurvature{};
    std::array<std::uint8_t, 7> pathRoute{};
    std::array<float, 7> pathDensity{};
};

struct SeedBassVoiceEvent {
    double localTimeSeconds = 0.0;
    double x = 0.0;
    double y = 0.0;
    float intensity = 1.0f;
    std::uint8_t voice = 0;
    float pathSpeed = 0.0f;
    float pathSignedCurvature = 0.0f;
};

// TOM DA SEED (12/11/2026; o autor: "necessário que seja em todos os tons,
// variar os tons na seed"). Antes as alturas do baixo eram uma fórmula
// contínua arredondada ao semitom: cromáticas, sem tônica nem escala. Agora a
// seed escolhe uma das 12 tônicas e um de 10 modos, e cada nota vai ao grau
// mais próximo. tonic < 0 = cromático (o comportamento anterior).
constexpr int kBassKeyScaleCount = 10;
// graus em semitons a partir da tônica; -1 encerra
inline constexpr std::array<std::array<int, 8>, kBassKeyScaleCount> kBassKeyScales = {{
    {{0, 2, 4, 5, 7, 9, 11, -1}},   // maior (jônio)
    {{0, 2, 3, 5, 7, 8, 10, -1}},   // menor natural (eólio)
    {{0, 2, 3, 5, 7, 9, 10, -1}},   // dórico
    {{0, 2, 4, 5, 7, 9, 10, -1}},   // mixolídio
    {{0, 1, 3, 5, 7, 8, 10, -1}},   // frígio
    {{0, 2, 4, 6, 7, 9, 11, -1}},   // lídio
    {{0, 2, 3, 5, 7, 8, 11, -1}},   // menor harmônica
    {{0, 2, 4, 7, 9, -1, -1, -1}},  // pentatônica maior
    {{0, 3, 5, 7, 10, -1, -1, -1}}, // pentatônica menor
    {{0, 3, 5, 6, 7, 10, -1, -1}}}};// blues
const char* bassKeyScaleName(int scale) noexcept;   // em português
const char* bassKeyTonicName(int tonic) noexcept;   // Dó, Dó♯, Ré…
// a nota da escala mais próxima de `midi` (empate: a de baixo), em 24–60
int quantizeBassToKey(int midi, int tonic, int scale) noexcept;
// a tônica mais próxima de `midi`, em 28–48
int nearestBassTonic(int midi, int tonic) noexcept;

struct SeedBassScheduleParameters {
    double bpm = 120.0;
    double sectionSeconds = 8.0;
    std::uint8_t meterNumerator = 4;
    std::uint8_t meterDenominator = 4;
    std::uint32_t seedHash = 0;
    std::uint8_t variationLevel = 2;
    float pitchVariation = 0.0f;
    float pathMemory = 0.72f;
    float pathBreath = 1.0f;
    BassPhraseMode phraseMode = BassPhraseMode::loop;
    BassDurationLaw durationLaw = BassDurationLaw::manual;
    double geometricRatio = 1.5;
    std::array<std::uint8_t, kBassStepsPerBar> manualDurationSteps{};
    BassRhythmRelation relation = BassRhythmRelation::follow;
    std::uint8_t voiceCount = 2;
    std::array<float, kBassRelationVoiceCount> relationStrengths = [] {
        std::array<float, kBassRelationVoiceCount> values{};
        values.fill(0.28f);
        values[0] = 0.0f;
        return values;
    }();
    GrooveMode groove{};
    DynamicCurve dynamics = static_cast<DynamicCurve>(2); // arch
    MusicalIntention intention = static_cast<MusicalIntention>(2); // meet
    int keyTonic = -1;   // tom da seed (ver kBassKeyScales); -1 = cromático
    int keyScale = 0;
};

// Pure, bounded preparation. Neither function belongs in the audio callback.
BassError buildBassScore(const BassRecipe& recipe, BassScore& score) noexcept;
BassError interpretBassScore(const BassScore& score,
                             const BassInterpretation& interpretation,
                             BassPerformance& performance) noexcept;
// Builds the bounded, seed-derived bass event calendar from prepared voice
// events. Input event times are local to a formal cycle and already swung.
BassError buildSeedBassPerformance(
    const std::array<std::vector<SeedBassVoiceEvent>, 7>& voiceEvents,
    const SeedBassScheduleParameters& parameters,
    BassPerformance& performance);

} // namespace batruc
