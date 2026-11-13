"""Proposta de logo do Batruc (30/10/2026) — gerada por cálculo.

Pedido do autor: refazer o desenho do logo (o lettering fica), desenho à
esquerda do lettering no cabeçalho; "deixe mais limpo, sem tiques de IA,
mas original"; o desenho se alimenta de batuque e ritmo, de conceitos
matemáticos e físicos e dos gráficos dados por algoritmos e teoremas.

O original (assets/brand/references/batruc-logo-selected-01.png) não é tocado:
tudo aqui sai em assets/brand/proposals/ e logo-proposta.html até a aprovação.

Desenho: o polígono de um ritmo — as batidas de um ciclo de 16 pulsos
ligadas em volta do círculo do tempo (G. Toussaint, The Geometry of Musical
Rhythm, 2013). O ponto vermelho é o tempo forte. Lettering: reconstrução
vetorial do lettering do original (medido na imagem: caixa 144, traço 31,
A e U ultrapassam 4 px), sem fonte de terceiros.
"""
import math, pathlib

INK = "#132a28"; RED = "#c65132"; PAPER = "#f8f6ec"
ROOT = pathlib.Path(__file__).resolve().parents[2] / "prototype"
OUT = ROOT / "assets" / "brand" / "proposals"

RHYTHMS = {  # posições de ataque num ciclo de 16
    "son": ([0, 3, 6, 10, 12], "clave son 3-2"),
    "rumba": ([0, 3, 7, 10, 12], "clave rumba 3-2"),
    "bossa": ([0, 3, 6, 10, 13], "clave bossa nova"),
}

def lettering(dx=0.0, dy=0.0):
    """O lettering BATRUC em vetor, origem na linha superior da caixa."""
    s = 31
    t = f'transform="translate({dx:.2f},{dy:.2f})"'
    return f'''<g {t} fill="{INK}">
  <path d="M15.5,68.5 H89 A26.5,26.5 0 0 0 89,15.5 H15.5 V128.5 H101.5 A30,30 0 0 0 101.5,68.5 H15.5" fill="none" stroke="{INK}" stroke-width="{s}" stroke-linejoin="miter"/>
  <polygon points="177,148 258.5,-4 340,148 305.4,148 258.5,60.5 211.6,148"/>
  <rect x="354" y="0" width="135" height="{s}"/><rect x="404" y="0" width="32" height="144"/>
  <rect x="533" y="0" width="{s}" height="144"/>
  <path d="M548.5,84 H614 A34.25,34.25 0 0 0 614,15.5 H548.5" fill="none" stroke="{INK}" stroke-width="{s}"/>
  <polygon points="579,84 619,84 667,144 627,144"/>
  <path d="M731.5,0 V86 A46.5,46.5 0 0 0 824.5,86 V0" fill="none" stroke="{INK}" stroke-width="{s}"/>
  <path d="M1046,15.5 H965 A56.5,56.5 0 0 0 965,128.5 H1046" fill="none" stroke="{INK}" stroke-width="{s}"/>
</g>'''

def mark(onsets, cx, cy, radius, stroke):
    pts = []
    for i in onsets:
        a = -math.pi / 2 + 2 * math.pi * i / 16
        pts.append((cx + radius * math.cos(a), cy + radius * math.sin(a)))
    poly = " ".join(f"{x:.2f},{y:.2f}" for x, y in pts)
    x0, y0 = pts[0]
    return (f'<polygon points="{poly}" fill="none" stroke="{INK}" stroke-width="{stroke:.2f}" stroke-linejoin="round"/>'
            f'<circle cx="{x0:.2f}" cy="{y0:.2f}" r="{stroke * 1.05:.2f}" fill="{RED}"/>')

def svg(w, h, body, x0=0, y0=0):
    return (f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="{x0} {y0} {w} {h}" width="{w}" height="{h}">'
            f'{body}</svg>\n')

def main():
    OUT.mkdir(parents=True, exist_ok=True)
    (OUT / "batruc-lettering-proposta-01.svg").write_text(svg(1046, 160, lettering(0, 6)))
    for name, (onsets, _) in RHYTHMS.items():
        # Desenho: o círculo inscrito tem a altura do lettering e um pouco mais.
        r = 76; stroke = 22
        # Margem: o ponto vermelho e as quinas não encostam na borda.
        m = mark(onsets, 104, 92, r, stroke)
        (OUT / f"batruc-desenho-{name}-proposta-01.svg").write_text(svg(208, 208, mark(onsets, 104, 104, r, stroke)))
        body = m + lettering(236, 16)
        (OUT / f"batruc-logo-horizontal-{name}-proposta-01.svg").write_text(svg(1300, 184, body))

if __name__ == "__main__":
    main()
