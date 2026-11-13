// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

#include "core/BassComposition.hpp"

#include <cassert>
#include <cmath>

using namespace batruc;

int main() {
    BassRecipe recipe;
    recipe.cells[0] = {BassCellKind::note, 36, 8, 1, BassIntent::normal};
    recipe.cells[4] = {BassCellKind::rest, 36, 2, 1, BassIntent::normal};
    recipe.cells[6] = {BassCellKind::note, 43, 2, 2, BassIntent::ghost};
    recipe.cells[7] = {BassCellKind::note, 39, 2, 1, BassIntent::accent};

    BassScore score;
    assert(buildBassScore(recipe, score) == BassError::none);
    assert(score.count == 12);
    assert(score.notes[0].durationSteps == 8);
    assert(std::abs(score.notes[0].durationSeconds - 4.0 * 15.0 / recipe.bpm) < 1e-9);
    assert(score.notes[0].writtenMidi == score.notes[0].calculatedMidi);

    BassInterpretation interpretation;
    interpretation.swing = 0.5f;
    BassPerformance performance;
    assert(interpretBassScore(score, interpretation, performance) == BassError::none);
    assert(performance.count == 16);
    assert(std::abs(performance.events[3].timeSeconds
        - (7.0 + 0.5) * 15.0 / recipe.bpm) < 1e-9);
    assert(score.notes[2].timeSeconds == 7.0 * 15.0 / recipe.bpm);
    assert(performance.events[1].velocity < performance.events[3].velocity);

    BassPlayedEvent previousCycleEvent;
    previousCycleEvent.timeSeconds = 0.25;
    previousCycleEvent.formCycle = 1;
    BassPlayedEvent repeatedInNextCycle = previousCycleEvent;
    repeatedInNextCycle.formCycle = 2;
    BassPlayedEvent nearDuplicate = previousCycleEvent;
    nearDuplicate.timeSeconds += 0.001;
    assert(!bassEventsCollideWithinSameCycle(previousCycleEvent,
        repeatedInNextCycle, 0.01));
    assert(bassEventsCollideWithinSameCycle(previousCycleEvent,
        nearDuplicate, 0.01));

    recipe.durationLaw = BassDurationLaw::fibonacci;
    assert(buildBassScore(recipe, score) == BassError::none);
    assert(score.notes[0].durationSteps == 1);
    assert(score.notes[1].durationSteps == 1);
    assert(score.notes[2].durationSteps == 2);

    recipe.durationLaw = BassDurationLaw::manual;
    recipe.pitchVariation = 1.0f;
    recipe.seed = 0x4E415632u;
    BassScore repeated;
    assert(buildBassScore(recipe, score) == BassError::none);
    assert(buildBassScore(recipe, repeated) == BassError::none);
    for (std::size_t index = 0; index < score.count; ++index) {
        assert(score.notes[index].calculatedMidi == repeated.notes[index].calculatedMidi);
        assert(score.notes[index].writtenMidi == repeated.notes[index].writtenMidi);
        assert(std::abs(static_cast<int>(score.notes[index].calculatedMidi)
            - static_cast<int>(score.notes[index].writtenMidi)) <= 7);
    }

    assert(score.notes[0].calculatedMidi != score.notes[0].writtenMidi);

    recipe.bpm = 0.0;
    assert(buildBassScore(recipe, score) == BassError::invalidRecipe);
    assert(score.count == 0);
}
