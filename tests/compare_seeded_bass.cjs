/* SPDX-License-Identifier: AGPL-3.0-or-later */
const assert = require("node:assert/strict");
const fs = require("node:fs");
const score = require("../prototype/score.js");

const outputDir = process.argv[2];
assert(outputDir, "passe o diretório que contém as agendas nativas");
const meters = ["3/4", "4/4", "5/4", "7/8"];
const phrases = ["loop", "breath", "cadence", "call-response"];
const relations = ["independent", "follow", "answer"];
const seeds = ["A1B2C3D4", "00000001", "5EED2026", "FFFFFFFF"];
const phases = [0, 0.125, 0.25];
const bassVariations = [0, 1];
let comparisons = 0;

for (const meter of meters) {
  const [numerator, denominator] = meter.split("/").map(Number);
  const bpms = meter === "5/4" ? [100, 120, 160, 220] : [97, 100, 120, 160, 220];
  const meterKey = meter.replace("/", "-");
  for (const phrase of phrases) {
    for (const relation of relations) {
      for (const seed of seeds) {
        for (const bpm of bpms) {
          for (const phase of phases) {
          for (const bassPitchVariationBase of bassVariations) {
          const phaseKey = String(phase).replace(".", "_");
          const cycleSeconds = 4 * numerator * 4 / denominator * 60 / bpm;
          const nativePath = `${outputDir}/${meterKey}-${phrase}-${relation}-${seed}-${bpm}-${phaseKey}-${bassPitchVariationBase}.json`;
          const native = JSON.parse(fs.readFileSync(nativePath, "utf8"))
            .sort((a, b) => a.time - b.time || a.cycle - b.cycle);
          const web = score.generate({
            recipe: "center",
            phase,
            durationSeconds: cycleSeconds * 7,
            cycleSeconds,
            bpm,
            meter,
            swing: 0.57,
            groove: "straight",
            dynamics: "steady",
            intention: "meet",
            voiceCount: 4,
            voiceCycles: [3, 4, 1, 2],
            voiceBasePhases: [0, 0, 0, 0],
            couplings: [0, 0, 0, 0],
            voiceTransforms: ["sine", "sine", "sine", "sine"],
            patternMode: "unrestricted",
            intensityLaw: "uniform",
            intensityAmount: 0,
            seed,
            variation: 0,
            bassPitchVariationBase,
            bassVoiceDriven: true,
            bassPhraseMode: phrase,
            bassRhythmRelation: relation,
            bassLfoEnabled: false,
            bassDurationFormula: "manual"
          }).events.filter((event) => event.voice === score.bassVoice)
            .map((event) => ({ cycle: event.cycle, time: event.time,
              midi: event.seededPitch }))
            .sort((a, b) => a.time - b.time || a.cycle - b.cycle);

          const firstDifference = Array.from({ length: Math.min(native.length, web.length) })
            .findIndex((_, index) => native[index].cycle !== web[index].cycle
              || native[index].midi !== web[index].midi
              || Math.abs(native[index].time - web[index].time) >= 1e-8);
          assert.equal(native.length, web.length,
            `${meter} ${phrase} ${relation} ${seed} ${bpm}: quantidade de ataques; ` +
            `primeira diferença=${firstDifference >= 0 ? JSON.stringify({ html: web[firstDifference], juce: native[firstDifference] }) : "nenhuma"}; ` +
            `HTML fim=${JSON.stringify(web.slice(-4))}; JUCE fim=${JSON.stringify(native.slice(-4))}`);
          for (let index = 0; index < web.length; index += 1) {
            assert.equal(native[index].cycle, web[index].cycle,
              `${meter} ${phrase} ${relation} ${seed} ${bpm}: ciclo ${index + 1}; ` +
              `HTML=${JSON.stringify(web[index])}; JUCE=${JSON.stringify(native[index])}`);
            assert.equal(native[index].midi, web[index].midi,
              `${meter} ${phrase} ${relation} ${seed} ${bpm}: altura ${index + 1}`);
            assert(Math.abs(native[index].time - web[index].time) < 1e-8,
              `${meter} ${phrase} ${relation} ${seed} ${bpm}: tempo ${index + 1} (${native[index].time} vs ${web[index].time})`);
          }
          comparisons += 1;
          }
          }
        }
      }
    }
  }
}

console.log(`seeded bass parity: ${comparisons} agendas HTML/JUCE conferem em métrica, frase, relação, seed, BPM, fase e variação de altura do baixo`);
