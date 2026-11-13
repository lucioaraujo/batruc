// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

const assert = require("node:assert/strict");
const { spawnSync } = require("node:child_process");
const score = require("../prototype/score.js");

const executable = process.argv[2];
assert.ok(executable, "expected path to C++ geometry fixture");
const result = spawnSync(executable, [], { encoding: "utf8", timeout: 5000 });
assert.equal(result.status, 0, result.stderr || "C++ geometry fixture failed");

const names = ["compass", "triangle", "square", "spiral", "lissajous",
  "fourier-triangle", "fourier-square", "epicycloid", "hypocycloid",
  "cycloid", "catenary", "billiard-square"];
let checked = 0;
let vectorsChecked = 0;
let maxError = 0;
for (const line of result.stdout.trim().split("\n")) {
  const fields = line.split(/\s+/);
  if (fields[0] === "v") {
    const [, curveIndex, terms, progress, index, harmonic,
      fromX, fromY, toX, toY, radius] = fields.map(Number);
    const shape = curveIndex === 5 ? "fourier-triangle" : "fourier-square";
    const vectors = score.geometryFourierEpicycles(shape, progress, 2.5, 0.137, terms);
    assert.equal(vectors.length, 2 * terms + 1);
    const expected = vectors[index];
    assert.equal(expected.harmonic, harmonic);
    for (const [key, actual] of Object.entries({ fromX, fromY, toX, toY, radius })) {
      const error = Math.abs(expected[key] - actual);
      maxError = Math.max(maxError, error);
      assert.ok(error < 2e-8,
        `${shape} H=${terms} vector ${index} ${key}: C++=${actual}, JS=${expected[key]}, error=${error}`);
    }
    vectorsChecked += 1;
    continue;
  }
  const [curveIndex, pattern, progress, x, y] = fields.map(Number);
  const terms = curveIndex === 10 ? 5 : curveIndex === 11 ? 11 : 8;
  const expected = score.geometryPoint(names[curveIndex], progress, 2.75, 0.137,
    terms, pattern);
  const error = Math.max(Math.abs(expected.x - x), Math.abs(expected.y - y));
  maxError = Math.max(maxError, error);
  assert.ok(error < 2e-8,
    `${names[curveIndex]} pattern ${pattern} at ${progress}: C++=(${x}, ${y}), JS=(${expected.x}, ${expected.y}), error=${error}`);
  checked += 1;
}
assert.equal(checked, 162, "all curve and billiard fixtures should be checked");
assert.equal(vectorsChecked, 222, "all Fourier epicycle vectors should be checked");
console.log(`geometry parity: ${checked} points + ${vectorsChecked} epicycle vectors; max error ${maxError}`);
