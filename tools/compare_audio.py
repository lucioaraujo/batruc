#!/usr/bin/env python3
# SPDX-License-Identifier: AGPL-3.0-or-later
# Copyright (c) 2026 Lúcio de Araújo

"""Compare two local Batruc renderings using bounded, reproducible metrics."""

from __future__ import annotations

import argparse
import array
import json
import math
import os
import subprocess
import tempfile
import wave
from pathlib import Path


MAX_INPUT_BYTES = 512 * 1024 * 1024
MAX_SECONDS = 15 * 60
MAX_RATE = 192_000


def decode_if_needed(path: Path, temp_dir: str) -> Path:
    try:
        with wave.open(str(path), "rb") as source:
            source.getparams()
        return path
    except (wave.Error, EOFError):
        decoded = Path(temp_dir) / (path.stem + ".decoded.wav")
        subprocess.run(
            ["ffmpeg", "-nostdin", "-v", "error", "-y", "-i", str(path),
             "-t", str(MAX_SECONDS), "-ac", "2", "-ar", "48000", "-c:a", "pcm_s16le",
             str(decoded)],
            check=True, timeout=90, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE,
        )
        return decoded


def read_audio(path: Path, temp_dir: str, window_ms: float) -> dict:
    if path.stat().st_size > MAX_INPUT_BYTES:
        raise ValueError(f"Arquivo acima do limite de {MAX_INPUT_BYTES // (1024 * 1024)} MiB: {path}")
    decoded = decode_if_needed(path, temp_dir)
    with wave.open(str(decoded), "rb") as source:
        channels, width, rate, frames = source.getnchannels(), source.getsampwidth(), source.getframerate(), source.getnframes()
        if channels not in (1, 2) or width not in (2, 3, 4) or rate < 8_000 or rate > MAX_RATE:
            raise ValueError(f"Formato PCM não suportado ou fora dos limites: {path}")
        if frames / rate > MAX_SECONDS:
            raise ValueError(f"Áudio excede o limite de {MAX_SECONDS} segundos: {path}")
        data = source.readframes(frames)

    if width == 2:
        samples = array.array("h")
        samples.frombytes(data)
        if os.sys.byteorder != "little":
            samples.byteswap()
        scale = 32768.0
        values = [sample / scale for sample in samples]
    elif width == 3:
        values = []
        for index in range(0, len(data), 3):
            raw = data[index] | (data[index + 1] << 8) | (data[index + 2] << 16)
            if raw & 0x800000:
                raw -= 1 << 24
            values.append(raw / 8388608.0)
    else:
        samples = array.array("i")
        samples.frombytes(data)
        if os.sys.byteorder != "little":
            samples.byteswap()
        values = [sample / 2147483648.0 for sample in samples]

    peak = max((abs(value) for value in values), default=0.0)
    square_sum = sum(value * value for value in values)
    clipped = sum(abs(value) >= 0.999 for value in values)
    frame_count = len(values) // channels
    window_frames = max(1, round(rate * window_ms / 1000.0))
    envelope = []
    for start in range(0, frame_count, window_frames):
        end = min(frame_count, start + window_frames)
        energy = 0.0
        for frame in range(start, end):
            offset = frame * channels
            mono = sum(values[offset:offset + channels]) / channels
            energy += mono * mono
        envelope.append(math.sqrt(energy / max(1, end - start)))
    max_envelope = max(envelope, default=0.0)
    threshold = max(1e-5, max_envelope * 0.001)
    onset = next((i for i, level in enumerate(envelope) if level >= threshold), len(envelope))
    quiet_threshold = max(1e-5, peak * 0.001)
    silent_frames = sum(1 for frame in range(frame_count)
                        if max(abs(values[frame * channels + ch]) for ch in range(channels)) < quiet_threshold)
    return {
        "path": str(path), "sample_rate": rate, "channels": channels,
        "duration_seconds": frame_count / rate,
        "rms": math.sqrt(square_sum / max(1, len(values))), "peak": peak,
        "clipped_sample_fraction": clipped / max(1, len(values)),
        "silence_fraction": silent_frames / max(1, frame_count),
        "onset_seconds": onset * window_frames / rate,
        "envelope": envelope,
    }


def correlation(left: list[float], right: list[float]) -> float | None:
    if len(left) < 2 or len(right) != len(left):
        return None
    mean_left = sum(left) / len(left)
    mean_right = sum(right) / len(right)
    covariance = sum((a - mean_left) * (b - mean_right) for a, b in zip(left, right))
    variance_left = sum((a - mean_left) ** 2 for a in left)
    variance_right = sum((b - mean_right) ** 2 for b in right)
    denominator = math.sqrt(variance_left * variance_right)
    return covariance / denominator if denominator > 1e-20 else None


def recipe_summary(path: Path, side: str) -> dict:
    document = json.loads(path.read_text(encoding="utf-8"))
    def choice(values: list[str], key: str) -> str | None:
        index = document.get(key)
        return values[index] if isinstance(index, int) and 0 <= index < len(values) else None

    if side == "html":
        config = document.get("configuration") or {}
        mixer = config.get("mixer") or {}
        voice_count = config.get("voiceCount")
        mixer_volumes = mixer.get("volumes") or []
        mixer_pans = mixer.get("pans") or []
        base_phases = config.get("voiceBasePhases") or []
        phase_offsets = config.get("voicePhaseOffsets") or []
        effective_phases = [
            0.0 if voice == 0 else (voice - 1) / 16
            + (base_phases[voice] if voice < len(base_phases) else 0.0)
            + (phase_offsets[voice] if voice < len(phase_offsets) else 0.0)
            for voice in range(voice_count or 0)
        ]
        transform_ids = {name: index for index, name in enumerate((
            "sine", "invert", "rectify", "harmonic", "amplitude",
            "quantized", "cubic", "thirdHarmonic", "triangle", "softClip"))}
        bass_variation = max(config.get("variation", 0.0),
                             config.get("bassPitchVariationBase", 0.0))
        return {
            "seed": document.get("seed", config.get("seed")),
            "duration_seconds": config.get("durationSeconds"),
            "bpm": config.get("bpm"), "meter": config.get("meter"),
            "voice_count": voice_count, "phase": config.get("phase"),
            "pattern_mode": config.get("patternMode"),
            "event_rule": config.get("recipe"),
            "intensity_law": config.get("intensityLaw"),
            "intensity_amount": config.get("intensityAmount"),
            "variation": config.get("variation"),
            "swing_ratio": config.get("swing"), "groove": config.get("groove"),
            "bass_relation": config.get("bassRhythmRelation"),
            "bass_duration_law": config.get("bassDurationFormula"),
            "bass_pitch_variation": bass_variation,
            "bass_oscillator_profile": config.get("bassOscillatorProfile"),
            "voice_sound": config.get("timbre"),
            "voice_cycles": config.get("voiceCycles"),
            "voice_transform_indices": [transform_ids.get(item, item)
                                         for item in config.get("voiceTransforms", [])],
            "voice_couplings": config.get("couplings"),
            "voice_phase_offsets": effective_phases,
            "mixer_master_gain": mixer.get("masterVolume"),
            "mixer_formula_bus_gain_db": mixer.get("formulaBusGainDb"),
            "mixer_bass_gain": config.get("bassOutputLevel"),
            "mixer_bass_pan": mixer_pans[12] if len(mixer_pans) > 12 else None,
            "mixer_voice_channel_volumes": mixer_volumes[:(voice_count or 0)],
            "mixer_voice_channel_pans": mixer_pans[:(voice_count or 0)],
        }
    mixer = document.get("mixer") or {}
    return {
        "seed": document.get("seed"),
        "duration_seconds": document.get("section_seconds", 0)
            * document.get("formal_cycles", 0),
        "bpm": document.get("bpm"), "meter": document.get("meter"),
        "voice_count": document.get("voice_count"),
        "phase": document.get("seeded_phase"),
        "pattern_mode": choice(["unrestricted", "adhesive-pads", "diagonal-lanes"],
                                "seeded_pattern_index"),
        "event_rule": choice(["center", "turn"], "seeded_event_rule_index"),
        "intensity_law": choice(["uniform", "projection-y", "product-xy",
                                  "quadratic-difference"], "seeded_intensity_law_index"),
        "intensity_amount": document.get("intensity_amount"),
        "variation": document.get("voice_variation"),
        "swing_ratio": document.get("swing_ratio"), "groove": document.get("groove"),
        "bass_relation": document.get("bass_relation"),
        "bass_duration_law": document.get("bass_duration_law"),
        "bass_pitch_variation": document.get("bass_variation"),
        "bass_oscillator_profile": choice(["harmonic", "odd", "inharmonic", "beat"],
                                           "bass_oscillator_profile_index"),
        "voice_sound": document.get("voice_sound"),
        "voice_cycles": document.get("voice_cycles"),
        "voice_transform_indices": document.get("voice_transform_indices"),
        "voice_couplings": document.get("voice_couplings"),
        "voice_phase_offsets": document.get("voice_phase_offsets"),
        "mixer_master_gain": mixer.get("master_gain"),
        "mixer_formula_bus_gain_db": mixer.get("formula_bus_gain_db"),
        "mixer_bass_gain": mixer.get("bass_gain"),
        "mixer_bass_pan": mixer.get("bass_pan"),
        "mixer_voice_channel_volumes": mixer.get("voice_channel_volumes"),
        "mixer_voice_channel_pans": mixer.get("voice_channel_pans"),
    }


def compare_recipes(html: dict, juce: dict) -> dict:
    fields = sorted(set(html) | set(juce))
    differences = []
    for field in fields:
        left, right = html.get(field), juce.get(field)
        if isinstance(left, list) and isinstance(right, list):
            equal = len(left) == len(right) and all(
                (abs((float(a) - float(b) + 0.5) % 1.0 - 0.5) <= 1e-5
                 if field == "voice_phase_offsets" else abs(float(a) - float(b)) <= 1e-5)
                if isinstance(a, (int, float)) and isinstance(b, (int, float))
                else a == b for a, b in zip(left, right)
            )
        elif isinstance(left, (int, float)) and isinstance(right, (int, float)):
            equal = abs(float(left) - float(right)) <= 1e-5
        else:
            equal = left == right
        if not equal:
            differences.append({"field": field, "html": left, "juce": right})
    return {"matches": not differences, "compared_fields": len(fields),
            "differences": differences}


def event_schedule(path: Path, side: str) -> list[dict]:
    manifest = json.loads(path.read_text(encoding="utf-8"))
    if side == "html":
        score_path = path.parent / manifest.get("score_file", "")
        score = json.loads(score_path.read_text(encoding="utf-8"))
        events = score.get("initial_event_plan", [])
    else:
        events = manifest.get("event_schedule", [])
    normalized = []
    for event in events:
        voice = event.get("voice")
        kind = event.get("type") or ("bass" if voice == "bass" else "voice")
        cycle = int(event.get("cycle", 0))
        if side == "html":
            # The score format numbers formal cycles from one; the native
            # renderer's event schedule stores them from zero.
            cycle -= 1
        normalized.append({
            "type": kind, "voice": str(voice),
            "cycle": cycle,
            "time_seconds": float(event.get("time_seconds", 0.0)),
            "pitch_midi": event.get("pitch_midi"),
            "duration_seconds": event.get("duration_seconds"),
            "source_voice": event.get("source_voice"),
            "source_x": event.get("source_x"),
            "source_y": event.get("source_y"),
            "bass_candidate_x": event.get("bass_source_x", event.get("bass_candidate_x")),
            "bass_candidate_y": event.get("bass_source_y", event.get("bass_candidate_y")),
            "bass_candidate_intensity": event.get(
                "bass_source_intensity", event.get("bass_candidate_intensity")),
            "intensity": (float(event["intensity"])
                          if isinstance(event.get("intensity"), (int, float)) else None),
        })
    return sorted(normalized, key=lambda item: (
        item["type"], item["voice"], item["cycle"], item["time_seconds"]))


def compare_event_schedules(html: list[dict], juce: list[dict]) -> dict:
    buckets: dict[tuple[str, str, int], tuple[list[dict], list[dict]]] = {}
    for side, events in ((0, html), (1, juce)):
        for event in events:
            key = (event["type"], event["voice"], event["cycle"])
            if key not in buckets:
                buckets[key] = ([], [])
            buckets[key][side].append(event)
    details = []
    count_differences = []
    compared = 0
    timing_mismatches = pitch_mismatches = duration_mismatches = intensity_mismatches = 0
    source_mismatches = 0
    candidate_mismatches = candidate_intensity_mismatches = 0
    max_time_delta = max_duration_delta = max_intensity_delta = 0.0
    max_source_delta = 0.0
    max_candidate_delta = max_candidate_intensity_delta = 0.0
    compared_intensity_count = 0
    for key in sorted(buckets):
        left, right = buckets[key]
        if len(left) != len(right):
            count_differences.append({"type": key[0], "voice": key[1], "cycle": key[2],
                                      "html": len(left), "juce": len(right)})
        for html_event, juce_event in zip(left, right):
            compared += 1
            dt = abs(html_event["time_seconds"] - juce_event["time_seconds"])
            max_time_delta = max(max_time_delta, dt)
            if dt > 0.005:
                timing_mismatches += 1
                if len(details) < 20:
                    details.append({"type": key[0], "voice": key[1], "cycle": key[2],
                                    "html_time": html_event["time_seconds"],
                                    "juce_time": juce_event["time_seconds"],
                                    "difference_seconds": dt})
            if html_event["pitch_midi"] != juce_event["pitch_midi"]:
                pitch_mismatches += 1
            if html_event["source_voice"] is not None and juce_event["source_voice"] is not None:
                source_delta = max(
                    abs(float(html_event["source_x"]) - float(juce_event["source_x"])),
                    abs(float(html_event["source_y"]) - float(juce_event["source_y"])),
                )
                max_source_delta = max(max_source_delta, source_delta)
                if html_event["source_voice"] != juce_event["source_voice"] or source_delta > 1e-4:
                    source_mismatches += 1
            if html_event["bass_candidate_x"] is not None and juce_event["bass_candidate_x"] is not None:
                candidate_delta = max(
                    abs(float(html_event["bass_candidate_x"]) - float(juce_event["bass_candidate_x"])),
                    abs(float(html_event["bass_candidate_y"]) - float(juce_event["bass_candidate_y"])),
                )
                max_candidate_delta = max(max_candidate_delta, candidate_delta)
                if candidate_delta > 1e-4:
                    candidate_mismatches += 1
            if (html_event["bass_candidate_intensity"] is not None
                    and juce_event["bass_candidate_intensity"] is not None):
                candidate_intensity_delta = abs(
                    float(html_event["bass_candidate_intensity"])
                    - float(juce_event["bass_candidate_intensity"]))
                max_candidate_intensity_delta = max(
                    max_candidate_intensity_delta, candidate_intensity_delta)
                if candidate_intensity_delta > 0.01:
                    candidate_intensity_mismatches += 1
            hd, jd = html_event["duration_seconds"], juce_event["duration_seconds"]
            if hd is not None and jd is not None:
                dd = abs(float(hd) - float(jd))
                max_duration_delta = max(max_duration_delta, dd)
                if dd > 0.005:
                    duration_mismatches += 1
            if html_event["intensity"] is not None and juce_event["intensity"] is not None:
                compared_intensity_count += 1
                di = abs(html_event["intensity"] - juce_event["intensity"])
                max_intensity_delta = max(max_intensity_delta, di)
                if di > 0.01:
                    intensity_mismatches += 1
    return {
        "matches": not count_differences and not timing_mismatches
            and not pitch_mismatches and not duration_mismatches
            and not intensity_mismatches and not source_mismatches
            and not candidate_mismatches and not candidate_intensity_mismatches,
        "html_event_count": len(html), "juce_event_count": len(juce),
        "paired_event_count": compared, "event_count_differences": count_differences,
        "timing_mismatches_over_5ms": timing_mismatches,
        "pitch_mismatches": pitch_mismatches,
        "bass_source_mismatches": source_mismatches,
        "bass_candidate_coordinate_mismatches": candidate_mismatches,
        "bass_candidate_intensity_mismatches_over_0_01": candidate_intensity_mismatches,
        "duration_mismatches_over_5ms": duration_mismatches,
        "intensity_pairs_compared": compared_intensity_count,
        "intensity_mismatches_over_0_01": intensity_mismatches,
        "max_time_delta_seconds": max_time_delta,
        "max_duration_delta_seconds": max_duration_delta,
        "max_intensity_delta": max_intensity_delta,
        "max_bass_source_delta": max_source_delta,
        "max_bass_candidate_delta": max_candidate_delta,
        "max_bass_candidate_intensity_delta": max_candidate_intensity_delta,
        "first_timing_differences": details,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("html_audio", type=Path)
    parser.add_argument("juce_audio", type=Path)
    parser.add_argument("--json", type=Path, dest="json_path")
    parser.add_argument("--window-ms", type=float, default=10.0)
    parser.add_argument("--html-manifest", type=Path)
    parser.add_argument("--juce-manifest", type=Path)
    args = parser.parse_args()
    if args.window_ms <= 0 or args.window_ms > 100:
        parser.error("--window-ms precisa estar entre 0 e 100")
    for path in (args.html_audio, args.juce_audio):
        if not path.is_file():
            parser.error(f"arquivo não encontrado: {path}")

    with tempfile.TemporaryDirectory(prefix="batruc-audio-compare-") as temporary:
        html = read_audio(args.html_audio, temporary, args.window_ms)
        juce = read_audio(args.juce_audio, temporary, args.window_ms)

    html_env, juce_env = html.pop("envelope"), juce.pop("envelope")
    html_onset = round(html["onset_seconds"] * 1000 / args.window_ms)
    juce_onset = round(juce["onset_seconds"] * 1000 / args.window_ms)
    html_aligned = html_env[html_onset:]
    juce_aligned = juce_env[juce_onset:]
    common = min(len(html_aligned), len(juce_aligned))
    envelope_correlation = correlation(html_aligned[:common], juce_aligned[:common])
    report = {
        "format": "batruc-audio-comparison-v1", "window_ms": args.window_ms,
        "html": html, "juce": juce,
        "delta": {
            "duration_seconds": juce["duration_seconds"] - html["duration_seconds"],
            "onset_seconds": juce["onset_seconds"] - html["onset_seconds"],
            "rms": juce["rms"] - html["rms"],
            "peak": juce["peak"] - html["peak"],
        },
        "aligned_envelope_windows": common,
        "envelope_correlation_after_onset_alignment": envelope_correlation,
        "limitations": ["Métricas não substituem escuta musical.",
                        "A correlação usa envelopes RMS, não comparação de fase de amostras."],
    }
    if args.html_manifest or args.juce_manifest:
        if not args.html_manifest or not args.juce_manifest:
            parser.error("--html-manifest e --juce-manifest devem ser usados juntos")
        report["recipe_comparison"] = compare_recipes(
            recipe_summary(args.html_manifest, "html"),
            recipe_summary(args.juce_manifest, "juce"))
        report["event_schedule_comparison"] = compare_event_schedules(
            event_schedule(args.html_manifest, "html"),
            event_schedule(args.juce_manifest, "juce"))
    rendered = json.dumps(report, ensure_ascii=False, indent=2, allow_nan=False)
    if args.json_path:
        args.json_path.parent.mkdir(parents=True, exist_ok=True)
        args.json_path.write_text(rendered + "\n", encoding="utf-8")
    print(rendered)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
