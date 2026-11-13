// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/BassComposition.hpp"
#include "core/CompositionCore.hpp"

#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>

namespace {

std::uint32_t hashSeed(const std::string& seed) {
    std::uint32_t hash = 2166136261u;
    for (const unsigned char character : seed)
        hash = (hash ^ character) * 16777619u;
    return hash;
}

double transformedVoiceValue(const std::size_t voice, const double progress,
                             const std::size_t cycle, const double phaseAmount,
                             const batruc::SeedVariationResult& variation) {
    using namespace batruc;
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
        case VoiceTransform::triangle: own = (2.0 / 3.14159265358979323846)
            * std::asin(std::clamp(sine, -1.0, 1.0)); break;
        case VoiceTransform::softClip: own = std::tanh(2.0 * sine)
            / std::tanh(2.0); break;
    }
    if (voice == 0 || variation.couplings[voice] == 0.0f)
        return own;
    return (1.0 - variation.couplings[voice]) * own
        + variation.couplings[voice] * transformedVoiceValue(
            voice - 1, progress, cycle, phaseAmount, variation);
}

bool parseRelation(const std::string& value, batruc::BassRhythmRelation& relation) {
    if (value == "independent") relation = batruc::BassRhythmRelation::independent;
    else if (value == "follow") relation = batruc::BassRhythmRelation::follow;
    else if (value == "answer") relation = batruc::BassRhythmRelation::answer;
    else if (value == "convolution") relation = batruc::BassRhythmRelation::convolution;
    else if (value == "eco-support") relation = batruc::BassRhythmRelation::ecologicalSupport;
    else if (value == "eco-imitation") relation = batruc::BassRhythmRelation::ecologicalImitation;
    else if (value == "eco-avoidance") relation = batruc::BassRhythmRelation::ecologicalAvoidance;
    else if (value == "eco-competition") relation = batruc::BassRhythmRelation::ecologicalCompetition;
    else return false;
    return true;
}

bool parseMeter(const std::string& value, int& numerator, int& denominator) {
    if (value == "3/4") { numerator = 3; denominator = 4; }
    else if (value == "4/4") { numerator = 4; denominator = 4; }
    else if (value == "5/4") { numerator = 5; denominator = 4; }
    else if (value == "7/8") { numerator = 7; denominator = 8; }
    else return false;
    return true;
}

bool parsePhrase(const std::string& value, batruc::BassPhraseMode& phrase) {
    if (value == "loop") phrase = batruc::BassPhraseMode::loop;
    else if (value == "breath") phrase = batruc::BassPhraseMode::breath;
    else if (value == "cadence") phrase = batruc::BassPhraseMode::cadence;
    else if (value == "call-response") phrase = batruc::BassPhraseMode::callResponse;
    else return false;
    return true;
}

bool parseGroove(const std::string& value, batruc::GrooveMode& groove) {
    if (value == "straight") groove = batruc::GrooveMode::straight;
    else if (value == "ternary") groove = batruc::GrooveMode::ternary;
    else if (value == "additive") groove = batruc::GrooveMode::additive;
    else if (value == "offbeat") groove = batruc::GrooveMode::offbeat;
    else return false;
    return true;
}

bool parseDynamics(const std::string& value, batruc::DynamicCurve& dynamics) {
    if (value == "steady") dynamics = batruc::DynamicCurve::steady;
    else if (value == "crescendo") dynamics = batruc::DynamicCurve::crescendo;
    else if (value == "arch") dynamics = batruc::DynamicCurve::arch;
    else if (value == "waves") dynamics = batruc::DynamicCurve::waves;
    else return false;
    return true;
}

bool parseIntention(const std::string& value, batruc::MusicalIntention& intention) {
    if (value == "build") intention = batruc::MusicalIntention::build;
    else if (value == "thin") intention = batruc::MusicalIntention::thin;
    else if (value == "meet") intention = batruc::MusicalIntention::meet;
    else if (value == "contrast") intention = batruc::MusicalIntention::contrast;
    else return false;
    return true;
}

bool parseDurationLaw(const std::string& value, batruc::BassDurationLaw& law) {
    if (value == "manual") law = batruc::BassDurationLaw::manual;
    else if (value == "fibonacci") law = batruc::BassDurationLaw::fibonacci;
    else if (value == "geometric") law = batruc::BassDurationLaw::geometric;
    else return false;
    return true;
}

} // namespace

int main(int argc, char** argv) {
    using namespace batruc;
    BassRhythmRelation relation{};
    GrooveMode groove{};
    DynamicCurve dynamics{};
    MusicalIntention intention{};
    BassDurationLaw durationLaw{};
    int numerator = 0;
    int denominator = 0;
    BassPhraseMode phrase{};
    if (argc != 16 || !parseRelation(argv[1], relation)
        || !parseMeter(argv[2], numerator, denominator)
        || !parsePhrase(argv[3], phrase) || !parseGroove(argv[11], groove)
        || !parseDynamics(argv[12], dynamics)
        || !parseIntention(argv[13], intention)
        || !parseDurationLaw(argv[14], durationLaw))
        return 2;

    const std::string seed(argv[4]);
    if (seed.size() != 8 || !std::all_of(seed.begin(), seed.end(),
        [](const unsigned char character) { return std::isxdigit(character) != 0; }))
        return 2;
    char* bpmEnd = nullptr;
    const double bpm = std::strtod(argv[5], &bpmEnd);
    if (bpmEnd == argv[5] || *bpmEnd != '\0' || !std::isfinite(bpm)
        || bpm < 20.0 || bpm > 300.0)
        return 2;
    char* phaseEnd = nullptr;
    const double phaseAmount = std::strtod(argv[6], &phaseEnd);
    if (phaseEnd == argv[6] || *phaseEnd != '\0' || !std::isfinite(phaseAmount)
        || phaseAmount < 0.0 || phaseAmount > 0.5)
        return 2;
    char* variationEnd = nullptr;
    const double bassPitchVariation = std::strtod(argv[7], &variationEnd);
    if (variationEnd == argv[7] || *variationEnd != '\0'
        || !std::isfinite(bassPitchVariation)
        || bassPitchVariation < 0.0 || bassPitchVariation > 1.0)
        return 2;
    char* voiceVariationEnd = nullptr;
    const double voiceVariation = std::strtod(argv[8], &voiceVariationEnd);
    if (voiceVariationEnd == argv[8] || *voiceVariationEnd != '\0'
        || !std::isfinite(voiceVariation) || voiceVariation < 0.0
        || voiceVariation > 1.0)
        return 2;
    char* voiceCountEnd = nullptr;
    const long parsedVoiceCount = std::strtol(argv[9], &voiceCountEnd, 10);
    if (voiceCountEnd == argv[9] || *voiceCountEnd != '\0'
        || parsedVoiceCount < 2 || parsedVoiceCount > static_cast<long>(kMaxVoices))
        return 2;
    char* swingEnd = nullptr;
    const double swingRatio = std::strtod(argv[10], &swingEnd);
    if (swingEnd == argv[10] || *swingEnd != '\0' || !std::isfinite(swingRatio)
        || swingRatio < 0.5 || swingRatio > 0.67)
        return 2;
    char* ratioEnd = nullptr;
    const double geometricRatio = std::strtod(argv[15], &ratioEnd);
    if (ratioEnd == argv[15] || *ratioEnd != '\0'
        || !std::isfinite(geometricRatio) || geometricRatio < 1.2
        || geometricRatio > 2.0)
        return 2;

    const double quarterNotesPerSection = 4.0 * numerator * 4.0 / denominator;
    const double sectionSeconds = quarterNotesPerSection * 60.0 / bpm;
    constexpr std::array<double, 7> formalStarts = {0, 0, 2, 2, 4, 5, 5};

    auto base = makeInitialRecipe();
    base.sectionSeconds = sectionSeconds;
    base.voiceCount = static_cast<std::size_t>(parsedVoiceCount);
    base.patternMode = PatternMode::unrestricted;
    for (std::size_t voice = 2; voice < base.voiceCount; ++voice)
        base.voices[voice] = base.voices[1];
    constexpr std::array<double, kMaxVoices> cycles = {
        3.0, 4.0, 1.0, 2.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0};
    SeedVariationParameters variationParameters;
    variationParameters.seed = hashSeed(seed);
    variationParameters.amount = static_cast<float>(voiceVariation);
    variationParameters.baseTransforms.fill(static_cast<int>(VoiceTransform::sine));
    variationParameters.baseCycles.fill(1.0f);
    variationParameters.baseCouplings.fill(0.0f);
    for (std::size_t voice = 0; voice < base.voiceCount; ++voice) {
        variationParameters.baseCycles[voice] = static_cast<float>(cycles[voice]);
        variationParameters.basePhases[voice] = voice == 0 ? 0.0f
            : static_cast<float>(voice - 1) / 16.0f;
        base.voices[voice].eventRule = EventRule::centreCrossings;
    }
    const auto realizedVariation = realizeSeedVariation(variationParameters);

    std::array<std::vector<SeedBassVoiceEvent>, kFormalCycleCount> sources;
    for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle) {
        auto recipe = base;
        const auto segment = formalPhaseSegmentForCycle(cycle, phaseAmount);
        for (std::size_t voice = 1; voice < recipe.voiceCount; ++voice) {
            recipe.voices[voice].cyclesPerSection = realizedVariation.cycles[voice]
                + segment.slopeCycles;
            recipe.voices[voice].phaseCycles = realizedVariation.phases[voice]
                + segment.offsetCycles;
        }
        recipe.voices[0].cyclesPerSection = realizedVariation.cycles[0];
        recipe.voices[0].phaseCycles = realizedVariation.phases[0];
        EventPlan plan;
        if (buildEventPlan(recipe, plan) != PlanError::none)
            return 3;
        for (const auto& event : plan.events) {
            if (event.voice >= activeVoiceCountInCycle(cycle, recipe.voiceCount))
                continue;
            const double progress = event.timeSeconds / sectionSeconds;
            const double performed = swingProgress(progress, swingRatio,
                                                    quarterNotesPerSection);
            const double sinceFormalStart = (static_cast<double>(cycle) + performed
                - formalStarts[cycle]) * sectionSeconds;
            const double baseIntensity = event.voice == 0 ? 0.76
                : 0.63 / std::sqrt(static_cast<double>(recipe.voiceCount - 1));
            const double globalProgress = (static_cast<double>(cycle) + progress)
                / static_cast<double>(kFormalCycleCount);
            const double x = transformedVoiceValue(0, progress, cycle,
                phaseAmount, realizedVariation);
            const double y = transformedVoiceValue(1, progress, cycle,
                phaseAmount, realizedVariation);
            const double intensity = baseIntensity * std::exp(-0.065 * sinceFormalStart)
                * expressionAtProgress(globalProgress, dynamics, intention)
                * grooveAccentAtProgress(performed, groove,
                                         numerator, denominator);
            sources[cycle].push_back({performed * sectionSeconds, x, y,
                static_cast<float>(intensity), event.voice});
        }
        std::stable_sort(sources[cycle].begin(), sources[cycle].end(),
            [](const SeedBassVoiceEvent& left, const SeedBassVoiceEvent& right) {
                if (left.voice != right.voice) return left.voice < right.voice;
                return left.localTimeSeconds < right.localTimeSeconds;
            });
    }

    SeedBassScheduleParameters parameters;
    parameters.bpm = bpm;
    parameters.sectionSeconds = sectionSeconds;
    parameters.meterNumerator = static_cast<std::uint8_t>(numerator);
    parameters.meterDenominator = static_cast<std::uint8_t>(denominator);
    parameters.seedHash = hashSeed(seed);
    parameters.pitchVariation = static_cast<float>(std::max(
        bassPitchVariation, voiceVariation));
    parameters.phraseMode = phrase;
    parameters.relation = relation;
    parameters.voiceCount = static_cast<std::uint8_t>(parsedVoiceCount);
    parameters.relationStrengths.fill(0.28f);
    parameters.relationStrengths[0] = 0.0f;
    parameters.groove = groove;
    parameters.dynamics = dynamics;
    parameters.intention = intention;
    parameters.durationLaw = durationLaw;
    parameters.geometricRatio = geometricRatio;
    parameters.manualDurationSteps.fill(2);
    BassPerformance performance;
    if (buildSeedBassPerformance(sources, parameters, performance) != BassError::none)
        return 4;

    std::cout << std::setprecision(17) << '[';
    for (std::size_t index = 0; index < performance.count; ++index) {
        if (index != 0) std::cout << ',';
        const auto& event = performance.events[index];
        const double absoluteTime = static_cast<double>(event.formCycle)
            * sectionSeconds + event.timeSeconds;
        double nextAbsoluteTime = sectionSeconds * kFormalCycleCount;
        for (std::size_t next = 0; next < performance.count; ++next) {
            if (next == index)
                continue;
            const auto& nextEvent = performance.events[next];
            const double candidateTime = static_cast<double>(nextEvent.formCycle)
                * sectionSeconds + nextEvent.timeSeconds;
            if (candidateTime >= absoluteTime - 1.0e-9
                && candidateTime < nextAbsoluteTime)
                nextAbsoluteTime = candidateTime;
        }
        double performedDuration = std::max(0.025,
            std::min(event.durationSeconds, nextAbsoluteTime - absoluteTime));
        const auto stepsPerBar = bassStepsPerBar(numerator, denominator);
        const auto stepsPerSection = stepsPerBar * kBassBarsPerSection;
        const double stepSeconds = 15.0 / bpm;
        const auto firstStep = static_cast<std::size_t>(event.formCycle)
            * stepsPerSection + event.sourceNote + 1;
        const auto lastStep = static_cast<std::size_t>(std::ceil(
            (absoluteTime + performedDuration) / stepSeconds));
        for (std::size_t step = firstStep; step <= lastStep; ++step) {
            const auto bar = step / stepsPerBar;
            const auto stepInBar = step % stepsPerBar;
            const bool phraseRest = phrase == BassPhraseMode::breath
                    ? bar % 2 == 1 && stepInBar >= stepsPerBar - 2
                : phrase == BassPhraseMode::cadence
                    ? bar % 4 == 3 && stepInBar >= stepsPerBar - 4
                : phrase == BassPhraseMode::callResponse
                    ? bar % 2 == 1 : false;
            if (!phraseRest)
                continue;
            performedDuration = std::max(0.025, std::min(performedDuration,
                step * stepSeconds - absoluteTime));
            break;
        }
        const double cycleEnd = (static_cast<double>(event.formCycle) + 1.0)
            * sectionSeconds;
        performedDuration = std::max(0.0, std::min(performedDuration,
            cycleEnd - absoluteTime));
        std::cout << "{\"cycle\":" << static_cast<int>(event.formCycle)
                  << ",\"time\":" << static_cast<double>(event.formCycle)
                        * sectionSeconds + event.timeSeconds
                  << ",\"midi\":" << static_cast<int>(event.midi)
                  << ",\"duration\":" << performedDuration
                  << ",\"intensity\":" << event.velocity
                  << ",\"slot\":" << event.sourceNote << '}';
    }
    std::cout << "]\n";
    return 0;
}
