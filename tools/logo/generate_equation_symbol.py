#!/usr/bin/env python3
"""Generate the Batruc function-and-sampling logo symbol from a Fourier curve."""

from __future__ import annotations

import argparse
from math import cos, hypot, pi, sin
from pathlib import Path


WIDTH = 280
HEIGHT = 190
LEFT = 20
RIGHT = 260
AXIS_Y = 95
AMPLITUDE_SCALE = 52
PRESETS = {
    "v08": {
        "terms": ((0.66, 1, 0.0), (0.24, 2, pi / 3), (0.10, 3, -pi / 5)),
        "sample_count": 5,
        "view_box": (0, 0, WIDTH, HEIGHT),
        "palette": ("#132a28", "#c65132", "#167d77", "#f8f6ec"),
    },
    "v09": {
        "terms": ((0.16, 1, -pi / 4), (0.52, 2, pi / 12), (0.11, 3, -pi / 6)),
        "sample_count": 8,
        "view_box": (0, 0, WIDTH, HEIGHT),
        "palette": ("#132a28", "#c65132", "#167d77", "#f8f6ec"),
    },
    "v10": {
        "terms": ((0.16, 1, -pi / 4), (0.52, 2, pi / 12), (0.11, 3, -pi / 6)),
        "sample_count": 8,
        "view_box": (18, 0, 244, HEIGHT),
        "palette": ("#132a28", "#c65132", "#167d77", "#f8f6ec"),
    },
    "v11": {
        "terms": ((0.16, 1, -pi / 4), (0.52, 2, pi / 12), (0.11, 3, -pi / 6)),
        "sample_count": 8,
        "view_box": (18, 55, 244, 84),
        "palette": ("#0c1525", "#e44533", "#0d8192", "#fdf9ee"),
    },
}


def function(t: float, terms: tuple[tuple[float, int, float], ...]) -> float:
    return sum(amplitude * sin(2 * pi * harmonic * t + phase)
               for amplitude, harmonic, phase in terms)


def function_derivative(
    t: float, terms: tuple[tuple[float, int, float], ...]
) -> float:
    return sum(
        amplitude * 2 * pi * harmonic * cos(2 * pi * harmonic * t + phase)
        for amplitude, harmonic, phase in terms
    )


def function_second_derivative(
    t: float, terms: tuple[tuple[float, int, float], ...]
) -> float:
    return sum(
        -amplitude * (2 * pi * harmonic) ** 2
        * sin(2 * pi * harmonic * t + phase)
        for amplitude, harmonic, phase in terms
    )


def point(t: float, terms: tuple[tuple[float, int, float], ...]) -> tuple[float, float]:
    return LEFT + (RIGHT - LEFT) * t, AXIS_Y - AMPLITUDE_SCALE * function(t, terms)


def graph_curvature(
    t: float, terms: tuple[tuple[float, int, float], ...]
) -> float:
    span = RIGHT - LEFT
    slope = -AMPLITUDE_SCALE * function_derivative(t, terms) / span
    second = -AMPLITUDE_SCALE * function_second_derivative(t, terms) / (span * span)
    return abs(second) / (1 + slope * slope) ** 1.5


def fmt(value: float) -> str:
    return f"{value:.2f}".rstrip("0").rstrip(".")


def svg_document(
    terms: tuple[tuple[float, int, float], ...],
    sample_count: int,
    stem_extension: float,
    node_outline_width: float,
    view_box: tuple[int, int, int, int],
    palette: tuple[str, str, str, str],
) -> str:
    ink, coral, teal, paper = palette
    samples = [(index + 0.5) / sample_count for index in range(sample_count)]
    sample_parameters = [index / 240 for index in range(241)]
    points = [point(t, terms) for t in sample_parameters]
    curvatures = [graph_curvature(t, terms) for t in sample_parameters]
    maximum_curvature = max(curvatures)
    positive_edge: list[tuple[float, float]] = []
    negative_edge: list[tuple[float, float]] = []
    last_index = len(points) - 1
    for index, (x, y) in enumerate(points):
        previous = points[max(0, index - 1)]
        following = points[min(last_index, index + 1)]
        dx = following[0] - previous[0]
        dy = following[1] - previous[1]
        tangent_length = max(hypot(dx, dy), 1e-9)
        normal_x, normal_y = -dy / tangent_length, dx / tangent_length
        curvature_weight = curvatures[index] / maximum_curvature
        edge_weight = min(1.0, 0.55 + index / 12, 0.55 + (last_index - index) / 12)
        half_width = (0.9 + 2.0 * curvature_weight) * edge_weight
        positive_edge.append((x + normal_x * half_width, y + normal_y * half_width))
        negative_edge.append((x - normal_x * half_width, y - normal_y * half_width))
    outline_points = positive_edge + list(reversed(negative_edge))
    wave_path = (
        "M" + " L".join(f"{fmt(x)} {fmt(y)}" for x, y in outline_points) + " Z"
    )

    stems: list[str] = []
    nodes: list[str] = []
    for t in samples:
        x, y = point(t, terms)
        tip_y = y + (y - AXIS_Y) * stem_extension
        color = coral if y < AXIS_Y else teal
        stems.append(
            f'<path d="M{fmt(x)} {AXIS_Y}V{fmt(tip_y)}" stroke="{color}"/>'
        )
        nodes.append(
            f'<circle cx="{fmt(x)}" cy="{fmt(tip_y)}" r="5.7" fill="{color}"/>'
        )

    outline = (
        f' stroke="{paper}" stroke-width="{fmt(node_outline_width)}"'
        if node_outline_width > 0
        else ""
    )

    view_box_text = " ".join(str(value) for value in view_box)
    return f'''<svg xmlns="http://www.w3.org/2000/svg" viewBox="{view_box_text}" role="img" aria-labelledby="title desc">
  <title id="title">Batruc — série de Fourier e amostragem</title>
  <desc id="desc">Uma série de Fourier de três harmônicos, medida em {sample_count} instantes sobre um eixo temporal.</desc>
  <path d="M18 {AXIS_Y}H262" fill="none" stroke="{ink}" stroke-width="2" stroke-linecap="round"/>
  <g fill="none" stroke-width="2.2" stroke-linecap="round">{' '.join(stems)}</g>
  <path d="{wave_path}" fill="{ink}" stroke="none"/>
  <g{outline}>{' '.join(nodes)}</g>
</svg>
'''


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--version", choices=PRESETS, default="v11")
    parser.add_argument("--output", type=Path)
    parser.add_argument("--stem-extension", type=float, default=0.42)
    parser.add_argument("--node-outline-width", type=float, default=0.0)
    args = parser.parse_args()
    preset = PRESETS[args.version]
    output = args.output or (
        Path(__file__).resolve().parents[2]
        / "prototype"
        / "assets"
        / "brand"
        / "studies"
        / f"batruc-symbol-fourier-sampling-{args.version}.svg"
    )
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(
        svg_document(
            preset["terms"], preset["sample_count"],
            args.stem_extension, args.node_outline_width,
            preset["view_box"], preset["palette"],
        ),
        encoding="utf-8",
    )
    print(output)


if __name__ == "__main__":
    main()
