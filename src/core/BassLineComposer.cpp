// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
#include "core/BassLineComposer.hpp"

#include "core/BassAudio.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <vector>

namespace batruc {
namespace {

std::uint32_t mix(std::uint32_t& state) noexcept {   // splitmix32
    state += 0x9E3779B9u;
    std::uint32_t z = state;
    z = (z ^ (z >> 16)) * 0x85EBCA6Bu;
    z = (z ^ (z >> 13)) * 0xC2B2AE35u;
    return z ^ (z >> 16);
}
double unit(std::uint32_t& state) noexcept {
    return static_cast<double>(mix(state) >> 8) / 16777216.0;
}

// progressões de graus (0 = I), um acorde por compasso; a seção tem 4
constexpr int kProgressionCount = 10;
constexpr int kProgressions[kProgressionCount][4] = {
    {0, 0, 3, 4},   // I I IV V
    {0, 3, 4, 0},   // I IV V I
    {0, 5, 3, 4},   // I vi IV V
    {0, 4, 5, 3},   // I V vi IV
    {0, 6, 5, 6},   // i VII VI VII (andaluz invertido, modal)
    {0, 6, 3, 0},   // I bVII IV I (mixolídio)
    {0, 3, 0, 4},   // I IV I V (blues curto)
    {0, 0, 0, 0},   // pedal / vamp
    {0, 2, 3, 4},   // I iii IV V
    {0, 3, 1, 4}};  // I IV ii V
// graus do modo da seed (no dórico, "1-7-6-7" é i–VII–VI–VII; no lídio,
// I–vii°–vi–vii°): por isso o nome em números
constexpr const char* kProgressionNames[kProgressionCount] = {
    "1-1-4-5", "1-4-5-1", "1-6-4-5", "1-5-6-4", "1-7-6-7",
    "1-7-4-1", "1-4-1-5", "pedal 1", "1-3-4-5", "1-4-2-5"};

struct Scale {
    int tones[8]{};
    int size = 0;
    int root = 28;   // fundamental grave (MIDI 28–39)

    int midi(const int index) const noexcept {
        const int octave = index >= 0 ? index / size : -((-index + size - 1) / size);
        const int inside = index - octave * size;
        return root + tones[inside] + 12 * octave;
    }
    // grau diatônico 0–6 → índice na escala (pentatônicas e blues: o grau
    // do modo paralelo mais próximo)
    int degreeIndex(const int degree) const noexcept {
        if (size == 7) return degree;
        bool major = false;
        for (int i = 0; i < size; ++i) major = major || tones[i] == 4;
        static constexpr int kMajor[7] = {0, 2, 4, 5, 7, 9, 11};
        static constexpr int kMinor[7] = {0, 2, 3, 5, 7, 8, 10};
        const int target = (major ? kMajor : kMinor)[degree % 7];
        int best = 0;
        for (int i = 1; i < size; ++i)
            if (std::abs(tones[i] - target) < std::abs(tones[best] - target)) best = i;
        return best;
    }
    // a altura do índice (± oitavas) mais próxima de `near`, no registro
    int nearest(const int index, const int near) const noexcept {
        int best = midi(index);
        for (int k = -3; k <= 3; ++k) {
            const int candidate = midi(index + k * size);
            if (candidate < 28 || candidate > 55) continue;
            if (best < 28 || best > 55 || std::abs(candidate - near) < std::abs(best - near))
                best = candidate;
        }
        return std::clamp(best, 28, 55);
    }
};

enum class SlotKind : std::uint8_t { downbeat, strong, support, answer };
struct Slot { int step = 0; SlotKind kind = SlotKind::downbeat; float kick = 0.0f; };

} // namespace

void seedKey(const std::uint32_t compositionSeed, int& tonic, int& scale) noexcept {
    std::uint32_t keyState = compositionSeed ^ 0x4B455931u;   // "KEY1"
    tonic = static_cast<int>(nextSeedWord(keyState) % 12u);
    scale = static_cast<int>(nextSeedWord(keyState)
        % static_cast<std::uint32_t>(kBassKeyScaleCount));
}

bool composeBassLine(const FigureComposition& figures,
                     const BassLineParameters& p,
                     BassPerformance& performance,
                     std::string* summary) {
    performance = {};
    if (p.barsPerSection != 4 || p.stepsPerBar < 4 || p.stepsPerBar > 32
        || !(p.sectionSeconds > 0.5 && p.sectionSeconds <= 60.0)
        || p.keyTonic < 0 || p.keyTonic > 11
        || p.keyScale < 0 || p.keyScale >= kBassKeyScaleCount)
        return false;

    Scale scale;
    for (const int tone : kBassKeyScales[static_cast<std::size_t>(p.keyScale)])
        if (tone >= 0) scale.tones[scale.size++] = tone;
    scale.root = 24 + p.keyTonic;
    if (scale.root < 28) scale.root += 12;

    // genes do baixo (separados dos genes das figuras)
    std::uint32_t state = p.seed ^ 0x42534C4Eu;   // "BSLN"
    const int progression = static_cast<int>(mix(state) % kProgressionCount);
    const double support = 0.35 + 0.5 * unit(state);    // apoio ao bumbo
    const double answer = 0.25 + 0.5 * unit(state);     // resposta nas lacunas
    const double legato = 0.55 + 0.4 * unit(state);
    const int substitutedBar = 1 + static_cast<int>(mix(state) % 3u);

    const int steps = p.stepsPerBar;
    const int bars = p.barsPerSection;
    const int sectionSteps = steps * bars;
    const double stepSeconds = p.sectionSeconds / sectionSteps;
    int previous = scale.midi(0);
    std::size_t written = 0;

    for (std::size_t cycle = 0; cycle < kFormalCycleCount; ++cycle) {
        // graus do ciclo: a progressão, com o desenvolvimento da forma
        int degrees[4];
        for (int b = 0; b < 4; ++b) degrees[b] = kProgressions[progression][b];
        if (cycle == 3)   // troca pelo relativo (I→vi, IV→ii, V→iii)
            degrees[substitutedBar] = (degrees[substitutedBar] + 5) % 7;
        if (cycle == kFormalCycleCount - 1)   // termina na dominante
            degrees[3] = 4;
        const bool breath = cycle == 2;

        // ataques do bumbo (V1) do ciclo, em passos
        std::vector<std::pair<int, float>> kicks;
        const auto& cell = figures.cells[cycle][0];
        if (cell.enabled)
            for (std::size_t i = 0; i < cell.count; ++i) {
                const int step = static_cast<int>(std::lround(cell.positions[i] * sectionSteps));
                if (step >= 0 && step < sectionSteps)
                    kicks.emplace_back(step, cell.strengths[i]);
            }

        std::vector<Slot> slots;
        for (int b = 0; b < bars; ++b) {
            const int barStart = b * steps;
            std::vector<Slot> bar{{barStart, SlotKind::downbeat, 1.0f}};
            if (breath) {
                if (steps % 2 == 0)
                    bar.push_back({barStart + steps / 2, SlotKind::strong, 0.0f});
            } else {
                std::vector<int> barKicks{barStart};
                for (const auto& [step, strength] : kicks) {
                    if (step <= barStart || step >= barStart + steps) continue;
                    barKicks.push_back(step);
                    if (unit(state) < support)
                        bar.push_back({step, SlotKind::support, strength});
                }
                barKicks.push_back(barStart + steps);
                for (std::size_t k = 0; k + 1 < barKicks.size(); ++k) {
                    const int gap = barKicks[k + 1] - barKicks[k];
                    if (gap < 6 || unit(state) >= answer) continue;
                    // no contratempo de colcheia (meio do tempo)
                    int middle = barKicks[k] + gap / 2;
                    middle -= middle % 2;
                    if (middle % 4 == 0) middle += 2;
                    if (middle >= barKicks[k + 1] - 1) continue;
                    bar.push_back({middle, SlotKind::answer, 0.0f});
                }
            }
            std::sort(bar.begin(), bar.end(),
                      [](const Slot& a, const Slot& b2) { return a.step < b2.step; });
            for (const auto& slot : bar)
                if (slots.empty() || slot.step > slots.back().step)
                    slots.push_back(slot);
        }

        for (std::size_t j = 0; j < slots.size(); ++j) {
            const auto& slot = slots[j];
            const int bar = slot.step / steps;
            const int local = slot.step % steps;
            const int degree = scale.degreeIndex(degrees[bar]);
            const int nextBar = bar + 1 < bars ? bar + 1 : -1;
            const int nextDegree = nextBar >= 0
                ? scale.degreeIndex(degrees[nextBar])
                : scale.degreeIndex(kProgressions[progression][0]);
            const bool lastInBar = j + 1 == slots.size()
                || slots[j + 1].step / steps != bar;
            int midi;
            float velocity;
            if (local == 0) {
                // o tempo forte puxa a linha de volta ao centro do registro
                // (a fundamental + uma quinta), para o contorno não derivar
                midi = scale.nearest(degree, (previous + scale.root + 7) / 2);
                velocity = 0.86f;
            } else if (lastInBar && nextDegree != degree && unit(state) < 0.75) {
                // passagem: vizinha de escala da próxima fundamental
                const int target = scale.nearest(nextDegree, previous);
                const int neighbour = unit(state) < 0.5 ? nextDegree - 1 : nextDegree + 1;
                midi = scale.nearest(neighbour, target);
                velocity = 0.62f;
            } else if (slot.kind == SlotKind::strong || local % (steps / 2) == 0) {
                midi = scale.nearest(unit(state) < 0.6 ? degree : degree + 4, previous);
                velocity = 0.74f;
            } else {
                const int choices[3] = {degree, degree + 2, degree + 4};
                int best = -1;
                int bestDistance = 1000;
                const int start = static_cast<int>(mix(state) % 3u);
                for (int c = 0; c < 3; ++c) {
                    const int candidate = scale.nearest(choices[(start + c) % 3], previous);
                    const int distance = std::abs(candidate - previous)
                        + (candidate == previous ? 4 : 0);
                    if (distance < bestDistance) { bestDistance = distance; best = candidate; }
                }
                midi = best;
                velocity = slot.kind == SlotKind::answer ? 0.56f
                    : 0.64f + 0.14f * std::clamp(slot.kick, 0.0f, 1.0f);
            }
            const int nextStep = j + 1 < slots.size() ? slots[j + 1].step : sectionSteps;
            const double gap = (nextStep - slot.step) * stepSeconds;
            const double share = slot.kind == SlotKind::answer ? 0.5 : legato;
            if (written >= performance.events.size()) break;
            auto& event = performance.events[written++];
            event.timeSeconds = slot.step * stepSeconds;
            event.durationSeconds = std::max(0.06, gap * share);
            event.midi = static_cast<std::uint8_t>(midi);
            event.velocity = velocity;
            event.sourceNote = static_cast<std::uint16_t>(slot.step);
            event.formCycle = static_cast<std::uint8_t>(cycle);
            event.pan = 0.0f;
            previous = midi;
        }
    }
    performance.count = written;
    performance.sectionSeconds = p.sectionSeconds;
    sanitizeBassPerformance(performance);

    if (summary != nullptr) {
        std::ostringstream out;
        out << "baixo: tom " << bassKeyTonicName(p.keyTonic) << " "
            << bassKeyScaleName(p.keyScale) << " · progressão "
            << kProgressionNames[progression] << " · apoio "
            << std::lround(support * 100) << "% · resposta "
            << std::lround(answer * 100) << "% · legato "
            << std::lround(legato * 100) << "% · notas " << written;
        *summary = out.str();
    }
    return written > 0;
}

} // namespace batruc
