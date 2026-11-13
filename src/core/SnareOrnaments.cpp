// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo
#include "core/SnareOrnaments.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

namespace batruc {
namespace {

// escolha determinística: o ataque i recebe o ornamento?
float unitHash(std::uint32_t seed, std::uint32_t index, std::uint32_t salt) noexcept {
    std::uint32_t z = seed ^ (index * 0x9E3779B9u) ^ (salt * 0x85EBCA6Bu);
    z = (z ^ (z >> 16)) * 0x7FEB352Du;
    z = (z ^ (z >> 15)) * 0x846CA68Bu;
    z ^= z >> 16;
    return static_cast<float>(z >> 8) / 16777216.0f;
}

// toques de UM ornamento numa nota (posição, força, próximo ataque escrito);
// usado pela inserção antiga e pela expansão da nota marcada
template <typename Add>
void ornamentStrokes(const SnareOrnament kind, const double position, const float strength,
                     const double nextPosition, const float amount, const double perSecond,
                     const double step, Add&& add) {
    switch (kind) {
        case SnareOrnament::flam:
        case SnareOrnament::drag:
        case SnareOrnament::ruff: {
            // enfeites antes da principal: (segundos antes, fração do acento)
            static constexpr double kFlam[1][2] = {{0.025, 0.32}};
            static constexpr double kDrag[2][2] = {{0.060, 0.28}, {0.030, 0.32}};
            static constexpr double kRuff[3][2] = {{0.075, 0.24}, {0.050, 0.28}, {0.025, 0.32}};
            const double (*graces)[2] = kind == SnareOrnament::flam ? kFlam
                : kind == SnareOrnament::drag ? kDrag : kRuff;
            const int graceCount = kind == SnareOrnament::flam ? 1
                : kind == SnareOrnament::drag ? 2 : 3;
            for (int g = 0; g < graceCount; ++g)
                add(position - graces[g][0] * perSecond,
                    strength * static_cast<float>(graces[g][1]));
            break;
        }
        case SnareOrnament::ricochet: {
            const int rebounds = 3 + static_cast<int>(std::lround(5.0f * amount));
            const double limit = nextPosition - 0.02 * perSecond;
            double t = position;
            double interval = 0.070;
            float s = strength * 0.6f;
            for (int k = 0; k < rebounds; ++k) {
                t += interval * perSecond;
                if (t >= limit || s < 0.05f) break;
                add(t, s);
                interval *= 0.8;    // acelera
                s *= 0.6f;          // perde força
            }
            break;
        }
        case SnareOrnament::roll: {
            const double beat = 4.0 * step;
            const double end = std::min(nextPosition - 0.5 * step, position + beat);
            const double thirtySecond = 0.5 * step;
            const int strokes = static_cast<int>((end - position) / thirtySecond);
            for (int k = 1; k < strokes; ++k) {
                const float crescendo = static_cast<float>(k) / static_cast<float>(strokes);
                add(position + k * thirtySecond, strength * (0.30f + 0.40f * crescendo));
            }
            break;
        }
        case SnareOrnament::buzz: {
            // toque múltiplo: 4 rebotes a ~12 ms, perdendo força, antes do próximo
            const double limit = nextPosition - 0.01 * perSecond;
            float s = strength * 0.55f;
            for (int k = 1; k <= 4; ++k) {
                const double t = position + k * 0.012 * perSecond;
                if (t >= limit) break;
                add(t, s);
                s *= 0.75f;
            }
            break;
        }
        case SnareOrnament::ghostNotes:
        case SnareOrnament::rim:
            break;
    }
}

bool marksNote(const SnareOrnament kind) noexcept {
    return kind == SnareOrnament::flam || kind == SnareOrnament::drag
        || kind == SnareOrnament::ruff || kind == SnareOrnament::ricochet
        || kind == SnareOrnament::roll || kind == SnareOrnament::buzz;
}

} // namespace

const char* snareOrnamentName(const SnareOrnament kind) noexcept {
    switch (kind) {
        case SnareOrnament::ghostNotes: return "notas fantasmas";
        case SnareOrnament::flam: return "flam";
        case SnareOrnament::drag: return "drag";
        case SnareOrnament::ruff: return "rufo";
        case SnareOrnament::ricochet: return "ricochete";
        case SnareOrnament::roll: return "rulo";
        case SnareOrnament::rim: return "aro";
        case SnareOrnament::buzz: return "toque múltiplo";
    }
    return "";
}

int ornamentSnareCell(RhythmicCell& cell, const SnareOrnamentParameters& p) noexcept {
    if (!cell.enabled || !(p.sectionSeconds > 0.1) || p.stepsPerSection < 4
        || !std::isfinite(p.amount))
        return 0;
    const float amount = std::clamp(p.amount, 0.0f, 1.0f);
    const auto salt = static_cast<std::uint32_t>(p.kind) + 1u;
    const double perSecond = 1.0 / p.sectionSeconds;   // segundos → posição
    const double step = 1.0 / static_cast<double>(p.stepsPerSection);

    // os ataques originais (as inserções não mexem neles)
    struct Attack { double position; float strength; };
    std::vector<Attack> original;
    original.reserve(cell.count);
    for (std::size_t i = 0; i < cell.count; ++i)
        original.push_back({cell.positions[i], cell.strengths[i]});
    // acento é relativo à célula: pelo menos 75 % do ataque mais forte
    float strongest = 0.0f;
    for (const auto& a : original) strongest = std::max(strongest, a.strength);
    const float accentThreshold = 0.75f * strongest;
    auto nextPosition = [&](std::size_t i) {
        return i + 1 < original.size() ? original[i + 1].position : 1.0;
    };
    int changed = 0;
    auto add = [&](double position, float strength) {
        if (position < 0.0 || position >= 1.0) return;
        if (insertRhythmicCellAttack(cell, position, std::clamp(strength, 0.03f, 1.0f)))
            ++changed;
    };

    switch (p.kind) {
        case SnareOrnament::ghostNotes:
            for (int s = 0; s < p.stepsPerSection; ++s) {
                if (s % 4 == 0) continue;   // fora dos tempos
                const double position = s * step;
                bool near = false;
                for (const auto& a : original)
                    near = near || std::abs(a.position - position) < 0.5 * step;
                if (near || unitHash(p.seed, static_cast<std::uint32_t>(s), salt) >= 0.6f * amount)
                    continue;
                add(position, 0.15f + 0.15f * unitHash(p.seed, static_cast<std::uint32_t>(s), salt + 17u));
            }
            break;
        case SnareOrnament::flam:
        case SnareOrnament::drag:
        case SnareOrnament::ruff:
        case SnareOrnament::ricochet:
        case SnareOrnament::roll:
        case SnareOrnament::buzz:
            for (std::size_t i = 0; i < original.size(); ++i) {
                // o flam prefere acentos; os demais valem para qualquer nota
                const bool eligible = p.kind != SnareOrnament::flam
                    || original[i].strength >= accentThreshold;
                if (!eligible || unitHash(p.seed, static_cast<std::uint32_t>(i), salt) >= amount)
                    continue;
                ornamentStrokes(p.kind, original[i].position, original[i].strength,
                                nextPosition(i), amount, perSecond, step, add);
            }
            break;
        case SnareOrnament::rim:
            for (std::size_t i = 0; i < cell.count; ++i) {
                if (cell.strengths[i] >= accentThreshold || cell.midiNotes[i] != kNoMidiNote)
                    continue;
                if (unitHash(p.seed, static_cast<std::uint32_t>(i), salt) >= amount)
                    continue;
                cell.midiNotes[i] = 37;   // GM side stick: articulação de aro
                ++changed;
            }
            break;
    }
    return changed;
}

int markSnareOrnaments(RhythmicCell& cell, const SnareOrnamentParameters& p) noexcept {
    if (!marksNote(p.kind)) {
        // fantasmas e aro escrevem como antes, e a nota afetada fica marcada
        const RhythmicCell before = cell;
        const int changed = ornamentSnareCell(cell, p);
        const auto code = static_cast<std::uint8_t>(static_cast<int>(p.kind) + 1);
        for (std::size_t i = 0; i < cell.count; ++i) {
            bool existed = false;
            std::size_t j = 0;
            for (; j < before.count; ++j)
                if (std::abs(before.positions[j] - cell.positions[i]) < 1.0e-12) { existed = true; break; }
            if (p.kind == SnareOrnament::ghostNotes && !existed)
                cell.ornaments[i] = code;
            if (p.kind == SnareOrnament::rim && existed && cell.ornaments[i] == 0
                && before.midiNotes[j] != cell.midiNotes[i])
                cell.ornaments[i] = code;
        }
        return changed;
    }
    if (!cell.enabled || !std::isfinite(p.amount))
        return 0;
    const float amount = std::clamp(p.amount, 0.0f, 1.0f);
    const auto salt = static_cast<std::uint32_t>(p.kind) + 1u;
    float strongest = 0.0f;
    for (std::size_t i = 0; i < cell.count; ++i) strongest = std::max(strongest, cell.strengths[i]);
    int changed = 0;
    for (std::size_t i = 0; i < cell.count; ++i) {
        const bool eligible = p.kind != SnareOrnament::flam
            || cell.strengths[i] >= 0.75f * strongest;
        if (!eligible || unitHash(p.seed, static_cast<std::uint32_t>(i), salt) >= amount)
            continue;
        // nota fantasma e nota de aro não recebem outro enfeite por cima
        SnareOrnament existing;
        if (cellOrnament(cell, i, existing)
            && (existing == SnareOrnament::ghostNotes || existing == SnareOrnament::rim))
            continue;
        cell.ornaments[i] = static_cast<std::uint8_t>(static_cast<int>(p.kind) + 1);
        ++changed;
    }
    return changed;
}

bool cellOrnament(const RhythmicCell& cell, const std::size_t index, SnareOrnament& kind) noexcept {
    if (index >= cell.count || cell.ornaments[index] == 0
        || cell.ornaments[index] > kSnareOrnamentCount)
        return false;
    kind = static_cast<SnareOrnament>(cell.ornaments[index] - 1);
    return true;
}

void expandSnareOrnaments(const RhythmicCell& written, RhythmicCell& performed,
                          const double sectionSeconds, const int stepsPerSection,
                          const bool enabled) noexcept {
    performed = written;
    if (!enabled) {   // batida normal: tira fantasmas marcadas, aro volta ao normal
        for (std::size_t i = performed.count; i-- > 0;) {
            SnareOrnament kind;
            if (!cellOrnament(performed, i, kind)) continue;
            if (kind == SnareOrnament::ghostNotes)
                (void) removeRhythmicCellAttack(performed, i);
            else if (kind == SnareOrnament::rim)
                performed.midiNotes[i] = kNoMidiNote;
        }
        for (std::size_t i = 0; i < performed.count; ++i) performed.ornaments[i] = 0;
        return;
    }
    for (std::size_t i = 0; i < performed.count; ++i) performed.ornaments[i] = 0;
    if (!written.enabled || !(sectionSeconds > 0.1) || stepsPerSection < 4)
        return;
    const double perSecond = 1.0 / sectionSeconds;
    const double step = 1.0 / static_cast<double>(stepsPerSection);
    auto add = [&](double position, float strength) {
        if (position < 0.0 || position >= 1.0) return;
        (void) insertRhythmicCellAttack(performed, position, std::clamp(strength, 0.03f, 1.0f));
    };
    for (std::size_t i = 0; i < written.count; ++i) {
        SnareOrnament kind;
        if (!cellOrnament(written, i, kind)) continue;
        const double next = i + 1 < written.count ? written.positions[i + 1] : 1.0;
        // amount 0,6: ricochete de 6 rebotes (o tamanho padrão)
        ornamentStrokes(kind, written.positions[i], written.strengths[i], next,
                        0.6f, perSecond, step, add);
    }
}

int seedSnareVocabulary(const std::uint32_t seed, SnareOrnament (&kinds)[2], float& amount) noexcept {
    std::uint32_t state = seed ^ 0x534E5231u;   // "SNR1"
    const auto vocabulary = static_cast<int>(nextSeedWord(state) % 6u);
    amount = 0.25f + 0.35f * static_cast<float>(nextSeedWord(state) % 1000u) / 1000.0f;
    static constexpr SnareOrnament kVocabulary[6][2] = {
        {SnareOrnament::ghostNotes, SnareOrnament::ghostNotes},
        {SnareOrnament::ghostNotes, SnareOrnament::flam},
        {SnareOrnament::ghostNotes, SnareOrnament::drag},
        {SnareOrnament::ricochet, SnareOrnament::ricochet},
        {SnareOrnament::roll, SnareOrnament::roll},
        {SnareOrnament::ruff, SnareOrnament::rim}};
    kinds[0] = kVocabulary[vocabulary][0];
    kinds[1] = kVocabulary[vocabulary][1];
    return vocabulary;
}

const char* snareIdiomName(const SnareIdiom idiom) noexcept {
    switch (idiom) {
        case SnareIdiom::frevo: return "frevo";
        case SnareIdiom::maracatu: return "maracatu";
        case SnareIdiom::baiao: return "baião";
    }
    return "";
}

int applySnareIdiom(RhythmicCell& cell, const SnareIdiom idiom, const int beatsPerBar,
                    const int beatSteps, const int bars, const std::uint32_t seed) noexcept {
    if (beatsPerBar < 1 || beatSteps < 1 || bars < 1 || bars > 16)
        return 0;
    cell.enabled = true;
    const int barSteps = beatsPerBar * beatSteps;
    const double total = static_cast<double>(barSteps * bars);
    int changed = 0;
    // garante uma nota no passo (ou acha a que já está) e devolve o índice
    auto noteAt = [&](const int step, const float strength) -> int {
        const double position = step / total;
        for (std::size_t i = 0; i < cell.count; ++i)
            if (std::abs(cell.positions[i] - position) < 0.25 / total) {
                cell.strengths[i] = std::max(cell.strengths[i], strength);
                return static_cast<int>(i);
            }
        std::size_t inserted = 0;
        if (insertRhythmicCellAttack(cell, position, strength, &inserted)) {
            ++changed;
            return static_cast<int>(inserted);
        }
        return -1;
    };
    const auto mark = [&](const int index, const SnareOrnament kind) {
        if (index < 0) return;
        cell.ornaments[static_cast<std::size_t>(index)] =
            static_cast<std::uint8_t>(static_cast<int>(kind) + 1);
        ++changed;
    };
    switch (idiom) {
        case SnareIdiom::frevo:
            // fim de cada frase de 2 compassos: rulo de 1 tempo, acentuado
            for (int bar = 1; bar < bars; bar += 2)
                mark(noteAt(bar * barSteps + (beatsPerBar - 1) * beatSteps, 0.92f),
                     SnareOrnament::roll);
            break;
        case SnareIdiom::maracatu:
            // em cada tempo, a 2ª nota escrita ganha toque múltiplo e acento
            for (int beat = 0; beat < beatsPerBar * bars; ++beat) {
                const double start = beat * beatSteps / total;
                const double end = (beat + 1) * beatSteps / total;
                int seen = 0;
                for (std::size_t i = 0; i < cell.count; ++i) {
                    if (cell.positions[i] < start - 1e-9 || cell.positions[i] >= end - 1e-9) continue;
                    if (++seen == 2) {
                        cell.strengths[i] = std::max(cell.strengths[i], 0.9f);
                        mark(static_cast<int>(i), SnareOrnament::buzz);
                        break;
                    }
                }
            }
            break;
        case SnareIdiom::baiao: {
            // acento na 2ª colcheia do 2º tempo; fantasmas nas síncopes
            const int eighth = beatSteps / 2 > 0 ? beatSteps / 2 : 1;
            for (int bar = 0; bar < bars && beatsPerBar >= 2; ++bar)
                noteAt(bar * barSteps + beatSteps + eighth, 0.92f);
            SnareOrnamentParameters ghosts;
            ghosts.kind = SnareOrnament::ghostNotes;
            ghosts.stepsPerSection = barSteps * bars;
            ghosts.amount = 0.35f;
            ghosts.seed = seed;
            changed += markSnareOrnaments(cell, ghosts);
            break;
        }
    }
    return changed;
}

const char* microtimingProfileName(const MicrotimingProfile profile) noexcept {
    switch (profile) {
        case MicrotimingProfile::tarolBaqueSolto: return "tarol · baque solto";
        case MicrotimingProfile::bomboBaqueSolto: return "bombo · baque solto";
    }
    return "";
}

int applyMicrotimingProfile(RhythmicCell& cell, const MicrotimingProfile profile,
                            const int beatsPerSection, const float depth) noexcept {
    if (!cell.enabled || beatsPerSection < 1 || !std::isfinite(depth))
        return 0;
    const double d = std::clamp(static_cast<double>(depth), 0.0, 1.0);
    // posição medida da semicolcheia k (0–3) dentro do tempo
    static constexpr double kTarol[4] = {0.0, 0.25, 0.47, 0.715};
    static constexpr double kBombo[4] = {0.0, 0.25, 0.46, 0.69};
    const double* measured = profile == MicrotimingProfile::tarolBaqueSolto ? kTarol : kBombo;
    const double beat = 1.0 / static_cast<double>(beatsPerSection);
    const double sixteenth = beat / 4.0;
    RhythmicCell next = cell;
    int moved = 0;
    for (std::size_t i = 0; i < next.count; ++i) {
        const double position = cell.positions[i];
        const double beatStart = std::floor(position / beat + 1e-9) * beat;
        const double inBeat = (position - beatStart) / sixteenth;
        const int k = static_cast<int>(std::lround(inBeat));
        if (k < 2 || k > 3 || std::abs(inBeat - k) > 0.3) continue;   // só 3ª e 4ª
        const double target = beatStart + beat * (k * 0.25 + d * (measured[k] - k * 0.25));
        if (std::abs(target - position) < 1e-12) continue;
        next.positions[i] = target;
        ++moved;
    }
    if (moved == 0 || !validRhythmicCell(next))
        return 0;
    cell = next;
    return moved;
}

} // namespace batruc
