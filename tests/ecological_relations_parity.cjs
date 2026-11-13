// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

const assert = require("node:assert/strict");
const { spawnSync } = require("node:child_process");
const score = require("../prototype/score.js");

const executable = process.argv[2];
assert.ok(executable, "expected path to C++ ecological-relations fixture");
const cpp = spawnSync(executable, [], { encoding: "utf8", timeout: 5000 });
assert.equal(cpp.status, 0, cpp.stderr || "C++ ecological fixture failed");

const times = [0.5, 0.8, 1.0, 1.2, 1.4, 1.5, 1.9, 2.0, 2.2, 2.4];
const voices = [0, 1, 2, 3, 4, 0, 1, 2, 3, 4];
const intensities = [0.4, 0.5, 0.3, 0.25, 0.8, 0.6, 0.5, 0.45, 0.7, 0.6];
const events = times.map((timeSeconds, index) => ({
  timeSeconds, voice: voices[index], intensity: intensities[index],
  fieldX: index / 10, fieldY: -index / 10
}));
const rules = [
  { relation: "none", sourceVoice: 0, strength: 0, memorySeconds: 0, responseDelaySeconds: 0 },
  { relation: "imitation", sourceVoice: 0, strength: 0.8, memorySeconds: 1, responseDelaySeconds: 0.25 },
  { relation: "avoidance", sourceVoice: 0, strength: 0.8, memorySeconds: 1, responseDelaySeconds: 0 },
  { relation: "support", sourceVoice: 0, strength: 0.8, memorySeconds: 1, responseDelaySeconds: 0 },
  { relation: "competition", sourceVoice: 0, strength: 0.8, memorySeconds: 1, responseDelaySeconds: 0 }
];
const js = score.applyEcologicalEventRelations(events, 4, 5, rules);
const lines = cpp.stdout.trim().split("\n");
const cppTrace = lines.filter((line) => line.startsWith("t ")).map((line) => line.split(/\s+/).slice(1).map(Number));
const cppEvents = lines.filter((line) => line.startsWith("e ")).map((line) => line.split(/\s+/).slice(1).map(Number));
assert.equal(cppTrace.length, js.trace.length);
assert.equal(cppEvents.length, js.events.length);
const close = (actual, expected, label) => {
  assert.ok(Math.abs(actual - expected) < 2e-6,
    `${label}: C++=${actual}; JS=${expected}`);
};
for (let index = 0; index < js.trace.length; index += 1) {
  const actual = cppTrace[index];
  const expected = js.trace[index];
  assert.equal(actual[0], index);
  assert.equal(actual[1], expected.perceived ? 1 : 0);
  if (expected.perceived) assert.equal(actual[2], expected.perceivedEventIndex);
  close(actual[3], expected.memoryAgeSeconds, `trace ${index} age`);
  close(actual[4], expected.resultingTimeSeconds, `trace ${index} time`);
  close(actual[5], expected.originalIntensity, `trace ${index} original intensity`);
  close(actual[6], expected.resultingIntensity, `trace ${index} intensity`);
}
for (let index = 0; index < js.events.length; index += 1) {
  close(cppEvents[index][0], js.events[index].voice, `event ${index} voice`);
  close(cppEvents[index][1], js.events[index].timeSeconds, `event ${index} time`);
  close(cppEvents[index][2], js.events[index].intensity, `event ${index} intensity`);
}
console.log(`ecological relation parity: ${js.trace.length} causal traces, ${js.events.length} events`);
