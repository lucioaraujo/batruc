// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace batruc {

struct PassageDevelopmentState {
    double memory = 0.0;
    unsigned rotation = 0;
    unsigned restCell = 0;
    bool inverted = false;
};

// The seven-cycle gesture is the local form inside a passage. This second,
// slower layer gives a long performance a recognizable return path while
// keeping one seed and one source composition throughout.
enum class PassageRole : std::uint8_t {
    A,
    B,
    APrime,
    C,
    BPrime,
    Expansion,
    CPrime,
    ReturnA
};

inline constexpr std::size_t kPassageRoleCount = 8;

inline std::uint32_t passageHash(std::uint32_t value) noexcept {
    value ^= value >> 16u;
    value *= 0x7feb352du;
    value ^= value >> 15u;
    value *= 0x846ca68bu;
    value ^= value >> 16u;
    return value;
}

// Keep the opening readable across seed changes, then let the seed select
// deterministic developments for the second half of the form. Every passage
// remains active; contrast comes from density and balance, never a hidden
// whole-passage pause.
inline PassageRole passageRoleFor(const std::uint32_t seed,
                                 const std::uint64_t passage) noexcept {
    const auto slot = static_cast<std::size_t>(passage % kPassageRoleCount);
    constexpr PassageRole opening[4] = {
        PassageRole::A, PassageRole::B, PassageRole::APrime, PassageRole::C
    };
    if (slot < 4)
        return opening[slot];

    constexpr PassageRole endings[4][4] = {
        {PassageRole::BPrime, PassageRole::Expansion,
         PassageRole::CPrime, PassageRole::ReturnA},
        {PassageRole::CPrime, PassageRole::BPrime,
         PassageRole::Expansion, PassageRole::ReturnA},
        {PassageRole::Expansion, PassageRole::BPrime,
         PassageRole::C, PassageRole::ReturnA},
        {PassageRole::BPrime, PassageRole::CPrime,
         PassageRole::Expansion, PassageRole::ReturnA}
    };
    const auto route = passageHash(seed ^ 0x6d2b79f5u) & 3u;
    return endings[route][slot - 4];
}

// A bounded eight-passage memory. It is recalculated at preparation/event
// boundaries and never allocates. The leaky recurrence lets the current phrase
// inherit tension from its predecessors, while a new macro-cycle receives a
// reproducible integration constant from the seed and macro index.
inline PassageDevelopmentState passageDevelopmentState(
    const std::uint32_t seed, const std::uint64_t passage) noexcept {
    PassageDevelopmentState state;
    const auto macro = static_cast<std::uint32_t>(passage / 8u);
    const auto localPassage = static_cast<unsigned>(passage % 8u);
    const auto constant = passageHash(seed ^ (macro * 0x9e3779b9u));
    state.memory = (static_cast<double>(constant & 0xffffu) / 65535.0 - 0.5)
        * 0.55;
    for (unsigned index = 0; index <= localPassage; ++index) {
        const auto impulseHash = passageHash(constant
            ^ (0x85ebca6bu * static_cast<std::uint32_t>(index + 1u)));
        const double impulse = static_cast<double>(impulseHash & 0xffffu)
            / 32767.5 - 1.0;
        state.memory = std::clamp(0.68 * state.memory + 0.46 * impulse,
                                  -1.0, 1.0);
    }
    const auto decision = passageHash(constant
        ^ static_cast<std::uint32_t>(localPassage * 0xc2b2ae35u));
    state.rotation = static_cast<unsigned>((decision >> 3u) & 7u);
    state.restCell = static_cast<unsigned>((decision >> 9u) & 7u);
    state.inverted = ((decision >> 17u) & 1u) != 0u;
    return state;
}

// A new traversal develops the bass register from the same bounded memory
// used by the phrase grammar. Passage zero remains the seed's written pitch;
// later traversals move by one or two semitones, with inversion determining
// the direction. This is intentionally a small register displacement rather
// than a scale quantizer: generated notes retain their own interval logic.
inline int passagePitchOffset(const std::uint32_t seed,
                              const std::uint64_t passage) noexcept {
    if (passage == 0u)
        return 0;
    const auto state = passageDevelopmentState(seed, passage);
    const int direction = state.memory < 0.0 ? -1 : 1;
    const int magnitude = std::abs(state.memory) >= 0.62 ? 2 : 1;
    return direction * magnitude * (state.inverted ? -1 : 1);
}

// A seven-cycle form is one large passage. Across long-form playback these
// passages make an eight-part A/B/A'/C/B'/C'/variation/return arc. No role
// silences a passage. One role is
// shared by the voices and bass; it changes event weight, not the underlying
// event schedule or written seed. Passage memory supplies deterministic,
// bounded differences while keeping A and its returns recognizable.
inline float passageEventWeight(const std::uint32_t seed,
                                const std::uint64_t passage,
                                const std::size_t voice,
                                const double sectionProgress) noexcept {
    if (passage == 0 || !std::isfinite(sectionProgress))
        return 1.0f;
    const double position = std::clamp(sectionProgress, 0.0, 0.999999);
    const unsigned sourceSlot = static_cast<unsigned>(position * 8.0);
    const auto state = passageDevelopmentState(seed, passage);
    const unsigned directedSlot = state.inverted ? 7u - sourceSlot : sourceSlot;
    const unsigned slot = (directedSlot + state.rotation) & 7u;
    const unsigned rotation = ((seed >> 5u) + state.rotation) & 1u;
    const float memoryAccent = static_cast<float>(1.0 + 0.12 * state.memory
        * (((slot + voice) & 1u) == 0u ? 1.0 : -1.0));
    float roleWeight = 1.0f;
    switch (passageRoleFor(seed, passage)) {
        case PassageRole::B: { // Call and response, in eighth-section cells.
            const bool callerSlot = ((slot + rotation) & 1u) == 0u;
            const bool caller = voice == 0u;
            roleWeight = callerSlot == caller ? 1.15f : 0.38f;
            break;
        }
        case PassageRole::APrime: { // A returns with a softened seeded cell.
            const unsigned restSlot = (state.restCell
                + static_cast<unsigned>(voice * 3u)) & 7u;
            roleWeight = slot == restSlot ? 0.82f
                : (((slot + voice + rotation) & 3u) == 0u ? 1.12f : 0.98f);
            break;
        }
        case PassageRole::C: { // Shift weight from caller to the other voices.
            const bool upperCell = (slot & 1u) != 0u;
            roleWeight = (voice == 0u) == upperCell ? 0.58f : 1.18f;
            break;
        }
        case PassageRole::BPrime: { // The response changes its point of support.
            const bool callerSlot = ((slot + rotation + 1u) & 1u) == 0u;
            const bool caller = voice == 0u;
            roleWeight = callerSlot == caller ? 1.08f : 0.52f;
            break;
        }
        case PassageRole::Expansion: {
            roleWeight = 1.08f;
            break;
        }
        case PassageRole::CPrime: { // C returns with a shifted phrase balance.
            const bool upperCell = ((slot + rotation) & 3u) >= 2u;
            roleWeight = (voice == 0u) == upperCell ? 0.72f : 1.12f;
            break;
        }
        case PassageRole::ReturnA:
            roleWeight = ((slot + rotation) & 1u) == 0u ? 1.08f : 0.88f;
            break;
        case PassageRole::A:
            roleWeight = ((slot + rotation) & 1u) == 0u ? 1.03f : 0.97f;
            break;
        default:
            roleWeight = ((slot + rotation) & 1u) == 0u ? 1.08f : 0.88f;
            break;
    }
    if (roleWeight <= 0.0f)
        return 0.0f;
    return std::clamp(roleWeight * memoryAccent, 0.18f, 1.32f);
}

} // namespace batruc
