// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

#include "core/GeometryCurves.hpp"

namespace batruc {

constexpr std::size_t kMaxVoices = 12;
// Twelve-slot storage keeps older scene files readable; the instrument's
// currently playable composition and mixer are limited to eight voices.
constexpr std::size_t kMaxActiveVoices = 8;
constexpr std::size_t kFormalCycleCount = 7;
constexpr std::size_t kMaxEventsPerSection = 65'536;
constexpr std::size_t kMaxPreparedVariants = 12'288;
constexpr std::size_t kMaxPreparedEventsTotal = 1'048'576;
constexpr std::size_t kModesPerVoice = 8;
constexpr std::size_t kMaxGeometryEventsPerTraversal = 128;
constexpr std::size_t kSineTableSize = 4096;
constexpr double kTau = 6.28318530717958647692;

struct EffectiveMixerLevels {
    std::array<float, kMaxVoices> voices{};
    float bass = 0.0f;
};

// A cyclic pan path over one composition section. Values are normalized
// offsets around the channel's base pan; disabled paths retain field-X motion.
struct PanTrajectory {
    static constexpr std::size_t pointCount = 8;
    bool enabled = false;
    std::array<float, pointCount> points{};
};

float samplePanTrajectory(const PanTrajectory& trajectory,
                          float sectionProgress) noexcept;

// Resolve the standalone mixer's mute/solo state without allocating. A solo
// request isolates its channel(s); channel faders remain multiplicative.
EffectiveMixerLevels resolveMixerLevels(
    const std::array<float, kMaxVoices>& requestedVoiceLevels,
    const std::array<bool, kMaxVoices>& voiceMuted,
    const std::array<bool, kMaxVoices>& voiceSolo,
    std::size_t activeVoiceCount, float requestedBassLevel,
    bool bassEnabled, bool bassMuted, bool bassSolo) noexcept;

// Suggest a bounded static voice-bus trim that brings measured voice RMS
// toward bass RMS. Returns false until both buses have measurable energy.
bool suggestVoiceTrimDeltaDb(float voiceRms, float bassRms,
                             float currentTrimDb,
                             float& deltaDb) noexcept;

// Stable seed contract shared with prototype/score.js. Indices deliberately
// describe the native recipe table, rather than UI item IDs.
struct SeededRecipeSelection {
    std::size_t voiceTwoCycleIndex = 0;
    std::size_t phaseIndex = 0;
    std::size_t eventRuleIndex = 0;
    std::size_t patternIndex = 0;
    std::size_t voiceCount = 2;
    std::size_t intensityLawIndex = 0;
    float intensityAmount = 0.35f;
    // Added as independently salted genes so expanding the seed contract does
    // not change the legacy cycle/phase/rule/pattern mappings above.
    std::size_t soundFamilyIndex = 0;
    std::size_t grooveIndex = 0;
    std::size_t intentionIndex = 0;
    std::size_t dynamicsIndex = 0;
    std::size_t meterIndex = 1;
    float voiceTwoTravelPerBar = 0.0f;
    float compositionVariation = 0.35f;
    std::array<float, kMaxActiveVoices> chipDutyCycles{};
    std::array<float, kMaxActiveVoices> chipClockHz{};
    std::array<int, kMaxActiveVoices> chipBitDepth{};
    std::array<float, kMaxActiveVoices> chipNoiseMix{};
    std::array<float, kMaxActiveVoices> voiceBaseCouplings{};
    std::array<float, kMaxActiveVoices> voiceBasePhases{};
    std::array<int, kMaxActiveVoices> voiceBaseTransforms{};
    std::array<float, kMaxActiveVoices> formulaPhaseOffsets{};
    std::array<float, kMaxActiveVoices> voiceVariationDepths{};
    bool geometryVoiceOneSource = false;
    std::size_t geometryShapeIndex = 0;
    int geometryFourierTerms = 8;
    std::size_t billiardPatternIndex = 0;
    int lissajousXFrequency = 3;
    int lissajousYFrequency = 2;
    float lissajousPhase = 0.0f;
    float lissajousXAmplitude = 1.0f;
    float lissajousYAmplitude = 1.0f;
};

struct FormalPhaseSegment {
    double offsetCycles = 0.0;
    double slopeCycles = 0.0;
};

FormalPhaseSegment formalPhaseSegmentForCycle(std::size_t cycle,
                                               double amount) noexcept;
std::size_t activeVoiceCountInCycle(std::size_t cycle,
                                    std::size_t requestedCount) noexcept;

std::uint32_t nextSeedWord(std::uint32_t& state) noexcept;
std::uint32_t freshSeedOutsideRecent(
    std::uint32_t candidate, const std::uint32_t* recentSeeds,
    std::size_t recentCount, bool hasCurrentSeed,
    std::uint32_t currentSeed) noexcept;
double compositionSeedRandom(std::uint32_t& state) noexcept;
SeededRecipeSelection seededRecipeSelection(std::uint32_t seed) noexcept;
// Deterministically choose a different seed whose recipe contrasts with the
// current one across rhythm, timbre family, expression and intensity.
// salt = 0 mantém a sequência determinística de antes (testes, A/B); o app
// passa um sorteio novo a cada clique para a corrente de seeds não se repetir
// (relato do autor, 12/11/2026: "as seeds estão se repetindo").
std::uint32_t algorithmicNextSeed(std::uint32_t currentSeed,
                                  std::uint32_t salt = 0) noexcept;

struct PolyrhythmPreset {
    const char* label;
    std::array<double, 5> cycles;
    std::size_t voiceCount;
};

inline constexpr std::array<PolyrhythmPreset, 12> kPolyrhythmPresets = {{
    {"4:3", {4, 3, 0, 0, 0}, 2},
    {"5:4", {5, 4, 0, 0, 0}, 2},
    {"6:4", {6, 4, 0, 0, 0}, 2},
    {"7:4", {7, 4, 0, 0, 0}, 2},
    {"8:6", {8, 6, 0, 0, 0}, 2},
    {"10:4", {10, 4, 0, 0, 0}, 2},
    {"10:7", {10, 7, 0, 0, 0}, 2},
    {"2:3:5", {2, 3, 5, 0, 0}, 3},
    {"3:4:5", {3, 4, 5, 0, 0}, 3},
    {"4:5:7", {4, 5, 7, 0, 0}, 3},
    {"3:4:5:7", {3, 4, 5, 7, 0}, 4},
    {"3:4:5:7:8", {3, 4, 5, 7, 8}, 5}
}};

struct PolyrhythmApplication {
    bool valid = false;
    std::size_t voiceCount = 0;
    std::array<float, kMaxVoices> cycles{};
};

inline PolyrhythmApplication applyPolyrhythmPreset(
    const std::size_t presetIndex,
    const std::size_t currentVoiceCount,
    const std::array<float, kMaxVoices>& currentCycles) noexcept {
    if (presetIndex >= kPolyrhythmPresets.size()
        || currentVoiceCount < 2 || currentVoiceCount > kMaxVoices)
        return {};

    const auto& preset = kPolyrhythmPresets[presetIndex];
    PolyrhythmApplication result;
    result.valid = true;
    result.voiceCount = std::max(currentVoiceCount, preset.voiceCount);
    result.cycles = currentCycles;
    for (std::size_t voice = 0; voice < preset.voiceCount; ++voice)
        result.cycles[voice] = static_cast<float>(preset.cycles[voice]);
    return result;
}

enum class EventRule : std::uint8_t {
    centreCrossings,
    turningPoints
};

enum class PatternMode : std::uint8_t {
    unrestricted,
    adhesivePads,
    diagonalLanes
};

struct PatternFieldPose {
    double offsetX = 0.0;
    double offsetY = 0.0;
    double rotationRadians = 0.0;
    double halfWidth = 0.16;
    std::array<double, 4> padRadii{};
    std::array<double, 4> padShapePhases{};
    std::array<std::uint8_t, 4> padMorphologies{};
    std::array<Point2D, 4> padCentreOffsets{};
    double padEdgeX = 0.0;
    double padEdgeXY = 0.0;
    double padEdgeCos3 = 0.0;
    double padEdgeSin3 = 0.0;
    double laneBow = 0.0;
    double laneWaveA = 0.0;
    double laneWaveB = 0.0;
    std::array<double, 2> laneCentreOffsets{};
    std::array<double, 2> laneHalfWidths{};
    double focusX = 0.0;
    double focusY = 0.0;
    double focusRadius = 0.36;
};

struct PatternFieldBridge {
    Point2D start{};
    Point2D end{};
    double radius = 0.0;
};

constexpr double kPatternFieldEdgeSoftness = 0.055;

// Seeded moving field shared by the event engine and the visual field.
PatternFieldPose patternFieldPose(PatternMode mode, double progress,
                                  std::uint32_t seed) noexcept;
double patternFieldPadBoundary(const PatternFieldPose& pose,
                               double angle, std::size_t organism = 0) noexcept;
double patternFieldPadRadius(const PatternFieldPose& pose,
                             std::size_t organism) noexcept;
PatternFieldBridge patternFieldPadBridge(const PatternFieldPose& pose,
                                         std::size_t bridge) noexcept;
double patternFieldLaneDisplacement(const PatternFieldPose& pose,
                                    double x) noexcept;
double patternFieldLaneSlope(const PatternFieldPose& pose,
                             double x) noexcept;
double patternFieldBoundaryDistance(PatternMode mode, double x, double y,
                                    double progress, std::uint32_t seed) noexcept;
bool patternFieldAllows(PatternMode mode, double x, double y, double progress,
                        std::uint32_t seed) noexcept;
float patternFieldWeight(PatternMode mode, double x, double y, double progress,
                         std::uint32_t seed) noexcept;

enum class IntensityLaw : std::uint8_t {
    uniform,
    projectionY,
    productXY,
    quadraticDifference,
    gaussianCentre,
    cosineBands,
    potentialEnergy,
    kineticEnergy
};

// Returns a normalized event intensity for the declared mathematical model.
// For the energy laws, cyclesX/Y are proportional to oscillator frequencies.
float intensityFormulaValue(IntensityLaw law, float x, float y,
                            double cyclesX, double cyclesY) noexcept;
// Convex blend of two bounded intensity laws. A weight of 0 selects A and 1
// selects B; both laws see the same coordinates and cycle ratios.
float blendedIntensityFormulaValue(IntensityLaw lawA, IntensityLaw lawB,
                                   float blend, float x, float y,
                                   double cyclesX, double cyclesY) noexcept;

enum class SoundCharacter : std::uint8_t {
    sine,
    triangle,
    sawtooth,
    square,
    bell,
    membrane,
    kick,
    snare,
    hihat,
    ride,
    conga,
    brushes,
    chip8,
    openHihat,
    rimClick,
    shaker,
    agogo,
    woodblock
};

// Stable MIDI drum-role order used by import, family palettes and rendering.
// Keep this mapping in one place so a family change cannot invert kit stems.
inline constexpr std::array<SoundCharacter, kMaxActiveVoices>
    kMidiLaneDefaultSounds = {
        SoundCharacter::kick, SoundCharacter::snare, SoundCharacter::hihat,
        SoundCharacter::openHihat, SoundCharacter::membrane,
        SoundCharacter::ride, SoundCharacter::conga, SoundCharacter::brushes};

// Canonical stem palette shared by the live app and offline seed renderer.
std::array<SoundCharacter, kMaxActiveVoices> soundFamilyVoiceSounds(
    int familyIndex) noexcept;

struct Chip8Parameters {
    float dutyCycle = 0.50f;
    float clockHz = 8000.0f;
    int bitDepth = 8;
    float noiseMix = 0.34f;
};

enum class GrooveMode : std::uint8_t { straight, ternary, additive, offbeat };
enum class MusicalIntention : std::uint8_t { build, thin, meet, contrast };
enum class DynamicCurve : std::uint8_t { steady, crescendo, arch, waves };

// Progress is the performed (post-swing) position in a four-bar 4/4 section.
float grooveAccentAtProgress(double progress, GrooveMode groove,
                             int numerator = 4, int denominator = 4) noexcept;
// Maps straight beat progress to performed beat progress. The ratio uses
// eighth-note proportions (0.5 straight, 0.67 maximum swing).
double swingProgress(double progress, double ratio,
                     double quarterNotesPerSection) noexcept;
float expressionAtProgress(double progress, DynamicCurve dynamics,
                           MusicalIntention intention) noexcept;

enum class VoiceTransform : std::uint8_t {
    sine,
    invert,
    rectify,
    harmonic,
    amplitude,
    quantized,
    cubic,
    thirdHarmonic,
    triangle,
    softClip
};

struct SeedVariationParameters {
    std::uint32_t seed = 0;
    float amount = 0.0f;
    std::array<float, kMaxVoices> baseCycles{};
    std::array<float, kMaxVoices> baseCouplings{};
    // Base phases include the per-voice stagger used by the seed model.
    std::array<float, kMaxVoices> basePhases{};
    std::array<int, kMaxVoices> baseTransforms{};
};

struct SeedVariationResult {
    std::array<float, kMaxVoices> cycles{};
    std::array<float, kMaxVoices> couplings{};
    std::array<float, kMaxVoices> phases{};
    std::array<int, kMaxVoices> transforms{};
};

SeedVariationResult realizeSeedVariation(
    const SeedVariationParameters& parameters) noexcept;

struct ModalProfile {
    // Ratios and weights describe a virtual resonator, not a measured material.
    std::array<double, kModesPerVoice> frequencyRatios{};
    std::array<float, kModesPerVoice> modeWeights{};
    double decaySeconds = 0.45;
    double frequencyDependentLoss = 0.0;
    float strikeNoise = 0.08f;
};

struct VoiceRecipe {
    double cyclesPerSection = 1.0;
    double phaseCycles = 0.0;
    double resonanceHz = 110.0;
    float gain = 0.65f;
    float pan = 0.0f;
    EventRule eventRule = EventRule::centreCrossings;
    ModalProfile resonator{};
};

// A bounded, editable onset cell. Positions are normalized to [0, 1), sorted
// in time, and carry their own normalized accent/velocity. A disabled cell
// leaves the established formula-derived event rule untouched.
// A two-bar performed MIDI may contain more than 32 snare/hat hits in one
// lane. Manual cycle controls can retain their narrower 1–32 gesture range.
constexpr std::size_t kMaxRhythmicCellAttacks = 64;
// Marca de ornamento numa nota escrita (13/11/2026): 0 = nenhum; 1–15 =
// código do ornamento (ver SnareOrnaments: tipo + 1). Os toques do enfeite
// não ficam na célula escrita; nascem na célula tocada (expandSnareOrnaments).
constexpr std::uint8_t kMaxCellOrnamentCode = 15;
// A single onset can carry up to four distinct GM notes within one stem.
constexpr std::size_t kMaxMidiNotesPerAttack = 4;
constexpr std::size_t kMaxMidiCollisionNotesPerAttack = kMaxMidiNotesPerAttack - 1;
constexpr std::uint8_t kNoMidiNote = 0xffu;
constexpr std::uint8_t kNoMidiControllerValue = 0xffu;
struct RhythmicCell {
    bool enabled = false;
    std::uint8_t count = 0;
    std::array<double, kMaxRhythmicCellAttacks> positions{};
    std::array<float, kMaxRhythmicCellAttacks> strengths{};
    std::array<std::uint8_t, kMaxRhythmicCellAttacks> midiNotes = [] {
        std::array<std::uint8_t, kMaxRhythmicCellAttacks> notes{};
        notes.fill(kNoMidiNote);
        return notes;
    }();
    std::array<std::uint8_t, kMaxRhythmicCellAttacks> midiHatPedal = [] {
        std::array<std::uint8_t, kMaxRhythmicCellAttacks> values{};
        values.fill(kNoMidiControllerValue);
        return values;
    }();
    std::array<std::uint8_t, kMaxRhythmicCellAttacks> midiCollisionCount{};
    std::array<std::uint8_t, kMaxRhythmicCellAttacks> ornaments{};
    std::array<std::array<std::uint8_t, kMaxMidiCollisionNotesPerAttack>,
               kMaxRhythmicCellAttacks> midiCollisionNotes = [] {
        std::array<std::array<std::uint8_t, kMaxMidiCollisionNotesPerAttack>,
                   kMaxRhythmicCellAttacks> notes{};
        for (auto& attack : notes)
            attack.fill(kNoMidiNote);
        return notes;
    }();
    std::array<std::array<float, kMaxMidiCollisionNotesPerAttack>,
               kMaxRhythmicCellAttacks> midiCollisionStrengths{};
    std::array<std::array<std::uint8_t, kMaxMidiCollisionNotesPerAttack>,
               kMaxRhythmicCellAttacks> midiCollisionHatPedal = [] {
        std::array<std::array<std::uint8_t, kMaxMidiCollisionNotesPerAttack>,
                   kMaxRhythmicCellAttacks> values{};
        for (auto& attack : values)
            attack.fill(kNoMidiControllerValue);
        return values;
    }();
};

enum class RhythmicDynamicsShape : std::uint8_t {
    crescendo,
    diminuendo,
    alternating
};

bool validRhythmicCell(const RhythmicCell& cell) noexcept;
bool insertRhythmicCellAttack(RhythmicCell& cell, double position,
                              float strength, std::size_t* insertedIndex = nullptr) noexcept;
bool removeRhythmicCellAttack(RhythmicCell& cell, std::size_t index) noexcept;
bool moveRhythmicCellAttack(RhythmicCell& cell, std::size_t index,
                            double position) noexcept;
bool rotateRhythmicCell(RhythmicCell& cell, double phaseTurns) noexcept;
bool euclideanRedistributeRhythmicCell(RhythmicCell& cell,
                                       std::size_t steps) noexcept;
bool reduceRhythmicCellDensity(RhythmicCell& cell, double keepRatio,
                               std::uint32_t seed) noexcept;
bool interlockRhythmicCell(RhythmicCell& cell, const RhythmicCell& partner,
                           std::uint32_t seed) noexcept;
bool recombineRhythmicCell(RhythmicCell& cell, const RhythmicCell& partner,
                           std::uint32_t seed) noexcept;
bool recombineRhythmicPhraseBars(RhythmicCell& cell,
                                 const RhythmicCell& partner,
                                 std::uint8_t barCount,
                                 std::uint32_t seed) noexcept;
bool canRecombineRhythmicPhraseBars(const RhythmicCell& cell,
                                    const RhythmicCell& partner,
                                    std::uint8_t barCount) noexcept;
bool canAdaptRhythmicCellPairDensity(const RhythmicCell& first,
                                     const RhythmicCell& second,
                                     std::uint8_t barCount) noexcept;
bool adaptRhythmicCellPairDensity(RhythmicCell& first, RhythmicCell& second,
                                  std::uint8_t barCount,
                                  std::uint32_t seed) noexcept;
bool shapeRhythmicCellDynamics(RhythmicCell& cell,
                               RhythmicDynamicsShape shape,
                               std::uint32_t seed) noexcept;
bool varyRhythmicCellMicrotiming(RhythmicCell& cell,
                                 double maximumOffsetTurns,
                                 std::uint32_t seed) noexcept;
bool setRhythmicCellStrength(RhythmicCell& cell, std::size_t index,
                             float strength) noexcept;

struct CompositionRecipe {
    double sectionSeconds = 8.0;
    std::size_t voiceCount = 2;
    PatternMode patternMode = PatternMode::unrestricted;
    std::array<VoiceRecipe, kMaxVoices> voices{};
    std::array<RhythmicCell, kMaxVoices> rhythmicCells{};
};

struct MusicalEvent {
    double timeSeconds = 0.0;
    double phaseCycles = 0.0;
    std::uint8_t voice = 0;
    std::uint8_t midiNote = kNoMidiNote;
    std::uint8_t midiHatPedal = kNoMidiControllerValue;
    std::uint8_t soundLayer = 0;
    EventRule rule = EventRule::centreCrossings;
    float intensity = 1.0f;
    float fieldX = 0.0f;
    float fieldY = 0.0f;
    bool generatedByRelation = false;
};

struct EventPlan {
    // Built on the control thread. The audio renderer copies a bounded schedule
    // in prepare(); process() never grows this vector or allocates memory.
    std::vector<MusicalEvent> events;
};

struct CoupledPendulumParameters {
    double durationSeconds = 8.0;
    double sampleStepSeconds = 0.005;
    double gravity = 9.80665;
    double lengthMetres = 1.0;
    double massKilograms = 1.0;
    double springConstantNewtonsPerMetre = 0.7;
    double dampingPerSecond = 0.035;
    std::array<double, 3> initialAnglesRadians = {0.22, -0.08, 0.15};
    std::array<double, 3> initialVelocitiesRadiansPerSecond = {0.0, 0.12, -0.04};
};

struct CoupledPendulumSample {
    double timeSeconds = 0.0;
    std::array<double, 3> anglesRadians{};
    std::array<double, 3> angularVelocitiesRadiansPerSecond{};
    double kineticEnergyJoules = 0.0;
    double potentialEnergyJoules = 0.0;
    std::array<double, 3> kineticEnergyByPendulumJoules{};
    std::array<double, 3> potentialEnergyByPendulumJoules{};
};

struct CoupledPendulumTrajectory {
    std::vector<CoupledPendulumSample> samples;
};

enum class PendulumError : std::uint8_t {
    none,
    invalidParameters,
    invalidTrajectory,
    sampleLimitExceeded,
    nonFiniteState
};

enum class PlanError : std::uint8_t {
    none,
    invalidSectionDuration,
    invalidVoiceCount,
    invalidVoiceParameters,
    invalidRhythmicCell,
    eventLimitExceeded
};

struct ModalVoiceState {
    std::array<double, kModesPerVoice> phases{};
    std::array<float, kModesPerVoice> amplitudes{};
    std::array<float, kModesPerVoice> pendingExcitation{};
    double passageFrequencyScale = 1.0;
    double targetPassageFrequencyScale = 1.0;
    std::uint32_t excitationSamplesLeft = 0;
    std::uint32_t noiseState = 1;
    std::uint32_t noiseSamplesLeft = 0;
    float noiseEnvelope = 0.0f;
    float noiseEnvelopeTarget = 0.0f;
    float noiseDecayCoefficient = 1.0f;
    float noiseLowpass = 0.0f;
    float snareWireEnvelope = 0.0f;
    float snareWireDecay = 1.0f;
    float snareWireFast = 0.0f;
    float snareWireSlow = 0.0f;
    float excitationNoiseColour = 1.0f;
    float excitationDecayCoefficient = 1.0f;
    float hiHatOpenAmount = -1.0f;
    double transientFrequencyScale = 1.0;
    double notePitchScale = 1.0;
    float percussionDampingCoefficient = 1.0f;
    std::uint64_t previousPercussionAttackSample = 0;
    bool hasPreviousPercussionAttack = false;
    std::uint32_t chipHoldSamplesLeft = 0;
    float chipHeldSample = 0.0f;
    // corpo metálico do chimbal (lote 2 da auditoria musical, 12/11/2026)
    std::array<double, 6> metalPhases{};
    float metalEnvelope = 0.0f;
    float metalDecay = 1.0f;
    float metalHpIn1 = 0.0f, metalHpOut1 = 0.0f;
    float metalHpIn2 = 0.0f, metalHpOut2 = 0.0f;
    bool active = false;
};

class ResonatorRenderer final {
public:
    // Call configure() before playback and outside the audio callback.
    bool configure(const CompositionRecipe& recipe,
                   const EventPlan& plan,
                   double sampleRate) noexcept;
    // All plans share one transport and one set of resonator tails. Preparation
    // and allocation happen here, outside the audio callback.
    bool configureVariants(const CompositionRecipe* recipes,
                           const EventPlan* plans,
                           std::size_t planCount,
                           double sampleRate) noexcept;
    bool configureVariants(const CompositionRecipe& recipe,
                           const EventPlan* plans,
                           std::size_t planCount,
                           double sampleRate) noexcept;
    void reset() noexcept;

    // Interleaving and channel routing belong to the host. This renderer writes
    // one stereo frame into each output array, without heap use or locks.
    void process(float* left, float* right, std::size_t frames) noexcept;
    void processVariant(float* left, float* right, std::size_t frames,
                        std::size_t variant) noexcept;
    // Four bars per section. A requested plan becomes active at the first
    // sample of the next bar; tempo is applied at the next section boundary.
    void processVariantAtNextBar(float* left, float* right, std::size_t frames,
                                 std::size_t requestedVariant,
                                 const float* channelLevels = nullptr,
                                 std::uint64_t requestedSectionSamples = 0) noexcept;
    // Live host path: preserve the playhead and switch future attacks at the
    // next audio block, while retaining the per-stem level controls.
    void processVariantLive(float* left, float* right, std::size_t frames,
                            std::size_t requestedVariant,
                            const float* channelLevels = nullptr,
                            std::uint64_t requestedSectionSamples = 0) noexcept;
    // Call only from the audio thread while playback is paused.
    void selectVariantWhilePaused(std::size_t variant,
                                  std::uint64_t requestedSectionSamples = 0) noexcept;
    void setActiveVoiceCount(std::size_t count) noexcept;
    // Corpo de bateria do lote 2 (AUDITORIA_MUSICAL_2026-11-12.md): chimbal
    // metálico (seis quadrados inarmônicos + passa-altas) e bumbo com queda
    // de altura mais curta. false = o corpo anterior, para A/B.
    void setModernDrumBody(bool enabled) noexcept { modernDrumBody_ = enabled; }
    // Modo RITMO (decisão do autor, 12/11/2026; AUDITORIA_MUSICAL_2026-11-12.md):
    // o que está escrito (célula, MIDI, figura) toca exatamente como escrito.
    // Ficam neutras as camadas escondidas: máscara de campo, lei de
    // intensidade, fórmula da voz, acento de groove e curva de expressão
    // sorteados, e a redução formal de vozes. O swing (controle visível)
    // continua. false = modo GERATIVO, o comportamento de antes.
    void setRhythmMode(bool enabled) noexcept;
    // Profundidade do GERATIVO (12/11/2026; o autor: "no modo generativo a
    // bateria fica mais baixa, em certos momentos chega a sumir"; depois:
    // "mais próximo do volume do modo ritmo, podendo haver dinâmicas, mas não
    // muito acentuadas"). O produto das camadas (campo, lei, fórmula da voz,
    // groove, expressão) vira 1 − d·(1 − produto). Padrão d = 0,3: cada ataque
    // fica entre 70 % e 100 % do escrito (≤ ~3 dB), nada some; d = 1 é o
    // comportamento anterior; d = 0, igual ao RITMO. A mesma profundidade
    // rege a redução formal de vozes: abaixo de 1, a voz que a forma tiraria
    // continua tocando com nível 1 − d (a introdução fica mais rala, não muda).
    void setGenerativeDepth(float depth) noexcept;
    // Percussão no tom da seed (12/11/2026; o autor: "necessário que seja em
    // todos os tons"). Com tônica 0–11, cada ataque de percussão com altura
    // definida (membrana/toms, conga, agogô, wood block, caixa) vai à nota
    // do tom mais próxima (no máximo meio tom por lado de cada nota da
    // escala); o bumbo vai à tônica ou à quinta mais próxima. Pratos,
    // chimbais e shaker não mudam. tonic < 0 desliga (comportamento anterior).
    void setPercussionKey(int tonic, int scale) noexcept;
    bool rhythmMode() const noexcept { return rhythmMode_; }
    // Imported drum arrangements keep every kit lane available from C1.
    void setFormalVoiceReductionEnabled(bool enabled) noexcept;
    // Set absolute stereo positions from the audio thread; no plan rebuild is needed.
    void setChannelPans(const float* channelPans) noexcept;
    void setChannelPanMotion(const float* panMotionDepths) noexcept;
    void setChannelPanTrajectories(const PanTrajectory* trajectories) noexcept;
    void setChannelVariationDepths(const float* variationDepths) noexcept;
    void setChannelSound(std::size_t voice, SoundCharacter sound) noexcept;
    // A style colors the selected stem material without replacing its role
    // (for example, MIDI kick remains kick when the family changes).
    void setChannelTimbreProfile(std::size_t voice, int familyIndex) noexcept;
    void setChannelChip8Parameters(std::size_t voice,
                                   Chip8Parameters parameters) noexcept;
    void setVoiceFormulaData(const VoiceTransform* transforms,
                             const float* couplings,
                             const float* phaseOffsets) noexcept;
    // Drives the seven-cycle phase arc. A value in [0, 0.5] adds a linear
    // phase trajectory to voices 2..N; crossings are solved from its slope.
    void setFormalPhaseAmount(float amount) noexcept;
    // Call from a control thread. Voice 1 may use the y coordinate of a
    // normalized path; Fourier tables are warmed here before audio reads them.
    void setGeometryVoiceOneSource(bool enabled, GeometryCurve curve,
                                   int fourierTerms,
                                   int billiardPattern = 0,
                                   GeometryLissajousParameters lissajous = {}) noexcept;
    // Cycle overrides use 0 to inherit the active recipe; phase offsets are
    // turns added to that voice's event phase. With applyImmediately, future
    // attacks in the current section are rescheduled without restarting it.
    void setVoiceRhythmOverrides(const float* cycles,
                                 const float* phaseOffsets,
                                 const int* eventRuleOverrides,
                                 bool applyImmediately) noexcept;
    // Audio-thread call. A bounded cell replaces future attacks immediately;
    // no schedule rebuild or allocation occurs in render().
    void setRhythmicCells(const std::array<RhythmicCell, kMaxVoices>& cells) noexcept;
    // Desenvolvimento ao longo da forma (12/11/2026): uma frase por ciclo
    // formal para V1–V3 (as figuras do pêndulo). Onde `enabled[v]`, a seção do
    // ciclo N toca formalCycleCells[N][v] no lugar da célula única da voz.
    static constexpr std::size_t kFormalCellVoices = 3;
    using FormalCycleCells = std::array<std::array<RhythmicCell, kFormalCellVoices>,
                                        kFormalCycleCount>;
    void setFormalCycleCells(const FormalCycleCells& cells,
                             const std::array<bool, kFormalCellVoices>& enabled) noexcept;
    // Set from the audio thread; evaluated only at event onsets, with no
    // allocation and without rebuilding the prepared event schedule.
    void setIntensityFormula(IntensityLaw law, float amount,
                             IntensityLaw secondLaw = IntensityLaw::uniform,
                             float blend = 0.0f) noexcept;
    // Adjustable damping and first-mode balance; no allocation in the callback.
    void setDecayScale(float scale) noexcept;
    void setFundamentalLevel(float level) noexcept;
    // 0.5 is straight eighths; higher ratios delay the offbeat proportionally.
    void setSwingRatio(double ratio) noexcept;
    void setGrooveMode(GrooveMode groove) noexcept;
    void setTimeSignature(int numerator, int denominator) noexcept;
    // Audio-thread tempo update: preserve fractional section position and
    // reschedule only future events in the current section.
    void setLiveSectionSamples(std::uint64_t samples) noexcept {
        setSectionSamples(samples);
    }
    void setPerformanceExpression(DynamicCurve dynamics,
                                 MusicalIntention intention) noexcept;
    void setDevelopmentSeed(std::uint32_t seed, bool enabled) noexcept {
        developmentSeed_ = seed;
        developmentEnabled_ = enabled;
    }

    std::uint64_t renderedSamples() const noexcept {
        return publishedSamples_.load(std::memory_order_relaxed);
    }
    std::uint64_t completedBars() const noexcept {
        return publishedBars_.load(std::memory_order_relaxed);
    }
    std::size_t activeVariant() const noexcept {
        return publishedVariant_.load(std::memory_order_relaxed);
    }
    double sectionProgress() const noexcept {
        return publishedSectionProgress_.load(std::memory_order_relaxed);
    }
    // Post-fader mono RMS, before pan and the shared master. Safe to read
    // from the UI thread while the renderer runs on the audio thread.
    float voiceRms(std::size_t voice) const noexcept {
        return voice < kMaxActiveVoices
            ? publishedVoiceRms_[voice].load(std::memory_order_relaxed) : 0.0f;
    }
    std::uint64_t activeSectionSamples() const noexcept {
        return publishedSectionSamples_.load(std::memory_order_relaxed);
    }
    bool ready() const noexcept { return ready_; }

private:
    struct ScheduledEvent {
        std::uint64_t sectionPhaseQ32 = 0;
        MusicalEvent event{};
    };

    struct VariantTiming {
        PatternMode patternMode = PatternMode::unrestricted;
        std::array<double, kMaxVoices> cycles{};
        std::array<double, kMaxVoices> phases{};
        std::array<EventRule, kMaxVoices> rules{};
    };

    static float finiteClamp(float value, float low, float high) noexcept;
    static float nextNoise(ModalVoiceState& state) noexcept;
    float lookupSine(double phase) const noexcept;
    bool isBarBoundary(std::uint64_t sectionPosition) const noexcept;
    std::uint64_t sampleAtCurrentTempo(const ScheduledEvent& event) const noexcept;
    void setSectionSamples(std::uint64_t sectionSamples) noexcept;
    void applyRequestedTimeSignature() noexcept;
    void applyFormalVoiceCount() noexcept;
    void activateVariant(std::size_t variant) noexcept;
    void refreshRhythmState(std::size_t variant) noexcept;
    void refreshDynamicFirstVoiceAnchor(PatternMode pattern) noexcept;
    void updateNextDynamicEventSample() noexcept;
    void acquireGeometryEventTables() noexcept;
    void refreshDynamicCrossing(std::size_t voice, std::size_t stream,
                                bool preserveNextCycle = false) noexcept;
    void triggerDynamicVoiceEvents(std::size_t voice, std::uint64_t position,
                                   const float* targetLevels) noexcept;
    void processWithSwitch(float* left, float* right, std::size_t frames,
                           std::size_t requestedVariant, bool quantized,
                           const float* channelLevels,
                           std::uint64_t requestedSectionSamples) noexcept;
    void excite(const MusicalEvent& event) noexcept;
    void refreshVoiceSpectrum(std::size_t voice, std::size_t layer = 0) noexcept;
    void setPanTarget(std::size_t voice, float pan) noexcept;
    float renderVoice(std::size_t voiceIndex) noexcept;
    float renderVoiceLayer(std::size_t voiceIndex, std::size_t layer) noexcept;
    float voiceFormulaValue(std::size_t voiceIndex, double progress) const noexcept;

    CompositionRecipe recipe_{};
    std::array<std::array<ModalVoiceState, kMaxMidiNotesPerAttack>,
               kMaxVoices> states_{};
    std::array<float, kSineTableSize + 1> sineTable_{};
    std::array<std::array<std::array<float, kModesPerVoice>, kMaxMidiNotesPerAttack>,
               kMaxVoices> decayCoefficients_{};
    std::array<std::array<std::array<double, kModesPerVoice>, kMaxMidiNotesPerAttack>,
               kMaxVoices> modeFrequencyRatios_{};
    std::array<std::array<std::array<float, kModesPerVoice>, kMaxMidiNotesPerAttack>,
               kMaxVoices> modeSpectralWeights_{};
    std::array<std::array<float, kMaxMidiNotesPerAttack>, kMaxVoices> noiseColours_{};
    std::array<std::array<bool, kMaxMidiNotesPerAttack>, kMaxVoices>
        balanceFundamentalByLevel_{};
    std::array<float, kMaxVoices> panLeft_{};
    std::array<float, kMaxVoices> panRight_{};
    std::array<float, kMaxVoices> panBase_{};
    std::array<float, kMaxVoices> panBaseLeft_{};
    std::array<float, kMaxVoices> panBaseRight_{};
    std::array<float, kMaxVoices> panTargetLeft_{};
    std::array<float, kMaxVoices> panTargetRight_{};
    std::array<float, kMaxVoices> panMotionDepth_{};
    std::array<PanTrajectory, kMaxVoices> panTrajectories_{};
    std::array<float, kMaxVoices> stemVariationDepth_{};
    std::array<SoundCharacter, kMaxVoices> channelSounds_{};
    // The selected kit part is stable; a GM note can choose a different
    // articulation within that part for the current strike.
    std::array<std::array<SoundCharacter, kMaxMidiNotesPerAttack>, kMaxVoices>
        activeVoiceSounds_{};
    std::array<Chip8Parameters, kMaxVoices> chip8Parameters_{};
    std::array<VoiceTransform, kMaxVoices> voiceTransforms_{};
    std::array<float, kMaxVoices> voiceCouplings_{};
    std::array<float, kMaxVoices> voiceFormulaPhaseOffsets_{};
    std::atomic<bool> geometryVoiceOneSource_{false};
    std::atomic<int> geometryVoiceOneCurve_{static_cast<int>(GeometryCurve::compass)};
    std::atomic<int> geometryVoiceOneFourierTerms_{8};
    std::atomic<int> geometryVoiceOneBilliardPattern_{0};
    std::atomic<int> geometryLissajousXFrequency_{3};
    std::atomic<int> geometryLissajousYFrequency_{2};
    std::atomic<double> geometryLissajousYPhaseTurns_{0.0};
    std::atomic<double> geometryLissajousXAmplitude_{1.0};
    std::atomic<double> geometryLissajousYAmplitude_{1.0};
    GeometryLissajousParameters activeGeometryLissajous_{};
    std::atomic<std::uint64_t> geometryEventTableSequence_{0};
    std::array<std::array<std::atomic<double>, kMaxGeometryEventsPerTraversal>, 2>
        requestedGeometryEventPositions_{};
    std::array<std::array<std::atomic<double>, kMaxGeometryEventsPerTraversal>, 2>
        requestedGeometryEventX_{};
    std::array<std::array<std::atomic<double>, kMaxGeometryEventsPerTraversal>, 2>
        requestedGeometryEventY_{};
    std::array<std::atomic<std::size_t>, 2> requestedGeometryEventCounts_{};
    std::array<std::array<double, kMaxGeometryEventsPerTraversal>, 2>
        activeGeometryEventPositions_{};
    std::array<std::array<Point2D, kMaxGeometryEventsPerTraversal>, 2>
        activeGeometryEventPoints_{};
    std::array<std::size_t, 2> activeGeometryEventCounts_{};
    bool activeGeometryEventTiming_ = false;
    bool inFormalPause_ = false;
    std::array<float, kMaxVoices> currentChannelLevels_{};
    std::array<int, kMaxVoices> channelTimbreProfiles_ = [] {
        std::array<int, kMaxVoices> values{};
        values.fill(-1);
        return values;
    }();
    std::array<double, kMaxVoices> channelTimbreProfileSampleRates_{};
    std::array<float, kMaxVoices> channelTimbreTailCoefficients_ = [] {
        std::array<float, kMaxVoices> values{};
        values.fill(1.0f);
        return values;
    }();
    std::array<float, kMaxVoices> channelTimbreNoiseGains_ = [] {
        std::array<float, kMaxVoices> values{};
        values.fill(1.0f);
        return values;
    }();
    std::array<float, kMaxVoices> channelTimbreBrightness_{};
    std::array<float, kMaxVoices> channelTimbreBodyGains_ = [] {
        std::array<float, kMaxVoices> values{};
        values.fill(1.0f);
        return values;
    }();
    std::array<float, kMaxVoices> channelTimbreNoiseDurations_ = [] {
        std::array<float, kMaxVoices> values{};
        values.fill(1.0f);
        return values;
    }();
    std::array<float, kMaxVoices> requestedVoiceCycles_{};
    std::array<float, kMaxVoices> requestedVoicePhaseOffsets_{};
    std::array<int, kMaxVoices> requestedVoiceEventRules_{};
    std::array<RhythmicCell, kMaxVoices> requestedRhythmicCells_{};
    FormalCycleCells formalCycleCells_{};
    std::array<bool, kFormalCellVoices> formalCycleCellsEnabled_{};
    std::array<RhythmicCell, kMaxVoices> activeRhythmicCells_{};
    std::array<double, kMaxVoices> activeVoiceCycles_{};
    std::array<double, kMaxVoices> activeVoicePhases_{};
    std::array<double, kMaxVoices> activeFormalPhaseOffsets_{};
    std::array<double, kMaxVoices> activeFormalPhaseSlopes_{};
    std::array<EventRule, kMaxVoices> activeVoiceRules_{};
    std::array<bool, kMaxVoices> activeDynamicRhythm_{};
    std::array<std::size_t, kMaxVoices> dynamicEventStreamCounts_{};
    std::array<std::array<std::int64_t, kMaxGeometryEventsPerTraversal>, kMaxVoices>
        dynamicCycleIndices_{};
    std::array<std::array<std::uint64_t, kMaxGeometryEventsPerTraversal>, kMaxVoices>
        dynamicEventSamples_{};
    MusicalEvent dynamicFirstVoiceAnchor_{};
    std::uint64_t dynamicFirstVoiceAnchorSample_
        = std::numeric_limits<std::uint64_t>::max();
    std::uint64_t nextDynamicRhythmSample_
        = std::numeric_limits<std::uint64_t>::max();
    bool dynamicFirstVoiceAnchorFired_ = false;
    std::vector<VariantTiming> variantTimings_;
    std::vector<std::vector<ScheduledEvent>> schedules_;
    double sampleRate_ = 0.0;
    float channelSlew_ = 1.0f;
    float decayScale_ = 1.0f;
    float fundamentalLevel_ = 1.0f;
    std::uint64_t baseSectionSamples_ = 0;
    std::uint64_t sectionSamples_ = 0;
    double swingRatio_ = 0.5;
    std::atomic<double> requestedSwingRatio_{0.5};
    std::atomic<float> requestedFormalPhaseAmount_{0.0f};
    int meterNumerator_ = 4;
    int meterDenominator_ = 4;
    double quarterNotesPerSection_ = 16.0;
    std::atomic<int> requestedTimeSignature_{404};
    GrooveMode grooveMode_ = GrooveMode::straight;
    std::atomic<GrooveMode> requestedGrooveMode_{GrooveMode::straight};
    DynamicCurve dynamicCurve_ = DynamicCurve::arch;
    MusicalIntention musicalIntention_ = MusicalIntention::meet;
    std::atomic<DynamicCurve> requestedDynamicCurve_{DynamicCurve::arch};
    std::atomic<MusicalIntention> requestedMusicalIntention_{MusicalIntention::meet};
    std::uint64_t sectionPosition_ = 0;
    std::uint64_t absoluteSample_ = 0;
    std::uint64_t completedBars_ = 0;
    std::uint32_t developmentSeed_ = 0;
    bool developmentEnabled_ = false;
    std::size_t activeVoiceCount_ = 2;
    bool formalVoiceReductionEnabled_ = true;
    std::size_t requestedVoiceCount_ = 2;
    std::size_t renderVoiceCount_ = 2;
    bool modernDrumBody_ = true;
    bool rhythmMode_ = false;
    float generativeDepth_ = 0.3f;
    int percussionKeyTonic_ = -1;
    int percussionKeyScale_ = 0;
    double keyTunedPitchScale(SoundCharacter sound, double fundamentalHz) const noexcept;
    std::size_t formalVoiceLimit_ = kMaxVoices;   // vozes que a forma deixaria
    float formalVoiceScale(std::size_t voice) const noexcept {
        return voice >= formalVoiceLimit_ ? 1.0f - generativeDepth_ : 1.0f;
    }
    IntensityLaw intensityLaw_ = IntensityLaw::uniform;
    IntensityLaw secondIntensityLaw_ = IntensityLaw::uniform;
    float intensityBlend_ = 0.0f;
    float intensityAmount_ = 0.0f;
    std::atomic<std::uint64_t> publishedSamples_{0};
    std::atomic<std::uint64_t> publishedBars_{0};
    std::atomic<std::uint64_t> publishedSectionSamples_{0};
    std::atomic<double> publishedSectionProgress_{0.0};
    std::atomic<std::size_t> publishedVariant_{0};
    std::array<std::atomic<float>, kMaxActiveVoices> publishedVoiceRms_{};
    std::array<float, kMaxActiveVoices> smoothedVoiceRms_{};
    std::size_t nextEvent_ = 0;
    std::size_t activeVariant_ = 0;
    bool ready_ = false;
};

CompositionRecipe makeInitialRecipe() noexcept;
// Set sortChronologically=false only when the consumer reduces the events
// without relying on their order; this avoids an unnecessary O(n log n) sort.
PlanError buildEventPlan(const CompositionRecipe& recipe, EventPlan& plan,
                         bool sortChronologically = true);
// Prepare an A/B rhythm-role inversion outside the audio callback. The two
// channels keep their resonator, pan and register; their event schedules
// exchange ownership. Source is unchanged and the operation is reversible.
bool invertEventPlanVoices(const CompositionRecipe& recipe,
                           const EventPlan& source,
                           std::size_t firstVoice,
                           std::size_t secondVoice,
                           EventPlan& inverted);
PendulumError buildCoupledPendulumTrajectory(
    const CoupledPendulumParameters& parameters,
    CoupledPendulumTrajectory& trajectory);
PendulumError buildPendulumEventPlan(const CoupledPendulumTrajectory& trajectory,
                                    double cycleSeconds,
                                    EventRule rule,
                                    std::array<EventPlan, kFormalCycleCount>& plans);

} // namespace batruc
