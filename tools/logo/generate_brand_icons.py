#!/usr/bin/env python3
"""Create publication icons by tracing the approved B from the wordmark."""

from __future__ import annotations

import subprocess
import tempfile
import xml.etree.ElementTree as ET
from pathlib import Path


INK = "#0C1525"
PAPER = "#FDF9F2"
SVG_NS = "http://www.w3.org/2000/svg"


def run(*args: str) -> None:
    subprocess.run(args, check=True)


def main() -> None:
    project = Path(__file__).resolve().parents[2]
    assets = project / "prototype/assets/brand"
    source = assets / "active/batruc-wordmark-original-crop-v01.png"
    output = assets / "icons"
    output.mkdir(parents=True, exist_ok=True)

    with tempfile.TemporaryDirectory(prefix="batruc-brand-icons-") as temp_name:
        temp = Path(temp_name)
        bitmap = temp / "letter-b.pbm"
        traced = temp / "letter-b-traced.svg"
        run(
            "convert", str(source), "-crop", "155x156+0+0", "+repage", "-trim",
            "-background", "white", "-alpha", "remove", "-alpha", "off",
            "-colorspace", "Gray", "-threshold", "50%", str(bitmap),
        )
        run(
            "potrace", "-s", "--tight", "--fillcolor", INK,
            "-o", str(traced), str(bitmap),
        )

        traced_root = ET.parse(traced).getroot()
        view_box = [float(part) for part in traced_root.attrib["viewBox"].split()]
        group = traced_root.find(f"{{{SVG_NS}}}g")
        paths = traced_root.findall(f".//{{{SVG_NS}}}path")
        if group is None or not paths:
            raise RuntimeError("Potrace did not produce the B glyph path")
        transform = group.attrib.get("transform", "")
        data = " ".join(path.attrib["d"] for path in paths)
        width, height = view_box[2:]
        side = max(width, height) * 1.24
        x = (width - side) / 2
        y = (height - side) / 2
        (output / "favicon.svg").write_text(
            f'''<svg xmlns="{SVG_NS}" viewBox="{x:.4f} {y:.4f} {side:.4f} {side:.4f}" role="img" aria-label="Batruc">
  <path transform="{transform}" d="{data}" fill="{INK}" fill-rule="evenodd"/>
</svg>
''',
            encoding="utf-8",
        )

    svg_icon = output / "favicon.svg"
    for size in (16, 32, 48):
        run(
            "convert", "-background", "none", str(svg_icon),
            "-resize", f"{size}x{size}", "-gravity", "center",
            "-extent", f"{size}x{size}", f"PNG32:{output / f'favicon-{size}.png'}",
        )
    run(
        "convert", str(output / "favicon-16.png"), str(output / "favicon-32.png"),
        str(output / "favicon-48.png"), str(output / "favicon.ico"),
    )

    for filename, size, glyph_fraction in (
        ("apple-touch-icon.png", 180, 0.76),
        ("icon-192.png", 192, 0.76),
        ("icon-512.png", 512, 0.76),
        ("icon-512-maskable.png", 512, 0.76),
    ):
        glyph_size = round(size * glyph_fraction)
        run(
            "convert", "-size", f"{size}x{size}", f"xc:{PAPER}",
            "(", "-background", "none", str(svg_icon), "-resize",
            f"{glyph_size}x{glyph_size}", ")", "-gravity", "center",
            "-compose", "over", "-composite", f"PNG32:{output / filename}",
        )
    print(f"Publication icons created in {output}")


if __name__ == "__main__":
    main()
