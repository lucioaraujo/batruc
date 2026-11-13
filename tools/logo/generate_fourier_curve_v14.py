#!/usr/bin/env python3
"""Generate a single, axis-free Fourier curve for the Batruc wordmark."""

from __future__ import annotations

import math
from pathlib import Path


WIDTH = 420
HEIGHT = 102
INK = "#0C1525"
SAMPLES = 240


def curve_path() -> str:
    points = []
    for index in range(SAMPLES + 1):
        t = 2 * math.pi * index / SAMPLES
        # Two related harmonics create an asymmetric, two-cycle contour.
        value = 0.72 * math.sin(2 * t) + 0.22 * math.sin(3 * t + 0.55)
        x = 18 + 384 * index / SAMPLES
        y = HEIGHT / 2 - 34 * value
        points.append(f"{x:.2f},{y:.2f}")
    return "M" + " L".join(points)


def main() -> None:
    project = Path(__file__).resolve().parents[2]
    destination = project / "prototype/assets/brand/studies/batruc-symbol-fourier-curve-v14.svg"
    destination.write_text(
        f'''<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {WIDTH} {HEIGHT}" role="img" aria-labelledby="title desc">
  <title id="title">Batruc — curva de Fourier</title>
  <desc id="desc">Curva formada pela soma de dois harmônicos, sem eixos ou marcadores.</desc>
  <path d="{curve_path()}" fill="none" stroke="{INK}" stroke-width="5.2" stroke-linecap="round" stroke-linejoin="round"/>
</svg>
''',
        encoding="utf-8",
    )
    print(destination)


if __name__ == "__main__":
    main()
