/* SPDX-License-Identifier: AGPL-3.0-or-later */
const assert = require("node:assert/strict");
const fs = require("node:fs");
const score = require("../prototype/score.js");

const outputDir = process.argv[2];
assert(outputDir, "passe o diretório de agendas nativas");
let comparisons = 0;

function compareCase({ seed, variation, phrase, relation, meter, bpm, phase,
  voiceCount, swing, groove, dynamics, intention, durationLaw = "manual",
  geometricRatio = 1.5 }) {
  const meterKey = meter.replace("/", "_");
  const key = `${seed}-${variation}-${phrase}-${relation}-${meterKey}-${bpm}-${phase}-v${voiceCount}-s${swing}-g${groove}-d${dynamics}-i${intention}-l${durationLaw}-r${geometricRatio}`;
  const native = JSON.parse(fs.readFileSync(`${outputDir}/${key}.json`, "utf8"))
    .sort((a, b) => a.time - b.time || a.cycle - b.cycle);
  const [numerator, denominator] = meter.split("/").map(Number);
  const sectionSeconds = 4 * numerator * 4 / denominator * 60 / bpm;
  const baseCycles = [3, 4, 1, 2, 5, 6, 7, 8, 9, 10, 11, 12];
  const web = score.generate({
    recipe: "center", phase, cycleSeconds: sectionSeconds, bpm, meter,
    swing, groove, dynamics, intention,
    voiceCount, voiceCycles: baseCycles.slice(0, voiceCount),
    voiceBasePhases: Array(voiceCount).fill(0),
    couplings: Array(voiceCount).fill(0),
    voiceTransforms: Array(voiceCount).fill("sine"),
    patternMode: "unrestricted", intensityLaw: "uniform", intensityAmount: 0,
    seed, variation, bassPitchVariationBase: 0, bassVoiceDriven: true,
    bassPhraseMode: phrase, bassRhythmRelation: relation,
    bassDurationFormula: durationLaw, bassGeoRatio: geometricRatio,
    bassLfoEnabled: false
  }).events.filter((event) => event.voice === score.bassVoice)
    .map((event) => ({ cycle: event.cycle, time: event.time, midi: event.seededPitch,
      duration: event.durationSeconds, intensity: event.intensity,
      sourceVoice: event.sourceVoice, sourceX: event.sourceX, sourceY: event.sourceY }))
    .sort((a, b) => a.time - b.time || a.cycle - b.cycle);

  assert.equal(native.length, web.length, `${key}: quantidade de ataques`);
  for (let index = 0; index < web.length; index += 1) {
    assert.equal(native[index].cycle, web[index].cycle, `${key}: ciclo ${index + 1}`);
    assert.equal(native[index].midi, web[index].midi, `${key}: altura ${index + 1}`);
    assert(Math.abs(native[index].time - web[index].time) < 1e-7,
      `${key}: tempo ${index + 1} (${native[index].time} vs ${web[index].time})`);
    assert(Math.abs(native[index].duration - web[index].duration) < 1e-7,
      `${key}: duração ${index + 1} (${native[index].duration} vs ${web[index].duration})`);
    assert(Math.abs(native[index].intensity - web[index].intensity) < 1e-6,
      `${key}: intensidade ${index + 1} native=${JSON.stringify(native[index])}; ` +
      `web=${JSON.stringify(web[index])}`);
  }
  comparisons += 1;
}

for (const seed of ["A1B2C3D4", "00000001", "5EED2026", "FFFFFFFF"]) {
  for (const variation of [0.25, 0.5, 1]) {
    for (const phrase of ["loop", "breath", "cadence", "call-response"]) {
      for (const relation of ["independent", "follow", "answer"]) {
        for (const meter of ["3/4", "4/4", "5/4", "7/8"]) {
          for (const bpm of [97, 120, 220]) {
            if (meter === "5/4" && bpm === 97) continue;
            for (const phase of [0, 0.125, 0.25]) {
              compareCase({ seed, variation, phrase, relation, meter, bpm, phase,
                voiceCount: 4, swing: 0.57, groove: "straight",
                dynamics: "steady", intention: "meet" });
            }
          }
        }
      }
    }
  }
}

for (const seed of ["A1B2C3D4", "00000001", "5EED2026", "FFFFFFFF"]) {
  for (const variation of [0.5, 1]) {
    for (const phrase of ["loop", "call-response"]) {
      for (const relation of ["independent", "follow", "answer"]) {
        for (const voiceCount of [2, 4, 8]) {
          for (const swing of [0.5, 0.57, 0.67]) {
            for (const groove of ["straight", "ternary", "additive", "offbeat"]) {
              compareCase({ seed, variation, phrase, relation, meter: "4/4",
                bpm: 120, phase: 0.125, voiceCount, swing, groove,
                dynamics: "steady", intention: "meet" });
            }
          }
        }
      }
    }
  }
}

for (const seed of ["A1B2C3D4", "00000001", "5EED2026", "FFFFFFFF"]) {
  for (const phrase of ["loop", "call-response"]) {
    for (const relation of ["independent", "follow", "answer"]) {
      for (const groove of ["straight", "ternary", "additive", "offbeat"]) {
        for (const dynamics of ["steady", "crescendo", "arch", "waves"]) {
          for (const intention of ["build", "thin", "meet", "contrast"]) {
            compareCase({ seed, variation: 0.5, phrase, relation, meter: "4/4",
              bpm: 120, phase: 0.125, voiceCount: 4, swing: 0.57, groove,
              dynamics, intention });
          }
        }
      }
    }
  }
}

for (const seed of ["A1B2C3D4", "00000001", "5EED2026", "FFFFFFFF"]) {
  for (const variation of [0.5, 1]) {
    for (const phrase of ["loop", "breath", "cadence", "call-response"]) {
      for (const relation of ["independent", "follow", "answer"]) {
        for (const [durationLaw, geometricRatio] of [
          ["manual", 1.5], ["fibonacci", 1.5], ["geometric", 1.2],
          ["geometric", 1.5], ["geometric", 1.618], ["geometric", 2]
        ]) {
          compareCase({ seed, variation, phrase, relation, meter: "4/4",
            bpm: 120, phase: 0.125, voiceCount: 4, swing: 0.57,
            groove: "straight", dynamics: "steady", intention: "meet",
            durationLaw, geometricRatio });
        }
      }
    }
  }
}

assert.equal(comparisons, 8592, "a matriz precisa cobrir todas as receitas esperadas");
console.log(`seeded voice-to-bass parity: ${comparisons} agendas HTML/JUCE conferem`);
