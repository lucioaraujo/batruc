/* Maquete local de interface e audição do batruc.
 * Direção artística: Lúcio de Araújo. Implementação assistida por Codex.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */
(function () {
  "use strict";
  const scoreModel = globalThis.BatrucScore;
  const graph = document.getElementById("graph");
  const ctx = graph.getContext("2d");
  const geometryShapeInput = document.getElementById("geometry-shape");
  const geometryBilliardPatternInput = document.getElementById("geometry-billiard-pattern");
  const geometrySourceInput = document.getElementById("geometry-source");
  const geometryFourierTermsInput = document.getElementById("geometry-fourier-terms");
  const geometryFourierControl = document.getElementById("geometry-fourier-control");
  const geometryFourierValue = document.getElementById("geometry-fourier-value");
  const geometryCanvas = document.getElementById("geometry-trace");
  const geometryCtx = geometryCanvas.getContext("2d");
  const geometryPanel = document.querySelector(".geometry-panel");
  const geometryEquation = document.getElementById("geometry-equation");
  const recipeInput = document.getElementById("recipe");
  const patternModeInput = document.getElementById("pattern-mode");
  const intensityLawInput = document.getElementById("intensity-law");
  const intensityAmountInput = document.getElementById("intensity-amount");
  const motionModelInput = document.getElementById("motion-model");
  const pendulumControls = document.getElementById("pendulum-controls");
  const pendulumLengthInput = document.getElementById("pendulum-length");
  const pendulumCouplingInput = document.getElementById("pendulum-coupling");
  const pendulumDampingInput = document.getElementById("pendulum-damping");
  const voiceCountInput = document.getElementById("voice-count");
  const polyrhythmPresetInput = document.getElementById("polyrhythm-preset");
  const phaseInput = document.getElementById("phase");
  const phaseValue = document.getElementById("phase-value");
  const intentionInput = document.getElementById("intention");
  const dynamicsInput = document.getElementById("dynamics");
  const swingInput = document.getElementById("swing");
  const swingValue = document.getElementById("swing-value");
  const articulationInput = document.getElementById("articulation");
  const durationInput = document.getElementById("duration");
  const bpmInput = document.getElementById("bpm");
  const bpmValue = document.getElementById("bpm-value");
  const meterInput = document.getElementById("meter");
  const timbreInput = document.getElementById("timbre");
  const grooveInput = document.getElementById("groove");
  const bassShapeInput = document.getElementById("bass-shape");
  const bassTimbreInput = document.getElementById("bass-timbre");
  const bassOscillatorProfileInput = document.getElementById("bass-oscillator-profile");
  const bassRateInput = document.getElementById("bass-rate");
  const bassDepthInput = document.getElementById("bass-depth");
  const bassPitchVariationInput = document.getElementById("bass-pitch-variation");
  const bassLfoEnabledInput = document.getElementById("bass-lfo-enabled");
  const bassPhraseModeInput = document.getElementById("bass-phrase-mode");
  const bassRhythmRelationInput = document.getElementById("bass-rhythm-relation");
  const bassVcfCutoffInput = document.getElementById("bass-vcf-cutoff");
  const bassVcfResonanceInput = document.getElementById("bass-vcf-resonance");
  const bassEnvelopeInputs = {
    bassOutputLevel: document.getElementById("bass-output-level"),
    bassAmpAttackSeconds: document.getElementById("bass-amp-attack"),
    bassAmpDecaySeconds: document.getElementById("bass-amp-decay"),
    bassAmpSustain: document.getElementById("bass-amp-sustain"),
    bassAmpReleaseSeconds: document.getElementById("bass-amp-release"),
    bassFilterEnvelopeAmountHz: document.getElementById("bass-filter-amount"),
    bassFilterAttackSeconds: document.getElementById("bass-filter-attack"),
    bassFilterDecaySeconds: document.getElementById("bass-filter-decay"),
    bassFilterSustain: document.getElementById("bass-filter-sustain"),
    bassFilterReleaseSeconds: document.getElementById("bass-filter-release"),
    bassGateRatio: document.getElementById("bass-gate")
  };
  const bassDurationFormulaInput = document.getElementById("bass-duration-formula");
  const bassGeoRatioInput = document.getElementById("bass-geo-ratio");
  const bassEditor = document.getElementById("bass-editor");
  const bassCtx = bassEditor.getContext("2d");
  const bassDrawToggle = document.getElementById("bass-draw-toggle");
  const bassPatternGrid = document.getElementById("bass-pattern");
  const BASS_VOICE = scoreModel.bassVoice;
  const outputLevelInput = document.getElementById("output-level");
  const outputValue = document.getElementById("output-value");
  const formulaBusTrimInput = document.getElementById("formula-bus-trim");
  const formulaBusTrimValue = document.getElementById("formula-bus-trim-value");
  const mixerBalanceReadout = document.getElementById("mixer-balance-readout");
  const mixerAutoBalanceButton = document.getElementById("mixer-auto-balance");
  const relationSummary = document.getElementById("relation-summary");
  const status = document.getElementById("status");
  const timeline = document.getElementById("timeline");
  const formulaGrid = document.getElementById("formula-grid");
  const channelMixer = document.getElementById("channel-mixer");
  const graphLegend = document.getElementById("graph-legend");
  const graphZoomInput = document.getElementById("graph-zoom");
  const graphYScaleInput = document.getElementById("graph-y-scale");
  const graphYScaleValue = document.getElementById("graph-y-value");
  const graphFocusInput = document.getElementById("graph-focus");
  const graphZoomValue = document.getElementById("graph-zoom-value");
  const graphFocusValue = document.getElementById("graph-focus-value");
  const playButton = document.getElementById("play");
  const seedInput = document.getElementById("seed-value");
  const seedNewButton = document.getElementById("seed-new");
  const archiveNameInput = document.getElementById("archive-name");
  const archiveList = document.getElementById("archive-list");
  const archiveStatus = document.getElementById("archive-status");
  const archiveFolderButton = document.getElementById("archive-folder");
  const variationInput = document.getElementById("variation");
  const recordButton = document.getElementById("record");
  const recordStatus = document.getElementById("record-status");
  const scoreDownload = document.getElementById("score-download");
  const sectionStarts = [0, 0, 2, 2, 4, 5, 5];
  const colors = ["#c65132", "#167d77", "#6755a4", "#bf8b18", "#397ca1", "#914c68",
    "#4f8b53", "#bd5f94", "#455a87", "#8a6d41", "#3f868e", "#9b4b3f"];
  const DEFAULT_VOICE_CYCLES = [3, 4, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.5, 3, 3.5];
  const voiceCycles = DEFAULT_VOICE_CYCLES.slice();
  const voiceBasePhases = Array(12).fill(0);
  for (let voice = 2; voice < voiceBasePhases.length; voice += 1)
    voiceBasePhases[voice] = 0.137 * (voice - 1);
  const couplings = [0, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28];
  const voiceTransforms = Array(12).fill("sine");
  const voiceEventRelations = Array(12).fill("none");
  const transformEquations = {
    sine: "T(x) = sen(θ)", invert: "T(x) = −sen(θ)",
    rectify: "T(x) = 2|sen(θ)| − 1",
    harmonic: "T(x) = [sen(θ) + 0,5·sen(2θ)] / 1,5",
    amplitude: "T(x) = sen(θ)·[0,65 + 0,35·cos(θ/2)]",
    quantized: "T(x) = arred(4·sen(θ)) / 4",
    cubic: "T(x) = sen³(θ)",
    thirdHarmonic: "T(x) = sen(3θ)",
    triangle: "T(x) = (2/π)·arcsen(sen(θ))",
    softClip: "T(x) = tanh(2·sen(θ)) / tanh(2)"
  };
  const transformNames = { sine: "Seno original", invert: "Inversão", rectify: "Retificação",
    harmonic: "Segundo harmônico", amplitude: "Envelope de amplitude", quantized: "Quantização em 4 níveis",
    cubic: "Cúbica", thirdHarmonic: "Terceiro harmônico", triangle: "Triangular",
    softClip: "Saturação suave" };
  const bassPattern = [36, null, 36, null, 43, null, 36, null,
    36, null, 39, null, 41, null, 43, null, 36, null, 43, null].map((midi) => midi === null ? null :
    { midi, durationSteps: 2, repeats: 1, intent: "normal" });
  let bassVoiceDriven = true;
  const durationOptions = [[1, "1/16"], [2, "1/8"], [4, "1/4"], [8, "1/2"], [16, "compasso"]];
  let bassDrag = null;
  let bassDrawMode = false;
  const AudioContextClass = globalThis.AudioContext || globalThis.webkitAudioContext;
  let generated;
  let scheduleRevision = 0;
  let renderedVoiceCount = 0;
  let running = false;
  const mutedVoices = new Set();
  const soloVoices = new Set();
  let masterMuted = false;
  const channelVolumes = Array(13).fill(1);
  const channelPans = Array(13).fill(0);
  const manuallyPannedVoices = new Set();
  let audio = null;
  let master = null;
  let formulaBus = null;
  let formulaAnalyser = null;
  let bassAnalyser = null;
  let offlineRenderStem = null;
  const meterSamples = new Float32Array(2048);
  let formulaMeterDb = null;
  let bassMeterDb = null;
  let suggestedTrimDeltaDb = 0;
  let recordDestination = null;
  let mediaRecorder = null;
  let recordChunks = [];
  let scoreDownloadUrl = null;
  let recordingScoreRevisions = [];
  let recordingRevisionSignature = "";
  let recordingStartedAt = 0;
  let recordingStartSeconds = 0;
  let recordingAudioFile = null;
  let recordingDirectoryHandle = null;
  let recordingInitialEvents = [];
  let recordingRevisionTruncated = false;
  let archiveDirectoryHandle = null;
  let channelBuses = [];
  let audioStart = 0;
  let wallStart = 0;
  let nextEvent = 0;
  let scheduler = null;
  let frame = null;
  let currentCycle = -1;
  let graphZoom = 1;
  let graphYScale = 1;
  let graphStart = 0;
  let graphDrag = null;

  for (let i = 0; i < 7; i += 1) {
    const segment = document.createElement("span");
    segment.textContent = `C${i + 1}`;
    segment.setAttribute("role", "listitem");
    timeline.appendChild(segment);
  }

  function phaseText(value) {
    return value.toFixed(3).replace(".", ",");
  }

  function mathematicalPartitura(config) {
    const dynamicsEquations = {
      steady: "D(p)=1",
      crescendo: "D(p)=0.35+0.9p",
      arch: "D(p)=0.38+0.85sin(πp)",
      waves: "D(p)=0.75+0.25sin(4πp-π/2)"
    };
    const intentionEquations = {
      build: "I(p)=0.68+0.58p",
      thin: "I(p)=1.18-0.62p",
      meet: "I(p)=1",
      contrast: "I(p)=1.13 se floor(14p) par; senão 0.78"
    };
    const durationEquations = {
      manual: "d_n = duração escrita na célula",
      fibonacci: "d_n = min(passos_do_compasso, F_(n mod 8 + 1))",
      geometric: `d_n = min(passos_do_compasso, round(${config.bassGeoRatio}^(n mod 8)))`
    };
    const oscillatorProfileEquations = {
      harmonic: "s(t)=sin(ωt)+0,34·sin(2ωt)+0,16·sin(3ωt)",
      odd: "s(t)=sin(ωt)+0,34·sin(3ωt)+0,16·sin(5ωt)",
      inharmonic: "s(t)=sin(ωt)+0,34·sin(√2·ωt)+0,16·sin(√3·ωt)",
      beat: "s(t)=sin(ωt)+0,34·sin(1,003·ωt)+0,16·sin(0,997·ωt)"
    };
    return {
      phase: "θ_i(u) = 2π(c_i·u + φ_i)",
      seed_selection: {
        seed: config.seed, voice_count: config.voiceCount,
        voice_two_cycles: config.voiceCycleBases[1],
        phase_index: Math.round(config.phase / 0.005),
        event_rule: config.recipe, spatial_pattern: config.patternMode,
        intensity_law: config.intensityLaw,
        intensity_amount: config.intensityAmount
      },
      intensity: {
        equation: "I = 1 − λ + λ·clamp(L(x,y), 0, 1)",
        law: config.intensityLaw, amount: config.intensityAmount
      },
      event_rule: config.recipe === "center"
        ? "ataques nos cruzamentos de y_i(u)=0"
        : "ataques nos extremos locais de y_i(u)",
      voices: config.voiceTransforms.map((transform, voice) => ({
        voice: voice + 1,
        formula: transformEquations[transform],
        cycles_per_section: config.voiceCycles[voice],
        phase_offset_cycles: (voice - 1) / 16 +
          (config.voiceBasePhases?.[voice] || 0) +
          (config.voicePhaseOffsets?.[voice] || 0),
        coupling: config.couplings[voice],
        event_relation: config.voiceEventRelations[voice],
        event_relation_source_voice: voice > 0 ? voice : null,
        event_relation_strength: config.couplings[voice],
        relation: voice === 0 || !config.couplings[voice]
          ? "y_i(u)=T_i(sin(θ_i(u)))"
          : `y_i(u)=(1-λ_i)T_i(sin(θ_i(u)))+λ_i y_${voice}(u)`
      })),
      expression: {
        dynamics: dynamicsEquations[config.dynamics] || "D(p)=1",
        intention: intentionEquations[config.intention] || "I(p)=1",
        p: "progresso normalizado da forma, limitado a [0,1]"
      },
      rhythm: {
        bpm: config.bpm, meter: config.meter, swing_ratio: config.swing,
        swing_mapping: "u'=piecewise-linear(u; ponto médio deslocado pelo swing)"
      },
      bass: {
        seed: config.seed,
        composition_variation: config.variation,
        pitch_variation_base: config.bassPitchVariationBase,
        pitch_variation_effective: config.bassPitchVariation,
        amplitude_envelope: {
          equation: "A(t): ataque linear → decaimento linear → sustentação → soltura exponencial",
          level: config.bassOutputLevel,
          attack_seconds: config.bassAmpAttackSeconds,
          decay_seconds: config.bassAmpDecaySeconds,
          sustain: config.bassAmpSustain,
          release_seconds: config.bassAmpReleaseSeconds,
          gate_ratio: config.bassGateRatio
        },
        filter_envelope: {
          equation: "f_c(t)=f_base+abertura·E(t); E usa ADSR e retorna a f_base na soltura",
          amount_hz: config.bassFilterEnvelopeAmountHz,
          attack_seconds: config.bassFilterAttackSeconds,
          decay_seconds: config.bassFilterDecaySeconds,
          sustain: config.bassFilterSustain,
          release_seconds: config.bassFilterReleaseSeconds
        },
        oscillator_profile: config.bassOscillatorProfile,
        oscillator_profile_seed_rule: "perfil = avalanche32(uint32(seed_hex)) & 3; escolha manual pode substituir o perfil",
        oscillator_equation: oscillatorProfileEquations[config.bassOscillatorProfile]
          || oscillatorProfileEquations.harmonic,
        pitch_rule: config.bassPitchVariation > 0
          ? "A seed desloca deterministicamente as alturas calculadas; o raio de variação é 6·v semitons e a chance é v."
          : "Variação 0: as alturas escritas são preservadas; a seed não altera o baixo.",
        duration: durationEquations[config.bassDurationFormula] || durationEquations.manual,
        lfo: config.bassLfoEnabled
          ? `midi_n = clamp(midi_base + round(${config.bassDepth}·LFO(p)), 24, 60)`
          : "LFO de altura desligado"
      }
    };
  }

  function scoreConfig() {
    return { recipe: recipeInput.value, phase: Number(phaseInput.value),
      patternMode: patternModeInput.value, intensityLaw: intensityLawInput.value,
      intensityAmount: Number(intensityAmountInput.value),
      motionModel: motionModelInput.value,
      pendulumLength: Number(pendulumLengthInput.value),
      pendulumCoupling: Number(pendulumCouplingInput.value),
      pendulumDamping: Number(pendulumDampingInput.value),
      durationSeconds: Number(durationInput.value),
      bpm: Number(bpmInput.value), meter: meterInput.value,
      intention: intentionInput.value, dynamics: dynamicsInput.value,
      swing: Number(swingInput.value), articulation: articulationInput.value,
      timbre: timbreInput.value, groove: grooveInput.value,
      bassShape: bassShapeInput.value, bassRate: Number(bassRateInput.value),
      bassTimbre: bassTimbreInput.value,
      bassOscillatorProfile: bassOscillatorProfileInput.value,
      bassVcfCutoffHz: Number(bassVcfCutoffInput.value),
      bassVcfResonance: Number(bassVcfResonanceInput.value),
      ...Object.fromEntries(Object.entries(bassEnvelopeInputs).map(([key, input]) =>
        [key, Number(input.value)])),
      bassDepth: Number(bassDepthInput.value), bassLfoEnabled: bassLfoEnabledInput.checked,
      bassPhraseMode: bassPhraseModeInput.value,
      bassRhythmRelation: bassRhythmRelationInput.value,
      bassDurationFormula: bassDurationFormulaInput.value,
      bassGeoRatio: Number(bassGeoRatioInput.value),
      bassPattern: bassPattern.map((step) => step && ({ ...step })),
      bassPitchVariationBase: Number(bassPitchVariationInput.value) / 100,
      bassVoiceDriven,
      voiceCount: Number(voiceCountInput.value),
      voiceCycles: voiceCycles.slice(0, Number(voiceCountInput.value)),
      voiceBasePhases: voiceBasePhases.slice(0, Number(voiceCountInput.value)),
      couplings: couplings.slice(0, Number(voiceCountInput.value)),
      voiceEventRelations: voiceEventRelations.slice(0, Number(voiceCountInput.value)),
      voiceTransforms: voiceTransforms.slice(0, Number(voiceCountInput.value)),
      seed: seedInput.value, variation: Number(variationInput.value) / 100,
      geometryShape: geometryShapeInput.value, geometrySource: geometrySourceInput.checked,
      geometryFourierTerms: Number(geometryFourierTermsInput.value),
      geometryBilliardPattern: Number(geometryBilliardPatternInput.value) };
  }

  function freezeSeedVariationForManualEdit() {
    if (Number(variationInput.value) > 0)
      variationInput.value = "0";
  }

  function applySeedDerivedBassProfile(seed) {
    const profiles = ["harmonic", "odd", "inharmonic", "beat"];
    bassOscillatorProfileInput.value = profiles[
      scoreModel.bassOscillatorProfileForSeed(seed)];
  }

  function applySeedDerivedComposition(seed) {
    const selection = scoreModel.seededRecipeSelection(seed);
    recipeInput.value = selection.recipe;
    phaseInput.value = String(selection.phase);
    voiceCountInput.value = String(selection.voiceCount);
    patternModeInput.value = selection.patternMode;
    intensityLawInput.value = selection.intensityLaw;
    intensityAmountInput.value = String(selection.intensityAmount);
    document.getElementById("intensity-amount-value").textContent =
      `${Math.round(selection.intensityAmount * 100)}%`;
    voiceCycles.splice(0, voiceCycles.length, ...DEFAULT_VOICE_CYCLES);
    voiceCycles[1] = selection.voiceTwoCycles;
    voiceBasePhases.fill(0);
    voiceBasePhases[1] = selection.phase;
    for (let voice = 2; voice < voiceBasePhases.length; voice += 1)
      voiceBasePhases[voice] = 0.137 * (voice - 1);
  }

  async function chooseArchiveDirectory() {
    if (!globalThis.showDirectoryPicker) {
      archiveStatus.textContent = "Este navegador não oferece acesso à pasta local. Use um navegador Chromium recente.";
      return;
    }
    try {
      archiveDirectoryHandle = await globalThis.showDirectoryPicker({ mode: "readwrite" });
      archiveList.disabled = false;
      document.getElementById("archive-save").disabled = false;
      archiveStatus.textContent = `Pasta ativa: ${archiveDirectoryHandle.name}. Os arquivos ficam no seu computador.`;
      await refreshCompositionArchive();
    } catch (error) {
      if (error.name !== "AbortError") archiveStatus.textContent = `Não foi possível abrir a pasta: ${error.message}`;
    }
  }

  async function ensureArchivePermission() {
    if (!archiveDirectoryHandle) throw new Error("Escolha primeiro uma pasta local.");
    const descriptor = { mode: "readwrite" };
    let permission = await archiveDirectoryHandle.queryPermission(descriptor);
    if (permission !== "granted") permission = await archiveDirectoryHandle.requestPermission(descriptor);
    if (permission !== "granted") throw new Error("Permissão de leitura e escrita não concedida.");
  }

  async function refreshCompositionArchive(selectedName = archiveList.value) {
    if (!archiveDirectoryHandle) return;
    try {
      await ensureArchivePermission();
      const files = [];
      for await (const [name, handle] of archiveDirectoryHandle.entries()) {
        if (handle.kind !== "file" ||
            (!name.endsWith(".batruc.txt") && !name.endsWith(".trucmetry.txt"))) continue;
        files.push({ name, handle });
      }
      files.sort((left, right) => right.name.localeCompare(left.name));
      archiveList.replaceChildren();
      if (!files.length) {
        const option = document.createElement("option");
        option.value = "";
        option.textContent = "Nenhum cálculo salvo nesta pasta";
        archiveList.appendChild(option);
      }
      for (const item of files) {
        const option = document.createElement("option");
        option.value = item.name;
        try {
          const contents = JSON.parse(await (await item.handle.getFile()).text());
          option.textContent = `${contents.name || item.name} · ${contents.config?.seed || "sem seed"}`;
        } catch {
          option.textContent = `${item.name} · leitura indisponível`;
        }
        archiveList.appendChild(option);
      }
      if (files.some((item) => item.name === selectedName)) archiveList.value = selectedName;
      const available = files.length > 0;
      document.getElementById("archive-load").disabled = !available;
      document.getElementById("archive-delete").disabled = !available;
      archiveStatus.textContent = `${archiveDirectoryHandle.name}: ${files.length} cálculo(s) encontrado(s).`;
    } catch (error) {
      archiveStatus.textContent = `Não foi possível ler a pasta: ${error.message}`;
    }
  }

  function mixerSnapshot() {
    return {
      volumes: channelVolumes.slice(), pans: channelPans.slice(),
      muted: [...mutedVoices], solo: [...soloVoices],
      manuallyPanned: [...manuallyPannedVoices],
      masterVolume: Number(outputLevelInput.value), masterMuted,
      formulaBusGainDb: Number(formulaBusTrimInput.value)
    };
  }

  function saveCompositionData() {
    const config = scoreConfig();
    if (!/^[A-F0-9]{8}$/.test(config.seed)) {
      archiveStatus.textContent = "Complete os oito caracteres hexadecimais da seed antes de guardar.";
      return null;
    }
    const name = archiveNameInput.value.trim().slice(0, 64) || `Seed ${config.seed}`;
    const data = {
      format: "batruc-composition",
      schema_version: 1,
      name,
      created_at: new Date().toISOString(),
      config,
      mathematical_partitura: mathematicalPartitura(generated.config),
      mixer: mixerSnapshot()
    };
    return { name, data };
  }

  async function writeFileToArchive(directoryHandle, filename, contents, mimeType) {
    const descriptor = { mode: "readwrite" };
    let permission = await directoryHandle.queryPermission(descriptor);
    if (permission !== "granted") permission = await directoryHandle.requestPermission(descriptor);
    if (permission !== "granted") throw new Error("Permissão de leitura e escrita não concedida.");
    const handle = await directoryHandle.getFileHandle(filename, { create: true });
    const writable = await handle.createWritable();
    try {
      await writable.write(new Blob([contents], { type: mimeType }));
      await writable.close();
    } catch (error) {
      await writable.abort().catch(() => {});
      throw error;
    }
  }

  function safeFilename(text) {
    return text.normalize("NFKD").replace(/[\u0300-\u036f]/g, "")
      .replace(/[^A-Za-z0-9_-]+/g, "-").replace(/^-+|-+$/g, "").slice(0, 48) || "composicao";
  }

  async function saveCurrentComposition() {
    try {
      if (!archiveDirectoryHandle) {
        archiveStatus.textContent = "Escolha a pasta local antes de guardar o cálculo.";
        return;
      }
      const composition = saveCompositionData();
      if (!composition) return;
      const filename = `batruc-${safeFilename(composition.name)}-${composition.data.config.seed}-${Date.now()}.batruc.txt`;
      await writeFileToArchive(archiveDirectoryHandle, filename,
        JSON.stringify(composition.data, null, 2), "text/plain;charset=utf-8");
      archiveNameInput.value = "";
      await refreshCompositionArchive(filename);
      archiveStatus.textContent = `Cálculo “${composition.name}” salvo em ${archiveDirectoryHandle.name}.`;
    } catch (error) {
      archiveStatus.textContent = `Não foi possível guardar o cálculo: ${error.message}`;
    }
  }

  function applyCompositionData(data) {
    if ((data.format !== "batruc-composition" && data.format !== "trucmetry-composition") ||
        data.schema_version !== 1 ||
        !data.config || !/^[A-F0-9]{8}$/.test(data.config.seed || ""))
      throw new Error("Arquivo de composição inválido ou versão não suportada.");
    const config = data.config;
    const controls = {
      recipe: recipeInput, phase: phaseInput, patternMode: patternModeInput,
      intensityLaw: intensityLawInput, intensityAmount: intensityAmountInput,
      motionModel: motionModelInput,
      pendulumLength: pendulumLengthInput, pendulumCoupling: pendulumCouplingInput,
      pendulumDamping: pendulumDampingInput, durationSeconds: durationInput,
      bpm: bpmInput, meter: meterInput, intention: intentionInput, dynamics: dynamicsInput,
      swing: swingInput, articulation: articulationInput, timbre: timbreInput,
      groove: grooveInput, bassShape: bassShapeInput, bassRate: bassRateInput,
      bassTimbre: bassTimbreInput, bassOscillatorProfile: bassOscillatorProfileInput,
      bassVcfCutoffHz: bassVcfCutoffInput, bassVcfResonance: bassVcfResonanceInput,
      ...bassEnvelopeInputs,
      bassDepth: bassDepthInput,
      bassPitchVariationBase: bassPitchVariationInput,
      bassLfoEnabled: bassLfoEnabledInput, bassPhraseMode: bassPhraseModeInput,
      bassDurationFormula: bassDurationFormulaInput, bassGeoRatio: bassGeoRatioInput,
      voiceCount: voiceCountInput, seed: seedInput, geometryShape: geometryShapeInput,
      geometrySource: geometrySourceInput, geometryFourierTerms: geometryFourierTermsInput,
      geometryBilliardPattern: geometryBilliardPatternInput
    };
    for (const [key, input] of Object.entries(controls)) {
      if (config[key] === undefined) continue;
      if (input.type === "checkbox") input.checked = Boolean(config[key]);
      else input.value = String(config[key]);
    }
    document.getElementById("intensity-amount-value").textContent =
      `${Math.round(Number(intensityAmountInput.value) * 100)}%`;
    variationInput.value = String(Number(config.variation || 0) * 100);
    document.getElementById("bass-pitch-variation-value").textContent =
      `${Math.round(Number(bassPitchVariationInput.value) || 0)}%`;
    voiceCycles.splice(0, voiceCycles.length,
      ...config.voiceCycles, ...DEFAULT_VOICE_CYCLES.slice(config.voiceCycles.length));
    voiceBasePhases.splice(0, voiceBasePhases.length,
      ...(config.voiceBasePhases || Array.from({ length: config.voiceCount }, (_, voice) =>
        voice === 1 ? Number(config.phase || 0) : voice >= 2 ? 0.137 * (voice - 1) : 0)),
      ...Array.from({ length: 12 - config.voiceCount }, () => 0));
    couplings.splice(0, couplings.length,
      ...config.couplings, ...Array.from({ length: 12 - config.couplings.length }, () => 0.28));
    voiceEventRelations.splice(0, voiceEventRelations.length,
      ...(config.voiceEventRelations || []),
      ...Array.from({ length: 12 - (config.voiceEventRelations || []).length }, () => "none"));
    voiceTransforms.splice(0, voiceTransforms.length,
      ...config.voiceTransforms, ...Array.from({ length: 12 - config.voiceTransforms.length }, () => "sine"));
    bassPattern.splice(0, bassPattern.length, ...config.bassPattern.map((step) => step && ({ ...step })));
    bassVoiceDriven = config.bassVoiceDriven === true;

    const mixer = data.mixer || {};
    channelVolumes.splice(0, channelVolumes.length, ...(mixer.volumes || Array(13).fill(1)));
    channelPans.splice(0, channelPans.length, ...(mixer.pans || Array(13).fill(0)));
    mutedVoices.clear(); (mixer.muted || []).forEach((voice) => mutedVoices.add(voice));
    soloVoices.clear(); (mixer.solo || []).forEach((voice) => soloVoices.add(voice));
    manuallyPannedVoices.clear();
    (mixer.manuallyPanned || []).forEach((voice) => manuallyPannedVoices.add(voice));
    masterMuted = Boolean(mixer.masterMuted);
    formulaBusTrimInput.value = String(Math.max(-12, Math.min(24,
      Number(mixer.formulaBusGainDb ?? 16.5))));
    updateFormulaBusTrim();
    outputLevelInput.value = String(mixer.masterVolume ?? 0.75);
    const masterMuteButton = document.getElementById("master-mute");
    masterMuteButton.setAttribute("aria-pressed", String(masterMuted));
    masterMuteButton.setAttribute("aria-label", masterMuted ? "Retirar mute do master" : "Mute master");
    updateOutputLevel();
    refreshScore();
    renderMixerChannels(generated.config.voiceCount);
    renderBassPatternControls();
    drawBassEditor();
    archiveStatus.textContent = `“${data.name}” reaberta · seed ${config.seed}.`;
    status.textContent = `Composição local “${data.name}” carregada.`;
  }

  async function loadSelectedComposition() {
    if (!archiveDirectoryHandle || !archiveList.value) return;
    try {
      await ensureArchivePermission();
      const handle = await archiveDirectoryHandle.getFileHandle(archiveList.value);
      const data = JSON.parse(await (await handle.getFile()).text());
      applyCompositionData(data);
    } catch (error) {
      archiveStatus.textContent = `Não foi possível abrir o cálculo: ${error.message}`;
    }
  }

  async function deleteSelectedComposition() {
    if (!archiveDirectoryHandle || !archiveList.value) return;
    const filename = archiveList.value;
    const confirmation = `Apagar “${filename}” da pasta ${archiveDirectoryHandle.name}?`;
    if (!globalThis.confirm(globalThis.BatrucI18n?.t(confirmation) || confirmation)) return;
    try {
      await ensureArchivePermission();
      await archiveDirectoryHandle.removeEntry(filename);
      await refreshCompositionArchive();
      archiveStatus.textContent = `Arquivo ${filename} removido de ${archiveDirectoryHandle.name}.`;
    } catch (error) {
      archiveStatus.textContent = `Não foi possível apagar o cálculo: ${error.message}`;
    }
  }

  function updateOutputLevel() {
    const level = Number(outputLevelInput.value);
    outputValue.value = `${Math.round(level * 100)}%`;
    if (master && audio) master.gain.setTargetAtTime(masterMuted ? 0 : level, audio.currentTime, 0.025);
  }

  function updateFormulaBusTrim() {
    const gainDb = Math.max(-12, Math.min(24, Number(formulaBusTrimInput.value)));
    formulaBusTrimInput.value = String(gainDb);
    formulaBusTrimValue.value = `${gainDb >= 0 ? "+" : ""}${gainDb.toFixed(1).replace(".", ",")} dB`;
    applyMixer();
  }

  function analyserRmsDb(analyser) {
    if (!analyser) return null;
    analyser.getFloatTimeDomainData(meterSamples);
    let sum = 0;
    for (let index = 0; index < meterSamples.length; index += 1)
      sum += meterSamples[index] * meterSamples[index];
    const rms = Math.sqrt(sum / meterSamples.length);
    return rms > 1e-6 ? 20 * Math.log10(rms) : null;
  }

  function updateMixerBalance() {
    const formulaDb = analyserRmsDb(formulaAnalyser);
    const bassDb = analyserRmsDb(bassAnalyser);
    const formatDb = (value) => value === null ? "−∞" : `${value.toFixed(1)} dBFS`;
    if (formulaDb === null || bassDb === null) {
      mixerBalanceReadout.textContent = `SENSOR · vozes ${formatDb(formulaDb)} · baixo ${formatDb(bassDb)}`;
      mixerAutoBalanceButton.disabled = true;
      return;
    }
    suggestedTrimDeltaDb = Math.max(-12 - Number(formulaBusTrimInput.value),
      Math.min(24 - Number(formulaBusTrimInput.value), bassDb - formulaDb));
    mixerBalanceReadout.textContent = `RMS · vozes ${formatDb(formulaDb)} · baixo ${formatDb(bassDb)} · ajuste ${suggestedTrimDeltaDb >= 0 ? "+" : ""}${suggestedTrimDeltaDb.toFixed(1)} dB`;
    mixerAutoBalanceButton.disabled = false;
    mixerAutoBalanceButton.title = `Aplica ${suggestedTrimDeltaDb >= 0 ? "+" : ""}${suggestedTrimDeltaDb.toFixed(1)} dB às vozes para aproximar os níveis RMS medidos. É uma compensação estática, não automação contínua.`;
  }

  function applySuggestedMixerBalance() {
    if (mixerAutoBalanceButton.disabled) return;
    formulaBusTrimInput.value = String(Math.max(-12, Math.min(24,
      Number(formulaBusTrimInput.value) + suggestedTrimDeltaDb)));
    updateFormulaBusTrim();
    status.textContent = "Trim das vozes ajustado pela leitura RMS atual; o ajuste fica salvo na composição.";
  }

  function graphWindowSpan() { return 1 / graphZoom; }

  function updateGraphViewControls() {
    const maxStart = 1 - graphWindowSpan();
    graphStart = Math.max(0, Math.min(maxStart, graphStart));
    graphZoomInput.value = String(graphZoom);
    graphFocusInput.value = String(maxStart === 0 ? 0 : Math.round(graphStart / maxStart * 100));
    graphFocusInput.disabled = false;
    graphZoomValue.value = `${graphZoom.toFixed(graphZoom % 1 ? 2 : 0).replace(".", ",")}×`;
    graphFocusValue.value = `${Math.round((graphStart + graphWindowSpan() / 2) * 100)}%`;
    graph.style.cursor = graphZoom > 1 ? "grab" : "crosshair";
  }

  function setGraphViewFromControls(source = "zoom") {
    graphZoom = Number(graphZoomInput.value);
    if (source === "focus" && graphZoom === 1) {
      graphZoom = 2;
      graphZoomInput.value = "2";
    }
    graphStart = (1 - graphWindowSpan()) * Number(graphFocusInput.value) / 100;
    updateGraphViewControls();
    draw(running ? elapsed() : 0);
  }

  function setGraphViewAt(zoom, start) {
    graphZoom = Math.max(1, Math.min(8, zoom));
    graphStart = start;
    updateGraphViewControls();
    draw(running ? elapsed() : 0);
  }

  function elapsed() {
    const seconds = audio ? audio.currentTime - audioStart : (performance.now() - wallStart) / 1000;
    return Math.max(0, Math.min(generated ? generated.duration : 56, seconds));
  }

  function geometryEquationText(shape, isSource, billiardPattern = 0) {
    const equations = {
      compass: "x = cos(θ) · y = sen(θ)",
      triangle: "trajeto linear pelos 3 vértices · y = interpolação do percurso",
      square: "trajeto linear pelos 4 vértices · y = interpolação do percurso",
      spiral: "r = t · x = r cos(2πnt) · y = r sen(2πnt)",
      lissajous: "x = sen(3θ) · y = sen(2θ)",
      "fourier-triangle": "zₕ(t) = Σ cₙeⁱ²πⁿᵗ · triângulo fechado · n = −H…+H",
      "fourier-square": "zₕ(t) = Σ cₙeⁱ²πⁿᵗ · quadrado fechado · n = −H…+H",
      epicycloid: "Epicicloide: R=2r · x=(3 cos θ−cos 3θ)/4 · y=(3 sen θ−sen 3θ)/4",
      hypocycloid: "Hipocicloide: R=3r · x=(2 cos θ+cos 2θ)/3 · y=(2 sen θ−sen 2θ)/3",
      cycloid: "Cicloide: roda de raio r sobre reta · centro x=r(θ−π) · ponta x=r(θ−π−sen θ), y=−r cos θ",
      catenary: "Catenária: y=cosh(x) · x=sen θ · normalizada",
      "billiard-square": "Bilhar quadrado: reflexão especular · v′=v−2(v·n)n"
    };
    const relation = isSource ? "trajeto e colisões alimentam a voz 1" : "percurso visual";
    if (shape === "billiard-square") {
      const laps = [[2, 4], [4, 2], [2, 6], [6, 2], [4, 4], [4, 6], [6, 4]][billiardPattern] || [2, 4];
      const hits = scoreModel.billiardCollisionPositions(1, 0, billiardPattern).length;
      return `${equations[shape]} · voltas x:y=${laps[0]}:${laps[1]} · ${hits} impactos/ciclo · ${relation}`;
    }
    if (shape === "triangle" || shape === "square") {
      const metrics = scoreModel.geometryPolygonMetrics(shape);
      return `${equations[shape]} · A=${metrics.area.toFixed(3)} u² · P=${metrics.perimeter.toFixed(3)} u · ${relation}`;
    }
    if (shape === "fourier-triangle" || shape === "fourier-square") {
      const terms = Number(geometryFourierTermsInput.value);
      const error = scoreModel.geometryFourierError(shape, terms);
      return `${equations[shape]} · H=${terms} (${2 * terms + 1} componentes) · erro RMS relativo XY ${error.toFixed(2)}% · ${relation}`;
    }
    return `${equations[shape] || equations.compass} · ${relation}`;
  }

  function drawGeometryTrace(progress, cycle) {
    const bounds = geometryCanvas.getBoundingClientRect();
    const dpr = Math.min(2, globalThis.devicePixelRatio || 1);
    const width = Math.max(1, Math.round(bounds.width * dpr));
    const height = Math.max(1, Math.round(bounds.height * dpr));
    if (geometryCanvas.width !== width || geometryCanvas.height !== height) {
      geometryCanvas.width = width;
      geometryCanvas.height = height;
    }
    geometryCtx.setTransform(dpr, 0, 0, dpr, 0, 0);
    const w = bounds.width, h = bounds.height;
    const cx = w / 2, cy = h / 2;
    const radius = Math.max(20, Math.min(w * 0.39, h * 0.39));
    const shape = generated.config.geometryShape;
    const cycles = generated.config.voiceCycles[0];
    const fourierTerms = generated.config.geometryFourierTerms;
    const phase = scoreModel.phaseFor(cycle, 0, generated.config.phase, progress, generated.config);
    const end = running ? Math.max(0, Math.min(1, progress)) : 1;
    const sampleCount = Math.max(64, Math.ceil(360 * cycles * Math.max(end, 0.02)));
    const mapPoint = (t) => {
      const point = scoreModel.geometryPoint(shape, t, cycles, phase, fourierTerms,
        generated.config.geometryBilliardPattern);
      return { x: cx + point.x * radius, y: cy - point.y * radius };
    };
    geometryCtx.clearRect(0, 0, w, h);
    geometryCtx.fillStyle = "#f1f1e8";
    geometryCtx.fillRect(0, 0, w, h);
    if (generated.config.motionModel === "coupled-pendulum") {
      const trajectory = generated.config.pendulumTrajectory;
      const phaseRadius = Math.max(20, Math.min(w * 0.38, h * 0.38));
      const cycleSamples = Math.round((trajectory.sampleCount - 1) / 7);
      const sectionColors = ["#167d77", "#167d77", "#b07a31", "#b07a31",
        "#c65132", "#718b7d", "#718b7d"];
      const phasePoint = (index) => ({
        x: cx + trajectory.angles[0][index] / trajectory.maxAngle * phaseRadius,
        y: cy - trajectory.angles[1][index] / trajectory.maxAngle * phaseRadius
      });
      const visible = running
        ? Math.max(0, Math.min(trajectory.sampleCount - 1,
          Math.floor(((cycle + progress) / 7) * (trajectory.sampleCount - 1))))
        : trajectory.sampleCount - 1;
      geometryCtx.strokeStyle = "#d4ddd4";
      geometryCtx.lineWidth = 1;
      geometryCtx.beginPath(); geometryCtx.moveTo(16, cy); geometryCtx.lineTo(w - 16, cy);
      geometryCtx.moveTo(cx, 12); geometryCtx.lineTo(cx, h - 12); geometryCtx.stroke();
      geometryCtx.strokeStyle = "#bdc9bf";
      geometryCtx.beginPath();
      for (const fraction of [-1, -0.5, 0.5, 1]) {
        const tick = fraction * phaseRadius;
        geometryCtx.moveTo(cx + tick, cy - 4); geometryCtx.lineTo(cx + tick, cy + 4);
        geometryCtx.moveTo(cx - 4, cy + tick); geometryCtx.lineTo(cx + 4, cy + tick);
      }
      geometryCtx.stroke();
      const trace = (first, last, color, lineWidth) => {
        geometryCtx.beginPath();
        for (let index = first; index <= last; index += 1) {
          const point = phasePoint(index);
          if (index === first) geometryCtx.moveTo(point.x, point.y);
          else geometryCtx.lineTo(point.x, point.y);
        }
        geometryCtx.strokeStyle = color; geometryCtx.lineWidth = lineWidth;
        geometryCtx.lineJoin = "round"; geometryCtx.stroke();
      };
      trace(0, trajectory.sampleCount - 1, "#d4d9d1", 1.2);
      for (let section = 0; section < 7; section += 1) {
        const first = section * cycleSamples;
        const last = Math.min(visible, (section + 1) * cycleSamples,
          trajectory.sampleCount - 1);
        if (last > first) trace(first, last, sectionColors[section], 2.7);
        if (first > 0 && first <= visible) {
          const boundary = phasePoint(first);
          geometryCtx.fillStyle = sectionColors[section];
          geometryCtx.beginPath(); geometryCtx.arc(boundary.x, boundary.y, 2.8, 0, Math.PI * 2);
          geometryCtx.fill();
        }
      }
      const marker = phasePoint(visible);
      geometryCtx.strokeStyle = "#f1f1e8"; geometryCtx.lineWidth = 2;
      geometryCtx.beginPath(); geometryCtx.arc(marker.x, marker.y, 7, 0, Math.PI * 2);
      geometryCtx.stroke();
      geometryCtx.fillStyle = "#c65132"; geometryCtx.beginPath();
      geometryCtx.arc(marker.x, marker.y, 5, 0, Math.PI * 2); geometryCtx.fill();
      geometryCtx.fillStyle = "#536b62";
      geometryCtx.font = "10px ui-monospace, monospace";
      geometryCtx.fillText("θ₁ (rad)", w - 64, cy - 8);
      geometryCtx.fillText("θ₂ (rad)", cx + 7, 14);
      geometryCtx.fillText(`${trajectory.maxAngle.toFixed(2)}`, cx + 5, cy - phaseRadius + 4);
      geometryCtx.fillText(`${(-trajectory.maxAngle).toFixed(2)}`, cx + 5, cy + phaseRadius - 4);
      return;
    }
    geometryCtx.strokeStyle = "#d4ddd4";
    geometryCtx.lineWidth = 1;
    geometryCtx.beginPath(); geometryCtx.moveTo(cx, 8); geometryCtx.lineTo(cx, h - 8);
    geometryCtx.moveTo(8, cy); geometryCtx.lineTo(w - 8, cy); geometryCtx.stroke();
    const patternMode = generated.config.patternMode;
    const patternSeed = Number.parseInt(generated.config.seed, 16) >>> 0;
    const patternProgress = cycle + progress;
    const pose = scoreModel.patternFieldPose(patternMode, patternProgress, patternSeed);
    const mapFieldPoint = (x, y) => ({ x: cx + x * radius, y: cy - y * radius });
    if (patternMode === "unrestricted") {
      const focus = mapFieldPoint(pose.focusX, pose.focusY);
      geometryCtx.fillStyle = "rgba(22,125,119,.10)";
      geometryCtx.strokeStyle = "rgba(22,125,119,.55)";
      geometryCtx.lineWidth = 1.2;
      geometryCtx.beginPath();
      geometryCtx.arc(focus.x, focus.y, pose.focusRadius * radius, 0, TAU);
      geometryCtx.fill(); geometryCtx.stroke();
      geometryCtx.fillStyle = "#167d77";
      geometryCtx.beginPath(); geometryCtx.arc(focus.x, focus.y, 2.5, 0, TAU);
      geometryCtx.fill();
    } else if (patternMode === "adhesive-pads") {
      const centres = [[0, -0.58], [0, 0.58], [-0.58, 0], [0.58, 0]];
      const cosine = Math.cos(pose.rotation), sine = Math.sin(pose.rotation);
      geometryCtx.fillStyle = "rgba(22,125,119,.11)";
      geometryCtx.strokeStyle = "rgba(22,125,119,.56)";
      geometryCtx.lineWidth = 1.1;
      geometryCtx.lineCap = "round";
      for (let bridgeIndex = 0; bridgeIndex < 4; bridgeIndex += 1) {
        const bridge = scoreModel.patternFieldPadBridge(pose, bridgeIndex);
        if (bridge.radius <= 0) continue;
        const mapBridgePoint = (point) => mapFieldPoint(
          cosine * point.x - sine * point.y + pose.offsetX,
          sine * point.x + cosine * point.y + pose.offsetY);
        const start = mapBridgePoint(bridge.start), end = mapBridgePoint(bridge.end);
        geometryCtx.beginPath();
        geometryCtx.moveTo(start.x, start.y);
        geometryCtx.lineTo(end.x, end.y);
        const width = 2 * bridge.radius * radius;
        geometryCtx.strokeStyle = "rgba(22,125,119,.08)";
        geometryCtx.lineWidth = width + 2 * scoreModel.patternEdgeSoftness * radius;
        geometryCtx.stroke();
        geometryCtx.strokeStyle = "rgba(22,125,119,.19)";
        geometryCtx.lineWidth = width;
        geometryCtx.stroke();
      }
      for (let organism = 0; organism < centres.length; organism += 1) {
        const [centreX, centreY] = centres[organism];
        const organismRadius = scoreModel.patternFieldPadRadius(pose, organism);
        geometryCtx.beginPath();
        for (let step = 0; step <= 96; step += 1) {
          const angle = TAU * step / 96;
          const boundary = scoreModel.patternFieldPadBoundary(pose, angle, organism);
          const localX = centreX + pose.padCentreOffsets[organism].x +
            organismRadius * boundary * Math.cos(angle);
          const localY = centreY + pose.padCentreOffsets[organism].y +
            organismRadius * boundary * Math.sin(angle);
          const point = mapFieldPoint(cosine * localX - sine * localY + pose.offsetX,
            sine * localX + cosine * localY + pose.offsetY);
          if (step === 0) geometryCtx.moveTo(point.x, point.y);
          else geometryCtx.lineTo(point.x, point.y);
        }
        geometryCtx.closePath();
        geometryCtx.strokeStyle = "rgba(22,125,119,.10)";
        geometryCtx.lineWidth = 2 * scoreModel.patternEdgeSoftness * radius;
        geometryCtx.stroke();
        const pulseAlpha = 0.075 + (organismRadius - 0.34) * 0.42;
        geometryCtx.fillStyle = `rgba(22,125,119,${pulseAlpha})`;
        geometryCtx.fill();
        geometryCtx.strokeStyle = "rgba(22,125,119,.56)";
        geometryCtx.lineWidth = 1.1;
        geometryCtx.stroke();
      }
    } else if (patternMode === "diagonal-lanes") {
      const cosine = Math.cos(pose.rotation), sine = Math.sin(pose.rotation);
      geometryCtx.lineCap = "round";
      geometryCtx.lineJoin = "round";
      for (let laneIndex = 0; laneIndex < 2; laneIndex += 1) {
        const direction = laneIndex === 0 ? 1 : -1;
        geometryCtx.beginPath();
        for (let step = 0; step <= 128; step += 1) {
          const x = -1 + 2 * step / 128;
          const y = direction * (0.55 * x +
            scoreModel.patternFieldLaneDisplacement(pose, x)) +
            pose.laneCentreOffsets[laneIndex];
          const point = mapFieldPoint(cosine * x - sine * y + pose.offsetX,
            sine * x + cosine * y + pose.offsetY);
          if (step === 0) geometryCtx.moveTo(point.x, point.y);
          else geometryCtx.lineTo(point.x, point.y);
        }
        const strokeWidth = 2 * pose.laneHalfWidths[laneIndex] * radius;
        geometryCtx.strokeStyle = "rgba(22,125,119,.09)";
        geometryCtx.lineWidth = strokeWidth
          + 2 * scoreModel.patternEdgeSoftness * radius;
        geometryCtx.stroke();
        geometryCtx.strokeStyle = "rgba(22,125,119,.23)";
        geometryCtx.lineWidth = 2 * pose.laneHalfWidths[laneIndex] * radius;
        geometryCtx.stroke();
      }
    }
    if (shape === "billiard-square") {
      const wall = radius;
      geometryCtx.strokeStyle = "#6f8277";
      geometryCtx.lineWidth = 2;
      geometryCtx.strokeRect(cx - wall, cy - wall, wall * 2, wall * 2);
      geometryCtx.fillStyle = "#536b62";
      geometryCtx.font = "10px ui-monospace, monospace";
      geometryCtx.fillText("REFLEXÃO ESPECULAR · 6 COLISÕES / CICLO", 12, 18);
    } else {
      geometryCtx.setLineDash([3, 5]);
      geometryCtx.strokeStyle = "#bdc9bf";
      geometryCtx.beginPath(); geometryCtx.ellipse(cx, cy, radius, radius, 0, 0, Math.PI * 2);
      geometryCtx.stroke(); geometryCtx.setLineDash([]);
    }
    const drawPath = (limit, color, lineWidth) => {
      geometryCtx.beginPath();
      for (let i = 0; i <= sampleCount; i += 1) {
        const t = limit * i / sampleCount;
        const point = mapPoint(t);
        if (i === 0) geometryCtx.moveTo(point.x, point.y);
        else geometryCtx.lineTo(point.x, point.y);
      }
      geometryCtx.strokeStyle = color;
      geometryCtx.lineWidth = lineWidth;
      geometryCtx.lineJoin = "round";
      geometryCtx.lineCap = "round";
      geometryCtx.stroke();
    };
    const fourierShape = shape === "fourier-triangle" || shape === "fourier-square";
    if (fourierShape) {
      geometryCtx.save();
      geometryCtx.setLineDash([4, 5]);
      geometryCtx.strokeStyle = "#aab7ad";
      geometryCtx.lineWidth = 1.2;
      geometryCtx.beginPath();
      for (let i = 0; i <= sampleCount; i += 1) {
        const point = scoreModel.geometryReferencePoint(shape, i / sampleCount, cycles, phase);
        const mapped = { x: cx + point.x * radius, y: cy - point.y * radius };
        if (i === 0) geometryCtx.moveTo(mapped.x, mapped.y);
        else geometryCtx.lineTo(mapped.x, mapped.y);
      }
      geometryCtx.stroke();
      geometryCtx.restore();
    }
    drawPath(1, fourierShape ? "#d2d9d1" : "#c7d2c8", 1.5);
    if (end > 0) drawPath(end, "#167d77", 3);
    const pen = mapPoint(end);
    if (shape === "billiard-square") {
      for (const collision of scoreModel.billiardCollisionPositions(cycles, phase,
        generated.config.geometryBilliardPattern)) {
        if (collision > end + 1e-9) continue;
        const point = mapPoint(collision);
        geometryCtx.fillStyle = "#c65132";
        geometryCtx.beginPath(); geometryCtx.arc(point.x, point.y, 4, 0, TAU);
        geometryCtx.fill();
      }
      geometryCtx.fillStyle = "#e3aa43";
      geometryCtx.beginPath(); geometryCtx.arc(pen.x, pen.y, 6, 0, TAU);
      geometryCtx.fill();
    }
    if (shape === "epicycloid" || shape === "hypocycloid") {
      const externalRolling = shape === "epicycloid";
      const rollingRadius = radius * 0.25;
      const fixedRadius = radius * (externalRolling ? 0.5 : 0.75);
      const orbitRadius = fixedRadius + (externalRolling ? rollingRadius : -rollingRadius);
      const angle = TAU * (end * cycles + phase);
      const rollingX = cx + orbitRadius * Math.cos(angle);
      const rollingY = cy - orbitRadius * Math.sin(angle);
      geometryCtx.strokeStyle = "rgba(83,107,98,.65)";
      geometryCtx.lineWidth = 1;
      geometryCtx.beginPath(); geometryCtx.arc(cx, cy, fixedRadius, 0, TAU);
      geometryCtx.stroke();
      geometryCtx.strokeStyle = "rgba(103,85,164,.62)";
      geometryCtx.beginPath();
      geometryCtx.arc(rollingX, rollingY, rollingRadius, 0, TAU);
      geometryCtx.moveTo(rollingX, rollingY); geometryCtx.lineTo(pen.x, pen.y);
      geometryCtx.stroke();
      geometryCtx.fillStyle = "#6755a4";
      geometryCtx.beginPath(); geometryCtx.arc(rollingX, rollingY, 2.4, 0, TAU);
      geometryCtx.fill();
    }
    if (fourierShape) {
      const vectors = scoreModel.geometryFourierEpicycles(shape, end, cycles, phase, fourierTerms);
      geometryCtx.save();
      geometryCtx.lineWidth = 0.8;
      vectors.forEach((vector, index) => {
        const x = cx + vector.fromX * radius;
        const y = cy - vector.fromY * radius;
        const nextX = cx + vector.toX * radius;
        const nextY = cy - vector.toY * radius;
        const circleRadius = vector.radius * radius;
        if (circleRadius > 0.7) {
          geometryCtx.beginPath(); geometryCtx.arc(x, y, circleRadius, 0, Math.PI * 2);
          geometryCtx.strokeStyle = index === 0 ? "#b8c4ba" : "rgba(103,85,164,.22)";
          geometryCtx.stroke();
        }
        geometryCtx.beginPath(); geometryCtx.moveTo(x, y); geometryCtx.lineTo(nextX, nextY);
        geometryCtx.strokeStyle = "rgba(103,85,164,.55)"; geometryCtx.stroke();
      });
      geometryCtx.restore();
      geometryCtx.fillStyle = "#6755a4";
      geometryCtx.beginPath(); geometryCtx.arc(pen.x, pen.y, 2.5, 0, Math.PI * 2); geometryCtx.fill();
    }
    const ahead = mapPoint(Math.min(1, end + 0.002));
    const angle = Math.atan2(ahead.y - pen.y, ahead.x - pen.x);
    if (shape === "compass" || shape === "spiral") {
      geometryCtx.strokeStyle = "#6755a4"; geometryCtx.lineWidth = 1.5;
      geometryCtx.beginPath(); geometryCtx.moveTo(cx, cy); geometryCtx.lineTo(pen.x, pen.y); geometryCtx.stroke();
      geometryCtx.fillStyle = "#6755a4"; geometryCtx.beginPath(); geometryCtx.arc(cx, cy, 4, 0, Math.PI * 2); geometryCtx.fill();
    }
    geometryCtx.save(); geometryCtx.translate(pen.x, pen.y); geometryCtx.rotate(angle);
    geometryCtx.fillStyle = "#c65132";
    geometryCtx.beginPath(); geometryCtx.moveTo(8, 0); geometryCtx.lineTo(-5, -4); geometryCtx.lineTo(-5, 4); geometryCtx.closePath(); geometryCtx.fill();
    geometryCtx.restore();
    geometryCtx.fillStyle = "#536b62"; geometryCtx.font = "10px ui-monospace, monospace";
    geometryCtx.fillText("x", w - 16, cy - 7); geometryCtx.fillText("y", cx + 7, 15);
  }

  function refreshScore() {
    generated = scoreModel.generate(scoreConfig());
    scheduleRevision += 1;
    captureRecordingRevision();
    document.getElementById("bass-vcf-cutoff-value").textContent =
      `${Math.round(generated.config.bassVcfCutoffHz)} Hz`;
    document.getElementById("bass-vcf-resonance-value").textContent =
      Number(generated.config.bassVcfResonance).toFixed(2).replace(".", ",");
    const pendulumMode = generated.config.motionModel === "coupled-pendulum";
    geometryPanel.dataset.mode = pendulumMode ? "pendulum" : "functions";
    pendulumControls.hidden = !pendulumMode;
    document.getElementById("geometry-shape").disabled = pendulumMode;
    const billiardMode = generated.config.geometryShape === "billiard-square";
    document.getElementById("geometry-billiard-control").hidden = pendulumMode || !billiardMode;
    geometryBilliardPatternInput.disabled = pendulumMode;
    document.querySelector(".geometry-heading strong").textContent = pendulumMode
      ? "Plano de fase · θ₁ × θ₂" : "O instrumento traça a curva";
    document.querySelector(".geometry-heading span").textContent = pendulumMode
      ? "02 / MOVIMENTO · FASE" : "02 / DESENHO GEOMÉTRICO";
    document.querySelector(".geometry-discipline").innerHTML = pendulumMode
      ? '<span class="phase-key build">CONSTRUÇÃO · C1–2</span><span class="phase-key change">MUDANÇA · C3–4</span><span class="phase-key pause">EXPANSÃO · C5</span><span class="phase-key return">RETORNO · C6–7</span>'
      : "MATH ART · SOUND DESIGN · MATHEMATICAL SONIFICATION";
    geometryCanvas.setAttribute("aria-label", pendulumMode
      ? "Plano de fase animado: ângulo theta um no eixo horizontal e theta dois no vertical, colorido por seção formal"
      : "Percurso geométrico traçado pelo compasso, lápis ou esquadro");
    document.getElementById("pendulum-length-value").textContent =
      `${Number(pendulumLengthInput.value).toFixed(2).replace(".", ",")} m`;
    document.getElementById("pendulum-coupling-value").textContent =
      Number(pendulumCouplingInput.value).toFixed(2).replace(".", ",");
    document.getElementById("pendulum-damping-value").textContent =
      Number(pendulumDampingInput.value).toFixed(3).replace(".", ",");
    geometrySourceInput.disabled = pendulumMode;
    const fourierMode = generated.config.geometryShape === "fourier-triangle" ||
      generated.config.geometryShape === "fourier-square";
    geometryFourierControl.hidden = pendulumMode || !fourierMode;
    geometryFourierTermsInput.disabled = pendulumMode;
    geometryFourierValue.textContent = `H=${generated.config.geometryFourierTerms} · ${2 * generated.config.geometryFourierTerms + 1} componentes`;
    document.getElementById("reading-math").textContent = pendulumMode
      ? "três equações diferenciais acopladas por vizinhança"
      : "combinação linear de funções";
    document.getElementById("reading-physics").textContent = pendulumMode
      ? `ω₀ = ${generated.config.pendulumTrajectory.omega0.toFixed(2)} rad/s · energia total normalizada globalmente`
      : "influência idealizada entre movimentos";
    document.getElementById("reading-composition").textContent = pendulumMode
      ? "construção · mudança · pausa · retorno"
      : "imitação transformada, encontro e variação";
    document.getElementById("graph-title").textContent = pendulumMode
      ? "Ângulos dos três pêndulos e eventos de cruzamento"
      : "Funções convertidas em eventos sonoros";
    phaseValue.value = `${phaseText(generated.config.phase)} volta`;
    swingValue.value = `${Math.round(generated.config.swing * 100)}%`;
    bpmValue.value = `${generated.config.bpm} BPM`;
    document.getElementById("bass-depth-value").textContent = `${bassDepthInput.value} semitons`;
    document.getElementById("bass-pitch-variation-value").textContent =
      `${Math.round(Number(bassPitchVariationInput.value))}%`;
    document.getElementById("duration-label").textContent = `${generated.duration} s`;
    geometryEquation.textContent = pendulumMode
      ? "x = θ₁(t) · y = θ₂(t) · trajetória calculada nos sete ciclos"
      : geometryEquationText(generated.config.geometryShape, generated.config.geometrySource,
        generated.config.geometryBilliardPattern);
    const quarterNotesPerCycle = generated.config.cycleSeconds * generated.config.bpm / 60;
    document.getElementById("cycle-length").textContent = pendulumMode
      ? `PÊNDULO / 7 CICLOS · ${generated.config.cycleSeconds.toFixed(1).replace(".", ",")} S POR CICLO · ${generated.config.meter}`
      : `TRAÇADO / ${generated.config.cycleSeconds.toFixed(1).replace(".", ",")} S · ${quarterNotesPerCycle.toFixed(1).replace(".", ",")} SEMÍNIMAS/CICLO · ${generated.config.meter}`;
    const sectionDurations = [2, 2, 1, 2].map((cycles) => cycles * generated.config.cycleSeconds);
    ["presentation-time", "transformation-time", "suspension-time", "return-time"]
      .forEach((id, index) => {
        document.getElementById(id).textContent = `${sectionDurations[index].toFixed(0)} s`;
      });
    const partialVoices = scoreModel.activeVoiceCountInCycle(1, generated.config.voiceCount);
    document.getElementById("form-entry-map").textContent =
      `ENTRADAS · C1: V1 + baixo · C2: V1–V${partialVoices} + baixo · C3–5: todas + baixo · C6: V1–V${partialVoices} + baixo · C7: todas + baixo. FASE: avança em C3–4, sustenta em C5 e retorna em C6–7.`;
    [...timeline.children].forEach((segment, cycle) => {
      const activeVoices = scoreModel.activeVoiceCountInCycle(cycle, generated.config.voiceCount);
      const bassText = "e baixo";
      segment.setAttribute("aria-label",
        `Ciclo ${cycle + 1}, ${scoreModel.sectionForCycle(cycle)}, ${activeVoices} voz(es) ${bassText}`);
      segment.title = `C${cycle + 1}: ${activeVoices} voz(es) ${bassText}`;
    });
    updateFormulaSymbols();
    document.getElementById("variation-value").value = `${Math.round(generated.config.variation * 100)}%`;
    const alteredTransforms = generated.config.voiceTransforms.filter((value, voice) =>
      value !== generated.config.voiceTransformBases[voice]).length;
    const alteredCycles = generated.config.voiceCycles.filter((value, voice) =>
      value !== generated.config.voiceCycleBases[voice]).length;
    const alteredBassCells = generated.config.bassSeededPattern.filter((step, index) =>
      step && step.midi !== generated.config.bassPattern[index]?.midi).length;
    document.getElementById("seed-summary").textContent =
      `Seed ${generated.config.seed}: ${generated.config.voiceCount} vozes · ${generated.config.voiceCycles[1]} ciclos em V2 · ${generated.config.recipe} · ${generated.config.patternMode} · ${generated.config.intensityLaw} (${Math.round(generated.config.intensityAmount * 100)}%). ${generated.config.bassVoiceDriven ? "O baixo segue as vozes." : `${alteredTransforms} transformação(ões), ${alteredCycles} ciclo(s) e ${alteredBassCells} altura(s) variam.`}`;
    updateBassDurationDisplay();
    const average = generated.config.couplings.slice(1).reduce((sum, amount) => sum + amount, 0) /
      Math.max(1, generated.config.voiceCount - 1);
    const activeEventRelations = generated.config.voiceEventRelations
      .filter((relation) => relation !== "none").length;
    relationSummary.textContent = pendulumMode
      ? `Três pêndulos pequenos acoplados por molas efetivas: κ = ${Number(pendulumCouplingInput.value).toFixed(2).replace(".", ",")}, ζ = ${Number(pendulumDampingInput.value).toFixed(3).replace(".", ",")}. As vozes 1–3 seguem θᵢ(t); no ciclo 5, a fase se sustenta sem impor silêncio.`
      : `λ médio = ${phaseText(average)}. λ = 0 mantém o acoplamento matemático inativo; aumentar λ transmite mais movimento. ${activeEventRelations} relação(ões) de eventos ativa(s): memória de ½ ciclo; imitação atrasada em ¹⁄₁₆ de ciclo.`;
    if (renderedVoiceCount !== generated.config.voiceCount) {
      renderFormulaCards(generated.config.voiceCount);
      renderMixerChannels(generated.config.voiceCount);
      renderGraphLegend(generated.config.voiceCount);
      renderedVoiceCount = generated.config.voiceCount;
      updateFormulaSymbols();
    }
    for (let voice = 1; voice < generated.config.voiceCount; voice += 1) {
      const relationSelect = document.getElementById(`event-relation-${voice}`);
      if (relationSelect)
        relationSelect.value = generated.config.voiceEventRelations[voice];
    }
    if (running) {
      const cutoff = elapsed() + 0.13;
      nextEvent = scoreModel.eventCursorAtOrAfter(generated.events, cutoff);
      status.textContent = "Nova regra aplicada aos próximos eventos.";
    }
    draw(running ? elapsed() : 0);
    drawBassEditor();
  }

  function recordingConfigSnapshot() {
    const config = scoreConfig();
    return {
      ...config,
      mixer: mixerSnapshot(),
      voiceCycleBases: generated.config.voiceCycleBases,
      couplingBases: generated.config.couplingBases,
      voiceTransformBases: generated.config.voiceTransformBases,
      voiceCycles: generated.config.voiceCycles,
      couplings: generated.config.couplings,
      voiceTransforms: generated.config.voiceTransforms,
      voicePhaseOffsets: generated.config.voicePhaseOffsets
    };
  }

  function captureRecordingRevision() {
    if (!mediaRecorder || mediaRecorder.state !== "recording" || !generated) return;
    const config = recordingConfigSnapshot();
    const signature = JSON.stringify(config);
    if (signature === recordingRevisionSignature) return;
    if (recordingScoreRevisions.length >= 512) {
      recordingRevisionTruncated = true;
      return;
    }
    recordingRevisionSignature = signature;
    recordingScoreRevisions.push({
      effective_at_seconds: Math.max(0, Number(((performance.now() - recordingStartedAt) / 1000 + 0.13).toFixed(3))),
      configuration: config,
      mathematical_partitura: mathematicalPartitura(config)
    });
  }

  function recordedEventPlan() {
    return generated.events.map((event) => ({
      time_seconds: Number(event.time.toFixed(6)),
      cycle: event.cycle + 1,
      section: event.section,
      voice: event.voice === BASS_VOICE ? "bass" : event.voice + 1,
      position: Number(event.position.toFixed(6)),
      pitch_midi: Number.isFinite(event.pitch) ? event.pitch : null,
      duration_seconds: Number.isFinite(event.durationSeconds) ? Number(event.durationSeconds.toFixed(6)) : null,
      source_voice: Number.isInteger(event.sourceVoice) ? event.sourceVoice + 1 : null,
      source_x: Number.isFinite(event.sourceX) ? Number(event.sourceX.toFixed(6)) : null,
      source_y: Number.isFinite(event.sourceY) ? Number(event.sourceY.toFixed(6)) : null,
      bass_source_x: Number.isFinite(event.bassSourceX) ? Number(event.bassSourceX.toFixed(6)) : null,
      bass_source_y: Number.isFinite(event.bassSourceY) ? Number(event.bassSourceY.toFixed(6)) : null,
      bass_source_intensity: Number.isFinite(event.bassSourceIntensity)
        ? Number(event.bassSourceIntensity.toFixed(6)) : null,
      intensity: Number(event.intensity.toFixed(6)),
      physical_energy: Number.isFinite(event.physicalEnergy) ? Number(event.physicalEnergy.toFixed(6)) : null,
      recipe: event.recipe,
      articulation: event.intent || null,
      repeats: event.repeats || 1
    }));
  }

  function makeRecordingScore() {
    return {
      format: "batruc-recording-score",
      schema_version: 1,
      created_at: new Date().toISOString(),
      audio_file: recordingAudioFile,
      recording: {
        captured_from_seconds: Number(recordingStartSeconds.toFixed(3)),
        captured_duration_seconds: Number(Math.max(0, (performance.now() - recordingStartedAt) / 1000).toFixed(3)),
        configuration_revision_limit: 512,
        configuration_revisions_truncated: recordingRevisionTruncated
      },
      initial_configuration: recordingScoreRevisions[0]?.configuration || recordingConfigSnapshot(),
      initial_mathematical_partitura: recordingScoreRevisions[0]?.mathematical_partitura ||
        mathematicalPartitura(recordingScoreRevisions[0]?.configuration || recordingConfigSnapshot()),
      initial_event_plan: recordingInitialEvents,
      configuration_revisions: recordingScoreRevisions,
      notes: [
        "Arquivo TXT UTF-8 com JSON versionado para leitura humana e reconstrução computacional; salvo junto do áudio na pasta escolhida ou disponibilizado para download.",
        "O plano inicial descreve a agenda completa gerada no início da captura; revisões registram mudanças posteriores e o instante aproximado em que passam a valer.",
        "A gravação web é comprimida (Opus), não um master PCM/WAV sem perdas. Todo processamento e gravação de arquivo ocorrem localmente."
      ]
    };
  }

  function clearScoreDownload() {
    if (scoreDownloadUrl) URL.revokeObjectURL(scoreDownloadUrl);
    scoreDownloadUrl = null;
    scoreDownload.removeAttribute("href");
    scoreDownload.hidden = true;
  }

  function updateBassDurationDisplay() {
    const mode = bassDurationFormulaInput.value;
    const ratio = Number(bassGeoRatioInput.value);
    const stepsPerBar = bassStepsPerBar();
    const equation = document.getElementById("bass-duration-equation");
    if (mode === "fibonacci") {
      equation.textContent = `Fibonacci: dₙ = min(${stepsPerBar}, F₍ₙ mod 8₎₊₁) semicolcheias.`;
    } else if (mode === "geometric") {
      equation.textContent = `Geométrica: dₙ = min(${stepsPerBar}, arred.(${ratio.toString().replace(".", ",")}⁽ⁿ mod 8⁾)) semicolcheias.`;
    } else {
      equation.textContent = "Manual: dₙ = duração escolhida no passo.";
    }
    const active = bassPattern.map((note, index) => note ? index : -1).filter((index) => index >= 0);
    const values = active.slice(0, 8).map((stepIndex, ordinal) => Math.min(
      scoreModel.bassDurationStepsFor(ordinal, bassPattern[stepIndex].durationSteps, generated.config),
      stepsPerBar - stepIndex));
    equation.textContent += ` Aplicação no compasso: ${values.join(" · ")} semicolcheias.`;
    document.getElementById("bass-editor-hint").textContent = bassVoiceDriven
      ? "O gráfico mostra o baixo calculado dos ataques vocais no início da seção Mudança. A grade abaixo é um molde de edição: ao editar uma célula, o baixo passa ao modo manual."
      : mode === "manual"
        ? "Clique em passo vazio para inserir; arraste o corpo para mudar altura e a borda direita para mudar duração. No modo de desenho, o traço cria ou altera os passos atravessados."
        : "A duração é calculada pela lei selecionada; arrastar uma nota altera apenas a altura. No modo de desenho, o traço cria ou altera os passos atravessados.";
    bassPatternGrid.querySelectorAll(".bass-step").forEach((cell, index) => {
      const duration = cell.querySelectorAll("select")[1];
      duration.disabled = mode !== "manual";
      const seededPitch = generated.config.bassSeededPattern[index]?.midi;
      const basePitch = bassPattern[index]?.midi;
      const readout = cell.querySelector(".bass-step-seed");
      readout.textContent = bassVoiceDriven ? "voz → baixo" : seededPitch === undefined ? "—" :
        (seededPitch === basePitch ? `seed ${midiName(seededPitch)}` : `seed → ${midiName(seededPitch)}`);
      readout.title = bassVoiceDriven ? "O áudio usa uma linha calculada pelos eventos vocais; a grade é um molde para edição manual." : seededPitch === undefined ? "Pausa" :
        `Altura derivada da seed: ${midiName(seededPitch)}${seededPitch === basePitch ? " (igual à escrita)" : `; altura escrita ${midiName(basePitch)}`}`;
    });
    bassGeoRatioInput.disabled = mode !== "geometric";
  }

  function midiName(midi) {
    const names = ["C", "C♯", "D", "D♯", "E", "F", "F♯", "G", "G♯", "A", "A♯", "B"];
    return `${names[midi % 12]}${Math.floor(midi / 12) - 1}`;
  }

  function bassStepsPerBar() {
    const [numerator, denominator] = meterInput.value.split("/").map(Number);
    return Math.round(numerator * 16 / denominator);
  }

  function renderBassPatternControls() {
    bassPatternGrid.replaceChildren();
    const stepCount = bassStepsPerBar();
    document.getElementById("bass-step-count").textContent = `${stepCount} passos por compasso`;
    bassPatternGrid.style.setProperty("--bass-step-count", stepCount);
    for (let stepIndex = 0; stepIndex < stepCount; stepIndex += 1) {
      const cell = document.createElement("div");
      cell.className = "bass-step";
      const pitch = document.createElement("select");
      pitch.setAttribute("aria-label", `Nota do passo ${stepIndex + 1}`);
      pitch.add(new Option("Pausa", "rest"));
      for (let midi = 60; midi >= 24; midi -= 1) pitch.add(new Option(midiName(midi), String(midi)));
      pitch.value = bassPattern[stepIndex] ? String(bassPattern[stepIndex].midi) : "rest";
      const duration = document.createElement("select");
      duration.setAttribute("aria-label", `Duração do passo ${stepIndex + 1}`);
      for (const [steps, label] of durationOptions) duration.add(new Option(label, String(steps)));
      duration.value = String(bassPattern[stepIndex]?.durationSteps || 2);
      const repeats = document.createElement("select");
      repeats.setAttribute("aria-label", `Repetições do passo ${stepIndex + 1}`);
      for (const amount of [1, 2, 4, 8]) repeats.add(new Option(`×${amount}`, String(amount)));
      repeats.value = String(bassPattern[stepIndex]?.repeats || 1);
      const intent = document.createElement("select");
      intent.setAttribute("aria-label", `Intenção do passo ${stepIndex + 1}`);
      intent.add(new Option("Normal", "normal"));
      intent.add(new Option("Ghost", "ghost"));
      intent.add(new Option("Acento", "accent"));
      intent.value = bassPattern[stepIndex]?.intent || "normal";
      const number = document.createElement("span");
      number.className = "bass-step-number";
      number.textContent = String(stepIndex + 1).padStart(2, "0");
      const seededPitch = document.createElement("output");
      seededPitch.className = "bass-step-seed";
      seededPitch.setAttribute("aria-label", `Altura derivada da seed no passo ${stepIndex + 1}`);
      pitch.addEventListener("change", () => {
        bassVoiceDriven = false;
        bassPattern[stepIndex] = pitch.value === "rest" ? null :
          { midi: Number(pitch.value), durationSteps: Number(duration.value),
            repeats: Number(repeats.value), intent: intent.value };
        drawBassEditor(); refreshScore();
      });
      duration.addEventListener("change", () => {
        bassVoiceDriven = false;
        if (bassDurationFormulaInput.value === "manual" && bassPattern[stepIndex])
          bassPattern[stepIndex].durationSteps = Number(duration.value);
        drawBassEditor(); refreshScore();
      });
      repeats.addEventListener("change", () => {
        bassVoiceDriven = false;
        if (bassPattern[stepIndex]) bassPattern[stepIndex].repeats = Number(repeats.value);
        drawBassEditor(); refreshScore();
      });
      intent.addEventListener("change", () => {
        bassVoiceDriven = false;
        if (bassPattern[stepIndex]) bassPattern[stepIndex].intent = intent.value;
        drawBassEditor(); refreshScore();
      });
      duration.disabled = bassDurationFormulaInput.value !== "manual";
      cell.append(number, pitch, seededPitch, duration, repeats, intent);
      bassPatternGrid.appendChild(cell);
    }
    if (generated) updateBassDurationDisplay();
  }

  function drawBassEditor() {
    const bounds = bassEditor.getBoundingClientRect();
    const dpr = Math.min(2, globalThis.devicePixelRatio || 1);
    const width = Math.max(1, Math.round(bounds.width * dpr));
    const height = Math.max(1, Math.round(bounds.height * dpr));
    if (bassEditor.width !== width || bassEditor.height !== height) {
      bassEditor.width = width; bassEditor.height = height;
    }
    bassCtx.setTransform(dpr, 0, 0, dpr, 0, 0);
    const w = bounds.width, h = bounds.height, left = 42, right = 10, top = 12, bottom = 12;
    const stepCount = bassStepsPerBar();
    const plotW = w - left - right, plotH = h - top - bottom, stepW = plotW / stepCount;
    const yForMidi = (midi) => top + (60 - midi) / 36 * plotH;
    const visibleNotes = bassVoiceDriven && generated
      ? Array.from({ length: stepCount }, () => null)
      : bassPattern.slice(0, stepCount);
    if (bassVoiceDriven && generated) {
      const stepSeconds = 15 / generated.config.bpm;
      const firstBarEnd = stepCount * stepSeconds / generated.config.cycleSeconds;
      for (const event of generated.events) {
        if (event.voice !== BASS_VOICE || event.cycle !== 1
            || event.localProgress >= firstBarEnd) continue;
        const step = Math.min(stepCount - 1,
          Math.floor(event.localProgress * generated.config.cycleSeconds / stepSeconds));
        visibleNotes[step] = { midi: event.pitch,
          durationSteps: event.durationSteps, repeats: event.repeats,
          intent: event.intent || "normal" };
      }
    }
    bassCtx.clearRect(0, 0, w, h);
    bassCtx.fillStyle = "#f1f1e8"; bassCtx.fillRect(0, 0, w, h);
    for (let midi = 24; midi <= 60; midi += 1) {
      const y = yForMidi(midi);
      bassCtx.strokeStyle = midi % 12 === 0 ? "#9aada1" : midi % 3 === 0 ? "#d4ddd4" : "#e5e9e1";
      bassCtx.lineWidth = midi % 12 === 0 ? 1.2 : 0.7;
      bassCtx.beginPath(); bassCtx.moveTo(left, y); bassCtx.lineTo(w - right, y); bassCtx.stroke();
      if (midi % 12 === 0) {
        bassCtx.fillStyle = "#536b62"; bassCtx.font = "10px ui-monospace, monospace";
        bassCtx.fillText(midiName(midi), 4, y + 3);
      }
    }
    for (let step = 0; step <= stepCount; step += 1) {
      const x = left + step * stepW;
      bassCtx.strokeStyle = step % 4 === 0 ? "#8aa096" : "#d4ddd4";
      bassCtx.lineWidth = step % 4 === 0 ? 1.2 : 0.7;
      bassCtx.beginPath(); bassCtx.moveTo(x, top); bassCtx.lineTo(x, h - bottom); bassCtx.stroke();
    }
    bassCtx.strokeStyle = "#96aaa0"; bassCtx.lineWidth = 2;
    let pathActive = false;
    visibleNotes.forEach((note, step) => {
      if (!note) { pathActive = false; return; }
      const x = left + (step + 0.5) * stepW;
      const soundingMidi = bassVoiceDriven ? note.midi
        : generated?.config?.bassSeededPattern?.[step]?.midi ?? note.midi;
      const y = yForMidi(soundingMidi);
      if (pathActive) bassCtx.lineTo(x, y);
      else { bassCtx.beginPath(); bassCtx.moveTo(x, y); pathActive = true; }
      bassCtx.stroke();
    });
    let noteOrdinal = 0;
    visibleNotes.forEach((note, step) => {
      if (!note) return;
      const x = left + step * stepW + 2;
      const soundingMidi = bassVoiceDriven ? note.midi
        : generated?.config?.bassSeededPattern?.[step]?.midi ?? note.midi;
      const y = yForMidi(soundingMidi);
      const writtenY = yForMidi(note.midi);
      const barH = Math.max(9, plotH / 36 * 0.72);
      const formulaDuration = generated ? scoreModel.bassDurationStepsFor(noteOrdinal,
        note.durationSteps, generated.config) : note.durationSteps;
      const visiblePattern = visibleNotes;
      const laterActive = visiblePattern.findIndex((next, index) => index > step && next);
      const firstActive = visiblePattern.findIndex(Boolean);
      const gap = laterActive >= 0 ? laterActive - step : firstActive >= 0 ? stepCount - step + firstActive : stepCount;
      const renderedDuration = Math.min(formulaDuration, stepCount - step, gap);
      const barW = Math.max(4, Math.min(stepCount - step, renderedDuration) * stepW - 4);
      if (soundingMidi !== note.midi) {
        bassCtx.strokeStyle = "#167d77"; bassCtx.lineWidth = 1.4;
        bassCtx.beginPath(); bassCtx.moveTo(x + 4, writtenY); bassCtx.lineTo(x + 4, y); bassCtx.stroke();
        bassCtx.fillStyle = "#f1f1e8"; bassCtx.strokeStyle = "#203c38"; bassCtx.lineWidth = 1.5;
        bassCtx.beginPath(); bassCtx.arc(x + 4, writtenY, 3, 0, Math.PI * 2); bassCtx.fill(); bassCtx.stroke();
      }
      bassCtx.fillStyle = note.intent === "ghost" ? "#78948a" : note.intent === "accent" ? "#c65132" : "#203c38";
      bassCtx.fillRect(x, y - barH / 2, barW, barH);
      if ((note.repeats || 1) > 1) {
        bassCtx.strokeStyle = "#f8f6ec"; bassCtx.lineWidth = 1;
        for (let repeat = 1; repeat < note.repeats; repeat += 1) {
          const tickX = x + barW * repeat / note.repeats;
          bassCtx.beginPath(); bassCtx.moveTo(tickX, y - barH / 2); bassCtx.lineTo(tickX, y + barH / 2); bassCtx.stroke();
        }
      }
      bassCtx.fillStyle = "#fff"; bassCtx.font = "9px ui-monospace, monospace";
      bassCtx.fillText(soundingMidi === note.midi
        ? midiName(soundingMidi) : `${midiName(note.midi)}>${midiName(soundingMidi)}`, x + 3, y + 3);
      noteOrdinal += 1;
    });
  }

  function bassEditorPoint(event) {
    const bounds = bassEditor.getBoundingClientRect();
    const x = event.clientX - bounds.left, y = event.clientY - bounds.top;
    const left = 42, right = 10, top = 12, bottom = 12;
    const plotW = bounds.width - left - right, plotH = bounds.height - top - bottom;
    const stepCount = bassStepsPerBar();
    const stepW = plotW / stepCount;
    const step = Math.max(0, Math.min(stepCount - 1, Math.floor((x - left) / stepW)));
    const midi = Math.max(24, Math.min(60, Math.round(60 - (y - top) / plotH * 36)));
    return { step, midi, x, stepW, left };
  }

  function updateBassEditorDrag(event) {
    if (!bassDrag || bassDrag.pointerId !== event.pointerId) return;
    const point = bassEditorPoint(event);
    if (bassDrag.mode === "paint") {
      const direction = Math.sign(point.step - bassDrag.lastStep);
      const distance = Math.abs(point.step - bassDrag.lastStep);
      for (let offset = 1; offset <= distance; offset += 1) {
        const stepIndex = bassDrag.lastStep + direction * offset;
        const amount = offset / distance;
        const midi = Math.round(bassDrag.lastMidi + (point.midi - bassDrag.lastMidi) * amount);
        if (!bassPattern[stepIndex]) bassPattern[stepIndex] =
          { midi, durationSteps: 1, repeats: 1, intent: "normal" };
        else bassPattern[stepIndex].midi = midi;
      }
      bassDrag.lastStep = point.step;
      bassDrag.lastMidi = point.midi;
      bassDrag.moved = true;
      drawBassEditor(); renderBassPatternControls(); refreshScore();
      return;
    }
    const note = bassPattern[bassDrag.step];
    if (!note) return;
    if (bassDrag.mode === "resize") {
        const steps = Math.max(1, Math.min(bassStepsPerBar() - bassDrag.step,
        Math.round((point.x - bassDrag.left) / point.stepW) - bassDrag.step));
      note.durationSteps = durationOptions.reduce((best, option) =>
        Math.abs(option[0] - steps) < Math.abs(best - steps) ? option[0] : best, 1);
    } else {
      note.midi = point.midi;
    }
    const selects = bassPatternGrid.children[bassDrag.step].querySelectorAll("select");
    selects[0].value = String(note.midi);
    selects[1].value = String(note.durationSteps);
    bassDrag.moved = true;
    drawBassEditor(); refreshScore();
  }

  bassEditor.addEventListener("pointerdown", (event) => {
    bassVoiceDriven = false;
    const point = bassEditorPoint(event);
    if (bassDrawMode) {
      if (!bassPattern[point.step]) bassPattern[point.step] =
        { midi: point.midi, durationSteps: 1, repeats: 1, intent: "normal" };
      else bassPattern[point.step].midi = point.midi;
      bassDrag = { pointerId: event.pointerId, mode: "paint", lastStep: point.step,
        lastMidi: point.midi, moved: true };
      bassEditor.setPointerCapture(event.pointerId);
      drawBassEditor(); renderBassPatternControls(); refreshScore();
      return;
    }
    const existing = bassPattern[point.step];
    if (!existing) {
      bassPattern[point.step] = { midi: point.midi, durationSteps: 2, repeats: 1, intent: "normal" };
      bassDrag = { pointerId: event.pointerId, step: point.step, mode: "pitch", moved: false, created: true,
        initialX: point.x, left: point.left, stepW: point.stepW };
    } else {
      const noteEnd = point.left + (point.step + existing.durationSteps) * point.stepW;
      const mode = bassDurationFormulaInput.value === "manual"
        && Math.abs(point.x - noteEnd) < point.stepW * 0.8 ? "resize" : "pitch";
      bassDrag = { pointerId: event.pointerId, step: point.step, mode, moved: false,
        initialMidi: existing.midi, initialDuration: existing.durationSteps,
        initialX: point.x, left: point.left, stepW: point.stepW };
    }
    bassEditor.setPointerCapture(event.pointerId);
    drawBassEditor(); renderBassPatternControls(); refreshScore();
  });
  bassEditor.addEventListener("pointermove", updateBassEditorDrag);
  const endBassEditorDrag = (event) => {
    if (!bassDrag || bassDrag.pointerId !== event.pointerId) return;
    const { step, moved, created, initialMidi, initialDuration } = bassDrag;
    if (!moved && !created) bassPattern[step] = null;
    bassDrag = null;
    drawBassEditor(); renderBassPatternControls(); refreshScore();
  };
  bassEditor.addEventListener("pointerup", endBassEditorDrag);
  bassEditor.addEventListener("pointercancel", endBassEditorDrag);
  bassDrawToggle.addEventListener("click", () => {
    bassDrawMode = !bassDrawMode;
    bassDrawToggle.setAttribute("aria-pressed", String(bassDrawMode));
    bassDrawToggle.textContent = bassDrawMode ? "✎ DESENHANDO · CLIQUE PARA SAIR" : "✎ DESENHAR TRAÇO";
    bassEditor.classList.toggle("drawing-mode", bassDrawMode);
  });

  function renderFormulaCards(count) {
    formulaGrid.replaceChildren();
    for (let voice = 0; voice < count; voice += 1) {
      const number = voice + 1;
      const card = document.createElement("article");
      card.className = "formula-card";
      card.style.setProperty("--voice-color", colors[voice]);
      const oscillations = voiceCycles[voice];
      const equation = oscillatorEquationHtml(voice, oscillations);
      const couplingControl = voice === 0 ?
        `<div class="equation-control source">voz de referência · sem mistura anterior</div>` :
        `<label class="equation-control">λ<sub>${number}</sub><input id="coupling-${voice}" type="range" min="0" max="0.8" step="0.01" value="${couplings[voice]}"><output id="coupling-value-${voice}">${phaseText(couplings[voice])}</output></label>`;
      const relationOptions = ["none", "support", "imitation", "avoidance", "competition"];
      const relationNames = { none: "Independente", support: "Sustentar", imitation: "Imitar", avoidance: "Afastar", competition: "Competir" };
      const relationControl = voice === 0
        ? `<label class="equation-control source">V1 · início da cadeia relacional</label>`
        : `<label class="event-relation-control" for="event-relation-${voice}"><span>Relação ecológica</span><select id="event-relation-${voice}" title="V${number} responde aos eventos da voz anterior">${relationOptions.map((value) => `<option value="${value}"${voiceEventRelations[voice] === value ? " selected" : ""}>${relationNames[value]}</option>`).join("")}</select></label>`;
      const transformOptions = Object.entries(transformNames).map(([value, label]) =>
        `<option value="${value}"${voiceTransforms[voice] === value ? " selected" : ""}>${label}</option>`).join("");
      card.innerHTML = `<div class="formula-top"><span class="voice-tag">${String(number).padStart(2, "0")} / VOZ</span><span><span id="cycle-label-${voice}">${oscillations}</span> oscilações por ciclo</span><span class="voice-entry" id="voice-entry-${voice}">ATIVA</span></div>` +
        `<div class="equation" id="voice-equation-${voice}">${equation}</div>` +
        `<div class="formula-controls"><label class="equation-control">n<sub>${number}</sub><input id="cycles-${voice}" type="range" min="0.5" max="16" step="0.25" value="${oscillations}"><output id="cycles-value-${voice}">${oscillations}</output></label>${couplingControl}</div>` +
        `<div class="formula-transform-control"><label for="voice-transform-${voice}">Transformação-base · voz ${number}</label><select id="voice-transform-${voice}">${transformOptions}</select><small id="transform-equation-${voice}"></small></div>` +
        `<div class="formula-transform-control">${relationControl}<small>λ controla a força; memória = ½ ciclo e atraso de imitação = ¹⁄₁₆ ciclo.</small></div>` +
        `<div class="formula-foot"><span>valor x<sub>${number}</sub> <strong id="value-${voice}">+0,000</strong></span><span class="dot"></span></div>`;
      formulaGrid.appendChild(card);
      card.querySelector(`#cycles-${voice}`).addEventListener("input", (event) => {
        freezeSeedVariationForManualEdit();
        voiceCycles[voice] = Number(event.target.value);
        document.getElementById(`cycles-value-${voice}`).textContent = voiceCycles[voice];
        refreshScore();
      });
      if (voice > 0) {
        card.querySelector(`#coupling-${voice}`).addEventListener("input", (event) => {
          freezeSeedVariationForManualEdit();
          couplings[voice] = Number(event.target.value);
          document.getElementById(`coupling-value-${voice}`).textContent = phaseText(couplings[voice]);
          refreshScore();
        });
      }
      card.querySelector(`#voice-transform-${voice}`).addEventListener("change", (event) => {
        freezeSeedVariationForManualEdit();
        voiceTransforms[voice] = event.target.value;
        refreshScore();
      });
      if (voice > 0) {
        card.querySelector(`#event-relation-${voice}`).addEventListener("change", (event) => {
          freezeSeedVariationForManualEdit();
          voiceEventRelations[voice] = event.target.value;
          refreshScore();
        });
      }
    }
  }

  function oscillatorEquationHtml(voice, oscillations) {
    const number = voice + 1;
    return voice === 0 ?
      `y<sub>1</sub>(u) = T<sub>1</sub>(<span id="voice-source-${voice}">sen(2π · (<span id="cycle-symbol-${voice}">${oscillations}</span>u))</span>)` :
      `y<sub>${number}</sub>(u) = (1−λ<sub>${number}</sub>) · T<sub>${number}</sub>(sen(2π · (<span id="cycle-symbol-${voice}">${oscillations}</span>u + <span id="formula-phase-${voice}">0,000</span>))) + λ<sub>${number}</sub> · y<sub>${number - 1}</sub>(u)`;
  }

  function pendulumEquationHtml(voice) {
    const formulas = [
      "θ̈₁ + 2ζω₀θ̇₁ + ω₀²θ₁ + κω₀²(θ₁−θ₂) = 0",
      "θ̈₂ + 2ζω₀θ̇₂ + ω₀²θ₂ + κω₀²(2θ₂−θ₁−θ₃) = 0",
      "θ̈₃ + 2ζω₀θ̇₃ + ω₀²θ₃ + κω₀²(θ₃−θ₂) = 0"
    ];
    return formulas[voice];
  }

  function updateFormulaSymbols() {
    for (let voice = 0; voice < generated.config.voiceCount; voice += 1) {
      const cycleLabel = document.getElementById(`cycle-label-${voice}`);
      const cycleSymbol = document.getElementById(`cycle-symbol-${voice}`);
      const couplingValue = document.getElementById(`coupling-value-${voice}`);
      const transformEquation = document.getElementById(`transform-equation-${voice}`);
      if (cycleLabel) cycleLabel.textContent = generated.config.voiceCycles[voice];
      if (cycleSymbol) cycleSymbol.textContent = generated.config.voiceCycles[voice];
      const cycleSliderValue = document.getElementById(`cycles-value-${voice}`);
      const physicalVoice = generated.config.motionModel === "coupled-pendulum" && voice < 3;
      if (cycleSliderValue) cycleSliderValue.textContent = generated.config.voiceCycleBases[voice];
      if (couplingValue) couplingValue.textContent = phaseText(generated.config.couplings[voice]);
      const transformSelect = document.getElementById(`voice-transform-${voice}`);
      if (transformSelect) transformSelect.value = generated.config.voiceTransformBases[voice];
      if (transformEquation) transformEquation.textContent = `Aplicada: ${transformNames[generated.config.voiceTransforms[voice]]} · ${transformEquations[generated.config.voiceTransforms[voice]]}`;
      const card = formulaGrid.querySelector(`.formula-card:nth-child(${voice + 1})`);
      const equation = document.getElementById(`voice-equation-${voice}`);
      if (card) {
        card.dataset.model = physicalVoice ? "pendulum" : "functions";
        card.querySelector(".formula-controls").hidden = physicalVoice;
        card.querySelector(".formula-transform-control").hidden = physicalVoice;
        const label = card.querySelector(".formula-top span:nth-child(2)");
        if (label) label.innerHTML = physicalVoice ? "pêndulo · θᵢ(t)" :
          `<span id="cycle-label-${voice}">${generated.config.voiceCycles[voice]}</span> oscilações por ciclo`;
      }
      if (equation) {
        equation.classList.toggle("physics-equation", physicalVoice);
        equation.innerHTML = physicalVoice
          ? pendulumEquationHtml(voice)
          : oscillatorEquationHtml(voice, generated.config.voiceCycles[voice]);
      }
      const cycleControl = document.getElementById(`cycles-${voice}`);
      const couplingControl = document.getElementById(`coupling-${voice}`);
      const transformControl = document.getElementById(`voice-transform-${voice}`);
      if (cycleControl) cycleControl.value = String(generated.config.voiceCycleBases[voice]);
      if (couplingControl) couplingControl.value = String(generated.config.couplings[voice]);
      if (cycleControl) cycleControl.disabled = physicalVoice;
      if (couplingControl) couplingControl.disabled = physicalVoice;
      if (transformControl) transformControl.disabled = physicalVoice;
      const sourceSymbol = document.getElementById(`voice-source-${voice}`);
      if (sourceSymbol && voice === 0) {
        sourceSymbol.innerHTML = generated.config.geometrySource ?
          `G<sub>${generated.config.geometryShape}</sub>(u; n=<span id="cycle-symbol-${voice}">${generated.config.voiceCycles[voice]}</span>)` :
          `sen(2π · (<span id="cycle-symbol-${voice}">${generated.config.voiceCycles[voice]}</span>u))`;
      }
    }
  }

  function formatPan(value) {
    if (Math.abs(value) < 0.025) return "C";
    return `${value < 0 ? "L" : "R"}${Math.round(Math.abs(value) * 100)}`;
  }

  function channelIsAudible(voice) {
    if (mutedVoices.has(voice)) return false;
    return soloVoices.size === 0 || soloVoices.has(voice);
  }

  function ensureChannelBuses(count) {
    if (!audio || !master) return;
    while (channelBuses.length < count) {
      const gain = audio.createGain();
      const pan = audio.createStereoPanner();
      const analyser = audio.createAnalyser();
      analyser.fftSize = 2048;
      if (channelBuses.length === BASS_VOICE) {
        bassAnalyser = analyser;
        gain.connect(analyser).connect(pan).connect(
          offlineRenderStem ? audio.destination : master);
      } else {
        gain.connect(pan).connect(formulaBus);
      }
      channelBuses.push({ gain, pan, analyser });
    }
  }

  function applyMixer() {
    if (!audio || !master) return;
    ensureChannelBuses(BASS_VOICE + 1);
    const formulaGain = Math.pow(10, Number(formulaBusTrimInput.value) / 20);
    channelBuses.forEach((bus, voice) => {
      const active = voice < generated.config.voiceCount || voice === BASS_VOICE;
      bus.gain.gain.setTargetAtTime(active && channelIsAudible(voice)
        ? channelVolumes[voice] : 0,
        audio.currentTime, 0.012);
      bus.pan.pan.setTargetAtTime(active ? channelPans[voice] : 0, audio.currentTime, 0.012);
    });
    if (formulaBus)
      formulaBus.gain.setTargetAtTime(formulaGain, audio.currentTime, 0.012);
  }

  function renderMixerChannels(count) {
    channelMixer.replaceChildren();
    for (const voice of [...Array.from({ length: count }, (_, index) => index), BASS_VOICE]) {
      if (!manuallyPannedVoices.has(voice)) {
        channelPans[voice] = voice === BASS_VOICE ? 0 : (count < 2 ? 0 : -0.65 + 1.3 * voice / (count - 1));
      }
      const strip = document.createElement("article");
      strip.className = "channel-strip";
      const color = voice === BASS_VOICE ? "#203c38" : colors[voice];
      const label = voice === BASS_VOICE ? "BAIXO" : `V${String(voice + 1).padStart(2, "0")}`;
      strip.style.setProperty("--voice-color", color);
      strip.innerHTML = `<div class="channel-top"><strong>${label}</strong><span>CANAL ${String(voice + 1).padStart(2, "0")}</span></div>` +
        `<label class="channel-fader"><span>VOL</span><input id="channel-volume-${voice}" type="range" min="0" max="1.5" step="0.01" value="${channelVolumes[voice]}"><output id="channel-volume-value-${voice}">${Math.round(channelVolumes[voice] * 100)}%</output></label>` +
        `<label class="channel-pan"><span>PAN</span><input id="channel-pan-${voice}" type="range" min="-1" max="1" step="0.01" value="${channelPans[voice]}"><output id="channel-pan-value-${voice}">${formatPan(channelPans[voice])}</output></label>` +
        `<div class="channel-buttons"><button id="channel-mute-${voice}" type="button" aria-pressed="${mutedVoices.has(voice)}" aria-label="Mute ${label}">M</button><button id="channel-solo-${voice}" type="button" aria-pressed="${soloVoices.has(voice)}" aria-label="Solo ${label}">S</button></div>`;
      channelMixer.appendChild(strip);
      strip.querySelector(`#channel-volume-${voice}`).addEventListener("input", (event) => {
        channelVolumes[voice] = Number(event.target.value);
        strip.querySelector(`#channel-volume-value-${voice}`).textContent =
          `${Math.round(channelVolumes[voice] * 100)}%`;
        applyMixer();
      });
      strip.querySelector(`#channel-pan-${voice}`).addEventListener("input", (event) => {
        channelPans[voice] = Number(event.target.value);
        manuallyPannedVoices.add(voice);
        strip.querySelector(`#channel-pan-value-${voice}`).textContent = formatPan(channelPans[voice]);
        applyMixer();
      });
      strip.querySelector(`#channel-mute-${voice}`).addEventListener("click", () => {
        if (mutedVoices.has(voice)) mutedVoices.delete(voice); else mutedVoices.add(voice);
        renderMixerChannels(generated.config.voiceCount);
        status.textContent = mutedVoices.has(voice) ? `${label} em mute.` : `Mute retirado de ${label}.`;
      });
      strip.querySelector(`#channel-solo-${voice}`).addEventListener("click", () => {
        if (soloVoices.has(voice)) soloVoices.delete(voice); else soloVoices.add(voice);
        renderMixerChannels(generated.config.voiceCount);
        status.textContent = soloVoices.size ? `Solo ativo em ${soloVoices.size} canal(is).` : "Solo desligado; todos os canais não silenciados voltaram.";
      });
    }
    applyMixer();
  }

  function renderGraphLegend(count) {
    graphLegend.querySelectorAll(".voice-key").forEach((node) => node.remove());
    graphLegend.querySelectorAll(".bass-key").forEach((node) => node.remove());
    const note = graphLegend.querySelector(".graph-note");
    for (let voice = 0; voice < count; voice += 1) {
      const key = document.createElement("span");
      key.className = "voice-key";
      key.innerHTML = `<i class="swatch" style="background:${colors[voice]}"></i>voz ${voice + 1}`;
      graphLegend.insertBefore(key, note);
    }
    const bassKey = document.createElement("span");
    bassKey.className = "voice-key bass-key";
    bassKey.innerHTML = `<i class="swatch" style="background:#203c38"></i>baixo / LFO`;
    graphLegend.insertBefore(bassKey, note);
  }

  function amplitude(cycle, u) {
    const config = generated.config;
    const progress = (cycle + u) / 7;
    const seconds = (cycle + u - sectionStarts[cycle]) * config.cycleSeconds;
    return Math.exp(-0.065 * seconds) * scoreModel.expressionAt(progress, config);
  }

  function fitCanvas() {
    const bounds = graph.getBoundingClientRect();
    const dpr = Math.min(2, globalThis.devicePixelRatio || 1);
    const width = Math.max(1, Math.round(bounds.width * dpr));
    const height = Math.max(1, Math.round(bounds.height * dpr));
    if (graph.width !== width || graph.height !== height) {
      graph.width = width;
      graph.height = height;
    }
    ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
    return { width: bounds.width, height: bounds.height };
  }

  function draw(seconds) {
    const voiceCount = generated.config.voiceCount;
    const laneCount = voiceCount + 1;
    const compactGraph = globalThis.innerWidth < 700;
    graph.style.height = `${Math.max(compactGraph ? 360 : 420,
      laneCount * (compactGraph ? 32 : 43))}px`;
    const { width, height } = fitCanvas();
    ctx.clearRect(0, 0, width, height);
    const cycleSeconds = generated.config.cycleSeconds;
    const cycle = Math.min(6, Math.floor(seconds / cycleSeconds));
    const u = seconds >= generated.duration ? 1 : (seconds - cycle * cycleSeconds) / cycleSeconds;
    updateVoiceActivity(cycle);
    drawGeometryTrace(u, cycle);
    const left = 48;
    const right = width - 18;
    const span = Math.max(1, right - left);
    const stripHeight = (height - 34) / laneCount;
    const plotAmplitude = stripHeight * 0.34 * graphYScale;
    const viewSpan = graphWindowSpan();
    const viewEnd = graphStart + viewSpan;
    const traceEnd = running ? Math.min(viewEnd, u) : viewEnd;
    const traceVisible = traceEnd >= graphStart;
    const traceSamples = Math.max(1, Math.ceil(320 * Math.max(0, traceEnd - graphStart) / viewSpan));
    const events = generated.events.filter((event) => event.cycle === cycle);

    ctx.font = "10px ui-monospace, monospace";
    const beatSeconds = 60 / generated.config.bpm;
    const cycleStart = cycle * cycleSeconds;
    const firstVisibleBeat = Math.ceil((cycleStart + graphStart * cycleSeconds) / beatSeconds);
    const lastVisibleBeat = Math.floor((cycleStart + viewEnd * cycleSeconds) / beatSeconds);
    for (let beat = firstVisibleBeat; beat <= lastVisibleBeat; beat += 1) {
      const progress = (beat * beatSeconds - cycleStart) / cycleSeconds;
      const x = left + span * (progress - graphStart) / viewSpan;
      ctx.strokeStyle = "#d9dfd8";
      ctx.lineWidth = 0.7;
      ctx.beginPath(); ctx.moveTo(x, 6); ctx.lineTo(x, height - 7); ctx.stroke();
    }
    const [meterNumerator, meterDenominator] = generated.config.meter.split("/").map(Number);
    const barSeconds = meterNumerator * 4 / meterDenominator * beatSeconds;
    const firstVisibleBar = Math.ceil((cycleStart + graphStart * cycleSeconds) / barSeconds);
    const lastVisibleBar = Math.floor((cycleStart + viewEnd * cycleSeconds) / barSeconds);
    for (let bar = firstVisibleBar; bar <= lastVisibleBar; bar += 1) {
      const progress = (bar * barSeconds - cycleStart) / cycleSeconds;
      const x = left + span * (progress - graphStart) / viewSpan;
      ctx.strokeStyle = "#78948a";
      ctx.lineWidth = 1.25;
      ctx.beginPath(); ctx.moveTo(x, 6); ctx.lineTo(x, height - 7); ctx.stroke();
    }
    for (let voice = 0; voice < voiceCount; voice += 1) {
      const mid = 17 + stripHeight * (voice + 0.5);
      const offset = voice * stripHeight;
      ctx.strokeStyle = "#c4d0c5";
      ctx.lineWidth = 1;
      for (const factor of [-1, 0, 1]) {
        const y = mid + factor * stripHeight * 0.34;
        ctx.beginPath(); ctx.moveTo(left, y); ctx.lineTo(right, y); ctx.stroke();
      }
      for (let division = 0; division <= 8; division += 1) {
        const x = left + span * division / 8;
        ctx.beginPath(); ctx.moveTo(x, offset + 9); ctx.lineTo(x, offset + stripHeight + 7); ctx.stroke();
      }
      ctx.fillStyle = colors[voice];
      ctx.fillText(String(voice + 1).padStart(2, "0"), 15, mid + 4);
      ctx.save();
      ctx.beginPath();
      ctx.rect(left, offset + 9, span, stripHeight - 2);
      ctx.clip();
      ctx.strokeStyle = colors[voice];
      ctx.lineWidth = 2.3;
      ctx.beginPath();
      if (traceVisible) {
        for (let step = 0; step <= traceSamples; step += 1) {
          const p = graphStart + (traceEnd - graphStart) * step / traceSamples;
          const value = scoreModel.valueAt(cycle, voice, p, generated.config) *
            amplitude(cycle, p);
          const x = left + span * (p - graphStart) / viewSpan;
          const y = mid - value * plotAmplitude;
          if (step === 0) ctx.moveTo(x, y); else ctx.lineTo(x, y);
        }
      }
      ctx.stroke();
      for (const event of events) {
        if (event.voice !== voice || event.performedProgress < graphStart || event.performedProgress > traceEnd) continue;
        const x = left + span * (event.performedProgress - graphStart) / viewSpan;
        const y = mid - event.position * amplitude(cycle, event.localProgress) * plotAmplitude;
        ctx.fillStyle = "#f8f6ec";
        ctx.strokeStyle = colors[voice];
        ctx.lineWidth = 2;
        ctx.beginPath(); ctx.arc(x, y, 3.7, 0, 2 * Math.PI); ctx.fill(); ctx.stroke();
      }
      const current = scoreModel.valueAt(cycle, voice, u, generated.config) * amplitude(cycle, u);
      const currentY = mid - current * plotAmplitude;
      if (u >= graphStart && u <= viewEnd) {
        ctx.fillStyle = colors[voice];
        ctx.beginPath(); ctx.arc(left + span * (u - graphStart) / viewSpan, currentY, 5.3, 0, 2 * Math.PI); ctx.fill();
      }
      ctx.restore();
      if (generated.config.motionModel === "coupled-pendulum" && voice < 3) {
        const physicalState = scoreModel.pendulumStateAt(cycle, voice, u,
          generated.config);
        document.getElementById(`value-${voice}`).textContent =
          `θ=${physicalState.angle.toFixed(3)} rad · ω=${physicalState.velocity.toFixed(3)} rad/s`;
      } else {
        document.getElementById(`value-${voice}`).textContent =
          `${current >= 0 ? "+" : ""}${phaseText(current)}`;
      }
      const phaseSymbol = document.getElementById(`formula-phase-${voice}`);
      if (phaseSymbol) phaseSymbol.textContent =
        phaseText(scoreModel.phaseFor(cycle, voice, generated.config.phase, u, generated.config));
    }

    const bassMid = 17 + stripHeight * (voiceCount + 0.5);
    const bassOffset = voiceCount * stripHeight;
    ctx.strokeStyle = "#c4d0c5";
    ctx.lineWidth = 1;
    ctx.beginPath(); ctx.moveTo(left, bassMid); ctx.lineTo(right, bassMid); ctx.stroke();
    ctx.fillStyle = "#203c38"; ctx.fillText("BX", 12, bassMid + 4);
    ctx.save(); ctx.beginPath(); ctx.rect(left, bassOffset + 9, span, stripHeight - 2); ctx.clip();
    ctx.strokeStyle = "#203c38"; ctx.lineWidth = 2.2; ctx.beginPath();
    if (traceVisible) {
      for (let step = 0; step <= traceSamples; step += 1) {
        const p = graphStart + (traceEnd - graphStart) * step / traceSamples;
        const x = left + span * (p - graphStart) / viewSpan;
        const midi = scoreModel.bassPitchAt((cycle + p) / 7, generated.config);
        if (midi === null) { ctx.stroke(); ctx.beginPath(); continue; }
        const y = bassMid - ((midi - 36) / 12) * plotAmplitude;
        if (step === 0 || scoreModel.bassPitchAt((cycle + p - (traceEnd - graphStart) / traceSamples) / 7, generated.config) === null) ctx.moveTo(x, y);
        else ctx.lineTo(x, y);
      }
    }
    ctx.stroke();
    for (const event of events) {
      if (event.voice !== BASS_VOICE || event.performedProgress < graphStart || event.performedProgress > traceEnd) continue;
      const x = left + span * (event.performedProgress - graphStart) / viewSpan;
      const y = bassMid - event.position * plotAmplitude;
      ctx.fillStyle = "#f8f6ec";
      ctx.strokeStyle = event.intent === "ghost" ? "#78948a" : event.intent === "accent" ? "#c65132" : "#203c38";
      ctx.lineWidth = 2;
      ctx.beginPath(); ctx.arc(x, y, 3.7, 0, 2 * Math.PI); ctx.fill(); ctx.stroke();
    }
    const currentBass = scoreModel.bassPitchAt((cycle + u) / 7, generated.config);
    if (currentBass !== null && u >= graphStart && u <= viewEnd) {
      ctx.fillStyle = "#203c38";
      ctx.beginPath(); ctx.arc(left + span * (u - graphStart) / viewSpan,
        bassMid - ((currentBass - 36) / 12) * plotAmplitude, 5, 0, 2 * Math.PI); ctx.fill();
    }
    ctx.restore();

    ctx.fillStyle = "#45615c";
    for (let division = 0; division <= 8; division += 1) {
      const x = left + span * division / 8;
      ctx.fillText((graphStart + viewSpan * division / 8).toFixed(2), x - 11, height - 5);
    }
    if (u >= graphStart && u <= viewEnd) {
      const penX = left + span * (u - graphStart) / viewSpan;
      ctx.strokeStyle = "#132a28";
      ctx.lineWidth = 1.3;
      ctx.setLineDash([5, 4]);
      ctx.beginPath(); ctx.moveTo(penX, 6); ctx.lineTo(penX, height - 7); ctx.stroke();
      ctx.setLineDash([]);
    }
    const graphNote = graphLegend.querySelector(".graph-note");
    graphNote.textContent = running ?
      (u < graphStart || u > viewEnd ? "Traço cresce com a execução; cursor fora da janela. Y não muda o volume." :
        "Traço revelado pelo cálculo em tempo real; a forma muda com cada transformação.") :
      (u < graphStart || u > viewEnd ?
      "Cursor fora da janela; arraste para percorrer. Y não muda o volume." :
      "Arraste para percorrer. A escala Y é visual; não muda o volume.");
    document.getElementById("section-name").textContent = scoreModel.sectionForCycle(cycle);
    const minutes = String(Math.floor(seconds / 60)).padStart(2, "0");
    const secondsText = (seconds % 60).toFixed(1).padStart(4, "0");
    document.getElementById("clock").textContent = `${minutes}:${secondsText}`;
    if (cycle !== currentCycle) {
      currentCycle = cycle;
      [...timeline.children].forEach((bar, index) => bar.classList.toggle("active", index === cycle));
    }
  }

  function updateVoiceActivity(cycle) {
    const count = generated.config.voiceCount;
    const partial = scoreModel.activeVoiceCountInCycle(1, count);
    const activeCount = scoreModel.activeVoiceCountInCycle(cycle, count);
    for (let voice = 0; voice < count; voice += 1) {
      const card = formulaGrid.querySelector(`.formula-card:nth-child(${voice + 1})`);
      const badge = document.getElementById(`voice-entry-${voice}`);
      if (!card || !badge) continue;
      const active = voice < activeCount;
      let nextCycle = null;
      if (!active && cycle < 2) nextCycle = voice < partial ? 1 : 2;
      else if (!active && cycle === 5) nextCycle = 6;
      card.dataset.eventState = active ? "active" : "rest";
      badge.textContent = active ? "ATIVA" : `ENTRA C${nextCycle + 1}`;
      badge.setAttribute("aria-label", active
        ? `Voz ${voice + 1} ativa neste ciclo`
        : `Voz ${voice + 1} entra no ciclo ${nextCycle + 1}`);
    }
  }

  function scheduleVoice(event, when) {
    if (!audio || !channelIsAudible(event.voice)) return;
    const oscillators = [];
    const gain = audio.createGain();
    let bassFilter = null;
    let bassMaterialGain = null;
    let bassBaseCutoff = 0;
    const intensity = Math.min(1, Math.max(0.02, event.intensity));
    if (event.voice === BASS_VOICE) {
      const material = {
        sub: { cutoff: 150 },
        round: { cutoff: 420 },
        body: { cutoff: 680 },
        pulse: { cutoff: 520 },
        bright: { cutoff: 1800 },
        pluck: { cutoff: 1500 }
      }[generated.config.bassTimbre];
      const ratios = {
        harmonic: [1, 2, 3], odd: [1, 3, 5],
        inharmonic: [1, Math.SQRT2, Math.sqrt(3)],
        beat: [1, 1.003, 0.997]
      }[generated.config.bassOscillatorProfile] || [1, 2, 3];
      const partialLevels = [1 / 1.5, 0.34 / 1.5, 0.16 / 1.5];
      bassFilter = audio.createBiquadFilter();
      bassFilter.type = "lowpass";
      const cutoff = material.cutoff * generated.config.bassVcfCutoffHz / 420;
      bassBaseCutoff = cutoff;
      bassFilter.frequency.setValueAtTime(cutoff, when);
      bassFilter.Q.setValueAtTime(generated.config.bassVcfResonance, when);
      for (let partial = 0; partial < ratios.length; partial += 1) {
        const oscillator = audio.createOscillator();
        const partialGain = audio.createGain();
        oscillator.type = "sine";
        const frequency = 440 * Math.pow(2, (event.pitch - 69) / 12) * ratios[partial];
        if (!Number.isFinite(frequency) || !Number.isFinite(when))
          throw new RangeError(`Evento de baixo inválido: ${JSON.stringify({
            pitch: event.pitch, eventTime: event.time, when, profile: generated.config.bassOscillatorProfile
          })}`);
        oscillator.frequency.setValueAtTime(frequency, when);
        partialGain.gain.setValueAtTime(partialLevels[partial], when);
        oscillator.connect(partialGain).connect(bassFilter || gain);
        oscillator.start(when);
        oscillators.push({ oscillator, partialGain });
      }
      if (generated.config.bassTimbre === "pluck") {
        bassMaterialGain = audio.createGain();
        bassMaterialGain.gain.setValueAtTime(1, when);
        bassMaterialGain.gain.exponentialRampToValueAtTime(0.12,
          when + Math.min(0.12, Math.max(0.012, event.durationSeconds)));
      }
    } else {
      const oscillator = audio.createOscillator();
      oscillator.type = generated.config.timbre;
      oscillator.start(when);
      oscillators.push({ oscillator, partialGain: null });
    }
    if (event.voice !== BASS_VOICE) {
      const oscillator = oscillators[0].oscillator;
      if (event.voice === 0) {
        oscillator.frequency.setValueAtTime(155, when);
        oscillator.frequency.exponentialRampToValueAtTime(72, when + 0.16);
      } else {
        const frequency = 175 + event.voice * 93 + intensity * 45;
        oscillator.frequency.setValueAtTime(frequency, when);
        oscillator.frequency.exponentialRampToValueAtTime(frequency * 0.76, when + 0.14);
      }
    }
    const voiceScale = event.voice === BASS_VOICE ? 0.48 : 1 / Math.sqrt(generated.config.voiceCount - 1);
    gain.gain.setValueAtTime(0.0001, when);
    const articulation = generated.config.articulation;
    const release = event.voice === BASS_VOICE ? (generated.config.bassTimbre === "pluck" ? 0.07 :
      event.repeats > 1 ? 0.02 : 0.045) :
      articulation === "short" ? 0.075 : articulation === "long" ? 0.62 : 0.28;
    const peak = intensity * voiceScale * (event.voice === 0 ? 0.31 : event.voice === BASS_VOICE ? 0.52 : 0.19)
      * (event.voice === BASS_VOICE ? generated.config.bassOutputLevel / 0.2 : 1);
    gain.gain.linearRampToValueAtTime(peak, when + 0.006);
    if (event.voice === BASS_VOICE) {
      const config = generated.config;
      const ampAttack = config.bassAmpAttackSeconds;
      const ampDecay = config.bassAmpDecaySeconds;
      const ampSustain = config.bassAmpSustain;
      const ampRelease = config.bassAmpReleaseSeconds;
      const filterAttack = config.bassFilterAttackSeconds;
      const filterDecay = config.bassFilterDecaySeconds;
      const filterSustain = config.bassFilterSustain;
      const filterRelease = config.bassFilterReleaseSeconds;
      const gate = Math.max(0.006, event.durationSeconds * config.bassGateRatio);
      const noteOff = when + gate;
      const ampAt = (time, attack, decay, sustain) => time < attack
        ? Math.max(0.0001, time / attack)
        : time < attack + decay
          ? 1 + (sustain - 1) * (time - attack) / decay : sustain;
      const ampOff = Math.max(0.0001, ampAt(gate, ampAttack, ampDecay, ampSustain));
      gain.gain.cancelScheduledValues(when);
      if (peak > 0.0001) {
        gain.gain.setValueAtTime(0.0001, when);
        const ampAttackEnd = Math.min(ampAttack, gate);
        gain.gain.linearRampToValueAtTime(peak * ampAttackEnd / ampAttack,
          when + ampAttackEnd);
        if (gate > ampAttack) {
          const ampDecayEnd = Math.min(ampDecay, gate - ampAttack);
          const level = 1 + (ampSustain - 1) * ampDecayEnd / ampDecay;
          gain.gain.linearRampToValueAtTime(peak * level,
            when + ampAttack + ampDecayEnd);
        }
        gain.gain.cancelScheduledValues(noteOff);
        gain.gain.setValueAtTime(peak * ampOff, noteOff);
        gain.gain.exponentialRampToValueAtTime(0.0001, noteOff + ampRelease);
      } else {
        gain.gain.setValueAtTime(0, when);
      }
      if (bassFilter) {
        const baseCutoff = bassBaseCutoff;
        const depth = config.bassFilterEnvelopeAmountHz;
        const filterAt = (time) => time < filterAttack
          ? Math.max(0.0001, time / filterAttack)
          : time < filterAttack + filterDecay
            ? 1 + (filterSustain - 1) * (time - filterAttack) / filterDecay
            : filterSustain;
        const filterOff = filterAt(gate);
        bassFilter.frequency.cancelScheduledValues(when);
        bassFilter.frequency.setValueAtTime(baseCutoff, when);
        const filterAttackEnd = Math.min(filterAttack, gate);
        bassFilter.frequency.linearRampToValueAtTime(
          baseCutoff + depth * filterAttackEnd / filterAttack,
          when + filterAttackEnd);
        if (gate > filterAttack) {
          const filterDecayEnd = Math.min(filterDecay, gate - filterAttack);
          const level = 1 + (filterSustain - 1) * filterDecayEnd / filterDecay;
          bassFilter.frequency.linearRampToValueAtTime(
            baseCutoff + depth * level,
            when + filterAttack + filterDecayEnd);
        }
        bassFilter.frequency.cancelScheduledValues(noteOff);
        bassFilter.frequency.setValueAtTime(baseCutoff + depth * filterOff, noteOff);
        bassFilter.frequency.linearRampToValueAtTime(baseCutoff,
          noteOff + filterRelease);
      }
    } else {
      gain.gain.exponentialRampToValueAtTime(0.0001, when + release);
    }
    if (bassFilter) bassFilter.connect(bassMaterialGain || gain);
    if (bassMaterialGain) bassMaterialGain.connect(gain);
    else oscillators[0].oscillator.connect(gain);
    gain.connect(channelBuses[event.voice].gain);
    const bassRelease = event.voice === BASS_VOICE
      ? generated.config.bassAmpReleaseSeconds : release;
    const stopAt = when + (event.voice === BASS_VOICE
      ? Math.max(0.006, event.durationSeconds * generated.config.bassGateRatio) : 0)
      + (event.voice === BASS_VOICE
        ? Math.max(bassRelease, generated.config.bassFilterReleaseSeconds) : bassRelease)
      + 0.03;
    oscillators.forEach(({ oscillator, partialGain }) => {
      oscillator.stop(stopAt);
      oscillator.onended = () => {
        oscillator.disconnect();
        if (partialGain) partialGain.disconnect();
      };
    });
    oscillators[0].oscillator.onended = () => {
      oscillators[0].oscillator.disconnect();
      if (oscillators[0].partialGain) oscillators[0].partialGain.disconnect();
      gain.disconnect(); if (bassFilter) bassFilter.disconnect();
      if (bassMaterialGain) bassMaterialGain.disconnect();
    };
  }

  function schedule() {
    if (!running || !audio) return;
    const horizon = audio.currentTime + 0.12;
    while (nextEvent < generated.events.length) {
      const event = generated.events[nextEvent];
      const when = audioStart + event.time;
      if (when > horizon) break;
      if (when >= audio.currentTime - 0.03) {
        const trace = window.__batrucScheduleTrace;
        if (Array.isArray(trace) && trace.length < 4096)
          trace.push({ revision: scheduleRevision, seed: generated.config.seed,
            phase: generated.config.phase, time: event.time,
            voice: event.voice,
            voiceCycle: event.voice < generated.config.voiceCount
              ? generated.config.voiceCycles[event.voice] : null,
            voiceCount: generated.config.voiceCount, scheduledAt: elapsed() });
        scheduleVoice(event, Math.max(audio.currentTime + 0.003, when));
      }
      nextEvent += 1;
    }
  }

  function animate() {
    if (!running) return;
    const seconds = elapsed();
    if (audio) updateMixerBalance();
    draw(seconds);
    if (seconds >= generated.duration) {
      stop("Estudo concluído. Compare a outra receita ou mude a fase.");
      return;
    }
    frame = requestAnimationFrame(animate);
  }

  function startRecording() {
    if (!audio || !recordDestination || !globalThis.MediaRecorder) {
      recordStatus.textContent = "Gravação não disponível neste navegador.";
      return false;
    }
    try {
      clearScoreDownload();
      recordingStartedAt = performance.now();
      recordingStartSeconds = running ? elapsed() : 0;
      recordingDirectoryHandle = archiveDirectoryHandle;
      recordingScoreRevisions = [];
      recordingRevisionSignature = "";
      recordingRevisionTruncated = false;
      recordingInitialEvents = recordedEventPlan();
      recordingRevisionSignature = JSON.stringify(recordingConfigSnapshot());
      recordingScoreRevisions.push({
        effective_at_seconds: 0,
        configuration: recordingConfigSnapshot()
      });
      const candidates = ["audio/webm;codecs=opus", "audio/ogg;codecs=opus"];
      const mimeType = typeof MediaRecorder.isTypeSupported === "function" ?
        candidates.find((type) => MediaRecorder.isTypeSupported(type)) : undefined;
      const recorderOptions = mimeType ? { mimeType, audioBitsPerSecond: 192000 } :
        { audioBitsPerSecond: 192000 };
      mediaRecorder = new MediaRecorder(recordDestination.stream, recorderOptions);
      recordChunks = [];
      mediaRecorder.addEventListener("dataavailable", (event) => {
        if (event.data && event.data.size > 0) recordChunks.push(event.data);
      });
      mediaRecorder.addEventListener("stop", () => {
        const type = mediaRecorder?.mimeType || mimeType || "audio/webm";
        const blob = new Blob(recordChunks, { type });
        void (async () => {
        if (blob.size > 0) {
          const url = URL.createObjectURL(blob);
          const extension = type.includes("ogg") ? "ogg" : "webm";
          const baseName = `batruc-master-${new Date().toISOString().replace(/[:.]/g, "-")}`;
          recordingAudioFile = {
            filename: `${baseName}.${extension}`,
            mime_type: blob.type || type,
            encoding: type.includes("opus") ? "Opus" : "codec informado pelo navegador"
          };
          const scoreText = JSON.stringify(makeRecordingScore(), null, 2);
          const scoreFilename = `${baseName}-partitura.txt`;
          let savedToDirectory = false;
          if (recordingDirectoryHandle) {
            let audioWritten = false;
            try {
              const descriptor = { mode: "readwrite" };
              let permission = await recordingDirectoryHandle.queryPermission(descriptor);
              if (permission !== "granted") permission = await recordingDirectoryHandle.requestPermission(descriptor);
              if (permission !== "granted") throw new Error("Permissão da pasta não concedida.");
              const audioHandle = await recordingDirectoryHandle.getFileHandle(recordingAudioFile.filename, { create: true });
              const audioWriter = await audioHandle.createWritable();
              await audioWriter.write(blob);
              await audioWriter.close();
              audioWritten = true;
              await writeFileToArchive(recordingDirectoryHandle, scoreFilename,
                scoreText, "text/plain;charset=utf-8");
              savedToDirectory = true;
              recordStatus.textContent = `Áudio e partitura salvos em ${recordingDirectoryHandle.name} · ${(blob.size / 1024 / 1024).toFixed(2)} MB.`;
            } catch (error) {
              if (audioWritten) {
                await recordingDirectoryHandle.removeEntry(recordingAudioFile.filename).catch(() => {});
              }
              recordStatus.textContent = `A pasta não aceitou a gravação (${error.message}); downloads preparados.`;
            }
          }
          if (!savedToDirectory) {
            const audioLink = document.createElement("a");
            audioLink.href = url;
            audioLink.download = recordingAudioFile.filename;
            audioLink.click();
            const scoreBlob = new Blob([scoreText], { type: "text/plain;charset=utf-8" });
            scoreDownloadUrl = URL.createObjectURL(scoreBlob);
            scoreDownload.href = scoreDownloadUrl;
            scoreDownload.download = scoreFilename;
            scoreDownload.hidden = false;
            recordStatus.textContent += ` Master ${(blob.size / 1024 / 1024).toFixed(2)} MB · download iniciado; TXT disponível.`;
          }
          setTimeout(() => URL.revokeObjectURL(url), 30000);
        } else {
          recordStatus.textContent = "Gravação vazia; nenhum arquivo salvo.";
        }
        recordButton.textContent = "● Gravar master";
        recordButton.setAttribute("aria-pressed", "false");
        mediaRecorder = null;
        recordChunks = [];
        recordingDirectoryHandle = null;
        })();
      }, { once: true });
      mediaRecorder.start(250);
      recordButton.textContent = "■ Finalizar e salvar";
      recordButton.setAttribute("aria-pressed", "true");
      recordStatus.textContent = "REC · saída master estéreo.";
      return true;
    } catch (error) {
      mediaRecorder = null;
      recordStatus.textContent = `Não foi possível iniciar: ${error.message}`;
      return false;
    }
  }

  function finishRecording() {
    if (!mediaRecorder || mediaRecorder.state !== "recording") return Promise.resolve();
    return new Promise((resolve) => {
      mediaRecorder.addEventListener("stop", resolve, { once: true });
      mediaRecorder.stop();
    });
  }

  async function stop(message = "Parado. O desenho permanece no início do ciclo.") {
    running = false;
    if (scheduler) clearInterval(scheduler);
    if (frame) cancelAnimationFrame(frame);
    scheduler = null;
    frame = null;
    await finishRecording();
    if (audio) {
      const previous = audio;
      audio = null;
      master = null;
      formulaBus = null;
      formulaAnalyser = null;
      bassAnalyser = null;
      recordDestination = null;
      channelBuses = [];
      previous.close().catch(() => {});
    }
    mixerBalanceReadout.textContent = "SENSOR · parado";
    mixerAutoBalanceButton.disabled = true;
    playButton.textContent = "▶ Tocar estudo";
    status.textContent = message;
    currentCycle = -1;
    draw(0);
  }

  async function play(recordOnStart = false) {
    if (running) await stop();
    refreshScore();
    if (AudioContextClass) {
      try {
        audio = new AudioContextClass();
        await audio.resume();
        master = audio.createGain();
        master.gain.value = masterMuted ? 0 : Number(outputLevelInput.value);
        formulaBus = audio.createGain();
        formulaAnalyser = audio.createAnalyser();
        formulaAnalyser.fftSize = 2048;
        formulaBus.connect(formulaAnalyser).connect(master);
        const peakControl = audio.createDynamicsCompressor();
        peakControl.threshold.value = -8;
        peakControl.knee.value = 3;
        peakControl.ratio.value = 8;
        peakControl.attack.value = 0.003;
        peakControl.release.value = 0.16;
        master.connect(peakControl).connect(audio.destination);
        recordDestination = audio.createMediaStreamDestination();
        peakControl.connect(recordDestination);
        channelBuses = [];
        ensureChannelBuses(BASS_VOICE + 1);
        applyMixer();
        audioStart = audio.currentTime + 0.07;
      } catch (error) {
        if (audio) audio.close().catch(() => {});
        audio = null;
        master = null;
        formulaBus = null;
        formulaAnalyser = null;
        bassAnalyser = null;
        recordDestination = null;
        channelBuses = [];
      }
    }
    wallStart = performance.now() + (audio ? 70 : 0);
    nextEvent = 0;
    running = true;
    if (recordOnStart) startRecording();
    playButton.textContent = "↺ Reiniciar estudo";
    status.textContent = audio ? "Tocando. Fórmulas, traços e eventos compartilham o mesmo ciclo." :
      "Animação sem áudio: Web Audio indisponível neste navegador.";
    scheduler = setInterval(schedule, 25);
    schedule();
    frame = requestAnimationFrame(animate);
  }

  // Internal deterministic export hook for the HTML/JUCE parity renderer.
  // It reuses the live voice graph and event scheduler, but renders against an
  // OfflineAudioContext so automated headless runs do not depend on a device
  // clock or an attached physical audio output.
  window.__batrucRenderOffline = async (sampleRate = 48000, stem = null) => {
    if (running || !generated)
      throw new Error("Pare a execução antes de renderizar offline.");
    if (stem !== null && stem !== "voices" && stem !== "bass")
      throw new RangeError("Stem offline deve ser voices, bass ou null.");
    const OfflineContext = globalThis.OfflineAudioContext || globalThis.webkitOfflineAudioContext;
    if (!OfflineContext) throw new Error("OfflineAudioContext indisponível.");
    if (!Number.isInteger(sampleRate) || sampleRate < 8000 || sampleRate > 192000)
      throw new RangeError("Taxa de amostragem offline fora do limite.");
    const config = recordingConfigSnapshot();
    const durationSeconds = config.durationSeconds;
    if (!Number.isFinite(durationSeconds) || durationSeconds <= 0 || durationSeconds > 84)
      throw new RangeError("Duração offline fora do limite de 84 segundos.");
    const frameCount = Math.ceil(durationSeconds * sampleRate);
    const previous = { audio, master, formulaBus, formulaAnalyser, bassAnalyser,
      recordDestination, channelBuses, audioStart, offlineRenderStem };
    try {
      offlineRenderStem = stem;
      audio = new OfflineContext(2, frameCount, sampleRate);
      master = audio.createGain();
      master.gain.value = masterMuted ? 0 : Number(outputLevelInput.value);
      formulaBus = audio.createGain();
      formulaAnalyser = audio.createAnalyser();
      formulaAnalyser.fftSize = 2048;
      formulaBus.connect(formulaAnalyser).connect(stem ? audio.destination : master);
      const peakControl = audio.createDynamicsCompressor();
      peakControl.threshold.value = -8;
      peakControl.knee.value = 3;
      peakControl.ratio.value = 8;
      peakControl.attack.value = 0.003;
      peakControl.release.value = 0.16;
      master.connect(peakControl).connect(audio.destination);
      recordDestination = null;
      bassAnalyser = null;
      channelBuses = [];
      ensureChannelBuses(BASS_VOICE + 1);
      applyMixer();
      if (stem) {
        channelBuses.forEach((bus, voice) => {
          const selected = stem === "bass" ? voice === BASS_VOICE : voice < BASS_VOICE;
          bus.gain.gain.cancelScheduledValues(0);
          bus.gain.gain.setValueAtTime(selected ? channelVolumes[voice] : 0, 0);
        });
      }
      // Offline reference starts on the same transport origin as the JUCE
      // renderer; the live player still schedules ahead for device latency.
      audioStart = 0.0;
      for (const event of generated.events) {
        const when = audioStart + event.time;
        if (when >= 0 && when < durationSeconds) scheduleVoice(event, when);
      }
      const rendered = await audio.startRendering();
      window.__batrucOfflineBuffer = rendered;
      return {
        sample_rate: rendered.sampleRate,
        channels: rendered.numberOfChannels,
        frames: rendered.length,
        duration_seconds: rendered.duration,
        configuration: config,
        mathematical_partitura: mathematicalPartitura(config),
        event_plan: recordedEventPlan()
      };
    } finally {
      audio = previous.audio;
      master = previous.master;
      formulaBus = previous.formulaBus;
      formulaAnalyser = previous.formulaAnalyser;
      bassAnalyser = previous.bassAnalyser;
      recordDestination = previous.recordDestination;
      channelBuses = previous.channelBuses;
      audioStart = previous.audioStart;
      offlineRenderStem = previous.offlineRenderStem;
    }
  };

  window.__batrucOfflineChunkBase64 = (startFrame, frameCount) => {
    const rendered = window.__batrucOfflineBuffer;
    if (!rendered || !Number.isInteger(startFrame) || !Number.isInteger(frameCount) ||
        startFrame < 0 || frameCount < 0 || startFrame + frameCount > rendered.length ||
        frameCount > rendered.sampleRate)
      throw new RangeError("Trecho PCM offline inválido.");
    const left = rendered.getChannelData(0);
    const right = rendered.numberOfChannels > 1 ? rendered.getChannelData(1) : left;
    const bytes = new Uint8Array(frameCount * 4);
    const view = new DataView(bytes.buffer);
    for (let frame = 0; frame < frameCount; frame += 1) {
      const l = Math.max(-1, Math.min(1, left[startFrame + frame]));
      const r = Math.max(-1, Math.min(1, right[startFrame + frame]));
      view.setInt16(frame * 4, Math.round(l * 32767), true);
      view.setInt16(frame * 4 + 2, Math.round(r * 32767), true);
    }
    let binary = "";
    for (let offset = 0; offset < bytes.length; offset += 0x8000)
      binary += String.fromCharCode(...bytes.subarray(offset, offset + 0x8000));
    return btoa(binary);
  };

  recipeInput.addEventListener("change", () => {
    freezeSeedVariationForManualEdit();
    refreshScore();
  });
  motionModelInput.addEventListener("change", refreshScore);
  for (const input of [pendulumLengthInput, pendulumCouplingInput, pendulumDampingInput])
    input.addEventListener("input", refreshScore);
  function applySelectedSeed(seed) {
    seedInput.value = String(seed).toUpperCase();
    bassVoiceDriven = true;
    if (Number(variationInput.value) === 0) variationInput.value = "35";
    applySeedDerivedComposition(seedInput.value);
    applySeedDerivedBassProfile(seedInput.value);
    refreshScore();
  }
  seedInput.addEventListener("input", () => {
    const normalized = seedInput.value.toUpperCase().replace(/[^A-F0-9]/g, "").slice(0, 8);
    if (seedInput.value !== normalized) seedInput.value = normalized;
    if (normalized.length === 8) applySelectedSeed(normalized);
    else document.getElementById("seed-summary").textContent = "Digite oito caracteres hexadecimais para fixar a seed.";
  });
  seedNewButton.addEventListener("click", () => {
    if (/^[A-F0-9]{8}$/.test(seedInput.value))
      applySelectedSeed(scoreModel.algorithmicNextSeed(seedInput.value));
    else document.getElementById("seed-summary").textContent =
      "Digite oito caracteres hexadecimais para gerar a próxima seed.";
  });
  variationInput.addEventListener("input", refreshScore);
  bassPitchVariationInput.addEventListener("input", refreshScore);
  phaseInput.addEventListener("input", () => {
    freezeSeedVariationForManualEdit();
    voiceBasePhases[1] = Number(phaseInput.value);
    refreshScore();
  });
  patternModeInput.addEventListener("change", () => {
    freezeSeedVariationForManualEdit();
    refreshScore();
  });
  intensityLawInput.addEventListener("change", refreshScore);
  intensityAmountInput.addEventListener("input", () => {
    document.getElementById("intensity-amount-value").textContent =
      `${Math.round(Number(intensityAmountInput.value) * 100)}%`;
    refreshScore();
  });
  for (const input of [intentionInput, dynamicsInput, swingInput, articulationInput,
    durationInput, bpmInput, meterInput, timbreInput, grooveInput,
    bassShapeInput, bassRateInput, bassDepthInput, bassLfoEnabledInput,
    bassTimbreInput, bassOscillatorProfileInput, bassPhraseModeInput,
    bassRhythmRelationInput,
    bassDurationFormulaInput, bassGeoRatioInput]) {
    input.addEventListener(input.type === "range" ? "input" : "change", refreshScore);
  }
  for (const input of [bassVcfCutoffInput, bassVcfResonanceInput]) {
    input.addEventListener("input", () => {
      document.getElementById("bass-vcf-cutoff-value").textContent =
        `${Math.round(Number(bassVcfCutoffInput.value))} Hz`;
      document.getElementById("bass-vcf-resonance-value").textContent =
        Number(bassVcfResonanceInput.value).toFixed(2).replace(".", ",");
      refreshScore();
    });
  }
  for (const [key, input] of Object.entries(bassEnvelopeInputs)) {
    input.addEventListener("input", () => {
      const value = Number(input.value);
      const output = document.getElementById(`${input.id}-value`);
      if (output) {
        output.textContent = key.toLowerCase().includes("sustain") || key === "bassGateRatio"
          ? `${Math.round(value * 100)}%`
          : key === "bassFilterEnvelopeAmountHz" ? `${Math.round(value)} Hz`
            : key === "bassOutputLevel" ? value.toFixed(2).replace(".", ",")
              : `${Math.round(value * 1000)} ms`;
      }
      refreshScore();
    });
  }
  geometryShapeInput.addEventListener("change", refreshScore);
  geometryBilliardPatternInput.addEventListener("change", refreshScore);
  geometrySourceInput.addEventListener("change", refreshScore);
  geometryFourierTermsInput.addEventListener("input", refreshScore);
  meterInput.addEventListener("change", () => {
    renderBassPatternControls(); drawBassEditor();
  });
  bassDepthInput.addEventListener("input", () => {
    document.getElementById("bass-depth-value").textContent = `${bassDepthInput.value} semitons`;
    refreshScore();
  });
  outputLevelInput.addEventListener("input", updateOutputLevel);
  formulaBusTrimInput.addEventListener("input", updateFormulaBusTrim);
  mixerAutoBalanceButton.addEventListener("click", applySuggestedMixerBalance);
  document.getElementById("master-mute").addEventListener("click", (event) => {
    masterMuted = !masterMuted;
    event.currentTarget.setAttribute("aria-pressed", String(masterMuted));
    event.currentTarget.setAttribute("aria-label", masterMuted ? "Retirar mute do master" : "Mute master");
    event.currentTarget.textContent = "M";
    updateOutputLevel();
    status.textContent = masterMuted ? "Saída master silenciada." : "Saída master recuperada.";
  });
  graphZoomInput.addEventListener("input", setGraphViewFromControls);
  graphYScaleInput.addEventListener("input", () => {
    graphYScale = Number(graphYScaleInput.value);
    graphYScaleValue.value = `${graphYScale.toFixed(1).replace(".", ",")}×`;
    draw(running ? elapsed() : 0);
  });
  graphFocusInput.addEventListener("input", () => setGraphViewFromControls("focus"));
  document.getElementById("graph-reset").addEventListener("click", () => setGraphViewAt(1, 0));
  graph.addEventListener("wheel", (event) => {
    event.preventDefault();
    const bounds = graph.getBoundingClientRect();
    const plotWidth = Math.max(1, bounds.width - 66);
    const fraction = Math.max(0, Math.min(1, (event.clientX - bounds.left - 48) / plotWidth));
    const anchor = graphStart + fraction * graphWindowSpan();
    const nextZoom = Math.round(graphZoom * (event.deltaY < 0 ? 1.25 : 0.8) * 4) / 4;
    const nextSpan = 1 / Math.max(1, Math.min(8, nextZoom));
    setGraphViewAt(nextZoom, anchor - fraction * nextSpan);
  }, { passive: false });
  graph.addEventListener("pointerdown", (event) => {
    if (graphZoom <= 1) return;
    graphDrag = { pointerId: event.pointerId, x: event.clientX, start: graphStart,
      width: Math.max(1, graph.getBoundingClientRect().width - 66) };
    graph.setPointerCapture(event.pointerId);
    graph.style.cursor = "grabbing";
  });
  graph.addEventListener("pointermove", (event) => {
    if (!graphDrag || graphDrag.pointerId !== event.pointerId) return;
    const shift = (event.clientX - graphDrag.x) / graphDrag.width * graphWindowSpan();
    setGraphViewAt(graphZoom, graphDrag.start - shift);
  });
  const endGraphDrag = (event) => {
    if (!graphDrag || graphDrag.pointerId !== event.pointerId) return;
    graphDrag = null;
    graph.style.cursor = graphZoom > 1 ? "grab" : "crosshair";
  };
  graph.addEventListener("pointerup", endGraphDrag);
  graph.addEventListener("pointercancel", endGraphDrag);
  updateOutputLevel();
  updateFormulaBusTrim();
  renderBassPatternControls();
  drawBassEditor();
  globalThis.addEventListener("resize", drawBassEditor);
  playButton.addEventListener("click", play);
  archiveFolderButton.addEventListener("click", chooseArchiveDirectory);
  document.getElementById("archive-save").addEventListener("click", saveCurrentComposition);
  document.getElementById("archive-load").addEventListener("click", loadSelectedComposition);
  document.getElementById("archive-delete").addEventListener("click", deleteSelectedComposition);
  archiveList.addEventListener("change", () => {
    const hasSelection = Boolean(archiveList.value);
    document.getElementById("archive-load").disabled = !hasSelection;
    document.getElementById("archive-delete").disabled = !hasSelection;
  });
  document.getElementById("polyrhythm-apply").addEventListener("click", () => {
    const applied = scoreModel.applyPolyrhythmPreset(polyrhythmPresetInput.value,
      Number(voiceCountInput.value), voiceCycles);
    if (!applied) return;
    voiceCountInput.value = String(applied.voiceCount);
    voiceCycles.splice(0, voiceCycles.length, ...applied.voiceCycles);
    variationInput.value = String(applied.variation * 100);
    refreshScore();
  });
  recordButton.addEventListener("click", async () => {
    if (mediaRecorder?.state === "recording") await finishRecording();
    else if (running) startRecording();
    else await play(true);
  });
  document.getElementById("stop").addEventListener("click", () => stop());
  voiceCountInput.addEventListener("change", () => {
    freezeSeedVariationForManualEdit();
    refreshScore();
  });
  globalThis.addEventListener("resize", () => draw(running ? elapsed() : 0));
  updateGraphViewControls();
  // A seed inicial varia por sessão; o botão Nova seed, em seguida, percorre
  // receitas contrastantes pelo algoritmo determinístico compartilhado.
  const initialSeedWord = new Uint32Array(1);
  if (globalThis.crypto?.getRandomValues) globalThis.crypto.getRandomValues(initialSeedWord);
  else initialSeedWord[0] = Math.floor(Math.random() * 0xFFFFFFFF);
  applySelectedSeed(initialSeedWord[0].toString(16).toUpperCase().padStart(8, "0"));
})();
