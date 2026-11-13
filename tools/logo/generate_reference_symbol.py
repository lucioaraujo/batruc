#!/usr/bin/env python3
"""Trace the author's selected wave-and-node composition as a clean SVG."""

from __future__ import annotations

import math
from pathlib import Path


INK = "#0C1525"
CORAL = "#E44533"
TEAL = "#0D8192"
PAPER = "#FDF9EE"
AXIS_Y = 388.0
CURVES = (
    ((69, 478), (122, 455), (179, 278), (250, 292)),
    ((250, 292), (292, 295), (322, 354), (350, 388)),
    ((350, 388), (396, 438), (443, 522), (500, 522)),
    ((500, 522), (561, 522), (617, 417), (650, 388)),
    ((650, 388), (688, 336), (718, 285), (760, 292)),
    ((760, 292), (803, 295), (836, 353), (864, 388)),
    ((864, 388), (897, 431), (924, 477), (946, 472)),
    ((946, 472), (982, 469), (1004, 407), (1015, 388)),
    ((1015, 388), (1048, 340), (1075, 304), (1105, 315)),
    ((1105, 315), (1140, 323), (1160, 370), (1188, 388)),
)

# Centers and radii follow the source image; each stem retains its own color.
NODES = (
    (107, 303, 8, CORAL), (150, 221, 18, INK), (218, 129, 20, CORAL),
    (282, 198, 13, CORAL), (346, 263, 13, INK), (400, 326, 8, CORAL),
    (279, 465, 14, CORAL), (677, 259, 12, INK), (743, 214, 11, CORAL),
    (806, 135, 11, INK), (864, 213, 13, INK), (919, 277, 15, TEAL),
    (977, 337, 12, CORAL), (977, 519, 13, CORAL), (1039, 541, 17, INK),
    (1097, 476, 12, INK), (1142, 438, 9, CORAL),
)


def cubic(segment: tuple[tuple[float, float], ...], t: float) -> tuple[float, float]:
    p0, p1, p2, p3 = segment
    u = 1 - t
    return tuple(
        u**3 * p0[i] + 3 * u**2 * t * p1[i]
        + 3 * u * t**2 * p2[i] + t**3 * p3[i]
        for i in (0, 1)
    )


def cubic_derivatives(
    segment: tuple[tuple[float, float], ...], t: float
) -> tuple[tuple[float, float], tuple[float, float]]:
    p0, p1, p2, p3 = segment
    u = 1 - t
    first = tuple(
        3 * u**2 * (p1[i] - p0[i])
        + 6 * u * t * (p2[i] - p1[i])
        + 3 * t**2 * (p3[i] - p2[i])
        for i in (0, 1)
    )
    second = tuple(
        6 * u * (p2[i] - 2 * p1[i] + p0[i])
        + 6 * t * (p3[i] - 2 * p2[i] + p1[i])
        for i in (0, 1)
    )
    return first, second


def number(value: float) -> str:
    return f"{value:.2f}".rstrip("0").rstrip(".")


def wave_ribbon() -> str:
    samples: list[tuple[float, float, float, float, float]] = []
    for segment_index, segment in enumerate(CURVES):
        for step in range(41):
            if segment_index and step == 0:
                continue
            t = step / 40
            x, y = cubic(segment, t)
            first, second = cubic_derivatives(segment, t)
            speed = max(math.hypot(*first), 1e-9)
            curvature = abs(first[0] * second[1] - first[1] * second[0]) / speed**3
            samples.append((x, y, first[0], first[1], curvature))

    maximum_curvature = max(point[4] for point in samples)
    upper: list[tuple[float, float]] = []
    lower: list[tuple[float, float]] = []
    last = len(samples) - 1
    for index, (x, y, dx, dy, curvature) in enumerate(samples):
        speed = max(math.hypot(dx, dy), 1e-9)
        nx, ny = -dy / speed, dx / speed
        curvature_weight = curvature / maximum_curvature
        edge_taper = min(1.0, 0.72 + index / 10, 0.72 + (last - index) / 10)
        half_width = (3.2 + 2.3 * curvature_weight) * edge_taper
        upper.append((x + nx * half_width, y + ny * half_width))
        lower.append((x - nx * half_width, y - ny * half_width))

    ribbon = upper + list(reversed(lower))
    commands = " L".join(f"{number(x)} {number(y)}" for x, y in ribbon)
    return f"M{commands} Z"


def svg() -> str:
    stems = "".join(
        f'<path d="M{x} {AXIS_Y}V{y}" stroke="{color}"/>'
        for x, y, _radius, color in NODES
    )
    circles = "".join(
        f'<circle cx="{x}" cy="{y}" r="{radius}" fill="{color}"/>'
        for x, y, radius, color in NODES
    )
    return f'''<svg xmlns="http://www.w3.org/2000/svg" viewBox="66 108 1125 451" role="img" aria-labelledby="title desc">
  <title id="title">Batruc — onda, eixo e pontos rítmicos</title>
  <desc id="desc">Traçado vetorial baseado na referência do autor, com hastes, círculos e cores mantidos.</desc>
  <path d="M70 {AXIS_Y}H1188" fill="none" stroke="{INK}" stroke-width="5"/>
  <g fill="none" stroke-width="5" stroke-linecap="round">{stems}</g>
  <path d="{wave_ribbon()}" fill="{INK}" stroke="none"/>
  <g>{circles}</g>
</svg>
'''


def main() -> None:
    assets = Path(__file__).resolve().parents[2] / "prototype" / "assets" / "brand" / "studies"
    output = assets / "batruc-symbol-reference-trace-v12.svg"
    output.write_text(svg(), encoding="utf-8")
    print(output)


if __name__ == "__main__":
    main()
