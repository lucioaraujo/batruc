/* SPDX-License-Identifier: AGPL-3.0-or-later */
const assert = require("node:assert/strict");
const fs = require("node:fs");
const score = require("../prototype/score.js");

const outputDir = process.argv[2];
assert(outputDir, "passe o diretório de resultados da fixture nativa");
const seeds = ["A1B2C3D4", "00000001", "5EED2026", "FFFFFFFF"];
const amounts = [0, 0.25, 0.5, 1];
const transforms = ["sine", "invert", "rectify", "harmonic", "amplitude",
  "quantized", "cubic", "thirdHarmonic", "triangle", "softClip"];
const voiceCount = 8;
const baseCycles = [3, 4, 0.5, 0.75, 1, 1.25, 1.5, 1.75];
const baseCouplings = [0, ...Array(voiceCount - 1).fill(0.28)];
const basePhases = Array.from({ length: voiceCount }, (_, voice) =>
  voice >= 2 ? 0.137 * (voice - 1) : 0);
let comparisons = 0;

for (const seed of seeds) {
  for (const amount of amounts) {
    const key = String(amount).replace(".", "_");
    const native = JSON.parse(fs.readFileSync(`${outputDir}/${seed}-${key}.json`, "utf8"));
    const web = score.generate({
      recipe: "center", phase: 0.125, cycleSeconds: 8, bpm: 120, meter: "4/4",
      voiceCount, voiceCycles: baseCycles, voiceBasePhases: basePhases,
      couplings: baseCouplings, voiceTransforms: Array(voiceCount).fill("sine"),
      patternMode: "unrestricted", seed, variation: amount
    }).config;
    assert.deepEqual(native.cycles, web.voiceCycles, `${seed} ${amount}: ciclos`);
    for (let voice = 0; voice < voiceCount; voice += 1) {
      assert(Math.abs(native.couplings[voice] - web.couplings[voice]) < 1e-6,
        `${seed} ${amount}: acoplamento V${voice + 1}`);
      const stagger = voice === 0 ? 0 : (voice - 1) / 16;
      const expectedPhase = stagger + (web.voiceBasePhases[voice] || 0)
        + (web.voicePhaseOffsets[voice] || 0);
      const wrappedPhase = expectedPhase - Math.floor(expectedPhase + 0.5);
      assert(Math.abs(native.phases[voice] - wrappedPhase) < 1e-6,
        `${seed} ${amount}: fase V${voice + 1}`);
      assert.equal(native.transforms[voice], transforms.indexOf(web.voiceTransforms[voice]),
        `${seed} ${amount}: transformação V${voice + 1}`);
    }
    comparisons += 1;
  }
}

console.log(`seed variation parity: ${comparisons} receitas HTML/JUCE conferem em ciclos, fases, acoplamentos e transformações`);
