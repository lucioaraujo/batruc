// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/BassAudio.hpp"
#include "core/PassageDevelopment.hpp"

#include <algorithm>
#include <cmath>

namespace batruc {
namespace {

constexpr double kTau = 6.28318530717958647692;
constexpr std::uint64_t kBassFormalCycles = 7u;
constexpr double kSqrt2 = 1.41421356237309504880;
constexpr double kSqrt3 = 1.73205080756887729352;

constexpr std::array<std::array<double, 3>, 4> kOscillatorRatios = {{
    {{1.0, 2.0, 3.0}},
    {{1.0, 3.0, 5.0}},
    {{1.0, kSqrt2, kSqrt3}},
    {{1.0, 1.003, 0.997}}
}};
constexpr std::array<float, 6> kMaterialCutoffsHz = {
    150.0f, 420.0f, 680.0f, 520.0f, 1800.0f, 1500.0f};

float bounded(const float value, const float low, const float high) noexcept {
    return std::isfinite(value) ? std::clamp(value, low, high) : low;
}

float lfoWaveValue(const double formProgress,
                   const BassAudioParameters& parameters) noexcept {
    const unsigned period = parameters.lfoPeriodCycles == 1
        || parameters.lfoPeriodCycles == 4 ? parameters.lfoPeriodCycles : 2;
    double phase = std::fmod(formProgress / static_cast<double>(period), 1.0);
    if (phase < 0.0)
        phase += 1.0;

    double value = 0.0;
    switch (parameters.lfoShape) {
        case BassLfoShape::triangle:
            value = 1.0 - 4.0 * std::abs(phase - 0.5);
            break;
        case BassLfoShape::sine:
            value = std::sin(kTau * phase);
            break;
        case BassLfoShape::square:
            value = phase < 0.5 ? 1.0 : -1.0;
            break;
    }
    return static_cast<float>(value);
}

} // namespace

void sanitizeBassPerformance(BassPerformance& performance) noexcept {
    if (!std::isfinite(performance.sectionSeconds) || performance.sectionSeconds <= 0.0)
        return;
    const auto count = std::min(performance.count, performance.events.size());
    std::size_t kept = 0;
    for (std::size_t i = 0; i < count; ++i) {
        auto event = performance.events[i];
        if (!std::isfinite(event.timeSeconds) || event.timeSeconds < 0.0
            || event.timeSeconds >= performance.sectionSeconds)
            continue;   // fora da seção: impossível, sai
        if (!std::isfinite(event.durationSeconds) || event.durationSeconds <= 0.0)
            event.durationSeconds = 0.001;
        event.durationSeconds = std::min(event.durationSeconds,
            performance.sectionSeconds - event.timeSeconds);
        if (event.durationSeconds <= 0.0)
            continue;
        event.velocity = std::isfinite(event.velocity)
            ? std::clamp(event.velocity, 0.0f, 1.0f) : 0.7f;
        event.pan = std::isfinite(event.pan) ? std::clamp(event.pan, -0.24f, 0.24f) : 0.0f;
        event.midi = static_cast<std::uint8_t>(std::clamp<int>(event.midi, 24, 60));
        if (!std::isfinite(event.dynamicsMapState)
            || (event.dynamicsMapState != -1.0f
                && (event.dynamicsMapState < 0.0f || event.dynamicsMapState > 1.0f)))
            event.dynamicsMapState = -1.0f;
        performance.events[kept++] = event;
    }
    performance.count = kept;
    std::stable_sort(performance.events.begin(),
                     performance.events.begin() + static_cast<std::ptrdiff_t>(kept),
                     [](const BassPlayedEvent& a, const BassPlayedEvent& b) {
                         return a.timeSeconds < b.timeSeconds;
                     });
}

bool BassAudioRenderer::configure(const BassPerformance& performance,
                                  const double sampleRate,
                                  const bool deferUntilBar) noexcept {
    if (!std::isfinite(sampleRate) || sampleRate < 8000.0
        || !std::isfinite(performance.sectionSeconds)
        || performance.sectionSeconds <= 0.0
        || performance.count > performance.events.size())
        return false;

    for (std::size_t i = 0; i < performance.count; ++i) {
        const auto& event = performance.events[i];
        if (!std::isfinite(event.timeSeconds) || event.timeSeconds < 0.0
            || !std::isfinite(event.durationSeconds) || event.durationSeconds <= 0.0
            || event.timeSeconds + event.durationSeconds
                > performance.sectionSeconds + 1.0e-9
            || !std::isfinite(event.velocity) || event.velocity < 0.0f
            || event.velocity > 1.0f || !std::isfinite(event.pan)
            || event.pan < -0.24f || event.pan > 0.24f
            || event.midi < 24 || event.midi > 60
            || !std::isfinite(event.dynamicsMapState)
            || (event.dynamicsMapState != -1.0f
                && (event.dynamicsMapState < 0.0f
                    || event.dynamicsMapState > 1.0f))
            || (i > 0 && event.timeSeconds < performance.events[i - 1].timeSeconds))
            return false;
    }

    if (deferUntilBar && ready_ && sampleRate_ == sampleRate) {
        pendingPerformance_ = performance;
        hasPendingPerformance_ = true;
        return true;
    }

    hasPendingPerformance_ = false;
    const bool preserveState = ready_ && sampleRate_ == sampleRate
        && performance_.sectionSeconds > 0.0 && previousPosition_ >= 0.0;
    const double oldSectionSeconds = performance_.sectionSeconds;
    const double oldPosition = previousPosition_;
    performance_ = performance;
    sampleRate_ = sampleRate;
    ready_ = true;
    if (!preserveState) {
        reset();
        return true;
    }

    const double sectionRatio = performance_.sectionSeconds / oldSectionSeconds;
    previousPosition_ = std::fmod(oldPosition * sectionRatio,
                                  performance_.sectionSeconds);
    for (auto& voice : voices_)
        voice.gateEnd = std::fmod(voice.gateEnd * sectionRatio,
                                  performance_.sectionSeconds);
    nextEvent_ = 0;
    while (nextEvent_ < performance_.count) {
        const double eventTime = performance_.events[nextEvent_].timeSeconds;
        if (eventTime + 1.0 / sampleRate_ >= previousPosition_)
            break;
        ++nextEvent_;
    }
    return true;
}

void BassAudioRenderer::reset() noexcept {
    voices_ = {};
    previousPosition_ = -1.0;
    nextEvent_ = 0;
    voiceAge_ = 0;
    enabledPrevious_ = false;
    hasPendingPerformance_ = false;
}

void BassAudioRenderer::applyPendingPerformance(
    const double positionSeconds, const double sectionSeconds,
    const float releaseSeconds) noexcept {
    if (!hasPendingPerformance_ || sectionSeconds <= 0.0)
        return;
    performance_ = pendingPerformance_;
    hasPendingPerformance_ = false;
    releaseAll(releaseSeconds);
    nextEvent_ = 0;
    while (nextEvent_ < performance_.count) {
        const double eventTime = performance_.events[nextEvent_].timeSeconds
            * sectionSeconds / std::max(performance_.sectionSeconds, 1.0e-9);
        if (eventTime + 0.5 / sampleRate_ >= positionSeconds)
            break;
        ++nextEvent_;
    }
}

void BassAudioRenderer::noteOn(VoiceState& voice, const int midi,
                              const float velocity,
                              const double eventDurationSeconds,
                              const float dynamicsMapState,
                              const float pan) noexcept {
    if (voice.currentMidi >= 0 && voice.ampStage != Stage::off) {
        const unsigned fadeFrames = static_cast<unsigned>(std::max(1.0,
            std::round(0.004 * sampleRate_)));
        const float pendingResidual = voice.stealFadeTotal > 0
            ? voice.stealResidual * static_cast<float>(voice.stealFadeRemaining)
                / static_cast<float>(voice.stealFadeTotal)
            : 0.0f;
        voice.stealResidual = voice.lastOutput + pendingResidual;
        voice.stealFadeRemaining = fadeFrames;
        voice.stealFadeTotal = fadeFrames;
    }
    voice.currentMidi = std::clamp(midi, 24, 60);
    voice.velocity = bounded(velocity, 0.0f, 1.0f);
    voice.pan = bounded(pan, -0.24f, 0.24f);
    const float mapState = bounded(dynamicsMapState, 0.0f, 1.0f);
    voice.eventCutoffScale = dynamicsMapState >= 0.0f
        ? 0.28f + 3.2f * mapState * mapState : 1.0f;
    voice.pluckDecaySeconds = std::clamp(eventDurationSeconds, 0.012, 0.12);
    voice.pluckEnvelope = 1.0f;
    voice.pluckEnvelopeStep = sampleRate_ > 0.0
        ? static_cast<float>(std::pow(0.12,
            1.0 / (voice.pluckDecaySeconds * sampleRate_)))
        : 1.0f;
    voice.fundamentalHz = 440.0 * std::pow(2.0,
        (static_cast<double>(voice.currentMidi) - 69.0) / 12.0);
    voice.ampLevel = 0.0f;
    voice.filterLevel = 0.0f;
    voice.ampAttackStart = 0.0f;
    voice.filterAttackStart = 0.0f;
    voice.ampStage = Stage::attack;
    voice.filterStage = Stage::attack;
    voice.ampStagePosition = 0.0;
    voice.filterStagePosition = 0.0;
    voice.phases.fill(0.0);
    voice.filterX1 = voice.filterX2 = voice.filterY1 = voice.filterY2 = 0.0;
    voice.filterB0 = voice.filterB1 = voice.filterB2 = 0.0;
    voice.filterA1 = voice.filterA2 = 0.0;
    voice.filterCoefficientCountdown = 0;
    voice.lfoCountdown = 0;
    voice.lfoValue = 0.0f;
    voice.targetLfoValue = 0.0f;
    voice.lfoPitchRatio = 1.0;
    voice.targetLfoPitchRatio = 1.0;
    voice.age = ++voiceAge_;
}

void BassAudioRenderer::noteOff(VoiceState& voice,
                                const float releaseSeconds) noexcept {
    if (voice.ampStage == Stage::off || voice.ampStage == Stage::release)
        return;
    voice.ampReleaseStart = voice.ampLevel;
    const float safeRelease = bounded(releaseSeconds, 0.005f, 3.0f);
    voice.ampReleaseMultiplier = static_cast<float>(std::pow(0.0001,
        1.0 / (static_cast<double>(safeRelease) * sampleRate_)));
    voice.filterReleaseStart = voice.filterLevel;
    voice.ampStagePosition = 0.0;
    voice.filterStagePosition = 0.0;
    voice.ampStage = Stage::release;
    voice.filterStage = Stage::release;
}

void BassAudioRenderer::releaseAll(const float releaseSeconds) noexcept {
    for (auto& voice : voices_)
        noteOff(voice, releaseSeconds);
}

float BassAudioRenderer::nextSample(VoiceState& voice,
                                    const double formProgress,
                                    const BassAudioParameters& raw) noexcept {
    const float attack = bounded(raw.attackSeconds, 0.001f, 1.0f);
    const float decay = bounded(raw.decaySeconds, 0.005f, 2.0f);
    const float sustain = bounded(raw.sustain, 0.0f, 1.0f);
    const float release = bounded(raw.releaseSeconds, 0.005f, 3.0f);
    const float filterAttack = bounded(raw.filterAttackSeconds, 0.001f, 1.0f);
    const float filterDecay = bounded(raw.filterDecaySeconds, 0.005f, 2.0f);
    const float filterSustain = bounded(raw.filterSustain, 0.0f, 1.0f);
    const float filterRelease = bounded(raw.filterReleaseSeconds, 0.005f, 3.0f);

    const auto advance = [this, &voice](Stage& stage, float& level, float& attackStart,
                                double& position, float& releaseStart,
                                const float attackTime,
                                const float decayTime, const float sustainLevel,
                                const float releaseTime,
                                const bool exponentialRelease,
                                const float exponentialMultiplier) {
        if (stage == Stage::off)
            return 0.0f;
        const double dt = 1.0 / sampleRate_;
        switch (stage) {
            case Stage::off: return 0.0f;
            case Stage::attack: {
                position += dt;
                const float t = bounded(static_cast<float>(position / attackTime),
                                        0.0f, 1.0f);
                level = attackStart + (1.0f - attackStart) * t;
                if (t >= 1.0f) {
                    stage = Stage::decay;
                    position = 0.0;
                }
                break;
            }
            case Stage::decay: {
                position += dt;
                const float t = bounded(static_cast<float>(position / decayTime),
                                        0.0f, 1.0f);
                level = 1.0f + (sustainLevel - 1.0f) * t;
                if (t >= 1.0f) {
                    stage = Stage::sustain;
                    position = 0.0;
                }
                break;
            }
            case Stage::sustain:
                level = sustainLevel;
                break;
            case Stage::release: {
                position += dt;
                const float t = bounded(static_cast<float>(position / releaseTime),
                                        0.0f, 1.0f);
                if (exponentialRelease)
                    level *= exponentialMultiplier;
                else
                    level = releaseStart * (1.0f - t);
                if (t >= 1.0f) {
                    level = 0.0f;
                    stage = Stage::off;
                    voice.currentMidi = -1;
                    position = 0.0;
                }
                break;
            }
        }
        return level;
    };

    float amp = advance(voice.ampStage, voice.ampLevel,
                              voice.ampAttackStart, voice.ampStagePosition,
                              voice.ampReleaseStart, attack, decay, sustain, release,
                              true, voice.ampReleaseMultiplier);
    const float filterEnv = advance(voice.filterStage, voice.filterLevel,
                                    voice.filterAttackStart,
                                    voice.filterStagePosition,
                                    voice.filterReleaseStart, filterAttack, filterDecay,
                                    filterSustain, filterRelease, false, 0.0f);
    if (voice.currentMidi < 0 || amp <= 0.0f)
        return 0.0f;

    if (voice.lfoCountdown == 0) {
        voice.targetLfoValue = raw.lfoEnabled
            ? lfoWaveValue(formProgress, raw) : 0.0f;
        const double depth = static_cast<double>(
            std::min<int>(raw.lfoDepthSemitones, 12)) / 12.0;
        if (raw.lfoEnabled && raw.lfoTarget == BassLfoTarget::pitch) {
            const double midi = std::clamp(
                static_cast<double>(voice.currentMidi)
                    + voice.targetLfoValue * depth * 12.0,
                24.0, 60.0);
            voice.targetLfoPitchRatio = std::pow(2.0,
                (midi - static_cast<double>(voice.currentMidi)) / 12.0);
        } else {
            voice.targetLfoPitchRatio = 1.0;
        }
        voice.lfoValueStep = (voice.targetLfoValue - voice.lfoValue) / 16.0f;
        voice.lfoPitchRatioStep = (voice.targetLfoPitchRatio
            - voice.lfoPitchRatio) / 16.0;
        voice.lfoCountdown = 16;
    }
    voice.lfoValue += voice.lfoValueStep;
    voice.lfoPitchRatio += voice.lfoPitchRatioStep;
    --voice.lfoCountdown;
    if (raw.lfoEnabled && raw.lfoTarget == BassLfoTarget::amplitude) {
        const double depth = static_cast<double>(
            std::min<int>(raw.lfoDepthSemitones, 12)) / 12.0;
        amp *= static_cast<float>(1.0 + 0.65 * depth * voice.lfoValue);
    }

    float source = 0.0f;
    constexpr std::array<float, 3> weights = {1.0f / 1.5f, 0.34f / 1.5f,
                                               0.16f / 1.5f};
    const auto profile = static_cast<std::size_t>(raw.oscillatorProfile)
        < kOscillatorRatios.size()
        ? static_cast<std::size_t>(raw.oscillatorProfile) : 0u;
    for (std::size_t partial = 0; partial < voice.phases.size(); ++partial) {
        const double partialFrequency = voice.fundamentalHz
            * voice.lfoPitchRatio * kOscillatorRatios[profile][partial];
        if (partialFrequency >= sampleRate_ * 0.45)
            continue;
        source += weights[partial] * static_cast<float>(
            std::sin(kTau * voice.phases[partial]));
        voice.phases[partial] += partialFrequency / sampleRate_;
        if (voice.phases[partial] >= 1.0)
            voice.phases[partial] -= std::floor(voice.phases[partial]);
    }

    const auto materialIndex = static_cast<std::size_t>(raw.material);
    const float materialCutoff = kMaterialCutoffsHz[
        materialIndex < kMaterialCutoffsHz.size() ? materialIndex : 1u];
    const double baseCutoff = materialCutoff
        * bounded(raw.cutoffHz, 30.0f, 6000.0f) / 420.0f;
    const double lfoCutoff = raw.lfoEnabled
        && raw.lfoTarget == BassLfoTarget::filter
        ? voice.lfoValue * std::min<int>(raw.lfoDepthSemitones, 12) * 100.0
        : 0.0;
    const double cutoff = bounded((static_cast<float>(baseCutoff)
        + bounded(raw.filterEnvelopeAmountHz, 0.0f, 5000.0f) * filterEnv
        + static_cast<float>(lfoCutoff)) * voice.eventCutoffScale, 30.0f,
        static_cast<float>(sampleRate_ * 0.40));
    const double resonance = bounded(raw.filterResonance, 0.5f, 14.0f);
    if (voice.filterCoefficientCountdown == 0) {
        const double omega = kTau * cutoff / sampleRate_;
        const double cosine = std::cos(omega);
        const double alpha = std::sin(omega) / (2.0 * resonance);
        const double a0 = 1.0 + alpha;
        voice.filterB0 = (1.0 - cosine) * 0.5 / a0;
        voice.filterB1 = (1.0 - cosine) / a0;
        voice.filterB2 = voice.filterB0;
        voice.filterA1 = -2.0 * cosine / a0;
        voice.filterA2 = (1.0 - alpha) / a0;
        voice.filterCoefficientCountdown = 15;
    } else {
        --voice.filterCoefficientCountdown;
    }
    const double output = voice.filterB0 * source + voice.filterB1 * voice.filterX1
        + voice.filterB2 * voice.filterX2 - voice.filterA1 * voice.filterY1
        - voice.filterA2 * voice.filterY2;
    voice.filterX2 = voice.filterX1;
    voice.filterX1 = source;
    voice.filterY2 = voice.filterY1;
    voice.filterY1 = std::isfinite(output) ? output : 0.0;
    float materialEnvelope = 1.0f;
    if (raw.material == BassMaterial::pluck) {
        materialEnvelope = voice.pluckEnvelope;
        voice.pluckEnvelope = std::max(0.12f,
            voice.pluckEnvelope * voice.pluckEnvelopeStep);
    }
    return static_cast<float>(voice.filterY1) * amp * voice.velocity * materialEnvelope;
}

void BassAudioRenderer::release() noexcept {
    releaseAll(0.12f);
}

void BassAudioRenderer::process(float* left, float* right,
                                const std::size_t frames,
                                const double sectionProgress,
                                const double sectionSeconds,
                                const std::uint64_t formCycleIndex,
                                const bool enabled,
                                const BassAudioParameters& parameters) noexcept {
    if (!ready_ || left == nullptr || right == nullptr || sampleRate_ <= 0.0
        || !std::isfinite(sectionProgress) || !std::isfinite(sectionSeconds)
        || sectionSeconds <= 0.0) {
        return;
    }

    const bool enabling = enabled && !enabledPrevious_;
    if (!enabled && enabledPrevious_)
        releaseAll(parameters.releaseSeconds);
    enabledPrevious_ = enabled;

    double position = std::clamp(sectionProgress, 0.0, 1.0) * sectionSeconds;
    if (previousPosition_ >= 0.0 && position + 1.0 / sampleRate_ < previousPosition_) {
        nextEvent_ = 0;
        releaseAll(parameters.releaseSeconds);
    } else if (previousPosition_ < 0.0 || enabling) {
        nextEvent_ = 0;
        while (nextEvent_ < performance_.count) {
            const double eventTime = performance_.events[nextEvent_].timeSeconds
                * sectionSeconds / std::max(performance_.sectionSeconds, 1.0e-9);
            if (eventTime + 1.0 / sampleRate_ >= position)
                break;
            ++nextEvent_;
        }
    }

    const double dt = 1.0 / sampleRate_;
    const double barDuration = sectionSeconds
        / static_cast<double>(kBassBarsPerSection);
    const double barPosition = position / barDuration;
    const double nearestBar = std::round(barPosition);
    const double boundaryTolerance = 0.5 * dt / barDuration;
    if (hasPendingPerformance_
        && std::abs(barPosition - nearestBar) <= boundaryTolerance)
        applyPendingPerformance(position, sectionSeconds, parameters.releaseSeconds);
    std::uint64_t absoluteFormCycle = formCycleIndex;
    std::uint64_t currentFormCycle = absoluteFormCycle % kBassFormalCycles;
    for (std::size_t frame = 0; frame < frames; ++frame) {
        if (position >= sectionSeconds) {
            position -= sectionSeconds;
            ++absoluteFormCycle;
            currentFormCycle = absoluteFormCycle % kBassFormalCycles;
            nextEvent_ = 0;
            releaseAll(parameters.releaseSeconds);
        }

        const double currentBarPosition = position / barDuration;
        if (hasPendingPerformance_
            && std::abs(currentBarPosition - std::round(currentBarPosition))
                <= boundaryTolerance)
            applyPendingPerformance(position, sectionSeconds, parameters.releaseSeconds);

        // The score itself decides where the bass rests. A fixed C1/C5 gate
        // hid generated notes and made edits seem to have no effect.
        if (enabled) {
            while (nextEvent_ < performance_.count) {
                const auto& event = performance_.events[nextEvent_];
                const double eventTime = event.timeSeconds * sectionSeconds
                    / std::max(performance_.sectionSeconds, 1.0e-9);
                if (eventTime > position + 0.5 * dt)
                    break;
                const bool eventBelongsToCycle = event.formCycle >= 7
                    || event.formCycle == currentFormCycle;
                if (!eventBelongsToCycle) {
                    ++nextEvent_;
                    continue;
                }
                const float passageWeight = developmentEnabled_
                    ? passageEventWeight(developmentSeed_,
                        absoluteFormCycle / kBassFormalCycles, 0,
                        eventTime / sectionSeconds)
                    : 1.0f;
                if (passageWeight <= 0.0f) {
                    ++nextEvent_;
                    continue;
                }
                const int pitchOffsetSemitones = developmentEnabled_
                    ? passagePitchOffset(developmentSeed_,
                        absoluteFormCycle / kBassFormalCycles)
                    : 0;
                const double gate = event.durationSeconds * sectionSeconds
                    / std::max(performance_.sectionSeconds, 1.0e-9);
                VoiceState* target = nullptr;
                const auto stealRank = [](const VoiceState& candidate) {
                    if (candidate.ampStage == Stage::off) return 0;
                    if (candidate.ampStage == Stage::release) return 1;
                    return 2;
                };
                for (auto& voice : voices_) {
                    if (voice.ampStage == Stage::off) {
                        target = &voice;
                        break;
                    }
                    if (target == nullptr || stealRank(voice) < stealRank(*target)
                        || (stealRank(voice) == stealRank(*target)
                            && (voice.ampLevel < target->ampLevel
                                || (voice.ampLevel == target->ampLevel
                                    && voice.age < target->age))))
                        target = &voice;
                }
                if (target != nullptr) {
                    noteOn(*target, std::clamp(
                               static_cast<int>(event.midi) + pitchOffsetSemitones,
                               24, 60),
                           std::min(1.0f, event.velocity * passageWeight), gate,
                           event.dynamicsMapState, event.pan);
                    const double articulatedGate = gate
                        * bounded(parameters.gateRatio, 0.10f, 1.0f)
                        * (event.dynamicsMapState >= 0.0f
                            ? 0.28 + 0.72 * static_cast<double>(
                                event.dynamicsMapState)
                            : 1.0);
                    target->gateEnd = std::min(sectionSeconds,
                        eventTime + articulatedGate);
                }
                ++nextEvent_;
            }
        }

        float bassLeft = 0.0f;
        float bassRight = 0.0f;
        for (auto& voice : voices_) {
            if (voice.currentMidi >= 0 && voice.ampStage != Stage::release
                && position >= voice.gateEnd)
                noteOff(voice, parameters.releaseSeconds);
            float voiceSample = 0.0f;
            if (voice.currentMidi >= 0)
                voiceSample = nextSample(voice,
                    static_cast<double>(absoluteFormCycle)
                        + position / sectionSeconds, parameters);
            voice.lastOutput = voiceSample;
            if (voice.stealFadeRemaining > 0 && voice.stealFadeTotal > 0) {
                voiceSample += voice.stealResidual
                    * static_cast<float>(voice.stealFadeRemaining)
                    / static_cast<float>(voice.stealFadeTotal);
                --voice.stealFadeRemaining;
                if (voice.stealFadeRemaining == 0) {
                    voice.stealResidual = 0.0f;
                    voice.stealFadeTotal = 0;
                }
            }
            const float effectivePan = std::clamp(
                voice.pan + bounded(parameters.channelPan, -1.0f, 1.0f),
                -1.0f, 1.0f);
            bassLeft += voiceSample * std::sqrt(1.0f - effectivePan);
            bassRight += voiceSample * std::sqrt(1.0f + effectivePan);
        }
        const float gain = bounded(parameters.gain, 0.0f, 0.8f);
        left[frame] += bassLeft * gain;
        right[frame] += bassRight * gain;
        position += dt;
    }
    previousPosition_ = std::fmod(position, sectionSeconds);
}

} // namespace batruc
