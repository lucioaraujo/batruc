/* SPDX-License-Identifier: AGPL-3.0-or-later */
const assert = require("node:assert/strict");
const fs = require("node:fs");
const score = require("../prototype/score.js");

const outputDir = process.argv[2];
assert(outputDir, "passe o diretório dos calendários nativos");
const cases = [
  { seed: "5EED2026", meter: "4/4", bpm: 120, phase: 0, variation: 0.5 },
  { seed: "A1B2C3D4", meter: "4/4", bpm: 97, phase: 0.125, variation: 1 },
  { seed: "FFFFFFFF", meter: "5/4", bpm: 220, phase: 0.25, variation: 0.25 }
];

for (const item of cases) {
  const key = `${item.seed}-${item.meter.replace("/", "_")}-${item.bpm}-${item.phase}-${item.variation}`;
  const nativeCalendar = JSON.parse(fs.readFileSync(`${outputDir}/${key}.json`, "utf8"))
    .sort((a, b) => a.time - b.time || a.cycle - b.cycle);
  const [numerator, denominator] = item.meter.split("/").map(Number);
  const sectionSeconds = 4 * numerator * 4 * 60 / (denominator * item.bpm);
  const webCalendar = score.generate({
    recipe: "center", phase: item.phase, cycleSeconds: sectionSeconds,
    bpm: item.bpm, meter: item.meter, swing: 0.57, groove: "straight",
    dynamics: "steady", intention: "meet", voiceCount: 4,
    voiceCycles: [3, 4, 1, 2], voiceBasePhases: [0, 0, 0, 0],
    couplings: [0, 0, 0, 0], voiceTransforms: ["sine", "sine", "sine", "sine"],
    patternMode: "unrestricted", intensityLaw: "uniform", intensityAmount: 0,
    seed: item.seed, variation: item.variation, bassPitchVariationBase: 0,
    bassVoiceDriven: true, bassPhraseMode: "loop",
    bassRhythmRelation: "convolution", bassDurationFormula: "manual",
    bassGeoRatio: 1.5, bassLfoEnabled: false
  }).events.filter((event) => event.voice === score.bassVoice)
    .map((event) => ({ cycle: event.cycle, time: event.time,
      midi: event.seededPitch, duration: event.durationSeconds,
      intensity: event.intensity }))
    .sort((a, b) => a.time - b.time || a.cycle - b.cycle);

  const native = nativeCalendar;
  const web = webCalendar;

  assert.equal(native.length, web.length, `${item.seed}: quantidade de ataques`);
  for (let index = 0; index < web.length; index += 1) {
    assert.equal(native[index].cycle, web[index].cycle, `${item.seed}: cycle ${index}`);
    assert.equal(native[index].midi, web[index].midi, `${item.seed}: pitch ${index}`);
    assert(Math.abs(native[index].time - web[index].time) < 1e-7,
      `${item.seed}: time ${index} (${native[index].time} / ${web[index].time})`);
    assert(Math.abs(native[index].duration - web[index].duration) < 1e-7,
      `${item.seed}: duration ${index}`);
    assert(Math.abs(native[index].intensity - web[index].intensity) < 1e-6,
      `${item.seed}: intensity ${index}`);
  }
}
console.log(`cyclic bass parity: ${cases.length} native/web compositions agree`);
