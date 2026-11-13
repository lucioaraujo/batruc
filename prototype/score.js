/* Maquete local de composição do batruc.
 * Direção artística: Lúcio de Araújo. Implementação assistida por Codex.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * O modo senoidal é idealizado; o modo pendular usa aproximação angular linear.
 */
(function (root) {
  "use strict";

  const TAU = 2 * Math.PI;
  const MAX_VOICES = 8;
  const BASS_VOICE = 12;
  const SAMPLES = 512;
  const PATTERN_EDGE_SOFTNESS = 0.055;
  const MAX_GEOMETRY_FOURIER_TERMS = 32;
  const FOURIER_REFERENCE_SAMPLES = 2048;
  const SECTION_NAMES = ["Construção", "Construção", "Mudança",
    "Mudança", "Expansão", "Retorno", "Retorno"];
  const SECTION_START = [0, 0, 2, 2, 4, 5, 5];
  // Keep the native recipe's subharmonic and fractional cycle bases so a
  // scene can move between HTML and JUCE without rounding its voice motion.
  const DEFAULT_CYCLES = [3, 4, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.5, 3, 3.5];
  const DEFAULT_BASS_PATTERN = [36, null, 36, null, 43, null, 36, null,
    36, null, 39, null, 41, null, 43, null, 36, null, 43, null].map((midi) =>
    midi === null ? null : ({ midi, durationSteps: 2 }));
  const POLYRHYTHM_PRESETS = Object.freeze({
    "4:3": [4, 3], "5:4": [5, 4], "6:4": [6, 4], "7:4": [7, 4],
    "8:6": [8, 6], "10:4": [10, 4], "10:7": [10, 7],
    "2:3:5": [2, 3, 5], "3:4:5": [3, 4, 5], "4:5:7": [4, 5, 7],
    "3:4:5:7": [3, 4, 5, 7], "3:4:5:7:8": [3, 4, 5, 7, 8]
  });
  const INTENTIONS = ["build", "thin", "meet", "contrast"];
  const DYNAMICS = ["steady", "crescendo", "arch", "waves"];
  const GROOVES = ["straight", "ternary", "additive", "offbeat"];
  const METERS = ["3/4", "4/4", "5/4", "7/8"];
  const VOICE_TRANSFORMS = ["sine", "invert", "rectify", "harmonic", "amplitude",
    "quantized", "cubic", "thirdHarmonic", "triangle", "softClip"];
  const GEOMETRY_SHAPES = ["compass", "triangle", "square", "spiral", "lissajous",
    "fourier-triangle", "fourier-square", "epicycloid", "hypocycloid",
    "cycloid", "catenary", "billiard-square"];
  const BILLIARD_HALF = 0.78;
  const BILLIARD_START = { x: -0.31, y: -0.43 };
  const BILLIARD_PATTERNS = [[2, 4], [4, 2], [2, 6], [6, 2],
    [4, 4], [4, 6], [6, 4]];
  const geometryFourierCache = new Map();
  const SEEDED_CYCLE_INDICES = [1, 2, 3, 5, 7];
  const NATIVE_CYCLE_CHOICES = Array.from({ length: 16 }, (_, index) => index + 1);

  function nextSeedWord(state) {
    const next = (state + 0x9e3779b9) >>> 0;
    let value = next;
    value = Math.imul(value ^ value >>> 16, 0x85ebca6b) >>> 0;
    value = Math.imul(value ^ value >>> 13, 0xc2b2ae35) >>> 0;
    return { state: next, value: (value ^ value >>> 16) >>> 0 };
  }

  function seededRecipeSelection(seed) {
    if (!/^[A-Fa-f0-9]{8}$/.test(String(seed)))
      throw new RangeError("A seed da receita deve conter oito dígitos hexadecimais.");
    const numericSeed = Number.parseInt(String(seed), 16) >>> 0;
    let state = numericSeed;
    const draw = () => {
      const result = nextSeedWord(state);
      state = result.state;
      return result.value;
    };
    const voiceTwoCycleIndex = SEEDED_CYCLE_INDICES[draw() % SEEDED_CYCLE_INDICES.length];
    const phaseIndex = draw() % 101;
    const eventRuleIndex = draw() % 2;
    const patternIndex = draw() % 3;
    state = numericSeed;
    for (let index = 0; index < 6; index += 1) draw();
    const voiceCount = Math.min(MAX_VOICES, 2 + draw() % 11);
    state = numericSeed;
    for (let index = 0; index < 4; index += 1) draw();
    const intensityLawIndex = draw() % 4;
    state = numericSeed;
    for (let index = 0; index < 5; index += 1) draw();
    const intensityAmount = 0.35 + (draw() % 66) / 100;
    const patternModes = ["unrestricted", "adhesive-pads", "diagonal-lanes"];
    return {
      voiceTwoCycleIndex,
      voiceTwoCycles: NATIVE_CYCLE_CHOICES[voiceTwoCycleIndex],
      phaseIndex,
      phase: phaseIndex * 0.005,
      eventRuleIndex,
      recipe: eventRuleIndex === 0 ? "center" : "turn",
      patternIndex,
      patternMode: patternModes[patternIndex],
      voiceCount,
      intensityLawIndex,
      intensityLaw: ["uniform", "projection-y", "product-xy", "quadratic-difference"][intensityLawIndex],
      intensityAmount
    };
  }

  function algorithmicNextSeed(seed) {
    if (!/^[A-Fa-f0-9]{8}$/.test(String(seed)))
      throw new RangeError("A seed atual deve conter oito dígitos hexadecimais.");
    const currentSeed = Number.parseInt(String(seed), 16) >>> 0;
    const current = seededRecipeSelection(String(seed));
    let state = (currentSeed ^ 0xB47A5EED) >>> 0;
    let bestSeed = currentSeed;
    let bestScore = -1;
    for (let candidateIndex = 0; candidateIndex < 64; candidateIndex += 1) {
      const next = nextSeedWord(state);
      state = next.state;
      const candidateSeed = next.value >>> 0;
      if (candidateSeed === currentSeed) continue;
      const candidate = seededRecipeSelection(candidateSeed.toString(16).toUpperCase().padStart(8, "0"));
      const phaseDelta = Math.abs(current.phaseIndex - candidate.phaseIndex);
      const phaseDistance = Math.min(phaseDelta, 101 - phaseDelta);
      const amountDistance = Math.round(Math.abs(
        current.intensityAmount - candidate.intensityAmount) * 100);
      let score = Math.abs(current.voiceCount - candidate.voiceCount) * 100;
      score += Math.abs(current.voiceTwoCycleIndex - candidate.voiceTwoCycleIndex) * 80;
      score += phaseDistance * 2;
      score += current.eventRuleIndex !== candidate.eventRuleIndex ? 70 : 0;
      score += current.patternIndex !== candidate.patternIndex ? 70 : 0;
      score += current.intensityLawIndex !== candidate.intensityLawIndex ? 50 : 0;
      score += amountDistance * 2;
      let differingBits = (currentSeed ^ candidateSeed) >>> 0;
      while (differingBits !== 0) {
        score += differingBits & 1;
        differingBits >>>= 1;
      }
      if (score > bestScore) {
        bestScore = score;
        bestSeed = candidateSeed;
      }
    }
    if (bestSeed === currentSeed) {
      const next = nextSeedWord(state);
      bestSeed = next.value >>> 0;
    }
    return bestSeed.toString(16).toUpperCase().padStart(8, "0");
  }

  function seedHash(seed) {
    let hash = 2166136261;
    for (const character of seed) hash = Math.imul(hash ^ character.charCodeAt(0), 16777619);
    return hash >>> 0;
  }

  function analyseSeedBassPath(eventsByCycle, seed, memoryLeak = Math.fround(0.72)) {
    const offsets = Array(7).fill(0);
    const integrals = Array(7).fill(0);
    const derivatives = Array(7).fill(0);
    const curvatures = Array(7).fill(0);
    const speeds = Array(7).fill(0);
    const signedCurvatures = Array(7).fill(0);
    let accumulated = ((seed & 0xff) / 255 - 0.5) * 0.7;
    integrals[0] = accumulated;
    let previousField = 0;
    let previousDerivative = 0;
    let previousMove = 0;
    const moves = [-5, -3, -2, 0, 2, 3, 5];
    for (let cycle = 1; cycle < 7; cycle += 1) {
      const source = eventsByCycle[cycle].filter((event) =>
        Number.isFinite(event.bassSourceX ?? event.fieldX)
        && Number.isFinite(event.bassSourceY ?? event.fieldY)
        && Number.isFinite(event.bassSourceIntensity ?? event.intensity));
      const sourceX = (event) => Math.max(-1, Math.min(1,
        event.bassSourceX ?? event.fieldX));
      const sourceY = (event) => Math.max(-1, Math.min(1,
        event.bassSourceY ?? event.fieldY));
      const sourceIntensity = (event) => Math.max(0, Math.min(1,
        Math.fround(event.bassSourceIntensity ?? event.intensity)));
      const count = source.length;
      const meanX = count ? source.reduce((sum, event) =>
        sum + sourceX(event), 0) / count : 0;
      const meanY = count ? source.reduce((sum, event) =>
        sum + sourceY(event), 0) / count : 0;
      const varianceX = count ? Math.max(0, source.reduce((sum, event) => {
        const x = sourceX(event); return sum + x * x;
      }, 0) / count - meanX * meanX) : 0;
      const varianceY = count ? Math.max(0, source.reduce((sum, event) => {
        const y = sourceY(event); return sum + y * y;
      }, 0) / count - meanY * meanY) : 0;
      const covariance = count ? source.reduce((sum, event) =>
        sum + sourceX(event) * sourceY(event), 0) / count - meanX * meanY : 0;
      const denominator = Math.sqrt(varianceX * varianceY);
      const correlation = denominator > 1e-9
        ? Math.max(-1, Math.min(1, covariance / denominator)) : 0;
      const regression = varianceX > 1e-9 ? Math.tanh(covariance / varianceX) : 0;
      const field = Math.max(-1, Math.min(1, meanY - meanX));
      const activity = count ? source.reduce((sum, event) =>
        sum + sourceIntensity(event), 0) / count : 0;
      const meanSignedCurvature = count ? source.reduce((sum, event) =>
        sum + Math.max(-1, Math.min(1, event.pathSignedCurvature || 0)), 0) / count : 0;
      const derivative = field - previousField;
      const curvature = derivative - previousDerivative;
      accumulated = Math.max(-1.5, Math.min(1.5,
        Math.max(0, Math.min(0.95, memoryLeak)) * accumulated + field));
      integrals[cycle] = accumulated;
      derivatives[cycle] = derivative;
      curvatures[cycle] = curvature;
      signedCurvatures[cycle] = meanSignedCurvature;
      const route = ((seed >>> 3)
        + (previousMove < 0 ? 1 : previousMove > 0 ? 2 : 0)
        + (activity > 0.7 && (cycle & 1) ? 2 : 0)
        + (cycle === 3 ? 1 : 0)) & 3;
      const exponential = 4 * Math.expm1(1.1 * field) / Math.expm1(1.1);
      const opposition = -4 * Math.sin(Math.PI / 2 * field);
      const orbit = 4 * Math.sin(Math.atan2(meanY, meanX) + cycle * 0.8975979010256552);
      const radius = Math.hypot(meanX, meanY);
      const rupture = cycle === 3 && radius > 0.55
        ? (field >= 0 ? -5 : 5) : 2 * Math.tanh(2 * field);
      const target = Math.max(-5, Math.min(5,
        (route === 0 ? exponential : route === 1 ? opposition : route === 2 ? orbit : rupture)
        + (activity - 0.5) + 1.15 * accumulated + 0.55 * derivative
        + 0.22 * curvature + 0.85 * correlation + 0.65 * regression
        + 0.45 * meanSignedCurvature));
      let bestMove = 0;
      let bestCost = Infinity;
      for (const move of moves) {
        const next = offsets[cycle - 1] + move;
        if (next < -9 || next > 9) continue;
        let hash = (seed ^ Math.imul(0x9e3779b9, cycle)
          ^ Math.imul(0x85ebca6b, move + 6)) >>> 0;
        hash ^= hash >>> 16;
        hash = Math.imul(hash, 0x7feb352d) >>> 0;
        hash ^= hash >>> 15;
        const tieBreak = (hash & 0xffff) / 65535;
        let cost = Math.abs(move - target) + 0.22 * Math.abs(move - previousMove)
          + 0.12 * Math.abs(move) + 0.31 * tieBreak;
        if (cycle >= 5) cost += (cycle === 6 ? 1 : 0.48) * Math.abs(next);
        if (cycle === 4) cost += 0.75 * Math.abs(move);
        if (cost < bestCost) { bestCost = cost; bestMove = move; }
      }
      offsets[cycle] = offsets[cycle - 1] + bestMove;
      previousMove = bestMove;
      previousField = field;
      previousDerivative = derivative;
    }
    return { offsets, integrals, derivatives, curvatures, speeds,
      signedCurvatures };
  }

  function seededRandom(seed) {
    let state = seed >>> 0;
    return function () {
      state += 0x6D2B79F5;
      let value = state;
      value = Math.imul(value ^ value >>> 15, value | 1);
      value ^= value + Math.imul(value ^ value >>> 7, value | 61);
      return ((value ^ value >>> 14) >>> 0) / 4294967296;
    };
  }

  function wrapPhaseOffset(phase) {
    return phase - Math.floor(phase + 0.5);
  }

  function bassOscillatorProfileForSeed(seed) {
    if (!/^[A-Fa-f0-9]{8}$/.test(String(seed))) {
      throw new RangeError("A seed do perfil espectral deve conter oito dígitos hexadecimais.");
    }
    let mixed = Number.parseInt(String(seed), 16) >>> 0;
    mixed ^= mixed >>> 16;
    mixed = Math.imul(mixed, 0x7feb352d) >>> 0;
    mixed ^= mixed >>> 15;
    mixed = Math.imul(mixed, 0x846ca68b) >>> 0;
    mixed ^= mixed >>> 16;
    return mixed & 3;
  }

  function polyrhythmPreset(name) {
    const cycles = POLYRHYTHM_PRESETS[name];
    return cycles ? [...cycles] : null;
  }

  // Find the first event that may still be scheduled after a composition
  // revision. Events at the cutoff remain eligible; everything before it
  // belongs to the old revision or is already inside the audio scheduling
  // horizon.
  function eventCursorAtOrAfter(events, cutoffSeconds) {
    if (!Array.isArray(events) || !Number.isFinite(cutoffSeconds))
      throw new RangeError("A agenda e o limite temporal devem ser válidos.");
    let low = 0;
    let high = events.length;
    while (low < high) {
      const middle = low + Math.floor((high - low) / 2);
      if (events[middle].time < cutoffSeconds) low = middle + 1;
      else high = middle;
    }
    return low;
  }

  function applyPolyrhythmPreset(name, currentVoiceCount, currentVoiceCycles) {
    const cycles = polyrhythmPreset(name);
    if (!cycles) return null;
    if (!Number.isInteger(currentVoiceCount) || currentVoiceCount < 2 ||
        currentVoiceCount > MAX_VOICES || !Array.isArray(currentVoiceCycles) ||
        currentVoiceCycles.length < currentVoiceCount ||
        currentVoiceCycles.length > MAX_VOICES) {
      throw new RangeError("Estado atual de vozes inválido para aplicar polirritmia.");
    }
    const voiceCycles = [...currentVoiceCycles];
    cycles.forEach((value, voice) => { voiceCycles[voice] = value; });
    return { voiceCount: Math.max(currentVoiceCount, cycles.length), voiceCycles,
      variation: 0 };
  }

  function seededBassPattern(seed, variation, pattern) {
    const result = pattern.map((step) => step && ({ ...step }));
    if (variation <= 0) return result;
    const pitchClasses = [...new Set(pattern.filter(Boolean).map((step) => step.midi % 12))];
    if (pitchClasses.length < 2) return result;
    const random = seededRandom(seedHash(seed) ^ 0xBA551EED);
    const radius = 2 + Math.round(5 * variation);
    for (let index = 0; index < result.length; index += 1) {
      const step = result[index];
      if (!step || random() >= variation) continue;
      const options = [];
      for (let midi = 24; midi <= 60; midi += 1) {
        if (midi !== step.midi && pitchClasses.includes(midi % 12) &&
            Math.abs(midi - step.midi) <= radius) options.push(midi);
      }
      if (options.length) step.midi = options[Math.floor(random() * options.length)];
    }
    return result;
  }

  function normalizeConfig(input) {
    if (!input || (input.recipe !== "center" && input.recipe !== "turn")) {
      throw new RangeError("A receita deve ser center ou turn.");
    }
    if (!Number.isFinite(input.phase) || input.phase < 0 || input.phase > 0.5) {
      throw new RangeError("A fase deve estar entre 0 e 0,5 volta.");
    }
    const cycleSeconds = input.durationSeconds === undefined ? input.cycleSeconds :
      input.durationSeconds / 7;
    if (!Number.isFinite(cycleSeconds) || cycleSeconds < 2 || cycleSeconds > 12) {
      throw new RangeError("O ciclo deve durar entre 2 e 12 segundos.");
    }
    const motionModel = input.motionModel || "functions";
    const pendulumLength = input.pendulumLength === undefined ? 1 : input.pendulumLength;
    const pendulumCoupling = input.pendulumCoupling === undefined ? 0.35 : input.pendulumCoupling;
    const pendulumDamping = input.pendulumDamping === undefined ? 0.04 : input.pendulumDamping;
    if (!["functions", "coupled-pendulum"].includes(motionModel) ||
        !Number.isFinite(pendulumLength) || pendulumLength < 0.4 || pendulumLength > 2 ||
        !Number.isFinite(pendulumCoupling) || pendulumCoupling < 0 || pendulumCoupling > 1.5 ||
        !Number.isFinite(pendulumDamping) || pendulumDamping < 0 || pendulumDamping > 0.3) {
      throw new RangeError("Parâmetros do pêndulo acoplado inválidos.");
    }
    if (!INTENTIONS.includes(input.intention || "meet") ||
        !DYNAMICS.includes(input.dynamics || "arch") ||
        !GROOVES.includes(input.groove || "straight")) {
      throw new RangeError("Intenção, dinâmica ou groove inválido.");
    }
    if (!['short', 'balanced', 'long'].includes(input.articulation || 'balanced') ||
        !['sine', 'triangle', 'sawtooth', 'square'].includes(input.timbre || 'triangle')) {
      throw new RangeError("Articulação ou timbre inválido.");
    }
    const bpm = input.bpm === undefined ? 120 : input.bpm;
    const meter = input.meter || "4/4";
    if (!Number.isFinite(bpm) || bpm < 40 || bpm > 240 || !METERS.includes(meter)) {
      throw new RangeError("BPM deve estar entre 40 e 240 e o compasso deve ser suportado.");
    }
    const swing = input.swing === undefined ? 0 : input.swing;
    if (!Number.isFinite(swing) || swing < 0 || swing > 0.67) {
      throw new RangeError("O swing deve estar entre 0 e 0,67.");
    }
    const bassRate = input.bassRate === undefined ? 2 : input.bassRate;
    const bassDepth = input.bassDepth === undefined ? 7 : input.bassDepth;
    const bassShape = input.bassShape || "triangle";
    const bassTimbre = input.bassTimbre || "round";
    const bassOscillatorProfile = input.bassOscillatorProfile || "harmonic";
    const bassVcfCutoffHz = input.bassVcfCutoffHz === undefined ? 420 : input.bassVcfCutoffHz;
    const bassVcfResonance = input.bassVcfResonance === undefined ? 0.707 : input.bassVcfResonance;
    const bassOutputLevel = input.bassOutputLevel === undefined ? 0.2 : input.bassOutputLevel;
    const bassAmpAttackSeconds = input.bassAmpAttackSeconds === undefined ? 0.008 : input.bassAmpAttackSeconds;
    const bassAmpDecaySeconds = input.bassAmpDecaySeconds === undefined ? 0.18 : input.bassAmpDecaySeconds;
    const bassAmpSustain = input.bassAmpSustain === undefined ? 0.62 : input.bassAmpSustain;
    const bassAmpReleaseSeconds = input.bassAmpReleaseSeconds === undefined ? 0.12 : input.bassAmpReleaseSeconds;
    const bassFilterEnvelopeAmountHz = input.bassFilterEnvelopeAmountHz === undefined ? 900 : input.bassFilterEnvelopeAmountHz;
    const bassFilterAttackSeconds = input.bassFilterAttackSeconds === undefined ? 0.006 : input.bassFilterAttackSeconds;
    const bassFilterDecaySeconds = input.bassFilterDecaySeconds === undefined ? 0.243 : input.bassFilterDecaySeconds;
    const bassFilterSustain = input.bassFilterSustain === undefined ? 0.12 : input.bassFilterSustain;
    const bassFilterReleaseSeconds = input.bassFilterReleaseSeconds === undefined ? 0.12 : input.bassFilterReleaseSeconds;
    const bassGateRatio = input.bassGateRatio === undefined ? 0.86 : input.bassGateRatio;
    const bassLfoEnabled = input.bassLfoEnabled === true;
    const bassPhraseMode = input.bassPhraseMode || "loop";
    const bassRhythmRelation = input.bassRhythmRelation || "follow";
    const bassDurationFormula = input.bassDurationFormula || "manual";
    const bassVariationLevel = input.bassVariationLevel === undefined
      ? 2 : input.bassVariationLevel;
    const bassPathBreath = input.bassPathBreath === undefined
      ? 1 : input.bassPathBreath;
    const bassGeoRatio = input.bassGeoRatio === undefined ? 1.5 : input.bassGeoRatio;
    const geometryShape = input.geometryShape || "compass";
    const geometryFourierTerms = input.geometryFourierTerms === undefined ? 8 : input.geometryFourierTerms;
    const geometryBilliardPattern = input.geometryBilliardPattern === undefined ? 0 : input.geometryBilliardPattern;
    const geometrySource = input.geometrySource === true;
    const bassPattern = input.bassPattern ? input.bassPattern.map((step) => step && ({ ...step })) :
      DEFAULT_BASS_PATTERN.map((step) => step && ({ ...step }));
    const bassVoiceDriven = input.bassVoiceDriven === true;
    if (![1, 2, 4].includes(bassRate) || !Number.isFinite(bassDepth) || bassDepth < 0 || bassDepth > 12 ||
        !Number.isFinite(bassVcfCutoffHz) || bassVcfCutoffHz < 30 || bassVcfCutoffHz > 6000 ||
        !Number.isFinite(bassVcfResonance) || bassVcfResonance < 0.5 || bassVcfResonance > 14 ||
        !Number.isFinite(bassOutputLevel) || bassOutputLevel < 0 || bassOutputLevel > 0.8 ||
        !Number.isFinite(bassAmpAttackSeconds) || bassAmpAttackSeconds < 0.001 || bassAmpAttackSeconds > 1 ||
        !Number.isFinite(bassAmpDecaySeconds) || bassAmpDecaySeconds < 0.005 || bassAmpDecaySeconds > 2 ||
        !Number.isFinite(bassAmpSustain) || bassAmpSustain < 0 || bassAmpSustain > 1 ||
        !Number.isFinite(bassAmpReleaseSeconds) || bassAmpReleaseSeconds < 0.005 || bassAmpReleaseSeconds > 3 ||
        !Number.isFinite(bassFilterEnvelopeAmountHz) || bassFilterEnvelopeAmountHz < 0 || bassFilterEnvelopeAmountHz > 2400 ||
        !Number.isFinite(bassFilterAttackSeconds) || bassFilterAttackSeconds < 0.001 || bassFilterAttackSeconds > 1 ||
        !Number.isFinite(bassFilterDecaySeconds) || bassFilterDecaySeconds < 0.005 || bassFilterDecaySeconds > 2 ||
        !Number.isFinite(bassFilterSustain) || bassFilterSustain < 0 || bassFilterSustain > 1 ||
        !Number.isFinite(bassFilterReleaseSeconds) || bassFilterReleaseSeconds < 0.005 || bassFilterReleaseSeconds > 3 ||
        !Number.isFinite(bassGateRatio) || bassGateRatio < 0.4 || bassGateRatio > 1 ||
        !["sine", "triangle", "square"].includes(bassShape) ||
        !["sub", "round", "body", "pulse", "bright", "pluck"].includes(bassTimbre) ||
        !["harmonic", "odd", "inharmonic", "beat"].includes(bassOscillatorProfile) ||
        !["loop", "breath", "cadence", "call-response"].includes(bassPhraseMode) || bassPattern.length !== 20 ||
        !["manual", "fibonacci", "geometric"].includes(bassDurationFormula) ||
        ![1.2, 1.5, 1.618, 2].includes(bassGeoRatio) ||
        !Number.isInteger(bassVariationLevel) || bassVariationLevel < 0 || bassVariationLevel > 3 ||
        !Number.isFinite(bassPathBreath) || bassPathBreath < 0 || bassPathBreath > 2 ||
        !["independent", "follow", "answer", "convolution"].includes(bassRhythmRelation) ||
        bassPattern.some((step) => step && (!Number.isInteger(step.midi) || step.midi < 24 || step.midi > 60 ||
          ![1, 2, 4, 8, 16].includes(step.durationSteps) || ![1, 2, 4, 8].includes(step.repeats || 1) ||
          !["normal", "ghost", "accent"].includes(step.intent || "normal")))) {
      throw new RangeError("Parâmetros do LFO de baixo inválidos.");
    }
    const durationSeconds = cycleSeconds * 7;
    const voiceCount = input.voiceCount === undefined ? 2 : input.voiceCount;
    if (!Number.isInteger(voiceCount) || voiceCount < 2 || voiceCount > MAX_VOICES) {
      throw new RangeError("A quantidade de vozes deve estar entre 2 e 8.");
    }
    const voiceCycleBases = input.voiceCycleBases ? [...input.voiceCycleBases]
      : input.voiceCycles ? [...input.voiceCycles] : DEFAULT_CYCLES.slice(0, voiceCount);
    const couplingBases = input.couplingBases ? [...input.couplingBases]
      : input.couplings ? [...input.couplings] : Array(voiceCount).fill(0);
    const voiceEventRelations = input.voiceEventRelations
      ? [...input.voiceEventRelations] : Array(voiceCount).fill("none");
    const voiceTransformBases = input.voiceTransformBases ? [...input.voiceTransformBases]
      : input.voiceTransforms ? [...input.voiceTransforms] : Array(voiceCount).fill("sine");
    const seed = input.seed === undefined ? "4E415632" : String(input.seed);
    const variation = input.variation === undefined ? 0 : input.variation;
    const bassPitchVariationBase = input.bassPitchVariationBase === undefined
      ? 0 : input.bassPitchVariationBase;
    const bassPitchVariation = Math.max(variation, bassPitchVariationBase);
    if (voiceCycleBases.length !== voiceCount || voiceCycleBases.some((value) =>
      !Number.isFinite(value) || value < 0.5 || value > 16 ||
      Math.abs(value * 4 - Math.round(value * 4)) > 1e-9)) {
      throw new RangeError("Cada voz deve ter de 0,5 a 16 oscilações, em passos de 0,25.");
    }
    if (couplingBases.length !== voiceCount || couplingBases.some((value, voice) =>
      !Number.isFinite(value) || value < 0 || value > 0.8 || (voice === 0 && value !== 0))) {
      throw new RangeError("A mistura com a voz anterior deve estar entre 0 e 0,8.");
    }
    if (voiceTransformBases.length !== voiceCount || voiceTransformBases.some((value) => !VOICE_TRANSFORMS.includes(value))) {
      throw new RangeError("Transformação matemática de voz inválida.");
    }
    if (!/^[A-Fa-f0-9]{8}$/.test(seed) || !Number.isFinite(variation) || variation < 0 || variation > 1 ||
        !Number.isFinite(bassPitchVariationBase) || bassPitchVariationBase < 0 || bassPitchVariationBase > 1) {
      throw new RangeError("A seed deve ter oito dígitos hexadecimais e a variação ficar entre 0 e 1.");
    }
    const bassSeededPattern = seededBassPattern(seed, variation, bassPattern);
    if (!GEOMETRY_SHAPES.includes(geometryShape) || !Number.isInteger(geometryBilliardPattern) ||
        geometryBilliardPattern < 0 || geometryBilliardPattern >= BILLIARD_PATTERNS.length ||
        !Number.isInteger(geometryFourierTerms) ||
        geometryFourierTerms < 1 || geometryFourierTerms > MAX_GEOMETRY_FOURIER_TERMS)
      throw new RangeError("Forma geométrica ou quantidade de componentes de Fourier inválida.");
    const voiceCycles = [...voiceCycleBases];
    const couplings = [...couplingBases];
    const voiceTransforms = [...voiceTransformBases];
    const voicePhaseOffsets = Array(voiceCount).fill(0);
    const voiceBasePhases = input.voiceBasePhases ? [...input.voiceBasePhases] : Array(voiceCount).fill(0);
    const patternMode = input.patternMode || "unrestricted";
    const intensityLaw = input.intensityLaw || "uniform";
    const intensityAmount = input.intensityAmount === undefined ? 0.35 : input.intensityAmount;
    if (voiceBasePhases.length !== voiceCount || voiceBasePhases.some((value) =>
      !Number.isFinite(value) || value < -2 || value > 2) ||
      voiceEventRelations.length !== voiceCount ||
      voiceEventRelations.some((value, voice) =>
        !["none", "support", "imitation", "avoidance", "competition"].includes(value) ||
        (voice === 0 && value !== "none")) ||
      !["unrestricted", "adhesive-pads", "diagonal-lanes"].includes(patternMode) ||
      !["uniform", "projection-y", "product-xy", "quadratic-difference", "gaussian-centre",
        "cosine-bands", "potential-energy", "kinetic-energy"].includes(intensityLaw) ||
      !Number.isFinite(intensityAmount) || intensityAmount < 0 || intensityAmount > 1) {
      throw new RangeError("Receita seeded de padrão, fase ou intensidade inválida.");
    }
    const random = seededRandom(seedHash(seed));
    const maxCycleDelta = Math.round(3 * variation);
    for (let voice = 0; voice < voiceCount; voice += 1) {
      if (variation <= 0) continue;
      const cycleDelta = Math.floor(random() * (2 * maxCycleDelta + 1)) - maxCycleDelta;
      voiceCycles[voice] = Math.max(1, Math.min(12, voiceCycleBases[voice] + cycleDelta));
      if (voice > 0) {
        couplings[voice] = Math.max(0, Math.min(0.8,
          couplingBases[voice] + (random() * 2 - 1) * 0.25 * variation));
        const phaseJitter = (random() * 2 - 1) * 0.125 * variation;
        const stagger = (voice - 1) / 16;
        voicePhaseOffsets[voice] = wrapPhaseOffset(stagger + phaseJitter) - stagger;
      }
      if (random() < variation) {
        let selected = Math.floor(random() * (VOICE_TRANSFORMS.length - 1));
        if (VOICE_TRANSFORMS[selected] === voiceTransformBases[voice]) selected += 1;
        voiceTransforms[voice] = VOICE_TRANSFORMS[selected];
      }
    }
    return { ...input, durationSeconds, cycleSeconds, voiceCount, voiceCycles, couplings,
      voiceCycleBases, couplingBases, voiceTransformBases, voiceTransforms, voicePhaseOffsets,
      voiceBasePhases, voiceEventRelations, patternMode, intensityLaw, intensityAmount,
      seed, variation, bassPitchVariationBase, bassPitchVariation,
      bassVariationLevel, bassPathBreath,
      geometryShape, geometryBilliardPattern, geometrySource, motionModel,
      pendulumLength, pendulumCoupling, pendulumDamping, geometryFourierTerms,
      intention: input.intention || "meet", dynamics: input.dynamics || "arch",
      groove: input.groove || "straight", swing,
      bpm, meter,
      bassRate, bassDepth, bassShape, bassTimbre, bassLfoEnabled, bassPhraseMode,
      bassRhythmRelation,
      bassOscillatorProfile, bassVcfCutoffHz, bassVcfResonance,
      bassOutputLevel,
      bassAmpAttackSeconds, bassAmpDecaySeconds, bassAmpSustain,
      bassAmpReleaseSeconds, bassFilterEnvelopeAmountHz,
      bassFilterAttackSeconds, bassFilterDecaySeconds, bassFilterSustain,
      bassFilterReleaseSeconds, bassGateRatio,
      bassDurationFormula, bassGeoRatio, bassPattern,
      bassVoiceDriven,
      bassSeededPattern,
      articulation: input.articulation || 'balanced', timbre: input.timbre || 'triangle' };
  }

  function integrateTriplePendulum(config) {
    const step = config.cycleSeconds / SAMPLES;
    const sampleCount = 7 * SAMPLES + 1;
    const angles = Array.from({ length: 3 }, () => new Float32Array(sampleCount));
    const velocities = Array.from({ length: 3 }, () => new Float32Array(sampleCount));
    const energies = new Float32Array(sampleCount);
    const state = [0.22, -0.08, 0.15, 0, 0.12, -0.04];
    const omega0 = Math.sqrt(9.80665 / config.pendulumLength);
    const spring = config.pendulumCoupling * omega0 * omega0;
    const derivative = (value) => {
      const result = new Array(6);
      for (let voice = 0; voice < 3; voice += 1) {
        const theta = value[voice];
        let couplingForce = 0;
        if (voice > 0) couplingForce += spring * (value[voice - 1] - theta);
        if (voice < 2) couplingForce += spring * (value[voice + 1] - theta);
        result[voice] = value[voice + 3];
        result[voice + 3] = -omega0 * omega0 * theta
          + couplingForce - 2 * config.pendulumDamping * omega0 * value[voice + 3];
      }
      return result;
    };
    const shifted = (value, slope, amount) => value.map((entry, index) =>
      entry + slope[index] * amount);
    let maxAngle = 0;
    let maxEnergy = 0;
    for (let sample = 0; sample < sampleCount; sample += 1) {
      let kinetic = 0;
      let potential = 0;
      for (let voice = 0; voice < 3; voice += 1) {
        angles[voice][sample] = state[voice];
        velocities[voice][sample] = state[voice + 3];
        maxAngle = Math.max(maxAngle, Math.abs(state[voice]));
        kinetic += 0.5 * state[voice + 3] * state[voice + 3];
        potential += 0.5 * omega0 * omega0 * state[voice] * state[voice];
      }
      for (let voice = 0; voice < 2; voice += 1) {
        const delta = state[voice + 1] - state[voice];
        potential += 0.5 * config.pendulumCoupling * omega0 * omega0 * delta * delta;
      }
      energies[sample] = kinetic + potential;
      maxEnergy = Math.max(maxEnergy, energies[sample]);
      if (sample === sampleCount - 1) break;
      const k1 = derivative(state);
      const k2 = derivative(shifted(state, k1, step / 2));
      const k3 = derivative(shifted(state, k2, step / 2));
      const k4 = derivative(shifted(state, k3, step));
      for (let index = 0; index < state.length; index += 1) {
        state[index] += step * (k1[index] + 2 * k2[index] + 2 * k3[index] + k4[index]) / 6;
      }
    }
    return { angles, velocities, energies, maxAngle: Math.max(maxAngle, 1e-8),
      maxEnergy: Math.max(maxEnergy, 1e-8), sampleCount, step, omega0 };
  }

  function expressionAt(progress, config) {
    const p = Math.max(0, Math.min(1, progress));
    let curve = 1;
    if (config.dynamics === "crescendo") curve = 0.35 + 0.9 * p;
    else if (config.dynamics === "arch") curve = 0.38 + 0.85 * Math.sin(Math.PI * p);
    else if (config.dynamics === "waves") curve = 0.5 + 0.5 * (0.5 + 0.5 * Math.sin(4 * Math.PI * p - Math.PI / 2));
    if (config.intention === "build") curve *= 0.68 + 0.58 * p;
    else if (config.intention === "thin") curve *= 1.18 - 0.62 * p;
    else if (config.intention === "contrast") curve *= (Math.floor(p * 14) % 2 === 0 ? 1.13 : 0.78);
    return Math.max(0.08, Math.min(1.35, curve));
  }

  function swingPosition(u, ratio, cycleSeconds, bpm) {
    if (ratio <= 0.5) return u;
    const quarterNotesPerSection = cycleSeconds * bpm / 60;
    const beatPosition = u * quarterNotesPerSection;
    const beat = Math.floor(beatPosition);
    // Keep a final incomplete beat intact so each cycle lands on its boundary.
    if (beat >= Math.floor(quarterNotesPerSection + 1e-10)) return u;
    return swingProgress(u, ratio, quarterNotesPerSection);
  }

  function swingProgress(progress, ratio, quarterNotesPerSection) {
    if (![progress, ratio, quarterNotesPerSection].every(Number.isFinite) ||
        quarterNotesPerSection <= 0)
      throw new RangeError("Parâmetros de swing inválidos.");
    const bounded = Math.max(0, Math.min(1, progress));
    const boundedRatio = Math.max(0.5, Math.min(0.67, ratio));
    const beatPosition = bounded * quarterNotesPerSection;
    const beatFraction = beatPosition - Math.floor(beatPosition);
    const moved = beatFraction < 0.5
      ? beatFraction * 2 * boundedRatio
      : boundedRatio + (beatFraction - 0.5) * 2 * (1 - boundedRatio);
    return (Math.floor(beatPosition) + moved) / quarterNotesPerSection;
  }

  function grooveAccent(cycle, u, groove, bpm, meter, cycleSeconds) {
    const [numerator, denominator] = meter.split("/").map(Number);
    const eighthsPerBar = numerator * 8 / denominator;
    const totalEighths = (cycle * cycleSeconds + u * cycleSeconds) * bpm / 30;
    const step = ((Math.round(totalEighths + 1e-9) % eighthsPerBar) + eighthsPerBar) % eighthsPerBar;
    const beatStep = 8 / denominator;
    const accents = {
      straight: [0],
      ternary: Array.from({ length: 3 }, (_, index) =>
        Math.round(index * eighthsPerBar / 3) % eighthsPerBar),
      additive: [0, 3, 6].filter((value) => value < eighthsPerBar),
      offbeat: Array.from({ length: Math.ceil(eighthsPerBar / 2) }, (_, index) =>
        (index * 2 + 1) % eighthsPerBar)
    }[groove];
    if (groove === "straight" && step > 0 && step % beatStep === 0) return 1.0;
    return accents.includes(step) ? 1.16 : 0.82;
  }

  function intentionFactor(event, config, proximity) {
    if (config.intention === "meet") return 1 + 0.36 * proximity;
    if (config.intention === "contrast") return event.voice % 2 === 0 ? 1.12 : 0.82;
    return 1;
  }

  function phaseFor(cycle, voice, transformedPhase, localProgress = 1, config = {}) {
    if (voice === 0) return 0;
    const initialOffset = (voice - 1) / 16 + (config.voiceBasePhases?.[voice] || 0)
      + (config.voicePhaseOffsets?.[voice] || 0);
    const segment = formalPhaseSegment(cycle, transformedPhase);
    const progress = Math.max(0, Math.min(1, localProgress));
    return initialOffset + segment.offsetCycles + segment.slopeCycles * progress;
  }

  function formalPhaseSegment(cycle, amount) {
    if (!Number.isInteger(cycle) || !Number.isFinite(amount))
      throw new RangeError("Ciclo formal ou valor de fase inválido.");
    switch ((cycle % 7 + 7) % 7) {
      case 2: return { offsetCycles: 0, slopeCycles: 0.5 * amount };
      case 3: return { offsetCycles: 0.5 * amount, slopeCycles: 0.5 * amount };
      case 4: return { offsetCycles: amount, slopeCycles: 0 };
      case 5: return { offsetCycles: amount, slopeCycles: -0.5 * amount };
      case 6: return { offsetCycles: 0.5 * amount, slopeCycles: -0.5 * amount };
      default: return { offsetCycles: 0, slopeCycles: 0 };
    }
  }

  function activeVoiceCountInCycle(cycle, voiceCount) {
    if (!Number.isInteger(cycle) || !Number.isInteger(voiceCount) ||
        voiceCount < 1 || voiceCount > MAX_VOICES)
      throw new RangeError("Ciclo formal ou quantidade de vozes inválida.");
    switch ((cycle % 7 + 7) % 7) {
      case 0: return 1;
      case 1: case 5: return Math.ceil((voiceCount + 1) / 2);
      default: return voiceCount;
    }
  }

  function valueAt(cycle, voice, u, config) {
    if (config.motionModel === "coupled-pendulum" && voice < 3
        && config.pendulumTrajectory) {
      const state = pendulumStateAt(cycle, voice, u, config);
      return Math.max(-1, Math.min(1,
        state.angle / config.pendulumTrajectory.maxAngle));
    }
    const theta = TAU * (config.voiceCycles[voice] * u + phaseFor(cycle, voice, config.phase, u, config));
    const sine = Math.sin(theta);
    const geometryValue = config.geometrySource && voice === 0 ?
      geometryPoint(config.geometryShape, u, config.voiceCycles[voice],
        phaseFor(cycle, voice, config.phase, u, config), config.geometryFourierTerms,
        config.geometryBilliardPattern).y : sine;
    const transform = config.voiceTransforms[voice];
    let own = geometryValue;
    if (transform === "invert") own = -geometryValue;
    else if (transform === "rectify") own = 2 * Math.abs(geometryValue) - 1;
    else if (transform === "harmonic") own = (geometryValue + 0.5 * Math.sin(2 * theta)) / 1.5;
    else if (transform === "amplitude") own = geometryValue * (0.65 + 0.35 * Math.cos(theta * 0.5));
    else if (transform === "quantized") own = Math.round(geometryValue * 4) / 4;
    else if (transform === "cubic") own = geometryValue * geometryValue * geometryValue;
    else if (transform === "thirdHarmonic") own = Math.sin(3 * theta);
    else if (transform === "triangle") own = (2 / Math.PI) * Math.asin(
      Math.max(-1, Math.min(1, geometryValue)));
    else if (transform === "softClip") own = Math.tanh(2 * geometryValue) / Math.tanh(2);
    if (voice === 0 || config.couplings[voice] === 0) return own;
    const amount = config.couplings[voice];
    const previous = valueAt(cycle, voice - 1, u, config);
    return (1 - amount) * own + amount * previous;
  }

  function pendulumStateAt(cycle, voice, u, config) {
    const model = config.pendulumTrajectory;
    if (!model || voice < 0 || voice >= 3)
      return { angle: 0, velocity: 0, energy: 0 };
    const within = Math.max(0, Math.min(1, u));
    const index = Math.min(model.sampleCount - 1,
      Math.max(0, Math.round((cycle + within) * SAMPLES)));
    return { angle: model.angles[voice][index],
      velocity: model.velocities[voice][index],
      energy: model.energies[index] / model.maxEnergy };
  }

  function geometryBaseShape(shape) {
    if (shape === "fourier-triangle") return "triangle";
    if (shape === "fourier-square") return "square";
    return shape;
  }

  function polygonPoint(shape, t) {
    const vertices = shape === "triangle" ?
      [[-0.9, -0.62], [0.9, -0.62], [0, 0.94]] :
      [[-0.78, -0.78], [0.78, -0.78], [0.78, 0.78], [-0.78, 0.78]];
    const position = t * vertices.length;
    const segment = Math.floor(position) % vertices.length;
    const fraction = position - Math.floor(position);
    const start = vertices[segment];
    const end = vertices[(segment + 1) % vertices.length];
    return { x: start[0] + (end[0] - start[0]) * fraction,
      y: start[1] + (end[1] - start[1]) * fraction };
  }

  function geometryFourierCoefficients(shape) {
    const source = geometryBaseShape(shape);
    if (!geometryFourierCache.has(source)) {
      const coefficients = new Map();
      for (let harmonic = -MAX_GEOMETRY_FOURIER_TERMS;
        harmonic <= MAX_GEOMETRY_FOURIER_TERMS; harmonic += 1) {
        let real = 0;
        let imaginary = 0;
        for (let sample = 0; sample < FOURIER_REFERENCE_SAMPLES; sample += 1) {
          const point = polygonPoint(source, sample / FOURIER_REFERENCE_SAMPLES);
          const angle = TAU * harmonic * sample / FOURIER_REFERENCE_SAMPLES;
          const cosine = Math.cos(angle);
          const sine = Math.sin(angle);
          real += point.x * cosine + point.y * sine;
          imaginary += point.y * cosine - point.x * sine;
        }
        coefficients.set(harmonic, {
          real: real / FOURIER_REFERENCE_SAMPLES,
          imaginary: imaginary / FOURIER_REFERENCE_SAMPLES
        });
      }
      geometryFourierCache.set(source, coefficients);
    }
    return geometryFourierCache.get(source);
  }

  function fourierPoint(shape, t, terms) {
    const coefficients = geometryFourierCoefficients(shape);
    let x = 0;
    let y = 0;
    for (let harmonic = -terms; harmonic <= terms; harmonic += 1) {
      const coefficient = coefficients.get(harmonic);
      const angle = TAU * harmonic * t;
      const cosine = Math.cos(angle);
      const sine = Math.sin(angle);
      x += coefficient.real * cosine - coefficient.imaginary * sine;
      y += coefficient.real * sine + coefficient.imaginary * cosine;
    }
    return { x, y };
  }

  function geometryPoint(shape, progress, cycles = 1, phase = 0, fourierTerms = 8,
                         billiardPattern = 0) {
    const t = ((progress * cycles + phase) % 1 + 1) % 1;
    if (shape === "fourier-triangle" || shape === "fourier-square")
      return fourierPoint(shape, t, fourierTerms);
    if (shape === "compass") {
      const angle = TAU * (progress * cycles + phase);
      return { x: Math.cos(angle), y: Math.sin(angle) };
    }
    if (shape === "spiral") {
      const angle = TAU * (progress * cycles + phase);
      const radius = Math.max(0, Math.min(1, progress));
      return { x: radius * Math.cos(angle), y: radius * Math.sin(angle) };
    }
    if (shape === "lissajous") {
      const angle = TAU * (progress * cycles + phase);
      return { x: Math.sin(3 * angle), y: Math.sin(2 * angle) };
    }
    if (shape === "epicycloid") {
      const angle = TAU * (progress * cycles + phase);
      return { x: (3 * Math.cos(angle) - Math.cos(3 * angle)) / 4,
        y: (3 * Math.sin(angle) - Math.sin(3 * angle)) / 4 };
    }
    if (shape === "hypocycloid") {
      const angle = TAU * (progress * cycles + phase);
      return { x: (2 * Math.cos(angle) + Math.cos(2 * angle)) / 3,
        y: (2 * Math.sin(angle) - Math.sin(2 * angle)) / 3 };
    }
    if (shape === "cycloid") {
      // Match the rolling-circle model in GeometryCurves.cpp: one cycloid
      // revolution across a normalized straight track.
      const rollingRadius = 1 / (Math.PI + 1);
      const angle = TAU * t;
      const centreX = rollingRadius * (angle - Math.PI);
      return { x: centreX - rollingRadius * Math.sin(angle),
        y: -rollingRadius * Math.cos(angle) };
    }
    if (shape === "catenary") {
      const x = Math.sin(TAU * (progress * cycles + phase));
      const height = (Math.cosh(x) - 1) / (Math.cosh(1) - 1);
      return { x, y: 2 * height - 1 };
    }
    if (shape === "billiard-square") {
      const laps = BILLIARD_PATTERNS[billiardPattern] || BILLIARD_PATTERNS[0];
      const width = 2 * BILLIARD_HALF;
      const fold = (position) => {
        const wrapped = ((position + BILLIARD_HALF) % (2 * width) + 2 * width) % (2 * width);
        return wrapped <= width ? -BILLIARD_HALF + wrapped : 3 * BILLIARD_HALF - wrapped;
      };
      return { x: fold(BILLIARD_START.x + laps[0] * width * (progress * cycles + phase)) / BILLIARD_HALF,
        y: fold(BILLIARD_START.y + laps[1] * width * (progress * cycles + phase)) / BILLIARD_HALF };
    }
    return polygonPoint(shape, t);
  }

  function billiardCollisionPositions(cycles = 1, phase = 0, pattern = 0) {
    const width = 2 * BILLIARD_HALF;
    const laps = BILLIARD_PATTERNS[pattern] || BILLIARD_PATTERNS[0];
    const deltaX = laps[0] * width;
    const deltaY = laps[1] * width;
    const oneCycle = [];
    for (let hit = 0; hit < laps[0]; hit += 1)
      oneCycle.push((BILLIARD_HALF - BILLIARD_START.x + hit * width) / deltaX);
    for (let hit = 0; hit < laps[1]; hit += 1)
      oneCycle.push((BILLIARD_HALF - BILLIARD_START.y + hit * width) / deltaY);
    const output = [];
    for (const collision of oneCycle)
      for (let turn = Math.floor(phase); turn <= cycles + phase; turn += 1) {
        const u = (turn + collision - phase) / cycles;
        if (u >= 0 && u < 1) output.push(u);
      }
    return output.sort((a, b) => a - b).filter((u, index, all) =>
      index === 0 || u - all[index - 1] > 1e-9);
  }

  function geometryPolygonMetrics(shape) {
    const vertices = shape === "triangle"
      ? [[-0.9, -0.62], [0.9, -0.62], [0, 0.94]]
      : shape === "square"
        ? [[-0.78, -0.78], [0.78, -0.78], [0.78, 0.78], [-0.78, 0.78]]
        : null;
    if (!vertices) return null;
    let twiceSignedArea = 0;
    let perimeter = 0;
    vertices.forEach(([x, y], index) => {
      const [nextX, nextY] = vertices[(index + 1) % vertices.length];
      twiceSignedArea += x * nextY - nextX * y;
      perimeter += Math.hypot(nextX - x, nextY - y);
    });
    return { area: Math.abs(twiceSignedArea) / 2, perimeter };
  }

  function geometryReferencePoint(shape, progress, cycles = 1, phase = 0) {
    return geometryPoint(geometryBaseShape(shape), progress, cycles, phase);
  }

  function geometryFourierEpicycles(shape, progress, cycles, phase, terms) {
    if (shape !== "fourier-triangle" && shape !== "fourier-square") return [];
    const coefficients = geometryFourierCoefficients(shape);
    const t = ((progress * cycles + phase) % 1 + 1) % 1;
    const order = [0];
    for (let harmonic = 1; harmonic <= terms; harmonic += 1) order.push(harmonic, -harmonic);
    let x = 0;
    let y = 0;
    return order.map((harmonic) => {
      const coefficient = coefficients.get(harmonic);
      const angle = TAU * harmonic * t;
      const cosine = Math.cos(angle);
      const sine = Math.sin(angle);
      const vectorX = coefficient.real * cosine - coefficient.imaginary * sine;
      const vectorY = coefficient.real * sine + coefficient.imaginary * cosine;
      const vector = { harmonic, fromX: x, fromY: y,
        toX: x + vectorX, toY: y + vectorY,
        radius: Math.hypot(coefficient.real, coefficient.imaginary) };
      x += vectorX;
      y += vectorY;
      return vector;
    });
  }

  function geometryFourierError(shape, terms) {
    if (shape !== "fourier-triangle" && shape !== "fourier-square") return null;
    let errorPower = 0;
    let referencePower = 0;
    const samples = 512;
    for (let sample = 0; sample < samples; sample += 1) {
      const t = sample / samples;
      const reference = geometryReferencePoint(shape, t);
      const approximation = geometryPoint(shape, t, 1, 0, terms);
      errorPower += (reference.x - approximation.x) ** 2 + (reference.y - approximation.y) ** 2;
      referencePower += reference.x ** 2 + reference.y ** 2;
    }
    return Math.sqrt(errorPower / Math.max(referencePower, 1e-12)) * 100;
  }

  function sectionForCycle(cycle) {
    return SECTION_NAMES[cycle];
  }

  function bassLfoAt(progress, config) {
    const phase = ((progress * 7 / config.bassRate) % 1 + 1) % 1;
    if (config.bassShape === "square") return phase < 0.5 ? 1 : -1;
    if (config.bassShape === "triangle") return 1 - 4 * Math.abs(phase - 0.5);
    return Math.sin(TAU * phase);
  }

  function bassPitchAt(progress, config) {
    const stepSeconds = 15 / config.bpm;
    const stepsPerBar = Math.round(Number(config.meter.split("/")[0]) * 16 /
      Number(config.meter.split("/")[1]));
    const stepNumber = Math.floor(progress * config.durationSeconds / stepSeconds + 1e-8);
    const bar = Math.floor(stepNumber / stepsPerBar);
    const stepInBar = ((stepNumber % stepsPerBar) + stepsPerBar) % stepsPerBar;
    if (bassRestAt(bar, stepInBar, stepsPerBar, config.bassPhraseMode)) return null;
    const step = config.bassSeededPattern[((stepNumber % stepsPerBar) + stepsPerBar) % stepsPerBar];
    if (!step) return null;
    const modulation = config.bassLfoEnabled ? Math.round(bassLfoAt(progress, config) * config.bassDepth) : 0;
    return Math.max(24, Math.min(60, step.midi + modulation));
  }

  function bassRestAt(bar, step, stepsPerBar, mode) {
    if (mode === "breath") return bar % 2 === 1 && step >= stepsPerBar - 2;
    if (mode === "cadence") return bar % 4 === 3 && step >= stepsPerBar - 4;
    if (mode === "call-response") return bar % 2 === 1;
    return false;
  }

  function bassDurationStepsFor(noteOrdinal, manualSteps, config) {
    const [numerator, denominator] = config.meter.split("/").map(Number);
    const stepsPerBar = Math.round(numerator * 16 / denominator);
    if (config.bassDurationFormula === "fibonacci") {
      return Math.min(stepsPerBar, [1, 1, 2, 3, 5, 8, 13, 16][noteOrdinal % 8]);
    }
    if (config.bassDurationFormula === "geometric") {
      return Math.min(stepsPerBar, 16, Math.round(Math.pow(config.bassGeoRatio, noteOrdinal % 8)));
    }
    return manualSteps;
  }

  function localEvents(values, recipe) {
    const found = [];
    const value = (sample) => values[(sample + SAMPLES) % SAMPLES];
    if (recipe === "center") {
      for (let sample = 0; sample < SAMPLES; sample += 1) {
        const a = value(sample);
        const b = value(sample + 1);
        const u = sample / SAMPLES;
        if (Math.abs(a) < 1e-8 && u < 1 - 1e-8) found.push(u);
        else if (a * b < 0) {
          const fraction = Math.abs(a) / (Math.abs(a) + Math.abs(b));
          const crossing = (sample + fraction) / SAMPLES;
          if (crossing < 1 - 1e-8) found.push(crossing);
        }
      }
    } else {
      for (let sample = 0; sample < SAMPLES; sample += 1) {
        const before = value(sample) - value(sample - 1);
        const after = value(sample + 1) - value(sample);
        if ((before > 0 && after <= 0) || (before < 0 && after >= 0)) {
          found.push(sample / SAMPLES);
        }
      }
    }
    return found.filter((u, index) => index === 0 || u - found[index - 1] > 1e-5);
  }

  function analyticEvents(cycle, voice, config) {
    if (config.geometrySource && voice === 0 && config.geometryShape === "billiard-square")
      return billiardCollisionPositions(config.voiceCycles[voice], 0,
        config.geometryBilliardPattern);
    const sampleDriven = config.motionModel === "coupled-pendulum" ||
      (config.geometrySource && voice === 0);
    if (sampleDriven) {
      const values = Array.from({ length: SAMPLES }, (_, sample) =>
        valueAt(cycle, voice, sample / SAMPLES, config));
      return localEvents(values, config.recipe);
    }
    const initialPhase = phaseFor(cycle, voice, config.phase, 0, config);
    const finalPhase = phaseFor(cycle, voice, config.phase, 1, config);
    const phase = ((initialPhase % 1) + 1) % 1;
    const cycles = config.voiceCycles[voice] + finalPhase - initialPhase;
    const thresholds = config.recipe === "center" ? [0, 0.5] : [0.25, 0.75];
    const positions = [];
    for (const threshold of thresholds) {
      const first = Math.ceil(phase - threshold - 1e-12);
      const end = cycles + phase - threshold;
      for (let index = first; index < end - 1e-12; index += 1) {
        const u = (index + threshold - phase) / cycles;
        if (u >= -1e-12 && u < 1 - 1e-12)
          positions.push(Math.max(0, u));
      }
    }
    return positions.sort((a, b) => a - b)
      .filter((u, index, all) => index === 0 || u - all[index - 1] > 1e-10);
  }

  function patternFieldPose(mode, progress, seed) {
    const boundedProgress = Number.isFinite(progress) ? progress % 4096 : 0;
    const phase = TAU * ((seed >>> 0) & 0xffff) / 65536;
    const rate = 0.25 + 0.125 * (((seed >>> 16) % 4));
    const theta = TAU * rate * boundedProgress + phase;
    const pose = { offsetX: 0, offsetY: 0, rotation: 0, halfWidth: 0.16,
      padEdgeX: 0, padEdgeXY: 0, padEdgeCos3: 0,
      padEdgeSin3: 0, laneBow: 0, laneWaveA: 0, laneWaveB: 0,
      padRadii: [0, 0, 0, 0], padShapePhases: [0, 0, 0, 0],
      padMorphologies: [0, 0, 0, 0],
      padCentreOffsets: Array.from({ length: 4 }, () => ({ x: 0, y: 0 })),
      laneCentreOffsets: [0, 0], laneHalfWidths: [0, 0],
      focusX: 0, focusY: 0, focusRadius: 0.36 };
    if (mode === "unrestricted") {
      pose.focusX = 0.62 * Math.sin(theta * 0.67 + phase * 0.23);
      pose.focusY = 0.62 * Math.cos(theta * 0.83 + phase * 0.31);
      pose.focusRadius = 0.30 + 0.08 * (0.5 + 0.5 * Math.sin(theta * 0.43));
      return pose;
    }
    pose.offsetX = 0.12 * Math.sin(theta);
    pose.offsetY = 0.12 * Math.sin(theta * 0.73 + phase * 0.37);
    pose.rotation = 0.16 * Math.sin(theta * 0.5 + phase * 0.19);
    pose.halfWidth = 0.13 + 0.035 * (0.5 + 0.5 * Math.sin(theta + phase));
    const goldenAngle = 2.39996322972865332;
    for (let organism = 0; organism < 4; organism += 1) {
      const organismPhase = phase + goldenAngle * organism;
      const neighbourPhase = phase + goldenAngle * ((organism + 1) % 4);
      pose.padRadii[organism] = 0.38 +
        0.027 * Math.sin(theta * 0.61 + organismPhase) +
        0.013 * Math.sin(theta * 0.19 + neighbourPhase);
      pose.padShapePhases[organism] =
        0.34 * Math.sin(theta * 0.17 + organismPhase * 0.11);
      pose.padMorphologies[organism] = ((seed >>> (organism * 4)) & 0xff) % 28;
      pose.padCentreOffsets[organism] = {
        x: 0.026 * Math.sin(theta * 0.31 + organismPhase) +
          0.010 * Math.sin(theta * 0.13 + neighbourPhase),
        y: 0.026 * Math.sin(theta * 0.23 + organismPhase + 1.1) +
          0.010 * Math.sin(theta * 0.11 + neighbourPhase + 1.1)
      };
    }
    pose.padEdgeX = 0.065 + 0.018 * Math.sin(theta * 0.37 + phase * 0.71);
    pose.padEdgeXY = 0.052 * Math.cos(theta * 0.29 + phase * 0.43);
    pose.padEdgeCos3 = 0.022 * Math.sin(theta * 0.23 + phase * 0.53);
    pose.padEdgeSin3 = 0.017 * Math.cos(theta * 0.17 - phase * 0.29);
    pose.laneBow = 0.075 * Math.sin(theta * 0.31 + phase * 0.83);
    pose.laneWaveA = 0.026 * Math.sin(theta * 0.27 + phase * 0.59);
    pose.laneWaveB = 0.015 * Math.cos(theta * 0.19 + phase * 0.41);
    pose.laneCentreOffsets = [
      0.024 * Math.sin(theta * 0.23 + phase * 0.41),
      0.024 * Math.sin(theta * 0.23 + phase * 0.41 + 2.17)];
    pose.laneHalfWidths = [
      pose.halfWidth + 0.008 * Math.sin(theta * 0.29 + phase),
      pose.halfWidth + 0.008 * Math.sin(theta * 0.29 + phase + 2.17)];
    return pose;
  }

  function patternFieldPadBoundary(pose, angle, organism = 0) {
    const index = Math.max(0, Math.min(3, organism));
    const localAngle = angle + pose.padShapePhases[index];
    const cosine = Math.cos(localAngle), sine = Math.sin(localAngle);
    const cosine2 = Math.cos(2 * localAngle);
    let profile;
    switch (pose.padMorphologies[index]) {
      case 1:
        profile = 0.055 * Math.cos(4 * localAngle) - 0.025 * cosine2;
        break;
      case 2:
        profile = 0.10 * (Math.exp(2 * (cosine - 1)) - 0.30);
        break;
      case 3:
        profile = 0.105 * (1 / (3 - 2 * cosine) - 0.447213595499958);
        break;
      case 4: {
        const x = 2 * cosine;
        const softplus = Math.max(x, 0) + Math.log1p(Math.exp(-Math.abs(x)));
        profile = 0.045 * (softplus - 0.75);
        break;
      }
      case 5: {
        const x = 1.6 * cosine;
        const gelu = 0.5 * x * (1 + Math.tanh(
          0.7978845608028654 * (x + 0.044715 * x * x * x)));
        profile = 0.055 * (gelu - 0.45);
        break;
      }
      case 6:
        profile = 0.04 * (Math.abs(Math.sin(3 * localAngle)) - 0.6366197723675814) +
          0.02 * Math.cos(8 * localAngle);
        break;
      case 7:
        profile = 0.035 * (Math.sin(5 * localAngle) + Math.sin(6 * localAngle));
        break;
      case 8:
        profile = 0.09 * Math.tan(0.55 * cosine);
        break;
      case 9:
        profile = 0.07 * Math.sin(2 * localAngle + 0.8 * Math.sin(5 * localAngle));
        break;
      case 10:
        profile = 0.08 * cosine2 * cosine2 * cosine2;
        break;
      case 11:
        profile = 0.065 * Math.log(2 + cosine) - 0.065 * Math.log(2);
        break;
      case 12:
        profile = 0.08 * (Math.abs(cosine2) - 0.6366197723675814);
        break;
      case 13:
        profile = 0.065 * (Math.cos(3 * localAngle) + 0.35 * cosine2);
        break;
      case 14:
        profile = 0.065 * Math.sin(2 * localAngle + 0.32 * sine);
        break;
      case 15:
        profile = 0.045 * cosine + 0.03 * cosine2;
        break;
      case 16:
        profile = 0.045 * (Math.sin(2 * localAngle) + Math.cos(3 * localAngle));
        break;
      case 17:
        profile = 0.075 * Math.asin(cosine) / Math.PI;
        break;
      case 18:
        profile = 0.075 * (Math.acos(cosine) / Math.PI - 0.5);
        break;
      case 19:
        profile = 0.06 * Math.sin(3 * localAngle) * Math.cos(2 * localAngle);
        break;
      case 20:
        profile = 0.055 * (cosine - 0.25 * cosine2);
        break;
      case 21:
        profile = 0.045 * (Math.abs(Math.sin(2 * localAngle)) - 0.6366197723675814) +
          0.02 * Math.sin(5 * localAngle);
        break;
      case 22:
        profile = 0.06 * (cosine * cosine - 0.5);
        break;
      case 23:
        profile = 0.07 * (Math.abs(cosine2) - 0.6366197723675814) +
          0.015 * Math.sin(6 * localAngle);
        break;
      case 24:
        profile = 0.075 * (1 / (1.6 + cosine) - 0.8);
        break;
      case 25:
        profile = 0.075 * Math.log10(2 + cosine) - 0.075 * Math.log10(2);
        break;
      case 26:
        profile = 0.07 * cosine * Math.exp(-0.65 * Math.abs(sine));
        break;
      case 27:
        profile = 0.045 * cosine2;
        break;
      case 0:
      default:
        profile = pose.padEdgeX * cosine2 +
          pose.padEdgeXY * Math.sin(2 * localAngle) +
          pose.padEdgeCos3 * Math.cos(3 * localAngle) +
          pose.padEdgeSin3 * Math.sin(3 * localAngle);
        break;
    }
    return 1 + profile;
  }

  function patternFieldPadRadius(pose, organism) {
    return pose.padRadii[Math.max(0, Math.min(3, organism))];
  }

  function patternFieldPadBridge(pose, bridge) {
    const centres = [[0, -0.58], [0, 0.58], [-0.58, 0], [0.58, 0]];
    const neighbours = [[0, 2], [2, 1], [1, 3], [3, 0]];
    const [first, second] = neighbours[Math.max(0, Math.min(3, bridge))];
    const start = { x: centres[first][0] + pose.padCentreOffsets[first].x,
      y: centres[first][1] + pose.padCentreOffsets[first].y };
    const end = { x: centres[second][0] + pose.padCentreOffsets[second].x,
      y: centres[second][1] + pose.padCentreOffsets[second].y };
    const dx = end.x - start.x, dy = end.y - start.y;
    const distance = Math.hypot(dx, dy);
    if (distance <= 1e-12) return { start, end, radius: 0 };
    const angle = Math.atan2(dy, dx);
    const firstExtent = patternFieldPadRadius(pose, first) *
      patternFieldPadBoundary(pose, angle, first);
    const secondExtent = patternFieldPadRadius(pose, second) *
      patternFieldPadBoundary(pose, angle + Math.PI, second);
    const gap = distance - firstExtent - secondExtent;
    return { start, end, radius: Math.max(0, Math.min(0.045, 0.018 - 0.5 * gap)) };
  }

  function patternFieldLaneDisplacement(pose, x) {
    const boundedX = Math.max(-1, Math.min(1, x));
    const envelope = 1 - boundedX * boundedX;
    return envelope * (pose.laneBow + pose.laneWaveA * Math.sin(Math.PI * boundedX) +
      pose.laneWaveB * Math.sin(TAU * boundedX));
  }

  function patternFieldLaneSlope(pose, x) {
    const boundedX = Math.max(-1, Math.min(1, x));
    const envelope = 1 - boundedX * boundedX;
    const wave = pose.laneBow + pose.laneWaveA * Math.sin(Math.PI * boundedX) +
      pose.laneWaveB * Math.sin(TAU * boundedX);
    const waveSlope = pose.laneWaveA * Math.PI * Math.cos(Math.PI * boundedX) +
      pose.laneWaveB * TAU * Math.cos(TAU * boundedX);
    return 0.55 - 2 * boundedX * wave + envelope * waveSlope;
  }

  function patternFieldBoundaryDistance(mode, x, y, progress, seed) {
    if (!Number.isFinite(x) || !Number.isFinite(y)) return Infinity;
    if (mode === "unrestricted") return -1;
    const pose = patternFieldPose(mode, progress, seed);
    const dx = x - pose.offsetX, dy = y - pose.offsetY;
    const cosine = Math.cos(pose.rotation), sine = Math.sin(pose.rotation);
    const localX = cosine * dx + sine * dy;
    const localY = -sine * dx + cosine * dy;
    if (mode === "adhesive-pads") {
      const centres = [[0, -0.58], [0, 0.58], [-0.58, 0], [0.58, 0]];
      let nearest = Infinity;
      for (let organism = 0; organism < centres.length; organism += 1) {
        const [cx, cy] = centres[organism];
        const px = localX - cx - pose.padCentreOffsets[organism].x;
        const py = localY - cy - pose.padCentreOffsets[organism].y;
        const radius = Math.hypot(px, py);
        nearest = Math.min(nearest, radius - patternFieldPadRadius(pose, organism) *
          patternFieldPadBoundary(pose, Math.atan2(py, px), organism));
      }
      for (let index = 0; index < 4; index += 1) {
        const bridge = patternFieldPadBridge(pose, index);
        if (bridge.radius <= 0) continue;
        const vx = bridge.end.x - bridge.start.x;
        const vy = bridge.end.y - bridge.start.y;
        const lengthSquared = vx * vx + vy * vy;
        const amount = lengthSquared > 1e-12 ? Math.max(0, Math.min(1,
          ((localX - bridge.start.x) * vx + (localY - bridge.start.y) * vy) /
            lengthSquared)) : 0;
        const dx = localX - (bridge.start.x + amount * vx);
        const dy = localY - (bridge.start.y + amount * vy);
        nearest = Math.min(nearest, Math.hypot(dx, dy) - bridge.radius);
      }
      return nearest;
    }
    const displacement = patternFieldLaneDisplacement(pose, localX);
    const upperCentre = 0.55 * localX + displacement + pose.laneCentreOffsets[0];
    const lowerCentre = -0.55 * localX - displacement + pose.laneCentreOffsets[1];
    const slope = patternFieldLaneSlope(pose, localX);
    const normalizer = Math.sqrt(1 + slope * slope);
    return Math.min(
      Math.abs(localY - upperCentre) / normalizer - pose.laneHalfWidths[0],
      Math.abs(localY - lowerCentre) / normalizer - pose.laneHalfWidths[1]);
  }

  function patternFieldAllows(mode, x, y, progress, seed) {
    return mode === "unrestricted" || patternFieldBoundaryDistance(
      mode, x, y, progress, seed) < PATTERN_EDGE_SOFTNESS;
  }

  function patternFieldWeight(mode, x, y, progress, seed) {
    if (mode === "unrestricted") {
      const pose = patternFieldPose(mode, progress, seed);
      const distanceSquared = (x - pose.focusX) ** 2 + (y - pose.focusY) ** 2;
      return 0.68 + 0.52 * Math.exp(-distanceSquared /
        (2 * pose.focusRadius * pose.focusRadius));
    }
    const distance = patternFieldBoundaryDistance(mode, x, y, progress, seed);
    const normalized = Math.max(0, Math.min(1,
      (distance + PATTERN_EDGE_SOFTNESS) / (2 * PATTERN_EDGE_SOFTNESS)));
    const smooth = normalized * normalized * (3 - 2 * normalized);
    return 1 - smooth;
  }

  function seededIntensityValue(config, x, y, activeCycles) {
    const x2 = x * x;
    const y2 = y * y;
    let value = 1;
    if (config.intensityLaw === "projection-y") value = 0.5 + 0.5 * y;
    else if (config.intensityLaw === "product-xy") value = 0.5 + 0.5 * x * y;
    else if (config.intensityLaw === "quadratic-difference")
      value = 0.5 + 0.5 * (x2 - y2);
    else if (config.intensityLaw === "gaussian-centre") value = Math.exp(-3 * (x2 + y2));
    else if (config.intensityLaw === "cosine-bands") value =
      0.5 + 0.5 * Math.cos(Math.PI * (x - y));
    else if (config.intensityLaw === "potential-energy") {
      const wx = activeCycles[0] ** 2;
      const wy = activeCycles[1] ** 2;
      value = (wx * x2 + wy * y2) / (wx + wy);
    } else if (config.intensityLaw === "kinetic-energy") {
      const wx = activeCycles[0] ** 2;
      const wy = activeCycles[1] ** 2;
      value = 1 - (wx * x2 + wy * y2) / (wx + wy);
    }
    return 1 - config.intensityAmount + config.intensityAmount *
      Math.max(0, Math.min(1, value));
  }

  // Event-level ecological relations mirror EcologicalRelations.cpp. This is
  // a control-thread composition transform; it must run before audio rendering.
  function applyEcologicalEventRelations(sourceEvents, sectionSeconds,
                                         voiceCount, rules) {
    const maximumEvents = 65536;
    const relationNames = ["none", "support", "imitation", "avoidance", "competition"];
    if (!Number.isFinite(sectionSeconds) || sectionSeconds <= 0 ||
        !Number.isInteger(voiceCount) || voiceCount < 1 || voiceCount > MAX_VOICES ||
        !Array.isArray(sourceEvents) || sourceEvents.length > maximumEvents ||
        !Array.isArray(rules) || rules.length < voiceCount) {
      throw new RangeError("Receita ou plano ecológico fora dos limites.");
    }
    const normalizedRules = rules.slice(0, voiceCount);
    for (let voice = 0; voice < voiceCount; voice += 1) {
      const rule = normalizedRules[voice];
      if (!rule || !relationNames.includes(rule.relation) ||
          !Number.isInteger(rule.sourceVoice) || rule.sourceVoice < 0 ||
          rule.sourceVoice > 255 ||
          (rule.relation !== "none" &&
            (rule.sourceVoice >= voiceCount || rule.sourceVoice === voice)) ||
          !Number.isFinite(rule.strength) || rule.strength < 0 || rule.strength > 1 ||
          !Number.isFinite(rule.memorySeconds) || rule.memorySeconds < 0 ||
          rule.memorySeconds > sectionSeconds ||
          !Number.isFinite(rule.responseDelaySeconds) || rule.responseDelaySeconds < 0 ||
          rule.responseDelaySeconds > sectionSeconds) {
        throw new RangeError("Regra ecológica fora dos limites.");
      }
    }
    let previousTime = -1;
    const events = sourceEvents.map((event) => {
      if (!event || !Number.isInteger(event.voice) || event.voice < 0 ||
          event.voice >= voiceCount || !Number.isFinite(event.timeSeconds) ||
          event.timeSeconds < 0 || event.timeSeconds >= sectionSeconds ||
          event.timeSeconds < previousTime || !Number.isFinite(event.intensity) ||
          event.intensity < 0 || event.intensity > 1 ||
          !Number.isFinite(event.fieldX) || !Number.isFinite(event.fieldY)) {
        throw new RangeError("Evento do plano ecológico fora dos limites.");
      }
      previousTime = event.timeSeconds;
      return { ...event };
    });
    const lastEvent = Array(voiceCount).fill(-1);
    const lastEventTime = Array(voiceCount).fill(-1);
    const trace = [];
    for (let index = 0; index < events.length; index += 1) {
      const event = events[index];
      const rule = normalizedRules[event.voice];
      const record = {
        eventIndex: index, perceivedEventIndex: 0, perceived: false,
        memoryAgeSeconds: 0, originalTimeSeconds: event.timeSeconds,
        resultingTimeSeconds: event.timeSeconds,
        originalIntensity: event.intensity, resultingIntensity: event.intensity,
        relation: rule.relation
      };
      if (rule.relation !== "none" && rule.strength > 0) {
        const sourceIndex = lastEvent[rule.sourceVoice];
        const sourceTime = lastEventTime[rule.sourceVoice];
        const age = event.timeSeconds - sourceTime;
        if (sourceIndex >= 0 && age > 0 && age <= rule.memorySeconds) {
          const recency = rule.memorySeconds > 0
            ? Math.max(0, Math.min(1, 1 - age / rule.memorySeconds)) : 0;
          // The C++ event model stores strengths/intensities as float. Keep
          // those rounding points so near-simultaneous transformed events sort
          // in the same order in both engines.
          const influence = Math.fround(Math.fround(rule.strength) * Math.fround(recency));
          record.perceived = true;
          record.perceivedEventIndex = sourceIndex;
          record.memoryAgeSeconds = age;
          if (rule.relation === "support") {
            event.intensity = Math.fround(event.intensity + Math.fround(
              Math.fround(1 - event.intensity) * influence));
          } else if (rule.relation === "imitation") {
            event.timeSeconds += (sourceTime + rule.responseDelaySeconds - event.timeSeconds) * influence;
            event.timeSeconds = Math.max(0, Math.min(sectionSeconds - Number.EPSILON,
              event.timeSeconds));
          } else if (rule.relation === "avoidance" && rule.memorySeconds > 0 &&
                     age < rule.memorySeconds) {
            const forward = sourceTime + rule.memorySeconds;
            const backward = Math.max(0, sourceTime - rule.memorySeconds);
            const target = forward < sectionSeconds ? forward : backward;
            event.timeSeconds += (target - event.timeSeconds) * influence;
            event.timeSeconds = Math.max(0, Math.min(sectionSeconds - Number.EPSILON,
              event.timeSeconds));
          } else if (rule.relation === "competition") {
            event.intensity = Math.fround(event.intensity * Math.fround(1 - influence));
          }
        }
      }
      event.intensity = Math.fround(Math.max(0, Math.min(1, event.intensity)));
      record.resultingTimeSeconds = event.timeSeconds;
      record.resultingIntensity = event.intensity;
      trace.push(record);
      // Same-time events do not react retroactively to one another.
      lastEvent[event.voice] = index;
      lastEventTime[event.voice] = record.originalTimeSeconds;
    }
    events.sort((a, b) => a.timeSeconds - b.timeSeconds || a.voice - b.voice);
    return { events, trace };
  }

  function effectiveCycles(cycle, config) {
    const cycles = [...config.voiceCycles];
    if (cycle === 2 || cycle === 3) cycles[1] += config.phase / 2;
    else if (cycle === 5 || cycle === 6) cycles[1] -= config.phase / 2;
    return cycles;
  }

  function generate(input) {
    const config = normalizeConfig(input);
    if (config.motionModel === "coupled-pendulum")
      config.pendulumTrajectory = integrateTriplePendulum(config);
    let events = [];
    const eventsByCycle = Array.from({ length: 7 }, () => []);
    const duration = config.durationSeconds;
    let bassNoteOrdinal = 0;
    const seededBassRandom = seededRandom(seedHash(config.seed) ^ 0x42545243);
    const seededBassBase = 31 + 12 * seededBassRandom();
    const seededBassX = 2 + 6 * seededBassRandom();
    const seededBassY = 4 + 8 * seededBassRandom();
    const phraseShiftRandom = seededRandom(seedHash(config.seed) ^ 0x53484946);
    const phraseShifts = [-7, -5, -4, -3, -2, 0, 2, 3, 4, 5, 7];
    const seededBassPhraseShift = phraseShifts[
      Math.min(phraseShifts.length - 1,
        Math.floor(phraseShiftRandom() * phraseShifts.length))];
    const patternSeed = Number.parseInt(config.seed, 16) >>> 0;
    for (let cycle = 0; cycle < 7; cycle += 1) {
      for (let voice = 0; voice < config.voiceCount; voice += 1) {
        // A forma constrói por camadas, suspende no ciclo 5 e retorna em camadas.
        if (voice >= activeVoiceCountInCycle(cycle, config.voiceCount)) continue;
        const positions = analyticEvents(cycle, voice, config);
        for (const u of positions) {
          const phaseX = TAU * (config.voiceCycles[0] * u +
            phaseFor(cycle, 0, config.phase, u, config));
          const phaseY = TAU * (config.voiceCycles[1] * u +
            phaseFor(cycle, 1, config.phase, u, config));
          const geometricPoint = config.geometrySource && voice === 0
            ? geometryPoint(config.geometryShape, u, config.voiceCycles[voice],
              phaseFor(cycle, voice, config.phase, u, config), config.geometryFourierTerms,
              config.geometryBilliardPattern)
            : null;
          const fieldX = geometricPoint ? geometricPoint.x : Math.sin(phaseX);
          const fieldY = geometricPoint ? geometricPoint.y : Math.sin(phaseY);
          const fieldWeight = patternFieldWeight(config.patternMode, fieldX,
            fieldY, cycle + u, patternSeed);
          if (fieldWeight <= 0) continue;
          const activeCycles = effectiveCycles(cycle, config);
          const globalProgress = (cycle + u) / 7;
          const performedU = swingPosition(u, config.swing, config.cycleSeconds, config.bpm);
          const time = (cycle + performedU) * config.cycleSeconds;
          const sinceSection = time - SECTION_START[cycle] * config.cycleSeconds;
          const decay = Math.exp(-0.065 * sinceSection);
          const base = voice === 0 ? 0.76 : 0.63 / Math.sqrt(config.voiceCount - 1);
          const pendulumState = config.motionModel === "coupled-pendulum" && voice < 3
            ? pendulumStateAt(cycle, voice, u, config) : null;
          const intensity = base * decay * seededIntensityValue(config, fieldX, fieldY, activeCycles) *
            expressionAt(globalProgress, config) *
            grooveAccent(cycle, performedU, config.groove, config.bpm, config.meter, config.cycleSeconds) *
            (pendulumState ? 0.35 + 0.65 * pendulumState.energy : 1) * fieldWeight;
          const bassSourceIntensity = base * decay
            * seededIntensityValue(config, fieldX, fieldY, activeCycles)
            * expressionAt(globalProgress, config)
            * grooveAccent(cycle, performedU, config.groove, config.bpm,
              config.meter, config.cycleSeconds);
          events.push({
            time, cycle, voice, localProgress: u, performedProgress: performedU,
            position: valueAt(cycle, voice, u, config),
            fieldX, fieldY,
            bassSourceX: valueAt(cycle, 0, u, config),
            bassSourceY: valueAt(cycle, Math.min(1, config.voiceCount - 1), u, config),
            bassSourceIntensity,
            intensity,
            physicalEnergy: pendulumState ? pendulumState.energy : null,
            recipe: config.recipe,
            section: sectionForCycle(cycle)
          });
          eventsByCycle[cycle].push(events[events.length - 1]);
        }
      }
      if (config.voiceEventRelations.some((relation) => relation !== "none")) {
        const cycleVoiceEvents = eventsByCycle[cycle];
        const ordered = cycleVoiceEvents.slice().sort((a, b) =>
          a.time - b.time || a.voice - b.voice);
        const source = ordered.map((event) => ({
          timeSeconds: event.time - cycle * config.cycleSeconds,
          voice: event.voice,
          // The event planner uses normalized intensity; performance layers
          // (groove and intention) can exceed 1 and are preserved separately.
          intensity: Math.max(0, Math.min(1, event.intensity)),
          fieldX: event.fieldX,
          fieldY: event.fieldY
        }));
        const rules = config.voiceEventRelations.map((relation, voice) => ({
          relation,
          sourceVoice: Math.max(0, voice - 1),
          strength: config.couplings[voice],
          memorySeconds: config.cycleSeconds * 0.5,
          responseDelaySeconds: config.cycleSeconds / 16
        }));
        const transformed = applyEcologicalEventRelations(source,
          config.cycleSeconds, config.voiceCount, rules);
        for (let index = 0; index < ordered.length; index += 1) {
          const event = ordered[index];
          const trace = transformed.trace[index];
          const localTime = trace.resultingTimeSeconds;
          event.time = cycle * config.cycleSeconds + localTime;
          event.performedProgress = localTime / config.cycleSeconds;
          event.localProgress = event.performedProgress;
          if (config.voiceEventRelations[event.voice] !== "none") {
            const normalized = Math.max(1e-9, trace.originalIntensity);
            const relationGain = trace.resultingIntensity / normalized;
            event.intensity *= relationGain;
            event.bassSourceIntensity *= relationGain;
          }
        }
      }
    }

    // Bass path analysis depends on all seven source cycles, so calculate it
    // after the voice agenda is complete and before emitting bass events.
    const bassPath = analyseSeedBassPath(eventsByCycle, seedHash(config.seed));
    for (let cycle = 0; cycle < 7; cycle += 1) {
      const stepSeconds = 15 / config.bpm;
      const [numerator, denominator] = config.meter.split("/").map(Number);
      const stepsPerBar = Math.round(numerator * 16 / denominator);
      const cycleStart = cycle * config.cycleSeconds;
      const cycleEnd = (cycle + 1) * config.cycleSeconds;
      const cycleBassRandom = config.bassRhythmRelation === "convolution"
        ? seededRandom((seedHash(config.seed) ^ 0x42545243
          ^ Math.imul(cycle, 0x9e3779b9)) >>> 0) : seededBassRandom;
      if (config.bassVoiceDriven) {
        const slots = Array(stepsPerBar * 4).fill(null);
        for (const voiceEvent of eventsByCycle[cycle]) {
          if (voiceEvent.voice >= config.voiceCount) continue;
          // Use cycle-local progress directly; subtracting two absolute times
          // loses precision at later formal cycles and can move boundary hits.
          const localTime = Math.max(0,
            voiceEvent.performedProgress * config.cycleSeconds);
          const slot = Math.min(slots.length - 1,
            Math.floor(localTime / stepSeconds + 1e-9));
          const bassSourceIntensity = voiceEvent.bassSourceIntensity
            ?? voiceEvent.intensity;
          const sourceIntensity = Math.fround(bassSourceIntensity);
          const priority = (voiceEvent.voice === 0 ? 2 : 1)
            * (0.5 + Math.max(0, Math.min(1, sourceIntensity)));
          if (slots[slot] && priority <= slots[slot].priority) continue;
          const x = valueAt(cycle, 0, voiceEvent.localProgress, config);
          const y = valueAt(cycle, Math.min(1, config.voiceCount - 1),
            voiceEvent.localProgress, config);
          slots[slot] = { voiceEvent, localTime, priority, x, y,
            intensity: sourceIntensity, countertime: false };
        }

        // Keep the percussion-derived source slots immutable while adding
        // bass counterbeats. Mutating this alias made each new offbeat a
        // candidate for the next one, unlike the native value-copy schedule.
        let scheduleSlots = slots.map((slot) => slot ? { ...slot } : null);
        if (config.bassRhythmRelation === "convolution") {
          // Produto em R[x]/(x^N−1): uma resposta de três taps circula pelo
          // compasso. A seed escolhe os dois deslocamentos; pesos acumulam.
          const length = slots.length;
          let hash = (seedHash(config.seed) ^ Math.imul(cycle + 1, 0x9e3779b9)) >>> 0;
          hash ^= hash >>> 16;
          hash = Math.imul(hash, 0x7feb352d) >>> 0;
          hash ^= hash >>> 15;
          hash = Math.imul(hash, 0x846ca68b) >>> 0;
          hash = (hash ^ (hash >>> 16)) >>> 0;
          const firstOffset = 1 + (hash % (length - 1));
          let secondOffset = 1 + ((hash >>> 16) % (length - 1));
          if (secondOffset === firstOffset) secondOffset = secondOffset % (length - 1) + 1;
          const taps = [[0, 0.5], [firstOffset, 0.32], [secondOffset, 0.18]];
          const weights = Array(length).fill(0);
          const sources = Array(length).fill(null);
          for (let source = 0; source < length; source += 1) {
            if (!slots[source]) continue;
            for (const [offset, weight] of taps) {
              const target = (source + offset) % length;
              const contribution = weight * Math.max(0, Math.min(1,
                slots[source].voiceEvent.bassSourceIntensity
                  ?? slots[source].voiceEvent.intensity));
              weights[target] += contribution;
              if (!sources[target] || contribution > sources[target].contribution) {
                sources[target] = { ...slots[source], contribution };
              }
            }
          }
          const peak = Math.max(...weights);
          scheduleSlots = weights.map((weight, slot) => weight > 0 && sources[slot]
            ? { ...sources[slot], localTime: slot * stepSeconds,
              convolutionGain: peak > 0 ? weight / peak : 0 } : null);
        }

        // The JUCE scheduler adds seed-selected counterbeats on free offbeats.
        // Keep the source voices untouched and derive each new event from the
        // nearest existing attack, with one deterministic fallback per cycle.
        if (config.bassVariationLevel >= 1) {
          const pulseSteps = Math.max(1, Math.floor(16 / denominator));
          const offbeatInPulse = Math.max(1, Math.floor(pulseSteps / 2));
          const probability = config.bassVariationLevel === 1 ? 0.16
            : config.bassVariationLevel === 2 ? 0.34 : 0.62;
          let fallbackSource = null;
          let fallbackTarget = slots.length;
          let fallbackScore = 0xffffffff;
          let addedCountertime = false;
          const counterbeatsPerBar = Array(4).fill(0);
          const maxCounterbeatsPerBar = config.bassVariationLevel === 1 ? 1
            : config.bassVariationLevel === 2 ? 2 : 3;
          for (let target = 0; target < slots.length; target += 1) {
            if (scheduleSlots[target]) continue;
            const stepInBar = target % stepsPerBar;
            const pulsePosition = stepInBar % pulseSteps;
            const eighthOffbeat = pulsePosition === offbeatInPulse;
            const sixteenthAnticipation = config.bassVariationLevel >= 2
              && pulseSteps >= 4 && pulsePosition === pulseSteps - 1;
            if ((!eighthOffbeat && !sixteenthAnticipation)
                || counterbeatsPerBar[Math.floor(target / stepsPerBar)]
                  >= maxCounterbeatsPerBar) continue;

            let decision = (seedHash(config.seed)
              ^ Math.imul(0x9e3779b9, cycle + 1)
              ^ Math.imul(0x85ebca6b, target + 1)) >>> 0;
            decision ^= decision >>> 16;
            decision = Math.imul(decision, 0x7feb352d) >>> 0;
            decision = (decision ^ (decision >>> 15)) >>> 0;

            let source = null;
            const searchRadius = Math.max(2, pulseSteps);
            for (let distance = 1; distance <= searchRadius; distance += 1) {
              for (const direction of [-1, 1]) {
                const sourceIndex = target + direction * distance;
                if (sourceIndex < 0 || sourceIndex >= slots.length) continue;
                const candidate = slots[sourceIndex];
                if (!candidate) continue;
                const sourcePriority = candidate.priority / distance;
                if (!source || sourcePriority > source.priority)
                  source = { ...candidate, priority: sourcePriority };
              }
            }
            if (!source) continue;

            source.localTime = target * stepSeconds;
            source.intensity = Math.fround(source.intensity
              * (config.bassVariationLevel === 1 ? 0.42
                : config.bassVariationLevel === 2 ? 0.62 : 0.78));
            if (sixteenthAnticipation)
              source.intensity = Math.fround(source.intensity * 0.82);
            source.countertime = true;
            if ((decision & 0xffff) / 65535 < probability) {
              scheduleSlots[target] = source;
              counterbeatsPerBar[Math.floor(target / stepsPerBar)] += 1;
              addedCountertime = true;
            } else if (decision < fallbackScore) {
              fallbackScore = decision;
              fallbackTarget = target;
              fallbackSource = source;
            }
          }
          if (!addedCountertime && fallbackTarget < slots.length) {
            scheduleSlots[fallbackTarget] = fallbackSource;
            counterbeatsPerBar[Math.floor(fallbackTarget / stepsPerBar)] += 1;
          }
        }

        for (let slot = 0; slot < scheduleSlots.length; slot += 1) {
          const selected = scheduleSlots[slot];
          if (!selected) continue;
          const absoluteTime = cycleStart + selected.localTime;
          const stepNumber = Math.floor(absoluteTime / stepSeconds + 1e-9);
          const stepInBar = ((stepNumber % stepsPerBar) + stepsPerBar) % stepsPerBar;
          const bar = Math.floor(stepNumber / stepsPerBar);
          if (bassRestAt(bar, stepInBar, stepsPerBar, config.bassPhraseMode)) continue;
          const memory = Math.abs(bassPath.integrals[cycle]) / 1.5;
          const motion = Math.min(1, Math.abs(bassPath.derivatives[cycle])
            + 0.35 * Math.abs(bassPath.curvatures[cycle]));
          const breathAmount = Math.max(0, Math.min(0.30,
            config.bassPathBreath * (config.bassVariationLevel / 2)
              * (0.025 + 0.13 * memory - 0.06 * motion
                + (cycle === 3 ? 0.035 : 0))));
          const rhythmicAnchor = !selected.countertime
            && selected.voiceEvent.voice === 0 && stepInBar % 4 === 0;
          const patternCell = config.bassPattern[stepInBar];
          const metadata = patternCell || { durationSteps: 2, repeats: 1, intent: "normal" };
          const levelPitchVariation = config.bassVariationLevel === 0 ? 0
            : config.bassVariationLevel === 1
              ? Math.min(config.bassPitchVariation, 0.5)
            : config.bassVariationLevel === 2 ? config.bassPitchVariation
              : Math.max(config.bassPitchVariation, 0.55);
          const pitchRadius = Math.round(6 * levelPitchVariation);
          let pitch = Math.max(24, Math.min(60, Math.round(
            seededBassBase + seededBassX * selected.x + seededBassY * selected.y
              + bassPath.offsets[cycle]) + seededBassPhraseShift));
          if (pitchRadius > 0 && cycleBassRandom() < levelPitchVariation) {
            pitch = Math.max(24, Math.min(60, pitch
              + Math.floor(cycleBassRandom() * (2 * pitchRadius + 1)) - pitchRadius));
          }
          const globalProgress = absoluteTime / duration;
          const formulaDuration = bassDurationStepsFor(
            bassNoteOrdinal++, metadata.durationSteps || 2, config);
          let durationSteps = Math.min(formulaDuration, stepsPerBar - stepInBar);
          if (config.bassDurationFormula !== "manual") {
            const kinematicScale = 1.15 - 0.30 * bassPath.speeds[cycle];
            durationSteps = Math.max(1, Math.min(stepsPerBar - stepInBar,
              Math.floor(durationSteps * kinematicScale + 0.5)));
          }
          let bassTime = config.bassRhythmRelation === "independent"
            || config.bassRhythmRelation === "convolution"
            ? cycleStart + slot * stepSeconds : absoluteTime;
          if (config.bassRhythmRelation === "answer" && !selected.countertime) {
            let nearestDelta = 0;
            let nearestDistance = Infinity;
            for (const other of eventsByCycle[cycle]) {
              if (other.voice === BASS_VOICE) continue;
              const delta = ((other.time - bassTime + config.cycleSeconds * 1.5)
                % config.cycleSeconds) - config.cycleSeconds / 2;
              if (Math.abs(delta) < nearestDistance) {
                nearestDistance = Math.abs(delta);
                nearestDelta = delta;
              }
            }
            if (nearestDistance <= stepSeconds * 1.5) {
              bassTime = cycleStart + Math.max(0, Math.min(config.cycleSeconds - 1e-6,
                selected.localTime + Math.max(-stepSeconds,
                  Math.min(stepSeconds, nearestDelta + stepSeconds * 0.5))));
            }
          }
          if (config.bassVariationLevel > 0 && !rhythmicAnchor
              && !selected.countertime) {
            let timingHash = (seedHash(config.seed)
              ^ Math.imul(0x27d4eb2d, cycle + 1)
              ^ Math.imul(0x165667b1, slot + 1)
              ^ Math.imul(0xd3a2646c,
                selected.voiceEvent.voice + 1)) >>> 0;
            timingHash ^= timingHash >>> 15;
            timingHash = Math.imul(timingHash, 0x2c1b3c6d) >>> 0;
            timingHash ^= timingHash >>> 12;
            const draw = (timingHash & 0xffff) / 65535;
            const amount = config.bassVariationLevel === 1 ? 0.25
              : config.bassVariationLevel === 2 ? 0.70 : 1.0;
            const signedOffset = (draw * 2 - 1) * amount;
            const localBassTime = Math.max(0, Math.min(config.cycleSeconds - 1e-6,
              bassTime - cycleStart + signedOffset * stepSeconds));
            bassTime = cycleStart + localBassTime;
          }
          const relatedU = (bassTime - cycleStart) / config.cycleSeconds;
          const lfo = bassLfoAt(globalProgress, config);
          const pulseAnchor = rhythmicAnchor;
          const sourceVelocity = Math.fround(Math.max(pulseAnchor ? 0.72
            : selected.countertime ? 0.32 : 0.46,
            Math.min(selected.countertime ? 0.65 : 1.0,
              0.18 + 0.72 * selected.intensity)));
          const expressiveGain = Math.fround(expressionAt(globalProgress, config));
          const grooveGain = Math.fround(grooveAccent(cycle, relatedU, config.groove,
            config.bpm, config.meter, config.cycleSeconds));
          const curvatureGain = Math.fround(1 + 0.08
            * Math.min(1, Math.abs(bassPath.curvatures[cycle])));
          const convolutionGain = selected.convolutionGain === undefined
            ? 1 : selected.convolutionGain;
          const velocity = Math.fround(Math.fround(Math.fround(sourceVelocity
            * expressiveGain) * grooveGain) * Math.fround(curvatureGain
              * (1 + 0.06 * Math.abs(bassPath.signedCurvatures[cycle] || 0)))
            * convolutionGain);
          events.push({ time: bassTime, cycle, voice: BASS_VOICE,
            localProgress: relatedU, performedProgress: relatedU,
            position: (pitch - 36) / 12, pitch: Math.max(24, Math.min(60,
              pitch + (config.bassLfoEnabled ? Math.round(lfo * config.bassDepth) : 0))),
            seededPitch: pitch, seedVoicePitch: true, lfo,
            sourceX: selected.x, sourceY: selected.y,
            durationSeconds: durationSteps * stepSeconds * (1 - breathAmount), durationSteps,
            repeats: metadata.repeats || 1, intent: metadata.intent || "normal",
            stepNumber, sourceVoice: selected.voiceEvent.voice,
            intensity: velocity,
            recipe: "seed-voice-bass", section: sectionForCycle(cycle) });
        }
        continue;
      }
      const firstStep = Math.ceil(cycleStart / stepSeconds - 1e-8);
      const lastStep = Math.ceil(cycleEnd / stepSeconds - 1e-8);
      for (let stepNumber = firstStep; stepNumber < lastStep; stepNumber += 1) {
        const step = config.bassSeededPattern[((stepNumber % stepsPerBar) + stepsPerBar) % stepsPerBar];
        const stepInBar = ((stepNumber % stepsPerBar) + stepsPerBar) % stepsPerBar;
        const bar = Math.floor(stepNumber / stepsPerBar);
        if (!step || bassRestAt(bar, stepInBar, stepsPerBar, config.bassPhraseMode)) continue;
        const absoluteTime = stepNumber * stepSeconds;
        const u = (absoluteTime - cycleStart) / config.cycleSeconds;
        if (u < 0 || u >= 1) continue;
        const performedU = swingPosition(u, config.swing, config.cycleSeconds, config.bpm);
        const globalProgress = (cycle + u) / 7;
        const lfo = bassLfoAt(globalProgress, config);
        const pitch = bassPitchAt(globalProgress, config);
        let bassTime = (cycle + performedU) * config.cycleSeconds;
        if (config.bassRhythmRelation !== "independent") {
          const localBassTime = bassTime - cycleStart;
          let nearestDelta = 0;
          let nearestDistance = Infinity;
          for (const voiceEvent of eventsByCycle[cycle]) {
            if (voiceEvent.voice === BASS_VOICE) continue;
            const delta = ((voiceEvent.time - bassTime + config.cycleSeconds * 1.5) %
              config.cycleSeconds) - config.cycleSeconds / 2;
            if (Math.abs(delta) < nearestDistance) {
              nearestDistance = Math.abs(delta);
              nearestDelta = delta;
            }
          }
          if (nearestDistance <= stepSeconds * 1.5) {
            const adjustment = config.bassRhythmRelation === "follow"
              ? nearestDelta * 0.5
              : Math.max(-stepSeconds, Math.min(stepSeconds,
                nearestDelta + stepSeconds * 0.5));
            bassTime = cycleStart + Math.max(0, Math.min(config.cycleSeconds - 1e-6,
              localBassTime + adjustment));
          }
        }
        const relatedU = (bassTime - cycleStart) / config.cycleSeconds;
        const formulaDuration = bassDurationStepsFor(bassNoteOrdinal++, step.durationSteps, config);
        const durationSteps = config.bassDurationFormula === "manual" ? formulaDuration :
          Math.min(formulaDuration, stepsPerBar - stepInBar);
        const event = { time: bassTime, cycle,
          voice: BASS_VOICE, localProgress: relatedU, performedProgress: relatedU,
          position: (pitch - 36) / 12, pitch, lfo,
          durationSeconds: durationSteps * stepSeconds, durationSteps,
          repeats: step.repeats || 1, intent: step.intent || "normal", stepNumber,
          intensity: 0.62 * expressionAt(globalProgress, config) *
            grooveAccent(cycle, relatedU, config.groove, config.bpm, config.meter, config.cycleSeconds),
          recipe: "lfo-bass", section: sectionForCycle(cycle) };
        events.push(event);
        eventsByCycle[cycle].push(event);
      }
    }

    events.sort((a, b) => a.time - b.time || a.voice - b.voice);
    const bassEvents = events.filter((event) => event.voice === BASS_VOICE);
    const [bassMeterNumerator, bassMeterDenominator] = config.meter.split("/").map(Number);
    const bassStepsPerBar = Math.round(bassMeterNumerator * 16 / bassMeterDenominator);
    const expandedBassEvents = [];
    for (let index = 0; index < bassEvents.length; index += 1) {
      const event = bassEvents[index];
      const next = bassEvents[index + 1];
      let gate = Math.max(0.025, Math.min(event.durationSeconds,
        next ? next.time - event.time : duration - event.time));
      const cycleEnd = (event.cycle + 1) * config.cycleSeconds;
      gate = Math.max(0, Math.min(gate, cycleEnd - event.time));
      const stepSeconds = 15 / config.bpm;
      const lastGateStep = Math.ceil((event.time + gate) / stepSeconds);
      for (let stepNumber = event.stepNumber + 1; stepNumber <= lastGateStep; stepNumber += 1) {
        const stepInBar = ((stepNumber % bassStepsPerBar) + bassStepsPerBar) % bassStepsPerBar;
        const bar = Math.floor(stepNumber / bassStepsPerBar);
        if (bassRestAt(bar, stepInBar, bassStepsPerBar, config.bassPhraseMode)) {
          gate = Math.max(0.025, Math.min(gate, stepNumber * stepSeconds - event.time));
          break;
        }
      }
      // Rest limits may impose their 25 ms floor, but a gate cannot cross
      // the formal cycle boundary used by the native renderer.
      gate = Math.max(0, Math.min(gate, cycleEnd - event.time));
      for (let repetition = 0; repetition < event.repeats; repetition += 1) {
        const time = event.time + gate * repetition / event.repeats;
        const cycle = Math.min(6, Math.floor(time / config.cycleSeconds + 1e-9));
        const localProgress = Math.max(0,
          (time - cycle * config.cycleSeconds) / config.cycleSeconds);
        const globalProgress = time / duration;
        const pitch = event.seedVoicePitch
          ? Math.max(24, Math.min(60, event.seededPitch
            + (config.bassLfoEnabled ? Math.round(bassLfoAt(globalProgress, config)
              * config.bassDepth) : 0)))
          : bassPitchAt(globalProgress, config);
        if (pitch === null) continue;
        expandedBassEvents.push({ ...event, time, cycle, localProgress,
          performedProgress: localProgress, pitch, position: (pitch - 36) / 12,
          durationSeconds: gate / event.repeats,
          intensity: Math.max(0, Math.min(1, event.intensity
            * (event.intent === "ghost" ? 0.42 : event.intent === "accent" ? 1.35 : 1)
            * (repetition ? Math.max(0.55, 0.92 - repetition * 0.08) : 1))) });
      }
    }
    events = events.filter((event) => event.voice !== BASS_VOICE).concat(expandedBassEvents);
    events.sort((a, b) => a.time - b.time || a.voice - b.voice);
    // Coincidências entre vozes ganham um acento comum.
    for (const event of events) {
      if (event.voice === BASS_VOICE) continue;
      let nearest = Infinity;
      for (const other of eventsByCycle[event.cycle]) {
        if (other.voice === event.voice || other.voice === BASS_VOICE) continue;
        nearest = Math.min(nearest, Math.abs(other.time - event.time));
      }
      const proximity = Math.max(0, 1 - nearest / 0.22);
      event.intensity = Math.min(1, event.intensity * intentionFactor(event, config, proximity));
    }

    if (events.length > 16384 || events.some((event) =>
      !Number.isFinite(event.time) || !Number.isFinite(event.intensity) ||
      !Number.isFinite(event.position) || event.time < 0 || event.time >= duration)) {
      throw new Error("O gerador produziu eventos fora dos limites.");
    }
    return { events, duration, config };
  }

  const api = { generate, valueAt, phaseFor, expressionAt, pendulumStateAt,
    applyEcologicalEventRelations,
    polyrhythmPreset, applyPolyrhythmPreset, effectiveCycles, analyticEvents,
    billiardCollisionPositions,
    seededRecipeSelection, algorithmicNextSeed,
    bassLfoAt, bassPitchAt,
    bassDurationStepsFor, bassOscillatorProfileForSeed, swingPosition, swingProgress,
    eventCursorAtOrAfter,
    geometryPoint, geometryReferencePoint, geometryPolygonMetrics,
    patternFieldPose, patternFieldBoundaryDistance, patternFieldAllows,
    patternFieldWeight, patternFieldPadBoundary, patternFieldPadRadius,
    patternFieldPadBridge,
    patternFieldLaneDisplacement,
    patternFieldLaneSlope, patternEdgeSoftness: PATTERN_EDGE_SOFTNESS, seedHash,
    geometryFourierEpicycles, geometryFourierError,
    sectionForCycle, activeVoiceCountInCycle, formalPhaseSegment, samplesPerCycle: SAMPLES,
    bassVoice: BASS_VOICE };
  if (typeof module !== "undefined" && module.exports) module.exports = api;
  root.BatrucScore = api;
})(typeof globalThis !== "undefined" ? globalThis : this);
