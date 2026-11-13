#!/usr/bin/env node
// SPDX-License-Identifier: AGPL-3.0-or-later
// Copyright (c) 2026 Lúcio de Araújo

"use strict";

const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const { spawn } = require("node:child_process");
const { pathToFileURL } = require("node:url");

const project = path.resolve(__dirname, "..");
const html = path.join(project, "prototype", "index.html");
const seed = String(process.argv[2] || "").toUpperCase();
const outputDir = path.resolve(process.argv[3] || ".local-build/parity-audio");
const options = new Set(process.argv.slice(4));
const exportStems = options.has("--stems");
const mixerSmoke = options.has("--mixer-smoke");
const archiveSmoke = options.has("--archive-smoke");
const recordingSmoke = options.has("--recording-smoke");
const recordingFolderSmoke = options.has("--recording-folder-smoke");
const liveSeedSmoke = options.has("--live-seed-smoke");
const durationSeconds = Number(process.env.BATRUC_RENDER_SECONDS || 56);
if (![28, 42, 56, 70, 84].includes(durationSeconds)) {
  console.error("BATRUC_RENDER_SECONDS deve ser 28, 42, 56, 70 ou 84.");
  process.exit(2);
}
const maxInputSeed = /^[A-F0-9]{8}$/;
if (!maxInputSeed.test(seed)) {
  console.error("Uso: node tools/render_web.cjs SEED_HEX [diretório-de-saída] [--stems] [--mixer-smoke] [--archive-smoke] [--recording-smoke] [--recording-folder-smoke] [--live-seed-smoke]");
  process.exit(2);
}
if (options.size > 3 || [...options].some((option) =>
  option !== "--stems" && option !== "--mixer-smoke" && option !== "--archive-smoke" &&
  option !== "--recording-smoke" && option !== "--recording-folder-smoke" &&
  option !== "--live-seed-smoke") ||
  [mixerSmoke, archiveSmoke, recordingSmoke, recordingFolderSmoke, liveSeedSmoke].filter(Boolean).length > 1) {
  console.error("Opções de smoke são exclusivas: --mixer-smoke, --archive-smoke, --recording-smoke, --recording-folder-smoke, --live-seed-smoke.");
  process.exit(2);
}

function delay(ms) { return new Promise((resolve) => setTimeout(resolve, ms)); }

function openCdp(browser) {
  const commandPipe = browser.stdio[3];
  const responsePipe = browser.stdio[4];
  if (!commandPipe || !responsePipe)
    throw new Error("Chrome não forneceu os pipes DevTools herdados.");

  const pending = new Map();
  let nextId = 0;
  let buffered = "";
  const failPending = (error) => {
    for (const item of pending.values()) {
      clearTimeout(item.timer);
      item.rejectCommand(error);
    }
    pending.clear();
  };
  responsePipe.setEncoding("utf8");
  responsePipe.on("data", (chunk) => {
    buffered += chunk;
    if (buffered.length > 8 * 1024 * 1024) {
      const error = new Error("Resposta DevTools excedeu o limite local de 8 MiB.");
      failPending(error);
      buffered = "";
      return;
    }
    let boundary;
    while ((boundary = buffered.indexOf("\0")) >= 0) {
      const frame = buffered.slice(0, boundary);
      buffered = buffered.slice(boundary + 1);
      let message;
      try { message = JSON.parse(frame); } catch { continue; }
      const item = pending.get(message.id);
      if (!item) continue;
      clearTimeout(item.timer);
      pending.delete(message.id);
      if (message.error) item.rejectCommand(new Error(message.error.message));
      else item.resolveCommand(message.result || {});
    }
  });
  commandPipe.on("error", failPending);
  responsePipe.on("error", failPending);

  return {
    send(method, params = {}, sessionId) {
      const id = ++nextId;
      const command = { id, method, params };
      if (sessionId) command.sessionId = sessionId;
      return new Promise((resolveCommand, rejectCommand) => {
        const timer = setTimeout(() => {
          pending.delete(id);
          rejectCommand(new Error(`Timeout no comando DevTools ${method}`));
        }, 15000);
        pending.set(id, { resolveCommand, rejectCommand, timer });
        commandPipe.write(`${JSON.stringify(command)}\0`, (error) => {
          if (!error) return;
          clearTimeout(timer);
          pending.delete(id);
          rejectCommand(error);
        });
      });
    },
    close() {
      commandPipe.on("error", () => {});
      responsePipe.on("error", () => {});
      commandPipe.end();
      responsePipe.destroy();
      for (const item of pending.values()) {
        clearTimeout(item.timer);
        item.rejectCommand(new Error("Canal DevTools encerrado."));
      }
      pending.clear();
    }
  };
}

async function evaluate(send, expression) {
  const response = await send("Runtime.evaluate", {
    expression, awaitPromise: true, returnByValue: true, userGesture: true
  });
  if (response.exceptionDetails) {
    const details = response.exceptionDetails;
    const reason = details.exception?.description || details.exception?.value || details.text;
    throw new Error(reason || "Erro ao executar automação na página.");
  }
  return response.result?.value;
}

async function encodePcm16Wav(send, rendered) {
  const chunks = [];
  const framesPerChunk = 48000;
  for (let start = 0; start < rendered.frames; start += framesPerChunk) {
    const count = Math.min(framesPerChunk, rendered.frames - start);
    const encoded = await evaluate(send,
      `window.__batrucOfflineChunkBase64(${start}, ${count})`);
    const pcm = Buffer.from(encoded, "base64");
    if (pcm.length !== count * rendered.channels * 2)
      throw new Error(`Bloco PCM inválido no quadro ${start}.`);
    chunks.push(pcm);
  }
  const dataBytes = chunks.reduce((sum, chunk) => sum + chunk.length, 0);
  const header = Buffer.alloc(44);
  header.write("RIFF", 0); header.writeUInt32LE(36 + dataBytes, 4);
  header.write("WAVE", 8); header.write("fmt ", 12);
  header.writeUInt32LE(16, 16); header.writeUInt16LE(1, 20);
  header.writeUInt16LE(rendered.channels, 22);
  header.writeUInt32LE(rendered.sample_rate, 24);
  header.writeUInt32LE(rendered.sample_rate * rendered.channels * 2, 28);
  header.writeUInt16LE(rendered.channels * 2, 32);
  header.writeUInt16LE(16, 34); header.write("data", 36);
  header.writeUInt32LE(dataBytes, 40);
  return Buffer.concat([header, ...chunks]);
}

async function main() {
  if (!fs.existsSync(html)) throw new Error(`Página não encontrada: ${html}`);
  fs.mkdirSync(outputDir, { recursive: true });
  const base = `batruc-${seed}-web-offline`;
  const finalWav = path.join(outputDir, `${base}.wav`);
  const finalScore = path.join(outputDir, `${base}-partitura.txt`);
  const finalReport = path.join(outputDir, `${base}.json`);
  const stemPaths = ["voices", "bass"].map((stem) =>
    path.join(outputDir, `${base}-${stem}.wav`));
  if ([finalWav, finalScore, finalReport, ...(exportStems ? stemPaths : [])].some(fs.existsSync))
    throw new Error(`Já existe saída para ${seed}; escolha outro diretório ou arquive a saída anterior.`);

  const temporaryProfile = fs.mkdtempSync(path.join(os.tmpdir(), "batruc-web-render-"));
  const chrome = process.env.BATRUC_CHROME || "google-chrome";
  let browser;
  let cdp;
  try {
    browser = spawn(chrome, [
      "--headless=new", "--disable-gpu", "--disable-dev-shm-usage",
      "--no-first-run", "--no-default-browser-check",
      "--enable-automation",
      "--disable-breakpad", "--disable-crash-reporter",
      "--autoplay-policy=no-user-gesture-required",
      "--remote-debugging-pipe",
      `--user-data-dir=${temporaryProfile}`, pathToFileURL(html).href
    ], { stdio: ["ignore", "ignore", "pipe", "pipe", "pipe"],
      detached: process.platform !== "win32" });
    browser.once("error", (error) => { console.error(`Chrome: ${error.message}`); });
    browser.stderr.setEncoding("utf8");
    browser.stderr.on("data", (chunk) => process.stderr.write(`Chrome: ${chunk}`));
    cdp = openCdp(browser);
    const runRecordingFolderSmoke = async () => {
      const setupFolder = await evaluate(pageSend, `(() => {
        const files = new Map();
        const fileHandle = (name) => ({ kind: 'file',
          async getFile() { return files.get(name) || new Blob([]); },
          async createWritable() { return {
            async write(blob) { files.set(name, blob); },
            async close() {}, async abort() {}
          }; }
        });
        const directory = {
          name: 'Batruc gravação local simulada', kind: 'directory',
          async queryPermission() { return 'granted'; },
          async requestPermission() { return 'granted'; },
          async getFileHandle(name, options = {}) {
            if (!files.has(name) && options.create) files.set(name, new Blob([]));
            if (!files.has(name)) throw new DOMException('Arquivo ausente', 'NotFoundError');
            return fileHandle(name);
          },
          async *entries() {
            for (const name of files.keys()) yield [name, fileHandle(name)];
          },
          async removeEntry(name) {
            if (!files.delete(name)) throw new DOMException('Arquivo ausente', 'NotFoundError');
          }
        };
        globalThis.showDirectoryPicker = async () => directory;
        globalThis.__batrucRecordingFolderFiles = files;
        document.querySelector('#archive-folder').click();
        return true;
      })()`);
      if (!setupFolder) throw new Error('Não foi possível preparar a pasta simulada.');
      const folderDeadline = Date.now() + 3000;
      let folderReady = false;
      while (Date.now() < folderDeadline) {
        folderReady = await evaluate(pageSend,
          "!document.querySelector('#archive-save').disabled");
        if (folderReady) break;
        await delay(20);
      }
      if (!folderReady) throw new Error('A página não selecionou a pasta simulada.');
      const point = await evaluate(pageSend, `(() => {
        const urls = [];
        const blobs = [];
        const createObjectURL = URL.createObjectURL.bind(URL);
        URL.createObjectURL = (blob) => {
          urls.push({ size: blob.size, type: blob.type }); blobs.push(blob);
          return createObjectURL(blob);
        };
        const button = document.querySelector('#record');
        button.scrollIntoView({ block: 'center' });
        window.__batrucFolderRecording = { urls, blobs };
        const rect = button.getBoundingClientRect();
        return { x: rect.x + rect.width / 2, y: rect.y + rect.height / 2 };
      })()`);
      const clickRecord = async () => {
        await pageSend('Input.dispatchMouseEvent', { type: 'mousePressed',
          x: point.x, y: point.y, button: 'left', clickCount: 1 });
        await pageSend('Input.dispatchMouseEvent', { type: 'mouseReleased',
          x: point.x, y: point.y, button: 'left', clickCount: 1 });
      };
      const waitFor = async (expression, label, timeoutMs = 10000) => {
        const deadline = Date.now() + timeoutMs;
        while (Date.now() < deadline) {
          if (await evaluate(pageSend, expression)) return;
          await delay(25);
        }
        throw new Error(`Timeout ${label}: ${await evaluate(pageSend,
          `document.querySelector('#record-status').textContent`)}`);
      };
      await clickRecord();
      await waitFor("document.querySelector('#record').getAttribute('aria-pressed') === 'true'",
        'início da gravação na pasta simulada');
      await delay(4000);
      await clickRecord();
      await waitFor("document.querySelector('#record').getAttribute('aria-pressed') === 'false' && document.querySelector('#record-status').textContent.includes('salvos em')",
        'fechamento do áudio e sidecar na pasta');
      const result = await evaluate(pageSend, `(async () => {
        const files = globalThis.__batrucRecordingFolderFiles;
        const items = [...files.entries()];
        const audioItem = items.find(([, blob]) => blob.type.startsWith('audio/'));
        const scoreItem = items.find(([name]) => name.endsWith('-partitura.txt'));
        const audioBytes = audioItem?.[1] || null;
        const scoreText = scoreItem?.[1] || null;
        const score = scoreText ? JSON.parse(await scoreText.text()) : null;
        let decoded = null;
        if (audioBytes && (window.OfflineAudioContext || window.webkitOfflineAudioContext)) {
          const OfflineContext = window.OfflineAudioContext || window.webkitOfflineAudioContext;
          const decoder = new OfflineContext(2, 1, 48000);
          const buffer = await decoder.decodeAudioData(await audioBytes.arrayBuffer());
          let peak = 0; let sumSquares = 0; let samples = 0;
          for (let channel = 0; channel < buffer.numberOfChannels; channel += 1) {
            const data = buffer.getChannelData(channel);
            for (let index = 0; index < data.length; index += 1) {
              peak = Math.max(peak, Math.abs(data[index]));
              sumSquares += data[index] * data[index]; samples += 1;
            }
          }
          decoded = { duration_seconds: buffer.duration, peak,
            rms: Math.sqrt(sumSquares / Math.max(1, samples)) };
        }
        const checks = {
          exactlyTwoFiles: items.length === 2,
          audioWritten: Boolean(audioBytes && audioBytes.size > 1024),
          sidecarWritten: Boolean(scoreText && scoreText.size > 0),
          linkedNames: Boolean(score && score.audio_file?.filename === audioItem?.[0]),
          versionedScore: score?.format === 'batruc-recording-score' && score?.schema_version === 1,
          audioHasSignal: Boolean(decoded && decoded.peak > 1e-5)
        };
        return { files: items.map(([name, blob]) => ({ name, bytes: blob.size, type: blob.type })),
          decoded_audio: decoded, checks, passed: Object.values(checks).every(Boolean) };
      })()`);
      if (!result?.passed)
        throw new Error(`Teste de gravação na pasta falhou: ${JSON.stringify(result)}`);
      console.log(JSON.stringify(result, null, 2));
      return;
    };
    if (recordingSmoke) {
      await cdp.send("Browser.setDownloadBehavior", {
        behavior: "allow", downloadPath: outputDir, eventsEnabled: true
      });
    }
    const targetDeadline = Date.now() + 15000;
    let target;
    while (Date.now() < targetDeadline) {
      const response = await cdp.send("Target.getTargets");
      target = response.targetInfos?.find((item) => item.type === "page");
      if (target) break;
      await delay(100);
    }
    if (!target) throw new Error("Chrome não abriu uma aba para o Batruc.");
    const attached = await cdp.send("Target.attachToTarget", {
      targetId: target.targetId, flatten: true
    });
    const sessionId = attached.sessionId;
    const pageSend = (method, params = {}) => cdp.send(method, params, sessionId);
    await pageSend("Page.enable");
    await pageSend("Runtime.enable");
    await pageSend("Page.navigate", { url: pathToFileURL(html).href });
    const readyDeadline = Date.now() + 15000;
    let ready = false;
    while (Date.now() < readyDeadline) {
      ready = await evaluate(pageSend,
        "document.readyState === 'complete' && !!document.querySelector('#record') && !!document.querySelector('#seed-value')");
      if (ready) break;
      await delay(100);
    }
    if (!ready) throw new Error("Interface do Batruc não terminou de carregar.");

    const setup = await evaluate(pageSend, `(() => {
      const seed = document.querySelector('#seed-value');
      const setter = Object.getOwnPropertyDescriptor(HTMLInputElement.prototype, 'value').set;
      setter.call(seed, '${seed}');
      seed.dispatchEvent(new Event('input', { bubbles: true }));
      const master = document.querySelector('#output-level');
      master.step = '0.001';
      master.value = '0.72';
      master.dispatchEvent(new Event('input', { bubbles: true }));
      const formulaTrim = document.querySelector('#formula-bus-trim');
      formulaTrim.value = '16.5';
      formulaTrim.dispatchEvent(new Event('input', { bubbles: true }));
      const duration = document.querySelector('#duration');
      duration.value = '${durationSeconds}';
      duration.dispatchEvent(new Event('change', { bubbles: true }));
      const playable = Boolean(window.OfflineAudioContext || window.webkitOfflineAudioContext) &&
        typeof window.__batrucRenderOffline === 'function';
      if (!playable) return { playable, message: 'OfflineAudioContext do Batruc indisponível.' };
      return { playable, seed: seed.value, seconds: Number(duration.value),
        master: Number(master.value), formulaTrim: Number(formulaTrim.value) };
    })()`);
    if (!setup?.playable) throw new Error(setup?.message || "Captura Web Audio indisponível.");
    if (liveSeedSmoke) {
      const initialized = await evaluate(pageSend, `(() => {
        window.__batrucScheduleTrace = [];
        window.__batrucCursorTrace = [];
        window.__batrucCompositionTrace = [];
        const score = window.BatrucScore;
        if (!score || typeof score.eventCursorAtOrAfter !== 'function')
          throw new Error('Helper do cursor de eventos indisponível.');
        const findCursor = score.eventCursorAtOrAfter;
        score.eventCursorAtOrAfter = (events, cutoff) => {
          window.__batrucCursorTrace.push(cutoff);
          return findCursor(events, cutoff);
        };
        const generate = score.generate;
        score.generate = (input) => {
          const result = generate(input);
          if (window.__batrucCompositionTrace.length < 64)
          window.__batrucCompositionTrace.push({ seed: result.config.seed,
            phase: result.config.phase, voiceCount: result.config.voiceCount,
              voiceCycles: result.config.voiceCycles.slice(0, 2),
              variation: result.config.variation, eventCount: result.events.length,
              bassPlan: result.events.filter(event => event.voice === 12).map(event =>
                [event.cycle, Number(event.time.toFixed(6)), event.pitch, event.sourceVoice ?? null]) });
          return result;
        };
        const button = document.querySelector('#play');
        button.scrollIntoView({ block: 'center' });
        const rect = button.getBoundingClientRect();
        return { x: rect.x + rect.width / 2, y: rect.y + rect.height / 2 };
      })()`);
      const clickAt = async (point) => {
        await pageSend('Input.dispatchMouseEvent', { type: 'mousePressed',
          x: point.x, y: point.y, button: 'left', clickCount: 1 });
        await pageSend('Input.dispatchMouseEvent', { type: 'mouseReleased',
          x: point.x, y: point.y, button: 'left', clickCount: 1 });
      };
      const waitFor = async (expression, label, timeoutMs = 5000) => {
        const deadline = Date.now() + timeoutMs;
        while (Date.now() < deadline) {
          if (await evaluate(pageSend, expression)) return;
          await delay(20);
        }
        throw new Error(`Timeout ${label}: ${await evaluate(pageSend,
          `document.querySelector('#status').textContent`)}`);
      };
      await clickAt(initialized);
      await waitFor("document.querySelector('#status').textContent.startsWith('Tocando.')",
        'início do transporte com áudio');
      await delay(350);
      const changedSeed = seed === '5EED2030' ? 'A1B2C3D4' : '5EED2030';
      const revision = await evaluate(pageSend, `(() => {
        const input = document.querySelector('#seed-value');
        const setter = Object.getOwnPropertyDescriptor(HTMLInputElement.prototype, 'value').set;
        setter.call(input, '${changedSeed}');
        input.dispatchEvent(new Event('input', { bubbles: true }));
        return { seed: input.value, status: document.querySelector('#status').textContent };
      })()`);
      await waitFor(`window.__batrucScheduleTrace.some(event => event.seed === '${changedSeed}')`,
        'primeiros ataques da nova seed', 5000);
      await delay(200);
      const seedRevision = await evaluate(pageSend, `Math.max(...window.__batrucScheduleTrace
        .filter(event => event.seed === '${changedSeed}').map(event => event.revision))`);
      const phaseChange = await evaluate(pageSend, `(() => {
        const input = document.querySelector('#phase');
        const setter = Object.getOwnPropertyDescriptor(HTMLInputElement.prototype, 'value').set;
        setter.call(input, '0.25');
        input.dispatchEvent(new Event('input', { bubbles: true }));
        return { phase: Number(input.value) };
      })()`);
      await waitFor("window.__batrucScheduleTrace.some(event => Math.abs(event.phase - 0.25) < 1e-9)",
        'ataque após mudança de fase', 5000);
      const phaseRevision = await evaluate(pageSend, `Math.max(...window.__batrucScheduleTrace
        .filter(event => Math.abs(event.phase - 0.25) < 1e-9).map(event => event.revision))`);
      const beforePolyrhythm = await evaluate(pageSend,
        "window.__batrucCompositionTrace.at(-1)");
      const polyrhythm = await evaluate(pageSend, `(() => {
        const select = document.querySelector('#polyrhythm-preset');
        select.value = '5:4';
        document.querySelector('#polyrhythm-apply').click();
        const generated = window.__batrucCompositionTrace.at(-1);
        return { voiceCount: document.querySelector('#voice-count').value,
          generatedVoiceCount: generated?.voiceCount,
          voiceCycles: generated?.voiceCycles, variation: generated?.variation,
          eventCount: generated?.eventCount, bassPlanChanged: generated &&
            JSON.stringify(generated.bassPlan) !== ${JSON.stringify(JSON.stringify(beforePolyrhythm.bassPlan))} };
      })()`);
      await waitFor(`window.__batrucScheduleTrace.some(event => event.revision > ${phaseRevision})`,
        'ataque após aplicar a polirritmia', 5000);
      await delay(150);
      const result = await evaluate(pageSend, `(() => {
        const trace = window.__batrucScheduleTrace;
        const cutoffs = window.__batrucCursorTrace;
        const cutoff = cutoffs.at(-1);
        const seeded = trace.filter(event => event.seed === '${changedSeed}');
        const phased = trace.filter(event => Math.abs(event.phase - ${phaseChange.phase}) < 1e-9);
        const phaseCutoff = cutoffs[1];
        const polyRevision = Math.max(...trace.map(event => event.revision));
        const revised = trace.filter(event => event.revision === polyRevision);
        const keys = revised.map(event => String(event.voice) + ':' + event.time.toFixed(9));
        const checks = {
          seedInputApplied: document.querySelector('#seed-value').value === '${changedSeed}',
          seedRevisionScheduled: seeded.length > 0 && ${seedRevision} > 0,
          phaseChangedDuringPlayback: Number(document.querySelector('#phase').value) === ${phaseChange.phase} &&
            phased.length > 0 && ${phaseRevision} > ${seedRevision} && Number.isFinite(phaseCutoff),
          phaseRevisionHonoursCommittedWindow: phased.every(event => event.time + 1e-9 >= phaseCutoff),
          polyrhythmPresetApplied: document.querySelector('#polyrhythm-preset').value === '5:4' &&
            document.querySelector('#voice-count').value === '${polyrhythm.voiceCount}' &&
            ${polyrhythm.voiceCycles?.[0]} === 5 && ${polyrhythm.voiceCycles?.[1]} === 4 &&
            ${polyrhythm.variation} === 0 &&
            ${polyrhythm.voiceCount} === ${polyrhythm.generatedVoiceCount},
          polyRevisionScheduled: revised.length > 0,
          polyRevisionCarries5to4Cycles: ${polyrhythm.voiceCycles?.[0]} === 5 &&
            ${polyrhythm.voiceCycles?.[1]} === 4,
          bassPlanRecalculatedFromRhythm: ${polyrhythm.bassPlanChanged},
          cursorRebasedDuringPlayback: Number.isFinite(cutoff) && cutoff > 0,
          noNewRevisionEventsInsideCommittedWindow: revised.every(event => event.time + 1e-9 >= cutoff),
          noDuplicateEventsInRevision: new Set(keys).size === keys.length,
          transportStillRunning: document.querySelector('#play').textContent.includes('Reiniciar')
        };
        return { initial_seed: '${seed}', changed_seed: '${changedSeed}',
          polyrhythm: '5:4', polyrhythm_voice_count: '${polyrhythm.voiceCount}',
          generated_event_count: ${polyrhythm.eventCount},
          bass_plan_changed: ${polyrhythm.bassPlanChanged},
          seed_revision: ${seedRevision}, phase_revision: ${phaseRevision},
          phase_cutoff_seconds: phaseCutoff, polyrhythm_revision: polyRevision,
          cutoff_seconds: cutoff, scheduled_event_count: trace.length,
          polyrhythm_revision_events: revised.length, checks,
          passed: Object.values(checks).every(Boolean) };
      })()`);
      await evaluate(pageSend, `document.querySelector('#stop').click()`);
      if (!result?.passed)
        throw new Error(`Alteração generativa durante reprodução falhou: ${JSON.stringify(result)}`);
      console.log(JSON.stringify(result, null, 2));
      return;
    }
    if (recordingFolderSmoke) {
      await runRecordingFolderSmoke();
      return;
    }
    if (mixerSmoke) {
      const result = await evaluate(pageSend, `(async () => {
        const measure = async () => {
          const rendered = await window.__batrucRenderOffline(48000);
          const buffer = window.__batrucOfflineBuffer;
          if (!buffer || rendered.sample_rate !== 48000) throw new Error('Render do mixer indisponível.');
          let sum = 0;
          let samples = 0;
          for (let channel = 0; channel < buffer.numberOfChannels; channel += 1) {
            const data = buffer.getChannelData(channel);
            for (let index = 0; index < data.length; index += 1) {
              sum += data[index] * data[index];
              samples += 1;
            }
          }
          return Math.sqrt(sum / Math.max(1, samples));
        };
        const click = (id) => {
          const button = document.getElementById(id);
          if (!button) throw new Error('Controle de mixer ausente: ' + id);
          button.click();
        };
        const baseline = await measure();
        click('channel-mute-0');
        const voiceMuted = await measure();
        click('channel-mute-0');
        click('channel-solo-0');
        const voiceSolo = await measure();
        click('channel-solo-0');
        click('channel-solo-12');
        const bassSolo = await measure();
        click('channel-solo-12');
        click('master-mute');
        const masterMuted = await measure();
        const checks = {
          baselineAudible: baseline > 1e-5,
          voiceMuteChangesMix: voiceMuted > 1e-5 && voiceMuted < baseline * 0.98,
          voiceSoloExcludesOtherChannels: voiceSolo > 1e-5 && voiceSolo < baseline * 0.98,
          bassSoloAudible: bassSolo > 1e-5 && bassSolo < baseline * 0.98,
          masterMuteSilencesMix: masterMuted < 1e-8
        };
        return { seed: '${seed}', duration_seconds: Number(document.querySelector('#duration').value),
          rms: { baseline, voiceMuted, voiceSolo, bassSolo, masterMuted }, checks,
          passed: Object.values(checks).every(Boolean) };
      })()`);
      if (!result?.passed)
        throw new Error(`Teste funcional do mixer falhou: ${JSON.stringify(result)}`);
      console.log(JSON.stringify(result, null, 2));
      return;
    }
    if (archiveSmoke) {
      const result = await evaluate(pageSend, `(async () => {
        const files = new Map();
        const fileHandle = (name) => ({
          kind: 'file',
          async getFile() { return files.get(name) || new Blob([]); },
          async createWritable() {
            return {
              async write(blob) { files.set(name, blob); },
              async close() {}, async abort() {}
            };
          }
        });
        const directory = {
          name: 'Batruc smoke local', kind: 'directory',
          async queryPermission() { return 'granted'; },
          async requestPermission() { return 'granted'; },
          async getFileHandle(name, options = {}) {
            if (!files.has(name) && options.create) files.set(name, new Blob([]));
            if (!files.has(name)) throw new DOMException('Arquivo ausente', 'NotFoundError');
            return fileHandle(name);
          },
          async *entries() {
            for (const name of files.keys()) yield [name, fileHandle(name)];
          },
          async removeEntry(name) {
            if (!files.delete(name)) throw new DOMException('Arquivo ausente', 'NotFoundError');
          }
        };
        globalThis.showDirectoryPicker = async () => directory;
        globalThis.confirm = () => true;
        const waitFor = async (predicate, label) => {
          const deadline = performance.now() + 3000;
          while (performance.now() < deadline) {
            if (predicate()) return;
            await new Promise((resolve) => setTimeout(resolve, 10));
          }
          throw new Error('Timeout: ' + label);
        };
        document.querySelector('#archive-folder').click();
        await waitFor(() => !document.querySelector('#archive-save').disabled,
          'seleção de pasta');
        document.querySelector('#archive-name').value = 'Smoke composição';
        document.querySelector('#archive-save').click();
        await waitFor(() => files.size === 1 && !document.querySelector('#archive-load').disabled,
          'guardar/listar composição');
        const [filename] = files.keys();
        const listedCompositionCount = document.querySelector('#archive-list').options.length;
        const saved = JSON.parse(await files.get(filename).text());
        const seed = document.querySelector('#seed-value');
        const setter = Object.getOwnPropertyDescriptor(HTMLInputElement.prototype, 'value').set;
        setter.call(seed, 'DEADBEEF');
        seed.dispatchEvent(new Event('input', { bubbles: true }));
        await new Promise((resolve) => setTimeout(resolve, 20));
        document.querySelector('#archive-list').value = filename;
        document.querySelector('#archive-load').click();
        await waitFor(() => document.querySelector('#archive-status').textContent.includes('reaberta'),
          'abrir composição');
        const restoredSeed = seed.value;
        document.querySelector('#archive-delete').click();
        await waitFor(() => files.size === 0 && document.querySelector('#archive-delete').disabled,
          'apagar composição');
        const checks = {
          savedFormat: saved.format === 'batruc-composition' && saved.schema_version === 1,
          savedSeedAndName: saved.config.seed === '${seed}' && saved.name === 'Smoke composição',
          archiveListed: listedCompositionCount === 1,
          restoredChangedSeed: restoredSeed === '${seed}' && restoredSeed !== 'DEADBEEF',
          deletionRemovedEntry: files.size === 0
        };
        return { saved_filename: filename, saved_format: saved.format,
          restored_seed: restoredSeed, checks,
          passed: Object.values(checks).every(Boolean) };
      })()`);
      if (!result?.passed)
        throw new Error(`Teste funcional do arquivo local falhou: ${JSON.stringify(result)}`);
      console.log(JSON.stringify(result, null, 2));
      return;
    }
    if (recordingSmoke) {
      const buttonPoint = await evaluate(pageSend, `(() => {
        if (typeof MediaRecorder !== 'function')
          throw new Error('MediaRecorder indisponível neste Chromium.');
        const createdUrls = [];
        const createdBlobs = [];
        const createObjectURL = URL.createObjectURL.bind(URL);
        URL.createObjectURL = (blob) => {
          createdUrls.push({ size: blob.size, type: blob.type });
          createdBlobs.push(blob);
          return createObjectURL(blob);
        };
        const button = document.querySelector('#record');
        button.scrollIntoView({ block: 'center' });
        window.__batrucRecordingSmoke = { createdUrls, createdBlobs };
        const rect = button.getBoundingClientRect();
        return { x: rect.x + rect.width / 2, y: rect.y + rect.height / 2 };
      })()`);
      const clickRecord = async () => {
        await pageSend('Input.dispatchMouseEvent', { type: 'mousePressed',
          x: buttonPoint.x, y: buttonPoint.y, button: 'left', clickCount: 1 });
        await pageSend('Input.dispatchMouseEvent', { type: 'mouseReleased',
          x: buttonPoint.x, y: buttonPoint.y, button: 'left', clickCount: 1 });
      };
      const waitFor = async (expression, label, timeoutMs = 10000) => {
        const deadline = Date.now() + timeoutMs;
        while (Date.now() < deadline) {
          if (await evaluate(pageSend, expression)) return;
          await delay(25);
        }
        const current = await evaluate(pageSend,
          `document.querySelector('#record-status').textContent`);
        throw new Error(`Timeout ${label}: ${current}`);
      };
      await clickRecord();
      await waitFor("document.querySelector('#record').getAttribute('aria-pressed') === 'true'",
        'início da captura');
      await delay(4000);
      await clickRecord();
      await waitFor("document.querySelector('#record').getAttribute('aria-pressed') === 'false' && document.querySelector('#record-status').textContent.includes('Master')",
        'finalização e arquivos');
      const result = await evaluate(pageSend, `(async () => {
        const createdUrls = window.__batrucRecordingSmoke.createdUrls;
        const createdBlobs = window.__batrucRecordingSmoke.createdBlobs;
        const status = document.querySelector('#record-status').textContent;
        const audio = createdUrls.find((item) => item.size > 0 && item.type.startsWith('audio/'));
        const score = createdUrls.find((item) => item.type.startsWith('text/plain'));
        const audioBlob = createdBlobs.find((blob) => blob.type.startsWith('audio/'));
        let decodedAudio = null;
        if (audioBlob && (window.OfflineAudioContext || window.webkitOfflineAudioContext)) {
          const OfflineContext = window.OfflineAudioContext || window.webkitOfflineAudioContext;
          const decoder = new OfflineContext(2, 1, 48000);
          const buffer = await decoder.decodeAudioData(await audioBlob.arrayBuffer());
          let peak = 0;
          let sumSquares = 0;
          let samples = 0;
          for (let channel = 0; channel < buffer.numberOfChannels; channel += 1) {
            const data = buffer.getChannelData(channel);
            for (let index = 0; index < data.length; index += 1) {
              const value = data[index];
              peak = Math.max(peak, Math.abs(value));
              sumSquares += value * value;
              samples += 1;
            }
          }
          decodedAudio = { duration_seconds: buffer.duration, peak,
            rms: Math.sqrt(sumSquares / Math.max(1, samples)) };
        }
        const checks = {
          recorderStopped: document.querySelector('#record').getAttribute('aria-pressed') === 'false',
          audioBlobCreated: Boolean(audio && audio.size > 1024),
          audioContainsSignal: Boolean(decodedAudio && decodedAudio.peak > 1e-5),
          scoreTextCreated: Boolean(score && score.size > 0),
          statusReportsDownload: status.includes('download iniciado')
        };
        return { status, audio_blob: audio || null, decoded_audio: decodedAudio,
          score_blob: score || null, checks,
          passed: Object.values(checks).every(Boolean) };
      })()`);
      if (!result?.passed)
        throw new Error(`Teste funcional da gravação falhou: ${JSON.stringify(result)}`);
      await evaluate(pageSend, `(() => {
        const link = document.querySelector('#score-download');
        if (link.hidden || !link.href) throw new Error('Link TXT da gravação não ficou disponível.');
        link.click();
        return true;
      })()`);
      const downloadDeadline = Date.now() + 10000;
      let downloadedAudio;
      let downloadedScore;
      while (Date.now() < downloadDeadline) {
        const files = fs.readdirSync(outputDir);
        downloadedAudio = files.find((file) => /^batruc-master-.*\.(webm|ogg)$/.test(file));
        downloadedScore = files.find((file) => /-partitura\.txt$/.test(file));
        if (downloadedAudio && downloadedScore) break;
        await delay(50);
      }
      if (!downloadedAudio || !downloadedScore)
        throw new Error(`Browser não materializou os dois downloads em ${outputDir}.`);
      const downloadedAudioBytes = fs.statSync(path.join(outputDir, downloadedAudio)).size;
      const downloadedScoreData = JSON.parse(fs.readFileSync(
        path.join(outputDir, downloadedScore), "utf8"));
      result.downloaded_files = {
        audio: downloadedAudio, audio_bytes: downloadedAudioBytes,
        score: downloadedScore, score_schema: downloadedScoreData.schema_version
      };
      result.checks.downloadedAudioMatchesBlob = downloadedAudioBytes === result.audio_blob.size;
      result.checks.downloadedScoreIsRecordingSchema =
        downloadedScoreData.format === "batruc-recording-score" &&
        downloadedScoreData.schema_version === 1 &&
        downloadedScoreData.audio_file?.filename === downloadedAudio;
      result.passed = Object.values(result.checks).every(Boolean);
      if (!result.passed)
        throw new Error(`Arquivos baixados não correspondem aos blobs: ${JSON.stringify(result)}`);
      console.log(JSON.stringify(result, null, 2));
      return;
    }
    const rendered = await evaluate(pageSend, "window.__batrucRenderOffline(48000)");
    if (!rendered || rendered.sample_rate !== 48000 || rendered.channels !== 2 ||
        !Number.isInteger(rendered.frames) || rendered.frames <= 0 || rendered.frames > 48000 * 84)
      throw new Error(`Formato do render offline inválido: ${JSON.stringify(rendered)}`);

    const audioBytes = await encodePcm16Wav(pageSend, rendered);
    const configuration = rendered.configuration;
    const scoreData = {
      format: "batruc-recording-score", schema_version: 1,
      created_at: new Date().toISOString(),
      audio_file: { filename: path.basename(finalWav), mime_type: "audio/wav", encoding: "PCM16" },
      recording: { captured_from_seconds: 0, captured_duration_seconds: rendered.duration_seconds,
        configuration_revision_limit: 512, configuration_revisions_truncated: false },
      initial_configuration: configuration,
      initial_mathematical_partitura: rendered.mathematical_partitura,
      initial_event_plan: rendered.event_plan,
      configuration_revisions: [{ effective_at_seconds: 0, configuration }],
      notes: ["Render HTML feito pelo OfflineAudioContext; exportação PCM16 local para comparação técnica."]
    };
    const scoreBytes = Buffer.from(`${JSON.stringify(scoreData, null, 2)}\n`, "utf8");
    fs.writeFileSync(finalWav, audioBytes, { flag: "wx" });
    fs.writeFileSync(finalScore, scoreBytes, { flag: "wx" });
    const report = {
      format: "batruc-web-offline-render-v1", engine: "HTML Web Audio / OfflineAudioContext",
      seed, configured_duration_seconds: setup.seconds,
      rendered_duration_seconds: rendered.duration_seconds,
      event_count: rendered.event_plan.length,
      audio_file: path.basename(finalWav), score_file: path.basename(finalScore),
      audio_bytes: audioBytes.length, codec: "PCM16",
      configuration,
      capture_mode: "local headless Chromium; Web Audio OfflineAudioContext render",
      parity_note: "Render HTML PCM offline; usa o grafo de síntese da página sem o relógio de áudio do dispositivo."
    };
    fs.writeFileSync(finalReport, `${JSON.stringify(report, null, 2)}\n`, "utf8");
    if (exportStems) {
      for (const stem of ["voices", "bass"]) {
        const isolated = await evaluate(pageSend,
          `window.__batrucRenderOffline(48000, ${JSON.stringify(stem)})`);
        if (!isolated || isolated.sample_rate !== 48000 || isolated.channels !== 2 ||
            isolated.frames !== rendered.frames)
          throw new Error(`Formato de stem ${stem} inválido.`);
        const stemWav = await encodePcm16Wav(pageSend, isolated);
        fs.writeFileSync(stemPaths[stem === "voices" ? 0 : 1], stemWav, { flag: "wx" });
      }
    }
    console.log(JSON.stringify(report, null, 2));
  } finally {
    if (browser && browser.exitCode === null) {
      try { await Promise.race([cdp?.send("Browser.close"), delay(1000)]); }
      catch { /* Browser may already have exited or closed its DevTools pipe. */ }
    }
    if (browser) {
      const exited = () => browser.exitCode !== null || browser.signalCode !== null;
      const waitForExit = (milliseconds) => Promise.race([
        exited() ? Promise.resolve() : new Promise((resolve) => browser.once("exit", resolve)),
        delay(milliseconds)
      ]);
      const signalTree = (signal) => {
        try {
          if (process.platform !== "win32" && browser.pid)
            process.kill(-browser.pid, signal);
          else if (!exited()) browser.kill(signal);
        } catch {
          if (!exited()) browser.kill(signal);
        }
      };
      signalTree("SIGTERM");
      await waitForExit(1500);
      signalTree("SIGKILL");
      await waitForExit(1000);
    }
    cdp?.close();
    try {
      fs.rmSync(temporaryProfile, {
        recursive: true, force: true, maxRetries: 5, retryDelay: 200
      });
    } catch (error) {
      console.warn(`Perfil temporário do Chromium preservado em ${temporaryProfile}: ${error.message}`);
    }
  }
}

main().catch((error) => {
  console.error(`Render web Batruc falhou: ${error.message}`);
  process.exitCode = 1;
});
