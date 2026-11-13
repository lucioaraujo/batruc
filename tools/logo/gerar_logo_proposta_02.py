"""Proposta 02 do logo do Batruc (30/10/2026) — gerada por cálculo.

Pedidos do autor depois da proposta 01 ("não gostei do desenho"): algo que
traga percussão, ritmo, matemática, física, os cálculos, os gráficos, as
curvas notáveis de Vygodsky, o batuque; "o arco do berimbau pode ser um
caminho"; "gosto dos símbolos matemáticos também"; limpo, sem tiques de IA,
original. O lettering continua o do original, em vetor (proposta 01).

A · BERIMBAU DE ELÁSTICA — o arco é a elástica de Euler (a forma de uma vara
    elástica curvada pela corda: θ'' + λ·sen θ = 0, integrada aqui), o arame
    é a corda, a cabaça vermelha é o ressonador, e o dobrão encosta a 1/9 do
    arame: encurtar a corda em 1/9 sobe a nota um tom (9:8), as duas notas
    do berimbau.
B · A MESMA, com a onda estacionária do 3º modo desenhada no arame.
C · BERIMBAU-INTEGRAL — o arco é o sinal ∫ (a integral, soma contínua — o
    som é a soma das vibrações da corda); os terminais do ∫ são a ponta da
    corda (em cima) e a cabaça (embaixo, vermelha); o arame liga os dois.

O original (assets/brand/references/batruc-logo-selected-01.png) e a proposta
01 ficam intactos. Saídas: assets/brand/proposals/*-proposta-02.svg e
logo-proposta-02.html.
"""
import math, pathlib, importlib.util

HERE = pathlib.Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location("p01", HERE / "gerar_logo_proposta.py")
p01 = importlib.util.module_from_spec(spec); spec.loader.exec_module(p01)
INK, RED, PAPER, OUT = p01.INK, p01.RED, p01.PAPER, p01.OUT

def path(pts):
    return "M" + " L".join(f"{x:.2f},{y:.2f}" for x, y in pts)

def elastica(alpha, ds=0.002):
    """Elástica de Euler com as pontas sem momento (curvatura zero), de θ=α
    a θ=−α: θ'' = −sen θ (λ = 1; a escala vem depois)."""
    th, w, x, y = alpha, 0.0, 0.0, 0.0
    pts = [(0.0, 0.0)]
    while len(pts) < 200000:
        prev = w
        w += -math.sin(th) * ds
        th += w * ds
        x += math.cos(th) * ds
        y += math.sin(th) * ds
        pts.append((x, y))
        if prev < 0 <= w:
            break
    return pts

def onto(pts, p0, p1, bulge_left=True):
    (ax, ay), (bx, by) = pts[0], pts[-1]
    a0 = math.atan2(by - ay, bx - ax); a1 = math.atan2(p1[1] - p0[1], p1[0] - p0[0])
    s = math.hypot(p1[0] - p0[0], p1[1] - p0[1]) / math.hypot(bx - ax, by - ay)
    c, n = math.cos(a1 - a0), math.sin(a1 - a0)
    out = [(p0[0] + ((x - ax) * c - (y - ay) * n) * s, p0[1] + ((x - ax) * n + (y - ay) * c) * s) for x, y in pts]
    mid_x = sum(p[0] for p in out) / len(out)
    if (mid_x > (p0[0] + p1[0]) / 2) == bulge_left:
        ux, uy = (p1[0] - p0[0]), (p1[1] - p0[1]); L = math.hypot(ux, uy); ux, uy = ux / L, uy / L
        def mirror(p):
            px, py = p[0] - p0[0], p[1] - p0[1]; d = px * ux + py * uy
            return (p0[0] + 2 * d * ux - px, p0[1] + 2 * d * uy - py)
        out = [mirror(p) for p in out]
    return out

def berimbau(x0, y0, h, wave=False, alpha=62):
    top = (x0 + h * 0.34, y0); bot = (x0 + h * 0.44, y0 + h)
    bow = onto(elastica(math.radians(alpha)), top, bot)
    along = lambda f: (top[0] + (bot[0] - top[0]) * f, top[1] + (bot[1] - top[1]) * f)
    body = f'<path d="{path(bow)}" fill="none" stroke="{INK}" stroke-width="{h * 0.1:.2f}" stroke-linecap="round"/>'
    if wave:
        L = math.hypot(bot[0] - top[0], bot[1] - top[1])
        nx, ny = -(bot[1] - top[1]) / L, (bot[0] - top[0]) / L
        pts = []
        for i in range(301):
            f = i / 300; amp = math.sin(3 * math.pi * f) * h * 0.035
            px, py = along(f); pts.append((px + nx * amp, py + ny * amp))
        body += f'<path d="{path(pts)}" fill="none" stroke="{INK}" stroke-width="{h * 0.018:.2f}"/>'
    else:
        body += (f'<line x1="{top[0]:.2f}" y1="{top[1]:.2f}" x2="{bot[0]:.2f}" y2="{bot[1]:.2f}" '
                 f'stroke="{INK}" stroke-width="{h * 0.022:.2f}" stroke-linecap="round"/>')
    gx, gy = along(0.80); cx, cy = along(1 / 9)
    body += (f'<circle cx="{gx:.2f}" cy="{gy:.2f}" r="{h * 0.105:.2f}" fill="{RED}"/>'
             f'<circle cx="{cx:.2f}" cy="{cy:.2f}" r="{h * 0.034:.2f}" fill="{INK}"/>')
    half = h * 0.05
    xs = [p[0] for p in bow] + [gx - h * 0.105, gx + h * 0.105]
    ys = [p[1] for p in bow] + [gy - h * 0.105, gy + h * 0.105]
    return body, (min(xs) - half, min(ys) - half, max(xs) + half, max(ys) + half)

def integral(x0, y0, h):
    """O sinal ∫ como berimbau: haste levemente inclinada, curvas nas pontas
    e terminais em bola (a de baixo, vermelha, é a cabaça)."""
    w = h * 0.46; sw = h * 0.085
    ball = sw * 1.2
    top_end = (x0 + w * 0.92, y0 + h * 0.13); bot_end = (x0 + w * 0.08, y0 + h * 0.87)
    d = (f"M{top_end[0]:.2f},{top_end[1]:.2f} "
         f"C{x0 + w * 0.86:.2f},{y0 + h * 0.01:.2f} {x0 + w * 0.60:.2f},{y0 - h * 0.01:.2f} {x0 + w * 0.56:.2f},{y0 + h * 0.20:.2f} "
         f"L{x0 + w * 0.44:.2f},{y0 + h * 0.80:.2f} "
         f"C{x0 + w * 0.40:.2f},{y0 + h * 1.01:.2f} {x0 + w * 0.14:.2f},{y0 + h * 0.99:.2f} {bot_end[0]:.2f},{bot_end[1]:.2f}")
    body = f'<path d="{d}" fill="none" stroke="{INK}" stroke-width="{sw:.2f}" stroke-linecap="round"/>'
    body += (f'<line x1="{top_end[0]:.2f}" y1="{top_end[1]:.2f}" x2="{bot_end[0]:.2f}" y2="{bot_end[1]:.2f}" '
             f'stroke="{INK}" stroke-width="{h * 0.02:.2f}" stroke-linecap="round"/>')
    body += (f'<circle cx="{top_end[0]:.2f}" cy="{top_end[1]:.2f}" r="{sw * 0.95:.2f}" fill="{INK}"/>'
             f'<circle cx="{bot_end[0]:.2f}" cy="{bot_end[1]:.2f}" r="{ball:.2f}" fill="{RED}"/>')
    return body, (bot_end[0] - ball, y0 - sw, x0 + w + sw, y0 + h + sw)

def main():
    marks = {"a-berimbau": lambda x, y, h: berimbau(x, y, h),
             "b-berimbau-onda": lambda x, y, h: berimbau(x, y, h, wave=True),
             "c-berimbau-integral": integral}
    margin = 16
    for name, draw in marks.items():
        body, (x0, y0, x1, y1) = draw(0, 0, 200)
        w, h = x1 - x0, y1 - y0
        alone = f'<g transform="translate({margin - x0:.2f},{margin - y0:.2f})">{body}</g>'
        (OUT / f"batruc-desenho-{name}-proposta-02.svg").write_text(p01.svg(round(w + 2 * margin), round(h + 2 * margin), alone))
        # Lockup: o desenho à esquerda, centrado na altura do lettering
        # (caixa 144); espaço de meia caixa até o B.
        total_h = max(h, 152) + 2 * margin
        mark_y = margin + (total_h - 2 * margin - h) / 2 - y0
        letter_x = margin + w + 72
        letter_y = (total_h - 144) / 2
        lockup = (f'<g transform="translate({margin - x0:.2f},{mark_y:.2f})">{body}</g>'
                  + p01.lettering(letter_x, letter_y))
        (OUT / f"batruc-logo-horizontal-{name}-proposta-02.svg").write_text(
            p01.svg(round(letter_x + 1046 + margin), round(total_h), lockup))

if __name__ == "__main__":
    main()
