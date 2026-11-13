/* SPDX-License-Identifier: AGPL-3.0-or-later */
const assert = require("node:assert/strict");
const score = require("./score.js");

const runtimeAgenda = [{ time: 0 }, { time: 0.12 }, { time: 0.13 },
  { time: 0.130001 }, { time: 0.4 }];
assert.equal(score.eventCursorAtOrAfter(runtimeAgenda, 0.13), 2,
  "revisão em execução mantém elegível o primeiro evento fora da janela antiga");
assert.equal(score.eventCursorAtOrAfter(runtimeAgenda, 0.130001), 3,
  "cursor de revisão respeita a fronteira temporal sem reexecutar ataques anteriores");
assert.equal(score.eventCursorAtOrAfter(runtimeAgenda, 1), runtimeAgenda.length,
  "cursor aponta para o fim quando a nova agenda já passou");
assert.equal(score.eventCursorAtOrAfter([], 0), 0,
  "cursor aceita agenda vazia");
assert.throws(() => score.eventCursorAtOrAfter(runtimeAgenda, Infinity), RangeError,
  "cursor rejeita limites temporais inválidos");

const config = { recipe: "center", phase: 0.125, cycleSeconds: 8 };
const billiardImpactsPerCycle = [6, 6, 8, 8, 8, 10, 10];
const billiardVoiceOneFixtures = [];
for (let pattern = 0; pattern < billiardImpactsPerCycle.length; pattern += 1) {
  const impacts = score.billiardCollisionPositions(1, 0, pattern);
  assert.equal(impacts.length, billiardImpactsPerCycle[pattern],
    `bilhar: quantidade de colisões na razão ${pattern}`);
  assert(impacts.every((value, index) => value > 0 && value < 1 &&
    (!index || value > impacts[index - 1])),
  `bilhar: posições ordenadas e internas ao ciclo ${pattern}`);
  const input = { ...config, phase: 0, durationSeconds: 28,
    geometryShape: "billiard-square", geometryBilliardPattern: pattern,
    geometrySource: true, voiceCount: 2, seed: "5EED2027" };
  const first = score.generate(input);
  const repeated = score.generate(input);
  const voiceOne = first.events.filter((event) => event.voice === 0);
  assert.equal(voiceOne.length, billiardImpactsPerCycle[pattern] * 3 * 7,
    `bilhar: impactos propagam para os eventos da V1 na razão ${pattern}`);
  assert.deepEqual(voiceOne.map((event) => [event.time, event.position]),
    repeated.events.filter((event) => event.voice === 0)
      .map((event) => [event.time, event.position]),
    `bilhar: trajetória e eventos reproduzíveis pela seed na razão ${pattern}`);
  billiardVoiceOneFixtures.push(voiceOne.map((event) => event.position));
}
assert.notDeepEqual(billiardVoiceOneFixtures[0], billiardVoiceOneFixtures[1],
  "bilhar: mudar a razão altera as posições composicionais da V1");
const expectedPhaseSegments = [
  [0, 0], [0, 0], [0, 0.5], [0.5, 0.5], [1, 0], [1, -0.5], [0.5, -0.5]
];
for (let cycle = 0; cycle < 7; cycle += 1) {
  for (const amount of [0, 0.1, 0.125, 0.25, 0.5]) {
    const segment = score.formalPhaseSegment(cycle, amount);
    assert.equal(segment.offsetCycles, expectedPhaseSegments[cycle][0] * amount,
      `arco de fase, deslocamento C${cycle + 1}`);
    assert.equal(segment.slopeCycles, expectedPhaseSegments[cycle][1] * amount,
      `arco de fase, inclinação C${cycle + 1}`);
  }
  for (let voices = 2; voices <= 8; voices += 1) {
    const expected = cycle === 0 ? 1
      : cycle === 1 || cycle === 5 ? Math.ceil((voices + 1) / 2) : voices;
    assert.equal(score.activeVoiceCountInCycle(cycle, voices), expected,
      `entrada formal C${cycle + 1}, ${voices} vozes`);
  }
}
const formalCounts = [
  [6, 0, 0], [6, 16, 1], [6, 16, 1], [6, 16, 1],
  [6, 16, 1], [6, 16, 1], [6, 16, 1]
];
const formalFirst = [
  [0, 0, 0], [0, 0.046875, 0.601],
  [0, 0.046511627906976744, 0.5342222222222222],
  [0, 0.03875968992248062, 0.4231111111111111], [0, 0.03125, 0.351],
  [0, 0.031496062992125984, 0.40114285714285713], [0, 0.03937007874015748, 0.544]
];
const formalLast = [
  [5 / 6, 0, 0], [5 / 6, 0.984375, 0.601],
  [5 / 6, 0.9767441860465116, 0.5342222222222222],
  [5 / 6, 0.9689922480620154, 0.4231111111111111], [5 / 6, 0.96875, 0.351],
  [5 / 6, 0.9763779527559056, 0.40114285714285713],
  [5 / 6, 0.984251968503937, 0.544]
];
const formalFixture = score.generate({ recipe: "center", phase: 0.125,
  cycleSeconds: 8, bpm: 120, meter: "4/4", voiceCount: 8,
  voiceCycles: [3, 8, 0.5, 0.75, 1, 1.25, 1.5, 1.75],
  voiceBasePhases: [0, 0.125, 0.137, 0.274, 0.411, 0.548, 0.685, 0.822],
  couplings: Array(8).fill(0), voiceTransforms: Array(8).fill("sine"),
  patternMode: "unrestricted", variation: 0 });
for (let cycle = 0; cycle < 7; cycle += 1) {
  for (let voice = 0; voice < 3; voice += 1) {
    const points = formalFixture.events.filter((event) =>
      event.cycle === cycle && event.voice === voice).map((event) => event.localProgress);
    assert.equal(points.length, formalCounts[cycle][voice],
      `agenda formal C${cycle + 1}, V${voice + 1}`);
    if (points.length) {
      assert(Math.abs(points[0] - formalFirst[cycle][voice]) < 1e-9,
        `primeiro ataque C${cycle + 1}, V${voice + 1}`);
      assert(Math.abs(points.at(-1) - formalLast[cycle][voice]) < 1e-9,
        `último ataque C${cycle + 1}, V${voice + 1}`);
    }
  }
}
for (const quarterNotesPerSection of [12, 16, 20, 14]) {
  for (const bpm of [60, 97, 120, 220]) {
    const cycleSeconds = quarterNotesPerSection * 60 / bpm;
    for (const ratio of [0.5, 0.57, 0.67]) {
      for (let beat = 0; beat < quarterNotesPerSection; beat += 1) {
        const straight = (beat + 0.5) / quarterNotesPerSection;
        const performed = score.swingPosition(straight, ratio, cycleSeconds, bpm);
        const expectedTime = (beat + ratio) * 60 / bpm;
        assert(Math.abs(performed * cycleSeconds - expectedTime) < 1e-10,
          `swing: ${quarterNotesPerSection} semínimas, ${bpm} BPM, ${ratio}`);
        const downbeat = beat / quarterNotesPerSection;
        assert(Math.abs(score.swingPosition(downbeat, ratio, cycleSeconds, bpm)
          - downbeat) < 1e-12, "swing preserva fronteiras de semínima");
      }
    }
  }
}
const partialBeatCycle = 8.1;
const partialBeatProgress = 16.1 / (partialBeatCycle * 120 / 60);
assert.equal(score.swingPosition(partialBeatProgress, 0.67, partialBeatCycle, 120),
  partialBeatProgress, "um fragmento final de semínima não ultrapassa o ciclo");
const seedVectors = [
  ["00000000", 7, 20, 0, 1, 8, 3, 0.72],
  ["4E415632", 2, 58, 0, 2, 2, 1, 0.57],
  ["A1B2C3D4", 7, 1, 1, 0, 8, 2, 0.54],
  ["FFFFFFFF", 1, 14, 1, 2, 8, 3, 0.80],
  ["01234567", 7, 25, 1, 0, 8, 3, 0.50]
];
for (const [seed, cycle, phase, rule, pattern, voices, law, amount] of seedVectors) {
  const selected = score.seededRecipeSelection(seed);
  assert.equal(selected.voiceTwoCycleIndex, cycle, `${seed}: índice de ciclo da V2`);
  assert.equal(selected.phaseIndex, phase, `${seed}: índice de fase`);
  assert.equal(selected.eventRuleIndex, rule, `${seed}: receita de eventos`);
  assert.equal(selected.patternIndex, pattern, `${seed}: campo espacial`);
  assert.equal(selected.voiceCount, voices, `${seed}: quantidade de vozes`);
  assert.equal(selected.intensityLawIndex, law, `${seed}: lei de intensidade`);
  assert.equal(selected.intensityAmount, amount, `${seed}: profundidade da lei`);
  assert.equal(selected.voiceTwoCycles, [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16][cycle]);
}
assert.throws(() => score.seededRecipeSelection("4E41563"), RangeError);

const center = score.generate(config);
const turn = score.generate({ ...config, recipe: "turn" });
const expectedPolyrhythms = {
  "4:3": [4, 3], "5:4": [5, 4], "6:4": [6, 4], "7:4": [7, 4],
  "8:6": [8, 6], "10:4": [10, 4], "10:7": [10, 7],
  "2:3:5": [2, 3, 5], "3:4:5": [3, 4, 5], "4:5:7": [4, 5, 7],
  "3:4:5:7": [3, 4, 5, 7], "3:4:5:7:8": [3, 4, 5, 7, 8]
};
for (const [name, cycles] of Object.entries(expectedPolyrhythms))
  assert.deepEqual(score.polyrhythmPreset(name), cycles, `${name} preset`);
assert.equal(score.polyrhythmPreset("missing"), null);
const polyApplication = score.applyPolyrhythmPreset("3:4:5:7:8", 4,
  [3, 4, 0.5, 0.75, 1, 1.25]);
assert.equal(polyApplication.voiceCount, 5, "ativa a quinta voz necessária");
assert.equal(polyApplication.variation, 0,
  "a aplicação garante os valores exatos do preset, como o controle manual JUCE");
assert.deepEqual(polyApplication.voiceCycles.slice(0, 5), [3, 4, 5, 7, 8]);
assert.deepEqual(polyApplication.voiceCycles.slice(5), [1.25],
  "preserva os ciclos dos demais canais");

assert.equal(center.duration, 56);
assert(center.events.length > 90);
assert(turn.events.length > 90);
assert(center.events.some((event) => event.voice === score.bassVoice),
  "o padrão inicial do baixo produz eventos");
assert(center.events.some((event) => event.voice === score.bassVoice && event.cycle === 0),
  "o baixo participa desde o primeiro ciclo");
assert(center.events.some((event) => event.voice === score.bassVoice && event.cycle === 4),
  "o baixo permanece audível no quinto ciclo");
assert(center.events.some((event) => event.cycle === 4 && event.voice > 0
  && event.voice !== score.bassVoice), "o quinto ciclo preserva as vozes");
assert(center.events.some((event) => event.cycle === 5 && event.voice === 1));
assert(center.events.every((event) => event.time >= 0 && event.time < 56));
assert(center.events.every((event) => event.intensity > 0 && event.intensity <= 1));
assert(center.events.every((event, index) => index === 0 ||
  center.events[index - 1].time <= event.time));
assert(center.events.some((event, index) =>
  Math.abs(event.time - turn.events[index].time) > 1e-3));
assert.deepEqual(score.generate(config), center);
assert.equal(score.analyticEvents(0, 0, center.config).length, 6,
  "três oscilações V1 geram seis cruzamentos analíticos do centro");
assert.equal(score.analyticEvents(0, 1, center.config).length, 8,
  "quatro oscilações V2 geram oito cruzamentos analíticos do centro");
const transformedSchedule = score.generate({ ...config,
  voiceTransforms: ["invert", "thirdHarmonic"], couplings: [0, 0.8] });
assert.deepEqual(transformedSchedule.events.filter((event) => event.voice === 0)
  .map((event) => event.time), center.events.filter((event) => event.voice === 0)
  .map((event) => event.time),
"transformações e acoplamento alteram a fórmula sonora sem deslocar os ataques, como no JUCE");
const seededSelection = score.seededRecipeSelection("4E415632");
const seededComposition = score.generate({ ...config,
  recipe: seededSelection.recipe, phase: seededSelection.phase,
  patternMode: seededSelection.patternMode,
  intensityLaw: seededSelection.intensityLaw,
  intensityAmount: seededSelection.intensityAmount,
  voiceCount: seededSelection.voiceCount,
  voiceCycles: [3, seededSelection.voiceTwoCycles].concat([0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.5, 3, 3.5])
    .slice(0, seededSelection.voiceCount),
  voiceBasePhases: [0, seededSelection.phase, ...Array(10).fill(0)]
    .slice(0, seededSelection.voiceCount),
  seed: "4E415632", variation: 0 });
assert.equal(seededComposition.config.voiceCount, 2);
assert.equal(seededComposition.config.patternMode, "diagonal-lanes");
assert.equal(seededComposition.config.intensityLaw, "projection-y");
assert.equal(seededComposition.config.voiceBasePhases[1], seededSelection.phase,
  "a fase-base da segunda voz acompanha a fase escolhida pela JUCE");
assert(seededComposition.events.filter((event) => event.voice < 12).length > 0,
  "a receita seeded filtrada continua gerando eventos");
assert(seededComposition.events.every((event) => event.voice === score.bassVoice ||
  Number.isFinite(event.intensity) && event.intensity >= 0 && event.intensity <= 1));
const sameUnfilteredSeed = score.generate({ ...seededComposition.config,
  patternMode: "unrestricted" });
assert(seededComposition.events.filter((event) => event.voice < 12).length <
  sameUnfilteredSeed.events.filter((event) => event.voice < 12).length,
"o padrão espacial da seed remove eventos fora das duas faixas do JUCE");
const intensityLaws = ["uniform", "projection-y", "product-xy", "quadratic-difference",
  "gaussian-centre", "cosine-bands", "potential-energy", "kinetic-energy"];
const lawRuns = intensityLaws.map((intensityLaw) => score.generate({
  ...seededComposition.config, patternMode: "unrestricted", intensityLaw,
  intensityAmount: 1
}));
assert(lawRuns.every((run) => Number.isFinite(run.events[0].intensity)),
  "as oito leis aceitas pelo JUCE produzem intensidade finita na web");
assert(new Set(lawRuns.map((run) => run.events[0].intensity.toFixed(6))).size >= 6,
  "as leis mantêm respostas diferentes na web");
const analyticSelection = score.seededRecipeSelection("01234567");
const variedSelection = score.generate({ ...config,
  recipe: analyticSelection.recipe, phase: analyticSelection.phase,
  patternMode: analyticSelection.patternMode,
  intensityLaw: analyticSelection.intensityLaw,
  intensityAmount: analyticSelection.intensityAmount,
  voiceCount: 8,
  voiceCycles: [3, 8, 0.5, 0.75, 1, 1.25, 1.5, 1.75],
  voiceBasePhases: [0, 0.125, 0.137, 0.274, 0.411, 0.548, 0.685, 0.822],
  couplings: [0, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28],
  voiceTransforms: Array(8).fill("sine"),
  seed: "01234567", variation: 0.35 }).config;
assert.deepEqual(variedSelection.voiceCycles, [3, 7, 1, 1, 1, 1, 1, 1]);
const expectedCouplings = [0, 0.31529078, 0.22725685, 0.31561735,
  0.35834316, 0.25274628, 0.26565421, 0.30497080];
const expectedRealizedPhases = [0, 0.02093841, 0.09552807, 0.11793486,
  0.18120375, 0.23107325, 0.34523529, 0.36763129];
const expectedTransforms = ["sine", "sine", "thirdHarmonic", "sine", "sine",
  "thirdHarmonic", "invert", "sine"];
for (let voice = 0; voice < 8; voice += 1) {
  assert(Math.abs(variedSelection.couplings[voice] - expectedCouplings[voice]) < 1e-6,
    `coupling seeded V${voice + 1}`);
  const stagger = voice === 0 ? 0 : (voice - 1) / 16;
  const realized = stagger + variedSelection.voicePhaseOffsets[voice];
  const wrapped = realized - Math.floor(realized + 0.5);
  assert(Math.abs(wrapped - expectedRealizedPhases[voice]) < 1e-6,
    `phase seeded V${voice + 1}`);
  assert.equal(variedSelection.voiceTransforms[voice], expectedTransforms[voice],
    `transformation seeded V${voice + 1}`);
}
const analyticFixture = score.generate({ ...config,
  recipe: analyticSelection.recipe, phase: analyticSelection.phase,
  patternMode: analyticSelection.patternMode,
  intensityLaw: analyticSelection.intensityLaw,
  intensityAmount: analyticSelection.intensityAmount,
  voiceCount: 8,
  voiceCycles: [3, 8, 0.5, 0.75, 1, 1.25, 1.5, 1.75],
  voiceBasePhases: [0, 0.125, 0.137, 0.274, 0.411, 0.548, 0.685, 0.822],
  seed: "01234567", variation: 0 });
const fixtureCounts = [6, 16, 1, 1, 2];
const fixturePositions = [
  [1 / 12, 0.25, 5 / 12, 7 / 12, 0.75, 11 / 12],
  Array.from({ length: 16 }, (_, index) => (1 + 4 * index) / 64),
  [0.101], [0.468], [0.1515, 0.6515]
];
for (let voice = 0; voice < fixtureCounts.length; voice += 1) {
  const sectionEvents = analyticFixture.events.filter((event) =>
    event.cycle === 1 && event.voice === voice);
  assert.equal(sectionEvents.length, fixtureCounts[voice], `agenda analítica V${voice + 1}`);
  sectionEvents.forEach((event, index) =>
    assert(Math.abs(event.localProgress - fixturePositions[voice][index]) < 1e-9,
      `posição ${index + 1} da agenda analítica V${voice + 1}`));
}
const fractionalVoices = score.generate({ ...config, voiceCount: 4,
  voiceCycles: [3, 4, 0.5, 0.75], variation: 0 });
assert.deepEqual(fractionalVoices.config.voiceCycles, [3, 4, 0.5, 0.75],
  "o motor web preserva ciclos sub-harmônicos e fracionários usados pelo JUCE");
assert.throws(() => score.generate({ ...config, voiceCycles: [3, 4.6] }), RangeError,
  "ciclos fracionários mantêm a grade de 0,25 usada pelo JUCE");
assert.deepEqual(center.config.bassSeededPattern, center.config.bassPattern,
  "variação zero preserva todas as alturas escritas no baixo");
const bassVariation = { ...config, seed: "A1B2C3D4", variation: 1 };
const variedBass = score.generate(bassVariation);
assert.deepEqual(score.generate(bassVariation).config.bassSeededPattern,
  variedBass.config.bassSeededPattern, "a mesma seed reproduz as alturas do baixo");
assert.notDeepEqual(variedBass.config.bassSeededPattern, variedBass.config.bassPattern,
  "uma seed com variação máxima altera células do baixo");
const bassEvents = variedBass.events.filter((event) => event.voice === score.bassVoice);
assert(bassEvents.some((event) => event.pitch !==
  variedBass.config.bassPattern[event.stepNumber % 16].midi),
"as alturas derivadas chegam aos eventos sonoros do baixo");
assert(bassEvents.every((event) => event.pitch ===
  variedBass.config.bassSeededPattern[event.stepNumber % 16].midi),
"os eventos usam as alturas exibidas no padrão derivado");
const allowedPitchClasses = new Set(variedBass.config.bassPattern.filter(Boolean)
  .map((step) => step.midi % 12));
for (let index = 0; index < variedBass.config.bassPattern.length; index += 1) {
  const written = variedBass.config.bassPattern[index];
  const seeded = variedBass.config.bassSeededPattern[index];
  if (!written) assert.equal(seeded, null, `a pausa ${index + 1} permanece pausa`);
  else {
    assert(allowedPitchClasses.has(seeded.midi % 12), `a célula ${index + 1} usa a coleção escrita`);
    assert(Math.abs(seeded.midi - written.midi) <= 7, `a célula ${index + 1} respeita o raio`);
    assert.deepEqual({ ...seeded, midi: written.midi }, written,
      `duração e articulação da célula ${index + 1} permanecem intactas`);
  }
}
assert.deepEqual(score.generate({ ...bassVariation, voiceCount: 8 }).config.bassSeededPattern,
  variedBass.config.bassSeededPattern,
  "a variação do baixo não depende da quantidade de vozes");
assert.deepEqual(score.generate({ ...config, seed: "A1B2C3D4", variation: 0 })
  .config.bassSeededPattern, center.config.bassPattern,
"com variação zero, trocar a seed preserva as alturas escritas do baixo");
const alternateSeedBass = score.generate({ ...bassVariation, seed: "0F1E2D3C" })
  .config.bassSeededPattern;
assert.notDeepEqual(alternateSeedBass, variedBass.config.bassSeededPattern,
  "com variação positiva, seeds diferentes podem gerar alturas diferentes no baixo");
const bassPitchOnlyConfig = { ...config, seed: "A1B2C3D4", variation: 0,
  bassPitchVariationBase: 1, bassVoiceDriven: true };
const bassPitchOnly = score.generate(bassPitchOnlyConfig);
assert.equal(bassPitchOnly.config.bassPitchVariation, 1,
  "o mínimo próprio do baixo continua ativo com variação global zero");
assert.deepEqual(score.generate(bassPitchOnlyConfig).events
  .filter((event) => event.voice === score.bassVoice).map((event) => event.seededPitch),
bassPitchOnly.events.filter((event) => event.voice === score.bassVoice)
  .map((event) => event.seededPitch),
"a variação própria do baixo é determinística para a mesma seed");
assert.notDeepEqual(bassPitchOnly.events.filter((event) => event.voice === score.bassVoice)
  .map((event) => event.seededPitch),
score.generate({ ...bassPitchOnlyConfig, bassPitchVariationBase: 0 }).events
  .filter((event) => event.voice === score.bassVoice).map((event) => event.seededPitch),
"o mínimo próprio do baixo altera alturas calculadas sem variar as vozes");
assert.equal(score.phaseFor(1, 1, config.phase), 0);
assert.equal(score.phaseFor(5, 1, config.phase), config.phase / 2);
assert.throws(() => score.generate({ ...config, phase: Infinity }), RangeError);
assert.throws(() => score.generate({ ...config, phase: -0.1 }), RangeError);
assert.throws(() => score.generate({ ...config, cycleSeconds: 100 }), RangeError);

const transformCases = [
  ["cubic", 0.125],
  ["thirdHarmonic", 1],
  ["triangle", 1 / 3],
  ["softClip", Math.tanh(1) / Math.tanh(2)]
];
for (const [transform, expected] of transformCases) {
  const transformed = score.generate({ ...config, phase: 0,
    voiceTransforms: [transform, "sine"] });
  assert(Math.abs(score.valueAt(0, 0, 1 / 36, transformed.config) - expected) < 1e-12,
    `${transform} corresponde à expressão matemática documentada`);
}

const ecologicalBase = score.generate({ ...config, phase: 0, durationSeconds: 56,
  voiceCount: 2, voiceCycles: [3, 4], couplings: [0, 0.8],
  voiceEventRelations: ["none", "none"], bassVoiceDriven: true,
  seed: "5EED2027" });
for (const relation of ["support", "imitation", "avoidance", "competition"]) {
  const result = score.generate({ ...config, phase: 0, durationSeconds: 56,
    voiceCount: 2, voiceCycles: [3, 4], couplings: [0, 0.8],
    voiceEventRelations: ["none", relation], bassVoiceDriven: true,
    seed: "5EED2027" });
  const baseVoice = ecologicalBase.events.filter((event) => event.voice === 1);
  const relatedVoice = result.events.filter((event) => event.voice === 1);
  assert.equal(relatedVoice.length, baseVoice.length,
    `${relation}: interação transforma eventos existentes sem criar novos ataques`);
  assert.notDeepEqual(relatedVoice.map((event) => [event.time, event.intensity]),
    baseVoice.map((event) => [event.time, event.intensity]),
    `${relation}: interação altera a realização da voz-alvo`);
  const baseBass = ecologicalBase.events.filter((event) => event.voice === score.bassVoice);
  const relatedBass = result.events.filter((event) => event.voice === score.bassVoice);
  assert.notDeepEqual(relatedBass.map((event) => [event.time, event.pitch, event.intensity]),
    baseBass.map((event) => [event.time, event.pitch, event.intensity]),
    `${relation}: baixo seedado responde à composição relacional das vozes`);
}
assert.throws(() => score.generate({ ...config, voiceCount: 2,
  voiceEventRelations: ["imitation", "none"] }), RangeError,
  "V1 deve iniciar a cadeia relacional sem fonte anterior");

console.log("score: eventos, receitas, expansão, retorno e limites verificados");
